/**
 * Opensource Gablys firmware
 *
 * Nuit du Hack 2017 - Make hardware great again !
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "nordic_common.h"
#include "nrf.h"
#include "nrf_soc.h"
#include "app_error.h"
#include "app_button.h"
#include "ble.h"
#include "ble_hci.h"
#include "ble_srv_common.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "boards.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "device_manager.h"
#include "pstorage.h"
#include "app_trace.h"
#include "bsp.h"
#include "bsp_btn_ble.h"
#include "sensorsim.h"
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advertising.h"
#include "nrf_delay.h"
#include "ble_bas.h"
#include "ble_ias.h"
#include "ble_ias_c.h"
#include "ble_lls.h"
#include "ble_db_discovery.h"
#include "app_scheduler.h"
#include "app_timer_appsh.h"
#include "nrf_gpiote.h"

#include "gss.h"

#ifdef NRF51
#include "nrf_drv_adc.h"
#else
#include "nrf_drv_saadc.h"
#endif //NRF51

#include "nrf_gpio.h"
#include "app_gpiote.h"
#include "nrf_delay.h"
#include "piezzo.h"

#define GPIOTE_CHANNEL_0 0

#define c 261
#define d 294
#define e 329
#define f 349
#define g 391
#define gS 415
#define a 440
#define aS 455
#define b 466
#define cH 523
#define cSH 554
#define dH 587
#define dSH 622
#define eH 659
#define fH 698
#define fSH 740
#define gH 784
#define gSH 830
#define aH 880

#define BUZZ_PIN 17
#define SW_PIN1 25
#define SW_PIN2 24

void beep(int freq, long delay) {
    long delay_amount = (long)(1000000/freq);
    long x,looptime = (long)((delay*1000)/(delay_amount*2));
    for (x=0;x<looptime;x++) {
        nrf_gpio_pin_set(BUZZ_PIN);
        nrf_delay_us(delay_amount);
        nrf_gpio_pin_clear(BUZZ_PIN);
        nrf_delay_us(delay_amount);
    }
    nrf_delay_ms(20);
}

#define IS_SRVC_CHANGED_CHARACT_PRESENT  0                                         /**< Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device*/

#define CENTRAL_LINK_COUNT               0                                          /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT            1                                          /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/

#define DEVICE_NAME                      "GABLYS LITE"                          /**< Name of device. Will be included in the advertising data. */
#define MANUFACTURER_NAME                "Virtualabs"                             /**< Manufacturer. Will be passed to Device Information Service. */
#define APP_ADV_INTERVAL                 300                                        /**< The advertising interval (in units of 0.625 ms. This value corresponds to 25 ms). */
#define APP_ADV_TIMEOUT_IN_SECONDS       180                                        /**< The advertising timeout in units of seconds. */

#define BATTERY_LEVEL_MEAS_INTERVAL         APP_TIMER_TICKS(120000, APP_TIMER_PRESCALER) /**< Battery level measurement interval (ticks). This value corresponds to 120 seconds. */

#define APP_TIMER_PRESCALER              0                                          /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE          4                                          /**< Size of timer operation queues. */

#define MIN_CONN_INTERVAL                MSEC_TO_UNITS(100, UNIT_1_25_MS)           /**< Minimum acceptable connection interval (0.1 seconds). */
#define MAX_CONN_INTERVAL                MSEC_TO_UNITS(200, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (0.2 second). */
#define SLAVE_LATENCY                    0                                          /**< Slave latency. */
#define CONN_SUP_TIMEOUT                 MSEC_TO_UNITS(4000, UNIT_10_MS)            /**< Connection supervisory timeout (4 seconds). */

#define FIRST_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER) /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define NEXT_CONN_PARAMS_UPDATE_DELAY    APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER)/**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define MAX_CONN_PARAMS_UPDATE_COUNT     3                                          /**< Number of attempts before giving up the connection parameter negotiation. */

