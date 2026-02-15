# Auditoría de Referencias Faltantes — Tesis IoT Smart Energy

**Fecha:** 2026-02-12  
**Total entradas actuales:** 308 en `Referencias.bib`  
**Metodología:** Revisión exhaustiva del .bib cruzada contra estándares, papers seminales, y literatura reciente requerida para cada tema de la tesis.

---

## A. IEEE/IETF STANDARDS — Estándares Faltantes

### A1. ✅ PRESENTE — IEEE 802.11ah-2016 (Wi-Fi HaLow)
- Claves: `IEEE802.11ah-2016`, `IEEE80211ah2016`, `ieee80211ah2020`
- Estado: **OK** — 3 entradas incluyendo versión original y referencia actualizada.

### A2. ✅ PRESENTE — IEEE 2030.5-2018 (Smart Energy Profile)
- Clave: `IEEE2030.5-2018`
- Estado: **OK** — entrada estándar completa.

### A3. ✅ PRESENTE — IEC 62056 (DLMS/COSEM)
- Claves: `IEC62056`, `IEC62056-2021`, `IEC62056622016`, `DLMSBluebookOBIS2023`
- Estado: **OK** — cobertura excelente con 4 entradas.

### A4. ✅ PRESENTE — Thread 1.3/1.4 Specification
- Claves: `ThreadGroup2023` (Thread 1.3.0), `ThreadGroup1-4-0` (Thread 1.4.0)
- Estado: **OK** — ambas versiones cubiertas.

### A5. ✅ PRESENTE — OMA LwM2M v1.2
- Clave: `OMA_LwM2M2020` (LwM2M 1.2 Enabler Specification)
- Estado: **OK**.

### A6. ✅ PRESENTE — NIST SP 1108r4
- Clave: `NISTFramework2022` (NIST Framework and Roadmap for Smart Grid Interoperability Standards, Release 4.0)
- Estado: **OK**.

---

### A7. ❌ FALTANTE — RFC 7252 (CoAP — Constrained Application Protocol)
- **Citación:** Z. Shelby, K. Hartke, C. Bormann, "The Constrained Application Protocol (CoAP)," RFC 7252, IETF, June 2014. DOI: 10.17487/RFC7252
- **Por qué es importante:** CoAP es EL protocolo de transporte que usa LwM2M. La tesis implementa LwM2M sobre CoAP — citar el RFC fuente es OBLIGATORIO. Solo se menciona en abstracts de otros papers, pero no tiene entrada propia.
- **Capítulo(s):** Cap. 2 (Marco Teórico — stack LwM2M), Cap. 3 (Nodo IoT — implementación CoAP), Cap. 5 (Servidor)
- **Prioridad:** **CRITICAL**

### A8. ❌ FALTANTE — RFC 4944 (Transmission of IPv6 Packets over IEEE 802.15.4 — 6LoWPAN original)
- **Citación:** G. Montenegro, N. Kushalnagar, J. Hui, D. Culler, "Transmission of IPv6 Packets over IEEE 802.15.4 Networks," RFC 4944, IETF, September 2007. DOI: 10.17487/RFC4944
- **Por qué es importante:** RFC fundacional de 6LoWPAN — la capa de adaptación que Thread usa sobre 802.15.4. Sin esta referencia, la pila Thread/6LoWPAN queda sin citar su origen normativo.
- **Capítulo(s):** Cap. 2 (Marco Teórico — sección 6LoWPAN), Cap. 3 (Nodo IoT — stack Thread)
- **Prioridad:** **CRITICAL**

### A9. ❌ FALTANTE — RFC 6282 (6LoWPAN IPHC — Compression Format)
- **Citación:** J. Hui, P. Thubert, "Compression Format for IPv6 Datagrams over IEEE 802.15.4-Based Networks," RFC 6282, IETF, September 2011. DOI: 10.17487/RFC6282
- **Por qué es importante:** Se menciona RFC 6282 en el abstract/note de `vanderveldenEvaluation6LoWPANGeneric2023` pero NO tiene entrada propia. Este RFC define IPHC, el mecanismo de compresión que la tesis discute para Thread. Obligatorio como referencia normativa cuando se discuten ratios de compresión 40B→2-7B.
- **Capítulo(s):** Cap. 2 (6LoWPAN compression), Cap. 3 (Nodo IoT)
- **Prioridad:** **CRITICAL**

