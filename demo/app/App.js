/**
 * THLS Scanner — POC v2.1
 *
 * Flujo dual-radio:
 *  1. WiFi RSSI (pasivo, sin conectar) → búsqueda largo alcance
 *  2. BLE RSSI                         → última milla, precisión cercana
 *  3. WiFi conectado                   → solo para el minijuego
 *
 * MODO SIM: recorre los 9 estados automáticamente para testear en Expo Go.
 */
import React, { useState, useEffect, useRef, useCallback } from 'react';
import {
  View, Text, StyleSheet, TouchableOpacity, Animated,
  Linking, SafeAreaView, Platform, Alert,
} from 'react-native';
import { StatusBar } from 'expo-status-bar';

// ── Módulos opcionales (requieren dev build, fallan silenciosamente en Expo Go) ──
let BleManager  = null;
let WifiManager = null;
try { BleManager  = require('react-native-ble-plx').BleManager;       } catch (_) {}
try { WifiManager = require('react-native-wifi-reborn').default;       } catch (_) {}

// ─────────────────────────────────────────────
//  TABLA DE ESTADOS
// ─────────────────────────────────────────────
const STATES = [
  { id: 'none',    label: 'SIN SEÑAL',        min: -Infinity, max: -94, color: '#37474f', pulse: null,  mode: 'wifi' },
  { id: 'polar',   label: 'POLAR',             min: -94,       max: -84, color: '#0d47a1', pulse: 3500, mode: 'wifi' },
  { id: 'cold',    label: 'FRÍO',              min: -84,       max: -75, color: '#1976d2', pulse: 3000, mode: 'wifi' },
  { id: 'cool',    label: 'FRESCO',            min: -75,       max: -67, color: '#00838f', pulse: 2500, mode: 'wifi' },
  { id: 'warm',    label: 'TIBIO',             min: -67,       max: -60, color: '#7b1fa2', pulse: 2000, mode: 'wifi' },
  { id: 'hot',     label: 'CALIENTE',          min: -60,       max: -53, color: '#c2185b', pulse: 1200, mode: 'ble'  },
  { id: 'vhot',    label: 'MUY CALIENTE',      min: -53,       max: -47, color: '#e64a19', pulse: 700,  mode: 'ble'  },
  { id: 'burning', label: 'ARDIENDO',          min: -47,       max: -41, color: '#ff6f00', pulse: 350,  mode: 'ble'  },
  { id: 'found',   label: 'BALIZA LOCALIZADA', min: -41, max: Infinity,  color: '#00c853', pulse: 150,  mode: 'ble'  },
];

const BLE_TAKEOVER_RSSI = -60;   // BLE toma el mando a partir de aquí

function rssiToState(rssi) {
  return STATES.find(s => rssi >= s.min && rssi < s.max) ?? STATES[0];
}

// ─────────────────────────────────────────────
//  SIMULACIÓN
// ─────────────────────────────────────────────
// [wifiRssi, bleRssi, wifiConnected]
const SIM_STEPS = [
  [-97, null,  false],  // SIN SEÑAL
  [-91, null,  false],  // POLAR
  [-81, null,  false],  // FRÍO
  [-72, null,  false],  // FRESCO
  [-63, null,  false],  // TIBIO
  [-58, -62,   false],  // CALIENTE  — BLE toma el mando
  [-55, -51,   false],  // MUY CALIENTE
  [-53, -45,   false],  // ARDIENDO
  [-51, -38,   false],  // BALIZA LOCALIZADA — banner WiFi
  [-51, -38,   false],  // (pausa)
  [-51, -38,   true ],  // WiFi conectado → botón MISIÓN
  [-51, -38,   true ],  // (pausa)
];
const SIM_MS = 1800;

