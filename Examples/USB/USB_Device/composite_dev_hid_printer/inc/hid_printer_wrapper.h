/*!
    \file    hid_printer_wrapper.h
    \brief   header file for the hid_printer_wrapper.c file

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

#ifndef HID_PRINTER_WRAPPER_H
#define HID_PRINTER_WRAPPER_H

#include "custom_hid_core.h"
#include "printer_core.h"

#define HID_INTERFACE              0x00
#define PRINTER_INTERFACE          0x01

typedef struct
{
    usb_desc_config                config;
    usb_desc_itf                   hid_interface;
    usb_desc_hid                   hid_vendorhid;
    usb_desc_ep                    hid_ep_report_in;
    usb_desc_ep                    hid_ep_report_out;
    usb_desc_itf                   printer_interface;
    usb_desc_ep                    printer_ep_data_in;
    usb_desc_ep                    printer_ep_data_out;
} usb_hid_printer_desc_config_set;

extern usb_desc hid_printer_desc;
extern usb_class_core usbd_hid_printer_cb;

#endif /* HID_PRINTER_WRAPPER_H */
