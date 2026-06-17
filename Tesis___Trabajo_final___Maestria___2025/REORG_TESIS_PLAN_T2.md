# Plan de Reorganización — Observaciones Tutor 2

> **Documento paralelo a** `REORG_TESIS_PLAN.md` (tutor 1, ya ejecutado al 100%).
> **Alcance:** atender las 4 observaciones del tutor 2 sin re-romper lo consolidado por el tutor 1.

---

## 1. Resumen ejecutivo

Este plan organiza las cuatro observaciones del tutor 2 (T2-C1 a T2-C4) en un flujo de trabajo ejecutable y trazable, paralelo al plan del tutor 1. T2-C1 (hipótesis redundantes con objetivos) y T2-C2 (Tabla 5-1 inconsistente con objetivos) son los hallazgos de mayor severidad porque tocan la coherencia metodológica del documento; T2-C3 (faltan énfasis en viabilidad de despliegue escalable, interoperabilidad, crecimiento y sostenibilidad) requiere contenido nuevo en Resultados y Conclusiones; T2-C4 (espacios en blanco páginas 25/32/41) es un fix tipográfico de una línea con efecto global. El trabajo se estructura en 4 fases (E1 a E4) y se proyecta en 2-3 sesiones de edición. Toda intervención respeta los `\label`/`\ref` y la estructura de archivos resultante de la reorganización del tutor 1.

---

## 2. Inventario de observaciones T2

### T2-C1 — Hipótesis vs objetivos: redundancia (mismo parámetro)

> **Cita del jurado:** *"Las hipótesis están vinculadas al mismo parámetro que los objetivos; se vuelven redundantes."*

**Diagnóstico (evidencia A.1):**
- **5 redundancias directas** confirmadas:
  - OE1 (consumo < 150 mW) ≡ OV5 (< 150 mW promedio)
  - OE1 (comisionamiento < 10 min) ≡ OV2 (< 10 min/nodo)
  - OE2 (entrega > 98%, alcance > 500 m) ≡ OV4 (> 98% entrega, > 500 m LoS)
  - OG (P95 EaE < 100 ms) ≡ OV3 (P95 borde < 10 ms + reducción EaE > 70%)
  - OG (disponibilidad > 99% durante 72 h) ≡ OV1 + OV6 (> 99% lecturas, > 99% uptime)
- **Causa raíz:** los OV están redactados como "el sistema cumplirá los umbrales operacionales de los OE" — son confirmaciones de implementación, no predicciones falsables que vayan más allá de la especificación.
- **Archivos involucrados:** `01Introduccion.tex` líneas 189-191 (OG), 200-222 (OE), 234-262 (OV).

**Severidad:** ALTA — el jurado leerá las OV como repetición de los OE y restará rigor metodológico.

---

### T2-C2 — Tabla 5-1: inconsistencias con objetivos

> **Cita del jurado:** *"La Tabla 5-1 presenta inconsistencias respecto a los objetivos planteados."*

**Diagnóstico (evidencia A.2):**

| Inconsistencia | Detalle |
|---|---|
| Latencia EaE: Tabla 5-1 fija **300 ms**, Objetivos fijan **< 100 ms** | Tabla usa umbral regulatorio ANSI C12.19; Objetivos usan umbral arquitectónico RF1. Coexisten sin nota explicativa. |
| Tráfico WAN (OV7 > 70%) | Ausente en Tabla 5-1 |
| Sobrecarga de paquetes (hipótesis general > 60%) | Ausente en Tabla 5-1 |
| Latencia por salto (OE2 < 30 ms) | Ausente en Tabla 5-1 |
| Alcance HaLow (OV4 > 500 m) | Ausente en Tabla 5-1 (OV4 reporta 180 m medido + 1,200 m de literatura) |
| Tiempo aprovisionamiento (OV2 < 10 min) | Ausente en Tabla 5-1 (resultado 2.8 min) |
| Tasa de entrega (OE2 > 98%) | Ausente en Tabla 5-1 (resultado 99.995% e2e) |

