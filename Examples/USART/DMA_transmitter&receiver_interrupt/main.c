/*!
    \file    gd32f4xx_libopt.h
    \brief   library optional for gd32f4xx

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

#include <stdio.h>
#include "gd32f4xx.h"
#include "gd32f450i_eval.h"

#define USART0_DATA_ADDRESS      ((uint32_t)&USART_DATA(USART0))
#define ARRAYNUM(arr_nanme)      (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))

__IO FlagStatus g_transfer_complete = RESET;
uint8_t rxbuffer[10];
uint8_t txbuffer[] = "\n\rUSART DMA interrupt receive and transmit example, please input 10 bytes:\n\r";

void nvic_config(void);
void dma_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* enable DMA1 clock */
    rcu_periph_clock_enable(RCU_DMA1);
    /* initialize USART */
    gd_eval_com_init(EVAL_COM0);

    /* USART DMA enable*/
    usart_dma_receive_config(USART0, USART_DENR_ENABLE);
    usart_dma_transmit_config(USART0, USART_DENT_ENABLE);

    /*configure DMA1 interrupt*/
    nvic_config();
    dma_config();

    /* waiting for the transfer to complete*/
    while(RESET == g_transfer_complete);

    g_transfer_complete = RESET;

    /* waiting for the transfer to complete*/
    while(RESET == g_transfer_complete);

    printf("\n\r%s\n\r", rxbuffer);
    while(1);
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
    nvic_irq_enable(DMA1_Channel2_IRQn, 0, 1);
}

/*!
    \brief      configure DMA
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    dma_single_data_parameter_struct dma_init_struct;

    /* deinitialize DMA1 channel7(USART0 TX) */
    dma_single_data_para_struct_init(&dma_init_struct);
    dma_deinit(DMA1, DMA_CH7);
    dma_init_struct.direction = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.memory0_addr = (uint32_t)txbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.number = ARRAYNUM(txbuffer);
    dma_init_struct.periph_addr = USART0_DATA_ADDRESS;
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH7, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH7);
    dma_channel_subperipheral_select(DMA1, DMA_CH7, DMA_SUBPERI4);
    /* enable DMA1 channel7 transfer complete interrupt */
    dma_interrupt_enable(DMA1, DMA_CH7, DMA_CHXCTL_FTFIE);
    /* enable DMA1 channel7 */
    dma_channel_enable(DMA1, DMA_CH7);

    /* deinitialize DMA1 channel2 (USART0 RX) */
    dma_deinit(DMA1, DMA_CH2);
    dma_init_struct.direction = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.memory0_addr = (uint32_t)rxbuffer;
    dma_init_struct.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.number = 10;
    dma_init_struct.periph_addr = (uint32_t)&USART_DATA(USART0);
    dma_init_struct.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH2, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(DMA1, DMA_CH2);
    dma_channel_subperipheral_select(DMA1, DMA_CH2, DMA_SUBPERI4);
    /* enable DMA1 channel2 transfer complete interrupt */
    dma_interrupt_enable(DMA1, DMA_CH2, DMA_CHXCTL_FTFIE);
    /* enable DMA1 channel2 */
    dma_channel_enable(DMA1, DMA_CH2);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
