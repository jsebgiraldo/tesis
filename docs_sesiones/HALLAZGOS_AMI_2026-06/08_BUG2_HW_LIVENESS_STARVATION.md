# BUG #2: inanición del HW watchdog de liveness + el método de auto-documentación de reboots (2026-06-15)

**Capítulo destino:** Cap 6 Resultados (segundo bug apilado) + Cap 7 Discusión
(el patrón "lifetime largo inane watchdogs" y el método de instrumentación).

## El contexto: bugs apilados

Tras arreglar el BUG #1 (lógica invertida del keepalive, doc 07), los boards
instrumentados **seguían reiniciándose cada ~600-700 s**. Las firmas:
`reset_reason=2` (SOFTWARE), `keepalive_consec_fail=0` (no era el keepalive),
`recover_count` y `watchdog_count` **planos** (no era el ciclo de recovery ni
el silence-watchdog). Un segundo mecanismo, con timing casi idéntico, estaba
montado debajo del primero — por eso el #1 parecía "la" causa.

## El método: auto-documentación del reboot (RID 37)

El problema de fondo: estos boards **no tienen consola serial** (modo WinUSB
para JTAG), así que no se puede ver *por qué* se reinician. Intentar RTT (logs
en vivo sobre JTAG) chocó con que el SoC ESP32-C6 no declara `HAS_SEGGER_RTT`
y forzarlo rompe el build.

**Solución instrumental:** se agregó un código de causa de reboot que sobrevive
el reset, expuesto como **Objeto 33000 RID 37 (`last_reboot_code`)**. El helper
`ami_reboot_drain()` —que envuelve *todos* los `sys_reboot()` del firmware— y el
panic handler estampan un código (1..11) antes de reiniciar; el boot siguiente
lo lee y lo publica. Once paths codificados (boot-watchdog, mesh-alone,
conn-monitor-wedged, max-recover-attempts, panic, etc.).

**El resultado fue el diagnóstico:** RID 37 leyó **0** en *todos* los resets.
Código 0 = "el reboot NO pasó por `ami_reboot_drain` ni por el panic handler".
Combinado con `reset_reason=2` (sí hubo un `sys_reboot` de software), eso señaló
el **único `sys_reboot()` directo del código fuera de ese helper**:
`src/hw_watchdog.c:154`. *La ausencia de código fue la pista.* Sin esta
instrumentación, el diagnóstico se quedaba entre dos candidatos
(conn-monitor-wedged vs panic) sin poder distinguirlos.

## La causa raíz

El **HW watchdog "real-liveness gate"** (`hw_wdog_kernel_thread`,
hw_watchdog.c:117-154) deja de alimentar su canal y hace
`sys_reboot(SYS_REBOOT_COLD)` directo si pasan
`CONFIG_AMI_REAL_LIVENESS_TIMEOUT_S` (600 s) sin un evento server-ACKed de
LwM2M. Ese "alimento" (`hw_watchdog_note_liveness()`) se dispara SOLO en
`REGISTRATION_COMPLETE` / `REG_UPDATE_COMPLETE`.

El conflicto: con `lifetime=86400` y `SECONDS_TO_UPDATE_EARLY=17280`, el motor
LwM2M manda el siguiente REG_UPDATE recién a las **~19 h**
(`lifetime - update_early = 69120 s`). Entonces, tras el registro inicial
(~uptime 45 s), nada alimenta el gate por 19 h → se inane → **cold reboot a
~645-700 s** (= 45 + 600), en bucle.

El gate fue añadido (v0.6.17) para cazar un nodo "registrado pero cortado"
(radio/USB colgado, CPU sana) — un objetivo legítimo. Pero el cambio a
`lifetime=86400` (v0.6.66, para habilitar RPC de TB Edge) lo dejó sin su única
fuente de alimento. **`hw_watchdog_init()` no está gateado por build**, así que
esto reinicia *todos* los builds con lifetime largo — muy probablemente el
"cycling crónico" de la flota desde v0.6.66, antes atribuido a mesh/brownout.

## El fix (config-only)

`CONFIG_LWM2M_UPDATE_PERIOD=300`. En Zephyr,
`next_update() = MIN(period, lifetime - early)`; con period=300 da
`MIN(300, 69120) = 300`, así que el motor emite un REG_UPDATE **server-ACKed
cada 300 s**, alimentando el HW liveness gate (timeout 600 s → 2× margen) **y**
el silence-watchdog, **sin bajar el lifetime** (el RPC de TB Edge sigue
funcionando — ver doc sobre lifetime↔RPC). Costo: un UPDATE CoAP chico por
board cada 5 min.

## Validación

Primera vez en toda la sesión que los 3 boards **cruzaron los 706 s**: corrieron
20+ min (uptime ~1200 s) con `total_resets` **congelado** (1494=334, f7b4=287,
fbb8=143) y RID 37 = 0. Antes de Fix A: reboot cada ~700 s como relojería. Soak
largo en curso para confirmar estabilidad de horas.

## Auditoría del patrón (Cap 7)

El mismo patrón —"un watchdog/mecanismo gateado por un evento que el lifetime
largo inane"— causó tanto el problema del silence-watchdog (mitigado en v0.6.67
con el keepalive) como este. Auditados *todos* los mecanismos con timeout:

| Watchdog | Timeout | Alimentado por | Con Fix A |
|----------|---------|----------------|-----------|
| HW real-liveness | 600 s | REG/REG_UPDATE | 300 s → 2× ✓ |
| Silence | 900 s | REG/REG_UPDATE + keepalive | 300 s → 3× ✓ |
| HW canales (kernel/workq) | 300 s | threads feeder (no REG) | 100 s ✓ |
| conn-monitor | 300 s gap | tick del loop (no REG) | ✓ |
| mesh-alone | 3600 s | attachment (no REG) | ✓ |

**Conclusión:** no hay un tercer bug de inanición. **Lección de diseño:** al
alargar el lifetime de LwM2M hay que auditar TODOS los watchdogs que dependen de
eventos de registro — o desacoplarlos con un latido independiente (keepalive /
UPDATE periódico). El RID 37 (auto-documentación de reboots) convierte futuras
regresiones de este tipo en un diagnóstico de un solo paso.

---
*Evidencia: `tools/rbtag_watch.py` (captura RID 37), `tools/overnight_soak.py`
(soak de validación). Fix en `prj.conf` (`CONFIG_LWM2M_UPDATE_PERIOD=300`),
instrumentación en `src/main.c` + `src/thread_conn_monitor.c` +
`src/lwm2m_obj_thread_diag.h` (RID 37). Firmware v0.7.5-live.*