**Archivos:** `06Resultados_Validacion.tex` (Tabla 5-1) + cross-check contra `01Introduccion.tex`.

**Severidad:** ALTA — afecta directamente la lectura del capítulo de Resultados frente a Objetivos.

---

### T2-C3 — Falta énfasis en viabilidad de despliegue escalable + interoperabilidad + crecimiento + sostenibilidad

> **Cita del jurado:** *"Falta énfasis en viabilidad de despliegues escalables, interoperabilidad, crecimiento de la infraestructura y sostenibilidad operativa en contextos reales de redes inteligentes."*

**Diagnóstico (evidencia A.3):**

| Sub-tema | Estado actual | Brecha |
|---|---|---|
| Escalabilidad | MEDIA (60→100 nodos validado, 100→1K extrapolado) | No hay modelo multi-gateway ni TCO 5 años |
| Interoperabilidad | PROFUNDA (mapeo ISO/IEC 30141:2024 completo, DLMS preservado) | Falta validación multi-vendor (sólo RPi4) |
| Crecimiento | SUPERFICIAL (sólo en Trabajo Futuro Línea 2/6) | Sin diagrama edificio→manzana→distrito |
| Sostenibilidad | MEDIA (OPEX = 0 demostrado, OTA validado) | Falta MTTF/MTTR, política de repuestos, monitoreo cuantitativo |

**Archivos:** `06Resultados_Validacion.tex` (§Pruebas Escalabilidad + §ISO 30141), `07Conclusiones_TrabajoFuturo.tex` (Trabajo Futuro), `01Introduccion.tex` §1.6 (Justificación económica).

**Severidad:** MEDIA-ALTA — el tutor pide producción real en red inteligente; lo que falta diferencia tesis académica de tesis aplicada.

---

### T2-C4 — Espacios en blanco páginas 25, 32, 41

> **Cita del jurado:** *"Hay espacios en blanco notorios en las páginas 25, 32 y 41."*

**Diagnóstico (evidencia A.4):**

| Página | Whitespace inferior medido | Naturaleza |
|---|---|---|
| p25 | 7.0% (margen normal) | Falso positivo en magnitud; **se ve "suelto"** por 4 headings (§1.5.3, §1.6, §1.6.1, §1.6.2) con `\parskip = \baselineskip` |
| p32 | **19.4%** (exceso ~12%) | Real — fin de Cap. 1 (`\include` fuerza `\clearpage`); 6 párrafos `\textbf{Capítulo …}` consecutivos con `\parskip` completo entre ellos |
| p41 | **14.4%** (exceso ~7%) | Real — fin de Cap. 2 (`\include` fuerza `\clearpage`); 4 párrafos `\textbf{Brecha N.}` consecutivos |

**Causa raíz global:** `0000.tex` línea 408 `\setlength{\parskip}{\baselineskip}` infla cada salto entre párrafos en todo el cuerpo principal.

**Severidad:** BAJA en contenido / ALTA en presentación — el fix global es 1 línea.

---

## 3. Plan por concern

### 3.1 T2-C1 — Reframe de OV de "verificación de spec" a "claim arquitectónico falsable"

**Estrategia (Opción A del análisis A.1):** mantener OE como checkpoints operacionales (lo que el sistema **debe hacer**); reformular OV1-OV7 como predicciones falsables **más allá** del threshold del OE asociado. El umbral cuantitativo se conserva en la OV pero la **declaración inicial** explica *por qué la arquitectura permite el threshold*, no que el sistema lo cumplirá.

**Patrón de redacción propuesto:**

```
OV<n>'. [Claim arquitectónico]: <pila X> permite <comportamiento Y>
        donde <baseline tradicional Z> no lo permite, porque <razón estructural>.
        Validación: bajo <carga/config>, el sistema cumple <umbral cuantitativo>
        con <confianza estadística>, satisfaciendo <RFx>.
```

**Acciones concretas:**

