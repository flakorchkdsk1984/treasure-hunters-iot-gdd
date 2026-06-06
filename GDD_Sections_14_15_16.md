# TREASURE HUNTERS IOT: THE LAST SIGNAL
## Game Design Document — Versión 1.0
### Secciones 14, 15 y 16: Sistemas Sociales, Economía y Monetización

---

# SECCIÓN 14: CLANES, TERRITORIOS Y SISTEMA SOCIAL

## 14.1 Sistema de Clanes

Los clanes constituyen el núcleo de la experiencia social competitiva de TREASURE HUNTERS IOT. Son organizaciones de jugadores con identidad propia, economía interna, gobernanza jerárquica y capacidad de influir sobre el mundo físico a través de la captura y defensa de territorios.

---

### 14.1.1 Creación de Clanes

Un jugador puede fundar un clan siempre que cumpla los siguientes requisitos mínimos. Este umbral garantiza que los fundadores tengan experiencia suficiente para gestionar una organización.

| Parámetro | Especificación |
|-----------|----------------|
| **Nivel mínimo** | Nivel 15 |
| **Costo de fundación** | 500 Créditos (descontados del inventario del fundador) |
| **Nombre del clan** | 3–20 caracteres; filtro automático de contenido ofensivo (moderación por IA + revisión humana) |
| **Tag del clan** | 3–5 caracteres alfanuméricos en mayúsculas; visible en chat global y rankings |
| **Descripción** | Hasta 500 caracteres; editable por el Líder en cualquier momento |
| **Lema (Motto)** | Hasta 50 caracteres; aparece en el perfil del clan y en la pantalla de guerra |
| **Alineación de facción** | El fundador elige entre: **Los Archivistas**, **Los Liberadores** o **Los Ingenieros** (define bonificaciones territoriales y misiones disponibles) |
| **Emblema** | Selector cosmético: paleta de 32 colores + 60 símbolos base; desbloqueables adicionales por progresión o Gemas |

> **Nota de diseño:** La alineación de facción es permanente para el clan pero no afecta las opciones de facción individual de sus miembros. Crea identidad colectiva sin forzar decisiones individuales irreversibles.

---

### 14.1.2 Estructura de Rangos del Clan

El clan opera con cinco rangos jerarquizados. Cada rango define permisos específicos, límites de cantidad y requisitos de acceso.

#### Tabla de Rangos

| Rango | Título Oficial | Límite por Clan | Requisitos de Acceso |
|-------|---------------|-----------------|----------------------|
| **Rango 5 — Líder** | Commander | 1 | Fundador del clan |
| **Rango 4 — Oficial** | Field Officer | 5 | Promovido por el Líder |
| **Rango 3 — Veterano** | Veteran Hunter | 20% del total de miembros activos | 30 días en el clan + umbral de contribución semanal |
| **Rango 2 — Cazador** | Hunter | Ilimitado | Aceptado mediante reclutamiento |
| **Rango 1 — Recluta** | Recruit | Ilimitado | Solicitud pendiente de revisión |

#### Matriz de Permisos

| Permiso | Commander | Field Officer | Veteran Hunter | Hunter | Recruit |
|---------|:---------:|:-------------:|:--------------:|:------:|:-------:|
| Invitar miembros | ✅ | ✅ | ❌ | ❌ | ❌ |
| Expulsar miembros | ✅ | ✅ (solo Rang. 1–2) | ❌ | ❌ | ❌ |
| Promover / degradar | ✅ | ✅ (hasta Rang. 3) | ❌ | ❌ | ❌ |
| Declarar guerra | ✅ | ✅ | ❌ | ❌ | ❌ |
| Gestionar operaciones territoriales | ✅ | ✅ | ✅ | ❌ | ❌ |
| Realizar donaciones al vault | ✅ | ✅ | ✅ | ✅ | ❌ |
| Acceder al War Chest | ✅ | ✅ | ❌ | ❌ | ❌ |
| Gestionar donaciones del vault | ✅ | ✅ | ❌ | ❌ | ❌ |
| Participar en misiones de clan | ✅ | ✅ | ✅ | ✅ | ❌ |
| Acceder a chat de clan | ✅ | ✅ | ✅ | ✅ | ✅ (solo lectura) |
| Modificar descripción / lema | ✅ | ❌ | ❌ | ❌ | ❌ |
| Disolver el clan | ✅ | ❌ | ❌ | ❌ | ❌ |

> **Regla de protección:** Un Field Officer no puede expulsar a otro Field Officer ni a rangos superiores. Si el Líder está inactivo más de 30 días, el sistema propone automáticamente una elección interna entre los Oficiales activos.

---

### 14.1.3 Sistema de Reclutamiento

El clan puede operar bajo tres modos de ingreso, configurables por el Líder en cualquier momento:

#### Modos de Reclutamiento

| Modo | Descripción | Uso recomendado |
|------|-------------|-----------------|
| **Abierto** | Cualquier jugador que cumpla el nivel mínimo puede unirse sin aprobación | Clanes nuevos en expansión |
| **Por Solicitud** | El jugador envía una solicitud con mensaje opcional; el Líder u Oficial aprueba o rechaza | Clanes establecidos con selección moderada |
| **Solo por Invitación** | Únicamente Field Officers y el Commander pueden enviar invitaciones | Clanes élite o privados |

#### Parámetros Configurables

- **Nivel mínimo de ingreso:** 1–100 (configurable por el Líder; por defecto 1 en modo Abierto)
- **Tamaño máximo del clan:** 50 miembros por defecto
  - Ampliable mediante la mejora de **Expansión de Cuartel** (hasta 100 miembros en Tier 5)
- **Período de prueba de Reclutas:** configurable entre 3 y 14 días antes de la promoción automática a Cazador

#### Sistema de Referidos

- Cuando un recluta que fue invitado por un miembro específico alcanza el **Nivel 20**, el miembro reclutador recibe:
  - **+150 Chatarra**
  - **+1 badge de Reclutador** (acumulable, visible en perfil)
  - Progreso en el logro "Forjador de Cazadores"
- Máximo de 5 bonificaciones por referido por semana (anti-abuso)

---

### 14.1.4 Donaciones y Economía del Clan

El **Vault del Clan** (bóveda común) es el repositorio central de recursos colectivos. Su correcto manejo diferencia a los clanes eficientes de los desorganizados.

#### Límites de Donación Diaria por Miembro

| Recurso | Límite diario por miembro |
|---------|--------------------------|
| Chatarra | 100 unidades |
| Energía | 50 unidades |
| Créditos | 25 unidades |

> El límite reinicia a las **00:00 UTC**. No es acumulable entre días.

#### Uso del Vault del Clan

| Destino | Recurso consumido | Notas |
|---------|------------------|-------|
| Mejoras del cuartel base | Chatarra + Créditos | Costos progresivos por tier |
| Financiar ataques territoriales | Créditos | Extraído del War Chest al declarar guerra |
| Activar misiones de clan especiales | Chatarra + Frags | Desbloquea misiones de Prestige tier |
| Organizar eventos de clan | Créditos | Torneos internos con premios del vault |

#### Leaderboard de Contribución

- **Reinicio:** cada lunes a las 00:00 UTC
- **Rankings:** Top 3 donantes de la semana reciben insignias de **Contribución** (Bronce, Plata, Oro)
- Las insignias son visibles en el perfil durante 7 días y se acumulan en el historial del jugador

