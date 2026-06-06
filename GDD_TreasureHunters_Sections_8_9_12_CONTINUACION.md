# TREASURE HUNTERS IOT: THE LAST SIGNAL
## GDD — Secciones 8, 9 y 12 · CONTINUACIÓN DESDE MINIJUEGO 28

> **Nota editorial:** Este archivo contiene la continuación directa del documento principal.
> El contenido completo arranca desde la mitad del minijuego 28 y cubre el resto de la Sección 12.

---

### 28. RECUPERACIÓN DE ARCHIVO *(continuación)*

- **Objetivo:** Navegar por un árbol de archivos corrupto para encontrar la copia limpia del archivo de control entre múltiples copias dañadas o señuelo.
- **Mecánica:** Árbol de directorios con estructura ramificada. Cada nodo es un archivo (limpio, corrompido o señuelo) o un subdirectorio. El jugador navega tocando nodos. Abrir un archivo consume 3-6 segundos de "inspección": los corrompidos muestran patrones de error; los señuelo muestran checksums falsos pero plausibles; el archivo limpio muestra un checksum verde verificado. El jugador debe identificar el archivo limpio y confirmarlo antes de agotar el tiempo.
- **Duración ideal:** 50-80 segundos
- **Dificultad base:** Difícil
- **Variaciones de dificultad:**
  - Fácil: árbol de 2 niveles, 8 archivos (1 limpio, 4 corrompidos, 3 señuelo), checksum claramente distinto en el limpio
  - Medio: árbol de 3 niveles, 14 archivos (1 limpio, 7 corrompidos, 6 señuelo), checksum sutil, inspección consume 4s
  - Difícil: árbol de 4 niveles, 20 archivos, 2 archivos señuelo con checksums casi idénticos al limpio, inspección consume 5s, algunos directorios están bloqueados y requieren una "llave" de otro archivo primero
  - Experto: árbol de 5 niveles, 28 archivos, el archivo limpio se mueve a otro nodo si el jugador tarda >40s, rutas circulares en el árbol
- **Fallos posibles:** Confirmar un archivo señuelo o corrompido: fallo inmediato. El sistema penaliza con una "cuarentena" de 15s durante la cual el árbol se reorganiza. Máximo 2 confirmaciones erróneas antes de fallo total.
- **Variantes:**
  - *Recuperación Bajo Borrado:* los archivos desaparecen del árbol uno por uno a medida que pasa el tiempo, empezando por el nivel más profundo
  - *Archivo Fragmentado:* el archivo limpio está dividido en 3 fragmentos en 3 ramas distintas del árbol; deben ser encontrados todos para reconstruirlo
- **Recompensa bonus:** +40% XP por encontrar el archivo limpio inspeccionando el mínimo de nodos posibles (ruta óptima)
- **Narrativa:** *"Los archivos operativos de la baliza fueron deliberadamente fragmentados y mezclados con copias señuelo para proteger la inteligencia en caso de captura. Navega el sistema de archivos corrupto y extrae la única copia limpia antes de que el protocolo de borrado definitivo la elimine."*

---

### 29. REINICIO DE SUBRUTINA

- **Objetivo:** Reiniciar las subrutinas del sistema de la baliza en el orden correcto de dependencias, sin activar conflictos de arranque que bloqueen el sistema.
- **Mecánica:** Se presentan entre 6 y 10 subrutinas, cada una con nombre, icono y una lista de "dependencias" (otras subrutinas que deben haberse iniciado antes). El jugador construye el orden de inicio tocando las subrutinas en secuencia. Un grafo de dependencias es visible al inicio (3-5 segundos) y luego se oculta parcialmente. Iniciar una subrutina antes que sus dependencias = conflicto = penalización.
- **Duración ideal:** 45-75 segundos
- **Dificultad base:** Difícil
- **Variaciones de dificultad:**
  - Fácil: 6 subrutinas, grafo de dependencias siempre visible, cadena lineal (solo un orden posible)
  - Medio: 7 subrutinas, grafo visible 5s, árbol con 2-3 ramas (varios órdenes válidos)
  - Difícil: 9 subrutinas, grafo visible 3s, grafo con dependencias circulares aparentes (el jugador debe identificar el ciclo inválido y reportarlo en lugar de iniciarlo)
  - Experto: 10 subrutinas, grafo visible 2s, dependencias con 2 niveles de profundidad, 1 subrutina trampa que nunca debe iniciarse (su inicio provoca fallo inmediato)
