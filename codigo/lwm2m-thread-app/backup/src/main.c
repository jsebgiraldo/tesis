/*
 * main.c — AMI LwM2M Client over OpenThread
 *
 * Based on Zephyr samples/net/lwm2m_client pattern:
 *   1. Wait for network connectivity (conn_mgr / L4 connected event)
 *   2. Setup LwM2M objects (Device, AMI sensors, FOTA)
 *   3. Start RD client registration
 *   4. Application loop updates sensor values periodically
 *
 * The OpenThread network is managed by Zephyr's L2 layer + Kconfig.
 * The Active Dataset is optionally injected from raw TLVs for exact
 * OTBR partition match.
 */

#define LOG_MODULE_NAME ami_lwm2m_client
#define LOG_LEVEL LOG_LEVEL_DBG

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/hwinfo.h>
#include <zephyr/net/lwm2m.h>
#include <zephyr/net/conn_mgr_monitor.h>
#include <zephyr/net/conn_mgr_connectivity.h>
#include <zephyr/net/openthread.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/random/random.h>

#include "modules.h"

#define APP_BANNER "AMI LwM2M Client v" CONFIG_AMI_FW_VERSION

/* --- LwM2M Device Object constants --- */
#define CLIENT_MANUFACTURER   "Espressif"
#define CLIENT_MODEL_NUMBER   "XIAO-ESP32-C6"
#define CLIENT_SERIAL_NUMBER  "SN-AMI-001"
#define CLIENT_FIRMWARE_VER   CONFIG_AMI_FW_VERSION
#define CLIENT_HW_VER         "1.0"
#define CLIENT_DEVICE_TYPE    "AMI Smart Meter Node"

/* Endpoint: use Kconfig if set, else board name */
static const char *endpoint =
	(sizeof(CONFIG_NET_SAMPLE_LWM2M_ID) > 1 ?
	 CONFIG_NET_SAMPLE_LWM2M_ID : CONFIG_BOARD);

static struct lwm2m_ctx client_ctx;
static struct k_sem quit_lock;

/* Zephyr NET management event callbacks */
static struct net_mgmt_event_callback l4_cb;
static struct net_mgmt_event_callback conn_cb;
static K_SEM_DEFINE(network_connected_sem, 0, 1);

/* LED */
static const struct gpio_dt_spec led_status =
	GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});

/* ------------------------------------------------------------------ */
/* Device Object (3) callbacks                                         */
/* ------------------------------------------------------------------ */

static int device_reboot_cb(uint16_t obj_inst_id,
			    uint8_t *args, uint16_t args_len)
{
	LOG_WRN("Reboot requested via LwM2M");
	k_sleep(K_SECONDS(1));
	sys_reboot(SYS_REBOOT_COLD);
	return 0;
}

static int device_factory_default_cb(uint16_t obj_inst_id,
				     uint8_t *args, uint16_t args_len)
{
	LOG_INF("Factory default requested via LwM2M");
	return 0;
}

/* ------------------------------------------------------------------ */
/* LwM2M setup: all objects                                            */
/* ------------------------------------------------------------------ */