#### Sistema de Impuesto Territorial

- Toda captura de beacon en territorio del clan genera un **impuesto automático del 5%** del valor del beacon hacia el Vault
- Este proceso es transparente: el registro de aportes automáticos aparece en el historial del Vault con timestamp y autor
- No requiere acción del jugador; incentiva la defensa activa del territorio

---

### 14.1.5 Misiones de Clan

Las misiones de clan son objetivos colectivos con ventanas de tiempo definidas que requieren coordinación entre miembros. Refuerzan la cohesión del grupo y justifican la comunicación activa dentro del clan.

#### Tipos de Misiones

| Tipo | Descripción | Duración típica |
|------|-------------|-----------------|
| **1. Misión Territorial** | Capturar N beacons en una región específica antes de la medianoche UTC | 12–24 h |
| **2. Misión de Asalto** | Atacar y tomar territorio de un clan enemigo en un distrito designado | 24–48 h |
| **3. Misión de Defensa** | Evitar que el clan enemigo capture beacons del clan durante Y horas consecutivas | 24–48 h |
| **4. Misión de Investigación** | Los miembros recolectan colectivamente N fragmentos de lore | 24–48 h |
| **5. Misión de Prestigio** | Completar un beacon Clase S u Omega en grupo (mínimo 3 miembros) | 48 h |
| **6. Misión Comunitaria** | Contribuir al evento global en curso (progreso compartido con otros clanes) | Variable |

#### Mecánicas de Diseño

- **Ranura simultánea:** máximo 3 misiones activas en paralelo
- **Duración:** 24–48 horas según tipo y dificultad
- **Seguimiento en tiempo real:** barra de progreso colectivo visible en la pantalla del clan
- **Recompensas parciales:** si la misión falla, se otorga el porcentaje de recompensa equivalente al progreso alcanzado (ej.: 60% completado = 60% de la recompensa base)
- **Cadena de misiones:** completar 3 misiones consecutivas sin fallar desbloquea una **Misión Bonus** de mayor recompensa con timer de 12 horas

#### Tabla de Recompensas por Tipo

| Tipo de Misión | Recompensa base (clan) | Recompensa individual |
|----------------|------------------------|----------------------|
| Territorial | 500 Chatarra + 100 Créditos | 50 XP bonus |
| Asalto | 300 Créditos + 200 Chatarra | 75 XP + badge de Asalto |
| Defensa | 400 Chatarra + territorio estabilizado | 50 XP + 1 Tech Frag |
| Investigación | 5–10 Tech Frags + 3 Data Keys | 1 Data Key |
| Prestigio | 1,000 Chatarra + 1 Clan Prestige Point | 200 XP + 2 Tech Frags |
| Comunitaria | Puntos evento global (clan ranking) | Cosmético de evento |

---

### 14.1.6 Guerras de Clanes

Las guerras son el mecanismo competitivo más intenso del sistema social. Representan conflictos territoriales formales con consecuencias económicas reales.

#### Proceso de Declaración

1. El Commander o Field Officer navega a **Panel de Clan → Diplomacia → Declarar Guerra**
2. Se selecciona el clan objetivo (debe ser visible en el mapa territorial)
3. El sistema descuenta **500 Créditos** del War Chest del clan declarante
4. El clan defensor recibe notificación push inmediata con un período de gracia de **2 horas** para prepararse
5. La guerra comienza oficialmente al vencer el período de gracia

#### Parámetros de la Guerra

| Parámetro | Valor |
|-----------|-------|
| **Duración total** | 72 horas |
| **Costo de declaración** | 500 Créditos del War Chest |
| **Cooldown por par de clanes** | 7 días desde el fin de la guerra anterior |
| **Puntuación** | 1 punto por beacon capturado en zona de guerra |
| **Puntos bonus** | +3 puntos por captura en territorio del enemigo (dentro de su zona principal) |
| **Canal dedicado** | War Chat activado automáticamente para ambos clanes |

#### Visualización y Mecánicas de Guerra

- **Mapa de guerra:** los territorios en disputa se resaltan con los colores de ambos clanes en gradiente según control actual
- **Tablero de guerra en tiempo real:** puntos acumulados de cada clan, capturas recientes, miembro más activo
- **Captura en zona de guerra:** cualquier beacon ubicado en el territorio disputado cuenta para la puntuación de guerra

#### Condiciones de Victoria

| Resultado | Criterio | Consecuencia |
|-----------|----------|--------------|
| **Victoria** | Mayor cantidad de puntos de guerra al finalizar las 72 horas | Transferencia de territorio + recursos + puntos de Prestigio del clan |
| **Derrota** | Menos puntos que el rival | Pérdida de hasta 2 distritos disputados + recursos del War Chest |
| **Empate** | Diferencia de ≤5% de los puntos totales | Territorio congelado sin transferencia; ambos ganan puntos de Participación |

#### Bonificaciones de Victoria

- **Transferencia territorial:** hasta 1 distrito del clan derrotado pasa al control del vencedor
- **Botín de guerra:** 30% del War Chest del clan perdedor
- **Puntos de Prestigio de Clan:** +50 PP (visibles en rankings globales)
- **Título temporal:** "Conquistadores de [nombre del clan rival]" durante 7 días

#### Protección Anti-Abuso (Matchmaking por Tamaño)

| Clan atacante (miembros) | Clan defensor mínimo (miembros) |
|--------------------------|---------------------------------|
| 1–10 | 1–15 |
| 11–25 | 8–30 |
| 26–40 | 20–50 |
| 41–50 | 30–50 |

> Un clan de 50 miembros no puede declarar guerra a un clan de 5 miembros. El sistema rechaza la declaración y muestra un mensaje: *"Diferencia de tamaño excesiva: busca un rival más equilibrado."*

---

## 14.2 Sistema Territorial

### 14.2.1 Jerarquía del Mundo

El mundo de juego está estructurado en capas geográficas anidadas. Cada capa tiene significado tanto lúdico como narrativo (los nodos son vestigios de la red ARGOS, los continentes son zonas de influencia de las facciones).

```
MUNDO GLOBAL
  └── Continentes (6 — África, Américas, Asia-Pacífico, Europa, Medio Oriente, Oceanía)
        └── Regiones (~500 — correspondencia aproximada con países/provincias reales)
              └── Distritos (~10 por región — zonas urbanas, áreas naturales, corredores)
                    └── Sectores (~10 por distrito — barrios, polígonos de ~1–5 km²)
                          └── Nodos (beacons individuales, 1–20 por sector según densidad urbana)
```

> **Escala de la red:** En su estado inicial de lanzamiento, el mapa contendrá aproximadamente **250,000 nodos activos** distribuidos globalmente, con crecimiento orgánico basado en densidad de jugadores registrados.

---

### 14.2.2 Mecánica de Captura de Territorio

La captura de territorio es el resultado natural de la actividad de caza de beacons. No existe un mecanismo de "reclamar territorio" separado: el dominio emerge de la actividad agregada.

#### Flujo de Captura

