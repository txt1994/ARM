/**
  **************************************************************************
  * @file     at32f435_437_can.h
  * @brief    at32f435_437 can header file
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

/* define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F435_437_CAN_H
#define __AT32F435_437_CAN_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/** @addtogroup AT32F435_437_Periph_driver
  * @{
  */

/** @addtogroup CAN
  * @{
  */


/** @defgroup CAN_TimeOut_count
  * @{
  */

#define FZC_TIMEOUT                      ((uint32_t)0x0000FFFF) /*!< time out for fzc bit */
#define DZC_TIMEOUT                      ((uint32_t)0x0000FFFF) /*!< time out for dzc bit */

/**
  * @}
  */

/** @defgroup CAN_flags_definition
  * @brief can flag
  * @{
  */

#define CAN_EAF_FLAG                     ((uint32_t)0x01) /*!< error active flag */
#define CAN_EPF_FLAG                     ((uint32_t)0x02) /*!< error passive flag */
#define CAN_BOF_FLAG                     ((uint32_t)0x03) /*!< bus-off flag */
#define CAN_ETR_FLAG                     ((uint32_t)0x04) /*!< error type record flag */
#define CAN_EOIF_FLAG                    ((uint32_t)0x05) /*!< error occur interrupt flag */
#define CAN_TM0TCF_FLAG                  ((uint32_t)0x06) /*!< transmit mailbox 0 transmission completed flag */
#define CAN_TM1TCF_FLAG                  ((uint32_t)0x07) /*!< transmit mailbox 1 transmission completed flag */
#define CAN_TM2TCF_FLAG                  ((uint32_t)0x08) /*!< transmit mailbox 2 transmission completed flag */
#define CAN_RF0MN_FLAG                   ((uint32_t)0x09) /*!< receive fifo 0 message num flag */
#define CAN_RF0FF_FLAG                   ((uint32_t)0x0A) /*!< receive fifo 0 full flag */
#define CAN_RF0OF_FLAG                   ((uint32_t)0x0B) /*!< receive fifo 0 overflow flag */
#define CAN_RF1MN_FLAG                   ((uint32_t)0x0C) /*!< receive fifo 1 message num flag */
#define CAN_RF1FF_FLAG                   ((uint32_t)0x0D) /*!< receive fifo 1 full flag */
#define CAN_RF1OF_FLAG                   ((uint32_t)0x0E) /*!< receive fifo 1 overflow flag */
#define CAN_QDZIF_FLAG                   ((uint32_t)0x0F) /*!< quit doze mode interrupt flag */
#define CAN_EDZC_FLAG                    ((uint32_t)0x10) /*!< enter doze mode confirm flag */
#define CAN_TMEF_FLAG                    ((uint32_t)0x11) /*!< transmit mailbox empty flag */

/**
  * @}
  */

/** @defgroup CAN_Interrupts_definition
  * @brief can interrupt
  * @{
  */

#define CAN_TCIEN_INT                    ((uint32_t)0x00000001) /*!< transmission complete 中断 */
#define CAN_RF0MIEN_INT                  ((uint32_t)0x00000002) /*!< receive fifo 0 message 中断 */
#define CAN_RF0FIEN_INT                  ((uint32_t)0x00000004) /*!< receive fifo 0 full 中断 */
#define CAN_RF0OIEN_INT                  ((uint32_t)0x00000008) /*!< receive fifo 0 overflow 中断 */
#define CAN_RF1MIEN_INT                  ((uint32_t)0x00000010) /*!< receive fifo 1 message 中断 */
#define CAN_RF1FIEN_INT                  ((uint32_t)0x00000020) /*!< receive fifo 1 full 中断 */
#define CAN_RF1OIEN_INT                  ((uint32_t)0x00000040) /*!< receive fifo 1 overflow 中断 */
#define CAN_EAIEN_INT                    ((uint32_t)0x00000100) /*!< error active 中断 */
#define CAN_EPIEN_INT                    ((uint32_t)0x00000200) /*!< error passive 中断 */
#define CAN_BOIEN_INT                    ((uint32_t)0x00000400) /*!< bus-off 中断 */
#define CAN_ETRIEN_INT                   ((uint32_t)0x00000800) /*!< error type record 中断 */
#define CAN_EOIEN_INT                    ((uint32_t)0x00008000) /*!< error occur 中断 */
#define CAN_QDZIEN_INT                   ((uint32_t)0x00010000) /*!< quit doze mode 中断 */
#define CAN_EDZIEN_INT                   ((uint32_t)0x00020000) /*!< enter doze mode confirm 中断 */

/**
  * @}
  */

/**
  * @brief  can flag clear operation macro definition val
  */
