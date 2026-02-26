# Inventario Completo de Figuras — Tesis Maestría 2025

**Generado:** 2026-02-22  
**Directorio base:** `Tesis___Trabajo_final___Maestria___2025/`

---

## Resumen Ejecutivo

| Capítulo | Líneas | Tablas | Figuras existentes | Figuras sugeridas |
|----------|--------|--------|--------------------|-------------------|
| Ch1 – 01Introduccion.tex | 568 | 4 | **0** | 4 |
| Ch2 – 02MarcoTeorico_Rework_V2.tex | 720 | 4 | **0** | 5 |
| Ch3 – 03NodoIoT_Thread_LwM2M.tex | 1216 | 7 | 5 (2 inline + 3 via `\input`) | 3 |
| Ch4 – 04Gateway_HaLow_Edge.tex | 1462 | 6 | 9 (6 inline + 3 via `\input`) | 2 |
| Ch5 – 06Resultados_Validacion.tex | 1713 | 25 | 3 (via `\input` at top) | 6 |
| Ch6 – 07Conclusiones_TrabajoFuturo.tex | 1377 | 10 | 2 | 3 |
| Anexo – 05Server_ThingsBoard_Kafka.tex | 957 | 3 | 1 | 2 |
| **TOTAL** | **8013** | **59** | **20** | **25** |

**3 de 6 capítulos principales (Ch1, Ch2, Ch5) NO tienen ninguna figura.** Además, 2 figuras existentes están marcadas como `[FIGURA PENDIENTE]` (solo caption, sin imagen generada).

---

## Ch1 — 01Introduccion.tex (568 líneas)

### Figuras Existentes
**NINGUNA** — El capítulo contiene 4 tablas comparativas pero cero figuras.

### Figuras Sugeridas

| # | Concepto | Sección / Línea | Tipo Sugerido | Justificación |
|---|----------|-----------------|---------------|---------------|
| 1 | **Arquitectura general de la propuesta (overview de 4 capas)** | §1.1 Contexto (~L8) | Diagrama de bloques | El capítulo describe extensamente la arquitectura dispositivo→Thread→HaLow→Cloud pero nunca la visualiza. Una figura overview sería esencial para que el lector entienda la propuesta desde el capítulo 1. Imagen `figures/arquitectura-completa.png` existe pero no se referencia. |
| 2 | **Comparación visual tecnologías IoT (radar chart)** | §1.1.1 Comparativa Técnica (~L24) | Radar/Spider chart | Las 3 tablas comparativas (mesh, edge, backhaul) se beneficiarían de un gráfico radar multidimensional que sintetice las diferencias clave. |
| 3 | **Brechas en arquitecturas existentes** | §1.2 Brechas (~L148) | Diagrama causa-efecto | Se describen 3 brechas (cloud-centric SPOF, overhead multi-protocolo, ausencia procesamiento distribuido) que se podrían visualizar como fishbone/Ishikawa. |
| 4 | **Metodología — Fases de investigación** | §1.5 Metodología (~L385) | Flowchart/timeline | 4 fases secuenciales (Análisis→Implementación→Validación→Evaluación) descritas textualmente, ideales para un diagrama de flujo. |

---

## Ch2 — 02MarcoTeorico_Rework_V2.tex (720 líneas)

### Figuras Existentes
**NINGUNA** — 4 tablas pero cero figuras. El capítulo de marco teórico de 720 líneas carece completamente de representaciones gráficas.

### Figuras Sugeridas

| # | Concepto | Sección / Línea | Tipo Sugerido | Justificación |
|---|----------|-----------------|---------------|---------------|
| 1 | **Arquitectura NIST Smart Grid (3 capas)** | §2.2.1 Arquitectura NIST (~L30) | Diagrama de capas | Se describen 3 capas (Infraestructura, Comunicación, Aplicación) textualmente. Imagen existente en tesis anterior (`02MarcoTeorico.tex`) usaba `protocol-stack.png`. |
| 2 | **Stack de protocolos 6LoWPAN/CoAP/LwM2M** | §2.3.3 Gestión Dispositivos (~L347) | Diagrama de stack | La pila de protocolos Thread/CoAP/LwM2M descrita extensamente. Archivo `fig_protocol_stack.tex` existe con imagen `protocol_stack.png` pero NO está incluido en Ch2. |
| 3 | **Comparación LPWAN (LoRaWAN vs NB-IoT vs HaLow vs Wi-SUN)** | §2.3.1 Tecnologías LPWAN (~L100) | Gráfico de barras/radar | Tabla comparativa cuantitativa (L213) sería más impactante con visualización gráfica (throughput, alcance, latencia, costo). |
| 4 | **Arquitectura híbrida Edge-Cloud** | §2.3.4.3 Arquitectura Híbrida (~L405) | Diagrama de bloques | Sección describe distribución tareas edge vs cloud con tabla, pero falta diagrama de flujo de datos entre capas. |
| 5 | **Topología Wi-SUN FAN vs Thread** | §2.3.1.5 Wi-SUN (~L277) | Diagrama comparativo | Comparación arquitectónica RPL tree-based (Wi-SUN) vs MLE mesh flat (Thread); ideal para diagrama lado a lado. |

