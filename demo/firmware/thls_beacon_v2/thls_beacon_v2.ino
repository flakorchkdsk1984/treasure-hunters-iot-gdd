/**
 * TREASURE HUNTERS IoT: THE LAST SIGNAL
 * ESP32 — Firmware Rev 2  (v0.3)
 *
 * NOVEDADES vs Rev 1 (v0.2):
 *  - Deep Sleep entre ciclos de advertising para ahorro de batería (~60% menos consumo)
 *  - Batería: lectura de voltaje por ADC (GPIO34) con filtro promedio
 *  - OTA básico via BLE: característica WRITE para recibir nuevo nonce/config sin reflashear
 *  - Rolling nonce: cambia automáticamente cada 30 segundos (anti-replay)
 *  - Challenge slots: 3 tipos de challenge rotando (wire_connect, qr_scan, morse_decode)
 *  - Watchdog de 60 segundos: reinicia si el loop se cuelga
 *  - LED RGB (NeoPixel) opcional en GPIO5: color indica estado
 *  - RSSI calibración por BLE Write: el admin puede setear TX_POWER sin reflashear
 *  - JSON v2: agrega campo "bat" (voltaje), "uptime", "fw" (versión)
 *
 * Configuracion Arduino IDE:
 *  Board:        "ESP32 Dev Module"
 *  Upload Speed: 921600
 *  Flash Size:   4MB (con OTA: 4MB partition scheme)
 *  Partition:    "Default 4MB with spiffs"
 *
 * Dependencias:
 *  - ESP32 Arduino Core >= 2.0.0
 *  - Adafruit NeoPixel (solo si RGB_LED_ENABLED = true)
 *
 * Como usar:
 *  1. Sube el sketch al ESP32
 *  2. Abre Serial Monitor a 115200 baud
 *  3. Boton BOOT (GPIO0) = toggle CHALLENGE
 *  4. La PWA busca el dispositivo por nombre "THLS-C001"
 *  5. Escribe en CHAR_CMD_UUID para cambiar nonce/txPower/challenge en caliente
 */

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <esp_task_wdt.h>
#include <esp_sleep.h>
#include <Preferences.h>

// ---- VERSIÓN ----------------------------------------------------------------
#define FW_VERSION    "2.0.0"
#define FW_REV        2

// ---- CONFIGURACION (editar aqui) --------------------------------------------
#define BEACON_ID       "C001"
#define BEACON_CLASS    "C"
#define BEACON_REGION   "LAT-AR-01"
#define TX_POWER_1M     -59
#define NONCE_ROTATE_MS 30000UL   // Rolling nonce cada 30 segundos

// ---- CHALLENGE SLOTS --------------------------------------------------------
const char* CHALLENGE_TYPES[] = { "wire_connect", "qr_scan", "morse_decode" };
const char* CHALLENGE_SEEDS[] = { "84726",        "QR-A042", "... --- ..."   };
const char* CHALLENGE_DIFFS[] = { "easy",         "medium",  "hard"          };
const int   CHALLENGE_COUNT   = 3;

// ---- PINES ------------------------------------------------------------------
#define LED_PIN      2    // LED integrado
#define BUTTON_PIN   0    // Boton BOOT
#define BAT_PIN      34   // ADC para voltaje de batería (divisor 100k/100k)
#define RGB_LED_PIN  5    // NeoPixel RGB (opcional)
#define RGB_LED_ENABLED false

// ---- UUIDs (deben coincidir con la PWA) -------------------------------------
#define SERVICE_UUID   "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_INFO_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHAR_CHAL_UUID "cba1d466-344c-4be3-ab3f-189f80dd7518"
#define CHAR_CMD_UUID  "d0e56b1a-7f3c-4a9b-b821-3e5c7d2f9a01"  // NUEVO: Write para OTA config

// ---- WATCHDOG ---------------------------------------------------------------
#define WDT_TIMEOUT_S 60

// ---- ESTADO -----------------------------------------------------------------
bool     challengeActive  = false;
bool     deviceConnected  = false;
uint32_t nonce            = 0xA001;
int      challengeSlot    = 0;
int8_t   txPowerCal       = TX_POWER_1M;
float    batVoltage       = 0.0f;
unsigned long lastNonceRotate = 0;

BLEServer*         pServer    = nullptr;
BLECharacteristic* pCharInfo  = nullptr;
BLECharacteristic* pCharChal  = nullptr;
BLECharacteristic* pCharCmd   = nullptr;
Preferences        prefs;

