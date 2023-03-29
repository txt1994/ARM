/*!
    文件:    gd32f4xx_rtc.c
    简介:   RTC driver

    版本: 2016-08-15, V1.0.0, firmware for GD32F4xx
    版本: 2018-12-12, V2.0.0, firmware for GD32F4xx
    版本: 2020-09-30, V2.1.0, firmware for GD32F4xx
    版本: 2022-03-09, V3.0.0, firmware for GD32F4xx
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


#include "gd32f4xx_rtc.h"

/* RTC timeout value */
#define RTC_WTWF_TIMEOUT                   ((uint32_t)0x00004000U)                    /*!< wakeup timer can be write flag timeout */
#define RTC_INITM_TIMEOUT                  ((uint32_t)0x00004000U)                    /*!< initialization state flag timeout */
#define RTC_RSYNF_TIMEOUT                  ((uint32_t)0x00008000U)                    /*!< register synchronization flag timeout */
#define RTC_HRFC_TIMEOUT                   ((uint32_t)0x20000000U)                    /*!< recalibration pending flag timeout */
#define RTC_SHIFTCTL_TIMEOUT               ((uint32_t)0x00001000U)                    /*!< shift function operation pending flag timeout */
#define RTC_ALRMXWF_TIMEOUT                ((uint32_t)0x00008000U)                    /*!< alarm configuration can be write flag timeout */

/*!
    简介:    reset most of the RTC registers
    参数[输入]:  无
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_DeInit(void) {
    ErrStatus error_status = ERROR;
    volatile uint32_t time_index = RTC_WTWF_TIMEOUT;
    uint32_t flag_status = RESET;
    /* RTC_TAMP register is not under write protection */
    RTC_TAMP = RTC_REGISTER_RESET;

    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* enter init mode */
    error_status = RTC_init_mode_enter();

    if(ERROR != error_status) {
        /* reset RTC_CTL register, but RTC_CTL[2��0] */
        RTC_CTL &= (RTC_REGISTER_RESET | RTC_CTL_WTCS);
        /* before reset RTC_TIME and RTC_DATE, BPSHAD bit in RTC_CTL should be reset as the condition.
           in order to read calendar from shadow register, not the real registers being reset */
        RTC_TIME = RTC_REGISTER_RESET;
        RTC_DATE = RTC_DATE_RESET;

        RTC_PSC = RTC_PSC_RESET;

        /* only when RTC_CTL_WTEN=0 and RTC_STAT_WTWF=1 can write RTC_CTL[2��0] */
        /* wait until the WTWF flag to be set */
        do {
            flag_status = RTC_STAT & RTC_STAT_WTWF;
        } while((--time_index > 0U) && ((uint32_t)RESET == flag_status));

        if((uint32_t)RESET == flag_status) {
            error_status = ERROR;
        } else {
            RTC_CTL &= RTC_REGISTER_RESET;
            RTC_WUT = RTC_WUT_RESET;
            RTC_COSC = RTC_REGISTER_RESET;
            /* to write RTC_ALRMxSS register, ALRMxEN bit in RTC_CTL register should be reset as the condition */
            RTC_ALRM0TD = RTC_REGISTER_RESET;
            RTC_ALRM1TD = RTC_REGISTER_RESET;
            RTC_ALRM0SS = RTC_REGISTER_RESET;
            RTC_ALRM1SS = RTC_REGISTER_RESET;
            /* reset RTC_STAT register, also exit init mode.
               at the same time, RTC_STAT_SOPF bit is reset, as the condition to reset RTC_SHIFTCTL register later */
            RTC_STAT = RTC_STAT_RESET;
            /* reset RTC_SHIFTCTL and RTC_HRFC register, this can be done without the init mode */
            RTC_SHIFTCTL   = RTC_REGISTER_RESET;
            RTC_HRFC       = RTC_REGISTER_RESET;
            error_status = RTC_register_sync_Wait();
        }
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}

/*!
    简介:    initialize RTC registers
    参数[输入]:  RTC_initpara_struct: pointer to a RTC_parameter_struct structure which contains
                parameters for initialization of the rtc peripheral
                members of the structure and the member values are shown as below:
                  year: 0x0 - 0x99(BCD format)
                  month: RTC_JAN, RTC_FEB, RTC_MAR, RTC_APR, RTC_MAY, RTC_JUN,
                             RTC_JUL, RTC_AUG, RTC_SEP, RTC_OCT, RTC_NOV, RTC_DEC
                  date: 0x1 - 0x31(BCD format)
                  day_of_week: RTC_MONDAY, RTC_TUESDAY, RTC_WEDSDAY, RTC_THURSDAY
                                   RTC_FRIDAY, RTC_SATURDAY, RTC_SUNDAY
                  hour: 0x0 - 0x12(BCD format) or 0x0 - 0x23(BCD format) depending on the RTC_display_format chose
                  minute: 0x0 - 0x59(BCD format)
                  second: 0x0 - 0x59(BCD format)
                  factor_asyn: 0x0 - 0x7F
                  factor_syn: 0x0 - 0x7FFF
                  am_pm: RTC_AM, RTC_PM
                  display_format: RTC_24HOUR, RTC_12HOUR
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_init(RTC_parameter_struct *RTC_initpara_struct) {
    ErrStatus error_status = ERROR;
    uint32_t reg_time = 0U, reg_date = 0U;

    reg_date = (DATE_YR(RTC_initpara_struct->year) | \
                DATE_DOW(RTC_initpara_struct->day_of_week) | \
                DATE_MON(RTC_initpara_struct->month) | \
                DATE_DAY(RTC_initpara_struct->date));

    reg_time = (RTC_initpara_struct->am_pm | \
                TIME_HR(RTC_initpara_struct->hour)  | \
                TIME_MN(RTC_initpara_struct->minute) | \
                TIME_SC(RTC_initpara_struct->second));

    /* 1st: disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* 2nd: enter init mode */
    error_status = RTC_init_mode_enter();

    if(ERROR != error_status) {
        RTC_PSC = (uint32_t)(PSC_FACTOR_A(RTC_initpara_struct->factor_asyn) | \
                             PSC_FACTOR_S(RTC_initpara_struct->factor_syn));

        RTC_TIME = (uint32_t)reg_time;
        RTC_DATE = (uint32_t)reg_date;

        RTC_CTL &= (uint32_t)(~RTC_CTL_CS);
        RTC_CTL |=  RTC_initpara_struct->display_format;

        /* 3rd: exit init mode */
        RTC_init_mode_exit();

        /* 4th: wait the RSYNF flag to set */
        error_status = RTC_register_sync_Wait();
    }

    /* 5th:  enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}

/*!
    简介:    enter RTC init mode
    参数[输入]:  无
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_init_mode_enter(void) {
    volatile uint32_t time_index = RTC_INITM_TIMEOUT;
    uint32_t flag_status = RESET;
    ErrStatus error_status = ERROR;

    /* check whether it has been in init mode */
    if((uint32_t)RESET == (RTC_STAT & RTC_STAT_INITF)) {
        RTC_STAT |= RTC_STAT_INITM;

        /* wait until the INITF flag to be set */
        do {
            flag_status = RTC_STAT & RTC_STAT_INITF;
        } while((--time_index > 0U) && ((uint32_t)RESET == flag_status));

        if((uint32_t)RESET != flag_status) {
            error_status = SUCCESS;
        }
    } else {
        error_status = SUCCESS;
    }

    return error_status;
}

