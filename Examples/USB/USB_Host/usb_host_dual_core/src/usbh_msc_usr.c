/*!
    \file    usbh_msc_usr.c
    \brief   user application layer for USBHS host-mode MSC class operation

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
    \version 2022-03-09, V3.1.0, firmware for GD32F4xx
    \version 2022-06-30, V3.2.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include <string.h>
#include "usbh_msc_usr.h"
#include "drv_usb_hw.h"
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bbb.h"
#include "ff.h"

extern usb_core_driver usbhs_core;
extern usbh_host usb_host_msc;

FATFS fatfs;
FIL file;

uint8_t line_idx;
uint8_t usbh_usr_application_state = USBH_USR_FS_INIT;

/* local function prototypes ('static') */
static void usbh_user_init(void);
static void usbh_user_deinit(void);
static void usbh_user_device_connected(void);
static void usbh_user_unrecovered_error(void);
static void usbh_user_device_disconnected(void);
static void usbh_user_device_reset(void);
static void usbh_user_device_speed_detected(uint32_t DeviceSpeed);
static void usbh_user_device_desc_available(void *device_desc);
static void usbh_user_device_address_assigned(void);
static void usbh_user_configuration_descavailable(usb_desc_config *cfgDesc,
                                                  usb_desc_itf *itfDesc,
                                                  usb_desc_ep *epDesc);
static void usbh_user_manufacturer_string(void *manufacturer_string);
static void usbh_user_product_string(void *product_string);
static void usbh_user_serialnum_string(void *serial_num_string);
static void usbh_user_enumeration_finish(void);
static void usbh_user_device_not_supported(void);
static usbh_user_status usbh_user_userinput(void);
static void usbh_user_over_current_detected(void);
static uint8_t explore_disk(char* path, uint8_t recu_level);
static void toggle_leds(void);
static int usbh_usr_msc_application(void);

/* points to the DEVICE_PROP structure of current device */
usbh_user_cb msc_usr_cb =
{
    usbh_user_init,
    usbh_user_deinit,
    usbh_user_device_connected,
    usbh_user_device_reset,
    usbh_user_device_disconnected,
    usbh_user_over_current_detected,
    usbh_user_device_speed_detected,
    usbh_user_device_desc_available,
    usbh_user_device_address_assigned,
    usbh_user_configuration_descavailable,
    usbh_user_manufacturer_string,
    usbh_user_product_string,
    usbh_user_serialnum_string,
    usbh_user_enumeration_finish,
    usbh_user_userinput,
    usbh_usr_msc_application,
    usbh_user_device_not_supported,
    usbh_user_unrecovered_error
};

/*!
    \brief      user operation for host-mode initialization
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_init(void)
{
    static uint8_t startup = 0U;

    if (0U == startup) {
        startup = 1U;

        /* configure the LEDs and KEYs*/
        gd_eval_led_init(LED2);
        gd_eval_led_init(LED3);
        gd_eval_key_init(KEY_USER, KEY_MODE_GPIO);
        gd_eval_key_init(KEY_TAMPER, KEY_MODE_GPIO);
        gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);

        printf("> USB MSC host library started\n");
    }
}

/*!
    \brief      deinitialize user state and associated variables
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_deinit(void)
{
    usbh_usr_application_state = USBH_USR_FS_INIT;
}

/*!
    \brief      user operation for device attached
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_device_connected(void)
{
    printf("> Device Attached.\n");
}

/*!
    \brief      user operation for unrecovered error happens
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_unrecovered_error (void)
{
    printf("> UNRECOVERED ERROR STATE.\n");
}

/*!
    \brief      user operation for device disconnect event
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_device_disconnected (void)
{
    printf("> Device Disconnected.\n");
}

/*!
    \brief      user operation for reset USB Device
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_device_reset(void)
{
    printf("> Reset the USB device.\n");
}

/*!
    \brief      user operation for detecting device speed
    \param[in]  device_speed: device speed
    \param[out] none
    \retval     none
*/
static void usbh_user_device_speed_detected(uint32_t device_speed)
{
    if(PORT_SPEED_HIGH == device_speed){
        printf("> High speed device detected.\n");
    }else if(PORT_SPEED_FULL == device_speed){
        printf("> Full speed device detected.\n");
    }else if(PORT_SPEED_LOW == device_speed){
        printf("> Low speed device detected.\n");
    }else{
        printf("> Device Fault.\n");
    }
}

