# AUDITORÍA INTEGRAL: Capítulos de Implementación (3, 4, 5)

**Fecha:** 2025-01-XX  
**Archivos activos auditados:**
- `03NodoIoT_Thread_LwM2M.tex` (Cap 3, 1225 líneas)
- `04Gateway_HaLow_Edge.tex` (Cap 4, 1467 líneas)
- `05Server_ThingsBoard_Kafka.tex` (Cap 5, 955 líneas)

**Archivos contextuales revisados:**
- `0000.tex` (documento maestro, 440 líneas)
- `06Resultados_Validacion.tex` (Cap 6, 1404 líneas)
- `07Conclusiones_TrabajoFuturo.tex` (Cap 7, 1445 líneas)
- 12 archivos deprecated/backup

---

## 1. RESUMEN POR CAPÍTULO

### Capítulo 3 — "Capa de Dispositivos: Diseño e Implementación de Nodos Thread con LwM2M"
**Nivel:** 1 (Field Network)  
**Contenido:** Diseño e implementación del nodo IoT ESP32-C6 con pila IEEE 802.15.4 / Thread 1.3 / CoAP / LwM2M para lectura DLMS/COSEM de medidores trifásicos.

| Sección | Contenido |
|---|---|
| §3.1 Introducción | Visión general del capítulo, piloto de "30 nodos" |
| §3.2 Pila de protocolos | IEEE 802.15.4 PHY/MAC, Thread 1.3, topología mesh, TREL, routing, CoAP, LwM2M Object 10242 |
| §3.3 Overhead y optimización | Comparación DLMS-TCP vs CoAP-UDP, análisis Observe vs polling, payload Thread/6LoWPAN, fragmentación, carga de red agregada |
| §3.4 Hardware y firmware | XIAO ESP32C6 + RS485 Breakout, ESP-IDF 5.1.2, OpenThread, Anjay LwM2M, 5 tareas FreeRTOS, modos energéticos |
| §3.5 Configuración LwM2M | Perfil "ESP32-Thread-Meter", mapeo 26 recursos LwM2M → ThingsBoard keys, alarmas bidireccionales |
| §3.6 Pruebas unitarias | Consumo energético 48h (19.3 mA medido), latencia local Thread 500 muestras (45.2 ms promedio), Thread join time |
| §3.7 Conclusiones | Síntesis, "Regla de Exclusión" Thread |

### Capítulo 4 — "Gateway Inteligente de Borde: Agregación Thread y Backhaul HaLow Mesh"
**Nivel:** 2-3 (HaLow mesh + Edge Gateway)

| Sección | Contenido |
|---|---|
| §4.1 Introducción | Arquitectura: 3 Alfa Networks Tube AHM + 2 RPi4 gateways |
| §4.2 Hardware RPi4 | Especificaciones, tabla comparativa, 6 microservicios Docker |
| §4.3 OTBR | nRF52840 USB RCP, NAT64/DNS64, Thread Commissioner, validación |
| §4.4 HaLow mesh | IEEE 802.11ah (S1G PHY), 802.11s, HWMP, RAW, config Alfa Tube AHM, validación (4.2 Mbps, 11.3 ms RTT, 1.2 km rango) |
| §4.5 ThingsBoard Edge | Arquitectura monolítica, 7 containers Docker, Docker Compose config, 4 Rule Chains pre-procesamiento, operación offline, características clave (5 features), tabla comparativa vs AWS Greengrass / Azure IoT Edge |
| §4.6 Validación operacional | Métricas 90 días: 99.93% uptime, 72% reducción WAN, 8 ms P95, 1.2 GB RAM |
| §4.7 Conclusiones | Síntesis, "Regla de Exclusión" HaLow/OTBR |

### Capítulo 5 — "Plataforma Cloud de Gestión AMI: Arquitectura Escalable con ThingsBoard y Kafka"
**Nivel:** 4 (Cloud Server)

