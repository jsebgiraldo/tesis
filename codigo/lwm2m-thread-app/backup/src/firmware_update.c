/*
 * firmware_update.c — LwM2M Firmware Update Object (5)
 *
 * Provides FOTA callbacks for MCUboot-based firmware update.
 * Simplified from the Zephyr LwM2M sample.
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/lwm2m.h>

LOG_MODULE_DECLARE(ami_lwm2m_client, LOG_LEVEL_DBG);

#ifdef CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT

static uint8_t firmware_buf[64];

static void *firmware_get_buf(uint16_t obj_inst_id,
			      uint16_t res_id,
			      uint16_t res_inst_id,
			      size_t *data_len)
{
	*data_len = sizeof(firmware_buf);
	return firmware_buf;
}

static int firmware_block_received_cb(uint16_t obj_inst_id,
				      uint16_t res_id,
				      uint16_t res_inst_id,
				      uint8_t *data, uint16_t data_len,
				      bool last_block,
				      size_t total_size, size_t offset)
{
	LOG_INF("FOTA block: offset=%zu len=%u last=%d total=%zu",
		offset, data_len, last_block, total_size);
	return 0;
}

static int firmware_update_cb(uint16_t obj_inst_id,
			      uint8_t *args, uint16_t args_len)
{
	LOG_INF("FOTA update requested");
	/* MCUboot will swap the images on next reboot */
	return 0;
}

#endif /* CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT */

int init_firmware_update(void)
{
#ifdef CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT
	lwm2m_firmware_set_write_cb(firmware_block_received_cb);
	lwm2m_firmware_set_update_cb(firmware_update_cb);
	LOG_INF("Firmware Update object initialised");
#endif
	return 0;
}
