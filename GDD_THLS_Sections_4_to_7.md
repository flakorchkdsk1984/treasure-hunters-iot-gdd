# TREASURE HUNTERS IOT: THE LAST SIGNAL
## Game Design Document — Versión 1.0
### Secciones 4 a 7: Sistema BLE, Localización, Señal y Filtrado

---

> **Clasificación del documento:** Confidencial — Uso interno del equipo de desarrollo  
> **Autores:** Equipo de Arquitectura IoT & Game Design  
> **Fecha de revisión:** Junio 2026  
> **Estado:** Borrador técnico aprobado para POC

---

---

# SECCIÓN 4: SISTEMA DE LOCALIZACIÓN FÍSICA Y BLE

> *"La señal es tenue. La baliza existe en algún lugar de esta ciudad. Solo los cazadores más pacientes llegarán a encontrarla."*

---

## 4.1 — Análisis de Alcances por Entorno

El rendimiento del sistema BLE depende críticamente del entorno físico donde se despliegan las balizas. El factor de pérdida de ruta `n` es el parámetro más importante para modelar el comportamiento de la señal en cada escenario. A continuación se presenta el análisis completo por tipo de entorno:

---

### Tabla de Alcances por Entorno

| Entorno | Alcance Mín | Alcance Máx | Factor n | Varianza RSSI | Notas clave |
|---------|------------|------------|---------|---------------|-------------|
| Interior con muros gruesos (hormigón, sótanos) | 5 m | 20 m | 3.5 – 4.5 | ±12 dBm | Atenuación severa, reflexiones complejas |
| Oficina abierta (espacio diáfano, tabiques ligeros) | 15 m | 40 m | 2.5 – 3.5 | ±8 dBm | Interferencias de equipos WiFi, cuerpos humanos |
| Exterior urbano (calles con edificios altos) | 20 m | 80 m | 2.5 – 3.0 | ±10 dBm | Reflexiones en fachadas, multipath severo |
| Parque urbano (vegetación, terreno irregular) | 30 m | 80 m | 2.0 – 2.5 | ±7 dBm | Condiciones semi-ideales, humedad variable |
| Exterior abierto (campos, playas, explanadas) | 60 m | 200 m | 1.8 – 2.2 | ±5 dBm | Condiciones casi ideales, viento afecta mínimamente |
| Con antena optimizada (directiva externa, campo abierto) | 100 m | 300 m+ | 1.5 – 1.8 | ±4 dBm | Requiere hardware específico, uso en misiones épicas |

---

### 4.1.1 — Interior con Muros Gruesos (n = 3.5 – 4.5)

**Descripción del entorno:** Sótanos, aparcamientos subterráneos, almacenes industriales, bunkers históricos, iglesias antiguas, museos con muros de piedra.

**Colocación ideal de la baliza:**
- Altura: 1.5 a 2.5 metros sobre el suelo para minimizar atenuación por suelo y maximizar cobertura esférica.
- Alejada de estructuras metálicas (tuberías, armarios de distribución eléctrica) al menos 30 cm.
- Preferiblemente en una esquina elevada donde la señal pueda irradiarse libremente en dos hemisferios.
- Envolver la baliza en carcasa plástica resistente a la humedad (IP54 mínimo).

**Experiencia del jugador:**
El jugador experimenta una sensación de claustrofobia narrativa. La señal es errática, los estados de temperatura cambian bruscamente, y el radar parpadea con frecuencia en zonas muertas. Esto refuerza la ficción de que A.R.G.O.S. está activamente bloqueando la señal. La tensión es máxima porque no se puede predecir con facilidad la dirección de la baliza.

**Tiempo de búsqueda esperado:** 15 – 45 minutos para equipos de 2-3 jugadores con coordinación activa.

**Consideraciones anti-trampa:**
- En entornos cerrados es más difícil que un jugador externo haga relay del RSSI sin estar físicamente presente.
- Se recomienda activar validación de timestamp estricta (ventana ±3 minutos en lugar de ±5) para reducir superficie de ataque por captura y reenvío de paquetes BLE.
- Monitorizar si múltiples jugadores declaran éxito desde coordenadas GPS idénticas o imposibles para el entorno cerrado.

---

### 4.1.2 — Oficina Abierta (n = 2.5 – 3.5)

**Descripción del entorno:** Plantas diáfanas de oficinas, coworkings, centros comerciales, aeropuertos, estaciones de tren.

**Colocación ideal de la baliza:**
- Altura: 3 a 5 metros (encima de estanterías, sobre falsos techos accesibles, en vigas expuestas).
- Evitar proximidad a puntos de acceso WiFi (interferencia en 2.4 GHz compartido con BLE) — mantener separación mínima de 2 metros.
- En plantas diáfanas grandes, una sola baliza puede cubrir 20-30 metros de radio útil para gameplay.

**Experiencia del jugador:**
Experiencia de espionaje corporativo. El jugador se mueve entre estaciones de trabajo, columnas y salas de reuniones siguiendo la señal. La atmósfera recuerda a operaciones de infiltración en sedes enemigas. El ruido electromagnético del entorno añade variabilidad realista a la señal.

**Tiempo de búsqueda esperado:** 10 – 25 minutos.

**Consideraciones anti-trampa:**
- La densidad de personas en estos entornos puede crear patrones de RSSI reproducibles (el cuerpo humano atenúa ~3-8 dBm).
- El backend debe rechazar claims de captura si el delta temporal entre primer avistamiento y captura es menor a 60 segundos (prevención de Warbaling desde vehículo).

---

### 4.1.3 — Exterior Urbano (n = 2.5 – 3.0)

**Descripción del entorno:** Calles de ciudad, barrios históricos, plazas urbanas con edificios circundantes.

**Colocación ideal de la baliza:**
- Altura: 2 a 4 metros (semáforos, postes de luz, bancos de jardín, muros de piedra bajos).
- Orientación: la antena del ESP32 debe apuntar hacia arriba (antena omnidireccional PCB integrada) para maximizar cobertura hemisférica hacia el nivel del peatón.
- Protección: carcasa IP65 obligatoria, resistente a lluvia y vandalismos menores.

**Experiencia del jugador:**
El jugador se convierte en un agente encubierto que navega la ciudad con su dispositivo como único instrumento de detección. El ruido urbano (tráfico, otras señales BLE de dispositivos cotidianos) añade autenticidad a la ficción de "caza en territorio hostil".

**Tiempo de búsqueda esperado:** 20 – 40 minutos.

**Consideraciones anti-trampa:**
- Las reflexiones en fachadas pueden crear "fantasmas de señal" donde el jugador detecta un máximo de RSSI en una dirección incorrecta. El backend debe validar que la posición GPS declarada sea geofísicamente plausible respecto a la ubicación registrada de la baliza.
- Activar validación de nonce rotativo con ventana estrecha.

---

### 4.1.4 — Parque Urbano (n = 2.0 – 2.5)

**Descripción del entorno:** Parques metropolitanos, jardines botánicos, campus universitarios con zonas verdes.

**Colocación ideal de la baliza:**
- Altura: 1.5 a 3 metros (bancos de piedra, bases de estatuas, postes de señalización del parque).
- Aprovechar estructuras naturales (rocas grandes, troncos de árboles anchos) para camuflar la baliza y añadir elemento físico de búsqueda.
- Separar de árboles con follaje denso (la vegetación húmeda atenúa significativamente la señal).

**Experiencia del jugador:**
Las misiones en parques ofrecen la combinación ideal de exploración física (el jugador se mueve activamente por el terreno) y tensión de señal. La variabilidad del RSSI en vegetación crea una experiencia de "radar analógico" auténtica.

**Tiempo de búsqueda esperado:** 15 – 35 minutos.

**Consideraciones anti-trampa:**
- Los parques son vulnerables a ataques de "drone scout" donde un jugador malicioso sobrevuela la zona para mapear la señal desde el aire. Mitigación: el campo de aplicación debe requerir que el GPS del jugador permanezca a nivel del suelo (altitud razonable).

---

### 4.1.5 — Exterior Abierto (n = 1.8 – 2.2)

**Descripción del entorno:** Campos deportivos, playas, explanadas industriales, aeródromos, zonas portuarias.

**Colocación ideal de la baliza:**
- Altura: 1 a 2 metros. En exteriores abiertos la señal se propaga de forma casi esférica sin obstáculos.
- Usar postes o trípodes ligeros para misiones temporales (eventos).
- Enterrar parcialmente el enclosure para ocultamiento físico adicional.

**Experiencia del jugador:**
La señal se detecta desde muy lejos, pero la precisión del RSSI es insuficiente para navegar directamente. El jugador debe realizar barridos en ángulo para triangular la dirección. Esta experiencia de "orientación sin mapa" es la más cinematográfica del juego.

**Tiempo de búsqueda esperado:** 25 – 60 minutos.

**Consideraciones anti-trampa:**
- En exteriores abiertos, el GPS del dispositivo tiene buena precisión. El backend puede realizar validación geoespacial estricta: si el jugador declara captura a más de 15 metros de las coordenadas registradas de la baliza, la reclamación es sospechosa.

---

### 4.1.6 — Con Antena Optimizada (Alcance 300 m+)

**Descripción del entorno:** Exclusivo para misiones épicas de alto nivel (Clase Omega, Clase S). Requiere ESP32 con antena externa direccional u omnidireccional de alta ganancia.

**Colocación ideal de la baliza:**
- Puntos elevados: torres, tejados de edificios, miradores.
- Antena external de alta ganancia (3-9 dBi) para maximizar EIRP dentro de los límites regulatorios del país de despliegue.

**Experiencia del jugador:**
Misiones de alcance extendido reservadas para cacerías épicas de facción. Un equipo de hasta 10 jugadores coordina posiciones distribuidas en un área amplia para triangular la señal usando comunicación interna.

**Tiempo de búsqueda esperado:** 60 – 180 minutos (misiones de evento especial).

**Consideraciones anti-trampa:**
- Requiere validación adicional: los jugadores deben registrar su posición GPS en tiempo real durante la misión. Cualquier salto de posición imposible (>50 km/h a pie) invalida la sesión.

---

## 4.2 — Recomendaciones para el POC (Proof of Concept)

### Potencia BLE Recomendada: +6 dBm

Para el POC se recomienda configurar el ESP32 a una potencia de transmisión de **+6 dBm** por las siguientes razones técnicas y de gameplay:

| Parámetro | Valor a +6 dBm | Valor a 0 dBm | Valor a -12 dBm |
|-----------|--------------|--------------|----------------|
| Alcance efectivo (parque) | 40 – 60 m | 20 – 35 m | 8 – 15 m |
| Consumo adicional vs 0 dBm | +15 mA | baseline | -8 mA |
| Resolución de estados fría-caliente | Óptima (6 estados diferenciados) | Reducida (4 estados útiles) | Mínima (2 estados) |
| Susceptibilidad a colisión con otros BLE | Media | Baja | Muy baja |
| Viabilidad para misiones en parque | ✅ Excelente | ⚠️ Marginal | ❌ Insuficiente |

**Justificación de gameplay:** El rango de 40-60 metros crea la "zona de caza" óptima. El jugador puede detectar la baliza desde suficiente distancia para empezar la navegación por señal, pero no tan lejos como para que sea trivial. Este rango permite el ciclo completo de juego:

```
Detección lejana (FRÍO)  →  Aproximación activa (TIBIO)  →  Búsqueda local (CALIENTE)  →  Captura (LOCALIZADO)
      ~50m                          ~30m                           ~10m                         <5m
```

**Justificación técnica:** A +6 dBm se maximiza la diferenciación entre estados de temperatura con el rango de RSSI disponible (-40 a -100 dBm). Cada estado ocupa un rango de ~10 dBm, lo que corresponde a aproximadamente un factor 2x en distancia, proporcionando retroalimentación perceptible y consistente al jugador.

### Configuración de TX Power en ESP32

```cpp
// Configurar potencia de transmisión BLE en ESP32
#include "esp_bt.h"
#include "esp_gap_ble_api.h"

void setBLETxPower() {
    // ESP_PWR_LVL_P9  = +9 dBm (máximo, solo misiones épicas)
    // ESP_PWR_LVL_P6  = +6 dBm (recomendado para POC)
    // ESP_PWR_LVL_P3  = +3 dBm
    // ESP_PWR_LVL_N0  =  0 dBm (default del SDK)
    // ESP_PWR_LVL_N3  = -3 dBm
    // ESP_PWR_LVL_N6  = -6 dBm
    // ESP_PWR_LVL_N9  = -9 dBm
    // ESP_PWR_LVL_N12 = -12 dBm

    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_ADV, ESP_PWR_LVL_P6);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_SCAN, ESP_PWR_LVL_P6);
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, ESP_PWR_LVL_P6);
}
```

> ⚠️ **Nota regulatoria:** En la Unión Europea, la directiva ETSI EN 300 328 limita el EIRP BLE a 10 dBm (10 mW). Con la antena PCB integrada del ESP32 (~0 dBi de ganancia), operar a +6 dBm está dentro de los límites legales. Con antena externa, verificar que el EIRP total no supere el límite regional.

---

## 4.3 — Ventajas y Desventajas por Rango

### Rango Corto: 5 – 20 m

| Dimensión | Detalle |
|-----------|---------|
| ✅ **Precisión** | Alta fidelidad en la estimación de proximidad final. Ideal para el momento de "encontrar" la baliza. |
| ✅ **Entornos interiores** | Funciona bien en espacios cerrados donde el largo alcance causaría problemas de interferencia. |
| ✅ **Consumo energético** | La potencia baja (-12 a -6 dBm) extiende la vida de la batería considerablemente. |
| ✅ **Privacidad del operador** | La baliza solo es detectable a muy corta distancia, dificultando el mapeo no autorizado. |
| ❌ **Demasiado fácil** | La búsqueda se reduce a un área muy pequeña. Un jugador que llegue al punto general encuentra la baliza casi de inmediato, eliminando la tensión. |
| ❌ **Sin fase de aproximación** | Se pierde la fase de navegación activa que es el corazón del gameplay. La transición es: nada → encontrado. |
| ❌ **Dependencia de pistas externas** | Requiere hints adicionales (coordenadas aproximadas, fotografías) para llevar al jugador a la zona de detección. |