| Sección | Contenido |
|---|---|
| §5.1 Introducción | 192 medidores, ThingsBoard 3.6.2 CE, Apache Kafka |
| §5.2 Infraestructura AWS | EC2 t3.xlarge, RDS PostgreSQL, ElastiCache Redis, MSK Kafka, S3. Total $440/mes |
| §5.2.2 Kafka | 4 topics, consumer groups, configs producción Broker/Producer/Consumer, tuning AMI |
| §5.2.3 Balanceo | ALB, rate limiting per-device/global/API REST |
| §5.3 ThingsBoard Server | Microservicios (transport, core Akka actors, data, API/UI) |
| §5.3.2 Docker Compose | **VACÍO — solo TODO** |
| §5.4 PostgreSQL + TimescaleDB | Hypertables, compresión, continuous aggregates, backup |
| §5.5 Lógica negocio | Rule Chains (telemetría, facturación, alarmas, export), dashboards, APIs REST/WebSocket |
| §5.6 APIs Programáticas | **VACÍO — solo TODO** (contenido ya cubierto en §5.5) |
| §5.7 Conclusiones | Síntesis |

---

## 2. SECCIONES VACÍAS / PLACEHOLDER

| Archivo | Sección | Estado |
|---|---|---|
| `05Server_ThingsBoard_Kafka.tex` | §5.3.2 "Configuración Docker Compose" | **VACÍO** — solo comentario `% TODO: MIGRAR desde 04Implementacion_NEW.tex §4.5` |
| `05Server_ThingsBoard_Kafka.tex` | §5.6 "APIs Programáticas y Webhooks" | **VACÍO** — duplicado de §5.5 que ya tiene el contenido |

---

## 3. INCONSISTENCIAS ENTRE CAPÍTULOS (CRÍTICAS)

### 3.1 Tamaño del piloto

| Fuente | Cifra | Contexto |
|---|---|---|
| Cap 3 intro (§3.1) | 30 nodos | "30 nodos ESP32-C6-DevKitC-1" |
| Cap 3 hardware (§3.4) | **60 unidades** | Stock XIAO ESP32C6 |
| Cap 4 conclusiones | **192 devices** | Validación ThingsBoard Edge |
| Cap 5 intro (§5.1) | **192 medidores piloto** | Gestión central |
| Cap 6 (resultados) | **30 medidores** | Piloto real validado |
| 04Implementacion_NEW.tex | 192 medidores pero BoM lista 30 ESP32 | Contradictorio internamente |

**Diagnóstico:** El piloto real fue de **30 medidores** (evidencia en Cap 6). Los 192 son el total de apartamentos del complejo (capacidad proyecto completa). Los 60 son stock de compra. Debe normalizarse la redacción: "piloto de 30 medidores en un complejo residencial de 192 viviendas".

### 3.2 Modelo de hardware del nodo

| Fuente | Modelo |
|---|---|
| Cap 3 intro | ESP32-C6-**DevKitC-1** |
| Cap 3 hardware (§3.4) | Seeed Studio **XIAO ESP32C6** |
| Cap 3 pruebas (§3.6) | ESP32-C6-**DevKitC-1** (vuelve al anterior) |
| 04Implementacion_NEW.tex | ESP32-C6-**DevKitC-1** |

**Diagnóstico:** Dos modelos de hardware distintos. El cuerpo del capítulo describe XIAO (con RS485 Breakout, factor de forma compacto), pero la intro y pruebas referencian DevKitC-1. Son boards **diferentes** (pinout, tamaño, precio). Unificar en uno solo.

### 3.3 Modelo del medidor

| Fuente | Modelo |
|---|---|
| Cap 3 (§3.4) | **Emsitech P2000-T** (trifásico) |
| Cap 6 (§6.2) | **Itron SL7000** (20 mono + 10 trifásicos) |
| 05Resultados_NEW.tex | **Itron SL7000** |

**Diagnóstico:** Discrepancia severa. Cap 3 diseña todo el mapeo OBIS alrededor del P2000-T (51 registros OBIS específicos). Cap 6 usa Itron SL7000 que tiene registros OBIS diferentes. Debe alinearse.

### 3.4 Versión de ThingsBoard

| Fuente | Versión |
|---|---|
| Cap 4 (activo) | ThingsBoard Edge **3.6.2** |
| Cap 5 (activo) | ThingsBoard CE **3.6.2** |
| Cap 7 / 05Conclusiones.tex | ThingsBoard Edge **3.6.0** |

**Diagnóstico:** Cap 7 parece escrito antes de la actualización a 3.6.2 mencionada en Cap 4. Actualizar Cap 7.

### 3.5 Cantidad de contenedores Docker Edge

