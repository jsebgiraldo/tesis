/*
 * AMI LwM2M Node — Thread + LwM2M on XIAO ESP32-C6
 *
 * LwM2M client that registers with a Leshan server via
 * Thread mesh network (OpenThread). Reports simulated
 * sensor data (voltage, current, temperature, energy).
 *
 * Flow:
 * 1. OpenThread joins the Thread network (credentials in prj.conf)
 * 2. Wait for L4 connectivity (IPv6 up via Thread)
 * 3. Register LwM2M client with Leshan server
 * 4. Periodically update IPSO sensor objects
 *
 * Aligned with working Windows build — single file, no MCUboot,
 * no dataset injection, no FOTA.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/lwm2m.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/net_event.h>
#include <zephyr/net/conn_mgr_monitor.h>
#include <zephyr/net/openthread.h>
#include <zephyr/random/random.h>

LOG_MODULE_REGISTER(ami_lwm2m, LOG_LEVEL_INF);

/* ---- Configuration ---- */
#define CLIENT_MANUFACTURER     "Tesis-AMI"
#define CLIENT_MODEL_NUMBER     "XIAO-ESP32-C6"
#define CLIENT_SERIAL_NUMBER    "AMI-001"
#define CLIENT_FIRMWARE_VER     "0.4.0"
#define CLIENT_HW_VER           "1.0"
#define ENDPOINT_NAME           "ami-esp32c6-001"

/* LwM2M Server URI — Leshan on OTBR mesh-local address */
#define LWM2M_SERVER_URI        "coap://[" CONFIG_NET_CONFIG_PEER_IPV6_ADDR "]:5683"

/* Sensor update interval */
#define SENSOR_UPDATE_INTERVAL  K_SECONDS(30)

/* LED */
static const struct gpio_dt_spec led0 =
	GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});

/* ---- Network connectivity ---- */
static K_SEM_DEFINE(network_connected_sem, 0, 1);

#define L4_EVENT_MASK (NET_EVENT_L4_CONNECTED | NET_EVENT_L4_DISCONNECTED)
static struct net_mgmt_event_callback l4_cb;

static void l4_event_handler(struct net_mgmt_event_callback *cb,
			     uint64_t event, struct net_if *iface)
{
	if (event == NET_EVENT_L4_CONNECTED) {
		LOG_INF("Network L4 connected (Thread IPv6 up)");
		k_sem_give(&network_connected_sem);
	} else if (event == NET_EVENT_L4_DISCONNECTED) {
		LOG_WRN("Network L4 disconnected");
	}
}

/* ---- Simulated sensor data ---- */
static double voltage_v = 220.0;
static double current_a = 1.5;
static double temperature_c = 25.0;
static double energy_kwh = 0.0;

/* ---- LwM2M context ---- */
static struct lwm2m_ctx client_ctx;
static bool lwm2m_connected;

/* ---- LwM2M callbacks ---- */
static int device_reboot_cb(uint16_t obj_inst_id,
			    uint8_t *args, uint16_t args_len)
{
	LOG_INF("DEVICE: Reboot requested");
	return 0;
}

static void rd_client_event(struct lwm2m_ctx *client,
			    enum lwm2m_rd_client_event client_event)
{
	switch (client_event) {
	case LWM2M_RD_CLIENT_EVENT_NONE:
		break;
	case LWM2M_RD_CLIENT_EVENT_REGISTRATION_COMPLETE:
		LOG_INF("LwM2M Registration complete!");
		lwm2m_connected = true;
		if (gpio_is_ready_dt(&led0)) {
			gpio_pin_set_dt(&led0, 1);
		}
		break;
	case LWM2M_RD_CLIENT_EVENT_REGISTRATION_FAILURE:
		LOG_ERR("LwM2M Registration FAILED");
		lwm2m_connected = false;
		break;
	case LWM2M_RD_CLIENT_EVENT_REG_TIMEOUT:
		LOG_WRN("LwM2M Registration timeout");
		lwm2m_connected = false;
		break;
	case LWM2M_RD_CLIENT_EVENT_REG_UPDATE_COMPLETE:
		LOG_DBG("LwM2M Registration update complete");
		break;
	case LWM2M_RD_CLIENT_EVENT_DISCONNECT:
		LOG_WRN("LwM2M Disconnected");
		lwm2m_connected = false;
		if (gpio_is_ready_dt(&led0)) {
			gpio_pin_set_dt(&led0, 0);
		}
		break;
	case LWM2M_RD_CLIENT_EVENT_NETWORK_ERROR:
		LOG_ERR("LwM2M network error — stopping client");
		lwm2m_rd_client_stop(client, rd_client_event, true);
		lwm2m_connected = false;
		break;
	default:
		LOG_DBG("LwM2M event: %d", client_event);
		break;
	}
}

