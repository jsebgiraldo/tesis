/*
 * thread_network.c — OpenThread FTD initialization and management
 *
 * Configura ESP32-C6 como Full Thread Device (FTD) con:
 * - Dataset de red pre-configurado (PAN ID, Channel, Network Key)
 * - Rol FTD con capacidad router
 * - State change callback para notificar a main
 *
 * Usa la API nueva de Zephyr v4.2.0:
 *   openthread_get_default_instance(), openthread_mutex_lock(),
 *   openthread_state_changed_callback_register()
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/openthread.h>
#include <openthread.h>  /* new module-level API (v4.2.0) */
#include <openthread/instance.h>
#include <openthread/thread.h>
#include <openthread/dataset.h>
#include <openthread/ip6.h>
#include <openthread/link.h>

#include "thread_network.h"

LOG_MODULE_REGISTER(thread_net, LOG_LEVEL_INF);

static thread_state_cb_t user_state_cb;
static struct openthread_state_changed_callback state_cb_entry;

/* State change handler registered with OpenThread */
static void ot_state_changed_handler(otChangedFlags flags,
				     void *context)
{
	ARG_UNUSED(context);

	if (flags & OT_CHANGED_THREAD_ROLE) {
		otDeviceRole role = otThreadGetDeviceRole(
			openthread_get_default_instance());
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
	otInstance *instance;
	otError error;

	user_state_cb = state_cb;

	/* Obtener instancia OpenThread (nueva API v4.2.0) */
	instance = openthread_get_default_instance();
	if (!instance) {
		LOG_ERR("Failed to get OpenThread instance");
		return -ENODEV;
	}

	/* Registrar callback con nueva API (no requiere context) */
	state_cb_entry.otCallback = ot_state_changed_handler;
	state_cb_entry.user_data = NULL;
	openthread_state_changed_callback_register(&state_cb_entry);

	openthread_mutex_lock();

	/*
	 * Configurar dataset usando TLVs raw EXACTOS del OTBR.
	 *
	 * Se usa otDatasetSetActiveTlvs() en vez de otDatasetSetActive() para
	 * garantizar coincidencia byte-a-byte con el OTBR, incluyendo:
	 *   - Security Policy de 4 bytes (Thread 1.2+, flags f7 f8)
	 *   - TLV 0x4a (Discovery Request / Thread version info)
	 *   - Channel Mask TLV 0x35
	 *
	 * Obtenido del OTBR con: ot-ctl dataset active -x
	 * Red: AMI-Pilot-2025, Canal 25, PAN 0xABCD
	 */
	static const uint8_t otbr_dataset_raw[] = {
		/* Active Timestamp (type=0x0e, len=8): seconds=1 */
		0x0e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
		0x00, 0x00,
		/* TLV 0x4a (len=3): Thread version / discovery info */
		0x4a, 0x03, 0x00, 0x00, 0x0d,
		/* Channel Mask (type=0x35, len=6) */
		0x35, 0x06, 0x00, 0x04, 0x00, 0x1f, 0xff, 0xe0,
		/* Mesh Local Prefix (type=0x07, len=8): fdc6:63fd:328d:66df:: */
		0x07, 0x08, 0xfd, 0xc6, 0x63, 0xfd, 0x32, 0x8d,
		0x66, 0xdf,
		/* PSKc (type=0x04, len=16) */
		0x04, 0x10, 0x9d, 0x53, 0x61, 0xd2, 0x4a, 0x2d,
		0x51, 0x80, 0x78, 0xeb, 0x9f, 0x8b, 0xc1, 0x96,
		0x5b, 0x80,
		/* Security Policy (type=0x0c, len=4): rotation=672, flags=f7 f8 */
		0x0c, 0x04, 0x02, 0xa0, 0xf7, 0xf8,
		/* Channel (type=0x00, len=3): page=0, channel=25 */
		0x00, 0x03, 0x00, 0x00, 0x19,
		/* PAN ID (type=0x01, len=2): 0xABCD */
		0x01, 0x02, 0xab, 0xcd,
		/* Extended PAN ID (type=0x02, len=8) */
		0x02, 0x08, 0x12, 0x34, 0x56, 0x78, 0x90, 0xab,
		0xcd, 0xef,
		/* Network Name (type=0x03, len=14): AMI-Pilot-2025 */
		0x03, 0x0e, 0x41, 0x4d, 0x49, 0x2d, 0x50, 0x69,
		0x6c, 0x6f, 0x74, 0x2d, 0x32, 0x30, 0x32, 0x35,
		/* Network Key (type=0x05, len=16) */
		0x05, 0x10, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55,
		0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd,
		0xee, 0xff,
	};

	otOperationalDatasetTlvs dataset_tlvs;

	memcpy(dataset_tlvs.mTlvs, otbr_dataset_raw,
	       sizeof(otbr_dataset_raw));
	dataset_tlvs.mLength = sizeof(otbr_dataset_raw);

	error = otDatasetSetActiveTlvs(instance, &dataset_tlvs);
	if (error != OT_ERROR_NONE) {
		LOG_ERR("Failed to set active dataset TLVs: %d", error);
		openthread_mutex_unlock();
		return -EIO;
	}
	LOG_INF("Active dataset set (raw TLVs from OTBR, %u bytes)",
		dataset_tlvs.mLength);

	/* Habilitar interfaz IPv6 */
	error = otIp6SetEnabled(instance, true);
	if (error != OT_ERROR_NONE) {
		LOG_ERR("Failed to enable IPv6: %d", error);
		openthread_mutex_unlock();
		return -EIO;
	}

	/* Iniciar Thread */
	error = otThreadSetEnabled(instance, true);
	if (error != OT_ERROR_NONE) {
		LOG_ERR("Failed to start Thread: %d", error);
		openthread_mutex_unlock();
		return -EIO;
	}

	openthread_mutex_unlock();

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
