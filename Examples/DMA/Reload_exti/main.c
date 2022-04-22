/*!
    \file    main.c
    \brief   DMA reload transfer number by EXTI interrupt in discontinuous mode demo

    \version 2022-03-09, V1.0.0, firmware for GD32F4xx
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
#include "main.h"

#define USART0_DATA_ADDRESS      (uint32_t)&USART_DATA(USART0)

uint8_t welcome[] = "hi,this is a example: RAM_TO_USART by DMA !\n";
__IO uint32_t transfer_flag = 0;
FlagStatus g_transfer_complete = RESET;

void led_config(void);
void nvic_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    dma_single_data_parameter_struct dma_init_struct;
    /* enable DMA1 clock */
    rcu_periph_clock_enable(RCU_DMA1);
    /* USART configure */
    gd_eval_com_init(EVAL_COM0);
    /* initialize KEY */
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_EXTI);
    /*configure DMA interrupt*/
    nvic_config();
    /* initialize LED2 */
    led_config();
    /* turn off LED2 */
    gd_eval_led_off(LED2);

    /* initialize DMA1 channel7 */
    dma_deinit(DMA1, DMA_CH7);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr = (uint32_t)welcome;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(welcome);
    dma_init_struct.periph_addr = USART0_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH7);
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);

    /* USART DMA enable for transmission */
    usart_dma_transmit_config(USART0, USART_DENT_ENABLE);

    /* enable DMA1 transfer complete interrupt */
    dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);

    /* enable DMA1 channel7 */
    dma_channel_enable(DMA1, DMA_CH7);

    while(1) {
    }
}

/*!
    \brief      LEDs configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    gd_eval_led_init(LED2);
}

/*!
    \brief      configure DMA interrupt
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_config(void)
{
    nvic_irq_enable(DMA1_Channel7_IRQn, 0, 0);
}
