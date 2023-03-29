/**
  **************************************************************************
  * @file     at32f435_437_spi.h
  * @brief    at32f435_437 spi header file
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AT32F435_437_SPI_H
#define __AT32F435_437_SPI_H

#ifdef __cplusplus
extern "C" {
#endif


/* Includes ------------------------------------------------------------------*/
#include "at32f435_437.h"

/** @addtogroup AT32F435_437_periph_driver
  * @{
  */

/** @addtogroup SPI
  * @{
  */

/**
  * @defgroup SPI_I2S_flags_definition
  * @brief spi i2s flag
  * @{
  */

#define SPI_I2S_RDBF_FLAG                0x0001 /*!< spi or i2s receive data buffer full flag */
#define SPI_I2S_TDBE_FLAG                0x0002 /*!< spi or i2s transmit data buffer empty flag */
#define I2S_ACS_FLAG                     0x0004 /*!< i2s audio channel state flag */
#define I2S_TUERR_FLAG                   0x0008 /*!< i2s transmitter underload error flag */
#define SPI_CCERR_FLAG                   0x0010 /*!< spi crc calculation error flag */
#define SPI_MMERR_FLAG                   0x0020 /*!< spi master mode error flag */
#define SPI_I2S_ROERR_FLAG               0x0040 /*!< spi or i2s receiver overflow error flag */
#define SPI_I2S_BF_FLAG                  0x0080 /*!< spi or i2s busy flag */
#define SPI_CSPAS_FLAG                   0x0100 /*!< spi cs pulse abnormal setting fiag */

/**
  * @}
  */

/**
  * @defgroup SPI_I2S_interrupts_definition
  * @brief spi i2s interrupt
  * @{
  */

#define SPI_I2S_Error_INT                0x0020 /*!< error interrupt */
#define SPI_I2S_RDBF_INT                 0x0040 /*!< receive data buffer full interrupt */
#define SPI_I2S_TDBE_INT                 0x0080 /*!< transmit data buffer empty interrupt */

/**
  * @}
  */

/** @defgroup SPI_exported_Types
  * @{
  */

/**
  * @brief spi frame bit num type
  */
typedef enum {
    SPI_FRAME_8BIT                         = 0x00, /*!< 8-bit data frame format */
    SPI_FRAME_16BIT                        = 0x01  /*!< 16-bit data frame format */
} spi_frame_Bit_Num_Type;

/**
  * @brief spi master/slave mode type
  */
typedef enum {
    SPI_Mode_SLAVE                         = 0x00, /*!< select as slave mode */
    SPI_Mode_MASTER                        = 0x01  /*!< select as master mode */
} spi_Master_Slave_Mode_Type;

/**
  * @brief spi clock polarity (clkpol) type
  */
typedef enum {
    SPI_Clock_POLARITY_LOW                 = 0x00, /*!< sck keeps low at idle state */
    SPI_Clock_POLARITY_HIGH                = 0x01  /*!< sck keeps high at idle state */
} spi_Clock_polarity_Type;

/**
  * @brief spi clock phase (clkpha) type
  */
typedef enum {
    SPI_Clock_PHASE_1EDGE                  = 0x00, /*!< data capture start from the first clock edge */
    SPI_Clock_PHASE_2EDGE                  = 0x01  /*!< data capture start from the second clock edge */
} spi_Clock_phase_Type;

/**
  * @brief spi cs mode type
  */
typedef enum {
    SPI_CS_HARDWARE_MODE                   = 0x00, /*!< cs is hardware mode */
    SPI_CS_Software_MODE                   = 0x01  /*!< cs is software mode */
} spi_cs_Mode_Type;

/**
  * @brief spi master clock frequency division type
  */
