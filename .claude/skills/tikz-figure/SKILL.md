---
name: tikz-figure
description: Build IEEE-compliant, accessible TikZ figures with the patterns refined in this repo (figures/ subfolder + standalone wrapper, saturated palette, no \tiny, single-source-of-truth body). Use when creating or fixing any TikZ diagram for paper or thesis.
---

# tikz-figure — Figuras TikZ IEEE-compliant y accesibles

Skill para autorar/refactorizar figuras TikZ siguiendo los patrones consolidados
en este repo (paper IEEE + tesis UNAL). Prioriza legibilidad a tamaño impreso,
contraste suficiente, una sola fuente de verdad y generación reproducible de
PDF/SVG/PNG.

---

## 1. Cuándo usar esta skill

Activar cuando el usuario:

- Pide diseñar, generar o "mejorar" un diagrama TikZ (arquitectura, topología,
  pila de protocolos, flujo de datos, hardware de gateway, etc.).
- Reporta problemas de legibilidad: "el texto se ve borroso", "las etiquetas
  están truncadas", "no se lee al imprimir", "labels solapan cajas".
- Pide auditoría de accesibilidad / IEEE compliance (tamaños mínimos de fuente,
  contraste color, reglas de figuras IEEE).
- Comparte un `.tex` con `\tiny`, `\scriptsize`, `fill=...!4`, doble
  `\resizebox`, o flechas que cruzan cajas.
- Solicita regenerar SVG/PNG de una figura existente.

No usar para: edición de PDFs ya compilados, figuras matplotlib/Python, o
diagramas drawio (existe skill aparte `drawio-diagrams`).

---

## 2. Estructura de archivos (single source of truth)

Para cada figura `<name>`:

```
paper_ieee/
  figures/
    <name>.tex          <-- SOLO el cuerpo \begin{tikzpicture}...\end{tikzpicture}
                            sin \documentclass, sin \begin{figure}, sin \caption
  <name>.tex            <-- WRAPPER standalone: \documentclass{standalone}
                            + \input{figures/<name>.tex}
  main.tex              <-- usa \input{figures/<name>.tex} dentro de \begin{figure*}
```

Beneficio: una sola edición actualiza paper + SVG + PNG. El wrapper se compila
solo para regenerar imágenes (Overleaf no necesita compilar TikZ pesado).

Para la tesis (no IEEE), aceptable mantener el archivo monolítico
`fig_<name>.tex` con `\begin{figure}[H] ... \end{figure}` y `\resizebox`.

---

## 3. Plantilla standalone wrapper

```latex
% Standalone wrapper para regenerar PDF/SVG/PNG.
% Cuerpo TikZ vive en figures/<name>.tex (compartido con main.tex).
\documentclass[border=12pt,tikz]{standalone}
\usepackage[T1]{fontenc}
\usepackage{amssymb}
\usepackage{tikz}
\usetikzlibrary{calc,arrows.meta,positioning,fit,backgrounds,decorations.pathmorphing}
\begin{document}
\input{figures/<name>.tex}
\end{document}
```

`border=12pt` deja margen suficiente para que el SVG no recorte etiquetas en
los bordes. Si la figura tiene labels muy cercanos al borde, usar `border=20pt`.

---

## 4. Plantilla TikZ body inicial (4 zonas, IEEE figure*)

Patrón base que ha funcionado bien (ancho 17.6 cm = `\textwidth` IEEE en
doble columna, sin `\resizebox`):