| # | Archivo | Líneas | Acción |
|---|---------|--------|--------|
| T2-C1.1 | `01Introduccion.tex` | 234-262 | Reescribir bloque `\section{Hipótesis}` (sec:hipotesis) aplicando el patrón a las 7 OV. Conservar identificadores OV1-OV7 y `\label{sec:hipotesis}`. |
| T2-C1.2 | `01Introduccion.tex` | 232-234 (antes de OV1) | Insertar `\paragraph{Lógica de las hipótesis:}` de 4-6 líneas explicando que las OV no replican los OE sino que predicen comportamientos arquitectónicos cuya falla invalidaría el diseño aunque los OE se alcanzasen individualmente. |
| T2-C1.3 | `01Introduccion.tex` | 200-222 | Verificar que los OE conservan **sólo** el lenguaje operacional ("nodo con X, comisionado en Y, consumo Z"). Si alguno arrastra lenguaje de hipótesis, recortarlo. |
| T2-C1.4 | `06Resultados_Validacion.tex` | tabla(s) que vinculan OV↔resultados | Verificar que los resultados de OV (que ya existen en el piloto) ahora se reportan contra el **nuevo** texto de claim, no contra el umbral solo. |

**Reformulaciones específicas sugeridas (manteniendo numeración):**

- **OV1':** *La integración Thread+LwM2M+ThingsBoard-Edge es viable durante operación continua de 30 días bajo condiciones de un piloto urbano sin intervención manual.* Validación: > 99% lecturas exitosas sobre 60 nodos.
- **OV2':** *El comisionamiento Thread por QR escala sub-linealmente con el número de nodos.* Validación: < 10 min/nodo independiente de n para n ≤ 60.
- **OV3':** *El procesamiento en el borde reduce la latencia EaE por debajo del límite que exigiría retorno a nube.* Validación: P95 borde < 10 ms y reducción EaE > 70% vs arquitectura cloud-centric equivalente.
- **OV4':** *Sub-GHz HaLow es backhaul suficiente sin OPEX recurrente bajo restricciones de ancho de banda 2/4/8 MHz.* Validación: > 98% entrega y > 500 m LoS.
- **OV5':** *La pila ligera Thread+CoAP+LwM2M habilita operación < 150 mW donde Thread+MQTT excede ese presupuesto.* Validación: < 150 mW promedio en telemetría 30 s.
- **OV6':** *La arquitectura tolera particiones WAN ≥ 48 h sin pérdida de telemetría ni intervención.* Validación: cero pérdida + sincronización completa al restablecer enlace.
- **OV7':** *Las reglas de borde (filtrado + agregación) reducen estructuralmente el tráfico WAN respecto a arquitectura nube directa.* Validación: > 70% reducción medida.

**Esfuerzo estimado:** 2-3 h de edición + 30 min compilación/verificación de refs.

---

### 3.2 T2-C2 — Armonizar Tabla 5-1 con objetivos

**Tabla de acciones por fila (resumen ejecutable):**

| # | Métrica | Acción | Detalle |
|---|---------|--------|---------|
| T2-C2.1 | Latencia borde (< 10 ms) | KEEP_AS_IS | Coherente OE/OV/Tabla |
| T2-C2.2 | Latencia EaE | SEPARAR_EN_DOS_FILAS | Fila 1: "Latencia EaE arquitectónica" < 100 ms (RF1/OV3); Fila 2: "Latencia EaE regulatoria (ANSI C12.19)" < 300 ms. Nota al pie aclarando el contexto. |
| T2-C2.3 | Disponibilidad (> 99.5%) | KEEP_AS_IS | Tabla más estricta; coherente |
| T2-C2.4 | Consumo (< 150 mW) | KEEP_AS_IS | Coherente |
| T2-C2.5 | **Reducción tráfico WAN (> 70%)** | AGREGAR_FILA | Objetivo OV7. Resultado piloto 72%. |
| T2-C2.6 | **Sobrecarga de paquetes (> 60%)** | AGREGAR_FILA | Hipótesis general; resultado piloto 91% via IPHC. |
| T2-C2.7 | **Tiempo aprovisionamiento (< 10 min/nodo)** | AGREGAR_FILA | OV2/RF5; resultado piloto 2.8 min. |
| T2-C2.8 | **Tasa de entrega (> 98%)** | AGREGAR_FILA | OE2; resultado piloto 99.995% e2e. |
| T2-C2.9 | Latencia por salto Thread (< 30 ms) | DOCUMENTAR_DESVIO | Aclarar en §6 que el margen < 30 ms se cumple con 11 ± 3 ms/salto medido. |
| T2-C2.10 | Alcance HaLow (> 500 m LoS) | DOCUMENTAR_DESVIO + AGREGAR_FILA | Fila con valor objetivo; nota: 180 m validado empírico, > 500 m por literatura. Recomendar validación en campo (Trabajo Futuro). |

