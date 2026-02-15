# Auditoría Completa de Referencias.bib

**Archivo:** `Tesis___Trabajo_final___Maestria___2025/Referencias.bib`  
**Total líneas:** 4606  
**Entradas BibTeX aproximadas:** ~230+  
**Fecha de auditoría:** 2025

---

## 1. ENTRADAS SIN FECHA/AÑO (date/year)

Estas entradas carecen completamente del campo `date` o `year`:

| # | Clave | Línea | Tipo | Título (abreviado) |
|---|-------|-------|------|---------------------|
| 1 | `alamIoTEvolutionEssentials` | 121 | @article | IoT Evolution: Essentials & Applications |
| 2 | `banovicPortovanjeContikiNGOperativnog` | 448 | @article | Portovanje Contiki-NG operativnog sistema... |
| 3 | `boikoSoftwareModelingImplementation` | 635 | @article | Software Modeling and Implementation... Smart Home |
| 4 | `dongSOFTWAREDEFINEDNETWORKINGSMART` | 993 | @article | Software-Defined Networking for Smart Grid Resilience |
| 5 | `hamedSmartWebApplication` | 1143 | @article | Smart Web Application... |
| 6 | `harveSafeguardingIoTBig` | 1151 | @article | Safeguarding IoT Big... |
| 7 | `huangChannelAccessScheduling` | 1228 | @article | Channel Access Scheduling... |
| 8 | `IEEERecommendedPractice` | 1287 | @standard | IEEE Recommended Practice... |
| 9 | `jonnakutiEDGEBASEDFAULTDETECTION` | 1330 | @article | Edge-Based Fault Detection... |
| 10 | `makayaEdgeSphereThreeTierArchitecture` | 1850 | @article | EdgeSphere Three-Tier Architecture |
| 11 | `naikOptimalSinkNode` | 2135 | @article | Optimal Sink Node... |
| 12 | `rafiReliableCostEfficientIoT` | 2296 | @article | Reliable Cost Efficient IoT... |
| 13 | `ramzanReviewStudySmart` | 2351 | @article | Review Study Smart... |
| 14 | `riazENERGYINFORMATICSSMART` | 2359 | @article | Energy Informatics Smart... |
| 15 | `saadHeterogeneousIPv6Infrastructure` | 2422 | @article | Heterogeneous IPv6 Infrastructure |
| 16 | `saidaSystemeDetectionDIntrusion` | 2448 | @article | Système Détection D'Intrusion |
| 17 | `sanchezTrabajoFinMaster` | 2472 | @article | Trabajo Fin Master |
| 18 | `silardDemoVisualizationPlatform` | 2697 | @article | Demo Visualization Platform |
| 19 | `tangResearchInteroperabilityIoT` | 2851 | @article | Research Interoperability IoT |
| 20 | `zhouGatewayArchitectureSecurity` | 3207 | @article | Gateway Architecture Security |

**Total: 20 entradas sin fecha.**

---

## 2. ENTRADAS SIN DOI

Entradas que carecen del campo `doi` (excluyendo @misc/@online donde el DOI es opcional):