#define SEC_PARAM_BOND                   0                                          /**< Perform bonding. */
#define SEC_PARAM_MITM                   0                                          /**< Man In The Middle protection not required. */
#define SEC_PARAM_LESC                   0                                          /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS               0                                          /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES        BLE_GAP_IO_CAPS_NONE                       /**< No I/O capabilities. */
#define SEC_PARAM_OOB                    0                                          /**< Out Of Band data not available. */
#define SEC_PARAM_MIN_KEY_SIZE           7                                          /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE           16                                         /**< Maximum encryption key size. */

#define DEAD_BEEF                        0xDEADBEEF                                 /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#define ADC_REF_VBG_VOLTAGE_IN_MILLIVOLTS   1200                                         /**< Value in millivolts for voltage used as reference in ADC conversion on NRF51. */
#define ADC_INPUT_PRESCALER                 3                                            /**< Input prescaler for ADC convestion on NRF51. */
#define ADC_RES_10BIT                       1024
#define ADC_REF_VOLTAGE_IN_MILLIVOLTS       600                                          /**< Reference voltage (in milli volts) used by ADC while doing conversion. */
#define ADC_PRE_SCALING_COMPENSATION        6                                            /**< The ADC is configured to use VDD with 1/3 prescaling as input. And hence the result of conversion is to be multiplied by 3 to get the actual value of the battery voltage.*/
#define DIODE_FWD_VOLT_DROP_MILLIVOLTS      270                                          /**< Typical forward voltage drop of the diode (Part no: SD103ATW-7-F) that is connected in series with the voltage supply. This is the voltage drop when the forward current is 1mA. Source: Data sheet of 'SURFACE MOUNT SCHOTTKY BARRIER DIODE ARRAY' available at www.diodes.com. */

#define SCHED_MAX_EVENT_DATA_SIZE       sizeof(app_timer_event_t)                   /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#define SCHED_QUEUE_SIZE                10                                          /**< Maximum number of events in the scheduler queue. */

#define APP_GPIOTE_MAX_USERS            1

#define BUTTON_DEBOUNCE_DELAY			50

#define INITIAL_LLS_ALERT_LEVEL             BLE_CHAR_ALERT_LEVEL_NO_ALERT                /**< Initial value for the Alert Level characteristic in the Link Loss service. */

#ifdef NRF51
#define ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE)\
        ((((ADC_VALUE) * ADC_REF_VBG_VOLTAGE_IN_MILLIVOLTS) / ADC_RES_10BIT) * ADC_INPUT_PRESCALER)
#else // NRF52
#define ADC_RESULT_IN_MILLI_VOLTS(ADC_VALUE)\
        ((((ADC_VALUE) * ADC_REF_VOLTAGE_IN_MILLIVOLTS) / ADC_RES_10BIT) * ADC_PRE_SCALING_COMPENSATION)
#endif // NRF51

APP_TIMER_DEF(m_battery_timer_id);

static dm_application_instance_t         m_app_handle;                              /**< Application identifier allocated by device manager */

static uint16_t                          m_conn_handle = BLE_CONN_HANDLE_INVALID;   /**< Handle of the current connection. */
static ble_db_discovery_t               m_ble_db_discovery;                      /**< Structure used to identify the DB Discovery module. */
static ble_ias_t                        m_ias;                                   /**< Structure used to identify the Immediate Alert service. */
static ble_lls_t                        m_lls;                                   /**< Structure used to identify the Link Loss service. */

static ble_bas_t                        m_bas;                                   /**< Structure used to identify the battery service. */
static ble_ias_c_t                      m_ias_c;                                 /**< Structure used to identify the client to the Immediate Alert Service at peer. */
static volatile bool                    m_is_high_alert_signalled;               /**< Variable to indicate whether a high alert has been signalled to the peer. */
static volatile bool                    m_is_ias_present = false;                /**< Variable to indicate whether the immediate alert service has been discovered at the connected peer. */
static ble_gss_t                        m_gss;
static uint32_t app_ram_base;
static uint8_t gabs_type;

static uint8_t gPushed = 0;

/**@brief 128-bit UUID base List. */

static const ble_uuid128_t m_gablys_uuid =
{
   {
       0x45, 0xfa, 0x56, 0xc1, 0xfb, 0x1b, 0xc0, 0x28,
       0x96, 0xa8, 0x67, 0x18, 0x02, 0x28, 0x17, 0x4f
   }
};