| Fuente | Cantidad | Servicios listados |
|---|---|---|
| Cap 4 intro | **6** microservicios | Bridge CoAP-MQTT, Mosquitto, PostgreSQL+TimescaleDB, Node-RED, Grafana, ThingsBoard Edge 3.6.2 |
| Cap 4 detalle (§4.5) | **7** contenedores | tb-edge-core, js-executor, PostgreSQL+TimescaleDB, Redis, Mosquitto, tb-edge-transport-mqtt, Nginx |
| Cap 7 (conclusiones) | **7** servicios | ThingsBoard Edge, PostgreSQL+TimescaleDB, **Apache Kafka**, **Zookeeper**, IEEE 2030.5 Server, MQTT Bridge, **Ollama LLM** |

**Diagnóstico:** Tres listas completamente distintas. Cap 4 intro incluye Node-RED y Grafana (que no aparecen en la lista detallada). Cap 7 incluye Kafka, Zookeeper y Ollama LLM (¡no existen en Cap 3-5!). Cap 4 detalle (7 containers) parece ser la lista correcta para Edge.

### 3.6 Cantidad de gateways RPi4

| Fuente | Cantidad |
|---|---|
| Cap 4 intro (§4.1) | **2** RPi4 gateways |
| Cap 4 conclusiones | **3** Gateways |
| Cap 5 (§5.1) | **3** Gateways Edge |
| Cap 6 | **1** Gateway |

**Diagnóstico:** 2 vs 3 vs 1 — sin coherencia.

### 3.7 RAM del RPi4

| Fuente | RAM |
|---|---|
| Cap 4 (validación) | RPi4 **2GB** |
| Cap 4 (Docker resources) | Límites suman >3GB (1.5GB Postgres + 1GB tb-edge + 512MB js-executor + 256MB Redis + 256MB Mosquitto = 3.5GB) |
| Cap 6 | RPi4 **4GB RAM** |

**Diagnóstico:** Las resource limits configuradas (3.5 GB) son imposibles en un RPi4 2GB. El piloto real (Cap 6) usó 4GB, pero Cap 4 validación menciona 2GB. Corregir a 4GB en todo el documento.

### 3.8 Plataforma gateway: RPi OS vs OpenWRT

| Fuente | Plataforma |
|---|---|
| Cap 3-4-5 (activos) | Raspberry Pi OS + Docker Compose |
| Cap 7 (conclusiones) | **OpenWRT** con UCI, mwan3, etc. |

**Diagnóstico:** Cap 7 describe una plataforma completamente diferente (OpenWRT con Morse Micro MM6108 SPI, UCI bridge, mwan3 failover) que NO corresponde a los capítulos de implementación activos. Cap 7 parece pertenecer a una versión anterior de la tesis.

### 3.9 Latencia E2E

| Fuente | Valor | Método |
|---|---|---|
| Cap 3 | 45.2 ms (solo Thread local) | Medido 500 muestras |
| Cap 4 | 248 ms (estimado E2E completo), 380 ms alarm E2E | Latency budgeting |
| Cap 5 | 248 ms E2E | Referencia Cap 4 |
| Cap 7 (conclusiones) | **672 ms** vs 3247 ms baseline | Benchmark diferente |

**Diagnóstico:** 248 ms (Caps 4-5) vs 672 ms (Cap 7) para la misma arquitectura. Cap 7 pertenece a una iteración diferente del diseño.

### 3.10 Objetivos en Cap 7 no cubiertos en Cap 3-5

Los siguientes objetivos se declaran "CUMPLIDOS" en Cap 7 pero **no tienen cobertura** en los capítulos de implementación activos:

| Objetivo | Contenido Cap 7 | ¿En Cap 3-5? |
|---|---|---|
| OE4 (IEEE 2030.5) | Servidor Python/Flask con Function Sets DCAP, Time, ED, MUP | **NO** — solo en viejo `03Gateway.tex` |
| OE5 (Multi-WAN mwan3) | Configuración mwan3 3 interfaces, failover 3.2s | **NO** — incompatible con RPi OS |
| OE6 (Inferencia edge Ollama) | Llama 3.2 3B, MCP Server Python, 5 herramientas ThingsBoard | **NO** — en ningún capítulo |
| OE8 (Benchmark) | Comparación vs AWS IoT Core, Node-RED | **NO** — parcial en Cap 6 |

---

## 4. CONTENIDO DUPLICADO / SUPERPUESTO

### 4.1 ThingsBoard Edge: Cap 4 §4.5 vs Cap 5 generalidades