static void observe_cb(enum lwm2m_observe_event event,
		       struct lwm2m_obj_path *path, void *user_data)
{
	switch (event) {
	case LWM2M_OBSERVE_EVENT_OBSERVER_ADDED:
		LOG_INF("Observe started: /%u/%u/%u",
			path->obj_id, path->obj_inst_id, path->res_id);
		break;
	case LWM2M_OBSERVE_EVENT_OBSERVER_REMOVED:
		LOG_INF("Observe stopped: /%u/%u/%u",
			path->obj_id, path->obj_inst_id, path->res_id);
		break;
	case LWM2M_OBSERVE_EVENT_NOTIFY_ACK:
		LOG_DBG("Notify ACK: /%u/%u/%u",
			path->obj_id, path->obj_inst_id, path->res_id);
		break;
	default:
		break;
	}
}

/* ---- LwM2M object setup ---- */
static int lwm2m_setup(void)
{
	/* Security Object (0) */
	lwm2m_set_string(&LWM2M_OBJ(0, 0, 0), LWM2M_SERVER_URI);
	lwm2m_set_u8(&LWM2M_OBJ(0, 0, 2), 3); /* NoSec mode */
	lwm2m_set_u16(&LWM2M_OBJ(0, 0, 10), 101); /* Short Server ID */

	/* Server Object (1) */
	lwm2m_set_u16(&LWM2M_OBJ(1, 0, 0), 101); /* Short Server ID */
	lwm2m_set_u32(&LWM2M_OBJ(1, 0, 1), 300); /* Lifetime = 300s */

	/* Device Object (3) */
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 0),
			  CLIENT_MANUFACTURER, sizeof(CLIENT_MANUFACTURER),
			  sizeof(CLIENT_MANUFACTURER), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 1),
			  CLIENT_MODEL_NUMBER, sizeof(CLIENT_MODEL_NUMBER),
			  sizeof(CLIENT_MODEL_NUMBER), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 2),
			  CLIENT_SERIAL_NUMBER, sizeof(CLIENT_SERIAL_NUMBER),
			  sizeof(CLIENT_SERIAL_NUMBER), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 3),
			  CLIENT_FIRMWARE_VER, sizeof(CLIENT_FIRMWARE_VER),
			  sizeof(CLIENT_FIRMWARE_VER), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_register_exec_callback(&LWM2M_OBJ(3, 0, 4), device_reboot_cb);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 17),
			  CONFIG_BOARD, sizeof(CONFIG_BOARD),
			  sizeof(CONFIG_BOARD), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 18),
			  CLIENT_HW_VER, sizeof(CLIENT_HW_VER),
			  sizeof(CLIENT_HW_VER), LWM2M_RES_DATA_FLAG_RO);

	/* IPSO Voltage Sensor (3316) — instance 0 */
	lwm2m_set_f64(&LWM2M_OBJ(3316, 0, 5700), voltage_v);
	lwm2m_set_string(&LWM2M_OBJ(3316, 0, 5701), "V");

	/* IPSO Current Sensor (3317) — instance 0 */
	lwm2m_set_f64(&LWM2M_OBJ(3317, 0, 5700), current_a);
	lwm2m_set_string(&LWM2M_OBJ(3317, 0, 5701), "A");

	/* IPSO Temperature Sensor (3303) — instance 0 */
	lwm2m_set_f64(&LWM2M_OBJ(3303, 0, 5700), temperature_c);
	lwm2m_set_string(&LWM2M_OBJ(3303, 0, 5701), "Cel");

	/* IPSO Generic Sensor (3300) — instance 0: Energy (kWh) */
	lwm2m_set_f64(&LWM2M_OBJ(3300, 0, 5700), energy_kwh);
	lwm2m_set_string(&LWM2M_OBJ(3300, 0, 5701), "kWh");

	LOG_INF("LwM2M objects configured");
	LOG_INF("  Server: %s", LWM2M_SERVER_URI);
	LOG_INF("  Endpoint: %s", ENDPOINT_NAME);
	return 0;
}

