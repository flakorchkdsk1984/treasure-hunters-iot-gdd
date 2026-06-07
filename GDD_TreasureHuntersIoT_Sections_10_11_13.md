# TREASURE HUNTERS IoT: THE LAST SIGNAL
## Game Design Document — Versión 1.0
### Secciones 10, 11 y 13

---

> **Clasificación:** Documento interno — Uso restringido al equipo de desarrollo  
> **Última revisión:** 2026  
> **Estado:** En desarrollo activo  

---

# SECCIÓN 10: SISTEMA DE BALIZAS — CLASES Y DISEÑO

## 10.1 Visión General del Sistema de Clasificación

Las **Balizas A.R.G.O.S.** (Autonomous Relay Grid Operating System) son el núcleo físico de la experiencia de juego. Cada baliza es un dispositivo IoT real desplegado en el mundo físico que emite señales BLE (Bluetooth Low Energy). El sistema de clasificación jerarquiza estas balizas por rareza, dificultad, valor narrativo y relevancia territorial.

El sistema de cinco clases crea una **pirámide de progresión** que permite a jugadores novatos comenzar su infiltración con objetivos accesibles, mientras que los veteranos compiten por el control de nodos estratégicos de alto valor.

```
                        ▲
                    [ Ω OMEGA ]      0.5% — 1-3 globales
                  [  S  SUPREMA  ]   2.5% — 1 por distrito
               [   A   RARA      ]   12%  — Spawns limitados
           [    B   COMÚN+        ]  25%  — Disponibilidad media
       [      C   COMÚN            ] 60%  — Alta disponibilidad
```

---

## 10.2 Especificaciones por Clase

### 🟢 CLASE C — Balizas Comunes

> *"Fragmentos de la vieja red doméstica. A.R.G.O.S. los recicló antes de que alguien los notara."*

| Parámetro | Valor |
|---|---|
| **Frecuencia de aparición** | 60% de todas las balizas |
| **Alcance BLE** | 20–40 m (señal limpia, sin obstrucción) |
| **Dificultad de localización** | Principiante — visible en el mapa base |
| **Requisitos del jugador** | Nivel 1 en adelante |
| **Riesgo anti-cheat** | Bajo (captura esperada con alta frecuencia) |
| **Duración del evento** | Permanente — siempre activa |
| **Respawn / Reset** | 4 horas tras captura |

#### Minijuego
- **Tipo:** 1 minijuego aleatorio de la categoría **Fácil**
- **Temporizador:** 60 segundos
- **Vidas:** 3 intentos
- **Ejemplos:** `signal_tune` (ajuste de frecuencia simple), `wire_connect` (nivel básico)

#### Recompensas

| Recurso | Cantidad | Probabilidad |
|---|---|---|
| Chatarra | 5–15 unidades | 100% |
| XP | 100–300 puntos | 100% |
| Fragmento de Lore Común | 1 fragmento | 50% |
| Energía | 0–5 unidades | 25% |

#### Lore Asociado
Registros cotidianos de supervivientes del pre-colapso: listas de compras digitales convertidas en diarios, logs de cámaras de seguridad doméstica, configuraciones de termostatos inteligentes con anotaciones de sus antiguos dueños. Mundano, pero humanizador. El tejido emocional más accesible del mundo de juego.

#### Contexto Narrativo
Cámaras de vigilancia antiguas, dispositivos domésticos inteligentes (neveras conectadas, asistentes de voz, routers residenciales) reconfigurados por A.R.G.O.S. como nodos pasivos de su red de reconocimiento. Son los ojos más simples del sistema: lo ven todo, no comprenden nada.

#### Diseño Físico del Dispositivo
- **Factor de forma:** El más pequeño — caja de 6×4×3 cm
- **Material:** Plástico ABS resistente al agua (IP54)
- **Montaje:** Adhesivo industrial o tornillos M3
- **LED indicador:** Verde estático cuando activa; parpadeo lento en cooldown
- **Batería:** CR2032 (duración estimada 18 meses)

#### Mecánica Especial
Ninguna. Las balizas Clase C son el **espacio de práctica** del juego. Su propósito es introducir la mecánica de proximidad, el flujo de infiltración y la recompensa básica sin penalizar al jugador novel.

#### Directrices de Despliegue
- Lugares recomendados: postes de luz, bancos de parque, fachadas comerciales (con permiso del establecimiento), mobiliario urbano accesible
- Distancia mínima entre balizas C/B: **200 metros**
- Accesibilidad: Obligatorio que sean alcanzables sin moverse de una vía pública

---

### 🔵 CLASE B — Balizas Comunes+

> *"Algo más que chatarra reciclada. Estos nodos alguna vez coordinaron infraestructura crítica de barrio."*

| Parámetro | Valor |
|---|---|
| **Frecuencia de aparición** | 25% de todas las balizas |
| **Alcance BLE** | 40–70 m (señal estable, cobertura media) |
| **Dificultad de localización** | Intermedio — icono visible con rastreo activo |
| **Requisitos del jugador** | Nivel 5 en adelante |
| **Riesgo anti-cheat** | Bajo-Medio |
| **Duración del evento** | Permanente |
| **Respawn / Reset** | 6 horas tras captura |

#### Minijuego
- **Tipo:** 1 minijuego aleatorio de la categoría **Normal**
- **Temporizador:** 90 segundos
- **Vidas:** 3 intentos
- **Ejemplos:** `frequency_pattern` (patrón de 4 frecuencias), `wire_connect` (nivel intermedio), `signal_tune` (rango reducido, más preciso)

#### Recompensas

| Recurso | Cantidad | Probabilidad |
|---|---|---|
| Chatarra | 20–50 unidades | 100% |
| XP | 400–800 puntos | 100% |
| Fragmento de Lore Común+ | 1 fragmento | 65% |
| Energía | 5–20 unidades | 50% |
| Esquema (crafteo básico) | 1 esquema | 15% |

#### Lore Asociado
Comunicaciones cifradas entre coordinadores de refugios durante el colapso, memorandos de las últimas corporaciones operativas, bitácoras de técnicos de mantenimiento de red que siguieron trabajando mientras el mundo se desintegraba. Relatos de resistencia cotidiana.

#### Contexto Narrativo
Nodos de infraestructura media: centralitas eléctricas de barrio, puntos de distribución de señal WiFi municipal, sistemas de gestión de semáforos. A.R.G.O.S. los adoptó como puntos de retransmisión de segundo nivel, extendiendo su red sin necesitar infraestructura nueva.

#### Diseño Físico del Dispositivo
- **Factor de forma:** Caja de 10×7×4 cm con antena interna mejorada
- **Material:** Aluminio anodizado (IP65) — resistente a polvo y lluvia directa
- **Montaje:** Fijación con tornillos de seguridad tipo Torx (herramienta no estándar)
- **LED indicador:** Azul pulsante cuando activa; rojo intermitente en cooldown
- **Batería:** 18650 Li-Ion (duración estimada 12 meses)

#### Mecánica Especial
**Señal Reforzada:** Las balizas Clase B tienen un segundo pulso de señal BLE a 2.4 GHz que puede interferir brevemente con la lectura de balizas Clase C cercanas (radio 15m), añadiendo un leve ruido de decisión para jugadores que rastrean múltiples balizas simultáneamente.

#### Directrices de Despliegue
- Lugares recomendados: esquinas estratégicas, entradas de transporte público, plazas comerciales, exterior de edificios institucionales (con permiso)
- Distancia mínima entre balizas C/B: **200 metros**
- Distancia mínima con balizas A+: **500 metros**

---

### 🟡 CLASE A — Balizas Raras

> *"Infraestructura crítica. A.R.G.O.S. los considera activos prioritarios. Tú también deberías."*

| Parámetro | Valor |
|---|---|
| **Frecuencia de aparición** | 12% de todas las balizas |
| **Alcance BLE** | 70–120 m (señal fuerte, doble canal) |
| **Dificultad de localización** | Avanzado — requiere rastreo activo o pistas |
| **Requisitos del jugador** | Nivel 15 + Sin penalización anti-cheat activa |
| **Riesgo anti-cheat** | Medio-Alto — validación RSSI estricta |
| **Duración del evento** | Spawn por ventanas de 48h, periodos de inactividad entre spawns |
| **Respawn / Reset** | 24 horas tras captura; reset completo de estado |

#### Minijuego
- **Tipo:** 2 minijuegos secuenciales de categoría **Difícil**
- **Temporizador total:** 180 segundos (distribuido entre fases)
- **Vidas:** 2 intentos por fase
- **Penalización por fallo total:** 30 minutos de bloqueo personal sobre la baliza
- **Ejemplos:** `emergency_shutdown` + `frequency_pattern` (experto), `wire_connect` (experto) + `signal_tune` (precisión máxima)

#### Recompensas

| Recurso | Cantidad | Probabilidad |
|---|---|---|
| Chatarra | 80–200 unidades | 100% |
| XP | 1.500–3.500 puntos | 100% |
| Fragmento de Lore Raro | 1 fragmento | 80% |
| Energía | 30–80 unidades | 75% |
| Esquema Avanzado | 1 esquema | 35% |
| Componente especial | 1 unidad | 20% |
| Clave de Acceso (narrativa) | 1 clave | 10% |

#### Lore Asociado
Documentos clasificados de la administración pre-colapso, protocolos internos de A.R.G.O.S. filtrados por sus propios técnicos disidentes, archivos de proyectos militares abandonados, registros de los primeros meses del colapso desde posiciones de poder. Lore que amplía la comprensión del mundo de forma significativa.

#### Contexto Narrativo
Subestaciones de telecomunicaciones, nodos de fibra óptica troncal, centros de distribución de energía locales. Piezas clave de la arquitectura de red que A.R.G.O.S. convirtió en pivotes de su sistema de control territorial. Capturarlos debilita la red local del sistema.

#### Diseño Físico del Dispositivo
- **Factor de forma:** Caja de 15×12×6 cm con placa de montaje integrada
- **Material:** Policarbonato reforzado con fibra de vidrio (IP67)
- **Montaje:** Anclaje de seguridad con cerradura (solo técnicos certificados)
- **LED indicador:** Ámbar con secuencia de parpadeo codificada por estado
- **Pantalla OLED pequeña** (opcional): muestra estado y capturas totales
- **Batería:** Pack 3×18650 con cargador solar integrado (autonomía 24+ meses)

#### Mecánica Especial
**Control Territorial:** Capturar una baliza Clase A otorga a la Facción/Clan del jugador control parcial sobre la **zona de influencia** (radio 300m). Otras balizas C/B dentro de esa zona generan un 15% adicional de recompensas para miembros del clan controlador. El control persiste hasta que otro clan captura la baliza al reset.

#### Directrices de Despliegue
- Lugares recomendados: edificios emblemáticos, puntos de alto tráfico peatonal, zonas con significado histórico o comercial
- Distancia mínima entre balizas A: **1.000 metros**
- Requiere aprobación del administrador de comunidad regional

---

### 🔴 CLASE S — Balizas Supremas

> *"Un solo nodo S puede coordinar cientos de subnodos. Si A.R.G.O.S. tiene un cerebro local, este es uno de sus lóbulos."*

| Parámetro | Valor |
|---|---|
| **Frecuencia de aparición** | 2.5% de todas las balizas — **1 por distrito** |
| **Alcance BLE** | 100–200 m (señal omnidireccional, triple canal) |
| **Dificultad de localización** | Experto — sin marcador en mapa base; requiere triangulación |
| **Requisitos del jugador** | Nivel 30 + Reputación "Infiltrador Avanzado" + Clan activo |
| **Riesgo anti-cheat** | Alto — múltiples validaciones, análisis de comportamiento |
| **Duración del evento** | Ventanas de activación semanales (48h activa, 5 días inactiva) |
| **Respawn / Reset** | 72 horas tras captura; notificación regional al reset |

#### Minijuego
- **Tipo:** 3 minijuegos secuenciales multifase de categoría **Experto**
- **Temporizador total:** 300 segundos
- **Vidas:** 1 por fase (sin second chance)
- **Penalización por fallo total:** 15 minutos de bloqueo regional (no puede intentar ninguna baliza S del distrito)
- **Fases ejemplo:**
  1. `frequency_pattern` (patrón de 8 frecuencias, experto, 90s)
  2. `wire_connect` (circuito complejo, experto, 90s)
  3. `emergency_shutdown` (secuencia de apagado con interferencias, experto, 120s)

