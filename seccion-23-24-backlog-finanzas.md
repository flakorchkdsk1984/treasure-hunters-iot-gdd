# TREASURE HUNTERS IOT: THE LAST SIGNAL
## Game Design Document — Secciones 23 y 24

---

# SECCIÓN 23: BACKLOG — 100 HISTORIAS DE USUARIO PRIORIZADAS

> **Convenciones de formato**
>
> | Campo | Valores posibles |
> |---|---|
> | **Prioridad** | P0 = Crítico/MVP · P1 = Alta · P2 = Media · P3 = Baja/Nice-to-have |
> | **Estimación** | XS = 1 día · S = 2-3 días · M = 1 semana · L = 2 semanas · XL = 1 mes |
> | **Dependencias** | ID de historia(s) previas requeridas |

---

## ÉPICA 1: ONBOARDING

> **Objetivo:** Llevar al jugador desde la instalación hasta su primera captura completada con total comprensión del loop central del juego.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **OB-01** | **Como** nuevo jugador, **quiero** crear una cuenta con mi correo electrónico **para** acceder al juego de forma segura y personalizada. | ✅ Formulario valida formato de email en tiempo real. ✅ Contraseña requiere mínimo 8 caracteres, 1 mayúscula, 1 número. ✅ Correo de verificación enviado en < 30 s. ✅ El jugador puede iniciar sesión solo tras verificar email. ✅ Mensaje de error claro si el email ya está registrado. ✅ Flujo completo en < 3 pantallas. | P0 | S | — |
| **OB-02** | **Como** nuevo jugador, **quiero** registrarme con Google o Apple **para** no tener que crear y recordar otra contraseña. | ✅ Botones OAuth visibles en pantalla de bienvenida. ✅ Flujo completa en < 2 taps tras autorización externa. ✅ Foto de perfil importada automáticamente (con opción de cambiar). ✅ Token almacenado de forma segura (Keychain/Keystore). ✅ Funciona sin conexión previa a cuenta del juego. | P0 | S | OB-01 |
| **OB-03** | **Como** nuevo jugador, **quiero** un tutorial guiado con una baliza de práctica virtual **para** aprender las mecánicas sin necesidad de estar cerca de hardware real. | ✅ Baliza tutorial simulada aparece en radar al completar registro. ✅ Narrador/texto guía cada paso con flecha o spotlight. ✅ El jugador debe realizar cada acción (no solo leer) para avanzar. ✅ Tutorial completa en 5-8 min según pruebas de usuario. ✅ Progreso del tutorial guardado; si el jugador cierra y vuelve, continúa donde lo dejó. | P0 | L | OB-01 |
| **OB-04** | **Como** nuevo jugador, **quiero** aprender los estados visuales del escáner BLE **para** entender qué significa cada color y animación antes de salir a buscar balizas reales. | ✅ Tutorial muestra secuencia: Sin señal → Lejano → Próximo → Encontrado. ✅ Cada transición acompañada de animación, color y vibración correctos. ✅ El jugador identifica correctamente los 4 estados en una prueba breve (3/3 respuestas correctas). ✅ Opción de repetir esta sección desde ajustes. | P0 | M | OB-03 |
| **OB-05** | **Como** nuevo jugador, **quiero** elegir mi facción inicial **para** sentir pertenencia y que mis decisiones tengan impacto desde el primer momento. | ✅ Pantalla de selección presenta 3 facciones con lore resumido (< 60 palabras c/u) e icono distintivo. ✅ Animación de "reclutamiento" al confirmar elección. ✅ La facción seleccionada aparece en perfil y HUD inmediatamente. ✅ Advertencia clara: la facción puede cambiarse tras 30 días. ✅ Estadísticas globales de distribución de facciones mostradas como contexto. | P0 | M | OB-01 |
| **OB-06** | **Como** nuevo jugador, **quiero** jugar mi primer minijuego guiado **para** entender las mecánicas de descifrado antes de enfrentarme a una baliza real. | ✅ Minijuego de práctica usa dificultad fija (Nivel 1). ✅ Pasos del minijuego explicados con overlay antes de iniciar. ✅ Errores en tutorial no penalizan vidas. ✅ Completar el minijuego desbloquea el "Parche de Iniciado" cosmético. ✅ Duración del tutorial de minijuego < 3 min. | P0 | M | OB-03 |
| **OB-07** | **Como** nuevo jugador, **quiero** ver mi primera recompensa de XP y subir de nivel **para** experimentar el ciclo de gratificación antes de terminar el onboarding. | ✅ Pantalla de recompensa muestra XP ganado, nivel alcanzado y recursos desbloqueados. ✅ Animación de "level up" con efectos de partículas. ✅ Resumen de lo que se desbloquea al siguiente nivel visible. ✅ Transición fluida a pantalla principal del juego tras cerrar la pantalla de recompensa. | P0 | S | OB-06 |
| **OB-08** | **Como** nuevo jugador, **quiero** que el juego me pida permiso de notificaciones push de forma contextual y explicada **para** decidir con información si quiero recibirlas. | ✅ Solicitud de permiso aparece solo después de que el jugador complete el tutorial de baliza. ✅ Pantalla previa al diálogo del SO explica los beneficios (alertas de eventos, ataques al clan). ✅ Si el jugador deniega, flujo continúa normalmente sin bloquearse. ✅ Desde ajustes puede reactivar la solicitud en cualquier momento. | P0 | S | OB-03 |
| **OB-09** | **Como** nuevo jugador, **quiero** que el juego me pida permiso de ubicación explicando por qué lo necesita **para** confiar en el uso de mis datos antes de concederlo. | ✅ Explicación en pantalla propia: "Usamos tu ubicación para mostrar balizas cercanas. Nunca almacenamos tu posición exacta." ✅ Solicita permiso "Solo mientras se usa la app" (no siempre). ✅ Si se deniega, juego muestra mapa estático y explica funcionalidad reducida. ✅ No se solicita el permiso antes de que el jugador haya completado OB-04. | P0 | S | OB-04 |
| **OB-10** | **Como** jugador experimentado (reinstalación), **quiero** poder saltarme el tutorial **para** llegar al juego principal lo antes posible. | ✅ Opción "Saltar tutorial" visible desde la segunda pantalla del onboarding. ✅ Confirmación con resumen de lo que se omite antes de confirmar el skip. ✅ Al saltar, el jugador recibe igualmente los recursos base del onboarding (sin ventaja por completarlo). ✅ Puede acceder a tutorial desde el menú Ayuda en cualquier momento. ✅ El skip se registra en telemetría para análisis de UX. | P1 | XS | OB-01 |

---

## ÉPICA 2: ESCANEO BLE

> **Objetivo:** Proveer una capa de detección de balizas Bluetooth Low Energy confiable, eficiente en batería y con retroalimentación clara al jugador.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **BLE-01** | **Como** jugador, **quiero** activar el escáner BLE con un solo tap **para** comenzar a buscar balizas rápidamente. | ✅ Botón de escaneo visible en pantalla principal con estado ON/OFF claro. ✅ Escáner inicia en < 1 s tras el tap. ✅ Estado del escáner persiste si el jugador navega a otra pantalla. ✅ Si BLE está desactivado en el dispositivo, muestra diálogo de activación con deep link a ajustes. ✅ Icono animado indica escáner activo. | P0 | S | OB-04 |
| **BLE-02** | **Como** jugador, **quiero** que el juego detecte automáticamente las balizas cercanas **para** no tener que realizar acciones manuales de búsqueda. | ✅ Detección automática inicia cuando escáner está activo. ✅ Nueva baliza detectada genera notificación visual en radar en < 3 s tras estar en rango. ✅ Se muestran todas las balizas en rango simultáneamente (máx. 10 en pantalla). ✅ Detección funciona con app en primer plano. ✅ RSSI mínimo configurable (default -90 dBm). | P0 | M | BLE-01 |
| **BLE-03** | **Como** jugador, **quiero** que el escáner filtre solo balizas del juego **para** no ver ruido de otros dispositivos BLE cercanos. | ✅ Filtro por UUID de servicio específico del juego (configurado en backend). ✅ Dispositivos BLE genéricos (auriculares, relojes, etc.) no aparecen en el radar. ✅ UUID actualizable via remote config sin actualización de app. ✅ Log de balizas filtradas disponible en modo debug. | P0 | S | BLE-02 |
| **BLE-04** | **Como** jugador, **quiero** ver el nivel de señal RSSI de cada baliza **para** saber qué tan cerca estoy de ellas. | ✅ Valor RSSI mostrado en dBm junto al indicador de cada baliza. ✅ Barra visual de 5 niveles traduce RSSI a intensidad comprensible. ✅ Valor actualizado cada 500 ms cuando el escáner está activo. ✅ Historial de los últimos 10 valores RSSI almacenado por sesión para suavizado. | P0 | S | BLE-02 |
| **BLE-05** | **Como** jugador preocupado por la batería, **quiero** activar el modo de escaneo de bajo consumo **para** jugar durante más tiempo sin agotar la batería. | ✅ Modo bajo consumo reduce frecuencia de escaneo a 1 ciclo cada 5 s (vs 1 s en modo normal). ✅ Indicador visible en HUD cuando modo bajo consumo está activo. ✅ Puede activarse/desactivarse desde ajustes y desde el HUD del escáner. ✅ Latencia de detección en modo bajo consumo claramente comunicada al jugador (hasta 5 s adicionales). ✅ Se activa automáticamente cuando batería < 20%. | P1 | S | BLE-01 |
| **BLE-06** | **Como** jugador Android, **quiero** que el escaneo funcione en segundo plano **para** recibir alertas de balizas cercanas sin tener el teléfono en mano. | ✅ Servicio foreground activo cuando escaneo en segundo plano está habilitado. ✅ Notificación persistente en barra de estado con opción de detener. ✅ Detección en background genera notificación push local en < 10 s. ✅ Background scan se desactiva automáticamente si el jugador lleva 30 min sin interacción. ✅ Consumo de batería en background < 3% por hora (verificado en pruebas en Pixel 6). | P1 | L | BLE-01, OB-08 |
| **BLE-07** | **Como** jugador, **quiero** ver múltiples balizas en el radar al mismo tiempo **para** planificar qué capturar primero. | ✅ Radar muestra hasta 10 balizas simultáneas con íconos diferenciados por clase. ✅ Balizas más cercanas aparecen en anillo interior del radar. ✅ Tap en baliza en radar abre su ficha de detalle. ✅ Algoritmo de clustering evita superposición cuando hay múltiples balizas en el mismo rango. | P0 | M | BLE-02, RAD-01 |
| **BLE-08** | **Como** jugador, **quiero** ver un indicador de estabilidad de señal **para** saber si el RSSI que veo es confiable o hay interferencia. | ✅ Indicador de estabilidad calculado como desviación estándar de los últimos 5 valores RSSI. ✅ Tres estados: Estable (verde), Inestable (amarillo), Muy inestable (rojo). ✅ Tooltip explicando qué significa cada estado accesible con tap largo. ✅ Datos de estabilidad incluidos en el proof anti-cheat enviado al servidor. | P1 | S | BLE-04 |
| **BLE-09** | **Como** jugador, **quiero** calibrar el escáner según mi dispositivo **para** compensar diferencias de hardware entre teléfonos. | ✅ Proceso de calibración guiado: coloca el teléfono a 1 m de una baliza conocida y presiona calibrar. ✅ Factor de corrección calculado y almacenado localmente. ✅ Calibración mejora precisión de estimación de distancia en ±15% (validado en lab). ✅ Botón "restablecer calibración" disponible. ✅ Perfil de calibración vinculado a la cuenta (no al dispositivo) para portabilidad. | P2 | M | BLE-04 |
| **BLE-10** | **Como** jugador, **quiero** ver el historial de balizas detectadas en la sesión actual **para** recordar qué encontré durante mi cacería. | ✅ Lista ordenada por hora de detección, más reciente primero. ✅ Cada entrada muestra: nombre de baliza, clase, RSSI promedio, hora de detección. ✅ Historial persiste durante la sesión (hasta cerrar la app). ✅ Opción de exportar historial de sesión como texto plano. ✅ Tap en entrada navega a detalle de esa baliza. | P2 | S | BLE-02 |