#define CAN_MSTS_EOIF_VAL                ((uint32_t)0x00000004) /*!< eoif bit value, it clear by writing 1 */
#define CAN_MSTS_QDZIF_VAL               ((uint32_t)0x00000008) /*!< qdzif bit value, it clear by writing 1 */
#define CAN_MSTS_EDZIF_VAL               ((uint32_t)0x00000010) /*!< edzif bit value, it clear by writing 1 */
#define CAN_TSTS_TM0TCF_VAL              ((uint32_t)0x00000001) /*!< tm0tcf bit value, it clear by writing 1 */
#define CAN_TSTS_TM1TCF_VAL              ((uint32_t)0x00000100) /*!< tm1tcf bit value, it clear by writing 1 */
#define CAN_TSTS_TM2TCF_VAL              ((uint32_t)0x00010000) /*!< tm2tcf bit value, it clear by writing 1 */
#define CAN_TSTS_TM0CT_VAL               ((uint32_t)0x00000080) /*!< tm0ct bit value, it clear by writing 1 */
#define CAN_TSTS_TM1CT_VAL               ((uint32_t)0x00008000) /*!< tm1ct bit value, it clear by writing 1 */
#define CAN_TSTS_TM2CT_VAL               ((uint32_t)0x00800000) /*!< tm2ct bit value, it clear by writing 1 */
#define CAN_RF0_RF0FF_VAL                ((uint32_t)0x00000008) /*!< rf0ff bit value, it clear by writing 1 */
#define CAN_RF0_RF0OF_VAL                ((uint32_t)0x00000010) /*!< rf0of bit value, it clear by writing 1 */
#define CAN_RF0_RF0R_VAL                 ((uint32_t)0x00000020) /*!< rf0r bit value, it clear by writing 1 */
#define CAN_RF1_RF1FF_VAL                ((uint32_t)0x00000008) /*!< rf1ff bit value, it clear by writing 1 */
#define CAN_RF1_RF1OF_VAL                ((uint32_t)0x00000010) /*!< rf1of bit value, it clear by writing 1 */
#define CAN_RF1_RF1R_VAL                 ((uint32_t)0x00000020) /*!< rf1r bit value, it clear by writing 1 */

/** @defgroup CAN_exported_Types
  * @{
  */

/**
  * @brief  can filter fifo
  */
typedef enum {
    CAN_Filter_FIFO0                       = 0x00, /*!< filter fifo 0 assignment for filter x */
    CAN_Filter_FIFO1                       = 0x01  /*!< filter fifo 1 assignment for filter x */
} CAN_Filter_FIFO_Type;

/**
  * @brief  can filter mode
  */
typedef enum {
    CAN_Filter_Mode_ID_MASK                = 0x00, /*!< identifier mask mode */
    CAN_Filter_Mode_ID_LIST                = 0x01  /*!< identifier list mode */
} CAN_Filter_Mode_Type;

/**
  * @brief  can filter bit width select
  */
typedef enum {
    CAN_Filter_16BIT                       = 0x00, /*!< two 16-bit filters */
    CAN_Filter_32BIT                       = 0x01  /*!< one 32-bit filter */
} CAN_Filter_Bit_Width_Type;

/**
  * @brief  can mode
  */
typedef enum {
    CAN_Mode_COMMUNICATE                   = 0x00, /*!< communication mode */
    CAN_Mode_LOOPBACK                      = 0x01, /*!< loopback mode */
    CAN_Mode_LISTENONLY                    = 0x02, /*!< listen-only mode */
    CAN_Mode_LISTENONLY_LOOPBACK           = 0x03  /*!< loopback combined with listen-only mode */
} CAN_Mode_Type;

/**
  * @brief  can operating mode
  */
typedef enum {
    CAN_OPERATINGMODE_Freeze               = 0x00, /*!< freeze mode */
    CAN_OPERATINGMODE_DOZE                 = 0x01, /*!< doze mode */
    CAN_OPERATINGMODE_COMMUNICATE          = 0x02  /*!< communication mode */
} CAN_operating_Mode_Type;

/**
  * @brief  can resynchronization adjust width
  */
typedef enum {
    CAN_RSAW_1TQ                           = 0x00, /*!< 1 time quantum */
    CAN_RSAW_2TQ                           = 0x01, /*!< 2 time quantum */
    CAN_RSAW_3TQ                           = 0x02, /*!< 3 time quantum */
    CAN_RSAW_4TQ                           = 0x03  /*!< 4 time quantum */
} CAN_rsaw_Type;

/**
  * @brief  can bit time segment 1
  */
typedef enum {
    CAN_BTS1_1TQ                           = 0x00, /*!< 1 time quantum */
    CAN_BTS1_2TQ                           = 0x01, /*!< 2 time quantum */
    CAN_BTS1_3TQ                           = 0x02, /*!< 3 time quantum */
    CAN_BTS1_4TQ                           = 0x03, /*!< 4 time quantum */
    CAN_BTS1_5TQ                           = 0x04, /*!< 5 time quantum */
    CAN_BTS1_6TQ                           = 0x05, /*!< 6 time quantum */
    CAN_BTS1_7TQ                           = 0x06, /*!< 7 time quantum */
    CAN_BTS1_8TQ                           = 0x07, /*!< 8 time quantum */
    CAN_BTS1_9TQ                           = 0x08, /*!< 9 time quantum */
    CAN_BTS1_10TQ                          = 0x09, /*!< 10 time quantum */
    CAN_BTS1_11TQ                          = 0x0A, /*!< 11 time quantum */
    CAN_BTS1_12TQ                          = 0x0B, /*!< 12 time quantum */
    CAN_BTS1_13TQ                          = 0x0C, /*!< 13 time quantum */
    CAN_BTS1_14TQ                          = 0x0D, /*!< 14 time quantum */
    CAN_BTS1_15TQ                          = 0x0E, /*!< 15 time quantum */
    CAN_BTS1_16TQ                          = 0x0F  /*!< 16 time quantum */
} CAN_bts1_Type;

/**
  * @brief  can bit time segment 2
  */
