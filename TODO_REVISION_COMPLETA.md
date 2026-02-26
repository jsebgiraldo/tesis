# TODO — Revisión Completa de Tesis (rev5)
**Fecha:** 2025-01-22 | **Base:** tesis-rev5.pdf (356 pp, 0 errores LaTeX)  
**Estructura:** Ch1 Intro → Ch2 Marco → Ch3 Nodo IoT → Ch4 Gateway → Ch5 Resultados → Ch6 Conclusiones + Anexos A–H

---

## Leyenda de Severidad
- 🔴 **CRÍTICO** — Puede causar rechazo en defensa (datos falsos, contradicciones graves, código ficticio)
- 🟠 **ALTO** — Evaluador lo señalará con alta probabilidad (inconsistencias numéricas, citas faltantes clave)
- 🟡 **MEDIO** — Debilita la calidad académica (redacción, estructura, citas menores)
- 🟢 **BAJO** — Mejoras de pulido (estilo, nomenclatura)

---

## HALLAZGOS TRANSVERSALES (afectan múltiples capítulos)

### T1 🔴 Número de nodos del piloto inconsistente
- Ch3 L16: "60 nodos"
- Ch3 L757: "30 medidores"
- Ch3 L855: "100 medidores" (cálculo)
- Ch5 L40: "30 medidores"
- Ch6 L22: "10 nodos Thread"
- Ch6 L168: "12 nodos, 72 horas"
- Ch6 L175: "30 medidores"
- **FIX:** Fijar UNA cifra real y corregir en TODOS los capítulos.

### T2 🔴 Modelo de medidor contradictorio
- Ch3: Emsitech P2000-T
- Ch5 L40: Itron SL7000 (monofásicos + trifásicos)
- **FIX:** Determinar cuál es el real y corregir el otro.

### T3 🔴 OTBR: ¿ESP32-S3 standalone o nRF52840 dongle en RPi4?
- Ch4 L190: nRF52840 Dongle USB + otbr-agent en RPi4 (modo RCP)
- Ch5 L42: ESP32-S3 dual-core standalone
- **FIX:** Unificar a la arquitectura real.

### T4 🔴 Object 10242 ficticio — código usa IPSO estándar
- Ch3 L198–380: 51 recursos mapeados a Object 10242 (3-Phase Power Meter)
- Código real: Solo IPSO 3316 (Voltage), 3317 (Current), 3300 (Generic) con 6 campos
- **FIX:** O implementar Object 10242 o reescribir Ch3 con los IPSO objects reales.

### T5 🔴 Docker-compose real ≠ arquitectura descrita
- Código `codigo/docker-compose.yml`: Solo 2 servicios (Leshan + OTBR)
- Ch4: 6–7 servicios Docker (TB Edge, PostgreSQL, TimescaleDB, Mosquitto, etc.)
- Ch6 L33: "7 servicios: TB Edge, PostgreSQL, Kafka, Zookeeper, IEEE 2030.5, MQTT Bridge, Ollama"
- Anexo B: Tercera arquitectura distinta
- **FIX:** Alinear repo con tesis, o documentar que el repo es versión dev simplificada.

### T6 🔴 Latencia E2E: 248 ms vs 672 ms vs 520 ms baseline
- Ch5: baseline cloud-only = 520 ms, E2E medido = 248 ms → reducción 52%
- Ch6 L16: "672 ms vs 3,247 ms → reducción 79.3%"
- Ch5 declara H1 "PARCIALMENTE VALIDADA" (52%, no >70%)
- Ch6 declara reducción 79.3% como lograda
- **FIX:** Reconciliar un solo resultado con un solo baseline documentado.

### T7 🔴 Hipótesis H1–H8 son dos conjuntos incompatibles
- Ch5 define H1–H8 (e.g., H1=Viabilidad arquitectura, H3=Latencia Edge <10ms)
- Ch6 define H1–H8 distintas (e.g., H1=6LoWPAN reduce overhead >70%, H3=HaLow multi-banda)
- **FIX:** Unificar a un solo conjunto, idealmente definido en Ch1 y validado en Ch5.

