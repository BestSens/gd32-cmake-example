/*!
    \file    exmc_sram.h
    \brief   the header file of EXMC SRAM(ISSI IS61LV51216) driver

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

#ifndef EXMC_SRAM_H
#define EXMC_SRAM_H

#include "gd32f4xx.h"

void exmc_sram_init(uint32_t sram_region);
void exmc_sram_writebuffer_16(uint16_t *pbuffer, uint32_t writeaddr, uint32_t num_halfword_write);
void exmc_sram_readbuffer_16(uint16_t *pbuffer, uint32_t readaddr, uint32_t num_halfword_read);

void exmc_sram_writebuffer_8(uint8_t *pbuffer, uint32_t writeaddr, uint32_t num_byte_write);
void exmc_sram_readbuffer_8(uint8_t *pbuffer, uint32_t readaddr, uint32_t num_byte_read);

void exmc_sram_writebuffer_32(uint32_t *pbuffer, uint32_t writeaddr, uint32_t num_word_write);
void exmc_sram_readbuffer_32(uint32_t *pbuffer, uint32_t readaddr, uint32_t num_word_read);

void fill_buffer_16(uint16_t *pbuffer, uint16_t buffer_lenght, uint16_t offset);

#endif /* EXMC_SRAM_H */
