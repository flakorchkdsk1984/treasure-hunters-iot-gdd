/**
 * TREASURE HUNTERS IoT — THLS Scanner App
 * React Native + Expo + react-native-ble-plx
 *
 * Flujo:
 *  1. Escanea BLE pasivo (sin conectar a nada)
 *  2. Muestra RSSI + estado en tiempo real
 *  3. Al llegar a < 1m (RSSI ≥ -47): vibra + avisa conectar WiFi
 *  4. Al conectar WiFi "THLS-C001": lanza minijuego en WebView
 */

import React, { useState, useEffect, useRef, useCallback } from 'react';
import {
  View, Text, StyleSheet, TouchableOpacity, Vibration,
  Animated, Easing, Platform, Alert, Linking, ScrollView,
  SafeAreaView,
} from 'react-native';
import { BleManager, State } from 'react-native-ble-plx';
import * as Network from 'expo-network';
import { StatusBar } from 'expo-status-bar';

// ── CONFIG ────────────────────────────────────────────────
const BEACON_PREFIX   = 'THLS-';
const BEACON_WIFI     = 'THLS-C001';
const CHALLENGE_URL   = 'http://192.168.4.1';
const SCAN_INTERVAL   = 800;   // ms entre updates de RSSI
const CLOSE_THRESHOLD = -47;   // dBm = ~1 metro

// ── ESTADOS DE SEÑAL ─────────────────────────────────────
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

function getDist(rssi, txPower = -59) {
  const d = Math.pow(10, (txPower - rssi) / 25);
  if (d < 1)   return '< 1 m';
  if (d > 500) return '> 500 m';
  return d.toFixed(0) + ' m';
}

// ── SMOOTH RSSI ───────────────────────────────────────────
function useSmoothedRssi(windowSize = 5) {
  const history = useRef([]);
  return useCallback((raw) => {
    history.current.push(raw);
    if (history.current.length > windowSize) history.current.shift();
    return Math.round(
      history.current.reduce((a, b) => a + b, 0) / history.current.length
    );
  }, [windowSize]);
}