- **Fallos posibles:** Conflicto de dependencia: la subrutina iniciada en orden incorrecto se "congela" y bloquea a 2 subrutinas dependientes por 8s. Acumular 3 subrutinas congeladas simultáneamente = fallo.
- **Variantes:**
  - *Subrutinas en Tiempo Real:* el sistema intenta auto-iniciarse en orden incorrecto; el jugador debe cancelar los inicios automáticos incorrectos además de ejecutar los correctos
  - *Reinicio Parcial:* solo un subconjunto de las subrutinas debe reiniciarse; el jugador debe identificar cuáles son necesarias
- **Recompensa bonus:** +35% XP por completar el reinicio en el orden óptimo (mínimo tiempo de espera entre subrutinas)
- **Narrativa:** *"El sistema operativo de la baliza fue apagado de emergencia y sus módulos quedaron en estado inconsistente. Reinicia las subrutinas en el orden correcto de dependencias para restaurar el control sin que el sistema entre en bucle de arranque infinito."*

---

### 30. APAGADO DE EMERGENCIA

- **Objetivo:** Ejecutar la secuencia completa de apagado de emergencia de la red A.R.G.O.S. a través de una infiltración de múltiples fases bajo presión máxima. Este es el minijuego final, reservado para balizas de Clase Omega.
- **Mecánica:** Secuencia de **5 fases encadenadas** sin pausa entre ellas. Cada fase es una versión comprimida de un minijuego distinto seleccionado según el historial del jugador (se elige el tipo con que más ha fallado para maximizar el desafío). Entre fases, el sistema muestra mensajes narrativos de A.R.G.O.S. intentando detectar la intrusión. Si el jugador falla en cualquier fase, no reinicia desde el inicio: reinicia desde la fase fallida con una penalización de tiempo acumulada. La presión psicológica aumenta con cada fase completada.

  **Estructura de las 5 fases:**

  ```
  FASE 1 — ACCESO INICIAL
  ─────────────────────────────────────
  Tipo: Secuencia de Arranque (Mj. 13)
  Tiempo: 20s
  Dificultad: Experto
  Estado A.R.G.O.S.: "Actividad no autorizada detectada. Iniciando protocolo de rastreo..."

  FASE 2 — BYPASS DE SEGURIDAD
  ─────────────────────────────────────
  Tipo: Anulación de Protocolo (Mj. 26)
  Tiempo: 30s
  Dificultad: Experto
  Estado A.R.G.O.S.: "Contramedidas activas. Bloqueando vectores de acceso..."

  FASE 3 — NEUTRALIZACIÓN DE DEFENSA
  ─────────────────────────────────────
  Tipo: Cortafuegos Manual (Mj. 22) + Eliminación de Malware (Mj. 3) simultáneos
  Tiempo: 40s (ambos en pantalla dividida)
  Dificultad: Experto / Experto
  Estado A.R.G.O.S.: "ALERTA NIVEL ROJO. Intrusión en núcleo confirmada. Activando apagado forzado..."

  FASE 4 — EXTRACCIÓN DE DATOS
  ─────────────────────────────────────
  Tipo: Recuperación de Archivo (Mj. 28)
  Tiempo: 25s (árbol simplificado pero bajo presión extrema de tiempo)
  Dificultad: Experto
  Estado A.R.G.O.S.: "Apagado de emergencia en 30 segundos. Los datos serán destruidos."

  FASE 5 — APAGADO FINAL
  ─────────────────────────────────────
  Tipo: Sobrecarga Controlada (Mj. 6) + Calibración de Núcleo (Mj. 7) simultáneos
  Tiempo: 35s (los dos controles se interafectan: el voltaje afecta la velocidad de rotación de los núcleos)
  Dificultad: Experto / Experto
  Estado A.R.G.O.S.: "FALLO CRÍTICO DEL SISTEMA. APAGANDO..." → [VICTORIA]
  ```