**Caso de uso recomendado:** Final de puzzles multi-etapa donde la última baliza es la recompensa de un proceso largo. También útil para balizas "trampa" o falsas pistas.

---

### Rango Medio: 20 – 80 m

| Dimensión | Detalle |
|-----------|---------|
| ✅ **Balance gameplay** | Rango suficiente para navegación activa sin trivializar la búsqueda. El jugador experimenta todas las fases del ciclo de caza. |
| ✅ **Compatibilidad de entornos** | Funciona correctamente tanto en parques urbanos como en interiores grandes. |
| ✅ **Retroalimentación gradual** | Los 6 estados de temperatura tienen diferenciación perceptible y útil en este rango. |
| ✅ **Múltiples jugadores** | Varios cazadores pueden operar en la misma zona sin saturar el espectro. |
| ❌ **Interferencias urbanas** | Otros dispositivos BLE (auriculares, wearables, beacons de marketing) pueden crear ruido de fondo. Mitigación: UUID propietario. |
| ❌ **Variabilidad por entorno** | El mismo rango puede comportarse como "corto" en interiores densos o como "largo" en exteriores abiertos. Requiere calibración por ubicación. |

**Caso de uso recomendado:** Rango estándar para el 80% de las misiones del juego. Óptimo para el POC.

---

### Rango Largo: 100 m+

| Dimensión | Detalle |
|-----------|---------|
| ✅ **Épica outdoor** | Las misiones en exteriores abiertos adquieren dimensión cinematográfica. El jugador siente la vastedad de la búsqueda. |
| ✅ **Misiones de facción** | Permite coordinación de equipos grandes con roles diferenciados (navegadores, exploradores, guardas). |
| ✅ **Visibilidad de eventos** | Durante eventos en vivo, los espectadores pueden detectar balizas de exhibición desde lejos. |
| ❌ **Consumo de batería** | TX a +9 dBm aumenta el consumo activo hasta un 40% respecto a 0 dBm. Requiere baterías de mayor capacidad (18650 de 3000+ mAh). |
| ❌ **Precisión final** | A larga distancia, el gradiente de RSSI en los últimos metros es muy pequeño, haciendo difícil localizar la baliza exacta. |
| ❌ **Dependencia de GPS** | El backend necesita validación GPS adicional para anti-cheat a estas distancias. |
| ❌ **Regulatorio** | Potencias altas con antenas externas pueden requerir notificación regulatoria según el país. |

**Caso de uso recomendado:** Misiones Clase Omega, eventos especiales de temporada, hunts de facción con premios físicos.

---

## 4.4 — Consideraciones de Despliegue Físico

### 4.4.1 — Guía para Operadores de Baliza (Administradores de Comunidad)

Los operadores de baliza ("Guardians" en la ficción del juego) son los responsables de desplegar y mantener las balizas físicas. Deben seguir este protocolo:

**Proceso de despliegue:**

```
1. SELECCIÓN DE UBICACIÓN
   ├── Verificar que la zona sea de acceso público legal
   ├── Confirmar que no obstaculiza vías de emergencia
   ├── Evaluar visibilidad: la baliza debe ser ENCONTRABLE pero no OBVIA
   └── Fotografiar la ubicación antes del despliegue (evidencia para soporte)

2. REGISTRO EN LA PLATAFORMA
   ├── Escanear QR del ESP32 para vincular BeaconID al despliegue
   ├── Introducir coordenadas GPS precisas (precisión < 5m)
   ├── Definir radio de validación (geofence) para anti-cheat
   └── Configurar horario de actividad (si aplica)

3. INSTALACIÓN FÍSICA
   ├── Asegurar la baliza con brida de plástico, velcro industrial, o adhesivo de doble cara
   ├── Orientar la antena hacia arriba o lateralmente (nunca contra el suelo)
   ├── Verificar que la carcasa esté cerrada herméticamente
   └── Confirmar señal de test desde 20m con la app de gestión

4. VERIFICACIÓN FINAL
   ├── Escanear la baliza con el modo diagnóstico de la app
   ├── Confirmar que el RSSI a 5m es entre -40 y -55 dBm
   ├── Registrar nivel de batería inicial en el dashboard
   └── Activar la misión en la plataforma
```

### 4.4.2 — Directrices de Seguridad para Espacios Públicos

| Escenario | Regla | Razón |
|-----------|-------|-------|
| Colocación a menos de 50 cm del suelo | ❌ Prohibido | Riesgo de tropiezo y vandalismo por niños |
| Colocación en infraestructura eléctrica | ❌ Prohibido | Riesgo eléctrico y posible interacción EM adversa |
| Colocación en zonas con restricción de acceso (privado, militar, etc.) | ❌ Prohibido | Ilegal. Violación de propiedad privada |
| Colocación en monumentos históricos (pegamento) | ❌ Prohibido | Daño patrimonial |
| Colocación en entornos con riesgo de caída sobre personas | ❌ Prohibido | Responsabilidad civil |
| Colocación en exteriores sin carcasa IP65 | ⚠️ Desaconsejado | La lluvia y humedad dañarán el hardware |
| Baliza en parque con presencia de niños | ⚠️ Requiere aprobación municipal | Considera implicaciones de privacidad y seguridad infantil |

### 4.4.3 — Resistencia al Agua y Condiciones Climáticas

**Clasificación IP recomendada por entorno:**

| Entorno | IP Mínimo | IP Recomendado | Observaciones |
|---------|-----------|---------------|---------------|
| Interior seco (oficina) | IP20 | IP40 | Solo polvo |
| Interior húmedo (bodega, parking) | IP44 | IP54 | Salpicaduras |
| Exterior protegido (bajo marquesina) | IP54 | IP65 | Lluvia directa posible |
| Exterior expuesto (parque, playa) | IP65 | IP67 | Lluvia intensa, inmersión accidental |
| Exterior extremo (montaña, playa con olas) | IP67 | IP68 | Inmersión prolongada |

**Recomendación de carcasa para el POC:**
Cajas de proyecto ABS con juntas de silicona, clasificación IP65, de 100x60x35mm. El ESP32 se monta en el interior con la antena PCB orientada hacia el lado más delgado de la caja. El conector de carga USB-C se sella con un tapón de caucho cuando no está en uso.

### 4.4.4 — Vida de Batería y Modos de Ahorro del ESP32

El ESP32 tiene tres modos de consumo relevantes para el despliegue de balizas:

| Modo ESP32 | Consumo Típico | BLE Activo | Tiempo con 2000 mAh | Uso en el juego |
|-----------|---------------|------------|--------------------|----|
| Activo (TX +6 dBm) | 80 – 130 mA | ✅ Sí | 15 – 25 horas | Solo durante eventos en vivo |
| Modem Sleep | 20 – 30 mA | ✅ Sí (ciclos) | 65 – 100 horas | Modo normal de operación |
| Light Sleep | 0.8 – 1.5 mA | ⚠️ Limitado | 55 – 104 días | Balizas de baja actividad |
| Deep Sleep | 10 – 150 µA | ❌ No | >6 meses | Wake periódico para advertise |
| Hibernation | 2.5 µA | ❌ No | >2 años | Solo almacenamiento |

**Estrategia de ahorro de energía por clase de baliza:**

```
Clase C (Común) — Modem Sleep con intervalos de advertising 500ms
  └── Ciclo: Advertise 200ms → Sleep 300ms → repeat
  └── Consumo promedio: ~25 mA → Batería 18650 3000mAh → ~5 días

Clase B (Raro) — Light Sleep con wake cada 5 segundos
  └── Ciclo: Wake → Advertise 3 × 100ms → Deep sleep 4.9s
  └── Consumo promedio: ~3 mA → Batería 18650 3000mAh → ~40 días

Clase A (Épico) — Deep Sleep con wake controlado por RTC
  └── Ciclo: Wake cada 10s → Advertise 500ms → Sleep 9.5s
  └── Consumo promedio: ~0.5 mA → Batería 18650 3000mAh → >250 días

Clase Omega (Legendario) — Event-triggered, normalmente dormido
  └── Wake por señal RF externa o programación NTP
  └── Ciclo: Activo solo durante ventana de evento (2-4 horas)
  └── Batería 18650 dura prácticamente toda la temporada
```

### 4.4.5 — Calendario de Mantenimiento

| Frecuencia | Tarea | Responsable |
|------------|-------|-------------|
| Semanal | Verificación remota de nivel de batería (dashboard) | Sistema automático |
| Mensual | Revisión física de integridad de la carcasa | Operador local |
| Mensual | Rotación de nonce secreto compartido | Administrador de backend |
| Trimestral | Actualización de firmware OTA (Over The Air via BLE) | Equipo técnico |
| Semestral | Reemplazo preventivo de baterías (Clase C y B) | Operador local |
| Anual | Revisión de certificación regulatoria del hardware | Equipo legal/técnico |
| Tras incidente | Auditoría de logs de anti-cheat, reset de beaconID si comprometido | Security team |

---

---

# SECCIÓN 5: ARQUITECTURA BLE COMPLETA

> *"El ESP32 es el corazón que late en las sombras. Invisible al ojo, inaudible al oído, pero presente en las ondas que atraviesan cada muro de la ciudad."*

---

## 5.1 — Diseño del Hardware de la Baliza ESP32

### 5.1.1 — Selección del Microcontrolador

Se analizan dos opciones principales para el hardware de la baliza:

| Parámetro | ESP32-WROOM-32 | ESP32-C3-MINI-1 | Recomendación POC |
|-----------|---------------|-----------------|-------------------|
| Arquitectura CPU | Xtensa LX6 dual-core 240 MHz | RISC-V 160 MHz single-core | WROOM para POC |
| BLE versión | 4.2 | 5.0 | C3 para producción |
| WiFi | 802.11 b/g/n | 802.11 b/g/n | Ambos |
| Consumo Deep Sleep | 10 µA | 5 µA | C3 más eficiente |
| Tamaño módulo | 18×20 mm | 13.2×16.6 mm | C3 más compacto |
| Precio unitario (aprox.) | €2.50 – €4.00 | €1.80 – €3.00 | C3 más económico |
| Disponibilidad | Alta (muy común) | Media-alta | WROOM más disponible |
| Soporte comunidad | Excelente | Bueno | WROOM superior |
| BLE Long Range (Coded PHY) | ❌ No | ✅ Sí | C3 para misiones épicas |
| Antena | PCB integrada | PCB integrada + ext opcional | C3 más flexible |

**Recomendación para el POC:** **ESP32-WROOM-32** por su amplia disponibilidad, excelente soporte de la comunidad, y la madurez del ecosistema de Arduino/ESP-IDF. La familia WROOM tiene miles de proyectos de referencia y librerías estables.

**Recomendación para producción (v1.0):** **ESP32-C3** por su menor consumo en deep sleep (crítico para balizas clase B y A), soporte BLE 5.0 con Coded PHY (alcance extendido sin aumentar potencia), y menor coste de fabricación en volumen.

### 5.1.2 — Consideraciones BLE 5.0 vs BLE 4.2

| Característica | BLE 4.2 | BLE 5.0 | Impacto en el juego |
|---------------|---------|---------|---------------------|
| Velocidad máxima data rate | 1 Mbps | 2 Mbps | Mínimo impacto (solo advertising) |
| Long Range (Coded PHY) | ❌ | ✅ | Misiones épicas de 300 m+ |
| Extended Advertising | ❌ | ✅ | Payloads más grandes (255 → 1650 bytes) |
| Advertising Sets múltiples | ❌ | ✅ | Múltiples canales de advertising simultáneos |
| Mejora de privacidad | Básica | Mejorada | Protección de tracking del hardware |
| Consumo en Coded PHY | — | +30% vs 1M PHY | Trade-off para largo alcance |

**Conclusión:** Para el POC con ESP32-WROOM-32 (BLE 4.2), el payload de advertising de 31 bytes es suficiente con la serialización optimizada del paquete THLS. En la versión de producción con C3, se aprovechará el Extended Advertising de BLE 5.0 para incluir el payload completo sin necesidad de truncar campos.

### 5.1.3 — Opciones de Batería

| Tipo | Capacidad | Voltaje | Dimensiones | Vida estimada (Modem Sleep) | Recomendación |
|------|-----------|---------|-------------|----------------------------|---------------|
| 18650 Li-Ion (simple) | 2000-3500 mAh | 3.7V nominal | 18×65 mm | 3 – 6 días (activo) / 30-60 días (clase B) | ✅ Mejor opción para producción |
| 18650 Li-Ion (dual en paralelo) | 4000-7000 mAh | 3.7V nominal | 18×65 mm ×2 | 6 – 12 días (activo) / 60-120 días (clase B) | ✅ Para balizas clase A en campo |
| LiPo 103450 | 1800-2500 mAh | 3.7V nominal | 10×34×50 mm | Variable según capacidad | ✅ Para carcasas ultra-compactas |
| LiPo 606090 | 4000-5000 mAh | 3.7V nominal | 6×60×90 mm | Similar a 18650 dual | ⚠️ Requiere BMS protegido |
| AA Alcalinas × 3 | ~2400 mAh @ 3.6V | 4.5V (necesita regulador) | Estándar AA | 2 – 4 días (activo) | ❌ No recomendado (voltaje irregular) |
| AA Litio × 3 (Energizer L91) | ~3000 mAh @ 3.6V | 4.5V constante | Estándar AA | 3 – 6 días (activo) | ⚠️ Aceptable para POC rápido |

**Recomendación definitiva:** **18650 Li-Ion 3000 mAh con BMS integrado** (circuito de protección contra sobrecarga, sobredescarga y cortocircuito). Usar portapilas 18650 con conector JST para facilitar el reemplazo en campo sin herramientas.

### 5.1.4 — Recomendaciones de Carcasa

```
Carcasa recomendada para POC:
┌─────────────────────────────────────────────┐
│  Modelo: Hammond 1554B2GY                   │
│  Material: ABS resistente a UV              │
│  Dimensiones: 120 × 65 × 40 mm             │
│  Clasificación: IP65                        │
│  Juntas: Silicona perimetral                │
│  Puntos de fijación: 4 orificios M4         │
│  Color: Gris oscuro (bajo perfil visual)    │
└─────────────────────────────────────────────┘

Layout interno:
┌──────────────────────────────────┐
│  [ESP32-WROOM]  [18650 Holder]  │
│  [BMS circuit]  [Power switch]   │
│  [Charge port - USB-C sealed]    │
└──────────────────────────────────┘
```

