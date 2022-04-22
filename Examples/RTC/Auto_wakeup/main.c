/*!
    \file    main.c
    \brief   RTC auto wakeup demo

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

#define RTC_CLOCK_SOURCE_LXTAL
#define BKP_VALUE    0x32F0

rtc_timestamp_struct rtc_timestamp;
rtc_parameter_struct rtc_initpara;
__IO uint32_t prescaler_a = 0, prescaler_s = 0;

void rtc_setup(void);
void rtc_show_time(void);
uint8_t usart_input_threshold(uint32_t value);
void rtc_pre_config(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    gd_eval_com_init(EVAL_COM0);
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    printf("\n\r  ****************** RTC time-stamp demo ******************\n\r");
    gd_eval_key_init(KEY_TAMPER,KEY_MODE_GPIO);

    /* enable access to RTC registers in Backup domain */
    rcu_periph_clock_enable(RCU_PMU);
    pmu_backup_write_enable();

    rtc_pre_config();

    /* check if RTC has aready been configured */
    if (BKP_VALUE != RTC_BKP0){
        rtc_setup();
    }else{
        /* detect the reset source */
        if (RESET != rcu_flag_get(RCU_FLAG_PORRST)){
            printf("power on reset occurred....\n\r");
        }else if (RESET != rcu_flag_get(RCU_FLAG_EPRST)){
            printf("external reset occurred....\n\r");
        }
        printf("no need to configure RTC....\n\r");

        rtc_show_time();
    }

    rcu_all_reset_flag_clear();

    gd_eval_led_init(LED1);
    gd_eval_led_on(LED1);

    exti_flag_clear(EXTI_22);
    exti_init(EXTI_22,EXTI_INTERRUPT,EXTI_TRIG_RISING);
    nvic_irq_enable(RTC_WKUP_IRQn,0,0);

    /* clear wakeup timer occurs flag */
    rtc_flag_clear(RTC_FLAG_WT);
    /* RTC wakeup configuration */
    rtc_interrupt_enable(RTC_INT_WAKEUP);
    /* set wakeup clock as ck_spre(default 1Hz) */
    rtc_wakeup_clock_set(WAKEUP_CKSPRE);
    /* every 5+1 ck_spre period set wakeup flag */
    rtc_wakeup_timer_set(0x05);
    rtc_wakeup_enable();

    while (1);
}

/*!
    \brief      RTC configuration function
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_pre_config(void)
{
    #if defined (RTC_CLOCK_SOURCE_IRC32K)
          rcu_osci_on(RCU_IRC32K);
          rcu_osci_stab_wait(RCU_IRC32K);
          rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);

          prescaler_s = 0x13F;
          prescaler_a = 0x63;
    #elif defined (RTC_CLOCK_SOURCE_LXTAL)
          rcu_osci_on(RCU_LXTAL);
          rcu_osci_stab_wait(RCU_LXTAL);
          rcu_rtc_clock_config(RCU_RTCSRC_LXTAL);
          prescaler_s = 0xFF;
          prescaler_a = 0x7F;
    #else
    #error RTC clock source should be defined.
    #endif /* RTC_CLOCK_SOURCE_IRC32K */

    rcu_periph_clock_enable(RCU_RTC);
    rtc_register_sync_wait();
}

/*!
    \brief      use hyperterminal to setup RTC time and alarm
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_setup(void)
{
    /* setup RTC time value */
    uint32_t tmp_hh = 0xFF, tmp_mm = 0xFF, tmp_ss = 0xFF;

    rtc_initpara.factor_asyn = prescaler_a;
    rtc_initpara.factor_syn = prescaler_s;
    rtc_initpara.year = 0x16;
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = 0x30;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;

    /* current time input */
    printf("=======Configure RTC Time========\n\r");
    printf("  please input hour:\n\r");
    while (0xFF == tmp_hh){
        tmp_hh = usart_input_threshold(23);
        rtc_initpara.hour = tmp_hh;
    }
    printf("  %0.2x\n\r", tmp_hh);

    printf("  please input minute:\n\r");
    while (0xFF == tmp_mm){
        tmp_mm = usart_input_threshold(59);
        rtc_initpara.minute = tmp_mm;
    }
    printf("  %0.2x\n\r", tmp_mm);

    printf("  please input second:\n\r");
    while (0xFF == tmp_ss){
        tmp_ss = usart_input_threshold(59);
        rtc_initpara.second = tmp_ss;
    }
    printf("  %0.2x\n\r", tmp_ss);

    /* RTC current time configuration */
    if(ERROR == rtc_init(&rtc_initpara)){
        printf("\n\r** RTC time configuration failed! **\n\r");
    }else{
        printf("\n\r** RTC time configuration success! **\n\r");
        rtc_show_time();
        RTC_BKP0 = BKP_VALUE;
    }
}

/*!
    \brief      display the current time
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_show_time(void)
{
    rtc_current_time_get(&rtc_initpara);

    printf("Current time: %0.2x:%0.2x:%0.2x \n\r",
          rtc_initpara.hour, rtc_initpara.minute, rtc_initpara.second);
}

/*!
    \brief      get the input character string and check if it is valid
    \param[in]  none
    \param[out] none
    \retval     input value in BCD mode
*/
uint8_t usart_input_threshold(uint32_t value)
{
    uint32_t index = 0;
    uint32_t tmp[2] = {0, 0};

    while (index < 2){
        while (RESET == usart_flag_get(EVAL_COM0, USART_FLAG_RBNE));
        tmp[index++] = usart_data_receive(EVAL_COM0);
        if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39)){
            printf("\n\r please input a valid number between 0 and 9 \n\r");
            index--;
        }
    }

    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
    if (index > value){
        printf("\n\r please input a valid number between 0 and %d \n\r", value);
        return 0xFF;
    }

    index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) <<4);
    return index;
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM0, (uint8_t) ch);
    while (RESET == usart_flag_get(EVAL_COM0,USART_FLAG_TBE));
    return ch;
}