- **Duración ideal:** 2:30 - 4:00 minutos totales (suma de fases + transiciones narrativas)
- **Dificultad base:** Experto (no existe versión fácil o media)
- **Variaciones de dificultad:** La dificultad no escala hacia abajo. Solo escala hacia arriba: en Modo Prestige (post-nivel 30), las fases 3 y 5 añaden una tercera pantalla simultánea.
- **Fallos posibles:**
  - Fallo en cualquier fase: reinicio de esa fase con `-15s` de penalización acumulada sobre el temporizador global
  - 3 fallos en la misma fase: la fase se "bloquea" 60s (A.R.G.O.S. refuerza ese vector) antes de reintentarse
  - Temporizador global a cero: la baliza activa su protocolo de autoborrado. El minijuego falla por completo. La baliza queda inactiva 24h reales.
- **Variantes:**
  - *Modo Solitario:* jugador único, todas las fases disponibles
  - *Modo Cooperativo (2 jugadores):* la fase 3 y la fase 5 se dividen entre dos jugadores en dispositivos distintos; cada jugador ve solo su pantalla. Requieren comunicación verbal en tiempo real para coordinar.
  - *Modo Prestige:* disponible para jugadores nivel 30+. Añade una Fase 0 (Decodificación, Mj. 5, 15s) y una Fase 6 post-victoria: "Borrado de huellas" (Inyección de Código Limpio, Mj. 19, 20s). Total: 7 fases.
- **Recompensa bonus:**
  - Completar sin ningún fallo: +100% XP base, título "Fantasma Digital", fragmento de lore Clase Omega
  - Completar en modo cooperativo: +50% XP adicional para ambos jugadores, logro "Operación Conjunta"
  - Completar en Modo Prestige sin fallos: título único "El Último Operativo", cosmético exclusivo de la facción
- **Narrativa:**
  *"Has llegado al núcleo. La señal que has estado rastreando no era solo una baliza: era el nodo de control primario de la red A.R.G.O.S. en tu región. Si lo apagas ahora, toda la red cae. Pero A.R.G.O.S. ya sabe que estás aquí. Tienes entre dos y cuatro minutos antes de que el sistema se autodestruya y borre todo. Esta es la operación para la que te entrenaron. No existe segunda oportunidad."*

---

## 12.1 Sistema de Escalado de Dificultad

### Arquitectura de Generación Procedural

Cada minijuego se genera a partir de un **seed determinista** calculado en el servidor. El seed combina:

```
SEED = HASH( beacon_id + player_id + timestamp_dia + difficulty_class )
```

Esto garantiza que:
- Dos jugadores que intenten la misma baliza el mismo día obtengan el mismo puzzle (experiencia comparable)
- El jugador que intente de nuevo obtenga el mismo puzzle (consistencia de sesión)
- El server puede pre-calcular la solución correcta para validación

### Clases de Baliza y Configuración de Minijuegos

| Clase | Descripción | Minijuegos | Dificultad | Cadena |
|-------|-------------|-----------|-----------|--------|
| **C** | Baliza de entrenamiento / principiante | 1 de 5 tipos aleatorios | Fácil | Individual |
| **B** | Baliza estándar | Elección de 1 entre 3 ofrecidos | Medio | Individual |
| **A** | Baliza avanzada | Sin elección | Difícil | Cadena de 2 secuenciales |
| **S** | Baliza de élite | Sin elección | Experto | Cadena de 3 secuenciales |
| **Omega** | Baliza de evento / única | Fijo (Mj. 30) | Experto | 5 fases (ver Mj. 30) |

### Modificadores de Dificultad por Contexto

Además de la clase de baliza, el sistema aplica modificadores contextuales transparentes al jugador:

| Condición | Modificador |
|-----------|-------------|
| Primera vez jugando ese tipo de minijuego | −1 nivel de dificultad automático |
| Jugador ha fallado ese minijuego 3+ veces seguidas | −1 nivel de dificultad (silencioso, sin aviso) |
| Jugador tiene racha de 5 éxitos sin fallos | +15% velocidad de temporizador |
| Batería del teléfono <15% | Temporizadores +20% (el sistema es consciente del estrés del jugador) |
| Modo accesibilidad activo | Sin penalización por error, temporizadores +40% |

### Distribución de Tipos de Minijuego por Clase

Para asegurar variedad y que el jugador no repita el mismo tipo dos veces seguidas en balizas consecutivas, el sistema mantiene un historial de los últimos 6 minijuegos jugados y excluye esos tipos del pool de selección aleatoria.

