/*!
    文件:    gd32f4xx_dma.c
    简介:   DMA driver
    版本: 2016-08-15, V1.0.0, firmware for GD32F4xx
    版本: 2018-12-12, V2.0.0, firmware for GD32F4xx
    版本: 2020-09-30, V2.1.0, firmware for GD32F4xx
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

#include "gd32f4xx_dma.h"

/*  DMA register bit offset */
#define CHXCTL_PERIEN_OFFSET            ((uint32_t)25U)

/*!
    简介:    deinitialize DMA a channel registers
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel is deinitialized
      参数:        DMA_CHx(x=0..7)
    参数[输出]:  无
    返回值:      无
*/
void dma_deinit(uint32_t dma_periph, dma_channel_enum channelx) {
    /* disable DMA a channel */
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
    /* reset DMA channel registers */
    DMA_CHCTL(dma_periph, channelx) = DMA_CHCTL_RESET_VALUE;
    DMA_CHCNT(dma_periph, channelx) = DMA_CHCNT_RESET_VALUE;
    DMA_CHPADDR(dma_periph, channelx) = DMA_CHPADDR_RESET_VALUE;
    DMA_CHM0ADDR(dma_periph, channelx) = DMA_CHMADDR_RESET_VALUE;
    DMA_CHM1ADDR(dma_periph, channelx) = DMA_CHMADDR_RESET_VALUE;
    DMA_CHFCTL(dma_periph, channelx) = DMA_CHFCTL_RESET_VALUE;

    if(channelx < DMA_CH4) {
        DMA_INTC0(dma_periph) |= DMA_FLAG_ADD(DMA_CHINTF_RESET_VALUE, channelx);
    } else {
        channelx -= (dma_channel_enum)4;
        DMA_INTC1(dma_periph) |= DMA_FLAG_ADD(DMA_CHINTF_RESET_VALUE, channelx);
    }
}

/*!
    简介:    initialize the DMA single data mode parameters struct with the default values
    参数[输入]:  init_struct: the initialization data needed to initialize DMA channel
    参数[输出]:  无
    返回值:      无
*/
void dma_single_data_para_struct_init(dma_single_data_parameter_struct *init_struct) {
    /* set the DMA struct with the default values */
    init_struct->periph_addr         = 0U;
    init_struct->periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    init_struct->memory0_addr        = 0U;
    init_struct->memory_inc          = DMA_MEMORY_INCREASE_DISABLE;
    init_struct->periph_memory_width = 0U;
    init_struct->circular_mode       = DMA_CIRCULAR_MODE_DISABLE;
    init_struct->direction           = DMA_PERIPH_TO_MEMORY;
    init_struct->number              = 0U;
    init_struct->priority            = DMA_PRIORITY_LOW;
}

/*!
    简介:    initialize the DMA multi data mode parameters struct with the default values
    参数[输入]:  init_struct: the initialization data needed to initialize DMA channel
    参数[输出]:  无
    返回值:      无
*/
void dma_multi_data_para_struct_init(dma_multi_data_parameter_struct *init_struct) {
    /* set the DMA struct with the default values */
    init_struct->periph_addr         = 0U;
    init_struct->periph_width        = 0U;
    init_struct->periph_inc          = DMA_PERIPH_INCREASE_DISABLE;
    init_struct->memory0_addr        = 0U;
    init_struct->memory_width        = 0U;
    init_struct->memory_inc          = DMA_MEMORY_INCREASE_DISABLE;
    init_struct->memory_burst_width  = 0U;
    init_struct->periph_burst_width  = 0U;
    init_struct->circular_mode       = DMA_CIRCULAR_MODE_DISABLE;
    init_struct->direction           = DMA_PERIPH_TO_MEMORY;
    init_struct->number              = 0U;
    init_struct->priority            = DMA_PRIORITY_LOW;
}

