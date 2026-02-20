/*
 * fota.c — Firmware Over-The-Air via LwM2M Object 5 + MCUboot
 *
 * Implementación para ESP32-C6 con Zephyr RTOS:
 * - Recibe firmware vía CoAP Block Transfer (Object 5/0/0 PUSH o 5/0/1 PULL)
 * - Escribe bloques directamente a flash slot1 (image-1) vía stream_flash
 * - Al completar descarga: verifica integridad (hash MCUboot header)
 * - Execute resource (5/0/2): marca imagen con boot_request_upgrade()
 * - Tras reboot: MCUboot carga slot1, ejecuta como test → confirmación
 *
 * Dependencias Kconfig:
 *   CONFIG_BOOTLOADER_MCUBOOT=y
 *   CONFIG_LWM2M_FIRMWARE_UPDATE_OBJ_SUPPORT=y
 *   CONFIG_IMG_MANAGER=y
 *   CONFIG_MCUBOOT_IMG_MANAGER=y
 *   CONFIG_STREAM_FLASH=y
 *   CONFIG_FLASH=y
 *   CONFIG_FLASH_MAP=y
 *   CONFIG_REBOOT=y
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/lwm2m.h>
#include <zephyr/dfu/mcuboot.h>
#include <zephyr/dfu/flash_img.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/storage/flash_map.h>

#include "fota.h"

LOG_MODULE_REGISTER(fota, LOG_LEVEL_INF);

/* --- Versión de firmware (se actualiza en cada release) --- */
#define FW_VERSION CONFIG_AMI_FW_VERSION

/* --- Contexto interno --- */
static struct flash_img_context flash_ctx;
static struct fota_progress progress;
static fota_progress_cb_t user_cb;
static bool image_confirmed;
static uint8_t firmware_buf[CONFIG_LWM2M_COAP_BLOCK_SIZE];

/* Array con protocolos soportados (CoAP = 0) */
static uint8_t supported_protocols[] = { 0 /* CoAP */ };

/* --- Forward declarations --- */
static int fota_update_cb(uint16_t obj_inst_id,
			  uint8_t *args, uint16_t args_len);
static void *fota_get_buf(uint16_t obj_inst_id, uint16_t res_id,
			  uint16_t res_inst_id, size_t *data_len);
static int fota_block_received_cb(uint16_t obj_inst_id, uint16_t res_id,
				  uint16_t res_inst_id, uint8_t *data,
				  uint16_t data_len, bool last_block,
				  size_t total_size, size_t offset);
static int fota_cancel_cb(uint16_t obj_inst_id);

/* --- Notificar progreso --- */
static void notify_progress(void)
{
	if (user_cb) {
		user_cb(&progress);
	}
}

/* --- Confirmar imagen actual si es "test" tras OTA --- */
static void confirm_image_if_needed(void)
{
	if (!boot_is_img_confirmed()) {
		LOG_INF("FOTA: Image is unconfirmed (first boot after OTA)");
		int ret = boot_write_img_confirmed();
		if (ret == 0) {
			LOG_INF("FOTA: Image CONFIRMED — rollback disabled");
			image_confirmed = true;
		} else {
			LOG_ERR("FOTA: Failed to confirm image: %d", ret);
			LOG_WRN("FOTA: MCUboot will rollback on next reboot!");
			image_confirmed = false;
		}
	} else {
		LOG_INF("FOTA: Running confirmed image v%s", FW_VERSION);
		image_confirmed = true;
	}
}

/* --- Callback: bloque de firmware recibido --- */
static void *fota_get_buf(uint16_t obj_inst_id, uint16_t res_id,
			  uint16_t res_inst_id, size_t *data_len)
{
	*data_len = sizeof(firmware_buf);
	return firmware_buf;
}

static int fota_block_received_cb(uint16_t obj_inst_id, uint16_t res_id,
				  uint16_t res_inst_id, uint8_t *data,
				  uint16_t data_len, bool last_block,
				  size_t total_size, size_t offset)
{
	int ret;

	/* Primer bloque: inicializar contexto de flash */
	if (offset == 0) {
		LOG_INF("FOTA: Download started, total_size=%zu", total_size);
		progress.status = FOTA_STATUS_DOWNLOADING;
		progress.total_size = total_size;
		progress.bytes_received = 0;
		progress.percent = 0;

		ret = flash_img_init(&flash_ctx);
		if (ret) {
			LOG_ERR("FOTA: flash_img_init failed: %d", ret);
			progress.status = FOTA_STATUS_ERROR;
			progress.last_error = ret;
			notify_progress();
			return ret;
		}

		/* Informar al servidor LwM2M: estado = descargando */
		lwm2m_set_u8(&LWM2M_OBJ(5, 0, 3), STATE_DOWNLOADING);
	}

	/* Escribir bloque a flash slot1 */
	ret = flash_img_buffered_write(&flash_ctx, data, data_len, last_block);
	if (ret) {
		LOG_ERR("FOTA: flash write failed at offset %zu: %d",
			offset, ret);
		progress.status = FOTA_STATUS_ERROR;
		progress.last_error = ret;
		lwm2m_set_u8(&LWM2M_OBJ(5, 0, 3), STATE_IDLE);
		lwm2m_set_u8(&LWM2M_OBJ(5, 0, 5), RESULT_INTEGRITY_FAILED);
		notify_progress();
		return ret;
	}

	/* Actualizar progreso */
	progress.bytes_received = offset + data_len;
	if (total_size > 0) {
		progress.percent = (uint8_t)((progress.bytes_received * 100)
					     / total_size);
	}

	LOG_DBG("FOTA: block offset=%zu len=%u (%u%%)",
		offset, data_len, progress.percent);

	/* Último bloque: descarga completa */
	if (last_block) {
		LOG_INF("FOTA: Download complete — %zu bytes received",
			progress.bytes_received);
		progress.status = FOTA_STATUS_DOWNLOADED;

		/* Verificar integridad de la imagen (MCUboot header/trailer) */
		if (IS_ENABLED(CONFIG_IMG_ENABLE_IMAGE_CHECK)) {
			struct flash_img_check fic = { .match = NULL, .clen = 0 };
			ret = flash_img_check(&flash_ctx, &fic,
					      FIXED_PARTITION_ID(slot1_partition));
			if (ret) {
				LOG_ERR("FOTA: Image integrity check FAILED: %d", ret);
				progress.status = FOTA_STATUS_ERROR;
				progress.last_error = ret;
				lwm2m_set_u8(&LWM2M_OBJ(5, 0, 3), STATE_IDLE);
				lwm2m_set_u8(&LWM2M_OBJ(5, 0, 5),
					     RESULT_INTEGRITY_FAILED);
				notify_progress();
				return ret;
			}
		}

		lwm2m_set_u8(&LWM2M_OBJ(5, 0, 3), STATE_DOWNLOADED);
		lwm2m_set_u8(&LWM2M_OBJ(5, 0, 5), RESULT_DEFAULT);
	}

	notify_progress();
	return 0;
}

