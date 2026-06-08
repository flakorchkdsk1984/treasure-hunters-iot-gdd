/**
 * TREASURE HUNTERS IoT — THLS Scanner App v2.0
 * Detección HÍBRIDA: WiFi scan (lejos) + BLE scan (cerca) + WiFi connect (minijuego)
 *
 * MÁQUINA DE ESTADOS:
 *
 *   IDLE ──► [start] ──► WIFI_SCAN  (Android) / BLE_SCAN (iOS)
 *                              │
 *              WiFi RSSI > -70 │ (~20m)
 *                              ▼
 *                         HYBRID_SCAN  (WiFi + BLE simultáneo)
 *                              │
 *              BLE RSSI > -47  │ (~1m)
 *                              ▼
 *                         CLOSE  ── avisa conectar "THLS-C001"
 *                              │
 *              WiFi conectado  │
 *                              ▼
 *                         CHALLENGE  ── botón → 192.168.4.1
 *
 * iOS: sin WiFi scan (Apple lo prohíbe), solo BLE_SCAN → CLOSE → CHALLENGE
 */

import React, { useState, useEffect, useRef, useCallback } from 'react';
import {
  View, Text, StyleSheet, TouchableOpacity, Vibration,
  Animated, Easing, Platform, Alert, Linking, ScrollView,
  SafeAreaView, PermissionsAndroid,
} from 'react-native';
import { BleManager, State } from 'react-native-ble-plx';
import WifiManager from 'react-native-wifi-reborn';
import * as Network from 'expo-network';
import { StatusBar } from 'expo-status-bar';

// ── CONFIG ──────────────────────────────────────────────────
const BEACON_PREFIX       = 'THLS-';
const BEACON_WIFI_SSID    = 'THLS-C001';
const CHALLENGE_URL       = 'http://192.168.4.1';
const WIFI_SWITCH_RSSI    = -70;   // dBm: cambiar a híbrido (~20m)
const BLE_CLOSE_RSSI      = -47;   // dBm: activar challenge (~1m)
const WIFI_SCAN_INTERVAL  = 5000;  // ms entre scans WiFi
const CAN_WIFI_SCAN       = Platform.OS === 'android';  // iOS no permite

// ── ESTADOS DE SEÑAL ──────────────────────────────────────
const STATES = [
  { id:'dead',    label:'SIN SEÑAL',         min:-999, max:-94,  color:'#37474f', pulse:0    },
  { id:'polar',   label:'POLAR',             min:-94,  max:-84,  color:'#0d47a1', pulse:3.5  },
  { id:'cold',    label:'FRÍO',              min:-84,  max:-75,  color:'#1976d2', pulse:3    },
  { id:'cool',    label:'FRESCO',            min:-75,  max:-67,  color:'#00838f', pulse:2.5  },
  { id:'warm',    label:'TIBIO',             min:-67,  max:-60,  color:'#7b1fa2', pulse:2    },
  { id:'hot',     label:'CALIENTE',          min:-60,  max:-53,  color:'#c2185b', pulse:1.2  },
  { id:'vhot',    label:'MUY CALIENTE',      min:-53,  max:-47,  color:'#e64a19', pulse:0.7  },
  { id:'burning', label:'ARDIENDO',          min:-47,  max:-41,  color:'#ff6f00', pulse:0.35 },
  { id:'found',   label:'BALIZA LOCALIZADA', min:-41,  max:0,    color:'#00c853', pulse:0.15 },
];

function getState(rssi) {
  return STATES.find(s => rssi >= s.min && rssi < s.max) || STATES[0];
}
function getDist(rssi, txp = -59) {
  const d = Math.pow(10, (txp - rssi) / 25);
  return d < 1 ? '< 1 m' : d > 500 ? '> 500 m' : d.toFixed(0) + ' m';
}

