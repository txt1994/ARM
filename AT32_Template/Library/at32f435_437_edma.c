/**
  **************************************************************************
  * @file     at32f435_437_edma.c
  * @brief    contains all the functions for the edma firmware library
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

/** @defgroup EDMA
  * @brief EDMA driver modules
  * @{
  */

#ifdef EDMA_MODULE_EnableD

/** @defgroup EDMA_private_functions
  * @{
  */

/**
  * @brief  reset eDMA_streamx channely register.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @retval none.
  */
void eDMA_Reset(eDMA_Stream_Type *eDMA_streamx) {
    /* reset registers for the selected stream */
    eDMA_streamx->ctrl_bit.sen = FALSE;
    eDMA_streamx->ctrl = 0x0;
    eDMA_streamx->dtcnt = 0x0;
    eDMA_streamx->paddr = 0x0;
    eDMA_streamx->m0addr = 0x0;
    eDMA_streamx->m1addr = 0x0;
    eDMA_streamx->fctrl = (uint32_t)0x00000021;

    /* reset interrupt pending bits for the selected stream */
    switch((uint32_t)eDMA_streamx) {
        case EDMA_STREAM1_BASE:
            EDMA->clr1 = EDMA_STREAM1_INT_MASK;
            break;

        case EDMA_STREAM2_BASE:
            EDMA->clr1 = EDMA_STREAM2_INT_MASK;
            break;

        case EDMA_STREAM3_BASE:
            EDMA->clr1 = EDMA_STREAM3_INT_MASK;
            break;

        case EDMA_STREAM4_BASE:
            EDMA->clr1 = EDMA_STREAM4_INT_MASK;
            break;

        case EDMA_STREAM5_BASE:
            EDMA->clr2 = EDMA_STREAM5_INT_MASK;
            break;

        case EDMA_STREAM6_BASE:
            EDMA->clr2 = EDMA_STREAM6_INT_MASK;
            break;

        case EDMA_STREAM7_BASE:
            EDMA->clr2 = EDMA_STREAM7_INT_MASK;
            break;

        case EDMA_STREAM8_BASE:
            EDMA->clr2 = EDMA_STREAM8_INT_MASK;
            break;

        default:
            break;
    }
}

/**
  * @brief  edma init.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  eDMA_Init_struct: pointer to a eDMA_Init_Type structure.
  * @retval none.
  */
void eDMA_Init(eDMA_Stream_Type *eDMA_streamx, eDMA_Init_Type *eDMA_Init_struct) {
    /* config dtd bits */
    eDMA_streamx->ctrl_bit.dtd = eDMA_Init_struct->direction;

    /* config pincm bit */
    eDMA_streamx->ctrl_bit.pincm = eDMA_Init_struct->peripheral_inc_Enable;

    /* config mincm bit*/
    eDMA_streamx->ctrl_bit.mincm = eDMA_Init_struct->memory_inc_Enable;

    /* config pwidth bits */
    eDMA_streamx->ctrl_bit.pwidth = eDMA_Init_struct->peripheral_Data_width;

    /* config mwidth bits */
    eDMA_streamx->ctrl_bit.mwidth = eDMA_Init_struct->memory_Data_width;

    /* config lm bit */
    eDMA_streamx->ctrl_bit.lm = eDMA_Init_struct->loop_Mode_Enable;

    /* config spl bits */
    eDMA_streamx->ctrl_bit.spl = eDMA_Init_struct->priority;

    /* config mct bits */
    eDMA_streamx->ctrl_bit.mct = eDMA_Init_struct->memory_burst_mode;

    /* config pct bits */
    eDMA_streamx->ctrl_bit.pct = eDMA_Init_struct->peripheral_burst_mode;

    /* config fen bits */
    eDMA_streamx->fctrl_bit.fen = eDMA_Init_struct->fifo_Mode_Enable;

    /* config fthsel bits*/
    eDMA_streamx->fctrl_bit.fthsel = eDMA_Init_struct->fifo_threshold;

    /* config dtcnt */
    eDMA_streamx->dtcnt = eDMA_Init_struct->buffer_size;

    /* config paddr */
    eDMA_streamx->paddr = eDMA_Init_struct->peripheral_base_addr;

    /* config m0addr */
    eDMA_streamx->m0addr = eDMA_Init_struct->memory0_base_addr;
}

/**
  * @brief  edma init struct config with its default value.
  * @param  eDMA_Init_struct: pointer to a eDMA_Init_Type structure which will be initialized.
  * @retval none.
  */
