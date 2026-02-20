/*
 * meter_dlms.c — DLMS/COSEM parser (IEC 62056-21 Mode C)
 *
 * Comunicación UART @ 9600 bps con medidor vía RS-485.
 * Implementa secuencia IEC 62056-21 Mode C simplificada:
 *   /?!\r\n  →  /ISK5\2MT382  →  ACK 0:0:1\r\n  →  Data block
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/gpio.h>
#include <string.h>
#include <stdlib.h>

#include "meter_dlms.h"

LOG_MODULE_REGISTER(dlms, LOG_LEVEL_INF);

/* Device tree nodes */
#define METER_UART_NODE DT_ALIAS(meter_uart)
static const struct device *uart_dev = DEVICE_DT_GET(METER_UART_NODE);

/* RS-485 DE/RE control pin */
static const struct gpio_dt_spec rs485_de =
	GPIO_DT_SPEC_GET(DT_NODELABEL(rs485_de), gpios);

/* UART RX buffer */
#define RX_BUF_SIZE 512
static uint8_t rx_buf[RX_BUF_SIZE];
static volatile int rx_len;
static struct k_sem rx_sem;

/* Timeout for meter response (ms) */
#define METER_TIMEOUT_MS 5000

/* --- UART ISR callback --- */
static void uart_cb(const struct device *dev, struct uart_event *evt,
		    void *user_data)
{
	switch (evt->type) {
	case UART_RX_RDY:
		rx_len = evt->data.rx.len;
		k_sem_give(&rx_sem);
		break;
	case UART_RX_DISABLED:
		break;
	default:
		break;
	}
}

/* --- RS-485 direction control --- */
static void rs485_tx_enable(void)
{
	gpio_pin_set_dt(&rs485_de, 1); /* DE=HIGH → TX mode */
}

static void rs485_rx_enable(void)
{
	gpio_pin_set_dt(&rs485_de, 0); /* DE=LOW → RX mode */
}

/* --- Send IEC 62056-21 request --- */
static int dlms_send_request(const uint8_t *data, size_t len)
{
	rs485_tx_enable();
	k_usleep(100); /* Stabilization time */

	for (size_t i = 0; i < len; i++) {
		uart_poll_out(uart_dev, data[i]);
	}

	/* Wait for TX complete */
	k_msleep(10 + (len * 2)); /* ~1ms per byte @ 9600 */
	rs485_rx_enable();

	return 0;
}

/* --- Receive response with timeout --- */
static int dlms_receive(uint8_t *buf, size_t max_len, int timeout_ms)
{
	rx_len = 0;
	int ret = uart_rx_enable(uart_dev, rx_buf, sizeof(rx_buf),
				 timeout_ms * 1000); /* us */
	if (ret < 0) {
		return ret;
	}

	ret = k_sem_take(&rx_sem, K_MSEC(timeout_ms));
	if (ret < 0) {
		uart_rx_disable(uart_dev);
		return -ETIMEDOUT;
	}

	int copy_len = MIN(rx_len, max_len);
	memcpy(buf, rx_buf, copy_len);
	return copy_len;
}

/* --- Parse OBIS value from data block --- */
static float parse_obis_value(const char *data, const char *obis_code)
{
	const char *pos = strstr(data, obis_code);
	if (!pos) {
		return -1.0f;
	}

	/* Format: "1.8.0(012345.678*kWh)" */
	pos = strchr(pos, '(');
	if (!pos) {
		return -1.0f;
	}

	return strtof(pos + 1, NULL);
}

int meter_dlms_init(void)
{
	if (!device_is_ready(uart_dev)) {
		LOG_ERR("UART device not ready");
		return -ENODEV;
	}

	/* Configure RS-485 DE pin */
	if (gpio_is_ready_dt(&rs485_de)) {
		gpio_pin_configure_dt(&rs485_de, GPIO_OUTPUT_INACTIVE);
		rs485_rx_enable();
	}

	/* Init UART async */
	k_sem_init(&rx_sem, 0, 1);
	uart_callback_set(uart_dev, uart_cb, NULL);

	LOG_INF("DLMS meter UART initialized");
	return 0;
}

int meter_dlms_read(struct meter_reading *reading)
{
	uint8_t response[RX_BUF_SIZE];
	int ret;

	/* IEC 62056-21 Mode C: Send request "/?!\r\n" */
	static const uint8_t iec_request[] = "/?!\r\n";
	dlms_send_request(iec_request, sizeof(iec_request) - 1);

	/* Wait for identification response */
	ret = dlms_receive(response, sizeof(response) - 1, METER_TIMEOUT_MS);
	if (ret < 0) {
		LOG_WRN("DLMS: no response from meter");
		return ret;
	}
	response[ret] = '\0';

	/* Send ACK with programming mode request */
	static const uint8_t iec_ack[] = "\x06"  /* ACK */
					  "0"    /* Protocol mode */
					  "0"    /* Baud rate (keep 9600) */
					  "1"    /* Mode (data readout) */
					  "\r\n";
	dlms_send_request(iec_ack, sizeof(iec_ack) - 1);

	/* Wait for data block */
	ret = dlms_receive(response, sizeof(response) - 1, METER_TIMEOUT_MS);
	if (ret < 0) {
		LOG_WRN("DLMS: no data block received");
		return ret;
	}
	response[ret] = '\0';

	/* Parse OBIS values */
	reading->energy_kwh = parse_obis_value((char *)response, "1.8.0");
	reading->energy_exp_kwh = parse_obis_value((char *)response, "2.8.0");
	reading->current_a = parse_obis_value((char *)response, "31.7.0");
	reading->voltage_v = parse_obis_value((char *)response, "32.7.0");
	reading->power_factor = parse_obis_value((char *)response, "13.7.0");
	reading->timestamp_ms = k_uptime_get();

	/* Validate */
	if (reading->voltage_v < 0 || reading->current_a < 0) {
		LOG_WRN("DLMS: incomplete data (some OBIS codes missing)");
		return -EIO;
	}

	LOG_INF("DLMS read OK: %.1fV %.2fA %.3fkWh",
		reading->voltage_v, reading->current_a,
		reading->energy_kwh);

	return 0;
}