---

## ÉPICA 3: RADAR E INTERFAZ

> **Objetivo:** Ofrecer una interfaz de radar intuitiva, accesible e inmersiva que comunique el estado de proximidad a balizas de forma clara en cualquier condición de uso.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **RAD-01** | **Como** jugador, **quiero** ver una animación de radar circular en pantalla **para** sentirme inmerso en la narrativa de exploración tecnológica. | ✅ Radar animado con barrido rotatorio a 1 RPM cuando escáner activo. ✅ Barrido se detiene elegantemente cuando escáner OFF. ✅ Radio de radar visual corresponde simbólicamente al rango BLE real. ✅ Animación a 60 fps en dispositivos de gama media (Snapdragon 665+). ✅ Asset vectorial SVG/Lottie para independencia de resolución. | P0 | M | BLE-01 |
| **RAD-02** | **Como** jugador, **quiero** ver una barra de intensidad de señal **para** tener una referencia numérica y visual de qué tan cerca estoy de una baliza. | ✅ Barra de 5 segmentos iluminados progresivamente según RSSI. ✅ Valor numérico en dBm junto a la barra. ✅ Animación de pulso cuando se alcanza nivel máximo (baliza encontrada). ✅ Barra animada (transición suave entre niveles, no salto brusco). | P0 | S | BLE-04, RAD-01 |
| **RAD-03** | **Como** jugador, **quiero** que el color del radar cambie según el estado de la baliza más cercana **para** recibir información de proximidad de un vistazo. | ✅ Sin señal: gris / Lejano: azul / Próximo: amarillo / Encontrado: verde. ✅ Transición de color animada (fade de 300 ms). ✅ Colores definidos en design tokens centralizados para fácil theming. ✅ Cada color validado con contraste WCAG AA sobre fondo oscuro. | P0 | S | RAD-01 |
| **RAD-04** | **Como** jugador, **quiero** recibir retroalimentación háptica al cambiar de estado de proximidad **para** poder cazar balizas sin mirar la pantalla. | ✅ Patrón de vibración diferente para cada transición de estado (4 patrones únicos). ✅ Intensidad y duración de vibración configurables en ajustes. ✅ Opción de desactivar háptica completamente. ✅ Compatible con API de vibración de Android (VibrationEffect) e iOS (UIFeedbackGenerator). ✅ No vibra más de 1 vez por segundo para evitar spam. | P0 | S | RAD-03 |
| **RAD-05** | **Como** jugador, **quiero** escuchar efectos de audio al cambiar de estado **para** tener retroalimentación inmersiva y poder jugar con los auriculares puestos. | ✅ Sonido único para cada uno de los 4 estados. ✅ Efecto "ping" de sonar al detectar nueva baliza. ✅ Volumen de efectos de juego independiente del volumen del sistema y la música. ✅ Archivos de audio en < 100 KB cada uno (compresión OGG). ✅ Sin latencia de audio perceptible (< 50 ms desde evento a sonido). | P1 | S | RAD-03 |
| **RAD-06** | **Como** jugador con discapacidad visual, **quiero** usar el juego en modo solo-visual con patrones y formas en lugar de depender solo del color **para** no ser excluido por daltonismo o deficiencias de percepción de color. | ✅ Modo activable desde ajustes de accesibilidad. ✅ Cada estado tiene un patrón geométrico adicional (sin señal: círculos vacíos, lejano: cruces, próximo: triángulos, encontrado: estrella). ✅ Animaciones diferenciadas por velocidad además de color. ✅ Cumple WCAG 2.1 nivel AA sin depender exclusivamente del color. | P1 | M | RAD-03 |
| **RAD-07** | **Como** jugador, **quiero** cambiar entre modo oscuro y modo claro **para** usar el juego cómodamente en cualquier condición de iluminación. | ✅ Modo oscuro y claro disponibles en ajustes y desde el toggle rápido en el HUD. ✅ Sigue el sistema del OS por defecto si no hay preferencia guardada. ✅ Transición entre modos en < 200 ms. ✅ Todos los textos mantienen contraste WCAG AA en ambos modos. ✅ Preferencia sincronizada en la cuenta del jugador. | P1 | S | — |
| **RAD-08** | **Como** jugador con daltonismo, **quiero** activar un modo especial de colores **para** distinguir los estados del radar sin depender de la paleta estándar rojo/verde. | ✅ Paleta alternativa: sin señal (blanco), lejano (azul), próximo (naranja), encontrado (morado). ✅ Paleta validada con simuladores de deuteranopia, protanopia y tritanopia. ✅ Activable desde ajustes de accesibilidad, independiente de RAD-06. ✅ Los íconos de clase de baliza también tienen versión alta-visibilidad. | P1 | S | RAD-03 |

---

## ÉPICA 4: MISIONES

> **Objetivo:** Proveer al jugador con objetivos estructurados de corto y largo plazo que dirijan su exploración física y den sentido narrativo a sus cacerías.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **MIS-01** | **Como** jugador, **quiero** ver la lista de misiones disponibles **para** elegir en qué enfocar mi sesión de juego. | ✅ Pantalla de misiones muestra misiones activas, disponibles y completadas en tabs separados. ✅ Cada misión muestra: nombre, descripción breve, recompensa estimada, dificultad y tiempo restante. ✅ Lista cargada en < 2 s desde caché local; actualización en background. ✅ Badge en icono de misiones indica cuántas están listas para reclamar. | P0 | S | OB-03 |
| **MIS-02** | **Como** jugador, **quiero** aceptar o rechazar misiones individualmente **para** controlar qué compromisos adquiero. | ✅ Botones "Aceptar" y "Rechazar" visibles en detalle de cada misión. ✅ Rechazar una misión la oculta por 24 h (puede reclamarse desde historial). ✅ Máximo 5 misiones activas simultáneamente (límite configurable por backend). ✅ Confirmación al aceptar misión con resumen de objetivos. | P0 | S | MIS-01 |
| **MIS-03** | **Como** jugador, **quiero** leer el briefing narrativo de cada misión **para** entender el contexto lore de lo que estoy haciendo. | ✅ Briefing en formato de "transmisión interceptada" con tipografía monoespaciada y estilo terminal. ✅ Texto de 100-200 palabras con audio de voz opcional (idioma del dispositivo). ✅ Opción de saltar el briefing con botón visible. ✅ Briefing accesible nuevamente desde el detalle de misión activa. | P1 | S | MIS-01 |
| **MIS-04** | **Como** jugador, **quiero** ver un mapa aproximado de la zona de misión **para** planificar físicamente a dónde dirigirme. | ✅ Mapa muestra zona de búsqueda como círculo o polígono aproximado (no ubicación exacta de baliza). ✅ Radio de zona mínimo 200 m para no revelar posición precisa del hardware. ✅ Mapa usa tiles de OpenStreetMap (sin costo de licencia). ✅ Funciona offline para zonas previamente visitadas (caché de tiles). ✅ Botón "Cómo llegar" abre la app de mapas nativa del dispositivo. | P0 | M | MIS-01 |
| **MIS-05** | **Como** jugador, **quiero** ver el progreso de mi misión en tiempo real **para** saber cuánto me falta para completarla. | ✅ Barra de progreso o contador en detalle de misión actualizado en cada captura relevante. ✅ Notificación local cuando misión llega a 50% y 100% de progreso. ✅ Progreso visible también en la pantalla principal (widget de misión activa). ✅ Si el jugador pierde progreso (ej. baliza recapturada por rival), el contador se actualiza inmediatamente. | P0 | S | MIS-01, BLE-02 |
| **MIS-06** | **Como** jugador, **quiero** que las misiones completadas se marquen y otorguen recompensas automáticamente **para** no tener que hacer pasos adicionales para cobrar. | ✅ Al completar el último objetivo, pantalla de recompensa aparece en < 3 s. ✅ Recursos y XP acreditados instantáneamente y visibles en perfil. ✅ Misión pasa a tab "Completadas" con timestamp. ✅ Si el jugador está offline al completar, la recompensa se acredita al reconectar. | P0 | S | MIS-05 |
| **MIS-07** | **Como** jugador habitual, **quiero** recibir misiones diarias nuevas cada día **para** tener siempre un motivo para abrir el juego. | ✅ Set de 3 misiones diarias generadas a las 00:00 UTC y disponibles para todos los jugadores. ✅ Timer de cuenta regresiva hasta la próxima renovación visible en pantalla de misiones. ✅ Al menos 1 misión diaria es realizable sin moverse de un radio de 500 m. ✅ Completar las 3 misiones diarias otorga bonus acumulativo "Racha de día". | P0 | M | MIS-01 |
| **MIS-08** | **Como** miembro de un clan, **quiero** participar en misiones semanales de clan **para** contribuir con mi equipo y ganar recompensas colectivas. | ✅ Misiones de clan visibles en pantalla de clan y en tab de misiones del jugador. ✅ Progreso compartido: las capturas de cualquier miembro del clan cuentan. ✅ Contribución individual de cada miembro visible en el detalle de la misión. ✅ Recompensa de clan distribuida automáticamente al completarla o al final de la semana. ✅ Requiere ser miembro de clan activo. | P1 | L | MIS-01, CLAN-02 |

---

## ÉPICA 5: BALIZAS

> **Objetivo:** Hacer que cada baliza sea un objeto de juego con personalidad, historia y mecánicas de interacción distintivas según su clase.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **BAL-01** | **Como** jugador, **quiero** ver la clase de una baliza antes de intentar capturarla **para** saber qué tipo de desafío me espera. | ✅ Ícono de clase (C, B, A, A+, S) visible en radar y en ficha de detalle. ✅ Color de ícono diferenciado por clase. ✅ Tooltip con descripción de la clase accesible con tap largo en el ícono. ✅ Clase verificada con el servidor antes de mostrar (evita spoofing local). | P0 | S | BLE-07 |
| **BAL-02** | **Como** jugador, **quiero** ver el estado actual de una baliza (activa, en cooldown, capturada) **para** planificar si vale la pena ir hacia ella en este momento. | ✅ Estado mostrado con ícono y texto en ficha de baliza. ✅ Si en cooldown: timer de cuenta regresiva visible hasta reactivación. ✅ Si capturada por otro jugador/clan: nombre del captador y hora de captura visibles. ✅ Estado actualizado en tiempo real via WebSocket cuando se está en pantalla de detalle. | P0 | S | BAL-01 |
| **BAL-03** | **Como** jugador interesado en el lore, **quiero** ver una vista previa del fragmento de historia de una baliza antes de capturarla **para** motivarme a ir a buscarla. | ✅ Vista previa del fragmento de lore en ficha de baliza (primeras 30 palabras + "..."). ✅ Texto completo desbloqueado solo al capturar la baliza. ✅ Indicador visual si el jugador ya desbloqueó ese fragmento previamente. ✅ El lore preview es generado por el backend y no contiene spoilers narrativos mayores. | P2 | S | BAL-01 |
| **BAL-04** | **Como** jugador competitivo, **quiero** ver cuántas veces ha sido capturada una baliza y por quién **para** conocer su historial y la competencia alrededor de ella. | ✅ Contador de capturas totales visible en ficha de baliza. ✅ Top 3 captadores más frecuentes mostrados con avatar y número de capturas. ✅ Historial de últimas 10 capturas (jugador, fecha, hora) en tab separado. ✅ Información actualizada en < 5 s al abrir la ficha. | P2 | S | BAL-01 |
| **BAL-05** | **Como** jugador competitivo, **quiero** ver el récord de tiempo de captura de una baliza **para** intentar superarlo. | ✅ Tiempo récord mostrado en ficha de baliza (desde detección hasta fin de minijuego). ✅ Holder del récord mostrado con avatar y fecha. ✅ Si el jugador supera el récord, animación especial y notificación push al anterior holder. ✅ Récord verificado server-side (no manipulable desde cliente). | P2 | S | BAL-04 |
| **BAL-06** | **Como** jugador, **quiero** interactuar con balizas Clase C (más comunes) mediante un minijuego de dificultad baja **para** acumular recursos frecuentemente. | ✅ Balizas C desencadenan minijuego de Nivel 1-3 al ser encontradas. ✅ Cooldown de baliza C: 4 horas tras captura. ✅ Recompensa base: 50-100 XP + 1 recurso básico. ✅ Captura tarda < 90 s en completarse (minijuego + animaciones). | P0 | M | MJ-01, BAL-01 |
| **BAL-07** | **Como** jugador avanzado, **quiero** interactuar con balizas Clase A+ y S mediante mecánicas especiales **para** sentir que el esfuerzo de llegar a ellas tiene recompensa proporcional. | ✅ Balizas A+ requieren al menos 2 jugadores del mismo clan presentes simultáneamente (dentro de 30 m). ✅ Balizas S activadas solo durante eventos programados (ventana de 2 h). ✅ Minijuego de balizas S: Nivel 8-10, sin reintentos ilimitados. ✅ Recompensa S: > 5,000 XP + fragmento de lore épico + cosmético exclusivo. ✅ Notificación a todos los miembros del clan cuando se detecta baliza S activa. | P1 | XL | BAL-06, CLAN-01 |
| **BAL-08** | **Como** operador de hardware (admin), **quiero** registrar nuevas balizas físicas en el sistema **para** expandir la red de puntos de juego. | ✅ Panel admin con formulario: UUID, nombre, clase, coordenadas (lat/lon), descripción, lore. ✅ Validación de UUID único en la red. ✅ Baliza nueva aparece en el juego en < 5 min tras registro. ✅ Operador puede asignar baliza a zona territorial. ✅ Log de auditoría de creación/modificación de balizas. ✅ Acceso solo para rol "beacon_operator" o superior. | P0 | M | — |

