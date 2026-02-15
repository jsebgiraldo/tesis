# Auditoría de Citas Faltantes — Capítulos 1, 4, 5, 6

**Fecha:** 2025-01-XX  
**Estándar de referencia:** IEEE Editorial Style (toda afirmación fáctica requiere `\cite{}`)  
**Severidad:** CRITICAL (dato cuantitativo sin fuente), HIGH (especificación técnica sin referencia), MEDIUM (afirmación general sin soporte)

---

## Resumen Ejecutivo

| Capítulo | Líneas | `\cite{}` actuales | Gaps identificados | Severidad dominante |
|----------|--------|---------------------|---------------------|---------------------|
| Cap 1 — Introducción | 639 | 10 | ~38 | HIGH |
| Cap 4 — Gateway + HaLow | 1,467 | 4 | ~62 | CRITICAL |
| Cap 5 — Server ThingsBoard + Kafka | 955 | 0 | ~55 | CRITICAL |
| Cap 6 — Resultados y Validación | 1,405 | 2 | ~42 | HIGH |
| **TOTAL** | **4,466** | **16** | **~197** | **CRITICAL** |

---

## Capítulo 1 — Introducción (`01Introduccion.tex`)

**Citas actuales (10):** `velasquezSmartGridsEmpowered2024`, `SmartHomeEnergy2024`, `alsafranChallengesImplementingIoT2025`, `abdulsalamOverviewRecentWireless2024`, `dianeSystematicComprehensiveReview2025`, `alsuwaidiSecuringSmartGrid2024`, `ashfaqIoTSensorNetworks2024`, `choudharyInternetThingsComprehensive2024`, `amrilPerformanceEvaluationWiFi2025`, `IEEE802.11ah-2016`

### CRITICAL — Datos cuantitativos sin fuente

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C1.1 | ~12 | "15 petabytes de datos de telemetría diarios" (IEA projection) | Reporte técnico IEA |
| C1.2 | ~142 | "disponibilidades de 94-96% en conectividad celular LTE (downtimes acumulados 18-25 días/año)" | Paper / reporte telecomunicaciones |
| C1.3 | ~144 | "Plataformas cloud IoT típicamente cobran \$5-10 por millón de mensajes" | Pricing docs AWS/Azure/GCP |
| C1.4 | ~144 | "\$2,880/mes solo en ingesta" (cálculo derivado de precio sin fuente) | Pricing docs |
| C1.5 | ~146 | "overhead 150-300 ms" acumulado multi-protocolo | Paper medición latencias |
| C1.6 | ~278 | NB-IoT throughput "250 kbps nominal (Release 13)" y déficit 62% | 3GPP TS 36.306 o spec NB-IoT |
| C1.7 | ~290 | "5G OPEX \$15-25/dispositivo/mes tarifa MVNO típica Latinoamérica" | Reporte operador o GSMA |
| C1.8 | ~296 | "Small cells \$15K-25K/celda" para 5G indoor | Vendor datasheet / market report |
| C1.9 | ~305 | "PLC atenuación 40-60 dB por transformador" y "bypass \$500-1,200/unidad" | Paper PLC / datasheet coupling device |
| C1.10 | ~308 | "PLC atenuación 60-90 dB/km" | Paper PLC propagation |
| C1.11 | ~313 | "Zigbee 3.0 latencia +15-30 ms por hop" que acumula +45-150 ms | Paper medición Zigbee |
| C1.12 | ~328 | "TCO Cloud-centric+LTE \$330k vs Propuesta HaLow \$225k" para 1,000 medidores 5 años | Cálculo propio → documentar fuentes de cada componente |
| C1.13 | ~460 | "FTTH Council Americas 2024, penetración fibra 38% en 2023, 52% para 2026" | Reporte FTTH Council Americas 2024 |
| C1.14 | ~463 | "2.5 Gbps downstream GPON compartidos entre 32 ONTs" | ITU-T G.984 o spec GPON |
| C1.15 | ~465 | "\$8-12/mes por enlace LTE" | Pricing operador específico |
| C1.16 | ~475 | "HaLow ~10 dB menor path loss sub-GHz vs 2.4 GHz" | Paper propagación RF o IEEE 802.11ah spec |
| C1.17 | ~480 | "Chipsets Morse Micro MM6108 disponibles desde 2023, Vantron RAH305 \$600-800" | Vendor datasheet Morse Micro / Vantron |
| C1.18 | ~482 | "ahorro CAPEX -\$2,500 vs Ethernet PoE" | Estimación → necesita fuente de costo cableado |
| C1.19 | ~486 | "Raspberry Pi CM4: \$35-45, x86 mini-PCs: \$120-180" | Vendor pricing |
| C1.20 | ~488 | "72% reducción tráfico WAN (24.6 GB/día → 6.9 GB/día)" y "\$400/mes → \$112/mes AWS" | AWS pricing / medición propia → citar cap resultados |
| C1.21 | ~492 | "Llama 3.2 3B, 2GB RAM, latencia <500ms" | Meta AI / paper LLM |

