# Revisión tesis-rev11-com — Resumen de cambios aplicados

## Origen
Se extrajeron 18 anotaciones del PDF `rev/tesis-rev11-com.pdf` y se aplicaron correcciones
en los archivos LaTeX de la carpeta `Tesis___Trabajo_final___Maestria___2025/`.

---

## Arquitectura corregida (referencia)
| Componente | Antes (incorrecto) | Después (correcto) |
|---|---|---|
| Servidor LwM2M | Eclipse Leshan standalone | Transporte LwM2M integrado en TB Edge 3.6.2 |
| Docker containers | 6 (TB Edge, PG/TimescaleDB, Mosquitto, Node-RED, Grafana, OTBR) | 3 (TB Edge 3.6.2, PostgreSQL 15, Redis 7.0) |
| OTBR | Contenedor Docker | Servicio nativo de OpenWRT |
| SO pasarela | Ubuntu 22.04 | OpenWRT 23.05 (fork Morse Micro) |
| Servidor central | ThingsBoard Cloud / AWS | ThingsBoard on-premise (gRPC bidireccional) |
| Enlace WAN | mwan3 con 3 interfaces (Ethernet, HaLow, LTE) | Malla HaLow 802.11s HWMP + Ethernet respaldo |
| Módulo celular | Quectel EG25-G LTE | Eliminado |
| Piloto | 60 medidores físicos | 1 medidor Emsitech P2000-T + nodos simulados |

---

## Archivos modificados

### 1. `04Gateway_HaLow_Edge.tex` — Reescritura completa Ch4
- **§4.1** Introducción: 3 roles del gateway (OTBR, plataforma, router HaLow)
- **§4.2** Arquitectura RPi4: tabla comparativa hardware
- **§4.3** OpenWRT: justificación, build Morse Micro, configuración de red (NUEVO)
- **§4.4** OTBR: servicio nativo OpenWRT, NAT64, commissioner, validación
- **§4.5** ThingsBoard Edge: 3 contenedores Docker, LwM2M integrado, rule chains, comunicación on-premise, operación offline, tabla comparativa vs Leshan externo
- **§4.6** Topología de red y flujo de datos: 3 segmentos, flujo E2E
- **§4.7** HaLow: comparación tecnologías, Alfa Networks Tube AHM, malla 802.11s HWMP, RAW, validación
- **§4.8** Conclusiones: 6 contribuciones corregidas

### 2. `fig_docker_stack.tex` — Reescritura completa
- Antes: TikZ con 6 microservicios sobre Ubuntu 22.04
- Después: TikZ con 3 contenedores (TB Edge, PostgreSQL, Redis) sobre OpenWRT + OTBR como servicio nativo externo

### 3. `03NodoIoT_Thread_LwM2M.tex` — 3 secciones modificadas
- **§3.3.3** "Gestión de Energía y Modos de Operación": ELIMINADA (energía no es restricción, nodo alimentado desde medidor 5V)
- **§3.6.1** "Pruebas de Consumo Energético": ELIMINADA (mediciones PPK2 no realizadas)
- **§3.7** Conclusiones: eliminadas refs energía (135mW, 81% margen), piloto corregido ("1 medidor Emsitech + nodos simulados"), eliminada ref "enlace troncal LTE/fibra"

### 4. `02MarcoTeorico_Rework_V2.tex` — 6 correcciones puntuales
- "servidor LwM2M Leshan" → "transporte LwM2M integrado en ThingsBoard Edge"
- "OTBR opera como contenedor Docker nativo" → "servicio nativo de OpenWRT"
- Agregada mención EFR25 Wi-SUN 1.1
- Tabla comparativa: "Sí (Leshan)" → "Sí (TB Edge)"
- "implementado con Eclipse Leshan" → "transporte LwM2M integrado en TB Edge"
- Brecha 1: "multi-radio HaLow + LTE" → "malla 802.11s para AMI"