typedef enum {
    CAN_BTS2_1TQ                           = 0x00, /*!< 1 time quantum */
    CAN_BTS2_2TQ                           = 0x01, /*!< 2 time quantum */
    CAN_BTS2_3TQ                           = 0x02, /*!< 3 time quantum */
    CAN_BTS2_4TQ                           = 0x03, /*!< 4 time quantum */
    CAN_BTS2_5TQ                           = 0x04, /*!< 5 time quantum */
    CAN_BTS2_6TQ                           = 0x05, /*!< 6 time quantum */
    CAN_BTS2_7TQ                           = 0x06, /*!< 7 time quantum */
    CAN_BTS2_8TQ                           = 0x07  /*!< 8 time quantum */
} CAN_bts2_Type;

/**
  * @brief  can identifier type
  */
typedef enum {
    CAN_ID_STANDARD                        = 0x00, /*!< standard Id */
    CAN_ID_EXTENDED                        = 0x01  /*!< extended Id */
} CAN_Identifier_Type;

/**
  * @brief  can transmission frame type
  */
typedef enum {
    CAN_TFT_DATA                           = 0x00, /*!< data frame */
    CAN_TFT_REMOTE                         = 0x01  /*!< remote frame */
} CAN_trans_Frame_Type;

/**
  * @brief  can tx mailboxes
  */
typedef enum {
    CAN_TX_MAILBOX0                        = 0x00, /*!< can tx mailbox 0 */
    CAN_TX_MAILBOX1                        = 0x01, /*!< can tx mailbox 1 */
    CAN_TX_MAILBOX2                        = 0x02  /*!< can tx mailbox 2 */
} CAN_TX_mailbox_Num_Type;

/**
  * @brief  can receive fifo
  */
typedef enum {
    CAN_RX_FIFO0                           = 0x00, /*!< can fifo 0 used to receive */
    CAN_RX_FIFO1                           = 0x01  /*!< can fifo 1 used to receive */
} CAN_RX_FIFO_Num_Type;

/**
  * @brief  can transmit status
  */
typedef enum {
    CAN_TX_Status_FAILED                   = 0x00, /*!< can transmission failed */
    CAN_TX_Status_SUCCESSFUL               = 0x01, /*!< can transmission successful */
    CAN_TX_Status_PENDING                  = 0x02, /*!< can transmission pending */
    CAN_TX_Status_NO_EMPTY                 = 0x04  /*!< can transmission no empty mailbox */
} CAN_Transmit_Status_Type;

/**
  * @brief  can enter doze mode status
  */
typedef enum {
    CAN_Enter_Doze_FAILED                  = 0x00, /*!< can enter the doze mode failed */
    CAN_Enter_Doze_SUCCESSFUL              = 0x01  /*!< can enter the doze mode successful */
} CAN_Enter_Doze_Status_Type;

/**
  * @brief  can quit doze mode status
  */
typedef enum {
    CAN_QUIT_Doze_FAILED                   = 0x00, /*!< can quit doze mode failed */
    CAN_QUIT_Doze_SUCCESSFUL               = 0x01  /*!< can quit doze mode successful */
} CAN_quit_Doze_Status_Type;

/**
  * @brief  can message discarding rule select when overflow
  */
typedef enum {
    CAN_DISCARDING_FIRST_ReceiveD          = 0x00, /*!< can discarding the first received message */
    CAN_DISCARDING_LAST_ReceiveD           = 0x01  /*!< can discarding the last received message */
} CAN_msg_discarding_rule_Type;

/**
  * @brief  can multiple message sending sequence rule
  */
typedef enum {
    CAN_SendING_BY_ID                      = 0x00, /*!< can sending the minimum id message first*/
    CAN_SendING_BY_REQUEST                 = 0x01  /*!< can sending the first request message first */
} CAN_msg_Sending_rule_Type;

/**
  * @brief  can error type record
  */
typedef enum {
    CAN_ERRORRECORD_NOERR                  = 0x00, /*!< no error */
    CAN_ERRORRECORD_STUFFERR               = 0x01, /*!< stuff error */
    CAN_ERRORRECORD_FORMERR                = 0x02, /*!< form error */
    CAN_ERRORRECORD_ACKERR                 = 0x03, /*!< acknowledgment error */
    CAN_ERRORRECORD_BITRECESSIVEERR        = 0x04, /*!< bit recessive error */
    CAN_ERRORRECORD_BITDOMINANTERR         = 0x05, /*!< bit dominant error */
    CAN_ERRORRECORD_CRCERR                 = 0x06, /*!< crc error */
    CAN_ERRORRECORD_SOFTWARESETERR         = 0x07  /*!< software set error */
} CAN_Error_Record_Type;

/**
  * @brief  can init structure definition
  */
typedef struct {
    CAN_Mode_Type mode_Selection;          /*!< specifies the can mode.*/

    confirm_state ttc_Enable;              /*!< time triggered communication mode enable */

    confirm_state aebo_Enable;             /*!< automatic exit bus-off enable */

    confirm_state aed_Enable;              /*!< automatic exit doze mode enable */

    confirm_state prsf_Enable;             /*!< prohibit retransmission when sending fails enable */

    CAN_msg_discarding_rule_Type mdrsel_Selection; /*!< message discarding rule select when overflow */

    CAN_msg_Sending_rule_Type mmssr_Selection;     /*!< multiple message sending sequence rule */

} CAN_Base_Type;

/**
  * @brief  can baudrate structure definition
  */
typedef struct {
    uint16_t baudrate_div;                  /*!< baudrate division,该参数可以是 0x001~0x1000.*/

    CAN_rsaw_Type rsaw_size;                /*!< resynchronization adjust width */

    CAN_bts1_Type bts1_size;                /*!< bit time segment 1 */

    CAN_bts2_Type bts2_size;                /*!< bit time segment 2 */

} CAN_Baudrate_Type;

