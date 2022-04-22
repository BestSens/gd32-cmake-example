/*!
    \file    main.c
    \brief   the example of TRNG

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
#include <stdio.h>

FlagStatus g_tamperbutton_flag = RESET;
ErrStatus trng_configuration(void);
ErrStatus trng_ready_check(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t random_data = 0, random_lastdata = 0;
    uint8_t retry = 0;
    
    /* initialize  LED1 and turn on LED1 */
    gd_eval_led_init(LED1);
    gd_eval_led_on(LED1);
    /* initialize COM port */
    gd_eval_com_init(EVAL_COM0);
    
    /* configure the tamper key */
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_EXTI);
    
    /* show example introduce */
    printf("============ Gigadevice TRNG poll mode demo ============ \r\n");
    
    /* configure TRNG module */
    while((ERROR == trng_configuration()) && retry < 3){
        printf("TRNG init fail \r\n");
        printf("TRNG init retry \r\n");
        retry++;
    }
    
    printf("TRNG init ok \r\n");
    /* get the first random data */
    random_lastdata = trng_get_true_random_data();
    
    while (1){
        /* get random data if TEMPER key is pressed */
        if(SET == g_tamperbutton_flag){
            g_tamperbutton_flag = RESET;
            /* check wherther the random data is valid and get it */
            if(SUCCESS == trng_ready_check()){
                random_data = trng_get_true_random_data();
                if(random_data != random_lastdata){
                    random_lastdata = random_data;
                    printf("Get random data: 0x%08x \r\n", random_data);
                }else{
                    /* the random data is invalid */
                    printf("Error: Get the random data is same \r\n");
                }
            }
        }
    }
}

/*!
    \brief      check whether the TRNG module is ready
    \param[in]  none
    \param[out] none
    \retval     ErrStatus: SUCCESS or ERROR
*/
ErrStatus trng_ready_check(void)
{
    uint32_t timeout = 0;
    FlagStatus trng_flag = RESET;
    ErrStatus reval = SUCCESS;
    
    /* check wherther the random data is valid */
    do{
        timeout++;
        trng_flag = trng_flag_get(TRNG_FLAG_DRDY);
    }while((RESET == trng_flag) &&(0xFFFF > timeout));
    
    if(RESET == trng_flag)
    {   
        /* ready check timeout */
        printf("Error: TRNG can't ready \r\n");
        trng_flag = trng_flag_get(TRNG_FLAG_CECS);
        printf("Clock error current status: %d \r\n", trng_flag);
        trng_flag = trng_flag_get(TRNG_FLAG_SECS);
        printf("Seed error current status: %d \r\n", trng_flag);  
        reval = ERROR;
    }
    
    /* return check status */
    return reval;
}

/*!
    \brief      configure TRNG module
    \param[in]  none
    \param[out] none
    \retval     ErrStatus: SUCCESS or ERROR
*/
ErrStatus trng_configuration(void)
{
    ErrStatus reval = SUCCESS;
    
    /* TRNG module clock enable */
    rcu_periph_clock_enable(RCU_TRNG);
    
    /* TRNG registers reset */
    trng_deinit();
    trng_enable();
    /* check TRNG work status */
    reval = trng_ready_check();
    
    return reval;
}



/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TC));
    
    return ch;
}