/*!
    \brief      user operation when device descriptor is available
    \param[in]  device_desc: device descriptor
    \param[out] none
    \retval     none
*/
static void usbh_user_device_desc_available(void *device_desc)
{
    usb_desc_dev *pDevStr = device_desc;

    printf("VID: %04Xh\n", (uint32_t)pDevStr->idVendor);
    printf("PID: %04Xh\n", (uint32_t)pDevStr->idProduct);
}

/*!
    \brief      USB device is successfully assigned the address
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_device_address_assigned(void)
{
}

/*!
    \brief      user operation when configuration descriptor is available
    \param[in]  cfg_desc: pointer to configuration descriptor
    \param[in]  itf_desc: pointer to interface descriptor
    \param[in]  ep_desc: pointer to endpoint descriptor
    \param[out] none
    \retval     none
*/
static void usbh_user_configuration_descavailable(usb_desc_config *cfg_desc,
                                                  usb_desc_itf *itf_desc,
                                                  usb_desc_ep *ep_desc)
{
    usb_desc_itf *id = itf_desc;

    if(0x08U == (*id).bInterfaceClass){
        printf("> Mass storage device connected.\n");
    }else if (0x03U == (*id).bInterfaceClass){
        printf("> HID device connected.\n");
    }
}

/*!
    \brief      user operation when manufacturer string exists
    \param[in]  manufacturer_string: manufacturer string of usb device
    \param[out] none
    \retval     none
*/
static void usbh_user_manufacturer_string(void *manufacturer_string)
{
    printf("Manufacturer: %s\n", (char *)manufacturer_string);
}

/*!
    \brief      user operation when product string exists
    \param[in]  product_string: product string of USB device
    \param[out] none
    \retval     none
*/
static void usbh_user_product_string(void *product_string)
{
    printf("Product: %s\n", (char *)product_string);
}

/*!
    \brief      user operation when serialNum string exists
    \param[in]  serial_num_string: serialNum string of USB device
    \param[out] none
    \retval     none
*/
static void usbh_user_serialnum_string(void *serial_num_string)
{
    printf("Serial Number: %s\n", (char *)serial_num_string);
}

/*!
    \brief      user response request is displayed to ask for application jump to class
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_enumeration_finish(void)
{
    printf("> Enumeration completed.\n");
    printf(" ------------------------------------\n");
    printf("> To see the disk information: \n");
    printf("> Press USER Key...            \n");
}

/*!
    \brief      user operation when device is not supported
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_device_not_supported(void)
{
    printf("> Device not supported.\n");
}

/*!
    \brief      user action for application state entry
    \param[in]  none
    \param[out] none
    \retval     user response for user key
*/
static usbh_user_status usbh_user_userinput(void)
{
    usbh_user_status usbh_usr_status = USR_IN_NO_RESP;

    /*key USER is in polling mode to detect user action */
    if (RESET == gd_eval_key_state_get(KEY_USER)) {
        usbh_usr_status = USR_IN_RESP_OK;
    }

    return usbh_usr_status;
}