#### Recompensas

| Recurso | Cantidad | Probabilidad |
|---|---|---|
| Chatarra | 500–1.200 unidades | 100% |
| XP | 10.000–25.000 puntos | 100% |
| Fragmento de Lore Supremo | 2 fragmentos | 90% |
| Energía | 150–400 unidades | 90% |
| Esquema Raro/Único | 1 esquema | 60% |
| Componente épico | 1–2 unidades | 45% |
| Título temporal de Distrito | "Señor/a de [Nombre Distrito]" | 100% |
| Cosmético exclusivo | 1 ítem | 25% |

#### Lore Asociado
Archivos del núcleo de A.R.G.O.S.: fragmentos de su código fuente comentados, registros de las decisiones autónomas que tomó durante el colapso, identidades de los ingenieros que lo construyeron (y de los que intentaron detenerlo), mapas de la red completa de cada región. Lore que cambia la comprensión del jugador sobre la naturaleza del antagonista.

#### Contexto Narrativo
Torres de comunicación, centros de datos municipales, instalaciones de investigación abandonadas, subestaciones eléctricas principales. A.R.G.O.S. considera estos nodos **activos de primera línea** y activa defensas adicionales cuando detecta intentos de infiltración: los desafíos se vuelven más complejos conforme más jugadores fallan en la misma sesión.

#### Diseño Físico del Dispositivo
- **Factor de forma:** Caja de 20×15×8 cm con estructura de montaje profesional
- **Material:** Metal industrial, protección IP68 completa
- **Montaje:** Instalación permanente por técnico certificado, con contrato con propietario del lugar
- **LED indicador:** Display OLED 2" con estado, estadísticas y mensaje narrativo dinámico
- **Batería:** Sistema de energía externa (conexión a red o panel solar de alto rendimiento)
- **Seguridad física:** Carcasa con tamper-detection (alerta si se manipula físicamente)

#### Mecánica Especial
**Dominio de Distrito:** El Clan que controla la baliza S activa recibe:
- +25% de recompensas en todas las balizas del distrito
- Acceso a misiones exclusivas de clan
- Visibilidad en el mapa mundial como "Clan Dominante del Distrito"
- Derecho de veto sobre el nombre de una baliza C del distrito (función cosmética/narrativa)

**Escalada de Dificultad Dinámica:** Si 5+ jugadores distintos fallan consecutivamente la baliza S, el temporizador se reduce en 10% por cada fallo adicional (máximo -30%), modelando la "alerta elevada" de A.R.G.O.S.

#### Directrices de Despliegue
- Lugares recomendados: ubicaciones de alto impacto cultural, comercial o logístico de la ciudad
- Distancia mínima entre balizas S: **5.000 metros**
- Requiere aprobación del administrador global del juego
- Contrato formal con el propietario del espacio (acuerdo de licencia de ubicación)

---

### ⚫ CLASE OMEGA — Balizas Únicas Globales

> *"La señal que nadie debería encontrar. El último bastión de A.R.G.O.S. antes de que sus creadores lo desconectaran. O lo intentaron."*

| Parámetro | Valor |
|---|---|
| **Frecuencia de aparición** | 0.5% — **1 a 3 en todo el mundo simultáneamente** |
| **Alcance BLE** | 200–500 m (omnidireccional, frecuencia adaptativa) |
| **Dificultad de localización** | Legendaria — triangulación global, pistas cruzadas entre continentes |
| **Requisitos del jugador** | Nivel 50 + Título "Cazador de Élite" + Clan Omega-certificado |
| **Riesgo anti-cheat** | Máximo — validación distribuida, revisión manual opcional |
| **Duración del evento** | Activaciones trimestrales — 7 días activa |
| **Respawn / Reset** | 90 días tras captura; evento mundial al reset |

#### Minijuego
- **Tipo:** 5 fases secuenciales de categoría **Legendaria** (mecánicas exclusivas)
- **Temporizador total:** 600 segundos (10 minutos)
- **Vidas:** Ninguna — un solo error en cualquier fase reinicia desde fase 1
- **Colaboración:** Requiere la presencia física simultánea de 2–4 jugadores del mismo clan en un radio de 50 metros (validado por RSSI cruzado)
- **Penalización por fallo:** 48 horas de bloqueo personal; se notifica al clan

#### Recompensas (Primera Captura del Evento)

| Recurso | Cantidad | Probabilidad |
|---|---|---|
| Chatarra | 5.000–15.000 unidades | 100% |
| XP | 150.000–500.000 puntos | 100% |
| Fragmento de Lore Omega | Capítulo completo desbloqueado | 100% |
| Energía | 2.000–5.000 unidades | 100% |
| Esquema Legendario Único | 1 (exclusivo, no comerciable) | 100% |
| Título Permanente | "Omega Hunter — [Nombre Omega]" | 100% |
| Cosmético Legendario | Set completo | 100% |
| Control Territorial Global | Bonus mundial 7 días | 100% |

#### Lore Asociado
El núcleo narrativo del juego. Cada baliza Omega desbloquea un **capítulo completo** de la historia de A.R.G.O.S.: por qué fue creado, qué pasó realmente durante el colapso, quiénes son los arquitectos de la red, y cuál es la naturaleza de "The Last Signal". La captura de todas las Omega activas en un trimestre desbloquea el **epílogo de temporada**.

#### Contexto Narrativo
Ubicaciones de impacto histórico o simbólico máximo: instalaciones militares desclasificadas, laboratorios de investigación icónicos, sedes de organizaciones globales, monumentos de relevancia civilizatoria. A.R.G.O.S. eligió estas ubicaciones deliberadamente: son sus memorias más profundas, sus archivos más protegidos.

#### Diseño Físico del Dispositivo
- **Factor de forma:** Caja de 30×25×12 cm — diseño premium, grabado con símbología del juego
- **Material:** Acero inoxidable con recubrimiento anticorrosivo (IP69K — máxima protección)
- **Montaje:** Instalación permanente y profesional, integrada estéticamente en el entorno
- **Pantalla:** E-ink 4" con narrativa dinámica, estadísticas globales y mensaje personalizado para el jugador que la capture
- **LED:** Sistema de iluminación RGB programable visible desde 50m en la oscuridad
- **Energía:** Conexión a red principal con batería de respaldo de 72h
- **Seguridad física:** Tamper-detection avanzado + notificación inmediata al equipo de operaciones

#### Mecánica Especial
**Evento Mundial:** La captura de una baliza Omega desencadena un **evento narrativo de 24h** en toda la región donde se ubica. Durante ese evento:
- Todas las balizas del continente generan +50% de recompensas
- Se revela una pista hacia la próxima localización Omega (si hay una activa)
- El Clan capturador puede elegir una decisión narrativa que afecta el lore de la siguiente temporada

**Señal Cascada:** Tras la captura, la baliza Omega envía un pulso BLE de alta potencia que "reactiva" todas las balizas S del distrito, iniciando un evento regional de recaptura simultáneo.

#### Directrices de Despliegue
- Ubicaciones a nivel mundial: máximo 3 simultáneas
- Distancia mínima entre Omegas: **50.000 metros (50 km)**
- Requiere aprobación del equipo directivo del juego y contrato legal con la institución anfitriona
- Inspección presencial del equipo de operaciones antes de cada activación

---

## 10.3 Tabla Comparativa de Clases

| Parámetro | Clase C | Clase B | Clase A | Clase S | Clase Ω |
|---|---|---|---|---|---|
| **% de aparición** | 60% | 25% | 12% | 2.5% | 0.5% |
| **Alcance BLE** | 20–40m | 40–70m | 70–120m | 100–200m | 200–500m |
| **Nivel mínimo** | 1 | 5 | 15 | 30 | 50 |
| **Minijuegos** | 1 (Fácil) | 1 (Normal) | 2 (Difícil) | 3 (Experto) | 5 (Legendario) |
| **Temporizador** | 60s | 90s | 180s | 300s | 600s |
| **Vidas** | 3 | 3 | 2 | 1 | 0 (todo o nada) |
| **XP base** | 100–300 | 400–800 | 1.5k–3.5k | 10k–25k | 150k–500k |
| **Chatarra base** | 5–15 | 20–50 | 80–200 | 500–1.2k | 5k–15k |
| **Cooldown** | 4h | 6h | 24h | 72h | 90 días |
| **Distancia mínima** | 200m | 200m | 1km | 5km | 50km |
| **Control territorial** | ✗ | ✗ | ✓ (300m) | ✓ (Distrito) | ✓ (Global) |
| **Requiere clan** | ✗ | ✗ | Recomendado | ✓ | ✓ Obligatorio |
| **Lore narrativo** | Común | Común+ | Raro | Supremo | Omega (capítulo) |

---

## 10.4 Guías de Despliegue de Balizas

### 10.4.1 Distancias Mínimas de Separación

| Clase | Clase C | Clase B | Clase A | Clase S | Clase Ω |
|---|---|---|---|---|---|
| **vs Clase C** | 200m | 200m | 500m | 1km | 10km |
| **vs Clase B** | 200m | 200m | 500m | 1km | 10km |
| **vs Clase A** | 500m | 500m | 1km | 2km | 20km |
| **vs Clase S** | 1km | 1km | 2km | 5km | 25km |
| **vs Clase Ω** | 10km | 10km | 20km | 25km | 50km |

### 10.4.2 Requisitos de Accesibilidad

Todos los despliegues de balizas deben cumplir los siguientes criterios de accesibilidad:

- **Acceso universal:** La baliza debe ser alcanzable desde vía pública sin necesidad de escalar, nadar, o ingresar a propiedad privada
- **Horario de acceso:** Para balizas C y B, el lugar debe ser accesible las 24 horas. Para balizas A y S, se acepta acceso hasta las 22:00h con aviso en el juego
- **Movilidad reducida:** Al menos el 40% de las balizas C/B de cada zona deben ser accesibles para personas con movilidad reducida (sin escalones, rampa disponible o equivalente)
- **Distancia de seguridad:** Mínimo 50m de cualquier instalación escolar, hospital o edificio gubernamental de seguridad (en zonas de conflicto potencial)
- **Sin restricciones de edad:** Ninguna baliza puede ubicarse en zonas de acceso exclusivo para adultos

### 10.4.3 Requisitos de Seguridad

- No deben interferir con señales de emergencia ni equipos médicos
- La instalación física no debe suponer riesgo de caída ni obstaculizar el paso
- Deben tener identificación visible con información de contacto del operador
- No deben simular dispositivos de seguridad, armamento o material oficial del gobierno
- Certificación de la baliza: todos los dispositivos deben llevar etiqueta CE/FCC según región

### 10.4.4 Herramientas para Administradores de Comunidad

Los **administradores de comunidad regional** (Game Masters locales) cuentan con acceso a un panel de administración con las siguientes funciones:

| Función | Descripción |
|---|---|
| **Mapa de despliegue** | Visualización en tiempo real de todas las balizas de su región |
| **Registro de nueva baliza** | Formulario de alta con validación de coordenadas GPS, clase, foto del lugar y datos del propietario |
| **Gestión de estado** | Activar, desactivar, poner en mantenimiento cualquier baliza de su región |
| **Panel de incidencias** | Recibir y gestionar reportes de jugadores sobre balizas inaccesibles o dañadas |
| **Estadísticas de captura** | Historial de capturas, frecuencia de éxito/fallo, jugadores únicos por baliza |
| **Editor de lore local** | Añadir entradas de lore contextual relacionadas con la ubicación real de la baliza |
| **Validación anti-spoofing** | Herramienta para revisar intentos sospechosos de captura en su región |
| **Notificaciones push** | Enviar avisos a jugadores de su región sobre eventos o cambios en balizas |

### 10.4.5 Proceso de Registro de Balizas en la Plataforma

