/**
  **************************************************************************
  * @file     at32f435_437_crm.c
  * @brief    contains all the functions for the crm firmware library
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

/** @defgroup CRM
  * @brief CRM driver modules
  * @{
  */

#ifdef CRM_MODULE_EnableD

/** @defgroup CRM_private_functions
  * @{
  */

/**
  * @brief  reset the crm register
  * @param  none
  * @retval none
  */
void crm_Reset(void) {
    /* reset the crm clock configuration to the default reset state(for debug purpose) */
    /* set hicken bit */
    CRM->ctrl_bit.hicken = TRUE;

    /* wait hick stable */
    while(CRM->ctrl_bit.hickstbl != SET);

    /* hick used as system clock */
    CRM->cfg_bit.sclksel = CRM_SCLK_HICK;

    /* wait sclk switch status */
    while(CRM->cfg_bit.sclksts != CRM_SCLK_HICK);

    /* reset hexten, hextbyps, cfden and pllen bits */
    CRM->ctrl &= ~(0x010D0000U);

    /* reset cfg register, include sclk switch, ahbdiv, apb1div, apb2div, adcdiv, clkout bits */
    CRM->cfg = 0;

    /* reset pllms pllns pllfr pllrcs bits */
    CRM->pllcfg = 0x00033002U;

    /* reset clkout[3], usbbufs, hickdiv, clkoutdiv */
    CRM->misc1 = 0;

    /* disable all interrupts enable and clear pending bits  */
    CRM->clkint = 0x009F0000U;
}

/**
  * @brief  enable or disable crm low speed external crystal bypass
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_lext_bypass(confirm_state new_state) {
    CRM->bpdc_bit.lextbyps = new_state;
}

/**
  * @brief  enable or disable crm high speed external crystal bypass
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_hext_bypass(confirm_state new_state) {
    CRM->ctrl_bit.hextbyps = new_state;
}

/**
  * @brief  get crm flag status
  * @param  flag
  *         this parameter can be one of the following values:
  *         - CRM_HICK_STABLE_FLAG
  *         - CRM_HEXT_STABLE_FLAG
  *         - CRM_PLL_STABLE_FLAG
  *         - CRM_LEXT_STABLE_FLAG
  *         - CRM_LICK_STABLE_FLAG
  *         - CRM_Pin_Reset_FLAG
  *         - CRM_POR_Reset_FLAG
  *         - CRM_SW_Reset_FLAG
  *         - CRM_WDT_Reset_FLAG
  *         - CRM_WWDT_Reset_FLAG
  *         - CRM_LOWPOWER_Reset_FLAG
  *         interrupt flag:
  *         - CRM_LICK_ReadY_INT_FLAG
  *         - CRM_LEXT_ReadY_INT_FLAG
  *         - CRM_HICK_ReadY_INT_FLAG
  *         - CRM_HEXT_ReadY_INT_FLAG
  *         - CRM_PLL_ReadY_INT_FLAG
  *         - CRM_Clock_FAILURE_INT_FLAG
  * @retval flag_status (SET or RESET)
  */
flag_status crm_Flag_Get(uint32_t flag) {
    flag_status status = RESET;

    if((CRM_REG(flag) & CRM_REG_BIT(flag)) != CRM_REG_BIT(flag)) {
        status = RESET;
    } else {
        status = SET;
    }

    return status;
}

/**
  * @brief  wait for hext stable
  * @param  none
  * @retval error_status (ERROR or SUCCESS)
  */
error_status crm_hext_stable_wait(void) {
    uint32_t stable_cnt = 0;
    error_status status = ERROR;

    while((crm_Flag_Get(CRM_HEXT_STABLE_FLAG) != SET) && (stable_cnt < HEXT_StartUP_TIMEOUT)) {
        stable_cnt ++;
    }

    if(crm_Flag_Get(CRM_HEXT_STABLE_FLAG) != SET) {
        status = ERROR;
    } else {
        status = SUCCESS;
    }

    return status;
}

/**
  * @brief  set the hick trimming value
  * @param  trim_value (0x00~0x3F)
  * @retval none
  */
void crm_hick_Clock_trimming_Set(uint8_t trim_value) {
    CRM->ctrl_bit.hicktrim = trim_value;
}

/**
  * @brief  set the crm calibration value
  * @param  cali_value (0x00~0xFF)
  * @retval none
  */
void crm_hick_Clock_Calibration_Set(uint8_t cali_value) {
    /* enable write hick calibration */
    CRM->misc1_bit.hickcal_key = 0x5A;

    /* write hick calibration value */
    CRM->ctrl_bit.hickcal = cali_value;

    /* disable write hick calibration */
    CRM->misc1_bit.hickcal_key = 0x0;
}

