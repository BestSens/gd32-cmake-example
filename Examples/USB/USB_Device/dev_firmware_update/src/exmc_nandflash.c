/*!
    \file    exmc_nandflash.c
    \brief   nandflash(GD9FU1G8F2AMG) driver

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

#include "exmc_nandflash.h"

/* define the physical address of nand flash, and it is determined by the hardware */
#define BANK1_NAND_ADDR     ((uint32_t)0x70000000)
#define BANK_NAND_ADDR      BANK1_NAND_ADDR

/* define operating nand flash macro */
#define NAND_CMD_AREA       *(__IO uint8_t *)(BANK_NAND_ADDR | EXMC_CMD_AREA)
#define NAND_ADDR_AREA      *(__IO uint8_t *)(BANK_NAND_ADDR | EXMC_ADDR_AREA)
#define NAND_DATA_AREA      *(__IO uint8_t *)(BANK_NAND_ADDR | EXMC_DATA_AREA)

/* the macro of calculate nand flash operating address */
#define ROW_ADDRESS         (addr.page + (addr.block + (addr.zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)

/* define NWATI operation */
#define NWAIT_OPERA_FUNC()    while(gpio_input_bit_get(GPIOD, GPIO_PIN_6) == 0)

/* static variable definition */
static uint16_t cur_zone = 0;
static uint16_t LUT_tab[NAND_BLOCK_COUNT];
static uint8_t temp_buf[NAND_PAGE_TOTAL_SIZE];

/* local function prototypes ('static') */
static uint8_t exmc_nand_page_copyback(uint32_t src_pageno, uint32_t dest_pageno);
static uint8_t exmc_nand_page_copyback_ex(uint32_t src_pageno, uint32_t dest_pageno, uint8_t *pbuf, uint16_t offset, uint16_t size);
static uint8_t exmc_nand_write_page(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt);
static uint8_t exmc_nand_read_page(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt);
static uint8_t exmc_nand_write_spare(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt);
static uint8_t exmc_nand_read_spare(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt);
static uint8_t exmc_nand_write_data(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt);
static uint8_t exmc_nand_read_data(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt);
static uint8_t exmc_nand_erase_block(uint32_t blocknum);
static uint8_t exmc_nand_read_status(void);
static uint8_t exmc_nand_get_status(void);
static uint8_t nand_write_new_block(nand_address_struct addr, nand_address_struct logaddr, uint8_t *pwritebuf, uint16_t size);
static uint8_t nand_write_block(nand_address_struct logaddr, uint8_t *pwritebuf, uint16_t size);
static uint8_t nand_read_block(nand_address_struct logaddr, uint8_t *preadbuf, uint16_t size);
static uint8_t nand_build_LUT(uint16_t zone);
static uint16_t nand_find_free_block(uint8_t odd_even);
static uint8_t nand_judge_buf_ok(uint8_t *pbuf, uint32_t len, uint8_t value);
static uint8_t nand_isbad_block(uint32_t blocknum);
static uint8_t nand_mark_used_block(uint32_t blocknum);
static void nand_mark_bad_block(uint32_t blocknum);
static uint8_t nand_mark_logic_block(uint32_t blocknum, uint16_t logblock);

