/*!
    文件:    gd32f4xx_can.c
    简介:   CAN driver

    版本: 2016-08-15, V1.0.0, firmware for GD32F4xx
    版本: 2018-12-12, V2.0.0, firmware for GD32F4xx
    版本: 2019-11-27, V2.0.1, firmware for GD32F4xx
    版本: 2020-07-14, V2.0.2, firmware for GD32F4xx
    版本: 2020-09-30, V2.1.0, firmware for GD32F4xx
    版本: 2021-12-28, V2.1.1, firmware for GD32F4xx
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

#include "gd32f4xx_can.h"

#define CAN_Error_HANDLE(s)     do{}while(1)

/*!
    简介:      deinitialize CAN
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void CAN_DeInit(uint32_t CAN_periph) {
    if(CAN0 == CAN_periph) {
        RCU_Periph_Reset_Enable(RCU_CAN0RST);
        RCU_Periph_Reset_Disable(RCU_CAN0RST);
    } else {
        RCU_Periph_Reset_Enable(RCU_CAN1RST);
        RCU_Periph_Reset_Disable(RCU_CAN1RST);
    }
}

/*!
    简介:      initialize CAN parameter struct with a default value
    参数[输入]:  type: the type of CAN parameter struct
                only one parameter can be selected which is shown as below:
      参数:        CAN_INIT_Struct: the CAN initial struct
      参数:        CAN_Filter_Struct: the CAN filter struct
      参数:        CAN_TX_Message_Struct: the CAN TX message struct
      参数:        CAN_RX_Message_Struct: the CAN RX message struct
    参数[输出]: p_Struct: the pointer of the specific struct
    返回值:      无
*/
void CAN_Struct_Para_Init(CAN_Struct_type_enum type, void *p_Struct) {
    uint8_t i;

    /* get type of the struct */
    switch(type) {
    /* used for CAN_Init() */
    case CAN_INIT_Struct:
        ((CAN_Parameter_Struct *)p_Struct)->auto_bus_off_recovery = DISABLE;
        ((CAN_Parameter_Struct *)p_Struct)->auto_retrans = ENABLE;
        ((CAN_Parameter_Struct *)p_Struct)->auto_wake_up = DISABLE;
        ((CAN_Parameter_Struct *)p_Struct)->prescaler = 0x03FFU;
        ((CAN_Parameter_Struct *)p_Struct)->rec_FIFO_overwrite = ENABLE;
        ((CAN_Parameter_Struct *)p_Struct)->resync_jump_width = CAN_BT_SJW_1TQ;
        ((CAN_Parameter_Struct *)p_Struct)->time_segment_1 = CAN_BT_BS1_3TQ;
        ((CAN_Parameter_Struct *)p_Struct)->time_segment_2 = CAN_BT_BS2_1TQ;
        ((CAN_Parameter_Struct *)p_Struct)->time_triggered = DISABLE;
        ((CAN_Parameter_Struct *)p_Struct)->trans_FIFO_order = DISABLE;
        ((CAN_Parameter_Struct *)p_Struct)->working_mode = CAN_Normal_MODE;

        break;

    /* used for CAN_Filter_Init() */
    case CAN_Filter_Struct:
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_bits = CAN_FILTERBITS_32BIT;
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_enable = DISABLE;
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_FIFO_number = CAN_FIFO0;
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_list_high = 0x0000U;
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_list_low = 0x0000U;
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_mask_high = 0x0000U;
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_mask_low = 0x0000U;
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_mode = CAN_FILTERMODE_MASK;
        ((CAN_Filter_Parameter_Struct *)p_Struct)->filter_number = 0U;

        break;

    /* used for CAN_Message_Transmit() */
    case CAN_TX_Message_Struct:
        for(i = 0U; i < 8U; i++) {
            ((CAN_trasnmit_Message_Struct *)p_Struct)->tx_data[i] = 0U;
        }

        ((CAN_trasnmit_Message_Struct *)p_Struct)->tx_dlen = 0u;
        ((CAN_trasnmit_Message_Struct *)p_Struct)->tx_efid = 0U;
        ((CAN_trasnmit_Message_Struct *)p_Struct)->tx_ff = (uint8_t)CAN_FF_STANDARD;
        ((CAN_trasnmit_Message_Struct *)p_Struct)->tx_ft = (uint8_t)CAN_FT_DATA;
        ((CAN_trasnmit_Message_Struct *)p_Struct)->tx_sfid = 0U;

        break;

    /* used for CAN_Message_Receive() */
    case CAN_RX_Message_Struct:
        for(i = 0U; i < 8U; i++) {
            ((CAN_Receive_Message_Struct *)p_Struct)->rx_data[i] = 0U;
        }

        ((CAN_Receive_Message_Struct *)p_Struct)->rx_dlen = 0U;
        ((CAN_Receive_Message_Struct *)p_Struct)->rx_efid = 0U;
        ((CAN_Receive_Message_Struct *)p_Struct)->rx_ff = (uint8_t)CAN_FF_STANDARD;
        ((CAN_Receive_Message_Struct *)p_Struct)->rx_fi = 0U;
        ((CAN_Receive_Message_Struct *)p_Struct)->rx_ft = (uint8_t)CAN_FT_DATA;
        ((CAN_Receive_Message_Struct *)p_Struct)->rx_sfid = 0U;

        break;

    default:
        CAN_Error_HANDLE("parameter is invalid \r\n");
    }
}

