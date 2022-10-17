/*!
    \file    usbd_storage_msd.c
    \brief   this file provides the disk operations functions

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

#include "usb_conf.h"
#include "sram_msd.h"
#include "usbd_msc_mem.h"

/* usb mass storage standard inquiry data */
const int8_t STORAGE_InquiryData[] = 
{
    /* LUN 0 */
    0x00,
    0x80,
    0x00,
    0x01,
    (USBD_STD_INQUIRY_LENGTH - 5U),
    0x00,
    0x00,
    0x00,
    'G', 'D', '3', '2', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
    'I', 'n', 't', 'e', 'r', 'n', 'a', 'l', /* Product      : 16 Bytes */
    ' ', 's', 'r', 'a', 'm', ' ', ' ', ' ',
    '1', '.', '0' ,'0',                     /* Version      : 4 Bytes */
};

/* local function prototypes ('static') */
static int8_t  STORAGE_Init             (uint8_t Lun);
static int8_t  STORAGE_IsReady          (uint8_t Lun);
static int8_t  STORAGE_IsWriteProtected (uint8_t Lun);
static int8_t  STORAGE_GetMaxLun        (void);
static int8_t  STORAGE_Read             (uint8_t Lun,
                                        uint8_t *buf,
                                        uint32_t BlkAddr,
                                        uint16_t BlkLen);
static int8_t  STORAGE_Write            (uint8_t Lun,
                                        uint8_t *buf,
                                        uint32_t BlkAddr,
                                        uint16_t BlkLen);

usbd_mem_cb USBD_Internal_Storage_fops = 
{
    .mem_init      = STORAGE_Init,
    .mem_ready     = STORAGE_IsReady,
    .mem_protected = STORAGE_IsWriteProtected,
    .mem_read      = STORAGE_Read,
    .mem_write     = STORAGE_Write,
    .mem_maxlun    = STORAGE_GetMaxLun,

    .mem_inquiry_data = {(uint8_t *)STORAGE_InquiryData},

    .mem_block_size   = {ISRAM_BLOCK_SIZE},
    .mem_block_len    = {ISRAM_BLOCK_NUM}
};

usbd_mem_cb *usbd_mem_fops = &USBD_Internal_Storage_fops;

/*!
    \brief      initialize the storage medium
    \param[in]  Lun: logical unit number
    \param[out] none
    \retval     status
*/
static int8_t STORAGE_Init (uint8_t Lun)
{
    return 0;
}

/*!
    \brief      check whether the medium is ready
    \param[in]  Lun: logical unit number
    \param[out] none
    \retval     status
*/
static int8_t STORAGE_IsReady (uint8_t Lun)
{
    return 0;
}

/*!
    \brief      check whether the medium is write-protected
    \param[in]  Lun: logical unit number
    \param[out] none
    \retval     status
*/
static int8_t STORAGE_IsWriteProtected (uint8_t Lun)
{
    return 0;
}

/*!
    \brief      read data from the medium
    \param[in]  Lun: logical unit number
    \param[in]  buf: pointer to the buffer to save data
    \param[in]  BlkAddr: address of 1st block to be read
    \param[in]  BlkLen: number of blocks to be read
    \param[out] none
    \retval     status
*/
static int8_t STORAGE_Read (uint8_t Lun,
                            uint8_t *buf,
                            uint32_t BlkAddr,
                            uint16_t BlkLen)
{
    if(SRAM_ReadMultiBlocks(buf,
                            BlkAddr,
                            ISRAM_BLOCK_SIZE,
                            BlkLen) != 0U)
    {
        return 1;
    }

    return 0;
}

/*!
    \brief      write data to the medium
    \param[in]  Lun: logical unit number
    \param[in]  buf: pointer to the buffer to write
    \param[in]  BlkAddr: address of 1st block to be written
    \param[in]  BlkLen: number of blocks to be write
    \param[out] none
    \retval     status
*/
static int8_t STORAGE_Write (uint8_t Lun,
                             uint8_t *buf,
                             uint32_t BlkAddr,
                             uint16_t BlkLen)
{
    if(SRAM_WriteMultiBlocks(buf,
                             BlkAddr,
                             ISRAM_BLOCK_SIZE,
                             BlkLen) != 0U)
    {
        return 1;
    }

    return 0;
}

/*!
    \brief      get number of supported logical unit
    \param[in]  none
    \param[out] none
    \retval     number of logical unit
*/
static int8_t STORAGE_GetMaxLun (void)
{
    return (MEM_LUN_NUM - 1);
}
