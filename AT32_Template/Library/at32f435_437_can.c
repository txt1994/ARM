/**
  **************************************************************************
  * @file     at32f435_437_can.c
  * @brief    contains all the functions for the can firmware library
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

/** @defgroup CAN
  * @brief CAN driver modules
  * @{
  */

#ifdef CAN_MODULE_EnableD

/** @defgroup CAN_private_functions
  * @{
  */

/**
  * @brief  deinitialize the can peripheral registers to their default reset values.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @retval none.
  */
void can_Reset(can_Type* can_x) {
    if(can_x == CAN1) {
        crm_periph_Reset(CRM_CAN1_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_CAN1_PERIPH_Reset, FALSE);
    } else if(can_x == CAN2) {
        crm_periph_Reset(CRM_CAN2_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_CAN2_PERIPH_Reset, FALSE);
    }
}

/**
  * @brief  fill each can_baudrate_struct member with its default value.
  * @param  can_baudrate_struct: pointer to a can_baudrate_Type structure which will be initialized.
  * @retval none.
  */
void can_baudrate_Default_Para_Init(can_baudrate_Type* can_baudrate_struct) {
    /* reset can baudrate structure parameters values */

    /* baud rate division */
    can_baudrate_struct->baudrate_div = 1;

    /* resynchronization adjust width */
    can_baudrate_struct->rsaw_size = CAN_RSAW_2TQ;

    /* bit time segment 1 */
    can_baudrate_struct->bts1_size = CAN_BTS1_4TQ;

    /* bit time segment 2 */
    can_baudrate_struct->bts2_size = CAN_BTS2_3TQ;
}

/**
  * @brief  set the baudrate of the can peripheral
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  can_baudrate_struct: pointer to a can_baudrate_Type structure which will be set.
  * @note   baudrate calculate method is:
  *         baudrate = fpclk/(baudrate_div *(3 + bts1_size + bts2_size))
  * @retval the result of baudrate set
  *         this parameter can be one of the following values:
  *         SUCCESS or ERROR
  */
error_status can_baudrate_Set(can_Type* can_x, can_baudrate_Type* can_baudrate_struct) {
    error_status status_index = ERROR;
    uint32_t wait_ACK_index = 0x00000000;
    /* exit from doze mode */
    can_x->mctrl_bit.dzen = FALSE;

    /* request freeze mode */
    can_x->mctrl_bit.fzen = TRUE;

    /* wait the acknowledge */
    while((!can_x->msts_bit.fzc) && (wait_ACK_index != FZC_TIMEOUT)) {
        wait_ACK_index++;
    }

    /* check acknowledge */
    if(can_x->msts_bit.fzc) {
        can_x->btmg_bit.brdiv = can_baudrate_struct->baudrate_div - 1;
        can_x->btmg_bit.rsaw = can_baudrate_struct->rsaw_size;
        can_x->btmg_bit.bts1 = can_baudrate_struct->bts1_size;
        can_x->btmg_bit.bts2 = can_baudrate_struct->bts2_size;

        /* request leave freeze mode */
        can_x->mctrl_bit.fzen = FALSE;

        /* wait the acknowledge */
        wait_ACK_index = 0;

        while((can_x->msts_bit.fzc) && (wait_ACK_index != FZC_TIMEOUT)) {
            wait_ACK_index++;
        }

        /* check acknowledged */
        if(can_x->msts_bit.fzc) {
            status_index = ERROR;
        } else {
            status_index = SUCCESS ;
        }
    } else {
        status_index = ERROR;
    }

    /* return the status of baudrate set */
    return status_index;
}

/**
  * @brief  fill each can_Init_struct member with its default value.
  * @param  can_base_struct: pointer to a can_base_Type structure which will be initialized.
  * @retval none.
  */
void can_Default_Para_Init(can_base_Type* can_base_struct) {
    /* reset can init structure parameters values */

    /* initialize the time triggered communication mode */
    can_base_struct->ttc_Enable = FALSE;

    /* initialize the automatic exit bus-off management */
    can_base_struct->aebo_Enable = FALSE;

    /* initialize the automatic exit doze mode */
    can_base_struct->aed_Enable = FALSE;

    /* initialize the prohibit retransmission when sending fails */
    can_base_struct->prsf_Enable = FALSE;

    /* initialize the message discarding rule select when overflow */
    can_base_struct->mdrsel_Selection = CAN_DISCARDING_FIRST_RECEIVED;

    /* initialize the multiple message sending sequence rule */
    can_base_struct->mmssr_Selection = CAN_SENDING_BY_ID;

    /* initialize the can_Mode */
    can_base_struct->mode_Selection = CAN_Mode_COMMUNICATE;
}