/**
  * @brief  can filter init structure definition
  */
typedef struct {
    confirm_state filter_activate_Enable;  /*!< enable or disable the filter activate.*/

    CAN_Filter_Mode_Type filter_Mode;      /*!< config the filter mode mask or list.*/

    CAN_Filter_FIFO_Type filter_FIFO;      /*!< config the fifo which will be assigned to the filter. */

    uint8_t filter_Number;                 /*!< config the filter number, parameter ranges from 0 to 13. */

    CAN_Filter_Bit_Width_Type filter_bit;  /*!< config the filter bit width 16bit or 32bit.*/

    uint16_t filter_ID_high;               /*!< config the filter identification, for 32-bit configuration
                                              it's high 16 bits, for 16-bit configuration it's first. */

    uint16_t filter_ID_low;                /*!< config the filter identification, for 32-bit configuration
                                              it's low 16 bits, for 16-bit configuration it's second. */

    uint16_t filter_Mask_high;             /*!< config the filter mask or identification, according to the filtering mode,
                                              for 32-bit configuration it's high 16 bits, for 16-bit configuration it's first. */

    uint16_t filter_Mask_low;              /*!< config the filter mask or identification, according to the filtering mode,
                                              for 32-bit configuration it's low 16 bits, for 16-bit configuration it's second. */
} CAN_Filter_Init_Type;

/**
  * @brief  can tx message structure definition
  */
typedef struct {
    uint32_t standard_id;                  /*!< specifies the 11 bits standard identifier.
                                              该参数可以是 a value between 0 to 0x7FF. */

    uint32_t extended_id;                  /*!< specifies the 29 bits extended identifier.
                                              该参数可以是 a value between 0 to 0x1FFFFFFF. */

    CAN_Identifier_Type id_Type;           /*!< specifies identifier type for the transmit message.*/

    CAN_trans_Frame_Type frame_Type;       /*!< specifies frame type for the transmit message.*/

    uint8_t dlc;                           /*!< specifies frame data length that will be transmitted.
                                              该参数可以是 a value between 0 to 8 */

    uint8_t data[8];                       /*!< contains the transmit data. it ranges from 0 to 0xFF. */

} CAN_TX_Message_Type;

/**
  * @brief  can rx message structure definition
  */
typedef struct {
    uint32_t standard_id;                /*!< specifies the 11 bits standard identifier
                                              该参数可以是 a value between 0 to 0x7FF. */

    uint32_t extended_id;                /*!< specifies the 29 bits extended identifier.
                                              该参数可以是 a value between 0 to 0x1FFFFFFF. */

    CAN_Identifier_Type id_Type;         /*!< specifies identifier type for the receive message.*/

    CAN_trans_Frame_Type frame_Type;     /*!< specifies frame type for the receive message.*/

    uint8_t dlc;                         /*!< specifies the frame data length that will be received.
                                              该参数可以是 a value between 0 to 8 */

    uint8_t data[8];                     /*!< contains the receive data. it ranges from 0 to 0xFF.*/

    uint8_t filter_index;                /*!< specifies the message stored in which filter
                                              该参数可以是 a value between 0 to 0xFF */
} CAN_RX_Message_Type;

/**
  * @brief can 控制器区域网络 tx mailbox
  */
typedef struct {
    /**
      * @brief can tmi register
      */
    union {
        __IO uint32_t tmi;
        struct {
            __IO uint32_t tmsr                 : 1; /* [0] */
            __IO uint32_t tmfrsel              : 1; /* [1] */
            __IO uint32_t tmidsel              : 1; /* [2] */
            __IO uint32_t tmeid                : 18;/* [20:3] */
            __IO uint32_t tmsid                : 11;/* [31:21] */
        } tmi_bit;
    };

    /**
      * @brief can tmc register
      */
    union {
        __IO uint32_t tmc;
        struct {
            __IO uint32_t tmdtbl               : 4; /* [3:0] */
            __IO uint32_t reserved1            : 4; /* [7:4] */
            __IO uint32_t tmtsten              : 1; /* [8] */
            __IO uint32_t reserved2            : 7; /* [15:9] */
            __IO uint32_t tmts                 : 16;/* [31:16] */
        } tmc_bit;
    };

    /**
      * @brief can tmdtl register
      */
    union {
        __IO uint32_t tmdtl;
        struct {
            __IO uint32_t tmdt0                : 8; /* [7:0] */
            __IO uint32_t tmdt1                : 8; /* [15:8] */
            __IO uint32_t tmdt2                : 8; /* [23:16] */
            __IO uint32_t tmdt3                : 8; /* [31:24] */
        } tmdtl_bit;
    };

    /**
      * @brief can tmdth register
      */
    union {
        __IO uint32_t tmdth;
        struct {
            __IO uint32_t tmdt4                : 8; /* [7:0] */
            __IO uint32_t tmdt5                : 8; /* [15:8] */
            __IO uint32_t tmdt6                : 8; /* [23:16] */
            __IO uint32_t tmdt7                : 8; /* [31:24] */
        } tmdth_bit;
    };
} CAN_TX_mailbox_Type;

/**
  * @brief can 控制器区域网络 fifo mailbox
  */
