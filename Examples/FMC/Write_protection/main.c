/*!
    \file    main.c
    \brief   main flash sectors write protection
    
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
#include "main.h"
#include "gd32f450i_eval.h"

#define WRITE_PROTECTION_DISABLE 
//#define WRITE_PROTECTION_ENABLE

int32_t data_32_1[] = {-1,-2,-3,-4,-5,-6,-7,-8,-9,-10};
int32_t data_32_2[10];

/*!
    \brief      gets the sector of a given address
    \param[in]  address: a given address(0x08000000~0x080FFFFF)
    \param[out] none
    \retval     the sector of a given address
*/
uint32_t fmc_sector_get(uint32_t address)
{
    uint32_t sector = 0;
    if((address < ADDR_FMC_SECTOR_1) && (address >= ADDR_FMC_SECTOR_0)){
        sector = CTL_SECTOR_NUMBER_0;
    }else if((address < ADDR_FMC_SECTOR_2) && (address >= ADDR_FMC_SECTOR_1)){
        sector = CTL_SECTOR_NUMBER_1;
    }else if((address < ADDR_FMC_SECTOR_3) && (address >= ADDR_FMC_SECTOR_2)){
        sector = CTL_SECTOR_NUMBER_2;
    }else if((address < ADDR_FMC_SECTOR_4) && (address >= ADDR_FMC_SECTOR_3)){
        sector = CTL_SECTOR_NUMBER_3;  
    }else if((address < ADDR_FMC_SECTOR_5) && (address >= ADDR_FMC_SECTOR_4)){
        sector = CTL_SECTOR_NUMBER_4;
    }else if((address < ADDR_FMC_SECTOR_6) && (address >= ADDR_FMC_SECTOR_5)){
        sector = CTL_SECTOR_NUMBER_5;
    }else if((address < ADDR_FMC_SECTOR_7) && (address >= ADDR_FMC_SECTOR_6)){
        sector = CTL_SECTOR_NUMBER_6;
    }else if((address < ADDR_FMC_SECTOR_8) && (address >= ADDR_FMC_SECTOR_7)){
        sector = CTL_SECTOR_NUMBER_7;  
    }else if((address < ADDR_FMC_SECTOR_9) && (address >= ADDR_FMC_SECTOR_8)){
        sector = CTL_SECTOR_NUMBER_8;
    }else if((address < ADDR_FMC_SECTOR_10) && (address >= ADDR_FMC_SECTOR_9)){
        sector = CTL_SECTOR_NUMBER_9;
    }else if((address < ADDR_FMC_SECTOR_11) && (address >= ADDR_FMC_SECTOR_10)){
        sector = CTL_SECTOR_NUMBER_10;
    }else{
        sector = CTL_SECTOR_NUMBER_11;
    }
    return sector;
}

/*!
    \brief      erases the sector of a given sector number
    \param[in]  fmc_sector: a given sector number
      \arg        CTL_SECTOR_NUMBER_0: sector 0
      \arg        CTL_SECTOR_NUMBER_1: sector 1
      \arg        CTL_SECTOR_NUMBER_2: sector 2
      \arg        CTL_SECTOR_NUMBER_3:sector 3
      \arg        CTL_SECTOR_NUMBER_4: sector 4
      \arg        CTL_SECTOR_NUMBER_5: sector 5
      \arg        CTL_SECTOR_NUMBER_6: sector 6
      \arg        CTL_SECTOR_NUMBER_7:sector 7
      \arg        CTL_SECTOR_NUMBER_8: sector 8
      \arg        CTL_SECTOR_NUMBER_9: sector 9
      \arg        CTL_SECTOR_NUMBER_10: sector 10
      \arg        CTL_SECTOR_NUMBER_11:sector 11
    \param[out] none
    \retval     none
*/
void fmc_erase_sector(uint32_t fmc_sector)
{
    /* unlock the flash program erase controller */
    fmc_unlock(); 
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* wait the erase operation complete*/
    if(FMC_READY != fmc_sector_erase(fmc_sector)){
        while(1);
    }
    /* lock the flash program erase controller */
    fmc_lock();
}

