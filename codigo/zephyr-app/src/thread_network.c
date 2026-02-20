/*
 * thread_network.c — OpenThread FTD initialization and management
 *
 * Configura ESP32-C6 como Full Thread Device (FTD) con:
 * - Dataset de red pre-configurado (PAN ID, Channel, Network Key)
 * - Rol FTD con capacidad router
 * - State change callback para notificar a main
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/openthread.h>
#include <openthread/instance.h>
#include <openthread/thread.h>
#include <openthread/dataset.h>
#include <openthread/ip6.h>
#include <openthread/link.h>

#include "thread_network.h"

LOG_MODULE_REGISTER(thread_net, LOG_LEVEL_INF);

static thread_state_cb_t user_state_cb;

/* State change handler registered with OpenThread */
static void ot_state_changed_handler(otChangedFlags flags,
				     void *context)
{
	struct openthread_context *ot_context = context;
	otInstance *instance = ot_context->instance;

	if (flags & OT_CHANGED_THREAD_ROLE) {
		otDeviceRole role = otThreadGetDeviceRole(instance);
		LOG_INF("Thread role changed: %s",
			otThreadDeviceRoleToString(role));
	}

	if (flags & OT_CHANGED_IP6_ADDRESS_ADDED) {
		LOG_INF("IPv6 address added");
	}

	/* Propagar al callback del usuario */
	if (user_state_cb) {
		user_state_cb(flags, context);
	}
}

int thread_network_init(thread_state_cb_t state_cb)
{
	struct openthread_context *ot_context;
	otInstance *instance;
	otError error;

	user_state_cb = state_cb;

	/* Obtener contexto OpenThread de Zephyr */
	ot_context = openthread_get_default_context();
	if (!ot_context) {
		LOG_ERR("Failed to get OpenThread context");
		return -ENODEV;
	}

	instance = ot_context->instance;

	openthread_api_mutex_lock(ot_context);

	/* Registrar callback de state change */
	otSetStateChangedCallback(instance, ot_state_changed_handler,
				  ot_context);

	/*
	 * El dataset se configura via Kconfig en prj.conf:
	 * CONFIG_OPENTHREAD_NETWORK_NAME, CONFIG_OPENTHREAD_CHANNEL,
	 * CONFIG_OPENTHREAD_PANID, CONFIG_OPENTHREAD_MASTERKEY, etc.
	 *
	 * Para commissioning dinámico, usar otDatasetSetActive() aquí.
	 */

	/* Habilitar interfaz IPv6 */
	error = otIp6SetEnabled(instance, true);
	if (error != OT_ERROR_NONE) {
		LOG_ERR("Failed to enable IPv6: %d", error);
		openthread_api_mutex_unlock(ot_context);
		return -EIO;
	}

	/* Iniciar Thread */
	error = otThreadSetEnabled(instance, true);
	if (error != OT_ERROR_NONE) {
		LOG_ERR("Failed to start Thread: %d", error);
		openthread_api_mutex_unlock(ot_context);
		return -EIO;
	}

	openthread_api_mutex_unlock(ot_context);

	LOG_INF("OpenThread started — joining network '%s' on channel %d",
		CONFIG_OPENTHREAD_NETWORK_NAME,
		CONFIG_OPENTHREAD_CHANNEL);

	return 0;
}

otDeviceRole thread_get_role(void)
{
	return otThreadGetDeviceRole(openthread_get_default_instance());
}

bool thread_is_connected(void)
{
	otDeviceRole role = thread_get_role();
	return (role == OT_DEVICE_ROLE_CHILD ||
		role == OT_DEVICE_ROLE_ROUTER ||
		role == OT_DEVICE_ROLE_LEADER);
}

uint16_t thread_get_rloc16(void)
{
	if (!thread_is_connected()) {
		return 0;
	}
	return otThreadGetRloc16(openthread_get_default_instance());
}

int8_t thread_get_rssi(void)
{
	int8_t avg_rssi;
	int8_t last_rssi;
	otError err;

	err = otThreadGetParentAverageRssi(openthread_get_default_instance(),
					   &avg_rssi);
	if (err == OT_ERROR_NONE) {
		return avg_rssi;
	}

	err = otThreadGetParentLastRssi(openthread_get_default_instance(),
					&last_rssi);
	if (err == OT_ERROR_NONE) {
		return last_rssi;
	}

	return 0; /* No RSSI available */
}