/*!
    简介:    initialize DMA single data mode
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel is initialized
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  init_struct: the data needed to initialize DMA single data mode
                  periph_addr: peripheral base address
                  periph_inc: DMA_PERIPH_INCREASE_ENABLE,DMA_PERIPH_INCREASE_DISABLE,DMA_PERIPH_INCREASE_FIX
                  memory0_addr: memory base address
                  memory_inc: DMA_MEMORY_INCREASE_ENABLE,DMA_MEMORY_INCREASE_DISABLE
                  periph_memory_width: DMA_PERIPH_WIDTH_8BIT,DMA_PERIPH_WIDTH_16BIT,DMA_PERIPH_WIDTH_32BIT
                  circular_mode: DMA_CIRCULAR_MODE_ENABLE,DMA_CIRCULAR_MODE_DISABLE
                  direction: DMA_PERIPH_TO_MEMORY,DMA_MEMORY_TO_PERIPH,DMA_MEMORY_TO_MEMORY
                  number: the number of remaining data to be transferred by the DMA
                  priority: DMA_PRIORITY_LOW,DMA_PRIORITY_MEDIUM,DMA_PRIORITY_HIGH,DMA_PRIORITY_ULTRA_HIGH
    参数[输出]:  无
    返回值:      无
*/
void dma_single_data_mode_init(uint32_t dma_periph, dma_channel_enum channelx, dma_single_data_parameter_struct *init_struct) {
    uint32_t ctl;

    /* select single data mode */
    DMA_CHFCTL(dma_periph, channelx) &= ~DMA_CHXFCTL_MDMEN;

    /* configure peripheral base address */
    DMA_CHPADDR(dma_periph, channelx) = init_struct->periph_addr;

    /* configure memory base address */
    DMA_CHM0ADDR(dma_periph, channelx) = init_struct->memory0_addr;

    /* configure the number of remaining data to be transferred */
    DMA_CHCNT(dma_periph, channelx) = init_struct->number;

    /* configure peripheral and memory transfer width,channel priotity,transfer mode */
    ctl = DMA_CHCTL(dma_periph, channelx);
    ctl &= ~(DMA_CHXCTL_PWIDTH | DMA_CHXCTL_MWIDTH | DMA_CHXCTL_PRIO | DMA_CHXCTL_TM);
    ctl |= (init_struct->periph_memory_width | (init_struct->periph_memory_width << 2) | init_struct->priority | init_struct->direction);
    DMA_CHCTL(dma_periph, channelx) = ctl;

    /* configure peripheral increasing mode */
    if(DMA_PERIPH_INCREASE_ENABLE == init_struct->periph_inc) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
    } else if(DMA_PERIPH_INCREASE_DISABLE == init_struct->periph_inc) {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PAIF;
    }

    /* configure memory increasing mode */
    if(DMA_MEMORY_INCREASE_ENABLE == init_struct->memory_inc) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
    } else {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
    }

    /* configure DMA circular mode */
    if(DMA_CIRCULAR_MODE_ENABLE == init_struct->circular_mode) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CMEN;
    } else {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CMEN;
    }
}