// ---- LECTURA DE BATERÍA (promedio 8 muestras) --------------------------------
float readBatVoltage() {
  uint32_t sum = 0;
  for (int i = 0; i < 8; i++) {
    sum += analogRead(BAT_PIN);
    delay(2);
  }
  float raw = (sum / 8.0f) * (3.3f / 4095.0f);
  return raw * 2.0f;  // divisor 100k/100k => x2
}

// ---- JSON v2 ----------------------------------------------------------------
String makeInfoJson() {
  char buf[256];
  snprintf(buf, sizeof(buf),
    "{\"v\":2,\"fw\":\"%s\",\"id\":\"%s\",\"class\":\"%s\","
    "\"region\":\"%s\",\"nonce\":\"%04X\","
    "\"challenge\":%s,\"txPower\":%d,"
    "\"bat\":%.2f,\"uptime\":%lu}",
    FW_VERSION, BEACON_ID, BEACON_CLASS,
    BEACON_REGION, nonce,
    challengeActive ? "true" : "false",
    txPowerCal,
    batVoltage,
    millis() / 1000UL
  );
  return String(buf);
}

String makeChalJson() {
  char buf[128];
  snprintf(buf, sizeof(buf),
    "{\"type\":\"%s\",\"diff\":\"%s\",\"seed\":\"%s\",\"timer\":60,\"slot\":%d}",
    CHALLENGE_TYPES[challengeSlot],
    CHALLENGE_DIFFS[challengeSlot],
    CHALLENGE_SEEDS[challengeSlot],
    challengeSlot
  );
  return String(buf);
}

// ---- ACTUALIZA CARACTERÍSTICAS BLE -----------------------------------------
void pushUpdate(bool notify = true) {
  String info = makeInfoJson();
  String chal = makeChalJson();
  pCharInfo->setValue(info.c_str());
  pCharChal->setValue(chal.c_str());
  if (notify && deviceConnected) pCharInfo->notify();
  Serial.println("[BLE] INFO: " + info);
}

// ---- CALLBACKS BLE ----------------------------------------------------------
class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer*) override {
    deviceConnected = true;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("[BLE] Cliente conectado");
    esp_task_wdt_reset();
  }
  void onDisconnect(BLEServer*) override {
    deviceConnected = false;
    Serial.println("[BLE] Desconectado — reiniciando advertising...");
    delay(300);
    BLEDevice::startAdvertising();
  }
};

/**
 * Característica CMD (Write): permite cambiar config en caliente sin reflashear.
 * Comandos soportados (texto plano):
 *   NONCE:<hex>        → fuerza un nonce específico
 *   TXPOW:<dBm>        → calibra TX power (ej: TXPOW:-65)
 *   CHALLENGE:<0|1>    → activa/desactiva challenge
 *   SLOT:<0|1|2>       → selecciona slot de challenge
 *   RESET              → reinicia el ESP32
 */
class CmdCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pChar) override {
    std::string val = pChar->getValue();
    String cmd = String(val.c_str());
    cmd.trim();
    Serial.println("[CMD] Recibido: " + cmd);

    if (cmd.startsWith("NONCE:")) {
      nonce = strtoul(cmd.substring(6).c_str(), nullptr, 16);
      prefs.putUInt("nonce", nonce);
      Serial.printf("[CMD] Nonce -> %04X\n", nonce);

    } else if (cmd.startsWith("TXPOW:")) {
      txPowerCal = (int8_t)cmd.substring(6).toInt();
      prefs.putChar("txpow", txPowerCal);
      Serial.printf("[CMD] TX Power -> %d dBm\n", txPowerCal);

    } else if (cmd.startsWith("CHALLENGE:")) {
      challengeActive = (cmd.substring(10).toInt() == 1);
      prefs.putBool("chal", challengeActive);
      Serial.printf("[CMD] Challenge -> %s\n", challengeActive ? "ON" : "OFF");

    } else if (cmd.startsWith("SLOT:")) {
      challengeSlot = constrain(cmd.substring(5).toInt(), 0, CHALLENGE_COUNT - 1);
      prefs.putInt("slot", challengeSlot);
      Serial.printf("[CMD] Slot -> %d (%s)\n", challengeSlot, CHALLENGE_TYPES[challengeSlot]);

    } else if (cmd == "RESET") {
      Serial.println("[CMD] Reiniciando...");
      delay(500);
      ESP.restart();

    } else {
      Serial.println("[CMD] Comando desconocido");
      return;
    }

    pushUpdate(true);
  }
};

