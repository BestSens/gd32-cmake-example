/*!
    \file    usbd_conf.h
    \brief   the header file of USB device configuration

    \version 2024-12-20, V3.3.1, firmware for GD32F4xx
*/

/*
    Copyright (c) 2024, GigaDevice Semiconductor Inc.

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

#ifndef USBD_CONF_H
#define USBD_CONF_H

#include "usb_conf.h"

/* USB configure exported defines */
#define USBD_CFG_MAX_NUM                    1U
#define USBD_ITF_MAX_NUM                    1U

#define USB_STR_DESC_MAX_SIZE               255U

#define PRINTER_IN_EP                       EP_IN(1U)
#define PRINTER_OUT_EP                      EP_OUT(1U)

#define USB_STRING_COUNT                    4U

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#ifdef USE_USB_HS
    #ifdef USE_ULPI_PHY
        #define PRINTER_DATA_PACKET_SIZE           512U
    #else
        #define PRINTER_DATA_PACKET_SIZE           64U
    #endif

    #define PRINTER_IN_PACKET                      PRINTER_DATA_PACKET_SIZE
    #define PRINTER_OUT_PACKET                     PRINTER_DATA_PACKET_SIZE
#else
    #define PRINTER_DATA_PACKET_SIZE               64U
    #define PRINTER_IN_PACKET                      PRINTER_DATA_PACKET_SIZE
    #define PRINTER_OUT_PACKET                     PRINTER_DATA_PACKET_SIZE
#endif /* USE_USB_HS */

#endif /* USBD_CONF_H */