### HIGH — Datos de tablas comparativas sin fuentes por celda

| # | Líneas aprox. | Tabla | Datos sin cita |
|---|---------------|-------|----------------|
| C1.22 | ~38-75 | Tabla 1 (mesh comparison) | Thread latencia 40-60 ms, Zigbee 80-120 ms, BLE 100-200 ms; consumo RX/TX 19/22 mA vs 24/31 mA vs 9.2/10.5 mA; sleep current 5 µA ESP32-C6, 10 µA Zigbee, 2 µA nRF52840 | Datasheets ESP32-C6, nRF52840; papers Thread/Zigbee/BLE benchmarks |
| C1.23 | ~78-104 | Tabla 2 (edge platforms) | ThingsBoard 1-4 GB, Greengrass 512 MB-2 GB, Azure 256 MB-1 GB, Node-RED 128-512 MB; "sincronización unidireccional" Greengrass | Vendor docs ThingsBoard/AWS/Azure/Node-RED |
| C1.24 | ~106-132 | Tabla 3 (last-mile) | HaLow 43.3 Mbps, throughput/latencia/alcance cada tecnología, consumo TX 180/120/220/350 mA, costo por nodo \$25-40/\$8-15/\$12-25/\$5-10 | IEEE 802.11ah-2016, LoRa Alliance spec, 3GPP specs, vendor pricing |
| C1.25 | ~152-175 | Tabla 4 (latency overhead) | Cloud-Centric 210 ms P50 / 450 ms P99. "n=1000 muestras" → ¿medición propia o literatura? | Clarificar: si medición propia → citar capítulo resultados; si literatura → citar |

### MEDIUM — Afirmaciones generales sin soporte

| # | Líneas aprox. | Afirmación | Tipo cita |
|---|---------------|------------|-----------|
| C1.26 | ~14 | "latencias >200 ms dificulta aplicaciones demand response" | IEEE 2030.5 spec (timing requirement) |
| C1.27 | ~213 | "IEC 61000-4-30 muestreo mínimo 10 kHz para armónicos" | IEC 61000-4-30 standard |
| C1.28 | ~250 | "IEEE 2030.5 Demand Response ventanas 50-100 ms" | IEEE 2030.5-2018 standard |
| C1.29 | ~253 | "IEC 61850-90-5 ventanas respuesta 50-100 ms" | IEC 61850-90-5 standard |
| C1.30 | ~270 | "RFC 6282 reduce headers 40 bytes a 2-7 bytes" | RFC 6282 (ya mencionado pero no citado formalmente) |
| C1.31 | ~497 | "8,191 clientes/AP según estándar HaLow" | IEEE 802.11ah-2016 spec |
| C1.32 | ~501 | "99.99% uptime vs 99.91% enlace único" | Analysis/calculation → citar fuente modelo disponibilidad |

---

## Capítulo 4 — Gateway Edge y Red HaLow (`04Gateway_HaLow_Edge.tex`)

**Citas actuales (4):** `thingsboardEdgeArchitecture2024` (×2), `thingsboardEdgeFeatures2024`, `awsGreengrassPricing2024`  
**Nota:** Las 4 citas aparecen solo en líneas 1188-1370 (sección ThingsBoard Edge). **Las primeras ~1,187 líneas tienen CERO citas.**