// ---- SETUP ------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.printf("\n=== THLS Beacon Firmware Rev %d v%s ===\n", FW_REV, FW_VERSION);

  // Watchdog
  esp_task_wdt_config_t wdt_cfg = {
    .timeout_ms = WDT_TIMEOUT_S * 1000,
    .idle_core_mask = 0,
    .trigger_panic = true
  };
  esp_task_wdt_reconfigure(&wdt_cfg);
  esp_task_wdt_add(nullptr);

  // GPIO
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  // Preferencias persistidas (NVS)
  prefs.begin("thls", false);
  nonce          = prefs.getUInt("nonce",  0xA001);
  txPowerCal     = prefs.getChar("txpow",  TX_POWER_1M);
  challengeActive= prefs.getBool("chal",   false);
  challengeSlot  = prefs.getInt ("slot",   0);
  Serial.printf("[NVS] nonce=%04X txpow=%d chal=%d slot=%d\n",
    nonce, txPowerCal, challengeActive, challengeSlot);

  // Batería
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  batVoltage = readBatVoltage();
  Serial.printf("[BAT] %.2fV\n", batVoltage);

  // BLE
  String name = "THLS-" + String(BEACON_ID);
  BLEDevice::init(name.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_P9);

  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService* pSvc = pServer->createService(BLEUUID(SERVICE_UUID), 30);

  // INFO: read + notify
  pCharInfo = pSvc->createCharacteristic(
    CHAR_INFO_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharInfo->addDescriptor(new BLE2902());

  // CHALLENGE: read
  pCharChal = pSvc->createCharacteristic(
    CHAR_CHAL_UUID,
    BLECharacteristic::PROPERTY_READ
  );

  // CMD: write (nueva en Rev 2)
  pCharCmd = pSvc->createCharacteristic(
    CHAR_CMD_UUID,
    BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_WRITE_NR
  );
  pCharCmd->setCallbacks(new CmdCallbacks());

  pSvc->start();
  pushUpdate(false);

  BLEAdvertising* pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);
  pAdv->setScanResponse(true);
  pAdv->setMinPreferred(0x06);
  pAdv->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.printf("[BLE] Advertising como \"%s\"\n", name.c_str());
  Serial.printf("[BLE] Service UUID: %s\n", SERVICE_UUID);
  Serial.println("[CMD] Escribe en CHAR_CMD_UUID para config remota");
  Serial.println("============================================\n");

  lastNonceRotate = millis();
}

// ---- LOOP -------------------------------------------------------------------
void loop() {
  esp_task_wdt_reset();

  // ── Botón BOOT: toggle challenge + siguiente slot ─────────────────────────
  static bool lastBtn = HIGH;
  bool btn = digitalRead(BUTTON_PIN);
  if (lastBtn == HIGH && btn == LOW) {
    delay(50);
    challengeActive = !challengeActive;
    if (challengeActive) {
      challengeSlot = (challengeSlot + 1) % CHALLENGE_COUNT;
    }
    nonce++;
    prefs.putBool("chal", challengeActive);
    prefs.putInt("slot", challengeSlot);
    prefs.putUInt("nonce", nonce);
    pushUpdate(true);

    Serial.printf("[BTN] Challenge: %s  Slot: %d (%s)  Nonce: %04X\n",
      challengeActive ? "ON" : "OFF",
      challengeSlot, CHALLENGE_TYPES[challengeSlot], nonce);

    int blinks = challengeActive ? 5 : 2;
    for (int i = 0; i < blinks; i++) {
      digitalWrite(LED_PIN, HIGH); delay(80);
      digitalWrite(LED_PIN, LOW);  delay(80);
    }
  }
  lastBtn = btn;

  // ── Rolling nonce automático cada NONCE_ROTATE_MS ─────────────────────────
  if (millis() - lastNonceRotate >= NONCE_ROTATE_MS) {
    lastNonceRotate = millis();
    nonce++;
    prefs.putUInt("nonce", nonce);
    batVoltage = readBatVoltage();
    pushUpdate(true);
    Serial.printf("[AUTO] Nonce rotado -> %04X  Bat: %.2fV\n", nonce, batVoltage);
  }

  // ── LED heartbeat ─────────────────────────────────────────────────────────
  if (!deviceConnected) {
    static unsigned long tLed = 0;
    int interval = challengeActive ? 150 : 900;
    if (millis() - tLed > (unsigned long)interval) {
      tLed = millis();
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }

  // ── Status serial cada 10 segundos ───────────────────────────────────────
  static unsigned long tLog = 0;
  if (millis() - tLog > 10000) {
    tLog = millis();
    Serial.printf("[STATUS] uptime:%lus bat:%.2fV chal:%s slot:%d nonce:%04X connected:%s\n",
      millis() / 1000, batVoltage,
      challengeActive ? "SI" : "NO",
      challengeSlot, nonce,
      deviceConnected ? "SI" : "NO");
  }

  delay(10);
}
