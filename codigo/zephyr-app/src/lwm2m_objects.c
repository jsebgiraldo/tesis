/*
 * lwm2m_objects.c — Definición Objects IPSO custom para AMI
 *
 * Este archivo extiende los objects estándar con resources
 * específicos para medición de energía eléctrica.
 * Los objects estándar (3, 4) son manejados por el subsistema
 * LwM2M de Zephyr automáticamente.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/lwm2m.h>

LOG_MODULE_REGISTER(lwm2m_obj, LOG_LEVEL_DBG);

/*
 * LwM2M Objects utilizados en la tesis:
 *
 * +--------+--------------------------+----------------------------------+
 * | ObjID  | Nombre                   | Uso en AMI                       |
 * +--------+--------------------------+----------------------------------+
 * | 3      | Device                   | Info HW: manufacturer, model,    |
 * |        |                          | firmware version, serial number  |
 * +--------+--------------------------+----------------------------------+
 * | 4      | Connectivity Monitoring  | RSSI, link quality, IP address,  |
 * |        |                          | network bearer (Thread)          |
 * +--------+--------------------------+----------------------------------+
 * | 3200   | Digital Input            | Contador pulsos medidor,         |
 * |        |                          | tamper detection flag             |
 * +--------+--------------------------+----------------------------------+
 * | 3202   | Analog Input             | Inst 0: Voltage (V)              |
 * |        |                          | Inst 1: Current (A)              |
 * +--------+--------------------------+----------------------------------+
 * | 3305   | Power Measurement        | Active power, power factor,      |
 * |        |                          | cumulative energy                |
 * +--------+--------------------------+----------------------------------+
 *
 * Mapeado OBIS → LwM2M:
 * 1.8.0  (Energy imported)  → 3305/0/5805 (Cumulative active power)
 * 2.8.0  (Energy exported)  → 3305/0/5805 (instance separada)
 * 31.7.0 (Current L1)       → 3202/1/5600 (Analog Input value)
 * 32.7.0 (Voltage L1)       → 3202/0/5600 (Analog Input value)
 * 13.7.0 (Power Factor)     → 3305/0/5820 (Power Factor)
 */

/* Este archivo se expandirá con custom objects si los estándar
 * IPSO no cubren todos los recursos necesarios.
 * Por ahora, la configuración se hace en lwm2m_client.c usando
 * los objects built-in de Zephyr.
 */