---

## ÉPICA 6: MINIJUEGOS

> **Objetivo:** Ofrecer mecánicas de habilidad justas, deterministas y escalables que traduzcan el "encuentro" con una baliza en un desafío satisfactorio.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **MJ-01** | **Como** jugador que encuentra una baliza, **quiero** que el minijuego se lance automáticamente **para** no tener que navegar por menús antes de comenzar. | ✅ Transición de pantalla de escáner a minijuego en < 1.5 s tras confirmar "Iniciar captura". ✅ Animación de transición temática (interferencia de señal, decodificación). ✅ El minijuego recibe semilla del servidor antes de renderizar el primer frame. ✅ Si falla la obtención de semilla (sin internet), muestra error con opción de reintentar. | P0 | M | BLE-02 |
| **MJ-02** | **Como** jugador, **quiero** ver un temporizador durante el minijuego **para** saber cuánto tiempo tengo para resolver el desafío. | ✅ Timer visible en esquina superior, cuenta regresiva. ✅ Timer cambia de color: verde > 60%, amarillo 30-60%, rojo < 30%. ✅ Animación de "tiempo agotado" al llegar a 0 (no corte brusco). ✅ Tiempo límite por nivel configurable desde backend. ✅ Timer se pausa si app va a background (previene trampa). | P0 | S | MJ-01 |
| **MJ-03** | **Como** jugador, **quiero** tener un sistema de vidas durante el minijuego **para** tener oportunidades de corregir errores sin perder todo el progreso. | ✅ 3 vidas por defecto en primer intento de cada baliza. ✅ Ícono de corazón por vida, animación al perder una. ✅ Al perder la última vida: pantalla de fallo con opciones (reintentar/abandonar). ✅ Reintentar en misma sesión comienza con 2 vidas. ✅ Nunca se ofrecen vidas de pago durante el flujo (evita P2W). | P0 | S | MJ-01 |
| **MJ-04** | **Como** jugador habilidoso, **quiero** que el juego detecte si completé el minijuego sin errores **para** recibir el bonus "Perfect Clear". | ✅ Perfect Clear: completar minijuego sin perder ninguna vida y sin ningún error (según tipo de minijuego). ✅ Animación y sonido especial al lograr Perfect Clear. ✅ Bonus de XP: +50% sobre recompensa base. ✅ Contador de Perfect Clears en perfil del jugador. ✅ No recibe Perfect Clear si usó power-up durante el minijuego. | P1 | S | MJ-03 |
| **MJ-05** | **Como** jugador, **quiero** ver una pantalla de fallo clara con opciones cuando no logro completar el minijuego **para** entender qué pasó y decidir qué hacer. | ✅ Pantalla de fallo muestra: causa del fallo, progreso alcanzado, opciones disponibles. ✅ Opciones: "Reintentar" (si quedan intentos), "Abandonar misión", "Ver pista" (si está desbloqueado). ✅ Sin opción de comprar reintento con gemas en pantalla de fallo (evita P2W en mecánica core). ✅ Abandonar devuelve al jugador al radar sin penalizar XP previo. | P0 | S | MJ-03 |
| **MJ-06** | **Como** desarrollador, **quiero** que los minijuegos se generen con una semilla determinista provista por el servidor **para** poder reproducir exactamente cualquier partida en auditorías anti-cheat. | ✅ Semilla entregada por el servidor, firmada digitalmente, con TTL de 60 s. ✅ El cliente envía la semilla de vuelta al completar junto con los eventos de la partida (secuencia de inputs). ✅ El servidor puede reproducir y verificar la partida en < 200 ms. ✅ Semillas caducadas o ya usadas rechazadas (previene replay attacks). ✅ Sistema documentado para auditoría externa. | P0 | L | MJ-01 |
| **MJ-07** | **Como** jugador experimentado, **quiero** que el minijuego se vuelva más difícil a medida que aumento de nivel **para** seguir encontrando el juego desafiante. | ✅ Dificultad del minijuego función del nivel del jugador + clase de baliza. ✅ 10 niveles de dificultad, cada uno con parámetros ajustados (velocidad, complejidad, tiempo). ✅ Curva de dificultad testeada: completion rate objetivo 85% nivel 1, 55% nivel 10. ✅ Parámetros de dificultad ajustables desde backend sin actualización de app. | P1 | M | MJ-01 |
| **MJ-08** | **Como** jugador, **quiero** ver una animación satisfactoria al completar un minijuego **para** sentir la gratificación del logro antes de ver la pantalla de recompensa. | ✅ Animación de "captura exitosa" de 2-3 s (no saltable en primera completación, saltable en posteriores). ✅ Efectos de partículas y sonido de fanfare. ✅ Transición animada a pantalla de recompensa. ✅ Animación diferenciada para Perfect Clear (más elaborada). ✅ Tiempo total animación + recompensa < 10 s para no frustrar el ritmo. | P1 | S | MJ-04 |

---

## ÉPICA 7: RECOMPENSAS Y ECONOMÍA

> **Objetivo:** Diseñar un sistema económico justo, transparente y sostenible que motive la progresión sin crear mecánicas pay-to-win.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **ECO-01** | **Como** jugador, **quiero** ver mi XP actual y cuánto me falta para el siguiente nivel **para** tener un objetivo claro de progresión. | ✅ Barra de XP en perfil y en HUD principal. ✅ Tooltip con número exacto (XP actual / XP requerido). ✅ Animación de ganancia de XP (barra se llena) visible tras cada captura. ✅ Historial de las últimas 10 fuentes de XP accesible en perfil. | P0 | S | OB-07 |
| **ECO-02** | **Como** jugador, **quiero** ver una pantalla de recompensa detallada después de cada captura **para** saber exactamente qué gané. | ✅ Pantalla muestra: XP ganado, recursos obtenidos, bonuses aplicados (Perfect Clear, primera captura del día, etc.). ✅ Cada ítem de recompensa tiene animación de aparición individual. ✅ Botón "Reclamar todo" o auto-reclamado tras 3 s de inactividad. ✅ Total acumulado del día visible para contexto. | P0 | S | MJ-08 |
| **ECO-03** | **Como** jugador coleccionista, **quiero** desbloquear fragmentos de lore al capturar balizas **para** construir la historia del mundo del juego. | ✅ Fragmento de lore nuevo añadido automáticamente a la Biblioteca del jugador. ✅ Notificación especial si el fragmento completa una "cadena" narrativa. ✅ Fragmentos visualizados en interfaz tipo diario/archivo con diseño temático. ✅ Compartir fragmento en RRSS desde la pantalla de unlock. | P1 | M | ECO-02 |
| **ECO-04** | **Como** jugador, **quiero** que el juego respete mi tope de ganancias diario **para** entender que la economía es justa y no veo ventaja en hacer grinding extremo. | ✅ Cap diario de recursos configurable por backend (default: 500 recursos básicos/día). ✅ Indicador de "cap diario alcanzado" claro en pantalla de recompensa. ✅ El cap se reinicia a las 00:00 UTC. ✅ El jugador puede seguir capturando balizas para XP y lore aunque haya alcanzado el cap. ✅ Cap diferente para jugadores premium (season pass). | P0 | S | ECO-02 |
| **ECO-05** | **Como** jugador, **quiero** ver mi saldo de todos los recursos y monedas en un solo lugar **para** planificar mis gastos. | ✅ Pantalla de cartera muestra todos los recursos con íconos, nombre y cantidad. ✅ Historial de transacciones de los últimos 30 días (ganado/gastado, fuente/destino). ✅ Recursos agrupados por categoría (básicos, raros, premium). ✅ Sincronizado con servidor; no editable desde cliente. | P0 | S | ECO-02 |
| **ECO-06** | **Como** jugador, **quiero** enviar recursos como regalo a amigos **para** fortalecer los lazos sociales dentro del juego. | ✅ Envío de recursos: máximo 3 por día por relación de amistad. ✅ Tipo y cantidad de recurso seleccionables (dentro de límites configurados). ✅ Mensaje opcional de hasta 100 caracteres. ✅ Receptor recibe notificación push con nombre del remitente. ✅ Registro de regalos enviados/recibidos en historial. ✅ Límite anti-abuse: no se pueden enviar recursos que se recibieron hace < 48 h. | P2 | M | ECO-05 |
| **ECO-07** | **Como** jugador, **quiero** mejorar mi escáner comprando upgrades con recursos del juego **para** sentir progresión tangible en mis capacidades de búsqueda. | ✅ Árbol de upgrades del escáner con 3 ramas (rango, velocidad, precisión). ✅ Cada upgrade muestra beneficio concreto ("Aumenta rango de detección 10 m"). ✅ Compra confirmada con recursos deducidos instantáneamente. ✅ Beneficios aplicados en la siguiente sesión de escaneo. ✅ No se pueden comprar upgrades con dinero real (solo recursos in-game). | P1 | L | ECO-05, PRG-02 |
| **ECO-08** | **Como** jugador, **quiero** comprar cosméticos premium con gemas **para** personalizar mi apariencia sin que eso afecte el gameplay. | ✅ Tienda cosmética con categorías: avatares, marcos, efectos de radar, efectos de captura. ✅ Precio en gemas visible en cada ítem. ✅ Vista previa antes de comprar. ✅ Gemas deducidas y cosmético desbloqueado instantáneamente. ✅ Sin cosméticos con beneficios de gameplay (validación server-side). ✅ Política de no-reembolso clara antes de confirmar compra. | P1 | M | MON-01 |

---

## ÉPICA 8: PROGRESIÓN