/*!
    \brief      user action for device over current detection event
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void usbh_user_over_current_detected(void)
{
    printf("> Overcurrent detected.\n");
}

/*!
    \brief      demo application for mass storage
    \param[in]  none
    \param[out] none
    \retval     status
*/
static int usbh_usr_msc_application(void)
{
    FRESULT res;
    msc_lun info;
    uint8_t WriteTextBuff[] = "GD32 Connectivity line Host Demo application using FAT_FS\r";
    uint16_t bytesWritten, bytesToWrite;

    switch(usbh_usr_application_state){
    case USBH_USR_FS_INIT:
        /* initializes the file system*/
        if (FR_OK != f_mount(&fatfs, "0:/", 0)) {
            printf("> Cannot initialize File System.\n");

            return(-1);
        }

        printf("> File System initialized.\n");

        if (USBH_OK == usbh_msc_lun_info_get(&usb_host_msc, 0, &info)){
            printf("> Disk capacity: %ud Bytes.\n", info.capacity.block_nbr * info.capacity.block_size);
        }

        usbh_usr_application_state = USBH_USR_FS_READLIST;
        break;

    case USBH_USR_FS_READLIST:
        printf("> Exploring disk flash ...\n");

        printf("> To see the root content of disk\n");
        printf("> Press Tamper Key...            \n");

        /*Key TAMPER in polling*/
        while ((usbhs_core.host.connect_status) && \
            (SET == gd_eval_key_state_get (KEY_TAMPER))) {
            toggle_leds();
        }

        explore_disk("0:/", 1);
        line_idx = 0;
        usbh_usr_application_state = USBH_USR_FS_WRITEFILE;
        break;

    case USBH_USR_FS_WRITEFILE:
        usb_mdelay(100);

        printf("                                \n");
        printf("> Press Wakeup Key to write file\n");

        /*key WAKEUP in polling*/
        while ((usbhs_core.host.connect_status) && \
                (SET == gd_eval_key_state_get (KEY_WAKEUP))) {
            toggle_leds();
        }

        printf("> Writing File to disk flash ...\n");

        /* register work area for logical drives */
        f_mount(&fatfs, "0:/", 1);

        if (FR_OK == f_open(&file, "0:GD32.TXT", FA_CREATE_ALWAYS | FA_WRITE)) {
            /* write buffer to file */
            bytesToWrite = sizeof(WriteTextBuff); 
            res = f_write (&file, WriteTextBuff, bytesToWrite, (void *)&bytesWritten);
            /* EOF or error */
            if ((0U == bytesWritten) || (FR_OK != res)) {
                printf("> GD32.TXT CANNOT be written.\n");
            } else {
                printf("> GD32.TXT created in the disk.\n");
            }

            /* close file and file system */
            f_close(&file);
            f_mount(NULL, "0:/", 1); 
        } else {
            printf("> GD32.TXT created in the disk.\n");
        }

        usbh_usr_application_state = USBH_USR_FS_DEMOEND;
        printf("> The MSC host demo is end.\n");
        break;

    case USBH_USR_FS_DEMOEND:
        break;

    default:
        break;
    }

    return(0);
}

/*!
    \brief      displays disk content
    \param[in]  path: pointer to root path
    \param[in]  recu_level: recursive level
    \param[out] none
    \retval     status
*/
static uint8_t explore_disk (char* path, uint8_t recu_level)
{
    FRESULT res;
    FILINFO fno;
    DIR dir;
    char *fn;

    res = f_opendir(&dir, path);

    if (res == FR_OK) {
        while ((usbhs_core.host.connect_status)) {
            res = f_readdir(&dir, &fno);
            if (FR_OK != res || 0U == fno.fname[0]) {
                break;
            }

            if ('.' == fno.fname[0]) {
                continue;
            }

            fn = fno.fname;

            line_idx++;

            if (line_idx > 4) {
                line_idx = 0;

                printf("                                 \n");
                printf("> Press User Key to continue\n");

                /*key USER in polling*/
                while ((usbhs_core.host.connect_status) && \
                    (SET == gd_eval_key_state_get (KEY_USER))) {
                    toggle_leds();
                }
            }

            if(1U == recu_level){
                printf("   |__");
            }else if(2U == recu_level){
                printf("   |   |__");
            }

            if(AM_DIR == fno.fattrib){
                printf("%s\n", fno.fname);
            }else{
                printf("%s\n", fno.fname);
            }

            if((AM_DIR == fno.fattrib) && (1U == recu_level)){
                explore_disk(fn, 2);
            }
        }
    }

    return res;
}

/*!
    \brief      toggle LEDs to shows user input state
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void toggle_leds(void)
{
    static uint32_t i;

    if (0x10000U == i++) {
        gd_eval_led_toggle(LED2);
        gd_eval_led_toggle(LED3);
        i = 0;
    }
}