/**
  * @brief  enable or disable the peripheral clock
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_GPIOA_PERIPH_CLOCK        - CRM_GPIOB_PERIPH_CLOCK        - CRM_GPIOC_PERIPH_CLOCK       - CRM_GPIOD_PERIPH_CLOCK
  *         - CRM_GPIOE_PERIPH_CLOCK        - CRM_GPIOF_PERIPH_CLOCK        - CRM_GPIOG_PERIPH_CLOCK       - CRM_GPIOH_PERIPH_CLOCK
  *         - CRM_CRC_PERIPH_CLOCK          - CRM_EDMA_PERIPH_CLOCK         - CRM_DMA1_PERIPH_CLOCK        - CRM_DMA2_PERIPH_CLOCK
  *         - CRM_EMAC_PERIPH_CLOCK         - CRM_EMACTX_PERIPH_CLOCK       - CRM_EMACRX_PERIPH_CLOCK      - CRM_EMACPTP_PERIPH_CLOCK
  *         - CRM_OTGFS2_PERIPH_CLOCK       - CRM_DVP_PERIPH_CLOCK          - CRM_OTGFS1_PERIPH_CLOCK      - CRM_SDIO1_PERIPH_CLOCK
  *         - CRM_XMC_PERIPH_CLOCK          - CRM_QSPI1_PERIPH_CLOCK        - CRM_QSPI2_PERIPH_CLOCK       - CRM_SDIO2_PERIPH_CLOCK
  *         - CRM_TMR2_PERIPH_CLOCK         - CRM_TMR3_PERIPH_CLOCK         - CRM_TMR4_PERIPH_CLOCK        - CRM_TMR5_PERIPH_CLOCK
  *         - CRM_TMR6_PERIPH_CLOCK         - CRM_TMR7_PERIPH_CLOCK         - CRM_TMR12_PERIPH_CLOCK       - CRM_TMR13_PERIPH_CLOCK
  *         - CRM_TMR14_PERIPH_CLOCK        - CRM_WWDT_PERIPH_CLOCK         - CRM_SPI2_PERIPH_CLOCK        - CRM_SPI3_PERIPH_CLOCK
  *         - CRM_USART2_PERIPH_CLOCK       - CRM_USART3_PERIPH_CLOCK       - CRM_UART4_PERIPH_CLOCK       - CRM_UART5_PERIPH_CLOCK
  *         - CRM_I2C1_PERIPH_CLOCK         - CRM_I2C2_PERIPH_CLOCK         - CRM_I2C3_PERIPH_CLOCK        - CRM_CAN1_PERIPH_CLOCK
  *         - CRM_CAN2_PERIPH_CLOCK         - CRM_PWC_PERIPH_CLOCK          - CRM_DAC_PERIPH_CLOCK         - CRM_UART7_PERIPH_CLOCK
  *         - CRM_UART8_PERIPH_CLOCK        - CRM_TMR1_PERIPH_CLOCK         - CRM_TMR8_PERIPH_CLOCK        - CRM_USART1_PERIPH_CLOCK
  *         - CRM_USART6_PERIPH_CLOCK       - CRM_ADC1_PERIPH_CLOCK         - CRM_ADC2_PERIPH_CLOCK        - CRM_ADC3_PERIPH_CLOCK
  *         - CRM_SPI1_PERIPH_CLOCK         - CRM_SPI4_PERIPH_CLOCK         - CRM_SCFG_PERIPH_CLOCK        - CRM_TMR9_PERIPH_CLOCK
  *         - CRM_TMR10_PERIPH_CLOCK        - CRM_TMR11_PERIPH_CLOCK        - CRM_TMR20_PERIPH_CLOCK       - CRM_ACC_PERIPH_CLOCK
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_periph_Clock_Enable(crm_periph_Clock_Type value, confirm_state new_state) {
    /* enable periph clock */
    if(TRUE == new_state) {
        CRM_REG(value) |= CRM_REG_BIT(value);
    }
    /* disable periph clock */
    else {
        CRM_REG(value) &= ~(CRM_REG_BIT(value));
    }
}