// ── COMPONENTE PRINCIPAL ──────────────────────────────────
export default function App() {
  const [bleState, setBleState]       = useState('unknown');
  const [scanning, setScanning]       = useState(false);
  const [rssi, setRssi]               = useState(-100);
  const [beaconName, setBeaconName]   = useState(null);
  const [signal, setSignal]           = useState(STATES[0]);
  const [log, setLog]                 = useState([]);
  const [phase, setPhase]             = useState('scan'); // scan | close | challenge
  const [wifiConnected, setWifiConnected] = useState(false);

  const bleManager  = useRef(null);
  const scanRef     = useRef(null);
  const pulseAnim   = useRef(new Animated.Value(1)).current;
  const pulseLoop   = useRef(null);
  const smoothRssi  = useSmoothedRssi(5);

  // Init BLE Manager
  useEffect(() => {
    bleManager.current = new BleManager();
    const sub = bleManager.current.onStateChange(state => {
      setBleState(state);
      addLog(`BLE: ${state}`);
      if (state === State.PoweredOn) addLog('BLE listo — presiona DETECTAR');
    }, true);
    return () => {
      sub.remove();
      if (scanRef.current) bleManager.current.stopDeviceScan();
      bleManager.current.destroy();
    };
  }, []);

  // Pulso animado — velocidad según estado
  useEffect(() => {
    if (pulseLoop.current) pulseLoop.current.stop();
    if (!signal.pulse) {
      pulseAnim.setValue(1);
      return;
    }
    const dur = signal.pulse * 1000;
    pulseLoop.current = Animated.loop(
      Animated.sequence([
        Animated.timing(pulseAnim, { toValue: 1.35, duration: dur / 2, easing: Easing.inOut(Easing.ease), useNativeDriver: true }),
        Animated.timing(pulseAnim, { toValue: 1.0,  duration: dur / 2, easing: Easing.inOut(Easing.ease), useNativeDriver: true }),
      ])
    );
    pulseLoop.current.start();
  }, [signal.id]);

  // Monitor WiFi cuando fase es 'close'
  useEffect(() => {
    if (phase !== 'close') return;
    const iv = setInterval(async () => {
      const net = await Network.getNetworkStateAsync();
      if (net.type === Network.NetworkStateType.WIFI) {
        const ssid = await Network.getIpAddressAsync();
        // En iOS no podemos leer SSID sin entitlement especial,
        // así que chequeamos que hay WiFi y asumimos es THLS-C001
        setWifiConnected(true);
        setPhase('challenge');
        addLog('✓ WiFi conectado — cargando challenge...');
        Vibration.vibrate([200, 100, 200, 100, 400]);
        clearInterval(iv);
      }
    }, 1500);
    return () => clearInterval(iv);
  }, [phase]);

  function addLog(msg) {
    const ts = new Date().toLocaleTimeString('es', { hour12: false });
    setLog(prev => [`[${ts}] ${msg}`, ...prev].slice(0, 40));
  }

  function startScan() {
    if (bleState !== State.PoweredOn) {
      Alert.alert('Bluetooth', 'Activa el Bluetooth del teléfono primero.');
      return;
    }
    setScanning(true);
    addLog('Escaneando balizas THLS...');
    bleManager.current.startDeviceScan(null, { allowDuplicates: true }, (err, device) => {
      if (err) { addLog('Error BLE: ' + err.message); return; }
      if (!device?.name?.startsWith(BEACON_PREFIX)) return;

      const smooth = smoothRssi(device.rssi ?? -100);
      const st = getState(smooth);
      setRssi(smooth);
      setSignal(st);
      setBeaconName(device.name);
      addLog(`${device.name} → ${smooth} dBm | ${st.label}`);

      // ¿Llegamos a < 1 metro?
      if (smooth >= CLOSE_THRESHOLD && phase === 'scan') {
        setPhase('close');
        Vibration.vibrate([100, 80, 100, 80, 300]);
        addLog('⚡ BALIZA CERCA — Conecta WiFi "' + BEACON_WIFI + '"');
      }
    });
  }

  function stopScan() {
    bleManager.current?.stopDeviceScan();
    setScanning(false);
    setPhase('scan');
    setWifiConnected(false);
    addLog('Escaneo detenido');
  }

  function openChallenge() {
    Linking.openURL(CHALLENGE_URL);
  }

  // ── RENDER ────────────────────────────────────────────
  const circleStyle = {
    backgroundColor: signal.color + '33',
    borderColor: signal.color,
    transform: [{ scale: pulseAnim }],
  };

  return (
    <SafeAreaView style={styles.safe}>
      <StatusBar style="light" />

      {/* Header */}
      <View style={styles.header}>
        <Text style={styles.logo}>TREASURE <Text style={styles.logoRed}>HUNTERS</Text> IoT</Text>
        <Text style={styles.headerSub}>{beaconName || 'SIN BALIZA'}</Text>
      </View>

      {/* Círculo RSSI pulsante */}
      <View style={styles.circleWrap}>
        <Animated.View style={[styles.circle, circleStyle]}>
          <Text style={[styles.rssiText, { color: signal.color }]}>{rssi} dBm</Text>
          <Text style={styles.distText}>{getDist(rssi)}</Text>
        </Animated.View>
      </View>

      {/* Estado */}
      <Text style={[styles.stateLabel, { color: signal.color }]}>{signal.label}</Text>

      {/* Fase: CERCA */}
      {phase === 'close' && !wifiConnected && (
        <View style={[styles.alertBox, { borderColor: '#ff6f00' }]}>
          <Text style={styles.alertTitle}>⚡ BALIZA CERCA</Text>
          <Text style={styles.alertSub}>
            Conecta WiFi "<Text style={{ color: '#ff6f00' }}>{BEACON_WIFI}</Text>" para iniciar misión
          </Text>
          <TouchableOpacity style={styles.wifiBtn} onPress={() => Linking.openSettings()}>
            <Text style={styles.wifiBtnText}>Abrir Configuración WiFi →</Text>
          </TouchableOpacity>
        </View>
      )}

      {/* Fase: CHALLENGE */}
      {phase === 'challenge' && (
        <View style={[styles.alertBox, { borderColor: '#00c853' }]}>
          <Text style={[styles.alertTitle, { color: '#00c853' }]}>⚠ INFILTRACIÓN DISPONIBLE</Text>
          <Text style={styles.alertSub}>WiFi conectado. Abre el minijuego.</Text>
          <TouchableOpacity style={[styles.wifiBtn, { backgroundColor: '#00c85322', borderColor: '#00c853' }]}
            onPress={openChallenge}>
            <Text style={[styles.wifiBtnText, { color: '#00c853' }]}>▶ INICIAR MISIÓN</Text>
          </TouchableOpacity>
        </View>
      )}

      {/* Botones */}
      <View style={styles.btnRow}>
        {!scanning ? (
          <TouchableOpacity style={styles.btnStart} onPress={startScan}>
            <Text style={styles.btnText}>📡 DETECTAR BALIZA</Text>
          </TouchableOpacity>
        ) : (
          <TouchableOpacity style={styles.btnStop} onPress={stopScan}>
            <Text style={styles.btnText}>■ DETENER</Text>
          </TouchableOpacity>
        )}
      </View>

      {/* Log terminal */}
      <ScrollView style={styles.term} contentContainerStyle={{ paddingBottom: 8 }}>
        {log.map((l, i) => (
          <Text key={i} style={styles.logLine}>{l}</Text>
        ))}
      </ScrollView>
    </SafeAreaView>
  );
}