/*!
    简介:    initialize DMA multi data mode
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel is initialized
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  dma_multi_data_parameter_struct: the data needed to initialize DMA multi data mode
                  periph_addr: peripheral base address
                  periph_width: DMA_PERIPH_WIDTH_8BIT,DMA_PERIPH_WIDTH_16BIT,DMA_PERIPH_WIDTH_32BIT
                  periph_inc: DMA_PERIPH_INCREASE_ENABLE,DMA_PERIPH_INCREASE_DISABLE,DMA_PERIPH_INCREASE_FIX
                  memory0_addr: memory0 base address
                  memory_width: DMA_MEMORY_WIDTH_8BIT,DMA_MEMORY_WIDTH_16BIT,DMA_MEMORY_WIDTH_32BIT
                  memory_inc: DMA_MEMORY_INCREASE_ENABLE,DMA_MEMORY_INCREASE_DISABLE
                  memory_burst_width: DMA_MEMORY_BURST_SINGLE,DMA_MEMORY_BURST_4_BEAT,DMA_MEMORY_BURST_8_BEAT,DMA_MEMORY_BURST_16_BEAT
                  periph_burst_width: DMA_PERIPH_BURST_SINGLE,DMA_PERIPH_BURST_4_BEAT,DMA_PERIPH_BURST_8_BEAT,DMA_PERIPH_BURST_16_BEAT
                  critical_value: DMA_FIFO_1_WORD,DMA_FIFO_2_WORD,DMA_FIFO_3_WORD,DMA_FIFO_4_WORD
                  circular_mode: DMA_CIRCULAR_MODE_ENABLE,DMA_CIRCULAR_MODE_DISABLE
                  direction: DMA_PERIPH_TO_MEMORY,DMA_MEMORY_TO_PERIPH,DMA_MEMORY_TO_MEMORY
                  number: the number of remaining data to be transferred by the DMA
                  priority: DMA_PRIORITY_LOW,DMA_PRIORITY_MEDIUM,DMA_PRIORITY_HIGH,DMA_PRIORITY_ULTRA_HIGH
    参数[输出]:  无
    返回值:      无
*/
void dma_multi_data_mode_init(uint32_t dma_periph, dma_channel_enum channelx, dma_multi_data_parameter_struct *init_struct) {
    uint32_t ctl;

    /* select multi data mode and configure FIFO critical value */
    DMA_CHFCTL(dma_periph, channelx) |= (DMA_CHXFCTL_MDMEN | init_struct->critical_value);

    /* configure peripheral base address */
    DMA_CHPADDR(dma_periph, channelx) = init_struct->periph_addr;

    /* configure memory base address */
    DMA_CHM0ADDR(dma_periph, channelx) = init_struct->memory0_addr;

    /* configure the number of remaining data to be transferred */
    DMA_CHCNT(dma_periph, channelx) = init_struct->number;

    /* configure peripheral and memory transfer width,channel priotity,transfer mode,peripheral and memory burst transfer width */
    ctl = DMA_CHCTL(dma_periph, channelx);
    ctl &= ~(DMA_CHXCTL_PWIDTH | DMA_CHXCTL_MWIDTH | DMA_CHXCTL_PRIO | DMA_CHXCTL_TM | DMA_CHXCTL_PBURST | DMA_CHXCTL_MBURST);
    ctl |= (init_struct->periph_width | (init_struct->memory_width) | init_struct->priority | init_struct->direction | init_struct->memory_burst_width |
            init_struct->periph_burst_width);
    DMA_CHCTL(dma_periph, channelx) = ctl;

    /* configure peripheral increasing mode */
    if(DMA_PERIPH_INCREASE_ENABLE == init_struct->periph_inc) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
    } else if(DMA_PERIPH_INCREASE_DISABLE == init_struct->periph_inc) {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PAIF;
    }

    /* configure memory increasing mode */
    if(DMA_MEMORY_INCREASE_ENABLE == init_struct->memory_inc) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
    } else {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
    }

    /* configure DMA circular mode */
    if(DMA_CIRCULAR_MODE_ENABLE == init_struct->circular_mode) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CMEN;
    } else {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CMEN;
    }
}

/*!
    简介:    set DMA peripheral base address
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel to set peripheral base address
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  address: peripheral base address
    参数[输出]:  无
    返回值:      无
*/
void dma_periph_address_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t address) {
    DMA_CHPADDR(dma_periph, channelx) = address;
}

/*!
    简介:    set DMA Memory0 base address
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel to set Memory base address
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  memory_flag: DMA_MEMORY_x(x=0,1)
    参数[输入]:  address: Memory base address
    参数[输出]:  无
    返回值:      无
*/
void dma_memory_address_config(uint32_t dma_periph, dma_channel_enum channelx, uint8_t memory_flag, uint32_t address) {
    if(memory_flag) {
        DMA_CHM1ADDR(dma_periph, channelx) = address;
    } else {
        DMA_CHM0ADDR(dma_periph, channelx) = address;
    }
}

/*!
    简介:    set the number of remaining data to be transferred by the DMA
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel to set number
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  number: the number of remaining data to be transferred by the DMA
    参数[输出]:  无
    返回值:      无
*/
void dma_transfer_number_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t number) {
    DMA_CHCNT(dma_periph, channelx) = number;
}

/*!
    简介:    get the number of remaining data to be transferred by the DMA
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel to set number
      参数:        DMA_CHx(x=0..7)
    参数[输出]:  无
    返回值:     uint32_t: the number of remaining data to be transferred by the DMA
*/
uint32_t dma_transfer_number_get(uint32_t dma_periph, dma_channel_enum channelx) {
    return (uint32_t)DMA_CHCNT(dma_periph, channelx);
}

