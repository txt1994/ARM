/**
  **************************************************************************
  * @file     at32f435_437_sdio.c
  * @brief    contains all the functions for the sdio firmware library
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

/** @defgroup SDIO
  * @brief SDIO driver modules
  * @{
  */

#ifdef SDIO_MODULE_EnableD

/** @defgroup SDIO_private_functions
  * @{
  */

/**
  * @brief  reset the sdio register
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @retval none
  */
void sdio_Reset(sdio_Type *sdio_x) {
    sdio_x->pwrctrl = 0x0;
    sdio_x->clkctrl = 0x0;
    sdio_x->argu = 0x0;
    sdio_x->cmdctrl = 0x0;
    sdio_x->dttmr = 0x0;
    sdio_x->dtlen = 0x0;
    sdio_x->dtctrl = 0x0;
    sdio_x->inten = 0x0;
    sdio_x->intclr = 0x004007FF;
}

/**
  * @brief  set the power status of the controller
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  power_state
  *         this parameter can be one of the following values:
  *         - SDIO_Power_OFF
  *         - SDIO_Power_ON
  * @retval none
  */
void sdio_Power_Set(sdio_Type *sdio_x, sdio_Power_state_Type power_state) {
    sdio_x->pwrctrl_bit.ps = power_state;
}

/**
  * @brief  get power status.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @retval sdio_Power_state_Type (SDIO_Power_ON or SDIO_Power_OFF)
  */
sdio_Power_state_Type sdio_Power_Status_Get(sdio_Type *sdio_x) {
    return (sdio_Power_state_Type)(sdio_x->pwrctrl_bit.ps);
}

/**
  * @brief  config sdio clock
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  clk_div: sdio clock divide factor(frequency = sdio_clk / [clk_psc + 2]).
  * @param  clk_edg
  *         this parameter can be one of the following values:
  *         - SDIO_Clock_Edge_RISING
  *         - SDIO_Clock_Edge_FALLING
  * @retval none
  */
void sdio_Clock_Config(sdio_Type *sdio_x, uint16_t clk_div, sdio_Edge_phase_Type clk_edg) {
    /* config clock edge */
    sdio_x->clkctrl_bit.clkegs = clk_edg;

    /* config clock divide [7:0] */
    sdio_x->clkctrl_bit.clkdiv_l = (clk_div & 0xFF);

    /* config clock divide [9:8] */
    sdio_x->clkctrl_bit.clkdiv_h = ((clk_div & 0x300) >> 8);
}

/**
  * @brief  config sdio bus width
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  width
  *         this parameter can be one of the following values:
  *         - SDIO_BUS_WIDTH_D1
  *         - SDIO_BUS_WIDTH_D4
  *         - SDIO_BUS_WIDTH_D8
  * @retval none
  */
void sdio_bus_width_Config(sdio_Type *sdio_x, sdio_bus_width_Type width) {
    sdio_x->clkctrl_bit.busws = width;
}

/**
  * @brief  enable or disable clock divider bypss
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_Clock_bypass(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->clkctrl_bit.bypsen = new_state;
}

/**
  * @brief  enable or disable power saving mode, config sdio_ck clock output
  *         when the bus is idle.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_Power_saving_Mode_Enable(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->clkctrl_bit.pwrsven = new_state;
}

/**
  * @brief  enable or disable hardware flow control.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_flow_Control_Enable(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->clkctrl_bit.hfcen = new_state;
}

/**
  * @brief  enable or disable sdio_ck output.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_Clock_Enable(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->clkctrl_bit.clkoen = new_state;
}

/**
  * @brief  enable or disable dma.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_DMA_Enable(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->dtctrl_bit.dmaen = new_state;
}

/**
  * @brief  config corresponding interrupt.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  int_opt
  *         this parameter can be one of the following values:
  *         - SDIO_CMDFAIL_INT
  *         - SDIO_DTFAIL_INT
  *         - SDIO_CMDTIMEOUT_INT
  *         - SDIO_DTTIMEOUT_INT
  *         - SDIO_TXERRU_INT
  *         - SDIO_RXERRO_INT
  *         - SDIO_CMDRSPCMPL_INT
  *         - SDIO_CMDCMPL_INT
  *         - SDIO_DTCMP_INT
  *         - SDIO_SBITERR_INT
  *         - SDIO_DTBLKCMPL_INT
  *         - SDIO_DOCMD_INT
  *         - SDIO_DOTX_INT
  *         - SDIO_DORX_INT
  *         - SDIO_TXBUFH_INT
  *         - SDIO_RXBUFH_INT
  *         - SDIO_TXBUFF_INT
  *         - SDIO_RXBUFF_INT
  *         - SDIO_TXBUFE_INT
  *         - SDIO_RXBUFE_INT
  *         - SDIO_TXBUF_INT
  *         - SDIO_RXBUF_INT
  *         - SDIO_SDIOIF_INT
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_Interrupt_Enable(sdio_Type *sdio_x, uint32_t int_opt,  confirm_state new_state) {
    /* enable interrupt */
    if(TRUE == new_state) {
        sdio_x->inten |= int_opt;
    }
    /* disable interrupt */
    else {
        sdio_x->inten &= ~(int_opt);
    }
}

