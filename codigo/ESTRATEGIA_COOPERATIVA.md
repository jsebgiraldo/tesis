# Estrategia de Trabajo Cooperativo: Tesis + Servidor Desarrollo

## Resumen Ejecutivo

**Objetivo:** Construir el firmware Zephyr RTOS para ESP32-C6 (Capítulo 3: Nodo IoT Thread + LwM2M) coordinando entre:

| Nodo | IP / Ubicación | Rol |
|------|---------------|-----|
| **Workstation Tesis** | Local (macOS) | Documentación LaTeX, diseño arquitectura, notas Obsidian |
| **Servidor Desarrollo** | `100.67.60.126` | Toolchain Zephyr, compilación, flash ESP32-C6, pruebas OT/LwM2M |

---

## 1. Arquitectura de Trabajo Cooperativo

```
┌─────────────────────────────────────┐     ┌──────────────────────────────────────┐
│  WORKSTATION TESIS (macOS)          │     │  SERVIDOR 100.67.60.126              │
│                                     │     │                                      │
│  ┌─────────────────────────────┐    │     │  ┌──────────────────────────────┐    │
│  │ Repo: jsebgiraldo/tesis     │    │     │  │ Repo: jsebgiraldo/tesis-fw   │    │
│  │ - LaTeX capítulos           │    │     │  │ - Zephyr workspace           │    │
│  │ - Obsidian notas            │    │     │  │ - Firmware ESP32-C6          │    │
│  │ - Figuras/diagramas         │    │     │  │ - Tests unitarios            │    │
│  │ - docs_sesiones/            │    │     │  │ - CI/CD local                │    │
│  └──────────┬──────────────────┘    │     │  └──────────┬───────────────────┘    │
│             │                       │     │             │                         │
│  ┌──────────▼──────────────────┐    │     │  ┌──────────▼───────────────────┐    │
│  │ Agente IA (Copilot/Claude)  │    │     │  │ Agente IA (Copilot/Claude)   │    │
│  │ - Redacción LaTeX           │    │     │  │ - Código Zephyr C            │    │
│  │ - Revisión coherencia       │    │     │  │ - Debugging firmware         │    │
│  │ - Generación tablas/figs    │    │     │  │ - Análisis logs OpenThread   │    │
│  └─────────────────────────────┘    │     │  └──────────────────────────────┘    │
│                                     │     │                                      │
└─────────────────┬───────────────────┘     └──────────────┬───────────────────────┘
                  │                                        │
                  │        ┌───────────────┐               │
                  └───────►│  GitHub Repo   │◄──────────────┘
                           │  (shared)      │
                           │  + Git LFS     │
                           └───────┬───────┘
                                   │
                           ┌───────▼───────┐
                           │  Artifacts     │
                           │  - .bin/.elf   │
                           │  - logs/       │
                           │  - pcaps/      │
                           └───────────────┘
```

---

## 2. Estrategia Git: Dos Repos Sincronizados

### Repo 1: `jsebgiraldo/tesis` (existente)
- Documentación LaTeX, Obsidian, figuras
- Branch `master` → documento tesis
- Branch `cap3-firmware-docs` → documentación del firmware para Cap 3

### Repo 2: `jsebgiraldo/tesis-fw` (nuevo, en servidor 100.67.60.126)
- Código Zephyr RTOS exclusivamente
- Estructura west workspace

```
tesis-fw/
├── west.yml                    # Zephyr manifest
├── zephyr/                     # Zephyr RTOS (west update)
├── modules/                    # HALs, libraries
├── app/                        # ← NUESTRO CÓDIGO
│   ├── CMakeLists.txt
│   ├── prj.conf                # Kconfig general
│   ├── boards/
│   │   └── esp32c6_devkitc.conf  # Board-specific overlay
│   ├── src/
│   │   ├── main.c              # Entry point
│   │   ├── thread_network.c    # OpenThread config + join
│   │   ├── thread_network.h
│   │   ├── lwm2m_client.c      # LwM2M objects + registration
│   │   ├── lwm2m_client.h
│   │   ├── meter_dlms.c        # DLMS/COSEM parser UART
│   │   ├── meter_dlms.h
│   │   ├── meter_modbus.c      # Modbus RTU parser
│   │   ├── meter_modbus.h
│   │   └── power_mgmt.c        # Sleep/wake management
│   ├── dts/
│   │   └── esp32c6_ami.overlay # Device tree overlay (UART, GPIO)
│   └── tests/
│       ├── test_dlms_parser.c
│       └── test_lwm2m_objects.c
├── scripts/
│   ├── flash.sh                # Flash + verify
│   ├── monitor.sh              # Serial monitor
│   └── ot_commission.sh        # Thread commissioning
├── docs/
│   └── BUILD.md                # Instrucciones compilación
└── .github/
    └── workflows/
        └── build.yml           # CI compilación
```

