OpenLys - Opensource firmware for Gablys Lite and Premium
=========================================================

How to compile this firmware
----------------------------

Follow instructions at http://redbearlab.com/nrf51822-sdk/, use the nRF51 SDK v11.0 (I used this one to build this application).

Nordic' SDK version 11.0 is available here: https://developer.nordicsemi.com/nRF5_SDK/nRF5_SDK_v11.x.x/

Clone the project into the **ble_examples** directory, then go into the *gablys/s130/armgcc* folder.

Run the following commands:

```
$ make
rm -rf _build
echo  Makefile
Makefile
mkdir _build
Compiling file: app_button.c
Compiling file: app_error.c
Compiling file: app_error_weak.c
Compiling file: app_timer.c
Compiling file: app_trace.c
Compiling file: app_util_platform.c
Compiling file: fstorage.c
Compiling file: nrf_assert.c
Compiling file: nrf_log.c
Compiling file: retarget.c
Compiling file: sensorsim.c
Compiling file: RTT_Syscalls_GCC.c
Compiling file: SEGGER_RTT.c
Compiling file: SEGGER_RTT_printf.c
Compiling file: app_uart.c
Compiling file: app_gpiote.c
Compiling file: sha256.c
Compiling file: app_scheduler.c
Compiling file: app_timer_appsh.c
Compiling file: nrf_delay.c
Compiling file: nrf_drv_common.c
Compiling file: nrf_drv_uart.c
Compiling file: pstorage.c
Compiling file: nrf_adc.c
Compiling file: nrf_drv_gpiote.c
Compiling file: nrf_drv_adc.c
Compiling file: bsp.c
Compiling file: bsp_btn_ble.c
Compiling file: gss.c
Compiling file: main.c
Compiling file: piezzo.c
Compiling file: ble_advdata.c
Compiling file: ble_ias.c
Compiling file: ble_ias_c.c
Compiling file: ble_lls.c
Compiling file: ble_bas.c
Compiling file: ble_db_discovery.c
Compiling file: ble_advertising.c
Compiling file: ble_conn_params.c
Compiling file: ble_srv_common.c
Compiling file: device_manager_peripheral.c
Compiling file: system_nrf51.c
Compiling file: softdevice_handler.c
Assembly file: gcc_startup_nrf51.s
Linking target: nrf51822_xxaa_s130.out
make[1]: Entering directory '/home/virtualabs/openlys/gablys/s130/armgcc'
Preparing: nrf51822_xxaa_s130.bin
Preparing: nrf51822_xxaa_s130.hex

   text   data         bss    dec    hexfilename
     35372    164   3408  38944   9820                              _build/nrf51822_xxaa_s130.out

     make[1]: Leaving directory '/home/virtualabs/openlys/gablys/s130/armgcc'

``` 

How to flash your Gablys
------------------------

Use a ST-LINK v2 programming device, connected to the Gablys. Then flash the Gablys with the s130 SoftDevice (you may need root to perform this) :


```
$ make flash_softdevice
```

Then flash the application code, using the following command:

```
$ make flash
```

Unplug your Gablys, and voilà !
