#include "gss.h"
#include <string.h>
#include "ble_srv_common.h"
#include "app_error.h"


/**@brief 128-bit UUID base List. */
static const ble_uuid128_t m_btn_uuid128 =
{
   {
       0x45, 0xfa, 0x56, 0xc1, 0xfb, 0x1b, 0xc0, 0x28,
       0x96, 0xa8, 0x67, 0x18, 0x01, 0x28, 0x17, 0x4f
   }
};


static const ble_uuid128_t m_unk_uuid128 =
{
   {
       0x02, 0xdf, 0x1e, 0x9e, 0x39, 0x6d, 0x0d, 0xfc,
       0x1d, 0x7e, 0xb2, 0x99, 0x23, 0x15, 0xad, 0xb0
   }
};

static const ble_uuid128_t m_firmware_uuid128 =
{
   {
       0x3a, 0x6e, 0xb3, 0xed, 0x0e, 0xaf, 0xe9, 0xf8,
       0x8c, 0x7f, 0x77, 0x66, 0x68, 0x25, 0x12, 0x7b
   }
};



/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_bbs       Immediate Alert Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_connect(ble_gss_t * p_gss, ble_evt_t * p_ble_evt)
{
    p_gss->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_ias       Immediate Alert Service structure.
 * @param[in]   p_ble_evt   Event received from the BLE stack.
 */
static void on_write(ble_gss_t * p_gss, ble_evt_t * p_ble_evt)
{
    //ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
}

static void on_disconnect(ble_gss_t * p_bbs, ble_evt_t * p_ble_evt)
{
}


void ble_gss_on_ble_evt(ble_gss_t * p_gss, ble_evt_t * p_ble_evt)
{
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_gss, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_gss, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_gss, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


static uint32_t gss_btn_button_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t button[1];

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));
    memset(button, 0, 1);

    char_md.char_props.read   = 1;
    char_md.char_props.write = 0;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_BTN1_CHAR_UUID;

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
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 1;
    attr_char_value.p_value   = button;

    err_code = sd_ble_gatts_characteristic_add(p_gss->btn_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->button_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_btn_button2_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t button[1];

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));
    memset(button, 0, 1);


    char_md.char_props.read   = 1;
    char_md.char_props.write = 0;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_BTN2_CHAR_UUID;

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
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 1;
    attr_char_value.p_value   = button;

    err_code = sd_ble_gatts_characteristic_add(p_gss->btn_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->button2_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

/*** Unknown service characteristics ***/

static uint32_t gss_unk_unk1_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t button[1];

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));
    memset(button, 0, 1);


    char_md.char_props.read   = 1;
    char_md.char_props.write = 0;
    char_md.char_props.notify = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_UNK1_CHAR_UUID;

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
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 1;
    attr_char_value.p_value   = button;

    err_code = sd_ble_gatts_characteristic_add(p_gss->unk_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->unk1_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_unk_unk2_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t button[1];

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));
    memset(button, 0, 1);


    char_md.char_props.read   = 1;
    char_md.char_props.write = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_UNK2_CHAR_UUID;

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
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 1;
    attr_char_value.p_value   = button;

    err_code = sd_ble_gatts_characteristic_add(p_gss->unk_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->unk2_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_unk_unk3_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t button[1];

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));
    memset(button, 0, 1);


    char_md.char_props.read   = 1;
    char_md.char_props.write = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_UNK3_CHAR_UUID;

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
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 1;
    attr_char_value.p_value   = button;

    err_code = sd_ble_gatts_characteristic_add(p_gss->unk_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->unk3_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

/*** Firmware Service Characteristics. */
static uint32_t gss_fw_ota_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t ota[2];

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));
    memset(ota, 0, 2);


    char_md.char_props.read   = 0;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_OTA_CHAR_UUID;

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
    attr_char_value.init_len  = 2;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 2;
    attr_char_value.p_value   = ota;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->ota_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_fw_version_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t version[2] = {0x01, 0x06};

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));


    char_md.char_props.read   = 1;
    char_md.char_props.write  = 0;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_VERSION_CHAR_UUID;

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
    attr_char_value.init_len  = 2;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 2;
    attr_char_value.p_value   = version;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->version_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_fw_build_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t build[] = "OpenGablys Version 1.0";

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));


    char_md.char_props.read   = 1;
    char_md.char_props.write  = 0;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_BUILD_CHAR_UUID;

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
    attr_char_value.init_len  = strlen((char *)build)+1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = strlen((char *)build)+1;
    attr_char_value.p_value   = build;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->build_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_fw_errflag_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint32_t errflag = 0x0;

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));


    char_md.char_props.read   = 1;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_ERRFLAG_CHAR_UUID;

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
    attr_char_value.init_len  = 4;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 4;
    attr_char_value.p_value   = (uint8_t *)&errflag;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->errflag_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_fw_errline_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint32_t errline = 0xfb62F7fd;

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));


    char_md.char_props.read   = 1;
    char_md.char_props.write  = 0;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_ERRLINE_CHAR_UUID;

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
    attr_char_value.init_len  = 4;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 4;
    attr_char_value.p_value   = (uint8_t *)&errline;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->errline_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_fw_errcode_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint32_t errcode = 0x0;

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));


    char_md.char_props.read   = 1;
    char_md.char_props.write  = 0;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_ERRLINE_CHAR_UUID;

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
    attr_char_value.init_len  = 4;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 4;
    attr_char_value.p_value   = (uint8_t *)&errcode;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->errcode_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_fw_errfile_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t errfile[] = "nope, Chuck Testa.";

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));


    char_md.char_props.read   = 1;
    char_md.char_props.write  = 0;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_ERRLINE_CHAR_UUID;

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
    attr_char_value.init_len  = strlen((char *)errfile)+1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = strlen((char *)errfile)+1;
    attr_char_value.p_value   = errfile;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->errfile_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}