### T8 🟠 Consumo promedio del nodo: tres cifras distintas
- Ch3 L16: 27 mA @ 5V
- Ch3 L638: 51 mA @ 5V (255 mW)
- Ch3 L1158 (tabla medida): 19.3 mA
- **FIX:** Usar valor medido (19.3 mA) en todo el documento.

### T9 🟠 Throughput HaLow varía en 5 cifras
- Ch2 L176: ~4 Mbps
- Ch2 L240 tabla: 10 Mbps
- Ch2 L662: 40 Mbps
- Ch2 L172: 86.7 Mbps (máx teórico)
- Ch1 L330 OE3: ">80 Mbps"
- **FIX:** Estandarizar: ~4 Mbps operacional (MCS7/2 MHz), 86.7 Mbps máx teórico.

### T10 🟠 FreeRTOS vs Zephyr RTOS
- Ch5 L96: "FreeRTOS firmware"
- Ch3, Ch1, código: Zephyr RTOS
- **FIX:** Cambiar "FreeRTOS" → "Zephyr RTOS" en Ch5.

### T11 🟠 Separadores decimales inconsistentes
- Todo el documento mezcla punto (inglés) y coma (español)
- **FIX:** Unificar a coma decimal (norma SI en español académico) o declarar convención.

### T12 🟡 Spanglish / Términos ingleses sin itálica
- Patrón global: "throughput", "stress test", "hot-swap", "sleep mode", "batch processing", etc. sin `\textit{}`
- "caudal (throughput)" repetido ~25 veces en Ch2
- **FIX:** Definir cada término una vez; usar solo la forma elegida en adelante.

---

## CAPÍTULO 1 — Introducción (01Introduccion.tex)

### C1-1 🔴 Errores matemáticos en TCO/costos
- L116: $2,880/mes — cálculo real da $430–860
- L157: $150,000 en 5 años para 1,000 nodos — cálculo real da $50K–75K
- L295: OPEX $180,000 implica $36/nodo/año, no $15
- **FIX:** Recalcular y corregir todas las cifras económicas.

### C1-2 🔴 HaLow throughput 80 Mbps contradicción
- L330 OE3: ">80 Mbps" a 8 MHz
- L96 tabla: 43.3 Mbps @ 8 MHz (correcto para single-stream)
- **FIX:** Corregir OE3 a ">40 Mbps" o justificar MIMO.

### C1-3 🟠 Días downtime LTE inconsistentes
- L105: "18–25 días/año" (94–96% availability)
- L147: "15–22 días" (correcto: 4–6% × 365)
- **FIX:** Cambiar L105 a "15–22 días".

### C1-4 🟠 Wi-SUN prometido pero nunca comparado
- L3 intro: promete comparar Wi-SUN
- L243: Wi-SUN solo aparece en exclusiones
- **FIX:** Agregar a tabla comparativa o eliminar del intro.

### C1-5 🟠 Escalabilidad 2,500–12,500 nodos vs limitación de 10 nodos
- L340–350 OE4: evaluación 2,500–12,500 endpoints
- L460 Limitación 1: validación con 10 nodos solo
- **FIX:** Clarificar que son proyecciones analíticas, no empíricas.

### C1-6 🟠 Mapeo de anexos incorrecto en "Organización del Documento"
- L535–540: Lista solo A–E + server, pero son A–H
- Letras no corresponden: dice C=código nodo (real: C=Scripts), dice D=OpenWRT (real: D=IEEE specs, F=OpenWRT)
- **FIX:** Reescribir listado para A–H real.

