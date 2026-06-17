# Método: bisección por escalera de subsistemas

Protocolo reproducible para atribuir una falla eléctrica/de sistema a un
subsistema concreto del stack (HW → radio → mesh → aplicación), manteniendo
la instrumentación constante (mismo board, mismas probes AD2) y variando solo
el firmware. Cada escalón agrega exactamente un subsistema; el primer escalón
que reproduce la falla la nombra.

| # | Firmware | Subsistema agregado | Resultado (10 min AD2) |
|---|----------|--------------------|------------------------|
| 1 | Zephyr `hello_world` | CPU + boot | limpio |
| 2 | OT shell (default) | radio init + calibración RF | limpio |
| 3 | OT shell + auto-attach con credenciales falsas | TX continuo (MLE Parent Request scan) | limpio |
| 4 | OT shell + credenciales reales | mesh attach + child polls | limpio |
| 5 | FW completo con `AMI_BRN_TEST_NO_LWM2M=y` | toda la app salvo LwM2M | limpio |
| 6 | FW completo | cliente LwM2M (REGISTER + observes) | **falla** |

Conclusión del caso: Thread/802.15.4 exonerado en 4 escalones; el detonante
era el perfil de tráfico de la capa LwM2M. Tiempo total: ~2 h.

## Notas de implementación

- Escalones 2-4 son los samples de Zephyr (`samples/net/openthread/shell`) con
  overlays de Kconfig (`OPENTHREAD_MANUAL_START=n` + PANID/NETWORKKEY/CHANNEL).
- Escalón 5 requiere una compuerta de diagnóstico en el firmware propio
  (`CONFIG_AMI_BRN_TEST_NO_LWM2M`: omite `lwm2m_rd_client_start()` y los
  snapshots NVS periódicos, conserva Thread/watchdogs/timers).
- La gradualidad importa: si se hubiera saltado de 1 a 6, la conclusión habría
  sido "el firmware completo falla" sin atribución.

## Generalización

El patrón aplica a cualquier stack embebido multicapa: definir la escalera
mínima donde cada peldaño agrega un subsistema con consumo/comportamiento
distinguible, fijar la observación (instrumentos y board), y subir hasta
reproducir. Es O(capas) en vez de O(2^features) de un barrido de flags.
