/**
  **************************************************************************
  * @file     at32f435_437_spi.c
  * @brief    contains all the functions for the spi firmware library
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

/** @defgroup SPI
  * @brief SPI driver modules
  * @{
  */

#ifdef SPI_MODULE_EnableD

/** @defgroup SPI_private_functions
  * @{
  */

/**
  * @brief  spi reset by crm reset register
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @retval none
  */
void spi_i2s_Reset(spi_Type *spi_x) {
    if(spi_x == SPI1) {
        crm_periph_Reset(CRM_SPI1_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_SPI1_PERIPH_Reset, FALSE);
    } else if(spi_x == SPI2) {
        crm_periph_Reset(CRM_SPI2_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_SPI2_PERIPH_Reset, FALSE);
    } else if(spi_x == SPI3) {
        crm_periph_Reset(CRM_SPI3_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_SPI3_PERIPH_Reset, FALSE);
    } else if(spi_x == SPI4) {
        crm_periph_Reset(CRM_SPI4_PERIPH_Reset, TRUE);
        crm_periph_Reset(CRM_SPI4_PERIPH_Reset, FALSE);
    }
}

/**
  * @brief  spi init config with its default value.
  * @param  spi_Init_struct : pointer to a spi_Init_Type structure which will
  *         be initialized.
  * @retval none
  */
void spi_Default_Para_Init(spi_Init_Type* spi_Init_struct) {
    spi_Init_struct->transmission_Mode = SPI_Transmit_FULL_DUPLEX;
    spi_Init_struct->master_Slave_Mode = SPI_Mode_SLAVE;
    spi_Init_struct->mclk_Freq_division = SPI_MCLK_DIV_2;
    spi_Init_struct->first_Bit_transmission = SPI_FIRST_Bit_MSB;
    spi_Init_struct->frame_Bit_Num = SPI_FRAME_8BIT;
    spi_Init_struct->clock_polarity = SPI_Clock_POLARITY_LOW;
    spi_Init_struct->clock_phase = SPI_Clock_PHASE_1EDGE;
    spi_Init_struct->cs_Mode_Selection = SPI_CS_Software_Mode;
}

/**
  * @brief  spi init config with its setting value.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  spi_Init_struct : pointer to a spi_Init_Type structure which will be initialized.
  * @retval none
  */
void spi_Init(spi_Type* spi_x, spi_Init_Type* spi_Init_struct) {
    spi_x->i2sctrl_bit.i2smsel = FALSE;

    if(spi_Init_struct->transmission_Mode == SPI_Transmit_FULL_DUPLEX) {
        spi_x->ctrl1_bit.slben = FALSE;
        spi_x->ctrl1_bit.slbtd = FALSE;
        spi_x->ctrl1_bit.ora = FALSE;
    } else if(spi_Init_struct->transmission_Mode == SPI_Transmit_SIMPLEX_RX) {
        spi_x->ctrl1_bit.slben = FALSE;
        spi_x->ctrl1_bit.slbtd = FALSE;
        spi_x->ctrl1_bit.ora = TRUE;
    } else if(spi_Init_struct->transmission_Mode == SPI_Transmit_HALF_DUPLEX_RX) {
        spi_x->ctrl1_bit.slben = TRUE;
        spi_x->ctrl1_bit.slbtd = FALSE;
        spi_x->ctrl1_bit.ora = FALSE;
    } else if(spi_Init_struct->transmission_Mode == SPI_Transmit_HALF_DUPLEX_TX) {
        spi_x->ctrl1_bit.slben = TRUE;
        spi_x->ctrl1_bit.slbtd = TRUE;
        spi_x->ctrl1_bit.ora = FALSE;
    }

    spi_x->ctrl1_bit.swcsen = spi_Init_struct->cs_Mode_Selection;

    if((spi_Init_struct->master_Slave_Mode == SPI_Mode_MASTER) && (spi_Init_struct->cs_Mode_Selection == SPI_CS_Software_Mode)) {
        spi_x->ctrl1_bit.swcsil = TRUE;
    } else {
        spi_x->ctrl1_bit.swcsil = FALSE;
    }

    spi_x->ctrl1_bit.msten = spi_Init_struct->master_Slave_Mode;

    if(spi_Init_struct->mclk_Freq_division <= SPI_MCLK_DIV_256) {
        spi_x->ctrl2_bit.mdiv3en = FALSE;
        spi_x->ctrl2_bit.mdiv_h = FALSE;
        spi_x->ctrl1_bit.mdiv_l = spi_Init_struct->mclk_Freq_division;
    } else if(spi_Init_struct->mclk_Freq_division == SPI_MCLK_DIV_3) {
        spi_x->ctrl2_bit.mdiv3en = TRUE;
        spi_x->ctrl2_bit.mdiv_h = FALSE;
        spi_x->ctrl1_bit.mdiv_l = 0;
    } else {
        spi_x->ctrl2_bit.mdiv3en = FALSE;
        spi_x->ctrl2_bit.mdiv_h = TRUE;
        spi_x->ctrl1_bit.mdiv_l = spi_Init_struct->mclk_Freq_division & 0x7;
    }

    spi_x->ctrl1_bit.ltf = spi_Init_struct->first_Bit_transmission;
    spi_x->ctrl1_bit.fbn = spi_Init_struct->frame_Bit_Num;
    spi_x->ctrl1_bit.clkpol = spi_Init_struct->clock_polarity;
    spi_x->ctrl1_bit.clkpha = spi_Init_struct->clock_phase;
}

