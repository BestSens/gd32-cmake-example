/*!
    \file    usbd_conf.h
    \brief   the header file of USB device configuration

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

#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#include "usb_conf.h"

/* USB configure exported defines */
#define USBD_CFG_MAX_NUM                    1U
#define USBD_ITF_MAX_NUM                    2U

#define CUSTOM_HID_INTERFACE                0x00U
#define CDC_COM_INTERFACE                   0x01U

#define USB_STR_DESC_MAX_SIZE               255U

/* endpoint used by the custom HID device */
#define CUSTOMHID_IN_EP                     EP_IN(1U)
#define CUSTOMHID_OUT_EP                    EP_OUT(1U)

/* endpoint used by the USB printer device */
#define PRINTER_IN_EP                       EP_IN(2U)
#define PRINTER_OUT_EP                      EP_OUT(2U)

#define USB_STRING_COUNT                    4U

#define CUSTOMHID_OUT_PACKET                2U
#define CUSTOMHID_IN_PACKET                 2U

#ifdef USE_USB_HS  
    #ifdef USE_ULPI_PHY
        #define PRINTER_IN_PACKET           512U
        #define PRINTER_OUT_PACKET          512U
    #else
        #define PRINTER_IN_PACKET           64U
        #define PRINTER_OUT_PACKET          64U
    #endif
#else  /*USE_USB_FS*/
    #define PRINTER_IN_PACKET               64U
    #define PRINTER_OUT_PACKET              64U
#endif

#endif /* __USBD_CONF_H */