/*!
    简介:    exit RTC init mode
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void RTC_init_mode_exit(void) {
    RTC_STAT &= (uint32_t)(~RTC_STAT_INITM);
}

/*!
    简介:    wait until RTC_TIME and RTC_DATE registers are synchronized with APB clock, and the shadow
                registers are updated
    参数[输入]:  无
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_register_sync_Wait(void) {
    volatile uint32_t time_index = RTC_RSYNF_TIMEOUT;
    uint32_t flag_status = RESET;
    ErrStatus error_status = ERROR;

    if((uint32_t)RESET == (RTC_CTL & RTC_CTL_BPSHAD)) {
        /* disable the write protection */
        RTC_WPK = RTC_UNLOCK_KEY1;
        RTC_WPK = RTC_UNLOCK_KEY2;

        /* firstly clear RSYNF flag */
        RTC_STAT &= (uint32_t)(~RTC_STAT_RSYNF);

        /* wait until RSYNF flag to be set */
        do {
            flag_status = RTC_STAT & RTC_STAT_RSYNF;
        } while((--time_index > 0U) && ((uint32_t)RESET == flag_status));

        if((uint32_t)RESET != flag_status) {
            error_status = SUCCESS;
        }

        /* enable the write protection */
        RTC_WPK = RTC_LOCK_KEY;
    } else {
        error_status = SUCCESS;
    }

    return error_status;
}

/*!
    简介:    get current time and date
    参数[输入]:  无
    参数[输出]: RTC_initpara_struct: pointer to a RTC_parameter_struct structure which contains
                parameters for initialization of the rtc peripheral
                members of the structure and the member values are shown as below:
                  year: 0x0 - 0x99(BCD format)
                  month: RTC_JAN, RTC_FEB, RTC_MAR, RTC_APR, RTC_MAY, RTC_JUN,
                             RTC_JUL, RTC_AUG, RTC_SEP, RTC_OCT, RTC_NOV, RTC_DEC
                  date: 0x1 - 0x31(BCD format)
                  day_of_week: RTC_MONDAY, RTC_TUESDAY, RTC_WEDSDAY, RTC_THURSDAY
                                   RTC_FRIDAY, RTC_SATURDAY, RTC_SUNDAY
                  hour: 0x0 - 0x12(BCD format) or 0x0 - 0x23(BCD format) depending on the RTC_display_format chose
                  minute: 0x0 - 0x59(BCD format)
                  second: 0x0 - 0x59(BCD format)
                  factor_asyn: 0x0 - 0x7F
                  factor_syn: 0x0 - 0x7FFF
                  am_pm: RTC_AM, RTC_PM
                  display_format: RTC_24HOUR, RTC_12HOUR
    返回值:      无
*/
void RTC_current_time_get(RTC_parameter_struct *RTC_initpara_struct) {
    uint32_t temp_tr = 0U, temp_dr = 0U, temp_pscr = 0U, temp_ctlr = 0U;

    temp_tr = (uint32_t)RTC_TIME;
    temp_dr = (uint32_t)RTC_DATE;
    temp_pscr = (uint32_t)RTC_PSC;
    temp_ctlr = (uint32_t)RTC_CTL;

    /* get current time and construct RTC_parameter_struct structure */
    RTC_initpara_struct->year = (uint8_t)GET_DATE_YR(temp_dr);
    RTC_initpara_struct->month = (uint8_t)GET_DATE_MON(temp_dr);
    RTC_initpara_struct->date = (uint8_t)GET_DATE_DAY(temp_dr);
    RTC_initpara_struct->day_of_week = (uint8_t)GET_DATE_DOW(temp_dr);
    RTC_initpara_struct->hour = (uint8_t)GET_TIME_HR(temp_tr);
    RTC_initpara_struct->minute = (uint8_t)GET_TIME_MN(temp_tr);
    RTC_initpara_struct->second = (uint8_t)GET_TIME_SC(temp_tr);
    RTC_initpara_struct->factor_asyn = (uint16_t)GET_PSC_FACTOR_A(temp_pscr);
    RTC_initpara_struct->factor_syn = (uint16_t)GET_PSC_FACTOR_S(temp_pscr);
    RTC_initpara_struct->am_pm = (uint32_t)(temp_tr & RTC_TIME_PM);
    RTC_initpara_struct->display_format = (uint32_t)(temp_ctlr & RTC_CTL_CS);
}

/*!
    简介:    get current subsecond value
    参数[输入]:  无
    参数[输出]:  无
    返回值:     current subsecond value
*/
uint32_t RTC_subsecond_get(void) {
    uint32_t reg = 0U;
    /* if BPSHAD bit is reset, reading RTC_SS will lock RTC_TIME and RTC_DATE automatically */
    reg = (uint32_t)RTC_SS;
    /* read RTC_DATE to unlock the 3 shadow registers */
    (void)(RTC_DATE);

    return reg;
}

