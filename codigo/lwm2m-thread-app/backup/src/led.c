/*
 * led.c — LwM2M Light Control Object (3311)
 *
 * Provides on/off control of the board LED via LwM2M Object 3311.
 * Based on the Zephyr LwM2M sample led.c.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/net/lwm2m.h>

LOG_MODULE_DECLARE(ami_lwm2m_client, LOG_LEVEL_DBG);

static const struct gpio_dt_spec led_dev =
	GPIO_DT_SPEC_GET_OR(DT_ALIAS(led0), gpios, {0});
static bool led_state;

static int led_on_off_cb(uint16_t obj_inst_id, uint16_t res_id,
			 uint16_t res_inst_id, uint8_t *data,
			 uint16_t data_len, bool last_block,
			 size_t total_size, size_t offset)
{
	int ret = 0;
	bool new_state = *(bool *)data;

	if (!gpio_is_ready_dt(&led_dev)) {
		return -ENODEV;
	}

	ret = gpio_pin_set_dt(&led_dev, new_state);
	if (ret) {
		LOG_ERR("LED set failed: %d", ret);
		return ret;
	}

	led_state = new_state;
	LOG_INF("LED %s", led_state ? "ON" : "OFF");
	return 0;
}

int init_led_device(void)
{
	if (!gpio_is_ready_dt(&led_dev)) {
		LOG_WRN("LED device not ready (led0 alias missing?)");
		return -ENODEV;
	}

	gpio_pin_configure_dt(&led_dev, GPIO_OUTPUT_INACTIVE);

	lwm2m_create_object_inst(&LWM2M_OBJ(3311, 0));
	lwm2m_register_post_write_callback(&LWM2M_OBJ(3311, 0, 5850),
					   led_on_off_cb);
	lwm2m_set_bool(&LWM2M_OBJ(3311, 0, 5850), false);

	LOG_INF("LED control object (3311) initialised");
	return 0;
}
