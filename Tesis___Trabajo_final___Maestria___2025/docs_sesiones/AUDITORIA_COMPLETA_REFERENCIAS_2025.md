# AUDITORÍA COMPLETA DE REFERENCIAS BIBLIOGRÁFICAS
## Tesis: Arquitectura IoT Smart Energy con Thread, HaLow, Edge Computing
### Fecha: 2025-02-12

---

## 1. RESUMEN EJECUTIVO

| Métrica | Valor | Estado |
|---------|-------|--------|
| Entradas en `Referencias.bib` | 308 | Excesivo |
| Citas usadas en `.tex` activos | 162 | OK |
| Entradas **NUNCA citadas** | 167 (54%) | CRÍTICO |
| Citas rotas (key no existe en .bib) | 21 | CRÍTICO |
| Capítulos con **0 citas** | Cap. 5 (ThingsBoard/Kafka) | CRÍTICO |
| Capítulos con <5 citas | Cap. 4 (3), Cap. 6 (2) | CRÍTICO |
| Vacíos de citación estimados | ~197 claims sin cita | ALTO |
| Entradas sin año | ~20 | MEDIO |
| DOIs falsos/placeholder | 2 | MEDIO |
| Entradas duplicadas | 2-3 pares | BAJO |

**Diagnóstico global:** La bibliografía tiene buena cobertura temática en HaLow y Edge Computing, pero sufre de **tres problemas graves**: (1) más de la mitad de las entradas no se usan, (2) los capítulos de implementación carecen casi totalmente de citas, y (3) faltan estándares RFC fundamentales.

---

## 2. REFERENCIAS ROTAS (21 citas sin entrada .bib)

| Cita rota en `.tex` | Entrada similar en `.bib` | Acción |
|---|---|---|
| `DeCastroKorgi2010` | No existe | Agregar entrada o eliminar cita |
| `ITU-TG.984` | No existe | Agregar estándar ITU-T G.984 (GPON) |
| `adameTimeVariantChannelModeling2019` | `adameIEEE80211ahOverview2014` | Agregar nuevo paper de Adame 2019 |
| `alharbi2021` | `alharbi6LoWPANBasedAMI2021` | **TYPO** → corregir a `alharbi6LoWPANBasedAMI2021` |
| `amezvaValdDesignImplementation2024` | No existe | Agregar entrada |
| `augmentingSecuritySmart2024` | Key con nombre institucional largo | **TYPO** → usar key completo del .bib |
| `bahardimanSecureHybridGateway2024` | No existe | Agregar o verificar ortografía |
| `broadbandcommissionStateofBroadband2023` | No existe | Agregar reporte Broadband Commission |
| `eldefrawyFormallyVerifiedAuthentication2019` | No existe | Agregar paper |
| `khorovSurveyIEEE802112016` | `khorovSurveyIEEE80211ah2015` | **TYPO** → corregir a key 2015 |
| `li2022` | Múltiples "li..." | Identificar cuál paper Li 2022 |
| `liEstimatingPropagationCharacteristics2024` | No existe | Agregar entrada |
| `liangSecurityLightweightAuthentication2018` | No existe | Agregar entrada |
| `morse_micro_mm8108_2025` | No existe | Agregar datasheet Morse Micro MM8108 |
| `morsemicroMM61082024` | No existe | Agregar datasheet Morse Micro MM6108 |
| `park2023` | Múltiples "park..." | Identificar cuál paper Park 2023 |
| `perezImplicationSoftwareContainerization2024` | No existe | Agregar paper |
| `sairamIntegratedLightweightCryptographic2024` | No existe | Agregar paper |
| `syamsSecureCommunicationIntegrating2024` | No existe | Agregar paper |
| `wulurCoordinationEdgeCloud2024` | No existe | Agregar paper |
| `*` (cite{*}) | N/A | **ELIMINAR** - `\cite{*}` incluye TODAS las refs |

### Prioridad: 🔴 URGENTE — Corregir antes de compilar

---

## 3. DISTRIBUCIÓN DE CITAS POR CAPÍTULO

