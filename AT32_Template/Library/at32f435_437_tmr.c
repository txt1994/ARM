/**
  **************************************************************************
  * @file     at32f435_437_tmr.c
  * @brief    contains all the functions for the tmr firmware library
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

/** @defgroup TMR
  * @brief TMR driver modules
  * @{
  */

#ifdef TMR_MODULE_EnableD

/** @defgroup TMR_private_functions
  * @{
  */

/**
  * @brief  tmr reset by crm reset register
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @retval none
  */
void tmr_Reset(tmr_Type *tmr_x) {
    if(tmr_x == TMR1) {
        crm_periph_Reset(CRM_TMR1_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR1_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR2) {
        crm_periph_Reset(CRM_TMR2_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR2_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR3) {
        crm_periph_Reset(CRM_TMR3_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR3_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR4) {
        crm_periph_Reset(CRM_TMR4_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR4_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR5) {
        crm_periph_Reset(CRM_TMR5_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR5_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR6) {
        crm_periph_Reset(CRM_TMR6_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR6_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR7) {
        crm_periph_Reset(CRM_TMR7_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR7_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR8) {
        crm_periph_Reset(CRM_TMR8_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR8_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR9) {
        crm_periph_Reset(CRM_TMR9_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR9_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR10) {
        crm_periph_Reset(CRM_TMR10_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR10_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR11) {
        crm_periph_Reset(CRM_TMR11_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR11_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR12) {
        crm_periph_Reset(CRM_TMR12_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR12_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR13) {
        crm_periph_Reset(CRM_TMR13_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR13_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR14) {
        crm_periph_Reset(CRM_TMR14_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR14_PERIPH_Reset, FALSE);
    } else if(tmr_x == TMR20) {
        crm_periph_Reset(CRM_TMR20_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_TMR20_PERIPH_Reset, FALSE);
    }
}

/**
  * @brief  enable or disable tmr counter
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_Counter_Enable(tmr_Type *tmr_x, confirm_state new_state) {
    /* tmr counter enable  */
    tmr_x->ctrl1_bit.tmren = new_state;
}

/**
  * @brief  init tmr output default para
  * @param  tmr_OutPut_struct
  *         - to the structure of tmr_OutPut_Config_Type
  * @retval none
  */
void tmr_OutPut_Default_Para_Init(tmr_OutPut_Config_Type *tmr_OutPut_struct) {
    tmr_OutPut_struct->oc_Mode = TMR_OutPut_Control_OFF;
    tmr_OutPut_struct->oc_idle_state = FALSE;
    tmr_OutPut_struct->occ_idle_state = FALSE;
    tmr_OutPut_struct->oc_polarity = TMR_OutPut_ACTIVE_HIGH;
    tmr_OutPut_struct->occ_polarity = TMR_OutPut_ACTIVE_HIGH;
    tmr_OutPut_struct->oc_OutPut_state = FALSE;
    tmr_OutPut_struct->occ_OutPut_state = FALSE;
}

/**
  * @brief  init tmr input default para
  * @param  tmr_Input_struct
  *         - to the structure of tmr_Input_Config_Type
  * @retval none
  */
void tmr_Input_Default_Para_Init(tmr_Input_Config_Type *tmr_Input_struct) {
    tmr_Input_struct->input_Channel_Select = TMR_Select_Channel_1;
    tmr_Input_struct->input_polarity_Select = TMR_Input_RISING_EDGE;
    tmr_Input_struct->input_mapped_Select = TMR_CC_Channel_MAPPED_DIRECT;
    tmr_Input_struct->input_Filter_value = 0x0;
}

/**
  * @brief  init tmr brkdt default para
  * @param  tmr_brkdt_struct
  *         - to the structure of tmr_brkdt_Config_Type
  * @retval none
  */
void tmr_brkdt_Default_Para_Init(tmr_brkdt_Config_Type *tmr_brkdt_struct) {
    tmr_brkdt_struct->deadtime = 0x0;
    tmr_brkdt_struct->brk_polarity = TMR_BRK_Input_ACTIVE_LOW;
    tmr_brkdt_struct->wp_level = TMR_WP_OFF;
    tmr_brkdt_struct->auto_OutPut_Enable = FALSE ;
    tmr_brkdt_struct->fcsoen_state = FALSE ;
    tmr_brkdt_struct->fcsodis_state = FALSE ;
    tmr_brkdt_struct->brk_Enable = FALSE ;
}

/**
  * @brief  init tmr base
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_pr (for 16 bit tmr 0x0000~0xFFFF,
  *                  for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @param  tmr_div (timer div value:0x0000~0xFFFF)
  * @retval none
  */
void tmr_base_Init(tmr_Type* tmr_x, uint32_t tmr_pr, uint32_t tmr_div) {
    /* set the pr value */
    tmr_x->pr = tmr_pr;

    /* set the div value */
    tmr_x->div = tmr_div;

    /* trigger the overflow event to immediately reload pr value and div value */
    tmr_x->swevt_bit.ovfswtr = TRUE;
}

/**
  * @brief  set tmr clock source division
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_Clock_div
  *         this parameter can be one of the following values:
  *         - TMR_Clock_DIV1
  *         - TMR_Clock_DIV2
  *         - TMR_Clock_DIV4
  * @retval none
  */
void tmr_Clock_Source_div_Set(tmr_Type *tmr_x, tmr_Clock_Division_Type tmr_Clock_div) {
    /* set tmr clock source division */
    tmr_x->ctrl1_bit.clkdiv = tmr_Clock_div;
}

/**
  * @brief  set tmr counter count direction
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_cnt_dir
  *         this parameter can be one of the following values:
  *         - TMR_COUNT_UP
  *         - TMR_COUNT_DOWN
  *         - TMR_COUNT_TWO_WAY_1
  *         - TMR_COUNT_TWO_WAY_2
  *         - TMR_COUNT_TWO_WAY_3
  * @retval none
  */
void tmr_cnt_dir_Set(tmr_Type *tmr_x, tmr_count_Mode_Type tmr_cnt_dir) {
    /* set the cnt direct */
    tmr_x->ctrl1_bit.cnt_dir = tmr_cnt_dir;
}

/**
  * @brief  set the repetition counter register(rpr) value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8, TMR20
  * @param  tmr_rpr_value (0x0000~0xFFFF)
  * @retval none
  */
void tmr_repetition_Counter_Set(tmr_Type *tmr_x, uint8_t tmr_rpr_value) {
    /* set the repetition counter value */
    tmr_x->rpr_bit.rpr = tmr_rpr_value;
}

/**
  * @brief  set tmr counter value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_cnt_value (for 16 bit tmr 0x0000~0xFFFF,
  *                        for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @retval none
  */
void tmr_Counter_Value_Set(tmr_Type *tmr_x, uint32_t tmr_cnt_value) {
    /* set the tmr counter value */
    tmr_x->cval = tmr_cnt_value;
}

/**
  * @brief  get tmr counter value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @retval tmr counter value
  */
uint32_t tmr_Counter_Value_Get(tmr_Type *tmr_x) {
    return tmr_x->cval;
}

/**
  * @brief  set tmr div value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_div_value (for 16 bit tmr 0x0000~0xFFFF,
  *                        for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @retval none
  */
void tmr_div_Value_Set(tmr_Type *tmr_x, uint32_t tmr_div_value) {
    /* set the tmr div value */
    tmr_x->div = tmr_div_value;
}

/**
  * @brief  get tmr div value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @retval tmr div value
  */
uint32_t tmr_div_Value_Get(tmr_Type *tmr_x) {
    return tmr_x->div;
}

/**
  * @brief  config tmr output channel
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_5
  * @param  tmr_OutPut_struct
  *         - to the structure of tmr_OutPut_Config_Type
  * @retval none
  */
void tmr_OutPut_Channel_Config(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                               tmr_OutPut_Config_Type *tmr_OutPut_struct) {
    uint16_t channel_index = 0, channel_c_index = 0, channel = 0, chx_offset, chcx_offset;

    chx_offset = (8 + tmr_channel);
    chcx_offset = (9 + tmr_channel);

    /* get channel idle state bit position in ctrl2 register */
    channel_index = (uint16_t)(tmr_OutPut_struct->oc_idle_state << chx_offset);

    /* get channel complementary idle state bit position in ctrl2 register */
    channel_c_index = (uint16_t)(tmr_OutPut_struct->occ_idle_state << chcx_offset);

    /* set output channel complementary idle state */
    tmr_x->ctrl2 &= ~(1 << chcx_offset);
    tmr_x->ctrl2 |= channel_c_index;

    /* set output channel idle state */
    tmr_x->ctrl2 &= ~(1 << chx_offset);
    tmr_x->ctrl2 |= channel_index;

    /* set channel output mode */
    channel = tmr_channel;

    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cm1_OutPut_bit.c1octrl = tmr_OutPut_struct->oc_Mode;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cm1_OutPut_bit.c2octrl = tmr_OutPut_struct->oc_Mode;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cm2_OutPut_bit.c3octrl = tmr_OutPut_struct->oc_Mode;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cm2_OutPut_bit.c4octrl = tmr_OutPut_struct->oc_Mode;
            break;

        case TMR_Select_Channel_5:
            tmr_x->cm3_OutPut_bit.c5octrl = tmr_OutPut_struct->oc_Mode;
            break;

        default:
            break;
    }

    chx_offset = ((tmr_channel * 2) + 1);
    chcx_offset = ((tmr_channel * 2) + 3);

    /* get channel polarity bit position in cctrl register */
    channel_index = (uint16_t)(tmr_OutPut_struct->oc_polarity << chx_offset);

    /* get channel complementary polarity bit position in cctrl register */
    channel_c_index = (uint16_t)(tmr_OutPut_struct->occ_polarity << chcx_offset);

    /* set output channel complementary polarity */
    tmr_x->cctrl &= ~(1 << chcx_offset);
    tmr_x->cctrl |= channel_c_index;

    /* set output channel polarity */
    tmr_x->cctrl &= ~(1 << chx_offset);
    tmr_x->cctrl |= channel_index;

    chx_offset = (tmr_channel * 2);
    chcx_offset = ((tmr_channel * 2) + 2);

    /* get channel enable bit position in cctrl register */
    channel_index = (uint16_t)(tmr_OutPut_struct->oc_OutPut_state << (tmr_channel * 2));

    /* get channel complementary enable bit position in cctrl register */
    channel_c_index = (uint16_t)(tmr_OutPut_struct->occ_OutPut_state << ((tmr_channel * 2) + 2));

    /* set output channel complementary enable bit */
    tmr_x->cctrl &= ~(1 << chcx_offset);
    tmr_x->cctrl |= channel_c_index;

    /* set output channel enable bit */
    tmr_x->cctrl &= ~(1 << chx_offset);
    tmr_x->cctrl |= channel_index;
}

/**
  * @brief  select tmr output channel mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_5
  * @param  oc_Mode
  *         this parameter can be one of the following values:
  *         - TMR_OutPut_Control_OFF
  *         - TMR_OutPut_Control_HIGH
  *         - TMR_OutPut_Control_LOW
  *         - TMR_OutPut_Control_SWITCH
  *         - TMR_OutPut_Control_FORCE_HIGH
  *         - TMR_OutPut_Control_FORCE_LOW
  *         - TMR_OutPut_Control_PWM_Mode_A
  *         - TMR_OutPut_Control_PWM_Mode_B
  * @retval none
  */
void tmr_OutPut_Channel_Mode_Select(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                    tmr_OutPut_Control_Mode_Type oc_Mode) {
    uint16_t channel;

    channel = tmr_channel;

    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cm1_OutPut_bit.c1octrl = oc_Mode;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cm1_OutPut_bit.c2octrl = oc_Mode;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cm2_OutPut_bit.c3octrl = oc_Mode;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cm2_OutPut_bit.c4octrl = oc_Mode;
            break;

        case TMR_Select_Channel_5:
            tmr_x->cm3_OutPut_bit.c5octrl = oc_Mode;
            break;

        default:
            break;
    }
}
/**
  * @brief  set tmr period value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_pr_value: timer period register value of counter
  *                       (for 16 bit tmr 0x0000~0xFFFF,
  *                       for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @retval none
  */
void tmr_period_Value_Set(tmr_Type *tmr_x, uint32_t tmr_pr_value) {
    /* set tmr period value */
    tmr_x->pr = tmr_pr_value;
}

/**
  * @brief  get tmr period value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @retval timer period register value of counter
  *         (for 16 bit tmr 0x0000~0xFFFF, for 32 bit tmr
  *         0x0000_0000~0xFFFF_FFFF)
  */
uint32_t tmr_period_Value_Get(tmr_Type *tmr_x) {
    return tmr_x->pr;
}

/**
  * @brief  set tmr channel value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_5
  * @param  tmr_Channel_value (for 16 bit tmr 0x0000~0xFFFF,
  *                       for 32 bit tmr 0x0000_0000~0xFFFF_FFFF)
  * @retval none
  */
void tmr_Channel_Value_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                           uint32_t tmr_Channel_value) {
    uint16_t channel;

    channel = tmr_channel;

    /* set tmr channel value */
    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->c1dt = tmr_Channel_value;
            break;

        case TMR_Select_Channel_2:
            tmr_x->c2dt = tmr_Channel_value;
            break;

        case TMR_Select_Channel_3:
            tmr_x->c3dt = tmr_Channel_value;
            break;

        case TMR_Select_Channel_4:
            tmr_x->c4dt = tmr_Channel_value;
            break;

        case TMR_Select_Channel_5:
            tmr_x->c5dt = tmr_Channel_value;
            break;

        default:
            break;
    }
}

/**
  * @brief  get tmr channel value
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_5
  * @retval tmr channel value
  */
uint32_t tmr_Channel_Value_Get(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel) {
    uint32_t cc_Value_Get = 0;
    uint16_t channel;

    channel = tmr_channel;

    /* get tmr channel value */
    switch(channel) {
        case TMR_Select_Channel_1:
            cc_Value_Get = tmr_x->c1dt;
            break;

        case TMR_Select_Channel_2:
            cc_Value_Get = tmr_x->c2dt;
            break;

        case TMR_Select_Channel_3:
            cc_Value_Get = tmr_x->c3dt;
            break;

        case TMR_Select_Channel_4:
            cc_Value_Get = tmr_x->c4dt;
            break;

        case TMR_Select_Channel_5:
            cc_Value_Get = tmr_x->c5dt;
            break;

        default:
            break;
    }

    return cc_Value_Get;
}
/**
  * @brief  set tmr period buffer
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_period_buffer_Enable(tmr_Type *tmr_x, confirm_state new_state) {
    /* tmr period buffer set */
    tmr_x->ctrl1_bit.prben = new_state;
}

/**
  * @brief  set tmr output channel buffer
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_5
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_OutPut_Channel_buffer_Enable(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                      confirm_state new_state) {
    uint16_t channel;

    channel = tmr_channel;

    /* get tmr channel value */
    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cm1_OutPut_bit.c1oben = new_state;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cm1_OutPut_bit.c2oben = new_state;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cm2_OutPut_bit.c3oben = new_state;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cm2_OutPut_bit.c4oben = new_state;
            break;

        case TMR_Select_Channel_5:
            tmr_x->cm3_OutPut_bit.c5oben = new_state;
            break;

        default:
            break;
    }
}

/**
  * @brief  set tmr output channel immediately
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_5
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_OutPut_Channel_immediately_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                        confirm_state new_state) {
    uint16_t channel;

    channel = tmr_channel;

    /* get tmr channel value */
    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cm1_OutPut_bit.c1oien = new_state;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cm1_OutPut_bit.c2oien = new_state;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cm2_OutPut_bit.c3oien = new_state;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cm2_OutPut_bit.c4oien = new_state;
            break;

        case TMR_Select_Channel_5:
            tmr_x->cm3_OutPut_bit.c5oien = new_state;
            break;

        default:
            break;
    }
}

/**
  * @brief  set tmr output channel switch
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_5
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_OutPut_Channel_switch_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                   confirm_state new_state) {
    uint16_t channel;

    channel = tmr_channel;

    /* get tmr channel value */
    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cm1_OutPut_bit.c1osen = new_state;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cm1_OutPut_bit.c2osen = new_state;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cm2_OutPut_bit.c3osen = new_state;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cm2_OutPut_bit.c4osen = new_state;
            break;

        case TMR_Select_Channel_5:
            tmr_x->cm3_OutPut_bit.c5osen = new_state;
            break;

        default:
            break;
    }
}

/**
  * @brief  enable or disable tmr one cycle mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8, TMR9, TMR12, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_One_cycle_Mode_Enable(tmr_Type *tmr_x, confirm_state new_state) {
    /* tmr one cycle mode enable */
    tmr_x->ctrl1_bit.ocmen = new_state;
}

