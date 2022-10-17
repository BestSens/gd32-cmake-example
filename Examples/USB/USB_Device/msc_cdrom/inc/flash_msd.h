/*!
    \file    flash_msd.h
    \brief   the header file of flash_msd.c

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

#ifndef __FLASH_MSD_H
#define __FLASH_MSD_H

#include "usbd_conf.h"

#define FLASH_BASE_ADDR          ((uint32_t)(0x08000000 + 0x04000))
#define FLASH_END_ADDR           ((uint32_t)(0x08000000 + 0x40000))

#define BLOCK_SIZE               (uint32_t)512U
#define BLOCK_NUM                (uint32_t)((FLASH_END_ADDR - FLASH_BASE_ADDR) / BLOCK_SIZE)

/* function declarations */
/* initialize flash */
uint32_t flash_init (void);
/* read data from multiple blocks of flash */
uint32_t flash_read_multi_blocks  (uint8_t* buf,
                                   uint32_t read_addr,
                                   uint16_t blk_size,
                                   uint32_t blk_num);
/* write data to multiple blocks of flash */
uint32_t flash_write_multi_blocks (uint8_t* buf,
                                   uint32_t write_addr,
                                   uint16_t blk_size,
                                   uint32_t blk_num);

#endif /* __FLASH_MSD_H */