---

## Ch3 — 03NodoIoT_Thread_LwM2M.tex (1216 líneas)

### Figuras Existentes

| # | Archivo/Tipo | Label | Caption (resumen) | Línea |
|---|-------------|-------|-------------------|-------|
| 1 | `\input{fig_protocol_stack}` → `figures/protocol_stack.png` | `fig:protocol-stack` | Pila completa protocolos Thread/LwM2M (7 capas IEEE 802.15.4 → LwM2M 1.1) | L25 |
| 2 | `figures/openthread-osi-model.jpg` | `fig:openthread-osi-model` | Modelo OSI del stack OpenThread en nodos ESP32-C6 | L59-63 |
| 3 | `\input{fig_thread_topology_roles}` | `fig:thread-topology-roles` | **[FIGURA PENDIENTE]** Topología Thread mesh con 5 roles jerárquicos | L71 |
| 4 | `figures/1-113991254-seeedxiao-esp32c6-45font_1.jpg` | `fig:xiao-hardware` (subfig a) | XIAO ESP32C6 (21×17.5 mm) — foto hardware | L460-464 |
| 5 | `figures/1-113991354_1.jpg` | `fig:xiao-hardware` (subfig b) | RS485 Breakout con aislamiento galvánico — foto hardware | L460-470 |

### Figuras Sugeridas

| # | Concepto | Sección / Línea | Tipo Sugerido | Justificación |
|---|----------|-----------------|---------------|---------------|
| 1 | **Diagrama de bloques ESP32-C6** | §3.3 Implementación Hardware (~L477) | Diagrama de bloques | Arquitectura interna del SoC descrita con detalle. El archivo `fig_esp32c6_block_diagram.tex` (con imagen `esp32c6_block_diagram.png`) **existe pero NO está incluido** en este capítulo. |
| 2 | **Flujo DLMS/COSEM → LwM2M (mapeo de datos)** | §3.5 Configuración Perfil LwM2M (~L868) | Diagrama de flujo | Proceso de traducción OBIS → IPSO Objects → ThingsBoard telemetry keys descrito extensamente con tablas. Un diagrama de flujo end-to-end sería clarificador. |
| 3 | **Diagrama de secuencia: commissioning Thread** | §3.2.2 Auto-Formación (~L85) | Diagrama de secuencia | Proceso MLE completo (Discovery→DTLS→Dataset→Join) descrito textualmente, ideal para sequence diagram. |

---

## Ch4 — 04Gateway_HaLow_Edge.tex (1462 líneas)

### Figuras Existentes

| # | Archivo/Tipo | Label | Caption (resumen) | Línea |
|---|-------------|-------|-------------------|-------|
| 1 | `\input{fig_docker_stack}` | `fig:docker-stack` | **[FIGURA PENDIENTE]** Arquitectura Docker Compose 6 contenedores Gateway Edge | L100 |
| 2 | `\input{fig_halow_mesh_topology}` (TikZ) | `fig:halow-mesh-topology` | Topología HaLow Mesh peer-to-peer con Gateways Edge distribuidos | L522 |
| 3 | `\input{fig_halow_hardware_detail}` (TikZ) | `fig:halow-hardware-detail` | Detalle hardware Alfa Networks Tube AHM + RPi4 | L662 |
| 4 | `figures/thingsboard-edge-architecture-hq.png` | `fig:edge-architecture-official` | Arquitectura oficial ThingsBoard Edge 3.6.2 (7 componentes) | L813-817 |
| 5 | `figures/tb-edge-offline.svg` | `fig:edge-offline-operation` | Operación autónoma Edge durante desconexión WAN | L1188-1192 |
| 6 | `figures/tb-edge-filtering.svg` | `fig:edge-data-filtering` | Arquitectura filtrado datos Edge → Cloud (reducción 72% tráfico) | L1201-1205 |
| 7 | `figures/tb-edge-alarm.svg` | `fig:edge-local-alarms` | Flujo detección alarmas Edge con latencia ultra-baja (8 ms P95) | L1220-1224 |
| 8 | `figures/tb-edge-overview.svg` | `fig:edge-dashboard-topology` | Topología acceso dashboards ThingsBoard Edge (LAN + Cloud) | L1238-1242 |
| 9 | `figures/tb-edge-batch-update.svg` | `fig:edge-batch-update` | Actualización batch configuraciones Edge desde Cloud | L1279-1283 |