| # | Clave | Línea | Tipo |
|---|-------|-------|------|
| 1 | `alamIoTEvolutionEssentials` | 121 | @article |
| 2 | `banovicPortovanjeContikiNGOperativnog` | 448 | @article |
| 3 | `boikoSoftwareModelingImplementation` | 635 | @article |
| 4 | `hamedSmartWebApplication` | 1143 | @article |
| 5 | `harveSafeguardingIoTBig` | 1151 | @article |
| 6 | `huangChannelAccessScheduling` | 1228 | @article |
| 7 | `hmissiSurveyApplicationLayer2022` | 1191 | @article |
| 8 | `hredoyMULTIPURPOSELAMPPOSTURBAN2024` | 1219 | @article |
| 9 | `jonnakutiEDGEBASEDFAULTDETECTION` | 1330 | @article |
| 10 | `makayaEdgeSphereThreeTierArchitecture` | 1850 | @article |
| 11 | `naikOptimalSinkNode` | 2135 | @article |
| 12 | `rafiReliableCostEfficientIoT` | 2296 | @article |
| 13 | `ramzanReviewStudySmart` | 2351 | @article |
| 14 | `riazENERGYINFORMATICSSMART` | 2359 | @article |
| 15 | `saadHeterogeneousIPv6Infrastructure` | 2422 | @article |
| 16 | `saidaSystemeDetectionDIntrusion` | 2448 | @article |
| 17 | `sanchezTrabajoFinMaster` | 2472 | @article |
| 18 | `silardDemoVisualizationPlatform` | 2697 | @article |
| 19 | `tangResearchInteroperabilityIoT` | 2851 | @article |
| 20 | `zhouGatewayArchitectureSecurity` | 3207 | @article |
| 21 | `abowardahEdgeComputingIoT2024` | 3228 | @article |
| 22 | `daneshianMTProtoAlgorithmSmart2024` | 856 | @article |
| 23 | `dongSOFTWAREDEFINEDNETWORKINGSMART` | 993 | @article |

**Total: 23+ entradas @article sin DOI.**

---

## 3. TEXTO PLACEHOLDER / CLAVES SOSPECHOSAS

### 3.1 Clave literal "NoTitleFound"
- **Clave:** `NoTitleFound` (línea 2177)
- **Problema:** La clave es un placeholder evidente. Aunque tiene título real ("M2M Communication Protocols Overview for IoT Applications"), el campo `file` contiene `[No title found].pdf` múltiples veces.
- **Acción:** Renombrar la clave a algo descriptivo o eliminar si no se cita.

### 3.2 DOIs con placeholder "XXXXX"
| Clave | Línea | DOI ficticio |
|-------|-------|--------------|
| `cohenEdgeComputingEnabled2024` | 3251 | `10.1109/TSUSC.2024.XXXXX` |
| `shenAugmentingEdgeIntelligence2024` | 3263 | `10.1145/iot.2024.XXXXX` |

**Acción:** Buscar los DOI reales de estos papers o eliminar el campo DOI.

### 3.3 Claves con nombres de afiliación como autor
| Clave | Línea | Problema |
|-------|-------|---------|
| `assistantprofessordepartmentofcsevasavicollegeofengineeringhyderabadtelanganaindia.AugmentingSecuritySmart2024` | 287 | Clave extremadamente larga con afiliación institucional completa. Zotero importó la afiliación como parte del nombre del autor. |
| `departmentofelectricalengineeringuniversityofmosulmosuliraqSmartFactoryBased2024` | 919 | Mismo problema: afiliación usada como nombre de autor. |
| `masterofengineeringm.e.electricalandelectronicsengineeringlamaruniversityusaIOTENABLEDCONDITIONMONITORING2025` | 1925 | Grado académico usado como nombre de autor. |

---

## 4. ENTRADAS CON AÑO FUTURO O SOSPECHOSO

| Clave | Línea | Año | Observación |
|-------|-------|-----|-------------|
| `sommaTwinArchDigitalTwin2026` | 2756 | 2026-01 | Año futuro. ArXiv preprint aceptado pero aún no publicado. |

---

## 5. ENTRADAS SIN AUTOR

| Clave | Línea | Tipo | Problema |
|-------|-------|------|---------|
| `SmartHomeEnergy2024` | 2741 | @article | **No tiene campo `author`** en absoluto |

---

## 6. @article SIN journaltitle/journal O volume

Entradas de tipo `@article` que carecen de campos obligatorios:

| # | Clave | Línea | Falta `journaltitle` | Falta `volume` | Falta `date` |
|---|-------|-------|---------------------|----------------|--------------|
| 1 | `alamIoTEvolutionEssentials` | 121 | ? | ? | SÍ |
| 2 | `banovicPortovanjeContikiNGOperativnog` | 448 | ? | ? | SÍ |
| 3 | `boikoSoftwareModelingImplementation` | 635 | ? | ? | SÍ |
| 4 | `dongSOFTWAREDEFINEDNETWORKINGSMART` | 993 | ? | ? | SÍ |
| 5 | `hamedSmartWebApplication` | 1143 | SÍ | SÍ | SÍ |
| 6 | `harveSafeguardingIoTBig` | 1151 | SÍ | SÍ | SÍ |
| 7 | `hmissiSurveyApplicationLayer2022` | 1191 | SÍ | SÍ | NO |
| 8 | `hredoyMULTIPURPOSELAMPPOSTURBAN2024` | 1219 | SÍ | SÍ | NO |
| 9 | `huangChannelAccessScheduling` | 1228 | ? | ? | SÍ |
| 10 | `jonnakutiEDGEBASEDFAULTDETECTION` | 1330 | SÍ | NO | SÍ |
| 11 | `josephPerformanceEnergyOptimization2025` | 1340 | SÍ | NO | NO |
| 12 | `makayaEdgeSphereThreeTierArchitecture` | 1850 | ? | ? | SÍ |
| 13 | `naikOptimalSinkNode` | 2135 | ? | ? | SÍ |
| 14 | `rafiReliableCostEfficientIoT` | 2296 | ? | ? | SÍ |
| 15 | `ramzanReviewStudySmart` | 2351 | ? | ? | SÍ |
| 16 | `riazENERGYINFORMATICSSMART` | 2359 | NO | ? | SÍ |
| 17 | `saadHeterogeneousIPv6Infrastructure` | 2422 | ? | ? | SÍ |
| 18 | `saidaSystemeDetectionDIntrusion` | 2448 | ? | ? | SÍ |
| 19 | `sanchezTrabajoFinMaster` | 2472 | ? | ? | SÍ |
| 20 | `silardDemoVisualizationPlatform` | 2697 | ? | ? | SÍ |
| 21 | `tangResearchInteroperabilityIoT` | 2851 | ? | ? | SÍ |
| 22 | `zhouGatewayArchitectureSecurity` | 3207 | ? | ? | SÍ |

**Nota:** Las marcadas con "?" tienen campos tan incompletos que no se pueden verificar volumenes/journals sin revisar manualmente cada una.

---

## 7. @inproceedings SIN booktitle

No se detectaron `@inproceedings` sin `booktitle` en el archivo. Todas las entradas de conferencia revisadas incluyen `booktitle`.

---

## 8. POSIBLES DUPLICADOS

| Entrada A | Entrada B | Similitud |
|-----------|-----------|-----------|
| `chounosScalabilityPerformanceEvaluation2025` (L.769) | `chounosScalabilityPerformanceEvaluation2025a` (L.788) | Mismos autores, mismo título, mismo año. Una es `@inproceedings` (IEEE conf.) y la otra es `@online` (arXiv preprint). **Duplicado semántico.** |
| `IEEE80211ah2016` (~L.3450) | `IEEE802.11ah-2016` (~L.4270) | Ambas son el estándar IEEE 802.11ah-2016. **Duplicado.** |
| `khanWiFiHalowSignal2018` (L.1473) | `khanWiFiHalowSignal2020` (L.1491) | Mismos autores, título similar. Puede ser versión conferencia vs journal. Verificar si son pub. distintas o duplicado. |

---

## 9. INVESTIGACIÓN DE 21 CLAVES ROTAS

Claves citadas en archivos .tex pero que no existen en Referencias.bib:

| # | Clave rota citada en .tex | Clave más similar en .bib | Diagnóstico |
|---|--------------------------|---------------------------|-------------|
| 1 | `*` | N/A | **Cita inválida.** Probablemente un `\cite{*}` erróneo en el .tex. |
| 2 | `DeCastroKorgi2010` | No encontrada | **Sin coincidencia.** Referencia faltante, debe agregarse al .bib. |
| 3 | `ITU-TG.984` | No encontrada | **Sin coincidencia.** Estándar ITU-T G.984 (GPON). Debe crearse la entrada. |
| 4 | `adameTimeVariantChannelModeling2019` | No encontrada | **Sin coincidencia.** Paper de Adame 2019 sobre modelado de canal. Debe agregarse. |
| 5 | `alharbi2021` | `alharbi6LoWPANBasedAMI2021` (L.~3850) | **Probable abreviación.** En .tex se usa `alharbi2021` pero la clave real es `alharbi6LoWPANBasedAMI2021`. Corregir el \cite{} en el .tex. |
| 6 | `amezvaValdDesignImplementation2024` | `amezcuavaldovinosDesignImplementationEvaluation2024` (L.198) | **Typo.** "amezvaVald" ≠ "amezcuavaldovinos". Corregir el \cite{} en el .tex. |
| 7 | `augmentingSecuritySmart2024` | `assistantprofessordepartmentofcse...AugmentingSecuritySmart2024` (L.287) | **Clave truncada.** La clave real tiene el prefijo de afiliación grotescamente largo. Renombrar la clave en el .bib o corregir el \cite{}. |
| 8 | `bahardimanSecureHybridGateway2024` | `biteboDesignImplementationSecured2024` (L.584) | **Posible confusión.** Título de Bitebo menciona "Secured Hybrid Gateway Node". Pero el autor es Bitebo, no "Bahardiman". Puede ser un paper diferente que debe agregarse, o typo de autor. |
| 9 | `broadbandcommissionStateofBroadband2023` | No encontrada | **Sin coincidencia.** Reporte Broadband Commission "State of Broadband 2023". Debe agregarse. |
| 10 | `eldefrawyFormallyVerifiedAuthentication2019` | No encontrada | **Sin coincidencia.** Paper de Eldefrawy 2019 sobre autenticación verificada. Debe agregarse. |
| 11 | `khorovSurveyIEEE802112016` | `khorovEnablingInternetThings2019` (L.1563) | **Diferente paper.** La cita busca un survey de Khorov de 2016 sobre IEEE 802.11. Solo existen entradas de Khorov de 2019 y 2020. Debe agregarse el paper de 2016. |
| 12 | `li2022` | No encontrada | **Sin coincidencia directa.** Clave demasiado genérica. Debe identificarse qué paper de "Li, 2022" se intenta citar y agregar con clave completa. |
| 13 | `liEstimatingPropagationCharacteristics2024` | No encontrada | **Sin coincidencia.** Paper de Li 2024 sobre propagación. Debe agregarse. |
| 14 | `liangSecurityLightweightAuthentication2018` | No encontrada | **Sin coincidencia.** Existe `liangReviewEdgeComputing2024` (L.1761) pero es otro tema/año. Debe agregarse. |
| 15 | `morse_micro_mm8108_2025` | `morsemicroMM8108ProductBrief2025` (L.~3700) | **Formato de clave diferente.** Usa guiones bajos vs camelCase. Corregir el \cite{} para usar `morsemicroMM8108ProductBrief2025`. |
| 16 | `morsemicroMM61082024` | `morsemicroOpenWRTIntegration2024` (L.~3400) y `morsemicroOpenWRT2024` (L.1) | **Sin coincidencia exacta.** No hay entrada con "MM6108" en el nombre. Puede referirse al chipset MM6108 de Morse Micro. Debe crearse la entrada específica. |
| 17 | `park2023` | `parkScalableThreadMesh2023` (L.~3840) | **Probable abreviación.** En .tex se usa `park2023` pero la clave real es `parkScalableThreadMesh2023`. Corregir el \cite{}. |
| 18 | `perezImplicationSoftwareContainerization2024` | `perezContainerizingPowerAPI2025` (L.~3350) | **Diferente paper/año.** Contenido similar (containerización) pero título y año diferentes. Puede ser el mismo paper con clave incorrecta, o uno diferente que debe agregarse. |
| 19 | `sairamIntegratedLightweightCryptographic2024` | No encontrada | **Sin coincidencia.** Paper de Sairam 2024 sobre criptografía ligera. Debe agregarse. |
| 20 | `syamsSecureCommunicationIntegrating2024` | No encontrada | **Sin coincidencia.** Paper de Syams 2024 sobre comunicación segura. Debe agregarse. |
| 21 | `wulurCoordinationEdgeCloud2024` | No encontrada | **Sin coincidencia.** Paper de Wulur 2024 sobre coordinación edge-cloud. Debe agregarse. |

