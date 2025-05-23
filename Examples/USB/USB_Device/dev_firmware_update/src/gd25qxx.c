/*!
    \file    gd25qxx.c
    \brief   SPI flash gd25qxx driver

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

#include "gd25qxx.h"

#define WRITE            0x02U     /* write to memory instruction */
#define QUADWRITE        0x32U     /* quad write to memory instruction */
#define WRSR             0x01U     /* write status register instruction */
#define WREN             0x06U     /* write enable instruction */

#define READ             0x03U     /* read from memory instruction */
#define QUADREAD         0x6BU     /* read from memory instruction */
#define RDSR             0x05U     /* read status register instruction */
#define RDID             0x9FU     /* read identification */
#define SE               0x20U     /* sector erase instruction */
#define BE               0x52U     /* block erase instruction */
#define CE               0xC7U     /* chip erase instruction */

#define WTSR             0x05U     /* write status register instruction */

#define WIP_FLAG         0x01U     /* write in progress(wip) flag */
#define DUMMY_BYTE       0xA5U

/*!
    \brief      initialize SPI5 GPIO and parameter
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_flash_init(void)
{
    spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(RCU_GPIOG);
    rcu_periph_clock_enable(RCU_GPIOI);
    rcu_periph_clock_enable(RCU_SPI5);

    /* SPI5_CLK(PG13), SPI5_MISO(PG12), SPI5_MOSI(PG14),SPI5_IO2(PG10) and SPI5_IO3(PG11) GPIO pin configuration */
    gpio_af_set(GPIOG, GPIO_AF_5, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14);

    /* SPI5_CS(PI8) GPIO pin configuration */
    gpio_mode_set(GPIOI, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);

    /* chip select invalid */
    SPI_FLASH_CS_HIGH();

    /* SPI5 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_1EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_32;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI5, &spi_init_struct);

    /* quad wire SPI_IO2 and SPI_IO3 pin output enable */
    spi_quad_io23_output_enable(SPI5);

    /* enable SPI5 */
    spi_enable(SPI5);
}

