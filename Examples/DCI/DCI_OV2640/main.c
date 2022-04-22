/*!
    \file    main.c
    \brief   DCI display demo
    
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
#include "systick.h"
#include <stdio.h>
#include "dci_ov2640.h"

static void lcd_gpio_config(void);
static void lcd_config(void);

uint8_t capture_image[320*240*2];
/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    ov2640_id_struct ov2640id;
    /* configure Systick */
    systick_config();

    dci_ov2640_init();
    dci_ov2640_id_read(&ov2640id); 
    delay_1ms(100);  

    dma_channel_enable(DMA1, DMA_CH7);
    dci_enable();
    dci_capture_enable();
    delay_1ms(100);

    lcd_config();
    tli_layer_enable(LAYER0);
    tli_reload_config(TLI_REQUEST_RELOAD_EN);
    tli_enable();

    while(1){
    }
}

/*!
    \brief      LCD configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void lcd_config(void)
{
    tli_parameter_struct               tli_initstruct;
    tli_layer_parameter_struct         tli_layer_initstruct;

    rcu_periph_clock_enable(RCU_TLI);

    lcd_gpio_config();  
  
    /* enable pixel clock */
    /* configure PLLSAI prescalers for LCD */
    if(ERROR == rcu_pllsai_config(192, 2, 2)){
        while(1){}
    }
    rcu_tli_clock_div_config(RCU_PLLSAIR_DIV4);
    rcu_osci_on(RCU_PLLSAI_CK);
    if(ERROR == rcu_osci_stab_wait(RCU_PLLSAI_CK)){
        while(1){}
    }

    /* TLI initialization */

    /* initialize the horizontal synchronization polarity as active low*/
    tli_initstruct.signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
    /* initialize the vertical synchronization polarity as active low */  
    tli_initstruct.signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
    /* initialize the data enable polarity as active low */ 
    tli_initstruct.signalpolarity_de = TLI_DE_ACTLIVE_LOW;
    /* initialize the pixel clock polarity as input pixel clock */ 
    tli_initstruct.signalpolarity_pixelck = TLI_PIXEL_CLOCK_TLI;
  
    /* timing configuration */
    /* configure horizontal synchronization width */     
    tli_initstruct.synpsz_hpsz = 40;
    /* configure vertical synchronization height */
    tli_initstruct.synpsz_vpsz = 9;
    /* configure accumulated horizontal back porch */
    tli_initstruct.backpsz_hbpsz = 42; 
    /* configure accumulated vertical back porch */
    tli_initstruct.backpsz_vbpsz = 11;  
    /* configure accumulated active width */  
    tli_initstruct.activesz_hasz = 522;
    /* configure accumulated active height */
    tli_initstruct.activesz_vasz = 283;
    /* configure total width */
    tli_initstruct.totalsz_htsz = 524; 
    /* configure total height */
    tli_initstruct.totalsz_vtsz = 285;
  
    /* configure R,G,B component values for LCD background color */
    tli_initstruct.backcolor_red = 0XFF;
    tli_initstruct.backcolor_green = 0XFF;
    tli_initstruct.backcolor_blue = 0XFF; 

    tli_init(&tli_initstruct);

    /* windowing configuration */ 
    tli_layer_initstruct.layer_window_leftpos = 120;
    tli_layer_initstruct.layer_window_rightpos = (120+320 - 1); 
    tli_layer_initstruct.layer_window_toppos =  28;
    tli_layer_initstruct.layer_window_bottompos = (240 +28- 1);

    /* pixel format configuration */           
    tli_layer_initstruct.layer_ppf = LAYER_PPF_RGB565;

    /* alpha constant configuration : the constant alpha for layer 1 is decreased 
    to see the layer 0 in the intersection zone*/
    tli_layer_initstruct.layer_sa = 255;
  
    /* default color configuration (configure A,R,G,B component values) */           
    tli_layer_initstruct.layer_default_blue = 0x0;        
    tli_layer_initstruct.layer_default_green = 0x0;       
    tli_layer_initstruct.layer_default_red = 0x0;         
    tli_layer_initstruct.layer_default_alpha = 0x0;

    /* blending factors */    
    tli_layer_initstruct.layer_acf1 = LAYER_ACF1_PASA;    
    tli_layer_initstruct.layer_acf2 = LAYER_ACF2_PASA;

    /* configure input address : frame buffer is located at FLASH memory */    
    tli_layer_initstruct.layer_frame_bufaddr = (uint32_t)capture_image;

    tli_layer_initstruct.layer_frame_line_length = ((320 * 2) + 3); 
    tli_layer_initstruct.layer_frame_buf_stride_offset = (320 * 2);

    tli_layer_initstruct.layer_frame_total_line_number = 240; 

    tli_layer_init(LAYER0, &tli_layer_initstruct);
    tli_dither_config(TLI_DITHER_ENABLE);
}

