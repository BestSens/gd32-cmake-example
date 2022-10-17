/*!
    \file    at24cxx.c
    \brief   the read and write function file

    \version 2022-01-15, V1.0.0, firmware for GD32F4xx
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

#include "at24cxx.h"
#include "i2c.h"
#include <stdio.h>

#define EEPROM_BLOCK0_ADDRESS    0xA0
#define BUFFER_SIZE              256

uint16_t eeprom_address;

/*!
    \brief      I2C read and write functions
    \param[in]  none
    \param[out] none
    \retval     I2C_OK or I2C_FAIL
*/
uint8_t i2c_24c02_test(void)
{
    uint16_t i;
    uint8_t i2c_buffer_write[BUFFER_SIZE];
    uint8_t i2c_buffer_read[BUFFER_SIZE];

    printf("\r\nAT24C02 writing...\r\n");

    /* initialize i2c_buffer_write */
    for(i = 0; i < BUFFER_SIZE; i++) {
        i2c_buffer_write[i] = i;
        printf("0x%02X ", i2c_buffer_write[i]);
        if(15 == i % 16) {
            printf("\r\n");
        }
    }
    /* EEPROM data write */
    eeprom_buffer_write_timeout(i2c_buffer_write, EEP_FIRST_PAGE, BUFFER_SIZE);
    printf("AT24C02 reading...\r\n");
    /* EEPROM data read */
    eeprom_buffer_read_timeout(i2c_buffer_read, EEP_FIRST_PAGE, BUFFER_SIZE);
    /* compare the read buffer and write buffer */
    for(i = 0; i < BUFFER_SIZE; i++) {
        if(i2c_buffer_read[i] != i2c_buffer_write[i]) {
            printf("0x%02X ", i2c_buffer_read[i]);
            printf("Err:data read and write aren't matching.\n\r");
            return I2C_FAIL;
        }
        printf("0x%02X ", i2c_buffer_read[i]);
        if(15 == i % 16) {
            printf("\r\n");
        }
    }
    printf("I2C-AT24C02 test passed!\n\r");
    return I2C_OK;
}

/*!
    \brief      initialize peripherals used by the I2C EEPROM driver
    \param[in]  none
    \param[out] none
    \retval     none
*/
void i2c_eeprom_init(void)
{
    eeprom_address = EEPROM_BLOCK0_ADDRESS;
}

/*!
    \brief      write one byte to the EEPROM and use timeout function
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[out] none
    \retval     none
*/
uint8_t eeprom_byte_write_timeout(uint8_t *p_buffer, uint8_t write_address)
{
    uint8_t state = I2C_START;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    /* enable acknowledge */
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
    while(!(i2c_timeout_flag)) {
        switch(state) {
        case I2C_START:
            /* i2c master sends start signal only when the bus is idle */
            while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_start_on_bus(I2CX);
                timeout = 0;
                state = I2C_SEND_ADDRESS;
            } else {
                timeout = 0;
                state   = I2C_START;
                printf("i2c bus is busy in WRITE BYTE!\n");
            }
            break;
        case I2C_SEND_ADDRESS:
            /* i2c master sends START signal successfully */
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);
                timeout = 0;
                state = I2C_CLEAR_ADDRESS_FLAG;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends start signal timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:
            /* address flag set means i2c slave sends ACK */
            while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                timeout = 0;
                state = I2C_TRANSMIT_DATA;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master clears address flag timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_TRANSMIT_DATA:
            /* wait until the transmit data buffer is empty */
            while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                /* send the EEPROM's internal address to write to : only one byte address */
                i2c_data_transmit(I2CX, write_address);
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }

            /* wait until BTC bit is set */
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                /* send the EEPROM's internal address to write to : only one byte address */
                i2c_data_transmit(I2CX, *p_buffer);
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }
            /* wait until BTC bit is set */
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                state = I2C_STOP;
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE BYTE!\n");
            }
            break;
        case I2C_STOP:
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2CX);
            /* i2c master sends STOP signal successfully */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
                state = I2C_END;
                i2c_timeout_flag = I2C_OK;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends stop signal timeout in WRITE BYTE!\n");
            }
            break;
        default:
            state = I2C_START;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf("i2c master sends start signal in WRITE BYTE.\n");
            break;
        }
    }
    return I2C_END;
}