Cap 4 (§4.5) describe ThingsBoard Edge en detalle en RPi4 (1467 líneas incluyendo Docker, Rule Chains). Cap 5 describe ThingsBoard Server Cloud. Sin embargo:

- **Tabla de comparación Edge vs Cloud** aparece en **ambos** capítulos (Tab edge-vs-cloud-architecture en Cap 4, Tab tbedge-comparison en Cap 4 §4.6 — tabla duplicada dentro del mismo capítulo).
- Cap 4 tiene **dos tablas comparativas** TB Edge vs Greengrass vs Azure IoT Edge (Tablas `tab:edge-features-comparison` y `tab:tbedge-comparison`): son casi idénticas con columnas distintas.

### 4.2 Alarmas: Cap 3 §3.5 vs Cap 4 §4.5.3

- Cap 3 define alarm rules en Device Profile: voltage sag <207V, voltage swell >253V, over-current >25A, device offline >1800s.
- Cap 4 Rule Chain 2 define: voltage sag <207V, voltage swell >253V, tamper event, device offline >30 min.
- Las definiciones son similares pero no idénticas (Cap 3 tiene over-current y over-temperature, Cap 4 tiene tamper).
- No queda claro cuáles se procesan en Edge y cuáles en Cloud.

### 4.3 Rule Chains: Cap 4 §4.5.3 vs Cap 5 §5.5

- Cap 4 describe 4 Rule Chains Edge: Telemetry Filtering, Alarm Detection, Data Enrichment, Downlink Commands.
- Cap 5 describe Rule Chains Cloud: Telemetry Processing, Billing Chain, Alarm Processing (escalation), Data Export.
- Hay solapamiento en telemetría y alarmas (ambos capítulos procesan los mismos flujos). La "Regla de Exclusión" no se aplica aquí porque son niveles diferentes (Edge vs Cloud), pero la frontera de responsabilidad no está bien delimitada explícitamente.

---

## 5. FLUJO LÓGICO Cap 3 → 4 → 5

### Evaluación de la transición

| Transición | Calidad | Observaciones |
|---|---|---|
| Cap 3 → Cap 4 | **BUENA** | Cap 3 concluye con "Transición al Capítulo 4" explícita. Aplica "Regla de Exclusión" (Thread no se repite). |
| Cap 4 → Cap 5 | **BUENA** | Cap 4 concluye con "Transición al Capítulo 5" explícita. Aplica "Regla de Exclusión" (HaLow/OTBR no se repiten). |
| Cap 3+4+5 → Cap 7 | **MALA** | Cap 7 (Conclusiones) describe una arquitectura DIFERENTE (OpenWRT, Ollama, IEEE 2030.5 server Flask, Morse Micro MM6108) que no corresponde a lo implementado en Cap 3-5. |

### Jerarquía de niveles

La estructura por niveles funciona bien conceptualmente:
- **Nivel 1 (Cap 3)**: Nodo sensor → Border Router (Thread)
- **Nivel 2-3 (Cap 4)**: Border Router → Gateway edge (HaLow mesh + ThingsBoard Edge)
- **Nivel 4 (Cap 5)**: Gateway → Cloud (Kafka + ThingsBoard Server)

Sin embargo, hay mezcla de niveles:
- Cap 3 ya habla de ThingsBoard Device Profile (que es concepto de Edge/Cloud), porque define mapeo LwM2M → ThingsBoard.
- Cap 4 incluye validación 90 días con 64-192 devices (anticipando Cap 6 Resultados).
- Cap 5 intro menciona "192 medidores piloto" como si ya estuvieran desplegados.

---

## 6. INVENTARIO DE ARCHIVOS DEPRECATED/BACKUP

### 6.1 Archivos con duplicados exactos (deben eliminarse)

| Archivo | Líneas | Duplicado de | Evidencia |
|---|---|---|---|
| `03Gateway_backup.tex` | 3757 | `03Gateway.tex` | `diff` = 0 diferencias |
| `04Arquitectura_backup.tex` | 364 | `04Arquitectura.tex` | `diff` = 0 diferencias |
| `05Conclusiones.tex` | 1445 | `07Conclusiones_TrabajoFuturo.tex` | `diff` = 1 diferencia tipográfica (comillas) |

### 6.2 Archivos old-structure (no usados, estructura anterior)

