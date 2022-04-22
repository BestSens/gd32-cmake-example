/*!
    \file    sccb.c
    \brief   sccb driver
    
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

#include "sccb.h"

/*!
    \brief      configure I2C gpio and I2C parameter
    \param[in]  none
    \param[out] none
    \retval     none
*/
void sccb_config(void)
{
    /* enable GPIOB and I2C clock */
    rcu_periph_clock_enable(DCI_SCCB_SCL_GPIO_CLK);
    rcu_periph_clock_enable(DCI_SCCB_CLK);

    /* configure I2C GPIO pins AF */
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_6);
    gpio_af_set(GPIOB, GPIO_AF_4, GPIO_PIN_7);
    
    /* configure I2C0_SCL(PB6), I2C0_SDA(PB7) */ 
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,DCI_SCCB_SCL_PIN);
    
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ,DCI_SCCB_SDA_PIN);
    
    /* configure I2C parameter */
    i2c_deinit(DCI_SCCB);
    i2c_clock_config(DCI_SCCB,SCCB_SPEED,I2C_DTCY_2);
    i2c_mode_addr_config(DCI_SCCB,I2C_I2CMODE_ENABLE,I2C_ADDFORMAT_7BITS,SCCB_SLAVE_ADDRESS7);
    i2c_enable(DCI_SCCB);

    i2c_ack_config(DCI_SCCB,I2C_ACK_ENABLE);
}

/*!
    \brief      write a byte at a specific camera register
    \param[in]  reg:  camera register address
    \param[in]  data: data to be written to the specific register
    \param[out] none
    \retval     0x00 if write operation is OK. 
                0xFF if timeout condition occured (device not connected or bus error).
*/
uint8_t dci_byte_write(uint8_t reg, uint8_t data)
{
    volatile uint32_t timeout = SCCB_FLAG_TIMEOUT;

    /* the software must wait until i2c bus is idle */
    while(i2c_flag_get(DCI_SCCB,I2C_FLAG_I2CBSY));  
    /* generate the start condition */
    i2c_start_on_bus(DCI_SCCB);
    
    /* test on i2c I2C_SBSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0)
            return 0xFF;
    }
    
    /* send DCI selected device slave address for write */
    i2c_master_addressing(DCI_SCCB, OV2640_DEVICE_WRITE_ADDRESS,I2C_TRANSMITTER);
    
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(DCI_SCCB,I2C_FLAG_ADDSEND);
    
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB,I2C_FLAG_TBE)){
         if ((timeout--) == 0) 
             return 0xFF;
    }
    
    /* send register address */
    i2c_data_transmit(DCI_SCCB, (uint8_t)(reg));

    /* test on I2C I2C_BTC and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    /* send data */
    i2c_data_transmit(DCI_SCCB, data);    

    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_TBE)){
        if ((timeout--) == 0)
            return 0xFF;
    }  
    /* send I2C stop condition */
    i2c_stop_on_bus(DCI_SCCB);

    /* if operation is ok, return 0 */
    return 0;
}

/*!
    \brief      read a byte from a specific camera register 
    \param[in]  reg:  camera register address
    \param[out] data: read from the specific register
    \retval     0x00 if write operation is ok. 
                0xFF if timeout condition occured (device not connected or bus error).
*/
uint8_t dci_byte_read(uint8_t reg, uint8_t *data)
{
    volatile uint32_t timeout = SCCB_FLAG_TIMEOUT;

    I2C_STAT0(DCI_SCCB) |= (uint16_t)0x0400;

    /* generate the start condition */
    i2c_start_on_bus(DCI_SCCB);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    /* send DCI selcted device slave address for read */
    i2c_master_addressing(DCI_SCCB, OV2640_DEVICE_READ_ADDRESS,I2C_TRANSMITTER);

    /* test on I2C I2C_ADDSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(DCI_SCCB,I2C_FLAG_ADDSEND);
    
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB,I2C_FLAG_TBE)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    
    /* send register address */
    i2c_data_transmit(DCI_SCCB, (uint8_t)(reg));
          
    /* test on I2C I2C_BTC and clear it */
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_BTC)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 

    /* clear AF flag if arised */
    I2C_STAT0(DCI_SCCB) |= (uint16_t)0x0400;
        
    /* prepare stop after receiving data */
    i2c_stop_on_bus(DCI_SCCB);
    
    /* generate the start condition */
    i2c_start_on_bus(DCI_SCCB);

    /* test on I2C I2C_SBSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT; 
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_SBSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* send DCI selcted device slave address for write */
    i2c_master_addressing(DCI_SCCB, OV2640_DEVICE_WRITE_ADDRESS,I2C_RECEIVER);
    
    /* test on I2C I2C_ADDSEND and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_ADDSEND)){
        if ((timeout--) == 0) 
            return 0xFF;
    }
    i2c_flag_clear(DCI_SCCB,I2C_FLAG_ADDSEND); 
    
    /* prepare an nack for the next data received */
    i2c_ack_config(DCI_SCCB, I2C_ACK_DISABLE);

    /* test on I2C_RBNE and clear it */
    timeout = SCCB_FLAG_TIMEOUT;
    while(!i2c_flag_get(DCI_SCCB, I2C_FLAG_RBNE)){
        if ((timeout--) == 0) 
            return 0xFF;
    } 
    
    /* prepare stop after receiving data */
    i2c_stop_on_bus(DCI_SCCB); 

    /* receive the data */
    *data = i2c_data_receive(DCI_SCCB);
    /* clear AF flag if arised */
    I2C_STAT0(DCI_SCCB) |= (uint16_t)0x0400;

    return 0;
}
