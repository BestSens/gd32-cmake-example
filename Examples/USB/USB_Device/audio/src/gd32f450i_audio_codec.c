/*!
    \file    gd32f450i_audio_codec.c
    \brief   This file includes the low layer driver for CS4344 Audio Codec

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

#include "gd32f450i_audio_codec.h"
#include "audio_out_itf.h"
#include "audio_core.h"

uint32_t i2s_audiofreq = 0;
dma_single_data_parameter_struct dma_initstructure;

/* I2SPLL parameters configure based on the sampling-rate */
const uint32_t I2SFreq[8] = {8000, 11025, 16000, 22050, 32000, 44100, 48000, 96000};
const uint32_t I2SPLLN[8] = {256, 429, 213, 429, 426, 271, 258, 344};
const uint32_t I2SPLLR[8] = {5, 4, 2, 4, 4, 6, 3, 1};

/*!
    \brief      initializes the audio codec audio interface (i2s)
    \note       this function assumes that the i2s input clock (through pll_r in 
                devices reva/z and through dedicated plli2s_r in devices revb/y)
                is already configured and ready to be used
    \param[in]  audio_freq: audio frequency to be configured for the i2s peripheral
    \param[out] none
    \retval     none
*/
void codec_audio_interface_init(uint32_t audio_freq)
{
    uint8_t index = 0, freqindex = 0xFF;

    i2s_audiofreq = audio_freq;

    for (index = 0; index < 8; index++) {
        if (I2SFreq[index] == audio_freq) {
            freqindex = index;
        }
    }

    if (freqindex != 0xFF) {
        /* must turn off when modify PLLI2S rcu configure */
        rcu_osci_off(RCU_PLLI2S_CK);

        rcu_plli2s_config(I2SPLLN[freqindex], I2SPLLR[freqindex]);
        rcu_i2s_clock_config(RCU_I2SSRC_PLLI2S);

        /* must turn on when modify PLLI2S rcu configure */
        rcu_osci_on(RCU_PLLI2S_CK);
    }

    /* enable the AD_I2S peripheral clock */
    rcu_periph_clock_enable(AD_I2S_CLK);

    /* AD_I2S peripheral configuration */
    spi_i2s_deinit(AD_I2S);

    /* initialize the I2S peripheral with the structure above */
    i2s_psc_config(AD_I2S, audio_freq, I2S_FRAMEFORMAT_DT16B_CH16B, 
#ifdef IIS_MCLK_ENABLED
    I2S_MCKOUT_ENABLE
#elif defined(IIS_MCLK_DISABLED)
    I2S_MCKOUT_DISABLE
#endif
    );

    i2s_init(AD_I2S, I2S_MODE_MASTERTX, I2S_STD_LSB, I2S_CKPL_LOW);

    /* enable the I2S DMA TX request */
    spi_dma_enable(AD_I2S, SPI_DMA_TRANSMIT);
}

/*!
    \brief      deinitialize the audio codec audio interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void codec_audio_interface_deinit(void)
{
    /* disable the codec_i2s peripheral */
    i2s_disable(AD_I2S);

    /* deinitialize the codec_i2s peripheral */
    spi_i2s_deinit(AD_I2S);

    /* disable the codec_i2s peripheral clock */
    rcu_periph_clock_disable(AD_I2S_CLK);
}

/*!
    \brief      initializes IOs used by the audio codec
    \param[in]  none
    \param[out] none
    \retval     none
*/
void codec_gpio_init(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(AD_I2S_WS_CLK);
    rcu_periph_clock_enable(AD_I2S_SCK_CLK);
    rcu_periph_clock_enable(AD_I2S_SD_CLK);
    rcu_periph_clock_enable(RCU_SYSCFG);

    /* AD_I2S pins configuration: WS, SCK and SD pins */

    gpio_mode_set(AD_I2S_WS_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, AD_I2S_WS_PIN);
    gpio_output_options_set(AD_I2S_WS_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AD_I2S_WS_PIN);
    gpio_af_set(AD_I2S_WS_GPIO, GPIO_AF_5, AD_I2S_WS_PIN);

    gpio_mode_set(AD_I2S_SCK_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, AD_I2S_SCK_PIN);
    gpio_output_options_set(AD_I2S_SCK_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AD_I2S_SCK_PIN);
    gpio_af_set(AD_I2S_SCK_GPIO, GPIO_AF_5, AD_I2S_SCK_PIN);

    gpio_mode_set(AD_I2S_SD_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, AD_I2S_SD_PIN);
    gpio_output_options_set(AD_I2S_SD_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AD_I2S_SD_PIN);
    gpio_af_set(AD_I2S_SD_GPIO, GPIO_AF_7, AD_I2S_SD_PIN);

#ifdef IIS_MCLK_ENABLED
    /* enable GPIO clock */
    rcu_periph_clock_enable(AD_I2S_MCK_CLK);

    /* codec_i2s pins configuration: MCK pin */
    gpio_mode_set(AD_I2S_MCK_GPIO, GPIO_MODE_AF, GPIO_PUPD_NONE, AD_I2S_MCK_PIN);
    gpio_output_options_set(AD_I2S_MCK_GPIO, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, AD_I2S_MCK_PIN);
    gpio_af_set(AD_I2S_MCK_GPIO, GPIO_AF_6, AD_I2S_MCK_PIN);
#endif /* IIS_MCLK_ENABLED */
}