| Archivo | Líneas | Era | Contenido |
|---|---|---|---|
| `03Gateway.tex` | 3757 | Cap 3 (old) | Gateway completo monolítico: IEEE 2030.5, ISO 30141, Docker, TLS, HaLow, OTBR, todo junto |
| `04Arquitectura.tex` | 364 | Cap 4 (old) | Arquitectura high-level 4 capas (solapaba con `03Arquitectura_REFACTORED.tex`) |
| `03Arquitectura_REFACTORED.tex` | 450 | Cap 3 (refactored) | Arquitectura 4 niveles jerárquicos refactorizada (con Itron SL7000, Landis+Gyr E650, fibra GPON) |
| `04Implementacion_NEW.tex` | 970 | Cap 4 (new) | Implementación por 4 niveles. **Encoding corrupto** (UTF-8 garbled → ÃÂ). BoM contradictorio (192 medidores pero 30 ESP32) |
| `05Resultados_NEW.tex` | 739 | Cap 5 (old) | Resultados piloto 30 medidores, Itron SL7000, ESP32-S3 como DCU. **Datos consistentes con Cap 6 activo** |

### 6.3 Archivos comprimidos/optimizados (no usados)

| Archivo | Líneas | Contenido |
|---|---|---|
| `03Gateway_compressed.tex` | 193 | Versión reducida del gateway (resumen extremo) |
| `03Gateway_optimized.tex` | 756 | Versión media del gateway |
| `04Arquitectura_compressed.tex` | 214 | Versión reducida de arquitectura |

### 6.4 Archivos secciones nuevas independientes (no usados como include)

| Archivo | Líneas | Contenido |
|---|---|---|
| `05Conclusiones_SECCION_5_5_NUEVA.tex` | 488 | Análisis TCO 10 años, 10K medidores. **Valor potencial** pero no incluido en ningún capítulo activo |
| `02MarcoTeorico_SECCION_2_3_NUEVA.tex` | — | Fragmento nuevo para marco teórico |
| `02MarcoTeorico_SECCION_2_4_1_NUEVA.tex` | — | Fragmento nuevo para marco teórico |

### 6.5 Section stubs (1 línea cada uno, no usados)

- `03Seccion03.tex`, `04Seccion04.tex`, `05Seccion05.tex`, `06Seccion06.tex`, `07Seccion07.tex`

**Total archivos deprecated/backup: ~15 archivos, ~12,700 líneas de código LaTeX muerto.**

---

## 7. PROBLEMAS ESPECÍFICOS DENTRO DE CADA CAPÍTULO

### Capítulo 3

1. **Inconsistencia I2C vs RS-485**: Conclusiones (§3.7) dicen "sensor I2C 4.2 ms" en breakdown latencia, pero todo el capítulo usa RS-485 (9600 bps). El valor correcto en §3.6 es "DLMS polling RS-485: 8.5 ms".
2. **Recursos LwM2M**: Conclusión dice "8 recursos LwM2M" pero la tabla §3.5 mapea 26 recursos.
3. **Objetos IPSO**: Conclusión referencia "4 objetos IPSO: 3305, 3312, 3316, 3320" pero el cuerpo del capítulo usa Object 10242 personalizado (no IPSO estándar).
4. **Inconsistencia tareas FreeRTOS**: §3.4 dice "4 FreeRTOS tasks" pero conclusión dice "5 tareas FreeRTOS".

### Capítulo 4

1. **Monolítica vs Docker**: §4.5 describe arquitectura "monolítica single JVM" para ThingsBoard Edge, pero inmediatamente lista 7 containers Docker. Ambas afirmaciones no pueden ser ciertas simultáneamente. ThingsBoard Edge corre como monolito en 1 container; los otros 6 son servicios de apoyo.
2. **Intro dice 6 microservicios, detalle lista 7**: La intro incluye Node-RED y Grafana; el detalle no.
3. **Doble tabla comparativa**: `tab:edge-features-comparison` y `tab:tbedge-comparison` son casi idénticas (TB Edge vs Greengrass vs Azure IoT Edge). Eliminar una.
4. **Sección §4.5 demasiado larga**: ~600 líneas solo para ThingsBoard Edge features — podría ser un capítulo propio si el capítulo resulta excesivo.

### Capítulo 5