### CRITICAL — Especificaciones hardware sin datasheets

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C4.1 | ~25-40 | RPi4 specs: BCM2711 4×Cortex-A72 @1.5GHz, LPDDR4-3200, consumo 8.5W promedio / 15W pico | Raspberry Pi Foundation datasheet |
| C4.2 | ~25-40 | "Alternativa comercial (Cisco IR1101 \$2,200, Sierra Wireless XR90 \$1,800)" pricing | Vendor pricing Cisco / Sierra Wireless |
| C4.3 | ~55-70 | "Docker Engine 24.0.5, containerd 1.7.2" resource limits per container (CPU/RAM allocations for 6 microservices) | Docker official docs |
| C4.4 | ~120-160 | OTBR: NAT64 (RFC 6052), DNS64 (RFC 6147), "400+ dispositivos IPv6-only" scalability | RFC 6052, RFC 6147, OpenThread docs |
| C4.5 | ~180-220 | Thread Commissioner: "comisionado 30 nodos, 2.8±0.4s por nodo" — Zigbee comparison "15-20 min/nodo" | Paper/standard Zigbee commissioning benchmarks |
| C4.6 | ~250-310 | HaLow PHY specs: MCS0-MCS10, throughput 150 kbps-347 Mbps, sensibilidad -98 dBm MCS0, modulation BPSK/QPSK/16-QAM/64-QAM/256-QAM | IEEE 802.11ah-2016 standard |
| C4.7 | ~320-360 | Alfa Networks Tube AHM specs: MM6108 chipset ARM Cortex-M4 @120MHz, Flash 4MB QSPI, RAM 1.5MB DTCM+768KB SRAM, TX power +20 dBm, sensibilidad -95 dBm MCS0 | Alfa Networks datasheet, Morse Micro MM6108 datasheet |
| C4.8 | ~370-400 | "propagación Sub-1 GHz: atenuación 15 dB menor que 2.4 GHz en paredes ladrillo" | Paper RF propagation / ITU-R P.1238 |
| C4.9 | ~400-420 | Fresnel zone radius formula and calculation for 920 MHz / 1 km — r₁ = 9.02 m | Referencia fórmula Fresnel (ej. Rappaport textbook) |
| C4.10 | ~430-480 | IEEE 802.11s HWMP protocol: airtime cost metric, PREQ/PREP mechanism, proactive/reactive modes | IEEE 802.11s-2011 standard amendment |
| C4.11 | ~490-530 | RAW (Restricted Access Window): "reduce packet loss 93% (4.2% → 0.3%)" en burst 30 BRs | IEEE 802.11ah-2016 RAW mechanism + medición propia → citar ambos |
| C4.12 | ~530 | "Roaming HaLow latencia 180 ms" | Medición propia → cross-ref Cap 6 |

### CRITICAL — Métricas de rendimiento sin fuente

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C4.13 | ~540-580 | Throughput medido 4.2 Mbps @ 4 MHz MCS7, RTT 11.3 ms P50 / 14.2 ms P95 | Medición propia → citar instrumento/método en Cap 6 |
| C4.14 | ~600-620 | RSSI measurements (-68, -78, -85, -93 dBm) at 200/400/700/1200m | Medición propia → citar instrumento/método |
| C4.15 | ~620-640 | "sensibilidad MCS3 -91 dBm, margen 2 dB" | IEEE 802.11ah-2016 standard |
| C4.16 | ~650-690 | ThingsBoard Edge Docker Compose: 7 containers (tb-edge, postgres, kafka, zookeeper, redis, mqtt-bridge, tb-js-executor) con resource limits | ThingsBoard Edge docs |
| C4.17 | ~1040 | "Reducción mensajes 9,600→2,640 msgs/día (72.5%)" y "ahorro datos LTE 6.8 MB/día" | Medición propia → citar Cap 6 |
| C4.18 | ~1060 | "Voltage sag <207V, swell >253V" thresholds | IEC 61000-4-30, EN 50160, o CREG Colombia |
| C4.19 | ~1075 | "Latencia E2E alarma 380 ms P95" desglose: Thread 15ms + HaLow 11ms + Edge 8ms + LTE 35ms + Cloud 15ms | Medición propia → cross-ref Cap 6 |

