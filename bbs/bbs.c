#include "bbs.h"
#include <string.h>
#include "ble_srv_common.h"
#include "app_error.h"

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_bbs       Immediate Alert Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_bbs_t * p_bbs, ble_evt_t * p_ble_evt)
{
    p_bbs->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_ias       Immediate Alert Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_bbs_t * p_bbs, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if ((p_evt_write->handle == p_bbs->password_handles.value_handle))
    {
        /* Password write, call event handler. */
        ble_bbs_evt_t evt;

        evt.evt_type           = BLE_BBS_EVT_PASSWORD_UPDATED;
        memcpy(evt.params.password, p_evt_write->data, p_evt_write->len);

        p_bbs->evt_handler(p_bbs, &evt);
    }
}

static void on_disconnect(ble_bbs_t * p_bbs, ble_evt_t * p_ble_evt)
{
    uint8_t password[16];

    /* Reset password. */
    memset(password, 0, 16);
    ble_bbs_password_set(p_bbs, password, 16);
}


void ble_bbs_on_ble_evt(ble_bbs_t * p_bbs, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_bbs, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_bbs, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_bbs, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for adding Alert Level characteristics.
 *
 * @param[in]   p_ias        Immediate Alert Service structure.
 * @param[in]   p_ias_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t password_char_add(ble_bbs_t * p_bbs, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    ble_gatts_char_pf_t attr_pf;
    uint8_t password[16];

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));
    memset(&attr_pf, 0, sizeof(attr_pf));
    memset(password, 0, 16);

    attr_pf.format = BLE_GATT_CPF_FORMAT_UTF8S;


    char_md.char_props.read   = 1;
    char_md.char_props.write = 1;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = &attr_pf;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = BBS_PASSWORD_CHAR_UUID;

    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth = 0;
    attr_md.wr_auth = 0;
    attr_md.vlen    = 0;

    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 16;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 16;
    attr_char_value.p_value   = password;

    err_code = sd_ble_gatts_characteristic_add(p_bbs->service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_bbs->password_handles);
    APP_ERROR_CHECK(err_code);
    return err_code;
}


uint32_t ble_bbs_init(ble_bbs_t * p_bbs, const ble_bbs_init_t * p_bbs_init)
{
    uint32_t   err_code;
    ble_uuid_t ble_uuid;

    // Initialize service structure
    if (p_bbs_init->evt_handler == NULL)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    p_bbs->evt_handler = p_bbs_init->evt_handler;

    // Add service
    ble_uuid.uuid = BBS_SERVICE_UUID;
    err_code = sd_ble_uuid_vs_add(&m_base_uuid128, &ble_uuid.type);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_bbs->service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    // Add alert level characteristic
    return password_char_add(p_bbs, ble_uuid.type);
}


uint32_t ble_bbs_password_get(ble_bbs_t * p_bbs, uint8_t * p_password)
{
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = 16;
    gatts_value.offset  = 0;
    gatts_value.p_value = p_password;

    return sd_ble_gatts_value_get(p_bbs->conn_handle,
                                  p_bbs->password_handles.value_handle,
                                  &gatts_value);
}

uint32_t ble_bbs_password_set(ble_bbs_t * p_bbs, uint8_t * p_password, uint8_t len)
{
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = len;
    gatts_value.offset  = 0;
    gatts_value.p_value = p_password;

    return sd_ble_gatts_value_set(p_bbs->conn_handle,
                                  p_bbs->password_handles.value_handle,
                                  &gatts_value);
}