### Sincronización entre repos

```bash
# En el servidor 100.67.60.126, enlazar como submódulo o referencia:
cd ~/tesis-fw
git submodule add https://github.com/jsebgiraldo/tesis.git docs/tesis-ref

# O más simple: usar git artifacts para compartir binarios y logs
# Los resultados de pruebas van al repo tesis como datos:
scp user@100.67.60.126:~/tesis-fw/build/results/*.json \
    ~/tesis/datos/firmware_tests/
```

---

## 3. Protocolo de Trabajo con Agentes IA

### 3.1 Agente Local (Tesis - macOS)

**Contexto que debe tener siempre:**
```
Eres un asistente para una tesis de maestría sobre arquitectura IoT
para Smart Energy (AMI). El Capítulo 3 documenta el nodo IoT con:
- ESP32-C6 (RISC-V 160MHz, 802.15.4)
- Zephyr RTOS (no ESP-IDF)
- OpenThread 1.3+ (FTD, mesh networking)
- LwM2M 1.2 (device management, Objects 3/4/3200/3203/3305)
- CoAP (confirmable para alarmas, non-confirmable telemetría)
- DLMS/COSEM parser (IEC 62056, códigos OBIS)
- Modbus RTU (función 0x03)

El servidor de desarrollo está en 100.67.60.126.
El firmware se compila con west build para esp32c6_devkitc.
```

**Tareas típicas del agente local:**
1. Redactar secciones LaTeX del Cap 3 basándose en resultados del servidor
2. Generar tablas comparativas (Zephyr vs ESP-IDF, Thread vs Zigbee)
3. Crear diagramas de flujo del firmware (secuencias, estados)
4. Revisar coherencia entre código real y documentación

### 3.2 Agente Remoto (Servidor 100.67.60.126)

**Contexto que debe tener siempre:**
```
Eres un asistente de desarrollo de firmware embebido.
Proyecto: Nodo IoT para medición inteligente (AMI/Smart Energy).
Hardware: ESP32-C6-DevKitC-1
RTOS: Zephyr (último release estable)
Protocolos: OpenThread + LwM2M + CoAP sobre 802.15.4
El código vive en ~/tesis-fw/app/

Requisitos funcionales:
- Unirse a red Thread como FTD (Full Thread Device)
- Registrarse con servidor LwM2M (Bootstrap → Register → Observe)
- Leer medidor vía UART/RS-485 (DLMS o Modbus RTU)
- Reportar telemetría cada 15 min (CON/NON CoAP)
- Modo sleep entre lecturas (target <1 mA promedio)
- Reportar alarmas inmediatamente (voltage sag/swell, tamper)

El servidor LwM2M/Leshan está en esta misma máquina (100.67.60.126:5683).
```

**Tareas típicas del agente remoto:**
1. Escribir/modificar código C para Zephyr
2. Configurar Kconfig (prj.conf) y Device Tree overlays
3. Compilar (`west build`), flashear (`west flash`), depurar
4. Analizar logs de OpenThread (CLI, wpantund)
5. Probar registro LwM2M contra Leshan local
6. Capturar tráfico 802.15.4 (Wireshark + nRF Sniffer)

### 3.3 Protocolo de Handoff entre Agentes

