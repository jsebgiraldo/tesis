/*
 * lwm2m_client.c — LwM2M client: registration + object management
 *
 * Utiliza el subsistema LwM2M nativo de Zephyr para:
 * 1. Registrarse con servidor Leshan (100.67.60.126:5683)
 * 2. Exponer Objects IPSO con datos del medidor
 * 3. Soportar Observe/Notify para telemetría push
 * 4. Enviar alarmas como notificaciones CON (confirmable)
 *
 * Mapeado OBIS → LwM2M (Zephyr IPSO objects disponibles):
 *
 * +--------+--------------------------+----------------------------------+
 * | ObjID  | Nombre                   | Uso en AMI                       |
 * +--------+--------------------------+----------------------------------+
 * | 3      | Device                   | Info HW: manufacturer, model,    |
 * |        |                          | firmware version, serial number  |
 * +--------+--------------------------+----------------------------------+
 * | 4      | Connectivity Monitoring  | RSSI, link quality, IP address,  |
 * |        |                          | network bearer (Thread)          |
 * +--------+--------------------------+----------------------------------+
 * | 3316   | Voltage Sensor           | Tensión L1 (V) — OBIS 32.7.0    |
 * +--------+--------------------------+----------------------------------+
 * | 3317   | Current Sensor           | Corriente L1 (A) — OBIS 31.7.0  |
 * +--------+--------------------------+----------------------------------+
 * | 3300/0 | Generic Sensor (Energy)  | kWh importada — OBIS 1.8.0      |
 * +--------+--------------------------+----------------------------------+
 * | 3300/1 | Generic Sensor (PF)      | Factor de potencia — OBIS 13.7.0|
 * +--------+--------------------------+----------------------------------+
 */

#include <zephyr/kernel.h>
#include <zephyr/version.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/lwm2m.h>
#include <zephyr/sys/reboot.h>

#include "lwm2m_client.h"
#include "thread_network.h"

LOG_MODULE_REGISTER(lwm2m, LOG_LEVEL_INF);

/* Endpoint name for LwM2M registration */
#define LWM2M_ENDPOINT_NAME  "ami-esp32c6-001"

/* LwM2M server URI — Leshan en el servidor de desarrollo */
#define LWM2M_SERVER_URI     "coap://100.67.60.126:5683"

/* IPSO Resource IDs (common to all sensor objects) */
#define SENSOR_VALUE_RID     5700
#define SENSOR_UNITS_RID     5701

/* RD client context */
static struct lwm2m_ctx client_ctx;

/* --- Static buffers for Device Object (3) OPTDATA resources --- */
static char dev_manufacturer[]  = "Espressif";
static char dev_model_number[]  = "XIAO-ESP32-C6";
static char dev_serial_number[] = "SN-AMI-001";
static char dev_fw_version[16];
static char dev_device_type[]   = "ESP32-C6 RISC-V";
static char dev_hw_version[]    = "Zephyr " KERNEL_VERSION_STRING;
static char dev_sw_version[]    = KERNEL_VERSION_STRING;

/* --- Device Object (3) callbacks --- */
static int device_reboot_cb(uint16_t obj_inst_id,
			    uint8_t *args, uint16_t args_len)
{
	LOG_WRN("Reboot requested via LwM2M — rebooting in 1s");
	k_sleep(K_SECONDS(1));
	sys_reboot(SYS_REBOOT_COLD);
	return 0; /* unreachable */
}

/* --- Inicialización de Objects LwM2M --- */
static int init_lwm2m_objects(void)
{
	/*
	 * Object 3: Device — recursos son OPTDATA, necesitan buffer propio.
	 * Usamos lwm2m_set_res_buf() para asignar buffers estáticos.
	 */
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 0),
			  dev_manufacturer, sizeof(dev_manufacturer),
			  sizeof(dev_manufacturer), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 1),
			  dev_model_number, sizeof(dev_model_number),
			  sizeof(dev_model_number), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 2),
			  dev_serial_number, sizeof(dev_serial_number),
			  sizeof(dev_serial_number), LWM2M_RES_DATA_FLAG_RO);
	strncpy(dev_fw_version, CONFIG_AMI_FW_VERSION, sizeof(dev_fw_version) - 1);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 3),
			  dev_fw_version, strlen(dev_fw_version) + 1,
			  sizeof(dev_fw_version), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_register_exec_callback(&LWM2M_OBJ(3, 0, 4),
				     device_reboot_cb);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 17),
			  dev_device_type, sizeof(dev_device_type),
			  sizeof(dev_device_type), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 18),
			  dev_hw_version, sizeof(dev_hw_version),
			  sizeof(dev_hw_version), LWM2M_RES_DATA_FLAG_RO);
	lwm2m_set_res_buf(&LWM2M_OBJ(3, 0, 19),
			  dev_sw_version, sizeof(dev_sw_version),
			  sizeof(dev_sw_version), LWM2M_RES_DATA_FLAG_RO);

	/*
	 * Object 4: Connectivity Monitoring
	 * Instance 0 is auto-created by Zephyr, no need to create it.
	 */

	/*
	 * Object 3316: Voltage Sensor (instance 0) — Tensión L1
	 * Resource 5700 = Sensor Value (float64)
	 * Resource 5701 = Sensor Units (string)
	 */
	lwm2m_create_object_inst(&LWM2M_OBJ(3316, 0));

	/*
	 * Object 3317: Current Sensor (instance 0) — Corriente L1
	 */
	lwm2m_create_object_inst(&LWM2M_OBJ(3317, 0));

	/*
	 * Object 3300: Generic Sensor
	 * Instance 0 = Energía activa importada (kWh)
	 * Instance 1 = Factor de potencia
	 */
	lwm2m_create_object_inst(&LWM2M_OBJ(3300, 0));
	lwm2m_create_object_inst(&LWM2M_OBJ(3300, 1));

	LOG_INF("LwM2M objects: 3(Device), 4(Conn), "
		"3316(Voltage), 3317(Current), 3300x2(Energy,PF)");
	return 0;
}