/*!
    \brief      LCD GPIO configure
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void lcd_gpio_config(void)
{
    /* GPIO clock enable */
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOE);
    rcu_periph_clock_enable(RCU_GPIOH);
    rcu_periph_clock_enable(RCU_GPIOI);
    rcu_periph_clock_enable(RCU_GPIOG);
    rcu_periph_clock_enable(RCU_GPIOF);

    /* TLI pins AF configure */
    gpio_af_set(GPIOE,GPIO_AF_14,GPIO_PIN_5); 
    gpio_af_set(GPIOE,GPIO_AF_14,GPIO_PIN_6);  
    gpio_af_set(GPIOE,GPIO_AF_14,GPIO_PIN_4);
    
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_2);  
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_3);
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_8);
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_9);  
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_10); 
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_11);  
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_12); 
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_13);  
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_14);     
    gpio_af_set(GPIOH,GPIO_AF_14,GPIO_PIN_15); 
    
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_0);  
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_1); 
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_2);     
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_4); 
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_5);  
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_6);     
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_7);
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_9);     
    gpio_af_set(GPIOI,GPIO_AF_14,GPIO_PIN_10);
    
    gpio_af_set(GPIOG,GPIO_AF_14,GPIO_PIN_6);     
    gpio_af_set(GPIOG,GPIO_AF_14,GPIO_PIN_7);
    gpio_af_set(GPIOG,GPIO_AF_14,GPIO_PIN_10);
    gpio_af_set(GPIOG,GPIO_AF_14,GPIO_PIN_11);     
    gpio_af_set(GPIOG,GPIO_AF_14,GPIO_PIN_12);    

    gpio_af_set(GPIOF,GPIO_AF_14,GPIO_PIN_10);

    /* configure HSYNC(PI10), VSYNC(PI9), PCLK(PG7) */
    /* configure LCD_R7(PG6), LCD_R6(PH12), LCD_R5(PH11), LCD_R4(PH10), LCD_R3(PH9),LCD_R2(PH8), 
                 LCD_R1(PH3), LCD_R0(PH2), LCD_G7(PI2), LCD_G6(PI1), LCD_G5(PI0), LCD_G4(PH15), 
                 LCD_G3(PH14), LCD_G2(PH13),LCD_G1(PE6), LCD_G0(PE5),LCD_B7(PI7), LCD_B6(PI6), 
                 LCD_B5(PI5), LCD_B4(PI4), LCD_B3(PG11),LCD_B2(PG10), LCD_B1(PG12), LCD_B0(PE4) */
    gpio_mode_set(GPIOE, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);
    gpio_output_options_set(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6);

    gpio_mode_set(GPIOH, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                 |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);
    gpio_output_options_set(GPIOH, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                           |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

    gpio_mode_set(GPIOI, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                 |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10);
    gpio_output_options_set(GPIOI, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ,GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2
                           |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_9|GPIO_PIN_10);
    
    gpio_mode_set(GPIOG, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);
    gpio_output_options_set(GPIOG, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12);

    gpio_mode_set(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_10);

    /* configure LCD_PWM_BackLight(PB15) */
    gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP,GPIO_OSPEED_50MHZ, GPIO_PIN_15);
    
    gpio_bit_set(GPIOB,GPIO_PIN_15);
}
