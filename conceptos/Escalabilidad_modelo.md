# Escalabilidad — modelo (devices/gateways/throughput)

## Parámetros (según conversación)

- Dispositivos objetivo: **500**
- Gateways: ~**1 por 100** nodos (variable por distancia/posición)
- Mensajería DLMS (trifásico): cada **10 min** (modo normal)
- Modos especiales:
  - **1 min**
  - **1 s** (para IA)

## Modelo de carga (plantilla)

Define:
- `N` = #dispositivos
- `G` = #gateways
- `p` = fracción de dispositivos en modo especial (0..1)
- `T` = periodo (s) (600 para 10min, 60 para 1min, 1 para 1s)
- `m` = mensajes por lectura (DLMS puede implicar varios OBIS/frames)

Throughput aproximado (msgs/s):

`R = N * p * m / T`

### Escenarios a reportar

- S0: 500 dispositivos, 10 min (p=1, T=600)
- S1: 500 dispositivos, 1 min (p=1, T=60)
- S2: 10% en 1 s (p=0.1, T=1)
- S3: ráfaga (pico) durante X minutos

## Dimensionamiento por gateway

- `N_g = N / G` (promedio)
- Medir/estimar:
  - CPU/RAM del gateway
  - colas/buffers
  - tasa de retransmisión
  - ancho de banda uplink

## Métricas mínimas

- Latencia end-to-end (p50/p95/p99)
- Pérdida de mensajes / duplicados
- Disponibilidad / reconexiones
- Backpressure / tamaño de cola
- Coste por gateway (HW + energía)