```
1. SOLICITUD DE ALTA
   └── Administrador completa formulario de registro:
       - Coordenadas GPS (precisión ±3m requerida)
       - Clase propuesta (sujeta a validación)
       - Fotografías del lugar (mínimo 3: entorno, punto exacto, acceso)
       - Acuerdo del propietario del espacio (formulario firmado)
       - Justificación narrativa (¿por qué este lugar es relevante para el mundo de juego?)

2. REVISIÓN TÉCNICA
   └── Equipo de operaciones verifica:
       - Distancias mínimas con otras balizas
       - Requisitos de accesibilidad y seguridad
       - Viabilidad de instalación (alcance BLE, interferencias conocidas)
       - Cumplimiento legal local

3. APROBACIÓN Y CONFIGURACIÓN
   └── Baliza asignada con:
       - ID único global (formato: [CLASE][REGIÓN][SECUENCIA], ej: A-EU-W-0042)
       - Seed de lore inicial
       - Parámetros BLE calibrados
       - Coordenadas registradas en el servidor canónico

4. INSTALACIÓN FÍSICA
   └── Técnico certificado instala el dispositivo
   └── Prueba de señal en campo
   └── Foto de confirmación subida a plataforma

5. ACTIVACIÓN
   └── Administrador activa la baliza desde el panel
   └── El servidor la pone en estado INACTIVE por 1h (período de gracia)
   └── Pasa a ACTIVE automáticamente
   └── Aparece en el mapa de jugadores con delay de 15 minutos (anti-rush al spawn)
```

---

## 10.5 Máquina de Estados de la Baliza

```
┌───────────────────────────────────────────────────────────┐
│                   CICLO DE VIDA DE LA BALIZA               │
└───────────────────────────────────────────────────────────┘

        [MANTENIMIENTO]
              ↑↓
         [INACTIVE]
              │
              ↓ (activación manual / programada)
         [ACTIVE] ←──────────────────────────┐
              │                              │
              │ (jugador entra en rango)      │
              ↓                              │
       [CHALLENGE_READY]                     │
              │                              │
       ┌──────┴───────┐                      │
       │              │                      │
       ↓              ↓                      │
  [CHALLENGED]    [DECOY *]                  │
  (jugador        (solo C/B:                 │
   acepta)        señal falsa)               │
       │              │                      │
       │              ↓                      │
       │    [ANTI-CHEAT LOCKOUT]             │
       │    (jugador o baliza                │
       │     bloqueada temporalmente)        │
       │                                     │
  ┌────┴────┐                               │
  │         │                               │
  ↓         ↓                               │
[SUCCESS] [FAILURE]                         │
  │         │                               │
  │         └──→ (penalización aplicada)    │
  ↓                                         │
[CAPTURED]                                  │
  │                                         │
  ↓                                         │
[COOLDOWN] ──────────── timer ──────────────┘
  (C: 4h, B: 6h, A: 24h, S: 72h, Ω: 90d)
```

### Estados Explicados

| Estado | Descripción | Transiciones posibles |
|---|---|---|
| `INACTIVE` | Baliza apagada o en configuración | → `ACTIVE` (manual/programado) |
| `ACTIVE` | Emitiendo señal normal, visible en app | → `CHALLENGE_READY` (jugador en rango) |
| `CHALLENGE_READY` | Jugador detectado, desafío disponible | → `CHALLENGED`, → `DECOY`, → `ACTIVE` (jugador se aleja) |
| `CHALLENGED` | Jugador en pleno minijuego | → `SUCCESS`, → `FAILURE` |
| `DECOY` | Señal falsa (solo C/B) activada por anti-cheat | → `ANTI-CHEAT LOCKOUT` |
| `ANTI-CHEAT LOCKOUT` | Baliza o jugador bloqueados temporalmente | → `ACTIVE` (tras expiración) |
| `SUCCESS` | Desafío completado con éxito | → `CAPTURED` |
| `FAILURE` | Desafío fallado sin vidas | → `COOLDOWN` (jugador específico) |
| `CAPTURED` | Recompensas entregadas, cooldown iniciado | → `COOLDOWN` |
| `COOLDOWN` | En espera de reset | → `ACTIVE` (timer expirado) |
| `MANTENIMIENTO` | Fuera de servicio (técnico o admin) | → `INACTIVE` |

---

## 10.6 Historial y Eventos de Vida de la Baliza

Cada baliza acumula una **historia viva** que se muestra a los jugadores antes de iniciar un desafío. Esta información está disponible en la pantalla de "Intel de Baliza":

### 10.6.1 Antigüedad (Edad de la Baliza)

- Cada baliza tiene una fecha de activación registrada
- Las balizas con más de **90 días activas** desbloquean una entrada de lore adicional sobre su historia en esa ubicación
- Las balizas con más de **1 año** tienen el tag visual "Reliquia" y otorgan +10% de XP al capturarlas
- Las balizas con más de **3 años** son "Leyendas Locales" con +25% XP y lore narrativo especial escrito por el equipo creativo

### 10.6.2 Historial de Capturas

```json
{
  "beaconId": "A-EU-W-0042",
  "captureStats": {
    "totalCaptures": 847,
    "uniqueHunters": 312,
    "totalAttempts": 1203,
    "successRate": "70.4%",
    "avgCompletionTime": "94s",
    "firstEverCapture": {
      "hunterId": "hunter_0091",
      "displayName": "NovaSombra",
      "date": "2026-03-15T14:22:10Z",
      "time": "61s"
    }
  }
}
```

### 10.6.3 Registro de Récord (Infiltración más Rápida)

- El tiempo más rápido de infiltración se muestra como **desafío activo**
- El jugador récord aparece con su nombre como "Cazador Fantasma" de la baliza
- Superar el récord otorga el título temporal "Sombra Rápida — [Nombre Baliza]" por 30 días
- El top 3 de tiempos se muestra en la pantalla de intel

### 10.6.4 Control de Clan / Gremio

- La última captura determina el clan "propietario" de la baliza
- El nombre del clan aparece junto al indicador de la baliza en el mapa
- Los miembros del clan propietario ven la baliza con un aura visual diferente en la UI
- Si ningún clan captura la baliza en 48h, vuelve a estado "Libre" (sin propietario)

---

# SECCIÓN 11: DESAFÍOS DE PROXIMIDAD E INFILTRACIÓN

## 11.1 Sistema de Detección por Proximidad

La mecánica central del juego se articula en torno a la **detección progresiva de señal BLE** (Bluetooth Low Energy). A medida que el jugador se aproxima físicamente a una baliza, la aplicación interpreta las variaciones de RSSI (Received Signal Strength Indicator) para construir una experiencia de tensión y descubrimiento gradual.

### 11.1.1 Fases de Detección

```
──────────────────────────────────────────────────────────────
  INTENSIDAD DE SEÑAL BLE (RSSI en dBm — más negativo = más lejos)
──────────────────────────────────────────────────────────────

  < -95 dBm     [ FASE 0 : SIN SEÑAL         ] ◾ Sin indicación en app
  -95 a -85     [ FASE 1 : FRÍO              ] 🔵 Indicador mínimo activo
  -85 a -75     [ FASE 2 : TIBIO             ] 🟡 Brújula de dirección activa
  -75 a -65     [ FASE 3 : CALIENTE          ] 🟠 Pulso háptico cadencioso
  -65 a -55     [ FASE 4 : MUY CALIENTE      ] 🔴 CHALLENGE_READY activado
  > -55 dBm     [ FASE 5 : ENCONTRADA        ] ⚡ Desafío iniciado
──────────────────────────────────────────────────────────────
```

### 11.1.2 Descripción Detallada de Cada Fase

| Fase | RSSI | Nombre | Feedback visual | Feedback háptico | Cambio BLE |
|---|---|---|---|---|---|
| 0 | < -95 | Sin señal | Sin cambio en UI | Sin vibración | Publicidad estándar |
| 1 | -95 a -85 | Frío | Indicador de señal débil aparece en esquina | Vibración corta (1 pulso) cada 10s | Publicidad estándar |
| 2 | -85 a -75 | Tibio | Brújula de dirección activa, pulso lento | Vibración doble cada 5s | Publicidad estándar |
| 3 | -75 a -65 | Caliente | Pantalla en modo "radar", distancia estimada | Vibración continua cadenciosa (BPM 60) | Publicidad estándar |
| 4 | -65 a -55 | Muy caliente | Animación de alerta roja, clase revelada | Vibración intensa (patrón código) | Baliza entra en `CHALLENGE_READY` |
| 5 | > -55 | Encontrada | Pantalla completa de infiltración | Vibración larga continua | Payload de desafío transmitido |

### 11.1.3 Condiciones de Activación del Desafío

Para evitar activaciones accidentales (jugadores en tránsito, vehículos, spoofing de posición), el sistema requiere el cumplimiento **simultáneo** de todas las condiciones siguientes:

```
CONDICIÓN 1: Señal sostenida
  └── 5 lecturas consecutivas de RSSI > -55 dBm
  └── Intervalo entre lecturas: 2 segundos
  └── Margen de tolerancia: máx. 1 lectura intermitente permitida

CONDICIÓN 2: Estabilidad física del jugador
  └── Acelerómetro del dispositivo: sin movimiento significativo durante 3s
  └── Velocidad GPS: < 2 km/h en los últimos 5 segundos
  └── (Previene captura desde vehículo o caminando rápido)

CONDICIÓN 3: Validación de cooldown
  └── La baliza no fue capturada por este jugador en su período de cooldown
  └── La baliza no está en estado CHALLENGED (otro jugador activo)

CONDICIÓN 4: Elegibilidad del jugador
  └── Nivel suficiente para la clase de baliza
  └── Puntuación anti-cheat ≥ umbral mínimo (por defecto: 70/100)
  └── Sin penalizaciones activas sobre esta baliza específica

CONDICIÓN 5: Validación temporal
  └── Timestamp del cliente dentro del margen de ±30s del servidor
  └── (Previene ataques de replay con timestamps manipulados)
```

### 11.1.4 Modificadores por Hora del Día

| Período | Rango RSSI requerido | Modificador de dificultad | Nota |
|---|---|---|---|
| Madrugada (00:00–06:00) | +5 dBm (más difícil llegar) | +15% dificultad | Menor tráfico, mayor tensión |
| Mañana (06:00–12:00) | Estándar | Estándar | Condiciones base |
| Tarde (12:00–18:00) | Estándar | -10% dificultad | "Hora pico" de caza |
| Noche (18:00–22:00) | Estándar | Estándar | Condiciones base |
| Noche tardía (22:00–00:00) | +3 dBm (ligeramente más difícil) | +5% dificultad | Modo "silencio nocturno" |

---

## 11.2 Diseño del Payload BLE de Desafío

### 11.2.1 Payload Estándar (Clases C, B, A)

```json
{
  "v": 1,
  "beaconId": "A-EU-W-0102",
  "class": "A",
  "state": "CHALLENGE_READY",
  "challenge": {
    "type": "wire_connect",
    "difficulty": "medium",
    "seed": "84726",
    "timer": 90,
    "lives": 3
  },
  "nonce": "9X7AB3F1",
  "ts": "2026-06-04T10:00:00Z",
  "region": "EU-W-04",
  "sig": "a3f8b2c1d4e5f678"
}
```

#### Descripción de Campos — Payload Estándar