### C1-7 🟠 Referencias de fase incorrectas tras reestructuración
- L427 Fase 3: "gráficas Capítulo 4" → debería ser Cap. 5
- L438 Fase 4: "Tabla comparativa Capítulo 4" → debería ser Cap. 5
- **FIX:** Cambiar a `Capítulo~\ref{chap:resultados}`.

### C1-8 🟡 8 citas faltantes
- L59 Thread 250 nodos, L62 corrientes RX/TX, L97 pérdida propagación, L110 latencia gateway, L265 468 kbps, L267 atenuación PLC, L259 5G costos, L86–91 tabla edge platforms
- **FIX:** Agregar referencias para cada dato cuantitativo.

### C1-9 🟡 Capítulo demasiado largo (22–25 pp vs 8–12 normal)
- **FIX:** Mover tablas comparativas a Ch2, simplificar OE1–OE5, reducir metodología.

### C1-10 🟡 IEC 61000-4-30 mal interpretado (L170)
- No define latencia <100 ms de comunicación sino ventanas de medición
- **FIX:** Citar IEC 61850 GOOSE o corregir interpretación.

---

## CAPÍTULO 2 — Marco Teórico (02MarcoTeorico_Rework_V2.tex)

### C2-1 🔴 Forward references incorrectas (post-reestructuración)
- L662 Brecha 1: "Capítulo 3 implementa gateway" → es Cap. 4
- L666 Brecha 3: "Capítulo 6 presenta validación" → es Cap. 5
- L651: "Capítulos 3–6" → debería ser 3–5 (Cap. 6 = Conclusiones)
- **FIX:** Usar `\ref{chap:gateway-halow}`, `\ref{chap:resultados}`.

### C2-2 🔴 600M smart meters "con HaLow" es engañoso
- L275: Implica que 600M medidores en China usan 802.11ah
- Realidad: usan PLC/GPRS/RF mesh
- **FIX:** Reformular como "mercado potencial para migración a HaLow".

### C2-3 🔴 "Reducción 167%" matemáticamente imposible (L327)
- **FIX:** Reescribir como "62.5% menos paquetes" o verificar dato original.

### C2-4 🟠 Wi-SUN FAN omitido del estado del arte
- Competencia directa más relevante (>100M nodos desplegados, Itron/Landis+Gyr)
- **FIX:** Agregar §2.3.1.4 con comparación Thread+HaLow vs Wi-SUN FAN.

### C2-5 🟠 TCO $124/gateway no cuadra
- CAPEX hardware RPi4 = $55–75, enclosure IP67 = $80 → ya $135–155 solo en hardware
- **FIX:** Proporcionar desglose TCO completo y verificable.

### C2-6 🟠 Datos experimentales propios en Marco Teórico
- L465–467: 40 MB overhead, 53 msgs/s — son resultados, no literatura
- L508: "<15 min post-compromise" — dato propio en sección NIST CSF
- **FIX:** Mover a Ch5 Resultados.

### C2-7 🟡 Fórmula Path Cost de Thread incorrecta (L290)
- Usa LQI en vez de ETX (Thread real usa ETX)
- Cita OpenThread docs, no Thread Specification
- **FIX:** Citar Thread Group Spec 1.3.0 o marcar como simplificación.

### C2-8 🟡 Sección Seguridad demasiado genérica (~100 líneas)
- STRIDE, Stuxnet, Mirai, SQL injection — material de libro de texto
- **FIX:** Condensar a lo específico de la arquitectura propuesta.

### C2-9 🟡 Numeración duplicada §2.3.4
- Hay dos §2.3.4 (Edge vs Cloud y Plataformas Edge)
- **FIX:** Renumerar correctamente.

### C2-10 🟡 Sin referencia a Anexo H desde Ch2
- Kafka/ThingsBoard se mencionan sin apuntar al Anexo H
- **FIX:** Agregar "véase Anexo~H para detalle del servidor on-premise".

---

## CAPÍTULO 3 — Nodo IoT Thread LwM2M (03NodoIoT_Thread_LwM2M.tex)