### HIGH — Afirmaciones de comparación sin respaldo

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C4.20 | ~1100 | "reducir carga computacional cloud 40%" | Medición propia → citar instrumento (AWS CloudWatch) en Cap 6 |
| C4.21 | ~1155 | "MQTT QoS 1 At Least Once delivery guarantee" | OASIS MQTT 3.1.1 spec |
| C4.22 | ~1160 | "gzip level 6 ratio 4.2:1 para telemetry JSON" | Medición propia o library docs |
| C4.23 | ~1170 | "NTP sync ±50 ms precisión" | NTP spec (RFC 5905) |
| C4.24 | ~1230 | Data filtering "65-80% reducción" claim (already cited, but underlying measurement uncited) | Medición propia |
| C4.25 | ~1260 | "latencia <10 ms validado, P95: 8 ms" sin cita al protocolo de medición | Cross-ref Cap 6 §resultados-latencia-edge |
| C4.26 | ~1280 | "IEEE 2030.5 Demand Response <500 ms" requisito | IEEE 2030.5-2018 standard |
| C4.27 | ~1340-1370 | Tabla edge-features-comparison: "Greengrass 2 días SQLite, Azure 3 días local storage", RAM "780 MB típico Greengrass, 950 MB Azure IoT", "OPEX \$2.20/device Greengrass, \$1.50/device Azure", curva aprendizaje "4-5 días Greengrass, 3-4 días Azure" | AWS Greengrass docs, Azure IoT Edge docs, vendor pricing pages |
| C4.28 | ~1390-1420 | Tabla duplicada tb-edge-comparison (mismos datos sin fuente) | Same as above |
| C4.29 | ~1430-1467 | Conclusiones: "CAPEX \$120 hardware", "uptime 99.93%", "CPU 35%", "RAM 3.2 GB/4 GB", "alternativa AWS Greengrass \$450+" | Cross-ref mediciones Cap 6, vendor pricing |
| C4.30 | ~1440 | "IEC 62056 AMI latencia <500 ms requisito" | IEC 62056 standard |
| C4.31 | ~1445 | "Primera implementación documentada malla 802.11s HaLow para AMI en literatura científica" | Revisión sistemática / estado del arte — debe demostrarse |

### MEDIUM — Protocolos y estándares mencionados sin cita formal

| # | Líneas aprox. | Estándar/protocolo | Cita requerida |
|---|---------------|--------------------|----------------|
| C4.32 | ~120 | RFC 6052 (NAT64 prefix) | RFC 6052 |
| C4.33 | ~125 | RFC 6146 (Stateful NAT64) | RFC 6146 |
| C4.34 | ~180 | Thread 1.3 specification | Thread Group spec |
| C4.35 | ~250 | IEEE 802.11ah-2016 (whole HaLow section) | IEEE 802.11ah-2016 |
| C4.36 | ~430 | IEEE 802.11s (mesh networking amendment) | IEEE 802.11s-2011 |
| C4.37 | ~1155 | MQTT 3.1.1 / 5.0 QoS levels | OASIS MQTT spec |

---

## Capítulo 5 — Server ThingsBoard + Kafka (`05Server_ThingsBoard_Kafka.tex`)

**Citas actuales: 0 (CERO)**  
**Este capítulo tiene 955 líneas sin ninguna referencia bibliográfica. Es el gap más crítico de toda la tesis.**