/*!
    简介:      initialize CAN
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  CAN_Parameter_init: parameters for CAN initializtion
      参数:        working_mode: CAN_Normal_MODE, CAN_LOOPBACK_MODE, CAN_SILENT_MODE, CAN_SILENT_LOOPBACK_MODE
      参数:        resync_jump_width: CAN_BT_SJW_xTQ(x=1, 2, 3, 4)
      参数:        time_segment_1: CAN_BT_BS1_xTQ(1..16)
      参数:        time_segment_2: CAN_BT_BS2_xTQ(1..8)
      参数:        time_triggered: ENABLE or DISABLE
      参数:        auto_bus_off_recovery: ENABLE or DISABLE
      参数:        auto_wake_up: ENABLE or DISABLE
      参数:        auto_retrans: ENABLE or DISABLE
      参数:        rec_FIFO_overwrite: ENABLE or DISABLE
      参数:        trans_FIFO_order: ENABLE or DISABLE
      参数:        prescaler: 0x0001 - 0x0400
    参数[输出]:  无
    返回值:     ErrStatus: SUCCESS or ERROR
*/
ErrStatus CAN_Init(uint32_t CAN_periph, CAN_Parameter_Struct *CAN_Parameter_init) {
    uint32_t timeout = CAN_TIMEOUT;
    ErrStatus flag = ERROR;

    /* disable sleep mode */
    CAN_CTL(CAN_periph) &= ~CAN_CTL_SLPWMOD;
    /* enable initialize mode */
    CAN_CTL(CAN_periph) |= CAN_CTL_IWMOD;

    /* wait ACK */
    while((CAN_STAT_IWS != (CAN_STAT(CAN_periph) & CAN_STAT_IWS)) && (0U != timeout)) {
        timeout--;
    }

    /* check initialize working success */
    if(CAN_STAT_IWS != (CAN_STAT(CAN_periph) & CAN_STAT_IWS)) {
        flag = ERROR;
    } else {
        /* set the bit timing register */
        CAN_BT(CAN_periph) = (BT_Mode((uint32_t)CAN_Parameter_init->working_mode) | \
                              BT_SJW((uint32_t)CAN_Parameter_init->resync_jump_width) | \
                              BT_BS1((uint32_t)CAN_Parameter_init->time_segment_1) | \
                              BT_BS2((uint32_t)CAN_Parameter_init->time_segment_2) | \
                              BT_BAUDPSC(((uint32_t)(CAN_Parameter_init->prescaler) - 1U)));

        /* time trigger communication mode */
        if(ENABLE == CAN_Parameter_init->time_triggered) {
            CAN_CTL(CAN_periph) |= CAN_CTL_TTC;
        } else {
            CAN_CTL(CAN_periph) &= ~CAN_CTL_TTC;
        }

        /* automatic bus-off management */
        if(ENABLE == CAN_Parameter_init->auto_bus_off_recovery) {
            CAN_CTL(CAN_periph) |= CAN_CTL_ABOR;
        } else {
            CAN_CTL(CAN_periph) &= ~CAN_CTL_ABOR;
        }

        /* automatic wakeup mode */
        if(ENABLE == CAN_Parameter_init->auto_wake_up) {
            CAN_CTL(CAN_periph) |= CAN_CTL_AWU;
        } else {
            CAN_CTL(CAN_periph) &= ~CAN_CTL_AWU;
        }

        /* automatic retransmission mode disable */
        if(ENABLE == CAN_Parameter_init->auto_retrans) {
            CAN_CTL(CAN_periph) &= ~CAN_CTL_ARD;
        } else {
            CAN_CTL(CAN_periph) |= CAN_CTL_ARD;
        }

        /* receive FIFO overwrite mode disable */
        if(ENABLE == CAN_Parameter_init->rec_FIFO_overwrite) {
            CAN_CTL(CAN_periph) &= ~CAN_CTL_RFOD;
        } else {
            CAN_CTL(CAN_periph) |= CAN_CTL_RFOD;
        }

        /* transmit FIFO order */
        if(ENABLE == CAN_Parameter_init->trans_FIFO_order) {
            CAN_CTL(CAN_periph) |= CAN_CTL_TFO;
        } else {
            CAN_CTL(CAN_periph) &= ~CAN_CTL_TFO;
        }

        /* disable initialize mode */
        CAN_CTL(CAN_periph) &= ~CAN_CTL_IWMOD;
        timeout = CAN_TIMEOUT;

        /* wait the ACK */
        while((CAN_STAT_IWS == (CAN_STAT(CAN_periph) & CAN_STAT_IWS)) && (0U != timeout)) {
            timeout--;
        }

        /* check exit initialize mode */
        if(0U != timeout) {
            flag = SUCCESS;
        }
    }

    return flag;
}