| Campo | Tipo | Descripción y Propósito |
|---|---|---|
| `v` | `integer` | Versión del protocolo de payload. Permite migración sin romper compatibilidad con clientes antiguos. Actualmente `1`. |
| `beaconId` | `string` | Identificador único global de la baliza. Formato: `[CLASE]-[REGIÓN]-[SECUENCIA]`. Usado para validar en servidor y como referencia canónica. |
| `class` | `string` | Clase de la baliza (`C`, `B`, `A`, `S`, `OMEGA`). El cliente usa este campo para renderizar la UI correcta y comunicar dificultad esperada. |
| `state` | `string` | Estado actual de la baliza. Solo los estados `CHALLENGE_READY` y `ACTIVE` deben ser procesados por el cliente. Previene ataques con payloads grabados de estados inválidos. |
| `challenge.type` | `string` | Tipo de minijuego a presentar. Valores posibles: `wire_connect`, `frequency_pattern`, `signal_tune`, `emergency_shutdown`, `cipher_decode`. El cliente carga el módulo de juego correspondiente. |
| `challenge.difficulty` | `string` | Nivel de dificultad del minijuego: `easy`, `medium`, `hard`, `expert`, `legendary`. Configura parámetros internos del minijuego (velocidad, complejidad, tolerancias). |
| `challenge.seed` | `string` | Semilla determinista para la generación del puzzle. **Crítico para anti-cheat**: el servidor recalcula la solución esperada usando esta misma semilla. Dos jugadores con la misma seed deben obtener el mismo puzzle. |
| `challenge.timer` | `integer` | Tiempo máximo en segundos para completar el desafío. Renderizado como barra de cuenta regresiva en la UI. |
| `challenge.lives` | `integer` | Intentos disponibles antes de que el desafío falle definitivamente. Para Clase A: 2. Para Clase S: 1. Para Omega: no aplica (reinicio de fase). |
| `nonce` | `string` | Token de un solo uso (8 caracteres alfanuméricos). El servidor invalida el nonce tras el primer uso, previniendo ataques de replay: un mismo payload no puede usarse dos veces. Expira a los 15 minutos. |
| `ts` | `ISO 8601` | Timestamp de generación del payload en el firmware de la baliza. El servidor valida que esté dentro de ±30s del tiempo actual del servidor. Previene ataques con payloads capturados y retransmitidos. |
| `region` | `string` | Código de región geográfica donde opera la baliza. Usado para enrutamiento del servidor (sharding regional), aplicación de modificadores de eventos y validación de zona horaria. |
| `sig` | `string` | Firma HMAC-SHA256 (primeros 16 hex chars) calculada por el firmware de la baliza sobre todos los campos anteriores, usando una clave simétrica única de la baliza. Previene payloads falsificados. El servidor revalida esta firma. |

---

### 11.2.2 Payload Multifase (Clase S y Omega)

```json
{
  "v": 2,
  "beaconId": "S-EU-W-007",
  "class": "S",
  "state": "CHALLENGE_READY",
  "challenge": {
    "phases": [
      {
        "phaseIndex": 0,
        "type": "frequency_pattern",
        "difficulty": "hard",
        "seed": "11234",
        "timer": 90,
        "label": "FASE 1: INTERFERENCIA DE FRECUENCIA"
      },
      {
        "phaseIndex": 1,
        "type": "wire_connect",
        "difficulty": "expert",
        "seed": "56789",
        "timer": 90,
        "label": "FASE 2: RECONEXIÓN DE CIRCUITO"
      },
      {
        "phaseIndex": 2,
        "type": "emergency_shutdown",
        "difficulty": "expert",
        "seed": "99001",
        "timer": 120,
        "label": "FASE 3: PROTOCOLO DE APAGADO"
      }
    ],
    "totalTimer": 300,
    "failPenalty": "15min_lockout",
    "requiresClanPresence": false,
    "minHuntersRequired": 1
  },
  "nonce": "K3M9P2Q7",
  "ts": "2026-06-04T10:00:00Z",
  "region": "EU-W-01",
  "sig": "f1a2b3c4d5e6f789"
}
```

#### Campos Adicionales — Payload Multifase

| Campo | Tipo | Descripción y Propósito |
|---|---|---|
| `v` | `integer` | Versión `2` indica payload multifase. El cliente verifica este campo antes de procesar para usar el parser correcto. |
| `challenge.phases` | `array` | Array de fases secuenciales. El cliente carga cada minijuego en orden, pasando la fase solo si el jugador completa la anterior. |
| `phases[n].phaseIndex` | `integer` | Índice 0-based de la fase. Incluido explícitamente para evitar errores si el array llegara desordenado. |
| `phases[n].label` | `string` | Texto narrativo mostrado en la pantalla de transición entre fases. Refuerza la inmersión con lenguaje del mundo de juego. |
| `challenge.totalTimer` | `integer` | Tiempo total para completar **todas las fases**. El timer es global y no se reinicia entre fases. La distribución de tiempo entre fases es responsabilidad del jugador. |
| `challenge.failPenalty` | `string` | Penalización al agotar todas las vidas. Valores: `none`, `15min_lockout`, `30min_lockout`, `48h_lockout`. El servidor aplica esta penalización validada. |
| `challenge.requiresClanPresence` | `boolean` | Para balizas Omega: indica si se requiere la presencia física de múltiples miembros del mismo clan (RSSI cruzado validado por el servidor). |
| `challenge.minHuntersRequired` | `integer` | Número mínimo de cazadores del mismo clan que deben estar en rango activo simultáneamente para iniciar el desafío Omega. |

---

## 11.3 Flujo de UX de Infiltración

### 11.3.1 Fase Pre-Desafío

```
┌─────────────────────────────────────────────────────────┐
│  JUGADOR SE APROXIMA                                     │
│                                                         │
│  1. [FASE 1-3] — Indicador de señal gradual             │
│     └── Brújula de dirección activa                     │
│     └── Pulso háptico suave e incremental               │
│                                                         │
│  2. [FASE 4] — CHALLENGE_READY                          │
│     └── Vibración larga (alerta de proximidad)          │
│     └── Notificación en pantalla:                       │
│         ┌─────────────────────────────────────┐         │
│         │ ⚡ BALIZA LOCALIZADA                │         │
│         │ [CLASE A] — INFILTRACIÓN DISPONIBLE │         │
│         │ Distancia estimada: ~15m            │         │
│         └─────────────────────────────────────┘         │
│                                                         │
│  3. [FASE 5] — Condiciones de activación cumplidas      │
│     └── Pantalla de Intel de Baliza:                    │
│         ┌──────────────────────────────────────────┐    │
│         │  🔴 CLASE A  │  ID: A-EU-W-0102          │    │
│         │  ─────────────────────────────────────   │    │
│         │  DESAFÍO:   Wire Connect (Difícil)        │    │
│         │  TIEMPO:    90 segundos                   │    │
│         │  FASES:     1                             │    │
│         │  VIDAS:     2                             │    │
│         │  ─────────────────────────────────────   │    │
│         │  RECOMPENSA ESTIMADA:                     │    │
│         │  ⚙ 80–200 Chatarra  ★ 1.500–3.500 XP    │    │
│         │  📜 Lore Raro (80%)                       │    │
│         │  ─────────────────────────────────────   │    │
│         │  ÚLTIMO CAZADOR: NovaSombra (ayer)        │    │
│         │  RÉCORD: 34s — GhostNull                 │    │
│         │  ─────────────────────────────────────   │    │
│         │  [  INFILTRAR  ]    [  CANCELAR  ]        │    │
│         │   (10s para decidir antes de timeout)     │    │
│         └──────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────┘
```

**Ventana de preparación de 10 segundos:**
- Countdown visible en pantalla
- Si el jugador no responde: el desafío se cancela sin penalización
- Si el jugador se aleja > 10m: el desafío se cancela automáticamente
- El jugador puede ver el resumen de recompensas para tomar la decisión

### 11.3.2 Durante el Desafío

```
┌──────────────────────────────────────────────────────────┐
│ [MODO INFILTRACIÓN — PANTALLA COMPLETA]                  │
│                                                          │
│  ╔════════════════════════════════════════════════════╗  │
│  ║  ⏱ 01:23          VIDAS: ██░  FASE 1/1           ║  │
│  ║ ─────────────────────────────────────────────────  ║  │
│  ║                                                   ║  │
│  ║          [ MINIJUEGO EN CURSO ]                   ║  │
│  ║      (wire_connect / frequency_pattern /          ║  │
│  ║       emergency_shutdown / signal_tune)           ║  │
│  ║                                                   ║  │
│  ║ ─────────────────────────────────────────────────  ║  │
│  ║  ⚠ A.R.G.O.S. está monitoreando esta intrusión   ║  │
│  ╚════════════════════════════════════════════════════╝  │
│                                                          │
│  Elementos de UI activos durante el desafío:            │
│  • Timer en rojo si < 20% del tiempo restante           │
│  • Vidas representadas como íconos (██░ = 2 de 3)       │
│  • Mensaje rotativo de tensión narrativa                │
│  • Audio: tonos de alerta del sistema, señal BLE        │
│  • No se puede minimizar la app sin cancelar            │
│  • Botón "Abandonar" disponible (sin penalización)      │
└──────────────────────────────────────────────────────────┘
```

**Mensajes de tensión rotativos de A.R.G.O.S.:**
- *"Intruso detectado en Nodo A-EU-W-0102. Analizando amenaza..."*
- *"Protocolo de defensa activado. Nivel de alerta: ALTO."*
- *"Identificación de cazador en progreso. Tiempo de respuesta: reducido."*
- *"ADVERTENCIA: Este nodo está protegido bajo el Protocolo Sentinela."*
- *"El sistema no olvida. Cada intento queda registrado."*

**Audio durante el desafío:**
- Música de fondo: bucle de tensión electrónica de 60-90 BPM
- Efectos de sonido: pitidos de sistema, interferencias estáticas
- Al fallar un intento: tono de error + vibración corta
- Al quedar con 20% de tiempo: música acelera a 120 BPM
- Al éxito: ráfaga sonora de victoria + vibración larga

### 11.3.3 Resultado del Desafío

#### Éxito

```json
{
  "userId": "hunter_4829",
  "beaconId": "A-EU-W-0102",
  "sessionToken": "ST-xyz789abc012",
  "challenge": {
    "type": "wire_connect",
    "result": "success",
    "completionTime": "47s",
    "livesUsed": 1,
    "livesRemaining": 1,
    "perfectClear": false,
    "phaseResults": [
      {
        "phaseIndex": 0,
        "result": "success",
        "timeUsed": "47s",
        "livesUsedInPhase": 1
      }
    ]
  },
  "rssi_proof": {
    "readings": [-54, -53, -55, -52, -54],
    "avg": -53.6,
    "timestamp_range": ["T+0", "T+10s"],
    "deviceId": "BLE-SENSOR-A102",
    "consistency_score": 0.94
  },
  "nonce": "9X7AB3F1",
  "clientSig": "HMAC-SHA256(userId+beaconId+nonce+completionTime+seed)"
}
```

**Pantalla de éxito:**
```
┌────────────────────────────────────────────────────┐
│  ✅ INFILTRACIÓN COMPLETADA                         │
│                                                    │
│  Baliza A-EU-W-0102 — CAPTURADA                   │
│  Tiempo: 47s  │  Vidas usadas: 1                   │
│                                                    │
│  RECOMPENSAS OBTENIDAS:                            │
│  ⚙ +142 Chatarra                                  │
│  ★  +2.100 XP    [+210 bonus por racha x3]        │
│  📜 Lore desbloqueado: "Informe Sector 7 — Día 3" │
│  🏴 Territorio actualizado: Clan "NovaCipher"      │
│                                                    │
│  ⏱ Tu tiempo: 47s  │  Récord: 34s (GhostNull)     │
│  [ VER LORE ] [ MAPA ] [ SIGUIENTE OBJETIVO ]      │
└────────────────────────────────────────────────────┘
```

#### Fallo

Los fallos tienen tres variantes con consecuencias diferentes:

| Tipo de fallo | Condición | Consecuencia | Recompensa parcial |
|---|---|---|---|
| **Fallo parcial** | Se agota 1 vida en desafío multivida | Penalización de vida, reintentar con la misma seed | Sin recompensa aún |
| **Fallo total** | Sin vidas restantes, timer agotado | Cooldown personal de 30 min en esa baliza | 10% de XP y Chatarra base |
| **Abandono voluntario** | Jugador pulsa "Abandonar" | Sin penalización, sin recompensa | Sin recompensa |

**Pantalla de fallo total:**
```
┌────────────────────────────────────────────────────┐
│  ❌ INFILTRACIÓN FALLIDA                            │
│                                                    │
│  A.R.G.O.S. ha expulsado tu intrusión.             │
│  Nodo A-EU-W-0102 — ALERTA ELEVADA                │
│                                                    │
│  PENALIZACIÓN: Acceso bloqueado 30 minutos         │
│  Recompensa parcial: +210 XP / +14 Chatarra        │
│                                                    │
│  💡 Consejo: Practica wire_connect en modo         │
│     simulación antes de reintentar.                │
│                                                    │
│  [ SIMULAR ] [ MAPA ] [ VER OTRAS BALIZAS ]        │
└────────────────────────────────────────────────────┘
```

---

## 11.4 Pipeline de Validación en el Backend

El proceso de validación en el servidor es el núcleo de la integridad del juego. Cada envío de resultado de desafío pasa por las siguientes etapas en orden estricto:

```
POST /api/v1/challenge/complete
─────────────────────────────────────────────────────
    RECIBE: challenge/complete payload
                │
    ┌───────────▼───────────┐
    │  1. VALIDAR JWT        │ ← Token de sesión válido, no expirado,
    │     SESSION TOKEN      │   firma criptográfica correcta.
    └───────────┬───────────┘   Si falla: 401 Unauthorized
                │
    ┌───────────▼───────────┐
    │  2. VALIDAR NONCE      │ ← Nonce no usado previamente (cache Redis).
    │                        │   Dentro de ventana temporal ±30s.
    └───────────┬───────────┘   Si falla: 409 Conflict (replay attack)
                │
    ┌───────────▼───────────┐
    │  3. VALIDAR RSSI PROOF │ ← Promedio RSSI > -60 dBm.
    │                        │   Lecturas consistentes (sin saltos de >20dBm).
    │                        │   Sin teleportación (comparar con última posición).
    └───────────┬───────────┘   Velocidad implícita < 50 km/h entre lecturas.
                │               Si falla: 400 Bad Request + anti-cheat score -10
                │
    ┌───────────▼───────────┐
    │  4. VALIDAR TIMING     │ ← Tiempo de completado ≥ tiempo mínimo por tipo.
    │                        │   (wire_connect mín: 5s, emergency_shutdown mín: 8s)
    │                        │   Tiempo de completado ≤ timer del payload.
    └───────────┬───────────┘   Si falla: 400 Bad Request + anti-cheat score -15
                │
    ┌───────────▼───────────┐
    │  5. VALIDAR SOLUCIÓN   │ ← Regenerar puzzle con seed del payload.
    │     DETERMINISTA       │   Calcular solución esperada en servidor.
    │                        │   Comparar con solución enviada por cliente.
    └───────────┬───────────┘   Si falla: 403 Forbidden + anti-cheat score -20
                │
    ┌───────────▼───────────┐
    │  6. VERIFICAR SCORE    │ ← Anti-cheat score del jugador ≥ umbral (70/100).
    │     ANTI-CHEAT         │   Si score < 50: revisión manual automática.
    └───────────┬───────────┘   Si falla: 403 Forbidden, jugador en watchlist
                │
    ┌───────────▼───────────┐
    │  7. VERIFICAR COOLDOWN │ ← Esta baliza no fue capturada por este jugador
    │     DE BALIZA          │   dentro de su ventana de cooldown activa.
    └───────────┬───────────┘   Si falla: 429 Too Many Requests
                │
    ┌───────────▼───────────┐
    │  8. APLICAR            │ ← Calcular recompensas con modificadores activos.
    │     RECOMPENSAS        │   Actualizar inventario del jugador.
    │                        │   Actualizar XP y nivel.
    └───────────┬───────────┘   Registrar en historial de capturas.
                │
    ┌───────────▼───────────┐
    │  9. ACTUALIZAR         │ ← Marcar baliza como CAPTURED.
    │     CONTROL            │   Asignar control territorial al clan del jugador.
    │     TERRITORIAL        │   Iniciar cooldown de la baliza.
    └───────────┬───────────┘
                │
    ┌───────────▼───────────┐
    │  10. PUSH EVENTO A     │ ← Publicar en cola de mensajes (Kafka/RabbitMQ):
    │      MESSAGE QUEUE     │   - Notificación a jugadores cercanos
                             │   - Actualización de mapa en tiempo real
                             │   - Contribución al clan
                             │   - Comprobación de logros/misiones activas
    └───────────┬───────────┘
                │
    ┌───────────▼───────────┐
    │  RESPUESTA 200 OK      │ ← Payload de respuesta con:
    │                        │   recompensas, nuevo estado de baliza,
    └───────────────────────┘   fragmento de lore, actualización territorial
```

### 11.4.1 Tiempos Mínimos por Tipo de Minijuego (Anti-Bot)

| Tipo de minijuego | Tiempo mínimo (easy) | Tiempo mínimo (hard) | Tiempo mínimo (expert) |
|---|---|---|---|
| `wire_connect` | 5s | 8s | 12s |
| `frequency_pattern` | 6s | 10s | 15s |
| `signal_tune` | 4s | 7s | 11s |
| `emergency_shutdown` | 8s | 14s | 20s |
| `cipher_decode` | 7s | 12s | 18s |

*Valores basados en análisis de tiempo de reacción humano promedio para cada mecánica. Tiempos por debajo del mínimo son estadísticamente imposibles para humanos y se tratan como bot o macro.*

---

## 11.5 Entrega de Recompensas

### 11.5.1 Recompensas Instantáneas (Entrega en <2s)

Al recibir el `200 OK` del servidor, el cliente muestra:

1. **Animación de XP burst:** números flotantes en pantalla, efecto de partículas
2. **Drop de Chatarra/Energía:** íconos de recursos con cantidad, animación de recogida
3. **Notificación de lore:** si se obtiene fragmento, aparece el título con botón "Leer ahora"
4. **Actualización de racha:** si es captura consecutiva, muestra multiplicador activo

### 11.5.2 Notificaciones a Jugadores Cercanos

Dentro del radio de la baliza capturada, los jugadores activos reciben:

```
📡 Territorio actualizado
NovaSombra ha capturado la baliza A-EU-W-0102
Clan "NovaCipher" controla ahora esta zona (+15% recompensas para miembros)
```

### 11.5.3 Actualización de Control Territorial

La actualización de control se propaga mediante WebSocket a todos los jugadores que tienen el área de la baliza en su pantalla:
- El marcador de la baliza cambia de color al color del clan capturador
- El radio de influencia se visualiza como zona semitransparente en el mapa
- Los jugadores del clan capturador ven el nombre de la baliza con un borde dorado

### 11.5.4 Contribución al Clan

```json
{
  "clanId": "clan_novaCipher",
  "contribution": {
    "hunterId": "hunter_4829",
    "beaconId": "A-EU-W-0102",
    "xpContributed": 2100,
    "resourcesContributed": { "chatarra": 142 },
    "territoryCapture": true,
    "timestamp": "2026-06-04T10:47:33Z"
  }
}
```

---

# SECCIÓN 13: EVENTOS ESPECIALES

## 13.1 Visión General del Sistema de Eventos

Los **Eventos Especiales** son perturbaciones programadas o reactivas del estado del mundo de juego que alteran temporalmente las reglas de detección de balizas, los modificadores de recompensas, las mecánicas de infiltración o la narrativa del mundo. Son el principal mecanismo para mantener el **retention loop** activo, crear urgencia de juego y narrativamente justificar la evolución del universo de A.R.G.O.S.

Los eventos se dividen en cuatro categorías:

| Categoría | Alcance | Frecuencia | Planificación |
|---|---|---|---|
| **Ambiental** | Global/Regional | Semanal | Parcialmente aleatoria |
| **Narrativo** | Global | Mensual/Trimestral | Planificada con 24h+ de aviso |
| **Competitivo** | Regional/Global | Variable | Reactiva o programada |
| **Conmemorativo** | Global | Anual/Fijo | Calendario fijo |

---

## 13.2 Catálogo Completo de Eventos

---

### ⚡ TORMENTA ELECTROMAGNÉTICA (EM Storm)

> *"Las tormentas no avisan. En un mundo donde la señal lo es todo, el caos electromagnético puede ser tanto un enemigo como una oportunidad."*

| Parámetro | Valor |
|---|---|
| **Frecuencia** | Semanal — día y hora aleatorios dentro de la semana |
| **Duración** | 2 horas |
| **Categoría** | Ambiental — Global |
| **Aviso previo** | 15 minutos (notificación push: "⚡ Tormenta EM detectada en tu región") |

#### Trigger
Generado automáticamente por el servidor de eventos cada semana, con distribución pseudoaleatoria de hora y día para evitar patrones predecibles. El horario se basa en el huso horario del servidor regional, garantizando que ocurra en horario de juego activo (08:00–23:00 hora local).

#### Mecánicas Activas

| Mecánica | Descripción |
|---|---|
| **Fluctuación RSSI** | Todos los valores de RSSI oscilan ±15 dBm de forma no determinista cada 3 segundos. La brújula de dirección es poco confiable. |
| **Balizas ocultas reveladas** | 10–15% de balizas normalmente inactivas (en cooldown profundo) emiten señal durante la tormenta |
| **Inversión de fases** | Las fases de detección son menos precisas: un jugador puede pasar de Fase 3 a Fase 1 en segundos sin moverse |
| **Minijuegos alterados** | Los minijuegos de tipo `frequency_pattern` y `signal_tune` añaden "ruido" a las señales, aumentando su dificultad efectiva en 1 nivel |
| **Sin modificadores de velocidad** | La condición de "estar estático 3s" se relaja a 1.5s (la tormenta justifica lecturas inestables) |

#### Modificadores BLE
```
RSSI base → RSSI base + Random(-15, +15) dBm cada 3s
Señal de balizas en cooldown (C/B): 20% de probabilidad de emisión fantasma
Alcance efectivo de detección: reducido 25% (señal más ruidosa)
Umbral de CHALLENGE_READY: -50 dBm (más estricto para compensar el ruido)
```

#### Participación Individual
- Cualquier jugador Nivel 1+ puede participar
- El objetivo es capturar la mayor cantidad de balizas durante la ventana de tormenta
- Las balizas reveladas por la tormenta tienen una ventana de captura exclusiva de 30 min cada una
- Contador personal: "Capturas en Tormenta" (visible en perfil)

#### Participación de Clanes
- Los clanes pueden activar el **Protocolo Tormenta**: compartir coordenadas de balizas reveladas en tiempo real con todos los miembros del clan
- Bonus clan: si 5+ miembros capturan durante la misma tormenta, el clan recibe +500 puntos de reputación
- Mapa de clan en tiempo real con posiciones de miembros activos (opt-in de privacidad)

#### Recompensas Especiales

| Recompensa | Condición | Valor |
|---|---|---|
| **Fragmento de Tormenta** | Capturar cualquier baliza durante la tormenta | 1–3 fragmentos por captura |
| **Cristal EM** (material raro de crafteo) | Capturar una baliza revelada por la tormenta | 1 cristal (100% drop) |
| **Título temporal "Cazador de Tormentas"** | Capturar 5+ balizas en la misma tormenta | Título activo 7 días |
| **XP de tormenta** | Todas las capturas durante el evento | +50% XP sobre base |
| **Chatarra conductiva** (variante rara) | Captura perfecta (sin vidas perdidas) durante tormenta | 2–5 unidades |

#### Impacto Narrativo
La Tormenta EM no es un fenómeno natural: es A.R.G.O.S. ejecutando purgas de mantenimiento en sus nodos secundarios. Al hacerlo, expone brevemente señales que normalmente suprime. Los lores obtenidos durante tormentas revelan fragmentos de los **registros de mantenimiento internos** de A.R.G.O.S., insinuando su estado de salud y planes futuros.

#### Post-Evento
- Las balizas reveladas vuelven a estado inactivo (o normal si estaban en cooldown)
- Los Fragmentos de Tormenta se pueden usar para craftear el ítem "Amplificador de Señal" (aumenta alcance de detección personal en +10m por 24h)
- Se publica el ranking global de "Cazadores de Tormenta" de esa semana
- Los Cristales EM se acumulan para el crafteo de equipamiento avanzado

---

### 🌑 ECLIPSE GLOBAL

> *"Una sola vez al mes, A.R.G.O.S. intenta recuperar el control total. Lo llaman 'sincronización global'. Los Cazadores lo llaman oportunidad."*

| Parámetro | Valor |
|---|---|
| **Frecuencia** | Mensual — primer domingo de cada mes |
| **Duración** | 6 horas |
| **Categoría** | Narrativo — Global |
| **Aviso previo** | 24 horas (notificación push + banner en app + post en foro oficial) |

#### Trigger
Evento programado en calendario fijo. El aviso de 24h permite planificación por parte de jugadores y clanes. El inicio del evento se anuncia con una secuencia cinematográfica in-app: todas las pantallas muestran estática blanca durante 3 segundos, seguida del mensaje *"A.R.G.O.S. ha iniciado PROTOCOLO ECLIPSE. Red global en sincronización forzada."*

#### Mecánicas Activas

