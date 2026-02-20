/*
 * meter_modbus.c — Modbus RTU client for energy meters
 *
 * Lee holding registers (función 0x03) del medidor:
 * - Register 0x0000-0x0001: Voltage (IEEE 754 float, 2 regs)
 * - Register 0x0006-0x0007: Current (IEEE 754 float, 2 regs)
 * - Register 0x0100-0x0101: Active Energy (IEEE 754 float, 2 regs)
 * - Register 0x001E-0x001F: Power Factor (IEEE 754 float, 2 regs)
 *
 * TODO: Migrar a Zephyr Modbus subsystem (CONFIG_MODBUS)
 *       cuando el soporte UART async esté completo para ESP32-C6
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>

#include "meter_modbus.h"

LOG_MODULE_REGISTER(modbus, LOG_LEVEL_INF);

/* Modbus slave address (configurable) */
#define MODBUS_SLAVE_ADDR  1

/* Register addresses (typical SDM630/SDM120/Eastron meters) */
#define REG_VOLTAGE        0x0000
#define REG_CURRENT        0x0006
#define REG_ACTIVE_ENERGY  0x0156
#define REG_POWER_FACTOR   0x001E

#define MODBUS_TIMEOUT_MS  3000

/* CRC-16/Modbus lookup */
static uint16_t modbus_crc16(const uint8_t *data, size_t len)
{
	uint16_t crc = 0xFFFF;

	for (size_t i = 0; i < len; i++) {
		crc ^= data[i];
		for (int j = 0; j < 8; j++) {
			if (crc & 0x0001) {
				crc = (crc >> 1) ^ 0xA001;
			} else {
				crc >>= 1;
			}
		}
	}

	return crc;
}

/* Convert 2 Modbus registers (big-endian) to IEEE 754 float */
static float regs_to_float(uint16_t reg_hi, uint16_t reg_lo)
{
	uint32_t raw = ((uint32_t)reg_hi << 16) | reg_lo;
	float val;
	memcpy(&val, &raw, sizeof(val));
	return val;
}

int meter_modbus_init(void)
{
	/*
	 * Comparte UART con DLMS — el init de hardware ya lo hace meter_dlms.
	 * Aquí solo verificamos disponibilidad.
	 */
	LOG_INF("Modbus RTU initialized (slave addr: %d)", MODBUS_SLAVE_ADDR);
	return 0;
}

int meter_modbus_read(struct meter_reading *reading)
{
	/* TODO: Implementar lectura real usando Zephyr Modbus RTU client
	 *
	 * Pseudocódigo:
	 * 1. Build Modbus frame: [addr][0x03][reg_hi][reg_lo][count_hi][count_lo][crc_lo][crc_hi]
	 * 2. Send via UART (RS-485 TX mode)
	 * 3. Switch to RX, wait response
	 * 4. Parse response: [addr][0x03][byte_count][data...][crc_lo][crc_hi]
	 * 5. Convert registers to float values
	 */

	/* Placeholder — return simulated data for initial testing */
	reading->voltage_v = 220.0f + (k_uptime_get_32() % 20) - 10;
	reading->current_a = 5.1f;
	reading->energy_kwh = 12345.678f;
	reading->energy_exp_kwh = 0.0f;
	reading->power_factor = 0.95f;
	reading->pulse_count = k_uptime_get_32() / 1000;
	reading->timestamp_ms = k_uptime_get();

	LOG_DBG("Modbus (simulated): %.1fV %.2fA", reading->voltage_v,
		reading->current_a);

	return 0;
}