> **Objetivo:** Ofrecer múltiples ejes de crecimiento a largo plazo que den al jugador sensación de poder, reconocimiento y reinicio voluntario.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **PRG-01** | **Como** jugador, **quiero** recibir una notificación de level up con detalle de lo desbloqueado **para** sentir el progreso como un hito significativo. | ✅ Notificación in-app modal con animación de nivel. ✅ Lista de desbloqueos: habilidades, cosméticos, misiones, títulos. ✅ Botón "Ver árbol de habilidades" lleva directamente al árbol. ✅ Notificación visible aunque el jugador no esté en la pantalla principal. | P0 | S | ECO-01 |
| **PRG-02** | **Como** jugador, **quiero** explorar visualmente el árbol de habilidades **para** planificar mi camino de desarrollo. | ✅ Árbol visualizado como grafo con nodos conectados. ✅ Nodos desbloqueados (verdes), desbloqueables (amarillos), bloqueados (grises). ✅ Zoom y paneo con gestos. ✅ Tap en nodo muestra descripción, costo, prerrequisitos. ✅ Nodos organizados en 3 ramas: Exploración, Combate Cibernético, Liderazgo. | P1 | L | PRG-01 |
| **PRG-03** | **Como** jugador, **quiero** comprar habilidades del árbol usando puntos de habilidad **para** personalizar mi estilo de juego. | ✅ Puntos de habilidad ganados: 1 por level up. ✅ Compra de habilidad requiere prerrequisitos cumplidos y puntos suficientes. ✅ Confirmación antes de gastar puntos (acción no revertible). ✅ Habilidad activa inmediatamente tras compra. ✅ Respec del árbol disponible 1 vez cada 30 días (sin costo). | P1 | M | PRG-02 |
| **PRG-04** | **Como** jugador veterano (nivel máximo), **quiero** hacer prestige para reiniciar mi progresión a cambio de beneficios exclusivos **para** tener un desafío renovado. | ✅ Prestige disponible desde nivel 50. ✅ Al hacer prestige: nivel reinicia a 1, árbol de habilidades se resetea, se obtiene emblema de prestige y multiplicador de XP permanente (+10% por prestige). ✅ Confirmación con advertencia clara de qué se pierde. ✅ Cosméticos y lore coleccionados NO se resetean. ✅ Máximo 5 prestiges (Prestige V = multiplicador ×1.5 XP). | P2 | L | PRG-02 |
| **PRG-05** | **Como** jugador, **quiero** desbloquear logros al alcanzar hitos **para** tener objetivos secundarios y reconocimiento de mis hazañas. | ✅ Sistema de 50+ logros en categorías: exploración, social, combate, lore, consistencia. ✅ Notificación in-app al desbloquear logro (no intrusiva, tipo toast). ✅ Logros visibles en perfil público. ✅ Logros ocultos (desbloqueados sin conocer previamente el criterio) señalados como "???". ✅ Recompensas de logro: XP, cosméticos, títulos (según rareza). | P1 | L | — |
| **PRG-06** | **Como** jugador, **quiero** equipar títulos y distintivos en mi perfil **para** mostrar mis logros a otros jugadores. | ✅ Hasta 1 título activo y 3 badges simultáneos en perfil. ✅ Pantalla de equipamiento tipo drag-and-drop o selector. ✅ Vista previa del perfil antes de confirmar. ✅ Títulos y badges visibles en: perfil público, clasificaciones, chat de clan, historial de capturas. | P2 | S | PRG-05 |

---

## ÉPICA 9: CLANES

> **Objetivo:** Construir la capa social competitiva del juego con herramientas de organización, comunicación y cooperación para grupos de jugadores.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **CLAN-01** | **Como** jugador, **quiero** crear un clan propio **para** reunir a mis amigos bajo una identidad común. | ✅ Formulario: nombre (3-24 chars, único), tag (2-4 chars, mayúsculas), descripción, emblema (selección de galería). ✅ Costo de creación: 500 recursos básicos (evita creación masiva de clanes vacíos). ✅ Creador automáticamente se convierte en Líder. ✅ Validación de nombre (sin palabras prohibidas, sin duplicados). ✅ Clan visible en búsqueda en < 5 min tras creación. | P0 | M | ECO-05 |
| **CLAN-02** | **Como** jugador, **quiero** unirme a un clan existente **para** participar en el juego social y territorial. | ✅ Búsqueda de clanes por nombre o tag. ✅ Perfil de clan muestra: miembros activos, ranking territorial, descripción, modalidad (abierto/por invitación). ✅ Clan abierto: unirse instantáneamente. Clan cerrado: enviar solicitud. ✅ Límite de miembros configurable por backend (default: 50). ✅ No se puede estar en más de 1 clan simultáneamente. | P0 | S | CLAN-01 |
| **CLAN-03** | **Como** líder de clan, **quiero** ver el roster completo con actividad de cada miembro **para** gestionar mi equipo eficientemente. | ✅ Lista de miembros con: avatar, nombre, rango, fecha de ingreso, última actividad, contribución semanal. ✅ Ordenable por cualquier columna. ✅ Acciones disponibles por miembro: promover, degradar, expulsar. ✅ Filtro para ver solo miembros inactivos (> 7 días sin login). | P0 | M | CLAN-02 |
| **CLAN-04** | **Como** miembro de clan, **quiero** un chat de clan en tiempo real **para** coordinar capturas y estrategias con mis compañeros. | ✅ Chat en tiempo real via WebSocket. ✅ Mensajes de texto + emojis. ✅ Mensajes de sistema automáticos (captura de baliza, nueva misión de clan, inicio de guerra). ✅ Historial de los últimos 500 mensajes. ✅ Notificación push configurable para mensajes de clan. ✅ Moderación: líder/oficiales pueden borrar mensajes. | P1 | L | CLAN-02 |
| **CLAN-05** | **Como** miembro de clan, **quiero** donar recursos al almacén del clan **para** contribuir a las operaciones colectivas. | ✅ Donación de cualquier tipo de recurso básico o raro. ✅ Límite diario de donación individual (previene abuso). ✅ Donación registrada en log del clan con nombre y cantidad. ✅ Donantes top de la semana destacados en pantalla del clan. ✅ Recursos donados usados automáticamente para misiones de clan que lo requieran. | P1 | S | CLAN-02, ECO-05 |
| **CLAN-06** | **Como** miembro de clan, **quiero** participar en misiones de clan capturando balizas objetivos **para** contribuir al progreso colectivo. | ✅ Balizas objetivo de misión de clan marcadas con ícono especial en radar. ✅ Captura de baliza objetivo actualiza el progreso del clan en tiempo real (visible por todos los miembros). ✅ Contribución individual contabilizada para ranking de contribución. ✅ Al completar la misión, notificación a todos los miembros del clan. | P1 | M | MIS-08, CLAN-02 |
| **CLAN-07** | **Como** líder de clan, **quiero** declarar guerra a otro clan **para** iniciar una competencia por el control de territorio. | ✅ Declaración de guerra requiere: rol de Líder, clan con al menos 5 miembros activos, no estar ya en guerra. ✅ Clan objetivo recibe notificación de declaración de guerra. ✅ Período de preparación de 24 h antes de que inicie la guerra. ✅ Duración de guerra: 7 días. ✅ Ganador determinado por puntos de territorio capturado. ✅ Cooldown de 72 h entre guerras para el mismo par de clanes. | P1 | XL | CLAN-02, TER-01 |
| **CLAN-08** | **Como** jugador, **quiero** poder abandonar mi clan en cualquier momento **para** tener libertad de cambiar de equipo si mis circunstancias cambian. | ✅ Opción "Abandonar clan" accesible desde perfil de clan. ✅ Confirmación con advertencia de pérdida de progreso de misiones de clan activas. ✅ Si el jugador es el líder único, debe transferir liderazgo o el clan se disuelve. ✅ Cooldown de 48 h antes de poder unirse a otro clan. ✅ El jugador abandona sin perder recursos ni progresión individual. | P0 | S | CLAN-02 |

---

## ÉPICA 10: TERRITORIOS

> **Objetivo:** Crear una capa de meta-juego territorial que dé significado a largo plazo a las capturas individuales y motive la cooperación y el conflicto entre clanes.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **TER-01** | **Como** jugador, **quiero** ver el mapa de territorios de mi ciudad **para** entender el estado geopolítico del juego en mi zona. | ✅ Mapa territorial sobre OSM con sectores coloreados por clan controlador. ✅ Leyenda de colores con nombre de clan y número de balizas controladas. ✅ Zoom desde vista global hasta nivel de barrio. ✅ Mapa actualizado cada 5 min (no tiempo real para ahorrar costos). ✅ Mi zona siempre visible inicialmente (centrado en última ubicación conocida). | P1 | L | CLAN-01 |
| **TER-02** | **Como** jugador, **quiero** ver qué clan controla cada sector del mapa **para** saber dónde atacar o dónde defender. | ✅ Tap en sector muestra: clan controlador, % de balizas controladas, recursos generados por hora. ✅ Indicador de "en disputa" si otro clan capturó >40% de balizas del sector en las últimas 6 h. ✅ Estadísticas del sector actualizadas al abrir el detalle. | P1 | S | TER-01 |
| **TER-03** | **Como** miembro de un clan con territorio, **quiero** cosechar los recursos que genera nuestro territorio **para** usar esos recursos en mejoras del clan. | ✅ Botón "Cosechar" en pantalla de territorio del clan, disponible cada 8 h. ✅ Cantidad cosechada proporcional a número y clase de balizas bajo control. ✅ La cosecha no ocurre automáticamente (requiere acción del jugador para engagement). ✅ Notificación push cuando la cosecha está lista. ✅ Recursos van al almacén del clan. | P1 | M | TER-01, CLAN-05 |
| **TER-04** | **Como** miembro de un clan con territorio extenso, **quiero** ver el bonus de producción de recursos **para** saber cuánto vale la pena defender ese territorio. | ✅ Panel de territorio del clan muestra: zonas controladas, bonus total de XP, bonus de recursos raros, production rate. ✅ Comparativa con el turno anterior (subió/bajó la producción). ✅ Cálculo de bonus transparente (fórmula visible en tooltip). | P2 | S | TER-03 |
| **TER-05** | **Como** miembro de clan, **quiero** recibir alertas cuando un territorio del clan está siendo atacado **para** responder a tiempo y defenderlo. | ✅ Alerta push: "¡[ClanEnemigo] está atacando el Sector Norte!" al capturar primera baliza del sector. ✅ Alerta in-app con mapa del sector afectado. ✅ Umbral configurable por el líder: alerta al 1 baliza capturada / al 25% del sector / al 50%. ✅ Silenciamiento de alertas por período configurable (para no molestar de noche). | P1 | M | TER-01, CLAN-04 |
| **TER-06** | **Como** jugador, **quiero** ver el ranking de clanes por territorio controlado **para** conocer el panorama competitivo global y local. | ✅ Ranking con top 20 clanes por región y global. ✅ Posición del clan del jugador siempre visible (aunque no esté en top 20). ✅ Datos: posición, nombre de clan, sectores, balizas, puntuación territorial. ✅ Ranking actualizado cada hora. ✅ Historial de posiciones de los últimos 30 días (gráfico de línea). | P2 | M | TER-01 |

---

## ÉPICA 11: EVENTOS

