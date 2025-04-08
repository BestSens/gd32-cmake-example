/*!
    \file    flash_operation.c
    \brief   flash operation driver

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

#include "flash_operation.h"

/*!
    \brief      erase flash
    \param[in]  address: erase start address
    \param[in]  file_length: file length
    \param[out] none
    \retval     state of FMC, refer to fmc_state_enum
*/
fmc_state_enum flash_erase(uint32_t address, uint32_t file_length)
{
    fmc_state_enum fmc_state = FMC_READY;
    uint16_t start_sector = 0U, end_sector = 0U, i = 0U;

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    start_sector = fmc_sector_get(address);
    end_sector = fmc_sector_get(address + file_length);

    for(i = start_sector; i <= end_sector; i += 8U) {
        if(FMC_READY != fmc_sector_erase(i)) {
            while(1) {
            }
        }
    }

    /* lock the flash program erase controller */
    fmc_lock();

    return fmc_state;
}

/*!
    \brief      gets the sector of a given address
    \param[in]  addr: a given address(0x08000000~0x08300000)
    \param[out] none
    \retval     the sector of a given address
*/
uint32_t fmc_sector_get(uint32_t addr)
{
    uint32_t sector = 0U;

    if((addr < ADDR_FMC_SECTOR_1) && (addr >= ADDR_FMC_SECTOR_0)) {
        sector = CTL_SECTOR_NUMBER_0;
    } else if((addr < ADDR_FMC_SECTOR_2) && (addr >= ADDR_FMC_SECTOR_1)) {
        sector = CTL_SECTOR_NUMBER_1;
    } else if((addr < ADDR_FMC_SECTOR_3) && (addr >= ADDR_FMC_SECTOR_2)) {
        sector = CTL_SECTOR_NUMBER_2;
    } else if((addr < ADDR_FMC_SECTOR_4) && (addr >= ADDR_FMC_SECTOR_3)) {
        sector = CTL_SECTOR_NUMBER_3;
    } else if((addr < ADDR_FMC_SECTOR_5) && (addr >= ADDR_FMC_SECTOR_4)) {
        sector = CTL_SECTOR_NUMBER_4;
    } else if((addr < ADDR_FMC_SECTOR_6) && (addr >= ADDR_FMC_SECTOR_5)) {
        sector = CTL_SECTOR_NUMBER_5;
    } else if((addr < ADDR_FMC_SECTOR_7) && (addr >= ADDR_FMC_SECTOR_6)) {
        sector = CTL_SECTOR_NUMBER_6;
    } else if((addr < ADDR_FMC_SECTOR_8) && (addr >= ADDR_FMC_SECTOR_7)) {
        sector = CTL_SECTOR_NUMBER_7;
    } else if((addr < ADDR_FMC_SECTOR_9) && (addr >= ADDR_FMC_SECTOR_8)) {
        sector = CTL_SECTOR_NUMBER_8;
    } else if((addr < ADDR_FMC_SECTOR_10) && (addr >= ADDR_FMC_SECTOR_9)) {
        sector = CTL_SECTOR_NUMBER_9;
    } else if((addr < ADDR_FMC_SECTOR_11) && (addr >= ADDR_FMC_SECTOR_10)) {
        sector = CTL_SECTOR_NUMBER_10;
    } else if((addr < ADDR_FMC_SECTOR_12) && (addr >= ADDR_FMC_SECTOR_11)) {
        sector = CTL_SECTOR_NUMBER_11;
    } else if((addr < ADDR_FMC_SECTOR_13) && (addr >= ADDR_FMC_SECTOR_12)) {
        sector = CTL_SECTOR_NUMBER_12;
    } else if((addr < ADDR_FMC_SECTOR_14) && (addr >= ADDR_FMC_SECTOR_13)) {
        sector = CTL_SECTOR_NUMBER_13;
    } else if((addr < ADDR_FMC_SECTOR_15) && (addr >= ADDR_FMC_SECTOR_14)) {
        sector = CTL_SECTOR_NUMBER_14;
    } else if((addr < ADDR_FMC_SECTOR_16) && (addr >= ADDR_FMC_SECTOR_15)) {
        sector = CTL_SECTOR_NUMBER_15;
    } else if((addr < ADDR_FMC_SECTOR_17) && (addr >= ADDR_FMC_SECTOR_16)) {
        sector = CTL_SECTOR_NUMBER_16;
    } else if((addr < ADDR_FMC_SECTOR_18) && (addr >= ADDR_FMC_SECTOR_17)) {
        sector = CTL_SECTOR_NUMBER_17;
    } else if((addr < ADDR_FMC_SECTOR_19) && (addr >= ADDR_FMC_SECTOR_18)) {
        sector = CTL_SECTOR_NUMBER_18;
    } else if((addr < ADDR_FMC_SECTOR_20) && (addr >= ADDR_FMC_SECTOR_19)) {
        sector = CTL_SECTOR_NUMBER_19;
    } else if((addr < ADDR_FMC_SECTOR_21) && (addr >= ADDR_FMC_SECTOR_20)) {
        sector = CTL_SECTOR_NUMBER_20;
    } else if((addr < ADDR_FMC_SECTOR_22) && (addr >= ADDR_FMC_SECTOR_21)) {
        sector = CTL_SECTOR_NUMBER_21;
    } else if((addr < ADDR_FMC_SECTOR_23) && (addr >= ADDR_FMC_SECTOR_22)) {
        sector = CTL_SECTOR_NUMBER_22;
    } else if((addr < ADDR_FMC_SECTOR_24) && (addr >= ADDR_FMC_SECTOR_23)) {
        sector = CTL_SECTOR_NUMBER_23;
    } else if((addr < ADDR_FMC_SECTOR_25) && (addr >= ADDR_FMC_SECTOR_24)) {
        sector = CTL_SECTOR_NUMBER_24;
    } else if((addr < ADDR_FMC_SECTOR_26) && (addr >= ADDR_FMC_SECTOR_25)) {
        sector = CTL_SECTOR_NUMBER_25;
    } else if((addr < ADDR_FMC_SECTOR_27) && (addr >= ADDR_FMC_SECTOR_26)) {
        sector = CTL_SECTOR_NUMBER_26;
    } else {
        sector = CTL_SECTOR_NUMBER_27;
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
fmc_state_enum iap_data_write(uint8_t *data, uint32_t addr, uint32_t len)
{
    uint32_t idx = 0U;
    fmc_state_enum fmc_state = FMC_READY;

    /* check if the address is in protected area */
    if(IS_PROTECTED_AREA(addr)) {
        return FMC_BUSY;
    }

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    /* data received are word multiple */
    for(idx = 0U; idx < len; idx += 2U) {
        if(FMC_READY == (fmc_state = fmc_halfword_program(addr, *(uint32_t *)(data + idx)))) {
            addr += 2U;
        } else {
            while(1) {
            }
        }
    }

    /* lock the flash program erase controller */
    fmc_lock();

    return fmc_state;
}

/*!
    \brief      program option byte
    \param[in]  mem_add: target address
    \param[in]  data: pointer to target data
    \param[out] none
    \retval     state of FMC, refer to fmc_state_enum
*/
fmc_state_enum option_byte_write(uint32_t mem_add, uint8_t *data, uint16_t len)
{
    fmc_state_enum fmc_state ;

    /* unlock the flash program erase controller */
    fmc_unlock();

    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);

    fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

    if(FMC_READY == fmc_state) {
        /* authorize the small information block programming */
        ob_unlock();

        /* start erase the option byte */
        user_ob_erase(mem_add);

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

        ob_start();

        if(OPT_BYTE_ADDR1 == mem_add) {
            if(1U == ((data[0] >> 7U) & 0x01U)) {
                nRST_STDBY = OB_STDBY_NRST;
            } else {
                nRST_STDBY = OB_STDBY_RST;
            }

            if(1U == ((data[0] >> 6U) & 0x01U)) {
                nRST_DPSLP = OB_DEEPSLEEP_NRST;
            } else {
                nRST_DPSLP = OB_DEEPSLEEP_RST;
            }

            if(1U == ((data[0] >> 5U) & 0x01U)) {
                nWDG_HW = OB_FWDGT_SW;
            } else {
                nWDG_HW = OB_FWDGT_HW;
            }

            ob_user_write(nWDG_HW, nRST_DPSLP, nRST_STDBY);

            ob_security_protection_config(data[1]);

            reg0 = FMC_OBCTL0;

            if(0U == ((data[0] >> 2U) & 0x03U)) {
                BOR_TH = OB_BOR_TH_VALUE3;
            } else if(1U == ((data[0] >> 2U) & 0x03U)) {
                BOR_TH = OB_BOR_TH_VALUE2;
            } else if(2U == ((data[0] >> 2U) & 0x03U)) {
                BOR_TH = OB_BOR_TH_VALUE1;
            } else {
                BOR_TH = OB_BOR_TH_OFF;
            }
            ob_user_bor_threshold(BOR_TH);

            if(1U == ((data[0] >> 4U) & 0x01U)) {
                BB = OB_BB_ENABLE;
            } else {
                BB = OB_BB_DISABLE;
            }
            ob_boot_mode_config(BB);

            WP0 = (data[8] | (data[9] << 8U)) & 0x0FFFU;

            if(RESET != (FMC_OBCTL0 & FMC_OBCTL0_DRP)) {
                while(1) {
                }
            }
            /* wait for the FMC ready */
            fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

            reg0 &= (~((uint32_t)0x0FFFU << 16U));

            if(FMC_READY == fmc_state) {
                reg = (WP0 << 16U);
                FMC_OBCTL0 = reg0 | reg;
            }
        } else {
            reg1 = FMC_OBCTL1;

            WP1 = (data[8] | (data[9] << 8U)) & 0x0FFFU;

            if(RESET != (FMC_OBCTL0 & FMC_OBCTL0_DRP)) {
                while(1) {
                }
            }
            /* wait for the FMC ready */
            fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);

            reg1 &= (~((uint32_t)0x0FFFU << 16U));

            if(FMC_READY == fmc_state) {
                reg = (WP1 << 16U);
                FMC_OBCTL1 = reg1 | reg;
            }
        }

        ob_start();
    }

    /* lock the flash program erase controller */
    fmc_lock();

    return fmc_state;
}