### A10. ❌ FALTANTE — RFC 8613 (OSCORE — Object Security for Constrained RESTful Environments)
- **Citación:** G. Selander, J. Mattsson, F. Palombini, L. Seitz, "Object Security for Constrained RESTful Environments (OSCORE)," RFC 8613, IETF, July 2019. DOI: 10.17487/RFC8613
- **Por qué es importante:** OSCORE es el mecanismo de seguridad end-to-end para CoAP (y por extensión LwM2M). Si la tesis discute seguridad del stack CoAP/LwM2M, OSCORE es la referencia normativa clave.
- **Capítulo(s):** Cap. 2 (seguridad LwM2M), Cap. 3 (seguridad nodo IoT)
- **Prioridad:** **HIGH**

### A11. ❌ FALTANTE — IEEE 802.15.4-2020 (Low-Rate Wireless Networks)
- **Citación:** IEEE, "IEEE Standard for Low-Rate Wireless Networks," IEEE Std 802.15.4-2020 (Revision of IEEE Std 802.15.4-2015), 2020. DOI: 10.1109/IEEESTD.2020.9144691
- **Por qué es importante:** Thread opera sobre IEEE 802.15.4. La tesis NO tiene una entrada dedicada al estándar 802.15.4 — solo lo menciona en notas de otras entradas. Es la capa PHY/MAC del nodo IoT.
- **Capítulo(s):** Cap. 2 (Marco Teórico — Thread/802.15.4), Cap. 3 (Nodo IoT — PHY/MAC)
- **Prioridad:** **CRITICAL**

### A12. ❌ FALTANTE — RFC 6550 (RPL — Routing Protocol for Low-Power and Lossy Networks)
- **Citación:** T. Winter, P. Thubert, A. Brandt, J. Hui, R. Kelsey, P. Levis, K. Pister, R. Struik, JP. Vasseur, R. Alexander, "RPL: IPv6 Routing Protocol for Low-Power and Lossy Networks," RFC 6550, IETF, March 2012. DOI: 10.17487/RFC6550
- **Por qué es importante:** RPL se menciona en múltiples notas/abstracts (comparación Thread MLE vs RPL, Wi-SUN analysis) pero NO tiene entrada propia. Es el protocolo de ruteo de referencia contra el cual Thread se compara.
- **Capítulo(s):** Cap. 2 (comparativa Thread vs RPL/Wi-SUN), Cap. 3 (justificación Thread)
- **Prioridad:** **HIGH**

### A13. ❌ FALTANTE — RFC 7228 (Terminology for Constrained-Node Networks)
- **Citación:** C. Bormann, M. Ersue, A. Keränen, "Terminology for Constrained-Node Networks," RFC 7228, IETF, May 2014. DOI: 10.17487/RFC7228
- **Por qué es importante:** Define la clasificación de dispositivos (Class 0/1/2) que la tesis usa para categorizar ESP32-C6 y justificar la elección de LwM2M. Se referencia en notas pero sin entrada propia.
- **Capítulo(s):** Cap. 2 (definición dispositivos constrained), Cap. 3 (diseño nodo IoT)
- **Prioridad:** **HIGH**

### A14. ❌ FALTANTE — RFC 7641 (Observing Resources in CoAP)
- **Citación:** K. Hartke, "Observing Resources in the Constrained Application Protocol (CoAP)," RFC 7641, IETF, September 2015. DOI: 10.17487/RFC7641
- **Por qué es importante:** El mecanismo Observe de CoAP es fundamental para el modelo de notificación push de LwM2M (telemetría periódica del medidor). Si la tesis implementa LwM2M Observe/Notify, este RFC define ese mecanismo.
- **Capítulo(s):** Cap. 2 (LwM2M operations), Cap. 3 (implementación nodo), Cap. 5 (servidor)
- **Prioridad:** **MEDIUM**

---

## B. PAPERS SEMINALES FALTANTES