/**
  * @brief  get sdio flag.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  flag
  *         this parameter can be one of the following values:
  *         - SDIO_CMDFAIL_FLAG
  *         - SDIO_DTFAIL_FLAG
  *         - SDIO_CMDTIMEOUT_FLAG
  *         - SDIO_DTTIMEOUT_FLAG
  *         - SDIO_TXERRU_FLAG
  *         - SDIO_RXERRO_FLAG
  *         - SDIO_CMDRSPCMPL_FLAG
  *         - SDIO_CMDCMPL_FLAG
  *         - SDIO_DTCMPL_FLAG
  *         - SDIO_SBITERR_FLAG
  *         - SDIO_DTBLKCMPL_FLAG
  *         - SDIO_DOCMD_FLAG
  *         - SDIO_DOTX_FLAG
  *         - SDIO_DORX_FLAG
  *         - SDIO_TXBUFH_FLAG
  *         - SDIO_RXBUFH_FLAG
  *         - SDIO_TXBUFF_FLAG
  *         - SDIO_RXBUFF_FLAG
  *         - SDIO_TXBUFE_FLAG
  *         - SDIO_RXBUFE_FLAG
  *         - SDIO_TXBUF_FLAG
  *         - SDIO_RXBUF_FLAG
  *         - SDIO_SDIOIF_FLAG
  * @retval flag_status (SET or RESET)
  */
flag_status sdio_Flag_Get(sdio_Type *sdio_x, uint32_t flag) {
    flag_status status = RESET;

    if((sdio_x->sts & flag) == flag) {
        status = SET;
    } else {
        status = RESET;
    }

    return status;
}

/**
  * @brief  clear sdio flag.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  int_opt
  *         this parameter can be any combination of the following values:
  *         - SDIO_CMDFAIL_FLAG
  *         - SDIO_DTFAIL_FLAG
  *         - SDIO_CMDTIMEOUT_FLAG
  *         - SDIO_DTTIMEOUT_FLAG
  *         - SDIO_TXERRU_FLAG
  *         - SDIO_RXERRO_FLAG
  *         - SDIO_CMDRSPCMPL_FLAG
  *         - SDIO_CMDCMPL_FLAG
  *         - SDIO_DTCMPL_FLAG
  *         - SDIO_SBITERR_FLAG
  *         - SDIO_DTBLKCMPL_FLAG
  *         - SDIO_SDIOIF_FLAG
  * @retval none
  */
void sdio_Flag_Clear(sdio_Type *sdio_x, uint32_t flag) {
    sdio_x->intclr = flag;
}

/**
  * @brief  config sdio command.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  command_struct : pointer to a sdio_command_struct_Type structure
  *         that contains the configuration information for the sdio command.
  * @retval none
  */
void sdio_command_Config(sdio_Type *sdio_x, sdio_command_struct_Type *command_struct) {
    /* disable command path state machine */
    sdio_x->cmdctrl_bit.ccsmen = FALSE;

    /* config command argument */
    sdio_x->argu = command_struct->argument;

    /* config command register */
    sdio_x->cmdctrl_bit.cmdidx = command_struct->cmd_index;
    sdio_x->cmdctrl_bit.rspwt = command_struct->rsp_Type;
    sdio_x->cmdctrl_bit.intwt = (command_struct->wait_Type & 0x1); /* [1:0] -> [0] */
    sdio_x->cmdctrl_bit.pndwt = (command_struct->wait_Type & 0x2) >> 1; /* [1:0] -> [1] */
}

/**
  * @brief  enable or disable command path state machine(CPSM).
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_command_state_machine_Enable(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->cmdctrl_bit.ccsmen = new_state;
}

/**
  * @brief  get command index of last command for which response received.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval uint8_t: command index
  */
