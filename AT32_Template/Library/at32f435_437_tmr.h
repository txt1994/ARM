/**
  **************************************************************************
  * @file     at32f435_437_tmr.h
  * @brief    at32f435_437 tmr header file
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
#ifndef __AT32F435_437_TMR_H
#define __AT32F435_437_TMR_H

#ifdef __cplusplus
extern "C" {
#endif


/* includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/** @addtogroup AT32F435_437_periph_driver
  * @{
  */

/** @addtogroup TMR
  * @{
  */

/** @defgroup TMR_flags_definition
  * @brief tmr flag
  * @{
  */

#define TMR_OVF_FLAG                     ((uint32_t)0x000001) /*!< tmr flag overflow */
#define TMR_C1_FLAG                      ((uint32_t)0x000002) /*!< tmr flag channel 1 */
#define TMR_C2_FLAG                      ((uint32_t)0x000004) /*!< tmr flag channel 2 */
#define TMR_C3_FLAG                      ((uint32_t)0x000008) /*!< tmr flag channel 3 */
#define TMR_C4_FLAG                      ((uint32_t)0x000010) /*!< tmr flag channel 4 */
#define TMR_C5_FLAG                      ((uint32_t)0x010000) /*!< tmr flag channel 5 */
#define TMR_HALL_FLAG                    ((uint32_t)0x000020) /*!< tmr flag hall */
#define TMR_Trigger_FLAG                 ((uint32_t)0x000040) /*!< tmr flag trigger */
#define TMR_BRK_FLAG                     ((uint32_t)0x000080) /*!< tmr flag brake */
#define TMR_C1_RECAPTURE_FLAG            ((uint32_t)0x000200) /*!< tmr flag channel 1 recapture */
#define TMR_C2_RECAPTURE_FLAG            ((uint32_t)0x000400) /*!< tmr flag channel 2 recapture */
#define TMR_C3_RECAPTURE_FLAG            ((uint32_t)0x000800) /*!< tmr flag channel 3 recapture */
#define TMR_C4_RECAPTURE_FLAG            ((uint32_t)0x001000) /*!< tmr flag channel 4 recapture */

/**
  * @}
  */

/** @defgroup TMR_Interrupt_Select_Type_definition
  * @brief tmr interrupt select type
  * @{
  */

#define TMR_OVF_INT                      ((uint32_t)0x000001) /*!< tmr interrupt overflow */
#define TMR_C1_INT                       ((uint32_t)0x000002) /*!< tmr interrupt channel 1 */
#define TMR_C2_INT                       ((uint32_t)0x000004) /*!< tmr interrupt channel 2 */
#define TMR_C3_INT                       ((uint32_t)0x000008) /*!< tmr interrupt channel 3 */
#define TMR_C4_INT                       ((uint32_t)0x000010) /*!< tmr interrupt channel 4 */
#define TMR_HALL_INT                     ((uint32_t)0x000020) /*!< tmr interrupt hall */
#define TMR_Trigger_INT                  ((uint32_t)0x000040) /*!< tmr interrupt trigger */
#define TMR_BRK_INT                      ((uint32_t)0x000080) /*!< tmr interrupt brake */

/**
  * @}
  */

/** @defgroup TMR_exported_Types
  * @{
  */

/**
  * @brief tmr clock division type
  */
typedef enum {
    TMR_Clock_DIV1                         = 0x00, /*!< tmr clock division 1 */
    TMR_Clock_DIV2                         = 0x01, /*!< tmr clock division 2 */
    TMR_Clock_DIV4                         = 0x02  /*!< tmr clock division 4 */
} tmr_Clock_division_Type;

/**
  * @brief tmr counter mode type
  */
typedef enum {
    TMR_COUNT_UP                           = 0x00, /*!< tmr counter mode up */
    TMR_COUNT_DOWN                         = 0x01, /*!< tmr counter mode down */
    TMR_COUNT_TWO_WAY_1                    = 0x02, /*!< tmr counter mode two way 1 */
    TMR_COUNT_TWO_WAY_2                    = 0x04, /*!< tmr counter mode two way 2 */
    TMR_COUNT_TWO_WAY_3                    = 0x06  /*!< tmr counter mode two way 3 */
} tmr_count_Mode_Type;

/**
  * @brief tmr primary mode select type
  */
typedef enum {
    TMR_PRIMARY_SEL_Reset                  = 0x00, /*!< tmr primary mode select reset */
    TMR_PRIMARY_SEL_Enable                 = 0x01, /*!< tmr primary mode select enable */
    TMR_PRIMARY_SEL_OVERFLOW               = 0x02, /*!< tmr primary mode select overflow */
    TMR_PRIMARY_SEL_COMPARE                = 0x03, /*!< tmr primary mode select compare */
    TMR_PRIMARY_SEL_C1ORAW                 = 0x04, /*!< tmr primary mode select c1oraw */
    TMR_PRIMARY_SEL_C2ORAW                 = 0x05, /*!< tmr primary mode select c2oraw */
    TMR_PRIMARY_SEL_C3ORAW                 = 0x06, /*!< tmr primary mode select c3oraw */
    TMR_PRIMARY_SEL_C4ORAW                 = 0x07  /*!< tmr primary mode select c4oraw */
} tmr_primary_Select_Type;

/**
  * @brief tmr subordinate mode input select type
  */
