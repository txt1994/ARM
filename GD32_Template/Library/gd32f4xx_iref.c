/*!
    文件:    gd32f4xx_iref.c
    简介:   IREF driver

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

#include "gd32f4xx_iref.h"

/*!
    简介:      deinitialize IREF
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void iref_deinit(void) {
    rcu_periph_reset_enable(RCU_IREFRST);
    rcu_periph_reset_disable(RCU_IREFRST);
}

/*!
    简介:    enable IREF
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void iref_enable(void) {
    IREF_CTL |= IREF_CTL_CREN;
}

/*!
    简介:      disable IREF
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void iref_disable(void) {
    IREF_CTL &= ~IREF_CTL_CREN;
}

/*!
    简介:      set IREF mode
    参数[输入]:  step
      参数:        IREF_MODE_LOW_POWER: 1uA step
      参数:        IREF_MODE_HIGH_CURRENT: 8uA step
    参数[输出]:  无
    返回值:      无
*/
void iref_mode_set(uint32_t step) {
    IREF_CTL &= ~IREF_CTL_SSEL;
    IREF_CTL |= step;
}

/*!
    简介:      set IREF precision_trim_value
    参数[输入]:  precisiontrim
      参数:        IREF_CUR_PRECISION_TRIM_X(x=0..31): (-15+ x)%
    参数[输出]:  无
    返回值:      无
*/
void iref_precision_trim_value_set(uint32_t precisiontrim) {
    IREF_CTL &= ~IREF_CTL_CPT;
    IREF_CTL |= precisiontrim;
}

/*!
    简介:      set IREF sink mode
    参数[输入]:  sinkmode
      参数:        IREF_SOURCE_CURRENT : source current.
      参数:        IREF_SINK_CURRENT: sink current
    参数[输出]:  无
    返回值:      无
*/
void iref_sink_set(uint32_t sinkmode) {
    IREF_CTL &= ~IREF_CTL_SCMOD;
    IREF_CTL |= sinkmode;
}

/*!
    简介:      set IREF step data
    参数[输入]:  stepdata
      参数:        IREF_CUR_STEP_DATA_X:(x=0..63): step*x
    参数[输出]:  无
    返回值:      无
*/

void iref_step_data_config(uint32_t stepdata) {
    IREF_CTL &= ~IREF_CTL_CSDT;
    IREF_CTL |= stepdata;
}
