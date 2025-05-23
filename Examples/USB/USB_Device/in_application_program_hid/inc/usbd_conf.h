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

#define USBD_CFG_MAX_NUM                       1U
#define USBD_ITF_MAX_NUM                       1U
#define USB_STR_DESC_MAX_SIZE                  64U

#define USBD_IAP_INTERFACE                     0U

#define USB_STRING_COUNT                       4U

#define IAP_IN_EP                              EP1_IN
#define IAP_OUT_EP                             EP1_OUT

#ifdef USB_HS_CORE
    #ifdef USE_ULPI_PHY
        #define IAP_IN_PACKET                  1018U
        #define IAP_OUT_PACKET                 1018U
        #define TRANSFER_SIZE                  1012U  /* IAP maximum data packet size */
    #elif defined(USE_EMBEDDED_PHY)
        #define IAP_IN_PACKET                  62U
        #define IAP_OUT_PACKET                 62U
        #define TRANSFER_SIZE                  56U    /* IAP maximum data packet size */
    #endif
#elif defined(USB_FS_CORE)
    #define IAP_IN_PACKET                      62U
    #define IAP_OUT_PACKET                     62U
    #define TRANSFER_SIZE                      56U    /* IAP maximum data packet size */
#else
    #error "Please select USB_HS_CORE or USB_FS_CORE"
#endif

/* option byte size and address */
#define OPT_BYTE_SIZE1                         0x10U
#define OPT_BYTE_SIZE2                         0x10U
#define OPT_BYTE_ADDR1                         0x1FFFC000U
#define OPT_BYTE_ADDR2                         0x1FFEC000U

#define REPORT_IN_COUNT                        ((TRANSFER_SIZE) + 5U)
#define REPORT_OUT_COUNT                       ((TRANSFER_SIZE) + 5U)

/* memory address from where user application will be loaded, which represents 
   the DFU code protected against write and erase operations.*/
#define APP_LOADED_ADDR                        0x08008000U

/* make sure the corresponding memory where the DFU code should not be loaded
   cannot be erased or overwritten by DFU application. */
#define IS_PROTECTED_AREA(addr)                (uint8_t)(((addr >= 0x08000000U) && \
                                               (addr < (APP_LOADED_ADDR)))? 1U : 0U)

#endif /* USBD_CONF_H */