```
POOL DE SELECCIÓN (ejemplo para Clase B, seed = ABCD1234)
───────────────────────────────────────────────────────────
Pool completo:           30 tipos
Excluidos (historial):    6 tipos recientes
Pool activo:             24 tipos disponibles
Seed → selecciona 3:     Mj.07, Mj.14, Mj.22 (ofrecidos al jugador)
Jugador elige:           Mj.14 (Estabilización de Voltaje)
Seed → genera puzzle:    Parámetros específicos dentro de la variante Medio
```

---

## 12.2 Consideraciones Anti-Trampa en Minijuegos

### Modelo de Confianza Cliente-Servidor

El sistema de minijuegos opera bajo un modelo de **confianza verificada**:

```
CLIENTE (app móvil)                    SERVIDOR
──────────────────────────────────────────────────────────
1. Solicita minijuego para baliza X    
                                       2. Genera seed determinista
                                       3. Pre-calcula solución correcta
                                       4. Envía: seed + tipo + parámetros
5. Genera puzzle localmente (seed)     
6. Jugador resuelve puzzle             
7. Envía: solución + timestamps        
                                       8. Valida solución vs pre-calculada
                                       9. Valida timestamps vs umbrales
                                       10. Concede o deniega recompensa
```

La solución **nunca se envía al cliente**. El cliente solo recibe el seed para generar el puzzle visualmente. La validación es exclusivamente server-side.

### Detección de Bots por Análisis de Velocidad

Un humano tiene limitaciones físicas de tiempo de reacción y velocidad motora. El servidor establece **umbrales de tiempo mínimo realista** por tipo de minijuego:

| Tipo de Minijuego | Tiempo Mínimo Humano Realista | Umbral Bot (sospecha) |
|-------------------|------------------------------|----------------------|
| Reconexión de Circuitos (5x5, Medio) | 18s | <10s |
| Sincronización de Frecuencias (Medio) | 12s | <5s |
| Eliminación de Malware (Medio) | 20s | <8s |
| Secuencia de Arranque (Fácil) | 8s | <3s |
| Apagado de Emergencia (todas las fases) | 90s | <40s |

Si la solución llega por debajo del umbral bot:
1. **Primera vez:** la recompensa se concede pero se añade flag interno `BOT_SUSPICION_1`
2. **Segunda vez (misma sesión):** recompensa retenida hasta revisión manual, notificación al equipo de integridad
3. **Tercera vez:** suspensión temporal de 24h del sistema de recompensas, revisión obligatoria

### Detección de Patrones de Input Demasiado Perfectos

Los humanos cometen micro-errores en los gestos táctiles: pequeñas desviaciones, velocidades no uniformes, pausas naturales. El servidor analiza los **flujos de eventos de input** enviados junto con la solución:

```
ANÁLISIS DE INPUT (ejemplo: Reconexión de Circuitos)
──────────────────────────────────────────────────────────────────
Input humano típico:
  t=0.000  touch_start (x=142, y=387)
  t=0.083  touch_move  (x=145, y=389)  ← micro-desviación natural
  t=0.167  touch_move  (x=151, y=394)
  t=0.312  touch_move  (x=159, y=400)  ← pausa implícita (corrección)
  t=0.394  touch_end   (x=168, y=407)

Input bot típico:
  t=0.000  touch_start (x=142, y=387)
  t=0.050  touch_move  (x=150, y=397)  ← perfectamente lineal
  t=0.100  touch_move  (x=158, y=407)  ← velocidad constante exacta
  t=0.150  touch_end   (x=166, y=417)  ← sin ninguna corrección
```

El sistema calcula un **Índice de Naturalidad de Input (INI)** entre 0.0 y 1.0. Valores consistentemente cercanos a 1.0 (movimientos perfectamente lineales y uniformes) activan flags de revisión.

### Validación Geográfica de la Sesión

Dado que el juego requiere presencia física, el servidor cruza la solución del minijuego con:

1. **GPS del dispositivo en el momento de la infiltración:** debe estar dentro del radio de la baliza (≤10m tolerancia)
2. **Historial de señal RSSI de la sesión:** el servidor verifica que el cliente reportó señal creciente antes de la infiltración, no un salto directo de "sin señal" a "baliza encontrada"
3. **Velocidad de desplazamiento:** si el jugador "teletransportó" (GPS jumping) entre dos balizas en tiempo imposible para un humano caminando, se invalidan ambas sesiones

