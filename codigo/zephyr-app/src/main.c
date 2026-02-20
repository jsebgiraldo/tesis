/*
 * AMI Thread + LwM2M Node — Main Entry Point
 * 
 * Sistema de medición inteligente sobre ESP32-C6 con Zephyr RTOS.
 * Arquitectura: Thread mesh → CoAP/LwM2M → Border Router → Gateway Edge
 * 
 * Flujo principal:
 * 1. Inicializar hardware (UART, GPIO, LED)
 * 2. Unirse a red Thread (OpenThread FTD)
 * 3. Registrar cliente LwM2M con servidor (Leshan/ThingsBoard)
 * 4. Loop: leer medidor → actualizar objects LwM2M → esperar intervalo
 */

#include <zephyr/kernel.h>
#include <zephyr/version.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/openthread.h>
#include <openthread.h>  /* new module-level API (v4.2.0) */

#include "thread_network.h"
#include "lwm2m_client.h"
#include "meter_dlms.h"
#include "meter_modbus.h"
#include "fota.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

/* Intervalo de lectura del medidor (ms) */
#define METER_READ_INTERVAL_MS  (15 * 60 * 1000)  /* 15 minutos */

/* LED de estado */
static const struct gpio_dt_spec led_status =
	GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);

/* Tamper detection */
static const struct gpio_dt_spec tamper_sw =
	GPIO_DT_SPEC_GET(DT_NODELABEL(tamper_sw), gpios);

static struct gpio_callback tamper_cb_data;

/* --- Tamper ISR --- */
static void tamper_isr(const struct device *dev, struct gpio_callback *cb,
		       uint32_t pins)
{
	LOG_WRN("TAMPER DETECTED — sending critical alarm");
	/* TODO: Notificar vía LwM2M alarma CRITICAL */
	lwm2m_notify_tamper_alarm();
}

/* --- Estado del sistema --- */
typedef enum {
	STATE_INIT,
	STATE_THREAD_JOINING,
	STATE_THREAD_CONNECTED,
	STATE_LWM2M_REGISTERING,
	STATE_OPERATIONAL,
	STATE_ERROR,
} system_state_t;

static volatile system_state_t current_state = STATE_INIT;

/* --- Callback Thread state change --- */
static void on_thread_state_changed(otChangedFlags flags, void *context)
{
	ARG_UNUSED(context);

	if (flags & OT_CHANGED_THREAD_ROLE) {
		otDeviceRole role = otThreadGetDeviceRole(
			openthread_get_default_instance());

		switch (role) {
		case OT_DEVICE_ROLE_CHILD:
		case OT_DEVICE_ROLE_ROUTER:
		case OT_DEVICE_ROLE_LEADER:
			LOG_INF("Thread attached as %s",
				otThreadDeviceRoleToString(role));
			current_state = STATE_THREAD_CONNECTED;
			break;
		case OT_DEVICE_ROLE_DETACHED:
		case OT_DEVICE_ROLE_DISABLED:
			LOG_WRN("Thread detached/disabled");
			current_state = STATE_THREAD_JOINING;
			break;
		}
	}
}

