/*!
    \file    systick.c
    \简介:   the systick configuration file

    \版本: 2016-08-15, V1.0.0, firmware for GD32F4xx
    \版本: 2018-12-12, V2.0.0, firmware for GD32F4xx
    \版本: 2020-09-30, V2.1.0, firmware for GD32F4xx
    \版本: 2022-03-09, V3.0.0, firmware for GD32F4xx
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

volatile static uint32_t delay;

/*!
    \简介:    configure systick
    \参数[输入]:  无
    \参数[输出]:  无
    \返回值:      无
*/
void systick_config(void) {
    /* setup systick timer for 1000Hz 中断 */
    if(SysTick_Config(SystemCoreClock / 1000U)) {
        /* capture error */
        while(1) {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
}

/*!
    \简介:    delay a time in milliseconds
    \参数[输入]:  count: count in milliseconds
    \参数[输出]: 无
    \返回值:     无
*/
void delay_1ms(uint32_t count) {
    delay = count;

    while(0U != delay) {
    }
}

/*!
    \简介:    delay decrement
    \参数[输入]:  无
    \参数[输出]: 无
    \返回值:     无
*/
void delay_decrement(void) {
    if(0U != delay) {
        delay--;
    }
}