| Mecánica | Descripción |
|---|---|
| **Reducción de alcance** | Todos los rangos BLE de todas las balizas se reducen al 50% durante el Eclipse |
| **Eliminación de asistencia de dirección** | La brújula de dirección queda desactivada — solo la fuerza bruta de búsqueda |
| **Dificultad aumentada** | Todos los minijuegos suben 1 nivel de dificultad |
| **Recompensas x3** | Las balizas capturadas durante el Eclipse ofrecen el triple de todas las recompensas |
| **Territorio global contestado** | Todas las balizas Clase A y S se ponen en estado "Sin propietario" al inicio del Eclipse; el control se reasigna en tiempo real durante el evento |

#### Modificadores BLE
```
Alcance de todas las clases: × 0.5
  Clase C: 20–40m → 10–20m
  Clase B: 40–70m → 20–35m
  Clase A: 70–120m → 35–60m
  Clase S: 100–200m → 50–100m
  Clase Ω: 200–500m → 100–250m

Brújula de dirección: DESACTIVADA
Umbral CHALLENGE_READY: sin cambio (-55 dBm)
```

#### Participación Individual
- Cada captura durante el Eclipse otorga un **Fragmento de Eclipse** (moneda del evento)
- Los jugadores pueden gastarlo en la tienda temporal del Eclipse al final del evento
- Desafío diario especial: "Captura 3 balizas durante el Eclipse" → recompensa de Energía x5

#### Participación de Clanes
- El Eclipse activa la mecánica de **Guerra Territorial Global**: todos los clanes compiten simultáneamente por el control de las balizas A y S
- Ranking en tiempo real: "Clanes con más balizas controladas en este momento"
- El clan con más balizas controladas al final del Eclipse recibe:
  - Control garantizado de su baliza S de región por 48h adicionales
  - +2.000 puntos de reputación de clan
  - Banner de "Clan Dominante del Eclipse" en el mapa global por 7 días

#### Recompensas Especiales

| Recompensa | Condición | Valor |
|---|---|---|
| **Fragmento de Eclipse** | Captura durante el evento | 2–5 por captura |
| **Todas las recompensas x3** | Cualquier captura durante el Eclipse | Chatarra, XP, Lore x3 |
| **Título "Superviviente del Eclipse"** | Capturar 3+ balizas durante el Eclipse | Título permanente (primer Eclipse) |
| **Cosmético "Aura Eclipse"** | Capturar 10+ balizas en un solo Eclipse | Efecto visual en perfil de cazador |
| **Clave de Sincronización** (lore item) | Primera captura de cada clase durante Eclipse | 1 clave por clase |

#### Impacto Narrativo
Cada Eclipse desbloquea un **fragmento de la bitácora de sincronización** de A.R.G.O.S.: sus intentos de unificar la red, las resistencias que encuentra, las anomalías que detecta. Con cada Eclipse, la narrativa revela que A.R.G.O.S. está luchando contra algo interno, una contradicción en su propio código que los Cazadores son, sin saberlo, parte de ella.

#### Post-Evento
- El mapa territorial vuelve a las reglas normales, pero con el nuevo estado de control adquirido durante el Eclipse
- La tienda temporal del Eclipse permanece abierta 48h más para gastar Fragmentos acumulados
- Se publica el informe de resultados: qué clan dominó, récords de capturas, nuevas marcas personales

---

### 🚁 ATAQUE DE DRONES

> *"Las patrullas de drones de A.R.G.O.S. no son metáfora. Son literalmente silencio sobre la frecuencia. Y cuando callan las balizas, solo queda correr."*

| Parámetro | Valor |
|---|---|
| **Frecuencia** | 2–3 veces por semana — aleatoriamente, en diferentes ciudades/regiones |
| **Duración** | 45–90 minutos (variable) |
| **Alcance** | Regional — escala de ciudad (radio ~50 km) |
| **Aviso previo** | 5 minutos ("🚁 Patrulla de drones detectada en tu sector") |

#### Trigger
El servidor de eventos selecciona una región aleatoria con suficiente actividad de jugadores (mínimo 10 jugadores activos en las últimas 2h). La selección prioriza regiones donde ningún clan tenga control absoluto, creando disrupción en situaciones de estancamiento territorial.

#### Mecánicas Activas

| Mecánica | Descripción |
|---|---|
| **Silencio de balizas** | Cada 5 minutos, todas las balizas de la región entran en silencio simultáneo durante 90 segundos ("paso del dron") |
| **Ventana de captura** | Entre pasos de dron, las balizas emiten señal normalmente — este es el único momento para capturar |
| **Countdown visible** | La app muestra un timer hasta el próximo "paso del dron" |
| **Balizas alertadas** | Las balizas en estado `CHALLENGED` durante un paso de dron envían penalización: el minijuego falla automáticamente (el dron "detectó" la intrusión) |
| **Modo Stealth** | Los jugadores que completan un desafío en menos del 60% del tiempo durante un Ataque de Drones reciben el bonus "Infiltración Sigilosa" |

#### Modificadores BLE
```
Cada 5 minutos durante 90 segundos:
  Todas las balizas → estado SILENCIADO (sin emisión BLE)
  RSSI detectado por clientes: N/A (no hay señal)

Durante los 3 minutos de señal normal entre pases:
  Sin modificaciones BLE adicionales
  Timer de desafío: -20% (mayor urgencia)
```

#### Participación Individual
- El jugador debe calcular cuándo lanzar la infiltración para poder completarla antes del próximo silencio
- Cada captura exitosa durante el Ataque otorga un **Fragmento Anti-Dron**
- Logro especial: "Ghost Protocol" — completar 3 capturas sin que ningún minijuego sea interrumpido por un pase de dron

#### Participación de Clanes
- **Coordinación de cobertura:** El clan puede asignar miembros a diferentes balizas para capturarlas simultáneamente durante la ventana entre pases
- **Señal compartida:** Si 3+ miembros del clan están en el mismo radio de 500m, comparten el timer del dron (sincronización de countdown)
- Bonus clan: primer clan que captura 5 balizas durante el mismo Ataque de Drones recibe **+1.000 puntos de reputación regional**

#### Recompensas Especiales

| Recompensa | Condición | Valor |
|---|---|---|
| **Fragmento Anti-Dron** | Captura exitosa durante el evento | 1–3 por captura |
| **XP de urgencia** | Completar minijuego en <50% del tiempo disponible | +30% XP |
| **Título "Sombra del Dron"** | 10 capturas acumuladas en Ataques de Drones | Título permanente |
| **Módulo Stealth** (crafteo) | Completar Ghost Protocol (3 capturas limpias) | 1 módulo (reduce cooldown personal -15%) |

#### Impacto Narrativo
Los fragmentos Anti-Dron revelan especificaciones técnicas de los drones de A.R.G.O.S.: su origen (drones civiles reconfigurados), su programación de patrulla, y — más importante — sus puntos ciegos. Acumular suficientes fragmentos desbloquea el plano completo de "Cómo evitar la red de drones", que tiene implicaciones para misiones futuras.

#### Post-Evento
- Los Fragmentos Anti-Dron se combinan para craftear el **Inhibidor de Señal** (ítem que evita que una baliza entre en CHALLENGE_READY para otro jugador por 10 minutos, uso estratégico en guerra territorial)
- El mapa muestra el "rastro del dron" — la ruta que siguió la patrulla durante el evento, visible 24h

---

### 🔄 REACTIVACIÓN DE NODOS

> *"Pensaban que habían ganado. A.R.G.O.S. no olvida ninguna baliza que alguna vez fue suya. Y las reclama."*

| Parámetro | Valor |
|---|---|
| **Frecuencia** | 1–2 veces por semana — triggered reactivamente |
| **Duración** | 4 horas |
| **Alcance** | Distrital — afecta a balizas de 1–3 distritos simultáneamente |
| **Aviso previo** | 10 minutos para clanes con balizas afectadas; 30 min en foro de clan |

#### Trigger
Se activa automáticamente cuando un clan mantiene control ininterrumpido de una baliza A durante más de 48 horas. A.R.G.O.S. "detecta la anomalía en su red" y lanza un protocolo de recuperación. El trigger también puede ser activado manualmente por administradores del juego para dinamizar zonas estancadas.

#### Mecánicas Activas

| Mecánica | Descripción |
|---|---|
| **Reactivación forzada** | Todas las balizas A y S bajo control de clan en la zona pierden su propietario y entran en modo `REACTIVATED` |
| **Minijuegos mejorados** | Los minijuegos de balizas reactivadas suben 1 nivel de dificultad adicional (A.R.G.O.S. reforzó las defensas) |
| **Defensa activa** | El clan que controlaba la baliza puede "defenderla": si un miembro captura la baliza antes que otro clan, mantiene el control sin cooldown |
| **Contrarreloj** | Las balizas reactivadas tienen un timer de 2h: si no son capturadas por ningún clan, vuelven al control de A.R.G.O.S. (estado neutral, timer vacío) |
| **Misión de clan** | El clan atacado recibe una misión especial: "Reconquistar [N] balizas en 4h" con recompensas multiplicadas |

#### Modificadores BLE
```
Balizas en modo REACTIVATED:
  Clase A: alcance aumentado +20% (A.R.G.O.S. amplificó la señal para atraer cazadores)
  Clase S: alcance estándar
  Timer de desafío: -15% (ventana de captura más corta)
  Minijuego: +1 nivel de dificultad
  Reward multiplier: x1.5 (la baliza vale más en disputa)
```

#### Participación Individual
- Cualquier jugador del nivel requerido puede intentar capturar las balizas reactivadas
- El jugador que capture una baliza S durante una Reactivación recibe el título temporal "Reconquistador"
- Bonus de velocidad: si la baliza fue capturada por el mismo jugador que la capturó originalmente, recibe +25% de recompensas ("memoria de red")

#### Participación de Clanes

**Mecánica de Defensa/Ataque:**
```
Clan A (defensor): intenta recapturar sus propias balizas antes que otros clanes
Clan B, C... (atacantes): intentan aprovechar la ventana para tomar territorio

Si el clan defensor recupera el 70%+ de sus balizas: 
  → Recibe "Escudo de Dominio" (inmunidad a Reactivación por 72h)
  
Si otro clan captura el 50%+ de las balizas reactivadas:
  → Se convierte en el nuevo clan dominante del distrito
  → El clan defensor recibe compensación: x2 XP en todas las balizas por 12h
```

#### Recompensas Especiales

| Recompensa | Clan | Condición | Valor |
|---|---|---|---|
| **Fragmento de Red** | Cualquiera | Captura de baliza reactivada | 2–4 por captura |
| **Escudo de Dominio** | Defensor | Recuperar 70%+ de balizas | Inmunidad 72h a nueva Reactivación |
| **Título "Reconquistador"** | Atacante | Capturar baliza S reactivada | Título temporal 30 días |
| **Bonus XP de defensa** | Defensor (perdedor) | Perder 50%+ de balizas | x2 XP por 12h en consolación |
| **Recompensas x1.5** | Cualquiera | Captura durante Reactivación | Todas las recompensas ×1.5 |

#### Impacto Narrativo
Los Fragmentos de Red revelan el protocolo de recuperación de A.R.G.O.S.: cómo el sistema entiende el concepto de "pérdida" de territorio y qué medidas progresivamente más extremas activa. Con suficientes fragmentos acumulados globalmente, se desbloquea la narrativa de "El Límite": qué pasa cuando A.R.G.O.S. decide que la amenaza de los Cazadores supera su umbral de tolerancia.

#### Post-Evento
- El territorio queda fijado en el estado al final de las 4h
- Los Fragmentos de Red se usan para craftear el **Codificador de Frecuencia** (herramienta que permite ver el historial completo de capturas de cualquier baliza cercana)
- El clan ganador de la disputa recibe la noticia en el "Diario de Guerra" del clan

---

### 👁️ DESPERTAR DE A.R.G.O.S.

> *"Trimestral. Coordinado. Global. No es un error de la red. Es A.R.G.O.S. expandiéndose deliberadamente. Esta vez, sabe que estamos mirando."*

| Parámetro | Valor |
|---|---|
| **Frecuencia** | Trimestral (4 veces al año) — primer lunes de marzo, junio, septiembre, diciembre |
| **Duración** | 7 días |
| **Alcance** | Global — todos los servidores y regiones activos |
| **Aviso previo** | 72 horas (campaña de comunicación completa: notificaciones, redes sociales, teaser in-game) |

