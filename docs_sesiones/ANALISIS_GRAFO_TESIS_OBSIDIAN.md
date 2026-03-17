# Análisis inicial (coherencia + grafo) — Tesis ↔ Obsidian

Fecha: 2026-02-15

Este análisis usa heurísticas rápidas sobre el repo LaTeX (`Tesis___Trabajo_final___Maestria___2025/`) para identificar:
- **estructura real** (qué se compila)
- **duplicidades/deprecados** (capítulos paralelos)
- **densidad de soporte bibliográfico** (citas)
- **huecos de contenido** (TODO/MIGRAR)

---

## 1) Estructura “source of truth” (lo que realmente entra al PDF)

En `0000.tex` se están incluyendo estos capítulos (orden):

1. `01Introduccion.tex`
2. `02MarcoTeorico_Rework.tex`
3. `03NodoIoT_Thread_LwM2M.tex`
4. `04Gateway_HaLow_Edge.tex`
5. `05Server_ThingsBoard_Kafka.tex`
6. `06Resultados_Validacion.tex`
7. `07Conclusiones_TrabajoFuturo.tex`

Y anexos:
- `09AnexoA_InstalacionOpenWRT.tex`
- `10AnexoB_DockerCompose.tex`
- `11AnexoC_ScriptsIntegracion.tex`
- `12AnexoD_EspecificacionesIEEE.tex`
- `13AnexoE_NodoIoT.tex`
- `14AnexoF_ConfiguracionesOpenWRT.tex`
- `15AnexoG_HipotesisDetalladas.tex`

**Conclusión:** para coherencia, el grafo y el análisis deben “anclarse” a estos archivos, no a backups o versiones refactorizadas.

---

## 2) Duplicidades / versiones paralelas (riesgo de incoherencia)

Se detectan múltiples archivos que declaran `\chapter{...}` con títulos muy similares:

- Marco Teórico:
  - `02MarcoTeorico.tex` (marcado como DEPRECADO en `0000.tex`)
  - `02MarcoTeorico_REFACTORED.tex`
  - `02MarcoTeorico_Rework.tex` (**el que se compila**)
  - `02Seccion02.tex`

- Arquitectura/Gateway:
  - `03Arquitectura_REFACTORED.tex` (deprecado)
  - `03Gateway*.tex` (varias variantes: backup/compressed/optimized)
  - `04Arquitectura*.tex` (arquitectura + backup/compressed)
  - `04Implementacion_NEW.tex` (deprecado)

- Resultados/Conclusiones:
  - `05Resultados_NEW.tex` (deprecado)
  - `06Resultados_Validacion.tex` (**el que se compila**)
  - `05Conclusiones.tex` vs `07Conclusiones_TrabajoFuturo.tex` (**el que se compila**)

**Riesgo:** cuando existen varias “verdades”, el documento se vuelve incoherente (una sección se actualiza en un archivo pero el PDF se compila desde otro).

**Acción recomendada (para el grafo):**
- En Obsidian, marcar claramente: **ACTIVO** vs **DEPRECADO**.
- Crear una nota `conceptos/Source_of_truth.md` que liste:
  - archivos activos
  - archivos deprecados
  - por qué se deprecó cada uno

---

## 3) Señales de “huecos” de contenido (TODO/MIGRAR)

Hay TODOs explícitos (ejemplos):
- insertar diagrama completo de arquitectura (drawio)
- migrar secciones desde `04Implementacion_NEW.tex`
- consolidar resultados experimentales en cap 6
- múltiples bloques: **"NUEVO contenido (400–1500 palabras)"**

**Interpretación:** el grafo puede ayudarnos a visualizar qué capítulos todavía no tienen:
- evidencia experimental
- figuras/diagramas clave
- migraciones completadas

---

## 4) Soporte bibliográfico (citas) — estado rápido

- Total apariciones de comandos `\cite...` en `.tex`: **488**
- Claves bibliográficas únicas detectadas: **194**

Top claves más citadas (proxy de “pilares”):
- `shelby6LoWPANWirelessEmbedded2009` (45)
- `andriuloEdgeComputingCloud2024` (33)
- `alsafranChallengesImplementingIoT2025` (23)
- `IEEERecommendedPractice` (20)
- `tangResearchInteroperabilityIoT` (18)
- `minhEdgeComputingIoTEnabled2022` (16)
- `IEEE802.11ah-2016` (11)
- `ISOIEC30141v2024` (10)

**Cómo usar esto en el grafo:**
- Crear notas en `fuentes/` para las ~20 más citadas.
- Enlazar cada fuente → a los capítulos donde aporta (Thread/LwM2M, HaLow, ISO30141, SEP2/IEEE2030.5, edge AI, seguridad).

---

## 5) Qué análisis podemos “correr” ya (en Obsidian)

### A) Mapa de coherencia (capítulo → conceptos → evidencia)
Para cada capítulo activo, vamos a construir 3 listas:
1) **Claims** (qué afirmo)
2) **Conceptos** (qué necesito dominar/definir)
3) **Evidencia** (papers/estándares/experimentos/figuras)

Esto se puede ver como:
- Canvas (visual)
- o Dataview (tablas/listas automáticas)

### B) Detección de huecos
- Claims sin evidencia (no linkean a `fuentes/` o `experimentos/`)
- Secciones sin citas por encima de X líneas
- Figuras referenciadas pero inexistentes

### C) Limpieza de duplicidad
- Un “panel” en Obsidian: qué archivos están activos vs deprecados.

---

## 6) Próximo paso recomendado (1–2 horas)

1) En `capitulos/*.md`, reemplazar “TODO” por:
   - resumen de 5–10 líneas
   - lista de conceptos (links a `conceptos/`)
   - lista de fuentes (links a `fuentes/`)

2) Generar automáticamente notas de fuentes top-20 en `fuentes/`.

3) En el Canvas principal, agregar clusters:
- Nivel 1: Thread + LwM2M
- Nivel 2: HaLow mesh + backhaul
- Nivel 3: Gateway edge + IA
- Nivel 4: ThingsBoard + Kafka
- Validación end-to-end

---

## 7) Preguntas para afinar el análisis (rápidas)

1) ¿Cuál es tu **claim central** en 1 frase? (ej: “una arquitectura jerárquica de 4 niveles…”)
2) ¿Tu evaluación se apoya más en:
   - latencia end-to-end,
   - consumo energético,
   - costo,
   - escalabilidad,
   - seguridad,
   - confiabilidad,
   - o una combinación?

Con esas dos respuestas, el grafo deja de ser “bonito” y se convierte en una herramienta de coherencia.