uint8_t sdio_command_response_Get(sdio_Type *sdio_x) {
    return sdio_x->rspcmd_bit.rspcmd;
}

/**
  * @brief  get response received from the card for the last command.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  reg_index
  *         this parameter can be one of the following values:
  *         - SDIO_RSP1_INDEX
  *         - SDIO_RSP2_INDEX
  *         - SDIO_RSP3_INDEX
  *         - SDIO_RSP4_INDEX
  * @retval uint32_t: response register value
  */
uint32_t sdio_response_Get(sdio_Type *sdio_x, sdio_rsp_index_Type reg_index) {
    uint32_t response_value = 0;

    switch(reg_index) {
        case SDIO_RSP1_INDEX:
            response_value = sdio_x->rsp1;
            break;

        case SDIO_RSP2_INDEX:
            response_value = sdio_x->rsp2;
            break;

        case SDIO_RSP3_INDEX:
            response_value = sdio_x->rsp3;
            break;

        case SDIO_RSP4_INDEX:
            response_value = sdio_x->rsp4;
            break;

        default:
            break;
    }

    return response_value;
}

/**
  * @brief  config sdio data.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  data_struct : pointer to a sdio_Data_struct_Type structure
  *         that contains the configuration information for the sdio data.
  * @retval none
  */
void sdio_Data_Config(sdio_Type *sdio_x, sdio_Data_struct_Type *data_struct) {
    /* disable data path state machine */
    sdio_x->dtctrl_bit.tfren = FALSE;

    /* config data block, transfer mode and transfer direction */
    sdio_x->dtctrl_bit.blksize = data_struct->block_size;
    sdio_x->dtctrl_bit.tfrdir = data_struct->transfer_direction;
    sdio_x->dtctrl_bit.tfrmode = data_struct->transfer_Mode;

    /* config data length */
    sdio_x->dtlen_bit.dtlen = data_struct->data_length;

    /* config data transfer timeout */
    sdio_x->dttmr_bit.timeout = data_struct->timeout;
}

/**
  * @brief  enable or disable data path state machine(DPSM).
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_Data_state_machine_Enable(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->dtctrl_bit.tfren = new_state;
}

/**
  * @brief  get the number of remaining data bytes to be transferred.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @retval uint32_t: number of bytes
  */
uint32_t sdio_Data_Counter_Get(sdio_Type *sdio_x) {
    return sdio_x->dtcnt;
}

/**
  * @brief  read a word data from sdio fifo.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @retval uint32_t: data received
  */
uint32_t sdio_Data_Read(sdio_Type *sdio_x) {
    return sdio_x->buf;
}

/**
  * @brief  get the number of words left to be written to or read from fifo..
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @retval uint32_t: number of words
  */
uint32_t sdio_buffer_Counter_Get(sdio_Type *sdio_x) {
    return sdio_x->bufcnt;
}

/**
  * @brief  write one word data to fifo.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  data: data to be transferred.
  * @retval none
  */
void sdio_Data_Write(sdio_Type *sdio_x, uint32_t data) {
    sdio_x->buf = data;
}

/**
  * @brief  set the read wait mode.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  mode
  *         this parameter can be one of the following values:
  *         - SDIO_Read_Wait_CONTROLLED_BY_D2
  *         - SDIO_Read_Wait_CONTROLLED_BY_CK
  * @retval none
  */
void sdio_Read_Wait_Mode_Set(sdio_Type *sdio_x, sdio_Read_Wait_Mode_Type mode) {
    sdio_x->dtctrl_bit.rdwtmode = mode;
}

/**
  * @brief  enable or disable to start sd i/o read wait operation.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_Read_Wait_Start(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->dtctrl_bit.rdwtstart = new_state;
}

/**
  * @brief  enable or disable to stop sd i/o read wait operation.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_Read_Wait_stop(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->dtctrl_bit.rdwtstop = new_state;
}

/**
  * @brief  enable or disable the sd i/o function.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_io_function_Enable(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->dtctrl_bit.ioen = new_state;
}

/**
  * @brief  enable or disable sd i/o suspend command sending.
  * @param  sdio_x: to select the sdio peripheral.
  *         this parameter can be one of the following values:
  *         SDIO1, SDIO2.
  * @param  new_state (TRUE or FALSE)
  * @retval none
  */
void sdio_io_suspend_command_Set(sdio_Type *sdio_x, confirm_state new_state) {
    sdio_x->cmdctrl_bit.iosusp = new_state;
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