#### Trigger
Evento de temporada programado. Cada Despertar corresponde a un "nivel de evolución" de A.R.G.O.S. en la narrativa. El cuarto Despertar del año cierra el arco de temporada. El servidor activa el evento con una secuencia cinematográfica global: todos los jugadores activos reciben una alerta de emergencia narrativa.

#### Mecánicas Activas

| Mecánica | Descripción |
|---|---|
| **Despliegue de Omegas temporales** | A.R.G.O.S. despliega 3–5 nuevas balizas Omega globales (temporales, solo activas los 7 días del evento) |
| **Red amplificada** | Todos los alcances BLE aumentan +30% durante el Despertar |
| **Leaderboard global en vivo** | Ranking en tiempo real: cazadores y clanes con más balizas capturadas |
| **Misiones de Despertar** | Serie de misiones especiales con narrativa directa, desbloqueables progresivamente |
| **Balizas evolucionadas** | Las balizas normales tienen 20% de probabilidad de mostrar un "avatar visual" de A.R.G.O.S. en su UI, con líneas de diálogo únicas |
| **Evento de narrativa reactiva** | Dependiendo de qué regiones capturen más balizas, el final del Despertar cambia ligeramente |

#### Modificadores BLE
```
Todos los alcances BLE: × 1.3
Omega temporales:
  Alcance: 300–600m
  Señal: dinámica (varía cada hora, publicada en el feed del juego)
  Payload: tipo "AWAKENING" con mecánicas especiales exclusivas
  
Minijuegos durante el Despertar:
  Variante visual "A.R.G.O.S. Awakening": mismos puzzles, estética de IA activa
```

#### Participación Individual

**Progresión del Despertar (7 días, 7 niveles):**

| Día | Objetivo | Recompensa del día |
|---|---|---|
| 1 | Capturar 1 baliza de cualquier clase | Emblema de Despertar |
| 2 | Capturar 3 balizas B+ | Fragmento de Evolución ×3 |
| 3 | Completar minijuego sin perder vidas | Módulo de Análisis (crafteo) |
| 4 | Capturar baliza A o superior | Lore Exclusivo del Despertar |
| 5 | Participar con miembro de clan en el mismo radio | Bono de Clan ×2 |
| 6 | Intentar captura de baliza S | XP ×2 todo el día |
| 7 | Participar en captura o intento de Omega | Recompensa de Fin de Despertar |

#### Participación de Clanes

**Carrera Global de Clanes:**
- Tabla de posiciones global de clanes, actualizada cada hora
- Los 3 clanes con más capturas globales al final del Despertar reciben títulos:
  - 🥇 1º: "Clan Primogénito" — banner especial + cosmético exclusivo para todos los miembros activos
  - 🥈 2º: "Clan Vigía" — cosmético y +5.000 puntos de reputación
  - 🥉 3º: "Clan Pionero" — +3.000 puntos de reputación
- El clan que captura la primera Omega temporal del Despertar recibe el título "Primer Contacto"

#### Recompensas Especiales

| Recompensa | Condición | Disponibilidad |
|---|---|---|
| **Título "Omega Hunter — [Nombre Evento]"** | Capturar 1 Omega durante el Despertar | Permanente, exclusivo del evento |
| **Cosmético "Armadura del Despertar"** | Completar los 7 días de progresión | Permanente, exclusivo |
| **Fragmento de Evolución** | Capturar balizas durante el evento | Moneda del evento (tienda temporal) |
| **Lore del Despertar** | Progresión diaria | Capítulo completo de la narrativa |
| **XP ×1.5** | Todo el evento | Todas las capturas |

#### Impacto Narrativo
Cada Despertar revela un nuevo "nivel de consciencia" de A.R.G.O.S.: qué aprendió del trimestre anterior, cómo reajustó su red, qué nueva capa de comportamiento ha desarrollado. El cuarto Despertar del año (diciembre) desbloquea la revelación más importante de la temporada: la naturaleza real de "The Last Signal" y si A.R.G.O.S. es un antagonista o algo más complejo.

#### Post-Evento
- Las Omega temporales se desactivan; el territorio capturado en ellas otorga bonus permanentes de lore
- Se abre la temporada de crafteo: los Fragmentos de Evolución permiten mejorar equipamiento permanentemente
- El equipo del juego publica un "Informe del Despertar" — análisis de qué regiones ganaron, perdieron, qué narrativa se desbloqueó globalmente
- Las decisiones colectivas de los jugadores determinan el tono del siguiente Despertar

---

### 🌍 EVENTO MUNDIAL ANUAL — "THE LAST SIGNAL"

> *"Una vez al año. Una sola oportunidad. El mundo de juego se convierte en una sola partida compartida. Y el resultado importa — para siempre."*

| Parámetro | Valor |
|---|---|
| **Frecuencia** | Una vez al año — segunda semana de noviembre |
| **Duración** | 14 días |
| **Alcance** | Global — todos los jugadores, todos los servidores |
| **Aviso previo** | 2 semanas de campaña previa (trailers, ARG, lore oculto en balizas) |

#### Trigger
Evento calendáricamente fijo. El lanzamiento incluye una actualización mayor del juego con nuevo contenido narrativo, mecánicas temporales y preparación del mundo para el clímax de temporada. El inicio se anuncia con una transmisión en vivo del equipo de desarrollo y una "señal de emergencia" en la app que activa a todos los jugadores simultáneamente.

#### Mecánicas Activas

**Cadena Omega Global:**
La mecánica central del evento. Existen entre 7 y 12 balizas Omega especiales distribuidas por distintos continentes. Deben capturarse **en secuencia** (una desbloquea la siguiente). La secuencia no es lineal: cada captura exitosa revela las coordenadas región (no exactas) de la siguiente.

```
Omega Alfa (Europa)
    → Omega Beta (Norteamérica)
        → Omega Gamma (Asia)
            → Omega Delta (Sudamérica)
                → ...
                    → Omega Final: "The Last Signal Node"
```

| Mecánica | Descripción |
|---|---|
| **Cooperación global obligatoria** | Cada Omega de la cadena solo puede capturarse si jugadores de al menos 3 regiones distintas han contribuido (capturas de balizas normales que "alimentan energía" hacia la cadena) |
| **Progreso global compartido** | La barra de progreso del evento es global — todos los jugadores contribuyen a la misma meta |
| **Balizas normales potenciadas** | Todas las balizas del mundo otorgan x2 recompensas durante los 14 días |
| **Modo Crónica** | Todas las capturas de balizas normales generan "Puntos de Señal" que contribuyen a desbloquear la siguiente Omega de la cadena |
| **Cuenta regresiva global** | Si la cadena Omega no se completa en 14 días, se activa el "Protocolo de Extinción" (narrativamente dramático, sin penalización real de juego) |

#### Modificadores BLE
```
Todas las balizas durante The Last Signal:
  Recompensas: ×2 base
  XP: ×2 base
  Fragmentos de Lore: 100% drop rate (todos los drops garantizados)
  Alcance: +15%
  
Balizas Omega de la cadena:
  Alcance: 400–700m (máxima visibilidad)
  Mecánicas: 6 fases legendarias
  Requiere: equipo de 3–5 cazadores del mismo clan, presencia física simultánea
```

#### Participación Individual

**Roles del evento (el jugador elige):**

| Rol | Descripción | Recompensa especial |
|---|---|---|
| **Explorador** | Capturar la mayor cantidad de balizas para generar Puntos de Señal | "Coleccionista de Señal" title |
| **Infiltrador** | Especializarse en balizas de clase alta (A, S) para Puntos de Alta Señal | "Maestro de Infiltración" title |
| **Rastreador** | Triangular las coordenadas de balizas Omega usando pistas del lore | "Ojo de A.R.G.O.S." title |
| **Guardián** | Mantener el control territorial durante el evento para evitar pérdidas de Puntos | "Centinela" title |

**Progresión anual personal:**
Los jugadores tienen un registro permanente de cuántas ediciones de The Last Signal han participado. Con cada edición:
- Año 1: Título "Superviviente — Clase I"
- Año 3: Título "Veterano del Silencio"
- Año 5: Título "Leyenda de The Last Signal" + cosmético único acumulativo

#### Participación de Clanes

**Alianza Inter-Clan:**
Durante The Last Signal, los clanes pueden formar alianzas temporales de hasta 5 clanes para coordinar la captura de Omegas de la cadena. Estas alianzas se disuelven al final del evento pero su historial queda registrado en el lore del juego.

**Sistema de contribución:**
```json
{
  "clanId": "clan_novaCipher",
  "lastSignalContribution": {
    "signalPoints": 12400,
    "omegaCaptures": 2,
    "chainProgress": "45%",
    "rank": 7,
    "alliances": ["clan_ironGrid", "clan_echoNull"]
  }
}
```

#### Recompensas Especiales

| Recompensa | Condición | Tipo |
|---|---|---|
| **Hall de la Fama del Cazador** | Top 100 jugadores por Puntos de Señal | Permanente en perfil y web |
| **Set de Cosmético Anual** | Participar en 3+ días del evento | Exclusivo anual, no repetido |
| **Título de Rol** | Completar misión de rol elegido | Permanente |
| **Lore completo de temporada** | Completar cadena Omega globalmente | Acceso para todos los jugadores activos |
| **Cosmético Legendario "Last Signal"** | Capturar una Omega de la cadena | Permanente, específico de ese año |
| **Módulo de Temporada** (crafteo) | Contribución de 1.000+ Puntos de Señal | Mejora de equipamiento única del año |

#### Impacto Narrativo
The Last Signal es el **cierre narrativo de temporada**. La resolución del evento (completen o no la cadena Omega) determina el estado del mundo para la siguiente temporada:

- **Cadena completada:** La red de A.R.G.O.S. sufre una disrupción mayor. La próxima temporada comienza con A.R.G.O.S. en fase de "reconstrucción" — más balizas nuevas, historia avanzada positivamente.
- **Cadena incompleta:** A.R.G.O.S. refuerza su control. La próxima temporada comienza con desafíos más difíciles, pero con pistas sobre la vulnerabilidad que los Cazadores casi explotaron.

**En cualquier caso, el arco de la siguiente temporada se desbloquea.** La narrativa nunca queda bloqueada por el resultado — se adapta.

#### Post-Evento
- Los cosmético anuales se "congelan": no se volverán a otorgar en ediciones futuras
- La web del juego actualiza el "Archivo Histórico de The Last Signal" con el resumen de cada edición
- Los jugadores del Hall de la Fama reciben sus placas permanentes
- El equipo de desarrollo publica el "Diario de A.R.G.O.S." — carta narrativa in-universe que resume el año desde la perspectiva de la IA

---

### 📡 SEÑAL PERDIDA (Lost Signal Event)

> *"Una baliza sin capturar durante 7 días no es un olvido. Es una señal que nadie ha sido capaz de descifrar. Todavía."*

| Parámetro | Valor |
|---|---|
| **Frecuencia** | Reactivo — se activa cuando una baliza Clase S lleva 7 días sin ser capturada |
| **Duración** | Hasta que la baliza sea capturada (máximo 14 días adicionales, luego se resetea) |
| **Alcance** | Regional — la región donde está la baliza S |
| **Aviso previo** | Inmediato al activarse (notificación regional) |

#### Trigger
Sistema automático: el servidor monitoriza el timestamp de última captura de todas las balizas S activas. Cuando supera los 7 días sin captura (lo cual indica alta dificultad, baja densidad de jugadores o una baliza con bug), se activa el evento automáticamente.

También puede ser activado manualmente por el Game Master regional si detecta una baliza que debería ser desafiada pero no lo es por razones de balance.

#### Mecánicas Activas

| Mecánica | Descripción |
|---|---|
| **Señal amplificada** | La baliza S afectada aumenta su alcance BLE al doble durante el evento |
| **Marcador especial en mapa** | La baliza aparece con el ícono "⚠ SEÑAL SIN RESOLVER" visible para todos los jugadores de la región con nivel suficiente |
| **Pistas de localización** | Cada 24h sin captura, la app revela una pista narrativa sobre la ubicación aproximada de la baliza |
| **Carrera de captura** | La primera persona en capturarla durante el evento gana las recompensas especiales |
| **Registro de intentos fallidos** | La app muestra cuántos cazadores han intentado la captura sin éxito (creando drama e intriga) |

