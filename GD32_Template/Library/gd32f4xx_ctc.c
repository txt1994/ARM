/*!
    文件:    gd32f4xx_ctc.c
    简介:   CTC driver

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

#include "gd32f4xx_ctc.h"

#define CTC_FLAG_MASK            ((uint32_t)0x00000700U)

/* CTC register bit offset */
#define CTC_TRIMVALUE_OFFSET     ((uint32_t)8U)
#define CTC_TRIM_VALUE_OFFSET    ((uint32_t)8U)
#define CTC_REFCAP_OFFSET        ((uint32_t)16U)
#define CTC_LIMIT_VALUE_OFFSET   ((uint32_t)16U)

/*!
    简介:    reset CTC clock trim controller
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void ctc_deinit(void) {
    /* reset CTC */
    rcu_periph_reset_enable(RCU_CTCRST);
    rcu_periph_reset_disable(RCU_CTCRST);
}

/*!
    简介:    enable CTC trim counter
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void ctc_counter_enable(void) {
    CTC_CTL0 |= (uint32_t)CTC_CTL0_CNTEN;
}

/*!
    简介:    disable CTC trim counter
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void ctc_counter_disable(void) {
    CTC_CTL0 &= (uint32_t)(~CTC_CTL0_CNTEN);
}

/*!
    简介:    configure the IRC48M trim value
    参数[输入]:  ctc_trim_value: 8-bit IRC48M trim value
      参数:        0x00 - 0x3F
    参数[输出]:  无
    返回值:      无
*/
void ctc_irc48m_trim_value_config(uint8_t trim_value) {
    /* clear TRIMVALUE bits */
    CTC_CTL0 &= (~(uint32_t)CTC_CTL0_TRIMVALUE);
    /* set TRIMVALUE bits */
    CTC_CTL0 |= ((uint32_t)trim_value << CTC_TRIM_VALUE_OFFSET);
}

/*!
    简介:    generate software reference source sync pulse
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void ctc_software_refsource_pulse_generate(void) {
    CTC_CTL0 |= (uint32_t)CTC_CTL0_SWREFPUL;
}

/*!
    简介:    configure hardware automatically trim mode
    参数[输入]:  hardmode:
                only one parameter can be selected which is shown as below:
      参数:        CTC_HARDWARE_TRIM_MODE_ENABLE: hardware automatically trim mode enable
      参数:        CTC_HARDWARE_TRIM_MODE_DISABLE: hardware automatically trim mode disable
    参数[输出]:  无
    返回值:      无
*/
void ctc_hardware_trim_mode_config(uint32_t hardmode) {
    CTC_CTL0 &= (uint32_t)(~CTC_CTL0_AUTOTRIM);
    CTC_CTL0 |= (uint32_t)hardmode;
}

/*!
    简介:    configure reference signal source polarity
    参数[输入]:  polarity:
                only one parameter can be selected which is shown as below:
      参数:        CTC_REFSOURCE_POLARITY_FALLING: reference signal source polarity is falling edge
      参数:        CTC_REFSOURCE_POLARITY_RISING: reference signal source polarity is rising edge
    参数[输出]:  无
    返回值:      无
*/
void ctc_refsource_polarity_config(uint32_t polarity) {
    CTC_CTL1 &= (uint32_t)(~CTC_CTL1_REFPOL);
    CTC_CTL1 |= (uint32_t)polarity;
}

/*!
    简介:    select reference signal source
    参数[输入]:  refs:
                only one parameter can be selected which is shown as below:
      参数:        CTC_REFSOURCE_GPIO: GPIO is selected
      参数:        CTC_REFSOURCE_LXTAL: LXTAL is selected
    参数[输出]:  无
    返回值:      无
*/
void ctc_refsource_signal_select(uint32_t refs) {
    CTC_CTL1 &= (uint32_t)(~CTC_CTL1_REFSEL);
    CTC_CTL1 |= (uint32_t)refs;
}

