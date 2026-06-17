# Plan de Reorganización — Capítulo 1 (Introducción)

## 1. Resumen ejecutivo

El Cap. 1 actual ejecuta un embudo **tecnología → problema** (ratio 2.4:1 tecnología-vs-problema, ~114/446 líneas comparativas) que invierte la lógica esperada por el jurado. Esta propuesta reorganiza el capítulo bajo un eje **problema → requerimientos → tecnología**, traslada ~110 líneas de selección/justificación tecnológica al Marco Teórico y a la Metodología, replica el patrón estructural de §1.2 (segmentación → tabla → selección → razones → rechazo → frontera) en el resto del capítulo, y corrige la legibilidad de dos figuras TikZ flagged. Atiende los 6 concerns del jurado de forma trazable.

---

## 2. Estructura ACTUAL de Cap. 1

Archivo: `paper_ieee/chapters/01_introduccion.tex` (446 líneas)

| Líneas | Sección actual | Etiqueta semántica | Observación |
|--------|----------------|---------------------|-------------|
| 8–24 | §1.1 Contexto y Motivación (apertura) | ANTECEDENTE/DIAGNÓSTICO | OK: crecimiento sector, 15 PB telemetría, limitaciones cloud |
| 26–91 | §1.2.1 Comparativa mesh (intro+tabla+selección Thread) | SELECTION_JUSTIFICATION | Mover a Marco §2.2.2 |
| 92–96 | Puente narrativo (mesh→troncal) | TRANSICIÓN | Mantener (versión corta) |
| 97–137 | §1.2.2 Comparativa troncal (intro+tabla+selección HaLow) | SELECTION_JUSTIFICATION | Mover a Marco §2.2.1/§2.2.1.2 |
| 140–152 | Brechas arquitectónicas (4 gaps) | DIAGNÓSTICO | Mover a Marco §2.3.1/§2.3.2 |
| 154–172 | Escenario del armario concentrador | CONTEXTO/CARACTERIZACIÓN | **Adelantar** al inicio del capítulo |
| 159–165 | Mapeo restricciones→tecnología (embedded) | SELECTION_JUSTIFICATION | Extraer y mover a Metodología |
| 175–206 | §1.4 Planteamiento del Problema (3 problemas) | PROBLEM_STATEMENT | Reescribir problem-first |
| 198–202 | Problema 3 con comparativas embebidas | SELECTION_JUSTIFICATION | Eliminar (duplica §1.2.2) |
| 209–239 | §1.3 Pregunta de investigación + objetivos | OBJETIVOS | Reescribir aplicando patrón §1.2 |
| 242–290 | §1.5 Justificación | JUSTIFICACIÓN | Reescribir aplicando patrón §1.2 |
| 248–256 | Rechazo de 5G/NR-Light/NB-IoT/PLC/LoRaWAN | SELECTION_JUSTIFICATION | Mover a Marco §2.3 |
| 293–323 | §1.6 Hipótesis (OV1–OV7) | HIPÓTESIS | Reescribir aplicando patrón §1.2 |
| 326–390 | §1.7 Alcances y Limitaciones | ALCANCE | Reescribir aplicando patrón §1.2 |
| 393–446 | §1.8 Contribuciones y estructura del documento | METADATA | Mantener |

**Diagnóstico estructural:** el capítulo oscila 3 veces entre nivel macro (motivación/brechas) y micro (kbps/dB/tablas), repite 3 veces la justificación de HaLow, y entierra la caracterización del armario después de 120 líneas de comparativas.

---

## 3. Estructura PROPUESTA de Cap. 1

Eje narrativo único: **Contexto sectorial → Caso de estudio (armario) → Requerimientos cuantificados → Brechas del estado actual → Problema formal → Pregunta/Objetivos → Justificación → Hipótesis → Alcances → Contribuciones**.

| # | Nueva sección | Contenido | Origen (líneas actuales) | Concerns atendidos |
|---|---------------|-----------|--------------------------|--------------------|
| §1.1 | Contexto y motivación sectorial | Crecimiento medidores, telemetría, limitaciones cloud. **Sin** comparativas tecnológicas. | 13–24 | C1, C2, C4 |
| §1.2 | Caso de estudio: el armario concentrador | Caracterización física (10–60 medidores, <80 m, 5V/200mA, hormigón armado). Dimensiones del problema: densidad, latencia, disponibilidad, energía, costo. | 154–172 (adelantado) | C1, C4 |
| §1.3 | Requerimientos, contexto y restricciones | **NUEVA**. Lista consolidada "El sistema DEBE…" con umbrales cuantificados (P95 <100 ms, disp. >99 %, 60–100 nodos, banda libre, stakeholders, regulación LatAm por país). | Consolidación de 18–21, 195, 229, 277, 318–323, 133 | C1, C5 |
| §1.4 | Brechas del estado actual | 4 brechas arquitectónicas reformuladas como "qué falta para cumplir §1.3", sin selección tecnológica. | 140–152 (reescrito) | C2, C4 |
| §1.5 | Planteamiento del problema | 1–2 problemas formales derivados de §1.3 vs §1.4. Sin comparativas tecnológicas embebidas. | 175–206 (reescrito; elimina 198–202) | C1, C2 |
| §1.6 | Pregunta de investigación y objetivos | Aplicar patrón §1.2: segmentación (general/específicos) → criterio de éxito → restricción → frontera. | 209–239 (reescrito) | C5 |
| §1.7 | Justificación | Aplicar patrón §1.2: dimensión (técnica/económica/académica) → evidencia → frontera honesta. **Sin** rechazo de 5G/NB-IoT/etc. | 242–290 menos 248–256 | C2, C3, C5 |
| §1.8 | Hipótesis | Aplicar patrón §1.2 a OV1–OV7: dimensión → requerimiento → umbral → frontera. | 293–323 (reescrito) | C5 |
| §1.9 | Alcances y limitaciones | Aplicar patrón §1.2: qué se incluye / qué se excluye / cuándo el excluido sería relevante. | 326–390 (reescrito) | C5 |
| §1.10 | Contribuciones y estructura del documento | Mantener. | 393–446 | — |