### TikZpicture Environments
- `fig_halow_mesh_topology.tex` (L7): Topología HaLow Mesh completa con 3 Tubes + 2 RPi4
- `fig_halow_hardware_detail.tex` (L6): Diagrama detalle hardware Tube AHM + RPi4

### Figuras Sugeridas

| # | Concepto | Sección / Línea | Tipo Sugerido | Justificación |
|---|----------|-----------------|---------------|---------------|
| 1 | **Rule Chains ThingsBoard Edge (flujo de procesamiento)** | §4.4.4 Rule Chains (~L987) | Flowchart | Se describen 4 Rule Chains detalladamente (Filtering, Alarm, Enrichment, Downlink) pero no hay diagrama visual del flujo de procesamiento. |
| 2 | **Diagrama de red completo: Thread + HaLow + WAN** | §4.1 Arquitectura Gateway (~L18) | Diagrama de red | Sección merece un diagrama de red end-to-end que muestre las interfaces de red del RPi4 (wlan0, eth0, wpan0, wwan0). |

---

## Ch5 — 06Resultados_Validacion.tex (1713 líneas)

### Figuras Existentes
**Solo 3 figuras**, todas incluidas como `\input` al inicio del archivo (L1-5) y provenientes de archivos fig_*:

| # | Archivo/Tipo | Label | Caption (resumen) | Línea |
|---|-------------|-------|-------------------|-------|
| 1 | `\input{fig_architecture_comparison}` → `figures/architecture_comparison.png` | `fig:architecture-comparison` | Comparación 3 arquitecturas IoT AMI: Centralizada vs Edge vs Híbrida | L1 |
| 2 | `\input{fig_validation_heatmap}` → `figures/validation_heatmap.png` | `fig:validation-heatmap` | Heatmap validación hipótesis H1-H8 (escala 0-100%) | L3 |
| 3 | `\input{fig_cost_breakdown}` → `figures/cost_breakdown.png` | `fig:cost-breakdown` | Análisis económico CAPEX/OPEX edge vs centralizada, TCO 36 meses | L5 |

### Figuras Sugeridas

| # | Concepto | Sección / Línea | Tipo Sugerido | Justificación |
|---|----------|-----------------|---------------|---------------|
| 1 | **Distribución latencia edge (histograma/boxplot)** | §5.3.1 Latencia Edge Processing (~L193) | Gráfico estadístico | Tabla L208 tiene datos P50/P95/P99 perfectos para visualización. Archivo `fig_latency_results.tex` con `latency_results.png` **existe pero NO está incluido**. |
| 2 | **Breakdown latencia E2E (waterfall diagram)** | §5.3.2 Breakdown Latencia (~L240) | Diagrama waterfall | Hay un comentario explícito en L316: `% Figura: Breakdown latencia (diagrama waterfall)`. Tabla L253 lista segmentos perfectos para waterfall. |
| 3 | **Setup experimental del piloto (diagrama de despliegue)** | §5.2.1 Setup Experimental (~L27) | Diagrama/foto | 60 medidores, complejo de 192 apartamentos, topología Thread + HaLow descrita textualmente. |
| 4 | **Timeline de disponibilidad 90 días** | §5.7 Disponibilidad (~L1019) | Timeline chart | Tabla L1046 lista eventos downtime. Archivo `fig_availability_timeline.tex` con `availability_timeline.png` **existe pero NO está incluido**. |
| 5 | **Gráfico de escalabilidad (latencia/throughput vs nodos)** | §5.4 Escalabilidad (~L378) | Gráfico multi-eje | Extrapolación 60→100 medidores descrita con tablas. Archivo `fig_scalability_chart.tex` con `scalability_chart.png` **existe pero NO está incluido**. |
| 6 | **TCO comparativo (bar chart)** | §5.4.3 TCO (~L489) | Gráfico de barras | Múltiples tablas TCO (L495, L545, L569) serían más impactantes visualmente como gráficos. |

