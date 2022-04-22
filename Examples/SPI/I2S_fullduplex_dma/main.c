/*!
    \file    main.c
    \brief   I2S full-duplex communication by DMA

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

#define ARRAYSIZE         8

uint16_t i2s1_txbuffer[ARRAYSIZE] = {0x8121, 0x7342, 0x9563, 0x6784, 0xA9A5, 0x5BC6, 0xBDE7, 0x4F05};
uint16_t i2s1_rxbuffer[ARRAYSIZE] = {0};
uint16_t i2s2_txbuffer[ARRAYSIZE] = {0x1234, 0x2335, 0x4564, 0x5675, 0x9854, 0x7551, 0x0503, 0x1CBE};
uint16_t i2s2_rxbuffer[ARRAYSIZE] = {0};

void dma_config(void);
void rcu_config(void);
void gpio_config(void);
void spi_config(void);
ErrStatus memory_compare(uint16_t *src, uint16_t *dst, uint8_t length);
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
    /* configure I2S */
    spi_config();
    /* enbale I2S and I2S_ADD*/
    i2s_enable(SPI2);
    i2s_enable(I2S2_ADD);
    i2s_enable(I2S1_ADD);
    i2s_enable(SPI1);

    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
    dma_channel_enable(DMA0, DMA_CH5);
    dma_channel_enable(DMA0, DMA_CH4);
    dma_channel_enable(DMA0, DMA_CH3);

    /* enable SPI DMA */
    spi_dma_enable(I2S1_ADD, SPI_DMA_RECEIVE);
    spi_dma_enable(SPI2, SPI_DMA_RECEIVE);
    spi_dma_enable(I2S2_ADD, SPI_DMA_TRANSMIT);
    spi_dma_enable(SPI1, SPI_DMA_TRANSMIT);

    /* wait dma transmit complete */
    while(!dma_flag_get(DMA0, DMA_CH0, DMA_INTF_FTFIF));
    while(!dma_flag_get(DMA0, DMA_CH5, DMA_INTF_FTFIF));
    while(!dma_flag_get(DMA0, DMA_CH4, DMA_INTF_FTFIF));
    while(!dma_flag_get(DMA0, DMA_CH3, DMA_INTF_FTFIF));

    /* compare receive data with send data */
    if(ERROR != memory_compare(i2s2_txbuffer, i2s1_rxbuffer, ARRAYSIZE)) {
        gd_eval_led_on(LED1);
    } else {
        gd_eval_led_off(LED1);
    }

    /* compare receive data with send data */
    if(ERROR != memory_compare(i2s1_txbuffer, i2s2_rxbuffer, ARRAYSIZE)) {
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
    /* enable GPIOA clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    /* enable GPIOB clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    /* enable SPI1/I2S1 clock */
    rcu_periph_clock_enable(RCU_SPI1);
    /* enable SPI2/I2S2 clock */
    rcu_periph_clock_enable(RCU_SPI2);
    /* enable DMA0 clock */
    rcu_periph_clock_enable(RCU_DMA0);
    /* enable DMA1 clock */
    rcu_periph_clock_enable(RCU_DMA1);
}

