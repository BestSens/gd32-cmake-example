/*!
    \file    main.c
    \brief   master send and slave receive data use interrupt mode

    \version 2024-12-20, V3.3.1, firmware for GD32F4xx
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
#include "gd32f4xx_it.h"
#include "gd32f450i_eval.h"
#include "main.h"

#define SET_SPI1_NSS_HIGH          gpio_bit_set(GPIOI,GPIO_PIN_0);
#define SET_SPI1_NSS_LOW           gpio_bit_reset(GPIOI,GPIO_PIN_0);

__IO uint32_t send_n = 0, receive_n = 0;
uint8_t spi1_send_array[ARRAYSIZE] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t spi3_receive_array[ARRAYSIZE];
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint8_t length);

void rcu_config(void);
void gpio_config(void);
void spi_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize led1 */
    gd_eval_led_init(LED1);

    /* configure NVIC */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE1_SUB3);
    nvic_irq_enable(SPI1_IRQn, 1, 1);
    nvic_irq_enable(SPI3_IRQn, 0, 1);

    /* enable peripheral clock */
    rcu_config();
    /* configure GPIO */
    gpio_config();
    /* configure SPI*/
    spi_config();

    SET_SPI1_NSS_HIGH

    /* enable SPI interrupt */
    spi_i2s_interrupt_enable(SPI1, SPI_I2S_INT_TBE);
    spi_i2s_interrupt_enable(SPI3, SPI_I2S_INT_RBNE);
    SET_SPI1_NSS_LOW
    /* enable SPI */
    spi_enable(SPI3);
    spi_enable(SPI1);
    /* wait transmit complete */
    while(receive_n < ARRAYSIZE);
#if SPI_CRC_ENABLE
    /* check the CRC error status  */
    if(SET != spi_i2s_flag_get(SPI3, SPI_FLAG_CRCERR)) {
        gd_eval_led_on(LED1);
    } else {
        gd_eval_led_off(LED1);
    }
    SET_SPI1_NSS_HIGH
#else
    SET_SPI1_NSS_HIGH
    /* compare receive data with send data */
    if(memory_compare(spi3_receive_array, spi1_send_array, ARRAYSIZE)) {
        gd_eval_led_on(LED1);
    } else {
        gd_eval_led_off(LED1);
    }
#endif /* enable CRC function */
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
    gpio_af_set(GPIOI, GPIO_AF_5, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    gpio_mode_set(GPIOI, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);

    /* set SPI1_NSS as GPIO*/
    gpio_mode_set(GPIOI, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_0);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

    /* configure SPI3 GPIO */
    gpio_af_set(GPIOE, GPIO_AF_5, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);

}

/*!
    \brief      configure the SPI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct  spi_init_struct;

    /* configure SPI1 parameter */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;;
    spi_init(SPI1, &spi_init_struct);

    /* configure SPI3 parameter */
    spi_init_struct.device_mode = SPI_SLAVE;
    spi_init_struct.nss         = SPI_NSS_HARD;
    spi_init(SPI3, &spi_init_struct);
#if SPI_CRC_ENABLE
    /* configure SPI CRC function */
    spi_crc_polynomial_set(SPI1, 7);
    spi_crc_polynomial_set(SPI3, 7);
    spi_crc_on(SPI1);
    spi_crc_on(SPI3);
#endif /* enable CRC function */
}

/*!
    \brief      memory compare function
    \param[in]  src: source data pointer
    \param[in]  dst: destination data pointer
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
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
