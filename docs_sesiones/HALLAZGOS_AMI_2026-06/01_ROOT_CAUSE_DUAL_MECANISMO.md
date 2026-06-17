# Root cause: dos mecanismos de reset, ninguno era "brownout de firmware"

## Contexto

Desde v0.6.46 la flota de 30 ESP32-C6 SuperMini presentaba resets crónicos
(~10/h/nodo) con reset-cause = BRN. Catorce iteraciones de firmware (watchdogs,
keepalives, backoffs, TX power, caps en VBUS) no lo resolvieron porque el
diagnóstico de partida era erróneo.

**Hallazgo de método**: el registro de causa de reset del ESP32-C6 reporta BRN
indistintamente para (a) brownout interno real y (b) corte externo de
alimentación. Sin instrumentación analógica son indistinguibles.

## Instrumentación

Digilent Analog Discovery 2, captura disparada:
- CH1 → VBUS 5 V, CH2 → riel 3.3 V (salida del LDO), GND común al board
- Trigger: 3.3 V cayendo bajo 2.4 V (umbral BOD del chip: 2.51 V)
- 200 kHz, ventana 82 ms centrada en el trigger
- Script: `tools/ad2_brownout_capture.py` (ctypes + dwf.dll)

## Mecanismo Tipo 1 — corte de VBUS por sobrecorriente del host (dominante)

**Firma** (fig_tipo1_corte_instantaneo.png): ambos rieles operando nominales
(3.22 V / 5.04 V) → **0 V en una única muestra de 5 µs**. Sin precursor.

**Cadena causal**: transferencias CoAP block-1 sostenidas (REGISTER con 37
recursos custom a block-size 512 = ~10 ms de TX de radio continuo a ~130 mA)
→ el host USB integra la corriente en ventanas de 10-50 ms → dispara su
protección de sobrecorriente → VBUS cortado → reset. El chip reporta BRN
porque su supply desapareció, pero el BOD interno nunca actuó por sí solo.

**Reproducción y eliminación**: ocurre igual en hub USB, puerto directo del PC
y PSU de 30 puertos (es propiedad del perfil de tráfico, no de la fuente).
Eliminado con bursts cortos: block 64 = ~1.2 ms TX/fragmento → cero eventos
en 8+ horas.

## Mecanismo Tipo 2 — colapso transitorio del LDO del SuperMini (raro)

**Firma** (fig_tipo2_oscilacion_ldo.png, fig_tipo2_zoom.png): el riel 3.3 V
oscila entre 1.8 y 3.1 V durante ~40 ms **con VBUS perfectamente estable**
— hiccup del límite de corriente del LDO clase AMS1117 ante un spike
transitorio (radio PA + CPU + flash simultáneos). El chip cruza el BOD real,
entra en boot-loop, y el inrush repetido de los reboots termina disparando
también el corte del host ~40 ms después.

**Tasa**: ~0.6/h en SuperMini con block 256; no observado en Seeed XIAO ni
ESP32-C6-WROOM (mejor sección de potencia). Mitigación HW candidata:
condensador 100-470 µF low-ESR en el **riel 3.3 V** (salida del LDO). Nota:
un cap de 100 µF en VBUS demostró empíricamente NO servir — el riel que
colapsa está detrás del LDO.

## Tabla de evidencia

| Configuración | Tasa de eventos | Método de medición |
|---|---|---|
| FW completo, block 512 | ~10/h | AD2 + contadores NVS |
| FW completo, block 64 | ~2.4/h | AD2 |
| FW minimal, block 64 | **0 en 8+ h** | AD2 + TB re-REGISTERs |
| FW minimal, block 256 | ~0.6-0.85/h | AD2 (4.3 h, 3 eventos: 2×Tipo2, 1×Tipo1) |

## Implicación central

El tamaño de bloque CoAP es una **palanca eléctrica**, no solo de red: gobierna
la duración del burst de TX y por tanto la corriente integrada que ve la
protección del host. Este acoplamiento protocolo↔alimentación no está
documentado en la literatura de LwM2M/Thread consultada.
