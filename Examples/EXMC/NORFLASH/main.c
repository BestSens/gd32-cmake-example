/*!
    \file    main.c
    \brief   EXMC NOR Flash demo

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
#include "exmc_norflash.h"

#define BUFFER_SIZE               0x100
#define WRITE_READ_ADDR           0xfff200

nor_id_struct nor_id;
uint32_t writereadstatus = 0;
uint16_t writebuffer[BUFFER_SIZE];
uint16_t readbuffer[BUFFER_SIZE];
nor_status_struct status;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint32_t index;

    /* initialize LEDs */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED3);
    /* configure the USART */
    gd_eval_com_init(EVAL_COM0);
    /* configure the EXMC access mode */
    exmc_norflash_init(EXMC_BANK0_NORSRAM_REGION1);
    /* read nor flash id and printf */
    nor_read_id(&nor_id);

    printf("\r\nread NOR ID");
    /* print NOR ID */
    printf("\r\nNOR flash ID:0x%X 0x%X 0x%X 0x%X\r\n", nor_id.manufacturer_code, nor_id.device_code1,
           nor_id.device_code2, nor_id.device_code3);
    nor_return_to_read_mode();

    /* erase the nor flash block to be written data */
    status = nor_eraseblock(WRITE_READ_ADDR);
    if(NOR_SUCCESS == status) {
        printf("\r\nerase nor flash block successfully!");
    } else {
        printf("\r\nerase nor flash block failure!");

        /* failure, light on LED3 */
        gd_eval_led_on(LED3);
        while(1);
    }

    /* whether address cross-border */
    if((WRITE_READ_ADDR + BUFFER_SIZE) > NOR_MAX_ADDRESS) {
        printf("\n\raddress cross-border\n\r");

        /* failure, light on LED3 */
        gd_eval_led_on(LED3);
        while(1);
    }

    /* fill writeBuffer with the specified value */
    nor_fill_buffer(writebuffer, BUFFER_SIZE, 0x0006);

    /* write data to nor flash, WRITE_READ_ADDR: the starting address of the write data */
    status = nor_write_buffer(writebuffer, WRITE_READ_ADDR, BUFFER_SIZE);
    if(NOR_SUCCESS == status) {
        printf("\r\nwrite data to nor flash block successfully!");
    } else {
        printf("\r\nwrite data to nor flash block failure!");

        /* failure, light on LED3 */
        gd_eval_led_on(LED3);
        while(1);
    }

    /* read data from nor flash, WRITE_READ_ADDR: the starting address of the read data*/
    nor_readbuffer(readbuffer, WRITE_READ_ADDR, BUFFER_SIZE);

    /* read and write data comparison for equality */
    writereadstatus = 0;
    for(index = 0x00; index < BUFFER_SIZE; index++) {
        if(readbuffer[index] != writebuffer[index]) {
            writereadstatus++;
            break;
        }
    }

    printf("\n\rthe result to access the nor flash:\n\r");
    if(writereadstatus == 0) {
        printf("\n\raccess nor flash successfully!\n\r");
        gd_eval_led_on(LED1);
    } else {
        printf("\n\raccess nor flash failure!\n\r");

        /* failure, light on LED3 */
        gd_eval_led_on(LED3);
        while(1);
    }

    printf("\n\rprintf data to be read: \n\r");
    printf("\n\r");
    for(index = 0; index < BUFFER_SIZE; index++) {
        printf("%d ", readbuffer[index]);
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
