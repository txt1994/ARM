/*!
    文件:    gd32f4xx_trng.c
    简介:   TRNG driver

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

#include "gd32f4xx_trng.h"

/*!
    简介:      reset TRNG
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void TRNG_DeInit(void) {
    RCU_Periph_Reset_Enable(RCU_TRNGRST);
    RCU_Periph_Reset_Disable(RCU_TRNGRST);
}

/*!
    简介:      enable TRNG
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void TRNG_Enable(void) {
    TRNG_CTL |= TRNG_CTL_TRNGEN;
}

/*!
    简介:      disable TRNG
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void TRNG_Disable(void) {
    TRNG_CTL &= ~TRNG_CTL_TRNGEN;
}

/*!
    简介:      get the true random data
    参数[输入]:  无
    参数[输出]:  无
    返回值:     uint32_t: 0x0-0xFFFFFFFF
*/
uint32_t TRNG_Get_true_random_data(void) {
    return (TRNG_DATA);
}

/*!
    简介:      enable TRNG interrupt
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void TRNG_Interrupt_Enable(void) {
    TRNG_CTL |= TRNG_CTL_TRNGIE;
}

/*!
    简介:      disable TRNG interrupt
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void TRNG_Interrupt_Disable(void) {
    TRNG_CTL &= ~TRNG_CTL_TRNGIE;
}

/*!
    简介:      get TRNG flag status
    参数[输入]:  flag: TRNG flag
                only one parameter can be selected which is shown as below:
      参数:        TRNG_Flag_DRDY: random Data ready status
      参数:        TRNG_Flag_CECS: clock error current status
      参数:        TRNG_Flag_SECS: seed error current status
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus TRNG_Flag_Get(TRNG_Flag_enum flag) {
    if(RESET != (TRNG_STAT & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:      get TRNG interrupt flag status
    参数[输入]:  int_flag: TRNG interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        TRNG_INT_Flag_CEIF: clock error interrupt flag
      参数:        TRNG_INT_Flag_SEIF: seed error interrupt flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus TRNG_Interrupt_Flag_Get(TRNG_int_Flag_enum int_flag) {
    if(RESET != (TRNG_STAT & int_flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:      clear TRNG interrupt flag status
    参数[输入]:  int_flag: TRNG interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        TRNG_INT_Flag_CEIF: clock error interrupt flag
      参数:        TRNG_INT_Flag_SEIF: seed error interrupt flag
    参数[输出]:  无
    返回值:      无
*/
void TRNG_Interrupt_Flag_Clear(TRNG_int_Flag_enum int_flag) {
    TRNG_STAT &= ~(uint32_t)int_flag;
}