### 5.1.5 — Opciones de Antena

| Tipo de Antena | Ganancia | Costo | Tamaño | Direccionalidad | Uso recomendado |
|---------------|---------|-------|--------|-----------------|-----------------|
| PCB integrada (ESP32 onboard) | ~0 dBi | €0 (incluida) | 0 extra | Omnidireccional | ✅ POC y balizas estándar |
| Chip antenna (SMD) | -1 a +1 dBi | €0.50 | Muy pequeño | Semi-omni | ✅ Versión compacta de producción |
| Antena de varilla flexible (IPEX/U.FL) | +2 a +3 dBi | €1-2 | 50-100mm | Omnidireccional | ✅ Mejora de cobertura en carcasa metálica |
| Antena externa rosca SMA | +3 a +5 dBi | €3-8 | 100-200mm | Omnidireccional | ⚠️ Solo balizas de exterior fijas |
| Antena panel exterior | +6 a +9 dBi | €15-40 | 15×15 cm | Direccional | ⚠️ Solo misiones épicas, requiere orientación cuidadosa |

**Nota importante:** La antena PCB integrada del ESP32-WROOM está optimizada para el módulo y es la opción más equilibrada para el 90% de los casos de uso del juego.

---

## 5.2 — Diseño del Firmware de la Baliza

### 5.2.1 — Máquina de Estados del Beacon

```cpp
/**
 * THLS Beacon Firmware v1.0
 * Máquina de estados principal
 */

enum BeaconState {
    STATE_BOOT,         // Inicialización del hardware
    STATE_NORMAL,       // Advertising estándar (modo base)
    STATE_CHALLENGE,    // Jugador en proximidad, modo desafío activo
    STATE_LOCKED,       // Lockout anti-cheat activo
    STATE_LOW_BATTERY,  // Batería por debajo del umbral crítico (<15%)
    STATE_EVENT,        // Modo evento especial (misión de temporada)
    STATE_MAINTENANCE,  // Modo diagnóstico (solo para operadores)
    STATE_OTA           // Actualización de firmware en curso
};

/**
 * Transiciones de estado:
 *
 *                    ┌─────────────┐
 *                    │  STATE_BOOT │
 *                    └──────┬──────┘
 *                           │ Boot OK
 *                    ┌──────▼──────┐
 *          ┌─────────┤STATE_NORMAL ├─────────┐
 *          │         └──────┬──────┘         │
 *          │                │                │
 *    Low Battery      Player scan     Event trigger
 *          │         detected 3x      from backend
 *          ▼                ▼                ▼
 *   ┌────────────┐  ┌────────────┐   ┌────────────┐
 *   │STATE_LOW_  │  │STATE_CHAL- │   │STATE_EVENT │
 *   │BATTERY     │  │LENGE       │   │            │
 *   └─────┬──────┘  └─────┬──────┘   └─────┬──────┘
 *         │               │                │
 *    Charged OK      Cheat detected    Event ends
 *         │               │                │
 *         ▼               ▼                ▼
 *   [STATE_NORMAL]  ┌────────────┐   [STATE_NORMAL]
 *                   │STATE_LOCKED│
 *                   └─────┬──────┘
 *                         │ Lockout expires (30 min)
 *                         ▼
 *                  [STATE_NORMAL]
 */
```

**Descripción detallada de cada estado:**

| Estado | Descripción | Intervalo Advertising | Potencia TX | LED indicator |
|--------|------------|----------------------|-------------|---------------|
| `STATE_BOOT` | Inicialización: WiFi para sincronizar tiempo NTP, carga de config de flash | N/A | N/A | Blinking rápido |
| `STATE_NORMAL` | Operación estándar. Advertising pasivo esperando cazadores. | 500 ms | Configurada por clase | Latido lento (2s) |
| `STATE_CHALLENGE` | Un jugador está en proximidad y hay un desafío activo pendiente. Advertising acelerado. | 100 ms | +6 dBm forzado | Pulso rápido |
| `STATE_LOCKED` | Anti-cheat ha detectado actividad sospechosa. No responde a escaneos de captura. | 500 ms | -12 dBm (reducida) | Rojo fijo |
| `STATE_LOW_BATTERY` | Batería crítica. Sigue operando pero el dashboard alerta al operador. | 1000 ms | -6 dBm (reducida) | Naranja intermitente |
| `STATE_EVENT` | Modo de evento especial. El payload lleva indicadores adicionales de misión de temporada. | 200 ms | +9 dBm si aplica | Color de facción |
| `STATE_MAINTENANCE` | Modo diagnóstico activado por el operador. Muestra información técnica en BLE. | 1000 ms | 0 dBm | Azul parpadeante |
| `STATE_OTA` | Recibiendo actualización de firmware. BLE advertising pausado temporalmente. | N/A | N/A | Blanco fijo |

### 5.2.2 — Especificación Completa del Payload BLE

El payload BLE de THLS se transmite como datos de fabricante específico (Manufacturer Specific Data, tipo 0xFF) dentro del ADV_IND estándar de BLE 4.2.

**Estructura JSON del payload (forma legible — se serializa a binario en el firmware):**

```json
{
  "v": 1,
  "beaconId": "A102",
  "class": "A",
  "nonce": "9X7AB3",
  "ts": "2026-06-04T10:00:00Z",
  "challenge": false,
  "challengeType": null,
  "difficulty": null,
  "seed": null,
  "faction": "ARGOS",
  "region": "EU-W-04",
  "sig": "HMAC-SHA256-TRUNCATED-16BYTES"
}
```

**Descripción de cada campo y su rol anti-trampa:**

| Campo | Tipo | Bytes | Descripción | Rol Anti-trampa |
|-------|------|-------|-------------|-----------------|
| `v` | uint8 | 1 | Versión del protocolo THLS. Actualmente v1. | Permite invalidar payloads de versiones antiguas capturadas y almacenadas. |
| `beaconId` | string | 4 | Identificador único de la baliza (alfanumérico). Ej: "A102" = clase A, número 102. | El backend verifica que este ID corresponde a una baliza registrada y activa. |
| `class` | char | 1 | Clase de rareza: C (común), B (raro), A (épico), S (legendario), Ω (omega). | Determina los beneficios al capturar. No modificable sin invalidar la firma. |
| `nonce` | string | 6 | Valor pseudoaleatorio que cambia cada 5 minutos. Generado por RNG interno seeded con timestamp. | Prevención de ataques de replay: un nonce ya visto es rechazado por el backend. |
| `ts` | ISO8601 | 8* | Timestamp de creación del payload (UTC). *Comprimido a 4 bytes epoch en el payload binario. | El backend rechaza payloads con timestamp fuera de la ventana de ±5 minutos. |
| `challenge` | bool | 1 | Indica si hay un desafío activo asociado a la baliza. | Si `true`, la captura requiere completar el desafío. No falsificable sin invalidar `sig`. |
| `challengeType` | string | 2* | Tipo de desafío: `QZ` (quiz), `CR` (crypto), `PZ` (puzzle físico), `PR` (prueba en persona). | Define el flujo de resolución en la app. Modificarlo falsificado invalida la firma. |
| `difficulty` | uint8 | 1 | Nivel de dificultad del desafío: 1-5. | Afecta la puntuación y las recompensas. Protegido por firma. |
| `seed` | uint32 | 4 | Semilla para generación procedural del desafío específico. El mismo beaconId + seed siempre genera el mismo desafío. | Previene que un jugador comparta la "respuesta" de hoy a otro jugador en otra región. |
| `faction` | string | 3* | Facción propietaria de la baliza: `ARG` (ARGOS), `PSX` (Psion), `FRX` (FrexNet). | Afecta la narrativa y el sistema de control territorial. |
| `region` | string | 6* | Código de región geográfica del despliegue. Ej: `EU-W-04` = Europa Occidental, zona 4. | El backend valida coherencia entre región declarada y GPS del jugador. |
| `sig` | bytes | 16 | HMAC-SHA256 truncado a 16 bytes. Calculado sobre todos los campos anteriores con la clave secreta del beacon. | **Campo anti-trampa principal.** Sin la clave secreta del beacon, es computacionalmente imposible generar una firma válida. |

**Representación binaria del payload (31 bytes disponibles en BLE 4.2 ADV_IND):**

```
Byte 0     : Versión de protocolo (v=1)
Byte 1-4   : BeaconID codificado en base36 (4 bytes)
Byte 5     : Clase (1 byte, enum)
Byte 6-11  : Nonce (6 bytes, alfanumérico)
Byte 12-15 : Timestamp epoch UTC (uint32, 4 bytes)
Byte 16    : Flags: [challenge:1][challengeType:3][difficulty:3][unused:1]
Byte 17-20 : Seed para desafío (uint32, 4 bytes) — 0x00000000 si no hay desafío
Byte 21    : Faction enum (1 byte)
Byte 22    : Region code (1 byte, lookup table en backend)
Byte 23-30 : HMAC-SHA256 truncado (8 bytes) — primeros 8 bytes del digest de 32 bytes
             [En BLE 5.0 Extended Advertising, se usarán los 16 bytes completos]

Total: 31 bytes ✅ (límite exacto de BLE 4.2 Manufacturer Specific Data)
```

### 5.2.3 — Parámetros de Advertising

**Intervalo de advertising:**

| Modo | Intervalo | Justificación |
|------|-----------|---------------|
| `STATE_NORMAL` | 500 ms | Balance óptimo entre detectabilidad y consumo. El scanner puede detectar la baliza en su primer ciclo de escaneo de 500ms. Consumo de ~25 mA en promedio. |
| `STATE_CHALLENGE` | 100 ms | Cuando un jugador está en proximidad, la latencia de detección importa para la UX. 100ms asegura que el scanner de la app recibe el beacon en cada barrido. Consumo de ~80 mA — aceptable por periodos cortos. |
| `STATE_LOW_BATTERY` | 1000 ms | Reducir el trabajo del transmisor para maximizar el tiempo restante de operación antes de que el operador reemplace la batería. |
| `STATE_LOCKED` | 500 ms | La baliza sigue siendo visible (para no eliminar la experiencia de otros jugadores) pero el payload lleva `STATE_LOCKED` que la app interpreta como no capturable. |
| `STATE_EVENT` | 200 ms | Los eventos especiales justifican mayor consumo para asegurar que todos los participantes detecten la baliza de evento con mínima latencia. |

**Advertising connectable vs. no connectable:**

Se usa **Non-Connectable Undirected Advertising (ADV_NONCONN_IND)** para todas las balizas THLS. Razones:

1. **Seguridad:** Sin conexión BLE, no hay superficie de ataque para modificar el firmware remotamente o extraer la clave HMAC.
2. **Eficiencia energética:** El advertising no connectable consume significativamente menos que mantener conexiones abiertas.
3. **Escalabilidad:** Cientos de jugadores pueden detectar la misma baliza simultáneamente sin saturar los recursos de conexión del ESP32.
4. **Anti-tamper:** Un atacante no puede usar BLE para interactuar con el beacon y modificar su comportamiento.

La única excepción es `STATE_MAINTENANCE`, donde se habilita advertising connectable con autenticación para que el operador actualice la configuración vía app de gestión BLE.

**Por qué Manufacturer Specific Data en lugar de iBeacon/Eddystone:**

| Formato | Payload útil | Personalización | Compatibilidad app | Uso en THLS |
|---------|-------------|-----------------|-------------------|-------------|
| iBeacon (Apple) | 20 bytes (UUID+Major+Minor) | Limitada a 3 campos | Requiere app nativa en iOS | ❌ Insuficiente |
| Eddystone-UID (Google) | 16 bytes | Limitada | Web Bluetooth parcial | ❌ Insuficiente |
| Eddystone-URL | Variable (URL) | Solo URL | Limitada | ❌ No aplica |
| **Manufacturer Specific Data** | **31 bytes libres** | **Total libertad** | **Web Bluetooth completo** | **✅ Usado en THLS** |

Con Manufacturer Specific Data y un Company ID propietario (registrado en Bluetooth SIG o usando un ID de desarrollo 0xFFFF para el POC), el payload de 31 bytes da libertad total para el protocolo THLS. La app filtra exclusivamente por Company ID para ignorar todos los demás dispositivos BLE del entorno.

---

### 5.2.4 — Protección contra Ataques de Replay

El sistema THLS implementa tres capas de protección contra replay attacks:

**Capa 1 — HMAC-SHA256 con clave por beacon:**

```
Firma = HMAC-SHA256(
    key    = SECRET_KEY[beaconId],  // Clave de 256 bits única por beacon, almacenada en flash cifrado
    message = v || beaconId || class || nonce || ts || challenge || challengeType ||
              difficulty || seed || faction || region
)[0:8]  // Primeros 8 bytes del digest (64 bits de seguridad)
```

La clave secreta se genera en fábrica, se almacena en el ESP32 usando la partición NVS cifrada (ESP32 Flash Encryption), y se registra en el backend en el momento del aprovisionamiento del hardware. **Nunca viaja en el payload BLE.**

**Capa 2 — Nonce rotativo con ventana temporal:**

```python
# Pseudo-código de validación en el backend
def validate_nonce(beacon_id, nonce, timestamp):
    # El nonce es función del timestamp: cambia cada 5 minutos
    expected_nonce = generate_nonce(
        secret=BEACON_SECRETS[beacon_id],
        time_window=floor(timestamp / 300)  # Ventana de 300s = 5 min
    )

    # Aceptar la ventana actual y la anterior (tolerancia de ±5 min para
    # diferencias de reloj y latencia de red)
    valid_windows = [
        generate_nonce(secret, floor(timestamp / 300) - 1),
        generate_nonce(secret, floor(timestamp / 300)),
        generate_nonce(secret, floor(timestamp / 300) + 1)
    ]

    if nonce not in valid_windows:
        raise ReplayAttackException(f"Nonce {nonce} fuera de ventana temporal válida")

    # Verificar que este nonce no ha sido ya usado (anti double-spend)
    if nonce_cache.exists(beacon_id, nonce):
        raise ReplayAttackException(f"Nonce {nonce} ya utilizado")

    nonce_cache.store(beacon_id, nonce, ttl=600)  # TTL = 10 min
    return True
```