/*!
    \brief      deinitialize IOs used by the audio codec interface
    \param[in]  none
    \param[out] none
    \retval     none
*/
void codec_gpio_deinit(void)
{
    /* deinitialize all the GPIOs used by the driver */
    gpio_mode_set(AD_I2S_SD_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_NONE, AD_I2S_SD_PIN);
    gpio_mode_set(AD_I2S_WS_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_NONE, AD_I2S_WS_PIN);
    gpio_mode_set(AD_I2S_SCK_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_NONE, AD_I2S_SCK_PIN);

#ifdef IIS_MCLK_ENABLED
    /* AD_I2S pins deinitialization: MCK pin */
    gpio_mode_set(AD_I2S_MCK_GPIO, GPIO_MODE_INPUT, GPIO_PUPD_NONE, AD_I2S_MCK_PIN);
#endif /* IIS_MCLK_ENABLED */
}

/*!
    \brief      initializes dma to prepare for audio data transfer
                from Media to the I2S peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
void codec_i2s_dma_init(void)
{
    /* enable the DMA clock */
    rcu_periph_clock_enable(AD_DMA_CLOCK);

    /* configure the DMA Stream */
    dma_channel_enable(AD_DMA, AD_DMA_CHANNEL);
    dma_deinit(AD_DMA, AD_DMA_CHANNEL);

    /* set the parameters to be configured */
    dma_initstructure.periph_addr = AD_I2S_ADDRESS;
    dma_initstructure.memory0_addr = (uint32_t)0; /* this field will be configured in play function */
    dma_initstructure.direction = DMA_MEMORY_TO_PERIPH;
    dma_initstructure.number = (uint32_t)0xFFFE; /* this field will be configured in play function */
    dma_initstructure.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_initstructure.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_initstructure.periph_memory_width = AD_DMA_PERIPH_DATA_SIZE;
    dma_initstructure.circular_mode = DMA_CIRCULAR_MODE_DISABLE;
    dma_initstructure.priority = DMA_PRIORITY_ULTRA_HIGH;
    dma_single_data_mode_init(AD_DMA, AD_DMA_CHANNEL, &dma_initstructure);
    dma_channel_subperipheral_select(AD_DMA, AD_DMA_CHANNEL, DMA_SUBPERI0);

    /* clear the Interrupt flag */
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_FTFIFC);
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_HTFIFC);
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_TAEIFC);
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_SDEIFC);
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_FEEIFC);

    /* enable the selected DMA interrupts */
    dma_interrupt_enable(AD_DMA, AD_DMA_CHANNEL, DMA_CHXCTL_FTFIE);

    /* enable the I2S DMA request */
    spi_dma_enable(AD_I2S, SPI_DMA_TRANSMIT);

    /* I2S DMA IRQ channel configuration */
    nvic_irq_enable(AD_DMA_IRQ, AD_IRQ_PREPRIO, AD_IRQ_SUBRIO);
}

/*!
    \brief      restore default state of the used media
    \param[in]  none
    \param[out] none
    \retval     none
*/
void audio_mal_deinit(void)  
{
    /* deinitialize the NVIC interrupt for the I2S DMA Stream */
    nvic_irq_disable(AD_DMA_IRQ);

    /* disable the DMA channel before the deinitialize */
    dma_channel_disable(AD_DMA, AD_DMA_CHANNEL);

    /* deinitialize the DMA channel */
    dma_deinit(AD_DMA, AD_DMA_CHANNEL);
}

/*!
    \brief      starts playing audio stream from the audio media
    \param[in]  addr: pointer to the audio stream buffer
    \param[in]  size: number of data in the audio stream buffer
    \param[out] none
    \retval     none
*/
void audio_play(uint32_t addr, uint32_t size)
{
    /* disable the I2S DMA Stream*/
    dma_channel_disable(AD_DMA, AD_DMA_CHANNEL);

    /* clear the Interrupt flag */
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_FTFIFC);
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_HTFIFC);
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_TAEIFC);
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_SDEIFC);
    dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INTC_FEEIFC);

    /* configure the buffer address and size */
    dma_initstructure.memory0_addr = (uint32_t)addr;
    dma_initstructure.number = (uint32_t)(size);

    /* configure the DMA Stream with the new parameters */
    dma_single_data_mode_init(AD_DMA, AD_DMA_CHANNEL, &dma_initstructure);

    /* enable the I2S DMA Stream*/
    dma_channel_enable(AD_DMA, AD_DMA_CHANNEL);

    /* if the i2s peripheral is still not enabled, enable it */
    if ((SPI_I2SCTL(AD_I2S) & I2S_ENABLE_MASK) == 0) {
        i2s_enable(AD_I2S);
    }
}

