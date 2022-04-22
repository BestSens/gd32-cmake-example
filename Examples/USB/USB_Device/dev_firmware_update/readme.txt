/*!
    \file    readme.txt
    \brief   description of the USB DFU device demo

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
    \version 2022-03-09, V3.1.0, firmware for GD32F4xx
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

  It presents the implementation of a device firmware upgrade (DFU) capability in 
the GD32 USB device.

  It follows the DFU class specification defined by the USB Implementers Forum for 
reprogramming an application through USB-FS/HS-Device.

  The DFU principle is particularly well suited to USB-FS-Device applications that 
need to be reprogrammed in the field.

  To test the demo, you need a configuration hex image or bin image . The hex image 
and the bin image should set application address at 0x8004000. You can refer to 
"../Utilities/Binary/gd32f450i" folder and use the hex images and bin images in it.

  You need install the corresponding GD DFU Driver with your PC operation system. 

  Once the configuration *.hex image is generated, it can be downloaded into 
internal flash memory using the GD tool "GD MCU DFU Tool" available for download 
from http://www.gd32mcu.com/cn/download.

  The GD tool "GD MCU DFU Tool" also can operate the option Byte.

  The supported memory for this example is the internal flash memory, you can also
add a new memory interface if you have external memory.

  After each device reset, hold down the TAMPER key on the GD32450i-EVAL-V1.1 board.
