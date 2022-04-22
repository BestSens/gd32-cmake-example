/*!
    \file    readme.txt
    \brief   description of the ADC0_ADC1_ADC2_regular_parallel
    
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

  This demo is based on the GD32450I-EVAL-V1.1, it shows how to
use ADC0, ADC1 and ADC2 regular_parallel convert function.PA0/PA1
/PA2/PA3/PA4/PF7 are configured as AN mode.TIMER1_CH1 is the trigger 
source of ADC0.ADC1 and ADC2 external trigger is disabled.When the 
rising edge of TIMER1_CH1 coming,ADC0, ADC1 and ADC2 regular channel 
are triggered at the same time.The values of ADC0,ADC1 and ADC2 are 
transmit to array adc_value[] by DMA.
  We can watch array adc_value[] in debug mode or by COM0.Also we can 
watch array adc_value[] in debug mode.

  ADC0: PA0(ADC0_IN0),PA3(ADC0_IN3) 
  ADC1: PA1(ADC1_IN1),PA4(ADC1_IN4)
  ADC2: PA2(ADC2_IN2),PF7(ADC2_IN5)
