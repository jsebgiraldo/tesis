# Lista de componentes — Arquitectura AMI edge-centric

> Inventario completo del sistema validado en el piloto de 30 días.
> Estructura: por segmento de la arquitectura (Field → Gateway → Backhaul → Edge → On-prem), separando hardware, software, estándares e instrumentación.
> **Fecha:** 2026-05-15

---

## 1. Field Network (red de campo Thread)

### 1.1 Hardware

| Componente | Modelo / referencia | Cantidad | Rol |
|---|---|---|---|
| MCU + radio Thread | **ESP32-C6** (Seeed XIAO ESP32-C6 / DevKitC equivalente) | 31 | SoC con radio IEEE 802.15.4 integrada; ejecuta el firmware del nodo |
| Medidor eléctrico físico | **Emsitech P2000-T** trifásico ANSI | 1 | Fuente real de telemetría DLMS/COSEM por RS-485 |
| Interfaz RS-485 ↔ MCU | Transceptor RS-485 (TI MAX485 / SP3485 o integrado en placa) | 1 | Conexión nodo↔medidor (solo nodo físico) |
| Alimentación nodo | Puerto auxiliar 5 V del medidor (200 mA disp.) | 31 | Sin baterías, MED siempre activo (~27 mA) |

> **Nota:** los 30 nodos sintéticos son **hardware ESP32-C6 real** corriendo el mismo firmware de producción, pero generan el payload OBIS programáticamente en lugar de leerlo de un medidor físico. Esto preserva la realidad de la red de RF y de la malla Thread.

### 1.2 Software / firmware

| Componente | Versión | Rol |
|---|---|---|
| **Zephyr RTOS** | 4.x | Sistema operativo del nodo |
| **OpenThread** | 1.4 (Thread 1.4 spec, build 2024-10-15) | Pila Thread (MLE, mesh, IPv6) |
| **6LoWPAN / IPHC** | RFC 6282 | Compresión de cabeceras IPv6 |
| **CoAP** | RFC 7252 + Observe (RFC 7641) | Mensajería REST sobre UDP |
| **DTLS** | 1.2 | Seguridad CoAP |
| **LwM2M client** | OMA LwM2M 1.2 | Gestión de dispositivo + telemetría |
| **Objeto OMA personalizado** | `10242` (custom) | Recursos del medidor trifásico |
| **DLMS/COSEM** | IEC 62056-21 Mode C | Lectura del medidor físico |

---

## 2. Gateway / Edge Gateway (Raspberry Pi 4 — co-localizados en el piloto)

> En la **arquitectura de referencia (Fig. 1 del paper)** Gateway y Edge Gateway son dispositivos físicos separados. En el **piloto** se co-localizan en un mismo RPi4 por simplicidad; la pila lógica es idéntica.

### 2.1 Hardware

| Componente | Modelo / referencia | Cantidad | Rol |
|---|---|---|---|
| Computador de borde | **Raspberry Pi 4 Model B (4 GB RAM)** | 1 | Plataforma de cómputo del gateway |
| Dongle Thread | **Nordic nRF52840 USB dongle** | 1 | Radio Co-Processor (RCP) para OTBR |
| Radio HaLow | **Alfa Networks AHPI6108E HAT** (chipset Morse Micro **MM6108**, interfaz SDIO) | 1 | HaLow STA / cliente |
| Almacenamiento | NVMe SSD 128 GB (vía adaptador USB 3.0) | 1 | Persistencia local + buffer offline |
| UPS | Li-ion 12 V / 20 Ah | 1 | Respaldo de energía |
| Alimentación principal | Fuente 5 V/3 A | 1 | Alimentación primaria RPi4 |

### 2.2 Software (sistema base)

| Componente | Versión | Rol |
|---|---|---|
| **OpenWrt** | 23.05 (build oficial **Morse Micro fork**) | Sistema operativo base del gateway |
| Linux kernel | 5.15 LTS (parches BCM2711) | Kernel |
| Driver HaLow | `kmod-morse` (incluido en fork Morse Micro) | Driver MM6108 vía SDIO |
| `hostapd` / `wpa_supplicant` | versiones OpenWrt 23.05 | Configuración 802.11s/AP/STA |

