/*!
    \file    main.c
    \brief   reset source detect example

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
#include <stdio.h>

void mcu_software_reset(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize the Systick, USART and key */
    systick_config();
    gd_eval_com_init(EVAL_COM0);
      gd_eval_led_init(LED2);
      gd_eval_led_init(LED3);

    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);
    
    if(RESET != rcu_flag_get(RCU_FLAG_PORRST)){
          gd_eval_led_on(LED2);
        printf("\r\nPower reset generated");
    }
    
    if(RESET != rcu_flag_get(RCU_FLAG_SWRST)){
             gd_eval_led_on(LED3);
        printf("\r\nSoftware reset generated");
    }
    
    if((RESET != rcu_flag_get(RCU_FLAG_EPRST)) && (RESET == rcu_flag_get(RCU_FLAG_PORRST))){
        printf("\r\nExternal PIN reset generated");
    }
    /* clear all reset flags */
    rcu_all_reset_flag_clear();
    
    while(1){
        /* press Wakeup key to software reset mcu */
        if(RESET == gd_eval_key_state_get(KEY_WAKEUP)){
            delay_1ms(100);
            if(RESET == gd_eval_key_state_get(KEY_WAKEUP)){
                delay_1ms(50);
                mcu_software_reset();
            }
        }
    }
}

/*!
    \brief      software reset mcu
    \param[in]  none
    \param[out] none
    \retval     none
*/
void mcu_software_reset(void)
{
    /* set FAULTMASK */
    __set_FAULTMASK(1);
    NVIC_SystemReset();
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
