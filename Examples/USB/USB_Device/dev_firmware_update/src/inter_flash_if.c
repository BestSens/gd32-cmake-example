/*!
    \file    inter_flash_if.c
    \brief   USB DFU device flash interface functions

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

#include "inter_flash_if.h"

/* local function prototypes ('static') */
static uint8_t inter_flash_if_init (void);
static uint8_t inter_flash_if_deinit (void);
static uint8_t inter_flash_if_erase (uint32_t addr);
static uint8_t inter_flash_if_write (uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t* inter_flash_if_read (uint8_t *buf, uint32_t addr, uint32_t len);
static uint8_t inter_flash_if_checkaddr (uint32_t addr);
static uint32_t fmc_sector_get (uint32_t address);
static void fmc_erase_sector (uint32_t fmc_sector);

dfu_mem_prop dfu_inter_flash_cb =
{
    (const uint8_t *)INTER_FLASH_IF_STR,

    inter_flash_if_init,
    inter_flash_if_deinit,
    inter_flash_if_erase,
    inter_flash_if_write,
    inter_flash_if_read,
    inter_flash_if_checkaddr,
    60, /* flash erase timeout in ms */
    80  /* flash programming timeout in ms (80us * RAM Buffer size (1024 Bytes) */
};

/*!
    \brief      program option byte
    \param[in]  Mem_Add: target address
    \param[in]  data: pointer to target data
    \param[out] none
    \retval     state of FMC, refer to fmc_state_enum
*/
fmc_state_enum option_byte_write(uint32_t Mem_Add,uint8_t* data)
{
    fmc_state_enum status ;

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    status = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    /* authorize the small information block programming */
    ob_unlock();
    
    /* start erase the option byte */
    ob_erase();

    {
        uint32_t nRST_STDBY = 0U;
        uint32_t nRST_DPSLP = 0U;
        uint32_t nWDG_HW = 0U;
        uint32_t BB = 0U;
        uint32_t BOR_TH = 0U;
        uint16_t WP0 = 0U;
        uint16_t WP1 = 0U;
        uint32_t reg0 = FMC_OBCTL0;
        uint32_t reg1 = FMC_OBCTL1;
        uint32_t reg = 0U;
        fmc_state_enum fmc_state = FMC_READY;

        ob_start();

        if (1U == ((data[0]>>7U) & 0x01U)) {
            nRST_STDBY = OB_STDBY_NRST;
        } else {
            nRST_STDBY = OB_STDBY_RST;
        }
        
        if (1U == ((data[0]>>6U) & 0x01U)) {
            nRST_DPSLP = OB_DEEPSLEEP_NRST;
        } else {
            nRST_DPSLP = OB_DEEPSLEEP_RST;
        }
        
        if (1U == ((data[0]>>5U) & 0x01U)) {
            nWDG_HW = OB_FWDGT_SW;
        } else {
            nWDG_HW = OB_FWDGT_HW;
        }
        
        ob_user_write(nWDG_HW,nRST_DPSLP,nRST_STDBY);
        
        ob_security_protection_config(data[1]);
        
        reg0 = FMC_OBCTL0;
        reg1 = FMC_OBCTL1;
        if (0U == ((data[0]>>2U) & 0x03U)) {
            BOR_TH = OB_BOR_TH_VALUE3;
        } else if (1U == ((data[0]>>2U) & 0x03U)) {
            BOR_TH = OB_BOR_TH_VALUE2;
        } else if (2U == ((data[0]>>2U) & 0x03U)) {
            BOR_TH = OB_BOR_TH_VALUE1;
        } else {
            BOR_TH = OB_BOR_TH_OFF;
        }
        ob_user_bor_threshold(BOR_TH);
        
        if (1U == ((data[0]>>4U) & 0x01U)) {
            BB = OB_BB_ENABLE;
        } else {
            BB = OB_BB_DISABLE;
        }
        ob_boot_mode_config(BB);
        
        WP0 = (data[8] | (data[9] << 8U)) & 0x0FFFU;
        WP1 = (data[24] | (data[25] << 8U)) & 0x0FFFU;
        
        if(RESET != (FMC_OBCTL0 & FMC_OBCTL0_DRP)){
            while(1){
            }
        }
        /* wait for the FMC ready */
        fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
        
        reg0 &= (~((uint32_t)0x0FFFU << 16U));
        reg1 &= (~((uint32_t)0x0FFFU << 16U));
        
        if(FMC_READY == fmc_state){
            reg = (WP0 << 16U);
            FMC_OBCTL0 = reg0 | reg;
            reg = (WP1 << 16U);
            FMC_OBCTL1 = reg1 | reg;
        }

        ob_start();
    }

    fmc_lock();

    return status;
}

/*!
    \brief      flash memory interface initialization routine
    \param[in]  none
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t inter_flash_if_init (void)
{
    /* unlock the internal flash */
    fmc_unlock();

    return MEM_OK;
}

/*!
    \brief      flash memory interface deinitialization routine
    \param[in]  none
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t inter_flash_if_deinit (void)
{
    /* lock the internal flash */
    fmc_lock();

    return MEM_OK;
}

/*!
    \brief      erase flash sector
    \param[in]  addr: flash address to be erased
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t inter_flash_if_erase (uint32_t addr)
{
    uint32_t start_sector = 0U;

    /* get the number of sector */
    start_sector = fmc_sector_get(addr);

    /* erase the sector */
    fmc_erase_sector(start_sector);

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
static uint8_t inter_flash_if_write (uint8_t *buf, uint32_t addr, uint32_t len)
{
    uint32_t idx = 0U;

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    /* not an aligned data */
    if (len & 0x03U) {
        for (idx = len; idx < ((len & 0xFFFCU) + 4U); idx++) {
            buf[idx] = 0xFFU;
        }
    }

    /* data received are word multiple */
    for (idx = 0U; idx < len; idx += 4U) {
        fmc_word_program(addr, *(uint32_t *)(buf + idx));
        addr += 4U;
    }

    fmc_lock();

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
static uint8_t *inter_flash_if_read (uint8_t *buf, uint32_t addr, uint32_t len)
{
    return  (uint8_t *)(addr);
}

/*!
    \brief      check if the address is an allowed address for this memory
    \param[in]  addr: flash address to be checked
    \param[out] none
    \retval     MEM_OK if the operation is right, MEM_FAIL else
*/
static uint8_t inter_flash_if_checkaddr (uint32_t addr)
{
    if ((addr >= FLASH_START_ADDR) && (addr < FLASH_END_ADDR)) {
        return MEM_OK;
    } else {
        return MEM_FAIL;
    }
}

/*!
    \brief      erases the sector of a given sector number
    \param[in]  fmc_sector: a given sector number
      \arg        CTL_SECTOR_NUMBER_x (x = 0,..,27)
    \param[out] none
    \retval     none
*/
static void fmc_erase_sector(uint32_t fmc_sector)
{
    fmc_state_enum state;

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    state = fmc_sector_erase(fmc_sector);

    /* wait the erase operation complete*/
    if(FMC_READY != state){
       while(1){
       }
    }

    /* lock the flash program erase controller */
    fmc_lock();
}

/*!
    \brief      gets the sector of a given address
    \param[in]  addr: a given address(0x08000000~0x08300000)
    \param[out] none
    \retval     the sector of a given address
*/
static uint32_t fmc_sector_get(uint32_t addr)
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
        sector = CTL_SECTOR_NUMBER_27;
    }

    return sector;
}