// ── PERMISOS ANDROID ──────────────────────────────────────
async function requestAndroidPermissions() {
  if (Platform.OS !== 'android') return true;
  const api = Platform.Version;
  const perms = api >= 31
    ? [ PermissionsAndroid.PERMISSIONS.BLUETOOTH_SCAN,
        PermissionsAndroid.PERMISSIONS.BLUETOOTH_CONNECT,
        PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION ]
    : [ PermissionsAndroid.PERMISSIONS.ACCESS_FINE_LOCATION ];
  const r = await PermissionsAndroid.requestMultiple(perms);
  return Object.values(r).every(v => v === PermissionsAndroid.RESULTS.GRANTED);
}

// ── SMOOTH RSSI ───────────────────────────────────────────
function useSmoother(win = 5) {
  const h = useRef([]);
  return useCallback((v) => {
    h.current.push(v);
    if (h.current.length > win) h.current.shift();
    return Math.round(h.current.reduce((a, b) => a + b, 0) / h.current.length);
  }, []);
}

// ═══════════════════════════════════════════════════════════
// APP
// ═══════════════════════════════════════════════════════════
export default function App() {
  // Estado de detección híbrida
  const [scanPhase, setScanPhase] = useState('idle');
  // idle | wifi_scan | hybrid | close | challenge

  const [bleReady, setBleReady]   = useState(false);
  const [rssi, setRssi]           = useState(-100);
  const [wifiRssi, setWifiRssi]   = useState(null);  // RSSI del scan WiFi (sin conectar)
  const [bleRssi, setBleRssi]     = useState(null);  // RSSI BLE
  const [signal, setSignal]       = useState(STATES[0]);
  const [beaconName, setBeaconName] = useState(null);
  const [challengeFlag, setChallengeFlag] = useState(false);
  const [log, setLog]             = useState([]);

  const bleManager   = useRef(null);
  const wifiTimer    = useRef(null);
  const wifiMonitor  = useRef(null);
  const pulseAnim    = useRef(new Animated.Value(1)).current;
  const pulseLoop    = useRef(null);
  const smoothBle    = useSmoother(5);
  const smoothWifi   = useSmoother(3);
  const phaseRef     = useRef('idle');

  function setPhase(p) { phaseRef.current = p; setScanPhase(p); }

  // Init BLE
  useEffect(() => {
    bleManager.current = new BleManager();
    addLog(`${Platform.OS} ${Platform.Version} | WiFi scan: ${CAN_WIFI_SCAN ? 'SI' : 'NO (iOS)'}`);
    const sub = bleManager.current.onStateChange(s => {
      if (s === State.PoweredOn)    { setBleReady(true);  addLog('✓ BLE listo'); }
      if (s === State.PoweredOff)   { setBleReady(false); addLog('⚠ BT desactivado'); }
      if (s === State.Unauthorized) { addLog('✗ Permiso BLE denegado'); }
    }, true);
    return () => {
      sub.remove();
      stopAll();
      bleManager.current?.destroy();
    };
  }, []);

  // Animación pulso
  useEffect(() => {
    if (pulseLoop.current) pulseLoop.current.stop();
    if (!signal.pulse) { pulseAnim.setValue(1); return; }
    const dur = signal.pulse * 1000;
    pulseLoop.current = Animated.loop(Animated.sequence([
      Animated.timing(pulseAnim, { toValue: 1.4, duration: dur/2, easing: Easing.inOut(Easing.ease), useNativeDriver: true }),
      Animated.timing(pulseAnim, { toValue: 1.0, duration: dur/2, easing: Easing.inOut(Easing.ease), useNativeDriver: true }),
    ]));
    pulseLoop.current.start();
  }, [signal.id]);

  // Monitor WiFi conectado (fase close)
  useEffect(() => {
    if (scanPhase !== 'close') return;
    wifiMonitor.current = setInterval(async () => {
      try {
        const net = await Network.getNetworkStateAsync();
        if (net.type === Network.NetworkStateType.WIFI) {
          clearInterval(wifiMonitor.current);
          setPhase('challenge');
          addLog('✓ WiFi conectado — MISIÓN DISPONIBLE');
          Vibration.vibrate([200, 100, 200, 100, 400]);
        }
      } catch (_) {}
    }, 1500);
    return () => clearInterval(wifiMonitor.current);
  }, [scanPhase]);

  function addLog(msg) {
    const ts = new Date().toLocaleTimeString('es', { hour12: false });
    setLog(prev => [`[${ts}] ${msg}`, ...prev].slice(0, 50));
  }

  // ── UPDATE RSSI COMBINADO ──────────────────────────────
  function updateCombined(bRssi, wRssi) {
    let combined;
    if (bRssi !== null && wRssi !== null) {
      // Híbrido: BLE tiene más peso cuando está cerca, WiFi cuando lejos
      const bleWeight = bRssi > -65 ? 0.75 : 0.4;
      combined = Math.round(bRssi * bleWeight + wRssi * (1 - bleWeight));
    } else {
      combined = bRssi ?? wRssi ?? -100;
    }
    setRssi(combined);
    const st = getState(combined);
    setSignal(st);
    return combined;
  }

  // ── SCAN WIFI (Android, sin conectar) ──────────────────
  function startWifiScan() {
    if (!CAN_WIFI_SCAN) return;
    addLog('📶 Iniciando scan WiFi pasivo...');

    const doScan = async () => {
      try {
        const networks = await WifiManager.loadWifiList();
        const beacon = networks.find(n => n.SSID === BEACON_WIFI_SSID);
        if (beacon) {
          const ws = smoothWifi(beacon.level);
          setWifiRssi(ws);
          setBeaconName(beacon.SSID);
          addLog(`📶 WiFi: ${beacon.SSID} → ${ws} dBm`);
          const combined = updateCombined(bleRssi, ws);

          // Si WiFi ya es fuerte, arrancar BLE también
          if (ws >= WIFI_SWITCH_RSSI && phaseRef.current === 'wifi_scan') {
            addLog('📡 Señal fuerte — activando BLE...');
            setPhase('hybrid');
            startBleScan();
          }
        } else {
          addLog('📶 WiFi: baliza no detectada');
        }
      } catch (e) {
        addLog('WiFi scan error: ' + e.message);
      }
    };

    doScan();
    wifiTimer.current = setInterval(doScan, WIFI_SCAN_INTERVAL);
  }

  // ── SCAN BLE (pasivo) ───────────────────────────────────
  function startBleScan() {
    if (!bleReady) return;
    bleManager.current.startDeviceScan(null, { allowDuplicates: true }, (err, device) => {
      if (err) { addLog('BLE error: ' + err.message); return; }
      if (!device?.name?.startsWith(BEACON_PREFIX) &&
          !device?.localName?.startsWith(BEACON_PREFIX)) return;

      const name = device.name || device.localName || 'THLS-?';
      const bs = smoothBle(device.rssi ?? -100);
      setBleRssi(bs);
      setBeaconName(name);

      // Manufacturer data: challenge flag
      let chal = false;
      if (device.manufacturerData) {
        try {
          const b = Buffer.from(device.manufacturerData, 'base64');
          if (b.length >= 3) chal = b[2] === 0x01;
        } catch (_) {}
      }
      setChallengeFlag(chal);

      const combined = updateCombined(bs, wifiRssi);
      if (Math.random() < 0.4)
        addLog(`📡 BLE: ${name} → ${bs} dBm${chal ? ' ⚡' : ''} | combinado: ${combined}`);

      // Umbral de ~1 metro: activar fase CLOSE
      if (bs >= BLE_CLOSE_RSSI && phaseRef.current !== 'close' && phaseRef.current !== 'challenge') {
        setPhase('close');
        Vibration.vibrate([100, 80, 100, 80, 300]);
        addLog('⚡ A ~1 METRO — Conecta WiFi "' + BEACON_WIFI_SSID + '"');
        // Detener WiFi scan (ya no lo necesitamos)
        clearInterval(wifiTimer.current);
      }
    });
  }

  // ── START ──────────────────────────────────────────────
  async function startScan() {
    const granted = await requestAndroidPermissions();
    if (!granted) {
      Alert.alert('Permisos necesarios',
        'Activa Bluetooth y Ubicación en Ajustes → Aplicaciones → THLS Scanner.',
        [{ text: 'Ajustes', onPress: () => Linking.openSettings() }, { text: 'OK' }]);
      return;
    }

    if (CAN_WIFI_SCAN) {
      // Android: arrancar con WiFi scan (más alcance)
      setPhase('wifi_scan');
      addLog('=== BÚSQUEDA HÍBRIDA WiFi + BLE ===');
      startWifiScan();
      // BLE también arranca en paralelo para no perder tiempo
      addLog('📡 BLE en paralelo...');
      startBleScan();
    } else {
      // iOS: directo a BLE
      setPhase('hybrid');
      addLog('=== BÚSQUEDA BLE (iOS) ===');
      startBleScan();
    }
  }

  function stopAll() {
    bleManager.current?.stopDeviceScan();
    clearInterval(wifiTimer.current);
    clearInterval(wifiMonitor.current);
    setPhase('idle');
    setRssi(-100); setWifiRssi(null); setBleRssi(null);
    setSignal(STATES[0]); setBeaconName(null); setChallengeFlag(false);
    addLog('— Búsqueda detenida —');
  }

  // ── RENDER ─────────────────────────────────────────────
  const scanning   = scanPhase !== 'idle';
  const phaseLabel = {
    idle:       '—',
    wifi_scan:  '📶 WiFi',
    hybrid:     '📶+📡 Híbrido',
    close:      '📡 BLE',
    challenge:  '✓ Conectado',
  }[scanPhase] || '—';

  return (
    <SafeAreaView style={s.safe}>
      <StatusBar style="light" />

      {/* Header */}
      <View style={s.header}>
        <Text style={s.logo}>TREASURE <Text style={s.logoRed}>HUNTERS</Text> IoT</Text>
        <Text style={s.headerSub}>{phaseLabel} | {beaconName || 'sin baliza'}</Text>
      </View>

      {/* Círculo RSSI pulsante */}
      <View style={s.circleWrap}>
        <Animated.View style={[s.circle, {
          backgroundColor: signal.color + '33',
          borderColor: signal.color,
          transform: [{ scale: pulseAnim }],
        }]}>
          <Text style={[s.rssiText, { color: signal.color }]}>{rssi} dBm</Text>
          <Text style={s.distText}>{getDist(rssi)}</Text>
        </Animated.View>
      </View>

      {/* Estado */}
      <Text style={[s.stateLabel, { color: signal.color }]}>{signal.label}</Text>

      {/* Barras individuales WiFi / BLE */}
      {scanning && (wifiRssi !== null || bleRssi !== null) && (
        <View style={s.dualBar}>
          {wifiRssi !== null && (
            <View style={s.barItem}>
              <Text style={s.barIcon}>📶</Text>
              <Text style={s.barVal}>{wifiRssi} dBm</Text>
            </View>
          )}
          {bleRssi !== null && (
            <View style={s.barItem}>
              <Text style={s.barIcon}>📡</Text>
              <Text style={s.barVal}>{bleRssi} dBm</Text>
            </View>
          )}
          {challengeFlag && (
            <View style={s.barItem}>
              <Text style={[s.barVal, { color: '#e53935' }]}>⚡ CHALLENGE</Text>
            </View>
          )}
        </View>
      )}

      {/* Fase CLOSE */}
      {scanPhase === 'close' && (
        <View style={[s.alert, { borderColor: '#ff6f00' }]}>
          <Text style={s.alertTitle}>⚡ BALIZA A ~1 METRO</Text>
          <Text style={s.alertSub}>Conecta WiFi "<Text style={{ color: '#ff6f00' }}>{BEACON_WIFI_SSID}</Text>"</Text>
          <TouchableOpacity style={s.alertBtn} onPress={() => Linking.openSettings()}>
            <Text style={s.alertBtnTxt}>Abrir Configuración WiFi →</Text>
          </TouchableOpacity>
        </View>
      )}

      {/* Fase CHALLENGE */}
      {scanPhase === 'challenge' && (
        <View style={[s.alert, { borderColor: '#00c853' }]}>
          <Text style={[s.alertTitle, { color: '#00c853' }]}>⚠ INFILTRACIÓN DISPONIBLE</Text>
          <Text style={s.alertSub}>WiFi conectado. Ejecuta la misión.</Text>
          <TouchableOpacity style={[s.alertBtn, { backgroundColor: '#00c85322', borderColor: '#00c853' }]}
            onPress={() => Linking.openURL(CHALLENGE_URL)}>
            <Text style={[s.alertBtnTxt, { color: '#00c853' }]}>▶ INICIAR MISIÓN</Text>
          </TouchableOpacity>
        </View>
      )}

      {/* Botón principal */}
      <View style={s.btnRow}>
        {!scanning
          ? <TouchableOpacity style={s.btnStart} onPress={startScan}>
              <Text style={s.btnTxt}>📡 BUSCAR BALIZA</Text>
            </TouchableOpacity>
          : <TouchableOpacity style={s.btnStop} onPress={stopAll}>
              <Text style={[s.btnTxt, { color: '#e53935' }]}>■ DETENER</Text>
            </TouchableOpacity>
        }
      </View>

      {/* Terminal de log */}
      <ScrollView style={s.term}>
        {log.map((l, i) => <Text key={i} style={s.logLine}>{l}</Text>)}
      </ScrollView>
    </SafeAreaView>
  );
}