/*!
    \brief      initialize nand flash peripheral 
    \param[in]  none
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
uint8_t exmc_nandflash_init(uint32_t nand_bank)
{
    exmc_nand_parameter_struct nand_init_struct;
    exmc_nand_pccard_timing_parameter_struct nand_timing_init_struct;

    /* enable EXMC clock*/
    rcu_periph_clock_enable(RCU_EXMC);
    rcu_periph_clock_enable(RCU_GPIOD);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOG);

    /* common GPIO configuration */
    /* D2(PD0),D3(PD1),D0(PD14),D1(PD15) pin configuration */
    gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_14 | GPIO_PIN_15);

    /* D4(PE7),D5(PE8),D6(PE9),D7(PE10) pin configuration */
    gpio_af_set(GPIOE, GPIO_AF_12, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10);

    /* CLE(PD11),ALE(PD12) pin configuration */
    gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_11 | GPIO_PIN_12);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11 | GPIO_PIN_12);

    /* NOE(PD4),NWE(PD5) pin configuration */
    gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_4 | GPIO_PIN_5);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_4 | GPIO_PIN_5);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5);

    /* NWAIT(PD6) pin configuration */
    gpio_mode_set(GPIOD, GPIO_MODE_INPUT, GPIO_PUPD_PULLUP, GPIO_PIN_6);

    /* NCE1(PD7) pin configuration */
    gpio_af_set(GPIOD, GPIO_AF_12, GPIO_PIN_7);
    gpio_mode_set(GPIOD, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOD, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);

    /* EXMC configuration */
    nand_timing_init_struct.setuptime = 5;
    nand_timing_init_struct.waittime = 4;
    nand_timing_init_struct.holdtime = 2;
    nand_timing_init_struct.databus_hiztime = 2;

    nand_init_struct.nand_bank = nand_bank;
    nand_init_struct.ecc_size = EXMC_ECC_SIZE_2048BYTES;
    nand_init_struct.atr_latency = EXMC_ALE_RE_DELAY_3_HCLK;
    nand_init_struct.ctr_latency = EXMC_CLE_RE_DELAY_3_HCLK;
    nand_init_struct.ecc_logic = ENABLE;
    nand_init_struct.databus_width = EXMC_NAND_DATABUS_WIDTH_8B;
    nand_init_struct.wait_feature = ENABLE;
    nand_init_struct.common_space_timing = &nand_timing_init_struct;
    nand_init_struct.attribute_space_timing = &nand_timing_init_struct;

    exmc_nand_init(&nand_init_struct);

    /* enable EXMC NAND bank */
    exmc_nand_enable(nand_bank);

    /* build LUT(Look up table) */
    return nand_build_LUT(0);
}

/*!
    \brief      read NAND flash ID
    \param[in]  nand_id: structure of nand flash ID
    \param[out] none
    \retval     none
*/
void nand_read_id(nand_id_struct *nand_id)
{
    uint32_t data = 0;

    /* send command to the command area */
    *(__IO uint8_t *)(BANK_NAND_ADDR | EXMC_CMD_AREA) = 0x90;
    *(__IO uint8_t *)(BANK_NAND_ADDR | EXMC_ADDR_AREA) = 0x00;

    /* sequence to read ID from NAND flash */
    data = *(__IO uint32_t *)(BANK_NAND_ADDR | EXMC_DATA_AREA);

    nand_id->maker_id   = ADDR_1ST_CYCLE(data);
    nand_id->device_id  = ADDR_2ND_CYCLE(data);
    nand_id->third_id   = ADDR_3RD_CYCLE(data);
    nand_id->fourth_id  = ADDR_4TH_CYCLE(data);
}