```
PASO 1: Diseño (Agente Local)
  → Define requisito funcional en Obsidian/LaTeX
  → Genera pseudocódigo o diagrama de secuencia
  → Escribe archivo TASK_XXX.md con especificación

PASO 2: Implementación (Agente Remoto)
  → Lee TASK_XXX.md (compartido vía git)
  → Implementa en C/Zephyr
  → Compila y ejecuta tests
  → Genera RESULT_XXX.md con:
    - Código implementado (snippet)
    - Logs de compilación
    - Resultados de test
    - Métricas (RAM/Flash usage, timing)

PASO 3: Documentación (Agente Local)
  → Lee RESULT_XXX.md
  → Integra en sección LaTeX del Cap 3
  → Actualiza tablas de resultados
  → Genera figuras con datos reales

PASO 4: Validación cruzada
  → Agente Local revisa coherencia código↔documento
  → Agente Remoto verifica que specs match implementación
```

---

## 4. Setup Inicial del Servidor (100.67.60.126)

### 4.1 Instalar Zephyr SDK

```bash
# En el servidor 100.67.60.126
# Paso 1: Dependencias
sudo apt update && sudo apt install -y \
    git cmake ninja-build gperf ccache dfu-util device-tree-compiler \
    wget python3-dev python3-pip python3-setuptools python3-tk \
    python3-wheel xz-utils file make gcc gcc-multilib g++-multilib \
    libsdl2-dev libmagic1 python3-venv

# Paso 2: West (Zephyr meta-tool)
pip3 install west

# Paso 3: Crear workspace
mkdir -p ~/tesis-fw && cd ~/tesis-fw
west init -m https://github.com/zephyrproject-rtos/zephyr --mr main
west update
west zephyr-export

# Paso 4: Instalar Python requirements
pip3 install -r ~/tesis-fw/zephyr/scripts/requirements.txt

# Paso 5: Zephyr SDK (toolchain RISC-V para ESP32-C6)
cd /opt
wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/zephyr-sdk-0.16.8_linux-x86_64.tar.xz
tar xf zephyr-sdk-0.16.8_linux-x86_64.tar.xz
cd zephyr-sdk-0.16.8
./setup.sh

# Paso 6: Espressif HAL (binarios blob para ESP32-C6)
west blobs fetch hal_espressif

# Paso 7: Verificar compilación con sample
cd ~/tesis-fw
west build -b esp32c6_devkitc zephyr/samples/hello_world
```

### 4.2 Servidor LwM2M (Leshan)

```bash
# Eclipse Leshan como servidor LwM2M de prueba
docker run -d --name leshan \
    -p 5683:5683/udp \
    -p 5684:5684/udp \
    -p 8080:8080 \
    eclipsefoundation/leshan:latest

# Web UI: http://100.67.60.126:8080
# CoAP endpoint: coap://100.67.60.126:5683
```

### 4.3 OpenThread Border Router (OTBR)

```bash
# OTBR en Docker para crear red Thread
docker run -d --name otbr \
    --sysctl "net.ipv6.conf.all.disable_ipv6=0" \
    --sysctl "net.ipv6.conf.all.forwarding=1" \
    --privileged \
    -p 8081:80 \
    -v /dev:/dev \
    openthread/otbr:latest \
    --radio-url spinel+hdlc+uart:///dev/ttyUSB0

# Web UI: http://100.67.60.126:8081
# Requiere dongle nRF52840 como RCP (Radio Co-Processor)
```

---

## 5. Roadmap Capítulo 3 — Tareas Incrementales

### Fase 1: Infraestructura (Semana 1)
| # | Tarea | Dónde | Archivo Output |
|---|-------|-------|----------------|
| 1.1 | Setup Zephyr SDK + verificar build hello_world | Servidor | BUILD.md |
| 1.2 | Crear repo tesis-fw con estructura | Servidor | west.yml, CMakeLists.txt |
| 1.3 | Leshan Docker funcionando | Servidor | docker-compose.yml |
| 1.4 | Esquema capítulo 3 LaTeX | Local | 03_Nodo_IoT_Thread_LwM2M.md |

### Fase 2: Thread Networking (Semana 2)
| # | Tarea | Dónde | Archivo Output |
|---|-------|-------|----------------|
| 2.1 | OpenThread FTD join network | Servidor | thread_network.c |
| 2.2 | Verificar IPv6 connectivity (ping6) | Servidor | logs/thread_join.log |
| 2.3 | Documentar proceso commissioning | Local | Cap3 §3.x LaTeX |

