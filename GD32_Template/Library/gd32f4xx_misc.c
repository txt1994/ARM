/*!
    文件:    gd32f4xx_misc.c
    简介:   MISC driver
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

#include "gd32f4xx_misc.h"

/*!
    简介:    set the priority group
    参数[输入]:  NVIC_prigroup: the NVIC priority group
      参数:        NVIC_PRIGROUP_PRE0_SUB4:0 bits for pre-emption priority 4 bits for subpriority
      参数:        NVIC_PRIGROUP_PRE1_SUB3:1 bits for pre-emption priority 3 bits for subpriority
      参数:        NVIC_PRIGROUP_PRE2_SUB2:2 bits for pre-emption priority 2 bits for subpriority
      参数:        NVIC_PRIGROUP_PRE3_SUB1:3 bits for pre-emption priority 1 bits for subpriority
      参数:        NVIC_PRIGROUP_PRE4_SUB0:4 bits for pre-emption priority 0 bits for subpriority
    参数[输出]:  无
    返回值:      无
*/
void NVIC_Priority_Group_Set(uint32_t NVIC_prigroup) {
    /* set the priority group value */
    SCB->AIRCR = NVIC_AIRCR_VECTKEY_MASK | NVIC_prigroup;
}

/*!
    简介:    enable NVIC request
    参数[输入]:  NVIC_irq: the NVIC interrupt request, detailed in IRQn_Type
    参数[输入]:  NVIC_irq_pre_priority: the pre-emption priority needed to set
    参数[输入]:  NVIC_irq_sub_priority: the subpriority needed to set
    参数[输出]:  无
    返回值:      无
*/
void NVIC_irq_Enable(uint8_t NVIC_irq, uint8_t NVIC_irq_pre_priority,
                     uint8_t NVIC_irq_sub_priority) {
    uint32_t temp_priority = 0x00U, temp_pre = 0x00U, temp_sub = 0x00U;

    /* use the priority group value to get the temp_pre and the temp_sub */
    if(((SCB->AIRCR) & (uint32_t)0x700U) == NVIC_PRIGROUP_PRE0_SUB4) {
        temp_pre = 0U;
        temp_sub = 0x4U;
    } else if(((SCB->AIRCR) & (uint32_t)0x700U) == NVIC_PRIGROUP_PRE1_SUB3) {
        temp_pre = 1U;
        temp_sub = 0x3U;
    } else if(((SCB->AIRCR) & (uint32_t)0x700U) == NVIC_PRIGROUP_PRE2_SUB2) {
        temp_pre = 2U;
        temp_sub = 0x2U;
    } else if(((SCB->AIRCR) & (uint32_t)0x700U) == NVIC_PRIGROUP_PRE3_SUB1) {
        temp_pre = 3U;
        temp_sub = 0x1U;
    } else if(((SCB->AIRCR) & (uint32_t)0x700U) == NVIC_PRIGROUP_PRE4_SUB0) {
        temp_pre = 4U;
        temp_sub = 0x0U;
    } else {
        NVIC_Priority_Group_Set(NVIC_PRIGROUP_PRE2_SUB2);
        temp_pre = 2U;
        temp_sub = 0x2U;
    }

    /* get the temp_priority to fill the NVIC->IP register */
    temp_priority = (uint32_t)NVIC_irq_pre_priority << (0x4U - temp_pre);
    temp_priority |= NVIC_irq_sub_priority & (0x0FU >> (0x4U - temp_sub));
    temp_priority = temp_priority << 0x04U;
    NVIC->IP[NVIC_irq] = (uint8_t)temp_priority;
    /* enable the selected IRQ */
    NVIC->ISER[NVIC_irq >> 0x05U] = (uint32_t)0x01U << (NVIC_irq & (uint8_t)0x1FU);
}

/*!
    简介:    disable NVIC request
    参数[输入]:  NVIC_irq: the NVIC interrupt request, detailed in IRQn_Type
    参数[输出]:  无
    返回值:      无
*/
void NVIC_irq_Disable(uint8_t NVIC_irq) {
    /* disable the selected IRQ.*/
    NVIC->ICER[NVIC_irq >> 0x05] = (uint32_t)0x01 << (NVIC_irq & (uint8_t)0x1F);
}

/*!
    简介:    set the NVIC vector table base address
    参数[输入]:  NVIC_vict_tab: the RAM or FLASH base address
      参数:        NVIC_VECTTAB_RAM: RAM base address
      \are        NVIC_VECTTAB_FLASH: Flash base address
    参数[输入]:  offset: Vector Table offset
    参数[输出]:  无
    返回值:      无
*/
void NVIC_vector_table_Set(uint32_t NVIC_vict_tab, uint32_t offset) {
    SCB->VTOR = NVIC_vict_tab | (offset & NVIC_VECTTAB_Offset_MASK);
    __DSB();
}

/*!
    简介:    set the state of the low power mode
    参数[输入]:  lowpower_mode: the low power mode state
      参数:        SCB_LPM_SLEEP_EXIT_ISR: if chose this para, the system always enter low power
                    mode by exiting from ISR
      参数:        SCB_LPM_DEEPSLEEP: if chose this para, the system will enter the DEEPSLEEP mode
      参数:        SCB_LPM_WAKE_BY_ALL_INT: if chose this para, the lowpower mode can be woke up
                    by all the enable and disable interrupts
    参数[输出]:  无
    返回值:      无
*/
void system_Lowpower_Set(uint8_t lowpower_mode) {
    SCB->SCR |= (uint32_t)lowpower_mode;
}

/*!
    简介:    reset the state of the low power mode
    参数[输入]:  lowpower_mode: the low power mode state
      参数:        SCB_LPM_SLEEP_EXIT_ISR: if chose this para, the system will exit low power
                    mode by exiting from ISR
      参数:        SCB_LPM_DEEPSLEEP: if chose this para, the system will enter the SLEEP mode
      参数:        SCB_LPM_WAKE_BY_ALL_INT: if chose this para, the lowpower mode only can be
                    woke up by the enable interrupts
    参数[输出]:  无
    返回值:      无
*/
void system_Lowpower_Reset(uint8_t lowpower_mode) {
    SCB->SCR &= (~(uint32_t)lowpower_mode);
}

/*!
    简介:    set the systick clock source
    参数[输入]:  systick_clksource: the systick clock source needed to choose
      参数:        SYSTICK_CLKSOURCE_HCLK: systick clock source is from HCLK
      参数:        SYSTICK_CLKSOURCE_HCLK_DIV8: systick clock source is from HCLK/8
    参数[输出]:  无
    返回值:      无
*/

void Systick_CLKsource_Set(uint32_t systick_clksource) {
    if(SYSTICK_CLKSOURCE_HCLK == systick_clksource) {
        /* set the systick clock source from HCLK */
        SysTick->CTRL |= SYSTICK_CLKSOURCE_HCLK;
    } else {
        /* set the systick clock source from HCLK/8 */
        SysTick->CTRL &= SYSTICK_CLKSOURCE_HCLK_DIV8;
    }
}
