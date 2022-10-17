/*!
    \file    main.c
    \brief   led spark with systick
    
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
#include "gd32f450i_eval.h"
#include <stdio.h>

typedef enum
{
    CKOUT0_IRC16M_CKOUT1_SYSTEMCLOCK = 0,
    CKOUT0_LXTAL_CKOUT1_PLLI2SR = 1,
    CKOUT0_HXTAL_CKOUT1_HXTAL = 2,
}clock_output_enum;

void rcu_config(void);
void gpio_config(void);
void all_led_init(void);
void all_led_off(void);
void led_flash(int times);
void dac_config(void);
void clock_output_select(uint8_t seq);
void clock_output_config(void);

extern uint8_t g_button_press_flag;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    uint8_t func_seq = 0;
    
    /* initialize the USART */
    gd_eval_com_init(EVAL_COM0);
    
    printf("\r\n /=========== Gigadevice Clock output Demo ===========/ \r\n");
    printf("press tamper key to select clock output source \r\n");
    
    /* initialize the LEDs */
    all_led_init();
    /* initialize the clock output */
    clock_output_config();
    /* initialize the tamper key */
    gd_eval_key_init(KEY_TAMPER, KEY_MODE_EXTI);
   
    while (1){
        if(1 == g_button_press_flag){
            /* if the button is pressed */
            g_button_press_flag = 0;
            /* control the led */
            all_led_off();
            gd_eval_led_on((led_typedef_enum)func_seq);
            /* select the clock output mode */
            clock_output_select(func_seq);
            
            func_seq++;
            func_seq %= 3;
        }
    }
}

/*!
    \brief      all LEDs turn off
    \param[in]  none
    \param[out] none
    \retval     none
*/
void all_led_off(void)
{
    gd_eval_led_off(LED1);
    gd_eval_led_off(LED2);
    gd_eval_led_off(LED3);
}

/*!
    \brief      configure clock output function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void clock_output_config(void)
{
    /* peripheral clock enable */
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_PMU);   
    rcu_plli2s_config(200,2);
    rcu_osci_on(RCU_PLLI2S_CK);
    if(ERROR == rcu_osci_stab_wait(RCU_PLLI2S_CK)){
        printf("RCU_PLLI2S_CK rcu_osci_stab_wait timeout! \r\n");
        while(1);
    }
    /* backup domain write enable */
    pmu_backup_write_enable();
    rcu_lxtal_drive_capability_config(RCU_LXTALDRI_HIGHER_DRIVE);
    rcu_osci_on(RCU_LXTAL);
    if(ERROR == rcu_osci_stab_wait(RCU_LXTAL)){    
        printf("RCU_LXTAL rcu_osci_stab_wait timeout! \r\n");
        while(1);
    }
    /* configure clock output pin */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_mode_set(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_8);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_0, GPIO_PIN_8);
    gpio_af_set(GPIOC, GPIO_AF_0, GPIO_PIN_9);
    
}

/*!
    \brief      select type of clock output
    \param[in]  none
    \param[out] none
    \retval     none
*/
void clock_output_select(uint8_t seq)
{
    switch(seq){
        case CKOUT0_IRC16M_CKOUT1_SYSTEMCLOCK:
            rcu_ckout0_config(RCU_CKOUT0SRC_IRC16M, RCU_CKOUT0_DIV1);
            rcu_ckout1_config(RCU_CKOUT1SRC_SYSTEMCLOCK, RCU_CKOUT1_DIV5);
            printf("CK_OUT0: IRC16M, CK_OUT1: system clock/5 \r\n");
            break;
        case CKOUT0_LXTAL_CKOUT1_PLLI2SR:
            rcu_ckout0_config(RCU_CKOUT0SRC_LXTAL, RCU_CKOUT0_DIV1);
            rcu_ckout1_config(RCU_CKOUT1SRC_PLLI2SR, RCU_CKOUT1_DIV5);
            printf("CK_OUT0: LXTAL, CK_OUT1: PLLI2SR/5 \r\n");
            break;
        case CKOUT0_HXTAL_CKOUT1_HXTAL:
            rcu_ckout0_config(RCU_CKOUT0SRC_HXTAL, RCU_CKOUT0_DIV1);
            rcu_ckout1_config(RCU_CKOUT1SRC_HXTAL, RCU_CKOUT1_DIV1);
            printf("CK_OUT0: HXTAL, CK_OUT1: HXTAL \r\n");
            break;

        default:
            printf("clock output select parameter error! \r\n");
            while(1);
    }
}

/*!
    \brief      initialize the LEDs
    \param[in]  none
    \param[out] none
    \retval     none
*/
void all_led_init(void)
{
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM0, USART_FLAG_TC));
    
    return ch;
}