typedef enum {
    SPI_MCLK_DIV_2                         = 0x00, /*!< master clock frequency division 2 */
    SPI_MCLK_DIV_3                         = 0x0A, /*!< master clock frequency division 3 */
    SPI_MCLK_DIV_4                         = 0x01, /*!< master clock frequency division 4 */
    SPI_MCLK_DIV_8                         = 0x02, /*!< master clock frequency division 8 */
    SPI_MCLK_DIV_16                        = 0x03, /*!< master clock frequency division 16 */
    SPI_MCLK_DIV_32                        = 0x04, /*!< master clock frequency division 32 */
    SPI_MCLK_DIV_64                        = 0x05, /*!< master clock frequency division 64 */
    SPI_MCLK_DIV_128                       = 0x06, /*!< master clock frequency division 128 */
    SPI_MCLK_DIV_256                       = 0x07, /*!< master clock frequency division 256 */
    SPI_MCLK_DIV_512                       = 0x08, /*!< master clock frequency division 512 */
    SPI_MCLK_DIV_1024                      = 0x09  /*!< master clock frequency division 1024 */
} spi_mclk_Freq_div_Type;

/**
  * @brief spi transmit first bit (lsb/msb) type
  */
typedef enum {
    SPI_FIRST_Bit_MSB                      = 0x00, /*!< the frame format is msb first */
    SPI_FIRST_Bit_LSB                      = 0x01  /*!< the frame format is lsb first */
} spi_first_Bit_Type;

/**
  * @brief spi transmission mode type
  */
typedef enum {
    SPI_Transmit_FULL_DUPLEX               = 0x00, /*!< dual line unidirectional full-duplex mode(slben = 0 and ora = 0) */
    SPI_Transmit_SIMPLEX_RX                = 0x01, /*!< dual line unidirectional simplex receive-only mode(slben = 0 and ora = 1) */
    SPI_Transmit_HALF_DUPLEX_RX            = 0x02, /*!< single line bidirectional half duplex mode-receiving(slben = 1 and slbtd = 0) */
    SPI_Transmit_HALF_DUPLEX_TX            = 0x03  /*!< single line bidirectional half duplex mode-transmitting(slben = 1 and slbtd = 1) */
} spi_transmission_Mode_Type;

/**
  * @brief spi crc direction type
  */
typedef enum {
    SPI_CRC_RX                             = 0x0014, /*!< crc direction is rx */
    SPI_CRC_TX                             = 0x0018  /*!< crc direction is tx */
} spi_CRC_direction_Type;

/**
  * @brief spi single line bidirectional direction type
  */
typedef enum {
    SPI_HALF_DUPLEX_DIRECTION_RX           = 0x00, /*!< single line bidirectional half duplex mode direction: receive(slbtd = 0) */
    SPI_HALF_DUPLEX_DIRECTION_TX           = 0x01  /*!< single line bidirectional half duplex mode direction: transmit(slbtd = 1) */
} spi_half_duplex_direction_Type;

/**
  * @brief spi software cs internal level type
  */
typedef enum {
    SPI_SWCS_Internal_LEVEL_LOW            = 0x00, /*!< internal level low */
    SPI_SWCS_Internal_LEVEL_HIGHT          = 0x01  /*!< internal level high */
} spi_Software_cs_level_Type;

/**
  * @brief i2s audio protocol type
  */
typedef enum {
    I2S_AUDIO_PROTOCOL_PHILLIPS            = 0x00, /*!< i2s philip standard */
    I2S_AUDIO_PROTOCOL_MSB                 = 0x01, /*!< msb-justified standard */
    I2S_AUDIO_PROTOCOL_LSB                 = 0x02, /*!< lsb-justified standard */
    I2S_AUDIO_PROTOCOL_PCM_SHORT           = 0x03, /*!< pcm standard-short frame */
    I2S_AUDIO_PROTOCOL_PCM_LONG            = 0x04  /*!< pcm standard-long frame */
} i2s_audio_protocol_Type;

/**
  * @brief i2s audio frequency type
  */