| Capítulo | Archivo | Citas | Líneas | Ratio | Estado |
|----------|---------|:-----:|:------:|:-----:|--------|
| Cap 1 - Introducción | `01Introduccion.tex` | 10 | 639 | 1/64 | ⚠️ BAJO |
| Cap 2 - Marco Teórico | `02MarcoTeorico_Rework.tex` + inputs | 52 | 829 | 1/16 | ✅ Aceptable |
| Cap 3 - Nodo IoT Thread | `03NodoIoT_Thread_LwM2M.tex` | 13 | ~800 | 1/62 | ⚠️ BAJO |
| **Cap 4 - Gateway HaLow** | `04Gateway_HaLow_Edge.tex` | **3** | **1,467** | **1/489** | 🔴 CRÍTICO |
| **Cap 5 - Server Cloud** | `05Server_ThingsBoard_Kafka.tex` | **0** | **955** | **0** | 🔴 CRÍTICO |
| **Cap 6 - Resultados** | `06Resultados_Validacion.tex` | **2** | **1,405** | **1/703** | 🔴 CRÍTICO |
| Cap 7 - Conclusiones | `07Conclusiones_TrabajoFuturo.tex` | 63 | ~1,200 | 1/19 | ✅ OK |

### Estándar IEEE esperado:
- Marco Teórico: 1 cita cada 10-15 líneas ✅
- Implementación: 1 cita cada 30-50 líneas (specs, datasheets)
- Resultados: 1 cita cada 20-30 líneas (baselines de comparación)
- Introducción: 1 cita cada 20-30 líneas

---

## 4. VACÍOS DE CITACIÓN CRÍTICOS POR CAPÍTULO

### 4.1 Capítulo 5 — Server ThingsBoard/Kafka (0 CITAS — ~55 vacíos)

| Sección | Claim sin cita | Tipo de ref. necesaria |
|---------|---------------|----------------------|
| §5.2.1 Hardware specs | "Intel Xeon E-2288G", "64GB DDR4 ECC" | Datasheet/vendor |
| §5.2.2 Kafka architecture | Toda la descripción de topics, particiones, ISR | Apache Kafka docs + papers |
| §5.2.3 Rate limiting | "Token bucket algorithm a 10K msg/s" | Paper algoritmo |
| §5.3.1 Microservicios TB | Toda la arquitectura de ThingsBoard | ThingsBoard docs + papers |
| §5.3.3 TimescaleDB | "Compresión 90%", "20:1 ratio" | TimescaleDB paper/docs |
| §5.3.4 Backup RPO/RTO | "RPO < 1 hora, RTO < 30 min" | Best practice NIST |
| §5.5 Dashboards | Multi-tenant architecture claims | ThingsBoard docs |

### 4.2 Capítulo 4 — Gateway HaLow Edge (3 CITAS — ~62 vacíos)

| Sección | Claim sin cita | Tipo de ref. necesaria |
|---------|---------------|----------------------|
| §4.2.1 RPi4 specs | "BCM2711 Cortex-A72 1.8GHz", "NVMe 256GB" | RPi Foundation datasheet |
| §4.3 OTBR | Toda la config de OpenThread Border Router | Thread spec + OpenThread docs |
| §4.4.1 HaLow fundamentals | PHY specs, S1G bands, MCS tables | IEEE 802.11ah-2017 std |
| §4.4.2 802.11s mesh | HWMP protocol, path selection | IEEE 802.11s-2011 |
| §4.4.3 Alfa Networks | "Tube AHM", Morse Micro MM6108 chipset | Vendor datasheets |
| §4.5 ThingsBoard Edge | Rule chains, offline sync, buffer specs | ThingsBoard docs |
| §4.5.5 Filtrado WAN | "Reducción 73% tráfico" | Medición propia → referenciar Cap 6 |

### 4.3 Capítulo 6 — Resultados (2 CITAS — ~42 vacíos)

