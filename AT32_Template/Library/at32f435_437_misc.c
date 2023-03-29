/**
  **************************************************************************
  * @file     at32f435_437_misc.c
  * @brief    contains all the functions for the misc firmware library
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

/* includes ------------------------------------------------------------------*/
#include "at32f435_437_conf.h"

/** @addtogroup AT32F435_437_periph_driver
  * @{
  */

/** @defgroup MISC
  * @brief MISC driver modules
  * @{
  */

#ifdef MISC_MODULE_EnableD

/** @defgroup MISC_private_functions
  * @{
  */

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

/**
  * @brief  system reset
  * @param  none
  * @retval none
  */
void nvic_System_Reset(void) {
    NVIC_SystemReset();
}

/**
  * @brief  enable nvic irq
  * @param  irqn (IRQn_Type number)
  * @param  preempt_priority: preemptive priority value (starting from 0)
  * @param  sub_priority: subpriority value (starting from 0)
  * @retval none
  */
void nvic_irq_Enable(IRQn_Type irqn, uint32_t preempt_priority, uint32_t sub_priority) {
    uint32_t temp_priority = 0;

    /* encode priority */
    temp_priority = NVIC_EncodePriority(NVIC_GetPriorityGrouping(), preempt_priority, sub_priority);
    /* set priority */
    NVIC_SetPriority(irqn, temp_priority);
    /* enable irqn */
    NVIC_EnableIRQ(irqn);
}

/**
  * @brief  disable nvic irq number
  * @param  irqn (IRQn_Type number)
  * @retval none
  */
void nvic_irq_Disable(IRQn_Type irqn) {
    NVIC_DisableIRQ(irqn);
}

/**
  * @brief  config nvic priority group
  * @param  priority_group
  *         this parameter can be one of the following values:
  *         - NVIC_PRIORITY_GROUP_0
  *         - NVIC_PRIORITY_GROUP_1
  *         - NVIC_PRIORITY_GROUP_2
  *         - NVIC_PRIORITY_GROUP_3
  *         - NVIC_PRIORITY_GROUP_4
  * @retval none
  */
void nvic_priority_group_Config(nvic_priority_group_Type priority_group) {
    /* set the prigroup[10:8] bits according to nvic_prioritygroup value */
    NVIC_SetPriorityGrouping(priority_group);
}

/**
  * @brief  set the vector table location and offset.
  * @param  base
  *         this parameter can be one of the following values:
  *         - NVIC_VECTTAB_RAM
  *         - NVIC_VECTTAB_FLASH
  * @param  offset (vector table base offset field. this value must be a multiple of 0x200)
  * @retval none
  */
void nvic_vector_table_Set(uint32_t base, uint32_t offset) {
    SCB->VTOR = base | (offset & (uint32_t)0x1FFFFF80);
}

/**
  * @brief  config nvic lowpower mode
  * @param  lp_mode
  *         this parameter can be one of the following values:
  *         - NVIC_LP_SEVONPEND
  *         - NVIC_LP_SLEEPDEEP
  *         - NVIC_LP_SLEEPONEXIT
  * @param  new_state (new state of lp condition. ENABLE or DISABLE)
  * @retval none
  */
void nvic_lowpower_Mode_Config(nvic_lowpower_Mode_Type lp_mode, confirm_state new_state) {
    if(new_state != FALSE) {
        SCB->SCR |= lp_mode;
    } else {
        SCB->SCR &= (uint32_t)(~(uint32_t)lp_mode);
    }
}

/**
  * @brief  config systick clock source
  * @param  source
  *         this parameter can be one of the following values:
  *         - SYSTICK_Clock_Source_AHBCLK_DIV8
  *         - SYSTICK_Clock_Source_AHBCLK_NODIV
  * @retval none
  */
void systick_Clock_Source_Config(systick_Clock_Source_Type source) {
    if(source == SYSTICK_Clock_Source_AHBCLK_NODIV) {
        SysTick->CTRL |= SYSTICK_Clock_Source_AHBCLK_NODIV;
    } else {
        SysTick->CTRL &= ~(uint32_t)SYSTICK_Clock_Source_AHBCLK_NODIV;
    }
}

/**
  * @}
  */

#endif

/**
  * @}
  */

/**
  * @}
  */