**Capa 3 — Validación de firma en el backend:**

```python
def validate_claim(claim):
    # 1. Validar timestamp (±5 min)
    if abs(time.now() - claim.ts) > 300:
        return reject("Timestamp fuera de ventana")

    # 2. Recuperar clave del beacon
    secret_key = vault.get_beacon_key(claim.beaconId)
    if not secret_key:
        return reject("Beacon ID desconocido")

    # 3. Reconstruir firma esperada
    message = serialize_payload(claim)
    expected_sig = hmac_sha256(secret_key, message)[:8]

    # 4. Comparar con constant-time comparison (previene timing attacks)
    if not constant_time_compare(claim.sig, expected_sig):
        return reject("Firma inválida — posible payload manipulado")

    # 5. Validar nonce
    validate_nonce(claim.beaconId, claim.nonce, claim.ts)

    # 6. Validar geolocalización GPS (capa adicional)
    if not geovalidation.is_plausible(claim.playerGPS, claim.beaconId):
        return reject("Posición GPS incompatible con ubicación de la baliza")

    return accept(claim)
```

---

### 5.2.5 — Deep Sleep y Ahorro de Energía

**Cálculos detallados de vida de batería:**

```
Modelo de consumo para 18650 Li-Ion 3000 mAh @ 3.7V (11.1 Wh):

Clase C — Modem Sleep, advertising 500ms:
  Consumo activo (tx):     130 mA × 10ms  = 1.3 mAs por ciclo
  Consumo modem sleep:      25 mA × 490ms = 12.25 mAs por ciclo
  Consumo promedio:         (1.3 + 12.25) / 500ms = ~27.1 mA
  Vida estimada:            3000 mAh / 27.1 mA ≈ 110 horas ≈ 4.6 días

Clase B — Light Sleep, wake cada 5s, advertise 3 veces:
  Consumo activo (3 adv):   130 mA × 30ms  = 3.9 mAs
  Consumo light sleep:        1 mA × 4970ms = 4.97 mAs
  Consumo promedio:           (3.9 + 4.97) / 5000ms ≈ 1.77 mA
  Vida estimada:             3000 mAh / 1.77 mA ≈ 1695 horas ≈ 70 días

Clase A — Deep Sleep, wake cada 10s:
  Consumo activo (1 adv):   130 mA × 20ms = 2.6 mAs
  Consumo deep sleep:       0.015 mA × 9980ms ≈ 0.15 mAs
  Consumo promedio:         (2.6 + 0.15) / 10000ms ≈ 0.275 mA
  Vida estimada:            3000 mAh / 0.275 mA ≈ 10909 horas ≈ 454 días

Clase Omega — Event-triggered, 4h activas por mes:
  Consumo activo (4h/mes):  130 mA × 4h = 520 mAh/mes
  Consumo sleep resto:      0.01 mA × (720-4)h ≈ 7.16 mAh/mes
  Total mensual:            ≈ 527 mAh/mes
  Vida estimada:            3000 mAh / 527 mAh ≈ 5.7 meses por carga
```

**Implementación de Deep Sleep en firmware:**

```cpp
#include "esp_sleep.h"

#define DEEP_SLEEP_DURATION_US  10000000  // 10 segundos en microsegundos
#define ADVERTISING_DURATION_MS 20        // 20ms de advertising por ciclo

void beacon_sleep_cycle() {
    // Fase de advertising
    ble_start_advertising();
    vTaskDelay(pdMS_TO_TICKS(ADVERTISING_DURATION_MS));
    ble_stop_advertising();

    // Preparar para deep sleep
    esp_sleep_enable_timer_wakeup(DEEP_SLEEP_DURATION_US);

    // Guardar estado en RTC memory (persiste durante deep sleep)
    rtc_state.advertise_count++;
    rtc_state.last_wakeup = esp_rtc_get_time_us();

    // Entrar en deep sleep
    esp_deep_sleep_start();
    // El ESP32 reiniciará desde app_main() al despertar
}
```

---

## 5.3 — Diseño del Scanner PWA Angular

### 5.3.1 — Arquitectura Completa del Scanner

```
┌─────────────────────────────────────────────────────┐
│              THLS Angular PWA Scanner               │
├─────────────────────────────────────────────────────┤
│  UI Layer (Angular Components)                      │
│  ├── RadarComponent (SVG animado)                   │
│  ├── TemperatureOverlayComponent                    │
│  ├── ChallengeModalComponent                        │
│  └── HapticFeedbackService                         │
├─────────────────────────────────────────────────────┤
│  State Machine Layer (NgRx / RxJS)                  │
│  ├── ScanStateService                               │
│  ├── SignalProcessingService                        │
│  └── BeaconValidationService                        │
├─────────────────────────────────────────────────────┤
│  BLE Adapter Layer                                  │
│  ├── WebBluetoothAdapter (Chrome Android)           │
│  ├── CapacitorBLEAdapter (iOS native)               │
│  └── MockBLEAdapter (desarrollo/testing)            │
├─────────────────────────────────────────────────────┤
│  Native APIs                                        │
│  ├── Web Bluetooth API (navigator.bluetooth)        │
│  ├── Vibration API (navigator.vibrate)              │
│  ├── Web Audio API (AudioContext)                   │
│  └── Geolocation API (navigator.geolocation)        │
└─────────────────────────────────────────────────────┘
```

### 5.3.2 — Máquina de Estados del Scanner

```typescript
// scanner-state.enum.ts
export enum ScanState {
    IDLE             = 'IDLE',           // App abierta, sin escaneo activo
    SCANNING         = 'SCANNING',       // Escaneando activamente, sin señal
    SIGNAL_DETECTED  = 'SIGNAL_DETECTED',// Primera detección de beacon THLS
    APPROACH         = 'APPROACH',       // Señal estable, jugador acercándose
    CHALLENGE_READY  = 'CHALLENGE_READY',// Señal fuerte, desafío disponible
    INFILTRATING     = 'INFILTRATING',   // Desafío en curso
    SUCCESS          = 'SUCCESS',        // Baliza capturada correctamente
    FAILED           = 'FAILED',         // Fallo en el desafío o validación
    ERROR            = 'ERROR'           // Error de hardware/permisos BLE
}

// Transiciones válidas de estado
export const STATE_TRANSITIONS: Record<ScanState, ScanState[]> = {
    [ScanState.IDLE]:             [ScanState.SCANNING, ScanState.ERROR],
    [ScanState.SCANNING]:         [ScanState.SIGNAL_DETECTED, ScanState.IDLE, ScanState.ERROR],
    [ScanState.SIGNAL_DETECTED]:  [ScanState.APPROACH, ScanState.SCANNING],
    [ScanState.APPROACH]:         [ScanState.CHALLENGE_READY, ScanState.SIGNAL_DETECTED, ScanState.SCANNING],
    [ScanState.CHALLENGE_READY]:  [ScanState.INFILTRATING, ScanState.APPROACH],
    [ScanState.INFILTRATING]:     [ScanState.SUCCESS, ScanState.FAILED],
    [ScanState.SUCCESS]:          [ScanState.IDLE],
    [ScanState.FAILED]:           [ScanState.IDLE, ScanState.APPROACH],
    [ScanState.ERROR]:            [ScanState.IDLE]
};
```

### 5.3.3 — Implementación Web Bluetooth API

```typescript
// web-bluetooth.adapter.ts
import { Injectable } from '@angular/core';
import { Subject, Observable } from 'rxjs';

// THLS propietario: Company ID 0xFFFF (desarrollo) | 0xXXXX (registrado en SIG)
const THLS_COMPANY_ID = 0xFFFF;
const THLS_SERVICE_UUID = '0000THLS-0000-1000-8000-00805F9B34FB';

export interface THLSBeaconReading {
    beaconId: string;
    rssi: number;
    payload: THLSPayload;
    timestamp: number;
}

@Injectable({ providedIn: 'root' })
export class WebBluetoothAdapter {
    private beaconReadings$ = new Subject<THLSBeaconReading>();
    private scanAbortController: AbortController | null = null;

    async startScan(): Promise<void> {
        if (!navigator.bluetooth) {
            throw new Error('Web Bluetooth API no soportada en este navegador/dispositivo');
        }

        // Verificar disponibilidad (requiere HTTPS + Chrome Android)
        const available = await navigator.bluetooth.getAvailability();
        if (!available) {
            throw new Error('Bluetooth no disponible en este dispositivo');
        }

        this.scanAbortController = new AbortController();

        // Opciones de escaneo — filtrar EXCLUSIVAMENTE beacons THLS
        const scanOptions: RequestLEScanOptions = {
            filters: [{
                manufacturerData: [{
                    companyIdentifier: THLS_COMPANY_ID
                }]
            }],
            keepRepeatedDevices: true  // Necesario para actualizaciones RSSI continuas
        };

        try {
            // requestLEScan es una API experimental (Chrome flag o Chrome 79+ en Android)
            const scan = await (navigator.bluetooth as any).requestLEScan(
                scanOptions,
                { signal: this.scanAbortController.signal }
            );

            // Escuchar eventos de advertising
            navigator.bluetooth.addEventListener('advertisementreceived', (event: any) => {
                this.processAdvertisement(event);
            });

        } catch (error: any) {
            if (error.name === 'NotFoundError') {
                // El usuario canceló el diálogo de permisos
                throw new Error('Permiso BLE denegado por el usuario');
            }
            throw error;
        }
    }

    private processAdvertisement(event: any): void {
        const rssi: number = event.rssi;
        const manufacturerData = event.manufacturerData.get(THLS_COMPANY_ID);

        if (!manufacturerData) return;

        try {
            const payload = this.deserializePayload(new Uint8Array(manufacturerData.buffer));

            this.beaconReadings$.next({
                beaconId: payload.beaconId,
                rssi,
                payload,
                timestamp: Date.now()
            });
        } catch (e) {
            console.warn('Payload THLS inválido recibido, ignorando:', e);
        }
    }

    stopScan(): void {
        this.scanAbortController?.abort();
        this.scanAbortController = null;
    }

    getReadings(): Observable<THLSBeaconReading> {
        return this.beaconReadings$.asObservable();
    }

    private deserializePayload(bytes: Uint8Array): THLSPayload {
        // Deserializar payload binario de 31 bytes al objeto THLSPayload
        // (implementación completa omitida por brevedad, ver spec de payload en sección 5.2.2)
        return parseTHLSPayload(bytes);
    }
}
```

### 5.3.4 — Pipeline de Procesamiento de Señal

```
Lectura RSSI cruda
        │
        ▼ Paso 1: Recepción de evento BLE (advertisementreceived)
  [RSSI raw: -40 a -100 dBm, muy ruidoso, varianza ±15 dBm típico]
        │
        ▼ Paso 2: Rechazo de outliers (ver Sección 7.4)
  [Descartar lecturas fuera de media ± 2σ de la ventana actual]
        │
        ▼ Paso 3: Media móvil (ventana 5 lecturas)
  [Suavizado básico, latencia ~500ms a 100ms de scan rate]
        │
        ▼ Paso 4: Filtro de Kalman 1D
  [Estimación óptima minimizando ruido de proceso + ruido de medición]
        │
        ▼ Paso 5: Determinación de estado (frío/tibio/caliente...)
  [Con histéresis: 3 lecturas consecutivas para cambio de estado]
        │
        ▼ Paso 6: Actualización de UI + haptics + audio
  [Render del radar, vibración, audio frequency map]
```

### 5.3.5 — Estrategias de Ahorro de Batería

```typescript
// adaptive-scan.service.ts

enum ScanRate {
    AGGRESSIVE = 250,   // ms — Jugador muy cerca, máxima resolución
    NORMAL     = 500,   // ms — Aproximación activa
    SLOW       = 1000,  // ms — Señal detectada pero lejana
    IDLE       = 2000,  // ms — Sin señal, modo búsqueda pasiva
    BACKGROUND = 5000   // ms — App en segundo plano (si permitido)
}

@Injectable({ providedIn: 'root' })
export class AdaptiveScanService {

    getScanRateForState(state: ScanState, rssi: number | null): ScanRate {
        if (rssi === null) return ScanRate.IDLE;
        if (rssi > -55)   return ScanRate.AGGRESSIVE;  // Muy cerca, máxima prioridad
        if (rssi > -65)   return ScanRate.NORMAL;       // Aproximando
        if (rssi > -75)   return ScanRate.SLOW;         // Señal detectada
        return ScanRate.IDLE;                           // Señal lejana o ausente
    }
}
```

**Comportamiento con pantalla apagada:**

| Plataforma | Comportamiento | Mitigación |
|-----------|---------------|------------|
| Android (Chrome) | El escaneo BLE se pausa cuando Chrome va a background | Wake Lock API para mantener pantalla activa durante la misión |
| iOS (Safari) | Web Bluetooth NO soportado | Ver sección 5.3.7 (workaround iOS) |
| iOS (Capacitor) | Plugin nativo BLE en background con limitaciones | CoreBluetooth background mode, requiere declaración en Info.plist |
| Android (TWA) | Similar a Chrome | Mismas limitaciones de background |

### 5.3.6 — Diseño de Hápticos y Audio

**Patrones de vibración por estado:**

```typescript
// haptic-patterns.ts
export const HAPTIC_PATTERNS = {
    // Sin señal — sin vibración
    NO_SIGNAL:      [],

    // Frío — pulso suave único cada 4 segundos
    COLD:           [100],                              // 100ms vibración

    // Tibio — doble pulso cada 2 segundos
    WARM:           [100, 100, 100],                    // patrón: on-off-on

    // Caliente — triple pulso acelerado
    HOT:            [150, 75, 150, 75, 150],            // 3 pulsos con pausa corta

    // Muy caliente — pulsos rápidos continuos
    VERY_HOT:       [200, 50, 200, 50, 200, 50, 200],   // 4 pulsos densos

    // Localizado — vibración larga + 3 pulsos de confirmación
    FOUND:          [500, 100, 200, 100, 200, 100, 200], // LOCALIZADOOO

    // Desafío disponible — patrón de alerta
    CHALLENGE_READY:[100, 50, 100, 50, 100, 200, 300],

    // Captura exitosa — vibración de victoria
    SUCCESS:        [200, 100, 200, 100, 500],

    // Fallo — vibración larga descendente
    FAILED:         [400, 200, 200, 200, 100]
};
```