static void on_ias_evt(ble_ias_t * p_ias, ble_ias_evt_t * p_evt);
static void on_lls_evt(ble_lls_t * p_lls, ble_lls_evt_t * p_evt);
static void on_ias_c_evt(ble_ias_c_t * p_lls, ble_ias_c_evt_t * p_evt);
static void on_bas_evt(ble_bas_t * p_bas, ble_bas_evt_t * p_evt);

#ifdef NRF51
static nrf_adc_value_t adc_buf[1];
#else
static nrf_saadc_value_t adc_buf[2];
#endif //NRF52

static piezzo_melody_t melody;
static piezzo_note_t notes[3] = {{784, 500},{0, 500},{784, 500}};

#ifdef NRF51
/**@brief Function for handling the ADC driver eevent.
 *
 * @details  This function will fetch the conversion result from the ADC, convert the value into
 *           percentage and send it to peer.
 */
void adc_event_handler(nrf_drv_adc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_ADC_EVT_DONE)
    {
        nrf_adc_value_t adc_result;
        uint16_t        batt_lvl_in_milli_volts;
        uint8_t         percentage_batt_lvl;
        uint32_t        err_code;

        adc_result = p_event->data.done.p_buffer[0];

        err_code = nrf_drv_adc_buffer_convert(p_event->data.done.p_buffer, 1);
        APP_ERROR_CHECK(err_code);

        batt_lvl_in_milli_volts = ADC_RESULT_IN_MILLI_VOLTS(adc_result) +
                                  DIODE_FWD_VOLT_DROP_MILLIVOLTS;
        percentage_batt_lvl = battery_level_in_percent(batt_lvl_in_milli_volts);

        err_code = ble_bas_battery_level_update(&m_bas, percentage_batt_lvl);
        if (
            (err_code != NRF_SUCCESS)
            &&
            (err_code != NRF_ERROR_INVALID_STATE)
            &&
            (err_code != BLE_ERROR_NO_TX_PACKETS)
            &&
            (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
           )
        {
            APP_ERROR_HANDLER(err_code);
        }
    }
}


#else // NRF52
/**@brief Function for handling the ADC interrupt.
 *
 * @details  This function will fetch the conversion result from the ADC, convert the value into
 *           percentage and send it to peer.
 */
void saadc_event_handler(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        nrf_saadc_value_t adc_result;
        uint16_t          batt_lvl_in_milli_volts;
        uint8_t           percentage_batt_lvl;
        uint32_t          err_code;

        adc_result = p_event->data.done.p_buffer[0];

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, 1);
        APP_ERROR_CHECK(err_code);

        batt_lvl_in_milli_volts = ADC_RESULT_IN_MILLI_VOLTS(adc_result) +
                                  DIODE_FWD_VOLT_DROP_MILLIVOLTS;
        percentage_batt_lvl = battery_level_in_percent(batt_lvl_in_milli_volts);

        err_code = ble_bas_battery_level_update(&m_bas, percentage_batt_lvl);
        if (
            (err_code != NRF_SUCCESS)
            &&
            (err_code != NRF_ERROR_INVALID_STATE)
            &&
            (err_code != BLE_ERROR_NO_TX_PACKETS)
            &&
            (err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING)
           )
        {
            APP_ERROR_HANDLER(err_code);
        }
    }
}


#endif // NRF51

/**@brief Function for configuring ADC to do battery level conversion.
 */
static void adc_configure(void)
{
    #ifdef NRF51
    ret_code_t err_code = nrf_drv_adc_init(NULL, adc_event_handler);
    APP_ERROR_CHECK(err_code);

    static nrf_drv_adc_channel_t channel = NRF_DRV_ADC_DEFAULT_CHANNEL(NRF_ADC_CONFIG_INPUT_DISABLED);
    //channel.config.config.input = NRF_ADC_CONFIG_SCALING_SUPPLY_ONE_THIRD;
    channel.config.config.input = (uint32_t)NRF_ADC_CONFIG_SCALING_SUPPLY_ONE_THIRD;
    nrf_drv_adc_channel_enable(&channel);

    err_code = nrf_drv_adc_buffer_convert(&adc_buf[0], 1);
    APP_ERROR_CHECK(err_code);
    #else //  NRF52
    ret_code_t err_code = nrf_drv_saadc_init(NULL, saadc_event_handler);
    APP_ERROR_CHECK(err_code);

    nrf_saadc_channel_config_t config = NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_VDD);
    err_code = nrf_drv_saadc_channel_init(0,&config);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[0], 1);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_saadc_buffer_convert(&adc_buf[1], 1);
    APP_ERROR_CHECK(err_code);
    #endif //NRF51
}

