/*!
    \file    main.c
    \brief   IPA blend demo

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
#include "logo.h"
#include "logo2.h"

#define HORIZONTAL_SYNCHRONOUS_PULSE  41
#define HORIZONTAL_BACK_PORCH         2
#define ACTIVE_WIDTH                  480
#define HORIZONTAL_FRONT_PORCH        2

#define VERTICAL_SYNCHRONOUS_PULSE    10
#define VERTICAL_BACK_PORCH           2
#define ACTIVE_HEIGHT                 272
#define VERTICAL_FRONT_PORCH          2

uint8_t blended_address_buffer[73280];

/* private function prototypes */
static void ipa_config(void);
static void tli_config(void);
static void tli_blend_config(void);
static void tli_gpio_config(void);

/*!
    \brief      main program
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    gd_eval_key_init(KEY_USER, KEY_MODE_GPIO);
    tli_config();
    tli_layer_enable(LAYER0);
    tli_layer_enable(LAYER1);
    tli_reload_config(TLI_REQUEST_RELOAD_EN);
    tli_enable();
    /* wait for user button is pressed */
    while(gd_eval_key_state_get(KEY_USER) != RESET);
    /* wait for user button is released */
    while(gd_eval_key_state_get(KEY_USER) != SET);
    /* disable layer1 */
    tli_layer_disable(LAYER1);
    /* IPA configuration */
    ipa_config();
    /* transfer enable */
    ipa_transfer_enable();
    while(ipa_interrupt_flag_get(IPA_INT_FLAG_FTF) == RESET);
    tli_blend_config();
    tli_reload_config(TLI_REQUEST_RELOAD_EN);

    while(1) {
    }
}

/*!
    \brief      IPA initialize and configuration
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void ipa_config(void)
{
    ipa_destination_parameter_struct  ipa_destination_init_struct;
    ipa_foreground_parameter_struct   ipa_fg_init_struct;
    ipa_background_parameter_struct   ipa_bg_init_struct;

    rcu_periph_clock_enable(RCU_IPA);

    ipa_deinit();
    /* configure IPA pixel format convert mode */
    ipa_pixel_format_convert_mode_set(IPA_FGBGTODE);
    /* configure destination pixel format */
    ipa_destination_init_struct.destination_pf = IPA_DPF_RGB565;
    /* configure destination memory base address */
    ipa_destination_init_struct.destination_memaddr = (uint32_t)&blended_address_buffer;
    /* configure destination pre-defined alpha value RGB */
    ipa_destination_init_struct.destination_pregreen = 0;
    ipa_destination_init_struct.destination_preblue = 0;
    ipa_destination_init_struct.destination_prered = 0;
    ipa_destination_init_struct.destination_prealpha = 0;
    /* configure destination line offset */
    ipa_destination_init_struct.destination_lineoff = 0;
    /* configure height of the image to be processed */
    ipa_destination_init_struct.image_height = 160;
    /* configure width of the image to be processed */
    ipa_destination_init_struct.image_width = 229;
    /* ipa destination initialization */
    ipa_destination_init(&ipa_destination_init_struct);

    /* configure IPA foreground */
    ipa_fg_init_struct.foreground_memaddr = (uint32_t)&gImage_logo;
    ipa_fg_init_struct.foreground_pf = FOREGROUND_PPF_RGB565;
    ipa_fg_init_struct.foreground_alpha_algorithm = IPA_FG_ALPHA_MODE_1;
    ipa_fg_init_struct.foreground_prealpha = 0x75;
    ipa_fg_init_struct.foreground_lineoff = 0x00;
    ipa_fg_init_struct.foreground_preblue = 0x00;
    ipa_fg_init_struct.foreground_pregreen = 0x00;
    ipa_fg_init_struct.foreground_prered = 0x00;
    /* foreground initialization */
    ipa_foreground_init(&ipa_fg_init_struct);

    /* configure IPA background */
    ipa_bg_init_struct.background_memaddr = (uint32_t)&gImage_logo2;
    ipa_bg_init_struct.background_pf = BACKGROUND_PPF_RGB565;
    ipa_bg_init_struct.background_alpha_algorithm = IPA_BG_ALPHA_MODE_0;
    ipa_bg_init_struct.background_prealpha = 255;
    ipa_bg_init_struct.background_lineoff = 0x00;
    ipa_bg_init_struct.background_preblue = 0x00;
    ipa_bg_init_struct.background_pregreen = 0x00;
    ipa_bg_init_struct.background_prered = 0x00;
    /* background initialization */
    ipa_background_init(&ipa_bg_init_struct);
}