---

## Ch6 — 07Conclusiones_TrabajoFuturo.tex (1377 líneas)

### Figuras Existentes

| # | Archivo/Tipo | Label | Caption (resumen) | Línea |
|---|-------------|-------|-------------------|-------|
| 1 | `\begin{verbatim}` (ASCII art) | `fig:hierarchy-10k` | Arquitectura jerárquica 3 niveles para 10,000 medidores | L542-568 |
| 2 | `\begin{tabular}` (tabla como "Gantt") | `fig:roadmap-gantt` | Roadmap trabajo futuro 2026-2030 (5 líneas de investigación, TRL) | L973-976 |

**Nota:** Ambas "figuras" son representaciones textuales (ASCII art y tabla), no imágenes gráficas reales.

### Figuras Sugeridas

| # | Concepto | Sección / Línea | Tipo Sugerido | Justificación |
|---|----------|-----------------|---------------|---------------|
| 1 | **Arquitectura jerárquica 10K medidores (reemplazar ASCII art)** | §6.4.3 Topología 3 niveles (~L542) | Diagrama de arquitectura | El actual ASCII art en `verbatim` es inadecuado para una tesis de maestría. Debería ser un diagrama TikZ o imagen profesional. |
| 2 | **Gantt chart del roadmap (reemplazar tabla)** | §6.6 Trabajo Futuro (~L973) | Gantt chart real | La tabla actual simula un Gantt con celdas coloreadas. Un Gantt real (pgfgantt o imagen) sería más legible y profesional. |
| 3 | **Impacto ODS / Análisis ambiental** | §6.5 Impacto Social y Ambiental (~L815) | Infografía/diagrama | Sección extensa (~150 líneas) sobre impacto ODS 7/9/13, reducción CO₂, brecha de conectividad — completamente sin visualización. |

---

## Anexo — 05Server_ThingsBoard_Kafka.tex (957 líneas)

### Figuras Existentes

| # | Archivo/Tipo | Label | Caption (resumen) | Línea |
|---|-------------|-------|-------------------|-------|
| 1 | `figures/thingsboard-architecture.png` | `fig:thingsboard-msa-architecture` | Arquitectura oficial microservicios ThingsBoard 3.6.2 en AWS (4 capas) | L408-412 |

### Figuras Sugeridas

| # | Concepto | Sección / Línea | Tipo Sugerido | Justificación |
|---|----------|-----------------|---------------|---------------|
| 1 | **Docker Compose stack (12 containers)** | Despliegue ThingsBoard | Diagrama de contenedores | Se describe un stack Docker de 12 containers con relaciones complejas. |
| 2 | **Flujo Kafka partitions/topics** | §Kafka broker | Diagrama de flujo | Flujo de mensajes Kafka con topics, partitions, consumer groups descrito textualmente. |

---

## Archivos en `figures/`

### Total: 42 archivos

