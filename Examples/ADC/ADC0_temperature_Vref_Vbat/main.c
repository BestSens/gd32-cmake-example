/*!
    \file    main.c
    \brief   ADC channel of temperature,Vref and Vbat
    
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

#include "gd32f4xx.h"
#include <stdio.h>
#include "systick.h"
#include "gd32f450i_eval.h"

float temperature;
float vref_value;
float vbat_value;

void rcu_config(void);
void adc_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* system clocks configuration */
    rcu_config();
    /* systick configuration */
    systick_config();
    /* ADC configuration */
    adc_config();
    /* USART configuration */
    gd_eval_com_init(EVAL_COM0);
    /* wait for conversion to complete */
    delay_1ms(10);
    
    /* value convert */
    temperature = (1.43 - ADC_IDATA0(ADC0)*3.3/4096) * 1000 / 4.3 + 25;
    vref_value = (ADC_IDATA1(ADC0) * 3.3 / 4096);
    vbat_value = (ADC_IDATA2(ADC0) * 3.3 / 4096) * 4;

    printf("  the temperature data is %2.0f degrees Celsius\r\n", temperature);
    printf(" the reference voltage data is %5.3fV \r\n", vref_value);
    printf(" the battery voltage is %5.3fV\r\n", vbat_value);
    
    while(1);
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* ADC channel length config */
    adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,3);

    /* ADC temperature sensor channel config */
    adc_inserted_channel_config(ADC0,0,ADC_CHANNEL_16,ADC_SAMPLETIME_144);
    /* ADC internal reference voltage channel config */
    adc_inserted_channel_config(ADC0,1,ADC_CHANNEL_17,ADC_SAMPLETIME_144);
    /* ADC 1/4 voltate of external battery config */
    adc_inserted_channel_config(ADC0,2,ADC_CHANNEL_18,ADC_SAMPLETIME_144);

    /* ADC external trigger disable */
    adc_external_trigger_config(ADC0,ADC_INSERTED_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
    /* ADC SCAN function enable */
    adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
    /* ADC Vbat channel enable */
    adc_channel_16_to_18(ADC_VBAT_CHANNEL_SWITCH,ENABLE);
    /* ADC temperature and Vref enable */
    adc_channel_16_to_18(ADC_TEMP_VREF_CHANNEL_SWITCH,ENABLE);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC0,ADC_INSERTED_CHANNEL);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t) ch);
    while (RESET == usart_flag_get(EVAL_COM0,USART_FLAG_TBE));
    return ch;
}
