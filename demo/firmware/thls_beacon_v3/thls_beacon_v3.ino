/**
 * TREASURE HUNTERS IoT: THE LAST SIGNAL
 * ESP32 — Firmware Rev 3  (v3.1.0)
 *
 * TECNOLOGÍA PRINCIPAL: WiFi AP + HTTP REST API
 * STANDBY:             BLE advertising
 * DEBUG LED:           Códigos de blink no-bloqueantes en GPIO2
 *
 * CÓDIGOS DE BLINK (LED GPIO2):
 *   BOOT         ·−·−·−   3 destellos rápidos al arrancar
 *   WIFI_READY   −−       2 pulsos largos = AP listo
 *   CLIENT_IN    ·−·      corto-largo-corto = cliente conectado
 *   CLIENT_OUT   −·−      largo-corto-largo = cliente desconectó
 *   CHALLENGE_ON ····     4 rápidos = challenge activado
 *   IDLE         −        1 pulso largo cada 3 s = reposo normal
 *   HTTP_OK      ·        1 destello muy corto = petición atendida
 *   ERROR        SOS      ···−−−···  = error crítico (loop)
 *
 * Endpoints:
 *   GET  /api/status   → JSON con rssi, challenge, etc.
 *   POST /api/cmd      → body "CHALLENGE:1", "SLOT:2", "RESET"
 *   GET  /             → página de diagnóstico PWA
 */

#include <WiFi.h>
#include <WebServer.h>
#include <esp_wifi.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <esp_task_wdt.h>
#include <esp_idf_version.h>
#include <Preferences.h>

// ── VERSIÓN ────────────────────────────────────────────
#define FW_VERSION    "3.1.0"
#define FW_REV        3

// ── WIFI CONFIG ────────────────────────────────────────
#define WIFI_SSID     "THLS-C001"
#define WIFI_PASS     "thls2024"
#define SERVER_PORT   80

// ── BEACON CONFIG ──────────────────────────────────────
#define BEACON_ID     "C001"
#define BEACON_CLASS  "C"
#define BEACON_REGION "LAT-AR-01"
#define TX_POWER_1M   -59

// ── CHALLENGE SLOTS ────────────────────────────────────
const char* CHALLENGE_TYPES[] = { "wire_connect", "qr_scan", "morse_decode" };
const char* CHALLENGE_SEEDS[] = { "84726",        "QR-A042", "SOS"          };
const char* CHALLENGE_DIFFS[] = { "easy",         "medium",  "hard"         };
const int   CHALLENGE_COUNT   = 3;

// ── PINES ─────────────────────────────────────────────
#define LED_PIN       2
#define BUTTON_PIN    0
#define BAT_PIN       34
#define WDT_TIMEOUT_S 60

// ── UUIDs BLE ──────────────────────────────────────────
#define BLE_SVC_UUID  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

// ═══════════════════════════════════════════════════════
//  SISTEMA DE BLINK NO-BLOQUEANTE
// ═══════════════════════════════════════════════════════
// Duraciones base (ms)
#define DOT   120   // pulso corto  ·
#define DASH  400   // pulso largo  −
#define GAP   120   // pausa entre pulsos
#define PAUSE 600   // pausa entre grupos

// Patrones: {ON, OFF, ON, OFF, ...}, terminado en 0
//   ON  = tiempo encendido
//   OFF = tiempo apagado (negativo = pausa larga final)
const int16_t PAT_BOOT[]         = { DOT,GAP, DOT,GAP, DOT,-(PAUSE*2), 0 };
const int16_t PAT_WIFI_READY[]   = { DASH,GAP, DASH,-(PAUSE*3), 0 };
const int16_t PAT_CLIENT_IN[]    = { DOT,GAP, DASH,GAP, DOT,-(PAUSE*2), 0 };
const int16_t PAT_CLIENT_OUT[]   = { DASH,GAP, DOT,GAP, DASH,-(PAUSE*2), 0 };
const int16_t PAT_CHALLENGE_ON[] = { DOT,GAP, DOT,GAP, DOT,GAP, DOT,-(PAUSE*2), 0 };
const int16_t PAT_IDLE[]         = { DASH,-(3000), 0 };
const int16_t PAT_HTTP_OK[]      = { 40,60, 0 };
const int16_t PAT_ERROR[]        = {                          // SOS
  DOT,GAP, DOT,GAP, DOT, PAUSE,
  DASH,GAP, DASH,GAP, DASH, PAUSE,
  DOT,GAP, DOT,GAP, DOT,-(PAUSE*3), 0 };

