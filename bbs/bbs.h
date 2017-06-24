#ifndef __INC_BLE_SERVICE_BLACKBADGE_H
#define __INC_BLE_SERVICE_BLACKBADGE_H

#include <stdint.h>
#include "ble.h"

#define BBS_SERVICE_UUID 0x1337
#define BBS_PASSWORD_CHAR_UUID 0x4242

/**@brief 128-bit UUID base List. */
static const ble_uuid128_t m_base_uuid128 =
{
   {
       0x28, 0xD4, 0xE9, 0xDE, 0xBE, 0x94, 0x40, 0x3A,
       0xB7, 0xD1, 0x41, 0x4C, 0x00, 0x00, 0x00, 0x00
   }
};

/**@brief Black Badge Service event type. */
typedef enum
{
    BLE_BBS_EVT_PASSWORD_UPDATED,
    BLE_BBS_EVT_PASSWORD_READ
} ble_bbs_evt_type_t;

/**@brief Black Badge Service event. */
typedef struct
{
    ble_bbs_evt_type_t evt_type;                        /**< Type of event. */
    union
    {
        uint8_t password[16];                            /**< New Alert Level value. */
    } params;
} ble_bbs_evt_t;

// Forward declaration of the ble_ias_t type.
typedef struct ble_bbs_s ble_bbs_t;

/**@brief Black Badge Service event handler type. */
typedef void (*ble_bbs_evt_handler_t) (ble_bbs_t * p_ias, ble_bbs_evt_t * p_evt);

/**@brief Immediate Alert Service init structure. This contains all options and data needed for
 *        initialization of the service. */
typedef struct
{
    ble_bbs_evt_handler_t evt_handler;                  /**< Event handler to be called for handling events in the Immediate Alert Service. */
} ble_bbs_init_t;

/**@brief Immediate Alert Service structure. This contains various status information for the
 *        service. */
struct ble_bbs_s
{
    ble_bbs_evt_handler_t     evt_handler;              /**< Event handler to be called for handling events in the Immediate Alert Service. */
    uint16_t                  service_handle;           /**< Handle of Immediate Alert Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t  password_handles;      /**< Handles related to the Alert Level characteristic. */
    uint16_t                  conn_handle;              /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
    uint8_t                   unlocked;
};

/**@brief Function for initializing the Immediate Alert Service.
 *
 * @param[out]  p_ias       Immediate Alert Service structure. This structure will have to be
 *                          supplied by the application. It will be initialized by this function,
 *                          and will later be used to identify this particular service instance.
 * @param[in]   p_ias_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_bbs_init(ble_bbs_t * p_bbs, const ble_bbs_init_t * p_bbs_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Immediate Alert Service.
 *
 * @param[in]   p_ias      Immediate Alert Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_bbs_on_ble_evt(ble_bbs_t * p_bbs, ble_evt_t * p_ble_evt);

/**@brief Function for getting current value of the Alert Level characteristic.
 *
 * @param[in]   p_ias          Immediate Alert Service structure.
 * @param[out]  p_alert_level  Current Alert Level value.
 */
uint32_t ble_bbs_password_get(ble_bbs_t * p_bbs, uint8_t *password);
uint32_t ble_bbs_password_set(ble_bbs_t * p_bbs, uint8_t * p_password, uint8_t len);



#endif /* __INC_BLE_SERVICE_BLACKBADGE_H */
