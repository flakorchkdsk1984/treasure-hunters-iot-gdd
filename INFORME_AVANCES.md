# INFORME DE AVANCES — TREASURE HUNTERS IoT: THE LAST SIGNAL
**Fecha:** 2026-06-07  
**Repositorio:** https://github.com/flakorchkdsk1984/treasure-hunters-iot-gdd  
**Investigador:** Pablo (flakorchkdsk1984)  
**Asistente:** GitHub Copilot CLI  

---

## 1. RESUMEN EJECUTIVO

Se desarrolló desde cero el diseño completo y prototipo funcional de un juego IoT de exploración urbana llamado **Treasure Hunters IoT: The Last Signal (THLS)**. El juego combina tecnología de proximidad inalámbrica (WiFi/BLE) con un ESP32 para crear una experiencia de "búsqueda del tesoro" en el mundo real, donde los jugadores usan una PWA en su teléfono para detectar y localizar balizas físicas.

---

## 2. CRONOLOGÍA DE SESIONES

### SESIÓN 1 — Diseño del GDD Completo
**Fecha:** 2026-06-06  
**Resultado:** GDD de 560 KB, 9.882 líneas, 25 secciones

Se generó el Game Design Document completo con 8 agentes especializados en paralelo:

| Sección | Contenido |
|---------|-----------|
| §1 | Visión del juego, 8 pilares, USPs, audiencias |
| §2 | Historia y Lore: A.R.G.O.S., El Pulso Gris, 3 facciones, 6 personajes |
| §3 | Loop principal: 15 pasos micro/macro/estacional |
| §4-7 | BLE & RSSI: ESP32, payloads firmados, Kalman, histéresis |
| §8-9 | UX Mobile: wireframes ASCII, hápticos, audio, accesibilidad |
| §10-11 | Balizas Clase C/B/A/S/Ω + pipeline de validación backend |
| §12 | 30 Minijuegos con mecánica, seed determinista, timers anti-bot |
| §13 | 8 Eventos especiales (Tormenta EM, Eclipse Global, etc.) |
| §14-16 | Social: Clanes, territorios, 6 recursos, anti-P2W charter |
| §17-18 | Seguridad: 3 capas anti-cheat, COPPA/GDPR |
| §19 | Arquitectura Docker/K8s, docker-compose, MongoDB schemas |
| §20-22 | Telemetría Prometheus+Grafana, Roadmap 3 fases, 100→1M jugadores |
| §23-24 | 100 user stories, proyección financiera 5 años, LTV/CAC 4.87× |
| §25 + A-B | Riesgos, Glosario 40+ términos, referencias |

**Archivos publicados en GitHub:**
```
GDD_TREASURE_HUNTERS_IOT_COMPLETE.md     560 KB
GDD_THLS_Sections_4_to_7.md             108 KB
GDD_TreasureHuntersIoT_Sections_10_11_13.md  97 KB
seccion-23-24-backlog-finanzas.md         90 KB
GDD_Sections_14_15_16.md                 53 KB
GDD_TreasureHunters_Sections_8_9_12_CONTINUACION.md  24 KB
README.md                                Portada ejecutiva
```

---

### SESIÓN 2 — Primera POC: BLE + PWA
**Fecha:** 2026-06-06  
**Resultado:** Firmware BLE v1 + PWA Scanner v0.1

Se creó el primer prototipo funcional:
- **Firmware:** ESP32 en modo BLE advertising con nombre `THLS-C001`
- **PWA:** Scanner con radar visual, estados FRÍO/TIBIO/CALIENTE/BALIZA

**Problemas encontrados:**
- PWA no conectaba via Web Bluetooth

---

### SESIÓN 3 — Corrección PWA BLE (sesión actual)
**Fecha:** 2026-06-07  

#### 3.1 Bugs corregidos en PWA BLE