void eDMA_Default_Para_Init(eDMA_Init_Type *eDMA_Init_struct) {
    eDMA_Init_struct->buffer_size = 0;
    eDMA_Init_struct->loop_Mode_Enable = FALSE;
    eDMA_Init_struct->direction = EDMA_DIR_PERIPHERAL_TO_MEMORY;
    eDMA_Init_struct->fifo_threshold = EDMA_FIFO_THRESHOLD_1QUARTER;
    eDMA_Init_struct->fifo_Mode_Enable = FALSE;
    eDMA_Init_struct->memory0_base_addr = 0;
    eDMA_Init_struct->memory_burst_mode = EDMA_Memory_SINGLE;
    eDMA_Init_struct->memory_Data_width = EDMA_Memory_Data_WIDTH_BYTE;
    eDMA_Init_struct->memory_inc_Enable = FALSE;
    eDMA_Init_struct->peripheral_base_addr = 0;
    eDMA_Init_struct->peripheral_burst_mode = EDMA_PERIPHERAL_SINGLE;
    eDMA_Init_struct->peripheral_Data_width = EDMA_PERIPHERAL_Data_WIDTH_BYTE;
    eDMA_Init_struct->peripheral_inc_Enable = FALSE;
    eDMA_Init_struct->priority = EDMA_PRIORITY_LOW;
}

/**
  * @brief  enable or disable the edma streamx.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMA_Stream_Enable(eDMA_Stream_Type *eDMA_streamx, confirm_state new_state) {
    eDMA_streamx->ctrl_bit.sen = new_state;
}

/**
  * @brief  enable or disable the edma streamx interrupts.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  eDMA_int:
  *         this parameter can be one of the following values:
  *         - EDMA_FDT_INT
  *         - EDMA_HDT_INT
  *         - EDMA_DTERR_INT
  *         - EDMA_DMERR_INT
  *         - EDMA_FERR_INT
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMA_Interrupt_Enable(eDMA_Stream_Type *eDMA_streamx, uint32_t eDMA_int, confirm_state new_state) {
    if((eDMA_int & EDMA_FERR_INT) != 0) {
        if(new_state != FALSE) {
            eDMA_streamx->fctrl |= (uint32_t)EDMA_FERR_INT;
        } else {
            eDMA_streamx->fctrl &= ~(uint32_t)EDMA_FERR_INT;
        }
    }

    if(eDMA_int != EDMA_FERR_INT) {
        if(new_state != FALSE) {
            eDMA_streamx->ctrl |= (uint32_t)eDMA_int;
        } else {
            eDMA_streamx->ctrl &= ~(uint32_t)eDMA_int;
        }
    }
}

/**
  * @brief  config the edma peripheral increment offset size mode.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  offset: peripheral increment offset size.
  *         this parameter can be one of the following values:
  *         - EDMA_PERIPHERAL_INC_PSIZE
  *         - EDMA_PERIPHERAL_INC_4_BYTE
  * @retval none.
  */
void eDMA_peripheral_inc_offset_Set(eDMA_Stream_Type *eDMA_streamx, eDMA_peripheral_inc_offset_Type offset) {
    eDMA_streamx->ctrl_bit.pincos = offset;
}

/**
  * @brief  enable or disable the edma streamx flow controller.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMA_flow_Controller_Enable(eDMA_Stream_Type *eDMA_streamx, confirm_state new_state) {
    eDMA_streamx->ctrl_bit.pfctrl = new_state;
}

/**
  * @brief  set the number of data to be transferred.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  data_Number: the number of data to be transferred (0x0000~0xFFFF).
  * @retval none.
  */
void eDMA_Data_Number_Set(eDMA_Stream_Type *eDMA_streamx, uint16_t data_Number) {
    /* write the number of data units to be transferred */
    eDMA_streamx->dtcnt = data_Number;
}

/**
  * @brief  get the number of data to be transferred.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @retval the number value.
  */
uint16_t eDMA_Data_Number_Get(eDMA_Stream_Type *eDMA_streamx) {
    return ((uint16_t)(eDMA_streamx->dtcnt));
}

/**
  * @brief  config the the double buffer mode.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  memory1_addr: the address of the second buffer.
  * @param  current_memory: specifies the target area of the first transfer.
  *         this parameter can be one of the following values:
  *         - EDMA_Memory_0
  *         - EDMA_Memory_1
  * @retval none.
  */
void eDMA_Double_buffer_Mode_Init(eDMA_Stream_Type *eDMA_streamx, uint32_t memory1_addr, eDMA_Memory_Type current_memory) {
    if(current_memory != EDMA_Memory_0) {
        eDMA_streamx->ctrl_bit.cm = 1;
    } else {
        eDMA_streamx->ctrl_bit.cm = 0;
    }

    eDMA_streamx->m1addr = memory1_addr;
}

