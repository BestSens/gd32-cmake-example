/*!
    \file    gd32f450i_eval_exmc_sdram.h
    \brief   the header file of the read and write sdram.

    \version 2016-08-15, V1.0.0, firmware for GD32F4xx
    \version 2018-12-12, V2.0.0, firmware for GD32F4xx
    \version 2020-09-30, V2.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

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

#ifndef __GD32F450I_EVAL_EXMC_SDRAM_H
#define __GD32F450I_EVAL_EXMC_SDRAM_H

#include "gd32f4xx.h"

#define SDRAM_DEVICE0_ADDR                         ((uint32_t)0xC0000000)
#define SDRAM_DEVICE1_ADDR                         ((uint32_t)0xD0000000)

/* function declarations */
/* sdram peripheral initialize */
void exmc_synchronous_dynamic_ram_init(uint32_t sdram_device);
/* fill the buffer with specified value */
void fill_buffer(uint8_t *pbuffer, uint16_t buffer_lengh, uint16_t offset);
/* write a byte buffer(data is 8 bits) to the EXMC SDRAM memory */
void sdram_writebuffer_8(uint32_t sdram_device, uint8_t* pbuffer, uint32_t writeaddr, uint32_t numbytetowrite);
/* read a block of 8-bit data from the EXMC SDRAM memory */
void sdram_readbuffer_8(uint32_t sdram_device, uint8_t* pbuffer, uint32_t readaddr, uint32_t numbytetoread);

#endif /* __GD32F450I_EVAL_EXMC_SDRAM_H */