/* --- Callback: ejecutar actualización --- */
static int fota_update_cb(uint16_t obj_inst_id,
			  uint8_t *args, uint16_t args_len)
{
	int ret;

	LOG_INF("FOTA: Update requested — marking image for test swap");
	progress.status = FOTA_STATUS_UPDATING;
	notify_progress();

	/* Marcar imagen en slot1 como "pendiente de test" */
	ret = boot_request_upgrade(BOOT_UPGRADE_TEST);
	if (ret) {
		LOG_ERR("FOTA: boot_request_upgrade failed: %d", ret);
		progress.status = FOTA_STATUS_ERROR;
		progress.last_error = ret;
		lwm2m_set_u8(&LWM2M_OBJ(5, 0, 3), STATE_IDLE);
		lwm2m_set_u8(&LWM2M_OBJ(5, 0, 5), RESULT_UPDATE_FAILED);
		notify_progress();
		return ret;
	}

	lwm2m_set_u8(&LWM2M_OBJ(5, 0, 3), STATE_UPDATING);
	LOG_INF("FOTA: Image marked for swap — rebooting in 3s...");
	progress.status = FOTA_STATUS_REBOOTING;
	notify_progress();

	/* Delay antes de reboot para que la notificación LwM2M salga */
	k_sleep(K_SECONDS(3));
	sys_reboot(SYS_REBOOT_COLD);

	/* No se llega aquí */
	return 0;
}

/* --- Callback: cancelar descarga --- */
static int fota_cancel_cb(uint16_t obj_inst_id)
{
	LOG_INF("FOTA: Update cancelled by server");
	progress.status = FOTA_STATUS_IDLE;
	progress.bytes_received = 0;
	progress.total_size = 0;
	progress.percent = 0;

	lwm2m_set_u8(&LWM2M_OBJ(5, 0, 3), STATE_IDLE);
	lwm2m_set_u8(&LWM2M_OBJ(5, 0, 5), RESULT_DEFAULT);
	notify_progress();
	return 0;
}

/* === API pública === */

int fota_init(fota_progress_cb_t cb)
{
	user_cb = cb;
	progress.status = FOTA_STATUS_IDLE;
	progress.bytes_received = 0;
	progress.total_size = 0;
	progress.percent = 0;
	progress.last_error = 0;

	/* Verificar/confirmar imagen actual */
	confirm_image_if_needed();

	/* Configurar Object 5 (Firmware Update) */

	/* Buffer para recepción de bloques (resource 5/0/0: Package) */
	lwm2m_register_pre_write_callback(&LWM2M_OBJ(5, 0, 0), fota_get_buf);
	lwm2m_firmware_set_write_cb(fota_block_received_cb);

	/* Callback cancelar */
	lwm2m_firmware_set_cancel_cb(fota_cancel_cb);

	/* Callback ejecutar update (resource 5/0/2: Update) */
	lwm2m_firmware_set_update_cb(fota_update_cb);

	/* Protocolos soportados (resource 5/0/8) — CoAP */
	lwm2m_create_res_inst(&LWM2M_OBJ(5, 0, 8, 0));
	lwm2m_set_res_buf(&LWM2M_OBJ(5, 0, 8, 0),
			  &supported_protocols[0],
			  sizeof(supported_protocols[0]),
			  sizeof(supported_protocols[0]), 0);

	/* Firmware version en Object 5 (resource 5/0/6) */
	lwm2m_set_string(&LWM2M_OBJ(5, 0, 6), FW_VERSION);

	/* Estado inicial */
	lwm2m_set_u8(&LWM2M_OBJ(5, 0, 3), STATE_IDLE);
	lwm2m_set_u8(&LWM2M_OBJ(5, 0, 5), RESULT_DEFAULT);

	LOG_INF("FOTA: Initialized — fw v%s, MCUboot confirmed=%d",
		FW_VERSION, image_confirmed);

	return 0;
}

fota_status_t fota_get_status(void)
{
	return progress.status;
}

void fota_get_progress(struct fota_progress *out)
{
	if (out) {
		*out = progress;
	}
}

const char *fota_get_current_version(void)
{
	return FW_VERSION;
}

bool fota_is_confirmed(void)
{
	return image_confirmed;
}