/*!
    简介:    configure RTC alarm
    参数[输入]:  RTC_alarm: RTC_ALARM0 or RTC_ALARM1
    参数[输入]:  RTC_alarm_time: pointer to a RTC_alarm_struct structure which contains
                parameters for RTC alarm configuration
                members of the structure and the member values are shown as below:
                  alarm_mask: RTC_ALARM_NONE_MASK, RTC_ALARM_DATE_MASK, RTC_ALARM_HOUR_MASK
                                  RTC_ALARM_MINUTE_MASK, RTC_ALARM_SECOND_MASK, RTC_ALARM_ALL_MASK
                  weekday_or_date: RTC_ALARM_DATE_SELECTED, RTC_ALARM_WEEKDAY_SELECTED
                  alarm_day: 1) 0x1 - 0x31(BCD format) if RTC_ALARM_DATE_SELECTED is set
                                 2) RTC_MONDAY, RTC_TUESDAY, RTC_WEDSDAY, RTC_THURSDAY, RTC_FRIDAY,
                                    RTC_SATURDAY, RTC_SUNDAY if RTC_ALARM_WEEKDAY_SELECTED is set
                  alarm_hour: 0x0 - 0x12(BCD format) or 0x0 - 0x23(BCD format) depending on the RTC_display_format
                  alarm_minute: 0x0 - 0x59(BCD format)
                  alarm_second: 0x0 - 0x59(BCD format)
                  am_pm: RTC_AM, RTC_PM
    参数[输出]:  无
    返回值:      无
*/
void RTC_alarm_Config(uint8_t RTC_alarm, RTC_alarm_struct *RTC_alarm_time) {
    uint32_t reg_alrmtd = 0U;

    reg_alrmtd = (RTC_alarm_time->alarm_mask | \
                  RTC_alarm_time->weekday_or_date | \
                  RTC_alarm_time->am_pm | \
                  ALRMTD_DAY(RTC_alarm_time->alarm_day) | \
                  ALRMTD_HR(RTC_alarm_time->alarm_hour) | \
                  ALRMTD_MN(RTC_alarm_time->alarm_minute) | \
                  ALRMTD_SC(RTC_alarm_time->alarm_second));

    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    if(RTC_ALARM0 == RTC_alarm) {
        RTC_ALRM0TD = (uint32_t)reg_alrmtd;

    } else {
        RTC_ALRM1TD = (uint32_t)reg_alrmtd;
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    configure subsecond of RTC alarm
    参数[输入]:  RTC_alarm: RTC_ALARM0 or RTC_ALARM1
    参数[输入]:  mask_subsecond: alarm subsecond mask
      参数:        RTC_MASKSSC_0_14: mask alarm subsecond configuration
      参数:        RTC_MASKSSC_1_14: mask RTC_ALRMXSS_SSC[14:1], and RTC_ALRMXSS_SSC[0] is to be compared
      参数:        RTC_MASKSSC_2_14: mask RTC_ALRMXSS_SSC[14:2], and RTC_ALRMXSS_SSC[1:0] is to be compared
      参数:        RTC_MASKSSC_3_14: mask RTC_ALRMXSS_SSC[14:3], and RTC_ALRMXSS_SSC[2:0] is to be compared
      参数:        RTC_MASKSSC_4_14: mask RTC_ALRMXSS_SSC[14:4]], and RTC_ALRMXSS_SSC[3:0] is to be compared
      参数:        RTC_MASKSSC_5_14: mask RTC_ALRMXSS_SSC[14:5], and RTC_ALRMXSS_SSC[4:0] is to be compared
      参数:        RTC_MASKSSC_6_14: mask RTC_ALRMXSS_SSC[14:6], and RTC_ALRMXSS_SSC[5:0] is to be compared
      参数:        RTC_MASKSSC_7_14: mask RTC_ALRMXSS_SSC[14:7], and RTC_ALRMXSS_SSC[6:0] is to be compared
      参数:        RTC_MASKSSC_8_14: mask RTC_ALRMXSS_SSC[14:8], and RTC_ALRMXSS_SSC[7:0] is to be compared
      参数:        RTC_MASKSSC_9_14: mask RTC_ALRMXSS_SSC[14:9], and RTC_ALRMXSS_SSC[8:0] is to be compared
      参数:        RTC_MASKSSC_10_14: mask RTC_ALRMXSS_SSC[14:10], and RTC_ALRMXSS_SSC[9:0] is to be compared
      参数:        RTC_MASKSSC_11_14: mask RTC_ALRMXSS_SSC[14:11], and RTC_ALRMXSS_SSC[10:0] is to be compared
      参数:        RTC_MASKSSC_12_14: mask RTC_ALRMXSS_SSC[14:12], and RTC_ALRMXSS_SSC[11:0] is to be compared
      参数:        RTC_MASKSSC_13_14: mask RTC_ALRMXSS_SSC[14:13], and RTC_ALRMXSS_SSC[12:0] is to be compared
      参数:        RTC_MASKSSC_14: mask RTC_ALRMXSS_SSC[14], and RTC_ALRMXSS_SSC[13:0] is to be compared
      参数:        RTC_MASKSSC_NONE: mask none, and RTC_ALRMXSS_SSC[14:0] is to be compared
    参数[输入]:  subsecond: alarm subsecond value(0x000 - 0x7FFF)
    参数[输出]:  无
    返回值:      无
*/
void RTC_alarm_subsecond_Config(uint8_t RTC_alarm, uint32_t mask_subsecond, uint32_t subsecond) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    if(RTC_ALARM0 == RTC_alarm) {
        RTC_ALRM0SS = mask_subsecond | subsecond;
    } else {
        RTC_ALRM1SS = mask_subsecond | subsecond;
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    get RTC alarm
    参数[输入]:  RTC_alarm: RTC_ALARM0 or RTC_ALARM1
    参数[输出]: RTC_alarm_time: pointer to a RTC_alarm_struct structure which contains
                parameters for RTC alarm configuration
                members of the structure and the member values are shown as below:
                  alarm_mask: RTC_ALARM_NONE_MASK, RTC_ALARM_DATE_MASK, RTC_ALARM_HOUR_MASK
                                  RTC_ALARM_MINUTE_MASK, RTC_ALARM_SECOND_MASK, RTC_ALARM_ALL_MASK
                  weekday_or_date: RTC_ALARM_DATE_SELECTED, RTC_ALARM_WEEKDAY_SELECTED
                  alarm_day: 1) 0x1 - 0x31(BCD format) if RTC_ALARM_DATE_SELECTED is set
                                 2) RTC_MONDAY, RTC_TUESDAY, RTC_WEDSDAY, RTC_THURSDAY, RTC_FRIDAY,
                                    RTC_SATURDAY, RTC_SUNDAY if RTC_ALARM_WEEKDAY_SELECTED is set
                  alarm_hour: 0x0 - 0x12(BCD format) or 0x0 - 0x23(BCD format) depending on the RTC_display_format
                  alarm_minute: 0x0 - 0x59(BCD format)
                  alarm_second: 0x0 - 0x59(BCD format)
                  am_pm: RTC_AM, RTC_PM
    返回值:      无