/**
  * @brief  enable or disable the peripheral reset
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_GPIOA_PERIPH_Reset        - CRM_GPIOB_PERIPH_Reset        - CRM_GPIOC_PERIPH_Reset       - CRM_GPIOD_PERIPH_Reset
  *         - CRM_GPIOE_PERIPH_Reset        - CRM_GPIOF_PERIPH_Reset        - CRM_GPIOG_PERIPH_Reset       - CRM_GPIOH_PERIPH_Reset
  *         - CRM_CRC_PERIPH_Reset          - CRM_EDMA_PERIPH_Reset         - CRM_DMA1_PERIPH_Reset        - CRM_DMA2_PERIPH_Reset
  *         - CRM_EMAC_PERIPH_Reset         - CRM_OTGFS2_PERIPH_Reset       - CRM_DVP_PERIPH_Reset         - CRM_OTGFS1_PERIPH_Reset
  *         - CRM_SDIO1_PERIPH_Reset        - CRM_XMC_PERIPH_Reset          - CRM_QSPI1_PERIPH_Reset       - CRM_QSPI2_PERIPH_Reset
  *         - CRM_SDIO2_PERIPH_Reset        - CRM_TMR2_PERIPH_Reset         - CRM_TMR3_PERIPH_Reset        - CRM_TMR4_PERIPH_Reset
  *         - CRM_TMR5_PERIPH_Reset         - CRM_TMR6_PERIPH_Reset         - CRM_TMR7_PERIPH_Reset        - CRM_TMR12_PERIPH_Reset
  *         - CRM_TMR13_PERIPH_Reset        - CRM_TMR14_PERIPH_Reset        - CRM_WWDT_PERIPH_Reset        - CRM_SPI2_PERIPH_Reset
  *         - CRM_SPI3_PERIPH_Reset         - CRM_USART2_PERIPH_Reset       - CRM_USART3_PERIPH_Reset      - CRM_UART4_PERIPH_Reset
  *         - CRM_UART5_PERIPH_Reset        - CRM_I2C1_PERIPH_Reset         - CRM_I2C2_PERIPH_Reset        - CRM_I2C3_PERIPH_Reset
  *         - CRM_CAN1_PERIPH_Reset         - CRM_CAN2_PERIPH_Reset         - CRM_PWC_PERIPH_Reset         - CRM_DAC_PERIPH_Reset
  *         - CRM_UART7_PERIPH_Reset        - CRM_UART8_PERIPH_Reset        - CRM_TMR1_PERIPH_Reset        - CRM_TMR8_PERIPH_Reset
  *         - CRM_USART1_PERIPH_Reset       - CRM_USART6_PERIPH_Reset       - CRM_ADC_PERIPH_Reset         - CRM_SPI1_PERIPH_Reset
  *         - CRM_SPI4_PERIPH_Reset         - CRM_SCFG_PERIPH_Reset         - CRM_TMR9_PERIPH_Reset        - CRM_TMR10_PERIPH_Reset
  *         - CRM_TMR11_PERIPH_Reset        - CRM_TMR20_PERIPH_Reset        - CRM_ACC_PERIPH_Reset
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_periph_Reset(crm_periph_Reset_Type value, confirm_state new_state) {
    /* enable periph reset */
    if(new_state == TRUE) {
        CRM_REG(value) |= (CRM_REG_BIT(value));
    }
    /* disable periph reset */
    else {
        CRM_REG(value) &= ~(CRM_REG_BIT(value));
    }
}

/**
  * @brief  enable or disable the peripheral clock in lowpower mode
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_GPIOA_PERIPH_LOWPOWER     - CRM_GPIOB_PERIPH_LOWPOWER     - CRM_GPIOC_PERIPH_LOWPOWER    - CRM_GPIOD_PERIPH_LOWPOWER
  *         - CRM_GPIOE_PERIPH_LOWPOWER     - CRM_GPIOF_PERIPH_LOWPOWER     - CRM_GPIOG_PERIPH_LOWPOWER    - CRM_GPIOH_PERIPH_LOWPOWER
  *         - CRM_CRC_PERIPH_LOWPOWER       - CRM_EDMA_PERIPH_LOWPOWER      - CRM_DMA1_PERIPH_LOWPOWER     - CRM_DMA2_PERIPH_LOWPOWER
  *         - CRM_EMAC_PERIPH_LOWPOWER      - CRM_EMACTX_PERIPH_LOWPOWER    - CRM_EMACRX_PERIPH_LOWPOWER   - CRM_EMACPTP_PERIPH_LOWPOWER
  *         - CRM_OTGFS2_PERIPH_LOWPOWER    - CRM_DVP_PERIPH_LOWPOWER       - CRM_OTGFS1_PERIPH_LOWPOWER   - CRM_SDIO1_PERIPH_LOWPOWER
  *         - CRM_XMC_PERIPH_LOWPOWER       - CRM_QSPI1_PERIPH_LOWPOWER     - CRM_QSPI2_PERIPH_LOWPOWER    - CRM_SDIO2_PERIPH_LOWPOWER
  *         - CRM_TMR2_PERIPH_LOWPOWER      - CRM_TMR3_PERIPH_LOWPOWER      - CRM_TMR4_PERIPH_LOWPOWER     - CRM_TMR5_PERIPH_LOWPOWER
  *         - CRM_TMR6_PERIPH_LOWPOWER      - CRM_TMR7_PERIPH_LOWPOWER      - CRM_TMR12_PERIPH_LOWPOWER    - CRM_TMR13_PERIPH_LOWPOWER
  *         - CRM_TMR14_PERIPH_LOWPOWER     - CRM_WWDT_PERIPH_LOWPOWER      - CRM_SPI2_PERIPH_LOWPOWER     - CRM_SPI3_PERIPH_LOWPOWER
  *         - CRM_USART2_PERIPH_LOWPOWER    - CRM_USART3_PERIPH_LOWPOWER    - CRM_UART4_PERIPH_LOWPOWER    - CRM_UART5_PERIPH_LOWPOWER
  *         - CRM_I2C1_PERIPH_LOWPOWER      - CRM_I2C2_PERIPH_LOWPOWER      - CRM_I2C3_PERIPH_LOWPOWER     - CRM_CAN1_PERIPH_LOWPOWER
  *         - CRM_CAN2_PERIPH_LOWPOWER      - CRM_PWC_PERIPH_LOWPOWER       - CRM_DAC_PERIPH_LOWPOWER      - CRM_UART7_PERIPH_LOWPOWER
  *         - CRM_UART8_PERIPH_LOWPOWER     - CRM_TMR1_PERIPH_LOWPOWER      - CRM_TMR8_PERIPH_LOWPOWER     - CRM_USART1_PERIPH_LOWPOWER
  *         - CRM_USART6_PERIPH_LOWPOWER    - CRM_ADC1_PERIPH_LOWPOWER      - CRM_ADC2_PERIPH_LOWPOWER     - CRM_ADC3_PERIPH_LOWPOWER
  *         - CRM_SPI1_PERIPH_LOWPOWER      - CRM_SPI4_PERIPH_LOWPOWER      - CRM_SCFG_PERIPH_LOWPOWER     - CRM_TMR9_PERIPH_LOWPOWER
  *         - CRM_TMR10_PERIPH_LOWPOWER     - CRM_TMR11_PERIPH_LOWPOWER     - CRM_TMR20_PERIPH_LOWPOWER    - CRM_ACC_PERIPH_LOWPOWER
  *         - CRM_FLASH_PERIPH_LOWPOWER     - CRM_SRAM1_PERIPH_LOWPOWER     - CRM_SRAM2_PERIPH_LOWPOWER
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_periph_lowpower_Mode_Enable(crm_periph_Clock_lowpower_Type value, confirm_state new_state) {
    /* enable periph clock in lowpower mode */
    if(new_state == TRUE) {
        CRM_REG(value) |= (CRM_REG_BIT(value));
    }
    /* disable periph clock in lowpower mode */
    else {
        CRM_REG(value) &= ~(CRM_REG_BIT(value));
    }
}

/**
  * @brief  enable or disable the crm clock source
  * @param  source
  *         this parameter can be one of the following values:
  *         - CRM_Clock_Source_HICK
  *         - CRM_Clock_Source_HEXT
  *         - CRM_Clock_Source_PLL
  *         - CRM_Clock_Source_LEXT
  *         - CRM_Clock_Source_LICK
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_Clock_Source_Enable(crm_Clock_Source_Type source, confirm_state new_state) {
    switch(source) {
        case CRM_Clock_Source_HICK:
            CRM->ctrl_bit.hicken = new_state;
            break;

        case CRM_Clock_Source_HEXT:
            CRM->ctrl_bit.hexten = new_state;
            break;

        case CRM_Clock_Source_PLL:
            CRM->ctrl_bit.pllen = new_state;
            break;

        case CRM_Clock_Source_LEXT:
            CRM->bpdc_bit.lexten = new_state;
            break;

        case CRM_Clock_Source_LICK:
            CRM->ctrlsts_bit.licken = new_state;
            break;

        default:
            break;
    }
}

/**
  * @brief  clear the crm reset flags
  * @param  flag
  *         this parameter can be one of the following values:
  *         reset flag:
  *         - CRM_Pin_Reset_FLAG
  *         - CRM_POR_Reset_FLAG
  *         - CRM_SW_Reset_FLAG
  *         - CRM_WDT_Reset_FLAG
  *         - CRM_WWDT_Reset_FLAG
  *         - CRM_LOWPOWER_Reset_FLAG
  *         - CRM_All_Reset_FLAG
  *         interrupt flag:
  *         - CRM_LICK_ReadY_INT_FLAG
  *         - CRM_LEXT_ReadY_INT_FLAG
  *         - CRM_HICK_ReadY_INT_FLAG
  *         - CRM_HEXT_ReadY_INT_FLAG
  *         - CRM_PLL_ReadY_INT_FLAG
  *         - CRM_Clock_FAILURE_INT_FLAG
  * @retval none
  */
void crm_Flag_Clear(uint32_t flag) {
    switch(flag) {
        case CRM_NRST_Reset_FLAG:
        case CRM_POR_Reset_FLAG:
        case CRM_SW_Reset_FLAG:
        case CRM_WDT_Reset_FLAG:
        case CRM_WWDT_Reset_FLAG:
        case CRM_LOWPOWER_Reset_FLAG:
        case CRM_All_Reset_FLAG:
            CRM->ctrlsts_bit.rstfc = TRUE;

            while(CRM->ctrlsts_bit.rstfc == TRUE);

            break;

        case CRM_LICK_ReadY_INT_FLAG:
            CRM->clkint_bit.lickstblfc = TRUE;
            break;

        case CRM_LEXT_ReadY_INT_FLAG:
            CRM->clkint_bit.lextstblfc = TRUE;
            break;

        case CRM_HICK_ReadY_INT_FLAG:
            CRM->clkint_bit.hickstblfc = TRUE;
            break;

        case CRM_HEXT_ReadY_INT_FLAG:
            CRM->clkint_bit.hextstblfc = TRUE;
            break;

        case CRM_PLL_ReadY_INT_FLAG:
            CRM->clkint_bit.pllstblfc = TRUE;
            break;

        case CRM_Clock_FAILURE_INT_FLAG:
            CRM->clkint_bit.cfdfc = TRUE;
            break;

        default:
            break;
    }
}

/**
  * @brief  select ertc clock
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_ERTC_Clock_NOCLK
  *         - CRM_ERTC_Clock_LEXT
  *         - CRM_ERTC_Clock_LICK
  *         - CRM_ERTC_Clock_HEXT_DIV_2
  *         - CRM_ERTC_Clock_HEXT_DIV_3
  *         - CRM_ERTC_Clock_HEXT_DIV_4
  *         - CRM_ERTC_Clock_HEXT_DIV_5
  *         - CRM_ERTC_Clock_HEXT_DIV_6
  *         - CRM_ERTC_Clock_HEXT_DIV_7
  *         - CRM_ERTC_Clock_HEXT_DIV_8
  *         - CRM_ERTC_Clock_HEXT_DIV_9
  *         - CRM_ERTC_Clock_HEXT_DIV_10
  *         - CRM_ERTC_Clock_HEXT_DIV_11
  *         - CRM_ERTC_Clock_HEXT_DIV_12
  *         - CRM_ERTC_Clock_HEXT_DIV_13
  *         - CRM_ERTC_Clock_HEXT_DIV_14
  *         - CRM_ERTC_Clock_HEXT_DIV_15
  *         - CRM_ERTC_Clock_HEXT_DIV_16
  *         - CRM_ERTC_Clock_HEXT_DIV_17
  *         - CRM_ERTC_Clock_HEXT_DIV_18
  *         - CRM_ERTC_Clock_HEXT_DIV_19
  *         - CRM_ERTC_Clock_HEXT_DIV_20
  *         - CRM_ERTC_Clock_HEXT_DIV_21
  *         - CRM_ERTC_Clock_HEXT_DIV_22
  *         - CRM_ERTC_Clock_HEXT_DIV_23
  *         - CRM_ERTC_Clock_HEXT_DIV_24
  *         - CRM_ERTC_Clock_HEXT_DIV_25
  *         - CRM_ERTC_Clock_HEXT_DIV_26
  *         - CRM_ERTC_Clock_HEXT_DIV_27
  *         - CRM_ERTC_Clock_HEXT_DIV_28
  *         - CRM_ERTC_Clock_HEXT_DIV_29
  *         - CRM_ERTC_Clock_HEXT_DIV_30
  *         - CRM_ERTC_Clock_HEXT_DIV_31
  * @retval none
  */
void crm_ERTC_Clock_Select(crm_ERTC_Clock_Type value) {
    CRM->cfg_bit.ertcdiv = ((value & 0x1F0) >> 4);
    CRM->bpdc_bit.ertcsel = (value & 0xF);
}

/**
  * @brief  enable or disable ertc
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_ERTC_Clock_Enable(confirm_state new_state) {
    CRM->bpdc_bit.ertcen = new_state;
}

/**
  * @brief  set crm ahb division
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_AHB_DIV_1
  *         - CRM_AHB_DIV_2
  *         - CRM_AHB_DIV_4
  *         - CRM_AHB_DIV_8
  *         - CRM_AHB_DIV_16
  *         - CRM_AHB_DIV_64
  *         - CRM_AHB_DIV_128
  *         - CRM_AHB_DIV_256
  *         - CRM_AHB_DIV_512
  * @retval none
  */
void crm_ahb_div_Set(crm_ahb_div_Type value) {
    CRM->cfg_bit.ahbdiv = value;
}

/**
  * @brief  set crm apb1 division
  * @note   the maximum frequency of APB1/APB2 clock is 144 MHz
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_APB1_DIV_1
  *         - CRM_APB1_DIV_2
  *         - CRM_APB1_DIV_4
  *         - CRM_APB1_DIV_8
  *         - CRM_APB1_DIV_16
  * @retval none
  */
void crm_apb1_div_Set(crm_apb1_div_Type value) {
    CRM->cfg_bit.apb1div = value;
}

/**
  * @brief  set crm apb2 division
  * @note   the maximum frequency of APB1/APB2 clock is 144 MHz
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_APB2_DIV_1
  *         - CRM_APB2_DIV_2
  *         - CRM_APB2_DIV_4
  *         - CRM_APB2_DIV_8
  *         - CRM_APB2_DIV_16
  * @retval none
  */
void crm_apb2_div_Set(crm_apb2_div_Type value) {
    CRM->cfg_bit.apb2div = value;
}

/**
  * @brief  set usb division
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_USB_DIV_1_5
  *         - CRM_USB_DIV_1
  *         - CRM_USB_DIV_2_5
  *         - CRM_USB_DIV_2
  *         - CRM_USB_DIV_3_5
  *         - CRM_USB_DIV_3
  *         - CRM_USB_DIV_4_5
  *         - CRM_USB_DIV_4
  *         - CRM_USB_DIV_5_5
  *         - CRM_USB_DIV_5
  *         - CRM_USB_DIV_6_5
  *         - CRM_USB_DIV_6
  *         - CRM_USB_DIV_7
  * @retval none
  */
void crm_USB_Clock_div_Set(crm_USB_div_Type value) {
    CRM->misc2_bit.usbdiv = value;
}

/**
  * @brief  enable or disable clock failure detection
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_Clock_failure_detection_Enable(confirm_state new_state) {
    CRM->ctrl_bit.cfden = new_state;
}

/**
  * @brief  battery powered domain software reset
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_battery_powered_domain_Reset(confirm_state new_state) {
    CRM->bpdc_bit.bpdrst = new_state;
}

/**
  * @brief  auto step clock switch enable
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_Auto_Step_Mode_Enable(confirm_state new_state) {
    if(new_state == TRUE)
        CRM->misc2_bit.auto_Step_en = CRM_Auto_Step_Mode_Enable;
    else
        CRM->misc2_bit.auto_Step_en = CRM_Auto_Step_Mode_Disable;
}

/**
  * @brief  config hick divider select
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_HICK48_DIV6
  *         - CRM_HICK48_NODIV
  * @retval none
  */
void crm_hick_Divider_Select(crm_hick_div_6_Type value) {
    CRM->misc1_bit.hickdiv = value;
}

/**
  * @brief  hick as system clock frequency select
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_HICK_SCLK_8MHZ
  *         - CRM_HICK_SCLK_48MHZ
  * @retval none
  */
void crm_hick_sclk_Frequency_Select(crm_hick_sclk_Frequency_Type value) {
    crm_hick_Divider_Select(CRM_HICK48_NODIV);
    CRM->misc1_bit.hick_to_sclk = value;
}

/**
  * @brief  usb 48 mhz clock source select
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_USB_Clock_Source_PLL
  *         - CRM_USB_Clock_Source_HICK
  * @retval none
  */
void crm_USB_Clock_Source_Select(crm_USB_Clock_Source_Type value) {
    if(value == CRM_USB_Clock_Source_HICK) {
        crm_hick_sclk_Frequency_Select(CRM_HICK_SCLK_48MHZ);
    }

    CRM->misc1_bit.hick_to_usb = value;
}

/**
  * @brief  enable or disable clkout direct to tmr10 channel 1
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_clkout_to_tmr10_Enable(confirm_state new_state) {
    CRM->misc2_bit.clk_to_tmr = new_state;
}

/**
  * @brief  config crm pll
  *                        pll_rcs_freq * pll_ns
  *         pll clock = --------------------------------
  *                           pll_ms * pll_fr_n
  *         attemtion:
  *                  31 <= pll_ns <= 500
  *                  1  <= pll_ms <= 15
  *
  *                       pll_rcs_freq
  *         2mhz <=  ---------------------- <= 16mhz
  *                          pll_ms
  *
  *                       pll_rcs_freq * pll_ns
  *         500mhz <=  -------------------------------- <= 1200mhz
  *                               pll_ms
  * @param  clock_source
  *         this parameter can be one of the following values:
  *         - CRM_PLL_Source_HICK
  *         - CRM_PLL_Source_HEXT
  * @param  pll_ns (31~500)
  * @param  pll_ms (1~15)
  * @param  pll_fr
  *         this parameter can be one of the following values:
  *         - CRM_PLL_FR_1
  *         - CRM_PLL_FR_2
  *         - CRM_PLL_FR_4
  *         - CRM_PLL_FR_8
  *         - CRM_PLL_FR_16
  *         - CRM_PLL_FR_32
  * @retval none
  */
void crm_pll_Config(crm_pll_Clock_Source_Type clock_source, uint16_t pll_ns, \
                    uint16_t pll_ms, crm_pll_fr_Type pll_fr) {
    /* config pll clock source */
    if(clock_source == CRM_PLL_Source_HICK) {
        CRM->misc1_bit.hickdiv = CRM_HICK48_NODIV;
    }

    CRM->pllcfg_bit.pllrcs = clock_source;

    /* config pll multiplication factor */
    CRM->pllcfg_bit.pllns = pll_ns;
    CRM->pllcfg_bit.pllms = pll_ms;
    CRM->pllcfg_bit.pllfr = pll_fr;
}

/**
  * @brief  select system clock source
  * @param  value
  *         this parameter can be one of the following values:
  *         - CRM_SCLK_HICK
  *         - CRM_SCLK_HEXT
  *         - CRM_SCLK_PLL
  * @retval none
  */
void crm_sysclk_switch(crm_sclk_Type value) {
    CRM->cfg_bit.sclksel = value;
}

/**
  * @brief  indicate which clock source is used as system clock
  * @param  none
  * @retval crm_sclk
  *         this return can be one of the following values:
  *         - CRM_SCLK_HICK
  *         - CRM_SCLK_HEXT
  *         - CRM_SCLK_PLL
  */
crm_sclk_Type crm_sysclk_switch_Status_Get(void) {
    return (crm_sclk_Type)CRM->cfg_bit.sclksts;
}

/**
  * @brief  get crm clocks freqency
  * @param  clocks_struct
  *         - pointer to the crm_clocks_Freq_Type structure
  * @retval none
  */
void crm_clocks_Freq_Get(crm_clocks_Freq_Type *clocks_struct) {
    uint32_t pll_ns = 0, pll_ms = 0, pll_fr = 0, pll_Clock_source = 0, pllrcsfreq = 0;
    uint32_t temp = 0, div_value = 0;
    crm_sclk_Type sclk_source;

    static const uint8_t sclk_ahb_div_table[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
    static const uint8_t ahb_apb1_div_table[8] = {0, 0, 0, 0, 1, 2, 3, 4};
    static const uint8_t ahb_apb2_div_table[8] = {0, 0, 0, 0, 1, 2, 3, 4};
    static const uint8_t pll_fr_table[6] = {1, 2, 4, 8, 16, 32};

    /* get sclk source */
    sclk_source = crm_sysclk_switch_Status_Get();

    switch(sclk_source) {
        case CRM_SCLK_HICK:
            if(((CRM->misc1_bit.hick_to_sclk) != RESET) && ((CRM->misc1_bit.hickdiv) != RESET))
                clocks_struct->sclk_freq = HICK_VALUE * 6;
            else
                clocks_struct->sclk_freq = HICK_VALUE;

            break;

        case CRM_SCLK_HEXT:
            clocks_struct->sclk_freq = HEXT_VALUE;
            break;

        case CRM_SCLK_PLL:
            /* get pll clock source */
            pll_Clock_source = CRM->pllcfg_bit.pllrcs;

            /* get multiplication factor */
            pll_ns = CRM->pllcfg_bit.pllns;
            pll_ms = CRM->pllcfg_bit.pllms;
            pll_fr = pll_fr_table[CRM->pllcfg_bit.pllfr];

            if (pll_Clock_source == CRM_PLL_Source_HICK) {
                /* hick selected as pll clock entry */
                pllrcsfreq = HICK_VALUE;
            } else {
                /* hext selected as pll clock entry */
                pllrcsfreq = HEXT_VALUE;
            }

            clocks_struct->sclk_freq = (uint32_t)(((uint64_t)pllrcsfreq * pll_ns) / (pll_ms * pll_fr));
            break;

        default:
            clocks_struct->sclk_freq = HICK_VALUE;
            break;
    }

    /* compute sclk, ahbclk, abp1clk and apb2clk frequencies */
    /* get ahb division */
    temp = CRM->cfg_bit.ahbdiv;
    div_value = sclk_ahb_div_table[temp];
    /* ahbclk frequency */
    clocks_struct->ahb_freq = clocks_struct->sclk_freq >> div_value;

    /* get apb1 division */
    temp = CRM->cfg_bit.apb1div;
    div_value = ahb_apb1_div_table[temp];
    /* apb1clk frequency */
    clocks_struct->apb1_freq = clocks_struct->ahb_freq >> div_value;

    /* get apb2 division */
    temp = CRM->cfg_bit.apb2div;
    div_value = ahb_apb2_div_table[temp];
    /* apb2clk frequency */
    clocks_struct->apb2_freq = clocks_struct->ahb_freq >> div_value;
}

/**
  * @brief  set crm clkout1
  * @param  clkout
  *         this parameter can be one of the following values:
  *         - CRM_CLKOUT1_HICK
  *         - CRM_CLKOUT1_LEXT
  *         - CRM_CLKOUT1_HEXT
  *         - CRM_CLKOUT1_PLL
  * @retval none
  */
void crm_Clock_out1_Set(crm_clkout1_Select_Type clkout) {
    CRM->cfg_bit.clkout1_sel = clkout;
}

/**
  * @brief  set crm clkout2
  * @param  clkout
  *         this parameter can be one of the following values:
  *         - CRM_CLKOUT2_SCLK
  *         - CRM_CLKOUT2_HEXT
  *         - CRM_CLKOUT2_PLL
  *         - CRM_CLKOUT2_USB
  *         - CRM_CLKOUT2_ADC
  *         - CRM_CLKOUT2_HICK
  *         - CRM_CLKOUT2_LICK
  *         - CRM_CLKOUT2_LEXT
  * @retval none
  */
void crm_Clock_out2_Set(crm_clkout2_Select_Type clkout) {
    if(clkout < 0x10) {
        CRM->cfg_bit.clkout2_sel1 = (clkout & 0x3);
    } else {
        CRM->cfg_bit.clkout2_sel1 = 0x1;
        CRM->misc1_bit.clkout2_sel2 = (clkout & 0xF);
    }
}

/**
  * @brief  set crm clkout1 division1
  * @param  div1
  *         this parameter can be one of the following values:
  *         - CRM_CLKOUT_INDEX_1
  *         - CRM_CLKOUT_INDEX_2
  * @param  div1
  *         this parameter can be one of the following values:
  *         - CRM_CLKOUT_DIV1_1
  *         - CRM_CLKOUT_DIV1_2
  *         - CRM_CLKOUT_DIV1_3
  *         - CRM_CLKOUT_DIV1_4
  *         - CRM_CLKOUT_DIV1_5
  * @param  div2
  *         this parameter can be one of the following values:
  *         - CRM_CLKOUT_DIV2_1
  *         - CRM_CLKOUT_DIV2_2
  *         - CRM_CLKOUT_DIV2_4
  *         - CRM_CLKOUT_DIV2_8
  *         - CRM_CLKOUT_DIV2_16
  *         - CRM_CLKOUT_DIV2_64
  *         - CRM_CLKOUT_DIV2_128
  *         - CRM_CLKOUT_DIV2_256
  *         - CRM_CLKOUT_DIV2_512
  * @retval none
  */
void crm_clkout_div_Set(crm_clkout_index_Type index, crm_clkout_div1_Type div1, crm_clkout_div2_Type div2) {
    if(index == CRM_CLKOUT_INDEX_1) {
        CRM->cfg_bit.clkout1div1 = div1;
        CRM->misc1_bit.clkout1div2 = div2;
    } else {
        CRM->cfg_bit.clkout2div1 = div1;
        CRM->misc1_bit.clkout2div2 = div2;
    }
}

/**
  * @brief  set emac output pulse width
  * @param  width
  *         this parameter can be one of the following values:
  *         - CRM_EMAC_PULSE_125MS
  *         - CRM_EMAC_PULSE_1SCLK
  * @retval none
  */
void crm_EMAC_OutPut_pulse_Set(crm_EMAC_OutPut_pulse_Type width) {
    CRM->misc2_bit.EMAC_PPS_sel = width;
}

/**
  * @brief  config crm interrupt
  * @param  int
  *         this parameter can be any combination of the following values:
  *         - CRM_LICK_STABLE_INT
  *         - CRM_LEXT_STABLE_INT
  *         - CRM_HICK_STABLE_INT
  *         - CRM_HEXT_STABLE_INT
  *         - CRM_PLL_STABLE_INT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void crm_Interrupt_Enable(uint32_t crm_int, confirm_state new_state) {
    if(TRUE == new_state)
        CRM->clkint |= crm_int;
    else
        CRM->clkint &= ~crm_int;
}

/**
  * @brief  calculate the pll parameters with pll reference clock and target pll output frequency.
  *                        pll_rcs_freq * pll_ns
  *         pll clock = --------------------------------
  *                           pll_ms * pll_fr_n
  *         attemtion:
  *                  31 <= pll_ns <= 500
  *                  1  <= pll_ms <= 15
  *
  *                       pll_rcs_freq
  *         2mhz <=  ---------------------- <= 16mhz
  *                          pll_ms
  *
  *                       pll_rcs_freq * pll_ns
  *         500mhz <=  -------------------------------- <= 1200mhz
  *                               pll_ms
  * @param  pll_rcs
  *         this parameter can be one of the following values:
  *         - CRM_PLL_Source_HICK
  *         - CRM_PLL_Source_HEXT
  * @param  target_sclk_freq: target pll output frequency, such as 200 mhz (target_sclk_freq: 200000000)
  * @param  ret_ms: pointer to ms value, return the pll_ms of pll parameters
  * @param  ret_ns: pointer to ns value, return the pll_ns of pll parameters
  * @param  ret_fr: pointer to fr value, return the pll_fr of pll parameters
  * @retval error_status (SUCCESS or ERROR)
  */
error_status crm_pll_parameter_calculate(crm_pll_Clock_Source_Type pll_rcs, uint32_t target_sclk_freq, \
        uint16_t *ret_ms, uint16_t *ret_ns, uint16_t *ret_fr) {
    int32_t pll_rcs_freq = 0, ns = 0, ms = 0, fr = 0;
    uint32_t ms_min = 0, ms_max = 0, error_min = 0xFFFFFFFF;
    uint32_t result = 0, absolute_value = 0;

    /* reduce calculate accuracy, target_sclk_freq accuracy with khz */
    target_sclk_freq = target_sclk_freq / 1000;

    /* get pll reference clock frequency, accuracy with khz */
    if(pll_rcs == CRM_PLL_Source_HICK)
        pll_rcs_freq = HICK_VALUE / 1000;
    else
        pll_rcs_freq = HEXT_VALUE / 1000;

    /* polling ms range, accuracy with khz */
    for(ms = 1; ms <= 15; ms ++) {
        result = pll_rcs_freq / ms;

        if((result >= 2000U) && (result <= 16000U)) {
            if(ms_min == 0)
                ms_min = ms;

            ms_max = ms;
        }
    }

    /* polling pll parameters */
    for(ms = ms_min; ms <= ms_max; ms ++) {
        for(fr = 5; fr >= 0; fr --) {
            for(ns = 31; ns <= 500; ns ++) {
                result = (pll_rcs_freq * ns) / (ms);

                /* check vco frequency range, accuracy with khz */
                if((result < 500000U) || (result > 1200000U)) {
                    continue;
                }

                /* calculate pll output frequency */
                result = result / (0x1 << fr);

                /* check frequency */
                if(target_sclk_freq == result) {
                    *ret_ms = ms;
                    *ret_ns = ns;
                    *ret_fr = fr;
                    /* the pll parameters that is equal to target_sclk_freq */
                    return SUCCESS;
                }

                /* calculate error range, accuracy with khz */
                absolute_value = (result > target_sclk_freq) ? (result - target_sclk_freq) : (target_sclk_freq - result);

                if(absolute_value < error_min) {
                    error_min = absolute_value;
                    *ret_ms = ms;
                    *ret_ns = ns;
                    *ret_fr = fr;
                }
            }
        }
    }

    /* the pll parameters that is the closest approach to target_sclk_freq */
    return ERROR;
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