```
Jugador captura beacon
       ↓
Beacon registrado como "influenciado" por su clan
       ↓
¿El clan controla ≥60% de los beacons del sector?
       ↓ SÍ
Sector marcado como "controlado" por el clan
       ↓
¿El clan controla todos los sectores del distrito?
       ↓ SÍ
Distrito bajo control del clan
       ↓
¿El clan controla todos los distritos de la región?
       ↓ SÍ
Región controlada → título regional desbloqueado
```

#### Visualización Territorial

- **Mapa en tiempo real:** los sectores controlados se pintan con el color del clan (código hexadecimal del emblema)
- **Gradiente de disputa:** sectores con control entre 40–60% muestran degradado entre los colores de los dos clanes con mayor presencia
- **Notificaciones push:** cuando el estado territorial del clan cambia (pérdida o ganancia de sector/distrito), todos los miembros activos reciben alerta

---

### 14.2.3 Beneficios de Control Territorial

| Nivel de Control | Requisito | Beneficio Activo |
|-----------------|-----------|-----------------|
| **Sector — Dominado** | 60%+ beacons del sector | +15% XP de todos los beacons capturados en ese sector por miembros del clan |
| **Sector — Controlado** | 100% beacons del sector | +25% XP + producción pasiva de Chatarra activada |
| **Distrito Completo** | Todos los sectores del distrito | +10% producción de recursos en toda la región donde está el distrito |
| **Región Completa** | Todos los distritos de la región | Título regional único (ej.: "Señores del Valle Central") + cosmético de clan exclusivo |
| **Top-10 Global (territorio)** | Ranking de tamaño territorial | Listado permanente en el Hall of Fame + badge de élite en perfil de todos los miembros |

---

### 14.2.4 Producción de Recursos por Territorio

El territorio controlado genera recursos de forma pasiva, creando una economía sostenida que recompensa el dominio territorial activo.

#### Tasas de Producción Pasiva

| Clase del Beacon | Chatarra por hora (inactivo) |
|-----------------|------------------------------|
| Clase C | 1 unidad/hora |
| Clase B | 2 unidades/hora |
| Clase A | 3 unidades/hora |
| Clase S | 5 unidades/hora |
| Omega | 10 unidades/hora |

#### Sistema de Cosecha (Harvest Anti-AFK)

- Los recursos generados **no se otorgan automáticamente**; un miembro del clan debe desplazarse físicamente al sector y activar la **acción de Cosecha** dentro del radio del beacon
- **Acumulación máxima:** 72 horas de producción; los recursos adicionales se pierden (incentiva visitas regulares)
- **Cooldown de cosecha:** 24 horas por beacon por miembro; evita que un solo jugador acapare todas las cosechas

#### Raids de Recursos

- Un clan enemigo que capture un beacon con recursos acumulados obtiene el **30% de los recursos pendientes** del beacon capturado
- Los recursos capturados se transfieren al Vault del clan atacante inmediatamente

---

### 14.2.5 Defensa de Territorio

El sistema de defensa combina mecánicas automáticas con acciones activas de los miembros para crear capas de protección sin eliminar la competencia.

#### Mecanismos de Defensa

| Mecanismo | Descripción | Duración | Costo |
|-----------|-------------|----------|-------|
| **Boost de Defensa Reciente** | Si el mismo clan capturó el beacon hace menos de 24h, la dificultad de captura aumenta un 15% para los atacantes | 24 h automático | Gratis |
| **Scrambling de Señal** | Un miembro puede activar interferencia temporal que oculta el beacon en el radar de jugadores no aliados dentro de un radio de 200m | 4 h | 200 Chatarra del vault |
| **Sistema Centinela** | Notificación push automática enviada a los Oficiales y Veteranos cuando un jugador no aliado se acerca a menos de 100m de un beacon del clan | Permanente mientras esté activo | Configurable en ajustes |

> **Principio de diseño:** ningún mecanismo de defensa hace un beacon **incapturable**; solo incrementa la dificultad o alerta a los defensores. La captura siempre es posible con la habilidad y persistencia suficientes.

---

## 14.3 Sistema Social

### 14.3.1 Amigos y Equipos

#### Lista de Amigos

| Parámetro | Valor |
|-----------|-------|
| Capacidad máxima | 200 amigos |
| Métodos de agregar | Por Hunter ID único, escaneando QR code de perfil en persona, o desde el historial de partido |
| Visibilidad de estado | Online, En caza, En party, Offline (configurable por el usuario) |
| Localización compartida | Solo con amigos que hayan aceptado compartir ubicación en zona (no coordenadas exactas, sino región/distrito) |

#### Sistema de Party (Equipo de Caza)

Una party es un grupo temporal de 2–5 jugadores que coordina su exploración en el mundo real.

| Característica | Descripción |
|---------------|-------------|
| **Escáner compartido** | Cada miembro ve las lecturas RSSI de todos los demás en su HUD (promediadas y codificadas por color por jugador) |
| **Infiltración coordinada** | Si todos los miembros de la party inician el minijuego en el mismo beacon simultáneamente, se activa el modo **Asalto Coordinado** (+25% XP + multiplicador de combo) |
| **Bonus de XP en grupo** | +20% XP para todos los miembros por cada beacon capturado mientras la party está activa |
| **Voz en partido** | Push-to-talk integrado en la app (activable/desactivable individualmente); solo en modo party activo |
| **Radio de party** | Los bonuses de party se aplican si los miembros están dentro de un radio de **500 metros** entre sí |
| **Loot sharing** | Los recursos se reparten equitativamente entre los miembros presentes en la captura |

---

### 14.3.2 Sistema de Chat

El chat está diseñado para facilitar coordinación sin convertirse en un foco de toxicidad. Todas las conversaciones públicas pasan por filtros automáticos de moderación.

| Canal | Acceso | Alcance | Moderación |
|-------|--------|---------|------------|
| **Chat Global** | Todos los jugadores | Mundial | IA + reportes humanos |
| **Chat Regional** | Automático por región geográfica | Región activa del jugador | IA + reportes |
| **Chat de Zona** | Automático por proximidad | Radio de ~2 km del jugador | IA |
| **Chat de Clan** | Miembros del clan | Clan completo | Autogestión + IA |
| **Chat de Guerra** | Miembros de clanes en guerra activa | Ambos clanes en conflicto | IA |
| **Mensajes Directos** | Solo amigos mutuos | 1 a 1 | Filtro + bloqueo |

#### Tipos de Mensajes Soportados

- 📝 **Texto:** hasta 280 caracteres por mensaje
- 😀 **Emojis:** set estándar + emojis exclusivos del juego (capturas, facciones, eventos)
- 📍 **Pin de mapa:** comparte una ubicación aproximada (a nivel de sector, no coordenada exacta)
- 📡 **Beacon ping:** comparte el beacon activo que estás cazando (incluye clase y distancia estimada)
- 🏆 **Logro compartido:** tarjeta automática generada al desbloquear un achievement con botón de compartir

---

### 14.3.3 Sistema de Rankings

Los rankings ofrecen referencia competitiva a múltiples escalas, evitando que un sistema único dominie y desaliente a jugadores nuevos.