/*!
    简介:      initialize CAN filter
    参数[输入]:  CAN_Filter_Parameter_init: struct for CAN filter initialization
      参数:        filter_list_high: 0x0000 - 0xFFFF
      参数:        filter_list_low: 0x0000 - 0xFFFF
      参数:        filter_mask_high: 0x0000 - 0xFFFF
      参数:        filter_mask_low: 0x0000 - 0xFFFF
      参数:        filter_FIFO_number: CAN_FIFO0, CAN_FIFO1
      参数:        filter_number: 0 - 27
      参数:        filter_mode: CAN_FILTERMODE_MASK, CAN_FILTERMODE_LIST
      参数:        filter_bits: CAN_FILTERBITS_32BIT, CAN_FILTERBITS_16BIT
      参数:        filter_enable: ENABLE or DISABLE
    参数[输出]:  无
    返回值:      无
*/
void CAN_Filter_Init(CAN_Filter_Parameter_Struct *CAN_Filter_Parameter_init) {
    uint32_t val = 0U;

    val = ((uint32_t)1) << (CAN_Filter_Parameter_init->filter_number);
    /* filter lock disable */
    CAN_FCTL(CAN0) |= CAN_FCTL_FLD;
    /* disable filter */
    CAN_FW(CAN0) &= ~(uint32_t)val;

    /* filter 16 bits */
    if(CAN_FILTERBITS_16BIT == CAN_Filter_Parameter_init->filter_bits) {
        /* set filter 16 bits */
        CAN_FSCFG(CAN0) &= ~(uint32_t)val;
        /* first 16 bits list and first 16 bits mask or first 16 bits list and second 16 bits list */
        CAN_FDATA0(CAN0, CAN_Filter_Parameter_init->filter_number) = \
                FDATA_MASK_HIGH((CAN_Filter_Parameter_init->filter_mask_low) & CAN_Filter_MASK_16BITS) | \
                FDATA_MASK_LOW((CAN_Filter_Parameter_init->filter_list_low) & CAN_Filter_MASK_16BITS);
        /* second 16 bits list and second 16 bits mask or third 16 bits list and fourth 16 bits list */
        CAN_FDATA1(CAN0, CAN_Filter_Parameter_init->filter_number) = \
                FDATA_MASK_HIGH((CAN_Filter_Parameter_init->filter_mask_high) & CAN_Filter_MASK_16BITS) | \
                FDATA_MASK_LOW((CAN_Filter_Parameter_init->filter_list_high) & CAN_Filter_MASK_16BITS);
    }

    /* filter 32 bits */
    if(CAN_FILTERBITS_32BIT == CAN_Filter_Parameter_init->filter_bits) {
        /* set filter 32 bits */
        CAN_FSCFG(CAN0) |= (uint32_t)val;
        /* 32 bits list or first 32 bits list */
        CAN_FDATA0(CAN0, CAN_Filter_Parameter_init->filter_number) = \
                FDATA_MASK_HIGH((CAN_Filter_Parameter_init->filter_list_high) & CAN_Filter_MASK_16BITS) |
                FDATA_MASK_LOW((CAN_Filter_Parameter_init->filter_list_low) & CAN_Filter_MASK_16BITS);
        /* 32 bits mask or second 32 bits list */
        CAN_FDATA1(CAN0, CAN_Filter_Parameter_init->filter_number) = \
                FDATA_MASK_HIGH((CAN_Filter_Parameter_init->filter_mask_high) & CAN_Filter_MASK_16BITS) |
                FDATA_MASK_LOW((CAN_Filter_Parameter_init->filter_mask_low) & CAN_Filter_MASK_16BITS);
    }

    /* filter mode */
    if(CAN_FILTERMODE_MASK == CAN_Filter_Parameter_init->filter_mode) {
        /* mask mode */
        CAN_FMCFG(CAN0) &= ~(uint32_t)val;
    } else {
        /* list mode */
        CAN_FMCFG(CAN0) |= (uint32_t)val;
    }

    /* filter FIFO */
    if(CAN_FIFO0 == (CAN_Filter_Parameter_init->filter_FIFO_number)) {
        /* FIFO0 */
        CAN_FAFIFO(CAN0) &= ~(uint32_t)val;
    } else {
        /* FIFO1 */
        CAN_FAFIFO(CAN0) |= (uint32_t)val;
    }

    /* filter working */
    if(ENABLE == CAN_Filter_Parameter_init->filter_enable) {

        CAN_FW(CAN0) |= (uint32_t)val;
    }

    /* filter lock enable */
    CAN_FCTL(CAN0) &= ~CAN_FCTL_FLD;
}

/*!
    简介:      set CAN1 filter start bank number
    参数[输入]:  start_bank: CAN1 start bank number
                only one parameter can be selected which is shown as below:
      参数:        (1..27)
    参数[输出]:  无
    返回值:      无
*/
void can1_Filter_start_bank(uint8_t start_bank) {
    /* filter lock disable */
    CAN_FCTL(CAN0) |= CAN_FCTL_FLD;
    /* set CAN1 filter start number */
    CAN_FCTL(CAN0) &= ~(uint32_t)CAN_FCTL_HBC1F;
    CAN_FCTL(CAN0) |= FCTL_HBC1F(start_bank);
    /* filter lock enable */
    CAN_FCTL(CAN0) &= ~CAN_FCTL_FLD;
}

/*!
    简介:      enable CAN debug freeze
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void CAN_Debug_Freeze_Enable(uint32_t CAN_periph) {
    /* set DFZ bit */
    CAN_CTL(CAN_periph) |= CAN_CTL_DFZ;

    if(CAN0 == CAN_periph) {
        DBG_Periph_Enable(DBG_CAN0_HOLD);
    } else {
        DBG_Periph_Enable(DBG_CAN1_HOLD);
    }
}

/*!
    简介:      disable CAN debug freeze
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void CAN_Debug_Freeze_Disable(uint32_t CAN_periph) {
    /* set DFZ bit */
    CAN_CTL(CAN_periph) &= ~CAN_CTL_DFZ;

    if(CAN0 == CAN_periph) {
        DBG_Periph_Disable(DBG_CAN0_HOLD);
    } else {
        DBG_Periph_Disable(DBG_CAN1_HOLD);
    }
}

/*!
    简介:      enable CAN time trigger mode
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void CAN_Time_Trigger_Mode_Enable(uint32_t CAN_periph) {
    uint8_t mailbox_number;

    /* enable the TTC mode */
    CAN_CTL(CAN_periph) |= CAN_CTL_TTC;

    /* enable time stamp */
    for(mailbox_number = 0U; mailbox_number < 3U; mailbox_number++) {
        CAN_TMP(CAN_periph, mailbox_number) |= CAN_TMP_TSEN;
    }
}

/*!
    简介:      disable CAN time trigger mode
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void CAN_Time_Trigger_Mode_Disable(uint32_t CAN_periph) {
    uint8_t mailbox_number;

    /* disable the TTC mode */
    CAN_CTL(CAN_periph) &= ~CAN_CTL_TTC;

    /* reset TSEN bits */
    for(mailbox_number = 0U; mailbox_number < 3U; mailbox_number++) {
        CAN_TMP(CAN_periph, mailbox_number) &= ~CAN_TMP_TSEN;
    }
}