```latex
% TikZ body only -- IEEE-compliant.
% Source: 17.6 cm wide x 7.5 cm tall -> renders at IEEE figure* width 1:1
% so all \normalsize (10pt) text matches body-text size.
\begin{tikzpicture}[
  font=\sffamily\normalsize,
  % -- Zonas (bandas de color por subsistema) --
  Z1/.style={rounded corners=4pt, draw=green!50!black,  fill=green!4,  line width=0.9pt},
  Z2/.style={rounded corners=4pt, draw=blue!55!black,   fill=blue!3,   line width=0.9pt},
  Z3/.style={rounded corners=4pt, draw=orange!60!black, fill=orange!4, line width=0.9pt},
  Z4/.style={rounded corners=4pt, draw=purple!55!black, fill=purple!3, line width=0.9pt},
  zlab/.style={font=\sffamily\bfseries\normalsize, inner sep=2pt},
  % -- Componentes (cajas internas) --
  comp/.style={rounded corners=2pt, draw, line width=0.6pt, inner sep=3pt,
               minimum height=0.7cm, align=center,
               font=\sffamily\normalsize, fill=white},
  nodebox/.style={comp, draw=green!55!black,  fill=green!14, minimum width=1.8cm, text=green!25!black},
  tbedge/.style ={comp, draw=blue!50!black,   fill=blue!12,  minimum width=1.7cm, text=blue!30!black, font=\sffamily\normalsize\bfseries},
  server/.style ={comp, draw=purple!55!black, fill=purple!12,minimum width=1.8cm, text=purple!30!black},
  % -- Docker / container (caja dashed) --
  dockbox/.style={rounded corners=3pt, draw=blue!35!black, fill=blue!5, line width=0.5pt, dashed},
  % -- Flechas --
  arr/.style    ={-{Latex[length=2mm,width=1.4mm]}, line width=0.7pt},
  arrthick/.style={-{Latex[length=2.6mm,width=1.8mm]}, line width=1.1pt},
  arrbidir/.style={{Latex[length=2.6mm,width=1.8mm]}-{Latex[length=2.6mm,width=1.8mm]}, line width=1pt},
  % -- Mesh wireless (zigzag) --
  meshlink/.style={green!55!black, line width=0.7pt, decorate,
                   decoration={zigzag, amplitude=0.7pt, segment length=4pt}},
  % -- Label sobre flecha que cruza zonas (FILL=WHITE obligatorio) --
  xzlabel/.style={font=\sffamily\normalsize\bfseries, fill=white, inner sep=1.5pt, rounded corners=1pt},
]

% Ejemplo zona
\node[Z1, minimum width=4.4cm, minimum height=7.5cm,
      anchor=south west] (Z1F) at (0,0) {};
\node[zlab, green!35!black, anchor=north west]
  at (Z1F.north west) {Field network};

% Ejemplo cross-zone arrow con label
\draw[arrthick, green!55!black] (NS.east) -- (OTBR.west);
\node[xzlabel, text=green!30!black] at (4.5, 6.85) {Thread};
\end{tikzpicture}
```

**Regla clave:** color de texto SIEMPRE va como `text=...` en la opción
nombrada, NUNCA como segunda opción posicional (eso sobrescribe `fill=white`).

---

## 5. Paleta de colores y reglas de contraste

### Convención por subsistema (tesis + paper)

| Zona / rol           | Color base    | Uso                                    |
|----------------------|---------------|----------------------------------------|
| Red de campo / mesh  | `green`       | Nodos Zephyr, OTBR, Thread, 802.15.4   |
| Edge gateway / TB    | `blue`        | RPi4, TB Edge, Docker, OpenWrt         |
| Backhaul wireless    | `orange`      | HaLow, 802.11ah, antenas, RF           |
| On-prem / servidor   | `purple`      | TB server, PostgreSQL, base de datos   |
| Medidores físicos    | `brown`       | Emsitech P2000-T, DLMS/COSEM           |
| Visualización        | `teal`        | Grafana, paneles                       |
| WAN / nube           | `red` / `gray`| ThingsBoard Cloud, internet, paneles operador |

### Rellenos y contraste (REGLA DURA)

- **Banda de zona (fondo amplio):** `fill=<color>!3` a `!8` (sutil, sí impreso).
- **Caja de componente:** `fill=<color>!12` a `!22` MÍNIMO. Por debajo de
  `!10` se lava al imprimir en escala de grises.