| Leaderboard | Métrica | Reinicio | Visibilidad |
|-------------|---------|----------|-------------|
| **Global Vitalicio** | XP total acumulado en toda la historia del jugador | Nunca | Público |
| **Regional Semanal** | XP ganado en la semana en la región del jugador | Domingo 00:00 UTC | Público por región |
| **Global Semanal** | XP total ganado en la semana | Domingo 00:00 UTC | Público |
| **Ranking de Clanes** | Tamaño territorial + puntos de Prestigio | Mensual | Público |
| **Ranking de Eventos** | Puntos específicos del evento activo | Al finalizar el evento | Público durante evento |
| **Ranking de Distancia** | Kilómetros totales caminados en el período | Semanal | Público |

#### Protección Anti-Botting en Rankings

- Las posiciones en ranking son validadas por el sistema de telemetría de movimiento (patrones GPS anómalos activan revisión)
- Jugadores marcados como "pendientes de verificación" no aparecen en el Top-100 global hasta resolución
- Los primeros 10 puestos de cualquier ranking semanal requieren validación de actividad física mínima verificable

---

### 14.3.4 Comercio Limitado

El sistema de intercambio está deliberadamente restringido para proteger la economía del juego y prevenir la creación de mercados grises de dinero real.

| Mecanismo | Permitido | Restricciones |
|-----------|-----------|---------------|
| **Intercambio de cosméticos** | ✅ Solo skins y objetos visuales entre amigos | Máximo 1 regalo por semana; receptor debe ser amigo hace ≥7 días |
| **Mercado interno del clan** | ✅ Donación y solicitud de materiales de crafteo | Solo dentro del clan; no implica transferencia de moneda |
| **Trading de recursos** | ❌ Prohibido | Previene acumulación de recursos externos y grey market |
| **Trading de moneda** | ❌ Prohibido | Previene P2W y mercados externos |
| **Gifting de Gemas** | ❌ Prohibido | Previene compra indirecta de ventajas |
| **Gifting de cosméticos de tienda** | ✅ Un cosmético por semana | Solo a amigos verificados; registro en historial de cuenta |

---

# SECCIÓN 15: ECONOMÍA Y PROGRESIÓN

## 15.1 Monedas y Recursos

TREASURE HUNTERS IOT opera con un ecosistema de **seis recursos diferenciados**, cada uno con fuentes de entrada, sumideros de salida y propósito de diseño específico. La diversidad de recursos previene la simplificación del juego a una sola moneda todopoderosa y crea decisiones económicas significativas.

---

### 15.1.1 Recurso 1 — Chatarra (Scrap)

> *"Los fragmentos de la vieja red tienen valor. Todo cazador lo sabe."*

| Atributo | Valor |
|----------|-------|
| **Clasificación** | Recurso primario de progresión (free-to-earn) |
| **Símbolo** | ⚙️ |
| **Límite diario de obtención** | 1,000 unidades (previene farming automatizado) |
| **Almacenamiento** | Ilimitado (no hay techo de inventario) |

#### Fuentes de Obtención

| Fuente | Cantidad obtenida |
|--------|------------------|
| Beacon Clase C capturado | 5–15 |
| Beacon Clase B capturado | 15–40 |
| Beacon Clase A capturado | 40–100 |
| Beacon Clase S capturado | 100–300 |
| Beacon Omega capturado | 500–2,000 |
| Misiones diarias (bonus) | 50–200 por completar |
| Distancia caminada | 1 por cada 100 metros |

#### Usos (Sumideros)

- Mejoras de escáner (Tier 1–4)
- Fabricación de consumibles (amplificadores de señal, kits de análisis)
- Donaciones al Vault del clan
- Activar defensas territoriales temporales
- Desbloquear ranuras de mejora adicionales

---

### 15.1.2 Recurso 2 — Energía (Energy)

> *"El escáner consume energía. Tú también. Administra bien ambas."*

| Atributo | Valor |
|----------|-------|
| **Clasificación** | Recurso de estamina con regeneración temporal |
| **Símbolo** | ⚡ |
| **Capacidad base** | 100 unidades |
| **Capacidad máxima (con mejora)** | 150 unidades |
| **Tasa de regeneración** | 10 unidades/hora (recarga completa en 10 horas) |

#### Fuentes de Obtención (Bonus Extra)

| Fuente | Cantidad |
|--------|----------|
| Subir de nivel | +10 instantáneo |
| Login diario | +20 |
| Regalo de amigo (una vez al día) | +10 |
| Completar misión de clan | +10–30 según tipo |

#### Usos (Sumideros)

- Activar **Modo de Escaneo Extendido** (+60 min de boost de detección): cuesta 10 Energía
- Acceder a beacons Clase S (requiere 25 Energía de entrada)
- Acceder a beacons Omega (requiere 50 Energía de entrada)

> ⚠️ **Política Anti-P2W:** Las Gemas **no pueden** comprar Energía bajo ninguna circunstancia. La Energía no tiene bypass premium. Esto protege la igualdad competitiva.

---

### 15.1.3 Recurso 3 — Créditos (Credits)

> *"La moneda de los que saben moverse en las sombras de la red."*

| Atributo | Valor |
|----------|-------|
| **Clasificación** | Moneda ganada en juego (no premium) |
| **Símbolo** | 💠 |
| **No comprable** | ❌ No se puede adquirir con dinero real |

#### Fuentes de Obtención

| Fuente | Cantidad |
|--------|----------|
| Contribuciones al clan (recompensa semanal) | 10–50 |
| Recompensas de liga semanal | 50–500 según posición |
| Producción territorial del clan | 1–3/hora por distrito controlado |
| Logros desbloqueados | 10–100 según categoría |
| Misiones de clan completadas | 50–200 |

#### Usos (Sumideros)

- Crear un clan (500 Créditos, costo único)
- Expandir cuartel del clan (+10 slots: 1,000 Créditos por tier)
- Crafteo cosmético avanzado en el Taller de Facciones
- Operaciones en el Trade Post del clan

---

### 15.1.4 Recurso 4 — Gemas (Gems) — PREMIUM

> *"La estética no tiene precio. Pero tiene uno muy razonable."*

| Atributo | Valor |
|----------|-------|
| **Clasificación** | Moneda premium; compra directa con dinero real |
| **Símbolo** | 💎 |
| **Ganable sin pagar** | ❌ No hay alternativa F2P para obtener Gemas |
| **Uso exclusivo** | COSMÉTICOS únicamente |

#### Lo que las Gemas SÍ pueden comprar

- Skins de escáner (temas visuales del radar)
- Marcos de avatar del Hunter
- Efectos de animación de captura de beacon
- Poses de victoria
- Visuals de compañero (mini-dron cosmético)
- Emblemas de clan premium
- Pase de Temporada (track premium)
- Packs narrativos opcionales

#### Lo que las Gemas NUNCA pueden comprar

| Elemento | Razón del bloqueo |
|----------|-------------------|
| XP o aceleración de nivel | Ventaja competitiva directa |
| Mejoras de escáner | Afecta capacidad de juego |
| Recargas de Energía | Bypass de límite de tiempo de juego |
| Chatarra o Créditos | Economía en juego |
| Ventajas en minijuegos | Habilidad de captura |
| Control territorial | Core gameplay |
| Fragmentos de lore | Narrativa como recompensa exclusiva F2P |

---

### 15.1.5 Recurso 5 — Fragmentos Tecnológicos (Tech Fragments)

| Atributo | Valor |
|----------|-------|
| **Clasificación** | Material de crafteo; rareza media-alta |
| **Símbolo** | 🔩 |