/*!
    \brief      erase the specified flash sector
    \param[in]  sector_addr: address of the sector to erase
    \param[out] none
    \retval     none
*/
void spi_flash_sector_erase(uint32_t sector_addr)
{
    /* send write enable instruction */
    spi_flash_write_enable();

    /* sector erase */
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();
    /* send sector erase instruction */
    spi_flash_send_byte(SE);
    /* send sector_addr high nibble address byte */
    spi_flash_send_byte((sector_addr & 0xFF0000U) >> 16U);
    /* send sector_addr medium nibble address byte */
    spi_flash_send_byte((sector_addr & 0xFF00U) >> 8U);
    /* send sector_addr low nibble address byte */
    spi_flash_send_byte(sector_addr & 0xFFU);
    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      erase the specified flash block(unit: 32KB)
    \param[in]  block_addr: address of the block to erase
    \param[out] none
    \retval     none
*/
void spi_flash_block_erase(uint32_t block_addr)
{
    /* send write enable instruction */
    spi_flash_write_enable();

    /* block erase */
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();
    /* send block erase instruction */
    spi_flash_send_byte(BE);
    /* send block_addr high nibble address byte */
    spi_flash_send_byte((block_addr & 0xFF0000U) >> 16U);
    /* send block_addr medium nibble address byte */
    spi_flash_send_byte((block_addr & 0xFF00U) >> 8U);
    /* send block_addr low nibble address byte */
    spi_flash_send_byte(block_addr & 0xFFU);
    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      erase the entire flash
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_flash_bulk_erase(void)
{
    /* send write enable instruction */
    spi_flash_write_enable();

    /* bulk erase */
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();
    /* send bulk erase instruction  */
    spi_flash_send_byte(CE);
    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      write more than one byte to the flash
    \param[in]  pbuffer: pointer to the buffer
    \param[in]  write_addr: flash's internal address to write
    \param[in]  num_byte_to_write: number of bytes to write to the flash
    \param[out] none
    \retval     none
*/
void spi_flash_page_write(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    /* enable the write access to the flash */
    spi_flash_write_enable();

    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();

    /* send "write to memory" instruction */
    spi_flash_send_byte(WRITE);
    /* send write_addr high nibble address byte to write to */
    spi_flash_send_byte((write_addr & 0xFF0000U) >> 16U);
    /* send write_addr medium nibble address byte to write to */
    spi_flash_send_byte((write_addr & 0xFF00U) >> 8U);
    /* send write_addr low nibble address byte to write to */
    spi_flash_send_byte(write_addr & 0xFFU);

    /* while there is data to be written on the flash */
    while(num_byte_to_write--) {
        /* send the current byte */
        spi_flash_send_byte(*pbuffer);
        /* point on the next byte to be written */
        pbuffer++;
    }

    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();

    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      write block of data to the flash
    \param[in]  pbuffer: pointer to the buffer
    \param[in]  write_addr: flash's internal address to write
    \param[in]  num_byte_to_write: number of bytes to write to the flash
    \param[out] none
    \retval     none
*/
void spi_flash_buffer_write(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    uint8_t num_of_page = 0U, num_of_single = 0U, addr = 0U, count = 0U, temp = 0U;

    addr          = write_addr % SPI_FLASH_PAGE_SIZE;
    count         = SPI_FLASH_PAGE_SIZE - addr;
    num_of_page   = num_byte_to_write / SPI_FLASH_PAGE_SIZE;
    num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

    /* write_addr is SPI_FLASH_PAGE_SIZE aligned */
    if(0U == addr) {
        /* num_byte_to_write < SPI_FLASH_PAGE_SIZE */
        if(0U == num_of_page) {
            spi_flash_page_write(pbuffer, write_addr, num_byte_to_write);
        } else {
            /* num_byte_to_write >= SPI_FLASH_PAGE_SIZE */
            while(num_of_page--) {
                spi_flash_page_write(pbuffer, write_addr, SPI_FLASH_PAGE_SIZE);
                write_addr += SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }
            spi_flash_page_write(pbuffer, write_addr, num_of_single);
        }
    } else {
        /* write_addr is not SPI_FLASH_PAGE_SIZE aligned */
        if(0U == num_of_page) {
            /* (num_byte_to_write + write_addr) > SPI_FLASH_PAGE_SIZE */
            if(num_of_single > count) {
                temp = num_of_single - count;
                spi_flash_page_write(pbuffer, write_addr, count);
                write_addr += count;
                pbuffer += count;
                spi_flash_page_write(pbuffer, write_addr, temp);
            } else {
                spi_flash_page_write(pbuffer, write_addr, num_byte_to_write);
            }
        } else {
            /* num_byte_to_write >= SPI_FLASH_PAGE_SIZE */
            num_byte_to_write -= count;
            num_of_page = num_byte_to_write / SPI_FLASH_PAGE_SIZE;
            num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

            spi_flash_page_write(pbuffer, write_addr, count);
            write_addr += count;
            pbuffer += count;

            while(num_of_page--) {
                spi_flash_page_write(pbuffer, write_addr, SPI_FLASH_PAGE_SIZE);
                write_addr += SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }

            if(0U != num_of_single) {
                spi_flash_page_write(pbuffer, write_addr, num_of_single);
            }
        }
    }
}

/*!
    \brief      read a block of data from the flash
    \param[in]  pbuffer: pointer to the buffer that receives the data read from the flash
    \param[in]  read_addr: flash's internal address to read from
    \param[in]  num_byte_to_read: number of bytes to read from the flash
    \param[out] none
    \retval     none
*/
void spi_flash_buffer_read(uint8_t *pbuffer, uint32_t read_addr, uint16_t num_byte_to_read)
{
    /* select the flash: chip slect low */
    SPI_FLASH_CS_LOW();

    /* send "read from memory" instruction */
    spi_flash_send_byte(READ);

    /* send read_addr high nibble address byte to read from */
    spi_flash_send_byte((read_addr & 0xFF0000U) >> 16U);
    /* send read_addr medium nibble address byte to read from */
    spi_flash_send_byte((read_addr & 0xFF00U) >> 8U);
    /* send read_addr low nibble address byte to read from */
    spi_flash_send_byte(read_addr & 0xFFU);

    /* while there is data to be read */
    while(num_byte_to_read--) {
        /* read a byte from the flash */
        *pbuffer = spi_flash_send_byte(DUMMY_BYTE);
        /* point to the next location where the byte read will be saved */
        pbuffer++;
    }

    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
}

/*!
    \brief      read flash identification
    \param[in]  none
    \param[out] none
    \retval     flash identification
*/
uint32_t spi_flash_read_id(void)
{
    uint32_t temp = 0U, temp0 = 0U, temp1 = 0U, temp2 = 0U;

    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();

    /* send "RDID" instruction */
    spi_flash_send_byte(RDID);

    /* read a byte from the flash */
    temp0 = spi_flash_send_byte(DUMMY_BYTE);

    /* read a byte from the flash */
    temp1 = spi_flash_send_byte(DUMMY_BYTE);

    /* read a byte from the flash */
    temp2 = spi_flash_send_byte(DUMMY_BYTE);

    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();

    temp = (temp0 << 16U) | (temp1 << 8U) | temp2;

    return temp;
}

/*!
    \brief      start a read data byte (read) sequence from the flash
    \param[in]  read_addr: flash's internal address to read from
    \param[out] none
    \retval     none
*/
void spi_flash_start_read_sequence(uint32_t read_addr)
{
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();

    /* send "read from memory" instruction */
    spi_flash_send_byte(READ);

    /* send the 24-bit address of the address to read from */
    /* send read_addr high nibble address byte */
    spi_flash_send_byte((read_addr & 0xFF0000U) >> 16U);
    /* send read_addr medium nibble address byte */
    spi_flash_send_byte((read_addr & 0xFF00U) >> 8U);
    /* send read_addr low nibble address byte */
    spi_flash_send_byte(read_addr & 0xFFU);
}

/*!
    \brief      read a byte from the SPI flash
    \param[in]  none
    \param[out] none
    \retval     byte read from the SPI flash
*/
uint8_t spi_flash_read_byte(void)
{
    return(spi_flash_send_byte(DUMMY_BYTE));
}

/*!
    \brief      send a byte through the SPI interface and return the byte received from the SPI bus
    \param[in]  byte: byte to send
    \param[out] none
    \retval     the value of the received byte
*/
uint8_t spi_flash_send_byte(uint8_t byte)
{
    /* loop while data register in not empty */
    while(RESET == spi_i2s_flag_get(SPI5, SPI_FLAG_TBE));

    /* send byte through the SPI5 peripheral */
    spi_i2s_data_transmit(SPI5, byte);

    /* wait to receive a byte */
    while(RESET == spi_i2s_flag_get(SPI5, SPI_FLAG_RBNE));

    /* return the byte read from the SPI bus */
    return(spi_i2s_data_receive(SPI5));
}

/*!
    \brief      send a half word through the SPI interface and return the half word received from the SPI bus
    \param[in]  half_word: half word to send
    \param[out] none
    \retval     the value of the received byte
*/
uint16_t spi_flash_send_halfword(uint16_t half_word)
{
    /* loop while data register in not empty */
    while(RESET == spi_i2s_flag_get(SPI5, SPI_FLAG_TBE));

    /* send half word through the SPI5 peripheral */
    spi_i2s_data_transmit(SPI5, half_word);

    /* wait to receive a half word */
    while(RESET == spi_i2s_flag_get(SPI5, SPI_FLAG_RBNE));

    /* return the half word read from the SPI bus */
    return spi_i2s_data_receive(SPI5);
}

/*!
    \brief      enable the write access to the flash
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_flash_write_enable(void)
{
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();

    /* send "write enable" instruction */
    spi_flash_send_byte(WREN);

    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
}

/*!
    \brief      poll the status of the write in progress(wip) flag in the flash's status register
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_flash_wait_for_write_end(void)
{
    uint8_t flash_status = 0U;

    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();

    /* send "read status register" instruction */
    spi_flash_send_byte(RDSR);

    /* loop as long as the memory is busy with a write cycle */
    do {
        /* send a dummy byte to generate the clock needed by the flash
        and put the value of the status register in flash_status variable */
        flash_status = spi_flash_send_byte(DUMMY_BYTE);
    } while(SET == (flash_status & WIP_FLAG));

    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
}

/*!
    \brief      enable the flash quad mode
    \param[in]  none
    \param[out] none
    \retval     none
*/
void spi_quad_flash_quad_enable(void)
{
    /* enable the write access to the flash */
    spi_flash_write_enable();
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();
    /* send "write status register" instruction */
    spi_flash_send_byte(WRSR);

    spi_flash_send_byte(0x00U);
    spi_flash_send_byte(0x02U);
    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      write block of data to the flash using spi quad
    \param[in]  pbuffer : pointer to the buffer
    \param[in]  write_addr : flash's internal address to write to
    \param[in]  num_byte_to_write : number of bytes to write to the flash
    \param[out] none
    \retval     none
*/
void spi_quad_flash_buffer_write(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    uint8_t num_of_page = 0U, num_of_single = 0U, addr = 0U, count = 0U, temp = 0U;

    addr = write_addr % SPI_FLASH_PAGE_SIZE;
    count = SPI_FLASH_PAGE_SIZE - addr;
    num_of_page =  num_byte_to_write / SPI_FLASH_PAGE_SIZE;
    num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;
    /* write_addr is SPI_FLASH_PAGE_SIZE aligned */
    if(0U == addr) {
        /* num_byte_to_write < SPI_FLASH_PAGE_SIZE */
        if(0U == num_of_page) {
            spi_quad_flash_page_write(pbuffer, write_addr, num_byte_to_write);
        } else {
            /* num_byte_to_write >= SPI_FLASH_PAGE_SIZE */
            while(num_of_page--) {
                spi_quad_flash_page_write(pbuffer, write_addr, SPI_FLASH_PAGE_SIZE);
                write_addr +=  SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }
            spi_quad_flash_page_write(pbuffer, write_addr, num_of_single);
        }
    } else {
        /* write_addr is not SPI_FLASH_PAGE_SIZE aligned */
        if(0U == num_of_page) {
            /* (num_byte_to_write + write_addr) > SPI_FLASH_PAGE_SIZE */
            if(num_of_single > count) {
                temp = num_of_single - count;
                spi_quad_flash_page_write(pbuffer, write_addr, count);
                write_addr +=  count;
                pbuffer += count;
                spi_quad_flash_page_write(pbuffer, write_addr, temp);
            } else {
                spi_quad_flash_page_write(pbuffer, write_addr, num_byte_to_write);
            }
        } else {
            /* num_byte_to_write >= SPI_FLASH_PAGE_SIZE */
            num_byte_to_write -= count;
            num_of_page =  num_byte_to_write / SPI_FLASH_PAGE_SIZE;
            num_of_single = num_byte_to_write % SPI_FLASH_PAGE_SIZE;

            spi_quad_flash_page_write(pbuffer, write_addr, count);
            write_addr +=  count;
            pbuffer += count;

            while(num_of_page--) {
                spi_quad_flash_page_write(pbuffer, write_addr, SPI_FLASH_PAGE_SIZE);
                write_addr +=  SPI_FLASH_PAGE_SIZE;
                pbuffer += SPI_FLASH_PAGE_SIZE;
            }

            if(0U != num_of_single) {
                spi_quad_flash_page_write(pbuffer, write_addr, num_of_single);
            }
        }
    }
}

/*!
    \brief      read a block of data from the flash using spi quad
    \param[in]  pbuffer : pointer to the buffer that receives the data read from the flash
    \param[in]  read_addr : flash's internal address to read from
    \param[in]  num_byte_to_read : number of bytes to read from the flash
    \param[out] none
    \retval     none
*/
void spi_quad_flash_buffer_read(uint8_t *pbuffer, uint32_t read_addr, uint16_t num_byte_to_read)
{
    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();
    /* send "quad fast read from memory " instruction */
    spi_flash_send_byte(QUADREAD);

    /* send read_addr high nibble address byte to read from */
    spi_flash_send_byte((read_addr & 0xFF0000U) >> 16U);
    /* send read_addr medium nibble address byte to read from */
    spi_flash_send_byte((read_addr & 0xFF00U) >> 8U);
    /* send read_addr low nibble address byte to read from */
    spi_flash_send_byte(read_addr & 0xFFU);

    /* enable the spi quad */
    spi_quad_enable(SPI5);
    /* enable the spi quad read operation */
    spi_quad_read_enable(SPI5);

    spi_flash_send_byte(0xA5U);
    spi_flash_send_byte(0xA5U);
    spi_flash_send_byte(0xA5U);
    spi_flash_send_byte(0xA5U);

    /* while there is data to be read */
    while(num_byte_to_read--) {
        /* read a byte from the flash */
        *pbuffer = spi_flash_send_byte(DUMMY_BYTE);
        /* point to the next location where the byte read will be saved */
        pbuffer++;
    }
    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
    /* disable the spi quad */
    spi_quad_disable(SPI5);
    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}

/*!
    \brief      write more than one byte to the flash using spi quad
    \param[in]  pbuffer : pointer to the buffer
    \param[in]  write_addr : flash's internal address to write to
    \param[in]  num_byte_to_write : number of bytes to write to the flash
    \param[out] none
    \retval     none
*/
void spi_quad_flash_page_write(uint8_t *pbuffer, uint32_t write_addr, uint16_t num_byte_to_write)
{
    /* enable the flash quad mode */
    spi_quad_flash_quad_enable();
    /* enable the write access to the flash */
    spi_flash_write_enable();

    /* select the flash: chip select low */
    SPI_FLASH_CS_LOW();
    /* send "quad write to memory " instruction */
    spi_flash_send_byte(QUADWRITE);
    /* send writeaddr high nibble address byte to write to */
    spi_flash_send_byte((write_addr & 0xFF0000U) >> 16U);
    /* send writeaddr medium nibble address byte to write to */
    spi_flash_send_byte((write_addr & 0xFF00U) >> 8U);
    /* send writeaddr low nibble address byte to write to */
    spi_flash_send_byte(write_addr & 0xFFU);
    /* enable the spi quad */
    spi_quad_enable(SPI5);
    /* enable the spi quad write operation */
    spi_quad_write_enable(SPI5);

    /* while there is data to be written on the flash */
    while(num_byte_to_write--) {
        /* send the current byte */
        spi_flash_send_byte(*pbuffer);
        /* point on the next byte to be written */
        pbuffer++;
    }

    /* deselect the flash: chip select high */
    SPI_FLASH_CS_HIGH();
    /* disable the spi quad function */
    spi_quad_disable(SPI5);
    /* wait the end of flash writing */
    spi_flash_wait_for_write_end();
}