struct LedBlinker {
  const int16_t* pattern = nullptr;
  const int16_t* next    = nullptr;   // patrón a reproducir después
  int  step   = 0;
  bool on     = false;
  unsigned long t = 0;

  void play(const int16_t* pat, const int16_t* after = nullptr) {
    pattern = pat; next = after; step = 0;
    on = true;                      // enciende inmediatamente
    digitalWrite(LED_PIN, HIGH);    // primer destello ya visible
    t = millis();
  }

  void update() {
    if (!pattern) return;
    if (millis() - t < (unsigned long)abs(pattern[step])) return;
    t = millis();
    on = !on;
    digitalWrite(LED_PIN, on ? HIGH : LOW);
    step++;
    if (pattern[step] == 0) {
      pattern = next;  // encadena siguiente o nullptr = fin
      next    = nullptr;
      step    = 0;
      on      = false;
      digitalWrite(LED_PIN, LOW);
    }
  }

  bool idle() { return pattern == nullptr; }
} led;

// ── ESTADO ────────────────────────────────────────────
bool     challengeActive = false;
int      challengeSlot   = 0;
uint32_t nonce           = 0xA001;
float    batVoltage      = 0.0f;
unsigned long lastBtnMs  = 0;
int      lastClients     = 0;
Preferences prefs;
WebServer   server(SERVER_PORT);

// ── RSSI del cliente WiFi conectado ───────────────────
int getClientRSSI() {
  wifi_sta_list_t sta;
  if (esp_wifi_ap_get_sta_list(&sta) == ESP_OK && sta.num > 0) {
    return sta.sta[0].rssi;
  }
  return -100; // nadie conectado
}

// ── JSON de respuesta (sin ArduinoJson — snprintf puro) ──
String makeStatusJson() {
  char buf[512];
  snprintf(buf, sizeof(buf),
    "{\"v\":3,\"fw\":\"%s\",\"id\":\"%s\",\"class\":\"%s\","
    "\"region\":\"%s\",\"rssi\":%d,\"challenge\":%s,"
    "\"type\":\"%s\",\"seed\":\"%s\",\"diff\":\"%s\","
    "\"timer\":60,\"slot\":%d,\"nonce\":\"%04X\","
    "\"bat\":%.2f,\"uptime\":%lu,\"clients\":%d}",
    FW_VERSION, BEACON_ID, BEACON_CLASS,
    BEACON_REGION, getClientRSSI(), challengeActive ? "true" : "false",
    CHALLENGE_TYPES[challengeSlot], CHALLENGE_SEEDS[challengeSlot], CHALLENGE_DIFFS[challengeSlot],
    challengeSlot, nonce,
    batVoltage, millis() / 1000UL, WiFi.softAPgetStationNum()
  );
  return String(buf);
}

// ── CORS headers ──────────────────────────────────────
void addCORS() {
  server.sendHeader("Access-Control-Allow-Origin",  "*");
  server.sendHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
  server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
}

// ── HANDLER: GET /api/status ──────────────────────────
void handleStatus() {
  addCORS();
  server.send(200, "application/json", makeStatusJson());
  if (led.idle()) led.play(PAT_HTTP_OK);   // · destello rápido = petición OK
  esp_task_wdt_reset();
}