#### Fuentes

- Drops raros de beacons Clase A (15% de probabilidad)
- Drops de beacons Clase S (45% de probabilidad)
- Recompensas de eventos estacionales
- Misiones de Investigación del clan

#### Usos

- Craftear mejoras avanzadas de escáner (Tier 3–4)
- Desbloquear habilidades específicas de facción
- Fabricar cosméticos legendarios en el Taller
- Construir mejoras de cuartel de alto nivel

---

### 15.1.6 Recurso 6 — Llaves de Datos (Data Keys)

| Atributo | Valor |
|----------|-------|
| **Clasificación** | Consumible de acceso a contenido narrativo |
| **Símbolo** | 🗝️ |
| **Cap semanal gratuito** | 3 llaves/semana (reinicio domingo 00:00 UTC) |

#### Fuentes

- Asignación semanal gratuita: 3 llaves
- Logros específicos de exploración: 1–3 llaves
- Recompensas de misiones de Investigación del clan: 1–2 llaves
- Niveles de Pase de Temporada gratuito: 1 llave cada 10 niveles

#### Usos

- Desbloquear fragmentos de lore cifrados en beacons especiales
- Acceder a los Archivos ARGOS (repositorio de historia del juego)
- Abrir cápsulas de tiempo narrativas en beacons históricos

> ⚠️ **Política Anti-P2W:** Las Llaves de Datos **no pueden comprarse con Gemas**. El lore es una recompensa exclusiva de la actividad en juego. Ningún jugador puede pagar para ver la historia antes que otros.

---

## 15.2 Economía Balanceada — Fuentes y Sumideros

### 15.2.1 Tabla de Fuentes (Entrada de Recursos)

| Actividad | Chatarra | Energía | Créditos | Tech Frags |
|-----------|:--------:|:-------:|:--------:|:----------:|
| Beacon Clase C | 5–15 | — | — | 1% chance |
| Beacon Clase B | 15–40 | — | 5–10 | 5% chance |
| Beacon Clase A | 40–100 | — | 15–30 | 15% chance |
| Beacon Clase S | 100–300 | — | 50–100 | 45% chance |
| Beacon Omega | 500–2,000 | — | 200–500 | 100% (1–5) |
| Login diario | 25 | +20 | 5 | — |
| Misión diaria completada | 50–200 | +10–30 | 10–25 | 5% chance |
| Distancia caminada | 1/100m | — | — | — |
| Producción territorial (clan) | 1–10/hr | — | 1–3/hr | — |
| Subir de nivel | 50–200 | +10 máx. | 10–50 | 1–5 |
| Recompensa semanal de liga | — | — | 50–500 | 2–10 |
| Completar árbol de logros | — | — | 10–100 | 0–5 |

### 15.2.2 Tabla de Sumideros (Salida de Recursos)

| Gasto | Costo | Tipo |
|-------|-------|------|
| Mejora de escáner Tier 2 | 500 Chatarra | Único |
| Mejora de escáner Tier 3 | 1,500 Chatarra | Único |
| Mejora de escáner Tier 4 | 3,500 Chatarra | Único |
| Mejora Elite (escáner Tier 5) | 8,000 Chatarra + 20 Tech Frags | Único |
| Boost de rango de escáner (consumible) | 100 Chatarra | Por uso |
| Creación de clan | 500 Créditos | Único |
| Expansión del clan (+10 slots) | 1,000 Créditos | Por tier (máx. 5) |
| Boost de defensa territorial | 200 Chatarra | Por uso (24h) |
| Scrambling de señal | 200 Chatarra del vault | Por uso (4h) |
| Declarar guerra de clanes | 500 Créditos del vault | Por declaración |
| Craftear Llave de Datos adicional | 300 Tech Frags | Por llave |
| Habilidad de facción nivel 2 | 500 Tech Frags | Único |
| Expansión de cuartel Tier 3+ | 5,000 Chatarra + 1,500 Créditos | Por tier |

### 15.2.3 Mecanismos Anti-Inflación

| Mecanismo | Descripción | Recurso afectado |
|-----------|-------------|-----------------|
| **Cap diario duro** | Límite absoluto de ganancia diaria por jugador | Chatarra |
| **Sumideros de consumibles** | Costos recurrentes que incentivan gasto activo | Chatarra, Tech Frags |
| **Costos progresivos** | Cada mejora cuesta significativamente más que la anterior | Chatarra, Créditos |
| **Decay territorial** | Recursos no cosechados en 72h se pierden permanentemente | Chatarra (producción) |
| **Resets competitivos** | Rankings semanales/mensuales normalizan la ventaja acumulada | Créditos de liga |
| **Sin transferencia entre jugadores** | Los recursos no se pueden enviar (solo cosméticos) | Todos los recursos |
| **Cap de donación al clan** | Límite diario por miembro al Vault | Chatarra, Energía, Créditos |

---

## 15.3 Sistema de Progresión

### 15.3.1 Niveles 1–100: Umbrales Clave

La curva de niveles sigue una escala logarítmica que recompensa el juego frecuente inicial con progreso rápido, aumentando gradualmente la inversión necesaria para los niveles superiores sin crear muros frustrantes.

#### Fases de Progresión

| Fase | Rango de Nivel | Hitos Principales |
|------|---------------|-------------------|
| **Tutorial y Fundamentos** | 1–10 | Tutorial completo; todos los beacons Clase C; presentación de facciones; primeras misiones diarias |
| **Explorador** | 10–25 | Desbloqueo de clanes (Nv. 15); acceso a beacons Clase B; territorios básicos; mercado de clan |
| **Cazador Avanzado** | 25–50 | Elección de especialización (Árbol de Habilidades); escáner avanzado; beacons Clase A; guerras territoriales; acceso completo al sistema social |
| **Élite** | 50–75 | Escáner élite; acceso a beacons Clase S; habilidades de facción desbloqueadas; rankings de alto nivel |
| **Maestro Cazador** | 75–100 | Acceso a beacons Omega; Maestría disponible; misiones de Prestige; Hall of Fame elegible |

#### Curva de XP Requerido por Nivel

| Transición | XP Requerido | XP Acumulado |
|-----------|:------------:|:------------:|
| L1 → L2 | 500 | 500 |
| L5 → L6 | 2,000 | 7,500 |
| L10 → L11 | 6,000 | 28,500 |
| L25 → L26 | 25,000 | 235,000 |
| L50 → L51 | 80,000 | 1,800,000 |
| L75 → L76 | 200,000 | 7,500,000 |
| L99 → L100 | 500,000 | 22,000,000 |

#### Referencias de Progreso para Jugador F2P Activo

| Actividad diaria | XP estimado/día |
|-----------------|:--------------:|
| 5–10 beacons Clase C | 250–500 |
| 2–3 beacons Clase B | 300–600 |
| 1 beacon Clase A | 400–800 |
| Misión diaria completada | 500–1,500 |
| Distancia caminada (2–5 km) | 200–500 |
| **Total estimado (jugador activo)** | **2,000–5,000 XP/día** |

- **Tiempo estimado para alcanzar Nivel 50:** ~30 días de juego activo
- **Tiempo estimado para alcanzar Nivel 100:** ~90 días de juego activo

---

