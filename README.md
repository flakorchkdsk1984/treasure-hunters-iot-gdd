# 🎮 TREASURE HUNTERS IoT: THE LAST SIGNAL

**Game Design Document v1.0** — Documento profesional completo listo para POC.

## Concepto

Juego híbrido físico-digital donde los jugadores usan una **PWA Angular** para detectar señales **Bluetooth Low Energy** de balizas **ESP32** ocultas en el entorno real. Sin coordenadas exactas — solo intensidad RSSI interpretada como estados frío/caliente. Ambientado en 2089 en un mundo post-apocalíptico controlado por la IA **A.R.G.O.S.**

## Documento Principal

📄 **[GDD_TREASURE_HUNTERS_IOT_COMPLETE.md](./GDD_TREASURE_HUNTERS_IOT_COMPLETE.md)**
- **9,882 líneas · 81,102 palabras · 560 KB**
- 25 secciones completas + 2 apéndices

## Estructura del GDD

| Sección | Contenido |
|---------|-----------|
| §1–3 | Visión, Historia & Lore, Loop Principal |
| §4–7 | Sistema BLE, ESP32, RSSI frío-caliente, Filtros Kalman |
| §8–9 | UX Mobile, Wireframes ASCII, Exploración |
| §10–13 | Balizas C/B/A/S/Ω, Infiltración, 30 Minijuegos, Eventos |
| §14–16 | Clanes, Territorios, Economía F2P, Monetización |
| §17–19 | Anti-Cheat (3 capas), Gobernanza, Arquitectura Docker/K8s |
| §20–22 | Telemetría Prometheus+Grafana, Roadmap, Escalabilidad 1M+ |
| §23–24 | Backlog 100 User Stories, Proyección Financiera 5 años |
| §25 | Riesgos y Recomendaciones Finales |
| A–B | Glosario Técnico, Referencias |

## Stack Tecnológico

- **Frontend:** Angular 17+ PWA
- **Balizas:** ESP32 + BLE 5.0
- **Backend:** Node.js microservices + Go (anti-cheat)
- **DB:** MongoDB 7 + Redis 7
- **Infra:** Docker Compose (POC) → Kubernetes multi-region (producción)
- **Observabilidad:** Prometheus + Grafana + Loki + Jaeger
- **Seguridad:** HMAC-SHA256, Trust Score, ML Anomaly Detection

## Fases de Desarrollo

| Fase | Duración | Escala | Costo Infra |
|------|----------|--------|-------------|
| POC | 3 meses | 5 beacons, ~50 testers | $25–40/mes |
| Beta | 6 meses | 50–100 beacons, 500–2K players | $250–350/mes |
| Launch | 12 meses | 500+ beacons, 50K+ MAU | $1.5K–3K/mes |
| Global | Año 2+ | Multi-región, 1M+ players | $8K–15K+/mes |

## Archivos

| Archivo | Descripción | Tamaño |
|---------|-------------|--------|
| `GDD_TREASURE_HUNTERS_IOT_COMPLETE.md` | **Documento maestro completo** | 560 KB |
| `GDD_THLS_Sections_4_to_7.md` | BLE/RSSI/Arquitectura (detalle técnico) | 108 KB |
| `GDD_TreasureHuntersIoT_Sections_10_11_13.md` | Balizas/Infiltración/Eventos | 97 KB |
| `GDD_Sections_14_15_16.md` | Social/Economía/Monetización | 53 KB |
| `seccion-23-24-backlog-finanzas.md` | Backlog 100 US + Finanzas | 90 KB |
| `GDD_TreasureHunters_Sections_8_9_12_CONTINUACION.md` | Minijuegos 28-30 + Anti-Cheat | 24 KB |

---

> *Generado con equipo multidisciplinario IA: Game Designer, Narrative Designer, IoT Architect, Cloud Architect, UX Designer, Economy Designer, Security Engineer, DevOps Architect.*