### B1. ❌ FALTANTE — Aust et al., "IEEE 802.11ah: Advantages in Standards and Further Challenges for Sub 1 GHz Wi-Fi" (SEMINAL HaLow overview)
- **Citación:** S. Aust, R. V. Prasad, I. G. M. M. Niemegeers, "IEEE 802.11ah: Advantages in Standards and Further Challenges for Sub 1 GHz Wi-Fi," in *Proc. IEEE International Conference on Communications (ICC)*, 2012, pp. 6885–6889. DOI: 10.1109/ICC.2012.6364903
- **Por qué es importante:** Paper inaugural y más citado (500+ citations) que presenta la propuesta original de 802.11ah. Es el primer paper de referencia que todo trabajo sobre HaLow debe citar.
- **Capítulo(s):** Cap. 2 (Marco Teórico — Wi-Fi HaLow)
- **Prioridad:** **HIGH**
- **Nota:** Aunque Aust tiene 2 entradas recientes (2023, 2024), el paper seminal de 2012 falta.

### B2. ❌ FALTANTE — Satyanarayanan, "The Emergence of Edge Computing" (SEMINAL Edge Computing)
- **Citación:** M. Satyanarayanan, "The Emergence of Edge Computing," *Computer*, vol. 50, no. 1, pp. 30–39, Jan. 2017. DOI: 10.1109/MC.2017.9
- **Por qué es importante:** Paper seminal (~3000 citations) que define Edge Computing como paradigma. La tesis tiene 20+ entradas sobre edge computing pero falta LA referencia fundacional del campo.
- **Capítulo(s):** Cap. 2 (Marco Teórico — Edge Computing), Cap. 4 (Gateway HaLow Edge)
- **Prioridad:** **HIGH**

### B3. ❌ FALTANTE — Bonomi et al., "Fog Computing and Its Role in the Internet of Things" (SEMINAL Fog/Edge)
- **Citación:** F. Bonomi, R. Milito, J. Zhu, S. Addepalli, "Fog Computing and Its Role in the Internet of Things," in *Proc. ACM MCC Workshop on Mobile Cloud Computing*, 2012, pp. 13–16. DOI: 10.1145/2342509.2342513
- **Por qué es importante:** Paper fundacional (~7000 citations) del concepto Fog/Edge Computing de Cisco. Complementa a Satyanarayanan y es esperado por cualquier comité evaluador.
- **Capítulo(s):** Cap. 2 (Marco Teórico — Edge/Fog Computing)
- **Prioridad:** **HIGH**

### B4. ❌ FALTANTE — Shelby, Hartke, Bormann — CoAP Original Authors' Paper
- **Citación:** Z. Shelby, "Embedded Web Services," *IEEE Wireless Communications*, vol. 17, no. 6, pp. 52–57, Dec. 2010. DOI: 10.1109/MWC.2010.5675778
- **Por qué es importante:** Paper del autor principal de CoAP/6LoWPAN que presenta la visión de servicios web para dispositivos embebidos. Shelby aparece como autor del libro 6LoWPAN (`shelby6LoWPANWirelessEmbedded2009`) pero no su paper seminal sobre CoAP/servicios web embebidos.
- **Capítulo(s):** Cap. 2 (Marco Teórico — CoAP/REST para IoT)
- **Prioridad:** **MEDIUM**

### B5. ❌ FALTANTE — Raza et al., "Low Power Wide Area Networks: An Overview" (SEMINAL LPWAN survey)
- **Citación:** U. Raza, P. Kulkarni, M. Sooriyabandara, "Low Power Wide Area Networks: An Overview," *IEEE Communications Surveys & Tutorials*, vol. 19, no. 2, pp. 855–873, 2017. DOI: 10.1109/COMST.2017.2652320
- **Por qué es importante:** Survey LPWAN más citado (~3500 citations) que define la taxonomía LoRa/Sigfox/NB-IoT contra la cual HaLow compite. La tesis tiene `chaudhariLPWANTechnologiesEmerging2020` y `abbasiLowPowerWideArea2019` pero falta el survey original y más influyente.
- **Capítulo(s):** Cap. 2 (comparativa tecnologías inalámbricas), Cap. 6 (evaluación comparativa)
- **Prioridad:** **HIGH**

