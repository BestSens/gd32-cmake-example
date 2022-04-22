/*!
    \file  main.c
    \brief ADC0_ADC1_ADC2_inserted_parallel
    
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
#include "gd32f450i_eval.h"
#include "systick.h"
#include <stdio.h>

uint16_t inserted_data[6];

void rcu_config(void);
void gpio_config(void);
void adc_config(void);
void timer_config(void);
void nvic_config(void);

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
    /* GPIO configuration */
    gpio_config();
    /* NVIC configuration */
    nvic_config();
    /* configure EVAL_COM1 */
    gd_eval_com_init(EVAL_COM0);
    /* TIMER configuration */
    timer_config();
    /* ADC configuration */
    adc_config();

    while(1){
        delay_1ms(1000);
        printf("\n ADC0: PA0, inserted_data[0] = %04X \n",inserted_data[0]);
        printf("\n ADC1: PA1, inserted_data[1] = %04X \n",inserted_data[1]);
        printf("\n ADC2: PA2, inserted_data[2] = %04X \n",inserted_data[2]);
        printf("\n ADC0: PA3, inserted_data[3] = %04X \n",inserted_data[3]);
        printf("\n ADC1: PA4, inserted_data[4] = %04X \n",inserted_data[4]);
        printf("\n ADC2: PF7, inserted_data[5] = %04X \n",inserted_data[5]);
        printf("\n ******************* \n");
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
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable GPIOF clock */
    rcu_periph_clock_enable(RCU_GPIOF);
    /* enable timer1 clock */
    rcu_periph_clock_enable(RCU_TIMER1);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    /* enable ADC0 clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable ADC1 clock */
    rcu_periph_clock_enable(RCU_ADC1);
    /* enable ADC1 clock */
    rcu_periph_clock_enable(RCU_ADC2);
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
    /* ADC0: PA0,PA3 
    ** ADC1: PA1,PA4
    ** ADC2: PA2,PF7
    **/
    gpio_mode_set(GPIOA,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                    |GPIO_PIN_3|GPIO_PIN_4);
    gpio_mode_set(GPIOF,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO_PIN_7);
}

/*!
    \brief      configure the ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* configure the ADC sync mode */
    adc_sync_mode_config(ADC_ALL_INSERTED_PARALLEL);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0,ADC_DATAALIGN_RIGHT);
    adc_data_alignment_config(ADC1,ADC_DATAALIGN_RIGHT);
    adc_data_alignment_config(ADC2,ADC_DATAALIGN_RIGHT);
    /* ADC SCAN function enable */
    adc_special_function_config(ADC0,ADC_SCAN_MODE,ENABLE);
    adc_special_function_config(ADC1,ADC_SCAN_MODE,ENABLE);
    adc_special_function_config(ADC2,ADC_SCAN_MODE,ENABLE);
    
    /* ADC channel length config */
    adc_channel_length_config(ADC0,ADC_INSERTED_CHANNEL,2);
    adc_channel_length_config(ADC1,ADC_INSERTED_CHANNEL,2);
    adc_channel_length_config(ADC2,ADC_INSERTED_CHANNEL,2);
    /* ADC regular channel config */
    adc_inserted_channel_config(ADC0,0,ADC_CHANNEL_0,ADC_SAMPLETIME_15);
    adc_inserted_channel_config(ADC0,1,ADC_CHANNEL_3,ADC_SAMPLETIME_15);
    adc_inserted_channel_config(ADC1,0,ADC_CHANNEL_1,ADC_SAMPLETIME_15);
    adc_inserted_channel_config(ADC1,1,ADC_CHANNEL_4,ADC_SAMPLETIME_15);
    adc_inserted_channel_config(ADC2,0,ADC_CHANNEL_2,ADC_SAMPLETIME_15);
    adc_inserted_channel_config(ADC2,1,ADC_CHANNEL_5,ADC_SAMPLETIME_15);
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC0,ADC_INSERTED_CHANNEL,EXTERNAL_TRIGGER_RISING);
    adc_external_trigger_config(ADC1,ADC_INSERTED_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
    adc_external_trigger_config(ADC2,ADC_INSERTED_CHANNEL,EXTERNAL_TRIGGER_DISABLE);
    adc_external_trigger_source_config(ADC0,ADC_INSERTED_CHANNEL,ADC_EXTTRIG_INSERTED_T1_CH0);
    
    /* clear the ADC flag */
    adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOC);
    adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOIC);
    /* enable ADC interrupt */
    adc_interrupt_enable(ADC0, ADC_INT_EOIC);

    /* enable ADC interface */
    adc_enable(ADC0);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);
    /* enable ADC interface */
    adc_enable(ADC1);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC1);
    /* enable ADC interface */
    adc_enable(ADC2);
    /* wait for ADC stability */
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC2);
}

/*!
    \brief      configure the timer peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void timer_config(void)
{
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    /* TIMER1 configuration */
    timer_initpara.prescaler         = 19999;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 9999;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_initpara.repetitioncounter = 0;
    timer_init(TIMER1,&timer_initpara);

    /* CH1 configuration in PWM mode0 */
    timer_ocintpara.ocpolarity  = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_channel_output_config(TIMER1,TIMER_CH_0,&timer_ocintpara);

    timer_channel_output_pulse_value_config(TIMER1,TIMER_CH_0,9900);
    timer_channel_output_mode_config(TIMER1,TIMER_CH_0,TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER1,TIMER_CH_0,TIMER_OC_SHADOW_DISABLE);
    
    /* enable TIMER1 */
    timer_enable(TIMER1);
}

/**
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
  */
void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(ADC_IRQn, 1, 1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