/*!
    \brief      write more than one byte to the EEPROM with a single write cycle
    \param[in]  p_buffer: pointer to the buffer containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
uint8_t eeprom_page_write_timeout(uint8_t *p_buffer, uint8_t write_address, uint8_t number_of_byte)
{
    uint8_t state = I2C_START;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    /* enable acknowledge */
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
    while(!(i2c_timeout_flag)) {
        switch(state) {
        case I2C_START:
            /* i2c master sends start signal only when the bus is idle */
            while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_start_on_bus(I2CX);
                timeout = 0;
                state = I2C_SEND_ADDRESS;
            } else {
                i2c_bus_reset();
                timeout = 0;
                state = I2C_START;
                printf("i2c bus is busy in WRITE!\n");
            }
            break;
        case I2C_SEND_ADDRESS:
            /* i2c master sends START signal successfully */
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);
                timeout = 0;
                state = I2C_CLEAR_ADDRESS_FLAG;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends start signal timeout in WRITE!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:
            /* address flag set means i2c slave sends ACK */
            while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                timeout = 0;
                state = I2C_TRANSMIT_DATA;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master clears address flag timeout in WRITE!\n");
            }
            break;
        case I2C_TRANSMIT_DATA:
            /* wait until the transmit data buffer is empty */
            while((!i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                /* send the EEPROM's internal address to write to : only one byte address */
                i2c_data_transmit(I2CX, write_address);
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends EEPROM's internal address timeout in WRITE!\n");
            }
            /* wait until BTC bit is set */
            while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends data timeout in WRITE!\n");
            }
            while(number_of_byte--) {
                i2c_data_transmit(I2CX, *p_buffer);
                /* point to the next byte to be written */
                p_buffer++;
                /* wait until BTC bit is set */
                while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    printf("i2c master sends data timeout in WRITE!\n");
                }
            }
            timeout = 0;
            state = I2C_STOP;
            break;
        case I2C_STOP:
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2CX);
            /* i2c master sends STOP signal successfully */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
                state = I2C_END;
                i2c_timeout_flag = I2C_OK;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends stop signal timeout in WRITE!\n");
            }
            break;
        default:
            state = I2C_START;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf("i2c master sends start signal in WRITE.\n");
            break;
        }
    }
    return I2C_END;
}

/*!
    \brief      write buffer of data to the EEPROM use timeout function
    \param[in]  p_buffer: pointer to the buffer  containing the data to be written to the EEPROM
    \param[in]  write_address: EEPROM's internal address to write to
    \param[in]  number_of_byte: number of bytes to write to the EEPROM
    \param[out] none
    \retval     none
*/
void eeprom_buffer_write_timeout(uint8_t *p_buffer, uint8_t write_address, uint16_t number_of_byte)
{
    uint8_t number_of_page = 0, number_of_single = 0, address = 0, count = 0;

    address = write_address % I2C_PAGE_SIZE;
    count = I2C_PAGE_SIZE - address;
    number_of_page = number_of_byte / I2C_PAGE_SIZE;
    number_of_single = number_of_byte % I2C_PAGE_SIZE;

    /* if write_address is I2C_PAGE_SIZE aligned  */
    if(0 == address) {
        while(number_of_page--) {
            eeprom_page_write_timeout(p_buffer, write_address, I2C_PAGE_SIZE);
            eeprom_wait_standby_state_timeout();
            write_address += I2C_PAGE_SIZE;
            p_buffer += I2C_PAGE_SIZE;
        }
        if(0 != number_of_single) {
            eeprom_page_write_timeout(p_buffer, write_address, number_of_single);
            eeprom_wait_standby_state_timeout();
        }
    } else {
        /* if write_address is not I2C_PAGE_SIZE aligned */
        if(number_of_byte < count) {
            eeprom_page_write_timeout(p_buffer, write_address, number_of_byte);
            eeprom_wait_standby_state_timeout();
        } else {
            number_of_byte -= count;
            number_of_page = number_of_byte / I2C_PAGE_SIZE;
            number_of_single = number_of_byte % I2C_PAGE_SIZE;
            if(0 != count) {
                eeprom_page_write_timeout(p_buffer, write_address, count);
                eeprom_wait_standby_state_timeout();
                write_address += count;
                p_buffer += count;
            }
            /* write page */
            while(number_of_page--) {
                eeprom_page_write_timeout(p_buffer, write_address, I2C_PAGE_SIZE);
                eeprom_wait_standby_state_timeout();
                write_address += I2C_PAGE_SIZE;
                p_buffer += I2C_PAGE_SIZE;
            }
            /* write single */
            if(0 != number_of_single) {
                eeprom_page_write_timeout(p_buffer, write_address, number_of_single);
                eeprom_wait_standby_state_timeout();
            }
        }
    }
}

