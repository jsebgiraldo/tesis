# El espacio de diseño: perillas vs capacidad física

## Las dos perillas de software

1. **Número de paths observados** (server-side, device profile) — gobierna el
   tráfico periódico: cuántos notifies y de qué tamaño por intervalo.
2. **CoAP block size** (firmware, `LWM2M_COAP_BLOCK_SIZE`) — gobierna la
   duración del burst de TX por fragmento en transferencias block-1
   (REGISTER, payloads grandes, OTA).

Ambas se relacionan con **la capacidad física de la alimentación** (no de la
antena en sí): la protección de sobrecorriente del host USB/PSU integra la
corriente del transmisor en ventanas de 10-50 ms. El radio a ~130 mA puede
sostenerse unos pocos ms sin disparo; bursts ≥10 ms acumulan suficiente para
cortar VBUS (mecanismo Tipo 1).

## Mapa empírico (knob map)

| Observe paths | Block size | TX burst típico | Tasa de cortes | Estado |
|---|---|---|---|---|
| 37 (full) | 512 | ~10 ms/frag + pushes 36-RID | ~10/h | medido (la flota original) |
| 37 (full) | 64 | ~1.2 ms/frag, pushes largos | ~2.4/h | medido |
| 5 (minimal FW) | 64 | ~1.2 ms, pushes 1-paquete | **0 en 8+ h** | medido |
| 5 (minimal FW) | 256 | ~5 ms | ~0.6-0.85/h | medido (AD2 4.3 h) |
| 5 (server-trim) | 256 | ~5 ms | en medición | flota 30 nodos, 24 h en curso |
| **37 (full FW) + 5 observados** | **64** | ~1.2 ms, REGISTER fragmentado | **hipótesis: ~0** | **celda pendiente** |

La celda pendiente es la que valida la arquitectura preferida:
**fat firmware / thin profile** — el firmware embarca todas las capacidades
(diagnóstico, control, OTA) y el servidor decide qué se usa. Si da ~0, el
costo del firmware completo es solo flash/RAM, no estabilidad eléctrica.

## Reglas de diseño derivadas

1. **Block size ≤ 256 B para nodos USB-powered**; 64 B si se requiere cero
   eventos. El costo es velocidad de OTA (650 KB: ~30 s a 64 vs ~8 s a 256).
2. **El REGISTER es el burst más largo del ciclo de vida** — su tamaño escala
   con el árbol de objetos; fragmentarlo (block pequeño) lo neutraliza.
3. **pmin en los observes** acota la frecuencia de notifies bajo oscilación
   del dato — protege la misma restricción física desde la otra perilla.
4. **El presupuesto se evalúa en corriente integrada por ventana de ~10 ms**,
   no en pico instantáneo. Instrumento mínimo necesario: scope de 2 canales
   sobre VBUS y 3V3 con trigger por nivel.

## Trabajo futuro inmediato

- Medir la celda pendiente (1 board + AD2, ~2 h).
- Barrido fino de block ∈ {64,128,256} × observe ∈ {5,12,37} para la
  superficie completa tasa-de-cortes(paths, block) — material de paper.