/*!
    \brief      pauses or resumes the audio stream playing from the media
    \param[in]  cmd: AD_PAUSE (or 0) to pause, AD_RESUME (or any value different
                from 0) to resume
    \param[in]  addr: address from/at which the audio stream should resume/pause
    \param[in]  size: number of data to be configured for next resume
    \param[out] none
    \retval     none
*/
void audio_pause_resume(uint32_t cmd, uint32_t addr, uint32_t size)
{
    /* pause the audio file playing */
    if (cmd == AD_PAUSE) {
        /* stop the current DMA request by resetting the I2S peripherals */
        codec_audio_interface_deinit();

        /* re-configure the I2S interface for the next resume operation */
        codec_audio_interface_init(i2s_audiofreq);

        /* disable the DMA Stream */
        dma_channel_disable(AD_DMA, AD_DMA_CHANNEL);

        /* clear all the DMA flags for the next transfer */
        dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_FEE);
        dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_SDE);
        dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_TAE);
        dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_HTF);
        dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_FTF);
    } else {
        /* configure the buffer address and size */
        dma_initstructure.memory0_addr = (uint32_t)addr;
        dma_initstructure.number = (uint32_t)size;

        /* configure the DMA Stream with the new parameters */
        dma_single_data_mode_init(AD_DMA, AD_DMA_CHANNEL, &dma_initstructure);

        /* enable the I2S DMA Stream*/
        dma_channel_enable(AD_DMA, AD_DMA_CHANNEL);

        /* if the I2S peripheral is still not enabled, enable it */
        if (0U == (SPI_I2SCTL(AD_I2S) & I2S_ENABLE_MASK)) {
            i2s_enable(AD_I2S);
        }
    }
}

/*!
    \brief      stops audio stream playing on the used media
    \param[in]  none
    \param[out] none
    \retval     none
*/
void audio_stop(void)
{
    /* stop the transfer on the I2S side: Stop and disable the DMA stream */
    dma_channel_disable(AD_DMA, AD_DMA_CHANNEL);

    /* clear all the DMA flags for the next transfer */
    dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_FEE);
    dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_SDE);
    dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_TAE);
    dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_HTF);
    dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_FLAG_FTF);

    /* stop the current DMA request by resetting the I2S cell */
    codec_audio_interface_deinit();

    /* re-configure the I2S interface for the next play operation */
    codec_audio_interface_init(i2s_audiofreq);
}

/*!
    \brief      this function handles main media layer interrupt
    \param[in]  none
    \param[out] none
    \retval     0 if correct communication, else wrong communication
*/
void AD_DMA_IRQHandler(void)
{
    uint16_t remain_size = 0;

    /* transfer complete interrupt */
    if(dma_flag_get(AD_DMA, AD_DMA_CHANNEL, AD_DMA_FLAG_TC) != RESET){
        /* increment to the next sub-buffer */
        audio_handler.isoc_out_rdptr += audio_handler.dam_tx_len;

        if (audio_handler.isoc_out_rdptr >= (audio_handler.isoc_out_buff + TOTAL_OUT_BUF_SIZE)) {
            /* roll back to the start of buffer */
            audio_handler.isoc_out_rdptr = audio_handler.isoc_out_buff;
        }

        if(audio_handler.isoc_out_wrptr >= audio_handler.isoc_out_rdptr){
            remain_size = audio_handler.isoc_out_wrptr - audio_handler.isoc_out_rdptr;
        }else{
            remain_size = TOTAL_OUT_BUF_SIZE + audio_handler.isoc_out_buff - audio_handler.isoc_out_rdptr;
        }

        /* check if the end of file has been reached */
        if(remain_size > 0){
            dma_channel_disable(AD_DMA, AD_DMA_CHANNEL);

            /* clear the Interrupt flag */
            dma_flag_clear(AD_DMA, AD_DMA_CHANNEL, AD_DMA_FLAG_TC);

            /* clear the Interrupt flag */
            dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, AD_DMA_INT_FLAG_TE);

            DMA_CHM0ADDR(AD_DMA, AD_DMA_CHANNEL) = (uint32_t)audio_handler.isoc_out_rdptr;

            DMA_CHCNT(AD_DMA, AD_DMA_CHANNEL) = (uint32_t)(remain_size / 2);

            /* update the current DMA tx data length */
            audio_handler.dam_tx_len = (remain_size / 2) * 2;

            dma_channel_enable(AD_DMA, AD_DMA_CHANNEL);
        }else{
            /* disable the I2S DMA Stream*/
            dma_channel_disable(AD_DMA, AD_DMA_CHANNEL);

            /* clear the Interrupt flag */
            dma_interrupt_flag_clear(AD_DMA, AD_DMA_CHANNEL, DMA_INT_FLAG_FTF);

            /* clear flag */
            audio_handler.dam_tx_len = 0;
            audio_handler.play_flag = 0U;
        }
    }
}