> **Objetivo:** Generar picos de engagement y FOMO saludable mediante eventos temporales que creen experiencias compartidas en la comunidad.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **EVT-01** | **Como** jugador, **quiero** recibir una notificación push cuando empiece un nuevo evento **para** no perderme actividades de tiempo limitado. | ✅ Notificación enviada 24 h antes del inicio y al momento del inicio. ✅ Contenido de la notificación: nombre del evento, duración, recompensa destacada. ✅ Deep link directo a la pantalla del evento. ✅ Notificaciones de evento configurables independientemente (no afecta otras prefs). | P1 | S | OB-08 |
| **EVT-02** | **Como** jugador, **quiero** participar en eventos especiales con mecánicas únicas **para** tener experiencias que rompan la rutina del juego base. | ✅ Evento tiene pantalla propia con reglas, objetivos y leaderboard. ✅ Mecánica diferenciada del juego base (ej. capturar 10 balizas específicas en 2 h, modo cooperativo de toda la facción, etc.). ✅ Progreso del evento guardado si el jugador sale y vuelve. ✅ Evento activo claramente señalizado en la pantalla principal (banner o widget). | P1 | L | MIS-01 |
| **EVT-03** | **Como** jugador competitivo, **quiero** ver el leaderboard del evento en tiempo real **para** saber mi posición y motivarme a escalar. | ✅ Leaderboard actualizado cada 60 s. ✅ Mi posición siempre visible (sticky row si no estoy en top). ✅ Top 3 con diseño destacado (medallas). ✅ Filtros: Global, Por facción, Por región. | P1 | M | EVT-02 |
| **EVT-04** | **Como** jugador, **quiero** reclamar mis recompensas del evento una vez finalizado **para** obtener los ítems exclusivos que gané. | ✅ Pantalla de recompensas de evento disponible hasta 48 h después del fin del evento. ✅ Recompensas escalonadas: por participación, por objetivos, por posición en leaderboard. ✅ Cosméticos exclusivos de evento no disponibles en tienda ordinaria. ✅ Recompensas no reclamadas en 48 h se acreditan automáticamente. | P1 | S | EVT-02 |
| **EVT-05** | **Como** jugador, **quiero** ver el historial de eventos pasados **para** saber cuáles me perdí y cuándo podrían volver. | ✅ Historial de los últimos 12 eventos con: nombre, fecha, tipo, mi participación, mi posición. ✅ Indicador "Puede volver" para eventos recurrentes. ✅ Cosméticos ganados en cada evento visibles en el historial. | P2 | S | EVT-02 |

---

## ÉPICA 12: ANTI-CHEAT

> **Objetivo:** Mantener la integridad competitiva del juego garantizando que las capturas sean físicamente reales y detectando automáticamente comportamientos fraudulentos.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **AC-01** | **Como** sistema anti-cheat, **quiero** que cada captura incluya prueba criptográfica de presencia física **para** que sea imposible hacer spoofing desde casa. | ✅ Cliente envía al servidor: nonce firmado por la baliza (ESP32), RSSI promedio (últimas 5 muestras), timestamp del cliente, firma HMAC del paquete. ✅ Servidor valida: nonce no usado previamente, nonce generado en los últimos 120 s, RSSI en rango plausible (> -95 dBm). ✅ Capturas sin proof válido rechazadas con código de error específico. ✅ Tasa de rechazo falso positivo < 0.1% (validado en pruebas de campo). | P0 | XL | BLE-04 |
| **AC-02** | **Como** sistema anti-cheat, **quiero** que cada nonce de baliza sea de un solo uso **para** prevenir ataques de repetición. | ✅ Servidor mantiene set de nonces usados con TTL de 5 min. ✅ Reuso de nonce devuelve error 409 y registra evento de seguridad. ✅ Balizas ESP32 generan nuevo nonce cada 30 s (rotación de anuncio BLE). ✅ Sincronización de reloj entre baliza y servidor con tolerancia de ±60 s. | P0 | L | AC-01 |
| **AC-03** | **Como** moderador, **quiero** que el sistema marque automáticamente actividad sospechosa **para** poder investigarla sin necesidad de revisión manual exhaustiva. | ✅ Flags automáticos para: velocidad imposible (>50 km/h entre capturas), capturas múltiples simultáneas desde misma IP, RSSI pattern anómalo (siempre exactamente el mismo valor), tasa de perfect clears > 90% en nivel 8+. ✅ Flags visibles en dashboard de moderación. ✅ Umbral de flags antes de suspensión automática: configurable (default: 5 flags en 24 h). | P0 | L | AC-01 |
| **AC-04** | **Como** administrador, **quiero** ver el trust score de cada jugador **para** priorizar investigaciones y detectar patrones de abuso. | ✅ Trust score 0-100, calculado por ML (inicialmente por heurísticas simples). ✅ Factores: antigüedad de cuenta, capturas verificadas, flags, reportes de otros jugadores. ✅ Score visible en panel admin en ficha del jugador. ✅ Score actualizado cada 24 h. ✅ Historial de cambios de score con causa. | P1 | L | AC-03 |
| **AC-05** | **Como** jugador sancionado, **quiero** recibir una notificación clara explicando la sanción aplicada **para** entender qué ocurrió y conocer mis opciones. | ✅ Notificación in-app y push al aplicar sanción. ✅ Contenido: tipo de sanción, duración, motivo (genérico para no revelar métodos de detección), enlace a política de Fair Play, enlace a proceso de apelación. ✅ En caso de ban: mensaje al intentar iniciar sesión con código de referencia. ✅ Tono respetuoso, sin lenguaje acusatorio. | P0 | S | — |

---

## ÉPICA 13: MONETIZACIÓN

> **Objetivo:** Generar ingresos sostenibles mediante un modelo cosmético-first que nunca compromise la experiencia de los jugadores F2P.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **MON-01** | **Como** jugador, **quiero** explorar la tienda de cosméticos **para** ver qué hay disponible para personalizar mi perfil. | ✅ Tienda con categorías claras: Avatares, Marcos, Efectos de Radar, Efectos de Captura, Emotes. ✅ Items con precio, vista previa, rareza y descripción. ✅ Filtros: por categoría, por rareza, por precio, "solo disponibles con mis gemas". ✅ Items de edición limitada marcados con timer de disponibilidad. ✅ Carga en < 2 s con imágenes en CDN. | P1 | M | — |
| **MON-02** | **Como** jugador, **quiero** comprar gemas (moneda premium) con dinero real **para** adquirir cosméticos exclusivos. | ✅ Paquetes de gemas: 100 ($0.99), 550 ($4.99), 1200 ($9.99), 2500 ($19.99). ✅ Flujo de compra via Google Play Billing / Apple IAP nativo. ✅ Gemas acreditadas en < 5 s tras confirmación del pago. ✅ Recibo de compra disponible en historial. ✅ Manejo correcto de errores: pago denegado, sin conexión, compra duplicada. | P0 | L | — |
| **MON-03** | **Como** jugador, **quiero** comprar el Season Pass **para** acceder a recompensas exclusivas a lo largo de la temporada. | ✅ Season Pass con 2 versiones: Estándar ($4.99) y Premium ($9.99). ✅ Comparativa de beneficios clara antes de comprar. ✅ Compra via IAP nativo de la plataforma. ✅ Beneficios activos inmediatamente tras compra. ✅ Beneficios listados: multiplicador de XP, recompensas de pista exclusivas, cosméticos de temporada. | P0 | L | MON-02 |
| **MON-04** | **Como** jugador con Season Pass, **quiero** ver mi progreso en el pase de temporada **para** saber qué recompensas me esperan a continuación. | ✅ Pantalla de pase con track de 50 niveles visibles (scroll horizontal). ✅ Recompensas de cada nivel visibles sin comprar el pase (desbloqueadas al comprar). ✅ Progreso actual marcado con indicador animado. ✅ Recompensas reclamadas marcadas. ✅ Días restantes de temporada visibles. | P0 | M | MON-03 |
| **MON-05** | **Como** jugador, **quiero** ver el historial completo de mis compras **para** llevar registro de mis gastos. | ✅ Historial de transacciones: fecha, tipo (gemas, season pass, cosmético), monto, estado. ✅ Exportable como CSV o PDF. ✅ Datos consistentes con recibos de Google Play / App Store. ✅ Accesible desde Configuración > Mi cuenta > Historial de compras. | P1 | S | MON-02 |

---

## ÉPICA 14: TELEMETRÍA Y ADMINISTRACIÓN

> **Objetivo:** Dar al equipo de producto y operaciones herramientas para monitorear la salud del juego, gestionar el contenido y tomar decisiones basadas en datos.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **ADM-01** | **Como** product manager, **quiero** ver las métricas DAU/WAU/MAU en un dashboard actualizado diariamente **para** monitorear la salud de la comunidad. | ✅ Dashboard con gráficos de línea: DAU (últimos 30 días), WAU (últimas 12 semanas), MAU (últimos 12 meses). ✅ Comparativa con período anterior (semana pasada, mes pasado). ✅ Segmentación por plataforma (iOS/Android), región, facción. ✅ Dashboard accesible desde web (no requiere app). ✅ Datos actualizados cada 24 h (batch process a las 02:00 UTC). | P0 | L | — |
| **ADM-02** | **Como** operador, **quiero** gestionar las balizas desde el panel de admin **para** añadir, editar o desactivar hardware de la red. | ✅ CRUD completo de balizas (crear, leer, actualizar, desactivar). ✅ Búsqueda por UUID, nombre, zona, estado. ✅ Mapa visual con la posición de todas las balizas. ✅ Log de auditoría: quién hizo qué y cuándo. ✅ Desactivar baliza la quita del juego en < 5 min sin afectar a jugadores ya en captura. | P0 | L | BAL-08 |
| **ADM-03** | **Como** moderador, **quiero** buscar un jugador por nombre de usuario o ID **para** investigar reportes, revisar su historial y aplicar sanciones. | ✅ Búsqueda por username (parcial), ID de jugador, email. ✅ Ficha de jugador: datos de cuenta, última actividad, historial de capturas (últimas 100), historial de sanciones, trust score, flags de anti-cheat. ✅ Acciones desde ficha: advertir, suspender (1-30 días), banear permanentemente. ✅ Todas las acciones quedan en log con autor. | P0 | L | AC-04 |
| **ADM-04** | **Como** game designer, **quiero** crear y gestionar eventos desde el panel de admin **para** planificar el calendario de contenido sin necesidad de deployar código. | ✅ Formulario de evento: nombre, descripción, tipo, fechas inicio/fin, mecánica, recompensas. ✅ Preview del evento antes de publicar. ✅ Publicación programada (auto-publica en fecha/hora definida). ✅ Edición de evento activo (excepto fechas). ✅ Duplicar evento existente para reusar configuración. | P1 | L | EVT-02 |
| **ADM-05** | **Como** analista de datos, **quiero** exportar datos de analítica en formato CSV **para** realizar análisis avanzados en herramientas externas. | ✅ Exportación disponible para: capturas, sesiones, economía, cohortes de retención. ✅ Filtros por rango de fechas, región, segmento de usuario. ✅ Exportación asíncrona: se genera en background, notificación con link de descarga. ✅ Archivos disponibles para descarga por 24 h. ✅ Datos anonimizados por defecto (sin PII). ✅ Acceso solo para rol "analyst" o superior. | P1 | L | ADM-01 |

---

## ÉPICA 15: MODERACIÓN

> **Objetivo:** Mantener una comunidad sana y justa mediante herramientas de reporte, revisión y sanción que sean equitativas, transparentes y escalables.