/**
  * @brief  enable or disable  tmr 32 bit function(plus mode)
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR2, TMR5
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_32_Bit_function_Enable (tmr_Type *tmr_x, confirm_state new_state) {
    /* tmr 32 bit function(plus mode) enable,only for TMR2/TMR5 */
    if((tmr_x == TMR2) || (tmr_x == TMR5)) {
        tmr_x->ctrl1_bit.pmen = new_state;
    }
}

/**
  * @brief  select tmr the overflow event sources
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_OverFlow_Request_Source_Set(tmr_Type *tmr_x, confirm_state new_state) {
    tmr_x->ctrl1_bit.ovfs = new_state;
}

/**
  * @brief  enable or disable tmr overflow event generation
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_OverFlow_Event_Disable(tmr_Type *tmr_x, confirm_state new_state) {
    tmr_x->ctrl1_bit.ovfen = new_state;
}

/**
  * @brief  init tmr input channel
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  input_struct
  *         - to the structure of tmr_Input_Config_Type
  * @param  divider_factor
  *         this parameter can be one of the following values:
  *         - TMR_Channel_Input_DIV_1
  *         - TMR_Channel_Input_DIV_2
  *         - TMR_Channel_Input_DIV_4
  *         - TMR_Channel_Input_DIV_8
  * @retval none
  */
