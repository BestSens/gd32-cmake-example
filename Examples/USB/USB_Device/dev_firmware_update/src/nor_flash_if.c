/*!
    \file    nor_flash_if.c
    \brief   USB DFU device nor flash interface functions

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

#include "gd25qxx.h"
#include "nor_flash_if.h"

/* local function prototypes ('static') */
static uint8_t nor_flash_if_init (void);
static uint8_t nor_flash_if_deinit (void);
static uint8_t nor_flash_if_erase (uint32_t addr);
static uint8_t nor_flash_if_write (uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t* nor_flash_if_read (uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t nor_flash_if_checkaddr (uint32_t addr);

dfu_mem_prop dfu_nor_flash_cb =
{
    (const uint8_t *)NOR_FLASH_IF_STR,

    nor_flash_if_init,
    nor_flash_if_deinit,
    nor_flash_if_erase,
    nor_flash_if_write,
    nor_flash_if_read,
    nor_flash_if_checkaddr,
    400, /* flash erase timeout in ms */
    20  /* flash programming timeout in ms */
};

/*!
    \brief      flash memory interface initialization routine
    \param[in]  none
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nor_flash_if_init (void)
{
    /* initialize spi interface*/
    spi_flash_init();

    return MEM_OK;
}

/*!
    \brief      flash memory interface deinitialization routine
    \param[in]  none
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nor_flash_if_deinit (void)
{
    return MEM_OK;
}

/*!
    \brief      erase flash sector
    \param[in]  addr: flash address to be erased
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nor_flash_if_erase (uint32_t addr)
{
    /* erase the specified flash sector */
    spi_flash_block_erase(addr);

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
static uint8_t nor_flash_if_write (uint8_t *buf, uint32_t addr, uint32_t len)
{
    /* write block of data to the flash */
    spi_flash_buffer_write(buf, addr, len);

    return MEM_OK;
}

/*!
    \brief      flash memory read routine
    \param[in]  buf: data buffer pointer
    \param[in]  addr: flash address to be read from
    \param[in]  len: length of data to be read (in bytes)
    \param[out] none
    \retval     pointer to the physical address where data should be read
*/
static uint8_t *nor_flash_if_read (uint8_t *buf, uint32_t addr, uint32_t len)
{
    spi_flash_buffer_read(buf, addr, len);

    return buf;
}

/*!
    \brief      check if the address is an allowed address for this memory
    \param[in]  addr: flash address to be checked
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t nor_flash_if_checkaddr (uint32_t addr)
{
    if (((addr > NOR_FLASH_START_ADDR) && (addr < NOR_FLASH_END_ADDR)) || (NOR_FLASH_START_ADDR == addr)) {
        return MEM_OK;
    } else {
        return MEM_FAIL;
    }
}