/*!
    简介:    configure priority level of DMA channel
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  priority: priority Level of this channel
                only one parameter can be selected which is shown as below:
      参数:        DMA_PRIORITY_LOW: low priority
      参数:        DMA_PRIORITY_MEDIUM: medium priority
      参数:        DMA_PRIORITY_HIGH: high priority
      参数:        DMA_PRIORITY_ULTRA_HIGH: ultra high priority
    参数[输出]:  无
    返回值:      无
*/
void dma_priority_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t priority) {
    uint32_t ctl;
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PRIO;
    ctl |= priority;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    简介:    configure transfer burst beats of memory
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  mbeat: transfer burst beats
      参数:        DMA_MEMORY_BURST_SINGLE: memory transfer single burst
      参数:        DMA_MEMORY_BURST_4_BEAT: memory transfer 4-beat burst
      参数:        DMA_MEMORY_BURST_8_BEAT: memory transfer 8-beat burst
      参数:        DMA_MEMORY_BURST_16_BEAT: memory transfer 16-beat burst
    参数[输出]:  无
    返回值:      无
*/
void dma_memory_burst_beats_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t mbeat) {
    uint32_t ctl;
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_MBURST;
    ctl |= mbeat;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    简介:    configure transfer burst beats of peripheral
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  pbeat: transfer burst beats
                only one parameter can be selected which is shown as below:
      参数:        DMA_PERIPH_BURST_SINGLE: peripheral transfer single burst
      参数:        DMA_PERIPH_BURST_4_BEAT: peripheral transfer 4-beat burst
      参数:        DMA_PERIPH_BURST_8_BEAT: peripheral transfer 8-beat burst
      参数:        DMA_PERIPH_BURST_16_BEAT: peripheral transfer 16-beat burst
    参数[输出]:  无
    返回值:      无
*/
void dma_periph_burst_beats_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t pbeat) {
    uint32_t ctl;
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PBURST;
    ctl |= pbeat;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    简介:    configure transfer data size of memory
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  msize: transfer data size of memory
                only one parameter can be selected which is shown as below:
      参数:        DMA_MEMORY_WIDTH_8BIT: transfer data size of memory is 8-bit
      参数:        DMA_MEMORY_WIDTH_16BIT: transfer data size of memory is 16-bit
      参数:        DMA_MEMORY_WIDTH_32BIT: transfer data size of memory is 32-bit
    参数[输出]:  无
    返回值:      无
*/
void dma_memory_width_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t msize) {
    uint32_t ctl;
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_MWIDTH;
    ctl |= msize;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    简介:    configure transfer data size of peripheral
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  msize: transfer data size of peripheral
                only one parameter can be selected which is shown as below:
      参数:        DMA_PERIPHERAL_WIDTH_8BIT: transfer data size of peripheral is 8-bit
      参数:        DMA_PERIPHERAL_WIDTH_16BIT: transfer data size of peripheral is 16-bit
      参数:        DMA_PERIPHERAL_WIDTH_32BIT: transfer data size of peripheral is 32-bit
    参数[输出]:  无
    返回值:      无
*/
void dma_periph_width_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t psize) {
    uint32_t ctl;
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PWIDTH;
    ctl |= psize;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    简介:    configure memory address generation generation_algorithm
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  generation_algorithm: the address generation algorithm
                only one parameter can be selected which is shown as below:
      参数:        DMA_MEMORY_INCREASE_ENABLE: next address of memory is increasing address mode
      参数:        DMA_MEMORY_INCREASE_DISABLE: next address of memory is fixed address mode
    参数[输出]:  无
    返回值:      无
*/
void dma_memory_address_generation_config(uint32_t dma_periph, dma_channel_enum channelx, uint8_t generation_algorithm) {
    if(DMA_MEMORY_INCREASE_ENABLE == generation_algorithm) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
    } else {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
    }
}

/*!
    简介:    configure peripheral address generation_algorithm
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  generation_algorithm: the address generation algorithm
                only one parameter can be selected which is shown as below:
      参数:        DMA_PERIPH_INCREASE_ENABLE: next address of peripheral is increasing address mode
      参数:        DMA_PERIPH_INCREASE_DISABLE: next address of peripheral is fixed address mode
      参数:        DMA_PERIPH_INCREASE_FIX: increasing steps of peripheral address is fixed
    参数[输出]:  无
    返回值:      无
*/
void dma_peripheral_address_generation_config(uint32_t dma_periph, dma_channel_enum channelx, uint8_t generation_algorithm) {
    if(DMA_PERIPH_INCREASE_ENABLE == generation_algorithm) {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
    } else if(DMA_PERIPH_INCREASE_DISABLE == generation_algorithm) {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PAIF;
    }
}

