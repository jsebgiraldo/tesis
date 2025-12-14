# Tesis de Maestría - Arquitectura IoT para Smart Energy

**Arquitectura IoT Centrada en Pasarelas de Borde: Implementación de Protocolos basados en 6LowPAN para Smart Energy**

Juan Sebastian Giraldo Duque  
Universidad Nacional de Colombia - Sede Manizales  
Maestría en Ingeniería - Automatización Industrial  
2025

## 📋 Descripción

Tesis de maestría que propone, implementa y valida una arquitectura IoT de 4 niveles para infraestructuras de medición inteligente (AMI) en redes Smart Energy. La arquitectura integra Thread (802.15.4), Wi-Fi HaLow (802.11ah) y LTE Cat-M1, con edge computing basado en Docker y cumplimiento de estándares IEEE 2030.5 e ISO/IEC 30141.

**Despliegue experimental:** 30 medidores Itron SL7000 operando durante 90 días en entorno urbano.

## 🏗️ Arquitectura del Sistema (4 Niveles)

```
Nivel 1 - Campo:     [ESP32-C6 Thread Nodes] → CoAP/LwM2M → DCU
Nivel 2 - Barrio:    [DCU] ← HaLow 920 MHz 2/4 MHz → [Gateway]
Nivel 3 - Edge:      [Raspberry Pi 4 + Docker] → ThingsBoard Edge
Nivel 4 - Cloud:     [AWS] → EC2, RDS PostgreSQL, ElastiCache, MSK Kafka
```

## 📁 Estructura del Repositorio

```
├── Tesis___Trabajo_final___Maestria___2025/  # Archivos LaTeX de la tesis
│   ├── 0000.tex                               # Documento principal
│   ├── 01Introduccion.tex                     # Cap 1: Introducción
│   ├── 02MarcoTeorico.tex                     # Cap 2: Marco Teórico
│   ├── 03Arquitectura_NEW.tex                 # Cap 3: Arquitectura (4 niveles)
│   ├── 04Implementacion_NEW.tex               # Cap 4: Implementación
│   ├── 05Resultados_NEW.tex                   # Cap 5: Resultados Experimentales
│   ├── 05Conclusiones.tex                     # Conclusiones y Trabajo Futuro
│   ├── 09AnexoA-15AnexoG_*.tex               # 7 Anexos técnicos
│   ├── 00Figuras/                             # Diagramas y figuras
│   └── Bibliografia.bib                       # Referencias (180+ papers)
├── docs_sesiones/                             # Documentación de proceso
│   ├── AUDITORIA_REFERENCIAS_CRUZADAS.md      # Validación de citas
│   ├── DEFENSA_PREGUNTAS_WISUN.md             # Respuestas comité
│   ├── DEFINICIONES_TECNICAS_TESIS.md         # Glosario técnico
│   ├── HISPANIZACION_COMPLETA.md              # Términos en español
│   ├── INSTRUCCIONES_FINALES.md               # Guía compilación
│   └── RESUMEN_PARA_TUTOR_NOV25.md            # Resumen ejecutivo
├── ref/                                       # PDFs de estándares y papers
├── tesis.drawio                               # Diagramas de arquitectura
└── README.md                                  # Este archivo
```

## 🔧 Tecnologías Implementadas

### Hardware
- **Nodos Thread:** ESP32-C6 (RISC-V, 160 MHz, OpenThread main con TCP)
- **Gateway Edge:** Raspberry Pi 4 (4 GB RAM, BCM2711)
- **Radio HaLow:** Morse Micro MM6108 (920 MHz, 2/4/8 MHz BW)
- **Modem LTE:** Quectel EG25-G (Cat-M1, eDRX + PSM)

### Software
- **OS Gateway:** OpenWRT 23.05 (custom build Morse Micro)
- **Stack Edge:** Docker Compose (ThingsBoard Edge, PostgreSQL, Kafka)
- **Protocolos:** Thread, 6LoWPAN, CoAP, LwM2M, MQTT, HTTP/REST
- **Cloud:** ThingsBoard 3.6.2 Community Edition en AWS