| ID | Historia de Usuario | Criterios de Aceptación | Prioridad | Estimación | Dependencias |
|----|---------------------|-------------------------|-----------|------------|--------------|
| **MOD-01** | **Como** jugador, **quiero** reportar a un jugador o clan por comportamiento inapropiado **para** contribuir a mantener una comunidad sana. | ✅ Opción "Reportar" accesible desde perfil de jugador y perfil de clan. ✅ Categorías de reporte: trampa, acoso, nombre inapropiado, spam, otro. ✅ Campo de descripción opcional (máx. 500 chars). ✅ Confirmación de reporte recibido con número de referencia. ✅ Límite de 10 reportes por día por jugador (previene abuso). ✅ El reportado no es notificado de quién lo reportó. | P0 | S | — |
| **MOD-02** | **Como** moderador, **quiero** ver la cola de reportes pendientes priorizados por severidad **para** gestionar eficientemente mi tiempo de revisión. | ✅ Cola con filtros: tipo, severidad (alta/media/baja), estado (pendiente/en revisión/resuelto). ✅ Prioridad automática: reportes de trampa > acoso > otros. ✅ Asignación de caso a moderador con bloqueo (evita doble revisión). ✅ Tiempo medio de respuesta objetivo < 48 h visible en dashboard. ✅ Acciones disponibles: resolver (sin acción), advertir, suspender, banear. | P0 | L | MOD-01 |
| **MOD-03** | **Como** moderador, **quiero** emitir advertencias formales a jugadores que violen las normas **para** darles una oportunidad de corregir su comportamiento antes de sanciones mayores. | ✅ Advertencia enviada como notificación in-app + email. ✅ Contenido: descripción de la violación (sin revelar detalles del reporte), política infringida, consecuencias de reincidencia. ✅ Advertencia registrada en historial del jugador. ✅ Tres advertencias en 30 días activan revisión automática para suspensión. | P0 | S | MOD-02 |
| **MOD-04** | **Como** moderador, **quiero** aplicar suspensiones temporales a jugadores **para** frenar comportamientos dañinos sin recurrir inmediatamente al ban permanente. | ✅ Duraciones disponibles: 1, 3, 7, 14, 30 días. ✅ Jugador suspendido no puede iniciar sesión durante la suspensión. ✅ Notificación al jugador con duración, motivo (genérico) y fecha de fin. ✅ Durante suspensión, cuenta y datos preservados. ✅ Suspensión registrada en historial; 3+ suspensiones en 90 días → revisión para ban. | P0 | S | MOD-02 |
| **MOD-05** | **Como** jugador sancionado injustamente, **quiero** apelar una sanción **para** tener la oportunidad de que mi caso sea revisado por un humano. | ✅ Formulario de apelación disponible en el email de notificación de sanción y en la web de soporte. ✅ Campo de explicación (máx. 1,000 chars). ✅ Una sola apelación permitida por sanción. ✅ Tiempo de respuesta comprometido: < 5 días hábiles. ✅ Respuesta por email con decisión y razonamiento. ✅ Si se aprueba la apelación: sanción revertida, recursos/progreso restaurados, disculpa. | P1 | M | MOD-04 |

---

> **Resumen del Backlog**
>
> | Épica | Historias | P0 | P1 | P2 | P3 | Total SP (estimado) |
> |-------|-----------|----|----|----|----|---------------------|
> | 1. Onboarding | 10 | 8 | 2 | 0 | 0 | ~42 días |
> | 2. Escaneo BLE | 10 | 5 | 3 | 2 | 0 | ~47 días |
> | 3. Radar e Interfaz | 8 | 4 | 4 | 0 | 0 | ~35 días |
> | 4. Misiones | 8 | 6 | 2 | 0 | 0 | ~38 días |
> | 5. Balizas | 8 | 4 | 2 | 2 | 0 | ~40 días |
> | 6. Minijuegos | 8 | 5 | 3 | 0 | 0 | ~45 días |
> | 7. Recompensas / Economía | 8 | 4 | 3 | 1 | 0 | ~42 días |
> | 8. Progresión | 6 | 2 | 2 | 2 | 0 | ~33 días |
> | 9. Clanes | 8 | 4 | 4 | 0 | 0 | ~52 días |
> | 10. Territorios | 6 | 0 | 4 | 2 | 0 | ~35 días |
> | 11. Eventos | 5 | 0 | 4 | 1 | 0 | ~28 días |
> | 12. Anti-Cheat | 5 | 4 | 1 | 0 | 0 | ~55 días |
> | 13. Monetización | 5 | 3 | 2 | 0 | 0 | ~30 días |
> | 14. Telemetría / Admin | 5 | 3 | 2 | 0 | 0 | ~40 días |
> | 15. Moderación | 5 | 4 | 1 | 0 | 0 | ~25 días |
> | **TOTAL** | **100** | **56** | **37** | **10** | **0** | **~587 días-persona** |

---

---

# SECCIÓN 24: PROYECCIÓN FINANCIERA COMPARATIVA

> **Documento de referencia:** Análisis de viabilidad económica para TREASURE HUNTERS IOT: THE LAST SIGNAL  
> **Fecha de referencia:** 2025  
> **Audiencia:** Fundadores, inversores early-stage, comité de producto  

---

## 24.1 Supuestos Generales

```
╔══════════════════════════════════════════════════════════════╗
║          PARÁMETROS ECONÓMICOS BASE — V1.0                  ║
╠══════════════════════════════════════════════════════════════╣
║  Moneda base              │  USD                            ║
║  Modelo de negocio        │  Free-to-Play (F2P) cosmético   ║
║  Conversion rate          │  5% (F2P → jugador pagador)     ║
║  ARPU promedio target     │  $3.00 / mes                    ║
║  ARPPU target             │  $18.00 / mes                   ║
║  CAC blended              │  $1.50 (orgánico + paid media)  ║
║  Churn mensual — lanzam.  │  8%                             ║
║  Churn mensual — maduro   │  5% (objetivo Año 2+)           ║
║  D1 retention target      │  45%                            ║
║  D7 retention target      │  25%                            ║
║  D30 retention target     │  12%                            ║
║  Equipo POC               │  2-3 fundadores + 3-4 contractors║
╚══════════════════════════════════════════════════════════════╝
```

---

## 24.2 Costos de Infraestructura: Comparativa por Escala

### 24.2.1 Escala POC: 100 – 1,000 Jugadores Activos

| Componente | Docker Compose (VPS) | K8s Self-managed | Cloud Gestionado |
|---|---|---|---|
| **Cómputo** | $25–40/mes (1 VPS 4vCPU/8GB) | $80–150/mes (3 nodos × $28) | $150–300/mes (ECS/Cloud Run) |
| **MongoDB** | $0 (container) | $0 (container) | $57/mes (Atlas M10) |
| **Redis** | $0 (container) | $0 (container) | $30/mes (Redis Cloud 100MB) |
| **Storage** | Incluido en VPS | $10/mes (PV local) | $20/mes (S3-compatible) |
| **CDN** | $0 (Cloudflare free) | $0 | $5/mes |
| **Monitoreo** | $0 (Prometheus local) | $10/mes | $20/mes (managed) |
| **Backup** | $5/mes (snapshots) | $10/mes | $15/mes (automated) |
| **TOTAL MENSUAL** | **$30–45/mes** | **$110–180/mes** | **$297–447/mes** |
| **Mantenimiento devops** | Alto (manual, ops on-call) | Medio | Bajo (managed SLA) |
| **Alta disponibilidad** | ❌ No | ⚠️ Parcial (1 master) | ✅ Sí (multi-AZ) |
| **Escalado automático** | ❌ No | ⚠️ Manual (HPA) | ✅ Sí |
| **Time-to-deploy nuevo servicio** | 15 min | 45 min | 10 min |
| **Curva de aprendizaje** | Baja | Alta | Media |
| **Riesgo de downtime** | Alto | Medio | Bajo |
| **Recomendación** | ✅ **ELEGIR PARA POC** | ⚠️ Overcomplejo | ❌ Muy caro para este tamaño |

> **Veredicto POC:** Docker Compose en un VPS de DigitalOcean ($28/mes, 4vCPU/8GB RAM) es la decisión correcta. Permite al equipo fundador iterar rápido sin costos de infraestructura significativos ni overhead operativo.

---

### 24.2.2 Escala Beta: 10,000 Jugadores Activos

| Componente | Docker Compose (VPS) | K8s Self-managed | Cloud Gestionado |
|---|---|---|---|
| **Cómputo** | $120–200/mes (VPS grande) | $200–350/mes (5-6 nodos) | $400–700/mes (ECS / GKE) |
| **MongoDB** | $0 (replica set en VMs) | $40/mes (PVs) | $165/mes (Atlas M20) |
| **Redis** | $0 (Redis Sentinel) | $30/mes | $89/mes (Redis Cloud 1GB) |
| **Storage + CDN** | $30/mes | $40/mes | $60/mes |
| **Load Balancer** | $12/mes (DigitalOcean LB) | $20/mes | $25/mes |
| **Monitoreo (Grafana Cloud)** | $0–29/mes | $29/mes | $50/mes |
| **Backup y snapshots** | $20/mes | $30/mes | $50/mes |
| **TOTAL MENSUAL** | **$182–311/mes** | **$389–594/mes** | **$839–1,149/mes** |
| **Mantenimiento devops** | Muy alto (jugadores reales afectados) | Alto | Medio-bajo |
| **Alta disponibilidad** | ❌ No | ⚠️ Con trabajo extra | ✅ Sí |
| **Escalado ante picos** | ❌ Manual y lento | ⚠️ Semi-automático | ✅ Automático |
| **Costo de incidente** | Alto (pérdida de players) | Medio | Bajo (SLA 99.9%) |
| **Recomendación** | ❌ Riesgo alto | ✅ **ELEGIR PARA BETA** | ⚠️ Evaluar si funding disponible |

> **Veredicto Beta:** K8s self-managed en DigitalOcean Kubernetes (DOKS) + DigitalOcean Managed MongoDB o Atlas M20 ofrece el mejor balance coste/complejidad. El equipo aprende K8s con stakes reales pero manejables.

---

### 24.2.3 Escala Lanzamiento: 100,000+ Jugadores Activos

| Componente | K8s Self-managed | Híbrido (recomendado) | Cloud Gestionado Full |
|---|---|---|---|
| **Cómputo (K8s)** | $800–1,200/mes (10-15 nodos) | $600–900/mes (8-12 nodos) | $2,000–3,500/mes (GKE/EKS) |
| **MongoDB Atlas** | $0 (self-hosted) | $700/mes (Atlas M50) | $700/mes (Atlas M50) |
| **Redis Enterprise** | $200/mes (self-managed) | $290/mes (Redis Cloud 10GB) | $290/mes |
| **CDN (Cloudflare Pro)** | $200/mes | $200/mes | $200/mes |
| **Load Balancers (×3)** | $90/mes | $75/mes | $150/mes |
| **Storage (objetos + DB)** | $150/mes | $200/mes | $300/mes |
| **Monitoreo (Datadog)** | $200/mes | $250/mes | $400/mes |
| **WAF / DDoS protection** | $100/mes | $200/mes (Cloudflare Biz) | $300/mes |
| **Backup y DR** | $150/mes | $200/mes | $350/mes |
| **DevOps FTE (1 persona)** | $6,000/mes | $3,000/mes (part-time) | $1,500/mes (part-time) |
| **TOTAL MENSUAL** | **~$7,890/mes** | **~$5,715/mes** | **~$8,690/mes** |
| **Alta disponibilidad** | ⚠️ Con esfuerzo | ✅ Sí | ✅ Sí |
| **Escalado global** | ❌ Complejo | ✅ Con effort | ✅ Nativo |
| **Observabilidad** | ⚠️ Manual | ✅ Buena | ✅ Excelente |
| **Recomendación** | ⚠️ No escala bien | ✅ **ELEGIR PARA LAUNCH** | ⚠️ Overcost al inicio |

> **Veredicto Lanzamiento:** La arquitectura híbrida con K8s self-managed (workloads de juego) + MongoDB Atlas M50 (datos críticos) + Redis Cloud Enterprise (sesiones/caché) es el equilibrio óptimo entre control, costo y fiabilidad.

---

### 24.2.4 Escala Global: 1M+ Jugadores Activos

| Componente | Estimación Mensual |
|---|---|
| Cómputo (GKE multi-región, 30-50 nodos) | $8,000–15,000/mes |
| MongoDB Atlas M200 (sharded) | $2,500/mes |
| Redis Enterprise Cloud (25GB) | $800/mes |
| CDN (Cloudflare Enterprise) | $1,000/mes |
| Storage (multi-región) | $800/mes |
| Monitoreo + APM + logging | $1,000/mes |
| WAF + DDoS + seguridad | $1,500/mes |
| DevOps team (2 SREs) | $20,000/mes |
| **TOTAL MENSUAL** | **~$35,600–$42,600/mes** |