**Mapeo de frecuencias de audio por estado:**

```typescript
// audio-synthesizer.service.ts
// Usa Web Audio API (AudioContext) para síntesis de sonido en tiempo real

export const AUDIO_FREQUENCIES = {
    NO_SIGNAL:      { freq: 0,    type: 'noise', description: 'Ruido blanco estático suave' },
    COLD:           { freq: 220,  type: 'sine',  description: 'Tono grave y lento (pulso cada 2s)' },
    WARM:           { freq: 440,  type: 'sine',  description: 'La4 estándar (pulso cada 1.5s)' },
    HOT:            { freq: 660,  type: 'sine',  description: 'Mi5, más agudo (pulso cada 0.8s)' },
    VERY_HOT:       { freq: 880,  type: 'sine',  description: 'La5, urgente (pulso cada 0.4s)' },
    FOUND:          { freq: 1046, type: 'chord', description: 'Do6 + acorde mayor de resolución' },
    CHALLENGE_READY:{ freq: 523,  type: 'alert', description: 'Do5 con modulación de alerta' }
};
```

**Modo de accesibilidad (visual-only):**

Para usuarios con diversidad funcional auditiva o en entornos donde el audio no es apropiado, el modo accesible reemplaza:
- Audio → Indicadores visuales de color expandidos en toda la pantalla
- Vibración → Animación de pulso en el radar con mayor contraste
- Estados de temperatura → Texto en pantalla grande con nombre del estado y dirección estimada en cardinal (N/S/E/O) basada en magnetómetro del dispositivo.

---

### 5.3.7 — Limitaciones de Web Bluetooth API y Workarounds

**Matriz de compatibilidad:**

| Plataforma | Navegador | Web Bluetooth | requestLEScan | Capacitor BLE | Recomendación |
|-----------|-----------|:------------:|:------------:|:------------:|---------------|
| Android 6+ | Chrome 56+ | ✅ | ✅ (experimental) | ✅ | Web Bluetooth (primario) |
| Android | Firefox | ❌ | ❌ | ✅ | Capacitor como fallback |
| Android | Samsung Internet | ⚠️ Parcial | ❌ | ✅ | Capacitor recomendado |
| iOS 14+ | Safari | ❌ | ❌ | N/A | Solo app nativa |
| iOS 14+ | Chrome iOS | ❌ | ❌ | N/A | Solo app nativa |
| iOS 14+ | App Capacitor | N/A | N/A | ✅ | **Obligatorio para iOS** |
| macOS | Chrome | ✅ | ⚠️ | ✅ | Web Bluetooth (debug) |
| Windows 10+ | Chrome | ✅ | ⚠️ | N/A | Web Bluetooth |

**Workaround para iOS:**

```
Estrategia iOS THLS:
1. Detectar iOS en el navegador al abrir la PWA
2. Mostrar banner: "Para la experiencia completa en iOS, descarga la app THLS"
3. Redirigir a App Store con la app basada en Capacitor + @capacitor-community/bluetooth-le
4. La app Capacitor usa CoreBluetooth nativo de iOS bajo el capó
5. Toda la lógica Angular/TypeScript es compartida — solo el adaptador BLE es nativo

Flujo de fallback en PWA para iOS:
- Sin BLE: el jugador puede acceder a misiones de tipo QR/NFC como modo alternativo
- La misión "de acercamiento" se reemplaza por pistas textuales graduales
- El jugador introduce un código final que obtiene al llegar al punto físico
```

---

---

# SECCIÓN 6: SISTEMA FRÍO-CALIENTE BASADO EN RSSI

> *"La temperatura no es física. Es información. Y la información, en manos equivocadas, puede destruir civilizaciones."*
> — Archivo ARGOS, documento clasificado Ω-9

---

## 6.1 — Estados de Señal: Especificación Completa

El sistema de temperatura de THLS transforma el valor numérico abstracto del RSSI en una experiencia emocional de tensión y progresión. Cada estado tiene una identidad visual, sonora, háptica y narrativa completamente definida.

---

### Tabla Maestra de Estados de Señal

| Estado | RSSI (dBm) | Color UI | Hex Color | Mensaje UI | Vibración (ms) | Audio | Animación | Distancia Est. | Nivel de Tensión |
|--------|-----------|---------|-----------|------------|---------------|-------|-----------|---------------|-----------------|
| **SIN SEÑAL** | < -95 | Gris oscuro / Negro | `#1A1A1A` / `#0D0D0D` | `SIN SEÑAL — ZONA MUERTA` | Ninguna | Ruido blanco estático | Radar estático, sin movimiento | >100 m (desconocida) | ⬛ Nula |
| **FRÍO** | -95 a -85 | Azul oscuro gélido | `#0A2A4A` | `INTERFERENCIA DETECTADA` | `[100]` una vez/4s | Pulso grave 220 Hz/3s | Radar girando lento, un anillo tenue | 50 – 100 m | 🟦 Baja |
| **TIBIO** | -85 a -75 | Azul-verde teal | `#0E6655` | `SEÑAL TENUE — CONTINÚA` | `[100, 100, 100]` / 2s | Pulso medio 440 Hz/2s | Radar a media velocidad, 2 anillos | 20 – 50 m | 🟩 Media-baja |
| **CALIENTE** | -75 a -65 | Ámbar-naranja | `#D4842A` | `SEÑAL ACTIVA — ACÉRCATE` | `[150, 75, 150, 75, 150]` / 1s | Tono ágil 660 Hz/1s | Radar rápido, 3 anillos pulsantes | 10 – 20 m | 🟧 Media-alta |
| **MUY CALIENTE** | -65 a -55 | Rojo-naranja intenso | `#E84E0F` | `ORIGEN HOSTIL CERCA` | `[200,50,200,50,200,50,200]` / 0.5s | Pulsos agudos 880 Hz continuos | Radar a máxima velocidad, 4 anillos brillantes | 5 – 10 m | 🟥 Alta |
| **BALIZA LOCALIZADA** | > -55 | Blanco brillante + Destello | `#FFFFFF` / `#FFD700` glow | `BALIZA DETECTADA — INFILTRACIÓN` | `[500,100,200,100,200,100,200]` | Acorde Do6, jingle de resolución | Explosión radial, all rings + glow aura | < 5 m | ⭐ Máxima |

---

### 6.1.1 — Estado: SIN SEÑAL (RSSI < -95 dBm)

**Condición:** El dispositivo no recibe ningún paquete BLE del UUID THLS, o los recibe con intensidad tan baja que no supera el umbral mínimo del receptor (típicamente -100 dBm en smartphones consumer).

**Especificación UX completa:**
```
Color de fondo:    #0D0D0D (negro casi puro)
Color de radar:    #1A1A1A (gris muy oscuro, apenas visible)
Texto principal:   "SIN SEÑAL"
Texto secundario:  "ZONA MUERTA — ARGOS ACTIVO EN EL ÁREA"
Subtext narrativo: "Los bloqueadores electromagnéticos de ARGOS están operativos."

Vibración:         Ninguna. El silencio háptico refuerza la sensación de vacío.
Audio:             Ruido blanco suave (gain 0.05), con ocasionales crepiteos estáticos
                   que suenan como interferencia de radio. Frecuencia de base: N/A.
                   Loop de ambiance de ~30s: ruido blanco + glitch ocasional.

Animación de radar:
  - Radar circle completamente gris/estático
  - Sin animación de barrido
  - Línea de scan: presente pero sin brillo, como un osciloscopio muerto
  - Ocasional "glitch" visual: deformación del radar por 200ms cada ~8s
  - Partículas de interferencia: puntos blancos estáticos aleatorios en el canvas
  - Opacidad del radar: 30% (apagado, inactivo)

Indicador de dirección: Oculto (no hay información direccional)
Indicador de distancia: "???" parpadeando en rojo tenue
```

---

### 6.1.2 — Estado: FRÍO (-95 a -85 dBm)

**Condición:** Se detectan paquetes BLE del UUID THLS pero con señal muy débil. La baliza está lejana o hay obstrucción masiva.

```
Color de fondo:    Gradiente radial desde #0D1B2A (exterior) hasta #0A2A4A (centro)
Color de radar:    #1B4F72 (azul oscuro)
Texto principal:   "FRÍO"
Texto secundario:  "INTERFERENCIA DETECTADA EN EL SECTOR"
Subtext narrativo: "Detectando ondas residuales. La baliza existe. Sigue buscando."

Vibración:         Pulso único de 100ms una vez cada 4 segundos.
                   Patrón: [100] con delay 4000ms entre repeticiones.
Audio:             Tono sinusoidal 220 Hz (La3), duración 300ms,
                   silencio 2700ms. Gain: 0.15. Sensación: sonar de submarino lento.

Animación de radar:
  - Velocidad de barrido: 6 segundos por revolución (muy lento)
  - 1 anillo visible: tenue, radio al 80% del canvas, opacidad 40%
  - Trail del scanner: corto (15° de arco), desvanecimiento rápido
  - Color del scanner: #1B6CA8 (azul medio)
  - Partículas: 2-3 puntos azules tenues parpadeando aleatoriamente

Indicador de dirección: Oculto
Indicador de distancia: "~50-100m" (estimación muy aproximada, semitransparente)
```

---

### 6.1.3 — Estado: TIBIO (-85 a -75 dBm)

**Condición:** Señal detectada con calidad marginal. El jugador está en la zona general de la baliza pero lejos del objetivo.

```
Color de fondo:    Gradiente desde #0D2B2B hasta #0E6655 (teal oscuro)
Color de radar:    #17A589 (verde-azulado medio)
Texto principal:   "TIBIO"
Texto secundario:  "SEÑAL TENUE — CONTINÚA AVANZANDO"
Subtext narrativo: "La interferencia de ARGOS cede. Origen de señal en algún lugar cercano."

Vibración:         Doble pulso: [100, 100, 100] (on-off-on) cada 2 segundos.
                   Sensación: llamada suave, como un latido doble.
Audio:             Tono sinusoidal 440 Hz (La4 — afinación estándar), 400ms on,
                   1600ms off. Gain: 0.25. Ligeramente más urgente que FRÍO.

Animación de radar:
  - Velocidad de barrido: 3 segundos por revolución
  - 2 anillos visibles: interior (radio 40%) y exterior (radio 75%), opacidad 55%
  - Trail del scanner: 30° de arco, desvanecimiento medio
  - Color del scanner: #1ABC9C (verde teal brillante)
  - Partículas: 5-6 puntos verde-azulados con movimiento suave browniano

Indicador de dirección: Aparece un indicador de brújula muy impreciso (±90°)
Indicador de distancia: "~20-50m" (estimación aproximada)
```

---

### 6.1.4 — Estado: CALIENTE (-75 a -65 dBm)

**Condición:** Señal clara y estable. El jugador está aproximándose activamente. La tensión aumenta notablemente.

```
Color de fondo:    Gradiente desde #1A0A00 hasta #7D3C00 (marrón-naranja oscuro)
Color de radar:    #E67E22 (naranja medio)
Texto principal:   "CALIENTE"
Texto secundario:  "SEÑAL ACTIVA — OBJETIVO EN PROXIMIDAD"
Subtext narrativo: "Señal identificada. Origen hostil. Proceder con cautela extrema."

Vibración:         Triple pulso: [150, 75, 150, 75, 150] cada segundo.
                   Urgencia percibida: comparable a notificación de alerta importante.
Audio:             Tono 660 Hz (Mi5), pulsos de 500ms con 500ms de silencio.
                   Gain: 0.35. Ritmo cardíaco acelerado.

Animación de radar:
  - Velocidad de barrido: 1.5 segundos por revolución (notablemente más rápido)
  - 3 anillos visibles con efecto de "pulso" (escalan suavemente del 95% al 105%)
  - Trail del scanner: 60° de arco, desvanecimiento lento (cola luminosa)
  - Color del scanner: #F39C12 (ámbar brillante)
  - Efecto de calor: ondulación sutil del canvas (CSS filter: blur oscilante 0-1px)
  - Partículas: 10-12 puntos naranja-amarillos con movimiento errático ascendente

Indicador de dirección: Indicador de brújula con ±45° de precisión
Indicador de distancia: "~10-20m" (estimación útil)
```

---

### 6.1.5 — Estado: MUY CALIENTE (-65 a -55 dBm)

**Condición:** Señal fuerte. El jugador está muy cerca. El desafío puede estar disponible. Máxima urgencia antes del estado final.

```
Color de fondo:    Gradiente pulsante entre #2C0A00 y #C0392B (rojo oscuro)
Color de radar:    #E74C3C (rojo vivo)
Texto principal:   "MUY CALIENTE"
Texto secundario:  "ORIGEN HOSTIL DETECTADO — CASI AHÍÍ"
Subtext narrativo: "ALERTA: Sistema de seguridad ARGOS activo. Acceso no autorizado."

Vibración:         Cuádruple pulso rápido: [200,50,200,50,200,50,200] cada 500ms.
                   Sensación: alarma inminente, acción urgente requerida.
Audio:             Tono 880 Hz (La5) en pulsos muy rápidos, 200ms on/200ms off.
                   Gain: 0.45. Sonido de alarma táctica.
                   Añadir LFO de 2 Hz sobre el tono para efecto "escalofriante".

Animación de radar:
  - Velocidad de barrido: 0.8 segundos por revolución (muy rápido)
  - 4 anillos con pulso intenso y efecto de "latido cardíaco" (animación scale)
  - Trail del scanner: 90° de arco con glow rojo intenso
  - Color del scanner: #FF5733 (rojo-naranja brillante)
  - Efecto: el radar completo pulsa (opacity 85%-100%) al ritmo de la vibración
  - Partículas: 15-20 puntos rojos efervescentes con velocidades altas
  - Borde de la pantalla: efecto "sangrado" rojo pulsante (vignette animada)

Indicador de dirección: Indicador de brújula con ±20° de precisión
Indicador de distancia: "~5-10m" (estimación precisa)
```

---

### 6.1.6 — Estado: BALIZA LOCALIZADA (RSSI > -55 dBm)

**Condición:** La baliza está a menos de 5 metros. El sistema confirma presencia con múltiples lecturas consecutivas. Trigger del desafío o captura directa según configuración.