static int lwm2m_setup(void)
{
	/* Security Object (0) — server URI + mode */
	lwm2m_set_string(&LWM2M_OBJ(0, 0, 0), CONFIG_NET_SAMPLE_LWM2M_SERVER);
	lwm2m_set_u8(&LWM2M_OBJ(0, 0, 2), 3);   /* NoSec */

	/*
	 * Match Security object with Server object via Short Server ID.
	 * Security(0)/0/10 = SSID,  Server(1)/0/0 = SSID
	 */
	lwm2m_set_u16(&LWM2M_OBJ(0, 0, 10), CONFIG_LWM2M_SERVER_DEFAULT_SSID);
	lwm2m_set_u16(&LWM2M_OBJ(1, 0, 0), CONFIG_LWM2M_SERVER_DEFAULT_SSID);

	/* Device Object (3) */
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 0), CLIENT_MANUFACTURER,
			  sizeof(CLIENT_MANUFACTURER),
			  sizeof(CLIENT_MANUFACTURER), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 1), CLIENT_MODEL_NUMBER,
			  sizeof(CLIENT_MODEL_NUMBER),
			  sizeof(CLIENT_MODEL_NUMBER), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 2), CLIENT_SERIAL_NUMBER,
			  sizeof(CLIENT_SERIAL_NUMBER),
			  sizeof(CLIENT_SERIAL_NUMBER), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 3), CLIENT_FIRMWARE_VER,
			  sizeof(CLIENT_FIRMWARE_VER),
			  sizeof(CLIENT_FIRMWARE_VER), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_register_exec_callback(&LWM2M_OBJ(3, 0, 4), device_reboot_cb);
	lwm2m_register_exec_callback(&LWM2M_OBJ(3, 0, 5),
				     device_factory_default_cb);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 17), CLIENT_DEVICE_TYPE,
			  sizeof(CLIENT_DEVICE_TYPE),
			  sizeof(CLIENT_DEVICE_TYPE), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 18), CLIENT_HW_VER,
			  sizeof(CLIENT_HW_VER),
			  sizeof(CLIENT_HW_VER), LWM2M_RES_DATA_FLAG_RO);

	/* AMI sensor objects (Voltage, Current, Energy, PF) */
	init_ami_objects();

	/* LED control */
	init_led_device();

	/* Firmware Update (Object 5) */
	if (IS_ENABLED(CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT)) {
		init_firmware_update();
	}

	return 0;
}

/* ------------------------------------------------------------------ */
/* RD Client event callback                                            */
/* ------------------------------------------------------------------ */

static void rd_client_event(struct lwm2m_ctx *client,
			    enum lwm2m_rd_client_event client_event)
{
	switch (client_event) {
	case LWM2M_RD_CLIENT_EVENT_NONE:
		break;
	case LWM2M_RD_CLIENT_EVENT_REGISTRATION_COMPLETE:
		LOG_INF("LwM2M registration complete");
		if (gpio_is_ready_dt(&led_status)) {
			gpio_pin_set_dt(&led_status, 1);
		}
		break;
	case LWM2M_RD_CLIENT_EVENT_REGISTRATION_FAILURE:
		LOG_ERR("LwM2M registration failure");
		break;
	case LWM2M_RD_CLIENT_EVENT_REG_UPDATE_COMPLETE:
		LOG_DBG("LwM2M registration update complete");
		break;
	case LWM2M_RD_CLIENT_EVENT_REG_TIMEOUT:
		LOG_WRN("LwM2M registration timeout");
		break;
	case LWM2M_RD_CLIENT_EVENT_DISCONNECT:
		LOG_WRN("LwM2M disconnected");
		if (gpio_is_ready_dt(&led_status)) {
			gpio_pin_set_dt(&led_status, 0);
		}
		break;
	case LWM2M_RD_CLIENT_EVENT_NETWORK_ERROR:
		LOG_ERR("LwM2M network error — stopping client");
		lwm2m_rd_client_stop(client, rd_client_event, true);
		break;
	default:
		LOG_DBG("LwM2M RD event: %d", client_event);
		break;
	}
}

/* ------------------------------------------------------------------ */
/* Network events (L4 connected / disconnected via conn_mgr)           */
/* ------------------------------------------------------------------ */

static void on_net_event_l4_connected(void)
{
	LOG_INF("Network connected (L4 up)");
	k_sem_give(&network_connected_sem);
	lwm2m_engine_resume();
}

static void on_net_event_l4_disconnected(void)
{
	LOG_INF("Network disconnected (L4 down)");
	lwm2m_engine_pause();
}

static void l4_event_handler(struct net_mgmt_event_callback *cb,
			     uint64_t event, struct net_if *iface)
{
	switch (event) {
	case NET_EVENT_L4_CONNECTED:
		LOG_INF("IP Up");
		on_net_event_l4_connected();
		break;
	case NET_EVENT_L4_DISCONNECTED:
		LOG_INF("IP Down");
		on_net_event_l4_disconnected();
		break;
	default:
		break;
	}
}

static void connectivity_event_handler(struct net_mgmt_event_callback *cb,
				       uint64_t event, struct net_if *iface)
{
	if (event == NET_EVENT_CONN_IF_FATAL_ERROR) {
		LOG_ERR("Fatal connectivity error");
	}
}