/*!
    \brief      read data from the EEPROM
    \param[in]  p_buffer: pointer to the buffer that receives the data read from the EEPROM
    \param[in]  read_address: EEPROM's internal address to start reading from
    \param[in]  number_of_byte: number of bytes to reads from the EEPROM
    \param[out] none
    \retval     none
*/
uint8_t eeprom_buffer_read_timeout(uint8_t *p_buffer, uint8_t read_address, uint16_t number_of_byte)
{
    uint8_t state = I2C_START;
    uint8_t read_cycle = 0;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    /* enable acknowledge */
    i2c_ack_config(I2CX, I2C_ACK_ENABLE);
    while(!(i2c_timeout_flag)) {
        switch(state) {
        case I2C_START:
            if(RESET == read_cycle) {
                /* i2c master sends start signal only when the bus is idle */
                while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    /* whether to send ACK or not for the next byte */
                    if(2 == number_of_byte) {
                        i2c_ackpos_config(I2CX, I2C_ACKPOS_NEXT);
                    }
                } else {
                    i2c_bus_reset();
                    timeout = 0;
                    state = I2C_START;
                    printf("i2c bus is busy in READ!\n");
                }
            }
            /* send the start signal */
            i2c_start_on_bus(I2CX);
            timeout = 0;
            state = I2C_SEND_ADDRESS;
            break;
        case I2C_SEND_ADDRESS:
            /* i2c master sends START signal successfully */
            while((!i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                if(RESET == read_cycle) {
                    i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);
                    state = I2C_CLEAR_ADDRESS_FLAG;
                } else {
                    i2c_master_addressing(I2CX, eeprom_address, I2C_RECEIVER);
                    if(number_of_byte < 3) {
                        /* disable acknowledge */
                        i2c_ack_config(I2CX, I2C_ACK_DISABLE);
                    }
                    state = I2C_CLEAR_ADDRESS_FLAG;
                }
                timeout = 0;
            } else {
                timeout = 0;
                state = I2C_START;
                read_cycle = 0;
                printf("i2c master sends start signal timeout in READ!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:
            /* address flag set means i2c slave sends ACK */
            while((!i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                if((SET == read_cycle) && (1 == number_of_byte)) {
                    /* send a stop condition to I2C bus */
                    i2c_stop_on_bus(I2CX);
                }
                timeout = 0;
                state = I2C_TRANSMIT_DATA;
            } else {
                timeout = 0;
                state = I2C_START;
                read_cycle = 0;
                printf("i2c master clears address flag timeout in READ!\n");
            }
            break;
        case I2C_TRANSMIT_DATA:
            if(RESET == read_cycle) {
                /* wait until the transmit data buffer is empty */
                while((! i2c_flag_get(I2CX, I2C_FLAG_TBE)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    /* send the EEPROM's internal address to write to : only one byte address */
                    i2c_data_transmit(I2CX, read_address);
                    timeout = 0;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle = 0;
                    printf("i2c master wait data buffer is empty timeout in READ!\n");
                }
                /* wait until BTC bit is set */
                while((!i2c_flag_get(I2CX, I2C_FLAG_BTC)) && (timeout < I2C_TIME_OUT)) {
                    timeout++;
                }
                if(timeout < I2C_TIME_OUT) {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle++;
                } else {
                    timeout = 0;
                    state = I2C_START;
                    read_cycle = 0;
                    printf("i2c master sends EEPROM's internal address timeout in READ!\n");
                }
            } else {
                while(number_of_byte) {
                    timeout++;
                    if(3 == number_of_byte) {
                        /* wait until BTC bit is set */
                        while(!i2c_flag_get(I2CX, I2C_FLAG_BTC));
                        /* disable acknowledge */
                        i2c_ack_config(I2CX, I2C_ACK_DISABLE);
                    }
                    if(2 == number_of_byte) {
                        /* wait until BTC bit is set */
                        while(!i2c_flag_get(I2CX, I2C_FLAG_BTC));
                        /* send a stop condition to I2C bus */
                        i2c_stop_on_bus(I2CX);
                    }
                    /* wait until RBNE bit is set */
                    if(i2c_flag_get(I2CX, I2C_FLAG_RBNE)) {
                        /* read a byte from the EEPROM */
                        *p_buffer = i2c_data_receive(I2CX);
                        /* point to the next location where the byte read will be saved */
                        p_buffer++;
                        /* decrement the read bytes counter */
                        number_of_byte--;
                        timeout = 0;
                    }
                    if(timeout > I2C_TIME_OUT) {
                        timeout = 0;
                        state = I2C_START;
                        read_cycle = 0;
                        printf("i2c master sends data timeout in READ!\n");
                    }
                }
                timeout = 0;
                state = I2C_STOP;
            }
            break;
        case I2C_STOP:
            /* i2c master sends STOP signal successfully */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
                state = I2C_END;
                i2c_timeout_flag = I2C_OK;
            } else {
                timeout = 0;
                state = I2C_START;
                read_cycle = 0;
                printf("i2c master sends stop signal timeout in READ!\n");
            }
            break;
        default:
            state = I2C_START;
            read_cycle = 0;
            i2c_timeout_flag = I2C_OK;
            timeout = 0;
            printf("i2c master sends start signal in READ.\n");
            break;
        }
    }
    return I2C_END;
}

/*!
    \brief      wait for EEPROM standby state use timeout function
    \param[in]  none
    \param[out] none
    \retval     none
*/
uint8_t eeprom_wait_standby_state_timeout()
{
    uint8_t state = I2C_START;
    uint16_t timeout = 0;
    uint8_t i2c_timeout_flag = 0;

    while(!(i2c_timeout_flag)) {
        switch(state) {
        case I2C_START:
            /* i2c master sends start signal only when the bus is idle */
            while(i2c_flag_get(I2CX, I2C_FLAG_I2CBSY) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_start_on_bus(I2CX);
                timeout = 0;
                state = I2C_SEND_ADDRESS;
            } else {
                i2c_bus_reset();
                timeout = 0;
                state = I2C_START;
                printf("i2c bus is busy in EEPROM standby!\n");
            }
            break;
        case I2C_SEND_ADDRESS:
            /* i2c master sends START signal successfully */
            while((! i2c_flag_get(I2CX, I2C_FLAG_SBSEND)) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                i2c_master_addressing(I2CX, eeprom_address, I2C_TRANSMITTER);
                timeout = 0;
                state = I2C_CLEAR_ADDRESS_FLAG;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends start signal timeout in EEPROM standby!\n");
            }
            break;
        case I2C_CLEAR_ADDRESS_FLAG:
            while((!((i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) || (i2c_flag_get(I2CX, I2C_FLAG_AERR)))) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                if(i2c_flag_get(I2CX, I2C_FLAG_ADDSEND)) {
                    i2c_flag_clear(I2CX, I2C_FLAG_ADDSEND);
                    timeout = 0;
                    /* send a stop condition to I2C bus */
                    i2c_stop_on_bus(I2CX);
                    i2c_timeout_flag = I2C_OK;
                    /* exit the function */
                    return I2C_END;
                } else {
                    /* clear the bit of AERR */
                    i2c_flag_clear(I2CX, I2C_FLAG_AERR);
                    timeout = 0;
                    state = I2C_STOP;
                }
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master clears address flag timeout in EEPROM standby!\n");
            }
            break;
        case I2C_STOP:
            /* send a stop condition to I2C bus */
            i2c_stop_on_bus(I2CX);
            /* i2c master sends STOP signal successfully */
            while((I2C_CTL0(I2CX) & I2C_CTL0_STOP) && (timeout < I2C_TIME_OUT)) {
                timeout++;
            }
            if(timeout < I2C_TIME_OUT) {
                timeout = 0;
                state = I2C_START;
            } else {
                timeout = 0;
                state = I2C_START;
                printf("i2c master sends stop signal timeout in EEPROM standby!\n");
            }
            break;
        default:
            state = I2C_START;
            timeout = 0;
            printf("i2c master sends start signal end in EEPROM standby!\n");
            break;
        }
    }
    return I2C_END;
}
