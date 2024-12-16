/*!
    \file    readme.txt
    \brief   description of DACC_independent_trigger_LFSR_noise example

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

  This example is based on the GD32450I-EVAL-V1.1 board, it shows how to use DAC concurrent 
mode to generate LFSR noise wave with different trigger signal TIMER5 and TIMER6.

  TIMER5 and TIMER6 are configured as following:
  - Up counting mode.
  - System clock devide into 1Mhz.
  - Update event per millisecond.
  - Update event as TRGO to trigger DAC0_OUT0/DAC0_OUT1.
  DAC is configured as following:
  - Data 12-bit right alligned.
  - TIMER5/TIMER6 TRGO signal trigger DAC0_OUT0/DAC0_OUT1.
  - DAC0_OUT0/DAC0_OUT1 is configured as PA4/PA5.
  - LFSR mode, with 0-9/0-10 bit masking and offset value 0x7f0.

  After system start-up, the LFSR noise wave can be observed through the scilloscope.