### 2.3 Software (Border Router Thread)

| Componente | Versión | Rol |
|---|---|---|
| **OTBR (otbr-agent)** | OpenThread Border Router (build 2024) | Bridge Thread ↔ IPv6 |
| `wpantund` / RCP firmware | nRF52840 con `ot-rcp` | Capa de control del radio Thread |
| NAT64 / DNS64 | `tayga` o equivalente | Acceso IPv4 desde nodos IPv6 (si aplica) |

### 2.4 Software (stack de aplicación — contenedores Docker)

| Contenedor | Imagen / versión | Rol |
|---|---|---|
| **ThingsBoard Edge** | `thingsboard/tb-edge:3.6.2` (ARM64) | Plataforma IoT en el borde: ingesta LwM2M, motor de reglas, dashboards locales, sincronización gRPC con on-prem |
| **PostgreSQL** | `postgres:15-alpine` (ARM64) | Persistencia local de telemetría y device profiles |
| **Redis** | `redis:7.0-alpine` (ARM64) | Caché y coordinación |
| Docker Engine | última estable | Runtime de contenedores |

**Recursos consumidos:** ~2.5 vCPU, ~1.8 GB RAM, ~18 GB disco/60 días para 60 nodos.

---

## 3. HaLow Backhaul (enlace de radio sub-GHz)

### 3.1 Hardware

| Componente | Modelo / referencia | Cantidad | Rol |
|---|---|---|---|
| Mesh gate / HaLow AP | **Alfa Tube-AHM** (chipset MM6108) | 1 | Punto de acceso HaLow del lado del borde |
| Radio HaLow STA | (incluida en gateway, item 2.1: AHPI6108E) | 1 | Cliente HaLow del lado del campo |
| Antenas HaLow | Antenas integradas Tube-AHM + AHPI6108E HAT | — | RF sub-GHz |

### 3.2 Configuración del enlace

| Parámetro | Valor en el piloto |
|---|---|
| **Estándar** | IEEE 802.11ah-2016 (Wi-Fi HaLow) |
| Banda | 902–928 MHz (ISM, no licenciada — Región 2 / Américas) |
| Canal | 14 (909 MHz) primario; alternativos 8 (906 MHz) y 12 (908 MHz) |
| Ancho de banda | **2 MHz** (configuración recomendada del piloto); 4 MHz y 8 MHz también caracterizados |
| Topología | 802.11s mesh (`mesh_point` ↔ `mesh_gate`) |
| Distancia | **180 m LoS** (piloto); literatura ≥1 km LoS |
| Seguridad | WPA3-SAE |
| Potencia TX | Tube-AHM ~24 dBm; AHPI6108E ~1 dBm (asimetría documentada) |

---

## 4. On-Premises Platform (servidor central)

### 4.1 Hardware

| Componente | Modelo / referencia | Cantidad | Rol |
|---|---|---|---|
| Servidor on-prem | x86 server o VM (en piloto: equivalente a NUC i5 / 16 GB RAM) | 1 | Hosting de ThingsBoard server |
| Switch / red local | switch gigabit Ethernet | 1 | LAN entre Tube AHM y servidor |

### 4.2 Software

| Componente | Versión | Rol |
|---|---|---|
| **ThingsBoard server (full edition)** | 3.6.x | Plataforma central: agregación multi-gateway, persistencia histórica, dashboards |
| PostgreSQL central | 15+ | Base de datos del servidor |
| Sistema operativo | Ubuntu Server 22.04 LTS | OS host |

### 4.3 Conectividad

- Enlace **Edge Gateway ↔ On-prem** vía Ethernet LAN
- Protocolo de sincronización: **gRPC bidireccional** (TB Edge ↔ TB Server)
- Tráfico aplicativo upstream: **MQTT/TLS 1.3 sobre TCP/IPv4**

