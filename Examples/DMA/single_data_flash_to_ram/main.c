/*!
    \file    main.c
    \brief   DMA transfer data from FLASH to RAM

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

#include "gd32f4xx.h"
#include <stdio.h>
#include "gd32f450i_eval.h"
#include "gd32f4xx_dma.h"

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

/* data buffer size */
#define BUFFER_SIZE     (256U)

/* the flash address of source data */
#define FLASH_ADDRESS   (0x08004000)

static void fill_buffer_size(void);
uint32_t fmc_sector_get(uint32_t address);
void fmc_write_32bit_data(uint32_t address, uint16_t length, int32_t *data_32);
static ErrStatus memory_compare(int32_t *src, int32_t *dst, uint16_t length);

int32_t source[BUFFER_SIZE];
int32_t destination[BUFFER_SIZE];
ErrStatus status;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    dma_single_data_parameter_struct  dma_init_parameter;

    /* LED initialize */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED3);

    /* peripheral clock enable */
    rcu_periph_clock_enable(RCU_DMA1);

    /* DMA peripheral configure */
    dma_deinit(DMA1, DMA_CH0);

    dma_init_parameter.periph_addr = (uint32_t)FLASH_ADDRESS;
    dma_init_parameter.periph_inc = DMA_PERIPH_INCREASE_ENABLE;
    dma_init_parameter.memory0_addr = (uint32_t)destination;
    dma_init_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_parameter.periph_memory_width = DMA_PERIPH_WIDTH_32BIT;
    dma_init_parameter.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_init_parameter.direction = DMA_MEMORY_TO_MEMORY;
    dma_init_parameter.number = BUFFER_SIZE;
    dma_init_parameter.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(DMA1, DMA_CH0, &dma_init_parameter);

    fill_buffer_size();

    fmc_write_32bit_data(FLASH_ADDRESS, BUFFER_SIZE, source);

    dma_channel_enable(DMA1, DMA_CH0);

    while(!dma_flag_get(DMA1, DMA_CH0, DMA_CHXCTL_FTFIE));

    status = memory_compare(source, destination, BUFFER_SIZE);

    if(SUCCESS == status) {
        gd_eval_led_on(LED1);
    } else {
        gd_eval_led_on(LED3);
    }

    while(1) {
    }
}

/*!
    \brief      fill the buffer with specified value
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fill_buffer_size(void)
{
    uint32_t i;
    for(i = 0; i < BUFFER_SIZE; i++) {
        source[i] = i;
    }
}

/*!
    \brief      memory compare function
    \param[in]  src : source data
    \param[in]  dst : destination data
    \param[in]  length : the compare data length
    \param[out] none
    \retval     ErrStatus : ERROR or SUCCESS
*/
ErrStatus memory_compare(int32_t *src, int32_t *dst, uint16_t length)
{
    while(length--) {
        if(*src++ != *dst++) {
            return ERROR;
        }
    }
    return SUCCESS;
}

/*!
    \brief      gets the sector of a given address
    \param[in]  address: a given address(0x08000000~0x080FFFFF)
    \param[out] none
    \retval     the sector of a given address
*/
uint32_t fmc_sector_get(uint32_t address)
{
    uint32_t sector = 0;
    if((address < ADDR_FMC_SECTOR_1) && (address >= ADDR_FMC_SECTOR_0)) {
        sector = CTL_SECTOR_NUMBER_0;
    } else if((address < ADDR_FMC_SECTOR_2) && (address >= ADDR_FMC_SECTOR_1)) {
        sector = CTL_SECTOR_NUMBER_1;
    } else if((address < ADDR_FMC_SECTOR_3) && (address >= ADDR_FMC_SECTOR_2)) {
        sector = CTL_SECTOR_NUMBER_2;
    } else if((address < ADDR_FMC_SECTOR_4) && (address >= ADDR_FMC_SECTOR_3)) {
        sector = CTL_SECTOR_NUMBER_3;
    } else if((address < ADDR_FMC_SECTOR_5) && (address >= ADDR_FMC_SECTOR_4)) {
        sector = CTL_SECTOR_NUMBER_4;
    } else if((address < ADDR_FMC_SECTOR_6) && (address >= ADDR_FMC_SECTOR_5)) {
        sector = CTL_SECTOR_NUMBER_5;
    } else if((address < ADDR_FMC_SECTOR_7) && (address >= ADDR_FMC_SECTOR_6)) {
        sector = CTL_SECTOR_NUMBER_6;
    } else if((address < ADDR_FMC_SECTOR_8) && (address >= ADDR_FMC_SECTOR_7)) {
        sector = CTL_SECTOR_NUMBER_7;
    } else if((address < ADDR_FMC_SECTOR_9) && (address >= ADDR_FMC_SECTOR_8)) {
        sector = CTL_SECTOR_NUMBER_8;
    } else if((address < ADDR_FMC_SECTOR_10) && (address >= ADDR_FMC_SECTOR_9)) {
        sector = CTL_SECTOR_NUMBER_9;
    } else if((address < ADDR_FMC_SECTOR_11) && (address >= ADDR_FMC_SECTOR_10)) {
        sector = CTL_SECTOR_NUMBER_10;
    } else {
        sector = CTL_SECTOR_NUMBER_11;
    }
    return sector;
}

/*!
    \brief      write 32 bit length data to a given address
    \param[in]  address: a given address(0x08000000~0x080FFFFF)
    \param[in]  length: data length
    \param[in]  data_32: data pointer
    \param[out] none
    \retval     none
*/
void fmc_write_32bit_data(uint32_t address, uint16_t length, int32_t *data_32)
{
    uint16_t StartSector, EndSector, i;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* get the number of the start and end sectors */
    StartSector = fmc_sector_get(address);
    EndSector = fmc_sector_get(address + 4 * length);
    /* each time the sector number increased by 8, refer to the sector definition */
    for(i = StartSector; i <= EndSector; i += 8) {
        if(FMC_READY != fmc_sector_erase(i)) {
            while(1) {
            }
        }
    }
    /* write data_32 to the corresponding address */
    for(i = 0; i < length; i++) {
        if(FMC_READY == fmc_word_program(address, data_32[i])) {
            address = address + 4;
        } else {
            while(1) {
            }
        }
    }
    /* lock the flash program erase controller */
    fmc_lock();
}

/*!
    \brief      read 32 bit length data from a given address
    \param[in]  address: a given address(0x08000000~0x080FFFFF)
    \param[in]  length: data length
    \param[in]  data_32: data pointer
    \param[out] none
    \retval     none
*/
void fmc_read_32bit_data(uint32_t address, uint16_t length, int32_t *data_32)
{
    uint8_t i;
    for(i = 0; i < length; i++) {
        data_32[i] = *(__IO int32_t *)address;
        address = address + 4;
    }
}
