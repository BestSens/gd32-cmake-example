/*!
    \file    main.h
    \brief   the header file of main 
    
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

#ifndef __MAIN_H
#define __MAIN_H

#ifndef __PRINTF_H__
#define __PRINTF_H__

#include <stdio.h>
#include "gd32f4xx.h"

#ifndef __FLASH_H__
#define __FLASH_H__

/* base address of the FMC sectors */
#define ADDR_FMC_SECTOR_0     ((uint32_t)0x08000000) /*!< base address of sector 0, 16 kbytes */
#define ADDR_FMC_SECTOR_1     ((uint32_t)0x08004000) /*!< base address of sector 1, 16 kbytes */
#define ADDR_FMC_SECTOR_2     ((uint32_t)0x08008000) /*!< base address of sector 2, 16 kbytes */
#define ADDR_FMC_SECTOR_3     ((uint32_t)0x0800C000) /*!< base address of sector 3, 16 kbytes */
#define ADDR_FMC_SECTOR_4     ((uint32_t)0x08010000) /*!< base address of sector 4, 64 kbytes */
#define ADDR_FMC_SECTOR_5     ((uint32_t)0x08020000) /*!< base address of sector 5, 64 kbytes */
#define ADDR_FMC_SECTOR_6     ((uint32_t)0x08040000) /*!< base address of sector 6, 64 kbytes */
#define ADDR_FMC_SECTOR_7     ((uint32_t)0x08060000) /*!< base address of sector 7, 64 kbytes */
#define ADDR_FMC_SECTOR_8     ((uint32_t)0x08080000) /*!< base address of sector 8, 64 kbytes */
#define ADDR_FMC_SECTOR_9     ((uint32_t)0x080A0000) /*!< base address of sector 9, 64 kbytes */
#define ADDR_FMC_SECTOR_10    ((uint32_t)0x080C0000) /*!< base address of sector 10, 64 kbytes */
#define ADDR_FMC_SECTOR_11    ((uint32_t)0x080E0000) /*!< base address of sector 11, 64 kbytes */

#define FLASH_USER_START_ADDR ADDR_FMC_SECTOR_1

/* gets the sector of a given address */
uint32_t fmc_sector_get(uint32_t address);
/* erases the sector of a given sector number */
void fmc_erase_sector(uint32_t fmc_sector);
/* write 32 bit length data to a given address */
void fmc_write_32bit_data(uint32_t address, uint16_t length, int32_t* data_32);
/* read 32 bit length data from a given address */
void fmc_read_32bit_data(uint32_t address, uint16_t length, int32_t* data_32);

#endif

#endif

#endif