/*!
    \brief      user erase option byte
    \param[in]  opt_addr: option byte address
    \param[out] none
    \retval     none
*/
void user_ob_erase(uint32_t opt_addr)
{
    uint32_t reg, reg1;
    fmc_state_enum fmc_state = FMC_READY;
    /* wait for the FMC ready */
    fmc_state = fmc_ready_wait(FMC_TIMEOUT_COUNT);
    reg = FMC_OBCTL0;
    reg1 = FMC_OBCTL1;

    if(FMC_READY == fmc_state) {
        if(OPT_BYTE_ADDR1 == opt_addr) {
            /* reset the OB_FWDGT, OB_DEEPSLEEP and OB_STDBY, set according to ob_fwdgt ,ob_deepsleep and ob_stdby */
            reg |= (FMC_OBCTL0_NWDG_HW | FMC_OBCTL0_NRST_DPSLP | FMC_OBCTL0_NRST_STDBY);
            /* reset the BOR level */
            reg |= FMC_OBCTL0_BOR_TH;
            /* reset option byte boot bank value */
            reg &= ~FMC_OBCTL0_BB;
            /* reset option byte dbs value */
            reg &= ~FMC_OBCTL0_DBS;

            /* reset drp and wp value */
            reg |= FMC_OBCTL0_WP0;
            reg &= (~FMC_OBCTL0_DRP);
            FMC_OBCTL0 = reg;
        } else {
            reg1 |= FMC_OBCTL1_WP1;
            FMC_OBCTL1 = reg1;
        }
    }
}

