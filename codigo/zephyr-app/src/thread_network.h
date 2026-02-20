/*
 * thread_network.h — OpenThread network management
 */

#ifndef THREAD_NETWORK_H_
#define THREAD_NETWORK_H_

#include <openthread/instance.h>
#include <openthread/thread.h>

/**
 * @brief Callback para cambios de estado Thread
 */
typedef void (*thread_state_cb_t)(otChangedFlags flags, void *context);

/**
 * @brief Inicializar OpenThread y comenzar proceso de join
 * 
 * Configura el stack OpenThread como FTD (Full Thread Device),
 * aplica dataset de red y habilita Thread interface.
 * 
 * @param state_cb Callback para notificar cambios de estado
 * @return 0 en éxito, código de error negativo en fallo
 */
int thread_network_init(thread_state_cb_t state_cb);

/**
 * @brief Obtener rol actual del dispositivo en la red Thread
 * @return otDeviceRole (DISABLED, DETACHED, CHILD, ROUTER, LEADER)
 */
otDeviceRole thread_get_role(void);

/**
 * @brief Verificar si el dispositivo está conectado a la red Thread
 * @return true si está attached (CHILD, ROUTER o LEADER)
 */
bool thread_is_connected(void);

/**
 * @brief Obtener RLOC16 del dispositivo
 * @return RLOC16 address (0 si no conectado)
 */
uint16_t thread_get_rloc16(void);

/**
 * @brief Obtener RSSI del último paquete recibido
 * @return RSSI en dBm
 */
int8_t thread_get_rssi(void);

#endif /* THREAD_NETWORK_H_ */