typedef enum {
    I2S_AUDIO_Frequency_DEFAULT            = 2,     /*!< i2s audio sampling frequency default */
    I2S_AUDIO_Frequency_8K                 = 8000,  /*!< i2s audio sampling frequency 8k */
    I2S_AUDIO_Frequency_11_025K            = 11025, /*!< i2s audio sampling frequency 11.025k */
    I2S_AUDIO_Frequency_16K                = 16000, /*!< i2s audio sampling frequency 16k */
    I2S_AUDIO_Frequency_22_05K             = 22050, /*!< i2s audio sampling frequency 22.05k */
    I2S_AUDIO_Frequency_32K                = 32000, /*!< i2s audio sampling frequency 32k */
    I2S_AUDIO_Frequency_44_1K              = 44100, /*!< i2s audio sampling frequency 44.1k */
    I2S_AUDIO_Frequency_48K                = 48000, /*!< i2s audio sampling frequency 48k */
    I2S_AUDIO_Frequency_96K                = 96000, /*!< i2s audio sampling frequency 96k */
    I2S_AUDIO_Frequency_192K               = 192000 /*!< i2s audio sampling frequency 192k */
} i2s_audio_sampling_Freq_Type;

/**
  * @brief i2s data bit num and channel bit num type
  */
typedef enum {
    I2S_Data_16BIT_Channel_16BIT           = 0x01, /*!< 16-bit data packed in 16-bit channel frame */
    I2S_Data_16BIT_Channel_32BIT           = 0x02, /*!< 16-bit data packed in 32-bit channel frame */
    I2S_Data_24BIT_Channel_32BIT           = 0x03, /*!< 24-bit data packed in 32-bit channel frame */
    I2S_Data_32BIT_Channel_32BIT           = 0x04  /*!< 32-bit data packed in 32-bit channel frame */
} i2s_Data_Channel_format_Type;

/**
  * @brief i2s operation mode type
  */
typedef enum {
    I2S_Mode_Slave_TX                      = 0x00, /*!< slave transmission mode */
    I2S_Mode_Slave_RX                      = 0x01, /*!< slave reception mode */
    I2S_Mode_Master_TX                     = 0x02, /*!< master transmission mode */
    I2S_Mode_Master_RX                     = 0x03  /*!< master reception mode */
} i2s_Operation_Mode_Type;

/**
  * @brief i2s clock polarity type
  */
typedef enum {
    I2S_Clock_POLARITY_LOW                 = 0x00, /*!< i2s clock steady state is low level */
    I2S_Clock_POLARITY_HIGH                = 0x01  /*!< i2s clock steady state is high level */
} i2s_Clock_polarity_Type;

/**
  * @brief spi init type
  */
typedef struct {
    spi_transmission_Mode_Type             transmission_mode;     /*!< transmission mode selection */
    spi_Master_Slave_Mode_Type             master_Slave_mode;     /*!< master or slave mode selection */
    spi_mclk_Freq_div_Type                 mclk_Freq_division;    /*!< master clock frequency division selection */
    spi_first_Bit_Type                     first_Bit_transmission;/*!< transmit lsb or msb selection */
    spi_frame_Bit_Num_Type                 frame_Bit_Num;         /*!< frame bit num 8 or 16 bit selection */
    spi_Clock_polarity_Type                clock_polarity;        /*!< clock polarity selection */
    spi_Clock_phase_Type                   clock_phase;           /*!< clock phase selection */
    spi_cs_Mode_Type                       cs_Mode_Selection;     /*!< hardware or software cs mode selection */
} spi_Init_Type;

/**
  * @brief i2s init type
  */
typedef struct {
    i2s_Operation_Mode_Type                operation_mode;        /*!< operation mode selection */
    i2s_audio_protocol_Type                audio_protocol;        /*!< audio protocol selection */
    i2s_audio_sampling_Freq_Type           audio_sampling_freq;   /*!< audio frequency selection */
    i2s_Data_Channel_format_Type           data_Channel_format;   /*!< data bit num and channel bit num selection */
    i2s_Clock_polarity_Type                clock_polarity;        /*!< clock polarity selection */
    confirm_state                          mclk_OutPut_Enable;    /*!< mclk_output selection */
} i2s_Init_Type;

/**
  * @brief type define spi register all
  */
