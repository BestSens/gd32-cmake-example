/*!
    \file    main.c
    \brief   SPI fullduplex communication use DMA

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

#define arraysize         10

uint8_t spi1_send_array[arraysize] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t spi3_send_array[arraysize] = {0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA};
uint8_t spi1_receive_array[arraysize];
uint8_t spi3_receive_array[arraysize];
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint8_t length);

void rcu_config(void);
void gpio_config(void);
void dma_config(void);
void spi_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize led1 and led2 */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);

    /* enable peripheral clock */
    rcu_config();
    /* configure GPIO */
    gpio_config();
    /* configure DMA */
    dma_config();
    /* configure SPI */
    spi_config();

    /* enable SPI ti mode */
    spi_ti_mode_enable(SPI1);
    spi_ti_mode_enable(SPI3);

    /* enable SPI */
    spi_enable(SPI1);
    spi_enable(SPI3);

    /* enable DMA channel */
    dma_channel_enable(DMA1, DMA_CH4);
    dma_channel_enable(DMA1, DMA_CH3);
    dma_channel_enable(DMA0, DMA_CH4);
    dma_channel_enable(DMA0, DMA_CH3);

    /* enable SPI DMA */
    spi_dma_enable(SPI3, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI3, SPI_DMA_RECEIVE);
    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI1, SPI_DMA_RECEIVE);

    /* wait dma transmit complete */
    while(!dma_flag_get(DMA0, DMA_CH4, DMA_FLAG_FTF));
    while(!dma_flag_get(DMA0, DMA_CH3, DMA_FLAG_FTF));
    while(!dma_flag_get(DMA1, DMA_CH3, DMA_FLAG_FTF));
    while(!dma_flag_get(DMA1, DMA_CH4, DMA_FLAG_FTF));

    /* compare receive data with send data */
    if(memory_compare(spi3_receive_array, spi1_send_array, arraysize)) {
        gd_eval_led_on(LED1);
    } else {
        gd_eval_led_off(LED1);
    }
    if(memory_compare(spi1_receive_array, spi3_send_array, arraysize)) {
        gd_eval_led_on(LED2);
    } else {
        gd_eval_led_off(LED2);
    }

    while(1) {
    }
}

/*!
    \brief      configure different peripheral clocks
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rcu_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOI);
    rcu_periph_clock_enable(RCU_DMA0);
    rcu_periph_clock_enable(RCU_DMA1);
    rcu_periph_clock_enable(RCU_SPI1);
    rcu_periph_clock_enable(RCU_SPI3);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure SPI1 GPIO */
    gpio_af_set(GPIOI, GPIO_AF_5, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    gpio_mode_set(GPIOI, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    /* configure SPI3 GPIO */
    gpio_af_set(GPIOE, GPIO_AF_5, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
}

/*!
    \brief      configure the DMA peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dma_config(void)
{
    dma_single_data_parameter_struct dma_init_struct;

    /* configure SPI1 transmit dma */
    dma_deinit(DMA0, DMA_CH4);
    dma_init_struct.periph_addr         = (uint32_t)&SPI_DATA(SPI1);
    dma_init_struct.memory0_addr        = (uint32_t)spi1_send_array;
    dma_init_struct.direction           = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_8BIT;
    dma_init_struct.priority            = DMA_PRIORITY_LOW;
    dma_init_struct.number              = arraysize;
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;
    dma_single_data_mode_init(DMA0, DMA_CH4, &dma_init_struct);
    dma_channel_subperipheral_select(DMA0, DMA_CH4, DMA_SUBPERI0);

    /* configure SPI1 receive dma */
    dma_deinit(DMA0, DMA_CH3);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI1);
    dma_init_struct.memory0_addr = (uint32_t)spi1_receive_array;
    dma_init_struct.direction    = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH3, &dma_init_struct);
    dma_channel_subperipheral_select(DMA0, DMA_CH3, DMA_SUBPERI0);

    /* configure SPI3 transmit dma */
    dma_deinit(DMA1, DMA_CH4);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI3);
    dma_init_struct.memory0_addr = (uint32_t)spi3_send_array;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.priority     = DMA_PRIORITY_MEDIUM;
    dma_single_data_mode_init(DMA1, DMA_CH4, &dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH4, DMA_SUBPERI5);

    /* configure SPI3 receive dma */
    dma_deinit(DMA1, DMA_CH3);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI3);
    dma_init_struct.memory0_addr = (uint32_t)spi3_receive_array;
    dma_init_struct.direction    = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH3, &dma_init_struct);
    dma_channel_subperipheral_select(DMA1, DMA_CH3, DMA_SUBPERI5);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct spi_init_struct;

    /* configure SPI1 parameter */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_HARD;
    spi_init_struct.prescale             = SPI_PSC_64;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI1, &spi_init_struct);

    /* configure SPI3 parameter */
    spi_init_struct.trans_mode  = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode = SPI_SLAVE;
    spi_init_struct.nss         = SPI_NSS_HARD;
    spi_init_struct.prescale    = SPI_PSC_64;
    spi_init(SPI3, &spi_init_struct);
}

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint8_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}