### CRITICAL — Arquitectura y especificaciones sin ninguna fuente

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C5.1 | ~20-30 | "ThingsBoard 3.6.2 Community Edition" architecture description | ThingsBoard official docs / paper |
| C5.2 | ~35 | "OPEX 3× mayor AWS IoT (\$0.20/device/mes vs \$0.065/device/mes ThingsBoard self-hosted)" | AWS IoT Core pricing page, ThingsBoard TCO analysis |
| C5.3 | ~55-110 | AWS pricing: EC2 t3.xlarge \$122/mes, RDS db.t3.large \$136/mes, ElastiCache \$45/mes, MSK \$72/mes, S3 \$15/mes, NAT Gateway \$32/mes, CloudWatch \$18/mes | AWS official pricing (cada servicio) |
| C5.4 | ~130-145 | Tabla AWS cloud vs on-premise: "TCO 3 años \$15,840 vs \$7,520" | Cálculo derivado → documentar fuentes componentes |
| C5.5 | ~155 | "Apache Kafka 3.5.1" architecture claims | Apache Kafka docs / paper (Kreps et al. 2011) |
| C5.6 | ~170-180 | "LZ4 ratio 3.2:1 para JSON telemetry, CPU overhead 5%" | Medición propia o LZ4 benchmarks |
| C5.7 | ~175 | "8 partitions × 150 msgs/s/partition = 1,200 msgs/s teóricos" | Kafka documentation / benchmark paper |
| C5.8 | ~195 | "Kafka benchmark: 2M msgs/s 3-broker cluster" | LinkedIn/Confluent Kafka benchmark paper |
| C5.9 | ~240 | "Snappy ratio 2.8:1 CPU 3%, Gzip ratio 4.1:1 CPU 18%" comparación compresión | Compression benchmark / paper |
| C5.10 | ~245 | "GC pauses Java CMS P99 = 8s medidas" | Medición propia → documentar instrumento |

### CRITICAL — ThingsBoard architecture claims

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C5.11 | ~390 | "Actor System basado en Akka" — modelo actor, mailbox queue 10,000 | ThingsBoard architecture docs + Akka framework docs |
| C5.12 | ~395 | "1,200 msgs/s por instancia MQTT transport" throughput | ThingsBoard benchmark / medición propia |
| C5.13 | ~400 | "Actor System benchmark 3,300 msgs/s" | ThingsBoard docs / Akka benchmark |
| C5.14 | ~420 | "Cache hit rate 87%, miss penalty 12 ms PostgreSQL query" | Medición propia |
| C5.15 | ~430 | "VM2 aislamiento, timeout 5s, memoria limit 128 MB" (tb-js-executor) | ThingsBoard docs |
| C5.16 | ~470 | "BRIN index eficiente para timestamp ordenado" | PostgreSQL docs / TimescaleDB docs |
| C5.17 | ~500-520 | Compression ratio "10:1 columnar compression" y "12 GB → 1.2 GB" | TimescaleDB documentation |
| C5.18 | ~530 | "Retention policy 90 días" automática | TimescaleDB docs |

### HIGH — Configuraciones Kafka sin referencia a documentación oficial

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C5.19 | ~260-310 | Toda la configuración server.properties Kafka | Apache Kafka docs |
| C5.20 | ~310-365 | Producer/Consumer config con justificaciones (acks=1 vs all, batch.size, linger.ms) | Apache Kafka docs, Kafka definitive guide |
| C5.21 | ~345 | "ALB round robin, sticky sessions, TLS 1.3 cipher ECDHE-RSA-AES256-GCM-SHA384" | AWS ALB docs |
| C5.22 | ~380 | "Bucket4j token bucket algorithm" rate limiting | Bucket4j docs / paper token bucket |
| C5.23 | ~385 | "Stress test 500 devices @ 1 msg/s: 0 dropped, P95=38ms, CPU 72%" | Medición propia |
| C5.24 | ~405 | "PostgreSQL COPY bulk insert" write path | PostgreSQL docs |
| C5.25 | ~460 | "Spring Security" autenticación JWT | Spring Security docs |
| C5.26 | ~465 | "RBAC 4 roles predefinidos (SYS_ADMIN, TENANT_ADMIN, CUSTOMER_USER, GENERIC_USER)" | ThingsBoard docs |
| C5.27 | ~480 | Tabla escalado horizontal: scaling strategy indicators por capa | ThingsBoard architecture docs |

