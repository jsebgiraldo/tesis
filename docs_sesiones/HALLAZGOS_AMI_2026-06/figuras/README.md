# Figuras (200 dpi, listas para LaTeX)

| Archivo | Contenido | Doc fuente |
|---|---|---|
| `fig_tipo1_corte_instantaneo.png` | Evento Tipo 1: VBUS y 3V3 nominales → 0 V en 5 µs (corte de host USB) | 01 |
| `fig_tipo2_oscilacion_ldo.png` | Evento Tipo 2: oscilación del riel 3V3 con VBUS estable, 40 ms de hiccup → corte | 01 |
| `fig_tipo2_zoom.png` | Zoom 0-2 ms de la oscilación 1.8↔3.1 V (current-limit del AMS1117) | 01 |

Datos crudos: `Unal-Flash-tool/firmware/ami-lwm2m-node/logs/ad2/*.csv`
(columnas: t_us_from_buf_start, ch1_3v3, ch2_5v; 200 kHz; trigger en el punto
medio del buffer).

CSVs fuente de cada figura:
- Tipo 1: `capture_2026-06-10T162352_f1.csv`
- Tipo 2: `capture_2026-06-10T084350_f1.csv`

Pendientes de generar al cierre de las mediciones de 24 h:
- Curva delivery-ratio del rollout pmax=60 (de `logs/delivery_ratio.csv`)
- Histograma temporal de re-REGISTERs (episodios mesh-wide)
