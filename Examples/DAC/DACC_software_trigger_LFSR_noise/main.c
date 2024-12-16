/*!
    \file    main.c
    \brief   DAC concurrent mode software trigger LFSR noise wave demo

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

/* configure RCU peripheral */
void rcu_config(void);
/* configure GPIO peripheral */
void gpio_config(void);
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
    /* configure RCU peripheral */
    rcu_config();
    /* configure GPIO peripheral */
    gpio_config();
    /* configure DAC peripheral */
    dac_config();

    while(1){
        /* DAC software trigger enable */
        dac_concurrent_software_trigger_enable(DAC0);
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
    /* enable the clock of peripherals */
    rcu_periph_clock_enable(RCU_GPIOA);
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
    /* configure PA4 and PA5 as DAC output */
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_4 | GPIO_PIN_5);
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
    dac_trigger_source_config(DAC0, DAC_OUT1, DAC_TRIGGER_SOFTWARE);
    /* DAC trigger enable */
    dac_trigger_enable(DAC0, DAC_OUT0);
    dac_trigger_enable(DAC0, DAC_OUT1);
    /* DAC wave mode config */
    dac_wave_mode_config(DAC0, DAC_OUT0, DAC_WAVE_MODE_LFSR);
    dac_lfsr_noise_config(DAC0, DAC_OUT0, DAC_LFSR_BITS9_0);
    dac_wave_mode_config(DAC0, DAC_OUT1, DAC_WAVE_MODE_LFSR);
    dac_lfsr_noise_config(DAC0, DAC_OUT1, DAC_LFSR_BITS10_0);

    /* DAC enable */
    dac_concurrent_enable(DAC0);
    dac_concurrent_data_set(DAC0, DAC_ALIGN_12B_R, 0x7F0, 0x7F0);
}
