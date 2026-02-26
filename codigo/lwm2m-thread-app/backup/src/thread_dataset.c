/*
 * thread_dataset.c — Inject raw Active Dataset TLVs
 *
 * Copies the pre-built Active Dataset (exported from the OTBR) byte-for-byte
 * into the OpenThread stack using otDatasetSetActiveTlvs().
 *
 * This ensures the XIAO ESP32-C6 node joins the exact same Thread partition
 * as the OTBR, with identical Security Policy, PSKc, Timestamp, etc.
 *
 * Network: AMI-Pilot-2025  Channel: 25  PAN: 0xABCD
 */

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/net/openthread.h>

#include <openthread/dataset.h>
#include <openthread/instance.h>
#include <openthread/ip6.h>
#include <openthread/thread.h>

LOG_MODULE_DECLARE(ami_lwm2m_client, LOG_LEVEL_DBG);

/*
 * Active Dataset — raw TLV encoding exported from the OTBR.
 * Must match EXACTLY for partition merge.
 *
 * Decoded contents:
 *   Active Timestamp: 1
 *   Channel:          25
 *   Channel Mask:     0x07fff800
 *   Ext PAN ID:       fdc6:63fd:328d:66df
 *   Mesh Local Pfix:  fdc6:63fd:328d:66df::/64
 *   Network Key:      00112233445566778899aabbccddeeff
 *   Network Name:     AMI-Pilot-2025
 *   PAN ID:           0xABCD
 *   PSKc:             1234567890abcdef (derived)
 *   Security Policy:  672 onrc 0
 */
static const uint8_t otbr_dataset_raw[] = {
	0x0e, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
	0x00, 0x00, 0x4a, 0x03, 0x00, 0x00, 0x0d, 0x35,
	0x06, 0x00, 0x04, 0x00, 0x1f, 0xff, 0xe0, 0x07,
	0x08, 0xfd, 0xc6, 0x63, 0xfd, 0x32, 0x8d, 0x66,
	0xdf, 0x04, 0x10, 0x9d, 0x53, 0x61, 0xd2, 0x4a,
	0x2d, 0x51, 0x80, 0x78, 0xeb, 0x9f, 0x8b, 0xc1,
	0x96, 0x5b, 0x80, 0x0c, 0x04, 0x02, 0xa0, 0xf7,
	0xf8, 0x00, 0x03, 0x00, 0x19, 0x01, 0x02, 0xab,
	0xcd, 0x02, 0x08, 0x12, 0x34, 0x56, 0x78, 0x90,
	0xab, 0xcd, 0xef, 0x03, 0x0e, 0x41, 0x4d, 0x49,
	0x2d, 0x50, 0x69, 0x6c, 0x6f, 0x74, 0x2d, 0x32,
	0x30, 0x32, 0x35, 0x05, 0x10, 0x00, 0x11, 0x22,
	0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa,
	0xbb, 0xcc, 0xdd, 0xee, 0xff
};

int thread_inject_dataset(void)
{
	otError err;
	otOperationalDatasetTlvs dataset_tlvs;
	struct openthread_context *ot_ctx;

	ot_ctx = openthread_get_default_context();
	if (!ot_ctx) {
		LOG_ERR("Failed to get OpenThread context");
		return -ENODEV;
	}

	if (sizeof(otbr_dataset_raw) > sizeof(dataset_tlvs.mTlvs)) {
		LOG_ERR("Dataset too large (%zu > %zu)",
			sizeof(otbr_dataset_raw),
			sizeof(dataset_tlvs.mTlvs));
		return -ENOSPC;
	}

	memcpy(dataset_tlvs.mTlvs, otbr_dataset_raw,
	       sizeof(otbr_dataset_raw));
	dataset_tlvs.mLength = sizeof(otbr_dataset_raw);

	openthread_api_mutex_lock(ot_ctx);

	/* Try direct injection first */
	err = otDatasetSetActiveTlvs(ot_ctx->instance, &dataset_tlvs);

	if (err == OT_ERROR_INVALID_STATE) {
		/*
		 * Thread auto-started before main(). Must tear down the stack,
		 * inject dataset, and restart.  Order matters:
		 *   1. otThreadSetEnabled(false)
		 *   2. otIp6SetEnabled(false)
		 *   3. otDatasetSetActiveTlvs()
		 *   4. otIp6SetEnabled(true)
		 *   5. otThreadSetEnabled(true)
		 */
		LOG_WRN("Thread active — stopping for dataset injection");

		otError e = otThreadSetEnabled(ot_ctx->instance, false);
		if (e != OT_ERROR_NONE) {
			LOG_ERR("otThreadSetEnabled(false): %d", e);
		}

		e = otIp6SetEnabled(ot_ctx->instance, false);
		if (e != OT_ERROR_NONE) {
			LOG_ERR("otIp6SetEnabled(false): %d", e);
		}

		err = otDatasetSetActiveTlvs(ot_ctx->instance, &dataset_tlvs);
		if (err != OT_ERROR_NONE) {
			LOG_ERR("otDatasetSetActiveTlvs (2nd attempt): %d", err);
			openthread_api_mutex_unlock(ot_ctx);
			return -EIO;
		}

		e = otIp6SetEnabled(ot_ctx->instance, true);
		if (e != OT_ERROR_NONE) {
			LOG_ERR("otIp6SetEnabled(true): %d", e);
		}

		e = otThreadSetEnabled(ot_ctx->instance, true);
		if (e != OT_ERROR_NONE) {
			LOG_ERR("otThreadSetEnabled(true): %d", e);
		}
	} else if (err != OT_ERROR_NONE) {
		LOG_ERR("otDatasetSetActiveTlvs failed: %d", err);
		openthread_api_mutex_unlock(ot_ctx);
		return -EIO;
	}

	openthread_api_mutex_unlock(ot_ctx);

	LOG_INF("Active Dataset injected (%zu bytes)", sizeof(otbr_dataset_raw));
	return 0;
}