// ── HANDLER: POST /api/cmd ────────────────────────────
void handleCmd() {
  addCORS();
  if (server.method() == HTTP_OPTIONS) { server.send(204); return; }

  String cmd = server.hasArg("cmd") ? server.arg("cmd")
             : server.hasArg("plain") ? server.arg("plain")
             : "";
  cmd.trim();
  Serial.println("[CMD] " + cmd);
  bool ok = true;

  if (cmd.startsWith("CHALLENGE:")) {
    challengeActive = (cmd.substring(10).toInt() == 1);
    prefs.putBool("chal", challengeActive);
  } else if (cmd.startsWith("SLOT:")) {
    challengeSlot = constrain(cmd.substring(5).toInt(), 0, CHALLENGE_COUNT-1);
    prefs.putInt("slot", challengeSlot);
  } else if (cmd.startsWith("NONCE:")) {
    nonce = strtoul(cmd.substring(6).c_str(), nullptr, 16);
    prefs.putUInt("nonce", nonce);
  } else if (cmd == "RESET") {
    server.send(200, "application/json", "{\"ok\":true}");
    delay(300); ESP.restart(); return;
  } else { ok = false; }

  server.send(200, "application/json", ok ? makeStatusJson() : "{\"error\":\"unknown cmd\"}");
}

// ── HANDLER: GET / — sirve la PWA de test en chunks ───
void handleRoot() {
  addCORS();
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(200, "text/html", "");

  server.sendContent(F("<!DOCTYPE html><html lang='es'><head>"
    "<meta charset='UTF-8'>"
    "<meta name='viewport' content='width=device-width,initial-scale=1'>"
    "<title>THLS Test</title><style>"
    "*{box-sizing:border-box;margin:0;padding:0}"
    "body{font-family:monospace;background:#07090f;color:#fff;"
         "display:flex;flex-direction:column;align-items:center;"
         "padding:24px 16px;gap:14px}"
    "#circle{width:150px;height:150px;border-radius:50%;display:flex;"
            "flex-direction:column;align-items:center;justify-content:center;"
            "border:3px solid #333;transition:background .4s,border-color .4s}"
    "#rssi{font-size:36px;font-weight:bold}"
    "#dist{font-size:12px;opacity:.7;margin-top:2px}"
    "#state{font-size:18px;font-weight:bold;letter-spacing:2px;text-align:center}"
    "#status{font-size:11px;color:#555;text-align:center;max-width:280px}"
    "button{background:#00b4d8;color:#000;border:none;border-radius:6px;"
           "padding:16px 0;font-family:monospace;font-weight:bold;"
           "font-size:18px;cursor:pointer;width:220px;letter-spacing:1px}"
    "</style></head><body>"));

  server.sendContent(F("<div style='font-size:11px;color:#00b4d8;letter-spacing:2px'>TREASURE HUNTERS IoT</div>"
    "<div id='circle'><div id='rssi'>-- dBm</div><div id='dist'>sin datos</div></div>"
    "<div id='state'>SIN CONEXION</div>"
    "<div id='status'>Presiona START</div>"
    "<button id='btn' onclick='toggle()'>&#9654; START</button>"));

  server.sendContent(F("<script>"
    "console.log('[THLS] page loaded — v3.0.0');"
    "var C=["
      "{n:-999,x:-94,bg:'#1a1a2e',l:'SIN SENAL'},"
      "{n:-94,x:-84,bg:'#0d47a1',l:'POLAR'},"
      "{n:-84,x:-75,bg:'#1976d2',l:'FRIO'},"
      "{n:-75,x:-67,bg:'#00838f',l:'FRESCO'},"
      "{n:-67,x:-60,bg:'#7b1fa2',l:'TIBIO'},"
      "{n:-60,x:-53,bg:'#c2185b',l:'CALIENTE'},"
      "{n:-53,x:-47,bg:'#e64a19',l:'MUY CALIENTE'},"
      "{n:-47,x:-41,bg:'#ff6f00',l:'ARDIENDO'},"
      "{n:-41,x:0,  bg:'#00c853',l:'BALIZA LOCALIZADA'}"
    "];"
    "var tmr=null,polls=0,errs=0;"
    "console.log('[THLS] estados cargados: '+C.length);"
    "function gS(r){return C.find(function(c){return r>=c.n&&r<c.x;})||C[0];}"
    "function gD(r,t){t=t||-59;var d=Math.pow(10,(t-r)/25);"
      "return d<1?'<1m':d>500?'>500m':d.toFixed(0)+'m';}"
    "function poll(){"
      "polls++;"
      "console.log('[THLS] poll #'+polls+' ...');"
      "var x=new XMLHttpRequest();"
      "x.open('GET','/api/status',true);"
      "x.timeout=2000;"
      "x.onload=function(){"
        "console.log('[THLS] respuesta HTTP '+x.status+': '+x.responseText);"
        "var d=JSON.parse(x.responseText);"
        "var r=d.rssi!=null?d.rssi:-100;"
        "var s=gS(r);"
        "console.log('[THLS] rssi='+r+' estado='+s.l);"
        "document.getElementById('rssi').textContent=r+' dBm';"
        "document.getElementById('dist').textContent='aprox '+gD(r,d.txPower);"
        "document.getElementById('state').textContent=s.l+(d.challenge?' [!CHALLENGE!]':'');"
        "document.getElementById('circle').style.background=s.bg+'55';"
        "document.getElementById('circle').style.borderColor=s.bg;"
        "document.getElementById('status').textContent='OK #'+polls+' | '+new Date().toLocaleTimeString();"
      "};"
      "x.onerror=function(){"
        "errs++;"
        "console.error('[THLS] XHR error #'+errs+' — estas conectado a THLS-C001?');"
        "document.getElementById('rssi').textContent='ERR';"
        "document.getElementById('state').textContent='SIN RESPUESTA';"
        "document.getElementById('circle').style.borderColor='#c00';"
        "document.getElementById('status').textContent='ERROR: conectar WiFi THLS-C001';"
      "};"
      "x.ontimeout=function(){"
        "errs++;"
        "console.warn('[THLS] timeout #'+errs+' — respuesta lenta del ESP32');"
        "document.getElementById('status').textContent='TIMEOUT #'+errs;"
      "};"
      "x.send();"
    "}"
    "function toggle(){"
      "var b=document.getElementById('btn');"
      "if(tmr){"
        "clearInterval(tmr);tmr=null;"
        "b.textContent='\\u25B6 START';b.style.background='#00b4d8';"
        "console.log('[THLS] polling detenido. polls='+polls+' errores='+errs);"
      "}else{"
        "b.textContent='\\u25A0 STOP';b.style.background='#e53935';"
        "console.log('[THLS] iniciando polling cada 1s');"
        "poll();tmr=setInterval(poll,1000);"
      "}"
    "}"
    "console.log('[THLS] listo — presiona START para comenzar');"
    "</script></body></html>"));

  server.sendContent("");
}

