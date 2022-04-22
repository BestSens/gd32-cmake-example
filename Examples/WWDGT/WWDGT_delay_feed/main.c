/*!
    \file    main.c
    \brief   WWDGT delay feed demo 

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
#include "gd32f450i_eval.h"

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

    /* configure LED1 and LED2 */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    
    /* turn off LED1 and LED2 */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    
    /* delay */
    delay_1ms(150);

    /* check if the system has resumed from WWDGT reset */
    if(RESET != rcu_flag_get(RCU_FLAG_WWDGTRST)){
        /* WWDGTRST flag set */
        gd_eval_led_on(LED1);
        /* clear the WWDGTRST flag */
        rcu_all_reset_flag_clear();
        
        while(1);
    }

    /* enable WWDGT clock */
    rcu_periph_clock_enable(RCU_WWDGT);

    /*
     *  System clock up to 200Mhz, PCLK1=50MHz 
     *  set WWDGT clock = (PCLK1 (50MHz)/4096)/4 = 3051Hz (~328 us)   
     *  set counter value to 127
     *  set window value to 80
     *  refresh window is: ~328 * (127-80)= 15.4ms < refresh window < ~328 * (127-63) =20.9ms.
     */
    wwdgt_config(127, 80, WWDGT_CFG_PSC_DIV4);
    wwdgt_enable();

    while(1){
        /* toggle LED2 */
        gd_eval_led_toggle(LED2);
        /* insert 18 ms delay */
        delay_1ms(18);
        /* update WWDGT counter */
        wwdgt_counter_update(127);
    }

}