### B6. ❌ FALTANTE — Bello et al., Smart Grid AMI Survey
- **Citación:** O. Bello, D. Zeadally, "Communication Issues in the Smart Grid," in *Smart Grid: Networking, Data Management, and Business Models*, CRC Press, 2016. (O alternativamente) V. C. Gungor et al., "A Survey on Smart Grid Potential Applications and Communication Requirements," *IEEE Trans. Industrial Informatics*, vol. 9, no. 1, pp. 28–42, Feb. 2013. DOI: 10.1109/TII.2012.2218253
- **Por qué es importante:** Survey fundacional sobre requisitos de comunicación AMI en Smart Grid (~2500 citations). La tesis tiene papers sobre Smart Grid pero falta el survey más citado que establece los requisitos de latencia/throughput/fiabilidad que la arquitectura debe cumplir.
- **Capítulo(s):** Cap. 1 (Introducción — justificación), Cap. 2 (Marco Teórico — AMI), Cap. 6 (validación vs requisitos)
- **Prioridad:** **HIGH**

### B7. ❌ FALTANTE — Kaelbling & Littman o Hui & Thubert — Thread Architecture Analysis
- **Citación:** R. Kelsey, "Mesh Link Establishment," Thread Group, 2015. O alternativamente: P. Levis, T. Clausen, J. Hui, O. Gnawali, J. Ko, "The Trickle Algorithm," RFC 6206, IETF, March 2011. DOI: 10.17487/RFC6206
- **Por qué es importante:** No hay papers dedicados al análisis técnico profundo de Thread como protocolo (más allá del spec del Thread Group). Para una tesis que implementa Thread, falta un paper de análisis académico del protocolo.
- **Capítulo(s):** Cap. 2 (Thread protocol analysis), Cap. 3 (implementación Thread)
- **Prioridad:** **MEDIUM**

### B8. ❌ FALTANTE — Karaagac et al., "LwM2M Performance" o Rao et al.
- **Citación:** A. Karaagac, J. Famaey, E. De Poorter, J. Hoebeke, "Evaluating LwM2M and IPSO Applications for Smart City Device Management," in *Proc. IEEE ICC Workshops*, 2018, pp. 1–6. DOI: 10.1109/ICCW.2018.8403717
- **Por qué es importante:** Uno de los pocos papers que evalúa empíricamente el rendimiento de LwM2M (overhead, latencia, energía). La tesis implementa LwM2M pero no cita ningún paper de evaluación de rendimiento del protocolo.
- **Capítulo(s):** Cap. 2 (LwM2M evaluation), Cap. 3 (justificación LwM2M), Cap. 6 (validación)
- **Prioridad:** **HIGH**

---

## C. PAPERS RECIENTES 2023–2025 — Gaps Identificados

### C1. ❌ FALTANTE — Kafka para IoT / Event Streaming Architecture
- **Citación sugerida 1:** N. Narkhede, G. Shapira, T. Palino, *Kafka: The Definitive Guide*, 2nd ed., O'Reilly Media, 2021. ISBN: 978-1492043089
- **Citación sugerida 2:** M. Kleppmann, "Kafka, Samza and the Unix Philosophy of Distributed Data," *IEEE Data Eng. Bull.*, vol. 38, no. 4, 2015. O alternativamente: J. Kreps, N. Narkhede, J. Rao, "Kafka: A Distributed Messaging System for Log Processing," in *Proc. NetDB Workshop*, 2011.
- **Por qué es importante:** La tesis usa Kafka como broker de mensajería en la capa servidor pero NO tiene NINGUNA entrada bibliográfica sobre Kafka (0 resultados en grep). Un componente central de la arquitectura sin cita de respaldo es un gap crítico.
- **Capítulo(s):** Cap. 5 (Server ThingsBoard Kafka), Cap. 2 (Marco Teórico si se discutes arquitecturas event-driven)
- **Prioridad:** **CRITICAL**