### HIGH — Database design claims

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C5.28 | ~530-545 | "Continuous aggregates: speedup 68×" performance gains | TimescaleDB docs / benchmark |
| C5.29 | ~550 | "Compression ratio 6.2:1 (2.4 GB → 387 MB)" | Medición propia → documentar |
| C5.30 | ~700-720 | "Backup AWS RDS automated daily, DR test RTO 67 min" | Medición propia + AWS RDS docs |
| C5.31 | ~730-740 | Rule Chains: "Telemetry Processing, Energy Billing, Alarm Processing, Data Export" | ThingsBoard docs |
| C5.32 | ~800-830 | "REST API 45,280 requests/día, P99 120 ms, 99.97% success rate" | Medición propia |
| C5.33 | ~850-870 | Tabla Asset Model (Building → Floor → Apartment hierarchy) | ThingsBoard docs |
| C5.34 | ~900-955 | Conclusiones del capítulo: todas las afirmaciones cuantitativas sin fuente | Cross-ref mediciones + vendor docs |

---

## Capítulo 6 — Resultados y Validación (`06Resultados_Validacion.tex`)

**Citas actuales (2):** `ISOIEC30141v2024`, `tangResearchInteroperabilityIoT` (ambas en línea 1273, sección conformidad ISO)  
**Las primeras ~1,272 líneas tienen CERO citas.**

### CRITICAL — Baselines de comparación sin fuente

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C6.1 | ~50-80 | Piloto setup: "30 medidores Itron SL7000, DLMS/COSEM IEC 62056" | Itron SL7000 datasheet, IEC 62056 standard |
| C6.2 | ~90-100 | "Thread Channel 25 = 2.475 GHz", "TX power +4 dBm límite legal Colombia" | IEEE 802.15.4 channel allocation, Resolución 711 ANE Colombia |
| C6.3 | ~120-130 | "IEEE 2030.5 polling 96 lecturas/día" | IEEE 2030.5-2018 Mirror Meter Reading spec |
| C6.4 | ~200-230 | Latencia edge "8±2 ms" y desglose por componente (Thread 11ms, HaLow 11ms, Edge 8ms, LTE 210ms) | Medición propia → declarar instrumento (iperf3, Wireshark, ping) |
| C6.5 | ~250 | "Latencia E2E 248 ms vs 520 ms cloud-only (52% reducción)" | Cloud baseline → citar fuente del 520 ms (medición propia vs literatura) |
| C6.6 | ~300-320 | Stress test: "129,588 lecturas, 42% CPU, 99.99% success rate" | Medición propia |
| C6.7 | ~400-420 | "Thread latencia 11±3 ms por hop" y propagation delay model | IEEE 802.15.4 spec + medición propia |

### CRITICAL — TCO y costos sin fuentes de pricing

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C6.8 | ~450-500 | TCO tabla: ESP32-C6 \$15, RPi4 \$295, Alfa HaLow \$80/unidad, nRF52840 dongle \$10 | Vendor pricing pages (Espressif, Raspberry Pi, Alfa, Nordic) |
| C6.9 | ~510-520 | "NB-IoT SIM \$12/mes/dispositivo" y "gateway celular \$200/año" | Pricing operador NB-IoT específico |
| C6.10 | ~530 | "TCO \$107/medidor vs \$1,065 NB-IoT, ahorro 90%" | Cálculo derivado → documentar todas fuentes componentes |
| C6.11 | ~535 | "LoRaWAN TCO \$167/medidor" comparación | Paper/reporte LoRaWAN TCO |
| C6.12 | ~540 | "WiFi mesh TCO \$348/medidor" comparación | Paper/reporte WiFi mesh TCO |
| C6.13 | ~560 | "4G LTE \$21" OPEX 5 años y "TB Cloud \$30" | Pricing operador + ThingsBoard hosting pricing |

### CRITICAL — Disponibilidad y fiabilidad

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C6.14 | ~700-730 | "Disponibilidad 99.62% (2,151.82h / 2,160h)" | Medición propia → declarar método (uptime monitoring tool) |
| C6.15 | ~740-760 | Tabla de 5 incidentes: "downtime 4G LTE 229 min (47%)", "Gateway Docker crash 98 min" | Log de incidentes piloto |
| C6.16 | ~780-800 | "MTBF 432h, MTTR 98 min" | Cálculo → citar fórmula estándar (MIL-HDBK-217F o IEC 61709) |
| C6.17 | ~800 | "MTBF industrial modems >50,000h" como referencia comparativa | Vendor spec / reliability standard |