```
VALIDACIÓN DE SESIÓN COMPLETA
───────────────────────────────────────────────────────────────
✓ GPS en rango de baliza al iniciar infiltración
✓ Historial RSSI muestra progresión gradual (no salto abrupto)
✓ Tiempo mínimo en zona (>60s antes de infiltración)
✓ Velocidad de desplazamiento consistente con caminar (<7 km/h)
✓ Tiempo de resolución dentro de umbrales humanos
✓ INI dentro de rango natural (0.2 - 0.85)
✓ Solución coincide con pre-cálculo server-side

→ TODOS los checks superados: RECOMPENSA CONCEDIDA
→ 1-2 checks fallidos: RECOMPENSA + FLAG de monitoreo
→ 3+ checks fallidos: RETENCIÓN + REVISIÓN MANUAL
```

### Integridad del Seed y Ventana de Validez

Para evitar que un jugador compute la solución de un puzzle antes de llegar físicamente a la baliza:

- El seed incluye un **timestamp de minuto exacto**: el puzzle generado con seed de las 14:32 solo es válido entre 14:30 y 14:40
- Si la solución llega fuera de la ventana de validez, se rechaza automáticamente
- El servidor no acepta soluciones de puzzles generados más de 10 minutos antes

---

## 12.3 Tabla Resumen de los 30 Minijuegos

| # | Nombre | Tipo de Mecánica | Dificultad Base | Duración Ideal | Interacción Principal |
|---|--------|-----------------|-----------------|----------------|-----------------------|
| 1 | Reconexión de Circuitos | Puzzle de rutas | Medio | 45-90s | Drag continuo |
| 2 | Sincronización de Frecuencias | Ajuste analógico | Fácil | 30-60s | Deslizador |
| 3 | Eliminación de Malware | Tiempo real / tap | Medio | 40-70s | Tap rápido |
| 4 | Patrón de Frecuencia | Memoria + motor | Fácil | 20-45s | Tap secuencial |
| 5 | Decodificación | Lógica / cifrado | Difícil | 60-120s | Selección + entrada |
| 6 | Sobrecarga Controlada | Balance dinámico | Medio | 45-60s | Botones duales |
| 7 | Calibración de Núcleo | Timing / coordinación | Medio | 30-60s | Tap en timing |
| 8 | Derivación de Energía | Puzzle de rutas | Fácil | 40-75s | Tap para rotar |
| 9 | Reensamblaje de Paquetes | Ordenación | Medio | 45-80s | Drag & drop |
| 10 | Purga de Memoria | Memoria + orden | Difícil | 50-90s | Tap secuencial |
| 11 | Bloqueo de Drones | Tiempo real / intercepción | Medio | 35-55s | Drag del bloqueador |
| 12 | Rastreo de Señal | Exploración de grafo | Medio | 50-80s | Tap en nodos |
| 13 | Secuencia de Arranque | Memoria a corto plazo | Fácil | 20-40s | Teclado virtual |
| 14 | Estabilización de Voltaje | Balance con inercia | Difícil | 45-70s | Deslizadores duales |
| 15 | Reparación de Antena | Optimización analógica | Medio | 40-70s | Drag + rotación |
| 16 | Enrutamiento Óptico | Puzzle espacial | Medio | 45-90s | Tap para colocar/rotar |
| 17 | Desbloqueo de Nodo | Memoria + gestos | Fácil | 20-35s | Trazo continuo |
| 18 | Desfragmentación | Puzzle de empaquetado | Medio | 50-90s | Drag & drop + rotación |
| 19 | Inyección de Código Limpio | Lógica + velocidad | Difícil | 40-70s | Selección / escritura |
| 20 | Aislamiento de Virus | Control de área | Difícil | 50-80s | Tap para colocar |
| 21 | Reconstrucción de Llave | Puzzle tipo jigsaw | Medio | 35-65s | Drag + rotación |
| 22 | Cortafuegos Manual | Tower-defense | Difícil | 50-75s | Tap en intersecciones |
| 23 | Balanceo de Frecuencia | Ajuste multibanda | Medio | 45-70s | Deslizadores múltiples |
| 24 | Cifrado Inverso | Deducción lógica | Difícil | 60-100s | Análisis + entrada |
| 25 | Emparejamiento de Relés | Memoria + reconocimiento | Medio | 30-55s | Tap en pares |
| 26 | Anulación de Protocolo | Lógica inversa | Difícil | 35-60s | Selección secuencial |
| 27 | Control de Temperatura | Gestión de recursos | Difícil | 55-85s | Tap en acciones |
| 28 | Recuperación de Archivo | Navegación + deducción | Difícil | 50-80s | Tap para navegar |
| 29 | Reinicio de Subrutina | Ordenación lógica | Difícil | 45-75s | Drag para ordenar |
| 30 | Apagado de Emergencia | Multi-fase (boss) | Experto | 150-240s | Todas las anteriores |

