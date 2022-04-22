/*!
    \file    main.c
    \brief   UASRT receiver timeout

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
#include "gd32f450i_eval.h"

uint8_t rxbuffer[64];
uint8_t txbuffer[64];
extern __IO uint8_t txcount;
extern __IO uint16_t rxcount;
void nvic_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t i = 0, j = 0;
    nvic_config();
    gd_eval_com_init(EVAL_COM0);
    printf("a usart receive timeout test example!");

    while(1) {
        if(0 == rxcount) {
            /* enable the USART receive interrupt */
            usart_interrupt_enable(USART0, USART_INT_RBNE);
        } else {
            /* enable the USART receive timeout and configure the time of timeout */
            usart_receiver_timeout_enable(USART0);
            usart_receiver_timeout_threshold_config(USART0, 115200 * 3);

            while(RESET == usart_flag_get(USART0, USART_FLAG_RT));
            for(i = 0; i < rxcount; i++) {
                txbuffer[i] = rxbuffer[j++];
            }
            /* disable the USART receive interrupt and enable the USART transmit interrupt */
            usart_interrupt_disable(USART0, USART_INT_RBNE);
            usart_interrupt_enable(USART0, USART_INT_TBE);
            while(txcount < rxcount) {
            }
            usart_flag_clear(USART0, USART_FLAG_RT);
            txcount = 0;
            rxcount = 0;
            i = 0;
            j = 0;
        }
    }
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_irq_enable(USART0_IRQn, 0, 1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