**Contenido movido fuera de Cap. 1 (~110 líneas):**

| Origen Cap. 1 (líneas) | Contenido | Destino |
|------------------------|-----------|---------|
| 40–91 | Tabla mesh + razones Thread + rechazo sub-GHz/Wi-SUN/LoRa Mesh/DigiMesh | Marco §2.2.2 (Redes Mesh) |
| 97–137 | Tabla troncal + selección HaLow + rechazo 4 alternativas | Marco §2.2.1 / nueva §2.2.1.2 |
| 159–165 | Mapeo restricciones armario → Thread+HaLow | Metodología (criterios de decisión) |
| 198–202 | Comparativas LoRaWAN/LTE Cat-M1/Wi-Fi dentro de Problema 3 | Eliminar (ya cubierto en Marco) |
| 248–256 | Rechazo 5G/NR-Light/NB-IoT/PLC/LoRaWAN | Marco §2.3 (Estado del arte) |

**Resultado esperado:** Cap. 1 pasa de ~446 a ~330–350 líneas, con ratio problema-vs-tecnología invertido (~3:1 problema-vs-tecnología), y zero comparativas cuantitativas embebidas.

---

## 4. Tabla de mapeo: ACTUAL → PROPUESTA

| Sección actual | Líneas | Acción | Destino | Racional | Concern |
|----------------|--------|--------|---------|----------|---------|
| §1.1 apertura (13–24) | 13–24 | STAY (minor edits) | §1.1 nueva | Antecedentes sectoriales sin comparativas; mover anticipación "Thread vs Zigbee" línea 11 | C4 |
| §1.2.1 mesh (26–91) | 26–91 | MOVE | Marco §2.2.2 | Es selección, no presentación. Liberar 52 líneas de Cap. 1 | C1, C2, C3 |
| Puente mesh→troncal (92–96) | 92–96 | DROP | — | Innecesario tras mover §1.2.1/§1.2.2 | C2 |
| §1.2.2 troncal (97–137) | 97–137 | MOVE | Marco §2.2.1 / nueva §2.2.1.2 | Selección HaLow; mover 41 líneas | C1, C2, C3 |
| Brechas arquitectónicas (140–152) | 140–152 | REWRITE | §1.4 nueva | Reformular como "qué falta para cumplir requerimientos" sin elegir tech | C2, C4 |
| Escenario armario (154–172) | 154–172 | MOVE UP + SPLIT | §1.2 nueva (físico) + §1.3 nueva (requerimientos) | Adelantar a posición prominente; separar caracterización de mapeo a tech | C1, C4 |
| Mapeo restricciones→tech (159–165) | 159–165 | MOVE | Metodología | Es criterio de decisión, no contexto | C3 |
| §1.4 Problema 1, 2 (175–197, 203–206) | 175–197, 203–206 | REWRITE | §1.5 nueva | Reformular problem-first, derivar de §1.3 | C1, C2 |
| §1.4 Problema 3 (198–202) | 198–202 | DROP | — | Duplica comparativas de §1.2.2 ya movidas | C2 |
| §1.3 Pregunta + objetivos (209–239) | 209–239 | REWRITE | §1.6 nueva | Aplicar patrón §1.2 (segmentación + criterio + frontera) | C5 |
| §1.5 Justificación cuerpo (242–247, 257–290) | 242–247, 257–290 | REWRITE | §1.7 nueva | Aplicar patrón §1.2; separar dimensiones (técnica/econ/acad) | C5 |
| §1.5 Rechazo alternativas (248–256) | 248–256 | MOVE | Marco §2.3 | Selección tecnológica, no justificación | C2, C3 |
| §1.6 Hipótesis (293–323) | 293–323 | REWRITE | §1.8 nueva | Aplicar patrón §1.2: dimensión→umbral→frontera por OV | C5 |
| §1.7 Alcances/Limitaciones (326–390) | 326–390 | REWRITE | §1.9 nueva | Aplicar patrón §1.2: incluye/excluye/cuándo-excluido-relevante | C5 |
| §1.8 Contribuciones (393–446) | 393–446 | STAY | §1.10 nueva | OK como está | — |
| `fig:topologia-armario` | figura | REWRITE TikZ | mismo path | Fuentes \tiny→\footnotesize; rellenos !15→!25; texto gray!50→black!80 | C6 |
| `fig:arquitectura-tres-niveles` | figura | REWRITE TikZ + posible SPLIT | mismo path | Fuentes \tiny/\scriptsize→\footnotesize/\small; rellenos !3–!4→!20–!25; evaluar split en 2 sub-figs | C6 |
| `tab:lastmile-comparison` | tabla | VERIFY | Marco (tras mover) | Sustituir `\resizebox{\textwidth}{!}` por `\footnotesize` con `tabularx` | C6 |
| `tab:mesh-comparison` | tabla | STAY | Marco (tras mover) | OK | — |