// ── ESTILOS ───────────────────────────────────────────────
const styles = StyleSheet.create({
  safe:        { flex: 1, backgroundColor: '#07090f' },
  header:      { padding: 12, borderBottomWidth: 1, borderColor: '#1e3a5f',
                 flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center' },
  logo:        { color: '#00b4d8', fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace',
                 fontSize: 12, letterSpacing: 2 },
  logoRed:     { color: '#e53935' },
  headerSub:   { color: '#334', fontSize: 10,
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
  circleWrap:  { alignItems: 'center', marginTop: 24, marginBottom: 16 },
  circle:      { width: 170, height: 170, borderRadius: 85, borderWidth: 3,
                 alignItems: 'center', justifyContent: 'center' },
  rssiText:    { fontSize: 36, fontWeight: 'bold',
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
  distText:    { color: '#8fa8bf', fontSize: 12, marginTop: 4,
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
  stateLabel:  { textAlign: 'center', fontSize: 20, fontWeight: 'bold',
                 letterSpacing: 3, marginBottom: 12,
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
  alertBox:    { marginHorizontal: 16, borderWidth: 2, borderRadius: 8,
                 padding: 14, marginBottom: 10, backgroundColor: '#07090f' },
  alertTitle:  { color: '#ff6f00', fontWeight: 'bold', fontSize: 14,
                 letterSpacing: 2, textAlign: 'center', marginBottom: 6,
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
  alertSub:    { color: '#8fa8bf', fontSize: 12, textAlign: 'center',
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
  wifiBtn:     { marginTop: 10, borderWidth: 1, borderColor: '#ff6f00',
                 borderRadius: 6, padding: 12, alignItems: 'center',
                 backgroundColor: '#ff6f0022' },
  wifiBtnText: { color: '#ff6f00', fontWeight: 'bold', fontSize: 13,
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
  btnRow:      { marginHorizontal: 16, marginBottom: 8 },
  btnStart:    { backgroundColor: '#00b4d8', borderRadius: 8, padding: 16, alignItems: 'center' },
  btnStop:     { backgroundColor: '#e5393522', borderWidth: 1, borderColor: '#e53935',
                 borderRadius: 8, padding: 16, alignItems: 'center' },
  btnText:     { color: '#07090f', fontWeight: 'bold', fontSize: 15,
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
  term:        { flex: 1, marginHorizontal: 16, marginTop: 4,
                 backgroundColor: '#05080c', borderRadius: 8, padding: 8,
                 borderWidth: 1, borderColor: '#1e3a5f' },
  logLine:     { color: '#1e3a5f', fontSize: 10, lineHeight: 18,
                 fontFamily: Platform.OS === 'ios' ? 'Courier New' : 'monospace' },
});

    <View style={styles.container}>
      <Text>Open up App.js to start working on your app!</Text>
      <StatusBar style="auto" />
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },
});
