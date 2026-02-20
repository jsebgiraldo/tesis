# Claim central (tesis)

Un **claim central** es la afirmación principal que la tesis quiere demostrar con evidencia.

## Propuesta (borrador — ajustar)

> Una arquitectura jerárquica de 4 niveles para telemetría AMI/Smart Energy (Thread/LwM2M + DLMS, backhaul HaLow, gateways edge, y plataforma central ThingsBoard+Kafka) permite escalar hasta **~500 dispositivos** distribuidos en **múltiples gateways edge**, manteniendo **latencia** adecuada para modos de muestreo de **10 min / 1 min / 1 s**, con **confiabilidad** y **seguridad** transversales, validado mediante mediciones end‑to‑end (tasa de mensajería, latencia, pérdida, disponibilidad) y dimensionamiento de gateways.

## Qué debe cubrir la evidencia

- **Escalabilidad:** devices, gateways, throughput (msgs/s), almacenamiento, reglas/analítica.
- **Latencia:** normal (10 min) vs ráfagas/alta frecuencia (1 min / 1 s).
- **Confiabilidad:** pérdida, reintentos, colas, backpressure, disponibilidad.
- **Costo:** CAPEX/OPEX por gateway / nube.
- **Seguridad:** identidad, cifrado, segmentación, hardening.

## Cómo se “testea” un claim

Para cada sub-claim (escalabilidad/latencia/…) debe haber al menos una de:
- experimento/medición
- comparación con baseline
- modelo/estimación (dimensionamiento) validado con datos
- referencia normativa/estado del arte + argumento técnico