### C3-1 🔴 dlms_parser.c/680 LOC ficticio
- L571: Describe parser HDLC binario (flag 0x7E, bit stuffing) de 680 LOC
- Realidad: meter_dlms.c (~170 LOC) implementa IEC 62056-21 Mode C (texto ASCII, `strstr()`+`strtof()`)
- **FIX:** Corregir a "meter_dlms.c, ~170 LOC, IEC 62056-21 Mode C simplificado".

### C3-2 🔴 Anjay LwM2M ficticio (L1193)
- Dice "Anjay LwM2M 3.7.0 (120 KB Flash)"
- Realidad: No hay ningún `#include <anjay>` ni dependencia a AVSystems
- Se usa subsistema LwM2M nativo de Zephyr
- **FIX:** Eliminar toda referencia a Anjay.

### C3-3 🔴 lwm2m_bridge.py 450 LOC ficticio (L1083)
- No existe en `codigo/` ni en ningún lugar del workspace
- **FIX:** Crear el script o eliminar la referencia.

### C3-4 🔴 FTD vs MED contradicción
- L135: "utiliza ESP32-C6 como MED (RX always-on)"
- Ambos prj.conf: `CONFIG_OPENTHREAD_FTD=y` (Full Thread Device, puede ser Router)
- **FIX:** Cambiar firmware a MTD o cambiar texto a "FTD/REED".

### C3-5 🟠 Protocolo "Modbus RTU" vs realidad IEC 62056-21
- L378: "Modbus RTU @ 9600 baud"
- meter_dlms.c: IEC 62056-21 Mode C (texto ASCII con `/?!\r\n`)
- meter_modbus.c: retorna datos simulados con `TODO: Implementar lectura real`
- **FIX:** Corregir a IEC 62056-21 Mode C.

### C3-6 🟠 SRAM: 437 KB vs 512 KB
- L617: "252 KB de 437 KB (57.5%)"
- L500: ESP32-C6 tiene 512 KB SRAM → 252/512 = 49.2%
- **FIX:** Usar 512 KB o justificar el 437 KB.

### C3-7 🟠 TX Power: 3 cifras distintas
- L606: +4 dBm, L625: 21 dBm, código prj.conf: 8 dBm
- **FIX:** Unificar al configurado (8 dBm).

### C3-8 🟠 Thread version inconsistente entre proyectos
- lwm2m-thread-app: Thread 1.4
- zephyr-app: Thread 1.3
- Texto dice "Thread 1.3" consistentemente
- **FIX:** Unificar versión Thread entre proyectos y texto.

### C3-9 🟠 Sensibilidad -100 dBm vs -103 dBm
- L56: -100 dBm, L502: -103 dBm — sin cita a datasheet
- **FIX:** Citar datasheet ESP32-C6 y homogeneizar.

### C3-10 🟡 Mapeo OBIS duplicado (§3.2.2 + §3.4.2)
- >100 líneas de mapeo OBIS→LwM2M + tabla duplicada
- **FIX:** Consolidar en una sección y mover detalle a Anexo.

### C3-11 🟡 DTLS deshabilitado pero no documentado
- prj.conf: `# CONFIG_LWM2M_DTLS_SUPPORT=y` (comentado = NoSec)
- lwm2m-thread-app: configura modo NoSec (security mode = 3)
- **FIX:** Agregar subsección explicando NoSec para prototipo.

### C3-12 🟡 Snippet con datos simulados presentado como producción
- L592: lwm2m-thread-app usa datos simulados (`218.0 + random`)
- **FIX:** Aclarar que es versión simplificada; producción = zephyr-app con DLMS.

### C3-13 🟡 Stack size: texto dice 4 KB, código dice 8 KB
- L583: "main (4 KB)", código: MAIN_STACK_SIZE=8192
- **FIX:** Corregir a 8 KB.

