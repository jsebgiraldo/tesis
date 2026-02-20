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
	otOperationalDataset dataset;

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
	 * Configurar dataset COMPLETO del OTBR para unirse como child/router.
	 * Kconfig sólo establece channel/panid/networkkey pero NO el PSKc
	 * ni el Mesh Local Prefix, causando que el nodo cree su propia
	 * partición Thread en vez de unirse al OTBR.
	 */
	memset(&dataset, 0, sizeof(dataset));

	/* Active Timestamp — debe ser >= al del OTBR */
	dataset.mActiveTimestamp.mSeconds = 1;
	dataset.mActiveTimestamp.mTicks = 0;
	dataset.mActiveTimestamp.mAuthoritative = false;
	dataset.mComponents.mIsActiveTimestampPresent = true;

	/* Channel 25 */
	dataset.mChannel = CONFIG_OPENTHREAD_CHANNEL;
	dataset.mComponents.mIsChannelPresent = true;

	/* Channel Mask */
	dataset.mChannelMask = 0x07fff800;
	dataset.mComponents.mIsChannelMaskPresent = true;

	/* Extended PAN ID: 1234567890abcdef */
	static const uint8_t ext_panid[] = {
		0x12, 0x34, 0x56, 0x78, 0x90, 0xab, 0xcd, 0xef
	};
	memcpy(dataset.mExtendedPanId.m8, ext_panid, OT_EXT_PAN_ID_SIZE);
	dataset.mComponents.mIsExtendedPanIdPresent = true;

	/* Mesh Local Prefix: fdc6:63fd:328d:66df::/64 (del OTBR) */
	static const uint8_t mesh_prefix[] = {
		0xfd, 0xc6, 0x63, 0xfd, 0x32, 0x8d, 0x66, 0xdf
	};
	memcpy(dataset.mMeshLocalPrefix.m8, mesh_prefix, OT_MESH_LOCAL_PREFIX_SIZE);
	dataset.mComponents.mIsMeshLocalPrefixPresent = true;

	/* Network Key: 00112233445566778899aabbccddeeff */
	static const uint8_t network_key[] = {
		0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
		0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
	};
	memcpy(dataset.mNetworkKey.m8, network_key, OT_NETWORK_KEY_SIZE);
	dataset.mComponents.mIsNetworkKeyPresent = true;

	/* Network Name: AMI-Pilot-2025 */
	size_t name_len = strlen(CONFIG_OPENTHREAD_NETWORK_NAME);

	memcpy(dataset.mNetworkName.m8, CONFIG_OPENTHREAD_NETWORK_NAME,
	       name_len);
	dataset.mNetworkName.m8[name_len] = '\0';
	dataset.mComponents.mIsNetworkNamePresent = true;

	/* PAN ID: 0xABCD */
	dataset.mPanId = CONFIG_OPENTHREAD_PANID;
	dataset.mComponents.mIsPanIdPresent = true;

	/* PSKc del OTBR: 9d5361d24a2d518078eb9f8bc1965b80 */
	static const uint8_t pskc[] = {
		0x9d, 0x53, 0x61, 0xd2, 0x4a, 0x2d, 0x51, 0x80,
		0x78, 0xeb, 0x9f, 0x8b, 0xc1, 0x96, 0x5b, 0x80
	};
	memcpy(dataset.mPskc.m8, pskc, OT_PSKC_MAX_SIZE);
	dataset.mComponents.mIsPskcPresent = true;

	/* Security Policy */
	dataset.mSecurityPolicy.mRotationTime = 672;
	dataset.mSecurityPolicy.mObtainNetworkKeyEnabled = true;
	dataset.mSecurityPolicy.mNativeCommissioningEnabled = true;
	dataset.mSecurityPolicy.mRoutersEnabled = true;
	dataset.mSecurityPolicy.mExternalCommissioningEnabled = true;
	dataset.mComponents.mIsSecurityPolicyPresent = true;

	/* Aplicar dataset — esto sustituye lo que Kconfig haya puesto */
	error = otDatasetSetActive(instance, &dataset);
	if (error != OT_ERROR_NONE) {
		LOG_ERR("Failed to set active dataset: %d", error);
		openthread_mutex_unlock();
		return -EIO;
	}
	LOG_INF("Active dataset set (PSKc + MeshLocal from OTBR)");

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