/*!
    简介:    enable DMA circulation mode
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输出]:  无
    返回值:      无
*/
void dma_circulation_enable(uint32_t dma_periph, dma_channel_enum channelx) {
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CMEN;
}

/*!
    简介:    disable DMA circulation mode
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输出]:  无
    返回值:      无
*/
void dma_circulation_disable(uint32_t dma_periph, dma_channel_enum channelx) {
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CMEN;
}

/*!
    简介:    enable DMA channel
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输出]:  无
    返回值:      无
*/
void dma_channel_enable(uint32_t dma_periph, dma_channel_enum channelx) {
    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CHEN;
}

/*!
    简介:    disable DMA channel
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输出]:  无
    返回值:      无
*/
void dma_channel_disable(uint32_t dma_periph, dma_channel_enum channelx) {
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
}

/*!
    简介:    configure the direction of  data transfer on the channel
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  direction: specify the direction of  data transfer
                only one parameter can be selected which is shown as below:
      参数:        DMA_PERIPH_TO_MEMORY: read from peripheral and write to memory
      参数:        DMA_MEMORY_TO_PERIPH: read from memory and write to peripheral
      参数:        DMA_MEMORY_TO_MEMORY: read from memory and write to memory
    参数[输出]:  无
    返回值:      无
*/
void dma_transfer_direction_config(uint32_t dma_periph, dma_channel_enum channelx, uint8_t direction) {
    uint32_t ctl;
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_TM;
    ctl |= direction;

    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    简介:    DMA switch buffer mode config
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  memory1_addr: memory1 base address
    参数[输入]:  memory_select: DMA_MEMORY_0 or DMA_MEMORY_1
    参数[输出]:  无
    返回值:      无
*/
void dma_switch_buffer_mode_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t memory1_addr, uint32_t memory_select) {
    /* configure memory1 base address */
    DMA_CHM1ADDR(dma_periph, channelx) = memory1_addr;

    if(DMA_MEMORY_0 == memory_select) {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MBS;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MBS;
    }
}

/*!
    简介:    DMA using memory get
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输出]:  无
    返回值:     the using memory
*/
uint32_t dma_using_memory_get(uint32_t dma_periph, dma_channel_enum channelx) {
    if((DMA_CHCTL(dma_periph, channelx)) & DMA_CHXCTL_MBS) {
        return DMA_MEMORY_1;
    } else {
        return DMA_MEMORY_0;
    }
}

/*!
    简介:    DMA channel peripheral select
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  sub_periph: specify DMA channel peripheral
      参数:        DMA_SUBPERIx(x=0..7)
    参数[输出]:  无
    返回值:      无
*/
void dma_channel_subperipheral_select(uint32_t dma_periph, dma_channel_enum channelx, dma_subperipheral_enum sub_periph) {
    uint32_t ctl;
    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PERIEN;
    ctl |= ((uint32_t)sub_periph << CHXCTL_PERIEN_OFFSET);

    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    简介:    DMA flow controller configure
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  controller: specify DMA flow controler
                only one parameter can be selected which is shown as below:
      参数:        DMA_FLOW_CONTROLLER_DMA: DMA is the flow controller
      参数:        DMA_FLOW_CONTROLLER_PERI: peripheral is the flow controller
    参数[输出]:  无
    返回值:      无
*/
void dma_flow_controller_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t controller) {
    if(DMA_FLOW_CONTROLLER_DMA == controller) {
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_TFCS;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_TFCS;
    }
}

/*!
    简介:    DMA switch buffer mode enable
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  newvalue: ENABLE or DISABLE
    参数[输出]:  无
    返回值:      无
*/
void dma_switch_buffer_mode_enable(uint32_t dma_periph, dma_channel_enum channelx, ControlStatus newvalue) {
    if(ENABLE == newvalue) {
        /* switch buffer mode enable */
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_SBMEN;
    } else {
        /* switch buffer mode disable */
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_SBMEN;
    }
}