### 15.3.2 Árbol de Habilidades (Skill Tree)

A partir del **Nivel 25**, el jugador elige una de tres especializaciones. Esta elección define su rol preferido en el juego y puede cambiarse cada **30 días** (para prevenir arrepentimiento pero mantener peso en la decisión).

#### Path 1 — Buscador (Seeker) 🔍

*Especialización en detección y localización eficiente de beacons.*

| Habilidad | Nivel de desbloqueo | Efecto |
|-----------|:------------------:|--------|
| Rango de Escáner Ampliado | 25 | +20% de alcance de detección |
| Detección Rápida de Gradiente | 30 | Transición frío→caliente 30% más rápida |
| Filtro de Ruido Avanzado | 35 | Reduce lecturas falsas en -40% |
| Análisis de Terreno | 45 | Bonus de precisión en exteriores naturales (+15% RSSI accuracy) |
| **Pasiva: Instinto de Cazador** | 50 | Si un beacon no ha sido detectado en 2 minutos, el HUD muestra una dirección vaga aproximada |

#### Path 2 — Infiltrador (Infiltrator) ⚡

*Especialización en velocidad y eficiencia en la fase de captura.*

| Habilidad | Nivel de desbloqueo | Efecto |
|-----------|:------------------:|--------|
| Tiempo Extendido de Minijuego | 25 | +20% de tiempo en todos los minijuegos de captura |
| Vida Extra (B+) | 30 | 1 vida adicional en beacons Clase B o superior |
| Penalización Reducida | 35 | El lockout por fallo se reduce un 50% |
| Multiplicador de Cadena | 45 | Capturas consecutivas sin fallo acumulan un multiplicador de XP (+10% por captura, máx. ×3) |
| **Pasiva: Manos Rápidas** | 50 | Al iniciar cualquier minijuego, una pista se revela automáticamente desde el primer segundo |

#### Path 3 — Comandante (Commander) 🏴

*Especialización en liderazgo, coordinación de clan y dominio territorial.*

| Habilidad | Nivel de desbloqueo | Efecto |
|-----------|:------------------:|--------|
| Bonus Territorial de Clan | 25 | +10% a todos los beneficios de XP en territorio del clan |
| Party Ampliada | 30 | El tamaño máximo de la party aumenta a 6 jugadores |
| Boost de Defensa Mejorado | 35 | El boost de defensa del beacon alcanza +25% (vs +15% base) |
| Producción Intensificada | 45 | +20% a la producción de recursos en territorio controlado |
| **Pasiva: Liderazgo** | 50 | Los boosts de XP activos del Comandante se aplican también a todos los miembros de la party en radio de 500m |

---

### 15.3.3 Mejoras de Escáner

El escáner es el equipo central del jugador. Sus mejoras son permanentes, progresivas y no están detrás de paywalls.

```
Scanner Tier 1 (Base)
    └── Tier 2 — "Módulo de Amplificación"
          └── Tier 3 — "Procesador de Señal"
                └── Tier 4 — "Sistema Neural"
                      └── Tier Elite — "Escáner ARGOS"
```

#### Estadísticas por Tier

| Estadística | Tier 1 | Tier 2 | Tier 3 | Tier 4 | Elite |
|-------------|:------:|:------:|:------:|:------:|:-----:|
| **Rango de detección** | Base | +15% | +30% | +50% | +75% |
| **Frecuencia de lectura** | 1/seg | 1.5/seg | 2/seg | 3/seg | 4/seg |
| **Filtro de ruido** | Básico | Mejorado | Avanzado | Profesional | Militar |
| **Precisión háptica** | ± 5m | ± 4m | ± 3m | ± 2m | ± 1m |
| **Rango de audio** | 5m | 8m | 12m | 18m | 25m |
| **Eficiencia de batería** | Base | +10% | +20% | +35% | +50% |

#### Costos de Mejora

| Mejora | Chatarra | Tech Frags | Nivel requerido |
|--------|:--------:|:----------:|:--------------:|
| Tier 1 → Tier 2 | 500 | — | 10 |
| Tier 2 → Tier 3 | 1,500 | 5 | 25 |
| Tier 3 → Tier 4 | 3,500 | 15 | 50 |
| Tier 4 → Elite | 8,000 | 20 | 75 |

---

### 15.3.4 Sistema de Prestigio

El Prestigio es el sistema de metajuego para jugadores que alcancen el Nivel 100 y deseen extender su experiencia.

| Parámetro | Valor |
|-----------|-------|
| **Disponibilidad** | A partir del Nivel 100 (voluntario, no forzado) |
| **Qué se reinicia** | Nivel del jugador (vuelve a 1), recursos acumulados en inventario activo |
| **Qué se conserva** | Cosméticos, lore descubierto, skins de escáner, títulos, amigos, clan, Árbol de Habilidades |
| **Recompensa de Prestigio** | Estrella de Prestigio (P1–P10) visible en el perfil; estadística base ligeramente mejorada; cosmético exclusivo de Prestige |
| **Significado competitivo** | Los jugadores Prestige son elegibles para las ligas de élite y los eventos Omega exclusivos |

> **Filosofía del Prestigio:** No es un muro de contenido. Es una elección de re-jugabilidad que recompensa a los más dedicados con identidad visual y acceso a contenido especializado, sin crear una brecha de poder infranqueable con los jugadores regulares de Nivel 100.

---

### 15.3.5 Logros y Colecciones

#### Sistema de Logros

- **200+ logros** organizados en categorías temáticas
- Los puntos de logro se acumulan en el **Marcador de Maestría** del jugador
- Los puntos de Maestría desbloquean cosméticos únicos en el Árbol de Recompensas de Logros

| Categoría | Ejemplos de logros |
|-----------|-------------------|
| **Exploración** | "Primera señal detectada", "100 beacons capturados", "Omega por primera vez" |
| **Distancia** | "Primer kilómetro caminado", "Maratonista: 500 km totales" |
| **Social** | "Unirse a un clan", "Primer referido a Nivel 20", "Ganar una guerra de clanes" |
| **Colección** | "Completar set de Clase C en región", "Todos los logs de ARGOS Sector 1" |
| **Habilidad** | "Captura perfecta: 5 seguidas sin fallo", "Infiltrador: cadena de ×3" |
| **Temporal** | "Participar en el Evento de Apertura de Temporada" |

#### Sistema de Colecciones

- Cada región tiene un **set de beacons coleccionables** por clase
- Completar el set de una región desbloquea un título único de esa región (ej.: *"Explorador de los Andes"*)
- Las colecciones son permanentes y se muestran en el perfil del jugador

#### Sistema de Títulos

- **100+ títulos únicos** obtenibles por logros, colecciones, eventos, Prestige y clan
- El jugador selecciona 1 título activo para mostrar bajo su nombre en el juego
- Los títulos son indicadores de historia y dedicación; ninguno es comprable con Gemas

---

# SECCIÓN 16: MONETIZACIÓN — DISEÑO F2P SOSTENIBLE

## 16.1 Filosofía de Monetización

### Regla de Oro
> **"El jugador que paga debe verse más impresionante, no más poderoso."**