// ─────────────────────────────────────────────
//  COMPONENTE: CÍRCULO PULSANTE
// ─────────────────────────────────────────────
function PulsingCircle({ color, pulseMs, rssi, radioMode }) {
  const scale   = useRef(new Animated.Value(1)).current;
  const opacity = useRef(new Animated.Value(0.85)).current;
  const animRef = useRef(null);

  useEffect(() => {
    animRef.current?.stop();
    if (!pulseMs) { scale.setValue(1); opacity.setValue(0.25); return; }
    const loop = Animated.loop(
      Animated.sequence([
        Animated.parallel([
          Animated.timing(scale,   { toValue: 1.2,  duration: pulseMs / 2, useNativeDriver: true }),
          Animated.timing(opacity, { toValue: 1,    duration: pulseMs / 2, useNativeDriver: true }),
        ]),
        Animated.parallel([
          Animated.timing(scale,   { toValue: 1,    duration: pulseMs / 2, useNativeDriver: true }),
          Animated.timing(opacity, { toValue: 0.75, duration: pulseMs / 2, useNativeDriver: true }),
        ]),
      ])
    );
    animRef.current = loop;
    loop.start();
    return () => loop.stop();
  }, [pulseMs, color]);

  return (
    <Animated.View style={[s.circle, { backgroundColor: color, transform: [{ scale }], opacity }]}>
      <Text style={s.circleRssi}>{rssi !== null ? `${rssi}` : '---'}</Text>
      <Text style={s.circleUnit}>{rssi !== null ? 'dBm' : ''}</Text>
      <Text style={s.circleMode}>{rssi !== null ? (radioMode === 'ble' ? '🔵' : '📡') : ''}</Text>
    </Animated.View>
  );
}

