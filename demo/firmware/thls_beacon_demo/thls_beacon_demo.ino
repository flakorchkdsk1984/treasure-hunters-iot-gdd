/**
 * TREASURE HUNTERS IoT: THE LAST SIGNAL
 * ESP32 — Firmware Demo v0.1
 *
 * Funciones:
 *  - Advertising BLE con payload del juego (manufacturer data)
 *  - Servidor GATT con servicio de baliza (para PWA via Web Bluetooth)
 *  - LED integrado muestra estado (lento=normal, rapido=challenge)
 *  - Boton BOOT (GPIO0) cambia entre estado NORMAL y CHALLENGE
 *  - Serial Monitor: debug en tiempo real a 115200 baud
 *
 * Librerias requeridas (incluidas en ESP32 Arduino core):
 *  - BLEDevice, BLEServer, BLEUtils, BLE2902
 *  + ArduinoJson (instalar via Library Manager: buscar "ArduinoJson" by Benoit Blanchon)
 *
 * Configuracion Arduino IDE:
 *  - Board: "ESP32 Dev Module"
 *  - Upload Speed: 921600
 *  - CPU Freq: 240MHz
 *  - Soporte ESP32: https://dl.espressif.com/dl/package_esp32_index.json
 */

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <BLEAdvertising.h>
#include <ArduinoJson.h>

// ---- CONFIGURACION DE LA BALIZA (editar aqui) --------------------------------
#define BEACON_ID        "C001"          // ID unico de esta baliza
#define BEACON_CLASS     "C"             // Clase: C, B, A, S
#define BEACON_REGION    "LAT-AR-01"     // Region logica
#define TX_POWER_DBM     -59             // RSSI calibrado a 1 metro (medir fisicamente)
#define CHALLENGE_TYPE   "wire_connect"  // Tipo de minijuego
#define CHALLENGE_SEED   "84726"         // Seed del puzzle (en produccion: rotativo)

// ---- PINES -------------------------------------------------------------------
#define LED_PIN    2   // LED azul integrado en ESP32 DevKit
#define BUTTON_PIN 0   // Boton BOOT = GPIO0

// ---- UUIDs BLE (deben coincidir exactamente con la PWA) ----------------------
#define SERVICE_UUID     "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_INFO_UUID   "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHAR_CHAL_UUID   "cba1d466-344c-4be3-ab3f-189f80dd7518"
#define CHAR_TX_UUID     "d4e0b0c0-1234-5678-abcd-ef0123456789"

// ---- ESTADO GLOBAL -----------------------------------------------------------
bool challengeActive = false;
bool deviceConnected = false;
uint32_t nonceCounter = 0xA000;

BLEServer*         pServer      = nullptr;
BLECharacteristic* pCharInfo    = nullptr;
BLECharacteristic* pCharChallenge = nullptr;
BLECharacteristic* pCharTx      = nullptr;

// ---- HELPERS -----------------------------------------------------------------
String generateNonce() {
  nonceCounter++;
  String n = String(nonceCounter, HEX);
  n.toUpperCase();
  while (n.length() < 6) n = "0" + n;
  return n;
}

String currentNonce = "A00001";

String buildInfoJson() {
  StaticJsonDocument<200> doc;
  doc["v"]         = 1;
  doc["id"]        = BEACON_ID;
  doc["class"]     = BEACON_CLASS;
  doc["region"]    = BEACON_REGION;
  doc["nonce"]     = currentNonce;
  doc["challenge"] = challengeActive;
  doc["txPower"]   = TX_POWER_DBM;
  String out;
  serializeJson(doc, out);
  return out;
}

String buildChallengeJson() {
  StaticJsonDocument<200> doc;
  doc["type"]   = CHALLENGE_TYPE;
  doc["diff"]   = (strcmp(BEACON_CLASS,"C")==0) ? "easy" :
                  (strcmp(BEACON_CLASS,"B")==0) ? "medium" : "hard";
  doc["seed"]   = CHALLENGE_SEED;
  doc["nonce"]  = currentNonce;
  doc["timer"]  = (strcmp(BEACON_CLASS,"C")==0) ? 60 : 90;
  String out;
  serializeJson(doc, out);
  return out;
}

void updateCharacteristics() {
  String info = buildInfoJson();
  pCharInfo->setValue(info.c_str());
  if (deviceConnected) pCharInfo->notify();
  pCharChallenge->setValue(buildChallengeJson().c_str());
}

