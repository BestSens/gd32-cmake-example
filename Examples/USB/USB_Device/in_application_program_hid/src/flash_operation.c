/*!
    \file    flash_operation.c
    \brief   flash operation driver

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

#include "flash_operation.h"
#include "usbd_conf.h"

/*!
    \brief      erase flash
    \param[in]  address: erase start address
    \param[in]  file_length: file length
    \param[in]  report_buffer: report buffer
    \param[out] none
    \retval     MAL_OK if all operations are OK, MAL_FAIL else
*/
void flash_erase (uint32_t address, uint32_t file_length, uint8_t* report_buffer)
{
    uint16_t start_sector = 0U, end_sector = 0U, i = 0U;

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    start_sector = fmc_sector_get(address);
    end_sector = fmc_sector_get(address + file_length);

    for (i = start_sector; i <= end_sector; i += 8U) {
        if (FMC_READY != fmc_sector_erase(i)) {
            while(1){
            }
        }
    }
}

/*!
    \brief      gets the sector of a given address
    \param[in]  address: a given address(0x08000000~0x080FFFFF)
    \param[out] none
    \retval     the sector of a given address
*/
uint32_t fmc_sector_get(uint32_t addr)
{
    uint32_t sector = 0U;

    if ((addr < ADDR_FMC_SECTOR_1) && (addr >= ADDR_FMC_SECTOR_0)) {
        sector = CTL_SECTOR_NUMBER_0;
    } else if ((addr < ADDR_FMC_SECTOR_2) && (addr >= ADDR_FMC_SECTOR_1)) {
        sector = CTL_SECTOR_NUMBER_1;
    } else if ((addr < ADDR_FMC_SECTOR_3) && (addr >= ADDR_FMC_SECTOR_2)) {
        sector = CTL_SECTOR_NUMBER_2;
    } else if ((addr < ADDR_FMC_SECTOR_4) && (addr >= ADDR_FMC_SECTOR_3)) {
        sector = CTL_SECTOR_NUMBER_3;  
    } else if ((addr < ADDR_FMC_SECTOR_5) && (addr >= ADDR_FMC_SECTOR_4)) {
        sector = CTL_SECTOR_NUMBER_4;
    } else if ((addr < ADDR_FMC_SECTOR_6) && (addr >= ADDR_FMC_SECTOR_5)) {
        sector = CTL_SECTOR_NUMBER_5;
    } else if ((addr < ADDR_FMC_SECTOR_7) && (addr >= ADDR_FMC_SECTOR_6)) {
        sector = CTL_SECTOR_NUMBER_6;
    } else if ((addr < ADDR_FMC_SECTOR_8) && (addr >= ADDR_FMC_SECTOR_7)) {
        sector = CTL_SECTOR_NUMBER_7;  
    } else if ((addr < ADDR_FMC_SECTOR_9) && (addr >= ADDR_FMC_SECTOR_8)) {
        sector = CTL_SECTOR_NUMBER_8;
    } else if ((addr < ADDR_FMC_SECTOR_10) && (addr >= ADDR_FMC_SECTOR_9)) {
        sector = CTL_SECTOR_NUMBER_9;
    } else if ((addr < ADDR_FMC_SECTOR_11) && (addr >= ADDR_FMC_SECTOR_10)) {
        sector = CTL_SECTOR_NUMBER_10;
    } else if ((addr < ADDR_FMC_SECTOR_12) && (addr >= ADDR_FMC_SECTOR_11)) {
        sector = CTL_SECTOR_NUMBER_11;
    } else if ((addr < ADDR_FMC_SECTOR_13) && (addr >= ADDR_FMC_SECTOR_12)) {
        sector = CTL_SECTOR_NUMBER_12;
    } else if ((addr < ADDR_FMC_SECTOR_14) && (addr >= ADDR_FMC_SECTOR_13)) {
        sector = CTL_SECTOR_NUMBER_13;
    } else if ((addr < ADDR_FMC_SECTOR_15) && (addr >= ADDR_FMC_SECTOR_14)) {
        sector = CTL_SECTOR_NUMBER_14;
    } else if ((addr < ADDR_FMC_SECTOR_16) && (addr >= ADDR_FMC_SECTOR_15)) {
        sector = CTL_SECTOR_NUMBER_15;
    } else if ((addr < ADDR_FMC_SECTOR_17) && (addr >= ADDR_FMC_SECTOR_16)) {
        sector = CTL_SECTOR_NUMBER_16;
    } else if ((addr < ADDR_FMC_SECTOR_18) && (addr >= ADDR_FMC_SECTOR_17)) {
        sector = CTL_SECTOR_NUMBER_17;
    } else if ((addr < ADDR_FMC_SECTOR_19) && (addr >= ADDR_FMC_SECTOR_18)) {
        sector = CTL_SECTOR_NUMBER_18;
    } else if ((addr < ADDR_FMC_SECTOR_20) && (addr >= ADDR_FMC_SECTOR_19)) {
        sector = CTL_SECTOR_NUMBER_19;
    } else if ((addr < ADDR_FMC_SECTOR_21) && (addr >= ADDR_FMC_SECTOR_20)) {
        sector = CTL_SECTOR_NUMBER_20;
    } else if ((addr < ADDR_FMC_SECTOR_22) && (addr >= ADDR_FMC_SECTOR_21)) {
        sector = CTL_SECTOR_NUMBER_21;
    } else if ((addr < ADDR_FMC_SECTOR_23) && (addr >= ADDR_FMC_SECTOR_22)) {
        sector = CTL_SECTOR_NUMBER_22;
    } else if ((addr < ADDR_FMC_SECTOR_24) && (addr >= ADDR_FMC_SECTOR_23)) {
        sector = CTL_SECTOR_NUMBER_23;
    } else if ((addr < ADDR_FMC_SECTOR_25) && (addr >= ADDR_FMC_SECTOR_24)) {
        sector = CTL_SECTOR_NUMBER_24;
    } else if ((addr < ADDR_FMC_SECTOR_26) && (addr >= ADDR_FMC_SECTOR_25)) {
        sector = CTL_SECTOR_NUMBER_25;
    } else if ((addr < ADDR_FMC_SECTOR_27) && (addr >= ADDR_FMC_SECTOR_26)) {
        sector = CTL_SECTOR_NUMBER_26;
    } else {
    }

    return sector;
}

/*!
    \brief      write data to sectors of memory
    \param[in]  data: data to be written
    \param[in]  addr: sector address/code
    \param[in]  len: length of data to be written (in bytes)
    \param[out] none
    \retval     MAL_OK if all operations are OK, MAL_FAIL else
*/
void iap_data_write (uint8_t *data, uint32_t addr, uint32_t len)
{
    uint32_t idx = 0U;

    /* check if the address is in protected area */
    if (IS_PROTECTED_AREA(addr)) {
        return;
    }

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    /* data received are word multiple */
    for (idx = 0U; idx < len; idx += 2) {
        if (FMC_READY == fmc_halfword_program(addr, *(uint32_t *)(data + idx))) {
            addr += 2U;
        } else {
            while(1) {
            }
        }
    }

    fmc_lock();
}