/**
  * @brief  enable or disable the ti mode for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of ti mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_ti_Mode_Enable(spi_Type* spi_x, confirm_state new_state) {
    spi_x->ctrl2_bit.tien = new_state;
}

/**
  * @brief  spi next transmit crc for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @retval none
  */
void spi_CRC_next_transmit(spi_Type* spi_x) {
    spi_x->ctrl1_bit.ntc = TRUE;
}

/**
  * @brief  set the crc polynomial value for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  CRC_poly: crc polynomial value.
  * @retval none
  */
void spi_CRC_polynomial_Set(spi_Type* spi_x, uint16_t CRC_poly) {
    spi_x->cpoly_bit.cpoly = CRC_poly;
}

/**
  * @brief  return the crc polynomial register value for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @retval the select crc polynomial register value
  */
uint16_t spi_CRC_polynomial_Get(spi_Type* spi_x) {
    return spi_x->cpoly_bit.cpoly;
}

/**
  * @brief  enable or disable the hardware crc calculation for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of crc calculation.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_CRC_Enable(spi_Type* spi_x, confirm_state new_state) {
    spi_x->ctrl1_bit.ccen = new_state;
}

/**
  * @brief  return the transmit or the receive crc value for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  CRC_direction: select transmit or receive crc value to be read
  *         - SPI_CRC_RX
  *         - SPI_CRC_TX
  * @retval the select crc register value
  */
uint16_t spi_CRC_Value_Get(spi_Type* spi_x, spi_CRC_direction_Type CRC_direction) {
    if(CRC_direction == SPI_CRC_RX)
        return  spi_x->rCRC_bit.rcrc;
    else
        return  spi_x->tCRC_bit.tcrc;
}

/**
  * @brief  enable or disable the hardware cs output for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of spi master cs output.
  *         this parameter can be: TRUE or FALSE.
  *         note:the bit only use in spi master mode
  * @retval none
  */
void spi_hardware_cs_OutPut_Enable(spi_Type* spi_x, confirm_state new_state) {
    spi_x->ctrl2_bit.hwcsoe = new_state;
}

/**
  * @brief  set the software cs internal level for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  level: set the state of spi cs level.
  *         this parameter can be one of the following values:
  *         - SPI_SWCS_Internal_LEVEL_LOW
  *         - SPI_SWCS_Internal_LEVEL_HIGHT
  *         note:the bit only use when swcsen bit is set.
  *         note:when use this bit,io operation on the cs pin are invalid.
  * @retval none
  */
void spi_Software_cs_Internal_level_Set(spi_Type* spi_x, spi_Software_cs_level_Type level) {
    spi_x->ctrl1_bit.swcsil = level;
}

/**
  * @brief  set the data frame bit num for the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  bit_Num: set the data frame size
  *         - SPI_FRAME_8BIT
  *         - SPI_FRAME_16BIT
  * @retval none
  */