/*!
    简介:      transmit CAN message
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  transmit_message: struct for CAN transmit message
      参数:        tx_sfid: 0x00000000 - 0x000007FF
      参数:        tx_efid: 0x00000000 - 0x1FFFFFFF
      参数:        tx_ff: CAN_FF_STANDARD, CAN_FF_EXTENDED
      参数:        tx_ft: CAN_FT_DATA, CAN_FT_REMOTE
      参数:        tx_dlen: 0 - 8
      参数:        tx_data[]: 0x00 - 0xFF
    参数[输出]:  无
    返回值:     mailbox_number
*/
uint8_t CAN_Message_Transmit(uint32_t CAN_periph, CAN_trasnmit_Message_Struct *transmit_message) {
    uint8_t mailbox_number = CAN_MAILBOX0;

    /* select one empty mailbox */
    if(CAN_TSTAT_TME0 == (CAN_TSTAT(CAN_periph)&CAN_TSTAT_TME0)) {
        mailbox_number = CAN_MAILBOX0;
    } else if(CAN_TSTAT_TME1 == (CAN_TSTAT(CAN_periph)&CAN_TSTAT_TME1)) {
        mailbox_number = CAN_MAILBOX1;
    } else if(CAN_TSTAT_TME2 == (CAN_TSTAT(CAN_periph)&CAN_TSTAT_TME2)) {
        mailbox_number = CAN_MAILBOX2;
    } else {
        mailbox_number = CAN_NOMAILBOX;
    }

    /* return no mailbox empty */
    if(CAN_NOMAILBOX == mailbox_number) {
        return CAN_NOMAILBOX;
    }

    CAN_TMI(CAN_periph, mailbox_number) &= CAN_TMI_TEN;

    if(CAN_FF_STANDARD == transmit_message->tx_ff) {
        /* set transmit mailbox standard identifier */
        CAN_TMI(CAN_periph, mailbox_number) |= (uint32_t)(TMI_SFID(transmit_message->tx_sfid) | \
                                               transmit_message->tx_ft);
    } else {
        /* set transmit mailbox extended identifier */
        CAN_TMI(CAN_periph, mailbox_number) |= (uint32_t)(TMI_EFID(transmit_message->tx_efid) | \
                                               transmit_message->tx_ff | \
                                               transmit_message->tx_ft);
    }

    /* set the data length */
    CAN_TMP(CAN_periph, mailbox_number) &= ~CAN_TMP_DLENC;
    CAN_TMP(CAN_periph, mailbox_number) |= transmit_message->tx_dlen;
    /* set the data */
    CAN_TMDATA0(CAN_periph, mailbox_number) = TMDATA0_DB3(transmit_message->tx_data[3]) | \
            TMDATA0_DB2(transmit_message->tx_data[2]) | \
            TMDATA0_DB1(transmit_message->tx_data[1]) | \
            TMDATA0_DB0(transmit_message->tx_data[0]);
    CAN_TMDATA1(CAN_periph, mailbox_number) = TMDATA1_DB7(transmit_message->tx_data[7]) | \
            TMDATA1_DB6(transmit_message->tx_data[6]) | \
            TMDATA1_DB5(transmit_message->tx_data[5]) | \
            TMDATA1_DB4(transmit_message->tx_data[4]);
    /* enable transmission */
    CAN_TMI(CAN_periph, mailbox_number) |= CAN_TMI_TEN;

    return mailbox_number;
}

/*!
    简介:      get CAN transmit state
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  mailbox_number
                only one parameter can be selected which is shown as below:
      参数:        CAN_MAILBOX(x=0,1,2)
    参数[输出]:  无
    返回值:     CAN_Transmit_State_enum
*/
CAN_Transmit_State_enum CAN_Transmit_States(uint32_t CAN_periph, uint8_t mailbox_number) {
    CAN_Transmit_State_enum state = CAN_Transmit_FAILED;
    uint32_t val = 0U;

    /* check selected mailbox state */
    switch(mailbox_number) {
    /* mailbox0 */
    case CAN_MAILBOX0:
        val = CAN_TSTAT(CAN_periph) & (CAN_TSTAT_MTF0 | CAN_TSTAT_MTFNERR0 | CAN_TSTAT_TME0);
        break;

    /* mailbox1 */
    case CAN_MAILBOX1:
        val = CAN_TSTAT(CAN_periph) & (CAN_TSTAT_MTF1 | CAN_TSTAT_MTFNERR1 | CAN_TSTAT_TME1);
        break;

    /* mailbox2 */
    case CAN_MAILBOX2:
        val = CAN_TSTAT(CAN_periph) & (CAN_TSTAT_MTF2 | CAN_TSTAT_MTFNERR2 | CAN_TSTAT_TME2);
        break;

    default:
        val = CAN_Transmit_FAILED;
        break;
    }

    switch(val) {
    /* transmit pending */
    case(CAN_State_PENDING):
        state = CAN_Transmit_PENDING;
        break;

    /* mailbox0 transmit succeeded */
    case(CAN_TSTAT_MTF0 | CAN_TSTAT_MTFNERR0 | CAN_TSTAT_TME0):
        state = CAN_Transmit_OK;
        break;

    /* mailbox1 transmit succeeded */
    case(CAN_TSTAT_MTF1 | CAN_TSTAT_MTFNERR1 | CAN_TSTAT_TME1):
        state = CAN_Transmit_OK;
        break;

    /* mailbox2 transmit succeeded */
    case(CAN_TSTAT_MTF2 | CAN_TSTAT_MTFNERR2 | CAN_TSTAT_TME2):
        state = CAN_Transmit_OK;
        break;

    /* transmit failed */
    default:
        state = CAN_Transmit_FAILED;
        break;
    }

    return state;
}