### HIGH — Hipótesis validadas con claims comparativos sin fuente

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C6.18 | ~1100-1110 | H1: "265,000 esperadas, 264,988 exitosas" | Medición propia → documentar método conteo |
| C6.19 | ~1130 | H2: "Zigbee 15-20 min/nodo reportados en literatura" — sin citar qué literatura | Paper benchmarking Zigbee commissioning |
| C6.20 | ~1145 | H2: "BLE pairing PAKE ECC P-256 (45s), Network Key 128-bit AES (30s)" | Thread Group spec, Bluetooth SIG spec |
| C6.21 | ~1160 | H3: "IEEE 2030.5 Demand Response <50 ms requisito" | IEEE 2030.5-2018 standard |
| C6.22 | ~1175 | H4: "Alfa Network Tube-U4AHM, TX power +20 dBm" | Alfa Networks datasheet |
| C6.23 | ~1180 | H4: "WiFi 2.4 GHz alcance típico 100 m LOS (802.11n), 30 m con 2 paredes" | Paper WiFi propagation |
| C6.24 | ~1195 | H5: "Nordic PPK2" como instrumento de medición consumo | Nordic Semiconductor PPK2 datasheet |
| C6.25 | ~1200 | H5: "Deep sleep 12 µA, Active RX 11 mA, Active TX 21 mA @ +4 dBm" | ESP32-C6 datasheet (Espressif) |
| C6.26 | ~1250 | H7: "NB-IoT TCO \$1,065/medidor: módulo SIM integrado \$85, SIM \$12/mes" | Vendor pricing NB-IoT |
| C6.27 | ~1270-1310 | ISO/IEC 30141 conformity table: "7 entidades funcionales mandatorias" | Ya citado parcialmente, pero la tabla necesita más refs |
| C6.28 | ~1310 | "IEC 62053-21 precisión ±0.5% clase 1" | IEC 62053-21 standard |
| C6.29 | ~1320 | "Thread 1.4 + TCP" (corrección: piloto usa 1.3) | Thread Group spec v1.4 |
| C6.30 | ~1330 | "Suricata IDS monitoring" y "Let's Encrypt ACME" | Suricata docs, Let's Encrypt docs |
| C6.31 | ~1340 | "Eclipse Leshan server 2.0.0" y "LwM2M 1.1 IPSO Objects" | OMA LwM2M spec, Eclipse Leshan docs |

### HIGH — Comparaciones con estado del arte sin citas

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C6.32 | ~1370 | "vs NB-IoT: latencia 520 ms E2E, consumo 45 mW" | Paper NB-IoT AMI benchmarks |
| C6.33 | ~1372 | "vs LoRaWAN: latencia 350 ms E2E, throughput 5.5 kbps SF7" | Paper LoRaWAN AMI benchmarks |
| C6.34 | ~1374 | "vs WiFi mesh: disponibilidad 98.5% típico" | Paper WiFi mesh AMI reliability |
| C6.35 | ~1374 | "WiFi mesh TCO \$348" | Paper/reporte WiFi mesh costs |
| C6.36 | ~1385 | "Primera implementación documentada AMI Thread+HaLow+Edge" | Revisión sistemática (demostrar) |

### MEDIUM — Metodología estadística sin referencia

| # | Líneas aprox. | Afirmación sin cita | Tipo cita requerida |
|---|---------------|----------------------|---------------------|
| C6.37 | ~1095 | "t-test 1 muestra, t=5.32, df=89, p<0.001" — fórmula sin cita | Textbook estadística |
| C6.38 | ~1095 | "distribución t, α=0.05" — criterio de significancia | Declaración estándar |

---

## Plan de Acción Priorizado

### Prioridad 1 — BLOQUEANTES para defensa (semana 1)