/* ------------------------------------------------------------------ */
/* main()                                                              */
/* ------------------------------------------------------------------ */

int main(void)
{
	uint32_t flags = IS_ENABLED(CONFIG_LWM2M_RD_CLIENT_SUPPORT_BOOTSTRAP) ?
			 LWM2M_RD_CLIENT_FLAG_BOOTSTRAP : 0;
	int ret;

	LOG_INF(APP_BANNER);
	LOG_INF("Board: %s", CONFIG_BOARD);

	k_sem_init(&quit_lock, 0, K_SEM_MAX_LIMIT);

	/* LED init */
	if (gpio_is_ready_dt(&led_status)) {
		gpio_pin_configure_dt(&led_status, GPIO_OUTPUT_INACTIVE);
	}

	/*
	 * Inject raw Active Dataset TLVs from the OTBR.
	 * This overrides the Kconfig-based dataset for byte-for-byte match
	 * with the OTBR, ensuring partition merge.
	 */
	ret = thread_inject_dataset();
	if (ret < 0) {
		LOG_WRN("Dataset injection failed (%d), using Kconfig defaults", ret);
	}

	/*
	 * Network lifecycle via conn_mgr:
	 * OpenThread L2 brings up the interface; conn_mgr detects L4
	 * connectivity (IPv6 address assigned) and fires the event.
	 */
	if (IS_ENABLED(CONFIG_NET_CONNECTION_MANAGER)) {
		struct net_if *iface = net_if_get_default();

		if (!iface) {
			LOG_ERR("No network interface!");
			return -ENODEV;
		}

		net_mgmt_init_event_callback(&l4_cb, l4_event_handler,
					     NET_EVENT_L4_CONNECTED |
					     NET_EVENT_L4_DISCONNECTED);
		net_mgmt_add_event_callback(&l4_cb);

		net_mgmt_init_event_callback(&conn_cb, connectivity_event_handler,
					     NET_EVENT_CONN_IF_FATAL_ERROR);
		net_mgmt_add_event_callback(&conn_cb);

		ret = net_if_up(iface);
		if (ret < 0 && ret != -EALREADY) {
			LOG_ERR("net_if_up error: %d", ret);
			return ret;
		}

		(void)conn_mgr_if_connect(iface);

		LOG_INF("Waiting for Thread network connectivity...");
		k_sem_take(&network_connected_sem, K_FOREVER);
		LOG_INF("Thread network ready");
	}

	/* Setup all LwM2M objects */
	ret = lwm2m_setup();
	if (ret < 0) {
		LOG_ERR("LwM2M setup failed: %d", ret);
		return ret;
	}

	/* Start RD client */
	memset(&client_ctx, 0, sizeof(client_ctx));
	lwm2m_rd_client_start(&client_ctx, endpoint, flags,
			      rd_client_event, NULL);
	LOG_INF("LwM2M RD client started, endpoint: %s", endpoint);

	/* Application loop — update simulated sensor data */
	while (1) {
		/* Generate dummy meter readings (replace with real DLMS/Modbus) */
		double voltage = 220.0 + (double)(sys_rand32_get() % 200) / 10.0 - 10.0;
		double current = 5.0 + (double)(sys_rand32_get() % 100) / 100.0 - 0.5;
		double energy_kwh = 12345.678 + (double)(sys_rand32_get() % 1000) / 1000.0;
		double power_factor = 0.95 + (double)(sys_rand32_get() % 50) / 1000.0;

		lwm2m_set_f64(&LWM2M_OBJ(3316, 0, 5700), voltage);
		lwm2m_set_f64(&LWM2M_OBJ(3317, 0, 5700), current);
		lwm2m_set_f64(&LWM2M_OBJ(3300, 0, 5700), energy_kwh);
		lwm2m_set_f64(&LWM2M_OBJ(3300, 1, 5700), power_factor);

		LOG_INF("Meter: %.1fV, %.2fA, %.3fkWh, PF=%.3f",
			voltage, current, energy_kwh, power_factor);

		/* LED blink while operational */
		if (gpio_is_ready_dt(&led_status)) {
			gpio_pin_toggle_dt(&led_status);
		}

		k_sleep(K_MINUTES(2));
	}

	return 0;
}