El modelo de monetización de TREASURE HUNTERS IOT está construido sobre un principio no negociable: el dinero real **nunca compra ventaja competitiva**. Un jugador F2P dedicado debe poder alcanzar cualquier posición en cualquier ranking, capturar cualquier beacon y participar en cualquier guerra en igualdad de condiciones con un pagador.

Los jugadores que invierten dinero reciben recompensas estéticas, narrativas y de conveniencia que enriquecen su experiencia individual sin degradar la de los demás.

### Métricas Objetivo

| Métrica | Objetivo | Benchmark de referencia (juegos similares) |
|---------|---------|------------------------------------------|
| **Tasa de conversión F2P → Pagador** | 3–8% | 2–5% (industria móvil casual) |
| **ARPU** (Average Revenue Per User) | $2–5 / mes | $1.50–4.00 |
| **ARPPU** (Average Revenue Per Paying User) | $15–30 / mes | $12–25 |
| **Retención D30** | >40% | 25–35% en móvil |
| **Retención D90** | >20% | 10–15% en móvil |
| **NPS** (Net Promoter Score) | >50 | 30–45 |

> El objetivo de diseño es que **ningún jugador F2P sienta que está en desventaja por no pagar**. Si las encuestas de satisfacción muestran lo contrario, el balance debe ajustarse antes de cualquier nueva iniciativa de monetización.

---

## 16.2 La Carta Anti-P2W (Lo Prohibido)

Esta carta es vinculante para todos los sistemas de diseño, actualización de contenido y decisiones de producto. Ninguna presión comercial justifica su violación.

| Elemento | Estado | Justificación |
|----------|:------:|---------------|
| ❌ Venta de XP o aceleración de nivel | **PROHIBIDO** | Crea brecha de progresión irreversible entre pagadores y F2P |
| ❌ Venta de mejoras de escáner con Gemas | **PROHIBIDO** | El escáner es la herramienta de juego; pagarlo es P2W directo |
| ❌ Recargas de Energía con Gemas | **PROHIBIDO** | Bypass del límite de sesión; ventaja en velocidad de progresión |
| ❌ Ventajas en minijuegos de captura | **PROHIBIDO** | La habilidad de captura es el núcleo del gameplay |
| ❌ Manipulación de rankings competitivos | **PROHIBIDO** | Los rankings son el sistema de reconocimiento meritocrático |
| ❌ Ventajas territoriales por pago | **PROHIBIDO** | El dominio territorial es el competitivo principal del clan |
| ❌ Paywall de lore | **PROHIBIDO** | La narrativa es recompensa de exploración; no debe segmentarse por gasto |
| ❌ Llaves de Datos por Gemas | **PROHIBIDO** | Previene acceso exclusivo de pago al contenido de historia |
| ❌ Venta de Créditos | **PROHIBIDO** | Los Créditos son resultado de logros en juego; comprarlos desvincula el sistema de progresión |

---

## 16.3 Fuentes de Ingresos (Lo Permitido)

### 16.3.1 Cosméticos Directos — Tienda de Cazadores

La tienda ofrece objetos visuales de compra directa con Gemas. Los precios están calibrados para ser accesibles sin crear presión de gasto.

#### Catálogo Base

| Categoría de Cosmético | Descripción | Precio (USD equivalente) |
|----------------------|-------------|:------------------------:|
| **Skins de Escáner** | Temas visuales del radar: Militar, Neón, Retro, Biomecánico, Holográfico | $1.99–$4.99 |
| **Marcos de Avatar** | Bordes y fondos animados del perfil del Hunter | $0.99–$2.99 |
| **Efectos de Captura** | Animaciones de infiltración de beacon (explosión, disrupción, glitch) | $1.99–$3.99 |
| **Poses de Victoria** | Animaciones de celebración tras captura exitosa | $1.99 |
| **Companion Visual** | Mini-dron cosmético que sigue al jugador en el HUD | $4.99–$9.99 |
| **Emblemas de Clan Premium** | Diseños exclusivos de alto detalle para emblema del clan | $2.99 |
| **Efectos de Trail** | Rastro visual en el mapa mientras el jugador se desplaza | $2.99–$4.99 |

> **Política de Tienda:** ningún artículo de la tienda estará disponible por un tiempo limitado sin previo aviso de 7 días. Los artículos de temporada tienen ventana de compra claramente marcada.

---

### 16.3.2 Pase de Temporada (Battle Pass)

El Pase de Temporada es el principal vehículo de ingresos recurrentes. Su diseño prioriza la satisfacción del comprador sobre la presión de compra.

| Parámetro | Track Gratuito | Track Premium |
|-----------|:--------------:|:-------------:|
| **Costo** | 0 | $9.99 / temporada |
| **Duración** | 3 meses | 3 meses |
| **Niveles** | 50 | 50 |
| **Contenido** | Chatarra, Créditos, títulos, cosméticos básicos, 1 skin de escáner al completar | Todo lo del track gratuito + cosméticos exclusivos, 2× drops de cosméticos, pack narrativo bonus, título de Temporada |
| **Progreso** | XP de juego normal | Mismo XP; nunca requiere compra adicional |

#### Principios del Pase de Temporada

- **Nunca vende poder:** todos los premios del track premium son cosméticos o narrativos
- **Completable F2P (cosméticamente):** el track gratuito siempre incluye al menos 1 skin de escáner y 1 título únicos
- **Sin presión de tiempo artificial:** los jugadores que compren el pase en el último mes del trimestre reciben acceso retroactivo al progreso (desbloquean inmediatamente los niveles ya obtenidos)
- **Sin caducidad de recompensas:** los ítems ganados en el pase permanecen en el inventario para siempre

---

### 16.3.3 Packs Narrativos Opcionales

Los packs narrativos ofrecen profundidad de lore adicional para jugadores que desean sumergirse en la historia del mundo. Son complementarios, no necesarios para entender la trama principal.

| Pack | Contenido | Precio |
|------|-----------|:------:|
| **"Archivos Clasificados"** | 10 audio logs de lore profundo (20–40 min de contenido de audio) + efecto de escaneo personalizado "Protocolo ARGOS" | $3.99 |
| **"Memorias de A.R.G.O.S."** | 5 logs en primera persona desde la perspectiva de la IA ARGOS + título exclusivo "Oyente de ARGOS" + skin de escáner "Kernel Primario" | $4.99 |
| **"Los Archivistas: Caso Rojo"** | Arco narrativo expandido de la facción Archivistas: 8 documentos, 3 grabaciones, mapa anotado exclusivo | $5.99 |

> **Clasificación:** estos packs son **cosméticos + narrativos**. No otorgan XP, recursos, ventajas de escaneo ni acceso anticipado a beacons.

---

### 16.3.4 Pase de Cazador (Suscripción Mensual)

| Parámetro | Valor |
|-----------|-------|
| **Precio** | $4.99 / mes |
| **Renovación** | Automática; cancelable en cualquier momento sin penalización |

#### Beneficios del Pase de Cazador

| Beneficio | Descripción | Clasificación |
|-----------|-------------|:-------------:|
| +10% de producción de Chatarra | Solo sobre Chatarra, nunca sobre XP | Conveniencia |
| 2× recompensas de misiones diarias (track cosmético) | Los drops de cosméticos diarios se duplican, no el XP | Cosmético |
| Sin publicidad | Si el modelo de anuncios opcionales se implementa, el Pase los elimina | Conveniencia |
| 1 ítem cosmético mensual exclusivo | Skin, marco o efecto disponible solo para suscriptores activos | Cosmético |
| Acceso anticipado a la tienda de temporada | 24 horas antes que el resto | Cosmético / Conveniencia |

