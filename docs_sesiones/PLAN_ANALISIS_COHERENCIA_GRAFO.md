# Plan: Análisis de coherencia usando Obsidian (grafo)

Objetivo: usar el grafo para detectar
- capítulos desconectados
- claims sin evidencia
- conceptos sin definición
- resultados sin métrica

## 1) Canon: qué archivos son “activos”

Activos (según `0000.tex`):
- `01Introduccion.tex`
- `02MarcoTeorico_Rework.tex`
- `03NodoIoT_Thread_LwM2M.tex`
- `04Gateway_HaLow_Edge.tex`
- `05Server_ThingsBoard_Kafka.tex`
- `06Resultados_Validacion.tex`
- `07Conclusiones_TrabajoFuturo.tex`

## 2) Ontología mínima de notas

- Capítulo (nota en `capitulos/`):
  - Claims del capítulo
  - Conceptos clave (links a `conceptos/`)
  - Evidencia/Fuentes (links a `fuentes/`)
  - Experimentos/Métricas (links a sección/nota)

- Concepto (nota en `conceptos/`):
  - definición
  - por qué importa
  - dónde aparece (links)
  - métricas asociadas

- Fuente (nota en `fuentes/`):
  - cita bibtex key
  - qué aporta
  - qué claim soporta

## 3) Checks (lo que vamos a evaluar)

- Cada capítulo debe enlazar a:
  - al menos 5–10 conceptos
  - al menos X fuentes
  - al menos 1–3 métricas/experimentos

- Para escalabilidad:
  - escenario S0/S1/S2/S3 (ver `conceptos/Escalabilidad_modelo.md`)
  - dimensionamiento gateway (N/G)

## 4) Output

- Canvas actualizado: clusters por nivel (N1..N4) + Resultados
- Lista de “huecos”: TODOs en evidencia, figuras, migraciones