/*!
    简介:    configure reference signal source prescaler
    参数[输入]:  prescaler:
                only one parameter can be selected which is shown as below:
      参数:        CTC_REFSOURCE_PSC_OFF: reference signal not divided
      参数:        CTC_REFSOURCE_PSC_DIV2: reference signal divided by 2
      参数:        CTC_REFSOURCE_PSC_DIV4: reference signal divided by 4
      参数:        CTC_REFSOURCE_PSC_DIV8: reference signal divided by 8
      参数:        CTC_REFSOURCE_PSC_DIV16: reference signal divided by 16
      参数:        CTC_REFSOURCE_PSC_DIV32: reference signal divided by 32
      参数:        CTC_REFSOURCE_PSC_DIV64: reference signal divided by 64
      参数:        CTC_REFSOURCE_PSC_DIV128: reference signal divided by 128
    参数[输出]:  无
    返回值:      无
*/
void ctc_refsource_prescaler_config(uint32_t prescaler) {
    CTC_CTL1 &= (uint32_t)(~CTC_CTL1_REFPSC);
    CTC_CTL1 |= (uint32_t)prescaler;
}

/*!
    简介:    configure clock trim base limit value
    参数[输入]:  limit_value: 8-bit clock trim base limit value
      参数:        0x00 - 0xFF
    参数[输出]:  无
    返回值:      无
*/
void ctc_clock_limit_value_config(uint8_t limit_value) {
    CTC_CTL1 &= (uint32_t)(~CTC_CTL1_CKLIM);
    CTC_CTL1 |= (uint32_t)((uint32_t)limit_value << CTC_LIMIT_VALUE_OFFSET);
}

/*!
    简介:    configure CTC counter reload value
    参数[输入]:  reload_value: 16-bit CTC counter reload value
      参数:        0x0000 - 0xFFFF
    参数[输出]:  无
    返回值:      无
*/
void ctc_counter_reload_value_config(uint16_t reload_value) {
    CTC_CTL1 &= (uint32_t)(~CTC_CTL1_RLVALUE);
    CTC_CTL1 |= (uint32_t)reload_value;
}

/*!
    简介:    read CTC counter capture value when reference sync pulse occurred
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the 16-bit CTC counter capture value
*/
uint16_t ctc_counter_capture_value_read(void) {
    uint16_t capture_value = 0U;
    capture_value = (uint16_t)((CTC_STAT & CTC_STAT_REFCAP) >> CTC_REFCAP_OFFSET);
    return (capture_value);
}