void tmr_Input_Channel_Init(tmr_Type *tmr_x, tmr_Input_Config_Type *input_struct, \
                            tmr_Channel_Input_Divider_Type divider_factor) {
    uint16_t channel = 0;

    /* get channel selected */
    channel = input_struct->input_Channel_Select;

    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cctrl_bit.c1en       = FALSE;
            tmr_x->cctrl_bit.c1p        = (uint32_t)input_struct->input_polarity_Select;
            tmr_x->cctrl_bit.c1cp       = (input_struct->input_polarity_Select & 0x2) >> 1;
            tmr_x->cm1_Input_bit.c1c    = input_struct->input_mapped_Select;
            tmr_x->cm1_Input_bit.c1df   = input_struct->input_Filter_value;
            tmr_x->cm1_Input_bit.c1idiv = divider_factor;
            tmr_x->cctrl_bit.c1en       = TRUE;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cctrl_bit.c2en       = FALSE;
            tmr_x->cctrl_bit.c2p        = (uint32_t)input_struct->input_polarity_Select;
            tmr_x->cctrl_bit.c2cp       = (input_struct->input_polarity_Select & 0x2) >> 1;
            tmr_x->cm1_Input_bit.c2c    = input_struct->input_mapped_Select;
            tmr_x->cm1_Input_bit.c2df   = input_struct->input_Filter_value;
            tmr_x->cm1_Input_bit.c2idiv = divider_factor;
            tmr_x->cctrl_bit.c2en       = TRUE;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cctrl_bit.c3en       = FALSE;
            tmr_x->cctrl_bit.c3p        = (uint32_t)input_struct->input_polarity_Select;
            tmr_x->cctrl_bit.c3cp       = (input_struct->input_polarity_Select & 0x2) >> 1;
            tmr_x->cm2_Input_bit.c3c    = input_struct->input_mapped_Select;
            tmr_x->cm2_Input_bit.c3df   = input_struct->input_Filter_value;
            tmr_x->cm2_Input_bit.c3idiv = divider_factor;
            tmr_x->cctrl_bit.c3en       = TRUE;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cctrl_bit.c4en       = FALSE;
            tmr_x->cctrl_bit.c4p        = (uint32_t)input_struct->input_polarity_Select;
            tmr_x->cm2_Input_bit.c4c    = input_struct->input_mapped_Select;
            tmr_x->cm2_Input_bit.c4df   = input_struct->input_Filter_value;
            tmr_x->cm2_Input_bit.c4idiv = divider_factor;
            tmr_x->cctrl_bit.c4en       = TRUE;
            break;

        default:
            break;
    }
}