typedef struct {

    /**
      * @brief spi ctrl1 register, offset:0x00
      */
    union {
        __IO uint32_t ctrl1;
        struct {
            __IO uint32_t clkpha               : 1; /* [0] */
            __IO uint32_t clkpol               : 1; /* [1] */
            __IO uint32_t msten                : 1; /* [2] */
            __IO uint32_t mdiv_l               : 3; /* [5:3] */
            __IO uint32_t spien                : 1; /* [6] */
            __IO uint32_t ltf                  : 1; /* [7] */
            __IO uint32_t swcsil               : 1; /* [8] */
            __IO uint32_t swcsen               : 1; /* [9] */
            __IO uint32_t ora                  : 1; /* [10] */
            __IO uint32_t fbn                  : 1; /* [11] */
            __IO uint32_t ntc                  : 1; /* [12] */
            __IO uint32_t ccen                 : 1; /* [13] */
            __IO uint32_t slbtd                : 1; /* [14] */
            __IO uint32_t slben                : 1; /* [15] */
            __IO uint32_t reserved1            : 16;/* [31:16] */
        } ctrl1_bit;
    };

    /**
      * @brief spi ctrl2 register, offset:0x04
      */
    union {
        __IO uint32_t ctrl2;
        struct {
            __IO uint32_t dmaren               : 1; /* [0] */
            __IO uint32_t dmaten               : 1; /* [1] */
            __IO uint32_t hwcsoe               : 1; /* [2] */
            __IO uint32_t reserved1            : 1; /* [3] */
            __IO uint32_t tien                 : 1; /* [4] */
            __IO uint32_t errie                : 1; /* [5] */
            __IO uint32_t rdbfie               : 1; /* [6] */
            __IO uint32_t tdbeie               : 1; /* [7] */
            __IO uint32_t mdiv_h               : 1; /* [8] */
            __IO uint32_t mdiv3en              : 1; /* [9] */
            __IO uint32_t reserved2            : 22;/* [31:10] */
        } ctrl2_bit;
    };

    /**
      * @brief spi sts register, offset:0x08
      */
    union {
        __IO uint32_t sts;
        struct {
            __IO uint32_t rdbf                 : 1; /* [0] */
            __IO uint32_t tdbe                 : 1; /* [1] */
            __IO uint32_t acs                  : 1; /* [2] */
            __IO uint32_t tuerr                : 1; /* [3] */
            __IO uint32_t ccerr                : 1; /* [4] */
            __IO uint32_t mmerr                : 1; /* [5] */
            __IO uint32_t roerr                : 1; /* [6] */
            __IO uint32_t bf                   : 1; /* [7] */
            __IO uint32_t cspas                : 1; /* [8] */
            __IO uint32_t reserved1            : 23;/* [31:9] */
        } sts_bit;
    };

    /**
      * @brief spi dt register, offset:0x0C
      */
    union {
        __IO uint32_t dt;
        struct {
            __IO uint32_t dt                  : 16;/* [15:0] */
            __IO uint32_t reserved1           : 16;/* [31:16] */
        } dt_bit;
    };

    /**
      * @brief spi cpoly register, offset:0x10
      */
    union {
        __IO uint32_t cpoly;
        struct {
            __IO uint32_t cpoly               : 16;/* [15:0] */
            __IO uint32_t reserved1           : 16;/* [31:16] */
        } cpoly_bit;
    };

    /**
      * @brief spi rcrc register, offset:0x14
      */
    union {
        __IO uint32_t rcrc;
        struct {
            __IO uint32_t rcrc                : 16;/* [15:0] */
            __IO uint32_t reserved1           : 16;/* [31:16] */
        } rCRC_bit;
    };

    /**
      * @brief spi tcrc register, offset:0x18
      */
    union {
        __IO uint32_t tcrc;
        struct {
            __IO uint32_t tcrc                : 16;/* [15:0] */
            __IO uint32_t reserved1           : 16;/* [31:16] */
        } tCRC_bit;
    };

    /**
      * @brief spi i2sctrl register, offset:0x1C
      */
    union {
        __IO uint32_t i2sctrl;
        struct {
            __IO uint32_t i2scbn              : 1; /* [0] */
            __IO uint32_t i2sdbn              : 2; /* [2:1] */
            __IO uint32_t i2sclkpol           : 1; /* [3] */
            __IO uint32_t stdsel              : 2; /* [5:4] */
            __IO uint32_t reserved1           : 1; /* [6] */
            __IO uint32_t pcmfssel            : 1; /* [7] */
            __IO uint32_t opersel             : 2; /* [9:8] */
            __IO uint32_t i2sen               : 1; /* [10] */
            __IO uint32_t i2smsel             : 1; /* [11] */
            __IO uint32_t reserved2           : 20;/* [31:12] */
        } i2sctrl_bit;
    };

    /**
      * @brief spi i2sclk register, offset:0x20
      */
    union {
        __IO uint32_t i2sclk;
        struct {
            __IO uint32_t i2sdiv_l            : 8; /* [7:0] */
            __IO uint32_t i2sodd              : 1; /* [8] */
            __IO uint32_t i2smclkoe           : 1; /* [9] */
            __IO uint32_t i2sdiv_h            : 2; /* [11:10] */
            __IO uint32_t reserved1           : 20;/* [31:12] */
        } i2sclk_bit;
    };

} spi_Type;

