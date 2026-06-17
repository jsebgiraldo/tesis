# Hallazgos AMI — investigación fleet ESP32-C6 + LwM2M (2026-06-08 → 2026-06-11)

Paquete de hallazgos para integrar a la tesis. Cada documento es autocontenido
y mapea a un capítulo destino. Evidencia cruda en el repo de firmware
(`Unal-Flash-tool/firmware/ami-lwm2m-node/logs/`).

| Doc | Contenido | Capítulo destino |
|-----|-----------|------------------|
| `01_ROOT_CAUSE_DUAL_MECANISMO.md` | Los dos mecanismos de reset (corte USB por sobrecorriente + colapso LDO) con evidencia AD2 | Cap 6 Resultados |
| `02_METODO_BISECCION.md` | Protocolo de aislamiento por escalera de subsistemas (reproducible) | Cap 6 / metodología |
| `03_EXPERIMENTO_SED_FTD.md` | A/B 30 nodos: equivalencia SED≡FTD en SuperMini | Cap 6 Resultados |
| `04_GOBERNANZA_DATOS_SERVER.md` | pmax como SLA, observe profile, perfil mínimo facturación+calidad | Cap 5 Server |
| `05_PERILLAS_KNOB_MAP.md` | El espacio de diseño: observe paths × CoAP block vs capacidad física | Cap 6/7 — **el claim central empírico** |
| `06_GOTCHAS_OPERACIONALES.md` | OTBR (SRP, infra_if, ACM), TB Edge (purge, flags), USB-JTAG | Cap 4 Gateway / anexos |
| `07_AUTOPSIA_CAJA_NEGRA_RECONEXION.md` | Caja negra on-target: BUG #1 = lógica invertida en el keepalive (conteo-de-observers leído como status-OK → boards monitoreados rebootean cada 12 min). Heap 100% libre descarta fuga; fix v0.7.3-kafix | Cap 6 Resultados / Cap 7 Discusión |
| `08_BUG2_HW_LIVENESS_STARVATION.md` | BUG #2 (apilado): HW watchdog de liveness inanido por lifetime=86400 (REG_UPDATE cada 19h vs timeout 600s) → reboot ~700s. Cazado con auto-documentación de reboots (RID 37). Fix config-only UPDATE_PERIOD=300. Patrón "lifetime largo inane watchdogs" | Cap 6 Resultados / Cap 7 Discusión |
| `figuras/` | PNG 200dpi listos para LaTeX + nota de regeneración | Cap 6 |

## Estado del sistema al cierre (2026-06-11)

- 30 SuperMini en 2 PSUs corriendo v0.7.0-exp256 (15 SED + 15 FTD, minimal AMI, block 256)
- 2 routers bench (XIAO + WROOM) en v0.7.0-minimal block 64
- Perfil TB: 5 telemetrías (V, P, FP, kWh, Hz) con pmin=15/pmax=60 + 4 attrs on-change
- Medición SLA delivery-ratio 24 h en curso (`logs/delivery_ratio.csv`)
- Celda experimental pendiente: FW completo + block 64 + observe 5 (valida fat-firmware/thin-profile)