/**
  * @brief  tmr channel enable
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_1C
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_2C
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_3C
  *         - TMR_Select_Channel_4
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_Channel_Enable(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, confirm_state new_state) {
    uint16_t channel;

    channel = tmr_channel;

    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cctrl_bit.c1en         = new_state;
            break;

        case TMR_Select_Channel_1C:
            tmr_x->cctrl_bit.c1cen        = new_state;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cctrl_bit.c2en         = new_state;
            break;

        case TMR_Select_Channel_2C:
            tmr_x->cctrl_bit.c2cen        = new_state;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cctrl_bit.c3en         = new_state;
            break;

        case TMR_Select_Channel_3C:
            tmr_x->cctrl_bit.c3cen        = new_state;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cctrl_bit.c4en         = new_state;
            break;

        default:
            break;
    }
}

/**
  * @brief  set tmr input channel filter
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  * @param  filter_value (0x0~0xf)
  * @retval none
  */
void tmr_Input_Channel_Filter_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                  uint16_t filter_value) {
    uint16_t channel;

    channel = tmr_channel;

    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cm1_Input_bit.c1df = filter_value;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cm1_Input_bit.c2df = filter_value;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cm2_Input_bit.c3df = filter_value;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cm2_Input_bit.c4df = filter_value;
            break;

        default:
            break;
    }
}

/**
  * @brief  config tmr pwm input
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  input_struct
  *         - to the structure of tmr_Input_Config_Type
  * @param  divider_factor
  *         this parameter can be one of the following values:
  *         - TMR_Channel_Input_DIV_1
  *         - TMR_Channel_Input_DIV_2
  *         - TMR_Channel_Input_DIV_4
  *         - TMR_Channel_Input_DIV_8
  * @retval none
  */