/* --- RD Client callbacks --- */
static void rd_client_event_cb(struct lwm2m_ctx *client,
			       enum lwm2m_rd_client_event event)
{
	switch (event) {
	case LWM2M_RD_CLIENT_EVENT_REGISTRATION_COMPLETE:
		LOG_INF("LwM2M Registration complete");
		break;
	case LWM2M_RD_CLIENT_EVENT_REGISTRATION_FAILURE:
		LOG_ERR("LwM2M Registration failed");
		break;
	case LWM2M_RD_CLIENT_EVENT_REG_UPDATE_COMPLETE:
		LOG_DBG("LwM2M Registration update complete");
		break;
	case LWM2M_RD_CLIENT_EVENT_DEREGISTER:
		LOG_WRN("LwM2M Deregistered");
		break;
	case LWM2M_RD_CLIENT_EVENT_DISCONNECT:
		LOG_WRN("LwM2M Disconnected");
		break;
	default:
		LOG_DBG("LwM2M RD event: %d", event);
		break;
	}
}

int lwm2m_client_init(void)
{
	int ret;

	/* Inicializar objects */
	ret = init_lwm2m_objects();
	if (ret < 0) {
		return ret;
	}

	/* Configurar contexto del cliente */
	memset(&client_ctx, 0, sizeof(client_ctx));

	/* Iniciar RD Client — registro con servidor */
	lwm2m_rd_client_start(&client_ctx,
			      LWM2M_ENDPOINT_NAME,
			      0,		/* flags */
			      rd_client_event_cb,
			      NULL);		/* observer cb */

	LOG_INF("LwM2M RD client started, endpoint: %s",
		LWM2M_ENDPOINT_NAME);

	return 0;
}

int lwm2m_update_meter_data(const struct meter_reading *reading)
{
	/* Object 3316/0: Voltage Sensor — Tensión L1 (V) */
	lwm2m_set_f64(&LWM2M_OBJ(3316, 0, SENSOR_VALUE_RID),
		      (double)reading->voltage_v);

	/* Object 3317/0: Current Sensor — Corriente L1 (A) */
	lwm2m_set_f64(&LWM2M_OBJ(3317, 0, SENSOR_VALUE_RID),
		      (double)reading->current_a);

	/* Object 3300/0: Generic Sensor — Energía importada (kWh) */
	lwm2m_set_f64(&LWM2M_OBJ(3300, 0, SENSOR_VALUE_RID),
		      (double)reading->energy_kwh);

	/* Object 3300/1: Generic Sensor — Factor de potencia */
	lwm2m_set_f64(&LWM2M_OBJ(3300, 1, SENSOR_VALUE_RID),
		      (double)reading->power_factor);

	/* Object 4: Connectivity — update RSSI */
	int8_t rssi = thread_get_rssi();
	lwm2m_set_s8(&LWM2M_OBJ(4, 0, 2), rssi);

	LOG_DBG("LwM2M updated: V=%.1f, A=%.2f, kWh=%.3f, PF=%.2f, RSSI=%d",
		(double)reading->voltage_v, (double)reading->current_a,
		(double)reading->energy_kwh, (double)reading->power_factor, rssi);

	return 0;
}

void lwm2m_notify_tamper_alarm(void)
{
	/* Use Generic Sensor 3300/1 value 999.0 as tamper sentinel */
	lwm2m_set_f64(&LWM2M_OBJ(3300, 1, SENSOR_VALUE_RID), 999.0);
	LOG_WRN("Tamper alarm notified to LwM2M server");
}

void lwm2m_notify_voltage_alarm(float voltage_v)
{
	/* Update voltage value — Observe will trigger notification */
	lwm2m_set_f64(&LWM2M_OBJ(3316, 0, SENSOR_VALUE_RID),
		      (double)voltage_v);
	LOG_WRN("Voltage alarm: %.1f V notified", (double)voltage_v);
}