/*!
    \brief      configure TLI peripheral and display blend image
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void tli_blend_config(void)
{
    tli_parameter_struct               tli_init_struct;
    tli_layer_parameter_struct         tli_layer_init_struct;
    rcu_periph_clock_enable(RCU_TLI);
    tli_gpio_config();

    /* configure the PLLSAI clock to generate LCD clock */
    if(ERROR == rcu_pllsai_config(192, 2, 3)) {
        while(1) {
        }
    }
    rcu_tli_clock_div_config(RCU_PLLSAIR_DIV8);
    rcu_osci_on(RCU_PLLSAI_CK);
    if(ERROR == rcu_osci_stab_wait(RCU_PLLSAI_CK)) {
        while(1) {
        }
    }

    /* TLI initialization */
    tli_init_struct.signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_de = TLI_DE_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;
    /* LCD display timing configuration */
    tli_init_struct.synpsz_hpsz = HORIZONTAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.synpsz_vpsz = VERTICAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.backpsz_hbpsz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1;
    tli_init_struct.backpsz_vbpsz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1;
    tli_init_struct.activesz_hasz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH - 1;
    tli_init_struct.activesz_vasz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT - 1;
    tli_init_struct.totalsz_htsz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH + HORIZONTAL_FRONT_PORCH - 1;
    tli_init_struct.totalsz_vtsz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT + VERTICAL_FRONT_PORCH - 1;
    /* LCD background color configure */
    tli_init_struct.backcolor_red = 0xFF;
    tli_init_struct.backcolor_green = 0xFF;
    tli_init_struct.backcolor_blue = 0xFF;
    tli_init(&tli_init_struct);

    /* TLI layer0 configuration */
    /* TLI window size configuration */
    tli_layer_init_struct.layer_window_leftpos = 120 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_rightpos = (120 + 229 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_window_toppos = 60 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_bottompos = 60 + 160 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1;
    /* TLI window pixel format configuration */
    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
    /* TLI window specified alpha configuration */
    tli_layer_init_struct.layer_sa = 255;
    /* TLI window blend configuration */
    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_SA;
    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_SA;
    /* TLI layer default alpha R,G,B value configuration */
    tli_layer_init_struct.layer_default_alpha = 0;
    tli_layer_init_struct.layer_default_blue = 0;
    tli_layer_init_struct.layer_default_green = 0;
    tli_layer_init_struct.layer_default_red = 0;
    /* TLI layer frame buffer base address configuration */
    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)&blended_address_buffer;
    tli_layer_init_struct.layer_frame_line_length = ((229 * 2) + 3);
    tli_layer_init_struct.layer_frame_buf_stride_offset = (229 * 2);
    tli_layer_init_struct.layer_frame_total_line_number = 160;
    tli_layer_init(LAYER0, &tli_layer_init_struct);
    tli_dither_config(TLI_DITHER_ENABLE);
}

