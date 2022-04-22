/*!
    \file    main.c
    \brief   DMA transfer data from RAM to RAM

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
#include "gd32f4xx_dma.h"

#define BUFFER_SIZE     (256U)

static void fill_buffer_size(void);
static ErrStatus memory_compare(uint16_t *src, uint16_t *dst, uint16_t length);

uint16_t source[BUFFER_SIZE];
uint16_t destination[BUFFER_SIZE];
ErrStatus status;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    dma_multi_data_parameter_struct  dma_init_parameter;

    /* LED initialize */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED3);

    /* peripheral clock enable */
    rcu_periph_clock_enable(RCU_DMA1);

    /* DMA peripheral configure */
    dma_deinit(DMA1, DMA_CH0);

    dma_init_parameter.periph_addr = (uint32_t)source;
    dma_init_parameter.periph_width = DMA_PERIPH_WIDTH_16BIT;
    dma_init_parameter.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_init_parameter.memory0_addr = (uint32_t)destination;
    dma_init_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_init_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_parameter.memory_burst_width = DMA_MEMORY_BURST_4_BEAT;
    dma_init_parameter.periph_burst_width = DMA_PERIPH_BURST_4_BEAT;
    dma_init_parameter.critical_value = DMA_FIFO_2_WORD;
    dma_init_parameter.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_init_parameter.direction = DMA_MEMORY_TO_MEMORY;
    dma_init_parameter.number = BUFFER_SIZE;
    dma_init_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_multi_data_mode_init(DMA1, DMA_CH0, &dma_init_parameter);

    fill_buffer_size();

    dma_channel_enable(DMA1, DMA_CH0);

    while(!dma_flag_get(DMA1, DMA_CH0, DMA_INTF_FTFIF));

    status = memory_compare((uint16_t *)source, (uint16_t *)destination, BUFFER_SIZE);

    if(SUCCESS == status) {
        gd_eval_led_on(LED1);
    } else {
        gd_eval_led_on(LED3);
    }

    while(1) {
    }
}

/*!
    \brief      fill the buffer with specified value
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fill_buffer_size(void)
{
    uint32_t i;
    for(i = 0; i < BUFFER_SIZE; i++) {
        source[i] = i;
    }
}

/*!
    \brief      memory compare function
    \param[in]  src : source data
    \param[in]  dst : destination data
    \param[in]  length : the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint16_t *src, uint16_t *dst, uint16_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}