### Fase 3: LwM2M Client (Semana 3)
| # | Tarea | Dónde | Archivo Output |
|---|-------|-------|----------------|
| 3.1 | LwM2M client registration con Leshan | Servidor | lwm2m_client.c |
| 3.2 | Implementar Objects 3, 4 (Device, Connectivity) | Servidor | lwm2m_objects.c |
| 3.3 | Implementar Objects 3200, 3203, 3305 (custom) | Servidor | lwm2m_objects.c |
| 3.4 | Documentar registro y objects LwM2M | Local | Cap3 §3.x LaTeX |

### Fase 4: Meter Reading (Semana 4)
| # | Tarea | Dónde | Archivo Output |
|---|-------|-------|----------------|
| 4.1 | UART driver + DLMS parser | Servidor | meter_dlms.c |
| 4.2 | Modbus RTU parser | Servidor | meter_modbus.c |
| 4.3 | Integración meter→LwM2M reporting | Servidor | main.c |
| 4.4 | Documentar parsers y flujo datos | Local | Cap3 §3.x LaTeX |

### Fase 5: Power Management + Tests (Semana 5)
| # | Tarea | Dónde | Archivo Output |
|---|-------|-------|----------------|
| 5.1 | Sleep/wake cycle management | Servidor | power_mgmt.c |
| 5.2 | Medir consumo real (INA219 o multímetro) | Servidor | datos/power_profile.csv |
| 5.3 | Tests unitarios + integración | Servidor | tests/ |
| 5.4 | Capítulo 3 completo con datos reales | Local | Cap3 final LaTeX |

---

## 6. Comandos Útiles de Acceso Remoto

```bash
# SSH al servidor
ssh user@100.67.60.126

# Copiar resultados al workspace tesis local
scp user@100.67.60.126:~/tesis-fw/build/zephyr/zephyr.bin \
    ~/tesis/datos/firmware/

# Sincronizar logs de prueba
rsync -avz user@100.67.60.126:~/tesis-fw/logs/ \
    ~/tesis/datos/logs_firmware/

# Monitor serial remoto (util para ver output en tiempo real)
ssh user@100.67.60.126 "minicom -D /dev/ttyUSB0 -b 115200"

# Compilar remotamente desde local
ssh user@100.67.60.126 "cd ~/tesis-fw && west build -b esp32c6_devkitc app"

# VS Code Remote SSH (recomendado para desarrollo)
code --remote ssh-remote+100.67.60.126 ~/tesis-fw
```

---

## 7. ¿Por qué Zephyr en vez de ESP-IDF?

| Criterio | ESP-IDF (actual tesis) | Zephyr RTOS | Ventaja |
|----------|----------------------|-------------|---------|
| **OpenThread** | Port parcial, API custom | Nativo, API estándar OT | Zephyr |
| **LwM2M** | libcoap manual | `CONFIG_LWM2M` built-in | Zephyr |
| **Device Tree** | No tiene | DTS completo | Zephyr |
| **RTOS features** | FreeRTOS wrapper | Nativo (threads, semaphores, etc.) | Empate |
| **Portabilidad** | Solo Espressif | 600+ boards | Zephyr |
| **Testing** | Manual | Ztest framework | Zephyr |
| **Power mgmt** | Light/deep sleep API | PM subsystem unificado | Zephyr |
| **Madurez ESP32-C6** | Completa | En progreso (RISC-V) | ESP-IDF |
| **Comunidad** | Grande Espressif | Grande (Linux Foundation) | Empate |

**Decisión: Zephyr** — El soporte nativo de OpenThread + LwM2M + Ztest + Device Tree justifica la migración. La tesis se beneficia al demostrar uso de RTOS estándar industrial.

---

## 8. Checklist Pre-trabajo

- [ ] Verificar acceso SSH a 100.67.60.126
- [ ] Verificar que ESP32-C6 está conectado al servidor (lsusb, /dev/ttyUSB*)
- [ ] Instalar Zephyr SDK en servidor
- [ ] Crear repo tesis-fw en GitHub
- [ ] Configurar VS Code Remote SSH
- [ ] Instalar Leshan server (Docker)
- [ ] Tener dongle nRF52840 para OTBR (o usar OT CLI en segundo ESP32)
