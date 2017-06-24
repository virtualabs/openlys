#ifndef __INC_BLE_SERVICE_BLACKBADGE_H
#define __INC_BLE_SERVICE_BLACKBADGE_H

#include <stdint.h>
#include "ble.h"

#define GSS_BTN_SERVICE_UUID 0x2801
#define GSS_BTN1_CHAR_UUID 0x2491
#define GSS_BTN2_CHAR_UUID 0x2492

#define GSS_UNK_SERVICE_UUID 0x1523
#define GSS_UNK1_CHAR_UUID 0x1524
#define GSS_UNK2_CHAR_UUID 0x1525
#define GSS_UNK3_CHAR_UUID 0x1526

#define GSS_FIRMWARE_UUID       0x2568
#define GSS_OTA_CHAR_UUID       0x1990
#define GSS_VERSION_CHAR_UUID   0x1991
#define GSS_BUILD_CHAR_UUID     0x1992
#define GSS_ERRFLAG_CHAR_UUID   0x1993
#define GSS_ERRLINE_CHAR_UUID   0x1994
#define GSS_ERRCODE_CHAR_UUID   0x1995
#define GSS_ERRFILE_CHAR_UUID   0x1996
#define GSS_ERRUNK_CHAR_UUID    0x1997
#define GSS_ERRDISC_CHAR_UUID   0x1998

typedef struct ble_gss_s ble_gss_t;

/**@brief Immediate Alert Service structure. This contains various status information for the
 *        service. */
struct ble_gss_s
{
    uint16_t                  btn_service_handle;           /**< Handle of Immediate Alert Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t  button_handle;      /**< Handles related to the Alert Level characteristic. */
    ble_gatts_char_handles_t  button2_handle;      /**< Handles related to the Alert Level characteristic. */

    uint16_t                  unk_service_handle;           /**< Handle of Immediate Alert Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t  unk1_handle;      /**< Handles related to the Alert Level characteristic. */
    ble_gatts_char_handles_t  unk2_handle;      /**< Handles related to the Alert Level characteristic. */
    ble_gatts_char_handles_t  unk3_handle;      /**< Handles related to the Alert Level characteristic. */

    uint16_t                  fw_service_handle;           /**< Handle of Immediate Alert Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t  ota_handle;      /**< Handles related to the Alert Level characteristic. */
    ble_gatts_char_handles_t  version_handle;      /**< Handles related to the Alert Level characteristic. */
    ble_gatts_char_handles_t  build_handle;      /**< Handles related to the Alert Level characteristic. */
    ble_gatts_char_handles_t  errflag_handle;
    ble_gatts_char_handles_t  errline_handle;
    ble_gatts_char_handles_t  errcode_handle;
    ble_gatts_char_handles_t  errfile_handle;
    ble_gatts_char_handles_t  errunk_handle;
    ble_gatts_char_handles_t  errdisc_handle;

    uint16_t                  conn_handle;              /**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection). */
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
uint32_t ble_gss_init(ble_gss_t * p_bbs);

void ble_gss_on_ble_evt(ble_gss_t * p_gss, ble_evt_t * p_ble_evt);
void ble_gss_button_notify(ble_gss_t *p_gss);

#endif /* __INC_BLE_SERVICE_BLACKBADGE_H */