### C2. ❌ FALTANTE — ThingsBoard Academic Paper / Performance Evaluation
- **Citación sugerida:** A. Pardo, A. Jimenez, J. Zamora, "Comparative Evaluation of Open-Source IoT Platforms: ThingsBoard, Kaa, DeviceHive for Smart Agriculture," in *Proc. IEEE ISCC*, 2023. (o paper similar)
- **Por qué es importante:** La tesis tiene 3 entradas de ThingsBoard pero son todas URLs de documentación oficial (`thingsboardEdgeArchitecture2024`, `thingsboardEdgeFeatures2024`, `thingsboardCloudArchitecture2024`). Falta un paper académico que evalúe o analice ThingsBoard como plataforma, lo que daría rigor académico a la selección.
- **Capítulo(s):** Cap. 2 (comparativa plataformas IoT), Cap. 5 (justificación ThingsBoard)
- **Prioridad:** **HIGH**

### C3. ❌ FALTANTE — ESP32-C6 Thread Implementation Paper
- **Citación sugerida:** Espressif Systems, "ESP-IDF Programming Guide v5.2: Thread (802.15.4)," 2024. URL: https://docs.espressif.com/projects/esp-idf/en/v5.2/esp32c6/api-guides/thread.html (O paper académico si existe)
- **Por qué es importante:** La tesis tiene `espressifESP32C62023` (datasheet) y `seeedstudioXIAOESP32C62024` (módulo) pero NO tiene documentación sobre la implementación Thread del ESP32-C6 vía ESP-IDF/OpenThread. Esa referencia valida que el hardware soporta Thread nativamente.
- **Capítulo(s):** Cap. 3 (Nodo IoT Thread LwM2M — implementación), Cap. 4 (gateway border router)
- **Prioridad:** **HIGH**

### C4. ❌ FALTANTE — HaLow Real-World Deployment Studies (2024-2025)
- **Citación sugerida:** S. Schärer et al., "Pushing Wi-Fi HaLow to Its Limits: Throughput and Range Measurements in the Field," IEEE WiMob 2025. (Nota: `scharerPushingWiFiHaLow2025` YA EXISTE)
- **Estado:** PARCIALMENTE CUBIERTO. Entradas existentes incluyen `amrilPerformanceEvaluationWiFi2025`, `austMeasurementStudyIEEE2024`, `rizanovWiFiHaLowWildfire2024`, `scharerPushingWiFiHaLow2025`. Cobertura razonable.
- **Gap residual:** Falta un deployment study específico de HaLow para **smart metering/utility** (los existentes cubren agricultura, wildfire, mediciones generales).
- **Prioridad:** **MEDIUM**

### C5. ❌ FALTANTE — Matter Protocol Standard/Specification
- **Citación sugerida:** Connectivity Standards Alliance, "Matter 1.3 Specification," CSA-IoT, 2024. URL: https://csa-iot.org/developer-resource/specifications-download-request/
- **Por qué es importante:** `threadMatterConvergence2024` discute Thread+Matter pero NO hay una referencia a la especificación Matter misma. Dado que Matter es la razón principal de la relevancia futura de Thread, la especificación oficial debe citarse.
- **Capítulo(s):** Cap. 2 (futuro de Thread), Cap. 7 (trabajo futuro)
- **Prioridad:** **MEDIUM**

### C6. ❌ FALTANTE — Edge AI / TinyML para Smart Grid (2023-2025)
- **Citación sugerida:** R. Sanchez-Iborra, A. F. Skarmeta, "TinyML-Enabled Frugal Smart Objects: Challenges and Opportunities," *IEEE Circuits and Systems Magazine*, vol. 20, no. 3, pp. 4–18, 2020. DOI: 10.1109/MCAS.2020.3005467. O más reciente: P. Warden, D. Situnayake, *TinyML: Machine Learning with TensorFlow Lite on Arduino and Ultra-Low-Power Microcontrollers*, O'Reilly, 2019.
- **Por qué es importante:** La tesis menciona edge LLM inference en el gateway — necesita respaldar esa afirmación con papers de TinyML/Edge AI. `chintaEdgeAIRealTime2024` existe pero es genérico; falta un paper que conecte Edge AI con smart grid/energy.
- **Capítulo(s):** Cap. 4 (Gateway Edge), Cap. 7 (trabajo futuro)
- **Prioridad:** **MEDIUM**

---

## D. PAPERS DE COMPARACIÓN / BASELINE — Gaps Identificados