> **Veredicto Global:** A esta escala, cloud gestionado full (GCP/AWS) es la única opción viable. El costo de ingeniería de mantener infra propia supera el ahorro. El foco del equipo debe estar en el producto, no en ops.

---

### 24.2.5 Resumen Visual de Evolución de Costos de Infraestructura

```
Costo mensual de infraestructura por escala:

$45K ┤                                               ████
$40K ┤                                             ██████
$35K ┤                                           ████████
$30K ┤                                         ██████████
$25K ┤                                       ████████████
$20K ┤                                     ██████████████
$15K ┤                                   ████████████████
$10K ┤                             ████████████████████
 $5K ┤               ████████████████████████████████████
 $1K ┤█████████████████████████████████████████████████
     └────────────────────────────────────────────────────
      100    1K     5K    10K    50K   100K  500K    1M+
                         MAU (Jugadores activos)

  █ Docker Compose/VPS     █ K8s Híbrido     █ Cloud Full Managed
```

---

### 24.2.6 Recomendación de Arquitectura por Fase

| Fase | Escala | Stack Recomendado | Costo Infra/mes | Justificación |
|---|---|---|---|---|
| **POC** (meses 0-3) | < 1K MAU | Docker Compose en DigitalOcean VPS | $30–45 | Costo mínimo, iteración máxima, sin overhead de K8s |
| **Beta** (meses 4-9) | 1K–10K MAU | DOKS (K8s) + Atlas M20 + Redis Cloud 100MB | $400–600 | Balance costo/complejidad; equipo aprende K8s con stakes reales |
| **Launch** (meses 10-21) | 10K–100K MAU | K8s híbrido + Atlas M50 + Redis Cloud 10GB | $4,000–6,000 | Alta disponibilidad garantizada; costos controlados |
| **Scale** (Año 2+) | 100K–1M MAU | GKE multi-región + Atlas M200 + Redis Enterprise | $15,000–35,000 | Escalado automático global; SRE team justificado |
| **Global** (Año 3+) | 1M+ MAU | Full managed multi-cloud + CDN enterprise | $35,000–45,000 | Time-to-market > optimización de costo a esta escala |

---

## 24.3 Proyección Financiera — Año 1 (Post-Lanzamiento)

### 24.3.1 Escenario Conservador — 10,000 MAU al final del Año 1

#### Estructura de Costos Mensual (Mes 12)

| Categoría | Detalle | Costo/mes |
|---|---|---|
| **Infraestructura** | K8s híbrido + Atlas M20 | $500 |
| **Equipo (5 FTE)** | 2 dev backend, 1 dev mobile, 1 PM/diseño, 1 QA/ops | $25,000 |
| **Marketing** | Social media ads + ASO + influencers gaming | $2,000 |
| **Legal y compliance** | GDPR, términos, contratos | $500 |
| **Herramientas y SaaS** | Firebase, Figma, Jira, Slack, CI/CD | $300 |
| **Misceláneos** | Hardware ESP32 de prueba, eventos, gastos operativos | $200 |
| **TOTAL MENSUAL** | | **$28,500** |

#### Estructura de Ingresos (Mes 12)

| Métrica | Valor |
|---|---|
| MAU | 10,000 |
| DAU (30% de MAU) | 3,000 |
| Conversion rate | 5% |
| Jugadores pagadores activos | 150 |
| ARPPU | $18/mes |
| **Ingreso mensual** | **$2,700** |
| **Pérdida mensual** | **-$25,800** |

#### MAU necesario para break-even

```
Break-even = Costos totales / (ARPPU × conversion rate)
           = $28,500 / ($18 × 5%)
           = $28,500 / $0.90
           = 31,667 jugadores pagadores
           → Requiere ~633,340 MAU (con conversion 5%)

Nota: Reduciendo costos a estructura mínima ($10,000/mes con equipo parcial):
           = $10,000 / $0.90 = 11,111 jugadores pagadores
           → Requiere ~222,220 MAU

Objetivo realista conservador: 52,000 MAU → $93,600 ingresos vs $28,500 costos → Break-even
```

---

### 24.3.2 Escenario Medio — 50,000 MAU al final del Año 1

#### Estructura de Costos Mensual (Mes 12)

| Categoría | Detalle | Costo/mes |
|---|---|---|
| **Infraestructura** | K8s híbrido + Atlas M30 + Redis 1GB | $1,500 |
| **Equipo (8 FTE)** | 3 backend, 2 mobile, 1 PM, 1 diseño, 1 growth | $40,000 |
| **Marketing** | Performance ads + influencers + PR | $5,000 |
| **Legal y compliance** | GDPR + asesoría ongoing + registros IP | $1,000 |
| **Herramientas y SaaS** | Suite completa + analítica + soporte | $800 |
| **Customer Support** | 1 CS contractor part-time | $1,200 |
| **Misceláneos** | Eventos, partnerships, hardware | $500 |
| **TOTAL MENSUAL** | | **$50,000** |

#### Estructura de Ingresos (Mes 12)

| Métrica | Valor |
|---|---|
| MAU | 50,000 |
| DAU (36% de MAU) | 18,000 |
| Jugadores pagadores (5%) | 900 |
| ARPPU | $18/mes |
| **Ingreso mensual** | **$16,200** |
| **Pérdida mensual** | **-$33,800** |

#### MAU necesario para break-even (escenario medio)

```
Break-even = $50,000 / ($18 × 5%) = 55,556 jugadores pagadores
           → 1,111,120 MAU con conversion 5%

Con ARPPU mejorado a $25 (madurez económica año 2):
           = $50,000 / ($25 × 5%) = 40,000 pagadores → 800,000 MAU

Con mayor conversion (8%, post-optimización de monetización):
           = $50,000 / ($18 × 8%) = 34,722 pagadores → 434,025 MAU

Objetivo realista medio (fin Año 2): 180,000 MAU + ARPPU $22 + conversion 6% = Break-even
```

---

### 24.3.3 Escenario Optimista — 200,000 MAU al final del Año 1

#### Estructura de Costos Mensual (Mes 12)

| Categoría | Detalle | Costo/mes |
|---|---|---|
| **Infraestructura** | K8s multi-zona + Atlas M50 + Redis 10GB | $4,000 |
| **Equipo (15 FTE)** | Equipo completo cross-funcional | $75,000 |
| **Marketing** | Performance + brand + eventos + partnerships | $15,000 |
| **Legal** | Equipo legal in-house + compliance global | $3,000 |
| **Herramientas** | Enterprise stack completo | $2,000 |
| **TOTAL MENSUAL** | | **$99,000** |

#### Estructura de Ingresos (Mes 12)

| Métrica | Valor |
|---|---|
| MAU | 200,000 |
| DAU (40% de MAU) | 80,000 |
| Jugadores pagadores (5%) | 4,000 |
| ARPPU | $18/mes |
| **Ingreso mensual** | **$72,000** |
| **Pérdida mensual** | **-$27,000** |

```
Target fin Año 1: 300,000 MAU → 6,000 pagadores × $18 = $108,000 > $99,000 costos
→ Break-even logrado a los ~10 meses de lanzamiento en escenario optimista
```

---

## 24.4 Proyección Detallada a 3 Años

### 24.4.1 Escenario Conservador

```
┌──────────────────────────────────────────────────────────┐
│                 ESCENARIO CONSERVADOR                    │
├──────────┬──────────┬──────────┬────────────┬───────────┤
│  Período │    MAU   │ Ingresos │   Costos   │ Resultado │
├──────────┼──────────┼──────────┼────────────┼───────────┤
│  Año 1   │  10,000  │  $48K    │  $358K     │  -$310K   │
│  Año 2   │  50,000  │  $216K   │  $366K     │  -$150K   │
│  Año 3   │  200,000 │  $864K   │  $664K     │  +$200K   │
├──────────┼──────────┼──────────┼────────────┼───────────┤
│ TOTAL 3A │    —     │ $1,128K  │ $1,388K    │  -$260K   │
└──────────┴──────────┴──────────┴────────────┴───────────┘

Notas:
• Año 1 incluye costos pre-lanzamiento (desarrollo POC + beta: ~$150K)
• Año 2: break-even operativo en mes 20 (MAU ~85K)
• Año 3: equipo estabilizado (12 FTE), infra optimizada, churn reducido al 5%
• Déficit de 3 años (-$260K) es financiable con una seed round modesta
• Requiere: FFF ($40K) + Angel ($220K) = $260K de runway garantizado
```

### 24.4.2 Escenario Medio

```
┌──────────────────────────────────────────────────────────┐
│                   ESCENARIO MEDIO                        │
├──────────┬──────────┬──────────┬────────────┬───────────┤
│  Período │    MAU   │ Ingresos │   Costos   │ Resultado │
├──────────┼──────────┼──────────┼────────────┼───────────┤
│  Año 1   │  50,000  │  $80K    │  $480K     │  -$400K   │
│  Año 2   │  200,000 │  $720K   │  $670K     │  +$50K    │
│  Año 3   │  500,000 │ $2,400K  │ $1,600K    │  +$800K   │
├──────────┼──────────┼──────────┼────────────┼───────────┤
│ TOTAL 3A │    —     │ $3,200K  │ $2,750K    │  +$450K   │
└──────────┴──────────┴──────────┴────────────┴───────────┘

Notas:
• Break-even operativo en mes 18 (MAU ~120K)
• Año 2: arranque de línea de merchandising (~$50K revenue adicional)
• Año 3: expansión a LATAM + España abre nuevo segmento de mercado
• Primer repago a inversores posible en Q4 Año 3
• ARPPU mejorado a $22 en Año 3 (mejor catálogo cosmético + Season Pass premium)
```

### 24.4.3 Escenario Optimista

```
┌──────────────────────────────────────────────────────────┐
│                  ESCENARIO OPTIMISTA                     │
├──────────┬──────────┬──────────┬────────────┬───────────┤
│  Período │    MAU   │ Ingresos │   Costos   │ Resultado │
├──────────┼──────────┼──────────┼────────────┼───────────┤
│  Año 1   │  300,000 │  $500K   │ $1,000K    │  -$500K   │
│  Año 2   │  1,000K  │ $2,160K  │ $1,660K    │  +$500K   │
│  Año 3   │  2,000K  │ $8,640K  │ $5,140K    │ +$3,500K  │
├──────────┼──────────┼──────────┼────────────┼───────────┤
│ TOTAL 3A │    —     │$11,300K  │ $7,800K    │ +$3,500K  │
└──────────┴──────────┴──────────┴────────────┴───────────┘

Notas:
• Requiere crecimiento viral orgánico (K factor > 1.2) sostenido
• Año 2: licencias IP a terceros (hardware oficial, merchandising premium)
• Año 3: expansión a ciudades tier-1 en USA, Europa y Asia-Pacífico
• A esta escala, valoración de la compañía: $35-70M (10-20× revenue)
• Series A ($8-12M) requerida en mes 14-16 para sostener crecimiento
```

---

## 24.5 Proyección a 5 Años — Escenario Medio (Tabla Maestra)

| Año | MAU | DAU (est.) | Pagadores (5%) | ARPPU | Revenue Anual | Costos Anuales | EBITDA | Acumulado |
|-----|-----|------------|----------------|-------|---------------|----------------|--------|-----------|
| **1** | 50K | 18K | 900 | $18 | $80K | $480K | **-$400K** | -$400K |
| **2** | 200K | 80K | 4,000 | $20 | $960K | $910K | **+$50K** | -$350K |
| **3** | 500K | 210K | 10,500 | $22 | $2,772K | $1,972K | **+$800K** | +$450K |
| **4** | 800K | 360K | 18,000 | $24 | $5,184K | $2,984K | **+$2,200K** | +$2,650K |
| **5** | 1.2M | 576K | 30,000 | $25 | $9,000K | $4,500K | **+$4,500K** | +$7,150K |