/**
  * @brief  initialize the can peripheral according to the specified
  *         parameters in the can_Init_struct.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  can_base_struct: pointer to a can_base_struct structure that contains the configuration information for the can peripheral.
  * @retval the status of initialization
  *         this parameter can be one of the following values:
  *         SUCCESS or ERROR
  */
error_status can_base_Init(can_Type* can_x, can_base_Type* can_base_struct) {
    error_status init_Status_index = ERROR;
    uint32_t wait_ACK_index = 0x00000000;
    /* exit from doze mode */
    can_x->mctrl_bit.dzen = FALSE;

    /* request freeze mode */
    can_x->mctrl_bit.fzen = TRUE;

    /* wait the acknowledge */
    while((!can_x->msts_bit.fzc) && (wait_ACK_index != FZC_TIMEOUT)) {
        wait_ACK_index++;
    }

    /* check acknowledge */
    if(can_x->msts_bit.fzc) {
        /* set the time triggered communication mode */
        can_x->mctrl_bit.ttcen = can_base_struct->ttc_Enable;

        /* set the automatic exit bus-off management */
        can_x->mctrl_bit.aeboen = can_base_struct->aebo_Enable;

        /* set the automatic automatic exit doze mode */
        can_x->mctrl_bit.aeden = can_base_struct->aed_Enable;

        /* set the prohibit retransmission when sending fails */
        can_x->mctrl_bit.prsfen = can_base_struct->prsf_Enable;

        /* set the message discarding rule select when overflow */
        can_x->mctrl_bit.mdrsel = can_base_struct->mdrsel_Selection;

        /* set the multiple message sending sequence rule */
        can_x->mctrl_bit.mmssr = can_base_struct->mmssr_Selection;

        /* set the test mode */
        can_x->btmg_bit.lben = can_base_struct->mode_Selection & 0x01;
        can_x->btmg_bit.loen = (can_base_struct->mode_Selection >> 1) & 0x01;

        /* request leave freeze mode */
        can_x->mctrl_bit.fzen = FALSE;

        /* wait the acknowledge */
        wait_ACK_index = 0;

        while((can_x->msts_bit.fzc) && (wait_ACK_index != FZC_TIMEOUT)) {
            wait_ACK_index++;
        }

        /* check acknowledged */
        if(can_x->msts_bit.fzc) {
            init_Status_index = ERROR;
        } else {
            init_Status_index = SUCCESS ;
        }
    } else {
        init_Status_index = ERROR;
    }

    /* return the status of initialization */
    return init_Status_index;
}

/**
  * @brief  fill each can_Filter_Init_struct member with its default value.
  * @param  can_Filter_Init_struct: pointer to a can_Filter_Init_Type structure which will be initialized.
  * @retval none.
  */
void can_Filter_Default_Para_Init(can_Filter_Init_Type* can_Filter_Init_struct) {
    /* reset can filter init structure parameters values */

    /* initialize the filter activate state */
    can_Filter_Init_struct->filter_activate_Enable = FALSE;

    /* filter mode */
    can_Filter_Init_struct->filter_Mode = CAN_Filter_Mode_ID_MASK;

    /* filter relation fifo select */
    can_Filter_Init_struct->filter_FIFO = CAN_Filter_FIFO0;

    /* filter number select */
    can_Filter_Init_struct->filter_Number = 0;

    /* initialize the filter bit width */
    can_Filter_Init_struct->filter_bit = CAN_Filter_16BIT;

    /* initialize the filters filter data bit */
    can_Filter_Init_struct->filter_ID_high = 0;
    can_Filter_Init_struct->filter_ID_low = 0;
    can_Filter_Init_struct->filter_Mask_high = 0;
    can_Filter_Init_struct->filter_Mask_low = 0;
}

/**
  * @brief  initialize the can peripheral according to the specified
  *         parameters in the can_Filter_Init_struct.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  can_Filter_Init_struct: pointer to a can_Filter_Init_Type structure that contains the configuration information.
  * @retval none.
  */
void can_Filter_Init(can_Type* can_x, can_Filter_Init_Type* can_Filter_Init_struct) {
    uint32_t filter_Number_Bit_pos = 0;
    filter_Number_Bit_pos = ((uint32_t)1) << can_Filter_Init_struct->filter_Number;
    /* set the filter turn into configuration condition */
    can_x->fctrl_bit.fcs = TRUE;

    /* filter activate disable */
    can_x->facfg &= ~(uint32_t)filter_Number_Bit_pos;

    /* filter bit width */
    switch(can_Filter_Init_struct->filter_bit) {
        case CAN_Filter_16BIT:
            can_x->fbwcfg &= ~(uint32_t)filter_Number_Bit_pos;
            /* first 16-bit identifier and first 16-bit mask or first 16-bit identifier and second 16-bit identifier */
            can_x->ffb[can_Filter_Init_struct->filter_Number].ffdb1 = ((0x0000FFFF & (uint32_t)can_Filter_Init_struct->filter_Mask_low) << 16);
            can_x->ffb[can_Filter_Init_struct->filter_Number].ffdb1 |= (0x0000FFFF & (uint32_t)can_Filter_Init_struct->filter_ID_low);

            /* second 16-bit identifier and second 16-bit mask or third 16-bit identifier and fourth 16-bit identifier */
            can_x->ffb[can_Filter_Init_struct->filter_Number].ffdb2 = ((0x0000FFFF & (uint32_t)can_Filter_Init_struct->filter_Mask_high) << 16);
            can_x->ffb[can_Filter_Init_struct->filter_Number].ffdb2 |= (0x0000FFFF & (uint32_t)can_Filter_Init_struct->filter_ID_high);

            break;

        case CAN_Filter_32BIT:
            can_x->fbwcfg |= filter_Number_Bit_pos;
            /* 32-bit identifier or first 32-bit identifier */
            can_x->ffb[can_Filter_Init_struct->filter_Number].ffdb1 = ((0x0000FFFF & (uint32_t)can_Filter_Init_struct->filter_ID_high) << 16);
            can_x->ffb[can_Filter_Init_struct->filter_Number].ffdb1 |= (0x0000FFFF & (uint32_t)can_Filter_Init_struct->filter_ID_low);

            /* 32-bit mask or second 32-bit identifier */
            can_x->ffb[can_Filter_Init_struct->filter_Number].ffdb2 = ((0x0000FFFF & (uint32_t)can_Filter_Init_struct->filter_Mask_high) << 16);
            can_x->ffb[can_Filter_Init_struct->filter_Number].ffdb2 |= (0x0000FFFF & (uint32_t)can_Filter_Init_struct->filter_Mask_low);

            break;

        default:
            break;
    }

    /* filter mode */
    switch(can_Filter_Init_struct->filter_Mode) {
        case CAN_Filter_Mode_ID_MASK:
            can_x->fmcfg &= ~(uint32_t)filter_Number_Bit_pos;
            break;

        case CAN_Filter_Mode_ID_LIST:
            can_x->fmcfg |= (uint32_t)filter_Number_Bit_pos;
            break;

        default:
            break;
    }

    /* filter relation fifo select */
    switch(can_Filter_Init_struct->filter_FIFO) {
        case CAN_Filter_FIFO0:
            can_x->frf &= ~(uint32_t)filter_Number_Bit_pos;
            break;

        case CAN_Filter_FIFO1:
            can_x->frf |= (uint32_t)filter_Number_Bit_pos;
            break;

        default:
            break;
    }

    /* filter activate enable */
    switch(can_Filter_Init_struct->filter_activate_Enable) {
        case TRUE:
            can_x->facfg |= (uint32_t)filter_Number_Bit_pos;
            break;

        case FALSE:
            can_x->facfg &= ~(uint32_t)filter_Number_Bit_pos;
            break;

        default:
            break;
    }

    /* set the filter turn into working condition */
    can_x->fctrl_bit.fcs = FALSE;
}

/**
  * @brief  enable or disable the debug transmission prohibit of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  new_state: new state of debug transmission prohibit.
  *         this parameter can be: TRUE or FALSE.
  * @retval none.
  */
void can_DEBUG_transmission_prohibit(can_Type* can_x, confirm_state new_state) {
    can_x->mctrl_bit.ptd = new_state;
}

/**
  * @brief  enable or disable time trigger operation communication mode of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1 or CAN2.
  * @param  new_state : new state of time trigger operation communication mode.
  *         this parameter can be: TRUE or FALSE.
  * @note
  *         note1:
  *         when enabled, transmit mailbox time stamp(tmts[15:0]) value is sent in the last two data bytes of
  *         the 8-byte message: tmts[7:0] in data byte 6 and tmts[15:8] in data byte 7
  * @note
  *         note2:
  *         tmdtbl must be programmed as 8 in order time stamp (2 bytes) to be sent over the can bus.
  * @retval none
  */
void can_ttc_Mode_Enable(can_Type* can_x, confirm_state new_state) {
    /* config the ttc mode new_state */
    can_x->mctrl_bit.ttcen = new_state;

    /* config tmtsten bits new_state */
    can_x->tx_mailbox[0].tmc_bit.tmtsten = new_state;
    can_x->tx_mailbox[1].tmc_bit.tmtsten = new_state;
    can_x->tx_mailbox[2].tmc_bit.tmtsten = new_state;
}

/**
  * @brief  fill the transmission message and transmit of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  tx_Message_struct: pointer to a structure which contains the message to be trans.
  * @retval the number of the mailbox that is used for transmission:
  *         this parameter can be one of the following values:
  *         - CAN_TX_MAILBOX0
  *         - CAN_TX_MAILBOX1
  *         - CAN_TX_MAILBOX2
  *         - CAN_TX_Status_NO_EMPTY <meanings there is no empty mailbox, message cannot be filled>
  */
uint8_t can_Message_transmit(can_Type* can_x, can_TX_Message_Type* tx_Message_struct) {
    uint8_t transmit_mailbox = CAN_TX_Status_NO_EMPTY;

    /* select one empty transmit mailbox */
    if(can_x->tsts_bit.tm0ef) {
        transmit_mailbox = CAN_TX_MAILBOX0;
    } else if(can_x->tsts_bit.tm1ef) {
        transmit_mailbox = CAN_TX_MAILBOX1;
    } else if(can_x->tsts_bit.tm2ef) {
        transmit_mailbox = CAN_TX_MAILBOX2;
    } else {
        transmit_mailbox = CAN_TX_Status_NO_EMPTY;
    }

    if(transmit_mailbox != CAN_TX_Status_NO_EMPTY) {
        /* set up the id */
        can_x->tx_mailbox[transmit_mailbox].tmi &= 0x00000001;
        can_x->tx_mailbox[transmit_mailbox].tmi_bit.tmidsel = tx_Message_struct->id_Type;

        switch(tx_Message_struct->id_Type) {
            case CAN_ID_STANDARD:
                can_x->tx_mailbox[transmit_mailbox].tmi_bit.tmsid = tx_Message_struct->standard_id;
                break;

            case CAN_ID_EXTENDED:
                can_x->tx_mailbox[transmit_mailbox].tmi |= (tx_Message_struct->extended_id << 3);
                break;

            default:
                break;
        }

        can_x->tx_mailbox[transmit_mailbox].tmi_bit.tmfrsel = tx_Message_struct->frame_Type;
        /* set up the dlc */
        can_x->tx_mailbox[transmit_mailbox].tmc_bit.tmdtbl = (tx_Message_struct->dlc & ((uint8_t)0x0F));

        /* set up the data field */
        can_x->tx_mailbox[transmit_mailbox].tmdtl = (((uint32_t)tx_Message_struct->data[3] << 24) |
                ((uint32_t)tx_Message_struct->data[2] << 16) |
                ((uint32_t)tx_Message_struct->data[1] << 8) |
                ((uint32_t)tx_Message_struct->data[0]));
        can_x->tx_mailbox[transmit_mailbox].tmdth = (((uint32_t)tx_Message_struct->data[7] << 24) |
                ((uint32_t)tx_Message_struct->data[6] << 16) |
                ((uint32_t)tx_Message_struct->data[5] << 8) |
                ((uint32_t)tx_Message_struct->data[4]));

        /* request transmission */
        can_x->tx_mailbox[transmit_mailbox].tmi_bit.tmsr = TRUE;
    }

    return transmit_mailbox;
}

/**
  * @brief  check the transmission state of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1 or CAN2.
  * @param  transmit_mailbox: the number of the mailbox that is used for transmission.
  *         this parameter can be one of the following values:
  *         - CAN_TX_MAILBOX0
  *         - CAN_TX_MAILBOX1
  *         - CAN_TX_MAILBOX2
  * @retval can transmit status
  *         this parameter can be one of the following values:
  *         - CAN_TX_Status_SUCCESSFUL
  *         - CAN_TX_Status_FAILED
  *         - CAN_TX_Status_PENDING
  */
can_Transmit_Status_Type can_Transmit_Status_Get(can_Type* can_x, can_TX_mailbox_Num_Type transmit_mailbox) {
    can_Transmit_Status_Type state_index = CAN_TX_Status_FAILED;

    switch(transmit_mailbox) {
        case CAN_TX_MAILBOX0:
            if(can_x->tsts_bit.tm0tcf != RESET) {
                if(can_x->tsts_bit.tm0tsf != RESET) {
                    state_index = CAN_TX_Status_SUCCESSFUL;
                } else {
                    state_index = CAN_TX_Status_FAILED;
                }
            } else {
                state_index = CAN_TX_Status_PENDING;
            }

            break;

        case CAN_TX_MAILBOX1:
            if(can_x->tsts_bit.tm1tcf != RESET) {
                if(can_x->tsts_bit.tm1tsf != RESET) {
                    state_index = CAN_TX_Status_SUCCESSFUL;
                } else {
                    state_index = CAN_TX_Status_FAILED;
                }
            } else {
                state_index = CAN_TX_Status_PENDING;
            }

            break;

        case CAN_TX_MAILBOX2:
            if(can_x->tsts_bit.tm2tcf != RESET) {
                if(can_x->tsts_bit.tm2tsf != RESET) {
                    state_index = CAN_TX_Status_SUCCESSFUL;
                } else {
                    state_index = CAN_TX_Status_FAILED;
                }
            } else {
                state_index = CAN_TX_Status_PENDING;
            }

            break;

        default:
            state_index = CAN_TX_Status_FAILED;
            break;
    }

    return state_index;
}

/**
  * @brief  cancel a transmit request of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1 or CAN2.
  * @param  mailbox:  mailbox number.
  *         this parameter can be one of the following values:
  *         - CAN_TX_MAILBOX0
  *         - CAN_TX_MAILBOX1
  *         - CAN_TX_MAILBOX2
  * @retval none.
  */
void can_Transmit_cancel(can_Type* can_x, can_TX_mailbox_Num_Type transmit_mailbox) {
    switch (transmit_mailbox) {
        case CAN_TX_MAILBOX0:
            can_x->tsts = CAN_TSTS_TM0CT_VAL;
            break;

        case CAN_TX_MAILBOX1:
            can_x->tsts = CAN_TSTS_TM1CT_VAL;
            break;

        case CAN_TX_MAILBOX2:
            can_x->tsts = CAN_TSTS_TM2CT_VAL;
            break;

        default:
            break;
    }
}

/**
  * @brief  receive message of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  fifo_Number: receive fifo number.
  *         this parameter can be one of the following values:
  *         - CAN_RX_FIFO0
  *         - CAN_RX_FIFO1
  * @param  rx_Message_struct: pointer to a structure which store the receive message.
  * @retval none.
  */
void can_Message_receive(can_Type* can_x, can_RX_FIFO_Num_Type fifo_Number, can_RX_Message_Type* rx_Message_struct) {
    /* get the id type */
    rx_Message_struct->id_Type = (can_identifier_Type)can_x->fifo_mailbox[fifo_Number].rfi_bit.rfidi;

    switch (rx_Message_struct->id_Type) {
        case CAN_ID_STANDARD:
            rx_Message_struct->standard_id = can_x->fifo_mailbox[fifo_Number].rfi_bit.rfsid;
            break;

        case CAN_ID_EXTENDED:
            rx_Message_struct->extended_id = 0x1FFFFFFF & (can_x->fifo_mailbox[fifo_Number].rfi >> 3);
            break;

        default:
            break;
    }

    rx_Message_struct->frame_Type = (can_trans_frame_Type)can_x->fifo_mailbox[fifo_Number].rfi_bit.rffri;
    /* get the dlc */
    rx_Message_struct->dlc = can_x->fifo_mailbox[fifo_Number].rfc_bit.rfdtl;

    /* get the filter match number */
    rx_Message_struct->filter_index = can_x->fifo_mailbox[fifo_Number].rfc_bit.rffmn;

    /* get the data field */
    rx_Message_struct->data[0] = can_x->fifo_mailbox[fifo_Number].rfdtl_bit.rfdt0;
    rx_Message_struct->data[1] = can_x->fifo_mailbox[fifo_Number].rfdtl_bit.rfdt1;
    rx_Message_struct->data[2] = can_x->fifo_mailbox[fifo_Number].rfdtl_bit.rfdt2;
    rx_Message_struct->data[3] = can_x->fifo_mailbox[fifo_Number].rfdtl_bit.rfdt3;
    rx_Message_struct->data[4] = can_x->fifo_mailbox[fifo_Number].rfdth_bit.rfdt4;
    rx_Message_struct->data[5] = can_x->fifo_mailbox[fifo_Number].rfdth_bit.rfdt5;
    rx_Message_struct->data[6] = can_x->fifo_mailbox[fifo_Number].rfdth_bit.rfdt6;
    rx_Message_struct->data[7] = can_x->fifo_mailbox[fifo_Number].rfdth_bit.rfdt7;

    /* release the fifo */
    can_receive_FIFO_release(can_x, fifo_Number);
}

/**
  * @brief  release the specified fifo of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  fifo_Number: fifo to be release.
  *         this parameter can be one of the following values:
  *         - CAN_RX_FIFO0
  *         - CAN_RX_FIFO1
  * @retval none.
  */
void can_receive_FIFO_release(can_Type* can_x, can_RX_FIFO_Num_Type fifo_Number) {
    switch (fifo_Number) {
        case CAN_RX_FIFO0:
            can_x->rf0 = CAN_RF0_RF0R_VAL;
            break;

        case CAN_RX_FIFO1:
            can_x->rf1 = CAN_RF1_RF1R_VAL;
            break;

        default:
            break;
    }
}

/**
  * @brief  return the number of pending messages of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  fifo_Number: receive fifo number.
  *         this parameter can be one of the following values:
  *         - CAN_RX_FIFO0
  *         - CAN_RX_FIFO1
  * @retval the number of message pending in the receive fifo.
  */
uint8_t can_receive_Message_pending_Get(can_Type* can_x, can_RX_FIFO_Num_Type fifo_Number) {
    uint8_t message_pending = 0;

    switch (fifo_Number) {
        case CAN_RX_FIFO0:
            message_pending = can_x->rf0_bit.rf0mn;
            break;

        case CAN_RX_FIFO1:
            message_pending = can_x->rf1_bit.rf1mn;
            break;

        default:
            break;
    }

    return message_pending;
}

/**
  * @brief  set the operation mode of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  can_operating_Mode: can operating mode.
  *         this parameter can be one of the following values:
  *         - CAN_OPERATINGMODE_Freeze
  *         - CAN_OPERATINGMODE_DOZE
  *         - CAN_OPERATINGMODE_COMMUNICATE
  * @retval status of operation mode set
  *         this parameter can be one of the following values:
  *         SUCCESS or ERROR
  */
error_status can_operating_Mode_Set(can_Type* can_x, can_operating_Mode_Type can_operating_Mode) {
    error_status status = ERROR;
    uint32_t time_out_index = FZC_TIMEOUT;

    if (can_operating_Mode == CAN_OPERATINGMODE_Freeze) {
        /* request enter freeze mode */
        can_x->mctrl_bit.dzen = FALSE;
        can_x->mctrl_bit.fzen = TRUE;

        while(((can_x->msts_bit.dzc) || (!can_x->msts_bit.fzc)) && (time_out_index != 0)) {
            time_out_index--;
        }

        if((can_x->msts_bit.dzc) || (!can_x->msts_bit.fzc)) {
            status = ERROR;
        } else {
            status = SUCCESS;
        }
    } else if(can_operating_Mode == CAN_OPERATINGMODE_DOZE) {
        /* request enter doze mode */
        can_x->mctrl_bit.dzen = TRUE;
        can_x->mctrl_bit.fzen = FALSE;

        while(((!can_x->msts_bit.dzc) || (can_x->msts_bit.fzc)) && (time_out_index != 0)) {
            time_out_index--;
        }

        if((!can_x->msts_bit.dzc) || (can_x->msts_bit.fzc)) {
            status = ERROR;
        } else {
            status = SUCCESS;
        }
    } else if(can_operating_Mode == CAN_OPERATINGMODE_COMMUNICATE) {
        /* request enter normal mode */
        can_x->mctrl_bit.dzen = FALSE;
        can_x->mctrl_bit.fzen = FALSE;

        while(((can_x->msts_bit.dzc) || (can_x->msts_bit.fzc)) && (time_out_index != 0)) {
            time_out_index--;
        }

        if((can_x->msts_bit.dzc) || (can_x->msts_bit.fzc)) {
            status = ERROR;
        } else {
            status = SUCCESS;
        }
    } else {
        status = ERROR;
    }

    return status;
}

/**
  * @brief  enter the low power mode of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @retval status of doze mode enter, the returned value can be:
  *         - CAN_Enter_DOZE_SUCCESSFUL  <it meaning enter the doze mode succeed>
  *         - CAN_Enter_DOZE_FAILED  <it meaning enter the doze mode failed>
  */
can_Enter_doze_Status_Type can_doze_Mode_Enter(can_Type* can_x) {
    can_Enter_doze_Status_Type status = CAN_Enter_DOZE_FAILED;
    uint32_t time_out_index = FZC_TIMEOUT;
    can_x->mctrl_bit.fzen = FALSE;
    can_x->mctrl_bit.dzen = TRUE;

    while(((!can_x->msts_bit.dzc) || (can_x->msts_bit.fzc)) && (time_out_index != 0)) {
        time_out_index--;
    }

    if((!can_x->msts_bit.dzc) || (can_x->msts_bit.fzc)) {
        status =  CAN_Enter_DOZE_FAILED;
    } else {
        status =  CAN_Enter_DOZE_SUCCESSFUL;
    }

    return status;
}

/**
  * @brief  exit the doze mode of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @retval status of doze mode enter, the returned value can be:
  *         - CAN_QUIT_DOZE_SUCCESSFUL  <it meaning exit the doze mode succeed>
  *         - CAN_QUIT_DOZE_FAILED  <it meaning exit the doze mode failed>
  */
can_quit_doze_Status_Type can_doze_Mode_Exit(can_Type* can_x) {
    can_quit_doze_Status_Type status = CAN_QUIT_DOZE_FAILED;
    uint32_t time_out_index = DZC_TIMEOUT;
    can_x->mctrl_bit.dzen = FALSE;

    while((can_x->msts_bit.dzc) && (time_out_index != 0)) {
        time_out_index--;
    }

    if(!can_x->msts_bit.dzc) {
        status = CAN_QUIT_DOZE_SUCCESSFUL;
    }

    return status;
}