**Archivos:** `06Resultados_Validacion.tex` (estructura de Tabla 5-1) + texto narrativo aledaño.

**Esfuerzo estimado:** 1.5-2 h edición + 30 min recompilación.

---

### 3.3 T2-C3 — Agregar viabilidad de despliegue escalable + crecimiento + sostenibilidad

**Estrategia recomendada: (C) BOTH + CONTEXT** del análisis A.3 — añadir nueva sección de Resultados + expandir Conclusiones, sin tocar la sección de interoperabilidad ISO (ya profunda).

#### 3.3.1 Nueva §6.5 "Viabilidad de despliegue escalable"

**Ubicación:** `06Resultados_Validacion.tex`, después de §6.4 (ISO/IEC 30141) y antes del cierre del capítulo.

**Outline propuesto:**

```
\section{Viabilidad de despliegue escalable}
\label{sec:viabilidad-despliegue}

  \subsection{Topología de crecimiento: edificio → manzana → distrito}
  \label{sec:topologia-crecimiento}
    - Figura/diagrama: 1 gateway × 60 nodos → 5 gateways × 300 nodos →
      20 gateways × 1,200 nodos
    - Tabla: límites Thread (250 dev/partition, neighbor table 64) y
      HaLow (saturación a ~1,000 nodos/gateway @ 150 kbps) mapeados a
      umbrales de "añadir gateway".

  \subsection{Modelo de costo total de propiedad (TCO 5 años)}
  \label{sec:tco-5y}
    - Tabla: CAPEX (RPi4 + HAT HaLow + UPS) × N gateways +
      OPEX = 0 (ISM) + energía + mantenimiento.
    - Comparación contra arquitectura celular equivalente.

  \subsection{Redundancia geográfica}
  \label{sec:redundancia}
    - Estrategia de gateway de respaldo por zona;
    - Failover Thread (mecanismo nativo Leader→Router promotion);
    - Recuperación post-falla.

  \subsection{Multi-vendor portability}
  \label{sec:multi-vendor}
    - Discusión: la pila completa es Docker → portable a Intel NUC,
      Orange Pi, x86 industrial. Validación empírica acotada
      (pendiente Trabajo Futuro).
```

#### 3.3.2 Expansión §7 "Análisis de sostenibilidad operativa"

**Ubicación:** `07Conclusiones_TrabajoFuturo.tex`, nueva sub-sección antes de Trabajo Futuro.

**Outline propuesto:**

```
\subsection{Sostenibilidad operativa}
\label{sec:sostenibilidad}

  \paragraph{MTTF/MTTR proyectados.}
    Basado en 30 días de piloto sin fallos: límite inferior conservador
    MTTF ≥ 30 días/gateway; MTTR objetivo ≤ 4 h con repuestos en sitio.
    Implicación: 99.4% disponibilidad anual con un gateway, > 99.95%
    con redundancia activa-pasiva.

  \paragraph{Política OTA + rollback.}
    OTA validado (2 actualizaciones, 30 nodos, 100% éxito). Protocolo
    de despliegue gradual: canary (5 nodos × 24 h) → rampa 25% → 50%
    → 100%. Rollback automático si métrica de salud cae > 10%.

  \paragraph{Monitoreo y alertas.}
    Umbrales concretos: CPU > 80% (5 min) → alerta amarilla;
    almacenamiento < 1 GB → alerta amarilla; PDR < 95% (15 min)
    → alerta roja. Health checks cada 60 s.

  \paragraph{Lifecycle del firmware.}
    Versionado semántico, ventanas de soporte, plan de end-of-life.
```

