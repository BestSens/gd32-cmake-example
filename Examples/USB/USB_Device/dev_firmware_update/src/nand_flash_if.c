/*!
    \file    nand_flash_if.c
    \brief   USB DFU device nand flash interface functions

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
    3. Neither the name of the copyright holder nand the names of its contributors 
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

#include "nand_flash_if.h"
#include "exmc_nandflash.h"

/* local function prototypes ('static') */
static uint8_t nand_flash_if_init (void);
static uint8_t nand_flash_if_deinit (void);
static uint8_t nand_flash_if_erase (uint32_t addr);
static uint8_t nand_flash_if_write (uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t* nand_flash_if_read (uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t nand_flash_if_checkaddr (uint32_t addr);

dfu_mem_prop dfu_nand_flash_cb =
{
    (const uint8_t *)NAND_FLASH_IF_STR,

    nand_flash_if_init,
    nand_flash_if_deinit,
    nand_flash_if_erase,
    nand_flash_if_write,
    nand_flash_if_read,
    nand_flash_if_checkaddr,
    20, /* flash erase timeout in ms */
    40  /* flash programming timeout in ms */
};

/*!
    \brief      flash memory interface initialization routine
    \param[in]  none
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nand_flash_if_init (void)
{
    /* configure the EXMC access mode */
    exmc_nandflash_init(EXMC_BANK1_NAND);

    return MEM_OK;
}

/*!
    \brief      flash memory interface deinitialization routine
    \param[in]  none
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nand_flash_if_deinit (void)
{
    return MEM_OK;
}

/*!
    \brief      erase flash sector
    \param[in]  addr: flash address to be erased
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nand_flash_if_erase (uint32_t addr)
{
    return MEM_OK;
}

/*!
    \brief      flash memory write routine
    \param[in]  buf: data buffer pointer
    \param[in]  addr: flash address to be written
    \param[in]  len: length of data to be written (in bytes)
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nand_flash_if_write (uint8_t *buf, uint32_t addr, uint32_t len)
{
    /* calculate actual address */
    uint32_t addr_actual = addr - NAND_FLASH_START_ADDR;

    /* write block of data to the flash */
    if(NAND_OK == nand_write(addr_actual, buf, len)){
        return MEM_OK;
    }else{
        return MEM_FAIL;
    }
}

/*!
    \brief      flash memory read routine
    \param[in]  buf: data buffer pointer
    \param[in]  addr: flash address to be read from
    \param[in]  len: length of data to be read (in bytes)
    \param[out] none
    \retval     pointer to the physical address where data should be read
*/
static uint8_t *nand_flash_if_read (uint8_t *buf, uint32_t addr, uint32_t len)
{
    /* calculate actual address */
    uint32_t addr_actual = addr - NAND_FLASH_START_ADDR;

    nand_read(addr_actual, buf, len);

    return buf;
}

/*!
    \brief      check if the address is an allowed address for this memory
    \param[in]  addr: flash address to be checked
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nand_flash_if_checkaddr (uint32_t addr)
{
    if ((addr >= NAND_FLASH_START_ADDR) && (addr < NAND_FLASH_END_ADDR)) {
        return MEM_OK;
    } else {
        return MEM_FAIL;
    }
}