---

## CAPÍTULO 4 — Gateway HaLow Edge (04Gateway_HaLow_Edge.tex)

### C4-1 🔴 RouterOS (MikroTik) commands en capítulo "OpenWRT"
- L666–740: Sintaxis RouterOS (`/interface wireless set`, `/interface mesh add`)
- L680 título: "OpenWRT 23.05"
- RouterOS y OpenWRT son SO completamente diferentes
- **FIX:** Reescribir en UCI/OpenWRT syntax o cambiar texto a RouterOS.

### C4-2 🔴 RAM limits suman 4.75 GB en dispositivo de 4 GB
- L130–148: CPU 6.0 cores (texto dice 5.0), RAM 4,864 MB (texto dice 3.5 GB)
- Excede capacidad física del RPi4
- **FIX:** Recalcular y reconocer overcommit.

### C4-3 🔴 Tres listas de contenedores mutuamente contradictorias
- L91: 6 servicios (bridge, mosquitto, postgres, nodered, grafana, tb-edge)
- L873: 7 internos TB Edge (tb-edge-core, js-executor, postgres, redis, mosquitto, mqtt-transport, nginx)
- L1411: 6 distintos (OTBR, TB Edge, PostgreSQL+TimescaleDB, Kafka, Zookeeper, MQTT Bridge)
- PostgreSQL y Mosquitto con specs diferentes en cada lista
- **FIX:** Una sola lista canónica.

### C4-4 🟠 LTE Cat-4 vs Cat-M1
- L57: "Quectel EG25-G LTE Cat-4" (150 Mbps)
- L210: "LTE Cat-M1 Quectel" (1 Mbps)
- **FIX:** Elegir uno (EG25-G = Cat-4).

### C4-5 🟠 RPi4 RAM: 4 GB vs 2 GB
- L47: "4 GB RAM", L1382: "2 GB"
- **FIX:** Confirmar modelo real y unificar.

### C4-6 🟠 Costo gateway: $127 vs $35 vs $120
- L67: $127/gateway (BOM detallada)
- L1374: "CAPEX $35"
- L1411: "$120 hardware"
- **FIX:** Usar $127 (BOM itemizada) en todo.

### C4-7 🟠 CPU utilización: 42% vs 35%
- L148: "42% promedio", L1395: "35% promedio"
- **FIX:** Usar un solo valor medido.

### C4-8 🟠 Dispositivos por gateway: 15 vs 64
- L30: "15 nodos Thread locales" (30 total)
- L1382: "64 devices" por gateway (192 total)
- **FIX:** Clarificar piloto (30) vs diseño target (64/192).

### C4-9 🟠 Dos stacks Docker paralelos (§4.2 vs §4.4)
- §4.2 y §4.4 describen stacks solapados con specs distintas
- **FIX:** UNA arquitectura canónica con tabla servicios→contenedores.

### C4-10 🟠 ns-3 simulación sin metodología (L647–658)
- "30 STAs transmitiendo simultáneamente" sin params ni scripts
- **FIX:** Agregar metodología o eliminar.

### C4-11 🟡 Falta `\section{}` para conclusiones del capítulo
- L1406: Label `sec:gateway-conclusiones` sin heading
- **FIX:** Agregar `\section{Síntesis y Transición}`.

### C4-12 🟡 Tablas duplicadas TB Edge comparison
- L1345 y L1408: tablas casi idénticas (TB Edge vs Greengrass vs Azure)
- **FIX:** Eliminar una.

### C4-13 🟡 §4.4 ThingsBoard Edge excesivamente largo (~595 líneas)
- **FIX:** Mover minutiae (Brotli, VACUUM, gRPC) a Anexo B.

### C4-14 🟡 "72% WAN reduction" repetido 6 veces
- **FIX:** Declarar una vez, referenciar en adelante.