/*!
    简介:      stop CAN transmission
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  mailbox_number
                only one parameter can be selected which is shown as below:
      参数:        CAN_MAILBOXx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void CAN_Transmission_Stop(uint32_t CAN_periph, uint8_t mailbox_number) {
    if(CAN_MAILBOX0 == mailbox_number) {
        CAN_TSTAT(CAN_periph) |= CAN_TSTAT_MST0;

        while(CAN_TSTAT_MST0 == (CAN_TSTAT(CAN_periph) & CAN_TSTAT_MST0)) {
        }
    } else if(CAN_MAILBOX1 == mailbox_number) {
        CAN_TSTAT(CAN_periph) |= CAN_TSTAT_MST1;

        while(CAN_TSTAT_MST1 == (CAN_TSTAT(CAN_periph) & CAN_TSTAT_MST1)) {
        }
    } else if(CAN_MAILBOX2 == mailbox_number) {
        CAN_TSTAT(CAN_periph) |= CAN_TSTAT_MST2;

        while(CAN_TSTAT_MST2 == (CAN_TSTAT(CAN_periph) & CAN_TSTAT_MST2)) {
        }
    } else {
        /* illegal parameters */
    }
}

/*!
    简介:      CAN receive message
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  fifo_number
      参数:        CAN_FIFOx(x=0,1)
    参数[输出]: receive_message: struct for CAN receive message
      参数:        rx_sfid: 0x00000000 - 0x000007FF
      参数:        rx_efid: 0x00000000 - 0x1FFFFFFF
      参数:        rx_ff: CAN_FF_STANDARD, CAN_FF_EXTENDED
      参数:        rx_ft: CAN_FT_DATA, CAN_FT_REMOTE
      参数:        rx_dlen: 0 - 8
      参数:        rx_data[]: 0x00 - 0xFF
      参数:        rx_fi: 0 - 27
    返回值:      无
*/
void CAN_Message_Receive(uint32_t CAN_periph, uint8_t fifo_number, CAN_Receive_Message_Struct *receive_message) {
    /* get the frame format */
    receive_message->rx_ff = (uint8_t)(CAN_RFIFOMI_FF & CAN_RFIFOMI(CAN_periph, fifo_number));

    if(CAN_FF_STANDARD == receive_message->rx_ff) {
        /* get standard identifier */
        receive_message->rx_sfid = (uint32_t)(GET_RFIFOMI_SFID(CAN_RFIFOMI(CAN_periph, fifo_number)));
    } else {
        /* get extended identifier */
        receive_message->rx_efid = (uint32_t)(GET_RFIFOMI_EFID(CAN_RFIFOMI(CAN_periph, fifo_number)));
    }

    /* get frame type */
    receive_message->rx_ft = (uint8_t)(CAN_RFIFOMI_FT & CAN_RFIFOMI(CAN_periph, fifo_number));
    /* filtering index */
    receive_message->rx_fi = (uint8_t)(GET_RFIFOMP_FI(CAN_RFIFOMP(CAN_periph, fifo_number)));
    /* get receive data length */
    receive_message->rx_dlen = (uint8_t)(GET_RFIFOMP_DLENC(CAN_RFIFOMP(CAN_periph, fifo_number)));

    /* receive data */
    receive_message -> rx_data[0] = (uint8_t)(GET_RFIFOMDATA0_DB0(CAN_RFIFOMDATA0(CAN_periph, fifo_number)));
    receive_message -> rx_data[1] = (uint8_t)(GET_RFIFOMDATA0_DB1(CAN_RFIFOMDATA0(CAN_periph, fifo_number)));
    receive_message -> rx_data[2] = (uint8_t)(GET_RFIFOMDATA0_DB2(CAN_RFIFOMDATA0(CAN_periph, fifo_number)));
    receive_message -> rx_data[3] = (uint8_t)(GET_RFIFOMDATA0_DB3(CAN_RFIFOMDATA0(CAN_periph, fifo_number)));
    receive_message -> rx_data[4] = (uint8_t)(GET_RFIFOMDATA1_DB4(CAN_RFIFOMDATA1(CAN_periph, fifo_number)));
    receive_message -> rx_data[5] = (uint8_t)(GET_RFIFOMDATA1_DB5(CAN_RFIFOMDATA1(CAN_periph, fifo_number)));
    receive_message -> rx_data[6] = (uint8_t)(GET_RFIFOMDATA1_DB6(CAN_RFIFOMDATA1(CAN_periph, fifo_number)));
    receive_message -> rx_data[7] = (uint8_t)(GET_RFIFOMDATA1_DB7(CAN_RFIFOMDATA1(CAN_periph, fifo_number)));

    /* release FIFO */
    if(CAN_FIFO0 == fifo_number) {
        CAN_RFIFO0(CAN_periph) |= CAN_RFIFO0_RFD0;
    } else {
        CAN_RFIFO1(CAN_periph) |= CAN_RFIFO1_RFD1;
    }
}

/*!
    简介:      release FIFO
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  fifo_number
                only one parameter can be selected which is shown as below:
      参数:        CAN_FIFOx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void CAN_FIFO_Release(uint32_t CAN_periph, uint8_t fifo_number) {
    if(CAN_FIFO0 == fifo_number) {
        CAN_RFIFO0(CAN_periph) |= CAN_RFIFO0_RFD0;
    } else if(CAN_FIFO1 == fifo_number) {
        CAN_RFIFO1(CAN_periph) |= CAN_RFIFO1_RFD1;
    } else {
        /* illegal parameters */
        CAN_Error_HANDLE("CAN FIFO NUM is invalid \r\n");
    }
}

