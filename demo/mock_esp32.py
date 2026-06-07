#!/usr/bin/env python3
"""
THLS — Mock ESP32 Server
Simula el firmware del ESP32 para probar la PWA en local sin hardware.
Uso: python3 mock_esp32.py
Luego abrir: http://localhost:80
"""
import json, math, time, threading
from http.server import HTTPServer, BaseHTTPRequestHandler
from urllib.parse import urlparse, parse_qs

PORT      = 8080
start_t   = time.time()
challenge = False
slot      = 0
CHALLENGES = [
    {"type":"wire_connect","seed":"84726","diff":"easy"},
    {"type":"qr_scan",     "seed":"QR-A042","diff":"medium"},
    {"type":"morse_decode","seed":"SOS","diff":"hard"},
]

def get_rssi():
    """RSSI simulado: oscila entre -90 y -40 en ciclo de 30 segundos"""
    t = (time.time() - start_t) % 30
    # sube de -90 a -40 en los primeros 15s, baja en los 15s siguientes
    if t < 15:
        return int(-90 + (50 * t / 15))
    else:
        return int(-40 - (50 * (t - 15) / 15))

class Handler(BaseHTTPRequestHandler):
    def log_message(self, fmt, *args):
        print(f"  [{time.strftime('%H:%M:%S')}] {fmt % args}")

    def send_cors(self):
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type")

    def do_OPTIONS(self):
        self.send_response(204)
        self.send_cors()
        self.end_headers()

    def do_GET(self):
        global challenge, slot
        path = urlparse(self.path).path

        if path == "/api/status":
            rssi = get_rssi()
            chal = CHALLENGES[slot]
            body = json.dumps({
                "v": 3, "fw": "3.0.0-mock",
                "id": "C001", "class": "C", "region": "LAT-AR-01",
                "rssi": rssi,
                "challenge": challenge,
                "type": chal["type"],
                "seed": chal["seed"],
                "diff": chal["diff"],
                "timer": 60, "slot": slot,
                "nonce": "A001",
                "bat": 3.72,
                "uptime": int(time.time() - start_t),
                "clients": 1
            })
            self.send_response(200)
            self.send_cors()
            self.send_header("Content-Type", "application/json")
            self.end_headers()
            self.wfile.write(body.encode())

        elif path == "/" or path == "/index.html":
            # Leer y servir el test.html real
            try:
                with open("pwa/test.html", "rb") as f:
                    content = f.read()
                self.send_response(200)
                self.send_cors()
                self.send_header("Content-Type", "text/html; charset=utf-8")
                self.end_headers()
                self.wfile.write(content)
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"test.html not found")
        else:
            self.send_response(404)
            self.end_headers()

    def do_POST(self):
        global challenge, slot
        length = int(self.headers.get("Content-Length", 0))
        body   = self.rfile.read(length).decode().strip()
        print(f"  [CMD] {body}")

        if body.startswith("CHALLENGE:"):
            challenge = body.split(":")[1] == "1"
        elif body.startswith("SLOT:"):
            slot = int(body.split(":")[1]) % len(CHALLENGES)

        rssi = get_rssi()
        chal = CHALLENGES[slot]
        resp = json.dumps({"ok": True, "rssi": rssi, "challenge": challenge, "slot": slot})
        self.send_response(200)
        self.send_cors()
        self.send_header("Content-Type", "application/json")
        self.end_headers()
        self.wfile.write(resp.encode())

def toggle_challenge():
    """Cada 10s alterna el challenge automáticamente para simular botón BOOT"""
    global challenge, slot
    while True:
        time.sleep(10)
        challenge = not challenge
        if challenge:
            slot = (slot + 1) % len(CHALLENGES)
        print(f"\n  [SIM] Challenge {'ON' if challenge else 'OFF'} | Slot {slot} ({CHALLENGES[slot]['type']})\n")

if __name__ == "__main__":
    t = threading.Thread(target=toggle_challenge, daemon=True)
    t.start()
    print(f"""
╔══════════════════════════════════════════╗
║  THLS Mock ESP32 — localhost:{PORT}         ║
╠══════════════════════════════════════════╣
║  GET  http://localhost:{PORT}/             ║
║  GET  http://localhost:{PORT}/api/status   ║
╠══════════════════════════════════════════╣
║  RSSI oscila -90 → -40 cada 30s          ║
║  Challenge se alterna cada 10s (auto)    ║
╚══════════════════════════════════════════╝
""")
    server = HTTPServer(("0.0.0.0", PORT), Handler)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n[THLS] Servidor detenido.")