/*!
    \brief      write data to nand flash
    \param[in]  mem_addr: logic page number to be written
    \param[in]  pwritebuf: pointer to write data buffer
    \param[in]  size: data count
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
uint8_t nand_write(uint32_t mem_addr, uint8_t *pwritebuf, uint16_t size)
{
    uint32_t temp;
    uint32_t block_remain_size;
    nand_address_struct logaddr;

    temp = mem_addr % (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);

    logaddr.zone = mem_addr / (NAND_BLOCK_SIZE * NAND_PAGE_SIZE * NAND_ZONE_SIZE);
    logaddr.block = mem_addr / (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);
    logaddr.page = temp / NAND_PAGE_SIZE;
    logaddr.page_in_offset = temp % NAND_PAGE_SIZE;

    block_remain_size = (NAND_BLOCK_SIZE * NAND_PAGE_SIZE) - \
                        (NAND_PAGE_SIZE * logaddr.page + logaddr.page_in_offset);

    while(size > block_remain_size) {
        if(NAND_FAIL == nand_write_block(logaddr, pwritebuf, block_remain_size)) {
            return NAND_FAIL;
        }

        logaddr.block++;
        pwritebuf += block_remain_size;
        size -= block_remain_size;
        logaddr.page = 0;
        logaddr.page_in_offset = 0;
        block_remain_size = (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);
    }

    if(size > 0) {
        if(NAND_FAIL == nand_write_block(logaddr, pwritebuf, size)) {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/*!
    \brief      read data from nand flash
    \param[in]  mem_addr: logic page number to be read
    \param[in]  preadbuf: pointer to the data buffer to be read
    \param[in]  size: data count
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
uint8_t nand_read(uint32_t mem_addr, uint8_t *preadbuf, uint16_t size)
{
    uint32_t temp;
    uint32_t block_remain_size;
    nand_address_struct  logaddr;

    temp = mem_addr % (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);

    logaddr.zone = mem_addr / (NAND_BLOCK_SIZE * NAND_PAGE_SIZE * NAND_ZONE_SIZE);
    logaddr.block = mem_addr / (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);
    logaddr.page = temp / NAND_PAGE_SIZE;
    logaddr.page_in_offset = temp % NAND_PAGE_SIZE;

    block_remain_size = (NAND_BLOCK_SIZE * NAND_PAGE_SIZE) - \
                        (NAND_PAGE_SIZE * logaddr.page + logaddr.page_in_offset);

    while(size > block_remain_size) {
        if(NAND_FAIL == nand_read_block(logaddr, preadbuf, block_remain_size)) {
            return NAND_FAIL;
        }

        logaddr.block++;
        preadbuf += block_remain_size;
        size -= block_remain_size;
        logaddr.page = 0;
        logaddr.page_in_offset = 0;
        block_remain_size = (NAND_BLOCK_SIZE * NAND_PAGE_SIZE);
    }

    if(size > 0) {
        if(NAND_FAIL == nand_read_block(logaddr, preadbuf, size)) {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/*!
    \brief      scan and test specific block
    \param[in]  blocknum: block number
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
uint8_t nand_scan_block(uint32_t blocknum)
{
    uint32_t i, k;
    nand_address_struct addr;

    addr.zone = cur_zone;
    addr.block = blocknum;
    addr.page = 0;
    addr.page_in_offset = NAND_PAGE_SIZE;

    memset(temp_buf, 0x00, NAND_PAGE_TOTAL_SIZE);

    for(i = 0; i < BAD_BALOK_TEST_CYCLE; i++) {
        if(NAND_READY != exmc_nand_erase_block(blocknum)) {
            nand_mark_bad_block(blocknum);
            return NAND_FAIL;
        }

        addr.page = 0;
        for(k = 0; k < NAND_BLOCK_SIZE; k++) {
            exmc_nand_read_page(temp_buf, addr, NAND_PAGE_TOTAL_SIZE);

            if(NAND_OK != nand_judge_buf_ok(temp_buf, NAND_PAGE_TOTAL_SIZE, 0xFF)) {
                nand_mark_bad_block(blocknum);
                return NAND_FAIL;
            }

            addr.page++;
        }

        addr.page = 0;

        for(k = 0; k < NAND_BLOCK_SIZE; k++) {
            memset(temp_buf, 0x00, NAND_PAGE_TOTAL_SIZE);

            if(NAND_OK != exmc_nand_write_page(temp_buf, addr, NAND_PAGE_TOTAL_SIZE)) {
                nand_mark_bad_block(blocknum);
                return NAND_FAIL;
            }

            exmc_nand_read_page(temp_buf, addr, NAND_PAGE_TOTAL_SIZE);

            if(NAND_OK != nand_judge_buf_ok(temp_buf, NAND_PAGE_TOTAL_SIZE, 0x00)) {
                nand_mark_bad_block(blocknum);
                return NAND_FAIL;
            }

            addr.page++;
        }
    }

    if(NAND_READY != exmc_nand_erase_block(blocknum)) {
        nand_mark_bad_block(blocknum);
        return NAND_FAIL;
    }

    addr.page = 0;

    for(k = 0; k < NAND_BLOCK_SIZE; k++) {
        exmc_nand_read_page(temp_buf, addr, NAND_PAGE_TOTAL_SIZE);

        if(NAND_OK != nand_judge_buf_ok(temp_buf, NAND_PAGE_TOTAL_SIZE, 0xFF)) {
            nand_mark_bad_block(blocknum);
            return NAND_FAIL;
        }

        addr.page++;
    }

    return NAND_OK;
}

/*!
    \brief      check block is free block
    \param[in]  blocknum: block number
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
 uint8_t nand_judge_free_block(uint32_t blocknum)
{
    uint8_t flag;
    nand_address_struct addr;

    addr.zone = cur_zone;
    addr.block = blocknum;
    addr.page = 0;
    addr.page_in_offset = NAND_PAGE_SIZE + USED_OFFSET;

    if(nand_isbad_block(blocknum)) {
        return 0;
    }

    exmc_nand_read_page(&flag, addr, 1);

    if(0xFF == flag) {
        return 1;
    }

    return 0;
}

/*!
    \brief      copy data from a page to another page
    \param[in]  src_pageno: source page number
    \param[in]  dest_pageno: destination page number
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t exmc_nand_page_copyback(uint32_t src_pageno, uint32_t dest_pageno)
{
    NAND_CMD_AREA = NAND_CMD_COPYBACK_A;

    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = src_pageno;
    NAND_ADDR_AREA = (src_pageno & 0xFF00) >> 8;

    NAND_CMD_AREA = NAND_CMD_COPYBACK_B;

    /* must wait, or read data wrong */
    NWAIT_OPERA_FUNC();

    NAND_CMD_AREA = NAND_CMD_COPYBACK_C;

    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = dest_pageno;
    NAND_ADDR_AREA = (dest_pageno & 0xFF00) >> 8;

    NAND_CMD_AREA = NAND_CMD_COPYBACK_D;

    /* check operation status */
    if(NAND_READY == exmc_nand_get_status()) {
        return NAND_OK;
    }

    return NAND_FAIL;
}

/*!
    \brief      copy data from a page to another page and update some data in target page
    \param[in]  src_pageno: source page number
    \param[in]  dest_pageno: destination page number
    \param[in]  pbuf: data buffer
    \param[in]  offset: offset in the page, data buffer will write data from it
    \param[in]  size: data buffer size
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t exmc_nand_page_copyback_ex(uint32_t src_pageno, uint32_t dest_pageno, uint8_t *pbuf, uint16_t offset, uint16_t size)
{
    uint16_t i;

    NAND_CMD_AREA = NAND_CMD_COPYBACK_A;

    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = src_pageno;
    NAND_ADDR_AREA = (src_pageno & 0xFF00U) >> 8U;

    NAND_CMD_AREA = NAND_CMD_COPYBACK_B;

    /* must wait, or read data wrong */
    NWAIT_OPERA_FUNC();

    NAND_CMD_AREA = NAND_CMD_COPYBACK_C;

    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = 0;
    NAND_ADDR_AREA = dest_pageno;
    NAND_ADDR_AREA = (dest_pageno & 0xFF00U) >> 8U;

    /* need no data, need no wait */
    NAND_CMD_AREA = NAND_CMD_COPYBACK_C;

    NAND_ADDR_AREA = offset;
    NAND_ADDR_AREA = offset >> 8U;

    /* send data */
    for(i = 0; i < size; i++) {
        NAND_DATA_AREA = pbuf[i];
    }

    NAND_CMD_AREA = NAND_CMD_COPYBACK_D;

    /* check operation status */
    if(NAND_READY == exmc_nand_get_status()) {
        return NAND_OK;
    }

    return NAND_FAIL;
}

/*!
    \brief      write data to page
    \param[in]  pbuf: pointer on the buffer containing data to be written
    \param[in]  addr: the address of the data to be written
    \param[in]  bytecnt: byte count to be written
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t exmc_nand_write_page(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt)
{
    uint16_t i;

    /* send write page command */
    NAND_CMD_AREA = NAND_CMD_WRITE_1ST;

    NAND_ADDR_AREA = addr.page_in_offset;
    NAND_ADDR_AREA = addr.page_in_offset >> 8U;
    NAND_ADDR_AREA = ADDR_1ST_CYCLE(ROW_ADDRESS);
    NAND_ADDR_AREA = ADDR_2ND_CYCLE(ROW_ADDRESS);

    /* write data */
    for(i = 0; i < bytecnt; i++) {
        NAND_DATA_AREA = pbuf[i];
    }

    NAND_CMD_AREA = NAND_CMD_WRITE_2ND;

    /* check operation status */
    if(NAND_READY == exmc_nand_get_status()) {
        return NAND_OK;
    }

    return NAND_FAIL;
}

/*!
    \brief      read data from page
    \param[in]  pbuf: pointer on the buffer filling data to be read
    \param[in]  addr: the address of the data to be read
    \param[in]  bytecnt: byte count to be read
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t exmc_nand_read_page(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt)
{
    uint16_t i;

    /* send read page command */
    NAND_CMD_AREA = NAND_CMD_READ1_1ST;

    NAND_ADDR_AREA = addr.page_in_offset;
    NAND_ADDR_AREA = addr.page_in_offset >> 8;
    NAND_ADDR_AREA = ADDR_1ST_CYCLE(ROW_ADDRESS);
    NAND_ADDR_AREA = ADDR_2ND_CYCLE(ROW_ADDRESS);

    NAND_CMD_AREA = NAND_CMD_READ1_2ND;

    /* must wait, or read data wrong */
    NWAIT_OPERA_FUNC();

    /* read data to buffer */
    for(i = 0; i < bytecnt; i++) {
        pbuf[i] = NAND_DATA_AREA;
    }

    return NAND_OK;
}

/*!
    \brief      write the spare area of one page data
    \param[in]  pbuf: pointer on the buffer containing data to be written
    \param[in]  addr: the address of the data to be written
    \param[in]  bytecnt: byte count to be written
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t exmc_nand_write_spare(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt)
{
    if(addr.page_in_offset < NAND_PAGE_SIZE) {
        return NAND_FAIL;
    }

    if(bytecnt + addr.page_in_offset > NAND_PAGE_TOTAL_SIZE) {
        return NAND_FAIL;
    }

    return exmc_nand_write_page(pbuf, addr, bytecnt);
}

/*!
    \brief      read data from the spare area of 1 page
    \param[in]  pbuf: pointer on the buffer filling data to be read
    \param[in]  addr: the address of the data to be read
    \param[in]  bytecnt: count of spare area to be read
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t exmc_nand_read_spare(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt)
{
    if(addr.page_in_offset < NAND_PAGE_SIZE) {
        return NAND_FAIL;
    }

    if(bytecnt + addr.page_in_offset > NAND_PAGE_TOTAL_SIZE) {
        return NAND_FAIL;
    }

    return exmc_nand_read_page(pbuf, addr, bytecnt);
}

/*!
    \brief      write data to the page main data area
    \param[in]  pbuf: pointer on the buffer containing data to be written
    \param[in]  addr: physical address data to be written
    \param[in]  bytecnt: byte count to be written
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t exmc_nand_write_data(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt)
{
    uint8_t *temppbuf = pbuf;

    while(bytecnt + addr.page_in_offset > NAND_PAGE_SIZE) {
        if(NAND_OK != exmc_nand_write_page(temppbuf,
                                           addr,
                                           NAND_PAGE_SIZE - addr.page_in_offset)) {
            return NAND_FAIL;
        }

        bytecnt -= NAND_PAGE_SIZE - addr.page_in_offset;
        temppbuf += NAND_PAGE_SIZE - addr.page_in_offset;
        addr.page++;
        addr.page_in_offset = 0;
    }

    if(bytecnt > 0) {
        if(NAND_OK != exmc_nand_write_page(temppbuf, addr, bytecnt)) {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/*!
    \brief      read data from the page main data area
    \param[in]  pbuf: pointer on the buffer filling data to be read
    \param[in]  addr: the physical address of the data to be read
    \param[in]  bytecnt: byte count to be read
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t exmc_nand_read_data(uint8_t *pbuf, nand_address_struct addr, uint16_t bytecnt)
{
    uint8_t *temppbuf = pbuf;

    while(bytecnt + addr.page_in_offset > NAND_PAGE_SIZE) {
        if(NAND_OK != exmc_nand_read_page(temppbuf, addr, NAND_PAGE_SIZE - addr.page_in_offset)) {
            return NAND_FAIL;
        }

        addr.page++;
        temppbuf += NAND_PAGE_SIZE - addr.page_in_offset;
        bytecnt -= NAND_PAGE_SIZE - addr.page_in_offset;
        addr.page_in_offset = 0;
    }

    if(bytecnt > 0) {
        if(NAND_OK != exmc_nand_read_page(temppbuf, addr, bytecnt)) {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/*!
    \brief      erase complete block of NAND flash
    \param[in]  blocknum: block number to be erased
    \param[out] none
    \retval     NAND_TIMEOUT_ERROR, NAND_READY
*/
static uint8_t exmc_nand_erase_block(uint32_t blocknum)
{
    /* send erase command */
    NAND_CMD_AREA = NAND_CMD_ERASE_1ST;

    blocknum <<= 6;
    NAND_ADDR_AREA = blocknum;
    NAND_ADDR_AREA = blocknum >> 8;

    NAND_CMD_AREA = NAND_CMD_ERASE_2ND;

    return (exmc_nand_get_status());
}

/*!
    \brief      read the NAND flash status
    \param[in]  none
    \param[out] none
    \retval     NAND_BUSY, NAND_READY, NAND_ERROR
*/
static uint8_t exmc_nand_read_status(void)
{
    uint8_t data;
    uint8_t status = NAND_BUSY;

    /* read operation status */
    NAND_CMD_AREA = NAND_CMD_STATUS;
    data = *(__IO uint8_t *)(BANK_NAND_ADDR);

    if(NAND_ERROR == (data & NAND_ERROR)) {
        status = NAND_ERROR;
    } else if(NAND_READY == (data & NAND_READY)) {
        status = NAND_READY;
    } else {
        status = NAND_BUSY;
    }

    return (status);
}

/*!
    \brief      get NAND flash operation status
    \param[in]  none
    \param[out] none
    \retval     NAND_TIMEOUT_ERROR, NAND_READY
*/
static uint8_t exmc_nand_get_status(void)
{
    uint32_t timeout = 0x10000;
    uint8_t status = NAND_READY;

    status = exmc_nand_read_status();

    /* wait for operation is over, exit when time out */
    while((NAND_READY != status) && (0x00U != timeout)) {
        status = exmc_nand_read_status();
        timeout--;
    }

    if(0x00 == timeout) {
        status =  NAND_TIMEOUT_ERROR;
    }

    return status;
}