/**
  * @brief  enable or disable the double memory mode.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMA_Double_buffer_Mode_Enable(eDMA_Stream_Type *eDMA_streamx, confirm_state new_state) {
    if(new_state != FALSE) {
        eDMA_streamx->ctrl_bit.dmm = 1;
    } else {
        eDMA_streamx->ctrl_bit.dmm = 0;
    }
}

/**
  * @brief  config the memory address in double buffer mode.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @param  memory_addr: the address of the buffer.
  * @param  memory_target: indicates the which memory addr register will be config.
  *         this parameter can be one of the following values:
  *         - EDMA_Memory_0
  *         - EDMA_Memory_1
  * @retval none.
  */
void eDMA_Memory_Addr_Set(eDMA_Stream_Type *eDMA_streamx, uint32_t memory_addr, uint32_t memory_target) {
    if(memory_target != EDMA_Memory_0) {
        eDMA_streamx->m1addr = memory_addr;
    } else {
        eDMA_streamx->m0addr = memory_addr;
    }
}

/**
  * @brief  get the current memory target.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @retval the memory target
  *         - EDMA_Memory_0
  *         - EDMA_Memory_1
  */
eDMA_Memory_Type eDMA_Memory_Target_Get(eDMA_Stream_Type *eDMA_streamx) {
    return (eDMA_Memory_Type)(eDMA_streamx->ctrl_bit.cm);
}

/**
  * @brief  get the enable status of edma streamx.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @retval current state of the edma streamx (SET or RESET).
  */
flag_status eDMA_Stream_Status_Get(eDMA_Stream_Type *eDMA_streamx) {
    if((eDMA_streamx->ctrl_bit.sen) != RESET) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  get the fifo level status.
  * @param  eDMA_streamx:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1
  *         - EDMA_STREAM2
  *         - EDMA_STREAM3
  *         - EDMA_STREAM4
  *         - EDMA_STREAM5
  *         - EDMA_STREAM6
  *         - EDMA_STREAM7
  *         - EDMA_STREAM8
  * @retval the fifo filling state.
  *         - EDMA_FIFO_Status_LESS_1QUARTER: (0)   <  fifo level < (1/4).
  *         - EDMA_FIFO_Status_1QUARTER:      (1/4) <= fifo level < (1/2) .
  *         - EDMA_FIFO_Status_HALF:          (1/2) <= fifo level < (3/4).
  *         - EDMA_FIFO_Status_3QUARTER:      (3/4) <= fifo level < (1).
  *         - EDMA_FIFO_Status_EMPTY: fifo is empty.
  *         - EDMA_FIFO_Status_FULL: fifo is full.
  */
uint8_t eDMA_FIFO_Status_Get(eDMA_Stream_Type *eDMA_streamx) {
    return (uint8_t)(eDMA_streamx->fctrl_bit.fsts);
}

/**
  * @brief  get the edma flag.
  * @param  eDMA_flag:
  *         this parameter can be one of the following values:
  *         - EDMA_FERR1_FLAG   - EDMA_DMERR1_FLAG  - EDMA_DTERR1_FLAG  - EDMA_HDT1_FLAG   - EDMA_FDT1_FLAG
  *         - EDMA_FERR2_FLAG   - EDMA_DMERR2_FLAG  - EDMA_DTERR2_FLAG  - EDMA_HDT2_FLAG   - EDMA_FDT2_FLAG
  *         - EDMA_FERR3_FLAG   - EDMA_DMERR3_FLAG  - EDMA_DTERR3_FLAG  - EDMA_HDT3_FLAG   - EDMA_FDT3_FLAG
  *         - EDMA_FERR4_FLAG   - EDMA_DMERR4_FLAG  - EDMA_DTERR4_FLAG  - EDMA_HDT4_FLAG   - EDMA_FDT4_FLAG
  *         - EDMA_FERR5_FLAG   - EDMA_DMERR5_FLAG  - EDMA_DTERR5_FLAG  - EDMA_HDT5_FLAG   - EDMA_FDT5_FLAG
  *         - EDMA_FERR6_FLAG   - EDMA_DMERR6_FLAG  - EDMA_DTERR6_FLAG  - EDMA_HDT6_FLAG   - EDMA_FDT6_FLAG
  *         - EDMA_FERR7_FLAG   - EDMA_DMERR7_FLAG  - EDMA_DTERR7_FLAG  - EDMA_HDT7_FLAG   - EDMA_FDT7_FLAG
  *         - EDMA_FERR8_FLAG   - EDMA_DMERR8_FLAG  - EDMA_DTERR8_FLAG  - EDMA_HDT8_FLAG   - EDMA_FDT8_FLAG
  * @retval the new state of edma flag (SET or RESET).
  */
flag_status eDMA_Flag_Get(uint32_t eDMA_flag) {
    uint32_t status;

    if(eDMA_flag > ((uint32_t)0x20000000)) {
        status = EDMA->sts2;
    } else {
        status = EDMA->sts1;
    }

    if((status & eDMA_flag) != ((uint32_t)RESET)) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  clear the edma flag.
  * @param  eDMA_flag:
  *         this parameter can be one of the following values:
  *         - EDMA_FERR1_FLAG   - EDMA_DMERR1_FLAG  - EDMA_DTERR1_FLAG  - EDMA_HDT1_FLAG   - EDMA_FDT1_FLAG
  *         - EDMA_FERR2_FLAG   - EDMA_DMERR2_FLAG  - EDMA_DTERR2_FLAG  - EDMA_HDT2_FLAG   - EDMA_FDT2_FLAG
  *         - EDMA_FERR3_FLAG   - EDMA_DMERR3_FLAG  - EDMA_DTERR3_FLAG  - EDMA_HDT3_FLAG   - EDMA_FDT3_FLAG
  *         - EDMA_FERR4_FLAG   - EDMA_DMERR4_FLAG  - EDMA_DTERR4_FLAG  - EDMA_HDT4_FLAG   - EDMA_FDT4_FLAG
  *         - EDMA_FERR5_FLAG   - EDMA_DMERR5_FLAG  - EDMA_DTERR5_FLAG  - EDMA_HDT5_FLAG   - EDMA_FDT5_FLAG
  *         - EDMA_FERR6_FLAG   - EDMA_DMERR6_FLAG  - EDMA_DTERR6_FLAG  - EDMA_HDT6_FLAG   - EDMA_FDT6_FLAG
  *         - EDMA_FERR7_FLAG   - EDMA_DMERR7_FLAG  - EDMA_DTERR7_FLAG  - EDMA_HDT7_FLAG   - EDMA_FDT7_FLAG
  *         - EDMA_FERR8_FLAG   - EDMA_DMERR8_FLAG  - EDMA_DTERR8_FLAG  - EDMA_HDT8_FLAG   - EDMA_FDT8_FLAG
  * @retval none.
  */
void eDMA_Flag_Clear(uint32_t eDMA_flag) {
    if(eDMA_flag > ((uint32_t)0x20000000)) {
        EDMA->clr2 = (uint32_t)(eDMA_flag & 0x0FFFFFFF);
    } else {
        EDMA->clr1 = eDMA_flag;
    }
}

/**
  * @brief  initialize the edma 2d mode.
  * @param  eDMA_streamx_2d:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1_2D
  *         - EDMA_STREAM2_2D
  *         - EDMA_STREAM3_2D
  *         - EDMA_STREAM4_2D
  *         - EDMA_STREAM5_2D
  *         - EDMA_STREAM6_2D
  *         - EDMA_STREAM7_2D
  *         - EDMA_STREAM8_2D
  * @param  src_stride: source stride(-32768 ~ 32767).
  * @param  dst_stride: destination stride(-32768 ~ 32767).
  * @param  xcnt: x dimension transfer count(0x0000~ 0xFFFF).
  * @param  ycnt: y dimension transfer count(0x0000~ 0xFFFF).
  * @retval none.
  */
void eDMA_2D_Init(eDMA_Stream_2D_Type *eDMA_streamx_2d, int16_t src_stride, int16_t dst_stride, uint16_t xcnt, uint16_t ycnt) {
    eDMA_streamx_2d->s2dcnt = (uint32_t)((ycnt << 16) | (xcnt));

    eDMA_streamx_2d->stride = (uint32_t)((dst_stride << 16) | (src_stride));
}

/**
  * @brief  enable or disable the edma 2d mode.
  * @param  eDMA_streamx_2d:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1_2D
  *         - EDMA_STREAM2_2D
  *         - EDMA_STREAM3_2D
  *         - EDMA_STREAM4_2D
  *         - EDMA_STREAM5_2D
  *         - EDMA_STREAM6_2D
  *         - EDMA_STREAM7_2D
  *         - EDMA_STREAM8_2D
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMA_2D_Enable(eDMA_Stream_2D_Type *eDMA_streamx_2d, confirm_state new_state) {
    uint32_t offset;

    offset = ((uint32_t)eDMA_streamx_2d - EDMA_STREAM1_2D_BASE) / 4;

    if(new_state != FALSE) {
        EDMA->s2dctrl |= (uint16_t)0x0001 << offset;
    } else {
        EDMA->s2dctrl &= ~((uint16_t)0x0001 << offset);
    }
}

/**
  * @brief  initialize the edma link list.
  * @param  eDMA_streamx_ll:
  *         this parameter can be one of the following values:
  *         - EDMA_STREAM1_LL
  *         - EDMA_STREAM2_LL
  *         - EDMA_STREAM3_LL
  *         - EDMA_STREAM4_LL
  *         - EDMA_STREAM5_LL
  *         - EDMA_STREAM6_LL
  *         - EDMA_STREAM7_LL
  *         - EDMA_STREAM8_LL
  * @param  pointer: link list pointer.
  * @retval none.
  */
void eDMA_Link_List_Init(eDMA_Stream_Link_List_Type *eDMA_streamx_ll, uint32_t pointer) {
    eDMA_streamx_ll->llp = pointer;
}

/**
  * @brief  enable or disable the edma stream link list mode.
  * @param  eDMA_streamx_ll:
  *         this parameter can be any combination of the following values:
  *         - EDMA_STREAM1_LL
  *         - EDMA_STREAM2_LL
  *         - EDMA_STREAM3_LL
  *         - EDMA_STREAM4_LL
  *         - EDMA_STREAM5_LL
  *         - EDMA_STREAM6_LL
  *         - EDMA_STREAM7_LL
  *         - EDMA_STREAM8_LL
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMA_Link_List_Enable(eDMA_Stream_Link_List_Type *eDMA_streamx_ll, confirm_state new_state) {
    uint32_t offset;

    offset = ((uint32_t)eDMA_streamx_ll - EDMA_STREAM1_LL_BASE) / 4;

    if(new_state != FALSE) {
        EDMA->llctrl |= (uint16_t)0x0001 << offset;
    } else {
        EDMA->llctrl &= ~((uint16_t)0x0001 << offset);
    }
}

/**
  * @brief  enable or disable the edma edmamux.
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMAMUX_Enable(confirm_state new_state) {
    EDMA->muxsel_bit.tblsel = new_state;
}

/**
  * @brief  edmamux init.
  * @param  eDMAMUX_channelx:
  *         this parameter can be one of the following values:
  *         - EDMAMUX_CHANNEL1
  *         - EDMAMUX_CHANNEL2
  *         - EDMAMUX_CHANNEL3
  *         - EDMAMUX_CHANNEL4
  *         - EDMAMUX_CHANNEL5
  *         - EDMAMUX_CHANNEL6
  *         - EDMAMUX_CHANNEL7
  *         - EDMAMUX_CHANNEL8
  * @param  eDMAMUX_req_id:
  *         this parameter can be one of the following values:
  *         - EDMAMUX_DMAREQ_ID_REQ_G1       - EDMAMUX_DMAREQ_ID_REQ_G2       - EDMAMUX_DMAREQ_ID_REQ_G3       - EDMAMUX_DMAREQ_ID_REQ_G4
  *         - EDMAMUX_DMAREQ_ID_ADC1         - EDMAMUX_DMAREQ_ID_ADC2         - EDMAMUX_DMAREQ_ID_ADC3         - EDMAMUX_DMAREQ_ID_DAC1
  *         - EDMAMUX_DMAREQ_ID_DAC2         - EDMAMUX_DMAREQ_ID_TMR6_OVERFLOW- EDMAMUX_DMAREQ_ID_TMR7_OVERFLOW- EDMAMUX_DMAREQ_ID_SPI1_RX
  *         - EDMAMUX_DMAREQ_ID_SPI1_TX      - EDMAMUX_DMAREQ_ID_SPI2_RX      - EDMAMUX_DMAREQ_ID_SPI2_TX      - EDMAMUX_DMAREQ_ID_SPI3_RX
  *         - EDMAMUX_DMAREQ_ID_SPI3_TX      - EDMAMUX_DMAREQ_ID_SPI4_RX      - EDMAMUX_DMAREQ_ID_SPI4_TX      - EDMAMUX_DMAREQ_ID_I2S2_EXT_RX
  *         - EDMAMUX_DMAREQ_ID_I2S2_EXT_TX  - EDMAMUX_DMAREQ_ID_I2S3_EXT_RX  - EDMAMUX_DMAREQ_ID_I2S3_EXT_TX  - EDMAMUX_DMAREQ_ID_I2C1_RX
  *         - EDMAMUX_DMAREQ_ID_I2C1_TX      - EDMAMUX_DMAREQ_ID_I2C2_RX      - EDMAMUX_DMAREQ_ID_I2C2_TX      - EDMAMUX_DMAREQ_ID_I2C3_RX
  *         - EDMAMUX_DMAREQ_ID_I2C3_TX      - EDMAMUX_DMAREQ_ID_USART1_RX    - EDMAMUX_DMAREQ_ID_USART1_TX    - EDMAMUX_DMAREQ_ID_USART2_RX
  *         - EDMAMUX_DMAREQ_ID_USART2_TX    - EDMAMUX_DMAREQ_ID_USART3_RX    - EDMAMUX_DMAREQ_ID_USART3_TX    - EDMAMUX_DMAREQ_ID_UART4_RX
  *         - EDMAMUX_DMAREQ_ID_UART4_TX     - EDMAMUX_DMAREQ_ID_UART5_RX     - EDMAMUX_DMAREQ_ID_UART5_TX     - EDMAMUX_DMAREQ_ID_USART6_RX
  *         - EDMAMUX_DMAREQ_ID_USART6_TX    - EDMAMUX_DMAREQ_ID_UART7_RX     - EDMAMUX_DMAREQ_ID_UART7_TX     - EDMAMUX_DMAREQ_ID_UART8_RX
  *         - EDMAMUX_DMAREQ_ID_UART8_TX     - EDMAMUX_DMAREQ_ID_SDIO1        - EDMAMUX_DMAREQ_ID_SDIO2        - EDMAMUX_DMAREQ_ID_QSPI1
  *         - EDMAMUX_DMAREQ_ID_QSPI2        - EDMAMUX_DMAREQ_ID_TMR1_CH1     - EDMAMUX_DMAREQ_ID_TMR1_CH2     - EDMAMUX_DMAREQ_ID_TMR1_CH3
  *         - EDMAMUX_DMAREQ_ID_TMR1_CH4     - EDMAMUX_DMAREQ_ID_TMR1_OVERFLOW- EDMAMUX_DMAREQ_ID_TMR1_TRIG    - EDMAMUX_DMAREQ_ID_TMR1_COM
  *         - EDMAMUX_DMAREQ_ID_TMR8_CH1     - EDMAMUX_DMAREQ_ID_TMR8_CH2     - EDMAMUX_DMAREQ_ID_TMR8_CH3     - EDMAMUX_DMAREQ_ID_TMR8_CH4
  *         - EDMAMUX_DMAREQ_ID_TMR8_UP      - EDMAMUX_DMAREQ_ID_TMR8_TRIG    - EDMAMUX_DMAREQ_ID_TMR8_COM     - EDMAMUX_DMAREQ_ID_TMR2_CH1
  *         - EDMAMUX_DMAREQ_ID_TMR2_CH2     - EDMAMUX_DMAREQ_ID_TMR2_CH3     - EDMAMUX_DMAREQ_ID_TMR2_CH4     - EDMAMUX_DMAREQ_ID_TMR2_OVERFLOW
  *         - EDMAMUX_DMAREQ_ID_TMR2_TRIG    - EDMAMUX_DMAREQ_ID_TMR3_CH1     - EDMAMUX_DMAREQ_ID_TMR3_CH2     - EDMAMUX_DMAREQ_ID_TMR3_CH3
  *         - EDMAMUX_DMAREQ_ID_TMR3_CH4     - EDMAMUX_DMAREQ_ID_TMR3_OVERFLOW- EDMAMUX_DMAREQ_ID_TMR3_TRIG    - EDMAMUX_DMAREQ_ID_TMR4_CH1
  *         - EDMAMUX_DMAREQ_ID_TMR4_CH2     - EDMAMUX_DMAREQ_ID_TMR4_CH3     - EDMAMUX_DMAREQ_ID_TMR4_CH4     - EDMAMUX_DMAREQ_ID_TMR4_OVERFLOW
  *         - EDMAMUX_DMAREQ_ID_TMR4_TRIG    - EDMAMUX_DMAREQ_ID_TMR5_CH1     - EDMAMUX_DMAREQ_ID_TMR5_CH2     - EDMAMUX_DMAREQ_ID_TMR5_CH3
  *         - EDMAMUX_DMAREQ_ID_TMR5_CH4     - EDMAMUX_DMAREQ_ID_TMR5_OVERFLOW- EDMAMUX_DMAREQ_ID_TMR5_TRIG    - EDMAMUX_DMAREQ_ID_TMR20_CH1
  *         - EDMAMUX_DMAREQ_ID_TMR20_CH2    - EDMAMUX_DMAREQ_ID_TMR20_CH3    - EDMAMUX_DMAREQ_ID_TMR20_CH4    - EDMAMUX_DMAREQ_ID_TMR20_OVERFLOW
  *         - EDMAMUX_DMAREQ_ID_TMR20_TRIG   - EDMAMUX_DMAREQ_ID_TMR20_HALL   - EDMAMUX_DMAREQ_ID_DVP
  * @retval none.
  */
void eDMAMUX_Init(eDMAMUX_Channel_Type *eDMAMUX_channelx, eDMAMUX_Requst_ID_sel_Type eDMAMUX_req_id) {
    eDMAMUX_channelx->muxctrl_bit.reqsel = eDMAMUX_req_id;
}

/**
  * @brief  edmamux sync init struct config with its default value.
  * @param  eDMAMUX_Sync_Init_struct: pointer to a eDMAMUX_Sync_Init_Type structure which will be initialized.
  * @retval none.
  */
void eDMAMUX_Sync_Default_Para_Init(eDMAMUX_Sync_Init_Type *eDMAMUX_Sync_Init_struct) {
    eDMAMUX_Sync_Init_struct->sync_Enable = FALSE;
    eDMAMUX_Sync_Init_struct->sync_Event_Enable = FALSE;
    eDMAMUX_Sync_Init_struct->sync_polarity = EDMAMUX_Sync_POLARITY_Disable;
    eDMAMUX_Sync_Init_struct->sync_Request_Number = 0x0;
    eDMAMUX_Sync_Init_struct->sync_signal_sel = EDMAMUX_Sync_ID_EXINT0;
}

/**
  * @brief  edmamux synchronization config.
  * @param  eDMAMUX_channelx:
  *         this parameter can be one of the following values:
  *         - EDMAMUX_CHANNEL1
  *         - EDMAMUX_CHANNEL2
  *         - EDMAMUX_CHANNEL3
  *         - EDMAMUX_CHANNEL4
  *         - EDMAMUX_CHANNEL5
  *         - EDMAMUX_CHANNEL6
  *         - EDMAMUX_CHANNEL7
  *         - EDMAMUX_CHANNEL8
  * @param  eDMAMUX_Sync_Init_struct: ointer to a eDMAMUX_Sync_Init_Type structure.
  * @retval none.
  */
void eDMAMUX_Sync_Config(eDMAMUX_Channel_Type *eDMAMUX_channelx, eDMAMUX_Sync_Init_Type *eDMAMUX_Sync_Init_struct) {
    eDMAMUX_channelx->muxctrl_bit.syncsel = eDMAMUX_Sync_Init_struct->sync_signal_sel;
    eDMAMUX_channelx->muxctrl_bit.syncpol = eDMAMUX_Sync_Init_struct->sync_polarity;
    eDMAMUX_channelx->muxctrl_bit.reqcnt  = eDMAMUX_Sync_Init_struct->sync_Request_Number;
    eDMAMUX_channelx->muxctrl_bit.evtgen  = eDMAMUX_Sync_Init_struct->sync_Event_Enable;
    eDMAMUX_channelx->muxctrl_bit.syncen  = eDMAMUX_Sync_Init_struct->sync_Enable;
}

/**
  * @brief  edmamux request generator init struct config with its default value.
  * @param  eDMAMUX_Gen_Init_struct: pointer to a eDMAMUX_Gen_Init_Type structure which will be initialized.
  * @retval none.
  */
void eDMAMUX_Generator_Default_Para_Init(eDMAMUX_Gen_Init_Type *eDMAMUX_Gen_Init_struct) {
    eDMAMUX_Gen_Init_struct->gen_Enable         = FALSE;
    eDMAMUX_Gen_Init_struct->gen_polarity       = EDMAMUX_Gen_POLARITY_Disable;
    eDMAMUX_Gen_Init_struct->gen_Request_Number = 0x0;
    eDMAMUX_Gen_Init_struct->gen_signal_sel     = EDMAMUX_Gen_ID_EXINT0;
}

/**
  * @brief  edmamux request generator init.
  * @param  eDMAMUX_Gen_Init_struct: pointer to a eDMAMUX_Gen_Init_Type structure which will be initialized.
  * @retval none.
  */
void eDMAMUX_Generator_Config(eDMAMUX_Generator_Type *eDMAMUX_Gen_x, eDMAMUX_Gen_Init_Type *eDMAMUX_Gen_Init_struct) {
    eDMAMUX_Gen_x->gctrl_bit.sigsel  = eDMAMUX_Gen_Init_struct->gen_signal_sel;
    eDMAMUX_Gen_x->gctrl_bit.gpol    = eDMAMUX_Gen_Init_struct->gen_polarity;
    eDMAMUX_Gen_x->gctrl_bit.greqcnt = eDMAMUX_Gen_Init_struct->gen_Request_Number;
    eDMAMUX_Gen_x->gctrl_bit.gen     = eDMAMUX_Gen_Init_struct->gen_Enable;
}

/**
  * @brief  enable or disable the edmamux sync interrupts.
  * @param  eDMAMUX_channelx:
  *         this parameter can be one of the following values:
  *         - EDMAMUX_CHANNEL1
  *         - EDMAMUX_CHANNEL2
  *         - EDMAMUX_CHANNEL3
  *         - EDMAMUX_CHANNEL4
  *         - EDMAMUX_CHANNEL5
  *         - EDMAMUX_CHANNEL6
  *         - EDMAMUX_CHANNEL7
  *         - EDMAMUX_CHANNEL8
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMAMUX_Sync_Interrupt_Enable(eDMAMUX_Channel_Type *eDMAMUX_channelx, confirm_state new_state) {
    if(new_state != FALSE) {
        eDMAMUX_channelx->muxctrl_bit.syncovien = TRUE;
    } else {
        eDMAMUX_channelx->muxctrl_bit.syncovien = FALSE;
    }
}

/**
  * @brief  enable or disable the edmamux request generator interrupts.
  * @param  eDMAMUX_Gen_x: pointer to a eDMAMUX_Generator_Type structure.
  *         this parameter can be one of the following values:
  *         - EDMAMUX_GENERATOR1
  *         - EDMAMUX_GENERATOR2
  *         - EDMAMUX_GENERATOR3
  *         - EDMAMUX_GENERATOR4
  * @param  new_state (TRUE or FALSE).
  * @retval none.
  */
void eDMAMUX_Generator_Interrupt_Enable(eDMAMUX_Generator_Type *eDMAMUX_Gen_x, confirm_state new_state) {
    if(new_state != FALSE) {
        eDMAMUX_Gen_x->gctrl_bit.trgovien = TRUE;
    } else {
        eDMAMUX_Gen_x->gctrl_bit.trgovien = FALSE;
    }
}

/**
  * @brief  edmamux sync flag get.
  * @param  flag
  *         this parameter can be any combination of the following values:
  *         - EDMAMUX_Sync_OV1_FLAG
  *         - EDMAMUX_Sync_OV2_FLAG
  *         - EDMAMUX_Sync_OV3_FLAG
  *         - EDMAMUX_Sync_OV4_FLAG
  *         - EDMAMUX_Sync_OV5_FLAG
  *         - EDMAMUX_Sync_OV6_FLAG
  *         - EDMAMUX_Sync_OV7_FLAG
  *         - EDMAMUX_Sync_OV8_FLAG
  * @retval state of edmamux sync flag.
  */
flag_status eDMAMUX_Sync_Flag_Get(uint32_t flag) {
    if((EDMA->muxsyncsts & flag) != RESET) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  edmamux sync flag clear.
  * @param  flag
  *         this parameter can be any combination of the following values:
  *         - EDMAMUX_Sync_OV1_FLAG
  *         - EDMAMUX_Sync_OV2_FLAG
  *         - EDMAMUX_Sync_OV3_FLAG
  *         - EDMAMUX_Sync_OV4_FLAG
  *         - EDMAMUX_Sync_OV5_FLAG
  *         - EDMAMUX_Sync_OV6_FLAG
  *         - EDMAMUX_Sync_OV7_FLAG
  *         - EDMAMUX_Sync_OV8_FLAG
  * @retval none.
  */
void eDMAMUX_Sync_Flag_Clear(uint32_t flag) {
    EDMA->muxsyncclr = flag;
}

/**
  * @brief  edmamux request generator flag get.
  * @param  flag
  *         this parameter can be any combination of the following values:
  *         - EDMAMUX_Gen_TRIG_OV1_FLAG
  *         - EDMAMUX_Gen_TRIG_OV2_FLAG
  *         - EDMAMUX_Gen_TRIG_OV3_FLAG
  *         - EDMAMUX_Gen_TRIG_OV4_FLAG
  * @retval state of edmamux sync flag.
  */
flag_status eDMAMUX_Generator_Flag_Get(uint32_t flag) {
    if((EDMA->muxgsts & flag) != RESET) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  edmamux request generator flag clear.
  * @param  flag
  *         this parameter can be any combination of the following values:
  *         - EDMAMUX_Gen_TRIG_OV1_FLAG
  *         - EDMAMUX_Gen_TRIG_OV2_FLAG
  *         - EDMAMUX_Gen_TRIG_OV3_FLAG
  *         - EDMAMUX_Gen_TRIG_OV4_FLAG
  * @retval none.
  */
void eDMAMUX_Generator_Flag_Clear(uint32_t flag) {
    EDMA->muxgclr = flag;
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
