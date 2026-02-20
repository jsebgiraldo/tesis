/*
 * lwm2m_client.c — LwM2M client: registration + object management
 *
 * Utiliza el subsistema LwM2M nativo de Zephyr para:
 * 1. Registrarse con servidor Leshan (100.67.60.126:5683)
 * 2. Exponer Objects IPSO con datos del medidor
 * 3. Soportar Observe/Notify para telemetría push
 * 4. Enviar alarmas como notificaciones CON (confirmable)
 */

#include <zephyr/kernel.h>
#include <zephyr/version.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/lwm2m.h>

#include "lwm2m_client.h"
#include "thread_network.h"

LOG_MODULE_REGISTER(lwm2m, LOG_LEVEL_INF);

/* Endpoint name for LwM2M registration */
#define LWM2M_ENDPOINT_NAME  "ami-esp32c6-001"

/* LwM2M server URI — Leshan en el servidor de desarrollo */
#define LWM2M_SERVER_URI     "coap://100.67.60.126:5683"

/* Object IDs */
#define LWM2M_OBJECT_DEVICE_ID          3
#define LWM2M_OBJECT_CONN_MON_ID        4
#define LWM2M_OBJECT_DIGITAL_INPUT_ID   3200
#define LWM2M_OBJECT_ANALOG_INPUT_ID    3202
#define LWM2M_OBJECT_POWER_MEAS_ID      3305

/* RD client context */
static struct lwm2m_ctx client_ctx;

/* --- Device Object (3) callbacks --- */
static int device_reboot_cb(uint16_t obj_inst_id,
			    uint8_t *args, uint16_t args_len)
{
	LOG_WRN("Reboot requested via LwM2M");
	/* TODO: sys_reboot(SYS_REBOOT_COLD) after cleanup */
	return 0;
}

/* --- Inicialización de Objects LwM2M --- */
static int init_lwm2m_objects(void)
{
	/* Object 3: Device */
	lwm2m_set_string(&LWM2M_OBJ(3, 0, 0),
			 "Espressif");                /* Manufacturer */
	lwm2m_set_string(&LWM2M_OBJ(3, 0, 1),
			 "ESP32-C6-DevKitC-1");       /* Model */
	lwm2m_set_string(&LWM2M_OBJ(3, 0, 2),
			 "SN-AMI-001");              /* Serial Number */
	lwm2m_set_string(&LWM2M_OBJ(3, 0, 3),
			 "0.1.0");                   /* Firmware Version */
	lwm2m_register_exec_callback(&LWM2M_OBJ(3, 0, 4),
				     device_reboot_cb);   /* Reboot */
	lwm2m_set_string(&LWM2M_OBJ(3, 0, 17),
			 "ESP32-C6 RISC-V");         /* Device Type */
	lwm2m_set_string(&LWM2M_OBJ(3, 0, 18),
			 "Zephyr " KERNEL_VERSION_STRING); /* HW Version */
	lwm2m_set_string(&LWM2M_OBJ(3, 0, 19),
			 KERNEL_VERSION_STRING);     /* SW Version */

	/* Object 4: Connectivity Monitoring (instance 0) */
	lwm2m_create_object_inst(&LWM2M_OBJ(4, 0));

	/* Object 3202: Analog Input — Voltage sensor (instance 0) */
	lwm2m_create_object_inst(&LWM2M_OBJ(3202, 0));
	lwm2m_set_string(&LWM2M_OBJ(3202, 0, 5518),
			 "V");                       /* Sensor Units */

	/* Object 3202: Analog Input — Current sensor (instance 1) */
	lwm2m_create_object_inst(&LWM2M_OBJ(3202, 1));
	lwm2m_set_string(&LWM2M_OBJ(3202, 1, 5518),
			 "A");                       /* Sensor Units */

	/* Object 3200: Digital Input — Pulse counter (instance 0) */
	lwm2m_create_object_inst(&LWM2M_OBJ(3200, 0));

	/* Object 3305: Power Measurement (instance 0) */
	lwm2m_create_object_inst(&LWM2M_OBJ(3305, 0));

	LOG_INF("LwM2M objects initialized: 3, 4, 3200, 3202(x2), 3305");
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
	/* Object 3202/0: Voltage (Analog Input instance 0) */
	lwm2m_set_f64(&LWM2M_OBJ(3202, 0, 5600),
		      (double)reading->voltage_v);

	/* Object 3202/1: Current (Analog Input instance 1) */
	lwm2m_set_f64(&LWM2M_OBJ(3202, 1, 5600),
		      (double)reading->current_a);

	/* Object 3200/0: Pulse counter (Digital Input) */
	lwm2m_set_u32(&LWM2M_OBJ(3200, 0, 5501),
		      reading->pulse_count);

	/* Object 3305/0: Power Factor */
	lwm2m_set_f64(&LWM2M_OBJ(3305, 0, 5820),
		      (double)reading->power_factor);

	/* Object 4: Connectivity — update RSSI */
	int8_t rssi = thread_get_rssi();
	lwm2m_set_s8(&LWM2M_OBJ(4, 0, 2), rssi);

	LOG_DBG("LwM2M objects updated: V=%.1f, A=%.2f, PF=%.2f, RSSI=%d",
		reading->voltage_v, reading->current_a,
		reading->power_factor, rssi);

	return 0;
}

void lwm2m_notify_tamper_alarm(void)
{
	/* Set Digital Input state = 1 (alarm active) */
	lwm2m_set_bool(&LWM2M_OBJ(3200, 0, 5500), true);
	LOG_WRN("Tamper alarm notified to LwM2M server");
}

void lwm2m_notify_voltage_alarm(float voltage_v)
{
	/* Update voltage value — Observe will trigger notification */
	lwm2m_set_f64(&LWM2M_OBJ(3202, 0, 5600), (double)voltage_v);
	LOG_WRN("Voltage alarm: %.1f V notified", voltage_v);
}