```
Color de fondo:    Flash inicial: blanco puro (#FFFFFF) → luego negro con aura dorada
Color de radar:    Dorado brillante (#FFD700) con glow exterior
Texto principal:   "◉ BALIZA DETECTADA ◉"
Texto secundario:  "INICIANDO PROTOCOLO DE INFILTRACIÓN"
Subtext narrativo: "BALIZA DETECTADA. Nonce verificado. Firma válida. Acceso concedido."

Vibración:         Secuencia de victoria: [500, 100, 200, 100, 200, 100, 200]
                   Larga pulsación inicial + 3 pulsos de confirmación.
Audio:             Secuencia musical:
                     1. Silencio repentino (contraste dramático, 300ms)
                     2. Chord mayor en Do6: C6+E6+G6 (200ms)
                     3. Arpegio ascendente (500ms)
                     4. Jingle de victoria corto (2s, estilo THLS brand)
                   Gain: 0.65.

Animación de radar:
  - FLASH: pantalla blanca completa (200ms) simulando "overdrive" de señal
  - Explosión radial desde el centro: anillos que se expanden hacia el borde
  - Todos los anillos encendidos: 4 + pulso dorado adicional exterior
  - Aura de pulso dorada: efecto glow que rodea el radar completo
  - Partículas: 30+ puntos dorados-blancos en explosión radial
  - Logo de facción capturada aparece en el centro del radar
  - Countdown timer visible si hay desafío: "DESAFÍO DISPONIBLE — 180s"
  - Overlay semitransparente: información de la baliza capturada (clase, rareza, puntos)

Indicador de dirección: Oculto (ya no relevante)
Indicador de distancia: "< 5m — ¡OBJETIVO ALCANZADO!"
```

---

## 6.2 — Diseño del Radar UI

El radar es el elemento visual central de la experiencia THLS. Está implementado como un canvas HTML5 animado a 60 fps con elementos SVG para los indicadores.

```
Anatomía del radar:
                    ┌────────────────────────┐
                    │  ·  · SECTOR N  ·  ·  │
                    │ ·  ┌──────────┐  ·    │
                    │·   │  ○  ○  ○ │   ·   │
                    │  ←─┤    ◎    ├─→     │
                    │    │  ○  ○  ○ │       │
                    │    └────┬─────┘       │
                    │  ·  ·  SECTOR S  ·  ·  │
                    └────────────────────────┘

Componentes del canvas:
  ◎ = Centro del radar (posición del jugador)
  ○ = Anillos de señal (1 a 4 según estado)
  → = Brazo de barrido (scanner sweep)
  · = Partículas de señal / ruido de ambiente
```

**Frecuencia de barrido del radar:**

| Estado | Velocidad Angular | Periodo (rev completa) | Sensación |
|--------|-----------------|----------------------|-----------|
| SIN SEÑAL | 0° / s | Estático | Muerto |
| FRÍO | 60° / s | 6 segundos | Monótono, tranquilo |
| TIBIO | 120° / s | 3 segundos | Atento |
| CALIENTE | 240° / s | 1.5 segundos | Urgente |
| MUY CALIENTE | 450° / s | 0.8 segundos | Frenético |
| LOCALIZADO | 720° / s → Flash → Estático | 0.5s → explosión | Clímax dramático |

**Artefactos de ruido para inmersión:**

El radar incorpora efectos visuales que simulan las imperfecciones de un sistema de detección real:

```typescript
// noise-artifacts.ts
const RADAR_NOISE_CONFIG = {
    staticParticles: {
        count: 8,
        behavior: 'random-blink',   // Parpadeo aleatorio
        color: 'rgba(255,255,255,0.15)',
        size: [1, 3]                // 1 a 3 px
    },
    signalGhost: {
        probability: 0.05,          // 5% por frame: aparición de "fantasma" de señal
        duration: 200,              // 200ms de duración
        color: 'rgba(100,200,255,0.2)',
        position: 'random-ring'     // Aparece en un punto aleatorio de un anillo
    },
    scannerJitter: {
        maxDegrees: 2,              // ±2° de vibración en el brazo del scanner
        frequency: 0.3,             // 30% de frames con jitter
        behavior: 'perlin-noise'    // Jitter suave, no aleatorio puro
    },
    screenGlitch: {
        probability: 0.001,         // 0.1% por frame: glitch de pantalla completa
        duration: 50,               // 50ms, muy breve
        type: 'chromatic-aberration'// Separación de canales RGB
    }
};
```

---

## 6.3 — Estados MVP Simplificados para el POC

Para el Proof of Concept, se implementan los 3 estados esenciales que validan la mecánica core:

```
Estado MVP 1: FRÍO
  └── Criterio: Sin señal detectada O RSSI < -75 dBm (>20m estimado)
  └── UI: Radar lento, color azul, texto "FRÍO"
  └── Feedback: Vibración pulso simple cada 3s

Estado MVP 2: CALIENTE
  └── Criterio: RSSI entre -75 y -55 dBm (5-20m estimado)
  └── UI: Radar rápido, color naranja-rojo, texto "CALIENTE"
  └── Feedback: Vibración triple pulso cada 1s

Estado MVP 3: OBJETIVO ENCONTRADO
  └── Criterio: RSSI > -55 dBm (< 5m estimado), confirmado por 5 lecturas consecutivas
  └── UI: Flash + animación dorada + jingle
  └── Feedback: Vibración de victoria + audio
  └── Acción: Desbloquear formulario de captura / desafío
```

---

## 6.4 — Diseño de Transición de Estados con Histéresis

La histéresis previene el "flickering" de estados cuando el RSSI oscila en la frontera entre dos estados. Sin histéresis, el jugador experimentaría transiciones erráticas que romperían la tensión narrativa.

```
SIN HISTÉRESIS (problemático):
  RSSI oscila: -74, -76, -74, -76, -74...
  → Estado: CALIENTE, TIBIO, CALIENTE, TIBIO, CALIENTE... (parpadeante)
  → UX: Confuso, molesto, anti-inmersivo

CON HISTÉRESIS (implementado en THLS):
  RSSI oscila: -74, -76, -74, -76, -74...
  → Se requieren 3 lecturas consecutivas en nuevo estado para transición
  → Estado permanece CALIENTE durante toda la oscilación
  → UX: Estable, coherente, inmersivo
```

**Implementación de histéresis asimétrica:**

```typescript
// state-hysteresis.service.ts

interface StateHysteresisConfig {
    upgradeThreshold: number;  // Lecturas consecutivas para ir a estado "más caliente"
    downgradeThreshold: number; // Lecturas consecutivas para ir a estado "más frío"
}

// ASIMÉTRICO: más fácil subir (calentarse) que bajar (enfriarse)
// Razón de diseño: crear sensación de "progreso" cuando el jugador se acerca,
// pero mantener la tensión cuando se aleja ligeramente (no perder el estado ganado)
const HYSTERESIS_CONFIG: StateHysteresisConfig = {
    upgradeThreshold: 3,    // 3 lecturas consecutivas para calentar
    downgradeThreshold: 5   // 5 lecturas consecutivas para enfriar
};
```

**Justificación del diseño asimétrico:**

La histéresis asimétrica (calentar más fácil que enfriar) tiene un fundamento psicológico en el diseño de juegos:

1. **Retroalimentación positiva inmediata:** Cuando el jugador se acerca a la baliza, recibe retroalimentación positiva (calentamiento) con rapidez. Esto refuerza el comportamiento correcto.
2. **Frustración reducida al alejarse:** Si el jugador da un paso atrás sin intención, no pierde inmediatamente el estado "caliente" conseguido. Tiene 5 lecturas de "gracia".
3. **Mecánica de tensión controlada:** El downgrade más lento crea una ventana donde el jugador siente que "casi lo tiene" incluso si se ha desviado, motivándole a corregir el rumbo.

---

## 6.5 — Integración Narrativa

Cada estado del sistema de temperatura tiene un texto narrativo asociado que se muestra como overlay en la pantalla, reforzando la ficción del juego:

```
┌─────────────────────────────────────────────────────────┐
│ Estado: SIN SEÑAL                                        │
│ Overlay: "Los inhibidores de ARGOS están operativos     │
│           en este sector. La señal no penetra."         │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│ Estado: FRÍO                                             │
│ Overlay: "Detectando interferencias electromagnéticas   │
│           residuales. Origen: desconocido. Busca."      │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│ Estado: TIBIO                                            │
│ Overlay: "Frecuencia de ARGOS identificada. La señal    │
│           es real. Ajusta tu trayectoria."              │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│ Estado: CALIENTE                                         │
│ Overlay: "Señal identificada. Origen hostil.            │
│           Proceder con cautela extrema."                │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│ Estado: MUY CALIENTE                                     │
│ Overlay: "ALERTA: Seguridad ARGOS activa en el área.    │
│           Acceso no autorizado. Cuenta atrás iniciada." │
└─────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────┐
│ Estado: BALIZA LOCALIZADA                                │
│ Overlay: "BALIZA DETECTADA. Iniciando protocolo de      │
│           infiltración. Nonce validado. Procede."       │
└─────────────────────────────────────────────────────────┘
```

Los textos narrativos rotan entre variantes para evitar repetición. Cada estado tiene un banco de 5-8 variaciones textuales que el sistema selecciona aleatoriamente, manteniendo la narrativa fresca en repetidas misiones.

---

---

# SECCIÓN 7: FÓRMULA DE PROXIMIDAD Y FILTRADO DE SEÑAL

> *"La distancia no existe en la realidad del espectro. Solo existe la probabilidad de presencia. Aprende a leer el ruido, y el ruido te guiará."*
> — Manual de campo del cazador THLS

---

## 7.1 — Fórmula de Pérdida de Ruta (Path Loss Model)

El modelo de propagación de señal de radio más utilizado en sistemas de localización indoor/outdoor por BLE es el **Log-Distance Path Loss Model**:

```
         TxPower - RSSI
d = 10 ^ ─────────────────
              10 × n
```

Donde:
- `d` = distancia estimada en metros
- `TxPower` = potencia de transmisión calibrada a 1 metro (en dBm). En BLE, este es el campo "TX Power Level" del advertising data. Para ESP32 a +6 dBm, calibrado a 1m: **TxPower ≈ -59 dBm**
- `RSSI` = Received Signal Strength Indicator medido por el receptor (en dBm)
- `n` = factor de pérdida de ruta (path loss exponent), dependiente del entorno

---

### 7.1.1 — Tabla de Factores n por Entorno

| Entorno | Factor n típico | Rango n | Notas de calibración |
|---------|----------------|---------|---------------------|
| Espacio libre ideal (vacío) | 2.0 | 1.8 – 2.2 | Teórico, rara vez alcanzable en práctica |
| Exterior abierto (campo) | 2.1 | 2.0 – 2.3 | Buena aproximación en exteriores sin obstáculos |
| Parque urbano | 2.3 | 2.0 – 2.5 | Vegetación añade atenuación variable |
| Exterior urbano (calles) | 2.7 | 2.5 – 3.0 | Reflexiones en edificios, multipath moderado |
| Oficina diáfana | 3.0 | 2.5 – 3.5 | Mesas, personas, equipos electrónicos |
| Pasillo de edificio | 1.8 | 1.5 – 2.0 | Efecto "guía de onda", la señal se propaga mejor |
| Interior estándar (hogar) | 3.0 | 2.5 – 3.5 | Tabiques, muebles, electrodomésticos |
| Interior con muros de ladrillo | 3.5 | 3.0 – 4.0 | Alta atenuación por material |
| Interior con muros de hormigón | 4.0 | 3.5 – 4.5 | Atenuación severa, señal degradada rápidamente |
| Sótano / subterráneo | 4.5 | 4.0 – 5.0 | Reflexiones múltiples, alta varianza |

---

### 7.1.2 — Ejemplos Resueltos Paso a Paso

#### Ejemplo 1: Oficina Diáfana (n = 3)

**Parámetros:**
- Entorno: Planta abierta de oficinas, tabiques de vidrio y drywall
- n = 3 (pérdida de propagación moderada)
- TxPower = -59 dBm (ESP32 calibrado a 1m)
- RSSI medido = -80 dBm

**Cálculo:**

```
         TxPower - RSSI        -59 - (-80)         21
d = 10 ^ ──────────────── = 10 ^ ──────────── = 10 ^ ────
              10 × n               10 × 3              30

d = 10 ^ 0.7 = 5.012 ≈ 5 metros
```

**Interpretación:** El dispositivo está aproximadamente a **5 metros** de la baliza en una oficina diáfana. En el sistema THLS, este RSSI de -80 dBm correspondería al estado **TIBIO** (recordar: -85 a -75 dBm), con el jugador a pocos metros del objetivo.

---

#### Ejemplo 2: Parque Urbano (n = 2)

**Parámetros:**
- Entorno: Parque metropolitano con vegetación dispersa
- n = 2 (pérdida cercana al espacio libre)
- TxPower = -59 dBm
- RSSI medido = -70 dBm

**Cálculo:**

```
         TxPower - RSSI        -59 - (-70)         11
d = 10 ^ ──────────────── = 10 ^ ──────────── = 10 ^ ────
              10 × n               10 × 2              20

d = 10 ^ 0.55 = 3.548 ≈ 3.5 metros
```

**Interpretación:** A solo **3.5 metros** de la baliza en un parque abierto, el RSSI es de -70 dBm. En THLS esto corresponde al estado **CALIENTE** (-75 a -65 dBm). La señal relativamente alta a tan corta distancia explica por qué en exteriores la baliza se detecta fácilmente: el entorno abierto no atenúa tanto la señal.

> ⚠️ **Nota de diseño:** En parques abiertos con n=2, la baliza a +6 dBm se detecta desde 50+ metros. Esto crea la sensación épica de "rastreo de larga distancia" que buscamos para misiones al aire libre.

---

#### Ejemplo 3: Exterior Urbano con Edificios (n = 3.5)

**Parámetros:**
- Entorno: Calle urbana con edificios de 5 plantas a ambos lados (canyon urbano)
- n = 3.5 (reflexiones y difracción moderadas-altas)
- TxPower = -59 dBm
- RSSI medido = -85 dBm

**Cálculo:**