**Esfuerzo estimado:** 4-6 h (incluye creación de 1-2 figuras nuevas).

---

### 3.4 T2-C4 — Eliminar espacios en blanco p25/p32/p41

#### Tabla página × diagnóstico × fix

| Pág | Whitespace inferior | Diagnóstico | Fix recomendado |
|---|---|---|---|
| p25 | 7.0% (margen normal) | Sensación de "suelto" por 4 headings consecutivos con `\parskip` completo | Fix global de parskip (ver abajo) + opcional: convertir §1.6.1 y §1.6.2 de `\subsection` a `\paragraph` con título run-in |
| p32 | 19.4% (real) | Fin de Cap. 1 forzado por `\include` + 6 párrafos `\textbf{Capítulo …}` con `\parskip` completo entre cada uno | Fix global de parskip + envolver los 6 párrafos en `description` con `\itemsep=0pt` |
| p41 | 14.4% (real) | Fin de Cap. 2 forzado por `\include` + 4 párrafos `\textbf{Brecha N.}` con `\parskip` completo | Fix global de parskip + convertir B1-B4 a `enumerate` con `label=\textbf{Brecha \arabic*.}` |

#### Fix global de una sola línea (recomendado primer paso)

**Archivo:** `0000.tex`, línea 408.

```diff
- \setlength{\parskip}{\baselineskip}
+ \setlength{\parskip}{0.5\baselineskip plus 2pt minus 1pt}
```

**Efecto esperado:**
- p25: deja de verse "suelto" sin afectar legibilidad.
- p32: cierra o reduce drásticamente el gap (recupera 6-8 líneas).
- p41: cierra o reduce el gap.
- Resto del documento: tightens cada página de cuerpo principal sin reflujo agresivo (TeX redistribuye contenido para llenar).

**Verificación post-fix:** re-medir whitespace con `pdftoppm -r 110` + análisis de densidad (el script ya está en `audit-whitespace/`); aceptar si las 3 páginas quedan ≤ 10% whitespace inferior.

**Esfuerzo estimado:** 15 min (fix + recompilación + verificación visual).

---

## 4. Tabla consolidada de cambios propuestos

| ID | Concern | Archivo | Líneas | Acción | Esfuerzo |
|---|---|---|---|---|---|
| T2-C1.1 | C1 | `01Introduccion.tex` | 234-262 | REWRITE OV1-OV7 como claims arquitectónicos | 2 h |
| T2-C1.2 | C1 | `01Introduccion.tex` | ~233 | INSERT `\paragraph{Lógica de las hipótesis:}` | 30 min |
| T2-C1.3 | C1 | `01Introduccion.tex` | 200-222 | VERIFY/TRIM lenguaje de OE | 30 min |
| T2-C1.4 | C1 | `06Resultados_Validacion.tex` | Tabla(s) OV↔resultados | VERIFY refs a nuevos claims | 30 min |
| T2-C2.1 | C2 | `06Resultados_Validacion.tex` | Tabla 5-1 | SEPARAR fila latencia EaE en 2 (arquitectónica/regulatoria) | 30 min |
| T2-C2.2 | C2 | `06Resultados_Validacion.tex` | Tabla 5-1 | AGREGAR 5 filas (WAN, sobrecarga, aprovisionamiento, PDR, alcance HaLow) | 1 h |
| T2-C2.3 | C2 | `06Resultados_Validacion.tex` | texto §6 | DOCUMENTAR desvíos (latencia/salto, alcance HaLow) | 30 min |
| T2-C3.1 | C3 | `06Resultados_Validacion.tex` | después §6.4 | NEW §6.5 Viabilidad de despliegue escalable (4 sub-secciones) | 3 h |
| T2-C3.2 | C3 | `07Conclusiones_TrabajoFuturo.tex` | antes Trabajo Futuro | NEW §7.x Sostenibilidad operativa | 2 h |
| T2-C3.3 | C3 | `06Resultados_Validacion.tex` o `figs/` | nueva | CREATE figura topología crecimiento (edificio→manzana→distrito) | 1 h |
| T2-C4.1 | C4 | `0000.tex` | 408 | EDIT `\parskip` global a `0.5\baselineskip plus 2pt minus 1pt` | 5 min |
| T2-C4.2 | C4 | `01Introduccion.tex` | ~370-391 | OPTIONAL: envolver 6 párrafos `Capítulo …` en `description` compacta | 30 min |
| T2-C4.3 | C4 | `02MarcoTeorico_Rework_V2.tex` | ~315-330 | OPTIONAL: convertir B1-B4 a `enumerate` con label `Brecha N` | 30 min |
| T2-C4.4 | C4 | — | — | VERIFY whitespace post-fix con script `audit-whitespace/` | 15 min |

