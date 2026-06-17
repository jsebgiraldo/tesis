# Gobernanza de datos del lado servidor (TB Edge + LwM2M)

## Principio

El firmware expone el árbol de recursos completo; **el device profile del
servidor decide qué fluye y a qué cadencia**. Tres mecanismos, cero cambios
de firmware:

### 1. `observe` list = la fuente de verdad del dato

Solo los paths en `observeAttr.observe` generan telemetría. Un recurso que el
firmware expone pero el profile no observa **jamás llega** (bug recurrente
sufrido: voltage /10242/0/4 estuvo meses sin observarse mientras el dashboard
mostraba un valor congelado).

### 2. `pmax` = SLA de cadencia garantizado por protocolo

El atributo pmax del observe obliga al cliente LwM2M a notificar al menos cada
pmax segundos aunque el valor no cambie (OMA LwM2M Core, attributes pmin/pmax).
Es la respuesta correcta a "¿cómo garantizo dato cada minuto?": pmax=60 en el
profile, y el engine del nodo (Zephyr) lo cumple automáticamente.

`pmin` acota el extremo opuesto (no notificar más seguido que pmin aunque el
valor oscile) — protege el canal radio y el cliff eléctrico.

### 3. Perfil mínimo AMI aplicado (facturación + calidad de energía)

| Path | Key | Justificación | pmin/pmax |
|---|---|---|---|
| /10242/0/41 | activeEnergy (kWh) | registro de facturación | 15/60 s |
| /10242/0/34 | activePower (kW) | demanda | 15/60 s |
| /10242/0/4 | voltage (V) | calidad: sag/swell | 15/60 s |
| /10242/0/49 | frequency (Hz) | calidad | 15/60 s |
| /10242/0/39 | powerFactor | calidad + penalización FP | 15/60 s |
| /3/0/0..3 | mfr/model/serial/fw | identidad | on-change |

El perfil previo observaba 37 paths (18 muertos tras minimizar el firmware,
7 de reactivas/aparentes sin uso) con pmax=900 s.

## Propagación: el detalle operativo crítico

Los cambios de observe del profile se aplican a cada nodo **en su próximo
REGISTER**, no sobre las sesiones abiertas. Con lifetime=86400 s la propagación
natural tarda horas. Métodos de forzado verificados:
- RPC `Execute /3/0/4` (Reboot) por device — para nodos individuales
- Power-cycle de la PSU — propagación fleet-wide en ~3 min

Medición en vivo del rollout (delivery-ratio): 7% → 50% → 83% en ~8 min tras
el power-cycle.

## SLA realista

Con pmax=60: ~99.5% de los minutos entregan dato. Los huecos restantes son los
ciclos auto-recuperados (~4/board/día × 1-3 min). Garantía de completeness al
100% exige store-and-forward (buffer local de intervalos con timestamp y
re-entrega) — trabajo futuro; es el patrón de los medidores AMI comerciales.