/**
  * @brief  return the error type record (etr) of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @retval the value of the error code
  *         the return can be one of the follow values:
  *         - CAN_ERRORRECORD_NOERR
  *         - CAN_ERRORRECORD_STUFFERR,
  *         - CAN_ERRORRECORD_FORMERR,
  *         - CAN_ERRORRECORD_ACKERR,
  *         - CAN_ERRORRECORD_BITRECESSIVEERR,
  *         - CAN_ERRORRECORD_BITDOMINANTERR,
  *         - CAN_ERRORRECORD_CRCERR,
  *         - CAN_ERRORRECORD_SOFTWARESETERR
  */
can_Error_record_Type can_Error_Type_record_Get(can_Type* can_x) {
    can_Error_record_Type error_code = CAN_ERRORRECORD_NOERR;

    error_code = (can_Error_record_Type)can_x->ests_bit.etr;
    return error_code;
}

/**
  * @brief  return the receive error counter (rec) of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @retval the value of receive error counter.
  */
uint8_t can_receive_Error_Counter_Get(can_Type* can_x) {
    uint8_t counter = 0;
    counter = can_x->ests_bit.rec;
    return counter;
}

/**
  * @brief  return the transmit error counter of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @retval the value of transmit error counter.
  */
uint8_t can_Transmit_Error_Counter_Get(can_Type* can_x) {
    uint8_t counter = 0;
    counter = can_x->ests_bit.tec;
    return counter;
}

/**
  * @brief  enable or disable the interrupt of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  can_int: specifies the can interrupt sources to be enabled or disabled.
  *         this parameter can be one of the following values:
  *         - CAN_TCIEN_INT
  *         - CAN_RF0MIEN_INT
  *         - CAN_RF0FIEN_INT
  *         - CAN_RF0OIEN_INT
  *         - CAN_RF1MIEN_INT
  *         - CAN_RF1FIEN_INT
  *         - CAN_RF1OIEN_INT
  *         - CAN_EAIEN_INT
  *         - CAN_EPIEN_INT
  *         - CAN_BOIEN_INT
  *         - CAN_ETRIEN_INT
  *         - CAN_EOIEN_INT
  *         - CAN_QDZIEN_INT
  *         - CAN_EDZIEN_INT
  * @param  new_state: new state of the can interrupts.
  *         this parameter can be: TRUE or FALSE.
  * @retval none.
  */
void can_Interrupt_Enable(can_Type* can_x, uint32_t can_int, confirm_state new_state) {
    if (new_state != FALSE) {
        can_x->inten |= can_int;
    } else {
        can_x->inten &= ~can_int;
    }
}

/**
  * @brief  get flag of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  can_flag: select the flag.
  *         this parameter can be one of the following flags:
  *         - CAN_EAF_FLAG
  *         - CAN_EPF_FLAG
  *         - CAN_BOF_FLAG
  *         - CAN_ETR_FLAG
  *         - CAN_EOIF_FLAG
  *         - CAN_TM0TCF_FLAG
  *         - CAN_TM1TCF_FLAG
  *         - CAN_TM2TCF_FLAG
  *         - CAN_RF0MN_FLAG
  *         - CAN_RF0FF_FLAG
  *         - CAN_RF0OF_FLAG
  *         - CAN_RF1MN_FLAG
  *         - CAN_RF1FF_FLAG
  *         - CAN_RF1OF_FLAG
  *         - CAN_QDZIF_FLAG
  *         - CAN_EDZC_FLAG
  *         - CAN_TMEF_FLAG
  *         note:the state of CAN_EDZC_FLAG need to check dzc and edzif bit
  *         note:the state of CAN_TMEF_FLAG need to check rqc0,rqc1 and rqc2 bit
  * @retval status of can_flag, the returned value can be:SET or RESET.
  */