| Archivo | Tipo | Referenciado | Capítulo |
|---------|------|-------------|----------|
| `1-113991254-seeedxiao-esp32c6-45font_1.jpg` | JPG | ✅ | Ch3 |
| `1-113991354_1.jpg` | JPG | ✅ | Ch3 |
| `architecture_comparison.png` | PNG | ✅ | Ch5 (via fig_*) |
| `arquitectura-completa.mmd` | MMD | ❌ ORPHAN | — |
| `arquitectura-completa.png` | PNG | ❌ ORPHAN | — |
| `availability_timeline.png` | PNG | ⚠️ En fig_* NO incluido | — |
| `cost_breakdown.png` | PNG | ✅ | Ch5 (via fig_*) |
| `docker_stack.png` | PNG | ⚠️ fig_docker_stack tiene PENDIENTE | — |
| `esp32c6_block_diagram.png` | PNG | ⚠️ En fig_* NO incluido | — |
| `flujo-datos-edge.mmd` | MMD | ❌ ORPHAN | — |
| `flujo-datos-edge.png` | PNG | ❌ ORPHAN | — |
| `latency_results.png` | PNG | ⚠️ En fig_* NO incluido | — |
| `openthread-osi-model.jpg` | JPG | ✅ | Ch3 |
| `protocol-stack.mmd` | MMD | ❌ ORPHAN | — |
| `protocol-stack.png` | PNG | ❌ ORPHAN | — |
| `protocol_stack.png` | PNG | ✅ | Ch3 (via fig_*) |
| `scalability_chart.png` | PNG | ⚠️ En fig_* NO incluido | — |
| `tb-edge-alarm.png` | PNG | ❌ ORPHAN (SVG usado) | — |
| `tb-edge-alarm.svg` | SVG | ✅ | Ch4 |
| `tb-edge-batch-update.png` | PNG | ❌ ORPHAN (SVG usado) | — |
| `tb-edge-batch-update.svg` | SVG | ✅ | Ch4 |
| `tb-edge-data_filtering.png` | PNG | ❌ ORPHAN | — |
| `tb-edge-data_filtering.svg` | SVG | ❌ ORPHAN | — |
| `tb-edge-edge_overview.png` | PNG | ❌ ORPHAN | — |
| `tb-edge-edge_overview.svg` | SVG | ❌ ORPHAN | — |
| `tb-edge-filtering.png` | PNG | ❌ ORPHAN (SVG usado) | — |
| `tb-edge-filtering.svg` | SVG | ✅ | Ch4 |
| `tb-edge-offline.png` | PNG | ❌ ORPHAN (SVG usado) | — |
| `tb-edge-offline.svg` | SVG | ✅ | Ch4 |
| `tb-edge-offline_network.png` | PNG | ❌ ORPHAN | — |
| `tb-edge-offline_network.svg` | SVG | ❌ ORPHAN | — |
| `tb-edge-overview.png` | PNG | ❌ ORPHAN (SVG usado) | — |
| `tb-edge-overview.svg` | SVG | ✅ | Ch4 |
| `tb-edge-update_dashboard.png` | PNG | ❌ ORPHAN | — |
| `tb-edge-update_dashboard.svg` | SVG | ❌ ORPHAN | — |
| `thingsboard-architecture.png` | PNG | ✅ | Anexo |
| `thingsboard-architecture.svg` | SVG | ❌ ORPHAN | — |
| `thingsboard-edge-architecture-hq.png` | PNG | ✅ | Ch4 |
| `thingsboard-edge-architecture-official.svg` | SVG | ❌ ORPHAN | — |
| `thingsboard-mono-architecture.svg` | SVG | ❌ ORPHAN | — |
| `thingsboard-msa-architecture.svg` | SVG | ❌ ORPHAN | — |
| `thread_topology_roles.png` | PNG | ❌ ORPHAN | — |
| `validation_heatmap.png` | PNG | ✅ | Ch5 (via fig_*) |
| `wisun-protocol-stack.jpg` | JPG | ❌ ORPHAN | — |

### Archivos en `00Figuras/`
| Archivo | Referenciado |
|---------|-------------|
| `00f00EscudoUN2016.jpg` | ✅ Portada (`0000.tex`) |

---

## Figuras PENDIENTES (caption definido pero imagen vacía)

| Label | Archivo fig_* | Incluido en | Problema |
|-------|--------------|-------------|----------|
| `fig:thread-topology-roles` | `fig_thread_topology_roles.tex` | Ch3 (L71) | Caption `[FIGURA PENDIENTE]` — solo texto descriptivo, sin `\includegraphics` ni TikZ. Imagen `thread_topology_roles.png` existe en `figures/` pero no se referencia. |
| `fig:docker-stack` | `fig_docker_stack.tex` | Ch4 (L100) | Caption `[FIGURA PENDIENTE]` — solo texto descriptivo, sin `\includegraphics` ni TikZ. Imagen `docker_stack.png` existe en `figures/` pero no se referencia. |

---

## Archivos fig_* preparados pero NO incluidos en ningún capítulo

| Archivo | Imagen existente | Label | Capítulo lógico sugerido |
|---------|-----------------|-------|-------------------------|
| `fig_availability_timeline.tex` | `availability_timeline.png` ✅ | `fig:availability-timeline` | Ch5 (§5.7 Disponibilidad) |
| `fig_esp32c6_block_diagram.tex` | `esp32c6_block_diagram.png` ✅ | `fig:esp32c6-block-diagram` | Ch3 (§3.3 Hardware ESP32-C6) |
| `fig_latency_results.tex` | `latency_results.png` ✅ | `fig:latency-results` | Ch5 (§5.3 Latencia) |
| `fig_scalability_chart.tex` | `scalability_chart.png` ✅ | `fig:scalability-chart` | Ch5 (§5.4 Escalabilidad) |

