# pdfs_master — PDFs compilados de la tesis

Carpeta de PDFs compilados y registro de cambios. Aquí se guarda **siempre la versión
más reciente** del documento compilado más un PDF de diff para revisar los cambios sin
re-leer todo el documento.

## Contenido

| Archivo | Descripción |
|---|---|
| `tesis_2026-06-17_v-jurados_96pp.pdf` | Versión compilada más reciente de la tesis (96 páginas), con todas las observaciones de los jurados 1 y 2 atendidas. Fuente: `Tesis___Trabajo_final___Maestria___2025/0000.tex`. |
| `tesis_2026-06-17_diff-vs-ultimo-commit.pdf` | Diff unificado coloreado (verde = agregado, rojo = eliminado) de los 7 archivos `.tex` modificados, contra el último commit previo a la reorganización (`e4b3ea5`). 26 páginas. |
| `cambios_2026-06-17.diff` | El diff crudo en texto plano (fuente del PDF anterior). |

## Convención de nombres

`tesis_<fecha>_<etiqueta>_<páginas>pp.pdf` para versiones compiladas.
`tesis_<fecha>_diff-vs-<referencia>.pdf` para diffs.

## Cómo regenerar

```bash
# PDF de la tesis
cd Tesis___Trabajo_final___Maestria___2025
pdflatex 0000.tex && bibtex 0000 && pdflatex 0000.tex && pdflatex 0000.tex
cp 0000.pdf ../pdfs_master/tesis_<fecha>_<etiqueta>_<N>pp.pdf

# PDF de diff (desde la raíz del repo)
git diff HEAD -- <archivos .tex> > pdfs_master/cambios_<fecha>.diff
# luego compilar con el wrapper LaTeX (listings con lenguaje gitdiff coloreado)
```

## Trazabilidad de los cambios

- Observaciones del **jurado 1** (forma del discurso): ver `../Tesis___Trabajo_final___Maestria___2025/REORG_TESIS_PLAN.md`
- Observaciones del **jurado 2** (rigor metodológico + diagramación): ver `../Tesis___Trabajo_final___Maestria___2025/REORG_TESIS_PLAN_T2.md`
- Registro vivo de todos los cambios ejecutados: ver `../Tesis___Trabajo_final___Maestria___2025/REORG_TESIS_TRACKING.md`