void spi_frame_Bit_Num_Set(spi_Type* spi_x, spi_frame_Bit_Num_Type bit_Num) {
    spi_x->ctrl1_bit.fbn = bit_Num;
}

/**
  * @brief  set the data transmission direction in single line bidirectiona half duplex mode of the spi peripheral.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  direction: data transfer direction
  *         this parameter can be one of the following values:
  *         - SPI_HALF_DUPLEX_DIRECTION_RX
  *         - SPI_HALF_DUPLEX_DIRECTION_TX
  * @retval none
  */
void spi_half_duplex_direction_Set(spi_Type* spi_x, spi_half_duplex_direction_Type direction) {
    spi_x->ctrl1_bit.slbtd = direction;
}

/**
  * @brief  enable or disable spi.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4
  * @param  new_state: new state of spi.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_Enable(spi_Type* spi_x, confirm_state new_state) {
    spi_x->ctrl1_bit.spien = new_state;
}

/**
  * @brief  i2s init config with its default value.
  * @param  i2s_Init_struct : pointer to a i2s_Init_Type structure which will
  *         be initialized.
  * @retval none
  */
void i2s_Default_Para_Init(i2s_Init_Type* i2s_Init_struct) {
    i2s_Init_struct->operation_Mode = I2S_Mode_Slave_TX;
    i2s_Init_struct->audio_protocol = I2S_AUDIO_PROTOCOL_PHILLIPS;
    i2s_Init_struct->audio_sampling_freq = I2S_AUDIO_Frequency_DEFAULT;
    i2s_Init_struct->data_Channel_format = I2S_Data_16BIT_Channel_16BIT;
    i2s_Init_struct->clock_polarity = I2S_Clock_POLARITY_LOW;
    i2s_Init_struct->mclk_OutPut_Enable = FALSE;
}

/**
  * @brief  i2s init config with its setting value.
  * @param  spi_x: select the spi peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @param  i2s_Init_struct : pointer to a i2s_Init_Type structure which will be initialized.
  * @retval none
  */