1. **§5.3.2 Docker Compose VACÍA**: Solo tiene un comentario TODO.
2. **§5.6 APIs VACÍA**: Duplica header de §5.5 que ya tiene el contenido.
3. **TODO en §5.2**: Comentario `% TODO: MIGRAR desde 04Implementacion_NEW.tex §4.5` — migración nunca completada.
4. **Comparación AWS vs On-premise contradice decisión**: Tabla muestra TCO 3 años on-premise $7,520 vs AWS $15,840 (AWS 2× más caro), pero el texto selecciona AWS "por CAPEX \$0".

---

## 8. RECOMENDACIONES PRIORIZADAS

### URGENTE (pre-defensa)

1. **Alinear Cap 7 con Cap 3-5**: Cap 7 (Conclusiones) describe una tesis diferente. Debe reescribirse para reflejar la implementación actual (RPi OS + Docker, no OpenWRT; sin Ollama LLM; sin servidor IEEE 2030.5 Flask dedicado; ThingsBoard 3.6.2 no 3.6.0).

2. **Normalizar cifras del piloto**: Definir en un solo lugar: "piloto 30 medidores instrumentados de un complejo de 192 viviendas, con capacidad de expansión a 100+". Buscar/reemplazar toda instancia de "192 medidores piloto" → "192 viviendas (30 instrumentadas en piloto)".

3. **Unificar modelo de hardware**: Elegir entre ESP32-C6-DevKitC-1 y XIAO ESP32C6 (son boards distintas). Si el piloto usó XIAO, corregir introducción y pruebas.

4. **Unificar modelo de medidor**: Elegir entre Emsitech P2000-T y Itron SL7000. Si cambió entre diseño (P2000-T) y piloto (SL7000), documentar explícitamente.

5. **Completar §5.3.2** (Docker Compose Cloud) y **eliminar §5.6** (duplicada).

### IMPORTANTE (calidad)

6. **Corregir conclusiones Cap 3**: "8 recursos" → "26 recursos", "I2C" → "RS-485", "4 objetos IPSO" → "1 objeto custom 10242", "4 tareas" → consistente con cuerpo.

7. **Corregir RAM RPi4**: 2GB → 4GB en todo Cap 4 (los Docker resource limits demandan >3.5GB).

8. **Eliminar tabla duplicada en Cap 4**: `tab:tbedge-comparison` repite `tab:edge-features-comparison`.

9. **Eliminar archivos backup idénticos**: 03Gateway_backup.tex, 04Arquitectura_backup.tex, 05Conclusiones.tex (copia de 07).

### DESEABLE (limpieza)

10. **Mover/eliminar archivos deprecated** a carpeta `_deprecated/` o eliminarlos del repositorio.

11. **Integrar 05Conclusiones_SECCION_5_5_NUEVA.tex** (TCO 10 años) en Cap 6 Resultados o descartarlo formalmente.

12. **Refinar frontera alarmas Edge vs Cloud**: Documentar explícitamente qué alarmas se procesan en Edge (Cap 4) vs Cloud (Cap 5).

---

## 9. MATRIZ DE COHERENCIA CRUZADA

| Dato | Cap 3 | Cap 4 | Cap 5 | Cap 6 | Cap 7 | ¿Consistente? |
|---|---|---|---|---|---|---|
| Piloto size | 30 (intro) / 60 (hw) | 192 | 192 | 30 | 10 | ❌ |
| Nodo MCU | DevKitC-1 / XIAO | — | — | ESP32-C6 | ESP32-C6 | ⚠️ |
| Medidor | Emsitech P2000-T | — | — | Itron SL7000 | — | ❌ |
| RPi4 RAM | — | 2GB | — | 4GB | — | ❌ |
| TB version | — | 3.6.2 | 3.6.2 | — | 3.6.0 | ❌ |
| Docker services Edge | — | 6 / 7 | — | — | 7 (diferentes) | ❌ |
| Gateways RPi4 | — | 2 / 3 | 3 | 1 | — | ❌ |
| Plataforma GW | — | RPi OS + Docker | — | — | OpenWRT | ❌ |
| Latencia E2E | 45 ms (local) | 248 ms | 248 ms | — | 672 ms | ❌ |
| Thread Border Router | OTBR nRF52840 | OTBR nRF52840 | — | ESP32-S3 | nRF52840 RCP | ⚠️ |
| HaLow model | — | Alfa Tube AHM | — | Alfa Tube-U4AHM | — | ⚠️ |

**Leyenda:** ❌ = inconsistencia clara, ⚠️ = discrepancia menor o posible variante legítima.