/*!
    简介:    read CTC trim counter direction when reference sync pulse occurred
    参数[输入]:  无
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
      参数:        SET: CTC trim counter direction is down-counting
      参数:        RESET: CTC trim counter direction is up-counting
*/
FlagStatus ctc_counter_direction_read(void) {
    if(RESET != (CTC_STAT & CTC_STAT_REFDIR)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    read CTC counter reload value
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the 16-bit CTC counter reload value
*/
uint16_t ctc_counter_reload_value_read(void) {
    uint16_t reload_value = 0U;
    reload_value = (uint16_t)(CTC_CTL1 & CTC_CTL1_RLVALUE);
    return (reload_value);
}

/*!
    简介:    read the IRC48M trim value
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the 8-bit IRC48M trim value
*/
uint8_t ctc_irc48m_trim_value_read(void) {
    uint8_t trim_value = 0U;
    trim_value = (uint8_t)((CTC_CTL0 & CTC_CTL0_TRIMVALUE) >> CTC_TRIMVALUE_OFFSET);
    return (trim_value);
}

/*!
    简介:    enable the CTC interrupt
    参数[输入]:  interrupt: CTC interrupt enable
                one or more parameters can be selected which are shown as below:
      参数:        CTC_INT_CKOK: clock trim OK interrupt enable
      参数:        CTC_INT_CKWARN: clock trim warning interrupt enable
      参数:        CTC_INT_ERR: error interrupt enable
      参数:        CTC_INT_EREF: expect reference interrupt enable
    参数[输出]:  无
    返回值:      无
*/
void ctc_interrupt_enable(uint32_t interrupt) {
    CTC_CTL0 |= (uint32_t)interrupt;
}

/*!
    简介:    disable the CTC interrupt
    参数[输入]:  interrupt: CTC interrupt enable source
                one or more parameters can be selected which are shown as below:
      参数:        CTC_INT_CKOK: clock trim OK interrupt enable
      参数:        CTC_INT_CKWARN: clock trim warning interrupt enable
      参数:        CTC_INT_ERR: error interrupt enable
      参数:        CTC_INT_EREF: expect reference interrupt enable
    参数[输出]:  无
    返回值:      无
*/
void ctc_interrupt_disable(uint32_t interrupt) {
    CTC_CTL0 &= (uint32_t)(~interrupt);
}

/*!
    简介:    get CTC interrupt flag
    参数[输入]:  int_flag: the CTC interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        CTC_INT_FLAG_CKOK: clock trim OK interrupt
      参数:        CTC_INT_FLAG_CKWARN: clock trim warning interrupt
      参数:        CTC_INT_FLAG_ERR: error interrupt
      参数:        CTC_INT_FLAG_EREF: expect reference interrupt
      参数:        CTC_INT_FLAG_CKERR: clock trim error bit interrupt
      参数:        CTC_INT_FLAG_REFMISS: reference sync pulse miss interrupt
      参数:        CTC_INT_FLAG_TRIMERR: trim value error interrupt
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus ctc_interrupt_flag_get(uint32_t int_flag) {
    uint32_t interrupt_flag = 0U, intenable = 0U;

    /* check whether the interrupt is enabled */
    if(RESET != (int_flag & CTC_FLAG_MASK)) {
        intenable = CTC_CTL0 & CTC_CTL0_ERRIE;
    } else {
        intenable = CTC_CTL0 & int_flag;
    }

    /* get interrupt flag status */
    interrupt_flag = CTC_STAT & int_flag;

    if(interrupt_flag && intenable) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear CTC interrupt flag
    参数[输入]:  int_flag: the CTC interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        CTC_INT_FLAG_CKOK: clock trim OK interrupt
      参数:        CTC_INT_FLAG_CKWARN: clock trim warning interrupt
      参数:        CTC_INT_FLAG_ERR: error interrupt
      参数:        CTC_INT_FLAG_EREF: expect reference interrupt
      参数:        CTC_INT_FLAG_CKERR: clock trim error bit interrupt
      参数:        CTC_INT_FLAG_REFMISS: reference sync pulse miss interrupt
      参数:        CTC_INT_FLAG_TRIMERR: trim value error interrupt
    参数[输出]:  无
    返回值:      无
*/
void ctc_interrupt_flag_clear(uint32_t int_flag) {
    if(RESET != (int_flag & CTC_FLAG_MASK)) {
        CTC_INTC |= CTC_INTC_ERRIC;
    } else {
        CTC_INTC |= int_flag;
    }
}

/*!
    简介:    get CTC flag
    参数[输入]:  flag: the CTC flag
                only one parameter can be selected which is shown as below:
      参数:        CTC_FLAG_CKOK: clock trim OK flag
      参数:        CTC_FLAG_CKWARN: clock trim warning flag
      参数:        CTC_FLAG_ERR: error flag
      参数:        CTC_FLAG_EREF: expect reference flag
      参数:        CTC_FLAG_CKERR: clock trim error bit
      参数:        CTC_FLAG_REFMISS: reference sync pulse miss
      参数:        CTC_FLAG_TRIMERR: trim value error bit
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus ctc_flag_get(uint32_t flag) {
    if(RESET != (CTC_STAT & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear CTC flag
    参数[输入]:  flag: the CTC flag
                only one parameter can be selected which is shown as below:
      参数:        CTC_FLAG_CKOK: clock trim OK flag
      参数:        CTC_FLAG_CKWARN: clock trim warning flag
      参数:        CTC_FLAG_ERR: error flag
      参数:        CTC_FLAG_EREF: expect reference flag
      参数:        CTC_FLAG_CKERR: clock trim error bit
      参数:        CTC_FLAG_REFMISS: reference sync pulse miss
      参数:        CTC_FLAG_TRIMERR: trim value error bit
    参数[输出]:  无
    返回值:      无
*/
void ctc_flag_clear(uint32_t flag) {
    if(RESET != (flag & CTC_FLAG_MASK)) {
        CTC_INTC |= CTC_INTC_ERRIC;
    } else {
        CTC_INTC |= flag;
    }
}