void i2s_Init(spi_Type* spi_x, i2s_Init_Type* i2s_Init_struct) {
    crm_clocks_Freq_Type clocks_freq;
    uint32_t i2s_sclk_index = 0;
    uint32_t i2sdiv_index = 2, i2sodd_index = 0, frequency_index = 0;

    /* i2s audio frequency config */
    if(i2s_Init_struct->audio_sampling_freq == I2S_AUDIO_Frequency_DEFAULT) {
        i2sodd_index = 0;
        i2sdiv_index = 2;
    } else {
        crm_clocks_Freq_Get(&clocks_freq);
        i2s_sclk_index = clocks_freq.sclk_freq;

        if((i2s_Init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PCM_SHORT) || (i2s_Init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PCM_LONG)) {
            if(i2s_Init_struct->mclk_OutPut_Enable == TRUE) {
                frequency_index = (((i2s_sclk_index / 128) * 10) / i2s_Init_struct->audio_sampling_freq) + 5;
            } else {
                if(i2s_Init_struct->data_Channel_format == I2S_Data_16BIT_Channel_16BIT)
                    frequency_index = (((i2s_sclk_index / 16) * 10) / i2s_Init_struct->audio_sampling_freq) + 5;
                else
                    frequency_index = (((i2s_sclk_index / 32) * 10) / i2s_Init_struct->audio_sampling_freq) + 5;
            }
        } else {
            if(i2s_Init_struct->mclk_OutPut_Enable == TRUE) {
                frequency_index = (((i2s_sclk_index / 256) * 10) / i2s_Init_struct->audio_sampling_freq) + 5;
            } else {
                if(i2s_Init_struct->data_Channel_format == I2S_Data_16BIT_Channel_16BIT)
                    frequency_index = (((i2s_sclk_index / 32) * 10) / i2s_Init_struct->audio_sampling_freq) + 5;
                else
                    frequency_index = (((i2s_sclk_index / 64) * 10) / i2s_Init_struct->audio_sampling_freq) + 5;
            }
        }
    }

    frequency_index = frequency_index / 10;
    i2sodd_index = frequency_index & (uint16_t)0x0001;
    i2sdiv_index = (frequency_index - i2sodd_index) / 2;

    if((i2sdiv_index < 2) || (i2sdiv_index > 0x03FF)) {
        i2sodd_index = 0;
        i2sdiv_index = 2;
    }

    spi_x->i2sclk_bit.i2sodd = i2sodd_index;

    if(i2sdiv_index > 0x00FF) {
        spi_x->i2sclk_bit.i2sdiv_h = (i2sdiv_index >> 8) & 0x0003;
        spi_x->i2sclk_bit.i2sdiv_l = i2sdiv_index & 0x00FF;
    } else {
        spi_x->i2sclk_bit.i2sdiv_h = 0;
        spi_x->i2sclk_bit.i2sdiv_l = i2sdiv_index;
    }

    /* i2s audio_protocol set*/
    if(i2s_Init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PCM_LONG) {
        spi_x->i2sctrl_bit.pcmfssel = 1;
        spi_x->i2sctrl_bit.stdsel = 3;
    } else if(i2s_Init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PCM_SHORT) {
        spi_x->i2sctrl_bit.pcmfssel = 0;
        spi_x->i2sctrl_bit.stdsel = 3;
    } else if(i2s_Init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_LSB) {
        spi_x->i2sctrl_bit.pcmfssel = 0;
        spi_x->i2sctrl_bit.stdsel = 2;
    } else if(i2s_Init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_MSB) {
        spi_x->i2sctrl_bit.pcmfssel = 0;
        spi_x->i2sctrl_bit.stdsel = 1;
    } else if(i2s_Init_struct->audio_protocol == I2S_AUDIO_PROTOCOL_PHILLIPS) {
        spi_x->i2sctrl_bit.pcmfssel = 0;
        spi_x->i2sctrl_bit.stdsel = 0;
    }

    /* i2s data_Channel_format set*/
    if(i2s_Init_struct->data_Channel_format == I2S_Data_16BIT_Channel_16BIT) {
        spi_x->i2sctrl_bit.i2scbn = 0;
        spi_x->i2sctrl_bit.i2sdbn = 0;
    } else if(i2s_Init_struct->data_Channel_format == I2S_Data_16BIT_Channel_32BIT) {
        spi_x->i2sctrl_bit.i2scbn = 1;
        spi_x->i2sctrl_bit.i2sdbn = 0;
    } else if(i2s_Init_struct->data_Channel_format == I2S_Data_24BIT_Channel_32BIT) {
        spi_x->i2sctrl_bit.i2scbn = 1;
        spi_x->i2sctrl_bit.i2sdbn = 1;
    } else if(i2s_Init_struct->data_Channel_format == I2S_Data_32BIT_Channel_32BIT) {
        spi_x->i2sctrl_bit.i2scbn = 1;
        spi_x->i2sctrl_bit.i2sdbn = 2;
    }

    spi_x->i2sctrl_bit.i2sclkpol = i2s_Init_struct->clock_polarity;
    spi_x->i2sclk_bit.i2smclkoe = i2s_Init_struct->mclk_OutPut_Enable;
    spi_x->i2sctrl_bit.opersel = i2s_Init_struct->operation_Mode;
    spi_x->i2sctrl_bit.i2smsel = TRUE;
}

/**
  * @brief  enable or disable i2s.
  * @param  spi_x: select the i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @param  new_state: new state of i2s.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void i2s_Enable(spi_Type* spi_x, confirm_state new_state) {
    spi_x->i2sctrl_bit.i2sen = new_state;
}

/**
  * @brief  enable or disable the specified spi/i2s interrupts.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @param  spi_i2s_int: specifies the spi/i2s interrupt sources to be enabled or disabled.
  *         this parameter can be one of the following values:
  *         - SPI_I2S_Error_INT
  *         - SPI_I2S_RDBF_INT
  *         - SPI_I2S_TDBE_INT
  * @param  new_state: new state of the specified spi/i2s interrupts.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_i2s_Interrupt_Enable(spi_Type* spi_x, uint32_t spi_i2s_int, confirm_state new_state) {
    if(new_state != FALSE) {
        spi_x->ctrl2 |= spi_i2s_int;
    } else {
        spi_x->ctrl2 &= ~spi_i2s_int;
    }
}

/**
  * @brief  enable or disable the spi/i2s dma transmitter mode.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @param  new_state: new state of the dma request.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_i2s_DMA_transmitter_Enable(spi_Type* spi_x, confirm_state new_state) {
    spi_x->ctrl2_bit.dmaten = new_state;
}

/**
  * @brief  enable or disable the spi/i2s dma receiver mode.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @param  new_state: new state of the dma request.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void spi_i2s_DMA_Receiver_Enable(spi_Type* spi_x, confirm_state new_state) {
    spi_x->ctrl2_bit.dmaren = new_state;
}

/**
  * @brief  spi/i2s data transmit
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @param  tx_data: the data to be transmit.
  *         this parameter can be:
  *         - (0x0000~0xFFFF)
  * @retval none
  */