- **NUNCA usar `fill=<color>!4` o menor para cajas de contenido** — solo para
  bandas grandes que sirven de fondo.
- **Texto:** `text=<color>!25!black` a `!40!black`. `gray!50` o más claro NO
  pasa el contraste 4.5:1 contra fondos `!12`.
- **Borde de zona:** `draw=<color>!50!black` o `!55!black` (suficiente
  contraste con el fondo claro).
- **Label sobre flecha (xzlabel):** SIEMPRE `fill=white, inner sep=1.5pt,
  rounded corners=1pt` para que sea legible sobre cualquier fondo.

---

## 6. Tipografía IEEE-compliant

### Regla maestra

`tamaño_renderizado_pt = tamaño_fuente_pt × factor_scale`

donde `factor_scale = ancho_destino / ancho_fuente_tikz`.

IEEE pide texto en figuras **igual o mayor** al cuerpo (10pt en `\twocolumn`).
Por debajo de 8pt rendered se considera ilegible.

### Combinaciones recomendadas

| Destino IEEE       | Ancho destino | Ancho fuente TikZ | Scale | Fuente fuente requerida |
|--------------------|---------------|-------------------|-------|-------------------------|
| `figure*` 2-col    | 17.6 cm       | 17.6 cm           | 1.0   | `\normalsize` (10pt)    |
| `figure*` 2-col    | 17.6 cm       | 22 cm             | 0.80  | `\small` o `\normalsize` solo si quieres ~8pt |
| `figure` 1-col     | 8.6 cm        | 8.6 cm            | 1.0   | `\normalsize`           |
| `figure` 1-col     | 8.6 cm        | 17 cm             | 0.51  | `\Large` (12pt) → render ~6.1pt FAIL |

**Preferir 1:1 sin `\resizebox`.** Si necesitas resizebox, calcula la fuente
fuente para que el render quede ≥ 9pt.

### Jerarquía dentro de la figura

- `\Large` (12pt): título de zona, label de subsistema mayor.
- `\normalsize` (10pt): contenido por defecto de cajas, labels de flechas
  importantes (cross-zone).
- `\small` (9pt): aceptable para anotaciones secundarias en zonas densas.
- `\footnotesize` (8pt): SOLO para anotaciones marginales (subtítulo de zona
  tipo "Raspberry Pi 4 + OpenWrt 23.05").
- `\scriptsize` / `\tiny`: **PROHIBIDOS** en figura final IEEE.

---

## 7. Patrones recurrentes

### 7.1 Cross-zone label sobre flecha

```latex
\draw[arrthick, green!55!black] (NS.east) -- (OTBR.west);
\node[xzlabel, text=green!30!black] at (4.5, 6.85) {Thread};
```

El `xzlabel` con `fill=white` evita colisión visual con el shaft de la flecha.

### 7.2 Antena + ondas RF

```latex
\node[font=\sffamily\normalsize, orange!55!black] at (11.2, 6.2) {$\blacktriangle$};
\foreach \r/\op in {0.13/0.9, 0.24/0.6} {
  \draw[orange!60!black, line width=0.5pt, opacity=\op]
    (11.35, 6.25) ++(170:\r) arc[start angle=170, end angle=190, radius=\r];
}
```

### 7.3 Pila de protocolos (chips apilados)

```latex
protoG/.style={rounded corners=2pt, draw=green!55!black, fill=green!12,
               font=\sffamily\normalsize, inner sep=2pt, minimum height=0.5cm,
               text=green!28!black, minimum width=2.6cm, align=center},
% ...
\node[protoG] at (2.2, 3.05) {LwM2M / DTLS};
\node[protoG] at (2.2, 2.45) {CoAP / UDP};
\node[protoG] at (2.2, 1.85) {6LoWPAN / IPHC};
\node[protoG] at (2.2, 1.25) {IEEE 802.15.4};
```

