/**
 * TREASURE HUNTERS IoT: THE LAST SIGNAL
 * ESP32 — Firmware Demo v0.2  (sin dependencias extra)
 *
 * SOLO necesita el ESP32 Arduino core (no ArduinoJson, no librerias extra)
 *
 * Configuracion Arduino IDE:
 *  Board:        "ESP32 Dev Module"
 *  Upload Speed: 921600
 *  Soporte ESP32: Preferences -> Additional boards URLs:
 *    https://dl.espressif.com/dl/package_esp32_index.json
 *
 * Como usar:
 *  1. Sube el sketch al ESP32
 *  2. Abre Serial Monitor a 115200 baud
 *  3. Boton BOOT (GPIO0) = activa/desactiva modo CHALLENGE
 *  4. La PWA busca el dispositivo por nombre "THLS-C001"
 */

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// ---- CONFIGURACION (editar aqui) --------------------------------------------
#define BEACON_ID     "C001"
#define BEACON_CLASS  "C"
#define BEACON_REGION "LAT-AR-01"
#define TX_POWER_1M   -59    // RSSI medido a 1 metro (calibrar fisicamente)
#define CHALLENGE_TYPE "wire_connect"
#define CHALLENGE_SEED "84726"
#define CHALLENGE_DIFF "easy"

// ---- PINES ------------------------------------------------------------------
#define LED_PIN    2   // LED integrado en la mayoria de ESP32 DevKit
#define BUTTON_PIN 0   // Boton BOOT = GPIO0

// ---- UUIDs (deben coincidir exactamente con la PWA) -------------------------
#define SERVICE_UUID  "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR_INFO_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHAR_CHAL_UUID "cba1d466-344c-4be3-ab3f-189f80dd7518"

// ---- ESTADO -----------------------------------------------------------------
bool challengeActive = false;
bool deviceConnected = false;
uint32_t nonce = 0xA001;

BLEServer*         pServer  = nullptr;
BLECharacteristic* pCharInfo = nullptr;
BLECharacteristic* pCharChal = nullptr;

// ---- JSON sin ArduinoJson ---------------------------------------------------
String makeInfoJson() {
  String n = String(nonce, HEX);
  n.toUpperCase();
  return String("{\"v\":1,\"id\":\"") + BEACON_ID +
         "\",\"class\":\"" + BEACON_CLASS +
         "\",\"region\":\"" + BEACON_REGION +
         "\",\"nonce\":\"" + n +
         "\",\"challenge\":" + (challengeActive ? "true" : "false") +
         ",\"txPower\":" + String(TX_POWER_1M) + "}";
}

String makeChalJson() {
  return String("{\"type\":\"") + CHALLENGE_TYPE +
         "\",\"diff\":\"" + CHALLENGE_DIFF +
         "\",\"seed\":\"" + CHALLENGE_SEED +
         "\",\"timer\":60}";
}

// ---- CALLBACKS BLE ----------------------------------------------------------
class MyCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* s) override {
    deviceConnected = true;
    digitalWrite(LED_PIN, HIGH);
    Serial.println("[BLE] Cliente conectado");
  }
  void onDisconnect(BLEServer* s) override {
    deviceConnected = false;
    Serial.println("[BLE] Desconectado — reiniciando advertising...");
    delay(300);
    BLEDevice::startAdvertising();
  }
};

// ---- SETUP ------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("\n=== THLS Beacon Demo v0.2 ===");
  Serial.printf("ID: %s  Clase: %s  Region: %s\n", BEACON_ID, BEACON_CLASS, BEACON_REGION);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(LED_PIN, LOW);

  // Iniciar BLE con el nombre que busca la PWA
  String name = "THLS-" + String(BEACON_ID);
  BLEDevice::init(name.c_str());
  BLEDevice::setPower(ESP_PWR_LVL_P9);  // maxima potencia para demo

  // Servidor GATT
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyCallbacks());

  BLEService* pSvc = pServer->createService(SERVICE_UUID);

  // Caracteristica INFO: lectura + notify (estado de la baliza)
  pCharInfo = pSvc->createCharacteristic(
    CHAR_INFO_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharInfo->addDescriptor(new BLE2902());
  pCharInfo->setValue(makeInfoJson().c_str());

  // Caracteristica CHALLENGE: lectura (payload del minijuego)
  pCharChal = pSvc->createCharacteristic(
    CHAR_CHAL_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  pCharChal->setValue(makeChalJson().c_str());

  pSvc->start();

  // Advertising: SOLO addServiceUUID + setScanResponse = Chrome lo encuentra
  // NO usar setAdvertisementData() porque sobreescribe el service UUID
  BLEAdvertising* pAdv = BLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);  // <- CRITICO: esto lo hace visible al filtro de la PWA
  pAdv->setScanResponse(true);
  pAdv->setMinPreferred(0x06);
  pAdv->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  Serial.printf("[BLE] Advertising como \"%s\"\n", name.c_str());
  Serial.printf("[BLE] Service UUID: %s\n", SERVICE_UUID);
  Serial.println("[INFO] Presiona BOOT para toggle modo CHALLENGE");
  Serial.println("[INFO] Conéctate desde Chrome Android (busca " + name + ")");
  Serial.println("================================\n");
}

// ---- LOOP -------------------------------------------------------------------
void loop() {
  // Toggle challenge con boton BOOT
  static bool lastBtn = HIGH;
  bool btn = digitalRead(BUTTON_PIN);
  if (lastBtn == HIGH && btn == LOW) {
    delay(50);  // debounce
    challengeActive = !challengeActive;
    nonce++;

    String info = makeInfoJson();
    pCharInfo->setValue(info.c_str());
    if (deviceConnected) pCharInfo->notify();

    Serial.printf("\n--- MODO: %s --- nonce: %X ---\n",
      challengeActive ? "CHALLENGE ACTIVO" : "NORMAL", nonce);
    Serial.println("INFO: " + info);

    // Parpadeo de confirmacion
    int blinks = challengeActive ? 5 : 2;
    for (int i = 0; i < blinks; i++) {
      digitalWrite(LED_PIN, HIGH); delay(80);
      digitalWrite(LED_PIN, LOW);  delay(80);
    }
  }
  lastBtn = btn;

  // LED heartbeat: lento=normal, rapido=challenge, fijo=conectado
  if (!deviceConnected) {
    static unsigned long t = 0;
    int iv = challengeActive ? 150 : 900;
    if (millis() - t > iv) {
      t = millis();
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }

  // Status cada 5 segundos
  static unsigned long tLog = 0;
  if (millis() - tLog > 5000) {
    tLog = millis();
    Serial.printf("[OK] uptime:%lus challenge:%s connected:%s nonce:%X\n",
      millis()/1000, challengeActive?"SI":"NO",
      deviceConnected?"SI":"NO", nonce);
  }

  delay(10);
}