### C4-15 🟡 8 citas faltantes (60% cost, 99.7% pilot, electricity rate, benchmark comparisons, Cat6 cost, RAW 60%, ANE 711, NCP latency)

---

## CAPÍTULO 5 — Resultados y Validación (06Resultados_Validacion.tex)

### C5-1 🔴 8 secciones TODO vacías (~35% del capítulo)
- §5.4 Eficiencia Energética, §5.5 Costos, §5.6 Disponibilidad H6, §5.7 Casos de Uso, §5.8 Discusión — solo comentarios TODO
- La Validación de Hipótesis (§5.9) referencia resultados de estas secciones inexistentes
- **FIX:** Completar o eliminar secciones + ajustar hipótesis.

### C5-2 🔴 Referencias a Kafka/TimescaleDB/servidor on-premise
- L135: "ingesta Kafka + escritura TimescaleDB" en pipeline E2E
- L730–734 TODO: "Servidor: $2,400 (PC workstation)"
- Contradice decisión de usar ThingsBoard Cloud como Nivel 4
- **FIX:** Purgar refs on-premise; redirigir a Anexo H.

### C5-3 🔴 Breakdown latencia circular (L204–206)
- Suma teórica 8+4+6+8+210+12 = 248 ms "coincide con medición" — tautológico
- **FIX:** Medir E2E independientemente primero, luego comparar con suma de segmentos.

### C5-4 🟠 Extrapolación lineal declarada como "VALIDADA"
- L345–350: Escalabilidad 100+ medidores "validada" por extrapolación desde 30
- **FIX:** Cambiar "VALIDADA" → "estimada/proyectada".

### C5-5 🟠 Incertidumbre NTP ±5 ms vs latencias de 6–14 ms
- Error de medición 35–83% del valor medido
- **FIX:** Discutir incertidumbre explícitamente.

### C5-6 🟠 Reducción WAN 72% calculada sobre 192 medidores teóricos
- L1041: msgs_esperados = 192 × 96 = 9,600/día, pero piloto = 30 medidores
- **FIX:** Recalcular contra baseline de 30 medidores.

### C5-7 🟠 Disponibilidad 99.62% con solo n=5 eventos en 90 días
- Intervalo de confianza enorme para MTBF
- **FIX:** Agregar intervalo de confianza y reconocer limitación.

### C5-8 🟠 Tres modelos TCO contradictorios en mismo capítulo
- $107/med (§5.3.5), $124/med (§5.4), $174/med (TODO §5.5)
- **FIX:** Consolidar en uno solo.

### C5-9 🟠 HaLow params inconsistentes con Ch4
- Ch5: "920 MHz @ 2 MHz BW, single-hop"
- Ch4: "902–928 MHz @ 4 MHz BW, mesh"
- **FIX:** Unificar.

### C5-10 🟡 esp_timer_get_time() (ESP-IDF) vs Zephyr
- L192: API ESP-IDF, pero Ch3 dice Zephyr RTOS
- **FIX:** Aclarar framework real o usar k_uptime_get().

### C5-11 🟡 Object ID: 10242 vs 10243
- Ch3: Object 10242 (3-Phase Power Meter)
- Ch5 L1282: Object 10243 (Single-Phase Meter)
- **FIX:** Verificar en OMA Registry y unificar.

### C5-12 🟡 wpantund0 obsoleto (deprecado por otbr-agent/wpan0)
- Ch5 L345 usa wpantund0; Ch4 L244 usa wpan0
- **FIX:** Unificar a wpan0/otbr-agent.

### C5-13 🟡 "Thanksgiving streaming" como causa de interferencia
- Thanksgiving no se celebra en Colombia
- **FIX:** Reemplazar por "evento masivo streaming HD en horario pico".

---

## CAPÍTULO 6 — Conclusiones (07Conclusiones_TrabajoFuturo.tex)