### 7.4 Docker / containerización (caja dashed con label off-corner)

```latex
\node[dockbox, minimum width=7.8cm, minimum height=5cm, anchor=south] (DOCK) at (...) {};
\node[font=\sffamily\scriptsize\bfseries\itshape, fill=white, inner sep=2pt,
      anchor=west, text=blue!50!black]
  at ($(DOCK.north west)+(0.2,0)$) {Docker};
```

El `fill=white` en el label evita que la línea dashed se vea cruzando el texto.

### 7.5 Mesh wireless multi-hop (zigzag)

```latex
meshlink/.style={green!55!black, line width=0.7pt, decorate,
                 decoration={zigzag, amplitude=0.7pt, segment length=4pt}},
% ...
\draw[meshlink] (N1.east) -- (N2.west);
\draw[meshlink] ([yshift=-3pt]N1.east) -- ([yshift=-3pt]N2.west);  % doble línea = bidir
```

### 7.6 Banda de zona con label en esquina

```latex
\node[Z1, minimum width=4.4cm, minimum height=7.5cm,
      anchor=south west] (Z1F) at (0,0) {};
\node[zlab, green!35!black, anchor=north west]
  at (Z1F.north west) {Field network};
```

---

## 8. Anti-patrones a evitar

### 8.1 Doble `\resizebox` (figura DENTRO de algo ya escalado)

```latex
% MAL:
\begin{figure*}
\resizebox{\textwidth}{!}{ \input{fig.tex} }   % fig.tex YA tiene resizebox
\end{figure*}
```

Síntoma: texto borroso, ~5pt rendered. Solución: el body NO debe contener
`\resizebox`. El wrapper standalone tampoco.

### 8.2 `\tiny` o `\scriptsize` "para que quepa"

```latex
% MAL:
\node[font=\tiny] {LwM2M\\transport};
```

A IEEE `figure*` con scale 1.0 esto rinde ~5pt → ilegible. Solución: acortar
el texto (`"ThingsBoard Edge"` → `"TB Edge"`) o agrandar la caja.

### 8.3 Rellenos `!3` / `!4` en cajas con texto

```latex
% MAL: imprimir en B/N convierte fill=blue!4 en blanco puro
fill=blue!4, text=blue!30!black
```

Solución: bandas grandes pueden ser `!3-!8`; cajas con contenido `!12-!22`.

### 8.4 `gray!50` o texto claro sobre fondo claro

Fallo de contraste WCAG. Mínimo `text=<color>!25!black` o `black!80`.

### 8.5 Color de texto en segunda opción posicional (PISA `fill=white`)

```latex
% MAL: green!30!black se interpreta como color de relleno, no de texto
\node[xzlabel, green!30!black] at (...) {Thread};
```

```latex
% BIEN: text=... como opción nombrada
\node[xzlabel, text=green!30!black] at (...) {Thread};
```

### 8.6 Y-centers desalineados → flechas diagonales feas

```latex
% MAL:
\node[boxA] at (1, 6.2) {A};
\node[boxB] at (5, 6.0) {B};   % 0.2cm de diferencia → arrow diagonal
\draw[arr] (A.east) -- (B.west);
```

Solución: alinear y-coords. Si las cajas tienen alturas distintas, usar
`(A.east -| B.west)` o `([yshift=...]A.east)`.

### 8.7 Labels que no caben entre cajas

Si la distancia entre `boxA.east` y `boxB.west` es 1.0 cm y tu label dice
"LwM2M → MQTT translation", no cabe. Solución: poner label DEBAJO/ENCIMA de
la flecha con `node[font=\sffamily\normalsize\bfseries\itshape] at (midpoint, y-offset)`,
o acortar el texto.

### 8.8 `node distance` global + posiciones absolutas mezcladas

Confuso y frágil. Preferir todo absoluto (`at (x,y)`) o todo relativo. Para
figuras nuevas: absoluto.

---

## 9. Workflow de generación PDF / SVG / PNG