typedef struct {
    /**
      * @brief can rfi register
      */
    union {
        __IO uint32_t rfi;
        struct {
            __IO uint32_t reserved1            : 1; /* [0] */
            __IO uint32_t rffri                : 1; /* [1] */
            __IO uint32_t rfidi                : 1; /* [2] */
            __IO uint32_t rfeid                : 18;/* [20:3] */
            __IO uint32_t rfsid                : 11;/* [31:21] */
        } rfi_bit;
    };

    /**
      * @brief can rfc register
      */
    union {
        __IO uint32_t rfc;
        struct {
            __IO uint32_t rfdtl                : 4; /* [3:0] */
            __IO uint32_t reserved1            : 4; /* [7:4] */
            __IO uint32_t rffmn                : 8; /* [15:8] */
            __IO uint32_t rfts                 : 16;/* [31:16] */
        } rfc_bit;
    };

    /**
      * @brief can rfdtl register
      */
    union {
        __IO uint32_t rfdtl;
        struct {
            __IO uint32_t rfdt0                : 8; /* [7:0] */
            __IO uint32_t rfdt1                : 8; /* [15:8] */
            __IO uint32_t rfdt2                : 8; /* [23:16] */
            __IO uint32_t rfdt3                : 8; /* [31:24] */
        } rfdtl_bit;
    };

    /**
      * @brief can rfdth register
      */
    union {
        __IO uint32_t rfdth;
        struct {
            __IO uint32_t rfdt4                : 8; /* [7:0] */
            __IO uint32_t rfdt5                : 8; /* [15:8] */
            __IO uint32_t rfdt6                : 8; /* [23:16] */
            __IO uint32_t rfdt7                : 8; /* [31:24] */
        } rfdth_bit;
    };
} CAN_FIFO_mailbox_Type;

/**
  * @brief can 控制器区域网络 filter bit register
  */
typedef struct {
    __IO uint32_t ffdb1;
    __IO uint32_t ffdb2;
} CAN_Filter_Register_Type;

/**
  * @brief type define can register all
  */
