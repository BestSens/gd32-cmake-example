/*!
    \file    main.c
    \brief   main flash program, erase
    
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
#include "fmc_operation.h"
#include "gd32f450i_eval.h"

#define ERASE_ADDRESS               ((uint32_t)0x08004000)
#define WRITE_ADDRESS_WORD          ((uint32_t)0x08008000)
#define WRITE_ADDRESS_HALF_WORD     ((uint32_t)0x0800C000)
#define WRITE_ADDRESS_BYTE          ((uint32_t)0x08010000)

int32_t data_32_1[10] = {0x5555AAAA,0x5555AAAA,0x5555AAAA,0x5555AAAA,0x5555AAAA,0x5555AAAA,0x5555AAAA,0x5555AAAA,0x5555AAAA,0x5555AAAA};
int32_t data_32_2[10];
int16_t data_16_1[10] = {0x44BB,0x44BB,0x44BB,0x44BB,0x44BB,0x44BB,0x44BB,0x44BB,0x44BB,0x44BB,};
int16_t data_16_2[10];
int8_t data_8_1[10] = {0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,0x3D,0x3D};
int8_t data_8_2[10];

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* enable USART0 clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    /* configure COM port */
    gd_eval_com_init(EVAL_COM0);
    
    /* erases the sector of a given sector number */
    fmc_erase_sector_by_address(ERASE_ADDRESS);

    /* write 32 bit length data to a given address */
    fmc_write_32bit_data(WRITE_ADDRESS_WORD, 10, data_32_1);
    /* read 32 bit length data from a given address */
    fmc_read_32bit_data(WRITE_ADDRESS_WORD, 10, data_32_2);

    /* write 16 bit length data to a given address */
    fmc_write_16bit_data(WRITE_ADDRESS_HALF_WORD, 10, data_16_1);
    /* read 16 bit length data from a given address */
    fmc_read_16bit_data(WRITE_ADDRESS_HALF_WORD, 10, data_16_2);
    
    /* write 8 bit length data to a given address */
    fmc_write_8bit_data(WRITE_ADDRESS_BYTE, 10, data_8_1);
    /* read 8 bit length data from a given address */
    fmc_read_8bit_data(WRITE_ADDRESS_BYTE, 10, data_8_2);

    while(1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