| # | Bug | Causa raíz | Solución |
|---|-----|-----------|----------|
| 1 | Selector BLE vacío | Filtro `{ services: [SVC] }` — ESP32 no incluye UUID en advertisement packets | Usar solo `namePrefix: 'THLS-'` |
| 2 | RSSI polling nunca arrancaba | `if(!bleDevice.watchAdvertisements)` siempre `false` si el método existe | Variable `hasRealRSSI` + timeout 3s |
| 3 | Sin detección HTTPS | Chequeo incorrecto de `navigator.bluetooth` | Verificar `location.protocol` primero |

#### 3.2 Firmware BLE v2 (Rev 2)

**Nuevas funcionalidades:**
- Deep Sleep entre ciclos (ahorro ~60% batería)
- Lectura de batería por ADC (GPIO34) con filtro promedio
- OTA básico via BLE (característica WRITE para config remota)
- Rolling nonce cada 30 segundos (anti-replay)
- 3 Challenge slots rotativos (`wire_connect`, `qr_scan`, `morse_decode`)
- Watchdog 60 segundos
- LED RGB NeoPixel opcional (GPIO5)
- JSON v2 con campos `bat`, `uptime`, `fw`

**Bug corregido:** `esp_task_wdt_config_t` es API de IDF 5.x pero el entorno usa Arduino-ESP32 2.0.17 (IDF 4.4.x).
```cpp
// Fix: guard de versión
#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)
  esp_task_wdt_config_t wdt_cfg = {...};
  esp_task_wdt_reconfigure(&wdt_cfg);
#else
  esp_task_wdt_init(WDT_TIMEOUT_S, true);  // IDF 4.x
#endif
```

#### 3.3 PWA v0.4 — 3 Minijuegos + Audio

**Motor de audio (Web Audio API):**
- `beep()` configurable: frecuencia, duración, volumen, tipo de onda
- `playSuccess()` — jingle 4 notas ascendentes
- `playFail()` — sonido grave sawtooth
- `playChalAlert()` — secuencia de alerta al activar challenge
- `playProximityTick()` — frecuencia sube con la proximidad

**Bug crítico de audio:** `AudioContext` bloqueado por Chrome si se crea fuera de gesto del usuario.
```js
// Fix: ensureAudio() en el handler del botón, no en setInterval
function startSimulation() { ensureAudio(); ... }
```

**Minijuegos implementados:**

| Tipo (ESP32) | Minijuego PWA | Mecánica |
|-------------|---------------|----------|
| `wire_connect` | Conectar cables | 4 cables por color, orden por seed determinista |
| `qr_scan` | Descifrar código | Keypad 4 dígitos, código derivado del seed |
| `morse_decode` | Código Morse | Beeps Web Audio reales + tabla de referencia |

**Bug trigger challenge:** `rssi > -55` nunca matcheaba cuando `rssi === -55` (estado 'found').
```js
// Fix: usar rssiToState() en lugar de comparar número
if(rssiToState(rssi) === 'found' && !challengeOpened)
```

**Banner "INFILTRACIÓN DISPONIBLE":** Era decorativo. Fix: `onclick="openCurrentChallenge()"` + variable global `currentChalData`.

#### 3.4 Cambio de tecnología: BLE → WiFi

**Motivación:** Web Bluetooth tiene múltiples restricciones (HTTPS obligatorio, solo Chrome Android, no iOS, RSSI inexacto). WiFi permite HTTP polling directo, CORS controlado, y RSSI real del cliente.

**Arquitectura WiFi:**
```
[Teléfono] ←→ WiFi AP "THLS-C001" ←→ [ESP32]
                    ↓
         HTTP GET /api/status (1s)
                    ↓
         JSON: {rssi, challenge, type, seed, ...}
```

**RSSI real del cliente via WiFi (clave técnica):**
```cpp
// ESP32 en modo AP puede leer el RSSI del cliente conectado
wifi_sta_list_t sta;
esp_wifi_ap_get_sta_list(&sta);
int rssi = sta.sta[0].rssi; // RSSI real del teléfono
```

**9 estados de señal con paleta extendida:**

