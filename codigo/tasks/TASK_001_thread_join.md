# TASK_001: OpenThread FTD Join Network

- **Asignado a:** Servidor 100.67.60.126
- **Prioridad:** Alta  
- **Estado:** Pendiente
- **Branch:** `feat/thread-join`

## Requisito

El ESP32-C6 debe unirse a una red Thread como FTD (Full Thread Device) 
usando el dataset estático configurado en `prj.conf`. Debe ser capaz 
de obtener rol CHILD o ROUTER en la red.

## Dependencias

- Zephyr SDK instalado (ejecutar `setup_zephyr_server.sh`)
- nRF52840 Dongle como RCP para OTBR (o segundo ESP32 como OT CLI)
- OTBR en Docker funcionando

## Pasos

1. Compilar: `west build -b esp32c6_devkitc/esp32c6 app`
2. Flash: `west flash`
3. Verificar log serial: debe mostrar "Thread attached as ..."
4. Desde OTBR CLI: `ping6 <ipv6-del-nodo>`
5. Capturar logs y métricas

## Criterio de aceptación

- [ ] `west build` compila sin errores ni warnings críticos
- [ ] Log muestra "Thread attached as Router" o "Thread attached as Child"
- [ ] IPv6 ping desde OTBR al nodo responde en <50 ms
- [ ] RSSI reportado en log (>-90 dBm indoor)
- [ ] RAM usage < 200 KB, Flash < 800 KB

## Resultado

_(Completar al terminar)_

```
Adjuntar:
- Build output (RAM/Flash usage summary)
- Log serial primeros 60 segundos
- ping6 resultado
- Screenshot OTBR web UI mostrando el nodo
```