### C6-1 🔴 Objetivos inflados de 5 a 8
- Ch1 define OE1–OE5; Ch6 agrega OE6 (Inferencia edge), OE7 (Caso estudio), OE8 (Eval. comparativa)
- **FIX:** Eliminar OE6–OE8 o agregarlos primero en Ch1.

### C6-2 🔴 79.3% reducción latencia vs 52% en Ch5
- Ch6 L16: "79.3% (672 ms vs 3,247 ms)"
- Ch5: 52% (520 ms → 248 ms), H1 "PARCIALMENTE VALIDADA"
- Baseline 3,247 ms no aparece en Ch5
- **FIX:** Usar resultado de Ch5 (52%) o documentar segundo baseline.

### C6-3 🔴 L6 duplicada verbatim (~200 líneas)
- L1088–1280 duplica L1280–1464 (secciones L6.1, L6.2, L6.3 dos veces)
- **FIX:** Eliminar la primera copia (con encabezado incorrecto "Línea 1").

### C6-4 🔴 Conclusiones sobre TODOs vacíos de Ch5
- Ch6 concluye sobre eficiencia energética, costos, disponibilidad, casos de uso
- Esas secciones en Ch5 están vacías (solo TODO)
- **FIX:** Completar Ch5 primero o limitar conclusiones a lo documentado.

### C6-5 🟠 OE3 "CUMPLIDO" — stack 7 servicios Docker ficticio
- L33–34: Declara stack con Kafka, Zookeeper, IEEE 2030.5, Ollama
- Nada de esto existe en el código
- **FIX:** Alinear con lo realmente implementado.

### C6-6 🟠 OE4 IEEE 2030.5 "CUMPLIDO" sin validación en Ch5
- L37–38: "Validación interoperabilidad OpenADR VTN"
- Ch5 no tiene sección sobre IEEE 2030.5
- **FIX:** Agregar evidencia o rebajar conclusión.

### C6-7 🟠 Disponibilidad 99.7% vs 99.62% medido
- L20: "99.7%", Ch5 L793: "99.62%"
- **FIX:** Usar valor medido.

### C6-8 🟠 Referencias "Cap. 4" en tabla hipótesis → deberían ser Cap. 5
- L73, L75, L77, L78, L79: Resultados atribuidos a Cap. 4 (Gateway)
- Resultados = Cap. 5
- **FIX:** Actualizar todas las cross-refs.

### C6-9 🟠 Capítulo 3× más largo que meta (~45 pp vs ~15 pp)
- **FIX:** Mover escalabilidad 10K (§4, 320 líneas) y impacto social (§5, 300 líneas) a Anexos.

### C6-10 🟡 11 citas faltantes (CEPAL, GSMA, Carbon Trust, EPA, Ericsson, OLADE, Allcott & Rogers, OpenADR Alliance, Banco Mundial, IEA)
- **FIX:** Crear entradas BibTeX para cada fuente.

### C6-11 🟡 Tono promocional en reflexiones finales
- L1451–1464: "verdaderos 'nervous systems' distribuidos"
- **FIX:** Reescribir en tono académico.

---

## DISCREPANCIAS CÓDIGO vs TESIS (Resumen)

