/*
 * meter_dlms.h — DLMS/COSEM meter parser over RS-485/UART
 */

#ifndef METER_DLMS_H_
#define METER_DLMS_H_

#include "lwm2m_client.h"  /* struct meter_reading */

/**
 * @brief Inicializar interfaz UART para comunicación DLMS
 * @return 0 en éxito
 */
int meter_dlms_init(void);

/**
 * @brief Leer medidor vía protocolo DLMS/COSEM
 * 
 * Lee códigos OBIS estándar:
 * - 1.8.0: Energía activa importada (kWh)
 * - 2.8.0: Energía activa exportada (kWh)
 * - 31.7.0: Corriente fase L1 (A)
 * - 32.7.0: Tensión fase L1 (V)
 * 
 * @param reading Estructura donde almacenar la lectura
 * @return 0 en éxito, -ETIMEDOUT si no responde, -EIO en error
 */
int meter_dlms_read(struct meter_reading *reading);

#endif /* METER_DLMS_H_ */
