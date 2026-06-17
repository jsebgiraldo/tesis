# Gotchas operacionales del despliegue (Gateway + Server + tooling)

Catálogo de modos de falla no-obvios encontrados operando la flota real.
Material para Cap 4 (Gateway), Cap 5 (Server) y anexos de operación.

## OTBR (Raspberry Pi 4 border router)

1. **SRP lease expiry — causa raíz de los episodios fleet-wide recurrentes**
   (resuelto 2026-06-11): el init `otbr-srp` es *oneshot* — publica los
   services del Edge una vez al boot y termina; el lease SRP es de 7200 s.
   Sin renovación, `ThingsBoard-Edge._lwm2m._udp` **expira silenciosamente
   cada 2 h** tras el último publish. Los nodos con dirección cacheada en NVS
   sobreviven; cualquier nodo que cicle después no redescubre el Edge por
   DNS-SD → tormentas de reintento → las ráfagas episódicas de re-REGISTERs
   observadas (04:30, 09:30). La mesh permanece sana (partición única) — el
   diagnóstico engañoso clásico. Fix permanente: renovación por cron cada
   30 min (`*/30 * * * * /usr/sbin/otbr-srp-publish`), 4× margen sobre el
   lease. Lección de diseño: **todo registro con lease necesita un renovador
   supervisado**, no un publish one-shot.
2. **`infra_if_name` default (eth0) deja el BR `stopped`**: las IPs de wpan0 se
   deprecan, el Edge responde con src=br-lan y los nodos descartan por
   mismatch src/dst de CoAP. Fix UCI:
   `uci set otbr-agent.service.infra_if_name='br-lan'`.
3. **Renumeración USB del RCP** (/dev/ttyACM0→ACM1) crash-loopea el otbr-agent.
   Clave UCI correcta: `otbr-agent.service.uart_device`. Tras restart:
   `ot-ctl ifconfig up && thread start && srp server enable`.

## ThingsBoard Edge

4. **El flag `active` flapea con tráfico sparse**: inactivity-timeout default
   (10 min) < periodo de contacto natural de un SED en queue mode (~5-10 min).
   Un dashboard con "Inactive" NO implica nodo caído — verificar
   `lastActivityTime`. Fix: atributo SERVER_SCOPE `inactivityTimeoutMs` por
   device (20 min usado).
5. **Devices creados Edge-local se purgan en re-syncs con el Cloud**: el nodo
   sigue corriendo pero su REGISTER es rechazado silenciosamente. Provisionar
   vía Cloud o re-provisionar tras cada re-sync.
6. **REG_UPDATE no refresca lastActivityTime** (versión usada): con lifetime
   largo el device aparece inactivo y los RPC devuelven 504 — combinar
   lifetime=86400 con updates tempranos.
7. **Método RPC de escritura es `WriteReplace`**, no `Write` (devuelve
   METHOD_NOT_ALLOWED). El OTA por RPC usa ONEWAY WriteReplace de /5/0/0 (hex)
   + Execute /5/0/2.

## Nodos / USB / tooling

8. **El reset-cause BRN es ambiguo** (corte externo vs BOD interno) — ver doc 01.
9. **USB-Serial-JTAG del ESP32-C6 se wedgea** tras ciclos de alimentación
   repetidos (LIBUSB_ERROR_PIPE); solo replug físico lo recupera. Implicación
   de diseño: JTAG no es canal de recovery de campo; el recovery debe ser
   autónomo (watchdogs + OTA).
10. **OpenOCD compara el adapter serial case-sensitive** y el ESP32-C6 reporta
    el MAC en mayúsculas — los scripts deben normalizar `.upper()`.
11. **Cables USB marginales imitan defectos de hardware** (panic SW inmediato
    post-flash que desaparece al cambiar cable) — cambiar cable antes de
    etiquetar un board como defectuoso.
12. **MACs base de Espressif van en bloques de 4** (último byte siempre par):
    cualquier partición experimental por paridad de MAC da grupos sesgados —
    usar índice sobre la lista ordenada.
