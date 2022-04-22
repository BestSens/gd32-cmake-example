/*!
    \file    main.c
    \brief   ADC analog watchdog
    
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
#include "systick.h"
#include <stdio.h>
#include "gd32f450i_eval.h"

#define BOARD_ADC_CHANNEL   ADC_CHANNEL_3
#define ADC_GPIO_PORT       GPIOA
#define ADC_GPIO_PIN        GPIO_PIN_3
#define ADC_WATCHDOG_HT     0x0A00
#define ADC_WATCHDOG_LT     0x0400

uint16_t adc_value;
int data = 0;

void rcu_config(void);
void gpio_config(void);
void nvic_config(void);
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
    /* configure COM port */
    gd_eval_com_init(EVAL_COM0);
    /* GPIO configuration */
    gpio_config();
    /* NVIC configuration */
    nvic_config();
    /* ADC configuration */
    adc_config();

    while(1){
        delay_1ms(1000);
        adc_value = adc_regular_data_read(ADC1);
        printf("\r\n ADC1 watchdog low threshold: %04X \r\n",ADC_WATCHDOG_LT);
        printf("\r\n ADC1 watchdog hgih threshold: %04X \r\n",ADC_WATCHDOG_HT);
        printf("\r\n ADC1 regular channel data = %04X \r\n",adc_value);
        printf("\r\n ***********************************\r\n");
        if(1 == data){
            /* turn off LED2 */
            if((adc_value < ADC_WATCHDOG_HT) && (adc_value > ADC_WATCHDOG_LT)){
                data = 0;
                gd_eval_led_off(LED2);
            }
        }      
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
    /* configure led GPIO */
    gd_eval_led_init(LED2);
    /* config the GPIO as analog mode */
    gpio_mode_set(ADC_GPIO_PORT,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,ADC_GPIO_PIN);
}

/*!
    \brief      configure interrupt priority
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(ADC_IRQn, 2, 0);
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* ADC mode config */
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
    /* ADC continous and scan function enable */
    adc_special_function_config(ADC1, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC1, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC1, ADC_DATAALIGN_RIGHT);

    /* ADC regular channel config */
    adc_regular_channel_config(ADC1,0,BOARD_ADC_CHANNEL,ADC_SAMPLETIME_56);
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC1,ADC_REGULAR_CHANNEL,EXTERNAL_TRIGGER_DISABLE);

    /* ADC analog watchdog threshold config */
    adc_watchdog_threshold_config(ADC1,0x0400,0x0A00);
    /* ADC analog watchdog single channel config */
    adc_watchdog_single_channel_enable(ADC1,BOARD_ADC_CHANNEL);
    /* ADC analog watchdog enable on regular channel group*/
    adc_watchdog_group_channel_enable(ADC1,ADC_REGULAR_CHANNEL);

    /* ADC interrupt config */
    adc_interrupt_enable(ADC1, ADC_INT_WDE);

    /* enable ADC interface */
    adc_enable(ADC1);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);

    /* ADC software trigger enable */
    adc_software_trigger_enable(ADC1,ADC_REGULAR_CHANNEL);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t) ch);
    while (RESET == usart_flag_get(EVAL_COM0,USART_FLAG_TBE));
    return ch;
}
