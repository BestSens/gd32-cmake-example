/*!
    \file    main.c
    \brief   EXMC SRAM demo

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
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
#include "exmc_sram.h"

#define BUFFER_SIZE             100                  /*!< write or read buffer size */
#define WRITE_READ_ADDR         0x0000              /*!< SRAM write or read address */

/* SRAM */
uint16_t txbuffer[BUFFER_SIZE];
uint16_t rxbuffer[BUFFER_SIZE];
uint16_t writereadstatus = 0;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint16_t i = 0;

    /* initialize LEDs */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED3);
    /* configure the USART */
    gd_eval_com_init(EVAL_COM0);
    /* configure the EXMC access mode */
    exmc_sram_init(EXMC_BANK0_NORSRAM_REGION2);
    /* fill txbuffer */
    fill_buffer_16(txbuffer, BUFFER_SIZE, 0x1215);
    /* write data to SRAM  */
    exmc_sram_writebuffer_16(txbuffer, WRITE_READ_ADDR, BUFFER_SIZE);
    /* read data from SRAM */
    exmc_sram_readbuffer_16(rxbuffer, WRITE_READ_ADDR, BUFFER_SIZE);
    /* compare two buffers */
    for(i = 0; i < BUFFER_SIZE; i++) {
        if(rxbuffer[i] != txbuffer[i]) {
            writereadstatus ++;
            break;
        }
    }

    if(writereadstatus) {
        printf("\r\nSRAM test failed!");

        /* failure, light on LED3 */
        gd_eval_led_on(LED3);
    } else {
        printf("\r\nSRAM test successed!");
        printf("\r\nthe data is:\r\n");
        for(i = 0; i < BUFFER_SIZE; i++) {
            printf("%6x", rxbuffer[i]);
            if(((i + 1) % 6) == 0) {
                printf("\r\n");
            }
        }
        /* success, light on LED1 */
        gd_eval_led_on(LED1);
    }

    while(1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t) ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