/**@brief Function for handling Service errors.
 *
 * @details A pointer to this function will be passed to each service which may need to inform the
 *          application about an error.
 *
 * @param[in] nrf_error   Error code containing information about what went wrong.
 */
static void service_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for handling the Battery measurement timer timeout.
 *
 * @details This function will be called each time the battery level measurement timer expires.
 *          This function will start the ADC.
 *
 * @param[in] p_context   Pointer used for passing some arbitrary information (context) from the
 *                        app_start_timer() call to the timeout handler.
 */
static void battery_level_meas_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    #ifdef NRF51
    nrf_drv_adc_sample();
    #else // NRF52
    uint32_t err_code;
    err_code = nrf_drv_saadc_sample();
    APP_ERROR_CHECK(err_code);
    #endif // NRF51
}


/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in] line_num   Line number of the failing ASSERT call.
 * @param[in] file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}


/**@brief Function for initializing the Immediate Alert Service.
 */
static void ias_init(void)
{
    uint32_t       err_code;
    ble_ias_init_t ias_init_obj;

    memset(&ias_init_obj, 0, sizeof(ias_init_obj));
    ias_init_obj.evt_handler = on_ias_evt;

    err_code = ble_ias_init(&m_ias, &ias_init_obj);
    APP_ERROR_CHECK(err_code);
}