/*!
    简介:    DMA FIFO status get
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输出]:  无
    返回值:     the using memory
*/
uint32_t dma_fifo_status_get(uint32_t dma_periph, dma_channel_enum channelx) {
    return (DMA_CHFCTL(dma_periph, channelx) & DMA_CHXFCTL_FCNT);
}

/*!
    简介:    get DMA flag is set or not
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel to get flag
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      参数:        DMA_FLAG_FEE: FIFO error and exception flag
      参数:        DMA_FLAG_SDE: single data mode exception flag
      参数:        DMA_FLAG_TAE: transfer access error flag
      参数:        DMA_FLAG_HTF: half transfer finish flag
      参数:        DMA_FLAG_FTF: full transger finish flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus dma_flag_get(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag) {
    if(channelx < DMA_CH4) {
        if(DMA_INTF0(dma_periph) & DMA_FLAG_ADD(flag, channelx)) {
            return SET;
        } else {
            return RESET;
        }
    } else {
        channelx -= (dma_channel_enum)4;

        if(DMA_INTF1(dma_periph) & DMA_FLAG_ADD(flag, channelx)) {
            return SET;
        } else {
            return RESET;
        }
    }
}

/*!
    简介:    clear DMA a channel flag
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel to get flag
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  flag: specify get which flag
                only one parameter can be selected which is shown as below:
      参数:        DMA_FLAG_FEE: FIFO error and exception flag
      参数:        DMA_FLAG_SDE: single data mode exception flag
      参数:        DMA_FLAG_TAE: transfer access error flag
      参数:        DMA_FLAG_HTF: half transfer finish flag
      参数:        DMA_FLAG_FTF: full transger finish flag
    参数[输出]:  无
    返回值:      无
*/
void dma_flag_clear(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag) {
    if(channelx < DMA_CH4) {
        DMA_INTC0(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
    } else {
        channelx -= (dma_channel_enum)4;
        DMA_INTC1(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
    }
}

/*!
    简介:    enable DMA interrupt
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  source: specify which interrupt to enbale
                only one parameters can be selected which are shown as below:
      参数:        DMA_CHXCTL_SDEIE: single data mode exception interrupt enable
      参数:        DMA_CHXCTL_TAEIE: tranfer access error interrupt enable
      参数:        DMA_CHXCTL_HTFIE: half transfer finish interrupt enable
      参数:        DMA_CHXCTL_FTFIE: full transfer finish interrupt enable
      参数:        DMA_CHXFCTL_FEEIE: FIFO exception interrupt enable
    参数[输出]:  无
    返回值:      无
*/
void dma_interrupt_enable(uint32_t dma_periph, dma_channel_enum channelx, uint32_t source) {
    if(DMA_CHXFCTL_FEEIE != source) {
        DMA_CHCTL(dma_periph, channelx) |= source;
    } else {
        DMA_CHFCTL(dma_periph, channelx) |= source;
    }
}

/*!
    简介:    disable DMA interrupt
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  source: specify which interrupt to disbale
                only one parameters can be selected which are shown as below:
      参数:        DMA_CHXCTL_SDEIE: single data mode exception interrupt enable
      参数:        DMA_CHXCTL_TAEIE: tranfer access error interrupt enable
      参数:        DMA_CHXCTL_HTFIE: half transfer finish interrupt enable
      参数:        DMA_CHXCTL_FTFIE: full transfer finish interrupt enable
      参数:        DMA_CHXFCTL_FEEIE: FIFO exception interrupt enable
    参数[输出]:  无
    返回值:      无
*/
void dma_interrupt_disable(uint32_t dma_periph, dma_channel_enum channelx, uint32_t source) {
    if(DMA_CHXFCTL_FEEIE != source) {
        DMA_CHCTL(dma_periph, channelx) &= ~source;
    } else {
        DMA_CHFCTL(dma_periph, channelx) &= ~source;
    }
}

/*!
    简介:    get DMA interrupt flag is set or not
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel to get interrupt flag
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  interrupt: specify get which flag
                only one parameter can be selected which is shown as below:
      参数:        DMA_INT_FLAG_FEE: FIFO error and exception flag
      参数:        DMA_INT_FLAG_SDE: single data mode exception flag
      参数:        DMA_INT_FLAG_TAE: transfer access error flag
      参数:        DMA_INT_FLAG_HTF: half transfer finish flag
      参数:        DMA_INT_FLAG_FTF: full transger finish flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus dma_interrupt_flag_get(uint32_t dma_periph, dma_channel_enum channelx, uint32_t interrupt) {
    uint32_t interrupt_enable = 0U, interrupt_flag = 0U;
    dma_channel_enum channel_flag_offset = channelx;

    if(channelx < DMA_CH4) {
        switch(interrupt) {
        case DMA_INTF_FEEIF:
            interrupt_flag = DMA_INTF0(dma_periph) & DMA_FLAG_ADD(interrupt, channelx);
            interrupt_enable = DMA_CHFCTL(dma_periph, channelx) & DMA_CHXFCTL_FEEIE;
            break;

        case DMA_INTF_SDEIF:
            interrupt_flag = DMA_INTF0(dma_periph) & DMA_FLAG_ADD(interrupt, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_SDEIE;
            break;

        case DMA_INTF_TAEIF:
            interrupt_flag = DMA_INTF0(dma_periph) & DMA_FLAG_ADD(interrupt, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_TAEIE;
            break;

        case DMA_INTF_HTFIF:
            interrupt_flag = DMA_INTF0(dma_periph) & DMA_FLAG_ADD(interrupt, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_HTFIE;
            break;

        case DMA_INTF_FTFIF:
            interrupt_flag = (DMA_INTF0(dma_periph) & DMA_FLAG_ADD(interrupt, channelx));
            interrupt_enable = (DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_FTFIE);
            break;

        default:
            break;
        }
    } else {
        channel_flag_offset -= (dma_channel_enum)4;

        switch(interrupt) {
        case DMA_INTF_FEEIF:
            interrupt_flag = DMA_INTF1(dma_periph) & DMA_FLAG_ADD(interrupt, channel_flag_offset);
            interrupt_enable = DMA_CHFCTL(dma_periph, channelx) & DMA_CHXFCTL_FEEIE;
            break;

        case DMA_INTF_SDEIF:
            interrupt_flag = DMA_INTF1(dma_periph) & DMA_FLAG_ADD(interrupt, channel_flag_offset);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_SDEIE;
            break;

        case DMA_INTF_TAEIF:
            interrupt_flag = DMA_INTF1(dma_periph) & DMA_FLAG_ADD(interrupt, channel_flag_offset);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_TAEIE;
            break;

        case DMA_INTF_HTFIF:
            interrupt_flag = DMA_INTF1(dma_periph) & DMA_FLAG_ADD(interrupt, channel_flag_offset);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_HTFIE;
            break;

        case DMA_INTF_FTFIF:
            interrupt_flag = DMA_INTF1(dma_periph) & DMA_FLAG_ADD(interrupt, channel_flag_offset);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_FTFIE;
            break;

        default:
            break;
        }
    }

    if(interrupt_flag && interrupt_enable) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear DMA a channel interrupt flag
    参数[输入]:  dma_periph: DMAx(x=0,1)
      参数:        DMAx(x=0,1)
    参数[输入]:  channelx: specify which DMA channel to clear interrupt flag
      参数:        DMA_CHx(x=0..7)
    参数[输入]:  interrupt: specify get which flag
                only one parameter can be selected which is shown as below:
      参数:        DMA_INT_FLAG_FEE: FIFO error and exception flag
      参数:        DMA_INT_FLAG_SDE: single data mode exception flag
      参数:        DMA_INT_FLAG_TAE: transfer access error flag
      参数:        DMA_INT_FLAG_HTF: half transfer finish flag
      参数:        DMA_INT_FLAG_FTF: full transger finish flag
    参数[输出]:  无
    返回值:      无
*/
void dma_interrupt_flag_clear(uint32_t dma_periph, dma_channel_enum channelx, uint32_t interrupt) {
    if(channelx < DMA_CH4) {
        DMA_INTC0(dma_periph) |= DMA_FLAG_ADD(interrupt, channelx);
    } else {
        channelx -= (dma_channel_enum)4;
        DMA_INTC1(dma_periph) |= DMA_FLAG_ADD(interrupt, channelx);
    }
}