/*!
    \brief      jump to execute address
    \param[in]  addr: execute address
    \param[out] none
    \retval     none
*/
void jump_to_execute(uint32_t addr)
{
    static uint32_t stack_addr = 0U, exe_addr = 0U;

    /* set interrupt vector base address */
    SCB->VTOR = addr;
    __DSB();

    /* init user application's stack pointer and execute address */
    stack_addr = *(uint32_t *)addr;
    exe_addr = *(uint32_t *)(addr + 4U);

    /* re-configure MSP */
    __set_MSP(stack_addr);

    (*((void (*)())exe_addr))();
}

/*!
    \brief      reset all register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void register_reset(void)
{
    /* disable systick */
    SysTick->CTRL = 0U;
    SysTick->VAL = 0U;

    /* reset Peripherals */
    RCU_APB2RST = 0xFFFFFFFFU;
    RCU_APB2RST = 0x00000000U;
    RCU_APB1RST = 0xFFFFFFFFU;
    RCU_APB1RST = 0x00000000U;
    RCU_AHB1RST = 0xFFFFFFFFU;
    RCU_AHB1RST = 0x00000000U;
    RCU_AHB2RST = 0xFFFFFFFFU;
    RCU_AHB2RST = 0x00000000U;
    RCU_AHB3RST = 0xFFFFFFFFU;
    RCU_AHB3RST = 0x00000000U;
    RCU_APB2EN = 0x00000000U;
    RCU_APB1EN = 0x00000000U;
    RCU_AHB1EN = 0x00100000U;
    RCU_AHB2EN = 0x00000000U;

    rcu_deinit();

    RCU_CFG0 = 0x00000000U;
    RCU_CFG1 = 0x00000000U;
}