// ── BLE ADVERTISING — se llama al inicio y al cambiar challenge ──
BLEAdvertising* pAdv = nullptr;

void updateBLEAdvertising() {
  if (!pAdv) return;
  pAdv->stop();

  String devName = "THLS-" + String(BEACON_ID);

  // — Advertisement packet —
  // Contiene: flags + nombre corto + manufacturer data con challenge flag
  BLEAdvertisementData advData;
  advData.setFlags(0x06);  // LE General Discoverable, BR/EDR Not Supported

  // Nombre corto en el advertisement (no solo en scan response)
  // Así device.name llega aunque no se reciba scan response
  advData.setShortName(devName.c_str());

  // Manufacturer data: [0xFF 0xFF] company ID (custom) + [challenge] + [slot] + [txPower]
  // La app lo lee para saber si hay challenge ANTES de conectar WiFi
  String mfr;
  mfr += (char)0xFF; mfr += (char)0xFF;          // company ID (custom)
  mfr += (char)(challengeActive ? 0x01 : 0x00);  // challenge flag
  mfr += (char)(challengeSlot & 0xFF);            // slot actual
  mfr += (char)((uint8_t)(-TX_POWER_1M));         // txPower como byte positivo
  advData.setManufacturerData(mfr);
  pAdv->setAdvertisementData(advData);

  // — Scan response: nombre completo —
  BLEAdvertisementData scanData;
  scanData.setName(devName.c_str());
  pAdv->setScanResponseData(scanData);

  pAdv->start();
  Serial.printf("[BLE] Advertising actualizado — challenge:%s slot:%d\n",
    challengeActive ? "ON" : "OFF", challengeSlot);
}

// ── BLE STANDBY ───────────────────────────────────────
void initBLEStandby() {
  String devName = "THLS-" + String(BEACON_ID);
  BLEDevice::init(devName.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_P9);

  BLEServer*  pSrv = BLEDevice::createServer();
  BLEService* pSvc = pSrv->createService(BLE_SVC_UUID);
  pSvc->start();

  pAdv = BLEDevice::getAdvertising();
  pAdv->setMinPreferred(0x06);
  pAdv->setMaxPreferred(0x12);

  updateBLEAdvertising();  // configura y arranca
  Serial.printf("[BLE] Standby listo: \"%s\"\n", devName.c_str());
}

