/*!
    文件:    gd32f4xx_exti.c
    简介:   EXTI driver
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

#include "gd32f4xx_exti.h"

/*!
    简介:    deinitialize the EXTI
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void EXTI_DeInit(void) {
    /* reset the value of all the EXTI registers */
    EXTI_INTEN = (uint32_t)0x00000000U;
    EXTI_EVEN  = (uint32_t)0x00000000U;
    EXTI_RTEN  = (uint32_t)0x00000000U;
    EXTI_FTEN  = (uint32_t)0x00000000U;
    EXTI_SWIEV = (uint32_t)0x00000000U;
}

/*!
    简介:    initialize the EXTI
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输入]:  mode: interrupt or event mode, refer to EXTI_Mode_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_INTERRUPT: interrupt mode
      参数:        EXTI_EVENT: event mode
    参数[输入]:  trig_type: interrupt trigger type, refer to EXTI_trig_type_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_TRIG_RISING: rising edge trigger
      参数:        EXTI_TRIG_FALLING: falling trigger
      参数:        EXTI_TRIG_BOTH: rising and falling trigger
      参数:        EXTI_TRIG_NONE: without rising edge or falling edge trigger
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Init(EXTI_Line_enum linex, \
               EXTI_Mode_enum mode, \
               EXTI_trig_type_enum trig_type) {
    /* reset the EXTI line x */
    EXTI_INTEN &= ~(uint32_t)linex;
    EXTI_EVEN &= ~(uint32_t)linex;
    EXTI_RTEN &= ~(uint32_t)linex;
    EXTI_FTEN &= ~(uint32_t)linex;

    /* set the EXTI mode and enable the interrupts or events from EXTI line x */
    switch(mode) {
    case EXTI_INTERRUPT:
        EXTI_INTEN |= (uint32_t)linex;
        break;

    case EXTI_EVENT:
        EXTI_EVEN |= (uint32_t)linex;
        break;

    default:
        break;
    }

    /* set the EXTI trigger type */
    switch(trig_type) {
    case EXTI_TRIG_RISING:
        EXTI_RTEN |= (uint32_t)linex;
        EXTI_FTEN &= ~(uint32_t)linex;
        break;

    case EXTI_TRIG_FALLING:
        EXTI_RTEN &= ~(uint32_t)linex;
        EXTI_FTEN |= (uint32_t)linex;
        break;

    case EXTI_TRIG_BOTH:
        EXTI_RTEN |= (uint32_t)linex;
        EXTI_FTEN |= (uint32_t)linex;
        break;

    case EXTI_TRIG_NONE:
    default:
        break;
    }
}

/*!
    简介:    enable the interrupts from EXTI line x
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Interrupt_Enable(EXTI_Line_enum linex) {
    EXTI_INTEN |= (uint32_t)linex;
}

/*!
    简介:    disable the interrupt from EXTI line x
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Interrupt_Disable(EXTI_Line_enum linex) {
    EXTI_INTEN &= ~(uint32_t)linex;
}

/*!
    简介:    enable the events from EXTI line x
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Event_Enable(EXTI_Line_enum linex) {
    EXTI_EVEN |= (uint32_t)linex;
}

/*!
    简介:    disable the events from EXTI line x
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Event_Disable(EXTI_Line_enum linex) {
    EXTI_EVEN &= ~(uint32_t)linex;
}

/*!
    简介:    enable EXTI software interrupt event
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Software_Interrupt_Enable(EXTI_Line_enum linex) {
    EXTI_SWIEV |= (uint32_t)linex;
}

/*!
    简介:    disable EXTI software interrupt event
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Software_Interrupt_Disable(EXTI_Line_enum linex) {
    EXTI_SWIEV &= ~(uint32_t)linex;
}

/*!
    简介:    get EXTI lines flag
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:     FlagStatus: status of flag (RESET or SET)
*/
FlagStatus EXTI_Flag_Get(EXTI_Line_enum linex) {
    if(RESET != (EXTI_PD & (uint32_t)linex)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear EXTI lines pending flag
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Flag_Clear(EXTI_Line_enum linex) {
    EXTI_PD = (uint32_t)linex;
}

/*!
    简介:    get EXTI lines flag when the interrupt flag is set
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:     FlagStatus: status of flag (RESET or SET)
*/
FlagStatus EXTI_Interrupt_Flag_Get(EXTI_Line_enum linex) {
    uint32_t flag_left, flag_right;

    flag_left = EXTI_PD & (uint32_t)linex;
    flag_right = EXTI_INTEN & (uint32_t)linex;

    if((RESET != flag_left) && (RESET != flag_right)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear EXTI lines pending flag
    参数[输入]:  linex: EXTI line number, refer to EXTI_Line_enum
                only one parameter can be selected which is shown as below:
      参数:        EXTI_x (x=0..22): EXTI line x
    参数[输出]:  无
    返回值:      无
*/
void EXTI_Interrupt_Flag_Clear(EXTI_Line_enum linex) {
    EXTI_PD = (uint32_t)linex;
}
