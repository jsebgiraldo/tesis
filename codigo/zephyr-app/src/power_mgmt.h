/*
 * power_mgmt.h — Power management for ESP32-C6
 */

#ifndef POWER_MGMT_H_
#define POWER_MGMT_H_

#include <stdint.h>

/**
 * @brief Dormir el sistema por un período determinado
 * 
 * Utiliza el subsistema PM de Zephyr para entrar en el estado
 * de bajo consumo más profundo posible mientras mantiene
 * la red Thread en modo SED (Sleepy End Device) o CSL receiver.
 * 
 * @param duration_ms Duración del sleep en milisegundos
 */
void power_mgmt_sleep(uint32_t duration_ms);

/**
 * @brief Obtener consumo estimado actual (mA)
 * @return Corriente estimada en mA basada en estado actual
 */
float power_mgmt_get_current_ma(void);

#endif /* POWER_MGMT_H_ */