| Sección | Claim sin cita | Tipo de ref. necesaria |
|---------|---------------|----------------------|
| §6.3.3 Comparación Cloud-Only | "LoRaWAN 350ms", "NB-IoT 520ms" | Papers de benchmark |
| §6.4.3 TCO Analysis | "NB-IoT $1,000+/medidor" | Estudio de mercado/paper |
| §6.5.2 Comparación energética | "Zigbee 45mW", "BLE 38mW" | Datasheets comparativos |
| §6.7.1 Disponibilidad | "99.2% vs WiFi mesh 98.5%" | Paper de referencia |
| §6.10 Validación hipótesis | Todas las comparaciones con estado del arte | Papers por cada métrica |

### 4.4 Capítulo 1 — Introducción (10 CITAS — ~38 vacíos)

| Sección | Claim sin cita | Tipo de ref. necesaria |
|---------|---------------|----------------------|
| §1.1 Tablas comparativas | Datos de throughput, latencia, costo por tecnología | Papers per-cell |
| §1.2.1 Problem statement | "28 billion IoT devices by 2025" | Reporte IoT Analytics/Gartner |
| §1.2.3 Exclusión tecnologías | Cálculos de throughput 5G, PLC, TCO | Papers/standards |
| §1.3 Convergencia fibra | Datos de despliegue FTTN en LatAm | Reportes ITU/GSMA |

---

## 5. ESTÁNDARES RFC/IEEE FALTANTES (CRÍTICOS)

Estos estándares son **fundamentales** para la tesis y DEBEN estar en la bibliografía:

| Estándar | Tema | Usado en | Estado en .bib |
|----------|------|----------|----------------|
| **RFC 7252** | CoAP (Constrained Application Protocol) | Cap 3, 4 | ❌ FALTA |
| **RFC 4944** | 6LoWPAN (IPv6 over 802.15.4) | Cap 3 | ❌ FALTA |
| **RFC 6282** | IPHC (Header Compression for 6LoWPAN) | Cap 3 | ❌ FALTA |
| **RFC 8613** | OSCORE (Object Security for CoAP) | Cap 2, 3 | ❌ FALTA |
| **RFC 6550** | RPL (Routing Protocol for LLNs) | Cap 3 | ❌ FALTA |
| **IEEE 802.15.4-2020** | Thread PHY/MAC layer | Cap 3 | ❌ FALTA |
| IEEE 802.11ah-2017 | Wi-Fi HaLow | Cap 2, 4 | ✅ Existe (como 2016) |
| Thread 1.3 Spec | Thread protocol | Cap 3 | ✅ Existe |
| OMA LwM2M v1.2 | Device management | Cap 3, 4 | ✅ Existe |
| IEEE 2030.5-2018 | Smart Energy Profile | Cap 2, 4 | ✅ Existe |
| IEC 62056 | DLMS/COSEM metering | Cap 3 | ✅ Existe |
| **ITU-T G.984** | GPON fiber | Cap 1, 2 | ❌ FALTA (citado pero roto) |

### Entradas BibTeX a agregar:

```bibtex
@techreport{RFC7252,
  author = {Shelby, Z. and Hartke, K. and Bormann, C.},
  title = {{The Constrained Application Protocol (CoAP)}},
  type = {RFC},
  number = {7252},
  year = {2014},
  institution = {IETF},
  doi = {10.17487/RFC7252}
}

@techreport{RFC4944,
  author = {Montenegro, G. and Kushalnagar, N. and Hui, J. and Culler, D.},
  title = {{Transmission of IPv6 Packets over IEEE 802.15.4 Networks}},
  type = {RFC},
  number = {4944},
  year = {2007},
  institution = {IETF},
  doi = {10.17487/RFC4944}
}

@techreport{RFC6282,
  author = {Hui, J. and Thubert, P.},
  title = {{Compression Format for IPv6 Datagrams over IEEE 802.15.4-Based Networks}},
  type = {RFC},
  number = {6282},
  year = {2011},
  institution = {IETF},
  doi = {10.17487/RFC6282}
}

@techreport{RFC8613,
  author = {Selander, G. and Mattsson, J. and Palombini, F. and Seitz, L.},
  title = {{Object Security for Constrained RESTful Environments (OSCORE)}},
  type = {RFC},
  number = {8613},
  year = {2019},
  institution = {IETF},
  doi = {10.17487/RFC8613}
}

@techreport{RFC6550,
  author = {Winter, T. and Thubert, P. and Brandt, A. and others},
  title = {{RPL: IPv6 Routing Protocol for Low-Power and Lossy Networks}},
  type = {RFC},
  number = {6550},
  year = {2012},
  institution = {IETF},
  doi = {10.17487/RFC6550}
}

@standard{IEEE802.15.4-2020,
  author = {{IEEE}},
  title = {{IEEE Standard for Low-Rate Wireless Networks}},
  number = {802.15.4-2020},
  year = {2020},
  organization = {IEEE},
  doi = {10.1109/IEEESTD.2020.9144691}
}

@standard{ITUTG984,
  author = {{ITU-T}},
  title = {{G.984.1: Gigabit-capable Passive Optical Networks (GPON): General characteristics}},
  year = {2008},
  organization = {International Telecommunication Union}
}
```