typedef enum {
    TMR_Sub_Input_SEL_IS0                  = 0x00, /*!< subordinate mode input select is0 */
    TMR_Sub_Input_SEL_IS1                  = 0x01, /*!< subordinate mode input select is1 */
    TMR_Sub_Input_SEL_IS2                  = 0x02, /*!< subordinate mode input select is2 */
    TMR_Sub_Input_SEL_IS3                  = 0x03, /*!< subordinate mode input select is3 */
    TMR_Sub_Input_SEL_C1INC                = 0x04, /*!< subordinate mode input select c1inc */
    TMR_Sub_Input_SEL_C1DF1                = 0x05, /*!< subordinate mode input select c1df1 */
    TMR_Sub_Input_SEL_C2DF2                = 0x06, /*!< subordinate mode input select c2df2 */
    TMR_Sub_Input_SEL_EXTIN                = 0x07  /*!< subordinate mode input select extin */
} sub_tmr_Input_sel_Type;

/**
  * @brief tmr subordinate mode select type
  */
typedef enum {
    TMR_Sub_Mode_DIABLE                    = 0x00, /*!< subordinate mode disable */
    TMR_Sub_ENCODER_Mode_A                 = 0x01, /*!< subordinate mode select encoder mode a */
    TMR_Sub_ENCODER_Mode_B                 = 0x02, /*!< subordinate mode select encoder mode b */
    TMR_Sub_ENCODER_Mode_C                 = 0x03, /*!< subordinate mode select encoder mode c */
    TMR_Sub_Reset_MODE                     = 0x04, /*!< subordinate mode select reset */
    TMR_Sub_HANG_MODE                      = 0x05, /*!< subordinate mode select hang */
    TMR_Sub_Trigger_MODE                   = 0x06, /*!< subordinate mode select trigger */
    TMR_Sub_EXTERNAL_Clock_Mode_A          = 0x07  /*!< subordinate mode external clock mode a */
} tmr_Sub_Mode_Select_Type;

/**
  * @brief tmr encoder mode type
  */
typedef enum {
    TMR_ENCODER_Mode_A                     = TMR_Sub_ENCODER_Mode_A, /*!< tmr encoder mode a */
    TMR_ENCODER_Mode_B                     = TMR_Sub_ENCODER_Mode_B, /*!< tmr encoder mode b */
    TMR_ENCODER_Mode_C                     = TMR_Sub_ENCODER_Mode_C  /*!< tmr encoder mode c */
} tmr_encoder_Mode_Type;

/**
  * @brief tmr output control mode type
  */
typedef enum {
    TMR_OutPut_Control_OFF                 = 0x00, /*!< tmr output control mode off */
    TMR_OutPut_Control_HIGH                = 0x01, /*!< tmr output control mode high */
    TMR_OutPut_Control_LOW                 = 0x02, /*!< tmr output control mode low */
    TMR_OutPut_Control_SWITCH              = 0x03, /*!< tmr output control mode switch */
    TMR_OutPut_Control_FORCE_LOW           = 0x04, /*!< tmr output control mode force low */
    TMR_OutPut_Control_FORCE_HIGH          = 0x05, /*!< tmr output control mode force high */
    TMR_OutPut_Control_PWM_Mode_A          = 0x06, /*!< tmr output control mode pwm a */
    TMR_OutPut_Control_PWM_Mode_B          = 0x07  /*!< tmr output control mode pwm b */
} tmr_OutPut_Control_Mode_Type;

/**
  * @brief tmr force output type
  */
typedef enum {
    TMR_FORCE_OutPut_HIGH                  = TMR_OutPut_Control_FORCE_HIGH, /*!< tmr force output high */
    TMR_FORCE_OutPut_LOW                   = TMR_OutPut_Control_FORCE_LOW   /*!< tmr force output low */
} tmr_force_OutPut_Type;

/**
  * @brief tmr output channel polarity type
  */
typedef enum {
    TMR_OutPut_ACTIVE_HIGH                 = 0x00, /*!< tmr output channel polarity high */
    TMR_OutPut_ACTIVE_LOW                  = 0x01  /*!< tmr output channel polarity low */
} tmr_OutPut_polarity_Type;

/**
  * @brief tmr input channel polarity type
  */
typedef enum {
    TMR_Input_RISING_EDGE                  = 0x00, /*!< tmr input channel polarity rising */
    TMR_Input_FALLING_EDGE                 = 0x01, /*!< tmr input channel polarity falling */
    TMR_Input_BOTH_EDGE                    = 0x03  /*!< tmr input channel polarity both edge */
} tmr_Input_polarity_Type;

/**
  * @brief tmr channel select type
  */
typedef enum {
    TMR_Select_Channel_1                   = 0x00, /*!< tmr channel select channel 1 */
    TMR_Select_Channel_1C                  = 0x01, /*!< tmr channel select channel 1 complementary */
    TMR_Select_Channel_2                   = 0x02, /*!< tmr channel select channel 2 */
    TMR_Select_Channel_2C                  = 0x03, /*!< tmr channel select channel 2 complementary */
    TMR_Select_Channel_3                   = 0x04, /*!< tmr channel select channel 3 */
    TMR_Select_Channel_3C                  = 0x05, /*!< tmr channel select channel 3 complementary */
    TMR_Select_Channel_4                   = 0x06, /*!< tmr channel select channel 4 */
    TMR_Select_Channel_5                   = 0x07  /*!< tmr channel select channel 5 */
} tmr_Channel_Select_Type;

/**
  * @brief tmr channel1 input connected type
  */
typedef enum {
    TMR_CHANEL1_CONNECTED_C1IRAW           = 0x00, /*!< channel1 pins is only connected to C1IRAW input */
    TMR_CHANEL1_2_3_CONNECTED_C1IRAW_XOR   = 0x01  /*!< channel1/2/3 pins are connected to C1IRAW input after xored */
} tmr_channel1_Input_connected_Type;

/**
  * @brief tmr input channel mapped type channel direction
  */