flag_status can_Flag_Get(can_Type* can_x, uint32_t can_flag) {
    flag_status bit_status = RESET;

    switch(can_flag) {
        case CAN_EAF_FLAG:
            bit_status = (flag_status)can_x->ests_bit.eaf;
            break;

        case CAN_EPF_FLAG:
            bit_status = (flag_status)can_x->ests_bit.epf;
            break;

        case CAN_BOF_FLAG:
            bit_status = (flag_status)can_x->ests_bit.bof;
            break;

        case CAN_ETR_FLAG:
            if(can_x->ests_bit.etr != 0) {
                bit_status = SET;
            } else {
                bit_status = RESET;
            }

            break;

        case CAN_EOIF_FLAG:
            bit_status = (flag_status)can_x->msts_bit.eoif;
            break;

        case CAN_TM0TCF_FLAG:
            bit_status = (flag_status)can_x->tsts_bit.tm0tcf;
            break;

        case CAN_TM1TCF_FLAG:
            bit_status = (flag_status)can_x->tsts_bit.tm1tcf;
            break;

        case CAN_TM2TCF_FLAG:
            bit_status = (flag_status)can_x->tsts_bit.tm2tcf;
            break;

        case CAN_RF0MN_FLAG:
            if(can_x->rf0_bit.rf0mn != 0) {
                bit_status = SET;
            } else {
                bit_status = RESET;
            }

            break;

        case CAN_RF0FF_FLAG:
            bit_status = (flag_status)can_x->rf0_bit.rf0ff;
            break;

        case CAN_RF0OF_FLAG:
            bit_status = (flag_status)can_x->rf0_bit.rf0of;
            break;

        case CAN_RF1MN_FLAG:
            if(can_x->rf1_bit.rf1mn != 0) {
                bit_status = SET;
            } else {
                bit_status = RESET;
            }

            break;

        case CAN_RF1FF_FLAG:
            bit_status = (flag_status)can_x->rf1_bit.rf1ff;
            break;

        case CAN_RF1OF_FLAG:
            bit_status = (flag_status)can_x->rf1_bit.rf1of;
            break;

        case CAN_QDZIF_FLAG:
            bit_status = (flag_status)can_x->msts_bit.qdzif;
            break;

        case CAN_EDZC_FLAG:
            if((can_x->msts_bit.dzc != RESET) || (can_x->msts_bit.edzif != RESET)) {
                bit_status = SET;
            } else {
                bit_status = RESET;
            }

            break;

        case CAN_TMEF_FLAG:
            if((can_x->tsts_bit.tm0ef != RESET) || (can_x->tsts_bit.tm1ef != RESET) || (can_x->tsts_bit.tm2ef != RESET)) {
                bit_status = SET;
            } else {
                bit_status = RESET;
            }

            break;

        default:
            bit_status = RESET;
            break;
    }

    return bit_status;
}

/**
  * @brief  clear flag of the specified can peripheral.
  * @param  can_x: select the can peripheral.
  *         this parameter can be one of the following values:
  *         CAN1,CAN2.
  * @param  can_flag: select the flag.
  *         this parameter can be one of the following flags:
  *         - CAN_EAF_FLAG
  *         - CAN_EPF_FLAG
  *         - CAN_BOF_FLAG
  *         - CAN_ETR_FLAG
  *         - CAN_EOIF_FLAG
  *         - CAN_TM0TCF_FLAG
  *         - CAN_TM1TCF_FLAG
  *         - CAN_TM2TCF_FLAG
  *         - CAN_RF0FF_FLAG
  *         - CAN_RF0OF_FLAG
  *         - CAN_RF1FF_FLAG
  *         - CAN_RF1OF_FLAG
  *         - CAN_QDZIF_FLAG
  *         - CAN_EDZC_FLAG
  *         - CAN_TMEF_FLAG
  *         note:CAN_RF0MN_FLAG and CAN_RF1MN_FLAG can not clear by this function
  * @retval none.
  */
void can_Flag_Clear(can_Type* can_x, uint32_t can_flag) {
    switch(can_flag) {
        case CAN_EAF_FLAG:
        case CAN_EPF_FLAG:
        case CAN_BOF_FLAG:
        case CAN_EOIF_FLAG:
            can_x->msts = CAN_MSTS_EOIF_VAL;
            break;

        case CAN_ETR_FLAG:
            can_x->msts = CAN_MSTS_EOIF_VAL;
            can_x->ests = 0;
            break;

        case CAN_TM0TCF_FLAG:
            can_x->tsts = CAN_TSTS_TM0TCF_VAL;
            break;

        case CAN_TM1TCF_FLAG:
            can_x->tsts = CAN_TSTS_TM1TCF_VAL;
            break;

        case CAN_TM2TCF_FLAG:
            can_x->tsts = CAN_TSTS_TM2TCF_VAL;
            break;

        case CAN_RF0FF_FLAG:
            can_x->rf0 = CAN_RF0_RF0FF_VAL;
            break;

        case CAN_RF0OF_FLAG:
            can_x->rf0 = CAN_RF0_RF0OF_VAL;
            break;

        case CAN_RF1FF_FLAG:
            can_x->rf1 = CAN_RF1_RF1FF_VAL;
            break;

        case CAN_RF1OF_FLAG:
            can_x->rf1 = CAN_RF1_RF1OF_VAL;
            break;

        case CAN_QDZIF_FLAG:
            can_x->msts = CAN_MSTS_QDZIF_VAL;
            break;

        case CAN_EDZC_FLAG:
            can_x->msts = CAN_MSTS_EDZIF_VAL;
            break;

        case CAN_TMEF_FLAG:
            can_x->tsts = CAN_TSTS_TM0TCF_VAL | CAN_TSTS_TM1TCF_VAL | CAN_TSTS_TM2TCF_VAL;
            break;

        default:
            break;
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