// ---- CALLBACKS BLE -----------------------------------------------------------
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pSrv) override {
    deviceConnected = true;
    Serial.println("[BLE] >>> Cliente conectado");
    digitalWrite(LED_PIN, HIGH);
  }
  void onDisconnect(BLEServer* pSrv) override {
    deviceConnected = false;
    Serial.println("[BLE] <<< Cliente desconectado — reiniciando advertising...");
    delay(300);
    BLEDevice::startAdvertising();
  }
};

// ---- SETUP -------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println();
  Serial.println("================================================");
  Serial.println("  TREASURE HUNTERS IoT: THE LAST SIGNAL");
  Serial.println("  Beacon Demo Firmware v0.1");
  Serial.println("================================================");
  Serial.printf("  ID: %-6s  Clase: %s  Region: %s\n", BEACON_ID, BEACON_CLASS, BEACON_REGION);
  Serial.printf("  TxPower (1m): %d dBm\n", TX_POWER_DBM);
  Serial.printf("  Service UUID: %s\n", SERVICE_UUID);
  Serial.println("  [BOOT] = toggle modo CHALLENGE");
  Serial.println("================================================\n");

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  // Iniciar BLE
  String deviceName = "THLS-" + String(BEACON_ID);
  BLEDevice::init(deviceName.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_P9); // +9 dBm maximo para demo

  // Servidor GATT
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Caracteristica 1: Info de la baliza (readable + notify)
  pCharInfo = pService->createCharacteristic(
    CHAR_INFO_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharInfo->addDescriptor(new BLE2902());
  pCharInfo->setValue(buildInfoJson().c_str());

  // Caracteristica 2: Payload de desafio (readable)
  pCharChallenge = pService->createCharacteristic(
    CHAR_CHAL_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  pCharChallenge->setValue(buildChallengeJson().c_str());

  // Caracteristica 3: TxPower calibrado (1 byte signed, para calculo distancia en app)
  pCharTx = pService->createCharacteristic(
    CHAR_TX_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  int8_t txVal = (int8_t)TX_POWER_DBM;
  pCharTx->setValue((uint8_t*)&txVal, 1);

  pService->start();

  // Advertising
  BLEAdvertising* pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);
  pAdv->setScanResponse(true);
  pAdv->setMinPreferred(0x06);
  pAdv->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.println("[BLE] Advertising activo. Nombre: " + deviceName);
  Serial.println("[BLE] Esperando conexion desde Chrome Android...\n");
}

// ---- LOOP --------------------------------------------------------------------
void loop() {
  // Boton BOOT: toggle challenge mode
  static bool lastBtn = HIGH;
  bool btn = digitalRead(BUTTON_PIN);
  if (lastBtn == HIGH && btn == LOW) {
    delay(50); // debounce
    challengeActive = !challengeActive;
    currentNonce = generateNonce();
    updateCharacteristics();

    Serial.println("────────────────────────────────────");
    Serial.printf("[MODO] %s\n", challengeActive ? ">> CHALLENGE ACTIVO <<" : "NORMAL");
    Serial.printf("[NONCE] %s\n", currentNonce.c_str());
    Serial.printf("[JSON] %s\n", buildInfoJson().c_str());
    Serial.println("────────────────────────────────────");

    // Parpadeo de confirmacion
    for (int i = 0; i < (challengeActive ? 5 : 2); i++) {
      digitalWrite(LED_PIN, HIGH); delay(80);
      digitalWrite(LED_PIN, LOW);  delay(80);
    }
  }
  lastBtn = btn;

  // Heartbeat LED: lento=normal, rapido=challenge, fijo=conectado
  if (!deviceConnected) {
    static unsigned long lastBlink = 0;
    int interval = challengeActive ? 150 : 800;
    if (millis() - lastBlink > interval) {
      lastBlink = millis();
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }

  // Log cada 5 segundos
  static unsigned long lastLog = 0;
  if (millis() - lastLog > 5000) {
    lastLog = millis();
    Serial.printf("[STATUS] id:%s class:%s challenge:%s nonce:%s connected:%s uptime:%lus\n",
      BEACON_ID, BEACON_CLASS,
      challengeActive ? "YES" : "NO",
      currentNonce.c_str(),
      deviceConnected ? "YES" : "NO",
      millis() / 1000);
  }

  // Rotar nonce cada 5 minutos
  static unsigned long lastNonce = 0;
  if (millis() - lastNonce > 300000) {
    lastNonce = millis();
    currentNonce = generateNonce();
    updateCharacteristics();
    Serial.printf("[NONCE] Rotacion automatica -> %s\n", currentNonce.c_str());
  }

  delay(10);
}