1. **Cap 5 completo**: Agregar mínimo 15-20 citas (ThingsBoard docs, Apache Kafka docs/paper, AWS pricing, TimescaleDB docs, PostgreSQL docs, Redis docs)
2. **Cap 4 líneas 1-1187**: Agregar IEEE 802.11ah-2016, IEEE 802.11s-2011, RFC 6052, RFC 6146, datasheets RPi4/MM6108/Alfa Networks, Thread Group spec
3. **Cap 6 comparaciones**: Citar fuentes para TODOS los baselines (NB-IoT, LoRaWAN, WiFi mesh latencia/costo/disponibilidad)

### Prioridad 2 — IMPORTANTES (semana 2)

4. **Cap 1 tablas**: Agregar fuentes por celda para Tablas 1-4 (datasheets, specs, vendor pricing)
5. **Cap 6 TCO**: Documentar fuente de cada componente de costo
6. **Cap 4+6 mediciones propias**: Cross-references consistentes entre capítulos ("según resultados §6.X")
7. **Cap 6 hipótesis**: Citar IEEE 2030.5-2018 para cada claim de timing requirement

### Prioridad 3 — MEJORAS (semana 3)

8. **Standards formales**: Agregar `\cite{}` formal para cada estándar mencionado (IEEE, IEC, RFC, OMA)
9. **Vendor datasheets**: Crear entradas `.bib` para Espressif ESP32-C6, Nordic nRF52840, Itron SL7000, Morse Micro MM6108
10. **Market reports**: FTTH Council Americas, IEA World Energy Outlook, GSMA IoT pricing

### Referencias sugeridas para agregar al `.bib`

```bibtex
% Standards
@standard{IEEE802.11s-2011,
  title={IEEE Std 802.11s-2011 -- Mesh Networking},
  year={2011}
}
@standard{IEEE2030.5-2018,
  title={IEEE Std 2030.5-2018 -- Smart Energy Profile 2.0},
  year={2018}
}
@standard{IEC61000-4-30,
  title={IEC 61000-4-30 -- Power Quality Measurement Methods},
  year={2015}
}
@standard{IEC62056,
  title={IEC 62056 -- DLMS/COSEM},
  year={2022}
}

% Vendor docs
@manual{raspberrypi4datasheet,
  title={Raspberry Pi 4 Model B Datasheet},
  organization={Raspberry Pi Foundation},
  year={2019}
}
@manual{espressifESP32C6,
  title={ESP32-C6 Datasheet},
  organization={Espressif Systems},
  year={2023}
}
@manual{morseMicroMM6108,
  title={MM6108 Wi-Fi HaLow SoC Product Brief},
  organization={Morse Micro},
  year={2023}
}
@manual{alfaNetworksTubeAHM,
  title={Tube AHM -- Wi-Fi HaLow USB Adapter},
  organization={Alfa Networks},
  year={2024}
}
@manual{itronSL7000,
  title={Itron SL7000 Smart Meter Technical Specification},
  organization={Itron Inc.},
  year={2022}
}

% Software docs
@misc{thingsboardDocs2024,
  title={ThingsBoard 3.6 Architecture Documentation},
  howpublished={\url{https://thingsboard.io/docs/}},
  year={2024}
}
@misc{apacheKafkaDocs,
  title={Apache Kafka Documentation},
  howpublished={\url{https://kafka.apache.org/documentation/}},
  year={2024}
}
@misc{timescaleDBDocs,
  title={TimescaleDB Documentation},
  howpublished={\url{https://docs.timescale.com/}},
  year={2024}
}

% Key papers
@inproceedings{krepsKafka2011,
  author={Kreps, Jay and Narkhede, Neha and Rao, Jun},
  title={Kafka: a Distributed Messaging System for Log Processing},
  booktitle={Proc. NetDB Workshop},
  year={2011}
}

% Reports
@techreport{ftthCouncilAmericas2024,
  title={Fiber Broadband in the Americas: Market Panorama 2024},
  institution={FTTH Council Americas},
  year={2024}
}
@techreport{IEAWorldEnergy2023,
  title={World Energy Outlook 2023},
  institution={International Energy Agency},
  year={2023}
}
```

---

*Nota: Los números de línea son aproximados (±10 líneas) debido a que el conteo se basa en lectura por bloques. Se recomienda buscar cada afirmación por texto exacto dentro del archivo.*
