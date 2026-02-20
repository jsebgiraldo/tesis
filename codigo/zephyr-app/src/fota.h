/*
 * fota.h — Firmware Over-The-Air (FOTA) via LwM2M Object 5
 *
 * Soporta actualización de firmware usando:
 * - MCUboot bootloader (dual-slot swap con rollback)
 * - LwM2M Firmware Update Object (ID 5)
 * - CoAP Block-wise Transfer para recepción de imagen
 * - Verificación SHA256 antes de marcar imagen pendiente
 *
 * Flujo OTA:
 * 1. Servidor escribe URI en /5/0/1 (Package URI) — modo PULL
 *    o escribe bloques en /5/0/0 (Package) — modo PUSH
 * 2. Firmware descarga/recibe imagen a flash slot1
 * 3. Servidor ejecuta /5/0/2 (Update) → marca imagen pendiente
 * 4. MCUboot reinicia → prueba slot1 → confirma o rollback
 */

#ifndef FOTA_H_
#define FOTA_H_

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Estado interno del proceso FOTA
 */
typedef enum {
	FOTA_STATUS_IDLE,         /* Sin actualización en curso */
	FOTA_STATUS_DOWNLOADING,  /* Recibiendo bloques de firmware */
	FOTA_STATUS_DOWNLOADED,   /* Descarga completa, pendiente de verificación */
	FOTA_STATUS_UPDATING,     /* Marcando imagen para swap en MCUboot */
	FOTA_STATUS_REBOOTING,    /* Reinicio programado */
	FOTA_STATUS_ERROR,        /* Error en el proceso */
} fota_status_t;

/**
 * @brief Información de progreso FOTA
 */
struct fota_progress {
	fota_status_t status;
	size_t bytes_received;
	size_t total_size;
	uint8_t percent;          /* 0-100 */
	int last_error;           /* Código de error si status == ERROR */
};

/**
 * @brief Callback para notificar progreso FOTA al sistema principal
 */
typedef void (*fota_progress_cb_t)(const struct fota_progress *progress);

/**
 * @brief Inicializar subsistema FOTA
 *
 * Registra callbacks de LwM2M Object 5, configura buffer de escritura
 * a flash, y verifica estado de imagen actual con MCUboot.
 *
 * Si la imagen actual es "test" (primera ejecución tras OTA),
 * la confirma automáticamente como válida.
 *
 * @param cb Callback de progreso (puede ser NULL)
 * @return 0 en éxito, código de error negativo en fallo
 */
int fota_init(fota_progress_cb_t cb);

/**
 * @brief Obtener estado actual del proceso FOTA
 * @return Estado actual
 */
fota_status_t fota_get_status(void);

/**
 * @brief Obtener progreso actual de descarga
 * @param progress Estructura de progreso a llenar
 */
void fota_get_progress(struct fota_progress *progress);

/**
 * @brief Obtener versión de firmware actual
 * @return String estático con versión (e.g., "0.2.0")
 */
const char *fota_get_current_version(void);

/**
 * @brief verificar si el rollback es necesario
 *
 * Llamar periódicamente después de OTA. Si el sistema detecta
 * problemas persistentes, no confirmará la imagen y MCUboot
 * hará rollback en el próximo reinicio.
 *
 * @return true si la imagen actual fue confirmada exitosamente
 */
bool fota_is_confirmed(void);

#endif /* FOTA_H_ */