typedef enum {
    TMR_CC_Channel_MAPPED_DIRECT           = 0x01, /*!< channel is configured as input, mapped direct */
    TMR_CC_Channel_MAPPED_INDIRECT         = 0x02, /*!< channel is configured as input, mapped indirect */
    TMR_CC_Channel_MAPPED_STI              = 0x03  /*!< channel is configured as input, mapped sti */
} tmr_Input_direction_mapped_Type;

/**
  * @brief tmr input divider type
  */
typedef enum {
    TMR_Channel_Input_DIV_1                = 0x00, /*!< tmr channel input divider 1 */
    TMR_Channel_Input_DIV_2                = 0x01, /*!< tmr channel input divider 2 */
    TMR_Channel_Input_DIV_4                = 0x02, /*!< tmr channel input divider 4 */
    TMR_Channel_Input_DIV_8                = 0x03  /*!< tmr channel input divider 8 */
} tmr_Channel_Input_divider_Type;

/**
  * @brief tmr dma request source select type
  */
typedef enum {
    TMR_DMA_Request_BY_CHANNEL             = 0x00, /*!< tmr dma request source select channel */
    TMR_DMA_Request_BY_OVERFLOW            = 0x01  /*!< tmr dma request source select overflow */
} tmr_DMA_Request_Source_Type;

/**
  * @brief tmr dma request type
  */
typedef enum {
    TMR_OverFlow_DMA_REQUEST               = 0x00000100, /*!< tmr dma request select overflow */
    TMR_C1_DMA_REQUEST                     = 0x00000200, /*!< tmr dma request select channel 1 */
    TMR_C2_DMA_REQUEST                     = 0x00000400, /*!< tmr dma request select channel 2 */
    TMR_C3_DMA_REQUEST                     = 0x00000800, /*!< tmr dma request select channel 3 */
    TMR_C4_DMA_REQUEST                     = 0x00001000, /*!< tmr dma request select channel 4 */
    TMR_HALL_DMA_REQUEST                   = 0x00002000, /*!< tmr dma request select hall */
    TMR_Trigger_DMA_REQUEST                = 0x00004000  /*!< tmr dma request select trigger */
} tmr_DMA_Request_Type;

/**
  * @brief tmr event triggered by software type
  */
typedef enum {
    TMR_OverFlow_SWTRIG                    = 0x00000001, /*!< tmr event triggered by software of overflow */
    TMR_C1_SWTRIG                          = 0x00000002, /*!< tmr event triggered by software of channel 1 */
    TMR_C2_SWTRIG                          = 0x00000004, /*!< tmr event triggered by software of channel 2 */
    TMR_C3_SWTRIG                          = 0x00000008, /*!< tmr event triggered by software of channel 3 */
    TMR_C4_SWTRIG                          = 0x00000010, /*!< tmr event triggered by software of channel 4 */
    TMR_HALL_SWTRIG                        = 0x00000020, /*!< tmr event triggered by software of hall */
    TMR_Trigger_SWTRIG                     = 0x00000040, /*!< tmr event triggered by software of trigger */
    TMR_BRK_SWTRIG                         = 0x00000080  /*!< tmr event triggered by software of brake */
} tmr_Event_Trigger_Type;

/**
  * @brief tmr polarity active type
  */
typedef enum {
    TMR_POLARITY_ACTIVE_HIGH               = 0x00, /*!< tmr polarity active high */
    TMR_POLARITY_ACTIVE_LOW                = 0x01, /*!< tmr polarity active low */
    TMR_POLARITY_ACTIVE_BOTH               = 0x02  /*!< tmr polarity active both high ande low */
} tmr_polarity_active_Type;

/**
  * @brief tmr external signal divider type
  */
typedef enum {
    TMR_ES_Frequency_DIV_1                 = 0x00, /*!< tmr external signal frequency divider 1 */
    TMR_ES_Frequency_DIV_2                 = 0x01, /*!< tmr external signal frequency divider 2 */
    TMR_ES_Frequency_DIV_4                 = 0x02, /*!< tmr external signal frequency divider 4 */
    TMR_ES_Frequency_DIV_8                 = 0x03  /*!< tmr external signal frequency divider 8 */
} tmr_external_signal_divider_Type;

/**
  * @brief tmr external signal polarity type
  */
typedef enum {
    TMR_ES_POLARITY_NON_INVERTED           = 0x00, /*!< tmr external signal polarity non-inerted */
    TMR_ES_POLARITY_INVERTED               = 0x01  /*!< tmr external signal polarity inerted */
} tmr_external_signal_polarity_Type;

/**
  * @brief tmr dma transfer length type
  */
typedef enum {
    TMR_DMA_Transfer_1BYTE                 = 0x00, /*!< tmr dma transfer length 1 byte */
    TMR_DMA_Transfer_2BYTES                = 0x01, /*!< tmr dma transfer length 2 bytes */
    TMR_DMA_Transfer_3BYTES                = 0x02, /*!< tmr dma transfer length 3 bytes */
    TMR_DMA_Transfer_4BYTES                = 0x03, /*!< tmr dma transfer length 4 bytes */
    TMR_DMA_Transfer_5BYTES                = 0x04, /*!< tmr dma transfer length 5 bytes */
    TMR_DMA_Transfer_6BYTES                = 0x05, /*!< tmr dma transfer length 6 bytes */
    TMR_DMA_Transfer_7BYTES                = 0x06, /*!< tmr dma transfer length 7 bytes */
    TMR_DMA_Transfer_8BYTES                = 0x07, /*!< tmr dma transfer length 8 bytes */
    TMR_DMA_Transfer_9BYTES                = 0x08, /*!< tmr dma transfer length 9 bytes */
    TMR_DMA_Transfer_10BYTES               = 0x09, /*!< tmr dma transfer length 10 bytes */
    TMR_DMA_Transfer_11BYTES               = 0x0A, /*!< tmr dma transfer length 11 bytes */
    TMR_DMA_Transfer_12BYTES               = 0x0B, /*!< tmr dma transfer length 12 bytes */
    TMR_DMA_Transfer_13BYTES               = 0x0C, /*!< tmr dma transfer length 13 bytes */
    TMR_DMA_Transfer_14BYTES               = 0x0D, /*!< tmr dma transfer length 14 bytes */
    TMR_DMA_Transfer_15BYTES               = 0x0E, /*!< tmr dma transfer length 15 bytes */
    TMR_DMA_Transfer_16BYTES               = 0x0F, /*!< tmr dma transfer length 16 bytes */
    TMR_DMA_Transfer_17BYTES               = 0x10, /*!< tmr dma transfer length 17 bytes */
    TMR_DMA_Transfer_18BYTES               = 0x11  /*!< tmr dma transfer length 18 bytes */
} tmr_DMA_Transfer_length_Type;

