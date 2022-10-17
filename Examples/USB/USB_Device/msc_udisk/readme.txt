/*!
    \file    readme.txt
    \brief   description of the USB MSC device(Udisk) demo

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

  This demo is based on the GD32450i-EVAL-V1.1 board,it provides a description of 
how to use the USBFS/USBHS device application based on the Mass Storage Class (MSC).

  The GD32 MCU is enumerated as a MSC device using the native PC Host MSC driver
to which the GD32450i-EVAL-V1.1 board is connected.

  It illustrates an implementation of the MSC class which uses the bulk transfer
while the internal SRAM is used as storage media.

  This example supports the BBB (bulk only transfer) protocol and all needed SCSI
(small computer system interface) commands, and is compatible with Windows platform.

  After running the example, the user just has to plug the USB cable into a PC host
and the device is automatically detected. One new removable drives appear in the
system window and write/read/format operations can be performed as with any other
removable drive.

  To select the appropriate USB Core to work with, user must add the following macro 
defines within the compiler preprocessor (already done in the pre-configured projects 
provided with this application):
  - "USE_USB_FS" when using USB Full Speed (FS) Core
  - "USE_USB_HS, USE_ULPI_PHY" when using USB High Speed (HS) Core with ULPI PHY interface

  In order to make the program work, you must do the following:
    - Open your preferred tool-chain 
    - Rebuild all files and load your image into target memory
    - Run the application
