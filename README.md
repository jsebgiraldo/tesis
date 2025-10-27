# Tesis de Maestría: Gateway de Telemetría para Smart Energy

**Universidad Nacional de Colombia**  
**Programa de Maestría en Ingeniería Eléctrica**  
**Autor:** Luis Antonio  
**Año:** 2025

## 📋 Descripción del Proyecto

Desarrollo de un gateway IoT avanzado para infraestructuras de medición inteligente (AMI - Advanced Metering Infrastructure) en redes Smart Grid, implementado sobre **Raspberry Pi 4 + OpenWRT** con capacidades de edge computing, múltiples protocolos de comunicación y resiliencia offline.

## 🎯 Objetivos

- Diseñar arquitectura de gateway multi-protocolo para telemetría Smart Energy
- Implementar conectividad de largo alcance con **IEEE 802.11ah HaLow** (sub-GHz, 1-3 km)
- Integrar redes Thread/802.15.4 mesh para medidores inteligentes
- Desarrollar resiliencia offline con queue persistente (7 días buffer local)
- Cumplir estándares internacionales: IEEE 2030.5, ISO/IEC 30141, CREG Colombia

## 🏗️ Arquitectura del Sistema

```
[Medidores Thread] ──> [DCU/OTBR] ──> [Gateway RPi4] ──> [ThingsBoard Cloud]
   (ESP32-C6)         (nRF52840)      HaLow/LTE/Eth         (AWS/Azure)
                                      Docker Containers
                                      └── TB Edge
                                      └── PostgreSQL
                                      └── Kafka
                                      └── MQTT Broker
```

## 🔧 Hardware del Gateway (Raspberry Pi 4)

### Plataforma Base
- **SoC:** Broadcom BCM2711 (Cortex-A72 quad-core @ 1.5 GHz)
- **RAM:** 4 GB LPDDR4-3200
- **Almacenamiento:** 
  - Boot: microSD 32 GB (OpenWRT system)
  - Data: M.2 NVMe SSD 256 GB via PCIe HAT (Docker, PostgreSQL, queue)
- **Alimentación:** PoE+ IEEE 802.3at (25W) con ventilador activo

### Conectividad
- **Thread/802.15.4:** Nordic nRF52840 Dongle (USB, OpenThread RCP)
- **HaLow 802.11ah:** Morse Micro MM6108 via SPI (GPIO, 902-928 MHz, 40 Mbps)
- **LTE-M/NB-IoT:** Quectel BG95-M3 (USB, Cat-M1, 375 kbps)
- **Ethernet:** Gigabit RJ45 (WAN primaria)

## 💻 Software Stack

### Sistema Operativo
- **OpenWRT 23.05.0** (bcm27xx/bcm2711 target)
- Kernel Linux 5.15.134 LTS
- Arquitectura: aarch64 (ARM64v8)

### Contenedores Docker
- **ThingsBoard Edge 3.6:** Edge computing, reglas, dashboards locales
- **PostgreSQL 15 + TimescaleDB:** Series temporales, compresión 10-20×
- **Apache Kafka:** Message broker, buffer 7 días, >100k msg/s
- **Mosquitto:** MQTT broker, TLS/mTLS, QoS 0/1/2
- **OpenThread Border Router (OTBR):** Thread mesh IPv6 gateway

### Protocolos de Comunicación
- **MQTT** (QoS 0/1/2): Telemetría uplink, LWT, retained messages
- **CoAP** (UDP): Thread mesh intra-nodo, Observe, DTLS+PSK
- **HTTP/REST:** APIs gestión (TB Edge, IEEE 2030.5, LuCI)
- **LwM2M:** Device management, firmware OTA, objetos OMA

## 📡 Modos de Operación HaLow

El gateway soporta 4 modos de operación IEEE 802.11ah:

1. **AP (Access Point):** Cobertura centralizada 3 km, 2500 endpoints
2. **STA (Station):** Cliente para backhaul rural sin costo celular
3. **802.11s Mesh:** Auto-healing, extensión 6-9 km, HWMP routing
4. **EasyMesh (IEEE 1905.1):** Roaming transparente, steering inteligente

## 🔒 Seguridad

- **OpenWRT Firewall:** nftables, zonas aisladas (WAN/LAN/HaLow/Thread)
- **TLS 1.3:** TB Edge cloud sync (puerto 7070 gRPC)
- **WPA3-SAE:** HaLow AP con PMF obligatorio
- **Thread AES-128-CCM:** Mesh network encryption
- **mTLS:** MQTT broker con certificados X.509 ECC P-256
- **OpenVPN:** Túnel permanente para gestión remota (NOC)

## 🛠️ Gestión Remota

- **OpenWRT Feeds:** opkg package manager, custom feed para Smart Grid
- **OpenVPN Client:** Túnel permanente VPN a NOC (IPs fijas 10.8.0.100-199)
- **OpenWISP:** Gestión centralizada masiva (100-1000 gateways)
  - Templates UCI con variables
  - Firmware OTA scheduler con rollback automático
  - Monitoring (CPU/RAM/Storage/Interfaces/Docker)
  - Alertas (email/SMS/webhook)

## 📊 Resiliencia y Performance

### Almacenamiento Persistente
- **Queue TB Edge:** 100k msgs (2 GB), compresión gzip/lz4
- **PostgreSQL + TimescaleDB:** Retención 90 días, compresión 10-20×
- **Kafka:** Buffer 7 días, replay histórico, multi-consumidor
- **SSD NVMe:** >3000 IOPS, >1M ciclos E/W, latencia <1 ms

### Failover WAN
- **Ethernet (primaria):** Metric 10
- **LTE-M (secundaria):** Metric 20
- **Conmutación:** <30s automático con mwan3
- **Compresión LTE:** CBOR 40-60% reducción tráfico