---

## 5. Cronograma sugerido (orden de ejecución)

**Fase A — Reorganización estructural (resuelve C1, C2, C3, C4):**
1. Crear backup `01_introduccion.tex.backup` antes de editar.
2. Extraer §1.2.1 (líneas 26–91) → mover a Marco §2.2.2 con referencia cruzada.
3. Extraer §1.2.2 (líneas 97–137) → mover a Marco §2.2.1/§2.2.1.2.
4. Extraer rechazo 5G/NB-IoT (líneas 248–256) → mover a Marco §2.3.
5. Eliminar Problema 3 (líneas 198–202) y puente (92–96).
6. Adelantar escenario armario (154–172) al inicio como §1.2 nueva; separar 159–165 a Metodología.
7. Redactar §1.3 nueva "Requerimientos, contexto y restricciones" consolidando umbrales dispersos.
8. Reescribir brechas (140–152) como §1.4 nueva (sin tech).
9. Reescribir Planteamiento (175–206) como §1.5 nueva derivada de §1.3 + §1.4.

**Fase B — Aplicación del patrón §1.2 (resuelve C5):**
10. Reescribir §1.6 Pregunta/Objetivos con patrón §1.2.
11. Reescribir §1.7 Justificación con patrón §1.2.
12. Reescribir §1.8 Hipótesis (OV1–OV7) con patrón §1.2.
13. Reescribir §1.9 Alcances/Limitaciones con patrón §1.2.

**Fase C — Figuras (resuelve C6, paralelizable con A/B):**
14. Reescribir `fig_topologia_armario.tex` con tipografía y contraste accesible.
15. Reescribir `fig_arquitectura_tres_niveles.tex` (posible split en 2 sub-figs).
16. Verificar `tab:lastmile-comparison` post-mudanza.

**Fase D — Verificación:**
17. Compilar `0000.tex` y `0000_overleaf.tex`; resolver referencias rotas.
18. Verificar zero `undefined references` y zero comparativas duplicadas.
19. Diff visual contra versión previa; actualizar `REORG_TESIS_TRACKING.md`.

**Dependencias:** Fase B depende de Fase A (necesita el nuevo eje narrativo). Fase C es independiente. Fase D cierra todo.

---

## 6. Auditoría de figuras

| Figura/Tabla | Veredicto | Síntomas | Fix recomendado |
|--------------|-----------|----------|-----------------|
| `fig:topologia-armario` | BAD | `\tiny` en 8+ instancias; rellenos `yellow!15`, `teal!15`, `orange!15`, `red!10`, `gray!5`; texto `gray!50` sobre fondo gris; doble compresión `\resizebox{0.97\textwidth}{!}` | Subir fuentes a `\footnotesize`/`\small`; saturar rellenos a `!20–!30`; oscurecer texto a `black!80`; eliminar `\resizebox` y rediseñar layout |
| `fig:arquitectura-tres-niveles` | BAD | `\tiny` y `\scriptsize` en cajas métricas; rellenos `blue!3`, `blue!4`, `orange!4`, `green!4` (apenas visibles); `text width=2.6cm` fuerza envoltorio | Subir fuentes a `\footnotesize`/`\small`; saturar rellenos a `!20–!25`; ampliar `text width`; evaluar dividir en `fig:arq-3niv-overview` + `fig:arq-3niv-metricas` |
| `tab:lastmile-comparison` | SUSPECT | `\resizebox{\textwidth}{!}` puede comprimir texto a <7pt en impresión | Reemplazar por `\footnotesize` + `tabularx`/`tabular*` con anchos explícitos |
| `tab:mesh-comparison` | OK | `\footnotesize`, header `gray!20` aceptable | Mantener |

**Estándar objetivo:** fuente mínima 8 pt en figuras (equivalente a `\footnotesize` en `\normalsize` 10pt); contraste WCAG AA (≥4.5:1); paleta robusta a impresión B/N (usar también patrones o etiquetas, no solo color).

---

## 7. Registro de decisiones

*(Vacío — se llena durante la ejecución. Cada decisión incluye: fecha, contexto, opciones evaluadas, decisión, justificación.)*

| Fecha | Decisión | Contexto | Alternativas evaluadas | Justificación |
|-------|----------|----------|------------------------|---------------|
| — | — | — | — | — |