/*!
    简介:      CAN receive message length
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  fifo_number
                only one parameter can be selected which is shown as below:
      参数:        CAN_FIFOx(x=0,1)
    参数[输出]:  无
    返回值:     message length
*/
uint8_t CAN_Receive_Message_Length_Get(uint32_t CAN_periph, uint8_t fifo_number) {
    uint8_t val = 0U;

    if(CAN_FIFO0 == fifo_number) {
        /* FIFO0 */
        val = (uint8_t)(CAN_RFIFO0(CAN_periph) & CAN_RFIF_RFL_MASK);
    } else if(CAN_FIFO1 == fifo_number) {
        /* FIFO1 */
        val = (uint8_t)(CAN_RFIFO1(CAN_periph) & CAN_RFIF_RFL_MASK);
    } else {
        /* illegal parameters */
    }

    return val;
}

/*!
    简介:      set CAN working mode
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  CAN_Working_mode
                only one parameter can be selected which is shown as below:
      参数:        CAN_Mode_INITIALIZE
      参数:        CAN_Mode_NORMAL
      参数:        CAN_Mode_SLEEP
    参数[输出]:  无
    返回值:     ErrStatus: SUCCESS or ERROR
*/
ErrStatus CAN_Working_Mode_Set(uint32_t CAN_periph, uint8_t working_mode) {
    ErrStatus flag = ERROR;
    /* timeout for IWS or also for SLPWS bits */
    uint32_t timeout = CAN_TIMEOUT;

    if(CAN_Mode_INITIALIZE == working_mode) {
        /* disable sleep mode */
        CAN_CTL(CAN_periph) &= (~(uint32_t)CAN_CTL_SLPWMOD);
        /* set initialize mode */
        CAN_CTL(CAN_periph) |= (uint8_t)CAN_CTL_IWMOD;

        /* wait the acknowledge */
        while((CAN_STAT_IWS != (CAN_STAT(CAN_periph) & CAN_STAT_IWS)) && (0U != timeout)) {
            timeout--;
        }

        if(CAN_STAT_IWS != (CAN_STAT(CAN_periph) & CAN_STAT_IWS)) {
            flag = ERROR;
        } else {
            flag = SUCCESS;
        }
    } else if(CAN_Mode_NORMAL == working_mode) {
        /* enter normal mode */
        CAN_CTL(CAN_periph) &= ~(uint32_t)(CAN_CTL_SLPWMOD | CAN_CTL_IWMOD);

        /* wait the acknowledge */
        while((0U != (CAN_STAT(CAN_periph) & (CAN_STAT_IWS | CAN_STAT_SLPWS))) && (0U != timeout)) {
            timeout--;
        }

        if(0U != (CAN_STAT(CAN_periph) & (CAN_STAT_IWS | CAN_STAT_SLPWS))) {
            flag = ERROR;
        } else {
            flag = SUCCESS;
        }
    } else if(CAN_Mode_SLEEP == working_mode) {
        /* disable initialize mode */
        CAN_CTL(CAN_periph) &= (~(uint32_t)CAN_CTL_IWMOD);
        /* set sleep mode */
        CAN_CTL(CAN_periph) |= (uint8_t)CAN_CTL_SLPWMOD;

        /* wait the acknowledge */
        while((CAN_STAT_SLPWS != (CAN_STAT(CAN_periph) & CAN_STAT_SLPWS)) && (0U != timeout)) {
            timeout--;
        }

        if(CAN_STAT_SLPWS != (CAN_STAT(CAN_periph) & CAN_STAT_SLPWS)) {
            flag = ERROR;
        } else {
            flag = SUCCESS;
        }
    } else {
        flag = ERROR;
    }

    return flag;
}

/*!
    简介:      wake up CAN
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:     ErrStatus: SUCCESS or ERROR
*/
ErrStatus CAN_WakeUp(uint32_t CAN_periph) {
    ErrStatus flag = ERROR;
    uint32_t timeout = CAN_TIMEOUT;

    /* wakeup */
    CAN_CTL(CAN_periph) &= ~CAN_CTL_SLPWMOD;

    while((0U != (CAN_STAT(CAN_periph) & CAN_STAT_SLPWS)) && (0x00U != timeout)) {
        timeout--;
    }

    /* check state */
    if(0U != (CAN_STAT(CAN_periph) & CAN_STAT_SLPWS)) {
        flag = ERROR;
    } else {
        flag = SUCCESS;
    }

    return flag;
}

/*!
    简介:      get CAN error type
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:     CAN_Error_enum
      参数:        CAN_Error_NONE: no error
      参数:        CAN_Error_FILL: fill error
      参数:        CAN_Error_FORMATE: format error
      参数:        CAN_Error_ACK: ACK error
      参数:        CAN_Error_BITRECESSIVE: bit recessive
      参数:        CAN_Error_BITDOMINANTER: bit dominant error
      参数:        CAN_Error_CRC: CRC error
      参数:        CAN_Error_SOFTWARECFG: software configure
*/
CAN_Error_enum CAN_Error_Get(uint32_t CAN_periph) {
    CAN_Error_enum error;
    error = CAN_Error_NONE;

    /* get error type */
    error = (CAN_Error_enum)(GET_ERR_ERRN(CAN_ERR(CAN_periph)));
    return error;
}

/*!
    简介:      get CAN receive error number
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:     error number
*/
uint8_t CAN_Receive_Error_Number_Get(uint32_t CAN_periph) {
    uint8_t val;

    /* get error count */
    val = (uint8_t)(GET_ERR_RECNT(CAN_ERR(CAN_periph)));
    return val;
}

/*!
    简介:      get CAN transmit error number
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输出]:  无
    返回值:     error number
*/
uint8_t CAN_Transmit_Error_Number_Get(uint32_t CAN_periph) {
    uint8_t val;

    val = (uint8_t)(GET_ERR_TECNT(CAN_ERR(CAN_periph)));
    return val;
}

/*!
    简介:      get CAN flag state
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  flag: CAN flags, refer to CAN_Flag_enum
                only one parameter can be selected which is shown as below:
      参数:        CAN_Flag_RXL: RX level
      参数:        CAN_Flag_LASTRX: last sample value of RX pin
      参数:        CAN_Flag_RS: receiving state
      参数:        CAN_Flag_TS: transmitting state
      参数:        CAN_Flag_SLPIF: status change flag of entering sleep working mode
      参数:        CAN_Flag_WUIF: status change flag of wakeup from sleep working mode
      参数:        CAN_Flag_ERRIF: error flag
      参数:        CAN_Flag_SLPWS: sleep working state
      参数:        CAN_Flag_IWS: initial working state
      参数:        CAN_Flag_TMLS2: transmit mailbox 2 last sending in TX FIFO
      参数:        CAN_Flag_TMLS1: transmit mailbox 1 last sending in TX FIFO
      参数:        CAN_Flag_TMLS0: transmit mailbox 0 last sending in TX FIFO
      参数:        CAN_Flag_TME2: transmit mailbox 2 empty
      参数:        CAN_Flag_TME1: transmit mailbox 1 empty
      参数:        CAN_Flag_TME0: transmit mailbox 0 empty
      参数:        CAN_Flag_MTE2: mailbox 2 transmit error
      参数:        CAN_Flag_MTE1: mailbox 1 transmit error
      参数:        CAN_Flag_MTE0: mailbox 0 transmit error
      参数:        CAN_Flag_MAL2: mailbox 2 arbitration lost
      参数:        CAN_Flag_MAL1: mailbox 1 arbitration lost
      参数:        CAN_Flag_MAL0: mailbox 0 arbitration lost
      参数:        CAN_Flag_MTFNERR2: mailbox 2 transmit finished with no error
      参数:        CAN_Flag_MTFNERR1: mailbox 1 transmit finished with no error
      参数:        CAN_Flag_MTFNERR0: mailbox 0 transmit finished with no error
      参数:        CAN_Flag_MTF2: mailbox 2 transmit finished
      参数:        CAN_Flag_MTF1: mailbox 1 transmit finished
      参数:        CAN_Flag_MTF0: mailbox 0 transmit finished
      参数:        CAN_Flag_RFO0: receive FIFO0 overfull
      参数:        CAN_Flag_RFF0: receive FIFO0 full
      参数:        CAN_Flag_RFO1: receive FIFO1 overfull
      参数:        CAN_Flag_RFF1: receive FIFO1 full
      参数:        CAN_Flag_BOERR: bus-off error
      参数:        CAN_Flag_PERR: passive error
      参数:        CAN_Flag_WERR: warning error
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus CAN_Flag_Get(uint32_t CAN_periph, CAN_Flag_enum flag) {
    /* get flag and interrupt enable state */
    if(RESET != (CAN_REG_VAL(CAN_periph, flag) & BIT(CAN_Bit_POS(flag)))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:      clear CAN flag state
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  flag: CAN flags, refer to CAN_Flag_enum
                only one parameter can be selected which is shown as below:
      参数:        CAN_Flag_SLPIF: status change flag of entering sleep working mode
      参数:        CAN_Flag_WUIF: status change flag of wakeup from sleep working mode
      参数:        CAN_Flag_ERRIF: error flag
      参数:        CAN_Flag_MTE2: mailbox 2 transmit error
      参数:        CAN_Flag_MTE1: mailbox 1 transmit error
      参数:        CAN_Flag_MTE0: mailbox 0 transmit error
      参数:        CAN_Flag_MAL2: mailbox 2 arbitration lost
      参数:        CAN_Flag_MAL1: mailbox 1 arbitration lost
      参数:        CAN_Flag_MAL0: mailbox 0 arbitration lost
      参数:        CAN_Flag_MTFNERR2: mailbox 2 transmit finished with no error
      参数:        CAN_Flag_MTFNERR1: mailbox 1 transmit finished with no error
      参数:        CAN_Flag_MTFNERR0: mailbox 0 transmit finished with no error
      参数:        CAN_Flag_MTF2: mailbox 2 transmit finished
      参数:        CAN_Flag_MTF1: mailbox 1 transmit finished
      参数:        CAN_Flag_MTF0: mailbox 0 transmit finished
      参数:        CAN_Flag_RFO0: receive FIFO0 overfull
      参数:        CAN_Flag_RFF0: receive FIFO0 full
      参数:        CAN_Flag_RFO1: receive FIFO1 overfull
      参数:        CAN_Flag_RFF1: receive FIFO1 full
    参数[输出]:  无
    返回值:      无
*/
void CAN_Flag_Clear(uint32_t CAN_periph, CAN_Flag_enum flag) {
    CAN_REG_VAL(CAN_periph, flag) = BIT(CAN_Bit_POS(flag));
}

/*!
    简介:      enable CAN interrupt
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  interrupt
                one or more parameters can be selected which are shown as below:
      参数:        CAN_INT_TME: transmit mailbox empty interrupt enable
      参数:        CAN_INT_RFNE0: receive FIFO0 not empty interrupt enable
      参数:        CAN_INT_RFF0: receive FIFO0 full interrupt enable
      参数:        CAN_INT_RFO0: receive FIFO0 overfull interrupt enable
      参数:        CAN_INT_RFNE1: receive FIFO1 not empty interrupt enable
      参数:        CAN_INT_RFF1: receive FIFO1 full interrupt enable
      参数:        CAN_INT_RFO1: receive FIFO1 overfull interrupt enable
      参数:        CAN_INT_WERR: warning error interrupt enable
      参数:        CAN_INT_PERR: passive error interrupt enable
      参数:        CAN_INT_BO: bus-off interrupt enable
      参数:        CAN_INT_ERRN: error number interrupt enable
      参数:        CAN_INT_ERR: error interrupt enable
      参数:        CAN_INT_WAKEUP: wakeup interrupt enable
      参数:        CAN_INT_SLPW: sleep working interrupt enable
    参数[输出]:  无
    返回值:      无
*/
void CAN_Interrupt_Enable(uint32_t CAN_periph, uint32_t interrupt) {
    CAN_INTEN(CAN_periph) |= interrupt;
}

/*!
    简介:      disable CAN interrupt
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  interrupt
                one or more parameters can be selected which are shown as below:
      参数:        CAN_INT_TME: transmit mailbox empty interrupt enable
      参数:        CAN_INT_RFNE0: receive FIFO0 not empty interrupt enable
      参数:        CAN_INT_RFF0: receive FIFO0 full interrupt enable
      参数:        CAN_INT_RFO0: receive FIFO0 overfull interrupt enable
      参数:        CAN_INT_RFNE1: receive FIFO1 not empty interrupt enable
      参数:        CAN_INT_RFF1: receive FIFO1 full interrupt enable
      参数:        CAN_INT_RFO1: receive FIFO1 overfull interrupt enable
      参数:        CAN_INT_WERR: warning error interrupt enable
      参数:        CAN_INT_PERR: passive error interrupt enable
      参数:        CAN_INT_BO: bus-off interrupt enable
      参数:        CAN_INT_ERRN: error number interrupt enable
      参数:        CAN_INT_ERR: error interrupt enable
      参数:        CAN_INT_WAKEUP: wakeup interrupt enable
      参数:        CAN_INT_SLPW: sleep working interrupt enable
    参数[输出]:  无
    返回值:      无
*/
void CAN_Interrupt_Disable(uint32_t CAN_periph, uint32_t interrupt) {
    CAN_INTEN(CAN_periph) &= ~interrupt;
}

/*!
    简介:      get CAN interrupt flag state
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  flag: CAN interrupt flags, refer to CAN_Interrupt_Flag_enum
                only one parameter can be selected which is shown as below:
      参数:        CAN_INT_Flag_SLPIF: status change interrupt flag of sleep working mode entering
      参数:        CAN_INT_Flag_WUIF: status change interrupt flag of wakeup from sleep working mode
      参数:        CAN_INT_Flag_ERRIF: error interrupt flag
      参数:        CAN_INT_Flag_MTF2: mailbox 2 transmit finished interrupt flag
      参数:        CAN_INT_Flag_MTF1: mailbox 1 transmit finished interrupt flag
      参数:        CAN_INT_Flag_MTF0: mailbox 0 transmit finished interrupt flag
      参数:        CAN_INT_Flag_RFO0: receive FIFO0 overfull interrupt flag
      参数:        CAN_INT_Flag_RFF0: receive FIFO0 full interrupt flag
      参数:        CAN_INT_Flag_RFL0: receive FIFO0 not empty interrupt flag
      参数:        CAN_INT_Flag_RFO1: receive FIFO1 overfull interrupt flag
      参数:        CAN_INT_Flag_RFF1: receive FIFO1 full interrupt flag
      参数:        CAN_INT_Flag_RFL1: receive FIFO1 not empty interrupt flag
      参数:        CAN_INT_Flag_ERRN: error number interrupt flag
      参数:        CAN_INT_Flag_BOERR: bus-off error interrupt flag
      参数:        CAN_INT_Flag_PERR: passive error interrupt flag
      参数:        CAN_INT_Flag_WERR: warning error interrupt flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus CAN_Interrupt_Flag_Get(uint32_t CAN_periph, CAN_Interrupt_Flag_enum flag) {
    uint32_t ret1 = RESET;
    uint32_t ret2 = RESET;

    /* get the status of interrupt flag */
    if(flag == CAN_INT_Flag_RFL0) {
        ret1 = CAN_Receive_Message_Length_Get(CAN_periph, CAN_FIFO0);
    } else if(flag == CAN_INT_Flag_RFL1) {
        ret1 = CAN_Receive_Message_Length_Get(CAN_periph, CAN_FIFO1);
    } else if(flag == CAN_INT_Flag_ERRN) {
        ret1 = CAN_Error_Get(CAN_periph);
    } else {
        ret1 = CAN_REG_VALS(CAN_periph, flag) & BIT(CAN_Bit_POS0(flag));
    }

    /* get the status of interrupt enable bit */
    ret2 = CAN_INTEN(CAN_periph) & BIT(CAN_Bit_POS1(flag));

    if(ret1 && ret2) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:      clear CAN interrupt flag state
    参数[输入]:  CAN_periph
      参数:        CANx(x=0,1)
    参数[输入]:  flag: CAN interrupt flags, refer to CAN_Interrupt_Flag_enum
                only one parameter can be selected which is shown as below:
      参数:        CAN_INT_Flag_SLPIF: status change interrupt flag of sleep working mode entering
      参数:        CAN_INT_Flag_WUIF: status change interrupt flag of wakeup from sleep working mode
      参数:        CAN_INT_Flag_ERRIF: error interrupt flag
      参数:        CAN_INT_Flag_MTF2: mailbox 2 transmit finished interrupt flag
      参数:        CAN_INT_Flag_MTF1: mailbox 1 transmit finished interrupt flag
      参数:        CAN_INT_Flag_MTF0: mailbox 0 transmit finished interrupt flag
      参数:        CAN_INT_Flag_RFO0: receive FIFO0 overfull interrupt flag
      参数:        CAN_INT_Flag_RFF0: receive FIFO0 full interrupt flag
      参数:        CAN_INT_Flag_RFO1: receive FIFO1 overfull interrupt flag
      参数:        CAN_INT_Flag_RFF1: receive FIFO1 full interrupt flag
    参数[输出]:  无
    返回值:      无
*/
void CAN_Interrupt_Flag_Clear(uint32_t CAN_periph, CAN_Interrupt_Flag_enum flag) {
    CAN_REG_VALS(CAN_periph, flag) = BIT(CAN_Bit_POS0(flag));
}