/**
  * @brief tmr dma base address type
  */
typedef enum {
    TMR_CTRL1_Address                      = 0x0000, /*!< tmr dma base address ctrl1 */
    TMR_CTRL2_Address                      = 0x0001, /*!< tmr dma base address ctrl2 */
    TMR_STCTRL_Address                     = 0x0002, /*!< tmr dma base address stctrl */
    TMR_IDEN_Address                       = 0x0003, /*!< tmr dma base address iden */
    TMR_ISTS_Address                       = 0x0004, /*!< tmr dma base address ists */
    TMR_SWEVT_Address                      = 0x0005, /*!< tmr dma base address swevt */
    TMR_CM1_Address                        = 0x0006, /*!< tmr dma base address cm1 */
    TMR_CM2_Address                        = 0x0007, /*!< tmr dma base address cm2 */
    TMR_CCTRL_Address                      = 0x0008, /*!< tmr dma base address cctrl */
    TMR_CVAL_Address                       = 0x0009, /*!< tmr dma base address cval */
    TMR_DIV_Address                        = 0x000A, /*!< tmr dma base address div */
    TMR_PR_Address                         = 0x000B, /*!< tmr dma base address pr */
    TMR_RPR_Address                        = 0x000C, /*!< tmr dma base address rpr */
    TMR_C1DT_Address                       = 0x000D, /*!< tmr dma base address c1dt */
    TMR_C2DT_Address                       = 0x000E, /*!< tmr dma base address c2dt */
    TMR_C3DT_Address                       = 0x000F, /*!< tmr dma base address c3dt */
    TMR_C4DT_Address                       = 0x0010, /*!< tmr dma base address c4dt */
    TMR_BRK_Address                        = 0x0011, /*!< tmr dma base address brake */
    TMR_DMACTRL_Address                    = 0x0012  /*!< tmr dma base address dmactrl */
} tmr_DMA_Address_Type;

/**
  * @brief tmr brk polarity type
  */
typedef enum {
    TMR_BRK_Input_ACTIVE_LOW               = 0x00, /*!< tmr brk input channel active low */
    TMR_BRK_Input_ACTIVE_HIGH              = 0x01  /*!< tmr brk input channel active high */
} tmr_brk_polarity_Type;

/**
  * @brief tmr write protect level type
  */
typedef enum {
    TMR_WP_OFF                             = 0x00, /*!< tmr write protect off */
    TMR_WP_LEVEL_3                         = 0x01, /*!< tmr write protect level 3 */
    TMR_WP_LEVEL_2                         = 0x02, /*!< tmr write protect level 2 */
    TMR_WP_LEVEL_1                         = 0x03  /*!< tmr write protect level 1 */
} tmr_wp_level_Type;

/**
  * @brief tmr input remap type
  */
typedef enum {
    TMR2_TMR8TRGOUT_TMR5_GPIO              = 0x00, /*!< tmr2 input remap to tmr8_trgout or tmr5 remap to gpio */
    TMR2_PTP_TMR5_LICK                     = 0x01, /*!< tmr2 input remap to ptp or tmr5 remap to lick */
    TMR2_OTG1FS_TMR5_LEXT                  = 0x02, /*!< tmr2 input remap to otg1fs or tmr5 remap to lext */
    TMR2_OTG2FS_TMR5_ERTC                  = 0x03  /*!< tmr2 input remap to otg2fs or tmr5 remap to ertc */
} tmr_Input_remap_Type ;
/**

  * @brief tmr output config type
  */
typedef struct {
    tmr_OutPut_Control_Mode_Type           oc_mode;             /*!< output channel mode */
    confirm_state                          oc_idle_state;       /*!< output channel idle state */
    confirm_state                          occ_idle_state;      /*!< output channel complementary idle state */
    tmr_OutPut_polarity_Type               oc_polarity;         /*!< output channel polarity */
    tmr_OutPut_polarity_Type               occ_polarity;        /*!< output channel complementary polarity */
    confirm_state                          oc_OutPut_state;     /*!< output channel enable */
    confirm_state                          occ_OutPut_state;    /*!< output channel complementary enable */
} tmr_OutPut_Config_Type;

/**
  * @brief tmr input capture config type
  */
typedef struct {
    tmr_Channel_Select_Type                input_Channel_Select;   /*!< tmr input channel select */
    tmr_Input_polarity_Type                input_polarity_Select;  /*!< tmr input polarity select */
    tmr_Input_direction_mapped_Type        input_mapped_Select;    /*!< tmr channel mapped direct or indirect */
    uint8_t                                input_Filter_value;     /*!< tmr channel filter value */
} tmr_Input_Config_Type;

/**
  * @brief tmr brkdt config type
  */