/* --- Main --- */
int main(void)
{
	int ret;

	LOG_INF("=== AMI Thread+LwM2M Node v%s ===", fota_get_current_version());
	LOG_INF("Board: XIAO ESP32-C6");
	LOG_INF("RTOS: Zephyr %s", KERNEL_VERSION_STRING);

	/* 1. Inicializar LED de estado */
	if (gpio_is_ready_dt(&led_status)) {
		gpio_pin_configure_dt(&led_status, GPIO_OUTPUT_ACTIVE);
		gpio_pin_set_dt(&led_status, 0);
	}

	/* 2. Inicializar tamper detection */
	if (gpio_is_ready_dt(&tamper_sw)) {
		gpio_pin_configure_dt(&tamper_sw, GPIO_INPUT);
		gpio_pin_interrupt_configure_dt(&tamper_sw,
						GPIO_INT_EDGE_TO_ACTIVE);
		gpio_init_callback(&tamper_cb_data, tamper_isr,
				   BIT(tamper_sw.pin));
		gpio_add_callback(tamper_sw.port, &tamper_cb_data);
		LOG_INF("Tamper detection enabled on GPIO%d", tamper_sw.pin);
	}

	/* 3. Inicializar subsistema de medidores (UART) */
	ret = meter_dlms_init();
	if (ret < 0) {
		LOG_WRN("DLMS meter init failed: %d (will retry)", ret);
	}

	ret = meter_modbus_init();
	if (ret < 0) {
		LOG_WRN("Modbus meter init failed: %d (will retry)", ret);
	}

	/* 4. Inicializar y unirse a red Thread */
	LOG_INF("Joining Thread network...");
	current_state = STATE_THREAD_JOINING;
	ret = thread_network_init(on_thread_state_changed);
	if (ret < 0) {
		LOG_ERR("Thread init failed: %d", ret);
		current_state = STATE_ERROR;
		return ret;
	}

	/* Esperar conexión Thread (timeout 60s) */
	int timeout_ms = 60000;
	while (current_state != STATE_THREAD_CONNECTED && timeout_ms > 0) {
		k_msleep(500);
		timeout_ms -= 500;
		gpio_pin_toggle_dt(&led_status); /* Blink while joining */
	}

	if (current_state != STATE_THREAD_CONNECTED) {
		LOG_ERR("Thread join timeout after 60s");
		current_state = STATE_ERROR;
		return -ETIMEDOUT;
	}

	gpio_pin_set_dt(&led_status, 1); /* LED ON = connected */

	/* 5. Registrar cliente LwM2M */
	LOG_INF("Registering LwM2M client...");
	current_state = STATE_LWM2M_REGISTERING;
	ret = lwm2m_client_init();
	if (ret < 0) {
		LOG_ERR("LwM2M client init failed: %d", ret);
		current_state = STATE_ERROR;
		return ret;
	}

	/* 6. Inicializar FOTA (OTA firmware update via LwM2M Object 5) */
	ret = fota_init(NULL);
	if (ret < 0) {
		LOG_WRN("FOTA init failed: %d (OTA disabled)", ret);
	} else {
		LOG_INF("FOTA: OTA enabled — fw v%s, confirmed=%d",
			fota_get_current_version(), fota_is_confirmed());
	}

	current_state = STATE_OPERATIONAL;
	LOG_INF("System OPERATIONAL — reading meter every %d min",
		METER_READ_INTERVAL_MS / 60000);

	/* 6. Loop principal: leer medidor → report */
	while (1) {
		struct meter_reading reading = {0};

		/* Leer medidor (intentar DLMS primero, fallback Modbus) */
		ret = meter_dlms_read(&reading);
		if (ret < 0) {
			ret = meter_modbus_read(&reading);
		}

		if (ret == 0) {
			LOG_INF("Meter: %.1f V, %.2f A, %.3f kWh, PF=%.2f",
				(double)reading.voltage_v,
				(double)reading.current_a,
				(double)reading.energy_kwh,
				(double)reading.power_factor);

			/* Actualizar objects LwM2M (triggers Observe notify) */
			lwm2m_update_meter_data(&reading);

			/* Verificar alarmas de voltaje */
			if (reading.voltage_v < 207.0f ||
			    reading.voltage_v > 253.0f) {
				LOG_WRN("Voltage alarm: %.1f V",
					(double)reading.voltage_v);
				lwm2m_notify_voltage_alarm(reading.voltage_v);
			}
		} else {
			LOG_WRN("Meter read failed: %d", ret);
		}

		/* Esperar hasta próxima lectura (nodo alimentado por medidor, sin sleep) */
		k_sleep(K_MSEC(METER_READ_INTERVAL_MS));
	}

	return 0;
}
