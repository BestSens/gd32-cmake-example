/*!
    \file    sccb.h
    \brief   definitions for sccb 
    
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

#ifndef __SCCB_H
#define __SCCB_H

#include "gd32f4xx.h"

 /* configure I2C1 pins: PB10 and PB11 */ 
#define DCI_SCCB                       I2C0
#define DCI_SCCB_CLK                   RCU_I2C0

#define DCI_SCCB_SDA_PIN               GPIO_PIN_7
#define DCI_SCCB_SDA_GPIO_PORT         GPIOB
#define DCI_SCCB_SDA_GPIO_CLK          RCU_GPIOB

#define DCI_SCCB_SCL_PIN               GPIO_PIN_6
#define DCI_SCCB_SCL_GPIO_PORT         GPIOB
#define DCI_SCCB_SCL_GPIO_CLK          RCU_GPIOB

#define SCCB_FLAG_TIMEOUT              20000000
#define OV2640_DEVICE_WRITE_ADDRESS    0x60
#define OV2640_DEVICE_READ_ADDRESS     0x61

#define SCCB_SPEED                     100000
#define SCCB_SLAVE_ADDRESS7            0xFE

/* function declarations */
void sccb_config(void);
uint8_t dci_byte_write(uint8_t reg, uint8_t data);
uint8_t dci_byte_read(uint8_t reg, uint8_t *data);

#endif /* __SCCB_H */


