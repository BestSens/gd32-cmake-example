/*!
    \file    exmc_norflash.h
    \brief   the header file of EXMC NOR Flash(M29W128FH) driver

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

#include "gd32f4xx.h"

#ifndef EXMC_NORFLASH_H
#define EXMC_NORFLASH_H

/* read id command define */
#define CMD_READID_ADD_1ST                     0x0555
#define CMD_READID_DATA_1ST                    0x00AA
#define CMD_READID_ADD_2ND                     0x02AA
#define CMD_READID_DATA_2ND                    0x0055
#define CMD_READID_ADD_3RD                     0x0555
#define CMD_READID_DATA_3RD                    0x0090

/* Block Erase Command define */
#define CMD_BLOCKERASE_ADD_1ST                 0x0555
#define CMD_BLOCKERASE_DATA_1ST                0x00AA
#define CMD_BLOCKERASE_ADD_2ND                 0x02AA
#define CMD_BLOCKERASE_DATA_2ND                0x0055
#define CMD_BLOCKERASE_ADD_3RD                 0x0555
#define CMD_BLOCKERASE_DATA_3RD                0x0080
#define CMD_BLOCKERASE_ADD_4TH                 0x0555
#define CMD_BLOCKERASE_DATA_4TH                0x00AA
#define CMD_BLOCKERASE_ADD_5TH                 0x02AA
#define CMD_BLOCKERASE_DATA_5TH                0x0055
#define CMD_BLOCKERASE_DATA_6TH                0x0030

/* chip erase command define */
#define CMD_CHIPERASE_ADD_1ST                  0x0555
#define CMD_CHIPERASE_DATA_1ST                 0x00AA
#define CMD_CHIPERASE_ADD_2ND                  0x02AA
#define CMD_CHIPERASE_DATA_2ND                 0x0055
#define CMD_CHIPERASE_ADD_3RD                  0x0555
#define CMD_CHIPERASE_DATA_3RD                 0x0080
#define CMD_CHIPERASE_ADD_4TH                  0x0555
#define CMD_CHIPERASE_DATA_4TH                 0x00AA
#define CMD_CHIPERASE_ADD_5TH                  0x02AA
#define CMD_CHIPERASE_DATA_5TH                 0x0055
#define CMD_CHIPERASE_ADD_6TH                  0x0555
#define CMD_CHIPERASE_DATA_6TH                 0x0010

/* reset command define */
#define CMD_RESET_ADD_1ST                      0x0555
#define CMD_RESET_DATA_1ST                     0x00AA
#define CMD_RESET_ADD_2ND                      0x02AA
#define CMD_RESET_DATA_2ND                     0x0055
#define CMD_RESET_DATA_3RD                     0x00F0

/* read command define */
#define CMD_READ_ADD_1ST                       0x0555
#define CMD_READ_DATA_1ST                      0x00AA
#define CMD_READ_ADD_2ND                       0x02AA
#define CMD_READ_DATA_2ND                      0x0055
#define CMD_READ_DATA_3RD                      0x00F0

/* write command define */
#define CMD_WRITE_ADD_1ST                      0x0555
#define CMD_WRITE_DATA_1ST                     0x00AA
#define CMD_WRITE_ADD_2ND                      0x02AA
#define CMD_WRITE_DATA_2ND                     0x0055
#define CMD_WRITE_ADD_3RD                      0x0555
#define CMD_WRITE_DATA_3RD                     0x00A0

/* max read and write address */
#define NOR_MAX_ADDRESS                        ((uint32_t)0x01000000)

/* NOR id structure */
typedef struct {
    uint16_t manufacturer_code;
    uint16_t device_code1;
    uint16_t device_code2;
    uint16_t device_code3;
} nor_id_struct;

/* NOR status */
typedef enum {
    NOR_SUCCESS = 0,
    NOR_ONGOING,
    NOR_ERROR,
    NOR_TIMEOUT
} nor_status_struct;

void exmc_norflash_init(uint32_t nor_region);
void nor_read_id(nor_id_struct *nor_id);
nor_status_struct nor_eraseblock(uint32_t blockaddr);
nor_status_struct nor_erasechip(void);
nor_status_struct nor_write_halfword(uint32_t writeaddr, uint16_t data);
nor_status_struct nor_write_buffer(uint16_t *pbuffer, uint32_t writeaddr, uint32_t halfword_count);
uint16_t nor_read_halfword(uint32_t readaddr);
void nor_readbuffer(uint16_t *pbuffer, uint32_t readaddr, uint32_t halfwordcount);
void nor_reset(void);
nor_status_struct nor_get_status(uint32_t time_out);
void nor_return_to_read_mode(void);
void nor_fill_buffer(uint16_t *pbuffer, uint16_t buffer_lenght, uint32_t value);

#endif /* EXMC_NORFLASH_H */