**Total estimado:** ~13 h de edición efectiva (2-3 sesiones de 4-5 h cada una).

---

## 5. Cronograma (fases recomendadas)

| Fase | Contenido | Duración | Dependencias |
|---|---|---|---|
| **E1 — Fix tipográfico (T2-C4)** | T2-C4.1 + recompilación + verificación de las 3 páginas | 30 min | Ninguna. Hacer primero porque libera capacidad mental y desbloquea la lectura del documento. |
| **E2 — Reframe de hipótesis (T2-C1)** | T2-C1.1 → T2-C1.4 | 4 h | E1. Antes que T2-C2 porque OV reformuladas pueden cambiar qué se reporta en Tabla 5-1. |
| **E3 — Armonización Tabla 5-1 (T2-C2)** | T2-C2.1 → T2-C2.3 | 2 h | E2 (las nuevas OV definen qué medir). |
| **E4 — Viabilidad + sostenibilidad (T2-C3)** | T2-C3.1 → T2-C3.3 | 6 h | E3 (los resultados se referencian desde §6.5). |
| **E5 — Verificación final** | Compilación full + revisión refs + re-medición whitespace + lectura cruzada con T1 | 1 h | E1-E4 completas. |

**Total:** ~13.5 h, distribuibles en 2-3 sesiones.

---

## 6. Coordinación con plan del tutor 1

### 6.1 Qué pisa con `REORG_TESIS_PLAN.md` (T1, ya ejecutado)

| Cambio T2 | Riesgo vs T1 | Mitigación |
|---|---|---|
| T2-C1 (reescribir OV) | T1 Paso B.3 ya reescribió OV con patrón "carga + umbral + confianza + RF". El nuevo patrón T2 (claim arquitectónico) **se construye sobre** el patrón T1, no lo reemplaza. | Conservar dimensión + carga + RF de T1 como segunda frase de validación; añadir claim arquitectónico como primera frase. |
| T2-C2 (Tabla 5-1) | T1 no tocó Cap. 6. | Sin conflicto. |
| T2-C3 (§6.5 nueva + §7 expandida) | T1 no tocó Cap. 6 ni Cap. 7. | Sin conflicto. |
| T2-C4 (parskip global) | T1 ajustó figs (C6) que ya respetan IEEE pt mínimo. Cambiar parskip no afecta TikZ. | Sin conflicto; verificar overfull `\hbox` post-fix. |

### 6.2 Qué es nuevo (no cubierto por T1)

- Reframe semántico de OV (T1 reorganizó forma, T2 reorganiza fondo).
- Auditoría cuantitativa Tabla 5-1 vs Objetivos.
- §6.5 Viabilidad de despliegue escalable (sección nueva).
- §7.x Sostenibilidad operativa (sub-sección nueva).
- Fix tipográfico global de parskip.