---

## 6. PAPERS SEMINALES FALTANTES

| Paper | Autores | Año | Relevancia | Capítulo |
|-------|---------|:---:|-----------|----------|
| **"The Emergence of Edge Computing"** | Satyanarayanan | 2017 | Definición seminal de Edge Computing | Cap 2 |
| **"Fog Computing and Its Role in IoT"** | Bonomi et al. | 2012 | Base teórica Fog/Edge | Cap 2 |
| **"Low Power Wide Area Networks: An Overview"** | Raza et al. | 2017 | Survey LPWAN (LoRa/NB-IoT/HaLow) | Cap 2 |
| **"Smart Grid Communications: Overview of Research Challenges"** | Gungor et al. | 2013 | Fundamento Smart Grid AMI | Cap 1, 2 |
| **"LwM2M IoT Agent: experimental evaluation"** | Karaagac et al. | 2018 | Evaluación performance LwM2M | Cap 3 |
| **"IEEE 802.11ah: A Technology to Face IoT Challenge"** | Aust & Ito | 2012 | Paper inaugural HaLow | Cap 2 |
| **"Docker: Lightweight Linux Containers"** | Merkel | 2014 | Base teórica containerización | Cap 4, 5 |
| **"Kafka: a Distributed Messaging System"** | Kreps et al. | 2011 | Base teórica Kafka | Cap 5 |

---

## 7. PROBLEMAS DE CALIDAD EN `Referencias.bib`

### 7.1 Entradas problemáticas

| Problema | Entrada | Acción |
|----------|---------|--------|
| DOI placeholder "XXXXX" | `cohenEdgeComputingEnabled2024` | Buscar DOI real o eliminar campo |
| DOI placeholder "XXXXX" | `shenAugmentingEdgeIntelligence2024` | Buscar DOI real o eliminar campo |
| Sin autor | `SmartHomeEnergy2024` | Agregar autor o marcar como `@misc` |
| Año futuro 2026 | `sommaTwinArchDigitalTwin2026` | Verificar si es preprint; ajustar año |
| Placeholder "NoTitleFound" | Entry key `NoTitleFound` | Eliminar entrada |
| Key grotescamente largo | `assistantprofessor...india.Augmenting...` | Renombrar key |
| `\cite{*}` en `0000.tex` | Incluye TODA la bibliografía no citada | **ELIMINAR** — causa 167 refs fantasma |
| ~20 entradas sin año | Varias (Zotero imports incompletos) | Agregar años |

### 7.2 Duplicados potenciales

| Entrada 1 | Entrada 2 | Acción |
|-----------|-----------|--------|
| `IEEE80211ah2016` | `IEEE802.11ah-2016` | Unificar, mantener una |
| Chounos 2025 (conference) | Chounos 2025 (arXiv) | Mantener versión publicada |

---

## 8. CONFORMIDAD IEEE

### Estándar de citación IEEE (numérico [1])
- ✅ Se usa `natbib` con `numbers,sort&compress` → correcto
- ✅ `bibpunct{[}{]}{,}{n}{}{,}` → formato [1,2,3] correcto
- ✅ `bibliographystyle{unsrtnat}` → orden de aparición