*/
void RTC_alarm_get(uint8_t RTC_alarm, RTC_alarm_struct *RTC_alarm_time) {
    uint32_t reg_alrmtd = 0U;

    /* get the value of RTC_ALRM0TD register */
    if(RTC_ALARM0 == RTC_alarm) {
        reg_alrmtd = RTC_ALRM0TD;
    } else {
        reg_alrmtd = RTC_ALRM1TD;
    }

    /* get alarm parameters and construct the RTC_alarm_struct structure */
    RTC_alarm_time->alarm_mask = reg_alrmtd & RTC_ALARM_ALL_MASK;
    RTC_alarm_time->am_pm = (uint32_t)(reg_alrmtd & RTC_ALRMXTD_PM);
    RTC_alarm_time->weekday_or_date = (uint32_t)(reg_alrmtd & RTC_ALRMXTD_DOWS);
    RTC_alarm_time->alarm_day = (uint8_t)GET_ALRMTD_DAY(reg_alrmtd);
    RTC_alarm_time->alarm_hour = (uint8_t)GET_ALRMTD_HR(reg_alrmtd);
    RTC_alarm_time->alarm_minute = (uint8_t)GET_ALRMTD_MN(reg_alrmtd);
    RTC_alarm_time->alarm_second = (uint8_t)GET_ALRMTD_SC(reg_alrmtd);
}

/*!
    简介:    get RTC alarm subsecond
    参数[输入]:  RTC_alarm: RTC_ALARM0 or RTC_ALARM1
    参数[输出]:  无
    返回值:     RTC alarm subsecond value
*/
uint32_t RTC_alarm_subsecond_get(uint8_t RTC_alarm) {
    if(RTC_ALARM0 == RTC_alarm) {
        return ((uint32_t)(RTC_ALRM0SS & RTC_ALRM0SS_SSC));
    } else {
        return ((uint32_t)(RTC_ALRM1SS & RTC_ALRM1SS_SSC));
    }
}