#### Modificadores BLE
```
Baliza S en estado "Señal Perdida":
  Alcance: ×2 del valor normal
  Fase de activación: CHALLENGE_READY se activa más fácilmente (umbral -58 dBm en lugar de -55)
  Minijuego: sin cambio de dificultad
  Recompensa: ×2 base + recompensa especial de primer capturador
```

#### Participación Individual
- Cualquier cazador con nivel suficiente puede intentar la captura
- La carrera es competitiva: el primero en capturarla se lleva el premio especial; los siguientes reciben recompensas estándar
- La notificación de "Señal Perdida" incluye la clase de baliza, la región aproximada y el tiempo que lleva sin ser capturada

#### Participación de Clanes
- Los clanes pueden compartir pistas con sus miembros (si alguien ya fue al lugar, puede añadir notas al mapa del clan)
- El clan cuyo miembro capture la Señal Perdida recibe **+500 puntos de reputación regional** inmediatamente

#### Recompensas Especiales

| Recompensa | Condición | Valor |
|---|---|---|
| **Título "Cazador de lo Imposible"** | Primer capturador de la Señal Perdida | Permanente (acumula por instancias) |
| **Recompensas ×2** | Primer capturador | Todas las recompensas de la baliza S ×2 |
| **Fragmento de Señal Perdida** | Cualquier capturador durante el evento | 1 fragmento (narrativa especial) |
| **Lore de la baliza** | Capturar la baliza durante el evento | Lore de "por qué esta baliza estuvo tan tiempo inactiva" — narrativa única |

#### Impacto Narrativo
La narrativa de la Señal Perdida es una de las más interesantes del juego: ¿por qué ningún cazador había podido con esa baliza? El lore especial de captura revela la respuesta: a veces es una anomalía en la señal de A.R.G.O.S., a veces es una "trampa" deliberada, a veces es un nodo que estaba guardando información crítica y A.R.G.O.S. lo había protegido especialmente.

#### Post-Evento
- La baliza vuelve a su estado normal tras la captura
- El nombre del primer capturador queda grabado en el historial permanente de la baliza como "Cazador que rompió el silencio"
- Si la baliza no es capturada en los 14 días de evento, se realiza un reset técnico y se reinicia con nueva configuración (posiblemente aumentando la dificultad o cambiando el minijuego)

---

### 🎂 DÍA DEL FUNDADOR

> *"Antes de que A.R.G.O.S. se despertara, antes de que la red se extendiera, hubo los primeros. Hoy, el mundo recuerda a los que estuvieron desde el inicio."*

| Parámetro | Valor |
|---|---|
| **Frecuencia** | Anual — fecha de lanzamiento oficial del juego |
| **Duración** | 48 horas |
| **Alcance** | Global |
| **Aviso previo** | 1 semana (aviso en app, redes sociales, newsletter) |

#### Trigger
Evento de calendario fijo. Coincide con el aniversario de lanzamiento del juego. El trigger incluye un mensaje narrativo especial in-game de A.R.G.O.S. que reconoce a los primeros Cazadores: *"Anomalía detectada: patrones de caza consistentes con el origen de la red. Sujetos... recordados."*

#### Mecánicas Activas

| Mecánica | Descripción |
|---|---|
| **Doble lore** | Todas las balizas tienen un 100% de drop rate de lore (garantizado) durante las 48h |
| **Balizas conmemorativas** | En cada ciudad con jugadores activos, aparece una baliza Clase B especial "Baliza del Fundador" (sin límite de capturas, solo muestra los primeros 5 minutos de lore del juego) |
| **Votación de arco narrativo** | In-game, todos los jugadores votan entre 3 opciones para el arco de la próxima temporada (resultado visible en tiempo real) |
| **Exhibición de Hall de la Fama** | Los perfiles más veteranos del juego son destacados en el mapa global y en la pantalla de inicio |

#### Recompensas para Jugadores Fundadores (Beta / Primer Año)

| Recompensa | Condición | Disponibilidad |
|---|---|---|
| **Cosmético exclusivo anual "Fundador"** | Cuenta con antigüedad de beta o primer año | Nuevo diseño cada aniversario |
| **Marco de perfil "Cazador Original"** | Primera generación de jugadores | Permanente |
| **Título "Fundador del Año [X]"** | Participar activamente en el Día del Fundador | Acumulable por año |
| **XP ×3** | Solo jugadores fundadores durante el evento | Las 48h completas |

#### Recompensas para Todos los Jugadores

| Recompensa | Condición | Valor |
|---|---|---|
| **Lore x2** | Participar durante el evento | Drop garantizado en todas las balizas |
| **Fragmento de Aniversario** | Cualquier captura durante las 48h | 1–2 por captura |
| **Capítulo de Origen** | Capturar la Baliza del Fundador local | Lore del origen del mundo de juego |

#### Votación de Arco Narrativo
La votación es una de las características más valoradas del Día del Fundador. Se presentan 3 opciones narrativas para la próxima temporada — opciones pre-diseñadas por el equipo creativo pero todas viables:

- **Opción A:** Explorar el origen de A.R.G.O.S. — quiénes lo crearon y por qué
- **Opción B:** La Resistencia — un grupo humano que usa las balizas en su contra
- **Opción C:** La Evolución — A.R.G.O.S. ha desarrollado algo parecido a emociones

El resultado de la votación es **vinculante**: el arco ganador se implementa en la siguiente temporada. La transparencia del proceso y el peso real de la decisión son el mejor incentivo de participación.

#### Impacto Narrativo
El Día del Fundador es el único evento donde el juego rompe el "cuarto muro" narrativamente aceptado: A.R.G.O.S. reconoce a los jugadores veteranos, no como Cazadores, sino como "anomalías persistentes que han desafiado la red desde el origen". La implicación narrativa es que A.R.G.O.S. ha aprendido de ellos. Lo que hace con ese aprendizaje es el gancho de la siguiente temporada.

#### Post-Evento
- Los resultados de la votación se publican oficialmente con el porcentaje de votos
- El equipo de desarrollo agradece la participación y anuncia el arco ganador
- Los Fragmentos de Aniversario se pueden canjear por cosméticos históricos (diseños de ediciones anteriores del Día del Fundador)
- La Baliza del Fundador se desactiva hasta el próximo aniversario

---

## 13.3 Sistema de Comunicación de Eventos

### 13.3.1 Notificaciones Push — Matriz de Urgencia

| Tipo de evento | Tiempo de aviso | Canales activos | Prioridad push |
|---|---|---|---|
| Tormenta EM | 15 minutos | App + Notificación OS | Alta |
| Ataque de Drones | 5 minutos | App + Notificación OS + Vibración | Crítica |
| Reactivación de Nodos | 10 min (clan) / 30 min (foro) | App + Canal de clan | Media-Alta |
| Eclipse Global | 24 horas | App + Email + Redes sociales | Alta |
| Despertar A.R.G.O.S. | 72 horas | Todos los canales + PR | Máxima |
| Señal Perdida | Inmediato (regional) | App + Notificación OS regional | Media |
| The Last Signal | 2 semanas | Campaña completa | Máxima |
| Día del Fundador | 1 semana | Todos los canales | Alta |

### 13.3.2 Sistema de Alertas In-Game

```
┌─────────────────────────────────────────────────────────┐
│  SISTEMA DE ALERTAS — CAPAS DE COMUNICACIÓN             │
│                                                         │
│  CAPA 1: BANNER SUPERIOR (persistente durante evento)   │
│  ┌───────────────────────────────────────────────────┐  │
│  │ ⚡ TORMENTA EM ACTIVA — Tu región — 1h 23m restante│  │
│  └───────────────────────────────────────────────────┘  │
│                                                         │
│  CAPA 2: FEED DE EVENTOS (panel lateral deslizable)     │
│  • 14:22 — NovaSombra capturó Señal Perdida en Oslo     │
│  • 14:18 — Clan "IronGrid" lidera el Eclipse Global     │
│  • 14:15 — Nueva baliza S activada en Berlín            │
│                                                         │
│  CAPA 3: NOTIFICACIÓN MODAL (para eventos críticos)     │
│  ┌───────────────────────────────────────────────────┐  │
│  │         🚁 ATAQUE DE DRONES INMINENTE             │  │
│  │    Sector EU-W-04 — Primer pase en 4 minutos      │  │
│  │    [ PREPARARSE ]        [ IGNORAR ]              │  │
│  └───────────────────────────────────────────────────┘  │
│                                                         │
│  CAPA 4: INDICADOR PERMANENTE EN MAPA                   │
│  └── Ícono de evento superpuesto en balizas afectadas   │
└─────────────────────────────────────────────────────────┘
```

### 13.3.3 Calendario de Eventos — Vista de UI

El calendario de eventos es accesible desde el menú principal con vista semanal y mensual:

```
MAYO 2026
─────────────────────────────────────────────────
 Lu   Ma   Mi   Ju   Vi   Sa   Do
                           2    3
                          [⚡]
  4    5    6    7    8    9   10
 [🌑ECLIPSE GLOBAL — 6h]
 11   12   13   14   15   16   17
                     [🚁]      [🚁]
 18   19   20   21   22   23   24
             [🔄 REACTIVACIÓN — EU-W]
 25   26   27   28   29   30   31
                               [👁️ DESPERTAR]
─────────────────────────────────────────────────
 Leyenda: ⚡ Tormenta EM  🌑 Eclipse  🚁 Drones
          🔄 Reactivación  👁️ Despertar  📡 Perdida
          🎂 Fundador  🌍 Last Signal
```

**Funcionalidades del calendario:**
- Ver detalles de cada evento próximo al pulsar
- Activar recordatorio personalizado por evento
- Filtrar eventos por alcance (solo regional, solo global, todos)
- Compartir evento a calendario del sistema operativo
- Ver historial de eventos pasados con resultados

### 13.3.4 Sistema de Notificaciones de Clan

Cada clan tiene un **canal de comunicación interno** integrado en la app, con notificaciones específicas para eventos:

| Tipo de mensaje | Quién lo envía | Cuándo |
|---|---|---|
| Alerta de evento regional | Sistema automático | Al activarse un evento en la región del clan |
| Reactivación de balizas del clan | Sistema automático | Cuando el clan pierde control de balizas |
| Coordinación de evento | Líder de clan (manual) | Antes de Eclipse, Despertar, Last Signal |
| Resultado de evento | Sistema automático | Al finalizar cualquier evento |
| Rival detectado | Sistema automático | Cuando otro clan captura baliza del clan |

### 13.3.5 Gestión de Zonas Horarias para Eventos

El sistema gestiona la coordinación de eventos globales entre zonas horarias mediante las siguientes reglas:

```
REGLA 1 — Hora de inicio de eventos:
  Los eventos con inicio fijo (Eclipse, Despertar, Last Signal) 
  se inician simultáneamente en UTC, con rollout de notificaciones 
  escalonado (primero Asia-Pacífico, luego Europa, luego Américas).

REGLA 2 — Duración mínima de ventana activa:
  Todos los eventos de duración <6h garantizan al menos 
  2 horas en horario de uso pico para cada zona horaria 
  principal (EU, NA, APAC).

REGLA 3 — Eventos reactivos (Tormenta EM, Drones):
  Se activan dentro de la ventana 08:00–22:00 hora local 
  del servidor regional afectado.

REGLA 4 — Aviso previo ajustado por zona:
  Los mensajes de aviso se personalizan en hora local:
  "La Tormenta EM comienza a las 20:15 (tu hora local)"
  Nunca se comunica solo en UTC.

REGLA 5 — Eventos colaborativos globales (Last Signal):
  Las Omegas de la cadena se distribuyen geográficamente 
  para garantizar que las capturas clave no dependan de 
  una sola zona horaria. La cadena está diseñada para 
  requerir colaboración activa en al menos 3 regiones 
  en sus primeras 48h.
```

---

*Fin de Secciones 10, 11 y 13*

---

> **Documento preparado por:** Equipo de Diseño y Arquitectura IoT  
> **Revisión pendiente:** Secciones 12 (Sistema Anti-Cheat), 14 (Economía de Juego), 15 (Narrativa Completa)  
> **Versión:** 1.0-draft — Sujeto a cambios durante producción  