## 📊 Resultados Clave

- ✅ **Latencia edge processing:** 8±2 ms (p95: 10 ms, p99: 14 ms)
- ✅ **Disponibilidad offline:** 99.7% durante 48h sin WAN
- ✅ **Reducción tráfico WAN:** 72% mediante edge processing
- ✅ **Reducción overhead headers:** 78.1% (6LoWPAN IPHC/NHC)
- ✅ **TCO piloto 90 días:** $1,946 total = $10.13/dispositivo
- ✅ **Ahorro energético:** 85.5% vs arquitectura LTE siempre activa

## 🚀 Compilación del Documento

### Requisitos
- **LaTeX:** TeX Live 2023+ o MiKTeX
- **pdflatex** con soporte UTF-8
- **bibtex** para bibliografía
- Paquetes: babel, minted, hyperref, natbib, xcolor, etc.

### Comandos de Compilación

```bash
cd Tesis___Trabajo_final___Maestria___2025/

# Compilación completa (4 pasos para referencias cruzadas)
pdflatex -interaction=nonstopmode 0000.tex
bibtex 0000
pdflatex -interaction=nonstopmode 0000.tex
pdflatex -interaction=nonstopmode 0000.tex
```

**Output:** `0000.pdf` (~326 páginas)

### Solución de Problemas

Si aparecen errores Unicode, asegúrate de que todos los archivos `.tex` están en UTF-8:

```bash
# Verificar encoding
file -i *.tex

# Convertir a UTF-8 si es necesario
iconv -f ISO-8859-1 -t UTF-8 archivo.tex > archivo_utf8.tex
```

## 📖 Contenido de los Capítulos

### Capítulo 1: Introducción (30 págs)
- Contexto Smart Grid y AMI
- Planteamiento del problema
- Objetivos general y específicos
- Hipótesis de investigación (H1-H8)
- Justificación técnica y económica

### Capítulo 2: Marco Teórico (45 págs)
- Estándares: IEEE 2030.5, ISO/IEC 30141, Thread, HaLow
- Protocolos: 6LoWPAN, CoAP, LwM2M, MQTT
- Edge computing y arquitecturas IoT
- Estado del arte (180+ referencias)

### Capítulo 3: Arquitectura del Sistema (60 págs)
- Arquitectura de 4 niveles (Campo → Barrio → Edge → Cloud)
- Especificación técnica de cada nivel
- Flujos de datos y protocolos
- Seguridad y resiliencia
- Análisis comparativo vs arquitecturas baseline

### Capítulo 4: Implementación (55 págs)
- Firmware ESP32-C6 (Thread + DLMS parser)
- Configuración gateway OpenWRT + Docker
- Deployment ThingsBoard Edge + Cloud
- Procedimientos de instalación y comisionamiento
- Troubleshooting y lecciones aprendidas

### Capítulo 5: Resultados Experimentales (70 págs)
- Setup del piloto (30 medidores, 90 días)
- Métricas de latencia, disponibilidad, throughput
- Validación de hipótesis H1-H8
- Análisis de escalabilidad y TCO
- Comparación con literatura (benchmarking)

### Conclusiones (35 págs)
- Cumplimiento de objetivos
- Validación de hipótesis
- Contribuciones científicas
- Limitaciones del trabajo
- Trabajo futuro (roadmap 2025-2030)

### Anexos (31 págs)
- A: Instalación OpenWRT en Raspberry Pi 4
- B: Configuración Thread Border Router
- C: Configuración HaLow (AP/Mesh)
- D: Especificaciones IEEE 2030.5
- E: Código fuente nodo ESP32-C6
- F: Configuraciones OpenWRT completas
- G: Hipótesis detalladas (H1-H8)

## 📈 Estado del Proyecto

✅ **Completado:**
- Todos los capítulos (1-5) escritos y revisados
- 7 anexos técnicos con código fuente y configuraciones
- 180+ referencias bibliográficas validadas
- Limpieza Unicode/Cyrillic (50+ caracteres corregidos)
- Workspace consolidado (nov 26, 2025)

⏸️ **Pendiente:**
- Resolución de 3 caracteres U+FFFD (replacement character)
- Compilación PDF final limpia sin warnings
- Revisión final del tutor
- Preparación para sustentación

## 🔬 Validación de Hipótesis

| Hipótesis | Estado | Resultado |
|-----------|--------|-----------|
| **H1** - 6LoWPAN reduce overhead >70% | ✅ VALIDADA | 78.1% reducción |
| **H2** - Edge processing reduce WAN >65% | ✅ VALIDADA | 72% reducción |
| **H3** - HaLow multi-BW optimiza cobertura | ✅ VALIDADA | PDR >98% @ 2 MHz |
| **H4** - IPHC comprime headers >85% | ✅ VALIDADA | 87% compresión |
| **H5** - CoAP reduce latencia >50% vs MQTT | ✅ VALIDADA | 63% reducción |
| **H6** - LwM2M optimiza gestión dispositivos | ✅ VALIDADA | 85% menos tráfico |
| **H7** - CEP edge procesa >10k evt/s <10ms | ⚠️ PARCIAL | 12.3k evt/s, 8±2 ms |
| **H8** - Arquitectura supera baseline 5/7 métricas | ✅ VALIDADA | 7/7 métricas |

## 🌐 Estándares y Conformidad

- ✅ **IEEE 2030.5-2018:** Smart Energy Profile 2.0 (SEP 2.0)
- ✅ **ISO/IEC 30141:2024:** IoT Reference Architecture (7 FEs implementadas)
- ✅ **OpenThread main:** IPv6 mesh networking over 802.15.4 con soporte TCP nativo
- ✅ **IEEE 802.11ah-2016:** Wi-Fi HaLow (sub-GHz)
- ✅ **LwM2M 1.2:** OMA SpecWorks device management
- ✅ **MQTT 5.0:** ISO/IEC 20922
- ✅ **IEC 62056-21:** DLMS/COSEM meter reading

## 📝 Cambios Recientes

### Nov 26, 2025 - Limpieza Unicode y Workspace

**Commit 4e77cb1:** `fix: Limpieza Unicode y Cyrillic en LaTeX (50+ caracteres)`
- Reemplazados símbolos Unicode con comandos LaTeX apropiados
- Greek letters: μ → `$\mu$`, Ω → `$\Omega$`, φ → `$\varphi$`
- Operadores: ≥ → `$\geq$`, → → `$\rightarrow$`, ↔ → `$\leftrightarrow$`
- Subscripts: ₂ → `$_{2}$`
- Triángulos: ▶ → `$\blacktriangleright$`
- Corregidos caracteres Cyrillic (П, О, Н) con P, O, N latinas

**Commit 613d18c:** `chore: Limpieza workspace - eliminación archivos obsoletos`
- Eliminados 10 archivos markdown de sesiones antiguas
- Removidos scripts legacy y backups innecesarios
- Workspace consolidado para versión final de tesis

## 📚 Referencias Clave

- **Velásquez et al. (2024):** Smart Grids empowered by 5G and IoT
- **Alsafran et al. (2025):** Challenges implementing IoT in AMI
- **Knyazev et al. (2017):** IEEE 2030.5 vs DLMS/COSEM comparative analysis
- **Tang et al. (2024):** Interoperability research in IoT architectures
- **Liang et al. (2024):** Review of edge computing for IoT

## 📄 Licencia

© 2025 Juan Sebastian Giraldo Duque  
Universidad Nacional de Colombia - Sede Manizales  
Todos los derechos reservados

---

**Última actualización:** Noviembre 26, 2025  
**Estado:** Pre-defensa (correcciones finales)  
**Páginas:** 326 (incluye anexos)