---

## 5. Estándares y protocolos (transversales)

| Estándar / RFC | Capa | Uso |
|---|---|---|
| **IEEE 802.15.4-2020** | PHY/MAC | Radio Thread (2.4 GHz, O-QPSK, 250 kbps, MTU 127 B) |
| **Thread 1.4** | red/malla | Especificación Thread Group (build 2024-10-15) |
| **6LoWPAN (RFC 4944)** | adaptación | IPv6 sobre 802.15.4 |
| **IPHC (RFC 6282)** | adaptación | Compresión de cabeceras IPv6 (~91 % observado en piloto) |
| **IPv6** | red | Direccionamiento extremo a extremo |
| **UDP** | transporte | Transporte sin conexión para CoAP |
| **CoAP (RFC 7252)** | aplicación | Mensajería REST en dispositivos restringidos |
| **CoAP Observe (RFC 7641)** | aplicación | Suscripciones a recursos |
| **DTLS 1.2 (RFC 6347)** | seguridad | Seguridad CoAP |
| **OMA LwM2M 1.2** | gestión | Modelo de objetos del dispositivo + telemetría |
| **Objeto LwM2M 10242** | aplicación | Custom object del medidor trifásico |
| **DLMS/COSEM (IEC 62056)** | aplicación | Lectura del medidor físico vía RS-485 |
| **IEEE 802.11ah-2016** | PHY/MAC | Wi-Fi HaLow (sub-GHz) |
| **IEEE 802.11s** | red | Malla HaLow |
| **WPA3-SAE** | seguridad | Autenticación HaLow |
| **TCP (RFC 9293)** | transporte | Transporte upstream |
| **MQTT 5.0** | aplicación | Mensajería pub/sub upstream |
| **TLS 1.3 (RFC 8446)** | seguridad | Seguridad upstream |
| **gRPC** | aplicación | Sincronización TB Edge ↔ TB Server |
| **ISO/IEC 30141:2024** | arquitectura | Arquitectura de referencia IoT (entidades funcionales) |

---

## 6. Instrumentación y herramientas (durante el piloto)

| Herramienta | Uso |
|---|---|
| **NTP (chrony)** | Sincronización de relojes nodo / gateway / on-prem (±5 ms) |
| `tcpdump` / `tshark` | Captura de paquetes en `wlan0` y otras interfaces |
| `iperf3` | Pruebas de rendimiento HaLow (TCP, UDP, distintos BW) |
| `ping` | Latencia ICMP del enlace HaLow |
| **Nordic PPK2** | Profiler de potencia 100 kS/s (consumo nodo) |
| **Fluke 87V** | Multímetro registrador (tensiones, corrientes) |
| **Grafana + Prometheus** | Dashboards de métricas del sistema durante el piloto |
| `pandas` / `scipy` (Python) | Análisis estadístico de los datos del piloto |
| `iwinfo` / `station dump` (OpenWrt) | Captura de info inalámbrica (RSSI, MCS) |
| `journalctl` / `docker logs` | Trazas del sistema y los contenedores |

---

## 7. Trazabilidad con la Fig. 1 del paper

Para que se pueda navegar fácilmente entre la figura del paper y este inventario:

| Zona en Fig. 1 | Componentes en este documento |
|---|---|
| **Field network** | §1 (ESP32-C6, P2000-T, Zephyr, OpenThread, LwM2M, etc.) |
| **Gateway** | §2.1 (RPi4, nRF52840), §2.2–2.3 (OpenWrt, OTBR) |
| **HaLow backhaul #1 / #2** | §3 (Tube-AHM, AHPI6108E, configuración del enlace) |
| **Edge Gateway** | §2.1 (RPi4, AHPI6108E), §2.4 (Docker, TB Edge, PG, Redis) — esto es lo que detalla la nueva **Fig. 3** |
| **On-premises platform** | §4 (servidor, TB Server, Ethernet) |