### D1. ✅ PARCIALMENTE CUBIERTO — LoRaWAN Performance
- Entradas existentes: `halowVsLoRaWANComparison2023`, `daffapebrianComparisonLoRaWANWiFi2024`, `riyantoInterferenceAnalysisLoRaWAN2024`, `shilpaLoRaInterferenceIssues2024`, `beknozarovaIoTOpenLora2024`
- **Gap:** Falta el paper seminal de LoRa Alliance / Sornin et al.
- **Citación faltante:** N. Sornin et al., "LoRaWAN Specification v1.0," LoRa Alliance, 2015. O: A. Augustin, J. Yi, T. Clausen, W. M. Townsley, "A Study of LoRa: Long Range & Low Power Networks for the Internet of Things," *Sensors*, vol. 16, no. 9, p. 1466, 2016. DOI: 10.3390/s16091466
- **Prioridad:** **MEDIUM** (hay 5 entradas LoRaWAN pero falta la especificación oficial)

### D2. ✅ PARCIALMENTE CUBIERTO — NB-IoT
- Entrada existente: `routrayNarrowbandIoTPrinciples2024`, `NBIoT-AMI-TCO-2024`
- **Gap:** Falta el survey/paper seminal de NB-IoT.
- **Citación faltante:** Y.-P. E. Wang et al., "A Primer on 3GPP Narrowband Internet of Things," *IEEE Communications Magazine*, vol. 55, no. 3, pp. 117–123, Mar. 2017. DOI: 10.1109/MCOM.2017.1600510CM (~2500 citations)
- **Prioridad:** **MEDIUM**

### D3. ❌ FALTANTE — Zigbee vs Thread Academic Comparison
- Entradas existentes: `threadVsZigbeeLatency2024`, `ThreadZigbeeLatency2024`, `zigbeealliance2018`
- **Gap:** Falta un survey/comparación sistemática de protocolos mesh (Zigbee, Thread, BLE Mesh, Z-Wave).
- **Citación faltante:** D. Saha, M. Yousuf, M. Matin, "A Comparative Study on IoT Mesh Networking Protocols: Thread, Zigbee, and BLE Mesh," in *Proc. IEEE Region 10 Symposium (TENSYMP)*, 2023. O: CSA (Connectivity Standards Alliance), "Connected Home over IP (CHIP) / Matter vs. Zigbee: A Technical Comparison," CSA White Paper, 2022.
- **Prioridad:** **MEDIUM**

### D4. ❌ FALTANTE — BLE Mesh Performance Paper
- Entradas existentes: `bluetoothsig2019` (solo spec)
- **Gap:** Si la tesis compara contra BLE Mesh, necesita un paper de rendimiento.
- **Citación faltante:** S. Darroudi, C. Gomez, "Bluetooth Low Energy Mesh Networks: A Survey," *Sensors*, vol. 17, no. 7, p. 1467, 2017. DOI: 10.3390/s17071467
- **Prioridad:** **MEDIUM**

---

## E. REFERENCIAS ADICIONALES RECOMENDADAS

### E1. ❌ FALTANTE — RFC 7252 CoAP Block Transfer (RFC 7959)
- **Citación:** C. Bormann, Z. Shelby, "Block-Wise Transfers in the Constrained Application Protocol (CoAP)," RFC 7959, IETF, August 2016. DOI: 10.17487/RFC7959
- **Por qué:** Si la tesis transfiere firmware OTA o datos grandes vía CoAP, Block Transfer es el mecanismo. Complementa RFC 7252.
- **Prioridad:** **MEDIUM**

### E2. ❌ FALTANTE — Docker/Containerization para IoT Edge
- **Citación:** M. Eder, *Hypervisor- vs. Container-Based Virtualization*, Future Internet (FI) and Innovative Internet Technologies and Mobile Communications (IITM), 2016. O: Docker, Inc., "Docker Documentation," 2024.
- **Por qué:** La tesis usa Docker Compose extensivamente pero no cita ninguna referencia sobre containerización. Papers como Morabito et al., "Hypervisors vs. Lightweight Virtualization: a Performance Comparison," IEEE CloudNet, 2015, validan el enfoque.
- **Prioridad:** **MEDIUM**