static uint32_t gss_fw_errunk_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint32_t errunk = 0x0;

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));


    char_md.char_props.read   = 0;
    char_md.char_props.write  = 1;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_ERRUNK_CHAR_UUID;

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
    attr_char_value.init_len  = 4;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 4;
    attr_char_value.p_value   = (uint8_t *)&errunk;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->errunk_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}


static uint32_t gss_fw_errdisc_char_add(ble_gss_t * p_gss, uint8_t uuid_type)
{
    uint32_t            err_code;
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          char_uuid;
    ble_gatts_attr_md_t attr_md;
    uint8_t errdisc = 0;

    memset(&cccd_md, 0, sizeof(cccd_md));

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    memset(&char_md, 0, sizeof(char_md));

    char_md.char_props.read   = 1;
    char_md.char_props.write  = 0;
    char_md.char_props.notify = 0;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;

    char_uuid.type = uuid_type;
    char_uuid.uuid = GSS_ERRDISC_CHAR_UUID;

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
    attr_char_value.init_len  = 1;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = 1;
    attr_char_value.p_value   = &errdisc;

    err_code = sd_ble_gatts_characteristic_add(p_gss->fw_service_handle,
                                               &char_md,
                                               &attr_char_value,
                                               &p_gss->errdisc_handle);
    APP_ERROR_CHECK(err_code);
    return err_code;
}


uint32_t ble_gss_init(ble_gss_t * p_gss)
{
    uint32_t   err_code;
    ble_uuid_t ble_btn_uuid;
    ble_uuid_t ble_unk_uuid;
    ble_uuid_t ble_fw_uuid;

    /* Add Gablys Button service. */
    ble_btn_uuid.uuid = GSS_BTN_SERVICE_UUID;
    err_code = sd_ble_uuid_vs_add(&m_btn_uuid128, &ble_btn_uuid.type);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                            &ble_btn_uuid,
                            &p_gss->btn_service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = gss_btn_button_char_add(p_gss, ble_btn_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_btn_button2_char_add(p_gss, ble_btn_uuid.type);
    APP_ERROR_CHECK(err_code);

    /* Add unknown service. */
    ble_unk_uuid.uuid = GSS_UNK_SERVICE_UUID;
    err_code = sd_ble_uuid_vs_add(&m_unk_uuid128, &ble_unk_uuid.type);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                            &ble_unk_uuid,
                            &p_gss->unk_service_handle);


    err_code = gss_unk_unk1_char_add(p_gss, ble_unk_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_unk_unk2_char_add(p_gss, ble_unk_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_unk_unk3_char_add(p_gss, ble_unk_uuid.type);
    APP_ERROR_CHECK(err_code);

    /* Add Firmware service. */
    ble_fw_uuid.uuid = GSS_FIRMWARE_UUID;
    err_code = sd_ble_uuid_vs_add(&m_firmware_uuid128, &ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                            &ble_fw_uuid,
                            &p_gss->fw_service_handle);

    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    err_code = gss_fw_ota_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_fw_version_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_fw_build_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);

    err_code = gss_fw_errflag_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_fw_errline_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_fw_errcode_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_fw_errfile_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_fw_errunk_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);
    err_code = gss_fw_errdisc_char_add(p_gss, ble_fw_uuid.type);
    APP_ERROR_CHECK(err_code);

    return err_code;
}

void ble_gss_button_notify(ble_gss_t *p_gss){
    ble_gatts_hvx_params_t params;
    uint8_t button_state = 0;
    uint16_t len = sizeof(button_state);

    memset(&params, 0, sizeof(params));
    params.type = BLE_GATT_HVX_NOTIFICATION;
    params.handle = p_gss->button_handle.value_handle;
    params.p_data = &button_state;
    params.p_len = &len;

    sd_ble_gatts_hvx(p_gss->conn_handle, &params);
}