### 6.3 Lo que NO cambia

- Estructura de archivos `\include` en `0000.tex`.
- Reorganización Cap. 1 → Cap. 2 (T1 Pasos A.1-A.9 conservados).
- Figuras `fig_topologia_armario` y `fig_arquitectura_tres_niveles` (T1 Pasos C.1-C.2 conservados).
- Tablas `tab:mesh-comparison` y `tab:lastmile-comparison` en Marco (T1 Pasos A.3-A.4 conservados).

---

## 7. Trabajo paralelo: skill `tikz-figure` creada

**Ubicación:** `c:/Users/jsgir/Documents/UNAL/tesis/.claude/skills/tikz-figure/SKILL.md`

**Síntesis:** durante este ciclo se creó una skill de Claude Code (`tikz-figure`) que encapsula tres patrones reutilizables derivados del trabajo sobre `fig_topologia_armario` y `fig_arquitectura_tres_niveles` (T1 C6): (1) **single-source-of-truth** con `figures/<name>.tex` (cuerpo TikZ sin documentclass/caption) + wrapper `standalone` para regenerar PDF/SVG/PNG sin divergencia entre Overleaf y assets vectoriales; (2) **paleta saturada anti-blanco-impreso** con convención fija por subsistema (green=campo, blue=edge, orange=backhaul, purple=on-prem, brown=medidor) y reglas duras de fills/texto que prohíben `!4` en cajas y `gray!50` en texto; (3) **tipografía IEEE-compliant calculada** con fórmula `rendered_pt = source_pt × scale`, tabla de combinaciones, prohibición de `\tiny`/`\scriptsize` y la trampa del doble `\resizebox`. La skill es referenciable desde cualquier sesión futura cuando se cree o ajuste una figura TikZ.

**Follow-up no priorizado en este plan:** sub-skills tipo `tikz-protocol-stack`, `tikz-mesh-topology`, `tikz-pcb-block` heredando reglas base.

---

## 8. Registro de decisiones (plantilla — llenar durante ejecución)

| Fecha | Decisión | Alternativas evaluadas | Justificación | Impacto en plan |
|-------|----------|------------------------|---------------|-----------------|
| — | — | — | — | — |

---

## 9. Riesgos identificados

| Fecha | Riesgo | Severidad | Mitigación propuesta | Estado |
|-------|--------|-----------|----------------------|--------|
| 2026-06-17 | Cambiar `\parskip` global puede generar nuevos overfull `\hbox` en pasajes con tablas anchas o ecuaciones | BAJA | Recompilar y revisar log; en peor caso, restaurar `\parskip` original y aplicar fixes locales (description/enumerate) por página | ⬜ |
| 2026-06-17 | Reformular OV puede dejar resultados del piloto sin claim correspondiente si los datos no soportan el nuevo claim arquitectónico (ej. OV5' exige comparativa contra Thread+MQTT, no medida) | MEDIA | Donde no haya dato comparativo, formular como claim acotado a la pila implementada y delegar comparación a Trabajo Futuro | ⬜ |
| 2026-06-17 | §6.5 nueva puede empujar la numeración de §6.4 (ISO 30141) → cuidar `\ref{tab:conformidad-iso30141}` | BAJA | Si §6.5 se inserta DESPUÉS de §6.4, no hay renumeración. Verificar tras compilar. | ⬜ |

---

## 10. Próximos pasos (checklist primera sesión)

- [ ] Ejecutar E1 (fix `\parskip` global) y verificar p25/p32/p41 con script de whitespace
- [ ] Ejecutar E2.T2-C1.1 (reescritura OV1-OV7) y compilar
- [ ] Registrar decisiones en §8 conforme aparezcan dilemas (especialmente: ¿OV5' exige medición comparativa MQTT? Si no hay dato, ¿se acota el claim o se difiere?)
- [ ] Actualizar `REORG_TESIS_TRACKING.md` §8 con estado de cada concern T2
