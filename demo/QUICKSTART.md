# THLS Demo — Guía Rápida de Arranque

## Lo que necesitas

- ESP32 DevKit (cualquier variante con BLE)
- Arduino IDE 2.x
- Cable USB
- Android con Chrome (versión 80+)

---

## PASO 1 — Arduino IDE: instalar soporte ESP32

1. Abre Arduino IDE → **Preferences**
2. En *Additional boards manager URLs* agrega:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
3. **Tools → Board → Boards Manager** → busca `esp32` → instala **"esp32 by Espressif"**
4. **Tools → Board → ESP32 Dev Module**

---

## PASO 2 — Instalar librería ArduinoJson

**Tools → Manage Libraries** → busca `ArduinoJson` → instala **"ArduinoJson by Benoit Blanchon"** (v6.x)

---

## PASO 3 — Subir el firmware

1. Abre `firmware/thls_beacon_demo/thls_beacon_demo.ino`
2. (Opcional) Edita las constantes en las primeras líneas:
   ```cpp
   #define BEACON_ID   "C001"      // ID de tu baliza
   #define BEACON_CLASS "C"        // Clase (C para demo)
   #define TX_POWER_DBM -59        // Calibrar a 1 metro si quieres más precisión
   ```
3. **Tools → Port** → selecciona el puerto del ESP32
4. Presiona **Upload** (Ctrl+U)
5. Abre **Serial Monitor** (115200 baud) — debes ver:
   ```
   ================================================
     TREASURE HUNTERS IoT: THE LAST SIGNAL
     Beacon Demo Firmware v0.1
   ================================================
   [BLE] Advertising activo. Nombre: THLS-C001
   [BLE] Esperando conexion desde Chrome Android...
   ```

---

## PASO 4 — Abrir la PWA en Chrome Android

### Opción A: Servidor local (recomendado)
```bash
# Desde la carpeta demo/pwa/
python3 -m http.server 8080
# Luego visita: http://TU_IP_LOCAL:8080
```
O con Node:
```bash
npx serve .
```

### Opción B: Abrir directamente desde archivo
- Copia `index.html` al teléfono y abre con Chrome
- **LIMITACIÓN:** Web Bluetooth requiere HTTPS o localhost.
  Para archivo local, activa en Chrome: `chrome://flags/#unsafely-treat-insecure-origin-as-secure`

### Opción C: GitHub Pages (más fácil)
Sube `index.html` a GitHub Pages o [netlify.com/drop](https://app.netlify.com/drop) (drag & drop).

---

## PASO 5 — Conectar

1. Con el ESP32 encendido, abre la PWA en Chrome Android
2. Presiona **⬡ ESCANEAR**
3. Chrome te pide permiso Bluetooth → acepta
4. Aparece la lista de dispositivos BLE → selecciona **THLS-C001**
5. ¡Listo! Verás el estado en tiempo real

---

## Interacción con el ESP32

| Acción | Resultado |
|--------|-----------|
| Presiona botón **BOOT** del ESP32 | Activa/desactiva modo CHALLENGE |
| Aléjate del ESP32 | RSSI baja, estado cambia a FRÍO |
| Acércate al ESP32 | RSSI sube, estado cambia a CALIENTE |
| Challenge activo + cerca | Banner rojo de INFILTRACIÓN aparece |

> **Tip de calibración TxPower:** Para que la distancia estimada sea precisa, mide el RSSI
> a exactamente 1 metro del ESP32 en Serial Monitor y actualiza `TX_POWER_DBM` con ese valor.

---

## Limitaciones del demo

| Limitación | Razón | Solución futura |
|-----------|-------|-----------------|
| RSSI vía latencia (heurístico) | Web Bluetooth GATT no expone RSSI directamente | Usar `requestLEScan()` con flag experimental de Chrome |
| No funciona en iOS/Safari | Safari no implementa Web Bluetooth | App nativa con Capacitor |
| Requiere HTTPS o localhost | Seguridad de Web Bluetooth | Deploy en Netlify/GitHub Pages |

---

## Habilitar `requestLEScan` en Chrome Android (RSSI real)

Para una medición RSSI real sin conexión GATT, activa la flag experimental:
1. En Chrome Android, ve a: `chrome://flags/#enable-experimental-web-platform-features`
2. Activa la flag → reinicia Chrome
3. El escáner de la PWA detectará señales BLE sin necesidad de conectarse

---

## Estructura del proyecto

```
demo/
├── firmware/
│   └── thls_beacon_demo/
│       └── thls_beacon_demo.ino   ← Código ESP32
└── pwa/
    └── index.html                 ← App Web (todo en un archivo)
```

---

## UUIDs (para referencia)

```
Service:   4fafc201-1fb5-459e-8fcc-c5c9c331914b
Char Info: beb5483e-36e1-4688-b7f5-ea07361b26a8  (read+notify)
Char Chal: cba1d466-344c-4be3-ab3f-189f80dd7518  (read)
Char Tx:   d4e0b0c0-1234-5678-abcd-ef0123456789  (read)
```