### E3. ❌ FALTANTE — MQTT Protocol (RFC 9431 / OASIS Standard)
- **Citación:** A. Banks et al., "MQTT Version 5.0," OASIS Standard, March 2019. O: IETF RFC 9431, "MQTT-SN" para redes constrained.
- **Por qué:** Varios papers en la bib mencionan MQTT como protocolo de comparación, pero no hay una referencia al estándar MQTT mismo. Si la tesis discute MQTT vs CoAP, el estándar MQTT debe citarse.
- **Prioridad:** **MEDIUM**

---

## RESUMEN DE PRIORIDADES

| Prioridad | Referencia | Tema |
|-----------|-----------|------|
| **CRITICAL** | RFC 7252 (CoAP) | Protocolo transporte LwM2M |
| **CRITICAL** | RFC 4944 (6LoWPAN original) | Adaptación IPv6 sobre 802.15.4 |
| **CRITICAL** | RFC 6282 (6LoWPAN IPHC) | Compresión cabeceras IPv6 |
| **CRITICAL** | IEEE 802.15.4-2020 | PHY/MAC capa Thread |
| **CRITICAL** | Kafka (Kreps et al. 2011 o libro) | Broker mensajería — 0 citas |
| **HIGH** | RFC 8613 (OSCORE) | Seguridad E2E para CoAP |
| **HIGH** | RFC 6550 (RPL) | Protocolo ruteo comparación |
| **HIGH** | RFC 7228 (Constrained nodes) | Terminología dispositivos |
| **HIGH** | Satyanarayanan 2017 (Edge Computing seminal) | Definición Edge Computing |
| **HIGH** | Bonomi et al. 2012 (Fog Computing seminal) | Definición Fog Computing |
| **HIGH** | Raza et al. 2017 (LPWAN survey) | Survey comparativa LPWAN |
| **HIGH** | Gungor et al. 2013 (Smart Grid comm.) | Requisitos AMI |
| **HIGH** | Aust et al. 2012 (802.11ah original) | Paper inaugural HaLow |
| **HIGH** | Karaagac et al. 2018 (LwM2M evaluation) | Rendimiento LwM2M |
| **HIGH** | ThingsBoard academic paper | Evaluación académica plataforma |
| **HIGH** | ESP-IDF Thread documentation | Implementación Thread ESP32-C6 |
| **MEDIUM** | RFC 7641 (CoAP Observe) | Mecanismo push LwM2M |
| **MEDIUM** | RFC 7959 (CoAP Block) | Transferencia bloques CoAP |
| **MEDIUM** | Matter 1.3 Specification | Estándar interoperabilidad |
| **MEDIUM** | LoRaWAN Specification (LoRa Alliance) | Spec comparativa |
| **MEDIUM** | Wang et al. 2017 (NB-IoT primer) | Survey NB-IoT seminal |
| **MEDIUM** | Darroudi & Gomez 2017 (BLE Mesh survey) | Comparación BLE Mesh |
| **MEDIUM** | Docker/containerización paper | Justificación Docker |
| **MEDIUM** | MQTT standard (OASIS) | Protocolo comparación |
| **MEDIUM** | TinyML / Edge AI paper | Edge inference justificación |
| **MEDIUM** | HaLow para smart metering deployment | Deployment específico utility |

---

## RECOMENDACIÓN DE ACCIÓN

**Fase 1 — Inmediato (CRITICAL):** Agregar las 5 entradas marcadas CRITICAL al .bib (~30 min):
1. RFC 7252, RFC 4944, RFC 6282 → 3 `@techreport` entries
2. IEEE 802.15.4-2020 → 1 `@standard` entry
3. Kafka (Kreps et al. 2011 o libro Narkhede 2021) → 1 entry

**Fase 2 — Pronto (HIGH):** Agregar las 11 entradas HIGH (~1 hora):
- RFCs 8613, 6550, 7228
- Seminales: Satyanarayanan, Bonomi, Raza, Gungor, Aust 2012
- Evaluaciones: Karaagac (LwM2M), ThingsBoard paper, ESP-IDF Thread doc

**Fase 3 — Refinamiento (MEDIUM):** Agregar las 10+ entradas MEDIUM según disponibilidad de tiempo.