typedef struct {

    /**
      * @brief can mctrl register, offset:0x00
      */
    union {
        __IO uint32_t mctrl;
        struct {
            __IO uint32_t fzen                 : 1; /* [0] */
            __IO uint32_t dzen                 : 1; /* [1] */
            __IO uint32_t mmssr                : 1; /* [2] */
            __IO uint32_t mdrsel               : 1; /* [3] */
            __IO uint32_t prsfen               : 1; /* [4] */
            __IO uint32_t aeden                : 1; /* [5] */
            __IO uint32_t aeboen               : 1; /* [6] */
            __IO uint32_t ttcen                : 1; /* [7] */
            __IO uint32_t reserved1            : 7; /* [14:8] */
            __IO uint32_t sprst                : 1; /* [15] */
            __IO uint32_t ptd                  : 1; /* [16] */
            __IO uint32_t reserved2            : 15;/*[31:17] */
        } mctrl_bit;
    };

    /**
      * @brief can msts register, offset:0x04
      */
    union {
        __IO uint32_t msts;
        struct {
            __IO uint32_t fzc                  : 1; /* [0] */
            __IO uint32_t dzc                  : 1; /* [1] */
            __IO uint32_t eoif                 : 1; /* [2] */
            __IO uint32_t qdzif                : 1; /* [3] */
            __IO uint32_t edzif                : 1; /* [4] */
            __IO uint32_t reserved1            : 3; /* [7:5] */
            __IO uint32_t cuss                 : 1; /* [8] */
            __IO uint32_t curs                 : 1; /* [9] */
            __IO uint32_t lsamprx              : 1; /* [10] */
            __IO uint32_t realrx               : 1; /* [11] */
            __IO uint32_t reserved2            : 20;/*[31:12] */
        } msts_bit;
    };

    /**
      * @brief can tsts register, offset:0x08
      */
    union {
        __IO uint32_t tsts;
        struct {
            __IO uint32_t tm0tcf               : 1; /* [0] */
            __IO uint32_t tm0tsf               : 1; /* [1] */
            __IO uint32_t tm0alf               : 1; /* [2] */
            __IO uint32_t tm0tef               : 1; /* [3] */
            __IO uint32_t reserved1            : 3; /* [6:4] */
            __IO uint32_t tm0ct                : 1; /* [7] */
            __IO uint32_t tm1tcf               : 1; /* [8] */
            __IO uint32_t tm1tsf               : 1; /* [9] */
            __IO uint32_t tm1alf               : 1; /* [10] */
            __IO uint32_t tm1tef               : 1; /* [11] */
            __IO uint32_t reserved2            : 3; /* [14:12] */
            __IO uint32_t tm1ct                : 1; /* [15] */
            __IO uint32_t tm2tcf               : 1; /* [16] */
            __IO uint32_t tm2tsf               : 1; /* [17] */
            __IO uint32_t tm2alf               : 1; /* [18] */
            __IO uint32_t tm2tef               : 1; /* [19] */
            __IO uint32_t reserved3            : 3; /* [22:20] */
            __IO uint32_t tm2ct                : 1; /* [23] */
            __IO uint32_t tmnr                 : 2; /* [25:24] */
            __IO uint32_t tm0ef                : 1; /* [26] */
            __IO uint32_t tm1ef                : 1; /* [27] */
            __IO uint32_t tm2ef                : 1; /* [28] */
            __IO uint32_t tm0lpf               : 1; /* [29] */
            __IO uint32_t tm1lpf               : 1; /* [30] */
            __IO uint32_t tm2lpf               : 1; /* [31] */
        } tsts_bit;
    };

    /**
      * @brief can rf0 register, offset:0x0C
      */
    union {
        __IO uint32_t rf0;
        struct {
            __IO uint32_t rf0mn                : 2; /* [1:0] */
            __IO uint32_t reserved1            : 1; /* [2] */
            __IO uint32_t rf0ff                : 1; /* [3] */
            __IO uint32_t rf0of                : 1; /* [4] */
            __IO uint32_t rf0r                 : 1; /* [5] */
            __IO uint32_t reserved2            : 26;/* [31:6] */
        } rf0_bit;
    };

    /**
      * @brief can rf1 register, offset:0x10
      */
    union {
        __IO uint32_t rf1;
        struct {
            __IO uint32_t rf1mn                : 2; /* [1:0] */
            __IO uint32_t reserved1            : 1; /* [2] */
            __IO uint32_t rf1ff                : 1; /* [3] */
            __IO uint32_t rf1of                : 1; /* [4] */
            __IO uint32_t rf1r                 : 1; /* [5] */
            __IO uint32_t reserved2            : 26;/* [31:6] */
        } rf1_bit;
    };

    /**
      * @brief can inten register, offset:0x14
      */
    union {
        __IO uint32_t inten;
        struct {
            __IO uint32_t tcien               : 1; /* [0] */
            __IO uint32_t rf0mien              : 1; /* [1] */
            __IO uint32_t rf0fien              : 1; /* [2] */
            __IO uint32_t rf0oien              : 1; /* [3] */
            __IO uint32_t rf1mien              : 1; /* [4] */
            __IO uint32_t rf1fien              : 1; /* [5] */
            __IO uint32_t rf1oien              : 1; /* [6] */
            __IO uint32_t reserved1            : 1; /* [7] */
            __IO uint32_t eaien                : 1; /* [8] */
            __IO uint32_t epien                : 1; /* [9] */
            __IO uint32_t boien                : 1; /* [10] */
            __IO uint32_t etrien               : 1; /* [11] */
            __IO uint32_t reserved2            : 3; /* [14:12] */
            __IO uint32_t eoien                : 1; /* [15] */
            __IO uint32_t qdzien               : 1; /* [16] */
            __IO uint32_t edzien               : 1; /* [17] */
            __IO uint32_t reserved3            : 14;/* [31:18] */
        } inten_bit;
    };

    /**
      * @brief can ests register, offset:0x18
      */
    union {
        __IO uint32_t ests;
        struct {
            __IO uint32_t eaf                  : 1; /* [0] */
            __IO uint32_t epf                  : 1; /* [1] */
            __IO uint32_t bof                  : 1; /* [2] */
            __IO uint32_t reserved1            : 1; /* [3] */
            __IO uint32_t etr                  : 3; /* [6:4] */
            __IO uint32_t reserved2            : 9; /* [15:7] */
            __IO uint32_t tec                  : 8; /* [23:16] */
            __IO uint32_t rec                  : 8; /* [31:24] */
        } ests_bit;
    };

    /**
      * @brief can btmg register, offset:0x1C
      */
    union {
        __IO uint32_t btmg;
        struct {
            __IO uint32_t brdiv                : 12;/* [11:0] */
            __IO uint32_t reserved1            : 4; /* [15:12] */
            __IO uint32_t bts1                 : 4; /* [19:16] */
            __IO uint32_t bts2                 : 3; /* [22:20] */
            __IO uint32_t reserved2            : 1; /* [23] */
            __IO uint32_t rsaw                 : 2; /* [25:24] */
            __IO uint32_t reserved3            : 4; /* [29:26] */
            __IO uint32_t lben                 : 1; /* [30] */
            __IO uint32_t loen                 : 1; /* [31] */
        } btmg_bit;
    };

    /**
      * @brief can reserved register, offset:0x20~0x17C
      */
    __IO uint32_t reserved1[88];

    /**
      * @brief can 控制器区域网络 tx mailbox register, offset:0x180~0x1AC
      */
    CAN_TX_mailbox_Type tx_mailbox[3];

    /**
      * @brief can 控制器区域网络 fifo mailbox register, offset:0x1B0~0x1CC
      */
    CAN_FIFO_mailbox_Type fifo_mailbox[2];

    /**
      * @brief can reserved register, offset:0x1D0~0x1FC
      */
    __IO uint32_t reserved2[12];

    /**
      * @brief can fctrl register, offset:0x200
      */
    union {
        __IO uint32_t fctrl;
        struct {
            __IO uint32_t fcs                  : 1; /* [0] */
            __IO uint32_t reserved1            : 31;/* [31:1] */
        } fctrl_bit;
    };

    /**
      * @brief can fmcfg register, offset:0x204
      */
    union {
        __IO uint32_t fmcfg;
        struct {
            __IO uint32_t fmsel0               : 1; /* [0] */
            __IO uint32_t fmsel1               : 1; /* [1] */
            __IO uint32_t fmsel2               : 1; /* [2] */
            __IO uint32_t fmsel3               : 1; /* [3] */
            __IO uint32_t fmsel4               : 1; /* [4] */
            __IO uint32_t fmsel5               : 1; /* [5] */
            __IO uint32_t fmsel6               : 1; /* [6] */
            __IO uint32_t fmsel7               : 1; /* [7] */
            __IO uint32_t fmsel8               : 1; /* [8] */
            __IO uint32_t fmsel9               : 1; /* [9] */
            __IO uint32_t fmsel10              : 1; /* [10] */
            __IO uint32_t fmsel11              : 1; /* [11] */
            __IO uint32_t fmsel12              : 1; /* [12] */
            __IO uint32_t fmsel13              : 1; /* [13] */
            __IO uint32_t fmsel14              : 1; /* [14] */
            __IO uint32_t fmsel15              : 1; /* [15] */
            __IO uint32_t fmsel16              : 1; /* [16] */
            __IO uint32_t fmsel17              : 1; /* [17] */
            __IO uint32_t fmsel18              : 1; /* [18] */
            __IO uint32_t fmsel19              : 1; /* [19] */
            __IO uint32_t fmsel20              : 1; /* [20] */
            __IO uint32_t fmsel21              : 1; /* [21] */
            __IO uint32_t fmsel22              : 1; /* [22] */
            __IO uint32_t fmsel23              : 1; /* [23] */
            __IO uint32_t fmsel24              : 1; /* [24] */
            __IO uint32_t fmsel25              : 1; /* [25] */
            __IO uint32_t fmsel26              : 1; /* [26] */
            __IO uint32_t fmsel27              : 1; /* [27] */
            __IO uint32_t reserved1            : 4;/* [31:28] */
        } fmcfg_bit;
    };

    /**
      * @brief can reserved register, offset:0x208
      */
    __IO uint32_t reserved3;

    /**
      * @brief can fbwcfg register, offset:0x20C
      */
    union {
        __IO uint32_t fbwcfg;
        struct {
            __IO uint32_t fbwsel0              : 1; /* [0] */
            __IO uint32_t fbwsel1              : 1; /* [1] */
            __IO uint32_t fbwsel2              : 1; /* [2] */
            __IO uint32_t fbwsel3              : 1; /* [3] */
            __IO uint32_t fbwsel4              : 1; /* [4] */
            __IO uint32_t fbwsel5              : 1; /* [5] */
            __IO uint32_t fbwsel6              : 1; /* [6] */
            __IO uint32_t fbwsel7              : 1; /* [7] */
            __IO uint32_t fbwsel8              : 1; /* [8] */
            __IO uint32_t fbwsel9              : 1; /* [9] */
            __IO uint32_t fbwsel10             : 1; /* [10] */
            __IO uint32_t fbwsel11             : 1; /* [11] */
            __IO uint32_t fbwsel12             : 1; /* [12] */
            __IO uint32_t fbwsel13             : 1; /* [13] */
            __IO uint32_t fbwsel14             : 1; /* [14] */
            __IO uint32_t fbwsel15             : 1; /* [15] */
            __IO uint32_t fbwsel16             : 1; /* [16] */
            __IO uint32_t fbwsel17             : 1; /* [17] */
            __IO uint32_t fbwsel18             : 1; /* [18] */
            __IO uint32_t fbwsel19             : 1; /* [19] */
            __IO uint32_t fbwsel20             : 1; /* [20] */
            __IO uint32_t fbwsel21             : 1; /* [21] */
            __IO uint32_t fbwsel22             : 1; /* [22] */
            __IO uint32_t fbwsel23             : 1; /* [23] */
            __IO uint32_t fbwsel24             : 1; /* [24] */
            __IO uint32_t fbwsel25             : 1; /* [25] */
            __IO uint32_t fbwsel26             : 1; /* [26] */
            __IO uint32_t fbwsel27             : 1; /* [27] */
            __IO uint32_t reserved1            : 4;/* [31:28] */
        } fbwcfg_bit;
    };

    /**
      * @brief can reserved register, offset:0x210
      */
    __IO uint32_t reserved4;

    /**
      * @brief can frf register, offset:0x214
      */
    union {
        __IO uint32_t frf;
        struct {
            __IO uint32_t frfsel0              : 1; /* [0] */
            __IO uint32_t frfsel1              : 1; /* [1] */
            __IO uint32_t frfsel2              : 1; /* [2] */
            __IO uint32_t frfsel3              : 1; /* [3] */
            __IO uint32_t frfsel4              : 1; /* [4] */
            __IO uint32_t frfsel5              : 1; /* [5] */
            __IO uint32_t frfsel6              : 1; /* [6] */
            __IO uint32_t frfsel7              : 1; /* [7] */
            __IO uint32_t frfsel8              : 1; /* [8] */
            __IO uint32_t frfsel9              : 1; /* [9] */
            __IO uint32_t frfsel10             : 1; /* [10] */
            __IO uint32_t frfsel11             : 1; /* [11] */
            __IO uint32_t frfsel12             : 1; /* [12] */
            __IO uint32_t frfsel13             : 1; /* [13] */
            __IO uint32_t frfsel14             : 1; /* [14] */
            __IO uint32_t frfsel15             : 1; /* [15] */
            __IO uint32_t frfsel16             : 1; /* [16] */
            __IO uint32_t frfsel17             : 1; /* [17] */
            __IO uint32_t frfsel18             : 1; /* [18] */
            __IO uint32_t frfsel19             : 1; /* [19] */
            __IO uint32_t frfsel20             : 1; /* [20] */
            __IO uint32_t frfsel21             : 1; /* [21] */
            __IO uint32_t frfsel22             : 1; /* [22] */
            __IO uint32_t frfsel23             : 1; /* [23] */
            __IO uint32_t frfsel24             : 1; /* [24] */
            __IO uint32_t frfsel25             : 1; /* [25] */
            __IO uint32_t frfsel26             : 1; /* [26] */
            __IO uint32_t frfsel27             : 1; /* [27] */
            __IO uint32_t reserved1            : 4;/* [31:28] */
        } frf_bit;
    };

    /**
      * @brief can reserved register, offset:0x218
      */
    __IO uint32_t reserved5;

    /**
      * @brief can facfg register, offset:0x21C
      */
    union {
        __IO uint32_t facfg;
        struct {
            __IO uint32_t faen0                : 1; /* [0] */
            __IO uint32_t faen1                : 1; /* [1] */
            __IO uint32_t faen2                : 1; /* [2] */
            __IO uint32_t faen3                : 1; /* [3] */
            __IO uint32_t faen4                : 1; /* [4] */
            __IO uint32_t faen5                : 1; /* [5] */
            __IO uint32_t faen6                : 1; /* [6] */
            __IO uint32_t faen7                : 1; /* [7] */
            __IO uint32_t faen8                : 1; /* [8] */
            __IO uint32_t faen9                : 1; /* [9] */
            __IO uint32_t faen10               : 1; /* [10] */
            __IO uint32_t faen11               : 1; /* [11] */
            __IO uint32_t faen12               : 1; /* [12] */
            __IO uint32_t faen13               : 1; /* [13] */
            __IO uint32_t faen14               : 1; /* [14] */
            __IO uint32_t faen15               : 1; /* [15] */
            __IO uint32_t faen16               : 1; /* [16] */
            __IO uint32_t faen17               : 1; /* [17] */
            __IO uint32_t faen18               : 1; /* [18] */
            __IO uint32_t faen19               : 1; /* [19] */
            __IO uint32_t faen20               : 1; /* [20] */
            __IO uint32_t faen21               : 1; /* [21] */
            __IO uint32_t faen22               : 1; /* [22] */
            __IO uint32_t faen23               : 1; /* [23] */
            __IO uint32_t faen24               : 1; /* [24] */
            __IO uint32_t faen25               : 1; /* [25] */
            __IO uint32_t faen26               : 1; /* [26] */
            __IO uint32_t faen27               : 1; /* [27] */
            __IO uint32_t reserved1            : 4;/* [31:28] */
        } facfg_bit;
    };

    /**
      * @brief can reserved register, offset:0x220~0x23C
      */
    __IO uint32_t reserved6[8];

    /**
      * @brief can ffb register, offset:0x240~0x2AC
      */
    CAN_Filter_Register_Type ffb[28];
} CAN_Type;

