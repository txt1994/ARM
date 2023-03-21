/*!
    文件:    gd32f4xx_wwdgt.c
    简介:   WWDGT driver

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

#include "gd32f4xx_wwdgt.h"

/*!
    简介:    reset the window watchdog timer configuration
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void wwdgt_deinit(void) {
    rcu_periph_reset_enable(RCU_WWDGTRST);
    rcu_periph_reset_disable(RCU_WWDGTRST);
}

/*!
    简介:    start the window watchdog timer counter
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void wwdgt_enable(void) {
    WWDGT_CTL |= WWDGT_CTL_WDGTEN;
}

/*!
    简介:    configure the window watchdog timer counter value
    参数[输入]:  counter_value: 0x00 - 0x7F
    参数[输出]:  无
    返回值:      无
*/
void wwdgt_counter_update(uint16_t counter_value) {
    WWDGT_CTL = (uint32_t)(CTL_CNT(counter_value));
}

/*!
    简介:    configure counter value, window value, and prescaler divider value
    参数[输入]:  counter: 0x00 - 0x7F
    参数[输入]:  window: 0x00 - 0x7F
    参数[输入]:  prescaler: wwdgt prescaler value
                only one parameter can be selected which is shown as below:
      参数:        WWDGT_CFG_PSC_DIV1: the time base of window watchdog counter = (PCLK1/4096)/1
      参数:        WWDGT_CFG_PSC_DIV2: the time base of window watchdog counter = (PCLK1/4096)/2
      参数:        WWDGT_CFG_PSC_DIV4: the time base of window watchdog counter = (PCLK1/4096)/4
      参数:        WWDGT_CFG_PSC_DIV8: the time base of window watchdog counter = (PCLK1/4096)/8
    参数[输出]:  无
    返回值:      无
*/
void wwdgt_config(uint16_t counter, uint16_t window, uint32_t prescaler) {
    /* configure WIN and PSC bits, configure CNT bit */
    WWDGT_CTL = (uint32_t)(CTL_CNT(counter));
    WWDGT_CFG = (uint32_t)(CFG_WIN(window) | prescaler);
}

/*!
    简介:    check early wakeup interrupt state of WWDGT
    参数[输入]:  无
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus wwdgt_flag_get(void) {
    if(RESET != (WWDGT_STAT & WWDGT_STAT_EWIF)) {
        return SET;
    }

    return RESET;
}

/*!
    简介:    clear early wakeup interrupt state of WWDGT
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void wwdgt_flag_clear(void) {
    WWDGT_STAT = (uint32_t)(RESET);
}

/*!
    简介:    enable early wakeup interrupt of WWDGT
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void wwdgt_interrupt_enable(void) {
    WWDGT_CFG |= WWDGT_CFG_EWIE;
}