// ─────────────────────────────────────────────
//  APP
// ─────────────────────────────────────────────
export default function App() {
  const [active, setActive]               = useState(false);
  const [simMode, setSimMode]             = useState(false);
  const [wifiRssi, setWifiRssi]           = useState(null);
  const [bleRssi, setBleRssi]             = useState(null);
  const [beaconName, setBeaconName]       = useState(null);
  const [wifiConnected, setWifiConnected] = useState(false);

  const mgr        = useRef(null);
  const wifiTimer  = useRef(null);
  const simTimer   = useRef(null);
  const simStep    = useRef(0);

  // señal efectiva: BLE manda en última milla
  const useBle         = bleRssi !== null && bleRssi >= BLE_TAKEOVER_RSSI;
  const effectiveRssi  = useBle ? bleRssi : wifiRssi;
  const radioMode      = useBle ? 'ble' : 'wifi';
  const state          = effectiveRssi !== null ? rssiToState(effectiveRssi) : STATES[0];
  const isFound        = state.id === 'found';

  // ── Simulación ──
  useEffect(() => {
    clearInterval(simTimer.current);
    if (!simMode) return;

    // reset
    setActive(false);
    setWifiRssi(null); setBleRssi(null);
    setBeaconName(null); setWifiConnected(false);
    simStep.current = 0;

    simTimer.current = setInterval(() => {
      const i = simStep.current % SIM_STEPS.length;
      const [w, b, wConn] = SIM_STEPS[i];
      setWifiRssi(w);
      setBleRssi(b);
      if (b !== null) setBeaconName('THLS-C001');
      setWifiConnected(wConn);
      simStep.current += 1;
    }, SIM_MS);

    return () => clearInterval(simTimer.current);
  }, [simMode]);

  // ── Scan WiFi pasivo ──
  const scanWifi = useCallback(async () => {
    if (!WifiManager) return;
    try {
      const nets = await WifiManager.loadWifiList();
      const thls = nets.find(n => n.SSID === 'THLS-C001');
      setWifiRssi(thls ? (thls.level ?? thls.RSSI ?? null) : null);
    } catch (_) {}
  }, []);

  const checkWifiConn = useCallback(async () => {
    if (!WifiManager) return;
    try {
      const ssid = await WifiManager.getCurrentWifiSSID();
      setWifiConnected(ssid === 'THLS-C001');
    } catch (_) { setWifiConnected(false); }
  }, []);

  // ── Scan real (BLE + WiFi) ──
  useEffect(() => {
    if (!active) {
      clearInterval(wifiTimer.current);
      mgr.current?.stopDeviceScan();
      setWifiRssi(null); setBleRssi(null);
      setBeaconName(null);
      return;
    }
    if (WifiManager) {
      scanWifi();
      wifiTimer.current = setInterval(() => { scanWifi(); checkWifiConn(); }, 4000);
    }
    if (BleManager) {
      if (!mgr.current) mgr.current = new BleManager();
      mgr.current.startDeviceScan(null, { allowDuplicates: true }, (err, device) => {
        if (err || !device?.name?.startsWith('THLS')) return;
        setBleRssi(device.rssi ?? null);
        setBeaconName(device.name);
      });
    }
    return () => {
      clearInterval(wifiTimer.current);
      mgr.current?.stopDeviceScan();
    };
  }, [active]);

  function toggle() {
    if (!BleManager && !WifiManager) {
      Alert.alert('Dev Build requerido',
        'BLE y WiFi scan no funcionan en Expo Go.\nUsá el botón MODO SIMULACIÓN para testear la UI.');
      return;
    }
    setSimMode(false);
    setActive(v => !v);
  }

  function toggleSim() {
    setActive(false);
    setSimMode(v => !v);
  }

  function openMission() {
    Linking.openURL('http://192.168.4.1').catch(() =>
      Alert.alert('Error', 'Verificá estar conectado a la red WiFi THLS-C001')
    );
  }

  // ── Render ──
  return (
    <SafeAreaView style={s.safe}>
      <StatusBar style="light" />

      <View style={s.header}>
        <Text style={s.title}>TREASURE HUNTERS</Text>
        <Text style={s.subtitle}>THE LAST SIGNAL</Text>
      </View>

      <View style={s.radioBar}>
        <View style={[s.radioChip, !useBle && (active || simMode) && wifiRssi && s.radioActive]}>
          <Text style={s.radioTxt}>📡 WiFi  {wifiRssi !== null ? `${wifiRssi} dBm` : '---'}</Text>
        </View>
        <View style={[s.radioChip, useBle && s.radioActive]}>
          <Text style={s.radioTxt}>🔵 BLE  {bleRssi !== null ? `${bleRssi} dBm` : '---'}</Text>
        </View>
      </View>

      <View style={s.circleWrap}>
        <PulsingCircle color={state.color} pulseMs={state.pulse} rssi={effectiveRssi} radioMode={radioMode} />
        <Text style={[s.stateLabel, { color: state.color }]}>{state.label}</Text>
        {beaconName && <Text style={s.beaconName}>🎯 {beaconName}</Text>}
        {(active || simMode) && !useBle && <Text style={s.hint}>acercate — BLE activo a {'<'}10 m</Text>}
        {useBle && !isFound && <Text style={s.hint}>última milla — seguí acercándote</Text>}
      </View>

      {isFound && !wifiConnected && (
        <View style={[s.banner, { borderColor: '#00c853' }]}>
          <Text style={s.bannerTitle}>¡BALIZA LOCALIZADA!</Text>
          <Text style={s.bannerSub}>
            Conectate al WiFi <Text style={s.bannerHL}>THLS-C001</Text>
            {'\n'}(sin contraseña) para iniciar la misión
          </Text>
        </View>
      )}

      {wifiConnected && (
        <TouchableOpacity style={s.missionBtn} onPress={openMission}>
          <Text style={s.missionBtnTxt}>⚡  INICIAR MISIÓN</Text>
        </TouchableOpacity>
      )}

      <View style={s.btnRow}>
        {!simMode && (
          <TouchableOpacity style={active ? s.btnStop : s.btnStart} onPress={toggle}>
            <Text style={[s.btnTxt, active && { color: '#ef5350' }]}>
              {active ? '■  DETENER BÚSQUEDA' : '▶  INICIAR BÚSQUEDA'}
            </Text>
          </TouchableOpacity>
        )}
        <TouchableOpacity style={[s.btnSim, simMode && s.btnSimActive]} onPress={toggleSim}>
          <Text style={[s.btnSimTxt, simMode && { color: '#ffca28' }]}>
            {simMode ? '⏹  DETENER SIMULACIÓN' : '🎮  MODO SIMULACIÓN'}
          </Text>
        </TouchableOpacity>
      </View>

      <Text style={s.footer}>
        {simMode
          ? '🎮 simulación — recorriendo los 9 estados'
          : !BleManager && !WifiManager
            ? '⚠ Dev Build requerido para BLE y WiFi scan'
            : active
              ? useBle ? '● BLE — última milla' : '● WiFi — largo alcance'
              : '○ búsqueda detenida'}
      </Text>
    </SafeAreaView>
  );
}