/**
  * @}
  */

#define SPI1                            ((spi_Type *) SPI1_BASE)
#define SPI2                            ((spi_Type *) SPI2_BASE)
#define SPI3                            ((spi_Type *) SPI3_BASE)
#define SPI4                            ((spi_Type *) SPI4_BASE)
#define I2S2EXT                         ((spi_Type *) I2S2EXT_BASE)
#define I2S3EXT                         ((spi_Type *) I2S3EXT_BASE)

/** @defgroup SPI_exported_functions
  * @{
  */

void spi_i2s_Reset(spi_Type *spi_x);
void spi_Default_Para_Init(spi_Init_Type* spi_Init_struct);
void spi_Init(spi_Type* spi_x, spi_Init_Type* spi_Init_struct);
void spi_ti_Mode_Enable(spi_Type* spi_x, confirm_state new_state);
void spi_CRC_next_transmit(spi_Type* spi_x);
void spi_CRC_polynomial_Set(spi_Type* spi_x, uint16_t CRC_poly);
uint16_t spi_CRC_polynomial_Get(spi_Type* spi_x);
void spi_CRC_Enable(spi_Type* spi_x, confirm_state new_state);
uint16_t spi_CRC_value_Get(spi_Type* spi_x, spi_CRC_direction_Type CRC_direction);
void spi_hardware_cs_OutPut_Enable(spi_Type* spi_x, confirm_state new_state);
void spi_Software_cs_Internal_level_Set(spi_Type* spi_x, spi_Software_cs_level_Type level);
void spi_frame_Bit_Num_Set(spi_Type* spi_x, spi_frame_Bit_Num_Type bit_Num);
void spi_half_duplex_direction_Set(spi_Type* spi_x, spi_half_duplex_direction_Type direction);
void spi_Enable(spi_Type* spi_x, confirm_state new_state);
void i2s_Default_Para_Init(i2s_Init_Type* i2s_Init_struct);
void i2s_Init(spi_Type* spi_x, i2s_Init_Type* i2s_Init_struct);
void i2s_Enable(spi_Type* spi_x, confirm_state new_state);
void spi_i2s_Interrupt_Enable(spi_Type* spi_x, uint32_t spi_i2s_int, confirm_state new_state);
void spi_i2s_DMA_transmitter_Enable(spi_Type* spi_x, confirm_state new_state);
void spi_i2s_DMA_Receiver_Enable(spi_Type* spi_x, confirm_state new_state);
void spi_i2s_Data_transmit(spi_Type* spi_x, uint16_t tx_data);
uint16_t spi_i2s_Data_receive(spi_Type* spi_x);
flag_status spi_i2s_Flag_Get(spi_Type* spi_x, uint32_t spi_i2s_flag);
void spi_i2s_Flag_Clear(spi_Type* spi_x, uint32_t spi_i2s_flag);

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