| # | Descripción | Archivo tesis | Código real | Severidad |
|---|---|---|---|---|
| 1 | Object 10242 (51 recursos) | Ch3 L198–380 | IPSO 3316/3317/3300 (6 campos) | 🔴 |
| 2 | dlms_parser.c 680 LOC HDLC | Ch3 L571 | meter_dlms.c ~170 LOC IEC 62056-21 | 🔴 |
| 3 | Anjay LwM2M 3.7.0 | Ch3 L1193 | Zephyr LwM2M nativo | 🔴 |
| 4 | lwm2m_bridge.py 450 LOC | Ch3 L1083 | No existe | 🔴 |
| 5 | Docker 6–7 servicios | Ch4 L91, L873 | 2 servicios (Leshan+OTBR) | 🔴 |
| 6 | FTD vs MED | Ch3 L135 "MED" | prj.conf: `FTD=y` | 🟠 |
| 7 | Thread 1.3 vs 1.4 | Ch3 "Thread 1.3" | lwm2m-app: 1.4, zephyr-app: 1.3 | 🟠 |
| 8 | TX Power 4/21/8 dBm | Ch3 L606/L625 | prj.conf: 8 dBm | 🟠 |
| 9 | Modbus RTU | Ch3 L378 | IEC 62056-21 Mode C | 🟠 |
| 10 | UART baudrate 115200 | Ch4 L240 | docker-compose: 1000000 | 🟠 |
| 11 | Thread prefix fd11:22::/64 | Ch4 | docker-compose: fd00:db8:ami::/64 | 🟠 |
| 12 | DTLS habilitado | Ch3 L574 | prj.conf: comentado (NoSec) | 🟡 |
| 13 | Main stack 4 KB | Ch3 L583 | MAIN_STACK_SIZE=8192 | 🟡 |

---

## RESUMEN ESTADÍSTICO

| Severidad | Ch1 | Ch2 | Ch3 | Ch4 | Ch5 | Ch6 | Trans. | **Total** |
|---|---|---|---|---|---|---|---|---|
| 🔴 CRÍTICO | 2 | 3 | 4 | 3 | 3 | 4 | 7 | **26** |
| 🟠 ALTO | 5 | 3 | 5 | 7 | 7 | 5 | 4 | **36** |
| 🟡 MEDIO | 3 | 5 | 4 | 5 | 4 | 2 | 1 | **24** |
| 🟢 BAJO | — | — | — | — | — | — | — | — |
| **Total** | **10** | **11** | **13** | **15** | **14** | **11** | **12** | **86** |

---

## PLAN DE ACCIÓN SUGERIDO (Prioridad)

### Fase 1 — CRÍTICOS (1–2 semanas)
1. Unificar # nodos, modelo medidor, OTBR tipo (T1, T2, T3)
2. Corregir discrepancias código: Object 10242 → IPSO, dlms_parser → meter_dlms, eliminar Anjay y lwm2m_bridge.py (C3-1 a C3-4)
3. Reconciliar docker-compose con tesis o viceversa (T5)
4. Fijar un solo conjunto de hipótesis H1–H8 (T7)
5. Reconciliar latencia E2E 248/672/520 ms y % reducción (T6)
6. Eliminar duplicado L6 en Ch6 (C6-3)
7. Corregir RouterOS vs OpenWRT (C4-1)
8. Corregir RAM overcommit 4.75 GB (C4-2)

### Fase 2 — ALTOS (1–2 semanas)
9. Completar u eliminar ~8 secciones TODO de Ch5 (C5-1)
10. Purgar referencias on-premise/Kafka de Ch5 (C5-2)
11. Corregir forward refs Ch2 (C2-1) y Ch6 (C6-8)
12. Alinear OE count: 5 vs 8 (C6-1)
13. Corregir cálculos TCO en Ch1 (C1-1)
14. Agregar Wi-SUN al estado del arte (C2-4)
15. Unificar throughput HaLow (T9) y consumo nodo (T8)
16. Corregir FreeRTOS → Zephyr (T10)

### Fase 3 — MEDIOS (1 semana)
17. Agregar ~30 citas faltantes distribuidas en todos los capítulos
18. Reducir extensión Ch1 (22→12 pp) y Ch6 (45→15 pp)
19. Mover mapeo OBIS detallado a Anexo (C3-10)
20. Documentar DTLS NoSec como decisión de prototipo (C3-11)
21. Unificar separadores decimales (T11)
22. Hispanizar Spanglish sistemático (T12)

---

*Documento generado automáticamente por auditoría de subagentes. Revisar cada hallazgo contra el PDF compilado (tesis-rev5.pdf) antes de implementar correcciones.*
