/*!
    \file    main.c
    \brief   USART synchronous

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

#define txbuffer_size1   (countof(txbuffer1) - 1)
#define txbuffer_size2   (countof(txbuffer2) - 1)
#define DYMMY_BYTE       0x00
#define countof(a)       (sizeof(a) / sizeof(*(a)))

uint8_t txbuffer1[] = "USART synchronous example: USART0 -> SPI0 using TXE and RXNE Flags";
uint8_t txbuffer2[] = "USART synchronous example: SPI0 -> USART0 using TXE and RXNE Flags";
uint8_t rxbuffer1[txbuffer_size2];
uint8_t rxbuffer2[txbuffer_size1];
__IO uint8_t data_read1 = txbuffer_size2;
__IO uint8_t data_read2 = txbuffer_size1;
__IO uint8_t tx_counter1 = 0, rx_counter1 = 0;
__IO uint8_t tx_counter2 = 0, rx_counter2 = 0;
ErrStatus state1 = ERROR;
ErrStatus state2 = ERROR;

void usart_config(void);
void spi_config(void);
void led_init(void);
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* initialize leds */
    led_init();
    /* turn off LED1~4 */
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);

    /* configure USART */
    usart_config();

    /* configure SPI */
    spi_config();

    while(data_read2--) {
        while(RESET == usart_flag_get(USART0, USART_FLAG_TBE)) {
        }
        /* write one byte in the USART0 data register */
        usart_data_transmit(USART0, txbuffer1[tx_counter1++]);
        /* wait until end of transmit */
        while(RESET == usart_flag_get(USART0, USART_FLAG_TC)) {
        }
        /* wait the byte is entirely received by SPI0 */
        while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_RBNE)) {
        }
        /* store the received byte in the rxbuffer2 */
        rxbuffer2[rx_counter2++] = spi_i2s_data_receive(SPI0);
    }

    /* clear the USART0 data register */
    usart_data_receive(USART0);

    while(data_read1--) {
        /* wait until end of transmit */
        while(RESET == spi_i2s_flag_get(SPI0, SPI_FLAG_TBE)) {
        }
        /* write one byte in the SPI0 transmit data register */
        spi_i2s_data_transmit(SPI0, txbuffer2[tx_counter2++]);

        /* send a dummy byte to generate clock to slave */
        usart_data_transmit(USART0, DYMMY_BYTE);
        /* wait until end of transmit */
        while(RESET == usart_flag_get(USART0, USART_FLAG_TC)) {
        }
        /* wait the byte is entirely received by USART0 */
        while(RESET == usart_flag_get(USART0, USART_FLAG_RBNE)) {
        }
        /* store the received byte in the rxbuffer1 */
        rxbuffer1[rx_counter1++] = usart_data_receive(USART0);
    }

    /* check the received data with the send ones */
    state1 = memory_compare(txbuffer1, rxbuffer2, txbuffer_size1);
    state2 = memory_compare(txbuffer2, rxbuffer1, txbuffer_size2);

    if(SUCCESS == state1) {
        /* if the data transmitted from USART0 and received by SPI0 are the same */
        gd_eval_led_on(LED1);
    } else {
        /* if the data transmitted from USART0 and received by SPI0 are not the same */
        gd_eval_led_off(LED1);
    }
    if(SUCCESS == state2) {
        /* if the data transmitted from SPI0 and received by USART0 are the same */
        gd_eval_led_on(LED2);
    } else {
        /* if the data transmitted from SPI0 and received by USART0 are not the same */
        gd_eval_led_off(LED2);
    }
    while(1) {
    }
}
/*!
    \brief      configure USART
    \param[in]  none
    \param[out] none
    \retval     none
*/
void usart_config(void)
{
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    /* configure USART TX as alternate function push-pull */
    gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);

    /* configure USART synchronous mode */
    usart_synchronous_clock_enable(USART0);
    usart_synchronous_clock_config(USART0, USART_CLEN_EN, USART_CPH_2CK, USART_CPL_HIGH);

    usart_baudrate_set(USART0, 115200);
    /* configure USART transmitter */
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    /* configure USART receiver */
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    /* enable USART */
    usart_enable(USART0);
}

/*!
    \brief      configure SPI
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_config(void)
{
    spi_parameter_struct spi_init_parameter;
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_SPI0);

    spi_i2s_deinit(SPI0);

    gpio_af_set(GPIOA, GPIO_AF_5, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7);

    /* configure SPI0 */
    spi_init_parameter.device_mode = SPI_SLAVE;
    spi_init_parameter.trans_mode = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_parameter.frame_size = SPI_FRAMESIZE_8BIT;
    spi_init_parameter.nss = SPI_NSS_SOFT;
    spi_init_parameter.endian = SPI_ENDIAN_LSB;
    spi_init_parameter.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_parameter.prescale = SPI_PSC_32;
    spi_init(SPI0, &spi_init_parameter);

    /* SPI0 enable */
    spi_enable(SPI0);
}

/*!
    \brief      initialize leds
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_init(void)
{
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
}

/*!
    \brief      memory compare function
    \param[in]  src: source data
    \param[in]  dst: destination data
    \param[in]  length: the compare data length
    \param[out] none
    \retval     ErrStatus: ERROR or SUCCESS
*/
ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}
