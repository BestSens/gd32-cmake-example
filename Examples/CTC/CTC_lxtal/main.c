/*!
    \file    main.c
    \brief   CTC is used to trim internal 48MHz RC oscillator with LXTAL clock
    
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
#include "gd32f450i_eval.h"

void ctc_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initilize LED1 */
    gd_eval_led_init(LED1);

    /* enable IRC48M clock */
    rcu_osci_on(RCU_IRC48M);
    /* wait until IRC48M is ready */
    while(ERROR == rcu_osci_stab_wait(RCU_IRC48M)) {
    }

    /* configure LXTAL clock */
    rcu_periph_clock_enable(RCU_PMU);
    pmu_backup_write_enable();

    /* enable LXTAL clock */
    rcu_osci_on(RCU_LXTAL);
    /* wait until LXTAL is ready */
    while(ERROR == rcu_osci_stab_wait(RCU_LXTAL)) {
    }
    rcu_ckout0_config(RCU_CKOUT0SRC_LXTAL,RCU_CKOUT0_DIV1);


    /* configure CTC */
    ctc_config();

    while(1){
        /* if the clock trim is OK */
        if(ctc_flag_get(CTC_FLAG_CKOK) != RESET) {
            gd_eval_led_on(LED1);
        }else {
            gd_eval_led_off(LED1);
        }
    }
}

/*!
    \brief      configure the CTC peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void ctc_config(void)
{
    /* CTC peripheral clock enable */
    rcu_periph_clock_enable(RCU_CTC);
    /* configure CTC reference signal source prescaler */
    ctc_refsource_prescaler_config(CTC_REFSOURCE_PSC_OFF);
    /* select reference signal source */
    ctc_refsource_signal_select(CTC_REFSOURCE_LXTAL);
    /* select reference signal source polarity */
    ctc_refsource_polarity_config(CTC_REFSOURCE_POLARITY_RISING);
    /* configure hardware automatically trim mode */
    ctc_hardware_trim_mode_config(CTC_HARDWARE_TRIM_MODE_ENABLE);
    
    /* configure CTC counter reload value */
    ctc_counter_reload_value_config(0x05B8);
    /* configure clock trim base limit value */
    ctc_clock_limit_value_config(0x0002);

    /* enable CTC counter */
    ctc_counter_enable();
}