Desde la carpeta del wrapper (p.ej. `paper_ieee/`):

```bash
# 1) Compilar PDF vectorial
pdflatex -interaction=nonstopmode fig_<name>.tex

# 2) Generar SVG vectorial (preferir pdftocairo sobre dvisvgm)
pdftocairo -svg fig_<name>.pdf fig_<name>.svg

# 3) Generar PNG raster para previews / Overleaf gallery
pdftoppm -png -r 200 fig_<name>.pdf fig_<name>
# (produce fig_<name>-1.png si hay 1 página)
```

### Si dvisvgm falla con "transparency-zero bug"

Síntoma: SVG sale con elementos invisibles o `opacity=0` espurio.

Solución: usar `pdftocairo -svg` (no requiere `--enable-dvisvgm` en pdflatex)
o, si necesitas dvisvgm:

```bash
pdflatex -shell-escape -interaction=nonstopmode fig_<name>.tex
dvisvgm --pdf --no-fonts --exact fig_<name>.pdf
```

El flag `--no-fonts` convierte texto a paths (más seguro para web) y
`--exact` calcula bounding boxes precisas.

### Regenerar todas las figuras del paper

```bash
for f in fig_architecture_distributed fig_edge_gateway fig_protocol fig_esp32c6; do
  pdflatex -interaction=nonstopmode $f.tex && \
  pdftocairo -svg $f.pdf $f.svg && \
  pdftoppm -png -r 200 $f.pdf $f
done
```

---

## 10. Checklist de revisión antes de entregar

Antes de declarar una figura "lista":

- [ ] El body en `figures/<name>.tex` NO contiene `\begin{figure}`,
      `\resizebox`, `\caption` ni `\label` (eso va en `main.tex`).
- [ ] El wrapper standalone compila sin errores ni warnings de overfull.
- [ ] A 100% de zoom en el PDF compilado, TODO el texto es legible (≥ 8pt
      rendered; ≥ 10pt para IEEE figure*).
- [ ] Ningún `\tiny` ni `\scriptsize` en el body (Grep el archivo).
- [ ] Ningún `fill=<color>!4` o menor en cajas con texto interno (solo en
      bandas grandes de fondo).
- [ ] Todo `text=...` está como opción NOMBRADA (no como segunda opción
      posicional que pisaría `fill=white`).
- [ ] Flechas cross-zone tienen label `xzlabel` con `fill=white`.
- [ ] Cajas en la misma fila comparten coordenada-y (sin diagonales feas).
- [ ] Labels caben en su espacio (no se solapan con cajas adyacentes).
- [ ] Si imprimes la página en escala de grises, las zonas y cajas siguen
      siendo distinguibles (test de daltonismo / B/N).
- [ ] Generaste SVG con `pdftocairo` y PNG con `pdftoppm -r 200`.
- [ ] El `.svg` se ve idéntico al `.pdf` en Firefox / Chrome (sin elementos
      faltantes por bug de transparencia).
- [ ] El `\caption` describe la figura para alguien que no leyó el contexto
      (≥ 2 frases, menciona los componentes principales y los flujos).

---

## Referencias en el repo

Buenos ejemplos para copiar como punto de partida:

- `paper_ieee/figures/fig_architecture_distributed.tex` — 4 zonas, cross-zone
  arrows con xzlabel, antenas + ondas RF, pila de protocolos.
- `paper_ieee/figures/fig_edge_gateway.tex` — Docker dashed box, sub-bloques
  dentro de contenedor, flechas IN/OUT verticales con `iolabel`.
- `Tesis___Trabajo_final___Maestria___2025/fig_arquitectura_tres_niveles.tex`
  — bandas horizontales por nivel, grupos funcionales con `funcgroup`, cajas
  de métricas laterales.
- `Tesis___Trabajo_final___Maestria___2025/fig_topologia_armario.tex` —
  cabinet outline, pared de concreto, snake decoration para HaLow.
