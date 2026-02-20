/*
 * meter_modbus.h — Modbus RTU meter reader
 */

#ifndef METER_MODBUS_H_
#define METER_MODBUS_H_

#include "lwm2m_client.h"

/**
 * @brief Inicializar interfaz Modbus RTU
 * @return 0 en éxito
 */
int meter_modbus_init(void);

/**
 * @brief Leer medidor vía Modbus RTU (función 0x03 Read Holding Registers)
 * 
 * @param reading Estructura donde almacenar la lectura
 * @return 0 en éxito, código de error negativo en fallo
 */
int meter_modbus_read(struct meter_reading *reading);

#endif /* METER_MODBUS_H_ */