/*!
    \brief      configure TLI peripheral
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void tli_config(void)
{
    tli_parameter_struct               tli_init_struct;
    tli_layer_parameter_struct         tli_layer_init_struct;
    rcu_periph_clock_enable(RCU_TLI);
    tli_gpio_config();

    /* configure the PLLSAI clock to generate LCD clock */
    if(ERROR == rcu_pllsai_config(192, 2, 2)) {
        while(1) {
        }
    }
    rcu_tli_clock_div_config(RCU_PLLSAIR_DIV8);
    rcu_osci_on(RCU_PLLSAI_CK);
    if(ERROR == rcu_osci_stab_wait(RCU_PLLSAI_CK)) {
        while(1) {
        }
    }

    /* TLI initialization */
    tli_init_struct.signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_de = TLI_DE_ACTLIVE_LOW;
    tli_init_struct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;
    /* LCD display timing configuration */
    tli_init_struct.synpsz_hpsz = HORIZONTAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.synpsz_vpsz = VERTICAL_SYNCHRONOUS_PULSE - 1;
    tli_init_struct.backpsz_hbpsz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1;
    tli_init_struct.backpsz_vbpsz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1;
    tli_init_struct.activesz_hasz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH - 1;
    tli_init_struct.activesz_vasz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT - 1;
    tli_init_struct.totalsz_htsz = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH + ACTIVE_WIDTH + HORIZONTAL_FRONT_PORCH - 1;
    tli_init_struct.totalsz_vtsz = VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH + ACTIVE_HEIGHT + VERTICAL_FRONT_PORCH - 1;
    /* configure LCD background color */
    tli_init_struct.backcolor_red = 0x0;
    tli_init_struct.backcolor_green = 0x0;
    tli_init_struct.backcolor_blue = 0x0;
    tli_init(&tli_init_struct);

    /* TLI layer0 configuration */
    /* TLI window size configuration */
    tli_layer_init_struct.layer_window_leftpos = HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_rightpos = (229 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_window_toppos = 60 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_bottompos = 60 + 160 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1;
    /* TLI window pixel format configuration */
    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
    /* TLI window specified alpha configuration */
    tli_layer_init_struct.layer_sa = 250;
    /* TLI window blend configuration */
    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_PASA;
    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_PASA;
    /* TLI layer default alpha R,G,B value configuration */
    tli_layer_init_struct.layer_default_alpha = 0;
    tli_layer_init_struct.layer_default_blue = 0;
    tli_layer_init_struct.layer_default_green = 0;
    tli_layer_init_struct.layer_default_red = 0;
    /* TLI layer frame buffer base address configuration */
    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)&gImage_logo;
    tli_layer_init_struct.layer_frame_line_length = ((229 * 2) + 3);
    tli_layer_init_struct.layer_frame_buf_stride_offset = (229 * 2);
    tli_layer_init_struct.layer_frame_total_line_number = 160;
    tli_layer_init(LAYER0, &tli_layer_init_struct);

    /* TLI layer1 configuration */
    /* TLI window size configuration */
    tli_layer_init_struct.layer_window_leftpos = 239 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_rightpos = (239 + 229 + HORIZONTAL_SYNCHRONOUS_PULSE + HORIZONTAL_BACK_PORCH - 1);
    tli_layer_init_struct.layer_window_toppos = 60 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH;
    tli_layer_init_struct.layer_window_bottompos = (60 + 160 + VERTICAL_SYNCHRONOUS_PULSE + VERTICAL_BACK_PORCH - 1);
    /* TLI window pixel format configuration */
    tli_layer_init_struct.layer_ppf = LAYER_PPF_RGB565;
    tli_layer_init_struct.layer_sa = 250;
    /* TLI layer default alpha R,G,B value configuration */
    tli_layer_init_struct.layer_default_blue = 0;
    tli_layer_init_struct.layer_default_green = 0;
    tli_layer_init_struct.layer_default_red = 0;
    tli_layer_init_struct.layer_default_alpha = 0;
    /* TLI window blend configuration */
    tli_layer_init_struct.layer_acf1 = LAYER_ACF1_PASA;
    tli_layer_init_struct.layer_acf2 = LAYER_ACF2_PASA;
    /* TLI layer frame buffer base address configuration */
    tli_layer_init_struct.layer_frame_bufaddr = (uint32_t)&gImage_logo2;
    tli_layer_init_struct.layer_frame_line_length = ((229 * 2) + 3);
    tli_layer_init_struct.layer_frame_buf_stride_offset = (229 * 2);
    tli_layer_init_struct.layer_frame_total_line_number = 160;
    tli_layer_init(LAYER1, &tli_layer_init_struct);
}

/*!
    \brief      configure TLI GPIO
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void tli_gpio_config(void)
{
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_GPIOH);
    rcu_periph_clock_enable(RCU_GPIOI);
    rcu_periph_clock_enable(RCU_GPIOG);

    /* configure HSYNC(PI10), VSYNC(PI9), PCLK(PG7) */
    /* configure LCD_R7(PG6), LCD_R6(PH12), LCD_R5(PH11), LCD_R4(PH10), LCD_R3(PH9),LCD_R2(PH8),
                 LCD_R1(PH3), LCD_R0(PH2), LCD_G7(PI2), LCD_G6(PI1), LCD_G5(PI0), LCD_G4(PH15),
                 LCD_G3(PH14), LCD_G2(PH13),LCD_G1(PE6), LCD_G0(PE5),LCD_B7(PI7), LCD_B6(PI6),
                 LCD_B5(PI5), LCD_B4(PI4), LCD_B3(PG11),LCD_B2(PG10), LCD_B1(PG12), LCD_B0(PE4) */
    /* configure TLI pins AF function */
    gpio_af_set(GPIOE, GPIO_AF_14, GPIO_PIN_5);
    gpio_af_set(GPIOE, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOE, GPIO_AF_14, GPIO_PIN_4);

    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_2);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_3);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_8);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_9);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_10);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_11);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_12);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_13);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_14);
    gpio_af_set(GPIOH, GPIO_AF_14, GPIO_PIN_15);

    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_0);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_1);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_2);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_4);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_5);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_7);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_9);
    gpio_af_set(GPIOI, GPIO_AF_14, GPIO_PIN_10);

    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_6);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_7);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_10);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_11);
    gpio_af_set(GPIOG, GPIO_AF_14, GPIO_PIN_12);

    gpio_af_set(GPIOF, GPIO_AF_14, GPIO_PIN_10);

    /* configure TLI GPIO */
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6);

    gpio_mode_set(GPIOH, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10
                  | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
    gpio_output_options_set(GPIOH, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_8 | GPIO_PIN_9
                            | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);

    gpio_mode_set(GPIOI, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4
                  | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4
                            | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10);

    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12);

    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* LCD PWM BackLight(PB15) */
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    gpio_bit_set(GPIOB, GPIO_PIN_15);
}