static void gss_init(void)
{
    uint32_t       err_code;
    err_code = ble_gss_init(&m_gss);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Link Loss Service.
 */
static void lls_init(void)
{
    uint32_t       err_code;
    ble_lls_init_t lls_init_obj;

    // Initialize Link Loss Service
    memset(&lls_init_obj, 0, sizeof(lls_init_obj));

    lls_init_obj.evt_handler         = on_lls_evt;
    lls_init_obj.error_handler       = service_error_handler;
    lls_init_obj.initial_alert_level = INITIAL_LLS_ALERT_LEVEL;

    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&lls_init_obj.lls_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_ENC_NO_MITM(&lls_init_obj.lls_attr_md.write_perm);

    err_code = ble_lls_init(&m_lls, &lls_init_obj);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Battery Service.
 */
static void bas_init(void)
{
    uint32_t       err_code;
    ble_bas_init_t bas_init_obj;

    memset(&bas_init_obj, 0, sizeof(bas_init_obj));

    bas_init_obj.evt_handler          = on_bas_evt;
    bas_init_obj.support_notification = true;
    bas_init_obj.p_report_ref         = NULL;
    bas_init_obj.initial_batt_level   = 100;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_char_attr_md.cccd_write_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_char_attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&bas_init_obj.battery_level_char_attr_md.write_perm);

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&bas_init_obj.battery_level_report_read_perm);

    err_code = ble_bas_init(&m_bas, &bas_init_obj);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the immediate alert service client.
 *
 * @details This will initialize the client side functionality of the Find Me profile.
 */
static void ias_client_init(void)
{
    uint32_t         err_code;
    ble_ias_c_init_t ias_c_init_obj;

    memset(&ias_c_init_obj, 0, sizeof(ias_c_init_obj));

    m_is_high_alert_signalled = false;

    ias_c_init_obj.evt_handler   = on_ias_c_evt;
    ias_c_init_obj.error_handler = service_error_handler;

    err_code = ble_ias_c_init(&m_ias_c, &ias_c_init_obj);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for the Timer initialization.
 *
 * @details Initializes the timer module. This creates and starts application timers.
 */
static void timers_init(void)
{

    uint32_t err_code;

    //APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);

    // Create battery timer.
    err_code = app_timer_create(&m_battery_timer_id,
                                APP_TIMER_MODE_REPEATED,
                                battery_level_meas_timeout_handler);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name, appearance, and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_UNKNOWN);
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}

static void add_custom_services(void) {
    sd_ble_uuid_vs_add(&m_gablys_uuid, &gabs_type);
}


/**@brief Function for handling database discovery events.
 *
 * @details This function is callback function to handle events from the database discovery module.
 *          Depending on the UUIDs that are discovered, this function should forward the events
 *          to their respective services.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
    ble_ias_c_on_db_disc_evt(&m_ias_c, p_evt);
}


/** @brief Database discovery module initialization.
 */
static void db_discovery_init(void)
{
    uint32_t err_code = ble_db_discovery_init(db_disc_handler);

    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing services that will be used by the application.
 */
static void services_init(void)
{
    ias_init();
    lls_init();
    bas_init();
    ias_client_init();
    gss_init();
}


/**@brief Function for handling the Connection Parameters Module.
 *
 * @details This function will be called for all events in the Connection Parameters Module which
 *          are passed to the application.
 *          @note All this function does is to disconnect. This could have been done by simply
 *                setting the disconnect_on_fail config parameter, but instead we use the event
 *                handler mechanism to demonstrate its use.
 *
 * @param[in] p_evt  Event received from the Connection Parameters Module.
 */
static void on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    uint32_t err_code;

    if (p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}


/**@brief Function for handling a Connection Parameters error.
 *
 * @param[in] nrf_error  Error code containing information about what went wrong.
 */
static void conn_params_error_handler(uint32_t nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the Connection Parameters module.
 */
static void conn_params_init(void)
{
    uint32_t               err_code;
    ble_conn_params_init_t cp_init;

    memset(&cp_init, 0, sizeof(cp_init));

    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = on_conn_params_evt;
    cp_init.error_handler                  = conn_params_error_handler;

    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for putting the chip into sleep mode.
 *
 * @note This function will not return.
 */
static void sleep_mode_enter(void)
{
    uint32_t err_code = bsp_indication_set(BSP_INDICATE_IDLE);
    APP_ERROR_CHECK(err_code);

    // Prepare wakeup buttons.
    err_code = bsp_btn_ble_sleep_mode_prepare();
    APP_ERROR_CHECK(err_code);

    // Go to system-off mode (this function will not return; wakeup will cause a reset).
    err_code = sd_power_system_off();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
 static void on_adv_evt(ble_adv_evt_t ble_adv_evt)
{
    uint32_t err_code;

    switch (ble_adv_evt)
    {
        case BLE_ADV_EVT_FAST:
            err_code = bsp_indication_set(BSP_INDICATE_ADVERTISING);
            APP_ERROR_CHECK(err_code);
            break;
        case BLE_ADV_EVT_IDLE:
            sleep_mode_enter();
            break;
        default:
            break;
    }
}

void on_melody_end(void) {
    app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);
}

/**@brief Function for handling Immediate Alert events.
 *
 * @details This function will be called for all Immediate Alert events which are passed to the
 *          application.
 *
 * @param[in] p_ias  Immediate Alert structure.
 * @param[in] p_evt  Event received from the Immediate Alert service.
 */
static void on_ias_evt(ble_ias_t * p_ias, ble_ias_evt_t * p_evt)
{
    switch (p_evt->evt_type)
    {
        case BLE_IAS_EVT_ALERT_LEVEL_UPDATED:
            /* Async music plays here ! Trololo song FTW */
            app_timer_stop(m_battery_timer_id);
            melody.current = 0;
            play(&melody, (piezzo_cb *)on_melody_end);
            break;//BLE_IAS_EVT_ALERT_LEVEL_UPDATED

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for handling Link Loss events.
 *
 * @details This function will be called for all Link Loss events which are passed to the
 *          application.
 *
 * @param[in] p_lls  Link Loss structure.
 * @param[in] p_evt  Event received from the Link Loss service.
 */
static void on_lls_evt(ble_lls_t * p_lls, ble_lls_evt_t * p_evt)
{
    switch (p_evt->evt_type)
    {
        case BLE_LLS_EVT_LINK_LOSS_ALERT:
            //beep(fH, 500);
            break;//BLE_LLS_EVT_LINK_LOSS_ALERT

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for handling IAS Client events.
 *
 * @details This function will be called for all IAS Client events which are passed to the
 *          application.
 *
 * @param[in] p_ias_c  IAS Client structure.
 * @param[in] p_evt    Event received.
 */
static void on_ias_c_evt(ble_ias_c_t * p_ias_c, ble_ias_c_evt_t * p_evt)
{
    uint32_t err_code;

    switch (p_evt->evt_type)
    {
        case BLE_IAS_C_EVT_DISCOVERY_COMPLETE:
            // IAS is found on peer. The Find Me Locator functionality of this app will work.
            err_code = ble_ias_c_handles_assign(&m_ias_c,
                                                p_evt->conn_handle,
                                                p_evt->alert_level.handle_value);
            APP_ERROR_CHECK(err_code);

            m_is_ias_present = true;
            break;//BLE_IAS_C_EVT_DISCOVERY_COMPLETE

        case BLE_IAS_C_EVT_DISCOVERY_FAILED:
            // IAS is not found on peer. The Find Me Locator functionality of this app will NOT work.
            break;//BLE_IAS_C_EVT_DISCOVERY_FAILED

        case BLE_IAS_C_EVT_DISCONN_COMPLETE:
            // Disable alert buttons
            m_is_ias_present = false;
            break;//BLE_IAS_C_EVT_DISCONN_COMPLETE

        default:
            break;
    }
}


/**@brief Function for handling the Battery Service events.
 *
 * @details This function will be called for all Battery Service events which are passed to the
 |          application.
 *
 * @param[in] p_bas  Battery Service structure.
 * @param[in] p_evt  Event received from the Battery Service.
 */
static void on_bas_evt(ble_bas_t * p_bas, ble_bas_evt_t * p_evt)
{
    uint32_t err_code;

    switch (p_evt->evt_type)
    {
        case BLE_BAS_EVT_NOTIFICATION_ENABLED:
            // Start battery timer
            err_code = app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);
            APP_ERROR_CHECK(err_code);
            break;//BLE_BAS_EVT_NOTIFICATION_ENABLED

        case BLE_BAS_EVT_NOTIFICATION_DISABLED:
            err_code = app_timer_stop(m_battery_timer_id);
            APP_ERROR_CHECK(err_code);
            break;//BLE_BAS_EVT_NOTIFICATION_DISABLED

        default:
            // No implementation needed.
            break;
    }
}

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
            {
        case BLE_GAP_EVT_CONNECTED:
            err_code           = ble_db_discovery_start(
                &m_ble_db_discovery,
                p_ble_evt->evt.gap_evt.conn_handle
            );
            APP_ERROR_CHECK(err_code);

            m_conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

            err_code = app_button_enable();
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            m_conn_handle = BLE_CONN_HANDLE_INVALID;
            err_code = app_button_disable();
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for dispatching a BLE stack event to all modules with a BLE stack event handler.
 *
 * @details This function is called from the BLE Stack event interrupt handler after a BLE stack
 *          event has been received.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    dm_ble_evt_handler(p_ble_evt);
    ble_conn_params_on_ble_evt(p_ble_evt);
    ble_db_discovery_on_ble_evt(&m_ble_db_discovery, p_ble_evt);
    ble_ias_on_ble_evt(&m_ias, p_ble_evt);
    ble_lls_on_ble_evt(&m_lls, p_ble_evt);
    ble_bas_on_ble_evt(&m_bas, p_ble_evt);
    ble_ias_c_on_ble_evt(&m_ias_c, p_ble_evt);
    ble_gss_on_ble_evt(&m_gss, p_ble_evt);
    on_ble_evt(p_ble_evt);
    ble_advertising_on_ble_evt(p_ble_evt);

}


/**@brief Function for dispatching a system event to interested modules.
 *
 * @details This function is called from the System event interrupt handler after a system
 *          event has been received.
 *
 * @param[in] sys_evt  System stack event.
 */
static void sys_evt_dispatch(uint32_t sys_evt)
{
    pstorage_sys_event_handler(sys_evt);
    ble_advertising_on_sys_evt(sys_evt);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    uint32_t err_code;

    nrf_clock_lf_cfg_t clock_lf_cfg = {
    .source        = NRF_CLOCK_LF_SRC_RC,           
    .rc_ctiv       = 16,                            
    .rc_temp_ctiv  = 2,                             
   .xtal_accuracy = NRF_CLOCK_LF_XTAL_ACCURACY_250_PPM
    };
    
    // Initialize the SoftDevice handler module.
    SOFTDEVICE_HANDLER_INIT(&clock_lf_cfg, NULL);

    ble_enable_params_t ble_enable_params;
    err_code = softdevice_enable_get_default_config(CENTRAL_LINK_COUNT,
                                                    PERIPHERAL_LINK_COUNT,
                                                    &ble_enable_params);
    APP_ERROR_CHECK(err_code);

    //ble_enable_params.gatts_enable_params.attr_tab_size = 0x10;

    //Check the ram settings against the used number of links
    CHECK_RAM_START_ADDR(CENTRAL_LINK_COUNT,PERIPHERAL_LINK_COUNT);

    // Enable BLE stack.
    sd_ble_enable(&ble_enable_params, &app_ram_base);
    ble_enable_params.common_enable_params.vs_uuid_count = 10;
    err_code = softdevice_enable(&ble_enable_params);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    APP_ERROR_CHECK(err_code);

    // Register with the SoftDevice handler module for BLE events.
    err_code = softdevice_sys_evt_handler_set(sys_evt_dispatch);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling events from the BSP module.
 *
 * @param[in]   event   Event generated by button press.
 */
void bsp_event_handler(bsp_event_t event)
{
    uint32_t err_code;
    switch (event)
    {
        case BSP_EVENT_SLEEP:
            sleep_mode_enter();
            break;

        case BSP_EVENT_DISCONNECT:
            err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BSP_EVENT_WHITELIST_OFF:
            err_code = ble_advertising_restart_without_whitelist();
            if (err_code != NRF_ERROR_INVALID_STATE)
            {
                APP_ERROR_CHECK(err_code);
            }
            break;

        default:
            break;
    }
}


/**@brief Function for handling the Device Manager events.
 *
 * @param[in] p_evt  Data associated to the device manager event.
 */
static uint32_t device_manager_evt_handler(dm_handle_t const * p_handle,
                                           dm_event_t const  * p_event,
                                           ret_code_t        event_result)
{
    APP_ERROR_CHECK(event_result);

#ifdef BLE_DFU_APP_SUPPORT
    if (p_event->event_id == DM_EVT_LINK_SECURED)
    {
        app_context_load(p_handle);
    }
#endif // BLE_DFU_APP_SUPPORT

    return NRF_SUCCESS;
}


/**@brief Function for the Device Manager initialization.
 *
 * @param[in] erase_bonds  Indicates whether bonding information should be cleared from
 *                         persistent storage during initialization of the Device Manager.
 */
static void device_manager_init(bool erase_bonds)
{
    uint32_t               err_code;
    dm_init_param_t        init_param = {.clear_persistent_data = erase_bonds};
    dm_application_param_t register_param;

    // Initialize persistent storage module.
    err_code = pstorage_init();
    APP_ERROR_CHECK(err_code);

    err_code = dm_init(&init_param);
    APP_ERROR_CHECK(err_code);

    memset(&register_param.sec_param, 0, sizeof(ble_gap_sec_params_t));

    register_param.sec_param.bond         = SEC_PARAM_BOND;
    register_param.sec_param.mitm         = SEC_PARAM_MITM;
    register_param.sec_param.lesc         = SEC_PARAM_LESC;
    register_param.sec_param.keypress     = SEC_PARAM_KEYPRESS;
    register_param.sec_param.io_caps      = SEC_PARAM_IO_CAPABILITIES;
    register_param.sec_param.oob          = SEC_PARAM_OOB;
    register_param.sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    register_param.sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
    register_param.evt_handler            = device_manager_evt_handler;
    register_param.service_type           = DM_PROTOCOL_CNTXT_GATT_SRVR_ID;

    err_code = dm_register(&m_app_handle, &register_param);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing the Advertising functionality.
 */
static void advertising_init(void)
{
    uint32_t      err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;
    ble_advdata_manuf_data_t manuf;

    /* We define our own custom Service UUID (unexisting oO, WTF Netwyse ?) */
    ble_uuid_t m_adv_uuids[] = {
        {0x2802, gabs_type}
    }; /**< Universally unique service identifiers. */


    // Build advertising data struct to pass into @ref ble_advertising_init.
    memset(&advdata, 0, sizeof(advdata));
    memset(&scanrsp, 0, sizeof(scanrsp));
    memset(&manuf, 0, sizeof(manuf));

    /* Include complete name and flags in advertising data. */
    advdata.name_type               = BLE_ADVDATA_FULL_NAME;
    advdata.include_appearance      = true;
    advdata.flags                   = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    /* Add manufacturer data in advertising data. */
    manuf.company_identifier = 0xBCAB;
    manuf.data.size = 6;
    manuf.data.p_data = (uint8_t *)("\x99\x6A\x9F\xC6\xF4\xFD");
    advdata.p_manuf_specific_data = &manuf;

    /* Add complete 128-bit custom UUID in scan response. */
    scanrsp.uuids_complete.uuid_cnt = sizeof(m_adv_uuids) / sizeof(m_adv_uuids[0]);
    scanrsp.uuids_complete.p_uuids  = m_adv_uuids;


    ble_adv_modes_config_t options = {0};
    options.ble_adv_fast_enabled  = BLE_ADV_FAST_ENABLED;
    options.ble_adv_fast_interval = APP_ADV_INTERVAL;
    options.ble_adv_fast_timeout  = APP_ADV_TIMEOUT_IN_SECONDS;

    /* Start advertising. */
    err_code = ble_advertising_init(&advdata, &scanrsp, &options, on_adv_evt, NULL);
    APP_ERROR_CHECK(err_code);
}


void btn_handler(uint8_t pin_no, uint8_t action) {
    if ((action == APP_BUTTON_PUSH) && !gPushed) {
        gPushed = 1;
        //beep(440, 1000);
        ble_gss_button_notify(&m_gss);
    } else {
        gPushed = 0;
    }
}

void button_init(void) {
    uint32_t err_code;
    int i=0;
    for (i=0; i<4; i++) {
        nrf_gpio_port_dir_set(i, NRF_GPIO_PORT_DIR_OUTPUT);
        nrf_gpio_port_set(i, 0x00);
    }

    // Button configuration structure.
    static app_button_cfg_t p_button[] = {  {SW_PIN2, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, btn_handler},
    {SW_PIN1, APP_BUTTON_ACTIVE_LOW, NRF_GPIO_PIN_PULLUP, btn_handler}
    };

    // Initializing the buttons.
    err_code = app_button_init(p_button, sizeof(p_button) / sizeof(p_button[0]), BUTTON_DEBOUNCE_DELAY);
    APP_ERROR_CHECK(err_code);
}

static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}

static void gpiote_init(void)
{
    APP_GPIOTE_INIT(APP_GPIOTE_MAX_USERS);
}



/**@brief Function for the Power manager.
 */
static void power_manage(void)
{
    uint32_t err_code = sd_app_evt_wait();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for application main entry.
 */
int main(void)
{
    uint32_t err_code;
    bool erase_bonds = 0;
    //int state=0;

    // Initialize.
    timers_init();
    gpiote_init();
    button_init();
    //buttons_leds_init(&erase_bonds);
    ble_stack_init();
    scheduler_init();
    add_custom_services();
    adc_configure();
    device_manager_init(erase_bonds);
    gap_params_init();
    advertising_init();
    db_discovery_init();
    services_init();
    conn_params_init();

    memset(&melody, 0, sizeof(melody));
    melody.notes = notes;
    melody.nb_notes = 3;
    melody.current = 0;

    // Start execution.
    err_code = ble_advertising_start(BLE_ADV_MODE_FAST);
    APP_ERROR_CHECK(err_code);

    // Enter main loop.
    for (;;)
    {
        power_manage();
    }
}

/**
 * @}
 */
