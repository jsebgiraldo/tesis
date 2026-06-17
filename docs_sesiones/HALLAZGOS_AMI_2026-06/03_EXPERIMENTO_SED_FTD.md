# Experimento A/B a escala: SED vs FTD en SuperMini (30 nodos, 12 h)

## Pregunta

¿El hardware de bajo costo (SuperMini, LDO marginal) tolera el rol FTD de
Thread (radio RX siempre encendida + deberes de router), o debe limitarse a
SED (radio dormida ~99% del tiempo)?

## Diseño

- 30 SuperMini, firmware idéntico salvo el rol: 15 SED / 15 FTD
  (minimal AMI, CoAP block 256, v0.7.0-exp256)
- Asignación determinística y reproducible: índice par/impar sobre la lista
  ordenada de MACs del fleet_map (interleaving que neutraliza correlación de
  lote de fabricación)
- 2 PSUs USB de 30 puertos, 12 h overnight
- Métrica de ciclo: re-REGISTERs LwM2M en transportLog de TB (un nodo que se
  resetea debe re-registrar; uno estable solo envía UPDATEs)

## Resultados

| Métrica (12 h) | SED-256 | FTD-256 |
|---|---|---|
| Re-REGISTERs totales | 52 | 50 |
| Media por board | 3.5 | 3.3 |
| Boards con contacto ≤9 min al corte | 15/15 | 15/15 |

**SED ≡ FTD.** El RX continuo del FTD (~25-30 mA DC) no penaliza: es carga
sostenida que el LDO maneja; lo que importa es el perfil de bursts de TX, y
con minimal AMI ambos roles transmiten poco.

## Estructura temporal de los ciclos

Los re-REGISTERs NO se distribuyen uniformemente: se agrupan en ráfagas
(ola de deploy 22:40-23:50; episodios 04:30-05:30 y 09:30-10:30) separadas por
ventanas de silencio absoluto fleet-wide de 1.4 h y 3.7 h.

**Interpretación**: el ciclado residual es **episódico y mesh-wide**, no fallas
independientes por board. Esto redirige la optimización: atacar los
disparadores de episodio (server-side), no los boards individuales.

**Causa raíz de los episodios (confirmada al día siguiente)**: expiración del
lease SRP del servicio del Edge (publish one-shot + lease 2 h, ver doc 06 §1).
Los nodos que ciclaban durante una ventana de lease expirado no podían
redescubrir el servidor por DNS-SD y entraban en tormentas de reintento.
Verificación causal adicional: reiniciar 3 routers por RPC durante una ventana
expirada reprodujo el episodio (children huérfanos + sin re-discovery posible
→ caída del delivery-ratio de 83% a 43%); restaurar el SRP + renovación por
cron lo resolvió.

## Implicaciones de diseño

1. La topología es libre: cualquier board puede ser router → simplifica
   logística de despliegue (no hay "boards especiales").
2. Para una flota 30-60 nodos: 5-6 FTD bastan (≈6-10 children/router, OT
   soporta 32); más routers = más churn MLE sin beneficio a esta densidad.
3. El A/B a escala con asignación determinística por MAC es barato (un flag
   del script de flasheo) y debería ser práctica estándar antes de fijar
   parámetros de flota.