/*!
    \brief      configure the GPIO peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_config(void)
{
    /* configure I2S1 and I2S1_ADD pins: I2S1_WS(PB12), I2S1_CK(PB13), I2S1_SD(PB15), I2S1_ADD_SD(PB14) */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_af_set(GPIOB, GPIO_AF_5, GPIO_PIN_12);
    gpio_af_set(GPIOB, GPIO_AF_5, GPIO_PIN_13);
    gpio_af_set(GPIOB, GPIO_AF_6, GPIO_PIN_14);
    gpio_af_set(GPIOB, GPIO_AF_5, GPIO_PIN_15);

    /* configure I2S2 and I2S2_ADD pins: I2S2_WS(PA15), I2S2_CK(PB3), I2S2_SD(PB5), I2S2_ADD_SD(PB4) */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN_15);
    gpio_af_set(GPIOB, GPIO_AF_6, GPIO_PIN_3);
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_4);
    gpio_af_set(GPIOB, GPIO_AF_6, GPIO_PIN_5);
    gpio_af_set(GPIOA, GPIO_AF_6, GPIO_PIN_15);
}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_i2s_deinit(SPI2);
    spi_i2s_deinit(SPI1);

    /* configure I2S1 and I2S1_ADD */
    i2s_init(SPI1, I2S_MODE_MASTERTX, I2S_STD_PHILLIPS, I2S_CKPL_LOW);
    i2s_psc_config(SPI1, I2S_AUDIOSAMPLE_11K, I2S_FRAMEFORMAT_DT16B_CH16B, I2S_MCKOUT_DISABLE);
    i2s_full_duplex_mode_config(I2S1_ADD, I2S_MODE_MASTERTX, I2S_STD_PHILLIPS, I2S_CKPL_LOW, I2S_FRAMEFORMAT_DT16B_CH16B);

    /* configure I2S2 and I2S2_ADD */
    i2s_init(SPI2, I2S_MODE_SLAVERX, I2S_STD_PHILLIPS, I2S_CKPL_LOW);
    i2s_psc_config(SPI2, I2S_AUDIOSAMPLE_11K, I2S_FRAMEFORMAT_DT16B_CH16B, I2S_MCKOUT_DISABLE);
    i2s_full_duplex_mode_config(I2S2_ADD, I2S_MODE_SLAVERX, I2S_STD_PHILLIPS, I2S_CKPL_LOW, I2S_FRAMEFORMAT_DT16B_CH16B);

}

void dma_config(void)
{
    dma_single_data_parameter_struct dma_init_struct;

    dma_single_data_para_struct_init(&dma_init_struct);

    /* configure SPI1 transmit dma */
    dma_deinit(DMA0, DMA_CH4);
    dma_init_struct.periph_addr         = (uint32_t)&SPI_DATA(SPI1);
    dma_init_struct.memory0_addr        = (uint32_t)i2s1_txbuffer;
    dma_init_struct.direction           = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    dma_init_struct.priority            = DMA_PRIORITY_LOW;
    dma_init_struct.number              = ARRAYSIZE;
    dma_init_struct.periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.memory_inc          = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.circular_mode       = DMA_CIRCULAR_MODE_DISABLE;
    dma_single_data_mode_init(DMA0, DMA_CH4, &dma_init_struct);
    dma_channel_subperipheral_select(DMA0, DMA_CH4, DMA_SUBPERI0);

    /* configure I2S1_ADD receive dma */
    dma_deinit(DMA0, DMA_CH3);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(I2S1_ADD);
    dma_init_struct.memory0_addr = (uint32_t)i2s1_rxbuffer;
    dma_init_struct.direction    = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH3, &dma_init_struct);
    dma_channel_subperipheral_select(DMA0, DMA_CH3, DMA_SUBPERI3);

    /* configure I2S2_ADD transmit dma */
    dma_deinit(DMA0, DMA_CH5);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(I2S2_ADD);
    dma_init_struct.memory0_addr = (uint32_t)i2s2_txbuffer;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPH;
    dma_init_struct.priority     = DMA_PRIORITY_LOW;
    dma_single_data_mode_init(DMA0, DMA_CH5, &dma_init_struct);
    dma_channel_subperipheral_select(DMA0, DMA_CH5, DMA_SUBPERI2);

    /* configure SPI2 receive dma */
    dma_deinit(DMA0, DMA_CH0);
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(SPI2);
    dma_init_struct.memory0_addr = (uint32_t)i2s2_rxbuffer;
    dma_init_struct.direction    = DMA_PERIPH_TO_MEMORY;
    dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA0, DMA_CH0, &dma_init_struct);
    dma_channel_subperipheral_select(DMA0, DMA_CH0, DMA_SUBPERI0);
}
/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus memory_compare(uint16_t *src, uint16_t *dst, uint8_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}
