/*!
    \file    sram_msd.c
    \brief   internal flash functions

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

#include "sram_msd.h"

extern unsigned char sram_data[];

/*!
    \brief      read data from multiple blocks of sram_data
    \param[in]  pbuf: pointer to user buffer
    \param[in]  read_addr: address to be read
    \param[in]  blk_size: size of block
    \param[in]  blk_num: number of block
    \param[out] none
    \retval     status
*/
uint32_t sram_read_multi_blocks(uint8_t *pbuf, uint32_t read_addr, uint16_t blk_size, uint32_t blk_num)
{
    uint32_t i = 0U, Offset = 0U;

    while(blk_num--) {
        for(i = 0U; i < blk_size; i++) {
            *pbuf = sram_data[read_addr + Offset + i];
            pbuf++;
        }

        Offset += blk_size;
    }

    return 0U;
}

/*!
    \brief      write data to multiple blocks of sram_data
    \param[in]  pbuf: pointer to user buffer
    \param[in]  write_addr: address to be write
    \param[in]  blk_size: size of block
    \param[in]  blk_num: number of block
    \param[out] none
    \retval     status
*/
uint32_t sram_write_multi_blocks(uint8_t *pbuf, uint32_t write_addr, uint16_t blk_size, uint32_t blk_num)
{
    uint32_t i = 0U, Offset = 0U;

    while(blk_num--) {
        for(i = 0U; i < blk_size; i++) {
            sram_data[write_addr + Offset + i] = *pbuf;
            pbuf++;
        }

        Offset += blk_size;
    }

    return 0U;
}