### 5. `01Introduccion.tex` — 10 correcciones
- **OE3**: 6 servicios Docker → 3 contenedores + OTBR nativo
- **OE5**: "mwan3 con 3 interfaces (Ethernet, HaLow, LTE)" → malla HaLow 802.11s HWMP
- **OE7**: "60 nodos Zephyr durante 90 días" → "1 medidor Emsitech + nodos simulados"
- **Alcances #2**: eliminado LTE (Quectel EG25-G), 7→3 contenedores
- **Alcances #4**: "conmutación por fallo" → "resiliencia de enlace troncal"
- **Limitaciones #1**: "60 nodos" → "1 medidor físico y nodos simulados"
- **Aporte académico #3**: "conmutación por fallo multi-WAN" → "integración Thread–HaLow con malla 802.11s"
- **Fase 2**: "ThingsBoard Cloud" → "on-premise"; "AHPI7292S" → "Tube AHM"; eliminado mwan3
- **Contribuciones técnicas #4**: "TimescaleDB, Mosquitto" → "PostgreSQL, Redis"
- **Descripción Cap 4**: eliminado "conmutación por fallo multi-WAN"

### 6. `05Server_ThingsBoard_Kafka.tex` — Nota introductoria corregida
- "ThingsBoard Cloud como servicio gestionado" → "ThingsBoard on-premise local con sincronización gRPC"
- Reenmarcado como diseño de referencia para escalamiento con Kafka

---

## Comentarios del PDF atendidos
| # | Comentario | Estado |
|---|---|---|
| 1 | Alinear párrafos (indentado) Ch1 | Pendiente (formato PDF, no contenido) |
| 2 | Ch4: no definimos buen cuerpo/estructura | ✅ Reescritura completa |
| 3 | Ch4: OTBR no es contenedor Docker | ✅ Servicio nativo OpenWRT |
| 4 | Ch4: no usamos Leshan standalone | ✅ TB Edge con LwM2M integrado |
| 5 | Ch4: no hay 6 microservicios | ✅ 3 contenedores (TB Edge, PG, Redis) |
| 6 | Ch4: agregar sección OpenWRT | ✅ Nueva §4.3 |
| 7 | Ch4: TB Edge integra LwM2M | ✅ Documentado en §4.5 |
| 8 | Ch3: eliminar sección energía | ✅ §3.3.3 y §3.6.1 eliminadas |
| 9 | Ch3: piloto no son 60 medidores | ✅ Corregido a 1 Emsitech + simulados |
| 10 | Ch3: no hay enlace LTE/fibra | ✅ Eliminada referencia |
| 11 | Ch2: Leshan → TB Edge | ✅ Corregido |
| 12 | Ch2: OTBR Docker → nativo | ✅ Corregido |
| 13 | Ch2: agregar EFR25 Wi-SUN | ✅ Agregado |
| 14 | Ch2: Brecha 1 sin multi-radio LTE | ✅ Corregido |
| 15 | Ch1: OE3 6 servicios Docker | ✅ Corregido a 3 contenedores |
| 16 | Ch1: OE5 mwan3 + LTE | ✅ Corregido a malla HaLow |
| 17 | Ch1: alcances LTE + 7 Docker | ✅ Corregido |
| 18 | Ch4: Alfa Networks intro mejorada | ✅ Tube AHM (MM6108) |

---

## Pendiente (fuera del alcance de esta revisión)
- **Comentario #1**: Ajuste visual de indentado en Ch1 (requiere revisar PDF renderizado)
- **fig_docker_stack.tex**: Verificar compilación TikZ del diagrama nuevo
- **Compilación general**: Ejecutar `latexmk` para confirmar que no hay errores de compilación
- **Ch5 completo**: El capítulo describe arquitectura AWS/Kafka a escala; se corrigió la nota introductoria pero el contenido detallado permanece como diseño de referencia (trabajo futuro)
