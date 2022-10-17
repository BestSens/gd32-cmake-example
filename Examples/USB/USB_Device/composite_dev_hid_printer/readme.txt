/*!
    \file    readme.txt
    \brief   description of the the USB HID/printer composite demo

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

  This demo is based on the GD32450i-EVAL-V1.1 board, it provides a description of 
how to use the USBFS/USBHS.

  The demo provide a composite device.

  A composite device is one that has multiple interfaces controlled independently
of each other. Using composite device, multiple functions are combined into a single
device. In this Example the independent interfaces are : custom HID and USB Printer.

  This demo was created by combining the code in the HID and USB Printer.

  The implementation of the composite HID-Printer device is done by creating a wrapping class 
(see folder hid_printer_wrapper ) that makes calls to the separate HID and USB Printer class layer
handlers (files custom_hid_core.c and printer_core.c files).

  To test the demo, you can run HID demo and at same time you can select the
USB Printer.

  When even one of the interface class of the device is changed, it should be handled
differently by Windows. However, it doesn't recognize the modification. To avoid conflict
on Windows, we suggest to assign another VID/PID to the device or delete device instance 
from device manager.