typedef struct {
    uint8_t                                deadtime;            /*!< dead-time generator setup */
    tmr_brk_polarity_Type                  brk_polarity;        /*!< tmr brake polarity */
    tmr_wp_level_Type                      wp_level;            /*!< write protect configuration */
    confirm_state                          auto_OutPut_Enable;  /*!< automatic output enable */
    confirm_state                          fcsoen_state;        /*!< frozen channel status when output enable */
    confirm_state                          fcsodis_state;       /*!< frozen channel status when output disable */
    confirm_state                          brk_Enable;          /*!< tmr brk enale */
} tmr_brkdt_Config_Type;

/**
  * @brief type define tmr register all
  */
typedef struct {
    /**
      * @brief tmr ctrl1 register, offset:0x00
      */
    union {
        __IO uint32_t ctrl1;
        struct {
            __IO uint32_t tmren                : 1; /* [0] */
            __IO uint32_t ovfen                : 1; /* [1] */
            __IO uint32_t ovfs                 : 1; /* [2] */
            __IO uint32_t ocmen                : 1; /* [3] */
            __IO uint32_t cnt_dir              : 3; /* [6:4] */
            __IO uint32_t prben                : 1; /* [7] */
            __IO uint32_t clkdiv               : 2; /* [9:8] */
            __IO uint32_t pmen                 : 1; /* [10] */
            __IO uint32_t reserved1            : 21;/* [31:11] */
        } ctrl1_bit;
    };

    /**
      * @brief tmr ctrl2 register, offset:0x04
      */
    union {
        __IO uint32_t ctrl2;
        struct {
            __IO uint32_t cbctrl               : 1; /* [0] */
            __IO uint32_t reserved1            : 1; /* [1] */
            __IO uint32_t ccfs                 : 1; /* [2] */
            __IO uint32_t drs                  : 1; /* [3] */
            __IO uint32_t ptos                 : 3; /* [6:4] */
            __IO uint32_t c1insel              : 1; /* [7] */
            __IO uint32_t c1ios                : 1; /* [8] */
            __IO uint32_t c1cios               : 1; /* [9] */
            __IO uint32_t c2ios                : 1; /* [10] */
            __IO uint32_t c2cios               : 1; /* [11] */
            __IO uint32_t c3ios                : 1; /* [12] */
            __IO uint32_t c3cios               : 1; /* [13] */
            __IO uint32_t c4ios                : 1; /* [14] */
            __IO uint32_t reserved2            : 16;/* [30:15] */
            __IO uint32_t trgout2en            : 1; /* [31] */
        } ctrl2_bit;
    };

    /**
      * @brief tmr smc register, offset:0x08
      */
    union {
        __IO uint32_t stctrl;
        struct {
            __IO uint32_t smsel                : 3; /* [2:0] */
            __IO uint32_t reserved1            : 1; /* [3] */
            __IO uint32_t stis                 : 3; /* [6:4] */
            __IO uint32_t sts                  : 1; /* [7] */
            __IO uint32_t esf                  : 4; /* [11:8] */
            __IO uint32_t esdiv                : 2; /* [13:12] */
            __IO uint32_t ecmben               : 1; /* [14] */
            __IO uint32_t esp                  : 1; /* [15] */
            __IO uint32_t reserved2            : 16;/* [31:16] */
        } stctrl_bit;
    };

    /**
      * @brief tmr die register, offset:0x0C
      */
    union {
        __IO uint32_t iden;
        struct {
            __IO uint32_t ovfien               : 1; /* [0] */
            __IO uint32_t c1ien                : 1; /* [1] */
            __IO uint32_t c2ien                : 1; /* [2] */
            __IO uint32_t c3ien                : 1; /* [3] */
            __IO uint32_t c4ien                : 1; /* [4] */
            __IO uint32_t hallien              : 1; /* [5] */
            __IO uint32_t tien                 : 1; /* [6] */
            __IO uint32_t brkie                : 1; /* [7] */
            __IO uint32_t ovfden               : 1; /* [8] */
            __IO uint32_t c1den                : 1; /* [9] */
            __IO uint32_t c2den                : 1; /* [10] */
            __IO uint32_t c3den                : 1; /* [11] */
            __IO uint32_t c4den                : 1; /* [12] */
            __IO uint32_t hallde               : 1; /* [13] */
            __IO uint32_t tden                 : 1; /* [14] */
            __IO uint32_t reserved1            : 17;/* [31:15] */
        } iden_bit;
    };

    /**
      * @brief tmr ists register, offset:0x10
      */
    union {
        __IO uint32_t ists;
        struct {
            __IO uint32_t ovfif                : 1; /* [0] */
            __IO uint32_t c1if                 : 1; /* [1] */
            __IO uint32_t c2if                 : 1; /* [2] */
            __IO uint32_t c3if                 : 1; /* [3] */
            __IO uint32_t c4if                 : 1; /* [4] */
            __IO uint32_t hallif               : 1; /* [5] */
            __IO uint32_t trgif                : 1; /* [6] */
            __IO uint32_t brkif                : 1; /* [7] */
            __IO uint32_t reserved1            : 1; /* [8] */
            __IO uint32_t c1rf                 : 1; /* [9] */
            __IO uint32_t c2rf                 : 1; /* [10] */
            __IO uint32_t c3rf                 : 1; /* [11] */
            __IO uint32_t c4rf                 : 1; /* [12] */
            __IO uint32_t reserved2            : 19;/* [31:13] */
        } ists_bit;
    };

    /**
      * @brief tmr eveg register, offset:0x14
      */
    union {
        __IO uint32_t swevt;
        struct {
            __IO uint32_t  ovfswtr             : 1; /* [0] */
            __IO uint32_t  c1swtr              : 1; /* [1] */
            __IO uint32_t  c2swtr              : 1; /* [2] */
            __IO uint32_t  c3swtr              : 1; /* [3] */
            __IO uint32_t  c4swtr              : 1; /* [4] */
            __IO uint32_t  hallswtr            : 1; /* [5] */
            __IO uint32_t  trgswtr             : 1; /* [6] */
            __IO uint32_t  brkswtr             : 1; /* [7] */
            __IO uint32_t  reserved            : 24;/* [31:8] */
        } swevt_bit;
    };

    /**
      * @brief tmr ccm1 register, offset:0x18
      */
    union {
        __IO uint32_t cm1;

        /**
         * @brief channel mode
         */
        struct {
            __IO uint32_t c1c                  : 2; /* [1:0] */
            __IO uint32_t c1oien               : 1; /* [2] */
            __IO uint32_t c1oben               : 1; /* [3] */
            __IO uint32_t c1octrl              : 3; /* [6:4] */
            __IO uint32_t c1osen               : 1; /* [7] */
            __IO uint32_t c2c                  : 2; /* [9:8] */
            __IO uint32_t c2oien               : 1; /* [10] */
            __IO uint32_t c2oben               : 1; /* [11] */
            __IO uint32_t c2octrl              : 3; /* [14:12] */
            __IO uint32_t c2osen               : 1; /* [15] */
            __IO uint32_t reserved1            : 16;/* [31:16] */
        } cm1_OutPut_bit;

        /**
          * @brief input capture mode
          */
        struct {
            __IO uint32_t c1c                  : 2; /* [1:0] */
            __IO uint32_t c1idiv               : 2; /* [3:2] */
            __IO uint32_t c1df                 : 4; /* [7:4] */
            __IO uint32_t c2c                  : 2; /* [9:8] */
            __IO uint32_t c2idiv               : 2; /* [11:10] */
            __IO uint32_t c2df                 : 4; /* [15:12] */
            __IO uint32_t reserved1            : 16;/* [31:16] */
        } cm1_Input_bit;
    };

    /**
      * @brief tmr ccm2 register, offset:0x1C
      */
    union {
        __IO uint32_t cm2;

        /**
          * @brief channel mode
          */
        struct {
            __IO uint32_t c3c                  : 2; /* [1:0] */
            __IO uint32_t c3oien               : 1; /* [2] */
            __IO uint32_t c3oben               : 1; /* [3] */
            __IO uint32_t c3octrl              : 3; /* [6:4] */
            __IO uint32_t c3osen               : 1; /* [7] */
            __IO uint32_t c4c                  : 2; /* [9:8] */
            __IO uint32_t c4oien               : 1; /* [10] */
            __IO uint32_t c4oben               : 1; /* [11] */
            __IO uint32_t c4octrl              : 3; /* [14:12] */
            __IO uint32_t c4osen               : 1; /* [15] */
            __IO uint32_t reserved1            : 16;/* [31:16] */
        } cm2_OutPut_bit;

        /**
          * @brief input capture mode
          */
        struct {
            __IO uint32_t c3c                  : 2; /* [1:0] */
            __IO uint32_t c3idiv               : 2; /* [3:2] */
            __IO uint32_t c3df                 : 4; /* [7:4] */
            __IO uint32_t c4c                  : 2; /* [9:8] */
            __IO uint32_t c4idiv               : 2; /* [11:10] */
            __IO uint32_t c4df                 : 4; /* [15:12] */
            __IO uint32_t reserved1            : 16;/* [31:16] */
        } cm2_Input_bit;
    };

    /**
      * @brief tmr cce register, offset:0x20
      */
    union {
        uint32_t cctrl;
        struct {
            __IO uint32_t c1en                 : 1; /* [0] */
            __IO uint32_t c1p                  : 1; /* [1] */
            __IO uint32_t c1cen                : 1; /* [2] */
            __IO uint32_t c1cp                 : 1; /* [3] */
            __IO uint32_t c2en                 : 1; /* [4] */
            __IO uint32_t c2p                  : 1; /* [5] */
            __IO uint32_t c2cen                : 1; /* [6] */
            __IO uint32_t c2cp                 : 1; /* [7] */
            __IO uint32_t c3en                 : 1; /* [8] */
            __IO uint32_t c3p                  : 1; /* [9] */
            __IO uint32_t c3cen                : 1; /* [10] */
            __IO uint32_t c3cp                 : 1; /* [11] */
            __IO uint32_t c4en                 : 1; /* [12] */
            __IO uint32_t c4p                  : 1; /* [13] */
            __IO uint32_t reserved1            : 18;/* [31:14] */
        } cctrl_bit;
    };

    /**
      * @brief tmr cnt register, offset:0x24
      */
    union {
        __IO uint32_t cval;
        struct {
            __IO uint32_t cval                 : 32;/* [31:0] */
        } cval_bit;
    };

    /**
      * @brief tmr div, offset:0x28
      */
    union {
        __IO uint32_t div;
        struct {
            __IO uint32_t div                  : 16;/* [15:0] */
            __IO uint32_t reserved1            : 16;/* [31:16] */
        } div_bit;
    };

    /**
      * @brief tmr pr register, offset:0x2C
      */
    union {
        __IO  uint32_t pr;
        struct {
            __IO uint32_t pr                   : 32;/* [31:0] */
        } pr_bit;
    };

    /**
      * @brief tmr rpr register, offset:0x30
      */
    union {
        __IO uint32_t rpr;
        struct {
            __IO uint32_t rpr                  : 16;/* [15:0] */
            __IO uint32_t reserved1            : 16;/* [31:16] */
        } rpr_bit;
    };

    /**
      * @brief tmr c1dt register, offset:0x34
      */
    union {
        uint32_t c1dt;
        struct {
            __IO uint32_t c1dt                 : 32;/* [31:0] */
        } c1dt_bit;
    };

    /**
      * @brief tmr c2dt register, offset:0x38
      */
    union {
        uint32_t c2dt;
        struct {
            __IO uint32_t c2dt                 : 32;/* [31:0] */
        } c2dt_bit;
    };

    /**
      * @brief tmr c3dt register, offset:0x3C
      */
    union {
        __IO uint32_t c3dt;
        struct {
            __IO uint32_t c3dt                 : 32;/* [31:0] */
        } c3dt_bit;
    };

    /**
      * @brief tmr c4dt register, offset:0x40
      */
    union {
        __IO uint32_t c4dt;
        struct {
            __IO uint32_t c4dt                 : 32;/* [31:0] */
        } c4dt_bit;
    };

    /**
      * @brief tmr brk register, offset:0x44
      */
    union {
        __IO uint32_t brk;
        struct {
            __IO uint32_t dtc                  : 8; /* [7:0] */
            __IO uint32_t wpc                  : 2; /* [9:8] */
            __IO uint32_t fcsodis              : 1; /* [10] */
            __IO uint32_t fcsoen               : 1; /* [11] */
            __IO uint32_t brken                : 1; /* [12] */
            __IO uint32_t brkv                 : 1; /* [13] */
            __IO uint32_t aoen                 : 1; /* [14] */
            __IO uint32_t oen                  : 1; /* [15] */
            __IO uint32_t reserved1            : 16; /* [31:16] */
        } brk_bit;
    };
    /**
      * @brief tmr dmactrl register, offset:0x48
      */
    union {
        __IO uint32_t dmactrl;
        struct {
            __IO uint32_t addr                 : 5; /* [4:0] */
            __IO uint32_t reserved1            : 3; /* [7:5] */
            __IO uint32_t dtb                  : 5; /* [12:8] */
            __IO uint32_t reserved2            : 19;/* [31:13] */
        } dmactrl_bit;
    };

    /**
      * @brief tmr dmadt register, offset:0x4C
      */
    union {
        __IO uint32_t dmadt;
        struct {
            __IO uint32_t dmadt                : 16;/* [15:0] */
            __IO uint32_t reserved1            : 16;/* [31:16] */
        } dmadt_bit;
    };

    /**
      * @brief tmr rmp register, offset:0x50
      */
    union {
        __IO uint32_t rmp;
        struct {
            __IO uint32_t reserved1             : 6; /* [5:0] */
            __IO uint32_t tmr5_ch4_irmp         : 2; /* [7:6] */
            __IO uint32_t reserved2             : 2; /* [9:8] */
            __IO uint32_t tmr2_ch1_irmp         : 2; /* [11:10] */
            __IO uint32_t reserved3             : 20;/* [31:16] */
        } rmp_bit;
    };

    /**
      * @brief tmr reserved0 register, offset:0x54-0x6C
      */
    __IO uint32_t reserved1[7];

    /**
      * @brief tmr cm3 register, offset:0x70
      */
    union {
        __IO uint32_t cm3;
        struct {
            __IO uint32_t reserved1            : 2; /* [1:0] */
            __IO uint32_t c5oien               : 1; /* [2] */
            __IO uint32_t c5oben               : 1; /* [3] */
            __IO uint32_t c5octrl              : 3; /* [6:4] */
            __IO uint32_t c5osen               : 1; /* [7] */
            __IO uint32_t reserved2            : 24;/* [31:8] */
        } cm3_OutPut_bit;
    };

    /**
      * @brief tmr c5dt register, offset:0x74
      */
    union {
        __IO uint32_t c5dt;
        struct {
            __IO uint32_t c5dt                 : 32;/* [31:0] */
        } c5dt_bit;
    };
} tmr_Type;

