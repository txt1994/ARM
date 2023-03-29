/**
  **************************************************************************
  * @file     at32f435_437_wdt.c
  * @brief    contains all the functions for the wdt firmware library
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

#include "at32f435_437_conf.h"

/** @addtogroup AT32F435_437_periph_driver
  * @{
  */

/** @defgroup WDT
  * @brief WDT driver modules
  * @{
  */

#ifdef WDT_MODULE_EnableD

/** @defgroup WDT_private_functions
  * @{
  */

/**
  * @brief  wdt enable ,the reload value will be sent to the counter
  * @param  none
  * @retval none
  */
void WDT_Enable(void) {
    WDT->cmd = WDT_CMD_Enable;
}

/**
  * @brief  reload wdt counter
  * @param  none
  * @retval none
  */
void WDT_Counter_reload(void) {
    WDT->cmd = WDT_CMD_RELOAD;
}

/**
  * @brief  set wdt counter reload value
  * @param  reload_value (0x0000~0x0FFF)
  * @retval none
  */
void WDT_Reload_Value_Set(uint16_t reload_value) {
    WDT->rld = reload_value;
}

/**
  * @brief  set wdt division divider
  * @param  division
  *         this parameter can be one of the following values:
  *         - WDT_CLK_DIV_4
  *         - WDT_CLK_DIV_8
  *         - WDT_CLK_DIV_16
  *         - WDT_CLK_DIV_32
  *         - WDT_CLK_DIV_64
  *         - WDT_CLK_DIV_128
  *         - WDT_CLK_DIV_256
  * @retval none
  */
void WDT_Divider_Set(WDT_Division_Type division) {
    WDT->div_bit.div = division;
}

/**
  * @brief  enable or disable wdt cmd register write
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void WDT_Register_Write_Enable( confirm_state new_state) {
    if(new_state == FALSE) {
        WDT->cmd = WDT_CMD_LOCK;
    } else {
        WDT->cmd = WDT_CMD_UNLOCK;
    }
}

/**
  * @brief  get wdt flag
  * @param  WDT_flag
  *         this parameter can be one of the following values:
  *         - WDT_DIVF_Update_FLAG: division value update complete flag.
  *         - WDT_RLDF_Update_FLAG: reload value update complete flag.
  *         - WDT_WINF_Update_FLAG: window value update complete flag.
  * @retval state of wdt flag
  */
flag_status WDT_Flag_Get(uint16_t WDT_flag) {
    flag_status status = RESET;

    if ((WDT->sts & WDT_flag) != (uint16_t)RESET) {
        status = SET;
    } else {
        status = RESET;
    }

    return status;
}

/**
  * @brief  wdt window counter value set
  * @param  window_cnt (0x0000~0x0FFF)
  * @retval none
  */
void WDT_Window_Counter_Set(uint16_t window_cnt) {
    WDT->win_bit.win = window_cnt;
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
