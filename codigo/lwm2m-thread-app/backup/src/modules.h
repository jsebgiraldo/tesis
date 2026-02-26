/*
 * modules.h — AMI LwM2M Client module declarations
 */

#ifndef MODULES_H_
#define MODULES_H_

/**
 * Initialize AMI-specific LwM2M objects:
 *   Object 3316 (Voltage Sensor)
 *   Object 3317 (Current Sensor)
 *   Object 3300 instance 0 (Energy kWh)
 *   Object 3300 instance 1 (Power Factor)
 */
int init_ami_objects(void);

/**
 * Initialize LwM2M firmware update object (Object 5).
 */
int init_firmware_update(void);

/**
 * Initialize LED control via LwM2M Object 3311 (Light Control).
 */
int init_led_device(void);

/**
 * Inject the raw Active Dataset TLVs from the OTBR into OpenThread,
 * so the node joins the exact same partition (byte-for-byte match).
 *
 * Must be called AFTER OpenThread is initialized but BEFORE Thread starts.
 * With Zephyr's conn_mgr / OpenThread L2, the dataset can also be set
 * via Kconfig — this function overrides with raw TLVs for guaranteed match.
 */
int thread_inject_dataset(void);

#endif /* MODULES_H_ */