/**
  * @}
  */

#define CAN1                             ((CAN_Type *) CAN1_BASE)
#define CAN2                             ((CAN_Type *) CAN2_BASE)

/** @defgroup CAN_exported_functions
  * @{
  */

void CAN_Reset(CAN_Type* CAN_x);
void CAN_Baudrate_Default_Para_Init(CAN_Baudrate_Type* CAN_Baudrate_struct);
error_status CAN_Baudrate_Set(CAN_Type* CAN_x, CAN_Baudrate_Type* CAN_Baudrate_struct);
void CAN_Default_Para_Init(CAN_Base_Type* CAN_Base_struct);
error_status CAN_Base_Init(CAN_Type* CAN_x, CAN_Base_Type* CAN_Base_struct);
void CAN_Filter_Default_Para_Init(CAN_Filter_Init_Type* CAN_Filter_Init_struct);
void CAN_Filter_Init(CAN_Type* CAN_x, CAN_Filter_Init_Type* CAN_Filter_Init_struct);
void CAN_DEBUG_Transmission_Prohibit(CAN_Type* CAN_x, confirm_state new_state);
void CAN_ttc_Mode_Enable(CAN_Type* CAN_x, confirm_state new_state);
uint8_t CAN_Message_Transmit(CAN_Type* CAN_x, CAN_TX_Message_Type* tx_Message_struct);
CAN_Transmit_Status_Type CAN_Transmit_Status_Get(CAN_Type* CAN_x, CAN_TX_mailbox_Num_Type transmit_mailbox);
void CAN_Transmit_Cancel(CAN_Type* CAN_x, CAN_TX_mailbox_Num_Type transmit_mailbox);
void CAN_Message_Receive(CAN_Type* CAN_x, CAN_RX_FIFO_Num_Type fifo_Number, CAN_RX_Message_Type* rx_Message_struct);
void CAN_Receive_FIFO_release(CAN_Type* CAN_x, CAN_RX_FIFO_Num_Type fifo_Number);
uint8_t CAN_Receive_Message_pending_Get(CAN_Type* CAN_x, CAN_RX_FIFO_Num_Type fifo_Number);
error_status CAN_operating_Mode_Set(CAN_Type* CAN_x, CAN_operating_Mode_Type CAN_operating_Mode);
CAN_Enter_Doze_Status_Type CAN_Doze_Mode_Enter(CAN_Type* CAN_x);
CAN_quit_Doze_Status_Type CAN_Doze_Mode_Exit(CAN_Type* CAN_x);
CAN_Error_Record_Type CAN_Error_Type_Record_Get(CAN_Type* CAN_x);
uint8_t CAN_Receive_Error_Counter_Get(CAN_Type* CAN_x);
uint8_t CAN_Transmit_Error_Counter_Get(CAN_Type* CAN_x);
void CAN_Interrupt_Enable(CAN_Type* CAN_x, uint32_t CAN_int, confirm_state new_state);
flag_status CAN_Flag_Get(CAN_Type* CAN_x, uint32_t CAN_flag);
void CAN_Flag_Clear(CAN_Type* CAN_x, uint32_t CAN_flag);

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif
