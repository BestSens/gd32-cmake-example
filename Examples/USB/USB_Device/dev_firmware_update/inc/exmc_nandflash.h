/*!
    \file    exmc_nandflash.h
    \brief   the header file of nandflash(GD9FU1G8F2AMG) driver

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

#ifndef EXMC_NANDFLASH_H
#define EXMC_NANDFLASH_H

#include "gd32f4xx.h"
#include "string.h"

/* NAND area definition */
/* A16 = CLE high command area */
#define EXMC_CMD_AREA                 (uint32_t)(1U << 16U)
/* A17 = ALE high address area */
#define EXMC_ADDR_AREA                (uint32_t)(1U << 17U)
/* data area */
#define EXMC_DATA_AREA                ((uint32_t)0x00000000U)

/* NAND memory command (hynix GD9FU1G8F2AMG) */
#define NAND_CMD_READ1_1ST            ((uint8_t)0x00U)
#define NAND_CMD_READ1_2ND            ((uint8_t)0x30U)
#define NAND_CMD_WRITE_1ST            ((uint8_t)0x80U)
#define NAND_CMD_WRITE_2ND            ((uint8_t)0x10U)
#define NAND_CMD_ERASE_1ST            ((uint8_t)0x60U)
#define NAND_CMD_ERASE_2ND            ((uint8_t)0xD0U)
#define NAND_CMD_READID               ((uint8_t)0x90U)
#define NAND_CMD_STATUS               ((uint8_t)0x70U)
#define NAND_CMD_LOCK_STATUS          ((uint8_t)0x7AU)
#define NAND_CMD_RESET                ((uint8_t)0xFFU)

/* NAND memory status */
#define NAND_BUSY                     ((uint8_t)0x00U)
#define NAND_ERROR                    ((uint8_t)0x01U)
#define NAND_READY                    ((uint8_t)0x40U)
#define NAND_TIMEOUT_ERROR            ((uint8_t)0x80U)

/* NAND memory parameters */
#define NAND_ZONE_COUNT               ((uint16_t)0x0001U) /* NAND zone count */
#define NAND_PAGE_SIZE                ((uint16_t)0x0800U) /* 2 * 1024 bytes per page */
#define NAND_BLOCK_SIZE               ((uint16_t)0x0040U) /* 64 pages per block */
#define NAND_ZONE_SIZE                ((uint16_t)0x0400U) /* 1024 block per zone */
#define NAND_SPARE_AREA_SIZE          ((uint16_t)0x0080U) /* last 128 bytes as spare area */

#define NAND_BLOCK_COUNT              1024 /* block count */
#define NAND_PAGE_TOTAL_SIZE          (NAND_PAGE_SIZE + NAND_SPARE_AREA_SIZE) /* total page size = page size + spare are size */

/* command code type define */
#define NAND_CMD_COPYBACK_A           ((uint8_t)0x00U) /* PAGE COPY-BACK command sequence */
#define NAND_CMD_COPYBACK_B           ((uint8_t)0x35U)
#define NAND_CMD_COPYBACK_C           ((uint8_t)0x85U)
#define NAND_CMD_COPYBACK_D           ((uint8_t)0x10U)

#define NAND_CMD_STATUS               ((uint8_t)0x70U) /* Read NAND Flash status */

#define MAX_PHY_BLOCKS_PER_ZONE       1024U
#define MAX_LOG_BLOCKS_PER_ZONE       1000U

#define NAND_BAD_BLOCK_FLAG           0x00U
#define NAND_USED_BLOCK_FLAG          0xFEU

#define BI_OFFSET                     0U
#define USED_OFFSET                   1U
#define LBN0_OFFSET                   2U
#define LBN1_OFFSET                   3U
#define VALID_SPARE_SIZE              4U

#define NAND_OK                       0U
#define NAND_FAIL                     1U

#define BLOCK_ODD                     0U
#define BLOCK_EVEN                    1U

#define FREE_BLOCK                    (1U << 12U)
#define BAD_BLOCK                     (1U << 13U)
#define VALID_BLOCK                   (1U << 14U)
#define USED_BLOCK                    (1U << 15U)

#define EXCHANGE_BLOCKS_NUM           2U

#define BAD_BALOK_TEST_CYCLE          5U
#define DATA_BLOCK_PERCENT            98U

#define ADDR_1ST_CYCLE(ADDR)          (uint8_t)((ADDR)& 0xFFU)                /* 1st addressing cycle */
#define ADDR_2ND_CYCLE(ADDR)          (uint8_t)(((ADDR)& 0xFF00U) >> 8U)      /* 2nd addressing cycle */
#define ADDR_3RD_CYCLE(ADDR)          (uint8_t)(((ADDR)& 0xFF0000U) >> 16U)   /* 3rd addressing cycle */
#define ADDR_4TH_CYCLE(ADDR)          (uint8_t)(((ADDR)& 0xFF000000U) >> 24U) /* 4th addressing cycle */

/* NAND id structure */
typedef struct {
    uint8_t maker_id;
    uint8_t device_id;
    uint8_t third_id;
    uint8_t fourth_id;
} nand_id_struct;

/* NAND address structure */
typedef struct {
    uint16_t zone;
    uint16_t block;
    uint16_t page;
    uint16_t page_in_offset;
} nand_address_struct;

/* function declarations */
/* nand flash peripheral initialize */
uint8_t exmc_nandflash_init(uint32_t nand_bank);
/* read NAND flash ID */
void nand_read_id(nand_id_struct *nand_id);
/* write data to nand flash */
uint8_t nand_write(uint32_t memaddr, uint8_t *pwritebuf, uint16_t bytecount);
/* read data from nand flash */
uint8_t nand_read(uint32_t mem_addr, uint8_t *preadbuf, uint16_t bytecount);
/* scan and test specific block */
uint8_t nand_scan_block(uint32_t pagenum);
/* check block is free block */
uint8_t nand_judge_free_block(uint32_t blocknum);

#endif /* EXMC_NANDFLASH_H */