// ── ESTILOS ───────────────────────────────────────────────
const FONT = Platform.OS === 'ios' ? 'Courier New' : 'monospace';
const s = StyleSheet.create({
  safe:       { flex:1, backgroundColor:'#07090f' },
  header:     { padding:12, borderBottomWidth:1, borderColor:'#1e3a5f',
                flexDirection:'row', justifyContent:'space-between', alignItems:'center' },
  logo:       { color:'#00b4d8', fontFamily:FONT, fontSize:12, letterSpacing:2 },
  logoRed:    { color:'#e53935' },
  headerSub:  { color:'#334', fontSize:10, fontFamily:FONT },
  circleWrap: { alignItems:'center', marginTop:20, marginBottom:12 },
  circle:     { width:160, height:160, borderRadius:80, borderWidth:3,
                alignItems:'center', justifyContent:'center' },
  rssiText:   { fontSize:34, fontWeight:'bold', fontFamily:FONT },
  distText:   { color:'#8fa8bf', fontSize:12, marginTop:4, fontFamily:FONT },
  stateLabel: { textAlign:'center', fontSize:18, fontWeight:'bold',
                letterSpacing:3, marginBottom:8, fontFamily:FONT },
  dualBar:    { flexDirection:'row', justifyContent:'center', gap:20,
                marginBottom:8, flexWrap:'wrap' },
  barItem:    { alignItems:'center' },
  barIcon:    { fontSize:16 },
  barVal:     { color:'#8fa8bf', fontSize:11, fontFamily:FONT },
  alert:      { marginHorizontal:16, borderWidth:2, borderRadius:8,
                padding:14, marginBottom:8, backgroundColor:'#07090f' },
  alertTitle: { color:'#ff6f00', fontWeight:'bold', fontSize:13,
                letterSpacing:2, textAlign:'center', marginBottom:6, fontFamily:FONT },
  alertSub:   { color:'#8fa8bf', fontSize:12, textAlign:'center', fontFamily:FONT },
  alertBtn:   { marginTop:10, borderWidth:1, borderColor:'#ff6f00',
                borderRadius:6, padding:12, alignItems:'center', backgroundColor:'#ff6f0022' },
  alertBtnTxt:{ color:'#ff6f00', fontWeight:'bold', fontSize:13, fontFamily:FONT },
  btnRow:     { marginHorizontal:16, marginBottom:8 },
  btnStart:   { backgroundColor:'#00b4d8', borderRadius:8, padding:16, alignItems:'center' },
  btnStop:    { backgroundColor:'#0d1520', borderWidth:1, borderColor:'#e53935',
                borderRadius:8, padding:16, alignItems:'center' },
  btnTxt:     { color:'#07090f', fontWeight:'bold', fontSize:15, fontFamily:FONT },
  term:       { flex:1, marginHorizontal:16, backgroundColor:'#05080c',
                borderRadius:8, padding:8, borderWidth:1, borderColor:'#1e3a5f' },
  logLine:    { color:'#1e3a5f', fontSize:10, lineHeight:18, fontFamily:FONT },
});
