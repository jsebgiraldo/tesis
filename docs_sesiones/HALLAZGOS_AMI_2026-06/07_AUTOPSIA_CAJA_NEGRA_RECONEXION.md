# Autopsia de caja negra: el disconnect era un bug de lógica invertida (2026-06-15)

**Capítulo destino:** Cap 6 Resultados (cierre del diagnóstico) + Cap 7 Discusión
(la lección de método). Cierra el lazo entre "los boards se desconectan" y *por
qué*, con evidencia on-target **y** del código fuente, no inferencia.

## Qué se buscaba

Tras descartar el corte USB por sobrecorriente (doc 01) y validar el perfil
mínimo (doc 04), quedaba la pregunta del director: **¿hay un problema de recursos
del firmware (fuga de heap, agotamiento de stack, starvation) que genere los
disconnects?** Sin emulador viable (QEMU no modela la radio 802.15.4 del
ESP32-C6), se instrumentó **un** board con un build `resprobe`: Objeto custom
33000 con `heap_min_free_live` en vivo + caja negra `post_mortem` en NVS que, al
detectar un cuelgue, guarda un snapshot (uptime, heap, edad de registración,
estado LwM2M, rol Thread) legible al boot siguiente vía RPC. Los otros dos
SuperMini corrían el build de producción `send` (AMI mínimo, sin Objeto 33000).

## El evento — determinístico

En un soak pasivo (lectura RPC cada 2 min, **cero intervención física**), el
board instrumentado (`1494`) se reseteó **cuatro veces, idéntico**: uptime ≈706 s,
luego boot a ~46 s, en bucle de **≈11.75 min**. Los otros dos boards mantuvieron
telemetría fresca todo el tiempo → no era caída de Edge fleet-wide.

### Caja negra (idéntica en los 4 resets)

| RID | Campo | Valor | Lectura |
|-----|-------|-------|---------|
| 21 | reset_reason | **2 = RESET_SOFTWARE** | `sys_reboot` auto-infligido; NO brownout, NO watchdog HW |
| 27 | lwm2m_state | **0x08 = RECOVERING** | `recover_work_fn` corriendo |
| 28 | thread_role | **0 = DISABLED** | stack Thread caído (el recover lo cicla) |
| 26 | reg_age_s | **660** | *red herring* — ver abajo |
| 24/25 | heap_free / heap_min | **65 176 B / 65 176 B** | heap **100 % libre, fuga cero** |
| 17 | lwm2m_last_err | **0** | sin errno de rd_client |

## La causa raíz — bug de lógica invertida en el keepalive

El `reg_age = 660 s` parecía una registración vencida, pero es un señuelo:
`660 = uptime(706) − boot(46) = 3 × keepalive_period(300) − grace(60)`. El
disparador real está en `src/coap_keepalive.c`, que cada 300 s ejecuta:

```c
int ret = lwm2m_notify_observer(/33000/0/uptime_s);
if (ret == 0) { feed_watchdog(); consec_fail = 0; }    // creyó: 0 = "queued OK"
else          { consec_fail++; if (>=3) force_recover_reboot(); }
```

Pero en Zephyr (`subsys/net/lib/lwm2m/lwm2m_observation.c`,
`lwm2m_notify_observer_path`), el retorno es el **número de observers
notificados** (`ret++` por cada uno): **0 = nadie observa, N>0 = éxito (salió un
CoAP CON real), <0 = error real del engine**. La condición está **al revés**:
`ret>0` —el caso de éxito— se cuenta como falla. A las 3 "fallas" consecutivas
→ `recover_work` → **COLD `sys_reboot`**. Período exacto: 60 s de gracia + 3×300 s
≈ 706 s.

### Consecuencia perversa

**Los boards correctamente monitoreados (TB observando `/33000/0/uptime_s`)
rebootean cada ~12 min; los NO-monitoreados sobreviven.** Es lo opuesto a la
intuición, y por eso el bug fue invisible durante 14 iteraciones.

### Prueba triple

1. **Fuente**: `ret = #observers` (0 ó N), no un status `0=OK`.
2. **Forense**: 4 resets idénticos, heap 100 % libre en cada cuelgue.
3. **Empírico**: agregar un observe a `/33000/0/uptime_s` en 1494 **no ayudó** —
   `consec_fail` siguió trepando 1→2 (los observers son exactamente lo que el bug
   malcuenta). En contraste, los builds minimal (`ret=0`) nunca rebootean.

## Conclusiones (hechos)

1. **El firmware NO tiene problema de recursos.** Heap 100 % libre en el instante
   exacto de cada reset; sin errno, sin watchdog HW, sin brownout. La rama
   "recursos de firmware" del árbol de diagnóstico queda **descartada con
   evidencia on-target**. (QEMU se descartó honestamente; la caja negra en NVS
   fue el sustituto válido.)
2. **El disconnect era un bug de software de una línea**, no un fenómeno físico
   ni de mesh. Fix `v0.7.3-kafix`: `if (ret >= 0)` alimenta el watchdog (engine
   vivo con 0 o N observers); solo `ret < 0` cuenta como falla real. Preserva la
   detección de socket muerto y deja de rebootear boards sanos.

## Alcance honesto

- **Flota de producción (builds minimal, sin Objeto 33000)**: `ret=0` siempre →
  **esquivó** el loop. Reverso: ahí el keepalive es un **placebo** (siempre
  "éxito", nunca prueba el socket). El cycling residual de la flota minimal
  (doc 03, 52 ciclos/12 h) es **otra cosa**, no este bug.
- **Arquitectura "fat-firmware / thin-profile"** (la preferida en la tesis):
  queda **directamente comprometida** — un FW con todas las features (Objeto
  33000) + el server observando `uptime_s` = loop de 12 min garantizado. Este
  hallazgo es la razón técnica para o bien corregir el keepalive (hecho) o bien
  reconsiderar qué se observa.
- **Retroactivo**: muchos de los soaks de diagnóstico de las 14 iteraciones
  corrían builds con Objeto 33000 + observes → este loop **probablemente
  contaminó esos experimentos**, montándose sobre causas reales (brownout, SRP)
  y prolongando la búsqueda.

## Lección de método (Cap 7)

Un retorno de API mal interpretado (conteo-de-observers leído como status-OK)
convirtió la **red de seguridad** (el keepalive de recuperación) en la **causa**
del fallo que debía prevenir. La instrumentación on-target con caja negra en NVS
—uptime/heap/estado en el instante del cuelgue, legible por RPC sin serial— fue
lo que permitió pasar de "se desconecta" a la línea exacta de código en una
sesión. Es el argumento a favor de diseñar observabilidad de post-mortem desde el
firmware, no de depender de logs serial que el hardware de campo no expone.

---
*Evidencia: `tools/autopsy_1494_rpc.py` (RIDs 17/21/23–28 + keepalive 29/30),
`tools/resource_soak_watch.py`, `logs/keepalive_invert_confirm.log`. Fix en
`src/coap_keepalive.c` (v0.7.3-kafix). Snapshot original: soak `bq5717402`,
ticks t004/t010/t016/t022.*