---

## Referencias de Figuras Cruzadas (Cross-References)

Todas las `\ref{fig:...}` existentes en los 6 capítulos apuntan a labels definidos:

| Referencia | Archivo que la usa | Label definido en | Estado |
|-----------|-------------------|-------------------|--------|
| `\ref{fig:thingsboard-msa-architecture}` | Ch4 (×5), Anexo (×3) | Anexo L412 | ✅ OK |
| `\ref{fig:edge-architecture-official}` | Ch4 (×2), Anexo (×1) | Ch4 L817 | ✅ OK |
| `\ref{fig:roadmap-gantt}` | Ch6 L971 | Ch6 L976 | ✅ OK |
| `\ref{fig:openthread-osi-model}` | Ch3 L546 | Ch3 L63 | ✅ OK |

**No hay referencias rotas a figuras inexistentes** en los 6 capítulos + anexo actuales.

---

## Imágenes Orphan (existen en `figures/` pero no se referencian)

Total: **25 archivos orphan** (de 42 totales). Categorías:

1. **PNG duplicados de SVGs usados** (6): `tb-edge-alarm.png`, `tb-edge-batch-update.png`, `tb-edge-filtering.png`, `tb-edge-offline.png`, `tb-edge-overview.png`, `thingsboard-architecture.svg` — Los SVGs son los referenciados.

2. **Archivos Mermaid source** (3): `arquitectura-completa.mmd`, `flujo-datos-edge.mmd`, `protocol-stack.mmd` — Archivos fuente, no referenciables directamente en LaTeX.

3. **Imágenes de versiones anteriores de capítulos** (5): `arquitectura-completa.png`, `flujo-datos-edge.png`, `protocol-stack.png` — Usadas en archivos deprecados (`03Arquitectura_REFACTORED.tex`, `02MarcoTeorico.tex`).

4. **Variantes de nombre no usadas** (5): `tb-edge-data_filtering.svg/png`, `tb-edge-edge_overview.svg/png`, `tb-edge-offline_network.svg/png` — Variantes con underscore vs guion.

5. **Imágenes listas para integrar** (3): `thread_topology_roles.png` (fig PENDIENTE), `docker_stack.png` (fig PENDIENTE), `wisun-protocol-stack.jpg` (podría ir en Ch2 sección Wi-SUN).

6. **SVGs de backup** (3): `thingsboard-edge-architecture-official.svg`, `thingsboard-mono-architecture.svg`, `thingsboard-msa-architecture.svg`.

---

## Acciones Prioritarias Recomendadas

### Prioridad ALTA (figuras listas que solo necesitan `\input`)
1. **Completar `fig_thread_topology_roles.tex`** — Agregar `\includegraphics{figures/thread_topology_roles.png}` (imagen existe, solo falta linkear)
2. **Completar `fig_docker_stack.tex`** — Agregar `\includegraphics{figures/docker_stack.png}` (imagen existe, solo falta linkear)
3. **Incluir `fig_latency_results.tex` en Ch5** — Línea ~L204, archivo y PNG listos
4. **Incluir `fig_availability_timeline.tex` en Ch5** — Línea ~L1039, archivo y PNG listos
5. **Incluir `fig_scalability_chart.tex` en Ch5** — Línea ~L383, archivo y PNG listos
6. **Incluir `fig_esp32c6_block_diagram.tex` en Ch3** — Línea ~L477, archivo y PNG listos

### Prioridad MEDIA (figuras nuevas que mejorarían capítulos sin figuras)
7. **Ch1: Diagrama arquitectura overview** — Reusar `arquitectura-completa.png` existente
8. **Ch2: Stack de protocolos** — Reusar `fig_protocol_stack.tex` o `wisun-protocol-stack.jpg`
9. **Ch5: Breakdown latencia waterfall** — Comentario en L316 ya indica la intención
10. **Ch6: Reemplazar ASCII art** (fig:hierarchy-10k) con diagrama TikZ profesional
11. **Ch6: Reemplazar tabla-Gantt** (fig:roadmap-gantt) con diagrama Gantt real

### Prioridad BAJA (figuras nuevas opcionales)
12-25. Nuevos diagramas sugeridos en las tablas de cada capítulo arriba.
