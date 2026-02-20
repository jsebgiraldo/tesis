/*
 * lwm2m_client.h — LwM2M client for AMI telemetry
 */

#ifndef LWM2M_CLIENT_H_
#define LWM2M_CLIENT_H_

#include <stdint.h>

/**
 * @brief Estructura de lectura del medidor
 */
struct meter_reading {
	float voltage_v;      /* Tensión L1 (V) — OBIS 32.7.0 */
	float current_a;      /* Corriente L1 (A) — OBIS 31.7.0 */
	float energy_kwh;     /* Energía activa importada (kWh) — OBIS 1.8.0 */
	float energy_exp_kwh; /* Energía activa exportada (kWh) — OBIS 2.8.0 */
	float power_factor;   /* Factor de potencia — OBIS 13.7.0 */
	int64_t timestamp_ms; /* Timestamp lectura (uptime ms) */
};

/**
 * @brief Inicializar cliente LwM2M y registrar con servidor
 *
 * Configura Objects LwM2M:
 * - Object 3 (Device): info hardware
 * - Object 4 (Connectivity Monitoring): RSSI, link quality
 * - Object 3316 (Voltage Sensor): tensión L1
 * - Object 3317 (Current Sensor): corriente L1
 * - Object 3300/0 (Generic Sensor): energía importada (kWh)
 * - Object 3300/1 (Generic Sensor): factor de potencia
 *
 * @return 0 en éxito, código de error negativo en fallo
 */
int lwm2m_client_init(void);

/**
 * @brief Actualizar datos del medidor en objects LwM2M
 * 
 * Actualiza los recursos de los objects IPSO con los valores
 * de la última lectura del medidor. Si hay Observe activo,
 * esto triggers una notificación automática al servidor.
 *
 * @param reading Lectura del medidor
 * @return 0 en éxito
 */
int lwm2m_update_meter_data(const struct meter_reading *reading);

/**
 * @brief Notificar alarma de tamper (apertura caja)
 * Envía notificación inmediata (CON) al servidor
 */
void lwm2m_notify_tamper_alarm(void);

/**
 * @brief Notificar alarma de voltaje fuera de rango
 * @param voltage_v Voltaje actual que causó la alarma
 */
void lwm2m_notify_voltage_alarm(float voltage_v);

#endif /* LWM2M_CLIENT_H_ */