void spi_i2s_Data_transmit(spi_Type* spi_x, uint16_t tx_data) {
    spi_x->dt = tx_data;
}

/**
  * @brief  spi/i2s data receive
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @retval the received data value
  */
uint16_t spi_i2s_Data_receive(spi_Type* spi_x) {
    return (uint16_t)spi_x->dt;
}

/**
  * @brief  get flag of the specified spi/i2s peripheral.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @param  spi_i2s_flag: select the spi/i2s flag
  *         this parameter can be one of the following values:
  *         - SPI_I2S_RDBF_FLAG
  *         - SPI_I2S_TDBE_FLAG
  *         - I2S_ACS_FLAG    (this flag only use in i2s mode)
  *         - I2S_TUERR_FLAG  (this flag only use in i2s mode)
  *         - SPI_CCERR_FLAG  (this flag only use in spi mode)
  *         - SPI_MMERR_FLAG  (this flag only use in spi mode)
  *         - SPI_I2S_ROERR_FLAG
  *         - SPI_I2S_BF_FLAG
  *         - SPI_CSPAS_FLAG
  * @retval the new state of spi/i2s flag
  */
flag_status spi_i2s_Flag_Get(spi_Type* spi_x, uint32_t spi_i2s_flag) {
    flag_status status = RESET;

    if ((spi_x->sts & spi_i2s_flag) == RESET) {
        status = RESET;
    } else {
        status = SET;
    }

    return status;
}

/**
  * @brief  clear flag of the specified spi/i2s peripheral.
  * @param  spi_x: select the spi/i2s peripheral.
  *         this parameter can be one of the following values:
  *         SPI1, SPI2, SPI3 ,SPI4 , I2S2EXT, I2S3EXT
  * @param  spi_i2s_flag: select the spi/i2s flag
  *         this parameter can be one of the following values:
  *         - SPI_CCERR_FLAG
  *         - SPI_I2S_RDBF_FLAG
  *         - I2S_TUERR_FLAG
  *         - SPI_MMERR_FLAG
  *         - SPI_I2S_ROERR_FLAG
  *         - SPI_CSPAS_FLAG
  * @note
  *         SPI_I2S_TDBE_FLAG  this flag is cleared when the tx buffer already contain data to be transmit.
  *         I2S_ACS_FLAG       this flag cann't cleared by software,the flag indicate the channel side(not use in pcm standard mode).
  *         SPI_I2S_BF_FLAG    this flag cann't cleared by software, it's set and cleared by hardware.
  * @retval none
  */
void spi_i2s_Flag_Clear(spi_Type* spi_x, uint32_t spi_i2s_flag) {
    if(spi_i2s_flag == SPI_CCERR_FLAG)
        spi_x->sts = ~SPI_CCERR_FLAG;
    else if(spi_i2s_flag == SPI_I2S_RDBF_FLAG)
        UNUSED(spi_x->dt);
    else if(spi_i2s_flag == I2S_TUERR_FLAG)
        UNUSED(spi_x->sts);
    else if(spi_i2s_flag == SPI_CSPAS_FLAG)
        UNUSED(spi_x->sts);
    else if(spi_i2s_flag == SPI_MMERR_FLAG) {
        UNUSED(spi_x->sts);
        spi_x->ctrl1 = spi_x->ctrl1;
    } else if(spi_i2s_flag == SPI_I2S_ROERR_FLAG) {
        UNUSED(spi_x->dt);
        UNUSED(spi_x->sts);
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
