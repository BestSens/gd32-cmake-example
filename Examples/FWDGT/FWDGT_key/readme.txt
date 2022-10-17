/*!
    \file    readme.txt
    \brief   description of the FWDGT_key example

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
    \version 2022-03-09, V3.0.0, firmware for GD32F4xx
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

  This demo is based on the GD32450I-EVAL-V1.1 board, it shows how to reload the 
FWDGT counter at regulate period using the EXTI interrupt. The FWDGT timeout 
is set to 2s (the timeout may varies due to IRC32K frequency dispersion).
 
  An EXTI is connected to a specific GPIO pin and configured to generate an interrupt
on its falling edge: when the EXTI Line interrupt is triggered (by pressing the Wakeup
Key on the board), the corresponding interrupt is served. In the ISR, FWDGT counter is 
reloaded). 

  If the EXTI Line interrupt does not occur, the FWDGT counter is not reloaded before
the FWDGT counter reaches 00h, and the FWDGT reset. 

  There are three operation methods as follow:
  1.After the download is completed and the system is powered on, LED2 flashes once and 
then LED3 starts flashing.

  2.After the download is completed and the system is powered on, start to press the wake 
button in quick succession, then LED2 is always on, and stop pressing the wake button in
quick succession and LED3 starts to flash.

  3.After the download is completed and the system is powered on, LED2 flashes once and then 
LED3 starts flashing. At this time, after pressing the wake button in rapid succession, 
LED3 is always on, and after stopping the rapid and continuous pressing of the wake button,
LED3 starts flashing
