/*!
    \file    readme.txt
    \brief   description of the USB HID device(USB-Keyboard) demo

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
how to use the USBFS/USBHS device application based on the Human Interface Device(HID).

  The GD32 device is enumerated as an USB keyboard, that uses the native PC Host
HID driver to which the GD32F450I-EVAL board is connected.

  The USB keyboard use three key(wakeup key, tamper key and user key) to print 3 
characters('b', 'a' and 'c').

  This demo supports remote wakeup (which is the ability of a USB device to bring a 
suspended bus back to the active condition), and the wakeup key is used as the remote
wakeup source.

  In order to test USB remote wakeup function, you can do as follows:
    - Manually switch PC to standby mode

    - Wait for PC to fully enter the standby mode

    - Push the wakeup key

    - If PC is ON, remote wakeup is OK, else failed

  In order to make the program work, you must do the following:
    - Open your preferred tool-chain 

    - Rebuild all files and load your image into target memory

    - Run the application