---

## 12.4 Distribución de Tipos de Mecánica

Para garantizar variedad de experiencia durante una sesión de juego, los 30 minijuegos están distribuidos en 8 categorías de mecánica:

```
DISTRIBUCIÓN DE CATEGORÍAS
─────────────────────────────────────────────────────
Categoría                     Minijuegos    % del total
─────────────────────────────────────────────────────
Memoria y secuencias          4, 10, 13, 17      13%
Balance / gestión dinámica    6, 14, 23, 27      13%
Puzzles de rutas / espacial   1, 8, 16, 18       13%
Tiempo real / reacción        3, 11, 22, 25      13%
Lógica / deducción            5, 19, 24, 26      13%
Ajuste analógico              2, 15, 21, 28      13%
Exploración / navegación      12, 20, 29          9%
Multi-fase (boss)             7, 9, 30            9%
─────────────────────────────────────────────────────
TOTAL                         30               100%
```

El sistema de selección aleatoria respeta la distribución de categorías: en una cadena de 3 minijuegos (Clase S), el sistema garantiza 3 categorías distintas.

---

## 12.5 Narrativa Global de los Minijuegos

Los 30 minijuegos no son ejercicios genéricos de habilidad: son acciones con coherencia interna dentro del universo de **TREASURE HUNTERS IOT: THE LAST SIGNAL**. Cada uno representa un paso real en el proceso de infiltración de la red A.R.G.O.S., y su orden de presentación en una cadena sigue una lógica narrativa:

```
ARCO NARRATIVO DE UNA INFILTRACIÓN CLASE S (3 minijuegos)
──────────────────────────────────────────────────────────────
ACTO 1: ACCESO
  Minijuego de tipo "entrada" (Mj. 13, 17, 2, 4...)
  El Hunter fuerza el primer acceso al sistema de la baliza.
  A.R.G.O.S. no ha detectado la intrusión todavía.
  Tensión: moderada. El jugador siente que tiene una oportunidad.

ACTO 2: LUCHA INTERNA
  Minijuego de tipo "combate sistémico" (Mj. 3, 11, 20, 22...)
  El sistema de seguridad responde. El Hunter lucha contra las
  contramedidas activas mientras intenta mantener el acceso.
  Tensión: alta. El temporizador pesa más. A.R.G.O.S. reacciona.

ACTO 3: EXTRACCIÓN / APAGADO
  Minijuego de tipo "resolución" (Mj. 5, 6, 10, 28, 29...)
  La fase final: obtener lo que se vino a buscar y salir antes
  de que el sistema se bloquee definitivamente.
  Tensión: máxima. La música y los hápticos están en su pico.
  Completar este acto = victoria narrativa y mecánica.
──────────────────────────────────────────────────────────────
```

Esta estructura de tres actos aplica a todas las cadenas de minijuegos de Clase A y S. Para Clase Omega (Mj. 30), el arco se expande a cinco actos con su propio climax narrativo.

---

*Fin de la Sección 12 — 30 Minijuegos de Infiltración*

---

**Documento:** GDD · TREASURE HUNTERS IOT: THE LAST SIGNAL
**Versión:** 0.8.2 — Draft para revisión interna
**Secciones cubiertas en este archivo:** 12 (continuación desde Mj. 28) + sistemas de escalado y anti-trampa
**Archivo complementario:** `GDD_TreasureHunters_Sections_8_9_12_PRINCIPAL.md` (Secciones 8 completa, Sección 9 completa, Mj. 1-27)

