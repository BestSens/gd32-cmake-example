/*!
    \file    main.c
    \brief   ADC modresolution oversample
    
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
#include "gd32f4xx_libopt.h"
#include <stdio.h>
#include "systick.h"
#include "gd32f450i_eval.h"

#define BOARD_ADC_CHANNEL ADC_CHANNEL_3
#define ADC_GPIO_PORT GPIOA
#define ADC_GPIO_PIN GPIO_PIN_3

void rcu_config(void);
void gpio_config(void);
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
    /* GPIO configuration */
    gpio_config();
    /* SYSTICK configuration */
    systick_config();
    /* ADC configuration */
    adc_config();
    /* configures COM port */
    gd_eval_com_init(EVAL_COM0);

    while(1){
        /* ADC resolusion 12B */
        printf("\r\n resolusion 12B:\r\n");
        adc_resolution_config(ADC1,ADC_RESOLUTION_12B);
        adc_oversample_mode_config(ADC1,ADC_OVERSAMPLING_ALL_CONVERT,ADC_OVERSAMPLING_SHIFT_NONE,ADC_OVERSAMPLING_SHIFT_NONE);
        adc_oversample_mode_disable(ADC1);
        /* 1 time sample */
        adc_enable(ADC1);
        adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);
        while(RESET == adc_flag_get(ADC1,ADC_FLAG_EOC));
        delay_1ms(500);
        printf(" 1 time sample,the data is %d\r\n",ADC_RDATA(ADC1));
        adc_disable(ADC1);

        /* 16 times sample ,no shift */
        adc_oversample_mode_config(ADC1,ADC_OVERSAMPLING_ALL_CONVERT,ADC_OVERSAMPLING_SHIFT_NONE,ADC_OVERSAMPLING_RATIO_MUL16);
        adc_oversample_mode_enable(ADC1);
        adc_enable(ADC1);
        adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);
        while(RESET == adc_flag_get(ADC1,ADC_FLAG_EOC));
        delay_1ms(500);
        printf(" 16 times sample,no shift,the data is %d\r\n",ADC_RDATA(ADC1));
        adc_disable(ADC1);

        /* 16 times sample ,4 bits shift */
        adc_oversample_mode_config(ADC1,ADC_OVERSAMPLING_ALL_CONVERT,ADC_OVERSAMPLING_SHIFT_4B,ADC_OVERSAMPLING_RATIO_MUL16);
        adc_enable(ADC1);
        adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);
        while(RESET == adc_flag_get(ADC1,ADC_FLAG_EOC));
        delay_1ms(500);
        printf(" 16 times sample,4 bits shift,the data is %d\r\n",ADC_RDATA(ADC1));
        adc_disable(ADC1);

        /* ADC resolusion 6B */
        printf("\r\n resolusion 6B:\r\n");
        adc_resolution_config(ADC1,ADC_RESOLUTION_6B);
        adc_oversample_mode_config(ADC1,ADC_OVERSAMPLING_ALL_CONVERT,ADC_OVERSAMPLING_SHIFT_NONE,ADC_OVERSAMPLING_SHIFT_NONE);
        adc_oversample_mode_disable(ADC1);
        /* 1 time sample */
        adc_enable(ADC1);
        adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);
        while(RESET == adc_flag_get(ADC1,ADC_FLAG_EOC));
        delay_1ms(500);
        printf(" 1 time sample,the data is %d\r\n",ADC_RDATA(ADC1));
        adc_disable(ADC1);

        /* 16 times sample ,no shift */
        adc_oversample_mode_config(ADC1,ADC_OVERSAMPLING_ALL_CONVERT,ADC_OVERSAMPLING_SHIFT_NONE,ADC_OVERSAMPLING_RATIO_MUL16);
        adc_oversample_mode_enable(ADC1);
        adc_enable(ADC1);
        adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);
        while(RESET == adc_flag_get(ADC1,ADC_FLAG_EOC));
        delay_1ms(500);
        printf(" 16 times sample,no shift,the data is %d\r\n",ADC_RDATA(ADC1));
        adc_disable(ADC1);

        /* 16 times sample ,4 bits shift */
        adc_oversample_mode_config(ADC1,ADC_OVERSAMPLING_ALL_CONVERT,ADC_OVERSAMPLING_SHIFT_4B,ADC_OVERSAMPLING_RATIO_MUL16);
        adc_enable(ADC1);
        adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);
        while(RESET == adc_flag_get(ADC1,ADC_FLAG_EOC));
        delay_1ms(500);
        printf(" 16 times sample,4 bits shift,the data is %d\r\n",ADC_RDATA(ADC1));
        adc_disable(ADC1);
    }
}

/*!
    \brief      configure the different system clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC1);
    /* config ADC clock */
    adc_clock_config(ADC_ADCCK_PCLK2_DIV8);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* config the GPIO as analog mode */
    gpio_mode_set(ADC_GPIO_PORT,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC_GPIO_PIN);
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
    adc_channel_length_config(ADC1,ADC_REGULAR_CHANNEL,1);
    /* ADC regular channel config */
    adc_regular_channel_config(ADC1,0,BOARD_ADC_CHANNEL,ADC_SAMPLETIME_56);

    /* ADC external trigger enable */
    adc_external_trigger_config(ADC1,ADC_REGULAR_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC1,ADC_DATAALIGN_RIGHT);
    /* enable ADC interface */
    adc_enable(ADC1);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);
    /* disable ADC interface */
    adc_disable(ADC1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t) ch);
    while (RESET == usart_flag_get(EVAL_COM0,USART_FLAG_TBE));
    return ch;
}