/*!
    简介:    enable RTC alarm
    参数[输入]:  RTC_alarm: RTC_ALARM0 or RTC_ALARM1
    参数[输出]:  无
    返回值:      无
*/
void RTC_alarm_enable(uint8_t RTC_alarm) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    if(RTC_ALARM0 == RTC_alarm) {
        RTC_CTL |= RTC_CTL_ALRM0EN;
    } else {
        RTC_CTL |= RTC_CTL_ALRM1EN;
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    disable RTC alarm
    参数[输入]:  RTC_alarm: RTC_ALARM0 or RTC_ALARM1
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_alarm_disable(uint8_t RTC_alarm) {
    volatile uint32_t time_index = RTC_ALRMXWF_TIMEOUT;
    ErrStatus error_status = ERROR;
    uint32_t flag_status = RESET;

    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* clear the state of alarm */
    if(RTC_ALARM0 == RTC_alarm) {
        RTC_CTL &= (uint32_t)(~RTC_CTL_ALRM0EN);

        /* wait until ALRM0WF flag to be set after the alarm is disabled */
        do {
            flag_status = RTC_STAT & RTC_STAT_ALRM0WF;
        } while((--time_index > 0U) && ((uint32_t)RESET == flag_status));
    } else {
        RTC_CTL &= (uint32_t)(~RTC_CTL_ALRM1EN);

        /* wait until ALRM1WF flag to be set after the alarm is disabled */
        do {
            flag_status = RTC_STAT & RTC_STAT_ALRM1WF;
        } while((--time_index > 0U) && ((uint32_t)RESET == flag_status));
    }

    if((uint32_t)RESET != flag_status) {
        error_status = SUCCESS;
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}

/*!
    简介:    enable RTC time-stamp
    参数[输入]:  edge: specify which edge to detect of time-stamp
      参数:        RTC_TIMESTAMP_RISING_EDGE: rising edge is valid event edge for timestamp event
      参数:        RTC_TIMESTAMP_FALLING_EDGE: falling edge is valid event edge for timestamp event
    参数[输出]:  无
    返回值:      无
*/
void RTC_timestamp_enable(uint32_t edge) {
    uint32_t reg_ctl = 0U;

    /* clear the bits to be configured in RTC_CTL */
    reg_ctl = (uint32_t)(RTC_CTL & (uint32_t)(~(RTC_CTL_TSEG | RTC_CTL_TSEN)));

    /* new configuration */
    reg_ctl |= (uint32_t)(edge | RTC_CTL_TSEN);

    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    RTC_CTL = (uint32_t)reg_ctl;

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    disable RTC time-stamp
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void RTC_timestamp_disable(void) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* clear the TSEN bit */
    RTC_CTL &= (uint32_t)(~ RTC_CTL_TSEN);

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    get RTC timestamp time and date
    参数[输入]:  无
    参数[输出]: RTC_timestamp: pointer to a RTC_timestamp_struct structure which contains
                parameters for RTC time-stamp configuration
                members of the structure and the member values are shown as below:
                  timestamp_month: RTC_JAN, RTC_FEB, RTC_MAR, RTC_APR, RTC_MAY, RTC_JUN,
                                       RTC_JUL, RTC_AUG, RTC_SEP, RTC_OCT, RTC_NOV, RTC_DEC
                  timestamp_date: 0x1 - 0x31(BCD format)
                  timestamp_day: RTC_MONDAY, RTC_TUESDAY, RTC_WEDSDAY, RTC_THURSDAY, RTC_FRIDAY,
                                     RTC_SATURDAY, RTC_SUNDAY if RTC_ALARM_WEEKDAY_SELECTED is set
                  timestamp_hour: 0x0 - 0x12(BCD format) or 0x0 - 0x23(BCD format) depending on the RTC_display_format
                  timestamp_minute: 0x0 - 0x59(BCD format)
                  timestamp_second: 0x0 - 0x59(BCD format)
                  am_pm: RTC_AM, RTC_PM
    返回值:      无
*/
void RTC_timestamp_get(RTC_timestamp_struct *RTC_timestamp) {
    uint32_t temp_tts = 0U, temp_dts = 0U;

    /* get the value of time_stamp registers */
    temp_tts = (uint32_t)RTC_TTS;
    temp_dts = (uint32_t)RTC_DTS;

    /* get timestamp time and construct the RTC_timestamp_struct structure */
    RTC_timestamp->am_pm = (uint32_t)(temp_tts & RTC_TTS_PM);
    RTC_timestamp->timestamp_month = (uint8_t)GET_DTS_MON(temp_dts);
    RTC_timestamp->timestamp_date = (uint8_t)GET_DTS_DAY(temp_dts);
    RTC_timestamp->timestamp_day = (uint8_t)GET_DTS_DOW(temp_dts);
    RTC_timestamp->timestamp_hour = (uint8_t)GET_TTS_HR(temp_tts);
    RTC_timestamp->timestamp_minute = (uint8_t)GET_TTS_MN(temp_tts);
    RTC_timestamp->timestamp_second = (uint8_t)GET_TTS_SC(temp_tts);
}

/*!
    简介:    get RTC time-stamp subsecond
    参数[输入]:  无
    参数[输出]:  无
    返回值:     RTC time-stamp subsecond value
*/
uint32_t RTC_timestamp_subsecond_get(void) {
    return ((uint32_t)RTC_SSTS);
}

/*!
    简介:    RTC time-stamp mapping
    参数[输入]:  RTC_af:
      参数:        RTC_AF0_TIMESTAMP: RTC_AF0 use for timestamp
      参数:        RTC_AF1_TIMESTAMP: RTC_AF1 use for timestamp
    参数[输出]:  无
    返回值:      无
*/
void RTC_timestamp_pin_map(uint32_t RTC_af) {
    RTC_TAMP &= ~RTC_TAMP_TSSEL;
    RTC_TAMP |= RTC_af;
}

/*!
    简介:    enable RTC tamper
    参数[输入]:  RTC_tamper: pointer to a RTC_tamper_struct structure which contains
                parameters for RTC tamper configuration
                members of the structure and the member values are shown as below:
                  detecting tamper event can using edge mode or level mode
                  (1) using edge mode configuration:
                  tamper_source: RTC_TAMPER0, RTC_TAMPER1
                  tamper_trigger: RTC_TAMPER_TRIGGER_EDGE_RISING, RTC_TAMPER_TRIGGER_EDGE_FALLING
                  tamper_filter: RTC_FLT_EDGE
                  tamper_with_timestamp: DISABLE, ENABLE
                  (2) using level mode configuration:
                  tamper_source: RTC_TAMPER0, RTC_TAMPER1
                  tamper_trigger:RTC_TAMPER_TRIGGER_LEVEL_LOW, RTC_TAMPER_TRIGGER_LEVEL_HIGH
                  tamper_filter: RTC_FLT_2S, RTC_FLT_4S, RTC_FLT_8S
                  tamper_sample_frequency: RTC_FREQ_DIV32768, RTC_FREQ_DIV16384, RTC_FREQ_DIV8192,
                                               RTC_FREQ_DIV4096, RTC_FREQ_DIV2048, RTC_FREQ_DIV1024,
                                               RTC_FREQ_DIV512, RTC_FREQ_DIV256
                  tamper_precharge_enable: DISABLE, ENABLE
                  tamper_precharge_time: RTC_PRCH_1C, RTC_PRCH_2C, RTC_PRCH_4C, RTC_PRCH_8C
                  tamper_with_timestamp: DISABLE, ENABLE
    参数[输出]:  无
    返回值:      无
*/
void RTC_tamper_enable(RTC_tamper_struct *RTC_tamper) {
    /* disable tamper */
    RTC_TAMP &= (uint32_t)~(RTC_tamper->tamper_source);

    /* tamper filter must be used when the tamper source is voltage level detection */
    RTC_TAMP &= (uint32_t)~RTC_TAMP_FLT;

    /* the tamper source is voltage level detection */
    if((uint32_t)(RTC_tamper->tamper_filter) != RTC_FLT_EDGE) {
        RTC_TAMP &= (uint32_t)~(RTC_TAMP_DISPU | RTC_TAMP_PRCH | RTC_TAMP_FREQ | RTC_TAMP_FLT);

        /* check if the tamper pin need precharge, if need, then configure the precharge time */
        if(DISABLE == RTC_tamper->tamper_precharge_enable) {
            RTC_TAMP |= (uint32_t)RTC_TAMP_DISPU;
        } else {
            RTC_TAMP |= (uint32_t)(RTC_tamper->tamper_precharge_time);
        }

        RTC_TAMP |= (uint32_t)(RTC_tamper->tamper_sample_frequency);
        RTC_TAMP |= (uint32_t)(RTC_tamper->tamper_filter);

        /* configure the tamper trigger */
        RTC_TAMP &= ((uint32_t)~((RTC_tamper->tamper_source) << RTC_TAMPER_TRIGGER_POS));

        if(RTC_TAMPER_TRIGGER_LEVEL_LOW != RTC_tamper->tamper_trigger) {
            RTC_TAMP |= (uint32_t)((RTC_tamper->tamper_source) << RTC_TAMPER_TRIGGER_POS);
        }
    } else {

        /* configure the tamper trigger */
        RTC_TAMP &= ((uint32_t)~((RTC_tamper->tamper_source) << RTC_TAMPER_TRIGGER_POS));

        if(RTC_TAMPER_TRIGGER_EDGE_RISING != RTC_tamper->tamper_trigger) {
            RTC_TAMP |= (uint32_t)((RTC_tamper->tamper_source) << RTC_TAMPER_TRIGGER_POS);
        }
    }

    RTC_TAMP &= (uint32_t)~RTC_TAMP_TPTS;

    if(DISABLE != RTC_tamper->tamper_with_timestamp) {
        /* the tamper event also cause a time-stamp event */
        RTC_TAMP |= (uint32_t)RTC_TAMP_TPTS;
    }

    /* enable tamper */
    RTC_TAMP |= (uint32_t)(RTC_tamper->tamper_source);
}

/*!
    简介:    disable RTC tamper
    参数[输入]:  source: specify which tamper source to be disabled
      参数:        RTC_TAMPER0
      参数:        RTC_TAMPER1
    参数[输出]:  无
    返回值:      无
*/
void RTC_tamper_disable(uint32_t source) {
    /* disable tamper */
    RTC_TAMP &= (uint32_t)~source;

}

/*!
    简介:    RTC tamper0 mapping
    参数[输入]:  RTC_af:
      参数:        RTC_AF0_TAMPER0: RTC_AF0 use for tamper0
      参数:        RTC_AF1_TAMPER0: RTC_AF1 use for tamper0
    参数[输出]:  无
    返回值:      无
*/
void RTC_tamper0_pin_map(uint32_t RTC_af) {
    RTC_TAMP &= ~(RTC_TAMP_TP0EN | RTC_TAMP_TP0SEL);
    RTC_TAMP |= RTC_af;
}

/*!
    简介:    enable specified RTC interrupt
    参数[输入]:  interrupt: specify which interrupt source to be enabled
      参数:        RTC_INT_TIMESTAMP: timestamp interrupt
      参数:        RTC_INT_ALARM0: alarm0 interrupt
      参数:        RTC_INT_ALARM1: alarm1 interrupt
      参数:        RTC_INT_TAMP: tamper detection interrupt
      参数:        RTC_INT_WAKEUP: wakeup timer interrupt
    参数[输出]:  无
    返回值:      无
*/
void RTC_Interrupt_enable(uint32_t interrupt) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* enable the interrupts in RTC_CTL register */
    RTC_CTL |= (uint32_t)(interrupt & (uint32_t)~RTC_TAMP_TPIE);
    /* enable the interrupts in RTC_TAMP register */
    RTC_TAMP |= (uint32_t)(interrupt & RTC_TAMP_TPIE);

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    disble specified RTC interrupt
    参数[输入]:  interrupt: specify which interrupt source to be disabled
      参数:        RTC_INT_TIMESTAMP: timestamp interrupt
      参数:        RTC_INT_ALARM0: alarm interrupt
      参数:        RTC_INT_ALARM1: alarm interrupt
      参数:        RTC_INT_TAMP: tamper detection interrupt
      参数:        RTC_INT_WAKEUP: wakeup timer interrupt
    参数[输出]:  无
    返回值:      无
*/
void RTC_Interrupt_disable(uint32_t interrupt) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* disable the interrupts in RTC_CTL register */
    RTC_CTL &= (uint32_t)~(interrupt & (uint32_t)~RTC_TAMP_TPIE);
    /* disable the interrupts in RTC_TAMP register */
    RTC_TAMP &= (uint32_t)~(interrupt & RTC_TAMP_TPIE);

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    check specified flag
    参数[输入]:  flag: specify which flag to check
      参数:        RTC_STAT_SCP: smooth calibration pending flag
      参数:        RTC_FLAG_TP1: RTC tamper 1 detected flag
      参数:        RTC_FLAG_TP0: RTC tamper 0 detected flag
      参数:        RTC_FLAG_TSOVR: time-stamp overflow flag
      参数:        RTC_FLAG_TS: time-stamp flag
      参数:        RTC_FLAG_ALRM0: alarm0 occurs flag
      参数:        RTC_FLAG_ALRM1: alarm1 occurs flag
      参数:        RTC_FLAG_WT: wakeup timer occurs flag
      参数:        RTC_FLAG_INIT: initialization state flag
      参数:        RTC_FLAG_RSYN: register synchronization flag
      参数:        RTC_FLAG_YCM: year configuration mark status flag
      参数:        RTC_FLAG_SOP: shift function operation pending flag
      参数:        RTC_FLAG_ALRM0W: alarm0 configuration can be write flag
      参数:        RTC_FLAG_ALRM1W: alarm1 configuration can be write flag
      参数:        RTC_FLAG_WTW: wakeup timer can be write flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus RTC_flag_get(uint32_t flag) {
    FlagStatus flag_state = RESET;

    if((uint32_t)RESET != (RTC_STAT & flag)) {
        flag_state = SET;
    }

    return flag_state;
}

/*!
    简介:    clear specified flag
      参数:        RTC_FLAG_TP1: RTC tamper 1 detected flag
      参数:        RTC_FLAG_TP0: RTC tamper 0 detected flag
      参数:        RTC_FLAG_TSOVR: time-stamp overflow flag
      参数:        RTC_FLAG_TS: time-stamp flag
      参数:        RTC_FLAG_WT: wakeup timer occurs flag
      参数:        RTC_FLAG_ALARM0: alarm0 occurs flag
      参数:        RTC_FLAG_ALARM1: alarm1 occurs flag
      参数:        RTC_FLAG_RSYN: register synchronization flag
    参数[输出]:  无
    返回值:      无
*/
void RTC_flag_clear(uint32_t flag) {
    RTC_STAT &= (uint32_t)(~flag);
}

/*!
    简介:    configure rtc alarm output source
    参数[输入]:  source: specify signal to output
      参数:        RTC_ALARM0_HIGH: when the  alarm0 flag is set, the output pin is high
      参数:        RTC_ALARM0_LOW: when the  alarm0 flag is set, the output pin is low
      参数:        RTC_ALARM1_HIGH: when the  alarm1 flag is set, the output pin is high
      参数:        RTC_ALARM1_LOW: when the  alarm1 flag is set, the output pin is low
      参数:        RTC_WAKEUP_HIGH: when the  wakeup flag is set, the output pin is high
      参数:        RTC_WAKEUP_LOW: when the  wakeup flag is set, the output pin is low
    参数[输入]:  mode: specify the output pin mode when output alarm signal
      参数:        RTC_ALARM_OUTPUT_OD: open drain mode
      参数:        RTC_ALARM_OUTPUT_PP: push pull mode
    参数[输出]:  无
    返回值:      无
*/
void RTC_alarm_output_Config(uint32_t source, uint32_t mode) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    RTC_CTL &= ~(RTC_CTL_OS | RTC_CTL_OPOL);
    RTC_TAMP &= ~RTC_TAMP_AOT;

    RTC_CTL |= (uint32_t)(source);
    /* alarm output */
    RTC_TAMP |= (uint32_t)(mode);

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    configure rtc calibration output source
    参数[输入]:  source: specify signal to output
      参数:        RTC_CALIBRATION_512HZ: when the LSE freqency is 32768Hz and the RTC_PSC
                                         is the default value, output 512Hz signal
      参数:        RTC_CALIBRATION_1HZ: when the LSE freqency is 32768Hz and the RTC_PSC
                                       is the default value, output 1Hz signal
    参数[输出]:  无
    返回值:      无
*/
void RTC_calibration_output_Config(uint32_t source) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    RTC_CTL &= (uint32_t)~(RTC_CTL_COEN | RTC_CTL_COS);

    RTC_CTL |= (uint32_t)(source);
    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    adjust the daylight saving time by adding or substracting one hour from the current time
    参数[输入]:  operation: hour adjustment operation
      参数:        RTC_CTL_A1H: add one hour
      参数:        RTC_CTL_S1H: substract one hour
    参数[输出]:  无
    返回值:      无
*/
void RTC_hour_adjust(uint32_t operation) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    RTC_CTL |= (uint32_t)(operation);

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    adjust RTC second or subsecond value of current time
    参数[输入]:  add: add 1s to current time or not
      参数:        RTC_SHIFT_ADD1S_RESET: no effect
      参数:        RTC_SHIFT_ADD1S_SET: add 1s to current time
    参数[输入]:  minus: number of subsecond to minus from current time(0x0 - 0x7FFF)
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_second_adjust(uint32_t add, uint32_t minus) {
    volatile uint32_t time_index = RTC_SHIFTCTL_TIMEOUT;
    ErrStatus error_status = ERROR;
    uint32_t flag_status = RESET;
    uint32_t temp = 0U;
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* check if a shift operation is ongoing */
    do {
        flag_status = RTC_STAT & RTC_STAT_SOPF;
    } while((--time_index > 0U) && ((uint32_t)RESET != flag_status));

    /* check if the function of reference clock detection is disabled */
    temp = RTC_CTL & RTC_CTL_REFEN;

    if((RESET == flag_status) && (RESET == temp)) {
        RTC_SHIFTCTL = (uint32_t)(add | SHIFTCTL_SFS(minus));
        error_status = RTC_register_sync_Wait();
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}

/*!
    简介:    enable RTC bypass shadow registers function
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void RTC_bypass_shadow_enable(void) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    RTC_CTL |= RTC_CTL_BPSHAD;

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    disable RTC bypass shadow registers function
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void RTC_bypass_shadow_disable(void) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    RTC_CTL &= ~RTC_CTL_BPSHAD;

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    enable RTC reference clock detection function
    参数[输入]:  无
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_refclock_detection_enable(void) {
    ErrStatus error_status = ERROR;

    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* enter init mode */
    error_status = RTC_init_mode_enter();

    if(ERROR != error_status) {
        RTC_CTL |= (uint32_t)RTC_CTL_REFEN;
        /* exit init mode */
        RTC_init_mode_exit();
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}

/*!
    简介:    disable RTC reference clock detection function
    参数[输入]:  无
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_refclock_detection_disable(void) {
    ErrStatus error_status = ERROR;

    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* enter init mode */
    error_status = RTC_init_mode_enter();

    if(ERROR != error_status) {
        RTC_CTL &= (uint32_t)~RTC_CTL_REFEN;
        /* exit init mode */
        RTC_init_mode_exit();
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}

/*!
    简介:    enable RTC auto wakeup function
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void RTC_wakeup_enable(void) {
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    RTC_CTL |= RTC_CTL_WTEN;

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
}

/*!
    简介:    disable RTC auto wakeup function
    参数[输入]:  无
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_wakeup_disable(void) {
    ErrStatus error_status = ERROR;
    volatile uint32_t time_index = RTC_WTWF_TIMEOUT;
    uint32_t flag_status = RESET;
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;
    RTC_CTL &= ~RTC_CTL_WTEN;

    /* wait until the WTWF flag to be set */
    do {
        flag_status = RTC_STAT & RTC_STAT_WTWF;
    } while((--time_index > 0U) && ((uint32_t)RESET == flag_status));

    if((uint32_t)RESET == flag_status) {
        error_status = ERROR;
    } else {
        error_status = SUCCESS;
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
    return error_status;
}

/*!
    简介:    set RTC auto wakeup timer clock
    参数[输入]:  wakeup_clock:
      参数:        WAKEUP_RTCCK_DIV16: RTC auto wakeup timer clock is RTC clock divided by 16
      参数:        WAKEUP_RTCCK_DIV8: RTC auto wakeup timer clock is RTC clock divided by 8
      参数:        WAKEUP_RTCCK_DIV4: RTC auto wakeup timer clock is RTC clock divided by 4
      参数:        WAKEUP_RTCCK_DIV2: RTC auto wakeup timer clock is RTC clock divided by 2
      参数:        WAKEUP_CKSPRE: RTC auto wakeup timer clock is ckspre
      参数:        WAKEUP_CKSPRE_2EXP16: RTC auto wakeup timer clock is ckspre and wakeup timer add 2exp16
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_wakeup_clock_set(uint8_t wakeup_clock) {
    ErrStatus error_status = ERROR;
    volatile uint32_t time_index = RTC_WTWF_TIMEOUT;
    uint32_t flag_status = RESET;
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* only when RTC_CTL_WTEN=0 and RTC_STAT_WTWF=1 can write RTC_CTL[2��0] */
    /* wait until the WTWF flag to be set */
    do {
        flag_status = RTC_STAT & RTC_STAT_WTWF;
    } while((--time_index > 0U) && ((uint32_t)RESET == flag_status));

    if((uint32_t)RESET == flag_status) {
        error_status = ERROR;
    } else {
        RTC_CTL &= (uint32_t)~ RTC_CTL_WTCS;
        RTC_CTL |= (uint32_t)wakeup_clock;
        error_status = SUCCESS;
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}

/*!
    简介:    set wakeup timer value
    参数[输入]:  wakeup_timer: 0x0000-0xffff
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_wakeup_TIMER_set(uint16_t wakeup_timer) {
    ErrStatus error_status = ERROR;
    volatile uint32_t time_index = RTC_WTWF_TIMEOUT;
    uint32_t flag_status = RESET;
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* wait until the WTWF flag to be set */
    do {
        flag_status = RTC_STAT & RTC_STAT_WTWF;
    } while((--time_index > 0U) && ((uint32_t)RESET == flag_status));

    if((uint32_t)RESET == flag_status) {
        error_status = ERROR;
    } else {
        RTC_WUT = (uint32_t)wakeup_timer;
        error_status = SUCCESS;
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
    return error_status;
}

/*!
    简介:    get wakeup timer value
    参数[输入]:  无
    参数[输出]:  无
    返回值:     wakeup timer value
*/
uint16_t RTC_wakeup_TIMER_get(void) {
    return (uint16_t)RTC_WUT;
}

/*!
    简介:    configure RTC smooth calibration
    参数[输入]:  window: select calibration window
      参数:        RTC_CALIBRATION_WINDOW_32S: 2exp20 RTCCLK cycles, 32s if RTCCLK = 32768 Hz
      参数:        RTC_CALIBRATION_WINDOW_16S: 2exp19 RTCCLK cycles, 16s if RTCCLK = 32768 Hz
      参数:        RTC_CALIBRATION_WINDOW_8S: 2exp18 RTCCLK cycles, 8s if RTCCLK = 32768 Hz
    参数[输入]:  plus: add RTC clock or not
      参数:        RTC_CALIBRATION_PLUS_SET: add one RTC clock every 2048 rtc clock
      参数:        RTC_CALIBRATION_PLUS_RESET: no effect
    参数[输入]:  minus: the RTC clock to minus during the calibration window(0x0 - 0x1FF)
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_smooth_calibration_Config(uint32_t window, uint32_t plus, uint32_t minus) {
    volatile uint32_t time_index = RTC_HRFC_TIMEOUT;
    ErrStatus error_status = ERROR;
    uint32_t flag_status = RESET;

    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* check if a smooth calibration operation is ongoing */
    do {
        flag_status = RTC_STAT & RTC_STAT_SCPF;
    } while((--time_index > 0U) && ((uint32_t)RESET != flag_status));

    if((uint32_t)RESET == flag_status) {
        RTC_HRFC = (uint32_t)(window | plus | HRFC_CMSK(minus));
        error_status = SUCCESS;
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}

/*!
    简介:    enable RTC coarse calibration
    参数[输入]:  无
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_coarse_calibration_enable(void) {
    ErrStatus error_status = ERROR;
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;
    /* enter init mode */
    error_status = RTC_init_mode_enter();

    if(ERROR != error_status) {
        RTC_CTL |= (uint32_t)RTC_CTL_CCEN;
        /* exit init mode */
        RTC_init_mode_exit();
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
    return error_status;
}

/*!
    简介:    disable RTC coarse calibration
    参数[输入]:  无
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_coarse_calibration_disable(void) {
    ErrStatus error_status = ERROR;
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;
    /* enter init mode */
    error_status = RTC_init_mode_enter();

    if(ERROR != error_status) {
        RTC_CTL &= (uint32_t)~RTC_CTL_CCEN;
        /* exit init mode */
        RTC_init_mode_exit();
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;
    return error_status;
}

/*!
    简介:    config coarse calibration direction and step
    参数[输入]:  direction: CALIB_INCREASE or CALIB_DECREASE
    参数[输入]:  step: 0x00-0x1F
                COSD=0:
                  0x00:+0 PPM
                  0x01:+4 PPM
                  0x02:+8 PPM
                  ....
                  0x1F:+126 PPM
                COSD=1:
                  0x00:-0 PPM
                  0x01:-2 PPM
                  0x02:-4 PPM
                  ....
                  0x1F:-63 PPM
    参数[输出]:  无
    返回值:     ErrStatus: ERROR or SUCCESS
*/
ErrStatus RTC_coarse_calibration_Config(uint8_t direction, uint8_t step) {
    ErrStatus error_status = ERROR;
    /* disable the write protection */
    RTC_WPK = RTC_UNLOCK_KEY1;
    RTC_WPK = RTC_UNLOCK_KEY2;

    /* enter init mode */
    error_status = RTC_init_mode_enter();

    if(ERROR != error_status) {
        if(CALIB_DECREASE == direction) {
            RTC_COSC |= (uint32_t)RTC_COSC_COSD;
        } else {
            RTC_COSC &= (uint32_t)~RTC_COSC_COSD;
        }

        RTC_COSC &= ~RTC_COSC_COSS;
        RTC_COSC |= (uint32_t)((uint32_t)step & 0x1FU);
        /* exit init mode */
        RTC_init_mode_exit();
    }

    /* enable the write protection */
    RTC_WPK = RTC_LOCK_KEY;

    return error_status;
}
