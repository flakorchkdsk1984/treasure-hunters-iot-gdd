# THLS Scanner — App Expo

App nativa para iPhone y Android que detecta balizas BLE **sin conectarse** a ninguna red.

## Flujo de juego

```
[Teléfono] → escanea BLE pasivo (sin conectar)
                ↓ RSSI mejora al acercarse
         círculo pulsa más rápido
                ↓ rssi ≥ -47 dBm (~1 metro)
         vibra + avisa conectar WiFi "THLS-C001"
                ↓ usuario conecta WiFi (red abierta, sin pass)
         app detecta WiFi y abre botón "INICIAR MISIÓN"
                ↓
         abre http://192.168.4.1 → minijuego
```

## Instalación (primera vez)

### 1. Instalar dependencias

```bash
cd demo/app
npm install
```

### 2. Instalar Expo Go en el teléfono

- iPhone: https://apps.apple.com/app/expo-go/id982107779
- Android: https://play.google.com/store/apps/details?id=host.exp.exponent

### 3. Iniciar servidor de desarrollo

```bash
npx expo start
```

Aparece un QR en la terminal.

### 4. Escanear el QR

- **iPhone**: abrir la cámara y escanear
- **Android**: abrir Expo Go y escanear

## Permisos requeridos

| Permiso | Por qué |
|---------|---------|
| Bluetooth | Detectar balizas BLE (pasivo, sin conectar) |
| Ubicación | iOS/Android requieren ubicación para BLE scan |
| WiFi | Detectar cuando se conecta a THLS-C001 |

iOS pedirá los permisos la primera vez que se abre la app.

## Estados de señal

| Estado | RSSI | Color | Pulso |
|--------|------|-------|-------|
| SIN SEÑAL | < -94 | Gris | Sin pulso |
| POLAR | -94 a -84 | Azul navy | 3.5s |
| FRÍO | -84 a -75 | Azul | 3s |
| FRESCO | -75 a -67 | Teal | 2.5s |
| TIBIO | -67 a -60 | Púrpura | 2s |
| CALIENTE | -60 a -53 | Rosa | 1.2s |
| MUY CALIENTE | -53 a -47 | Naranja-rojo | 0.7s |
| ARDIENDO | -47 a -41 | Naranja | 0.35s |
| BALIZA LOCALIZADA | > -41 | Verde | 0.15s |

El círculo pulsa más rápido a medida que te acercás.