### Capacidad
- **Topología estrella (AP único):** 2,500 endpoints (10 DCUs × 250 nodos)
- **Topología mesh (3 gateways):** 7,500 endpoints, 9 km cobertura
- **Throughput HaLow:** 40 Mbps (20 Mbps agregado con 10 DCUs)
- **Latencia E2E:** <5s percentil 95 (Thread → TB Edge)

## 📚 Estructura del Repositorio

```
tesis/
├── Tesis___Trabajo_final___Maestria___2025/
│   ├── 0000.tex                    # Documento principal (118 páginas)
│   ├── 03Gateway.tex               # Capítulo gateway (3902 líneas)
│   ├── 04Arquitectura.tex          # Arquitectura completa E2E
│   ├── Referencias.bib             # Bibliografía (IEEE, ISO, papers)
│   ├── 00Figuras/                  # Imágenes y diagramas
│   ├── build.bat, build.ps1        # Scripts compilación Windows
│   ├── Makefile                    # Compilación Linux/macOS
│   └── dtvstyle.bst                # Estilo bibliográfico UN
├── ref/                            # PDFs de referencia (IEEE 2030.5, ISO 30141, etc.)
├── .gitignore                      # Archivos LaTeX compilación ignorados
└── README.md                       # Este archivo
```

## 🔨 Compilación del Documento

### Requisitos
- **LaTeX:** MiKTeX (Windows) o TeX Live (Linux/macOS)
- **Compilador:** pdflatex
- **Bibliografía:** bibtex
- **Paquetes:** babel, geometry, fancyhdr, minted, hyperref, etc.

### Compilar en Windows
```powershell
cd Tesis___Trabajo_final___Maestria___2025
.\build.ps1
# Output: 0000.pdf (118 páginas, ~800 KB)
```

### Compilar en Linux/macOS
```bash
cd Tesis___Trabajo_final___Maestria___2025
make
# o manualmente:
pdflatex 0000.tex
bibtex 0000
pdflatex 0000.tex
pdflatex 0000.tex
```

## 📖 Contenido del Capítulo Gateway (03Gateway.tex)

1. **Introducción:** AMI, función del gateway, estándares
2. **IEEE 2030.5 SEP 2.0:** Function Sets (DCAP, TM, MM, MSG, ED)
3. **ISO/IEC 30141:** Arquitectura IoT de referencia (4 vistas)
4. **OpenWRT:** Justificación, ventajas vs sistemas propietarios
5. **🆕 Implementación Raspberry Pi 4:**
   - Hardware real (BCM2711, 4 GB RAM, periféricos)
   - nRF52840 Thread dongle (USB, RCP firmware)
   - Morse Micro MM6108 HaLow (SPI via GPIO)
   - Quectel BG95 LTE-M (USB, ModemManager)
   - M.2 NVMe SSD (PCIe HAT, 256 GB, >3000 IOPS)
   - Instalación OpenWRT paso a paso
   - Configuración completa (Thread, HaLow, LTE, Docker)
6. **Modos HaLow:** AP, STA, 802.11s Mesh, EasyMesh (configuraciones UCI)
7. **Resiliencia:** SSD + TB Edge queue persistente (100k msgs, 7 días)
8. **Datos Distribuidos:** Kafka + PostgreSQL/TimescaleDB
9. **Protocolos IoT:** MQTT, CoAP, HTTP/REST, LwM2M (comparativa)
10. **Gestión Remota:** Feeds, OpenVPN, OpenWISP (OTA masivo)
11. **Validación:** Pruebas funcionales, performance, failover
12. **Trabajo Futuro:** IA local (Ollama roadmap), 5G RedCap, MPTCP

## 🌐 Estándares y Normativas

- **IEEE 2030.5-2023:** Smart Energy Profile 2.0 (SEP 2.0)
- **ISO/IEC 30141:2024:** IoT Reference Architecture
- **IEEE 802.11ah-2016:** Sub-1 GHz Wi-Fi (HaLow)
- **Thread 1.3:** Low-power mesh networking
- **MQTT 5.0:** ISO/IEC 20922
- **LwM2M 1.2:** OMA SpecWorks device management
- **CREG Colombia:** Resoluciones medición inteligente

## 📈 Roadmap Futuro

### Validaciones Pendientes
- [ ] Mediciones CPU/RAM/Temp bajo carga completa
- [ ] Benchmarks throughput E2E (Thread → TB Cloud)
- [ ] Stress test 1000 msg/s durante 24h
- [ ] Thermal throttling validation (<75°C objetivo)

### Mejoras Planeadas
- [ ] **Ollama IA Local:** Llama 3.2 1B / Phi-3 mini (RPi 4 8GB)
- [ ] **MCP Server Python:** Herramientas TB Edge API estructuradas
- [ ] **HaLow USB:** Migración a drivers ath11k_usb (Q2 2026)
- [ ] **5G RedCap:** Quectel RG500U (<50ms latencia)
- [ ] **Compute Module 4:** Hardware industrial (-40°C a +85°C)
- [ ] **Alta Disponibilidad:** Par activo-pasivo con VRRP

## 📄 Licencia

Este proyecto es material académico de la Universidad Nacional de Colombia.  
Todos los derechos reservados © 2025 Luis Antonio

## 👤 Contacto

**Autor:** Luis Antonio  
**Institución:** Universidad Nacional de Colombia  
**Programa:** Maestría en Ingeniería Eléctrica  
**Email:** [Agregar email institucional]  
**GitHub:** [Agregar URL del repositorio]

---

**Última actualización:** Octubre 27, 2025  
**Estado:** En desarrollo (Capítulo Gateway completo, pendiente arquitectura E2E)  
**Páginas:** 118 / ~150 estimadas (tesis completa)
