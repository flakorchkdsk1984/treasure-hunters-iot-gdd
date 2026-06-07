/**
 * TREASURE HUNTERS IoT: THE LAST SIGNAL
 * ESP32 — Firmware Rev 3  (v3.0.0)
 *
 * TECNOLOGÍA PRINCIPAL: WiFi AP + HTTP REST API
 * STANDBY:             BLE advertising (mismo UUID que Rev 2)
 *
 * Endpoints:
 *   GET  /api/status   → JSON con rssi, challenge, etc.
 *   POST /api/cmd      → body "CHALLENGE:1", "SLOT:2", "RESET", etc.
 *   GET  /             → página de diagnóstico
 *
 * Como usar:
 *   1. Sube el sketch al ESP32
 *   2. Conecta tu teléfono al WiFi "THLS-C001" (pass: thls2024)
 *   3. La PWA sondea http://192.168.4.1/api/status cada 1 segundo
 *   4. Botón BOOT (GPIO0) = toggle CHALLENGE
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
#define FW_VERSION    "3.0.0"
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
#define LED_PIN     2
#define BUTTON_PIN  0
#define BAT_PIN     34
#define WDT_TIMEOUT_S 60

// ── UUIDs BLE (standby, compatibles con Rev 2) ─────────
#define BLE_SVC_UUID  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

// ── ESTADO ────────────────────────────────────────────
bool     challengeActive = false;
int      challengeSlot   = 0;
uint32_t nonce           = 0xA001;
float    batVoltage      = 0.0f;
unsigned long lastBtnMs  = 0;
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

// ── HANDLER: GET / — sirve la PWA de test directamente ─
const char TEST_PAGE[] PROGMEM = R"rawhtml(<!DOCTYPE html>
<html lang="es"><head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>THLS Test</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:monospace;background:#07090f;color:#fff;
     display:flex;flex-direction:column;align-items:center;
     padding:24px 16px;gap:14px}
#circle{width:150px;height:150px;border-radius:50%;display:flex;
        flex-direction:column;align-items:center;justify-content:center;
        border:3px solid #333;transition:background .4s,border-color .4s}
#rssi{font-size:36px;font-weight:bold}
#dist{font-size:12px;opacity:.7;margin-top:2px}
#state{font-size:18px;font-weight:bold;letter-spacing:2px;text-align:center}
#status{font-size:11px;color:#555;text-align:center;max-width:280px}
button{background:#00b4d8;color:#000;border:none;border-radius:6px;
       padding:16px 0;font-family:monospace;font-weight:bold;
       font-size:18px;cursor:pointer;width:220px;letter-spacing:1px;
       margin-top:4px}
</style></head><body>
<div style="font-size:11px;color:#00b4d8;letter-spacing:2px">TREASURE HUNTERS IoT</div>
<div id="circle"><div id="rssi">—</div><div id="dist">sin datos</div></div>
<div id="state">SIN CONEXIÓN</div>
<div id="status">Presiona START para comenzar</div>
<button id="btn" onclick="toggle()">&#9654; START</button>
<script>
const C=[
  {n:-999,x:-94,bg:'#1a1a2e',l:'SIN SEÑAL'},
  {n:-94, x:-84,bg:'#0d47a1',l:'POLAR'},
  {n:-84, x:-75,bg:'#1976d2',l:'FRÍO'},
  {n:-75, x:-67,bg:'#00838f',l:'FRESCO'},
  {n:-67, x:-60,bg:'#7b1fa2',l:'TIBIO'},
  {n:-60, x:-53,bg:'#c2185b',l:'CALIENTE'},
  {n:-53, x:-47,bg:'#e64a19',l:'MUY CALIENTE'},
  {n:-47, x:-41,bg:'#ff6f00',l:'ARDIENDO'},
  {n:-41, x:0,  bg:'#00c853',l:'BALIZA LOCALIZADA'}
];
let tmr=null;
function gS(r){return C.find(c=>r>=c.n&&r<c.x)||C[0];}
function gD(r,t=-59){const d=Math.pow(10,(t-r)/25);return d<1?'< 1m':d>500?'>500m':d.toFixed(0)+'m';}
async function poll(){
  try{
    const r=await fetch('/api/status',{signal:AbortSignal.timeout(2000)});
    const d=await r.json();
    const s=gS(d.rssi??-100);
    document.getElementById('rssi').textContent=(d.rssi??-100)+' dBm';
    document.getElementById('dist').textContent='aprox '+gD(d.rssi??-100,d.txPower??-59);
    document.getElementById('state').textContent=s.l+(d.challenge?' ⚠':'');
    document.getElementById('circle').style.background=s.bg+'55';
    document.getElementById('circle').style.borderColor=s.bg;
    document.getElementById('status').textContent='OK · '+new Date().toLocaleTimeString();
  }catch(e){
    document.getElementById('rssi').textContent='✗';
    document.getElementById('dist').textContent='sin respuesta';
    document.getElementById('state').textContent='ERROR';
    document.getElementById('circle').style.borderColor='#c00';
    document.getElementById('status').textContent=e.message.slice(0,40);
  }
}
function toggle(){
  const b=document.getElementById('btn');
  if(tmr){clearInterval(tmr);tmr=null;b.textContent='\u25B6 START';b.style.background='#00b4d8';}
  else{b.textContent='\u25A0 STOP';b.style.background='#e53935';poll();tmr=setInterval(poll,1000);}
}
</script></body></html>)rawhtml";

void handleRoot() {
  addCORS();
  server.send_P(200, "text/html", TEST_PAGE);
}

// ── BLE STANDBY ───────────────────────────────────────
void initBLEStandby() {
  String devName = "THLS-" + String(BEACON_ID);
  BLEDevice::init(devName.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_P9);

  BLEServer*  pSrv = BLEDevice::createServer();
  BLEService* pSvc = pSrv->createService(BLE_SVC_UUID);
  pSvc->start();

  BLEAdvertising* pAdv = BLEDevice::getAdvertising();

  // Advertising data: flags + service UUID
  BLEAdvertisementData advData;
  advData.setFlags(0x06);                         // LE General Discoverable
  advData.setCompleteServices(BLEUUID(BLE_SVC_UUID));
  pAdv->setAdvertisementData(advData);

  // Scan response: nombre completo (Chrome/Android lo lee aquí)
  BLEAdvertisementData scanData;
  scanData.setName(devName.c_str());
  pAdv->setScanResponseData(scanData);

  pAdv->setMinPreferred(0x06);
  pAdv->setMaxPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.printf("[BLE] Standby: \"%s\" (scan response name activo)\n", devName.c_str());
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

  // WiFi AP
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASS);
  Serial.printf("[WiFi] AP: \"%s\" | IP: %s\n", WIFI_SSID, WiFi.softAPIP().toString().c_str());

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
    digitalWrite(LED_PIN, challengeActive ? HIGH : LOW);
    Serial.printf("[BTN] Challenge %s | Slot %d (%s)\n",
      challengeActive ? "ON" : "OFF", challengeSlot, CHALLENGE_TYPES[challengeSlot]);
  }
  lastBtn = btn;

  // LED: parpadeo lento normal, rápido en challenge
  static unsigned long ledMs = 0;
  unsigned long interval = challengeActive ? 150 : 1200;
  if (millis() - ledMs > interval) {
    ledMs = millis();
    if (!challengeActive) digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }

  delay(10);
}
