/**
 * THLS Scanner — DEBUG VERSION
 * Muestra logs detallados en pantalla para identificar el error
 */
import React, { useState, useEffect, useRef } from 'react';
import {
  View, Text, StyleSheet, TouchableOpacity,
  ScrollView, SafeAreaView, Platform,
} from 'react-native';
import { StatusBar } from 'expo-status-bar';

// ── Intentar cargar BLE (puede fallar en Expo Go sin dev build) ──
let BleManager = null;
let BleState = null;
let bleLoadError = null;
try {
  const ble = require('react-native-ble-plx');
  BleManager = ble.BleManager;
  BleState = ble.State;
  bleLoadError = null;
} catch (e) {
  bleLoadError = e.message;
}

export default function App() {
  const [logs, setLogs]       = useState([]);
  const [bleStatus, setBleStatus] = useState('no iniciado');
  const [scanning, setScanning]   = useState(false);
  const [devices, setDevices]     = useState([]);
  const mgr = useRef(null);

  function addLog(msg, color = '#8fa8bf') {
    const ts = new Date().toLocaleTimeString('es', { hour12: false });
    setLogs(prev => [{ ts, msg, color }, ...prev].slice(0, 60));
    console.log(`[THLS ${ts}] ${msg}`);
  }

  // ── Init ──
  useEffect(() => {
    addLog(`Plataforma: ${Platform.OS} ${Platform.Version}`);
    addLog(`BLE módulo: ${bleLoadError ? '✗ ' + bleLoadError : '✓ cargado'}`);

    if (!BleManager) {
      addLog('⚠ react-native-ble-plx no disponible', '#f39c12');
      addLog('Necesitás un Expo Dev Build (no Expo Go)', '#f39c12');
      addLog('Instrucciones: ver README.md', '#f39c12');
      setBleStatus('no disponible — dev build requerido');
      return;
    }

    try {
      addLog('Creando BleManager...');
      mgr.current = new BleManager();
      addLog('BleManager creado ✓', '#2ecc71');

      const sub = mgr.current.onStateChange(state => {
        addLog(`BLE state → ${state}`);
        setBleStatus(state);
      }, true);

      return () => {
        sub.remove();
        mgr.current?.stopDeviceScan();
        mgr.current?.destroy();
      };
    } catch (e) {
      addLog('Error init BLE: ' + e.message, '#e74c3c');
      setBleStatus('error: ' + e.message);
    }
  }, []);

  function startScan() {
    if (!mgr.current) {
      addLog('BleManager no disponible', '#e74c3c');
      return;
    }
    setDevices([]);
    setScanning(true);
    addLog('Iniciando scan BLE...');

    try {
      mgr.current.startDeviceScan(null, { allowDuplicates: false }, (err, device) => {
        if (err) {
          addLog('Scan error: ' + err.message, '#e74c3c');
          setScanning(false);
          return;
        }
        if (!device?.name) return;
        addLog(`→ ${device.name} | ${device.rssi} dBm | ${device.id?.slice(0,8)}`);
        setDevices(prev => {
          const exists = prev.find(d => d.id === device.id);
          if (exists) return prev.map(d => d.id === device.id ? { ...d, rssi: device.rssi } : d);
          return [...prev, { id: device.id, name: device.name, rssi: device.rssi }];
        });
      });

      setTimeout(() => {
        mgr.current?.stopDeviceScan();
        setScanning(false);
        addLog('Scan completado (10s)', '#2ecc71');
      }, 10000);
    } catch (e) {
      addLog('Error startScan: ' + e.message, '#e74c3c');
      setScanning(false);
    }
  }

  function stopScan() {
    mgr.current?.stopDeviceScan();
    setScanning(false);
    addLog('Scan detenido manualmente');
  }

  return (
    <SafeAreaView style={s.safe}>
      <StatusBar style="light" />

      <View style={s.header}>
        <Text style={s.title}>THLS — DEBUG</Text>
        <Text style={s.sub}>BLE: {bleStatus}</Text>
      </View>

      {/* Dispositivos encontrados */}
      {devices.length > 0 && (
        <View style={s.devList}>
          <Text style={s.devTitle}>DISPOSITIVOS ({devices.length})</Text>
          {devices.map(d => (
            <Text key={d.id} style={[s.devItem, d.name.startsWith('THLS') && s.devTHLS]}>
              {d.name.startsWith('THLS') ? '🎯 ' : '· '}{d.name} → {d.rssi} dBm
            </Text>
          ))}
        </View>
      )}

      {/* Botones */}
      <View style={s.btnRow}>
        {!scanning
          ? <TouchableOpacity style={s.btnOn} onPress={startScan}>
              <Text style={s.btnTxt}>▶ SCAN BLE (10s)</Text>
            </TouchableOpacity>
          : <TouchableOpacity style={s.btnOff} onPress={stopScan}>
              <Text style={[s.btnTxt, { color: '#e53935' }]}>■ DETENER</Text>
            </TouchableOpacity>
        }
      </View>

      {/* Log */}
      <Text style={s.logTitle}>LOG DE DEBUG</Text>
      <ScrollView style={s.logBox}>
        {logs.map((l, i) => (
          <Text key={i} style={[s.logLine, { color: l.color }]}>
            [{l.ts}] {l.msg}
          </Text>
        ))}
      </ScrollView>
    </SafeAreaView>
  );
}

const FONT = Platform.OS === 'ios' ? 'Courier New' : 'monospace';
const s = StyleSheet.create({
  safe:     { flex: 1, backgroundColor: '#07090f' },
  header:   { padding: 12, borderBottomWidth: 1, borderColor: '#1e3a5f' },
  title:    { color: '#00b4d8', fontFamily: FONT, fontSize: 14, fontWeight: 'bold', letterSpacing: 3 },
  sub:      { color: '#555', fontFamily: FONT, fontSize: 11, marginTop: 2 },
  devList:  { margin: 12, padding: 10, borderRadius: 8, borderWidth: 1, borderColor: '#1e3a5f', backgroundColor: '#0d1117' },
  devTitle: { color: '#334', fontFamily: FONT, fontSize: 10, letterSpacing: 2, marginBottom: 6 },
  devItem:  { color: '#8fa8bf', fontFamily: FONT, fontSize: 12, lineHeight: 20 },
  devTHLS:  { color: '#00c853', fontWeight: 'bold' },
  btnRow:   { marginHorizontal: 12, marginVertical: 8 },
  btnOn:    { backgroundColor: '#00b4d8', borderRadius: 8, padding: 16, alignItems: 'center' },
  btnOff:   { borderWidth: 1, borderColor: '#e53935', borderRadius: 8, padding: 16, alignItems: 'center', backgroundColor: '#0d1520' },
  btnTxt:   { color: '#07090f', fontWeight: 'bold', fontSize: 15, fontFamily: FONT },
  logTitle: { color: '#334', fontFamily: FONT, fontSize: 9, letterSpacing: 2, marginLeft: 12, marginBottom: 4 },
  logBox:   { flex: 1, marginHorizontal: 12, marginBottom: 8, backgroundColor: '#05080c', borderRadius: 8, padding: 8, borderWidth: 1, borderColor: '#1e3a5f' },
  logLine:  { fontFamily: FONT, fontSize: 10, lineHeight: 17 },
});