/*!
    \brief      copy data from old block to new block and write new data to the new block
    \param[in]  addr: physical address data to be written
    \param[in]  logaddr: logic address data to be written
    \param[in]  pwritebuf: pointer to write data buffer
    \param[in]  size: data count
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t nand_write_new_block(nand_address_struct addr, nand_address_struct logaddr, uint8_t *pwritebuf, uint16_t size)
{
    uint16_t n, i, offset;
    uint16_t newblock;

    for(n = 0; n < 5; n++) {
        if(addr.block & 0x0001) {
            newblock = nand_find_free_block(BLOCK_EVEN);
        } else {
            newblock = nand_find_free_block(BLOCK_ODD);
        }

        if(newblock >= NAND_BLOCK_COUNT) {
            return NAND_FAIL;
        }

        for(i = 0; i < NAND_BLOCK_SIZE; i++) {
            if(i == addr.page) {
                offset = addr.page_in_offset;
                while(offset + size > NAND_PAGE_SIZE) {
                    if(exmc_nand_page_copyback_ex(addr.block * NAND_BLOCK_SIZE + i,
                                                  newblock * NAND_BLOCK_SIZE + i,
                                                  pwritebuf, offset,
                                                  NAND_PAGE_SIZE - offset) == NAND_FAIL) {
                        nand_mark_bad_block(newblock);
                        nand_build_LUT(addr.zone);

                        break;
                    }

                    pwritebuf += NAND_PAGE_SIZE - offset;
                    size -= NAND_PAGE_SIZE - offset;
                    offset = 0;
                    i++;
                }

                if(size > 0) {
                    if(exmc_nand_page_copyback_ex(addr.block * NAND_BLOCK_SIZE + i,
                                                  newblock * NAND_BLOCK_SIZE + i,
                                                  pwritebuf, offset, size) == NAND_FAIL) {
                        nand_mark_bad_block(newblock);
                        nand_build_LUT(addr.zone);
                        break;
                    }
                }
            } else {
                if(exmc_nand_page_copyback(addr.block * NAND_BLOCK_SIZE + i,
                                           newblock * NAND_BLOCK_SIZE + i) == NAND_FAIL) {
                    nand_mark_bad_block(newblock);
                    nand_build_LUT(addr.zone);
                    break;
                }
            }
        }

        if(i == NAND_BLOCK_SIZE) {
            if(NAND_FAIL == nand_mark_used_block(newblock)) {
                nand_mark_bad_block(newblock);
                nand_build_LUT(addr.zone);
                continue;
            } else {
                nand_mark_logic_block(newblock, logaddr.block);
            }

            if(NAND_READY != exmc_nand_erase_block(addr.block)) {
                nand_mark_bad_block(addr.block);
                nand_build_LUT(addr.zone);
                continue;
            }

            nand_build_LUT(addr.zone);
            break;
        }
    }

    return NAND_OK;
}

/*!
    \brief      write data to block
    \param[in]  logaddr: logic address to be written
    \param[in]  pwritebuf: pointer to write data buffer
    \param[in]  size: data count
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t nand_write_block(nand_address_struct logaddr, uint8_t *pwritebuf, uint16_t size)
{
    uint16_t PBN;
    nand_address_struct addr;

    if(cur_zone != logaddr.zone) {
        nand_build_LUT(logaddr.zone);
    }

    PBN = LUT_tab[logaddr.block];

    if(0xFFFFU == PBN) {
        return NAND_FAIL;
    }

    addr.zone = logaddr.zone;
    addr.block = PBN & 0x03FF;
    addr.page = logaddr.page;
    addr.page_in_offset = logaddr.page_in_offset;

    if(PBN & USED_BLOCK) {
        return nand_write_new_block(addr, logaddr, (uint8_t *)pwritebuf, size);
    } else {
        if(NAND_FAIL == exmc_nand_write_data((uint8_t *)pwritebuf, addr, size)) {
            return NAND_FAIL;
        }

        if(NAND_FAIL == nand_mark_used_block(addr.block)) {
            return NAND_FAIL;
        } else {
            if(NAND_FAIL == nand_mark_logic_block(addr.block, logaddr.block)) {
                return NAND_FAIL;
            }
        }

        return NAND_OK;
    }
}


/*!
    \brief      read data from block
    \param[in]  logaddr: logic address to be read
    \param[in]  preadbuf: pointer to the data buffer to be read
    \param[in]  size: data count
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t nand_read_block(nand_address_struct logaddr, uint8_t *preadbuf, uint16_t size)
{
    uint16_t PBN;
    nand_address_struct addr;

    if(cur_zone != logaddr.zone) {
        nand_build_LUT(logaddr.zone);
    }

    PBN = LUT_tab[logaddr.block];
    addr.block = PBN & 0x03FF;

    if(addr.block >= NAND_BLOCK_COUNT) {
        return NAND_FAIL;
    }

    addr.zone = logaddr.zone;
    addr.page = logaddr.page;
    addr.page_in_offset = logaddr.page_in_offset;

    if(NAND_FAIL == exmc_nand_read_data((uint8_t *)preadbuf, addr, size)) {
        return NAND_FAIL;
    }

    return NAND_OK;
}

/*!
    \brief      build NAND flash look up table
    \param[in]  zone: bulid zone
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t nand_build_LUT(uint16_t zone)
{
    uint16_t logic_blockID;
    uint16_t bad_block, cur_block, free_block;
    uint8_t spare_area[NAND_SPARE_AREA_SIZE];
    nand_address_struct addr;

    cur_zone = zone;
    addr.zone = cur_zone;
    addr.block = 0x00;
    addr.page = 0x00;
    addr.page_in_offset = NAND_PAGE_SIZE;

    /* 1st step : init */
    for(cur_block = 0; cur_block < NAND_ZONE_SIZE; cur_block++) {
        LUT_tab[cur_block] = FREE_BLOCK;
    }

    /* init Pointers */
    bad_block = NAND_ZONE_SIZE - 1;
    cur_block = 0;

    /* 2nd step : locate used and bad blocks */
    while(cur_block < NAND_ZONE_SIZE) {
        addr.block = cur_block;
        exmc_nand_read_spare(spare_area, addr, NAND_SPARE_AREA_SIZE);

        if(spare_area[BI_OFFSET] != 0xFF) {
            LUT_tab[bad_block--] |= cur_block | (uint16_t)BAD_BLOCK;
            LUT_tab[cur_block] &= (uint16_t)~FREE_BLOCK;

            if(bad_block <= MAX_LOG_BLOCKS_PER_ZONE + EXCHANGE_BLOCKS_NUM) {
                return NAND_FAIL;
            }
        } else if(spare_area[USED_OFFSET] == 0xFE) {
            logic_blockID = (spare_area[LBN0_OFFSET] + (spare_area[LBN1_OFFSET] * 256)) & 0x03FF;
            LUT_tab[logic_blockID] |= cur_block | VALID_BLOCK | USED_BLOCK;
            LUT_tab[cur_block] &= (uint16_t)(~FREE_BLOCK);
        }

        cur_block++;
    }

    /* 3rd step : locate free blocks by scanning the LUT already built partially */
    free_block = 0;
    for(cur_block = 0; cur_block < NAND_ZONE_SIZE; cur_block++) {
        if(cur_block == 81) {
        }

        if(!(LUT_tab[cur_block] & USED_BLOCK)) {
            do {
                if(LUT_tab[free_block] & FREE_BLOCK) {
                    LUT_tab [cur_block] |= free_block;
                    LUT_tab [free_block] &= ~FREE_BLOCK;
                    break;
                }

                free_block++;
            } while(free_block < NAND_ZONE_SIZE);
        }
    }

    return NAND_OK;
}