// ── SETUP ─────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.printf("\n=== THLS Firmware Rev %d v%s ===\n", FW_REV, FW_VERSION);

  // Watchdog
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
  esp_task_wdt_config_t wdt_cfg = { .timeout_ms=WDT_TIMEOUT_S*1000, .idle_core_mask=0, .trigger_panic=true };
  esp_task_wdt_reconfigure(&wdt_cfg);
#else
  esp_task_wdt_init(WDT_TIMEOUT_S, true);
#endif
  esp_task_wdt_add(nullptr);

  // GPIO
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  // Debug: BOOT
  led.play(PAT_BOOT);

  // NVS
  prefs.begin("thls", false);
  nonce          = prefs.getUInt("nonce",  0xA001);
  challengeActive= prefs.getBool("chal",   false);
  challengeSlot  = prefs.getInt ("slot",   0);

  // Batería
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  uint32_t sum = 0;
  for (int i = 0; i < 8; i++) { sum += analogRead(BAT_PIN); delay(2); }
  batVoltage = (sum / 8.0f) * (3.3f / 4095.0f) * 2.0f;

  // WiFi AP — SIN CONTRASEÑA (red abierta para detección)
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID);   // open network
  Serial.printf("[WiFi] AP abierto: \"%s\" | IP: %s\n", WIFI_SSID, WiFi.softAPIP().toString().c_str());
  led.play(PAT_WIFI_READY);   // −− = AP listo

  // HTTP server
  server.on("/",            HTTP_GET,  handleRoot);
  server.on("/api/status",  HTTP_GET,  handleStatus);
  server.on("/api/cmd",     HTTP_POST, handleCmd);
  server.on("/api/cmd",     HTTP_OPTIONS, handleCmd);
  server.begin();
  Serial.printf("[HTTP] Server en http://%s/api/status\n", WiFi.softAPIP().toString().c_str());

  // BLE standby
  initBLEStandby();

  Serial.println("[READY] Boton BOOT = toggle CHALLENGE");
}

// ── LOOP ──────────────────────────────────────────────
void loop() {
  esp_task_wdt_reset();
  server.handleClient();
  led.update();   // motor de blink no-bloqueante

  // Detectar cliente WiFi conectado / desconectado
  int clients = WiFi.softAPgetStationNum();
  if (clients != lastClients) {
    if (clients > lastClients) {
      Serial.printf("[WiFi] Cliente conectado (%d total)\n", clients);
      led.play(PAT_CLIENT_IN);    // ·−·
    } else {
      Serial.printf("[WiFi] Cliente desconectó (%d total)\n", clients);
      led.play(PAT_CLIENT_OUT);   // −·−
    }
    lastClients = clients;
  }

  // Botón BOOT: toggle challenge
  static bool lastBtn = HIGH;
  bool btn = digitalRead(BUTTON_PIN);
  if (lastBtn == HIGH && btn == LOW && (millis() - lastBtnMs) > 200) {
    lastBtnMs = millis();
    challengeActive = !challengeActive;
    if (challengeActive) challengeSlot = (challengeSlot + 1) % CHALLENGE_COUNT;
    nonce++;
    prefs.putBool("chal", challengeActive);
    prefs.putInt("slot", challengeSlot);
    Serial.printf("[BTN] Challenge %s | Slot %d (%s)\n",
      challengeActive ? "ON" : "OFF", challengeSlot, CHALLENGE_TYPES[challengeSlot]);
    if (challengeActive) led.play(PAT_CHALLENGE_ON);
    updateBLEAdvertising();  // actualiza manufacturer data con nuevo challenge flag
  }
  lastBtn = btn;

  // IDLE: pulso largo cada 3 s cuando no hay nada más
  static unsigned long idleMs = 0;
  if (led.idle() && (millis() - idleMs) > 3000) {
    idleMs = millis();
    led.play(PAT_IDLE);   // −
  }

  delay(10);
}