/* ---- Simulated sensor update ---- */
static void update_sensors(void)
{
	/* Simulate small variations */
	voltage_v = 218.0 + (sys_rand32_get() % 100) / 10.0;   /* 218-228V */
	current_a = 1.0 + (sys_rand32_get() % 50) / 100.0;     /* 1.0-1.5A  */
	temperature_c = 22.0 + (sys_rand32_get() % 80) / 10.0;  /* 22-30C    */
	energy_kwh += 0.01;                                       /* Accumulate */

	lwm2m_set_f64(&LWM2M_OBJ(3316, 0, 5700), voltage_v);
	lwm2m_set_f64(&LWM2M_OBJ(3317, 0, 5700), current_a);
	lwm2m_set_f64(&LWM2M_OBJ(3303, 0, 5700), temperature_c);
	lwm2m_set_f64(&LWM2M_OBJ(3300, 0, 5700), energy_kwh);

	LOG_INF("Sensors: %.1fV  %.2fA  %.1fC  %.3fkWh",
		voltage_v, current_a, temperature_c, energy_kwh);
}

/* ---- Main ---- */
int main(void)
{
	int ret;

	LOG_INF("=== AMI LwM2M Node v%s ===", CLIENT_FIRMWARE_VER);
	LOG_INF("Board: %s", CONFIG_BOARD);
	LOG_INF("Network: Thread Ch%d PAN 0x%04X",
		CONFIG_OPENTHREAD_CHANNEL, CONFIG_OPENTHREAD_PANID);

	/* LED init */
	if (gpio_is_ready_dt(&led0)) {
		gpio_pin_configure_dt(&led0, GPIO_OUTPUT_INACTIVE);
	}

	/* Register L4 event callback */
	net_mgmt_init_event_callback(&l4_cb, l4_event_handler, L4_EVENT_MASK);
	net_mgmt_add_event_callback(&l4_cb);

	/* Wait for Thread to bring up IPv6 */
	LOG_INF("Waiting for Thread network (L4 connected)...");
	if (gpio_is_ready_dt(&led0)) {
		/* Blink while waiting */
		while (k_sem_take(&network_connected_sem, K_MSEC(500)) != 0) {
			gpio_pin_toggle_dt(&led0);
		}
		gpio_pin_set_dt(&led0, 1);
	} else {
		k_sem_take(&network_connected_sem, K_FOREVER);
	}

	LOG_INF("Thread connected — starting LwM2M client");

	/* Setup LwM2M objects */
	ret = lwm2m_setup();
	if (ret < 0) {
		LOG_ERR("LwM2M setup failed: %d", ret);
		return ret;
	}

	/* Start LwM2M RD client */
	memset(&client_ctx, 0, sizeof(client_ctx));
	lwm2m_rd_client_start(&client_ctx, ENDPOINT_NAME, 0,
			      rd_client_event, observe_cb);

	/* Main loop — update sensors periodically */
	LOG_INF("Entering sensor loop (every 30 seconds)");

	while (1) {
		k_sleep(SENSOR_UPDATE_INTERVAL);
		update_sensors();
	}

	return 0;
}