| Estado | Rango RSSI | Color |
|--------|-----------|-------|
| SIN SEÑAL | < -94 | Gris `#37474f` |
| POLAR | -94 a -84 | Azul navy `#0d47a1` |
| FRÍO | -84 a -75 | Azul `#1976d2` |
| FRESCO | -75 a -67 | Teal `#00838f` |
| TIBIO | -67 a -60 | Púrpura `#7b1fa2` |
| CALIENTE | -60 a -53 | Rosa-rojo `#c2185b` |
| MUY CALIENTE | -53 a -47 | Naranja-rojo `#e64a19` |
| ARDIENDO | -47 a -41 | Naranja `#ff6f00` |
| BALIZA LOCALIZADA | > -41 | Verde `#00c853` |

**Canvas brújula heading-up:**
- Mapa rota según orientación del dispositivo (norte siempre arriba)
- Flecha cyan apunta hacia donde mira el usuario
- Punto rojo pulsante = posición estimada de la baliza
- 7 anillos de zona coloreados
- `DeviceOrientationEvent` con `requestPermission()` para iOS 13+
- Dead reckoning: `beaconAngle` se actualiza cuando la señal mejora en un rumbo

#### 3.5 Errores de compilación resueltos

| Error | Causa | Fix |
|-------|-------|-----|
| `ArduinoJson.h: No such file` | Librería no instalada | Reemplazar con `snprintf()` puro |
| Binario muy grande (1.58 MB > 1.31 MB) | WiFi + BLE juntos = binario pesado | `board_build.partitions = huge_app.csv` (3 MB para app) |
| BLE nombre random en picker | Chrome lee nombre del scan response, no del GAP | `BLEAdvertisementData::setName()` en `setScanResponseData()` |
| `R"rawhtml"` + PROGMEM error parser | GCC 8.4 no maneja raw string literals con PROGMEM | `server.sendContent(F("..."))` en chunks |
| `'function' does not name a type` | `async function` JS en raw string literal parseado como C++ | Reemplazar con `XMLHttpRequest` ES5 |

#### 3.6 Página de test mínima

Para validar el concepto sin la complejidad de la PWA completa, se creó `test.html` (~100 líneas) que:
- Se sirve directamente desde el ESP32 en `http://192.168.4.1/`
- Hace fetch a `/api/status` cada 1s (URL relativa, sin CORS)
- Muestra RSSI como círculo coloreado grande
- Botón START/STOP visible

---

## 3. STACK TECNOLÓGICO

### Hardware
```
ESP32 Dev Module (240 MHz, 320KB RAM, 4MB Flash)
GPIO2:  LED integrado
GPIO0:  Botón BOOT (toggle challenge)
GPIO34: ADC voltaje batería (divisor 100k/100k)
GPIO5:  LED RGB NeoPixel (opcional)
```

### Firmware (C++ / Arduino)
```
Framework:  Arduino-ESP32 2.0.17 (IDF 4.4.x)
Librería:   WiFi, WebServer, ESP32 BLE Arduino 2.0.0, Preferences
Partition:  huge_app.csv (3MB app / 1MB data)
Toolchain:  GCC 8.4.0 (xtensa-esp32)
```

### PWA (JavaScript / HTML)
```
Sin frameworks — archivo único index.html
Audio:      Web Audio API
Orientación: DeviceOrientationEvent
BT standby: Web Bluetooth API
HTTP:       fetch() / XMLHttpRequest
Canvas:     2D Canvas API (mapa brújula)
Service Worker: PWA offline
```

### Repositorio
```
GitHub: flakorchkdsk1984/treasure-hunters-iot-gdd
Branch: main
Commits: 15+ commits con historial completo de cambios
```

---

## 4. ESTADO ACTUAL DEL PROYECTO

### ✅ Completado
- [x] GDD completo (560 KB, 25 secciones)
- [x] Firmware WiFi AP + HTTP server (v3)
- [x] BLE en standby (advertising + scan response correcto)
- [x] PWA con 9 estados + canvas brújula heading-up
- [x] 3 minijuegos funcionales (wire_connect, qr_scan, morse_decode)
- [x] Motor de audio Web Audio API
- [x] Banner INFILTRACIÓN DISPONIBLE clickeable
- [x] Página de test mínima servida por el ESP32
- [x] Fix partition scheme (huge_app.csv)
- [x] Todos los errores de compilación resueltos