/*!
    \brief      find free exchange block, according the block number(odd number select 1001, even number select 1000)
    \param[in]  odd_even: block number is odd or even
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint16_t nand_find_free_block(uint8_t odd_even)
{
    if(BLOCK_ODD == odd_even) {
        return LUT_tab[MAX_LOG_BLOCKS_PER_ZONE] & 0x03FF;
    } else if(BLOCK_EVEN == odd_even) {
        return LUT_tab[MAX_LOG_BLOCKS_PER_ZONE + 1] & 0x03FF;
    }

    return 0xFFFF;
}

/*!
    \brief      check NAND data is right
    \param[in]  pbuf: input buffer
    \param[in]  len: buffer length
    \param[in]  value: right value in buffer
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t nand_judge_buf_ok(uint8_t *pbuf, uint32_t len, uint8_t value)
{
    uint32_t i;

    for(i = 0; i < len; i++) {
        if(pbuf[i] != value) {
            return NAND_FAIL;
        }
    }

    return NAND_OK;
}

/*!
    \brief      check block is bad block
    \param[in]  blocknum: physical block number
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t nand_isbad_block(uint32_t blocknum)
{
    uint8_t flag;
    nand_address_struct addr;

    addr.zone = cur_zone;
    addr.block = blocknum;
    addr.page = 0;
    addr.page_in_offset = NAND_PAGE_SIZE + BI_OFFSET;

    exmc_nand_read_spare(&flag, addr, 1);

    if(0xFFU != flag) {
        return NAND_FAIL;
    }

    return NAND_OK;
}

/*!
    \brief      mark used block
    \param[in]  blocknum: physical block number
    \param[out] none
    \retval     NAND_OK, NAND_FAIL
*/
static uint8_t nand_mark_used_block(uint32_t blocknum)
{
    uint8_t flag;
    nand_address_struct addr;

    addr.zone = cur_zone;
    addr.block = blocknum;
    addr.page = 0;
    addr.page_in_offset = NAND_PAGE_SIZE + USED_OFFSET;

    flag = NAND_USED_BLOCK_FLAG;

    if(NAND_FAIL == exmc_nand_write_spare(&flag, addr, 1)) {
        return NAND_FAIL;
    }

    return NAND_OK;
}

/*!
    \brief      mark bad block
    \param[in]  blocknum: block number
    \param[out] none
    \retval     none
*/
static void nand_mark_bad_block(uint32_t blocknum)
{
    uint8_t flag;
    nand_address_struct addr;

    addr.zone = cur_zone;
    addr.block = blocknum;
    addr.page = 0;
    addr.page_in_offset = NAND_PAGE_SIZE + BI_OFFSET;

    flag = NAND_BAD_BLOCK_FLAG;

    if(NAND_FAIL == exmc_nand_write_spare(&flag, addr, 1)) {
        addr.page = 1;
        exmc_nand_write_spare(&flag, addr, 1);
    }
}

/*!
    \brief      mark logic block to specific block
    \param[in]  blocknum: physical block number
    \param[in]  logblock: logic block number
    \param[out] none
    \retval     none
*/
static uint8_t nand_mark_logic_block(uint32_t blocknum, uint16_t logblock)
{
    nand_address_struct addr;

    addr.zone = cur_zone;
    addr.block = blocknum;
    addr.page = 0;
    addr.page_in_offset = NAND_PAGE_SIZE + LBN0_OFFSET;

    if(NAND_FAIL == exmc_nand_write_spare((uint8_t *)&logblock, addr, 2)) {
        return NAND_FAIL;
    }

    return NAND_OK;
}