// ─────────────────────────────────────────────
//  ESTILOS
// ─────────────────────────────────────────────
const FONT = Platform.OS === 'ios' ? 'Courier New' : 'monospace';

const s = StyleSheet.create({
  safe:          { flex: 1, backgroundColor: '#07090f' },

  header:        { alignItems: 'center', paddingTop: 14, paddingBottom: 10, borderBottomWidth: 1, borderColor: '#1e3a5f' },
  title:         { color: '#00b4d8', fontFamily: FONT, fontSize: 15, fontWeight: 'bold', letterSpacing: 4 },
  subtitle:      { color: '#1e3a5f', fontFamily: FONT, fontSize: 10, letterSpacing: 6, marginTop: 2 },

  radioBar:      { flexDirection: 'row', justifyContent: 'center', gap: 10, paddingVertical: 8 },
  radioChip:     { paddingHorizontal: 12, paddingVertical: 5, borderRadius: 20, borderWidth: 1, borderColor: '#1e3a5f', backgroundColor: '#0d1117' },
  radioActive:   { borderColor: '#00b4d8', backgroundColor: '#0a1e2e' },
  radioTxt:      { color: '#4a7a9b', fontFamily: FONT, fontSize: 10 },

  circleWrap:    { flex: 1, alignItems: 'center', justifyContent: 'center' },
  circle:        { width: 190, height: 190, borderRadius: 95, alignItems: 'center', justifyContent: 'center', elevation: 10 },
  circleRssi:    { color: '#fff', fontFamily: FONT, fontSize: 28, fontWeight: 'bold' },
  circleUnit:    { color: 'rgba(255,255,255,0.7)', fontFamily: FONT, fontSize: 12 },
  circleMode:    { fontSize: 18, marginTop: 4 },
  stateLabel:    { marginTop: 18, fontFamily: FONT, fontSize: 12, fontWeight: 'bold', letterSpacing: 2 },
  beaconName:    { marginTop: 4, color: '#8fa8bf', fontFamily: FONT, fontSize: 11 },
  hint:          { marginTop: 8, color: '#2a4a6a', fontFamily: FONT, fontSize: 10 },

  banner:        { marginHorizontal: 16, marginBottom: 10, padding: 14, borderRadius: 10, borderWidth: 1, backgroundColor: '#071a0f' },
  bannerTitle:   { color: '#00c853', fontFamily: FONT, fontSize: 12, fontWeight: 'bold', letterSpacing: 2, marginBottom: 4 },
  bannerSub:     { color: '#8fa8bf', fontFamily: FONT, fontSize: 11, lineHeight: 18 },
  bannerHL:      { color: '#00b4d8', fontWeight: 'bold' },

  missionBtn:    { marginHorizontal: 16, marginBottom: 10, backgroundColor: '#00c853', borderRadius: 10, padding: 18, alignItems: 'center' },
  missionBtnTxt: { color: '#07090f', fontFamily: FONT, fontSize: 16, fontWeight: 'bold', letterSpacing: 3 },

  btnRow:        { marginHorizontal: 16, marginBottom: 8, gap: 8 },
  btnStart:      { backgroundColor: '#00b4d8', borderRadius: 10, padding: 16, alignItems: 'center' },
  btnStop:       { borderWidth: 1, borderColor: '#ef5350', borderRadius: 10, padding: 16, alignItems: 'center', backgroundColor: '#0d1520' },
  btnTxt:        { color: '#07090f', fontFamily: FONT, fontSize: 14, fontWeight: 'bold', letterSpacing: 2 },
  btnSim:        { borderWidth: 1, borderColor: '#37474f', borderRadius: 10, padding: 13, alignItems: 'center', backgroundColor: '#0d1117' },
  btnSimActive:  { borderColor: '#ffca28', backgroundColor: '#1a1500' },
  btnSimTxt:     { color: '#37474f', fontFamily: FONT, fontSize: 13, fontWeight: 'bold', letterSpacing: 1 },

  footer:        { color: '#2a4a6a', fontFamily: FONT, fontSize: 10, textAlign: 'center', marginBottom: 12 },
});