/**
  * @}
  */

#define TMR1                             ((tmr_Type *) TMR1_BASE)
#define TMR2                             ((tmr_Type *) TMR2_BASE)
#define TMR3                             ((tmr_Type *) TMR3_BASE)
#define TMR4                             ((tmr_Type *) TMR4_BASE)
#define TMR5                             ((tmr_Type *) TMR5_BASE)
#define TMR6                             ((tmr_Type *) TMR6_BASE)
#define TMR7                             ((tmr_Type *) TMR7_BASE)
#define TMR8                             ((tmr_Type *) TMR8_BASE)
#define TMR9                             ((tmr_Type *) TMR9_BASE)
#define TMR10                            ((tmr_Type *) TMR10_BASE)
#define TMR11                            ((tmr_Type *) TMR11_BASE)
#define TMR12                            ((tmr_Type *) TMR12_BASE)
#define TMR13                            ((tmr_Type *) TMR13_BASE)
#define TMR14                            ((tmr_Type *) TMR14_BASE)
#define TMR20                            ((tmr_Type *) TMR20_BASE)

/** @defgroup TMR_exported_functions
  * @{
  */

void tmr_Reset(tmr_Type *tmr_x);
void tmr_Counter_Enable(tmr_Type *tmr_x, confirm_state new_state);
void tmr_OutPut_Default_Para_Init(tmr_OutPut_Config_Type *tmr_OutPut_struct);
void tmr_Input_Default_Para_Init(tmr_Input_Config_Type *tmr_Input_struct);
void tmr_brkdt_Default_Para_Init(tmr_brkdt_Config_Type *tmr_brkdt_struct);
void tmr_base_Init(tmr_Type* tmr_x, uint32_t tmr_pr, uint32_t tmr_div);
void tmr_Clock_Source_div_Set(tmr_Type *tmr_x, tmr_Clock_division_Type tmr_Clock_div);
void tmr_cnt_dir_Set(tmr_Type *tmr_x, tmr_count_Mode_Type tmr_cnt_dir);
void tmr_repetition_Counter_Set(tmr_Type *tmr_x, uint8_t tmr_rpr_value);
void tmr_Counter_value_Set(tmr_Type *tmr_x, uint32_t tmr_cnt_value);
uint32_t tmr_Counter_value_Get(tmr_Type *tmr_x);
void tmr_div_value_Set(tmr_Type *tmr_x, uint32_t tmr_div_value);
uint32_t tmr_div_value_Get(tmr_Type *tmr_x);
void tmr_OutPut_Channel_Config(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                               tmr_OutPut_Config_Type *tmr_OutPut_struct);
void tmr_OutPut_Channel_Mode_Select(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                    tmr_OutPut_Control_Mode_Type oc_mode);
void tmr_period_value_Set(tmr_Type *tmr_x, uint32_t tmr_pr_value);
uint32_t tmr_period_value_Get(tmr_Type *tmr_x);
void tmr_Channel_value_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                           uint32_t tmr_Channel_value);
uint32_t tmr_Channel_value_Get(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel);
void tmr_period_buffer_Enable(tmr_Type *tmr_x, confirm_state new_state);
void tmr_OutPut_Channel_buffer_Enable(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                      confirm_state new_state);
void tmr_OutPut_Channel_immediately_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                        confirm_state new_state);
void tmr_OutPut_Channel_switch_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                   confirm_state new_state);
void tmr_One_cycle_Mode_Enable(tmr_Type *tmr_x, confirm_state new_state);
void tmr_32_Bit_function_Enable (tmr_Type *tmr_x, confirm_state new_state);
void tmr_OverFlow_Request_Source_Set(tmr_Type *tmr_x, confirm_state new_state);
void tmr_OverFlow_Event_Disable(tmr_Type *tmr_x, confirm_state new_state);
void tmr_Input_Channel_Init(tmr_Type *tmr_x, tmr_Input_Config_Type *input_struct, \
                            tmr_Channel_Input_divider_Type divider_factor);
