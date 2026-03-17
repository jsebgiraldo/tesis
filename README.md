# Tesis de Maestria - Arquitectura IoT para Smart Energy

**Arquitectura IoT Centrada en Pasarelas de Borde: Implementacion de Protocolos basados en 6LowPAN para Smart Energy**

Juan Sebastian Giraldo Duque  
Universidad Nacional de Colombia - Sede Manizales  
Maestria en Ingenieria - Automatizacion Industrial  
2025

---

## Inicio Rapido - Clonar y Compilar en otro PC

```bash
# 1. Clonar repositorio
git clone https://github.com/jsebgiraldo/tesis.git
cd tesis

# 2. Instalar TeX Live (Linux/macOS) o MiKTeX (Windows)
#    Ver INSTALACION.md para detalles

# 3. Compilar PDF
cd Tesis___Trabajo_final___Maestria___2025
pdflatex -shell-escape 0000.tex
bibtex 0000
pdflatex -shell-escape 0000.tex
pdflatex -shell-escape 0000.tex
# Output: 0000.pdf

# 4. (Opcional) Entorno Python para scripts auxiliares
cd ..
python3 -m venv .venv && source .venv/bin/activate
pip install pyserial
```

Ver [INSTALACION.md](./INSTALACION.md) para la guia completa.

---

## Estructura del Repositorio

```
tesis/
|-- Tesis___Trabajo_final___Maestria___2025/   # LaTeX - documento principal
|   |-- 0000.tex                                # Entrada principal (pdflatex)
|   |-- 0000_slim.tex                           # Version ligera sin anexos
|   |-- 01Introduccion.tex                      # Cap 1: Introduccion
|   |-- 02MarcoTeorico.tex                      # Cap 2: Marco Teorico
|   |-- 03NodoIoT_Thread_LwM2M.tex             # Cap 3: Nodo IoT
|   |-- 04Gateway_HaLow_Edge.tex                # Cap 4: Gateway HaLow + Edge
|   |-- 05Server_ThingsBoard_Kafka.tex          # Cap 5: Servidor ThingsBoard
|   |-- 06Resultados_Validacion.tex             # Cap 6: Resultados
|   |-- 07Conclusiones_TrabajoFuturo.tex        # Cap 7: Conclusiones
|   |-- 09AnexoA ... 15AnexoG_*.tex             # 7 Anexos tecnicos
|   |-- figures/                                # Figuras PNG/SVG
|   |-- 00Figuras/                              # Diagramas TikZ
|   +-- Bibliografia.bib                        # 180+ referencias
|
|-- codigo/                                     # Firmware + herramientas
|   |-- lwm2m-thread-app/                       # Firmware Zephyr ESP32-C6
|   |-- zephyr-app/                             # App Zephyr base
|   |-- scripts/                                # Scripts de pruebas
|   |-- docker-compose.yml                      # Stack ThingsBoard Edge
|   +-- *.py                                    # Herramientas seriales/OTA
|
|-- capitulos/                                  # Notas Obsidian por capitulo
|-- conceptos/                                  # Notas de conceptos clave
|-- datos/                                      # Datasets experimentales
|   |-- firmware_tests/                         # Logs de pruebas firmware
|   |-- power_measurements/                     # Mediciones de consumo
|   +-- thread_logs/                            # Logs Thread/CoAP
|
|-- docs_sesiones/                              # Auditorias y proceso de revision
|-- fuentes/                                    # Fuentes de figuras
|
|-- .gitignore
|-- INSTALACION.md                              # Guia de instalacion
|-- README.md                                   # Este archivo
+-- README_OBSIDIAN.md                          # Guia del vault Obsidian
```

### Contenido NO versionado (ver .gitignore)

| Directorio / Archivo | Descripcion | Como obtenerlo |
|---|---|---|
| `ref/` | 129 PDFs de bibliografia (~400 MB) | Descargar de Zotero/Scholar |
| `rev/` | PDFs de revisiones intermedias | Compilar en cada version |
| `.venv/` | Entorno virtual Python | `python3 -m venv .venv` |
| `Tesis.../ref_canon/` | Store canonico de PDFs + symlinks | Reconstruir con Zotero |
| `rework.docx`, `tesis.drawio` | Archivos de trabajo binarios | Locales |

---

## Descripcion

Tesis que propone, implementa y valida una arquitectura IoT de 4 niveles para AMI en redes Smart Energy. Integra Thread (802.15.4), Wi-Fi HaLow (802.11ah) y LTE Cat-M1, con edge computing Docker y cumplimiento de IEEE 2030.5 / ISO/IEC 30141.

**Despliegue experimental:** 30 medidores Itron SL7000 - 90 dias en entorno urbano.

```
Nivel 1 - Campo:   [ESP32-C6 Thread] - CoAP/LwM2M - DCU
Nivel 2 - Barrio:  [DCU] <- HaLow 920 MHz -> [Gateway]
Nivel 3 - Edge:    [RPi 4 + Docker] - ThingsBoard Edge
Nivel 4 - Cloud:   [AWS] - EC2, RDS, ElastiCache, MSK Kafka
```

## Resultados Clave

| Metrica | Resultado |
|---|---|
| Latencia edge processing | 8 +/- 2 ms (p99: 14 ms) |
| Disponibilidad offline 48h | 99.7 % |
| Reduccion trafico WAN | 72 % |
| Reduccion overhead 6LoWPAN | 78.1 % |
| TCO piloto 90 dias | $1,946 ($10.13/dispositivo) |
| Ahorro energetico vs LTE | 85.5 % |

## Compilacion

```bash
cd Tesis___Trabajo_final___Maestria___2025/
pdflatex -interaction=nonstopmode -shell-escape 0000.tex
bibtex 0000
pdflatex -interaction=nonstopmode -shell-escape 0000.tex
pdflatex -interaction=nonstopmode -shell-escape 0000.tex
```

**Output:** `0000.pdf`

## Licencia

(c) 2025 Juan Sebastian Giraldo Duque - Universidad Nacional de Colombia, Sede Manizales.  
Todos los derechos reservados.