> **Supuesto de mejora de ARPPU:** Incremento gradual de $18 → $25 debido a maduración del catálogo, mayor engagement con Season Pass y lanzamiento de eventos premium de pago.

```
Revenue 5 años (Escenario Medio):
$9M ┤                                                   ████
$8M ┤                                                 ██████
$7M ┤                                               ████████
$6M ┤                                             ██████████
$5M ┤                                           ████████████
$4M ┤                                 ██████████████████████
$3M ┤                           ████████████████████████████
$2M ┤               ████████████████████████████████████████
$1M ┤         ██████████████████████████████████████████████
$0K ┤█████████████████████████████████████████████████████
    └──────────────────────────────────────────────────────
     Año 1         Año 2        Año 3        Año 4       Año 5

  ██ Revenue     ██ Costos
```

---

## 24.6 Fuentes de Financiación Recomendadas

| Fase | Período | Fuente | Monto Target | Dilución Estimada | Hito a Demostrar |
|---|---|---|---|---|---|
| **Pre-seed / FFF** | Meses 0–3 | Bootstrapped + amigos/familia | $30–50K | 0–5% | POC funcional con ESP32 + app MVP |
| **Angel + Grants** | Meses 4–9 | Business angels gaming/IoT + gaming grants (CDMX/España/EU) | $200–500K | 10–20% | Beta con 500+ jugadores activos, KPIs de retención D7 > 20% |
| **Seed Round** | Meses 10–21 | VCs early-stage gaming/IoT (Bitkraft, Play Ventures, The Games Fund) | $1–3M | 20–30% | Lanzamiento público, 10K MAU, unit economics validados (LTV/CAC > 3x) |
| **Series A** | Año 2–3 | VCs growth (a16z games, Andreessen, fondos LATAM) | $5–15M | 15–25% | $1M+ ARR, D30 > 12%, expansión geográfica iniciada |

> **Nota sobre grants gaming:** Fondos disponibles para estudio con base en México/España: ProSoft (México, hasta $100K USD), CDPQ Gaming (Canadá, accesible con co-inversión), Creative Europe (UE, hasta €200K para prototipos con valor cultural), IndieCade grants. La naturaleza IoT del juego también abre grants de smart city en múltiples jurisdicciones.

---

## 24.7 Métricas de Economía Unitaria (Unit Economics)

### 24.7.1 Customer Acquisition Cost (CAC) por Canal

| Canal | CAC Estimado | Volumen posible | Calidad de usuario (D30) | Notas |
|---|---|---|---|---|
| Orgánico / WOM | $0.30 | Bajo-Medio | Alta (25%+ D30) | Mejor canal a largo plazo; community-led |
| ASO (App Store Optimization) | $0.50 | Medio | Alta (22% D30) | Requiere inversión en ratings y reviews |
| Social media (TikTok/Instagram) | $1.20 | Alto | Media (15% D30) | Alta escala, menor intent |
| Gaming ads (Unity Ads, IronSource) | $2.00 | Alto | Media-baja (12% D30) | Riesgo de install farms |
| Influencers gaming (micro) | $1.80 | Medio | Alta (20% D30) | Alto trust, difícil de escalar |
| Influencers gaming (macro) | $3.50 | Muy alto | Baja-media (10% D30) | Bueno para brand awareness |
| PR y prensa gaming | $0.80 | Bajo | Alta (28% D30) | Cost-effective pero impredecible |
| **CAC Blended Target** | **$1.50** | — | **18% D30** | Ponderado por mix de canales |

### 24.7.2 Lifetime Value (LTV)

```
LTV por segmento de usuario (horizonte 18 meses):

Segmento F2P (95% de usuarios):
  Ingreso directo:  $0 (sin IAP)
  Ingreso indirecto: No implementado en V1 (sin ads para preservar UX)
  LTV F2P:          $0.10 (valor de red para jugadores pagadores)

Segmento Pagador (5% de usuarios):
  ARPPU:            $18/mes
  Tiempo promedio pago: 8 meses (LTV window 18 meses, churn ajustado)
  LTV Pagador:      $18 × 8 = $144

LTV Blended (toda la cohorte de instalación):
  LTV = (LTV_F2P × 95%) + (LTV_Pago × 5%)
  LTV = ($0.10 × 0.95) + ($144 × 0.05)
  LTV = $0.095 + $7.20
  LTV Blended = $7.30

LTV/CAC Ratio:
  $7.30 / $1.50 = 4.87×  ✅ Saludable (objetivo: > 3×)

Payback Period:
  $1.50 CAC / ($18 × 5% conversion mensual de cohorte) = 1.67 meses ✅ Excelente
```

### 24.7.3 Cohort Retention y Revenue Model

```
Cohorte de 10,000 instalaciones (mes 0):

Mes 0:  10,000 usuarios (100%)       → $0 revenue (onboarding)
Mes 1:   4,500 usuarios (D30: 45%)   → 225 pagadores × $18 = $4,050
Mes 2:   2,700 usuarios (27%)        → 135 pagadores × $18 = $2,430
Mes 3:   1,890 usuarios (18.9%)      → 95 pagadores × $18  = $1,710
Mes 6:   1,080 usuarios (10.8%)      → 54 pagadores × $18  = $972
Mes 12:    648 usuarios (6.5%)       → 32 pagadores × $18  = $580
Mes 18:    388 usuarios (3.9%)       → 19 pagadores × $18  = $348

Revenue total de cohorte 10K en 18 meses:
  ≈ $23,500 total
  ÷ 10,000 instalaciones
  = $2.35 LTV real (sin considerar upsell)

Con upsell (season pass + eventos premium): LTV target $7.30
CAC $1.50 → Ratio 4.87× ✅
```

---

## 24.8 Riesgos Financieros y Plan de Mitigación

| # | Riesgo | Impacto | Probabilidad | Estrategia de Mitigación | Owner |
|---|---|---|---|---|---|
| **RF-01** | Costos de hardware ESP32 para despliegue de red | Medio | Alta | Modelo de operador comunitario: los operadores compran su propio hardware ($5-8/unidad) a cambio de revenue share del 10% de capturas en sus balizas. El estudio no financia el hardware. | CEO |
| **RF-02** | Limitaciones de iOS BLE retrasan el lanzamiento en Apple | Alto | Media | Priorizar lanzamiento en Android (MVP). Wrapper Capacitor para iOS con funcionalidad reducida (sin background scan). iOS prioritario en versión 1.2 post-validación de mercado. | CTO |
| **RF-03** | Retención D7 inferior al 20% objetivo | Muy Alto | Media | A/B testing intensivo pre-lanzamiento (mínimo 5 variantes de onboarding). Inversión en game feel y primer día de experiencia. Pivote rápido si D7 < 15% tras 2 semanas. | CPO |
| **RF-04** | Percepción de P2W destruye la comunidad | Alto | Baja | Política "Cosmetics-only" aplicada server-side. Transparency reports mensuales. Auditoría externa de balance de juego en Año 1. Consejo de jugadores consultivo. | CPO |
| **RF-05** | Multa regulatoria por datos de ubicación (GDPR/LGPD) | Medio | Media | Privacy-by-design: no almacenar coordenadas exactas (solo zona aproximada). DPA firmado con todos los proveedores cloud. Privacy audit externa antes del lanzamiento. | Legal |
| **RF-06** | Entrada de estudio AAA con producto similar | Alto | Baja | Velocidad de ejecución y construcción de moat comunitario antes de que AAA pueda reaccionar. Foco en ciudades tier-2/3 no rentables para grandes. Registrar IP clave. | CEO |
| **RF-07** | Burn rate supera el runway antes del próximo hito de funding | Crítico | Media | Hito de corte de costos pre-definido: si en mes 6 DAU < 500, reducir equipo a 3 FTE y operar como side project hasta ronda angel. No comprometer runway > 18 meses sin revenue visible. | CFO |
| **RF-08** | Fragmentación de Android afecta experiencia BLE | Medio | Alta | Test matrix con top-20 dispositivos Android por cuota de mercado. Feature flags para desactivar funciones inestables en dispositivos problemáticos. Política clara de versiones mínimas soportadas (Android 8+). | CTO |

---

## 24.9 Dashboard de Métricas Clave (KPIs de Seguimiento)

### Métricas de Salud del Juego (revisar semanalmente)

| KPI | Objetivo Lanzamiento | Objetivo Madurez | Señal de Alarma |
|---|---|---|---|
| D1 Retention | 45% | 50% | < 35% |
| D7 Retention | 25% | 30% | < 18% |
| D30 Retention | 12% | 18% | < 8% |
| DAU/MAU ratio | 30% | 40% | < 20% |
| Session length | 12 min | 18 min | < 8 min |
| Sessions/DAU | 1.8 | 2.5 | < 1.3 |
| Conversion F2P→Pago | 5% | 8% | < 3% |
| ARPPU | $18 | $25 | < $12 |
| LTV/CAC | 4.87× | 6× | < 3× |
| Churn mensual | 8% | 5% | > 12% |
| NPS | 40 | 60 | < 20 |

### Métricas de Operación (revisar diariamente en launch)

| KPI | Objetivo | Alarma |
|---|---|---|
| API latency p99 | < 200ms | > 500ms |
| Error rate | < 0.1% | > 1% |
| BLE proof rejection rate | < 0.1% (false positive) | > 0.5% |
| Uptime mensual | 99.9% | < 99.5% |
| Beacon online rate | > 95% | < 85% |
| Support tickets/DAU | < 0.5% | > 2% |

---

## 24.10 Cronograma Financiero Recomendado

```
TIMELINE DE HITOS FINANCIEROS:

Mes 0-3  │ POC + Bootstrapped
         │ Gasto: $15-30K (salarios mínimos + infra $45/mes)
         │ Hito: App funcional + 3 ESP32 desplegados + 20 testers

Mes 4-6  │ Cierre Angel Round ($200-300K)
         │ Gasto: $25K/mes (equipo ampliado)
         │ Hito: Beta cerrada con 200 jugadores, D7 > 20%

Mes 7-9  │ Beta abierta + optimización de monetización
         │ Gasto: $35K/mes
         │ Hito: 2,000 MAU beta, LTV/CAC validado > 3×

Mes 10-12│ Lanzamiento + Seed Round ($1-2M)
         │ Gasto: $45K/mes (equipo de lanzamiento)
         │ Hito: 10,000 MAU, revenue > $5K/mes

Mes 13-18│ Crecimiento post-launch
         │ Gasto: $50-70K/mes (escala de equipo)
         │ Hito: 50,000 MAU, break-even operativo en vista

Mes 19-24│ Series A prep ($5-10M)
         │ Gasto: $75K/mes
         │ Hito: 150,000 MAU, $50K/mes revenue, expansión geográfica

Año 3+   │ Series A deployment + scale
         │ Gasto: $150K/mes (equipo completo)
         │ Hito: 500K MAU, EBITDA positivo
```

---

> **Nota final de la Sección 24:** Las proyecciones presentadas son estimaciones basadas en benchmarks de la industria gaming móvil F2P (Sensor Tower, AppMagic, Newzoo 2024) ajustadas por la naturaleza IoT del producto. Las condiciones de mercado, la ejecución del equipo y los factores externos (regulatorios, competencia, adopción tecnológica BLE) pueden modificar significativamente estos números. Se recomienda revisar este documento financiero cada trimestre y ajustar las proyecciones con datos reales en cuanto estén disponibles.
>
> **El caso de inversión es sólido:** LTV/CAC 4.87×, payback < 2 meses, mercado IoT gaming inexplorado, y una propuesta de valor de experiencia física única que los juegos mobile convencionales no pueden replicar.

---

*— Fin Sección 23 y Sección 24 del GDD —*
*TREASURE HUNTERS IOT: THE LAST SIGNAL v1.0*