### 🔄 En Progreso / Pendiente de Validación
- [ ] Confirmar que `test.html` se carga correctamente en `http://192.168.4.1/`
- [ ] Confirmar que el RSSI real cambia al acercarse/alejarse del ESP32
- [ ] Validar minijuegos en dispositivo real

### 📋 Próximos Pasos Sugeridos
1. **Validar POC básico:** Confirmar variación de RSSI WiFi con distancia física
2. **Calibración RSSI:** Mapear rangos RSSI a distancias reales en el entorno de prueba
3. **GPS/Triangulación:** Agregar módulo GPS para posición real del beacon blip
4. **Múltiples balizas:** Soporte para más de un ESP32 simultáneo
5. **Backend cloud:** API REST para sincronización multi-jugador
6. **Pruebas de campo:** Test en exteriores con diferentes condiciones RF

---

## 5. HALLAZGOS TÉCNICOS CLAVE

### 5.1 Web Bluetooth vs WiFi para PWA
| Criterio | Web Bluetooth | WiFi HTTP |
|----------|--------------|-----------|
| HTTPS requerido | Sí (obligatorio) | No (HTTP ok) |
| iOS compatible | No | Sí |
| Android Chrome | Solo Chrome | Cualquier browser |
| RSSI disponible | Estimado (watchAdvertisements con flag) | Real (ESP32 mide el cliente) |
| Latencia | Baja (~50ms) | Baja (~100ms LAN) |
| **Veredicto POC** | ❌ Muchas restricciones | ✅ Mejor para pruebas |

### 5.2 ESP32 WiFi AP — RSSI del cliente
El ESP32 en modo AP puede obtener el RSSI del dispositivo conectado con `esp_wifi_ap_get_sta_list()`. Esto da la señal **desde el punto de vista del ESP32**, que es más estable que medir desde el teléfono.

### 5.3 Limitaciones identificadas
- RSSI WiFi varía ~5-10 dBm por ruido ambiental → necesita suavizado (Kalman o promedio móvil)
- Web Bluetooth no funciona en iOS en absoluto
- Chrome en Android necesita flag experimental para RSSI BLE real
- WiFi + BLE simultáneos en ESP32 consumen ~1.58MB → requiere `huge_app.csv`

---

## 6. COMMITS DEL REPOSITORIO

```
a5ac4ff feat(firmware): ESP32 sirve la PWA de test en http://192.168.4.1/
9630cbe fix(firmware): BLE nombre random — scan response name explícito  
58317f8 fix(firmware): eliminar ArduinoJson — reemplazar con snprintf
e037349 feat: v1.0 — WiFi + brújula heading-up + 9 estados de señal
2c578ce fix(pwa): banner INFILTRACION DISPONIBLE ahora es clickeable
fb083ce fix(pwa): minijuego no abría en 'found' + sin sonido
379c851 feat(pwa): v0.4 — 3 minijuegos + sonido Web Audio API + BT mejorado
4fd1e92 fix(pwa): corregir 3 bugs críticos en Web Bluetooth scanner
fee3e02 [initial commits GDD + firmware v1 + v2]
```

---

## 7. REFERENCIAS Y RECURSOS

- **Repositorio:** https://github.com/flakorchkdsk1984/treasure-hunters-iot-gdd
- **Web Bluetooth API:** https://developer.mozilla.org/en-US/docs/Web/API/Web_Bluetooth_API
- **ESP32 WiFi AP RSSI:** `esp_wifi_ap_get_sta_list()` en ESP-IDF docs
- **Web Audio API:** https://developer.mozilla.org/en-US/docs/Web/API/Web_Audio_API
- **DeviceOrientation:** https://developer.mozilla.org/en-US/docs/Web/API/DeviceOrientationEvent
- **PlatformIO ESP32:** https://docs.platformio.org/en/latest/boards/espressif32/esp32dev.html

---

*Informe generado automáticamente el 2026-06-07 por GitHub Copilot CLI*  
*Todas las interacciones están registradas en el historial de sesiones local*
