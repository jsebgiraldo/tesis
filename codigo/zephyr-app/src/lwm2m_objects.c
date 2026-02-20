/*
 * lwm2m_objects.c — Definición Objects IPSO para AMI
 *
 * Este archivo documenta los objects LwM2M utilizados.
 * La configuración real se hace en lwm2m_client.c usando
 * los objects built-in de Zephyr (IPSO).
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
 * | 3316   | Voltage Sensor (IPSO)    | Tensión L1 (V) — OBIS 32.7.0    |
 * |        |                          | Res 5700 = Sensor Value          |
 * +--------+--------------------------+----------------------------------+
 * | 3317   | Current Sensor (IPSO)    | Corriente L1 (A) — OBIS 31.7.0  |
 * |        |                          | Res 5700 = Sensor Value          |
 * +--------+--------------------------+----------------------------------+
 * | 3300/0 | Generic Sensor (Energy)  | kWh importada — OBIS 1.8.0      |
 * |        |                          | Res 5700 = Sensor Value          |
 * +--------+--------------------------+----------------------------------+
 * | 3300/1 | Generic Sensor (PF)      | Factor de potencia — OBIS 13.7.0|
 * |        |                          | Res 5700 = Sensor Value          |
 * +--------+--------------------------+----------------------------------+
 * | 5      | Firmware Update           | FOTA via MCUboot + LwM2M        |
 * +--------+--------------------------+----------------------------------+
 */