### Resumen de acciones para claves rotas:

| Acción | Cantidad | Claves |
|--------|----------|--------|
| **Corregir \cite{} en .tex** (typo/abreviación) | 5 | `alharbi2021`, `amezvaValdDesignImplementation2024`, `augmentingSecuritySmart2024`, `morse_micro_mm8108_2025`, `park2023` |
| **Agregar entrada al .bib** | 12 | `DeCastroKorgi2010`, `ITU-TG.984`, `adameTimeVariantChannelModeling2019`, `broadbandcommissionStateofBroadband2023`, `eldefrawyFormallyVerifiedAuthentication2019`, `khorovSurveyIEEE802112016`, `li2022`, `liEstimatingPropagationCharacteristics2024`, `liangSecurityLightweightAuthentication2018`, `sairamIntegratedLightweightCryptographic2024`, `syamsSecureCommunicationIntegrating2024`, `wulurCoordinationEdgeCloud2024` |
| **Verificar manualmente** | 3 | `bahardimanSecureHybridGateway2024`, `morsemicroMM61082024`, `perezImplicationSoftwareContainerization2024` |
| **Eliminar cita inválida** | 1 | `*` |

---

## 10. OTROS PROBLEMAS DETECTADOS

### 10.1 Entradas @article sin campo `abstract` y mínimos campos
Varias de las 20 entradas sin fecha (Sección 1) son prácticamente "esqueletos" con solo `title`, `author` y `file`. Parecen importaciones fallidas de Zotero.

### 10.2 Campos `file` con rutas Windows
Muchas entradas contienen `file = {C:\Users\Luis Antonio\Zotero\storage\...}`. Esto no afecta la compilación LaTeX pero es información innecesaria que abulta el archivo.

### 10.3 Entradas arXiv preprints como @article
Entradas como `dettmersGPTQAccuratePost2023` y `touvronLLaMAOpenEfficient2023` usan `journal = {arXiv preprint arXiv:...}` en campos @article. Deberían ser `@online` con campos `eprint`/`eprinttype`.

### 10.4 Posibles entradas no citadas
El archivo tiene ~230+ entradas. Considerar ejecutar un análisis cruzado con los .tex para identificar entradas no citadas y eliminar las innecesarias.

---

## RESUMEN EJECUTIVO

| Categoría | Cantidad |
|-----------|----------|
| Entradas sin fecha/año | 20 |
| Entradas @article sin DOI | 23+ |
| Placeholders/DOIs ficticios | 3 (1 clave + 2 DOIs) |
| Claves con nombre de afiliación | 3 |
| Año futuro (2026) | 1 |
| Sin autor | 1 |
| Posibles duplicados | 2-3 |
| Claves rotas en .tex | 21 |
| → Corregibles con typo fix | 5 |
| → Requieren nuevo .bib entry | 12 |
| → Verificar manualmente | 3 |
| → Cita inválida (`*`) | 1 |

### Prioridad de corrección:
1. **CRÍTICA:** Corregir las 5 claves rotas por typo (fix en .tex)
2. **CRÍTICA:** Agregar las 12 entradas faltantes al .bib  
3. **ALTA:** Completar fecha en las 20 entradas sin año
4. **ALTA:** Reemplazar 2 DOIs con "XXXXX" por DOIs reales
5. **MEDIA:** Eliminar/renombrar clave `NoTitleFound`
6. **MEDIA:** Agregar autor a `SmartHomeEnergy2024`
7. **MEDIA:** Resolver duplicado `IEEE80211ah2016` / `IEEE802.11ah-2016`
8. **BAJA:** Limpiar claves con nombres de afiliación
9. **BAJA:** Eliminar rutas `file` Windows innecesarias
