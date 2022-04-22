/*!
    \file    main.c
    \brief   deepsleep wakeup through RTC alarm interrupt

    \version 2021-12-31, V1.0.0, firmware for GD32F4xx
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
#include "systick.h"
#include "main.h"

void led_config(void);
void nvic_configuration(void);
void rtc_configuration(void);
void fwdgt_configuration(void);
void rtc_alarm_time_configuration(void);

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure nvic */
    nvic_configuration();
    /* configure led */
    led_config();
    /* configure systick */
    systick_config();
    gd_eval_led_on(LED2);
    delay_1ms(2000U);
    gd_eval_led_off(LED2);
    /* configure RTC */
    rtc_configuration();
    /* configure FWDGT */
    fwdgt_configuration();

    while(1) {
        delay_1ms(50U);
        /* PMU enters deepsleep mode */
        pmu_to_deepsleepmode(PMU_LDO_LOWPOWER, PMU_LOWDRIVER_ENABLE, WFI_CMD);
        gd_eval_led_toggle(LED1);
    }
}

/*!
    \brief      configure led
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_config(void)
{
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
}

/*!
    \brief      configure the nested vectored interrupt controller
    \param[in]  none
    \param[out] none
    \retval     none
*/
void nvic_configuration(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(RTC_Alarm_IRQn, 2U, 1U);
}

/*!
    \brief      configure RTC alarm time
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_alarm_time_configuration(void)
{
    rtc_alarm_struct     rtc_alarm;
    rtc_parameter_struct rtc_initpara;

    /* configure current time */
    rtc_initpara.factor_asyn = 0x63;
    rtc_initpara.factor_syn = 0x13F;
    rtc_initpara.year = 0x16;
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = 0x30;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;

    rtc_initpara.hour = 0x00;
    rtc_initpara.minute = 0x00;
    rtc_initpara.second = 0x00;
    rtc_init(&rtc_initpara);
    rtc_alarm_disable(RTC_ALARM0);
    rtc_alarm.alarm_mask = RTC_ALARM_DATE_MASK | RTC_ALARM_HOUR_MASK | RTC_ALARM_MINUTE_MASK;
    rtc_alarm.weekday_or_date = RTC_ALARM_DATE_SELECTED;
    rtc_alarm.alarm_day = 0x31;
    rtc_alarm.am_pm = RTC_AM;

    /* RTC alarm value */
    rtc_alarm.alarm_hour = 0x00;
    rtc_alarm.alarm_minute = 0x00;
    rtc_alarm.alarm_second = ALARM_TIME_INTERVAL;

    /* RTC alarm configuration */
    rtc_alarm_config(RTC_ALARM0, &rtc_alarm);

    rtc_interrupt_enable(RTC_INT_ALARM0);
    rtc_alarm_enable(RTC_ALARM0);
}

/*!
    \brief      configure the RTC
    \param[in]  none
    \param[out] none
    \retval     none
*/
void rtc_configuration(void)
{
    rtc_parameter_struct   rtc_initpara;
    rtc_alarm_struct  rtc_alarm;
    /* enable PMU clocks */
    rcu_periph_clock_enable(RCU_PMU);
    /* allow access to BKP domain */
    pmu_backup_write_enable();
    /* reset backup domain */
    rcu_bkp_reset_enable();
    rcu_bkp_reset_disable();

    /* enable IRC32K */
    rcu_osci_on(RCU_IRC32K);
    /* wait till IRC32K is ready */
    rcu_osci_stab_wait(RCU_IRC32K);
    /* select RCU_IRC32K as RTC clock source */
    rcu_rtc_clock_config(RCU_RTCSRC_IRC32K);
    /* enable RTC Clock */
    rcu_periph_clock_enable(RCU_RTC);

    rtc_initpara.factor_asyn = 0x63;
    rtc_initpara.factor_syn = 0x13F;
    rtc_initpara.year = 0x16;
    rtc_initpara.day_of_week = RTC_SATURDAY;
    rtc_initpara.month = RTC_APR;
    rtc_initpara.date = 0x30;
    rtc_initpara.display_format = RTC_24HOUR;
    rtc_initpara.am_pm = RTC_AM;

    /* configure current time */
    rtc_initpara.hour = 0x00;
    rtc_initpara.minute = 0x00;
    rtc_initpara.second = 0x00;
    rtc_init(&rtc_initpara);
    rtc_alarm_disable(RTC_ALARM0);
    rtc_alarm.alarm_mask = RTC_ALARM_DATE_MASK | RTC_ALARM_HOUR_MASK | RTC_ALARM_MINUTE_MASK;
    rtc_alarm.weekday_or_date = RTC_ALARM_DATE_SELECTED;
    rtc_alarm.alarm_day = 0x31;
    rtc_alarm.am_pm = RTC_AM;

    /* RTC alarm value */
    rtc_alarm.alarm_hour = 0x00;
    rtc_alarm.alarm_minute = 0x00;
    rtc_alarm.alarm_second = ALARM_TIME_INTERVAL;

    /* configure RTC alarm0 */
    rtc_alarm_config(RTC_ALARM0, &rtc_alarm);

    rtc_interrupt_enable(RTC_INT_ALARM0);
    rtc_alarm_enable(RTC_ALARM0);

    /* configure EXTI */
    exti_deinit();
    exti_init(EXTI_17, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    rtc_flag_clear(RTC_FLAG_ALRM0);
    exti_interrupt_flag_clear(EXTI_17);
    exti_interrupt_enable(EXTI_17);
}

/*!
    \brief      configure the FWDGT
    \param[in]  none
    \param[out] none
    \retval     none
*/
void fwdgt_configuration(void)
{
    /* configure FWDGT counter clock: 32KHz(IRC32K) / 256 = 0.125 KHz */
    fwdgt_config(1250, FWDGT_PSC_DIV256);
    while(FWDGT_STAT);
    /* after 10 seconds to generate a reset */
    fwdgt_enable();
}