/*!
    \brief      write 32 bit length data to a given address
    \param[in]  address: a given address(0x08000000~0x080FFFFF)
    \param[in]  length: data length
    \param[in]  data_32: data pointer
    \param[out] none
    \retval     none
*/
void fmc_write_32bit_data(uint32_t address, uint16_t length, int32_t* data_32)
{
    uint16_t StartSector, EndSector,i;
    /* unlock the flash program erase controller */
    fmc_unlock();
    /* clear pending flags */
    fmc_flag_clear(FMC_FLAG_END | FMC_FLAG_OPERR | FMC_FLAG_WPERR | FMC_FLAG_PGMERR | FMC_FLAG_PGSERR);
    /* get the number of the start and end sectors */
    StartSector = fmc_sector_get(address);
    EndSector = fmc_sector_get(address + 4*length);
    /* each time the sector number increased by 8, refer to the sector definition */
    for(i = StartSector; i <= EndSector; i += 8){
        if(FMC_READY != fmc_sector_erase(i)){
            while(1);
        }
    }
    /* write data_32 to the corresponding address */
    for(i=0; i<length; i++){
        if(FMC_READY == fmc_word_program(address, data_32[i])){
            address = address + 4;
        }else{ 
            while(1);
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
void fmc_read_32bit_data(uint32_t address, uint16_t length, int32_t* data_32)
{
    uint8_t i;
    for(i=0; i<length; i++){
        data_32[i] = *(__IO int32_t*)address;
        address=address + 4;
    }
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint8_t j;
    /* enable USART0 clock */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_USART0);
    /* configure COM port */
    gd_eval_com_init(EVAL_COM0);
    printf("\r\n 32 bit data :");
    printf("\r\n erase before:");
    /* read 32 bit length data from a given address */
    fmc_read_32bit_data(FLASH_USER_START_ADDR, 10, data_32_2);
    for(j=0; j<10; j++){
        printf(" %d ", data_32_2[j]);
    }
#ifdef WRITE_PROTECTION_DISABLE 
    fmc_unlock();
    ob_unlock();
    ob_write_protection_disable(OB_WP_1);
    ob_start();
    ob_lock();
    fmc_lock();
    /* erases the sector of a given sector number */
    fmc_erase_sector(CTL_SECTOR_NUMBER_1);
    printf("\r\n erase after :");
    /* read 32 bit length data from a given address */
    fmc_read_32bit_data(FLASH_USER_START_ADDR, 10, data_32_2);
    for(j=0; j<10; j++){
        printf(" %d ", data_32_2[j]);
    }
    printf("\r\n write after :");
    /* write 32 bit length data to a given address */
    fmc_write_32bit_data(FLASH_USER_START_ADDR, 10, data_32_1);
    /* read 32 bit length data from a given address */
    fmc_read_32bit_data(FLASH_USER_START_ADDR, 10, data_32_2);
    for(j=0; j<10; j++){
        printf(" %d ", data_32_2[j]);
    }
#elif defined WRITE_PROTECTION_ENABLE
    fmc_unlock();
    ob_unlock();
    ob_write_protection_enable(OB_WP_1);
    ob_start();
    ob_lock();
    fmc_lock();
    /* erases the sector of a given sector number */
    fmc_erase_sector(CTL_SECTOR_NUMBER_1);
    printf("\r\n erase after :");
    /* read 32 bit length data from a given address */
    fmc_read_32bit_data(FLASH_USER_START_ADDR, 10, data_32_2);
    for(j=0; j<10; j++){
        printf(" %d ", data_32_2[j]);
    }
    printf("\r\n write after :");
    /* write 32 bit length data to a given address */
    fmc_write_32bit_data(FLASH_USER_START_ADDR, 10, data_32_1);
    /* read 32 bit length data from a given address */
    fmc_read_32bit_data(FLASH_USER_START_ADDR, 10, data_32_2);
    for(j=0; j<10; j++){
        printf(" %d ", data_32_2[j]);
    }
#endif 
    while(1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TBE));
    return ch;
}