### Problemas de conformidad:
| Issue | Severidad | Detalle |
|-------|-----------|---------|
| `\cite{*}` en `0000.tex` | CRÍTICO | Incluye TODAS las refs, viola IEEE que solo lista las citadas |
| Entradas sin campos obligatorios | ALTO | ~20 entradas sin `year`, varias `@article` sin `journal` |
| DOIs faltantes | MEDIO | IEEE recomienda DOI en todas las entradas disponibles |
| URLs sin `urldate` | BAJO | Varias `@online` sin fecha de acceso |

---

## 9. PLAN DE ACCIÓN PRIORIZADO

### 🔴 FASE 1 — URGENTE (Semana 1)

| # | Acción | Impacto |
|---|--------|---------|
| 1.1 | **Eliminar `\cite{*}`** de `0000.tex` | Elimina 167 refs fantasma |
| 1.2 | **Corregir 5 typos** de citation keys (`alharbi2021` → `alharbi6LoWPANBasedAMI2021`, etc.) | 5 refs rotas resueltas |
| 1.3 | **Agregar 7 RFCs/estándares** faltantes (RFC 7252, 4944, 6282, 8613, 6550, IEEE 802.15.4-2020, ITU-T G.984) | Cierra gap de estándares |
| 1.4 | **Agregar citas al Cap 5** (mínimo 15-20: Kafka, ThingsBoard, TimescaleDB, PostgreSQL) | De 0 a ~20 citas |
| 1.5 | **Eliminar entrada `NoTitleFound`** y corregir DOIs placeholder | Limpieza bib |

### 🟡 FASE 2 — ALTA PRIORIDAD (Semana 2)

| # | Acción | Impacto |
|---|--------|---------|
| 2.1 | **Agregar citas al Cap 4** (20-30 citas: HaLow specs, OTBR, vendor datasheets, IEEE 802.11s) | De 3 a ~25 citas |
| 2.2 | **Agregar citas al Cap 6** (15-20 citas: baselines de comparación, benchmarks) | De 2 a ~20 citas |
| 2.3 | **Agregar 8 papers seminales** (Satyanarayanan, Bonomi, Raza, Gungor, etc.) | Fortalece marco teórico |
| 2.4 | **Agregar 12 entradas .bib** para citas rotas restantes | 12 refs rotas resueltas |
| 2.5 | **Completar campos faltantes** (year, journal, DOI) en ~20 entradas | Calidad bibliográfica |

### 🟢 FASE 3 — MEJORA (Semana 3)

| # | Acción | Impacto |
|---|--------|---------|
| 3.1 | **Reforzar citas en Cap 1** (10-15 más: datos de mercado, tablas comparativas) | De 10 a ~25 citas |
| 3.2 | **Reforzar citas en Cap 3** (5-10 más: Thread, LwM2M, ESP32 specs) | De 13 a ~23 citas |
| 3.3 | **Eliminar ~150 entradas no citadas** de `Referencias.bib` | Bib limpia y manejable |
| 3.4 | **Unificar duplicados** (2-3 pares) | Consistencia |
| 3.5 | **Agregar `urldate`** a todas las entradas `@online` | Conformidad IEEE |
| 3.6 | **Renombrar keys grotescos** (nombre institucional largo) | Mantenibilidad |

---

## 10. RESUMEN DE REFERENCIAS A AGREGAR POR CAPÍTULO

| Capítulo | Citas actuales | Meta | Entradas .bib nuevas | Tipo principal |
|----------|:--------------:|:----:|:--------------------:|---------------|
| Cap 1 | 10 | 25 | ~8 | Reportes mercado, surveys |
| Cap 2 | 52 | 60 | ~8 | Papers seminales |
| Cap 3 | 13 | 23 | ~5 | RFCs, specs, datasheets |
| Cap 4 | 3 | 25 | ~15 | Standards, datasheets, docs |
| Cap 5 | 0 | 20 | ~12 | Docs oficiales, papers SW |
| Cap 6 | 2 | 20 | ~10 | Papers benchmark, surveys |
| Cap 7 | 63 | 63 | 0 | OK como está |
| **TOTAL** | **143** | **236** | **~58 nuevas** | |

---

*Reporte generado automáticamente — Auditoría bibliográfica integral*