> ⚠️ **Límite estricto:** el Pase de Cazador **no otorga** ventajas de combate, progresión de nivel, energía adicional, ni acceso anticipado a beacons o contenido competitivo.

---

### 16.3.5 Cosméticos de Eventos (Tienda de Eventos)

Los eventos especiales tienen su propia tienda temporal con cosméticos únicos que no se venderán en ningún otro momento.

| Parámetro | Valor |
|-----------|-------|
| **Disponibilidad** | Solo durante la ventana del evento (7–21 días según tipo) |
| **Caducidad** | Expiran de la tienda al cerrar el evento; **nunca del inventario del jugador** |
| **Precios** | $2.99–$9.99 según complejidad del cosmético |
| **Reoferta** | Posible reoferta 1 vez por año; nunca de forma continua |

> La escasez temporal es legítima como palanca de urgencia, pero los jugadores que los compran deben sentir que los poseen permanentemente. Un cosmético de evento que "desaparece" del inventario del comprador destruye la confianza.

---

### 16.3.6 Merchandising Físico (Roadmap Año 2+)

La extensión al mundo físico capitaliza la naturaleza IoT del juego y crea coleccionables con valor intrínseco.

| Producto | Descripción | Precio estimado |
|----------|-------------|:---------------:|
| **Hunter Kit Físico** | Réplica de beacon ESP32 coleccionable con carcasa impresa; puede actuar como decoración funcional | $29.99 |
| **Hunter Journal** | Libro físico de lore con arte conceptual, mapas de ARGOS y documentos in-universe | $24.99 |
| **Merchandising General** | Ropa, pins, parches bordados con iconografía del juego | $9.99–$39.99 |
| **Beacon de Hogar/Eventos (Licenciado)** | Beacon funcional (ESP32/BLE) que puede configurarse como beacon del juego en ubicaciones privadas | $49.99 |

> El merchandising físico refuerza la identidad de la IP y crea un canal de ingresos sin impacto en el balance del juego en línea.

---

## 16.4 Modelo Económico a 10 Años

### 16.4.1 Estructura de Temporadas

Cada temporada dura **3 meses** y funciona como un ciclo completo de contenido, narrativa y monetización.

| Elemento por temporada | Descripción |
|----------------------|-------------|
| Nuevo arco narrativo | Avance en la historia principal de ARGOS con nuevo lore descubrible |
| Nuevo tema cosmético | Paleta de cosméticos coherente con el arco (ej.: Temporada 1 = "El Despertar"; Temporada 2 = "La Fractura") |
| Nuevo Pase de Temporada | Track gratuito + premium con cosméticos del tema |
| Nuevos cosméticos de evento | Al menos 1 evento especial por temporada con tienda dedicada |
| Rotación de contenido de beacons | Nuevos tipos de misiones, modificadores estacionales en beacons |

### 16.4.2 Fases de Crecimiento del Negocio

#### Año 1 — Fundación

| Parámetro | Objetivo |
|-----------|---------|
| Foco principal | Establecer hábitos de monetización sana; construir confianza con la base de jugadores |
| Temporadas | 4 (Q1–Q4) |
| Iniciativas clave | Lanzamiento del Pase de Temporada; primeros eventos especiales; tienda base |
| ARPU objetivo | $1.50 / mes |
| DAU objetivo | 10,000–50,000 |

#### Años 2–3 — Expansión

| Parámetro | Objetivo |
|-----------|---------|
| Foco principal | Diversificación de ingresos; merchandising físico; comunidad establecida |
| Iniciativas clave | Lanzamiento del Hunter Kit físico; packs narrativos por facción; cosméticos de clan |
| ARPU objetivo | $3.00 / mes |
| DAU objetivo | 100,000–300,000 |

#### Años 4–7 — Madurez

| Parámetro | Objetivo |
|-----------|---------|
| Foco principal | Herramientas de creador; licencias IP; eventos regionales premium |
| Iniciativas clave | Herramienta de cosméticos de comunidad (player-submitted skins); licencias a organizadores de eventos; colaboraciones con marcas alineadas |
| ARPU objetivo | $4.50 / mes |
| DAU objetivo | 500,000–1,000,000 |

#### Años 8–10 — Legado

| Parámetro | Objetivo |
|-----------|---------|
| Foco principal | Colaboraciones IP cruzadas; expansión de producto físico; eventos de realidad mixta |
| Iniciativas clave | Crossovers con otras IPs de juegos; evento físico anual ("Hunter Con"); línea de producto físico ampliada |
| ARPU objetivo | $5.00 / mes |
| DAU objetivo | 1,000,000+ |

---

### 16.4.3 Proyección de Ingresos por Escala

La siguiente tabla modela ingresos mensuales bajo los supuestos de conversión del 5% y ARPU de $3.00 (año 2–3 de operación).

| DAU | Pagadores (5% conv.) | ARPU $3.00 | Ingresos mensuales estimados | Fase |
|-----|:--------------------:|:----------:|:----------------------------:|------|
| 1,000 | 50 | $150 | **$150** | Seed stage / early access |
| 10,000 | 500 | $1,500 | **$1,500** | Early revenue / validación |
| 100,000 | 5,000 | $15,000 | **$15,000** | Viable / break-even posible |
| 500,000 | 25,000 | $75,000 | **$75,000** | Rentable / equipo ampliado |
| 1,000,000 | 50,000 | $150,000 | **$150,000** | Scale target / IP consolidada |

> **Nota de proyección:** estas cifras son conservadoras. Un ARPPU de $15–30 (pagadores comprometidos) eleva significativamente los ingresos reales. Un jugador que adquiera el Pase de Temporada ($9.99) + Pase de Cazador ($4.99/mes × 3) en una sola temporada representa $24.96, muy por encima del ARPU base.

---

### 16.4.4 Métricas de Salud del Ecosistema de Monetización

Además de los ingresos, se monitorizarán continuamente los siguientes indicadores para detectar desequilibrios o señales de insatisfacción:

| Métrica | Señal saludable | Señal de alerta |
|---------|:--------------:|:--------------:|
| % de jugadores F2P que llegan a Nivel 50 | >30% | <15% |
| NPS de jugadores F2P | >45 | <25 |
| Tasa de abandono D7 | <40% | >60% |
| Quejas de P2W en encuestas | <5% mencionan P2W | >15% |
| Ingresos por cosmético vs. ingresos totales | >95% | <80% (indica derivación hacia P2W) |
| Ratio de Pase/Cosmético directo | 60% Pase, 40% directo | — (solo referencia de mix) |

---

*Fin de las Secciones 14, 15 y 16 — TREASURE HUNTERS IOT: THE LAST SIGNAL GDD v1.0*

---

> **Control de versiones:**
> - v0.1 — Borrador inicial: estructura y placeholders
> - v0.5 — Revisión de economía y balance
> - v1.0 — Versión de referencia para producción
>
> **Próximas secciones:** Sección 17 (Sistemas Técnicos IoT), Sección 18 (Moderación y Seguridad), Sección 19 (Roadmap de Producción)
