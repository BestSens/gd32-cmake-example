/*!
    \file    main.c
    \brief   DAC ADC convert demo

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

#include "gd32f4xx.h"
#include "systick.h"

/* configure RCU peripheral */
void rcu_config(void);
/* configure GPIO peripheral */
void gpio_config(void);
/* configure NVIC peripheral */
void nvic_config(void);
/* configure ADC peripheral */
void adc_config(void);
/* configure DAC peripheral */
void dac_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure systick */
    systick_config();

    /* configure RCU peripheral */
    rcu_config();
    /* configure GPIO peripheral */
    gpio_config();
    /* configure NVIC peripheral */
    nvic_config();
    /* configure ADC peripheral */
    adc_config();
    /* configure DAC peripheral */
    dac_config();

    while (1){
    }
}

/*!
    \brief      configure RCU peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable GPIOC clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    /* enable ADC clock */
    rcu_periph_clock_enable(RCU_ADC0);
    /* enable DAC clock */
    rcu_periph_clock_enable(RCU_DAC);
}

/*!
    \brief      configure GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure PC3 as ADC input */
    gpio_mode_set(GPIOC, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_3);

    /* configure PA4 as DAC output */
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4);
}

/*!
    \brief      configure NVIC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(ADC_IRQn, 0, 0);
}

/*!
    \brief      configure ADC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void adc_config(void)
{
    /* ADC continous function enable */
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);
    /* ADC data alignment config */
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    /* ADC channel length config */
    adc_channel_length_config(ADC0, ADC_ROUTINE_CHANNEL, 1);
    /* ADC routine channel config */
    adc_routine_channel_config(ADC0, 0, ADC_CHANNEL_13, ADC_SAMPLETIME_56);
    /* ADC external trigger enable */
    adc_external_trigger_config(ADC0, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    /* enable ADC interface */
    adc_enable(ADC0);  
    delay_1ms(1);
    /* ADC calibration and reset calibration */
    adc_calibration_enable(ADC0);

    /* ADC interrupt enable */
    adc_interrupt_flag_clear(ADC0, ADC_INT_FLAG_EOC);
    adc_interrupt_enable(ADC0, ADC_INT_EOC);

    /* enable ADC software trigger */
    adc_software_trigger_enable(ADC0, ADC_ROUTINE_CHANNEL);
}

/*!
    \brief      configure DAC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dac_config(void)
{
    /* initialize DAC */
    dac_deinit(DAC0);
    /* DAC trigger config */
    dac_trigger_source_config(DAC0, DAC_OUT0, DAC_TRIGGER_SOFTWARE);
    /* DAC trigger enable */
    dac_trigger_enable(DAC0, DAC_OUT0);
    /* DAC wave mode config */
    dac_wave_mode_config(DAC0, DAC_OUT0, DAC_WAVE_DISABLE);
    /* DAC output buffer config */
    dac_output_buffer_enable(DAC0, DAC_OUT0);

    /* DAC enable */
    dac_enable(DAC0, DAC_OUT0);
}
