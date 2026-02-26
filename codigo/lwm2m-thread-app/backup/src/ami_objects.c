/*
 * ami_objects.c — AMI-specific LwM2M IPSO objects
 *
 * Registers:
 *   Object 3316 / Instance 0 — Voltage Sensor  (Vrms)
 *   Object 3317 / Instance 0 — Current Sensor   (Arms)
 *   Object 3300 / Instance 0 — Generic Sensor    (Energy kWh)
 *   Object 3300 / Instance 1 — Generic Sensor    (Power Factor)
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/lwm2m.h>

LOG_MODULE_DECLARE(ami_lwm2m_client, LOG_LEVEL_DBG);

/* Units strings */
static const char unit_volt[] = "V";
static const char unit_amp[]  = "A";
static const char unit_kwh[]  = "kWh";
static const char unit_pf[]   = "";

/* Application type descriptions */
static const char type_voltage[] = "Voltage RMS";
static const char type_current[] = "Current RMS";
static const char type_energy[]  = "Active Energy";
static const char type_pf[]      = "Power Factor";

/* --- helper: create one IPSO sensor instance --- */
static int create_sensor(uint16_t obj_id, uint16_t inst_id,
			 const char *units, size_t unit_len,
			 const char *app_type, size_t app_type_len,
			 double min_val, double max_val,
			 double initial_val)
{
	int ret;
	struct lwm2m_obj_path path;

	path = LWM2M_OBJ(obj_id, inst_id);
	ret = lwm2m_create_object_inst(&path);
	if (ret < 0) {
		LOG_ERR("Failed to create obj %u/%u: %d", obj_id, inst_id, ret);
		return ret;
	}

	/* 5700 — Sensor Value */
	lwm2m_set_f64(&LWM2M_OBJ(obj_id, inst_id, 5700), initial_val);

	/* 5701 — Units */
	lwm2m_set_res_buf(&LWM2M_OBJ(obj_id, inst_id, 5701),
			  (void *)units, unit_len, unit_len,
			  LWM2M_RES_DATA_FLAG_RO);

	/* 5603 — Min Range Value */
	lwm2m_set_f64(&LWM2M_OBJ(obj_id, inst_id, 5603), min_val);

	/* 5604 — Max Range Value */
	lwm2m_set_f64(&LWM2M_OBJ(obj_id, inst_id, 5604), max_val);

	/* 5750 — Application Type */
	lwm2m_set_res_buf(&LWM2M_OBJ(obj_id, inst_id, 5750),
			  (void *)app_type, app_type_len, app_type_len,
			  LWM2M_RES_DATA_FLAG_RO);

	LOG_INF("Created IPSO %u/%u (%s)", obj_id, inst_id, app_type);
	return 0;
}

/* --- public init — called from main after conn_mgr is ready --- */
int init_ami_objects(void)
{
	int ret;

	/* Voltage Sensor — Object 3316 / Instance 0 */
	ret = create_sensor(3316, 0,
			    unit_volt, sizeof(unit_volt),
			    type_voltage, sizeof(type_voltage),
			    0.0, 300.0, 220.0);
	if (ret < 0) return ret;

	/* Current Sensor — Object 3317 / Instance 0 */
	ret = create_sensor(3317, 0,
			    unit_amp, sizeof(unit_amp),
			    type_current, sizeof(type_current),
			    0.0, 100.0, 5.0);
	if (ret < 0) return ret;

	/* Energy (kWh) — Generic Sensor 3300 / Instance 0 */
	ret = create_sensor(3300, 0,
			    unit_kwh, sizeof(unit_kwh),
			    type_energy, sizeof(type_energy),
			    0.0, 999999.999, 12345.678);
	if (ret < 0) return ret;

	/* Power Factor — Generic Sensor 3300 / Instance 1 */
	ret = create_sensor(3300, 1,
			    unit_pf, sizeof(unit_pf),
			    type_pf, sizeof(type_pf),
			    0.0, 1.0, 0.95);
	if (ret < 0) return ret;

	LOG_INF("AMI IPSO objects initialised (3316, 3317, 3300x2)");
	return 0;
}
