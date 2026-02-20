/*
 * power_mgmt.c — Power management using Zephyr PM subsystem
 *
 * En la implementación actual, usamos k_msleep() como base.
 * Para producción, esto se extendería con:
 * - Light sleep (mantiene RAM, radio off)
 * - Deep sleep (solo RTC, wake por timer o GPIO)
 * - Thread SED polling interval coordination
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/pm/pm.h>
#include <zephyr/pm/device.h>
#include <zephyr/pm/policy.h>

#include "power_mgmt.h"

LOG_MODULE_REGISTER(power, LOG_LEVEL_INF);

void power_mgmt_sleep(uint32_t duration_ms)
{
	LOG_DBG("Sleeping for %u ms", duration_ms);

	/*
	 * Zephyr PM subsystem automáticamente selecciona el estado
	 * de bajo consumo más profundo permitido basándose en:
	 * - Duración del sleep solicitada
	 * - Latencia máxima permitida
	 * - Periféricos activos (UART, radio)
	 *
	 * Para ESP32-C6:
	 * - PM_STATE_RUNTIME_IDLE: ~7 mA (CPU idle, radio standby)
	 * - PM_STATE_SUSPEND_TO_IDLE: ~0.8 mA (light sleep, radio off)
	 * - PM_STATE_SUSPEND_TO_RAM: ~7 µA (deep sleep, solo RTC)
	 *
	 * Con Thread FTD activo, no podemos hacer deep sleep
	 * sin perder la conexión mesh. Usamos light sleep
	 * que permite wake rápido para recibir mensajes Thread.
	 */

	/* Por ahora, simple kernel sleep que permite PM automático */
	k_msleep(duration_ms);
}

float power_mgmt_get_current_ma(void)
{
	/*
	 * Estimación basada en datasheet ESP32-C6:
	 * - Active (CPU + radio TX): 22 mA
	 * - Active (CPU + radio RX): 19 mA
	 * - Modem sleep: 5 mA
	 * - Light sleep: 0.8 mA
	 * - Deep sleep: 0.007 mA
	 *
	 * TODO: Medir con INA219 para valores reales
	 */
	return 1.0f; /* Promedio estimado con duty cycle 5% */
}
