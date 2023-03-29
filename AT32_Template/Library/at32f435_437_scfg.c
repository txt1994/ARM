/**
  **************************************************************************
  * @file     at32f435_437_scfg.c
  * @brief    contains all the functions for the system config firmware library
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

/** @defgroup SCFG
  * @brief SCFG driver modules
  * @{
  */

#ifdef SCFG_MODULE_EnableD

/** @defgroup SCFG_private_functions
  * @{
  */

/**
  * @brief  scfg reset
  * @param  none
  * @retval none
  */
void scfg_Reset(void) {
    crm_periph_Reset(CRM_SCFG_PERIPH_Reset, TRUE);
    crm_periph_Reset(CRM_SCFG_PERIPH_Reset, FALSE);
}

/**
  * @brief  scfg xmc address mapping swap set
  * @param  xmc_swap
  *         this parameter can be one of the following values:
  *         - SCFG_XMC_SWAP_NONE
  *         - SCFG_XMC_SWAP_MODE1
  *         - SCFG_XMC_SWAP_MODE2
  *         - SCFG_XMC_SWAP_MODE3
  * @retval none
  */
void scfg_xmc_mapping_swap_Set(scfg_xmc_swap_Type xmc_swap) {
    SCFG->cfg1_bit.swap_xmc = xmc_swap;
}

/**
  * @brief  scfg infrared config
  * @param  source
  *         this parameter can be one of the following values:
  *         - SCFG_IR_Source_TMR10
  *         - SCFG_IR_Source_USART1
  *         - SCFG_IR_Source_USART2
  * @param  polarity
  *         this parameter can be one of the following values:
  *         - SCFG_IR_POLARITY_NO_AFFECTE
  *         - SCFG_IR_POLARITY_REVERSE
  * @retval none
  */
void scfg_infrared_Config(scfg_ir_Source_Type source, scfg_ir_polarity_Type polarity) {
    SCFG->cfg1_bit.ir_src_sel = source;
    SCFG->cfg1_bit.ir_pol = polarity;
}

/**
  * @brief  scfg memory address mapping set
  * @param  mem_map
  *         this parameter can be one of the following values:
  *         - SCFG_MEM_MAP_MAIN_MEMORY
  *         - SCFG_MEM_MAP_BOOT_MEMORY
  *         - SCFG_MEM_MAP_XMC_BANK1
  *         - SCFG_MEM_MAP_Internal_SRAM
  *         - SCFG_MEM_MAP_XMC_SDRAM_BANK1
  * @retval none
  */
void scfg_mem_map_Set(scfg_mem_map_Type mem_map) {
    SCFG->cfg1_bit.mem_map_sel = mem_map;
}

/**
  * @brief  scfg emac interface set
  * @param  mode
  *         this parameter can be one of the following values:
  *         - SCFG_EMAC_Select_MII
  *         - SCFG_EMAC_Select_RMII
  * @retval none
  */
void scfg_EMAC_interface_Set(scfg_EMAC_interface_Type mode) {
    SCFG->cfg2_bit.mii_rmii_sel = mode;
}

/**
  * @brief  select the gpio pin used as exint line.
  * @param  port_source:
  *         select the gpio port to be used as source for exint lines.
  *         this parameter can be one of the following values:
  *         - SCFG_PORT_Source_GPIOA
  *         - SCFG_PORT_Source_GPIOB
  *         - SCFG_PORT_Source_GPIOC
  *         - SCFG_PORT_Source_GPIOD
  *         - SCFG_PORT_Source_GPIOE
  *         - SCFG_PORT_Source_GPIOF
  *         - SCFG_PORT_Source_GPIOG
  *         - SCFG_PORT_Source_GPIOH
  * @param  pin_source:
  *         specifies the exint line to be configured.
  *         this parameter can be one of the following values:
  *         - SCFG_PINS_SOURCE0
  *         - SCFG_PINS_SOURCE1
  *         - SCFG_PINS_SOURCE2
  *         - SCFG_PINS_SOURCE3
  *         - SCFG_PINS_SOURCE4
  *         - SCFG_PINS_SOURCE5
  *         - SCFG_PINS_SOURCE6
  *         - SCFG_PINS_SOURCE7
  *         - SCFG_PINS_SOURCE8
  *         - SCFG_PINS_SOURCE9
  *         - SCFG_PINS_SOURCE10
  *         - SCFG_PINS_SOURCE11
  *         - SCFG_PINS_SOURCE12
  *         - SCFG_PINS_SOURCE13
  *         - SCFG_PINS_SOURCE14
  *         - SCFG_PINS_SOURCE15
  * @retval none
  */
void scfg_EXINT_line_Config(scfg_port_Source_Type port_source, scfg_pins_Source_Type pin_source) {
    uint32_t tmp = 0x00;
    tmp = ((uint32_t)0x0F) << (0x04 * (pin_source & (uint8_t)0x03));

    switch (pin_source >> 0x02) {
        case 0:
            SCFG->exintc1 &= ~tmp;
            SCFG->exintc1 |= (((uint32_t)port_source) << (0x04 * (pin_source & (uint8_t)0x03)));
            break;

        case 1:
            SCFG->exintc2 &= ~tmp;
            SCFG->exintc2 |= (((uint32_t)port_source) << (0x04 * (pin_source & (uint8_t)0x03)));
            break;

        case 2:
            SCFG->exintc3 &= ~tmp;
            SCFG->exintc3 |= (((uint32_t)port_source) << (0x04 * (pin_source & (uint8_t)0x03)));
            break;

        case 3:
            SCFG->exintc4 &= ~tmp;
            SCFG->exintc4 |= (((uint32_t)port_source) << (0x04 * (pin_source & (uint8_t)0x03)));
            break;

        default:
            break;
    }
}

/**
  * @brief  enable or disable gpio pins ultra driven.
  * @param  value:
  *         this parameter can be one of the following values:
  *         - SCFG_ULTRA_DRIVEN_PB3
  *         - SCFG_ULTRA_DRIVEN_PB9
  *         - SCFG_ULTRA_DRIVEN_PB10
  *         - SCFG_ULTRA_DRIVEN_PD12
  *         - SCFG_ULTRA_DRIVEN_PD13
  *         - SCFG_ULTRA_DRIVEN_PD14
  *         - SCFG_ULTRA_DRIVEN_PD15
  *         - SCFG_ULTRA_DRIVEN_PF14
  *         - SCFG_ULTRA_DRIVEN_PF15
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void scfg_pins_ultra_driven_Enable(scfg_ultra_driven_pins_Type value, confirm_state new_state) {
    if(TRUE == new_state) {
        SCFG_REG(value) |= SCFG_REG_BIT(value);
    } else {
        SCFG_REG(value) &= ~(SCFG_REG_BIT(value));
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