```
         TxPower - RSSI        -59 - (-85)         26
d = 10 ^ ──────────────── = 10 ^ ──────────── = 10 ^ ────
              10 × n               10 × 3.5            35

d = 10 ^ 0.7429 = 5.53 ≈ 5.5 metros
```

**Interpretación:** A **5.5 metros** en un entorno urbano denso. El mismo RSSI de -85 dBm que en el Ejemplo 2 indicaba 3.5 metros, aquí indica 5.5 metros. Esto demuestra el impacto crítico del factor n en la estimación: **el mismo RSSI puede representar distancias muy diferentes según el entorno.**

> Este es el argumento central de por qué en THLS no mostramos distancia exacta al jugador.

---

#### Ejemplo 4: Interior con Muros de Hormigón (n = 4)

**Parámetros:**
- Entorno: Sótano de parking con columnas y muros de hormigón armado
- n = 4 (alta atenuación)
- TxPower = -59 dBm
- RSSI medido = -90 dBm

**Cálculo:**

```
         TxPower - RSSI        -59 - (-90)         31
d = 10 ^ ──────────────── = 10 ^ ──────────── = 10 ^ ────
              10 × n               10 × 4              40

d = 10 ^ 0.775 = 5.96 ≈ 6 metros
```

**Interpretación:** Solo **6 metros** de la baliza pero con RSSI de -90 dBm, que en THLS corresponde al estado **FRÍO** (−95 a −85 dBm). En entornos con n=4, el jugador puede estar a apenas 6 metros de la baliza y sentirse "frío". Esto crea misiones de interior extremadamente tensas: el jugador sabe que la baliza está cerca, pero la señal es débil. La única forma de encontrarla es moverse metodicamente y observar el gradiente.

---

## 7.2 — Por Qué NO Se Muestra la Distancia Exacta al Jugador

**Esta es una decisión de diseño deliberada y fundamental en THLS.** Mostrar la distancia calculada sería técnicamente sencillo, pero destruiría la mecánica de juego.

### 7.2.1 — Análisis de Errores del RSSI

```
RSSI a 10 metros (teórico) ≈ -80 dBm (en entorno n=3)

Varianza típica en medición: ±10 dBm

Rango observado real: -70 dBm a -90 dBm

Distancias calculadas con esos RSSI:
  -70 dBm → d = 10^((−59−(−70))/(10×3)) = 10^(11/30) = 10^0.367 ≈ 2.3 metros
  -80 dBm → d = 10^((−59−(−80))/(10×3)) = 10^(21/30) = 10^0.7   ≈ 5.0 metros
  -90 dBm → d = 10^((−59−(−90))/(10×3)) = 10^(31/30) = 10^1.033 ≈ 10.8 metros

Resultado: El mismo punto físico a 5m produce estimaciones de 2.3m a 10.8m
           → Error de hasta 4.7x en la distancia estimada
```

### 7.2.2 — Fuentes de Error e Impacto

| Fuente de Error | Impacto Típico | Error RSSI | Error en Distancia |
|----------------|----------------|-----------|-------------------|
| **Varianza inherente del hardware BLE** | Muy alto | ±5 a ±15 dBm | 2 a 10x |
| **Cuerpo humano** (teléfono en bolsillo vs en mano) | Alto | -3 a -8 dBm | 2 a 5x |
| **Orientación del teléfono** (antena taponada por palma) | Medio | -3 a -6 dBm | 1.5 a 3x |
| **Muro de hormigón armado** (señal atravesando muro) | Muy alto | -10 a -20 dBm | 5 a 50x |
| **Muro de drywall / tabique ligero** | Bajo | -3 a -5 dBm | 1.5 a 3x |
| **Reflexión urbana (multipath)** | Medio | ±5 dBm | 1.5 a 4x |
| **Variación entre modelos de teléfono** (antena diferente) | Medio | ±5 dBm | 1.5 a 4x |
| **Temperatura ambiente** (efectos en electrónica) | Bajo | ±1 a ±2 dBm | 1.1 a 1.5x |
| **Interferencias WiFi 2.4 GHz** (canal solapado con BLE) | Medio | ±3 a ±8 dBm variable | 1.5 a 3x |
| **Humedad ambiental alta** (lluvia, niebla) | Bajo | -1 a -3 dBm | 1.1 a 1.8x |

**Conclusión del análisis de errores:**

```
Error total acumulado en el peor caso (interior, cuerpo humano, multipath):
  RSSI error: -8 (cuerpo) - 15 (muro) ± 5 (multipath) = hasta ±28 dBm de error

  Con n=3, 28 dBm de error RSSI:
  d_real = 5m → RSSI esperado: -80 dBm
  RSSI observado podría ser: -52 dBm a -108 dBm
  d_calculada: desde 0.5m hasta >100m

  Mostrar "DISTANCIA: 5 metros" cuando el jugador está a 5 metros
  pero el cálculo da "37 metros" sería peor que no mostrar nada.
```

### 7.2.3 — La Incertidumbre ES la Mecánica de Juego

Mostrar distancia exacta transformaría THLS en una app de navegación trivial. El diseño actual aprovecha la incertidumbre del RSSI para crear:

1. **Exploración activa:** Sin distancia exacta, el jugador debe moverse y observar el gradiente de cambio en el estado de temperatura. La direccionalidad emerge del movimiento, no de un número.

2. **Interpretación heurística:** Los jugadores expertos aprenden a "leer" el RSSI de forma intuitiva, desarrollando una habilidad genuina que los diferencia de los novatos.

3. **Narrativa de incertidumbre táctica:** La ficción del juego es coherente: en un mundo donde ARGOS jamming está activo, la precisión de los instrumentos de detección siempre es imperfecta.

4. **Prevención de trivialización:** Si el juego mostrara "DISTANCIA: 4.3 metros", cualquier jugador encontraría la baliza en segundos. La abstracción del sistema fría-caliente introduce fricción satisfactoria.

5. **Diferenciación de plataformas:** El sistema de temperatura funciona con cualquier teléfono, independientemente de la precisión de su receptor BLE. Normaliza las diferencias de hardware bajo una experiencia unificada.

---

## 7.3 — Pipeline de Filtrado de Señal: Diseño Completo

El pipeline de filtrado procesa los valores RSSI crudos en una estimación suavizada y confiable antes de determinar el estado de temperatura.

```
     ┌─────────────────────────────────────────────────────┐
     │              PIPELINE DE FILTRADO THLS              │
     ├─────────────────────────────────────────────────────┤
     │  1. Lectura RSSI cruda (evento BLE: -40 a -100 dBm) │
     │  2. Hard limits: descartar si < -105 o > -25 dBm    │
     │  3. Z-score outlier rejection (ventana deslizante)   │
     │  4. Media móvil o filtro Mediana (ventana 5-7 vals)  │
     │  5. Filtro de Kalman 1D (suavizado óptimo)           │
     │  6. Determinación de estado con histéresis           │
     │  7. Render UI + haptics + audio                      │
     └─────────────────────────────────────────────────────┘
```

---

### 7.3.1 — Filtro de Media Móvil (Moving Average)

El filtro de media móvil es el más simple y generalmente el primer paso del pipeline. Promedia las N lecturas más recientes.

```python
# moving_average_filter.py
from collections import deque
from typing import Optional

class MovingAverageFilter:
    """
    Filtro de media móvil para suavizado de señal RSSI.
    
    Args:
        window_size: Número de muestras en la ventana deslizante (recomendado: 3-7)
    """
    def __init__(self, window_size: int = 5):
        self.window_size = window_size
        self.window: deque = deque(maxlen=window_size)

    def add_reading(self, rssi: float) -> Optional[float]:
        """
        Agrega una nueva lectura RSSI y retorna el promedio actual.
        Retorna None si la ventana no está llena aún (warm-up period).
        """
        self.window.append(rssi)

        if len(self.window) < self.window_size:
            # Durante el warm-up, usar el promedio de las muestras disponibles
            return sum(self.window) / len(self.window)

        return sum(self.window) / self.window_size

    def reset(self):
        self.window.clear()


# Uso en el scanner THLS:
# rssi_filter = MovingAverageFilter(window_size=5)
# smoothed_rssi = rssi_filter.add_reading(raw_rssi)
```

**Cuándo usar la media móvil:**
- En el POC como primer filtro por su simplicidad de implementación
- Cuando la latencia de respuesta es crítica (la media móvil tiene baja latencia vs Kalman)
- Como paso previo al filtro de Kalman para pre-suavizado

**Tamaño de ventana recomendado:**

| Ventana | Suavizado | Latencia | Uso recomendado |
|---------|-----------|----------|-----------------|
| 3 muestras | Mínimo | ~300ms a 100ms scan | Debugging, respuesta rápida |
| 5 muestras | Moderado | ~500ms a 100ms scan | ✅ **Recomendado para THLS POC** |
| 7 muestras | Alto | ~700ms a 100ms scan | Entornos muy ruidosos |
| 10 muestras | Máximo | ~1000ms a 100ms scan | Perjudica la sensación de proximidad |

**Trade-offs:** Una ventana más grande suaviza más el ruido pero introduce más latencia. Si el jugador se mueve rápidamente hacia la baliza, una ventana de 10 lecturas significa ~1 segundo de retraso en reflejar el cambio de estado, lo que rompe la retroalimentación inmediata.

---

### 7.3.2 — Filtro Mediana (Median Filter)

El filtro de mediana es superior a la media móvil para rechazar outliers puntales (spikes de RSSI) sin distorsionar el valor central.

```python
# median_filter.py
from collections import deque
from statistics import median
from typing import Optional

class MedianFilter:
    """
    Filtro de mediana para señal RSSI. Superior a media móvil para
    rechazar spikes puntuales sin contaminar la estimación.
    
    Ideal cuando:
    - El ruido tiene distribución de Laplace (colas pesadas) en lugar de Gaussiana
    - Hay interferencias puntuales de otros dispositivos BLE
    - El entorno tiene reflexiones intermitentes (parking, estación de metro)
    """
    def __init__(self, window_size: int = 5):
        # window_size debe ser impar para mediana bien definida
        if window_size % 2 == 0:
            window_size += 1
        self.window_size = window_size
        self.window: deque = deque(maxlen=window_size)

    def add_reading(self, rssi: float) -> float:
        self.window.append(rssi)
        return median(self.window)
```

**Media móvil vs Mediana:**

| Característica | Media Móvil | Mediana |
|---------------|-------------|---------|
| Complejidad computacional | O(n) simple | O(n log n) por sort |
| Rechazo de outliers | Débil (el outlier influye) | ✅ Fuerte (el outlier no influye si es < 50% de la ventana) |
| Respuesta a cambio gradual | Lenta pero continua | Similar |
| Respuesta a cambio brusco real | Lenta | Lenta (igual) |
| Ideal para | Ruido gaussiano | Ruido impulsivo |
| Uso recomendado en THLS | Primera pasada | Segunda pasada post-media |

**Recomendación:** Usar media móvil + mediana en cascada para el pipeline de producción:

```
RSSI crudo → Media Móvil (ventana 3) → Mediana (ventana 5) → Kalman
```

---

### 7.3.3 — Filtro de Kalman Simple para RSSI

El filtro de Kalman es el suavizador óptimo para señales con ruido gaussiano. Para señal RSSI, usamos la formulación 1D (escalar) sin modelo de movimiento explícito.

```python
# kalman_filter.py

class KalmanFilter1D:
    """
    Filtro de Kalman 1D para suavizado de señal RSSI.
    
    Modelo de estado:
        x_k = x_{k-1} + w_k       (estado: RSSI estimado, sin modelo de movimiento)
        z_k = x_k + v_k           (medición: RSSI observado)
    
    donde:
        w_k ~ N(0, Q)  — ruido de proceso (deriva natural de la señal)
        v_k ~ N(0, R)  — ruido de medición (varianza del RSSI)
    """

    def __init__(self, Q: float = 0.5, R: float = 5.0, initial_estimate: float = -80.0):
        """
        Args:
            Q: Ruido de proceso (process noise). 
               Valor recomendado: 0.3-1.0
               - Q bajo: el filtro confía más en el modelo previo (suavizado fuerte)
               - Q alto: el filtro responde más rápido a cambios reales
               
            R: Ruido de medición (measurement noise).
               Valor recomendado: 3.0-10.0 (varianza típica del RSSI en dBm²)
               - R bajo: el filtro confía más en las mediciones (menos suavizado)
               - R alto: el filtro confía menos en las mediciones (más suavizado)
               
            initial_estimate: Estimación inicial del RSSI (usar -80 como "neutro")
        """
        self.Q = Q                          # Ruido de proceso
        self.R = R                          # Ruido de medición
        self.x = initial_estimate           # Estimación de estado actual
        self.P = 1.0                        # Error de covarianza estimado

    def update(self, measurement: float) -> float:
        """
        Actualiza el filtro con una nueva medición RSSI.
        
        FASE DE PREDICCIÓN:
            x_pred = x                      (sin modelo de movimiento explícito)
            P_pred = P + Q                  (la incertidumbre crece con el tiempo)
        
        FASE DE ACTUALIZACIÓN:
            K = P_pred / (P_pred + R)       (ganancia de Kalman)
            x = x_pred + K * (z - x_pred)  (corrección)
            P = (1 - K) * P_pred            (actualización de covarianza)
        """
        # Fase de predicción
        x_pred = self.x
        P_pred = self.P + self.Q

        # Calcular ganancia de Kalman
        K = P_pred / (P_pred + self.R)

        # Fase de actualización (corrección)
        self.x = x_pred + K * (measurement - x_pred)
        self.P = (1 - K) * P_pred

        return self.x

    def reset(self, initial_estimate: float = -80.0):
        """Reiniciar el filtro (cambio de baliza objetivo, inicio de nueva misión)."""
        self.x = initial_estimate
        self.P = 1.0


# Configuraciones recomendadas por entorno:
KALMAN_CONFIGS = {
    'outdoor_open':     {'Q': 0.3, 'R': 3.0},   # Señal estable, suavizado fuerte
    'outdoor_urban':    {'Q': 0.5, 'R': 5.0},   # ✅ Configuración por defecto THLS
    'indoor_office':    {'Q': 0.7, 'R': 7.0},   # Más varianza, respuesta más ágil
    'indoor_concrete':  {'Q': 1.0, 'R': 10.0},  # Entorno muy ruidoso, suavizado intenso
}
```