void tmr_Channel_Enable(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, confirm_state new_state);
void tmr_Input_Channel_Filter_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                  uint16_t filter_value);
void tmr_pwm_Input_Config(tmr_Type *tmr_x, tmr_Input_Config_Type *input_struct, \
                          tmr_Channel_Input_divider_Type divider_factor);
void tmr_channel1_Input_Select(tmr_Type *tmr_x, tmr_channel1_Input_connected_Type ch1_connect);
void tmr_Input_Channel_divider_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                   tmr_Channel_Input_divider_Type divider_factor);
void tmr_primary_Mode_Select(tmr_Type *tmr_x, tmr_primary_Select_Type primary_mode);
void tmr_Sub_Mode_Select(tmr_Type *tmr_x, tmr_Sub_Mode_Select_Type sub_mode);
void tmr_Channel_DMA_Select(tmr_Type *tmr_x, tmr_DMA_Request_Source_Type cc_DMA_Select);
void tmr_hall_Select(tmr_Type *tmr_x,  confirm_state new_state);
void tmr_Channel_buffer_Enable(tmr_Type *tmr_x, confirm_state new_state);
void tmr_trgout2_Enable(tmr_Type *tmr_x, confirm_state new_state);
void tmr_Trigger_Input_Select(tmr_Type *tmr_x, sub_tmr_Input_sel_Type trigger_Select);
void tmr_Sub_Sync_Mode_Set(tmr_Type *tmr_x, confirm_state new_state);
void tmr_DMA_Request_Enable(tmr_Type *tmr_x, tmr_DMA_Request_Type DMA_request, confirm_state new_state);
void tmr_Interrupt_Enable(tmr_Type *tmr_x, uint32_t tmr_interrupt, confirm_state new_state);
flag_status tmr_Flag_Get(tmr_Type *tmr_x, uint32_t tmr_flag);
void tmr_Flag_Clear(tmr_Type *tmr_x, uint32_t tmr_flag);
void tmr_Event_sw_trigger(tmr_Type *tmr_x, tmr_Event_Trigger_Type tmr_event);
void tmr_OutPut_Enable(tmr_Type *tmr_x, confirm_state new_state);
void tmr_Internal_Clock_Set(tmr_Type *tmr_x);
void tmr_OutPut_Channel_polarity_Set(tmr_Type *tmr_x, tmr_Channel_Select_Type tmr_channel, \
                                     tmr_polarity_active_Type oc_polarity);
