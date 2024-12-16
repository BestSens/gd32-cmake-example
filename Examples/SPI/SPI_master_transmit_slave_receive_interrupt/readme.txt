/*!
    \file    readme.txt
    \brief   description of the master transmit and slave receive through interrupt demo

    \version 2024-01-15, V3.2.0, firmware for GD32F4xx
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


  This example is based on the GD32450i-EVAL board, it shows master send data and 
slave receive data use intterupt mode.

  1.Modify files: example/SPI/SPI_master_transmit_slave_receive_interrupt/main.h, choose
to disable CRC function.
  #define SPI_CRC_ENABLE       0
  After the communication is completed, if the data received equal to the data sent, LED1
turn on, and if not, LED1 turn off.

  2.Modify files: example/SPI/SPI_master_transmit_slave_receive_interrupt/main.h, choose
to enable CRC function.
  #define SPI_CRC_ENABLE       1
  After the communication is completed, if the CRC checking is passed, LED1 turn on,
and if not, LED1 and LED2 turn off.

  Connect SPI1 NSS  PIN(PI0) TO SPI3 NSS  PIN(PE11).
  Connect SPI1 SCK  PIN(PI1) TO SPI3 SCK  PIN(PE12).
  Connect SPI1 MISO PIN(PI2) TO SPI3 MISO PIN(PE13).
  Connect SPI1 MOSI PIN(PI3) TO SPI3 MOSI PIN(PE14).
