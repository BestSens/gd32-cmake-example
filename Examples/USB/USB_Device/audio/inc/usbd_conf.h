/*!
    \file    usbd_conf.h
    \brief   the header file of USB device configuration

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

#ifndef __USBD_CONF_H
#define __USBD_CONF_H

#include "usb_conf.h"

/* usb configure exported defines */ 
#define USBD_CFG_MAX_NUM                    1
#define USBD_ITF_MAX_NUM                    3

#define USBD_AD_INTERFACE                   0

#define USB_STR_DESC_MAX_SIZE               255

/* Audio class layer parameter */ 
#define AD_OUT_EP                           EP_OUT(3)
#define AD_IN_EP                            EP_IN(1)
#define AD_FEEDBACK_IN_EP                   EP_IN(2)

#define USB_STRING_COUNT                    4U

/* micro-phone parameter */
#define USBD_MIC_FREQ                       USBD_AD_FREQ_8K
#define MIC_IN_BIT_RESOLUTION               16
#define MIC_IN_CHANNEL_NBR                  2 /* Mono = 1, Stereo = 2 */
#define MIC_IN_PACKET                       (uint32_t)(((USBD_MIC_FREQ * \
                                                        (MIC_IN_BIT_RESOLUTION/8) *\
                                                         MIC_IN_CHANNEL_NBR) /1000)) 

/* speaker parameter */
#define USBD_SPEAKER_FREQ                   USBD_AD_FREQ_48K
#define SPEAKER_OUT_BIT_RESOLUTION          16
#define SPEAKER_OUT_CHANNEL_NBR             2 /* Mono = 1, Stereo = 2 */
#define SPEAKER_OUT_PACKET                  (uint32_t)(((USBD_SPEAKER_FREQ * \
                                                        (SPEAKER_OUT_BIT_RESOLUTION/8) *\
                                                         SPEAKER_OUT_CHANNEL_NBR) /1000)) 

/* feedback parameter */
#define FEEDBACK_FREQ_OFFSET                (USBD_SPEAKER_FREQ/100)
#define FEEDBACK_IN_PACKET                  3
#define FEEDBACK_IN_INTERVAL                5

#define SPEAKER_OUT_MAX_PACKET              (SPEAKER_OUT_PACKET + 20)

/* audio frequency in Hz */
#define USBD_AD_FREQ_48K                    48000U
#define USBD_AD_FREQ_44K                    44100U
#define USBD_AD_FREQ_22K                    22000U
#define USBD_AD_FREQ_16K                    16000U
#define USBD_AD_FREQ_8K                     8000U

#define DEFAULT_VOLUME                      65U    /* Default volume in % (Mute=0%, Max = 100%) in Logarithmic values.
                                                      To get accurate volume variations, it is possible to use a logarithmic
                                                      coversion table to convert from percentage to logarithmic law.
                                                      In order to keep this example code simple, this conversion is not used.*/

/*!
    \brief      get the calculate value of i2s sample frequency
    \param[in]  set_freq: setting sample frequence
    \param[out] none
    \retval     i2s sample frequency
    \note       Users need to calculate the actual sampling frequency 
                of I2S module at different frequency points by themselves.
*/
__STATIC_INLINE uint32_t I2S_ACTUAL_SAM_FREQ(uint32_t set_freq)
{
    uint32_t actual_freq;

    switch(set_freq){
    case USBD_AD_FREQ_48K:
        actual_freq = 47990;
        break;

    case USBD_AD_FREQ_44K:
        actual_freq = 44100;
        break;

    case USBD_AD_FREQ_16K:
        actual_freq = 16000;
        break;

    case USBD_AD_FREQ_8K:
        actual_freq = 8000;
        break;

    default:
        actual_freq = set_freq;
        break;
    }

    return actual_freq;
}

#endif /* __USBD_CONF_H */