void tmr_external_Clock_Config(tmr_Type *tmr_x, tmr_external_signal_divider_Type es_divide, \
                               tmr_external_signal_polarity_Type  es_polarity, uint16_t es_filter);
void tmr_external_Clock_mode1_Config(tmr_Type *tmr_x, tmr_external_signal_divider_Type es_divide, \
                                     tmr_external_signal_polarity_Type  es_polarity, uint16_t es_filter);
void tmr_external_Clock_mode2_Config(tmr_Type *tmr_x, tmr_external_signal_divider_Type es_divide, \
                                     tmr_external_signal_polarity_Type  es_polarity, uint16_t es_filter);
void tmr_encoder_Mode_Config(tmr_Type *tmr_x, tmr_encoder_Mode_Type encoder_mode, tmr_Input_polarity_Type \
                             ic1_polarity, tmr_Input_polarity_Type ic2_polarity);
void tmr_force_OutPut_Set(tmr_Type *tmr_x,  tmr_Channel_Select_Type tmr_channel, \
                          tmr_force_OutPut_Type force_output);
void tmr_DMA_Control_Config(tmr_Type *tmr_x, tmr_DMA_Transfer_length_Type DMA_length, \
                            tmr_DMA_Address_Type DMA_base_Address);
void tmr_brkdt_Config(tmr_Type *tmr_x, tmr_brkdt_Config_Type *brkdt_struct);
void tmr_iremap_Config(tmr_Type *tmr_x, tmr_Input_remap_Type input_remap);

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