void tmr_pwm_Input_Config(tmr_Type *tmr_x, tmr_Input_Config_Type *input_struct, \
                          tmr_Channel_Input_Divider_Type divider_factor) {
    uint16_t channel = 0;

    /* get channel selected */
    channel = input_struct->input_Channel_Select;

    switch(channel) {
        case TMR_Select_Channel_1:
            if(input_struct->input_polarity_Select == TMR_Input_RISING_EDGE) {
                /* set channel polarity */
                tmr_x->cctrl_bit.c1p          = TMR_Input_RISING_EDGE;
                tmr_x->cctrl_bit.c2p          = TMR_Input_FALLING_EDGE;
            } else if(input_struct->input_polarity_Select == TMR_Input_FALLING_EDGE) {
                /* set channel polarity */
                tmr_x->cctrl_bit.c1p          = TMR_Input_FALLING_EDGE;
                tmr_x->cctrl_bit.c2p          = TMR_Input_RISING_EDGE;
            }

            if(input_struct->input_mapped_Select == TMR_CC_Channel_MAPPED_DIRECT) {
                /* ic1 is mapped on ti1 */
                tmr_x->cm1_Input_bit.c1c = TMR_CC_Channel_MAPPED_DIRECT;

                /* ic1 is mapped on ti2 */
                tmr_x->cm1_Input_bit.c2c = TMR_CC_Channel_MAPPED_INDIRECT;
            } else if(input_struct->input_mapped_Select == TMR_CC_Channel_MAPPED_INDIRECT) {
                /* ic1 is mapped on ti1 */
                tmr_x->cm1_Input_bit.c1c = TMR_CC_Channel_MAPPED_INDIRECT;

                /* ic1 is mapped on ti2 */
                tmr_x->cm1_Input_bit.c2c = TMR_CC_Channel_MAPPED_DIRECT;
            }

            /* set input ch1 and ch2 filter value*/
            tmr_x->cm1_Input_bit.c1df = input_struct->input_Filter_value;
            tmr_x->cm1_Input_bit.c2df = input_struct->input_Filter_value;

            /*set input ch1 and ch2 divider value*/
            tmr_x->cm1_Input_bit.c1idiv = divider_factor;
            tmr_x->cm1_Input_bit.c2idiv = divider_factor;

            tmr_x->cctrl_bit.c1en = TRUE;
            tmr_x->cctrl_bit.c2en = TRUE;
            break;

        case TMR_Select_Channel_2:
            if(input_struct->input_polarity_Select == TMR_Input_RISING_EDGE) {
                /* set channel polarity */
                tmr_x->cctrl_bit.c2p          = TMR_Input_RISING_EDGE;
                tmr_x->cctrl_bit.c1p          = TMR_Input_FALLING_EDGE;
            } else if(input_struct->input_polarity_Select == TMR_Input_FALLING_EDGE) {
                /* set channel polarity */
                tmr_x->cctrl_bit.c2p          = TMR_Input_FALLING_EDGE;
                tmr_x->cctrl_bit.c1p          = TMR_Input_RISING_EDGE;
            }

            if(input_struct->input_mapped_Select == TMR_CC_Channel_MAPPED_DIRECT) {
                /* set mapped direct */
                tmr_x->cm1_Input_bit.c2c = TMR_CC_Channel_MAPPED_DIRECT;
                tmr_x->cm1_Input_bit.c1c = TMR_CC_Channel_MAPPED_INDIRECT;
            } else if(input_struct->input_mapped_Select == TMR_CC_Channel_MAPPED_INDIRECT) {
                /* set mapped direct */
                tmr_x->cm1_Input_bit.c2c = TMR_CC_Channel_MAPPED_INDIRECT;
                tmr_x->cm1_Input_bit.c1c = TMR_CC_Channel_MAPPED_DIRECT;
            }

            /* set input ch1 and ch2 filter value*/
            tmr_x->cm1_Input_bit.c1df = input_struct->input_Filter_value;
            tmr_x->cm1_Input_bit.c2df = input_struct->input_Filter_value;

            /*set input ch1 and ch2 divider value*/
            tmr_x->cm1_Input_bit.c1idiv = divider_factor;
            tmr_x->cm1_Input_bit.c2idiv = divider_factor;

            tmr_x->cctrl_bit.c1en = TRUE;
            tmr_x->cctrl_bit.c2en = TRUE;
            break;

        default:
            break;
    }
}

/**
  * @brief  select tmr channel1 input
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR20
  * @param  ch1_Connect
  *         this parameter can be one of the following values:
  *         - TMR_CHANEL1_ConnectED_C1IRAW
  *         - TMR_CHANEL1_2_3_ConnectED_C1IRAW_XOR
  * @retval none
  */
void tmr_channel1_Input_Select(tmr_Type *tmr_x, tmr_channel1_Input_Connected_Type ch1_Connect) {
    tmr_x->ctrl2_bit.c1insel = ch1_Connect;
}

/**
  * @brief  set tmr input channel divider
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  * @param  divider_factor
  *         this parameter can be one of the following values:
  *         - TMR_Channel_Input_DIV_1
  *         - TMR_Channel_Input_DIV_2
  *         - TMR_Channel_Input_DIV_4
  *         - TMR_Channel_Input_DIV_8
  * @retval none
  */
void tmr_Input_Channel_Divider_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                   tmr_Channel_Input_Divider_Type divider_factor) {
    uint16_t channel;

    channel = tmr_channel;

    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cm1_Input_bit.c1idiv = divider_factor;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cm1_Input_bit.c2idiv = divider_factor;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cm2_Input_bit.c3idiv = divider_factor;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cm2_Input_bit.c4idiv = divider_factor;
            break;

        default:
            break;
    }
}

/**
  * @brief  select tmr primary mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8, TMR20
  * @param  primary_Mode
  *         this parameter can be one of the following values:
  *         - TMR_PRIMARY_SEL_Reset
  *         - TMR_PRIMARY_SEL_Enable
  *         - TMR_PRIMARY_SEL_OVERFLOW
  *         - TMR_PRIMARY_SEL_COMPARE
  *         - TMR_PRIMARY_SEL_C1ORAW
  *         - TMR_PRIMARY_SEL_C2ORAW
  *         - TMR_PRIMARY_SEL_C3ORAW
  *         - TMR_PRIMARY_SEL_C4ORAW
  * @retval none
  */
void tmr_primary_Mode_Select(tmr_Type *tmr_x, tmr_primary_Select_Type primary_Mode) {
    tmr_x->ctrl2_bit.ptos = primary_Mode;
}

/**
  * @brief  select tmr subordinate mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12, TMR20
  * @param  sub_Mode
  *         this parameter can be one of the following values:
  *         - TMR_Sub_Mode_DIABLE
  *         - TMR_Sub_ENCODER_Mode_A
  *         - TMR_Sub_ENCODER_Mode_B
  *         - TMR_Sub_ENCODER_Mode_C
  *         - TMR_Sub_Reset_Mode
  *         - TMR_Sub_HANG_Mode
  *         - TMR_Sub_Trigger_Mode
  *         - TMR_Sub_EXTERNAL_Clock_Mode_A

  * @retval none
  */
void tmr_Sub_Mode_Select(tmr_Type *tmr_x, tmr_Sub_Mode_Select_Type sub_Mode) {
    tmr_x->stctrl_bit.smsel = sub_Mode;
}

/**
  * @brief  select tmr channel dma
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12, TMR20
  * @param  cc_DMA_Select
  *         this parameter can be one of the following values:
  *         - TMR_DMA_Request_BY_CHANNEL
  *         - TMR_DMA_Request_BY_OVERFLOW
  * @retval none
  */
void tmr_Channel_DMA_Select(tmr_Type *tmr_x, tmr_DMA_Request_Source_Type cc_DMA_Select) {
    tmr_x->ctrl2_bit.drs = cc_DMA_Select;
}

/**
  * @brief  select tmr hall
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_hall_Select(tmr_Type *tmr_x,  confirm_state new_state) {
    tmr_x->ctrl2_bit.ccfs = new_state;
}

/**
  * @brief  select tmr channel buffer
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_Channel_buffer_Enable(tmr_Type *tmr_x, confirm_state new_state) {
    tmr_x->ctrl2_bit.cbctrl = new_state;
}

/**
  * @brief  select tmr trgout2
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_trgout2_Enable(tmr_Type *tmr_x, confirm_state new_state) {
    tmr_x->ctrl2_bit.trgout2en = new_state;
}

/**
  * @brief  select tmr sub-trigger
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12, TMR20
  * @param  trigger_Select
  *         this parameter can be one of the following values:
  *         - TMR_Sub_Input_SEL_IS0
  *         - TMR_Sub_Input_SEL_IS1
  *         - TMR_Sub_Input_SEL_IS2
  *         - TMR_Sub_Input_SEL_IS3
  *         - TMR_Sub_Input_SEL_C1INC
  *         - TMR_Sub_Input_SEL_C1DF1
  *         - TMR_Sub_Input_SEL_C2DF2
  *         - TMR_Sub_Input_SEL_EXTIN
  * @retval none
  */
void tmr_Trigger_Input_Select(tmr_Type *tmr_x, sub_tmr_Input_sel_Type trigger_Select) {
    tmr_x->stctrl_bit.stis = trigger_Select;
}

/**
  * @brief  set tmr subordinate synchronization mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_Sub_Sync_Mode_Set(tmr_Type *tmr_x, confirm_state new_state) {
    tmr_x->stctrl_bit.sts = new_state;
}

/**
  * @brief  enable or disable tmr dma request
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  DMA_request
  *         this parameter can be one of the following values:
  *         - TMR_OverFlow_DMA_REQUEST
  *         - TMR_C1_DMA_REQUEST
  *         - TMR_C2_DMA_REQUEST
  *         - TMR_C3_DMA_REQUEST
  *         - TMR_C4_DMA_REQUEST
  *         - TMR_HALL_DMA_REQUEST
  *         - TMR_Trigger_DMA_REQUEST
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_DMA_Request_Enable(tmr_Type *tmr_x, tmr_DMA_Request_Type DMA_request, confirm_state new_state) {
    if(new_state == TRUE) {
        tmr_x->iden |= DMA_request;
    } else if(new_state == FALSE) {
        tmr_x->iden &= ~DMA_request;
    }
}

/**
  * @brief  enable or disable tmr interrupt
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_interrupt
  *         this parameter can be one of the following values:
  *         - TMR_OVF_INT
  *         - TMR_C1_INT
  *         - TMR_C2_INT
  *         - TMR_C3_INT
  *         - TMR_C4_INT
  *         - TMR_HALL_INT
  *         - TMR_Trigger_INT
  *         - TMR_BRK_INT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_Interrupt_Enable(tmr_Type *tmr_x, uint32_t tmr_interrupt, confirm_state new_state) {
    if(new_state == TRUE) {
        tmr_x->iden |= tmr_interrupt;
    } else if(new_state == FALSE) {
        tmr_x->iden &= ~tmr_interrupt;
    }
}

/**
  * @brief  get tmr flag
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_flag
  *         this parameter can be one of the following values:
  *         - TMR_OVF_FLAG
  *         - TMR_C1_FLAG
  *         - TMR_C2_FLAG
  *         - TMR_C3_FLAG
  *         - TMR_C4_FLAG
  *         - TMR_C5_FLAG
  *         - TMR_HALL_FLAG
  *         - TMR_Trigger_FLAG
  *         - TMR_BRK_FLAG
  *         - TMR_C1_RECAPTURE_FLAG
  *         - TMR_C2_RECAPTURE_FLAG
  *         - TMR_C3_RECAPTURE_FLAG
  *         - TMR_C4_RECAPTURE_FLAG
  * @retval state of tmr flag
  */
flag_status tmr_Flag_Get(tmr_Type *tmr_x, uint32_t tmr_flag) {
    flag_status status = RESET;

    if((tmr_x->ists & tmr_flag) != RESET) {
        status = SET;
    } else {
        status = RESET;
    }

    return status;
}

/**
  * @brief  clear tmr flag
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_flag
  *         this parameter can be any combination of the following values:
  *         - TMR_OVF_FLAG
  *         - TMR_C1_FLAG
  *         - TMR_C2_FLAG
  *         - TMR_C3_FLAG
  *         - TMR_C4_FLAG
  *         - TMR_C5_FLAG
  *         - TMR_HALL_FLAG
  *         - TMR_Trigger_FLAG
  *         - TMR_BRK_FLAG
  *         - TMR_C1_RECAPTURE_FLAG
  *         - TMR_C2_RECAPTURE_FLAG
  *         - TMR_C3_RECAPTURE_FLAG
  *         - TMR_C4_RECAPTURE_FLAG
  * @retval none
  */
void tmr_Flag_Clear(tmr_Type *tmr_x, uint32_t tmr_flag) {
    tmr_x->ists = ~tmr_flag;
}

/**
  * @brief  generate tmr event
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR6, TMR7, TMR8,
  *         TMR9, TMR10, TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_event
  *         this parameter can be one of the following values:
  *         - TMR_OverFlow_SWTRIG
  *         - TMR_C1_SWTRIG
  *         - TMR_C2_SWTRIG
  *         - TMR_C3_SWTRIG
  *         - TMR_C4_SWTRIG
  *         - TMR_HALL_SWTRIG
  *         - TMR_Trigger_SWTRIG
  *         - TMR_BRK_SWTRIG
  * @retval none
  */
void tmr_Event_sw_trigger(tmr_Type *tmr_x, tmr_Event_Trigger_Type tmr_event) {
    tmr_x->swevt |= tmr_event;
}

/**
  * @brief  tmr output enable(oen),this function is important for advtm output enable
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8, TMR20
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void tmr_OutPut_Enable(tmr_Type *tmr_x, confirm_state new_state) {
    tmr_x->brk_bit.oen = new_state;
}

/**
  * @brief  set tmr select internal clock
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12, TMR20
  * @retval none
  */
void tmr_Internal_Clock_Set(tmr_Type *tmr_x) {
    tmr_x->stctrl_bit.smsel = TMR_Sub_Mode_DIABLE;
}

/**
  * @brief  set tmr output channel polarity
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_1C
  *         - TMR_Select_Channel_2C
  *         - TMR_Select_Channel_3C
  * @param  oc_polarity
  *         this parameter can be one of the following values:
  *         - TMR_POLARITY_ACTIVE_HIGH
  *         - TMR_POLARITY_ACTIVE_LOW
  * @retval none
  */
void tmr_OutPut_Channel_polarity_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                     tmr_polarity_active_Type oc_polarity) {
    uint16_t channel;

    channel = tmr_channel;

    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cctrl_bit.c1p = (uint32_t)oc_polarity;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cctrl_bit.c2p = (uint32_t)oc_polarity;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cctrl_bit.c3p = (uint32_t)oc_polarity;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cctrl_bit.c4p = (uint32_t)oc_polarity;
            break;

        case TMR_Select_Channel_1C:
            tmr_x->cctrl_bit.c1cp = (uint32_t)oc_polarity;
            break;

        case TMR_Select_Channel_2C:
            tmr_x->cctrl_bit.c2cp = (uint32_t)oc_polarity;
            break;

        case TMR_Select_Channel_3C:
            tmr_x->cctrl_bit.c3cp = (uint32_t)oc_polarity;
            break;

        default:
            break;
    }
}

/**
  * @brief  config tmr external clock
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR20
  * @param  es_divide
  *         this parameter can be one of the following values:
  *         - TMR_ES_Frequency_DIV_1
  *         - TMR_ES_Frequency_DIV_2
  *         - TMR_ES_Frequency_DIV_4
  *         - TMR_ES_Frequency_DIV_8
  * @param  es_polarity
  *         this parameter can be one of the following values:
  *         - TMR_ES_POLARITY_NON_INVERTED
  *         - TMR_ES_POLARITY_INVERTED
  * @param  es_filter (0x0~0xf)
  * @retval none
  */
void tmr_external_Clock_Config(tmr_Type *tmr_x, tmr_external_signal_Divider_Type es_divide, \
                               tmr_external_signal_polarity_Type  es_polarity, uint16_t es_filter) {
    tmr_x->stctrl_bit.esdiv = es_divide;
    tmr_x->stctrl_bit.esp = es_polarity;
    tmr_x->stctrl_bit.esf = es_filter;
}

/**
  * @brief  config tmr external clock mode1
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR12, TMR20
  * @param  es_divide
  *         this parameter can be one of the following values:
  *         - TMR_ES_Frequency_DIV_1
  *         - TMR_ES_Frequency_DIV_2
  *         - TMR_ES_Frequency_DIV_4
  *         - TMR_ES_Frequency_DIV_8
  * @param  es_polarity
  *         this parameter can be one of the following values:
  *         - TMR_ES_POLARITY_NON_INVERTED
  *         - TMR_ES_POLARITY_INVERTED
  * @param  es_filter (0x0~0xf)
  * @retval none
  */
void tmr_external_Clock_Mode1_Config(tmr_Type *tmr_x, tmr_external_signal_Divider_Type es_divide, \
                                     tmr_external_signal_polarity_Type  es_polarity, uint16_t es_filter) {
    tmr_external_Clock_Config(tmr_x, es_divide, es_polarity, es_filter);
    tmr_x->stctrl_bit.smsel = TMR_Sub_EXTERNAL_Clock_Mode_A;
    tmr_x->stctrl_bit.stis = TMR_Sub_Input_SEL_EXTIN;
}

/**
  * @brief  config tmr external clock mode2
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR20
  * @param  es_divide
  *         this parameter can be one of the following values:
  *         - TMR_ES_Frequency_DIV_1
  *         - TMR_ES_Frequency_DIV_2
  *         - TMR_ES_Frequency_DIV_4
  *         - TMR_ES_Frequency_DIV_8
  * @param  es_polarity
  *         this parameter can be one of the following values:
  *         - TMR_ES_POLARITY_NON_INVERTED
  *         - TMR_ES_POLARITY_INVERTED
  * @param  es_filter (0x0~0xf)
  * @retval none
  */
void tmr_external_Clock_Mode2_Config(tmr_Type *tmr_x, tmr_external_signal_Divider_Type es_divide, \
                                     tmr_external_signal_polarity_Type  es_polarity, uint16_t es_filter) {
    tmr_external_Clock_Config(tmr_x, es_divide, es_polarity, es_filter);
    tmr_x->stctrl_bit.ecmben = TRUE;
}

/**
  * @brief  config tmr encoder mode
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR20
  * @param  encoder_Mode
  *         this parameter can be one of the following values:
  *         - TMR_ENCODER_Mode_A
  *         - TMR_ENCODER_Mode_B
  *         - TMR_ENCODER_Mode_C
  * @param  ic1_polarity
  *         this parameter can be one of the following values:
  *         - TMR_Input_RISING_EDGE
  *         - TMR_Input_FALLING_EDGE
  *         - TMR_Input_BOTH_EDGE
  * @param  ic2_polarity
  *         this parameter can be one of the following values:
  *         - TMR_Input_RISING_EDGE
  *         - TMR_Input_FALLING_EDGE
  *         - TMR_Input_BOTH_EDGE
  * @retval none
  */
void tmr_encoder_Mode_Config(tmr_Type *tmr_x, tmr_encoder_Mode_Type encoder_Mode, tmr_Input_polarity_Type \
                             ic1_polarity, tmr_Input_polarity_Type ic2_polarity) {
    tmr_x->stctrl_bit.smsel = encoder_Mode;

    /* set ic1 polarity */
    tmr_x->cctrl_bit.c1p =  (ic1_polarity & 0x1);
    tmr_x->cctrl_bit.c1cp = (ic1_polarity >> 1);
    /* set ic1 as input channel */
    tmr_x->cm1_Input_bit.c1c = TMR_CC_Channel_MAPPED_DIRECT;

    /* set ic2 polarity */
    tmr_x->cctrl_bit.c2p =  (ic2_polarity & 0x1);
    tmr_x->cctrl_bit.c2cp = (ic2_polarity >> 1);
    /* set ic2 as input channel */
    tmr_x->cm1_Input_bit.c2c = TMR_CC_Channel_MAPPED_DIRECT;
}

/**
  * @brief  set tmr force output
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR9, TMR10,
  *         TMR11, TMR12, TMR13, TMR14, TMR20
  * @param  tmr_channel
  *         this parameter can be one of the following values:
  *         - TMR_Select_Channel_1
  *         - TMR_Select_Channel_2
  *         - TMR_Select_Channel_3
  *         - TMR_Select_Channel_4
  *         - TMR_Select_Channel_5
  * @param  force_output
  *         this parameter can be one of the following values:
  *         - TMR_FORCE_OutPut_HIGH
  *         - TMR_FORCE_OutPut_LOW
  * @retval none
  */
void tmr_force_OutPut_Set(tmr_Type *tmr_x,  tmr_Channel_Select_Type tmr_channel, \
                          tmr_force_OutPut_Type force_output) {
    uint16_t channel;

    channel = tmr_channel;

    switch(channel) {
        case TMR_Select_Channel_1:
            tmr_x->cm1_OutPut_bit.c1octrl = force_output;
            break;

        case TMR_Select_Channel_2:
            tmr_x->cm1_OutPut_bit.c2octrl = force_output;
            break;

        case TMR_Select_Channel_3:
            tmr_x->cm2_OutPut_bit.c3octrl = force_output;
            break;

        case TMR_Select_Channel_4:
            tmr_x->cm2_OutPut_bit.c4octrl = force_output;
            break;

        case TMR_Select_Channel_5:
            tmr_x->cm3_OutPut_bit.c5octrl = force_output;
            break;

        default:
            break;
    }
}

/**
  * @brief  config tmr dma control
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR2, TMR3, TMR4, TMR5, TMR8, TMR20
  * @param  DMA_length
  *         this parameter can be one of the following values:
  *         - TMR_DMA_Transfer_1BYTE
  *         - TMR_DMA_Transfer_2BYTES
  *         - TMR_DMA_Transfer_3BYTES
  *         ...
  *         - TMR_DMA_Transfer_17BYTES
  *         - TMR_DMA_Transfer_18BYTES
  * @param  DMA_base_Address
  *         this parameter can be one of the following values:
  *         - TMR_CTRL1_Address
  *         - TMR_CTRL2_Address
  *         - TMR_STCTRL_Address
  *         - TMR_IDEN_Address
  *         - TMR_ISTS_Address
  *         - TMR_SWEVT_Address
  *         - TMR_CM1_Address
  *         - TMR_CM2_Address
  *         - TMR_CCTRL_Address
  *         - TMR_CVAL_Address
  *         - TMR_DIV_Address
  *         - TMR_PR_Address
  *         - TMR_RPR_Address
  *         - TMR_C1DT_Address
  *         - TMR_C2DT_Address
  *         - TMR_C3DT_Address
  *         - TMR_C4DT_Address
  *         - TMR_BRK_Address
  *         - TMR_DMACTRL_Address
  * @retval none
  */
void tmr_DMA_Control_Config(tmr_Type *tmr_x, tmr_DMA_Transfer_length_Type DMA_length, \
                            tmr_DMA_Address_Type DMA_base_Address) {
    tmr_x->dmactrl_bit.dtb = DMA_length;
    tmr_x->dmactrl_bit.addr = DMA_base_Address;
}

/**
  * @brief  config tmr break mode and dead-time
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR1, TMR8, TMR20
  * @param  brkdt_struct
  *         - to the structure of tmr_brkdt_Config_Type
  * @retval none
  */
void tmr_brkdt_Config(tmr_Type *tmr_x, tmr_brkdt_Config_Type *brkdt_struct) {
    tmr_x->brk_bit.brken = brkdt_struct->brk_Enable;
    tmr_x->brk_bit.dtc = brkdt_struct->deadtime;
    tmr_x->brk_bit.fcsodis = brkdt_struct->fcsodis_state;
    tmr_x->brk_bit.fcsoen = brkdt_struct->fcsoen_state;
    tmr_x->brk_bit.brkv = brkdt_struct->brk_polarity;
    tmr_x->brk_bit.aoen = brkdt_struct->auto_OutPut_Enable;
    tmr_x->brk_bit.wpc = brkdt_struct->wp_level;
}

/**
  * @brief  set tmr2/tmr5 input channel remap
  * @param  tmr_x: select the tmr peripheral.
  *         this parameter can be one of the following values:
  *         TMR2, TMR5
  * @param  input_remap
  *         - TMR2_TMR8TRGOUT_TMR5_GPIO
  *         - TMR2_PTP_TMR5_LICK
  *         - TMR2_OTG1FS_TMR5_LEXT
  *         - TMR2_OTG2FS_TMR5_ERTC
  * @retval none
  */
void tmr_iremap_Config(tmr_Type *tmr_x, tmr_Input_remap_Type input_remap) {
    if(tmr_x == TMR2) {
        tmr_x->rmp_bit.tmr2_ch1_irmp = input_remap;
    } else if(tmr_x == TMR5) {
        tmr_x->rmp_bit.tmr5_ch4_irmp = input_remap;
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