**Interpretación práctica de Q y R:**

```
Ejemplo de convergencia:

Señal real estable a -72 dBm (jugador quieto, 8m de la baliza):
  Lecturas crudas: -68, -80, -71, -65, -78, -70, -72, -69, -74, -72

  Sin filtro:         promedio= -71.9, desv= ±4.8 dBm → estado oscila entre CALIENTE y TIBIO
  Media móvil (5):   ventana= [-70,-72,-69,-74,-72] → media= -71.4 → estado: CALIENTE ✅
  Kalman (Q=0.5,R=5): converge a -71.8 tras 5 lecturas → estado estable: CALIENTE ✅

Señal cambiando (jugador acercándose de 20m → 5m en 10 segundos):
  Lecturas crudas: -82, -81, -79, -76, -73, -71, -68, -65, -62, -58

  Sin filtro:         responde inmediatamente a cada cambio (ruidoso pero ágil)
  Media móvil (5):   ~2.5s de lag (ventana de 5 × 500ms scan rate)
  Kalman (Q=0.5,R=5): ~1.5s de lag, suavizado y sin ruido espúreo ✅
```

**¿Cuándo el Kalman es overkill y cuándo vale la pena?**

| Criterio | Usar Kalman | Usar solo Media Móvil |
|----------|:-----------:|:---------------------:|
| Entorno con ruido alto (interior, sótano) | ✅ | ❌ |
| POC rápido con recursos limitados de desarrollo | ❌ | ✅ |
| Señal muy variable (usuario moviéndose rápido) | ✅ | ⚠️ |
| App en dispositivo de gama baja (CPU limitada) | ⚠️ Evaluar | ✅ |
| Producción (v1.0 del juego) | ✅ Obligatorio | — |

---

### 7.3.4 — Rechazo de Outliers

Los outliers son lecturas RSSI estadísticamente imposibles o extremadamente improbables, causadas por interferencias puntuales, colisiones de paquetes BLE, o errores del hardware.

#### Método Z-Score (recomendado para THLS)

```python
# outlier_rejection.py
from statistics import mean, stdev
from collections import deque
from typing import Optional

class ZScoreOutlierRejector:
    """
    Rechaza lecturas RSSI que se desvían más de N desviaciones estándar
    de la media de la ventana reciente.
    
    Recomendación THLS: z_threshold = 2.0 (rechazar si |zscore| > 2σ)
    Esto mantiene el 95.4% de las lecturas de una distribución normal.
    """
    
    def __init__(self, window_size: int = 10, z_threshold: float = 2.0):
        self.window_size = window_size
        self.z_threshold = z_threshold
        self.window: deque = deque(maxlen=window_size)

    def is_valid(self, rssi: float) -> bool:
        """Retorna True si el valor RSSI es estadísticamente plausible."""
        # Hard limits: valores físicamente imposibles
        if rssi < -105 or rssi > -25:
            return False

        # Ventana insuficiente para calcular zscore: aceptar
        if len(self.window) < 3:
            self.window.append(rssi)
            return True

        mu = mean(self.window)
        sigma = stdev(self.window)

        if sigma < 0.1:  # Evitar división por cero si todos los valores son iguales
            self.window.append(rssi)
            return True

        z_score = abs(rssi - mu) / sigma

        if z_score > self.z_threshold:
            # Outlier detectado: no agregar a la ventana, retornar False
            return False

        self.window.append(rssi)
        return True


# Ejemplo:
# rejector = ZScoreOutlierRejector(window_size=10, z_threshold=2.0)
# if rejector.is_valid(raw_rssi):
#     smoothed = kalman.update(raw_rssi)
```

#### Método IQR (Interquartile Range)

```python
def reject_outliers_iqr(readings: list, iqr_factor: float = 1.5) -> list:
    """
    Rechaza valores fuera de [Q1 - 1.5*IQR, Q3 + 1.5*IQR].
    Más robusto que Z-score cuando la distribución es asimétrica.
    Útil para lotes de lecturas (post-procesamiento), no streaming.
    """
    if len(readings) < 4:
        return readings
    
    sorted_r = sorted(readings)
    n = len(sorted_r)
    q1 = sorted_r[n // 4]
    q3 = sorted_r[3 * n // 4]
    iqr = q3 - q1
    
    lower = q1 - iqr_factor * iqr
    upper = q3 + iqr_factor * iqr
    
    return [r for r in readings if lower <= r <= upper]
```

#### Hard Limits (siempre aplicados)

```python
# Constantes del protocolo THLS
RSSI_HARD_MIN = -105   # Por debajo de esto, el hardware no es fiable
RSSI_HARD_MAX = -25    # Por encima de esto, el beacon estaría a centímetros (imposible en campo)
RSSI_VALID_RANGE = range(RSSI_HARD_MIN, RSSI_HARD_MAX + 1)

def apply_hard_limits(rssi: float) -> Optional[float]:
    """Primer filtro: descartar valores físicamente imposibles."""
    if rssi < RSSI_HARD_MIN or rssi > RSSI_HARD_MAX:
        return None  # Descartar silenciosamente
    return rssi
```

---

### 7.3.5 — Histéresis para Transiciones de Estado

La histéresis previene el flickering de estados en zonas fronterizas del RSSI.

```typescript
// state-hysteresis.service.ts
import { Injectable } from '@angular/core';
import { SignalState } from './signal-state.enum';

@Injectable({ providedIn: 'root' })
export class StateHysteresisService {

    private consecutiveReadingsInNewState = 0;
    private currentState: SignalState = SignalState.NO_SIGNAL;
    private candidateState: SignalState | null = null;

    // ASIMÉTRICO: más fácil calentar (3) que enfriar (5)
    private readonly UPGRADE_THRESHOLD = 3;    // Para ir a estado "más caliente"
    private readonly DOWNGRADE_THRESHOLD = 5;  // Para ir a estado "más frío"

    processReading(rssi: number): SignalState {
        const rawState = this.rssiToRawState(rssi);

        if (rawState === this.currentState) {
            // Permanece en estado actual, resetear contador
            this.consecutiveReadingsInNewState = 0;
            this.candidateState = null;
            return this.currentState;
        }

        // Detectar si es upgrade (calentar) o downgrade (enfriar)
        const isUpgrade = this.isWarmerState(rawState, this.currentState);
        const threshold = isUpgrade ? this.UPGRADE_THRESHOLD : this.DOWNGRADE_THRESHOLD;

        if (rawState !== this.candidateState) {
            // Nuevo estado candidato distinto al anterior: reiniciar contador
            this.candidateState = rawState;
            this.consecutiveReadingsInNewState = 1;
        } else {
            this.consecutiveReadingsInNewState++;
        }

        if (this.consecutiveReadingsInNewState >= threshold) {
            // Transición confirmada
            this.currentState = rawState;
            this.candidateState = null;
            this.consecutiveReadingsInNewState = 0;
        }

        return this.currentState;
    }

    private rssiToRawState(rssi: number): SignalState {
        if (rssi > -55)  return SignalState.FOUND;
        if (rssi > -65)  return SignalState.VERY_HOT;
        if (rssi > -75)  return SignalState.HOT;
        if (rssi > -85)  return SignalState.WARM;
        if (rssi > -95)  return SignalState.COLD;
        return SignalState.NO_SIGNAL;
    }

    private isWarmerState(candidate: SignalState, current: SignalState): boolean {
        const order = [
            SignalState.NO_SIGNAL,
            SignalState.COLD,
            SignalState.WARM,
            SignalState.HOT,
            SignalState.VERY_HOT,
            SignalState.FOUND
        ];
        return order.indexOf(candidate) > order.indexOf(current);
    }
}
```

**Justificación del diseño asimétrico resumida:**

```
Escenario: Jugador a 8 metros de la baliza. RSSI oscila entre -73 y -77 dBm.
           La frontera CALIENTE/TIBIO está en -75 dBm.

Sin histéresis:
  Lecturas: -73(C) -77(T) -74(C) -76(T) -73(C)... → flickering constante

Con histéresis simétrica (3/3):
  -73(C) -77(T×1) -74(C→reset) -76(T×1) -73(C→reset)... → no transiciona nunca
  (malo: la oscilación impide que el estado cambie aunque el jugador sí se acerque)

Con histéresis asimétrica (3 upgrade / 5 downgrade):
  Si el jugador se acerca: 3 lecturas calientes → transiciona a CALIENTE en ~300ms ✅
  Si el jugador da un paso atrás: necesita 5 lecturas frías → ~500ms de gracia ✅
  Sensación: el estado "sube" con agilidad, "baja" con inercia → tensión sostenida ✅
```

---

### 7.3.6 — Confirmación por Múltiples Lecturas (Challenge Unlock)

La mecánica de captura de baliza requiere una confirmación estricta para prevenir activaciones accidentales y ataques de ingeniería de señal.

```typescript
// challenge-unlock.service.ts

const CHALLENGE_UNLOCK_CONFIG = {
    requiredConsecutiveReadings: 5,     // Lecturas consecutivas > -55 dBm
    maxTimeBetweenReadings: 500,        // ms máximo entre lecturas (scan rate)
    timeWindow: 3000,                   // ms ventana total de confirmación
    minimumRssiThreshold: -55           // dBm mínimo para cada lectura
};

class ChallengeUnlockValidator {
    private confirmationReadings: { rssi: number; timestamp: number }[] = [];

    addReading(rssi: number): ChallengeUnlockResult {
        const now = Date.now();

        // Limpiar lecturas fuera de la ventana temporal
        this.confirmationReadings = this.confirmationReadings.filter(
            r => now - r.timestamp < CHALLENGE_UNLOCK_CONFIG.timeWindow
        );

        if (rssi < CHALLENGE_UNLOCK_CONFIG.minimumRssiThreshold) {
            // Lectura insuficiente: resetear confirmación
            this.confirmationReadings = [];
            return { confirmed: false, progress: 0 };
        }

        this.confirmationReadings.push({ rssi, timestamp: now });

        const progress = this.confirmationReadings.length /
                        CHALLENGE_UNLOCK_CONFIG.requiredConsecutiveReadings;

        if (this.confirmationReadings.length >= CHALLENGE_UNLOCK_CONFIG.requiredConsecutiveReadings) {
            return {
                confirmed: true,
                progress: 1.0,
                averageRssi: this.confirmationReadings.reduce((s, r) => s + r.rssi, 0) /
                             this.confirmationReadings.length
            };
        }

        return { confirmed: false, progress };
    }
}
```

**¿Por qué 5 lecturas consecutivas y no 1?**

| Requisito | 1 lectura | 3 lecturas | 5 lecturas | 10 lecturas |
|-----------|:---------:|:----------:|:----------:|:-----------:|
| Probabilidad de activación accidental | Alta | Media | ✅ Baja | Muy baja |
| Latencia de activación legítima | ~100ms | ~300ms | ✅ ~500ms | ~1000ms |
| Vulnerabilidad a relay attack | Alta | Media | ✅ Reducida | Muy baja |
| UX (sensación de "conquista") | Trivial | Aceptable | ✅ Satisfactoria | Frustrante |

**5 lecturas a 100ms de scan rate = 500ms de permanencia en la zona < 5 metros.** Esto significa que el jugador debe quedarse quieto, con el teléfono apuntando hacia la baliza, durante medio segundo. Es prácticamente imperceptible para el usuario legítimo, pero elimina la posibilidad de activación por un rayo de señal reflejada o un paquete BLE de otro dispositivo que fortuitamente coincida con el UUID.

---

## 7.4 — Resumen del Pipeline Completo de Producción

```
┌─────────────────────────────────────────────────────────────┐
│                  PIPELINE COMPLETO THLS v1.0                │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  [1] BLE Event (advertisementreceived)                      │
│       └─ rssi: number, manufacturerData: DataView           │
│                                                             │
│  [2] Hard Limits Filter                                     │
│       └─ Descartar: rssi < -105 || rssi > -25               │
│                                                             │
│  [3] Z-Score Outlier Rejection (ventana 10, threshold 2σ)   │
│       └─ Descartar si |zscore| > 2.0                        │
│                                                             │
│  [4] Median Pre-filter (ventana 3)                          │
│       └─ Eliminar spikes puntuales antes del Kalman         │
│                                                             │
│  [5] Kalman Filter 1D (Q=0.5, R=5.0)                        │
│       └─ Estimación óptima del RSSI suavizado               │
│                                                             │
│  [6] State Machine con Histéresis Asimétrica                │
│       └─ Upgrade: 3 lecturas | Downgrade: 5 lecturas        │
│                                                             │
│  [7] Challenge Unlock Validator                             │
│       └─ 5 lecturas consecutivas > -55 dBm en 3s            │
│                                                             │
│  [8] UI Update + Haptics + Audio                            │
│       └─ 60 fps render | Vibration API | Web Audio API      │
│                                                             │
└─────────────────────────────────────────────────────────────┘

Latencia total del pipeline (estimada):
  Scan rate: 100ms (modo CHALLENGE) / 500ms (modo NORMAL)
  Procesamiento: <5ms por lectura (JavaScript single-threaded)
  Kalman warm-up: ~500ms (5 lecturas) desde inicio de escaneo
  Histéresis upgrade: ~300ms (3 lecturas a 100ms)
  Challenge unlock: ~500ms (5 lecturas a 100ms)

  Latencia total percibida (inicio scan → UI actualizada):
  Primera detección: ~600ms ← Kalman warm-up dominante
  Cambio de estado: ~300-500ms ← Histéresis dominante
  Activación de desafío: ~500ms ← Challenge validator dominante
  
  ✅ Todos dentro del umbral de percepción humana (<1000ms)
  ✅ Feedback háptico inmediato (no espera a renderización)
```

---

*Fin de las secciones 4, 5, 6 y 7 del Game Design Document.*
*Siguiente sección: Sección 8 — Sistema de Desafíos y Mecánicas de Captura.*

---

> **Documento preparado por:** Equipo de Arquitectura IoT & Game Design, THLS Project  
> **Versión del documento:** 1.0-DRAFT  
> **Fecha:** Junio 2026  
> **Próxima revisión:** Tras validación del POC físico con hardware ESP32-WROOM-32

