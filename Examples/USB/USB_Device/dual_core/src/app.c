/*!
    \file    app.c
    \brief   main routine

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

#include "drv_usb_hw.h"
#include "cdc_acm_core.h"
#include "custom_hid_core.h"

usb_core_driver usbhs_cdc_acm;
usb_core_driver usbfs_custom_hid;

extern hid_fop_handler fop_handler;

/*!
    \brief      main routine
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    usb_gpio_config();
    usb_rcu_config();
    usb_timer_init();

    custom_hid_itfop_register(&usbfs_custom_hid, &fop_handler);

    /* USB initialize */
    usbd_init (&usbfs_custom_hid,
               USB_CORE_ENUM_FS,
               &custom_hid_desc,
               &usbd_custom_hid_cb);

    usbd_init (&usbhs_cdc_acm,
               USB_CORE_ENUM_HS,
               &cdc_desc,
               &cdc_class);

    usb_intr_config();

    /* main loop */
    while (1) {
        if (USBD_CONFIGURED == usbhs_cdc_acm.dev.cur_status) {
            if (0U == cdc_acm_check_ready(&usbhs_cdc_acm)) {
                cdc_acm_data_receive(&usbhs_cdc_acm);
            } else {
                cdc_acm_data_send(&usbhs_cdc_acm);
            }
        }
    }
}
