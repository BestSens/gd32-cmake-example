/*!
    \file    sram_msd.c
    \brief   internal flash functions

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
    \version 2022-03-09, V3.1.0, firmware for GD32F4xx
    \version 2022-06-30, V3.2.0, firmware for GD32F4xx
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

#include "usb_conf.h"
#include "sram_msd.h"

extern unsigned char SRAM[];

/*!
    \brief      read data from multiple blocks of SRAM
    \param[in]  pBuf: pointer to user buffer
    \param[in]  ReadAddr: address to be read
    \param[in]  BlkSize: size of block
    \param[in]  BlkNum: number of block
    \param[out] none
    \retval     status
*/
uint32_t SRAM_ReadMultiBlocks (uint8_t *pBuf, uint32_t ReadAddr, uint16_t BlkSize, uint32_t BlkNum)
{
    uint32_t i = 0U, Offset = 0U;

    while (BlkNum--) {
        for (i = 0U; i < BlkSize; i++) {
            *pBuf = SRAM[ReadAddr + Offset + i];
            pBuf++;
        }

        Offset += BlkSize;
    }

    return 0U;
}

/*!
    \brief      write data to multiple blocks of SRAM
    \param[in]  pBuf: pointer to user buffer
    \param[in]  WriteAddr: address to be write
    \param[in]  BlkSize: size of block
    \param[in]  BlkNum: number of block
    \param[out] none
    \retval     status
*/
uint32_t SRAM_WriteMultiBlocks(uint8_t *pBuf, uint32_t WriteAddr, uint16_t BlkSize, uint32_t BlkNum)
{
    uint32_t i = 0U, Offset = 0U;

    while (BlkNum--) {
        for (i = 0U; i < BlkSize; i++) {
            SRAM[WriteAddr + Offset + i] = *pBuf;
            pBuf++;
        }

        Offset += BlkSize;
    }

    return 0U;
}
