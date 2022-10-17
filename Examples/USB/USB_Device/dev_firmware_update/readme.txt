/*!
    \file    readme.txt
    \brief   description of the USB DFU device demo

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

  It presents the implementation of a device firmware upgrade (DFU) capability in 
the GD32 USB device.

  It follows the DFU class specification defined by the USB Implementers Forum for 
reprogramming an application through USB-FS/HS-Device.

  The DFU principle is particularly well suited to USB-FS/HS-Device applications that 
need to be reprogrammed in the field.

  The supported memorys of this example are the internal flash memory, the external 
nand flash memory and the external nor flash memory.

  To test the demo, the internal flash is used as an example:

  Step 1: User need a configuration hex image or bin image. The hex image and the bin image 
should set application address at APP_LOADED_ADDR in the internal flash memory. User can refer to
"../Test_Images" folder and use the hex images and bin images in it.

  Step 2: User need install the corresponding GD DFU Driver with your PC operation system. 

  Step 3: Once the configuration *.hex image is generated, it can be downloaded into the 
internal flash memory or the external falsh memory using the GD tool "GD32 MCU DFUEx Tool
" available for download from http://www.gd32mcu.com/cn/download.

  The GD tool "GD32AllInOneProgrammer" can operate the option Byte in the internal flash.

  Step 4: After each device reset, hold down the TAMPER key on the GD32450i-EVAL board into app 
routine in the internal flash.

  JP12,JP13,JP20 must be fitted to SPI ports.
