/*!
    文件:    gd32f4xx_exmc.c
    简介:   EXMC driver

    版本: 2016-08-15, V1.0.0, firmware for GD32F4xx
    版本: 2018-12-12, V2.0.0, firmware for GD32F4xx
    版本: 2020-09-30, V2.1.0, firmware for GD32F4xx
    版本: 2022-03-09, V3.0.0, firmware for GD32F4xx
    版本: 2022-06-08, V3.0.1, firmware for GD32F4xx
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

#include "gd32f4xx_exmc.h"

/* EXMC bank0 register reset value */
#define BANK0_SNCTL_RESET                 ((uint32_t)0x000030DAU)
#define BANK0_SNTCFG_RESET                ((uint32_t)0x0FFFFFFFU)
#define BANK0_SNWTCFG_RESET               ((uint32_t)0x0FFFFFFFU)

/* EXMC bank1/2 register reset mask */
#define BANK1_2_NPCTL_RESET               ((uint32_t)0x00000008U)
#define BANK1_2_NPINTEN_RESET             ((uint32_t)0x00000042U)
#define BANK1_2_NPCTCFG_RESET             ((uint32_t)0xFFFFFFFFU)
#define BANK1_2_NPATCFG_RESET             ((uint32_t)0xFFFFFFFFU)

/* EXMC bank3 register reset mask */
#define BANK3_NPCTL_RESET                 ((uint32_t)0x00000008U)
#define BANK3_NPINTEN_RESET               ((uint32_t)0x00000040U)
#define BANK3_NPCTCFG_RESET               ((uint32_t)0xFFFFFFFFU)
#define BANK3_NPATCFG_RESET               ((uint32_t)0xFFFFFFFFU)
#define BANK3_PIOTCFG3_RESET              ((uint32_t)0xFFFFFFFFU)

/* EXMC SDRAM device register reset mask */
#define SDRAM_DEVICE_SDCTL_RESET          ((uint32_t)0x000002D0U)
#define SDRAM_DEVICE_SDTCFG_RESET         ((uint32_t)0x0FFFFFFFU)
#define SDRAM_DEVICE_SDCMD_RESET          ((uint32_t)0x00000000U)
#define SDRAM_DEVICE_SDARI_RESET          ((uint32_t)0x00000000U)
#define SDRAM_DEVICE_SDSTAT_RESET         ((uint32_t)0x00000000U)
#define SDRAM_DEVICE_SDRSCTL_RESET        ((uint32_t)0x00000000U)

/* EXMC bank0 SQPI-PSRAM register reset mask */
#define BANK0_SQPI_SINIT_RESET            ((uint32_t)0x18010000U)
#define BANK0_SQPI_SRCMD_RESET            ((uint32_t)0x00000000U)
#define BANK0_SQPI_SWCMD_RESET            ((uint32_t)0x00000000U)
#define BANK0_SQPI_SIDL_RESET             ((uint32_t)0x00000000U)
#define BANK0_SQPI_SIDH_RESET             ((uint32_t)0x00000000U)

/* EXMC register bit offset */
#define SNCTL_NRMUX_OFFSET                ((uint32_t)1U)
#define SNCTL_SBRSTEN_OFFSET              ((uint32_t)8U)
#define SNCTL_WRAPEN_OFFSET               ((uint32_t)10U)
#define SNCTL_WREN_OFFSET                 ((uint32_t)12U)
#define SNCTL_NRWTEN_OFFSET               ((uint32_t)13U)
#define SNCTL_EXMODEN_OFFSET              ((uint32_t)14U)
#define SNCTL_ASYNCWAIT_OFFSET            ((uint32_t)15U)

#define SNTCFG_AHLD_OFFSET                ((uint32_t)4U)
#define SNTCFG_DSET_OFFSET                ((uint32_t)8U)
#define SNTCFG_BUSLAT_OFFSET              ((uint32_t)16U)

#define NPCTL_NDWTEN_OFFSET               ((uint32_t)1U)
#define NPCTL_ECCEN_OFFSET                ((uint32_t)6U)

#define NPCTCFG_COMWAIT_OFFSET            ((uint32_t)8U)
#define NPCTCFG_COMHLD_OFFSET             ((uint32_t)16U)
#define NPCTCFG_COMHIZ_OFFSET             ((uint32_t)24U)

#define NPATCFG_ATTWAIT_OFFSET            ((uint32_t)8U)
#define NPATCFG_ATTHLD_OFFSET             ((uint32_t)16U)
#define NPATCFG_ATTHIZ_OFFSET             ((uint32_t)24U)

#define PIOTCFG_IOWAIT_OFFSET             ((uint32_t)8U)
#define PIOTCFG_IOHLD_OFFSET              ((uint32_t)16U)
#define PIOTCFG_IOHIZ_OFFSET              ((uint32_t)24U)

#define SDCTL_WPEN_OFFSET                 ((uint32_t)9U)
#define SDCTL_BRSTRD_OFFSET               ((uint32_t)12U)

#define SDTCFG_XSRD_OFFSET                ((uint32_t)4U)
#define SDTCFG_RASD_OFFSET                ((uint32_t)8U)
#define SDTCFG_ARFD_OFFSET                ((uint32_t)12U)
#define SDTCFG_WRD_OFFSET                 ((uint32_t)16U)
#define SDTCFG_RPD_OFFSET                 ((uint32_t)20U)
#define SDTCFG_RCD_OFFSET                 ((uint32_t)24U)

#define SDCMD_NARF_OFFSET                 ((uint32_t)5U)
#define SDCMD_MRC_OFFSET                  ((uint32_t)9U)

#define SDARI_ARINTV_OFFSET               ((uint32_t)1U)

#define SDSTAT_STA0_OFFSET                ((uint32_t)1U)
#define SDSTAT_STA1_OFFSET                ((uint32_t)3U)

#define SRCMD_RWAITCYCLE_OFFSET           ((uint32_t)16U)
#define SWCMD_WWAITCYCLE_OFFSET           ((uint32_t)16U)

#define INTEN_INTS_OFFSET                 ((uint32_t)3U)

/*!
    简介:      deinitialize EXMC NOR/SRAM region
    参数[输入]:  EXMC_norsram_region: select the region of bank0
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK0_NORSRAM_REGIONx(x=0..3)
    参数[输出]:  无
    返回值:      无
*/
void EXMC_norsram_DeInit(uint32_t EXMC_norsram_region) {
    /* reset the registers */
    EXMC_SNCTL(EXMC_norsram_region) = BANK0_SNCTL_RESET;
    EXMC_SNTCFG(EXMC_norsram_region) = BANK0_SNTCFG_RESET;
    EXMC_SNWTCFG(EXMC_norsram_region) = BANK0_SNWTCFG_RESET;
}

/*!
    简介:      initialize EXMC_norsram_Parameter_Struct with the default values
    参数[输入]:  无
    参数[输出]: EXMC_norsram_init_Struct: the initialized struct EXMC_norsram_Parameter_Struct pointer
    返回值:      无
*/
void EXMC_norsram_Struct_Para_Init(EXMC_norsram_Parameter_Struct *EXMC_norsram_init_Struct) {
    /* configure the structure with default values */
    EXMC_norsram_init_Struct->norsram_region = EXMC_BANK0_NORSRAM_REGION0;
    EXMC_norsram_init_Struct->address_Data_mux = ENABLE;
    EXMC_norsram_init_Struct->memory_type = EXMC_Memory_TYPE_SRAM;
    EXMC_norsram_init_Struct->databus_width = EXMC_NOR_DATABUS_Width_8B;
    EXMC_norsram_init_Struct->burst_mode = DISABLE;
    EXMC_norsram_init_Struct->nwait_polarity = EXMC_NWAIT_Polarity_LOW;
    EXMC_norsram_init_Struct->wrap_Burst_mode = DISABLE;
    EXMC_norsram_init_Struct->nwait_Config = EXMC_NWAIT_Config_BEFORE;
    EXMC_norsram_init_Struct->memory_write = ENABLE;
    EXMC_norsram_init_Struct->nwait_signal = ENABLE;
    EXMC_norsram_init_Struct->extended_mode = DISABLE;
    EXMC_norsram_init_Struct->asyn_Wait = DISABLE;
    EXMC_norsram_init_Struct->write_mode = EXMC_ASYN_WRITE;

    /* configure read/write timing */
    EXMC_norsram_init_Struct->read_Write_timing->asyn_Address_setuptime = 0xFU;
    EXMC_norsram_init_Struct->read_Write_timing->asyn_Address_holdtime = 0xFU;
    EXMC_norsram_init_Struct->read_Write_timing->asyn_Data_setuptime = 0xFFU;
    EXMC_norsram_init_Struct->read_Write_timing->bus_latency = 0xFU;
    EXMC_norsram_init_Struct->read_Write_timing->syn_clk_division = EXMC_SYN_Clock_RATIO_16_CLK;
    EXMC_norsram_init_Struct->read_Write_timing->syn_Data_latency = EXMC_DATALAT_17_CLK;
    EXMC_norsram_init_Struct->read_Write_timing->asyn_access_mode = EXMC_ACCESS_Mode_A;

    /* configure write timing, when extended mode is used */
    EXMC_norsram_init_Struct->write_timing->asyn_Address_setuptime = 0xFU;
    EXMC_norsram_init_Struct->write_timing->asyn_Address_holdtime = 0xFU;
    EXMC_norsram_init_Struct->write_timing->asyn_Data_setuptime = 0xFFU;
    EXMC_norsram_init_Struct->write_timing->bus_latency = 0xFU;
    EXMC_norsram_init_Struct->write_timing->asyn_access_mode = EXMC_ACCESS_Mode_A;
}

/*!
    简介:      initialize EXMC NOR/SRAM region
    参数[输入]:  EXMC_norsram_Parameter_Struct: configure the EXMC NOR/SRAM parameter
                  norsram_region: EXMC_BANK0_NORSRAM_REGIONx, x=0..3
                  write_mode: EXMC_ASYN_WRITE, EXMC_SYN_WRITE
                  extended_mode: ENABLE or DISABLE
                  asyn_Wait: ENABLE or DISABLE
                  nwait_signal: ENABLE or DISABLE
                  memory_write: ENABLE or DISABLE
                  nwait_Config: EXMC_NWAIT_Config_BEFORE, EXMC_NWAIT_Config_DURING
                  wrap_Burst_mode: ENABLE or DISABLE
                  nwait_polarity: EXMC_NWAIT_Polarity_LOW, EXMC_NWAIT_Polarity_HIGH
                  burst_mode: ENABLE or DISABLE
                  databus_width: EXMC_NOR_DATABUS_Width_8B, EXMC_NOR_DATABUS_Width_16B
                  memory_type: EXMC_Memory_TYPE_SRAM, EXMC_Memory_TYPE_PSRAM, EXMC_Memory_TYPE_NOR
                  address_Data_mux: ENABLE or DISABLE
                  read_Write_timing: struct EXMC_norsram_timing_Parameter_Struct set the time
                    asyn_access_mode: EXMC_ACCESS_Mode_A, EXMC_ACCESS_Mode_B, EXMC_ACCESS_Mode_C, EXMC_ACCESS_Mode_D
                    syn_Data_latency: EXMC_DATALAT_x_CLK, x=2..17
                    syn_clk_division: EXMC_SYN_Clock_RATIO_DISABLE, EXMC_SYN_Clock_RATIO_x_CLK, x=2..16
                    bus_latency: 0x0U~0xFU
                    asyn_Data_setuptime: 0x01U~0xFFU
                    asyn_Address_holdtime: 0x1U~0xFU
                    asyn_Address_setuptime: 0x0U~0xFU
                  write_timing: struct EXMC_norsram_timing_Parameter_Struct set the time
                    asyn_access_mode: EXMC_ACCESS_Mode_A, EXMC_ACCESS_Mode_B, EXMC_ACCESS_Mode_C, EXMC_ACCESS_Mode_D
                    syn_Data_latency: EXMC_DATALAT_x_CLK, x=2..17
                    syn_clk_division: EXMC_SYN_Clock_RATIO_x_CLK, x=2..16
                    bus_latency: 0x0U~0xFU
                    asyn_Data_setuptime: 0x01U~0xFFU
                    asyn_Address_holdtime: 0x1U~0xFU
                    asyn_Address_setuptime: 0x0U~0xFU
    参数[输出]:  无
    返回值:      无
*/
void EXMC_norsram_Init(EXMC_norsram_Parameter_Struct *EXMC_norsram_init_Struct) {
    uint32_t snctl = 0x00000000U, sntcfg = 0x00000000U, snwtcfg = 0x00000000U;

    /* get the register value */
    snctl = EXMC_SNCTL(EXMC_norsram_init_Struct->norsram_region);

    /* clear relative bits */
    snctl &= ((uint32_t)~(EXMC_SNCTL_NREN | EXMC_SNCTL_NRTP | EXMC_SNCTL_NRW | EXMC_SNCTL_SBRSTEN |
                          EXMC_SNCTL_NRWTPOL | EXMC_SNCTL_WRAPEN | EXMC_SNCTL_NRWTCFG | EXMC_SNCTL_WEN |
                          EXMC_SNCTL_NRWTEN | EXMC_SNCTL_EXMODEN | EXMC_SNCTL_ASYNCWTEN | EXMC_SNCTL_SYNCWR |
                          EXMC_SNCTL_NRMUX));

    /* configure control bits */
    snctl |= (uint32_t)(EXMC_norsram_init_Struct->address_Data_mux << SNCTL_NRMUX_OFFSET) |
             EXMC_norsram_init_Struct->memory_type |
             EXMC_norsram_init_Struct->databus_width |
             (EXMC_norsram_init_Struct->burst_mode << SNCTL_SBRSTEN_OFFSET) |
             EXMC_norsram_init_Struct->nwait_polarity |
             (EXMC_norsram_init_Struct->wrap_Burst_mode << SNCTL_WRAPEN_OFFSET) |
             EXMC_norsram_init_Struct->nwait_Config |
             (EXMC_norsram_init_Struct->memory_write << SNCTL_WREN_OFFSET) |
             (EXMC_norsram_init_Struct->nwait_signal << SNCTL_NRWTEN_OFFSET) |
             (EXMC_norsram_init_Struct->extended_mode << SNCTL_EXMODEN_OFFSET) |
             (EXMC_norsram_init_Struct->asyn_Wait << SNCTL_ASYNCWAIT_OFFSET) |
             EXMC_norsram_init_Struct->write_mode;

    /* configure timing */
    sntcfg = (uint32_t)EXMC_norsram_init_Struct->read_Write_timing->asyn_Address_setuptime |
             (EXMC_norsram_init_Struct->read_Write_timing->asyn_Address_holdtime << SNTCFG_AHLD_OFFSET) |
             (EXMC_norsram_init_Struct->read_Write_timing->asyn_Data_setuptime << SNTCFG_DSET_OFFSET) |
             (EXMC_norsram_init_Struct->read_Write_timing->bus_latency << SNTCFG_BUSLAT_OFFSET) |
             EXMC_norsram_init_Struct->read_Write_timing->syn_clk_division |
             EXMC_norsram_init_Struct->read_Write_timing->syn_Data_latency |
             EXMC_norsram_init_Struct->read_Write_timing->asyn_access_mode;

    /* enable nor flash access */
    if(EXMC_Memory_TYPE_NOR == EXMC_norsram_init_Struct->memory_type) {
        snctl |= (uint32_t)EXMC_SNCTL_NREN;
    }

    /* configure extended mode */
    if(ENABLE == EXMC_norsram_init_Struct->extended_mode) {
        snwtcfg = (uint32_t)EXMC_norsram_init_Struct->write_timing->asyn_Address_setuptime |
                  (EXMC_norsram_init_Struct->write_timing->asyn_Address_holdtime << SNTCFG_AHLD_OFFSET) |
                  (EXMC_norsram_init_Struct->write_timing->asyn_Data_setuptime << SNTCFG_DSET_OFFSET) |
                  (EXMC_norsram_init_Struct->write_timing->bus_latency << SNTCFG_BUSLAT_OFFSET) |
                  EXMC_norsram_init_Struct->write_timing->asyn_access_mode;
    } else {
        snwtcfg = BANK0_SNWTCFG_RESET;
    }

    /* configure the registers */
    EXMC_SNCTL(EXMC_norsram_init_Struct->norsram_region) = snctl;
    EXMC_SNTCFG(EXMC_norsram_init_Struct->norsram_region) = sntcfg;
    EXMC_SNWTCFG(EXMC_norsram_init_Struct->norsram_region) = snwtcfg;
}

/*!
    简介:      enable EXMC NOR/PSRAM bank region
    参数[输入]:  EXMC_norsram_region: specify the region of NOR/PSRAM bank
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK0_NORSRAM_REGIONx(x=0..3)
    参数[输出]:  无
    返回值:      无
*/
void EXMC_norsram_Enable(uint32_t EXMC_norsram_region) {
    EXMC_SNCTL(EXMC_norsram_region) |= (uint32_t)EXMC_SNCTL_NRBKEN;
}

/*!
    简介:      disable EXMC NOR/PSRAM bank region
    参数[输入]:  EXMC_norsram_region: specify the region of NOR/PSRAM Bank
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK0_NORSRAM_REGIONx(x=0..3)
    参数[输出]:  无
    返回值:      无
*/
void EXMC_norsram_Disable(uint32_t EXMC_norsram_region) {
    EXMC_SNCTL(EXMC_norsram_region) &= ~(uint32_t)EXMC_SNCTL_NRBKEN;
}

/*!
    简介:      deinitialize EXMC NAND bank
    参数[输入]:  EXMC_nand_bank: select the bank of NAND
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANKx_NAND(x=1..2)
    参数[输出]:  无
    返回值:      无
*/
void EXMC_nand_DeInit(uint32_t EXMC_nand_bank) {
    /* EXMC_BANK1_NAND or EXMC_BANK2_NAND */
    EXMC_NPCTL(EXMC_nand_bank) = BANK1_2_NPCTL_RESET;
    EXMC_NPINTEN(EXMC_nand_bank) = BANK1_2_NPINTEN_RESET;
    EXMC_NPCTCFG(EXMC_nand_bank) = BANK1_2_NPCTCFG_RESET;
    EXMC_NPATCFG(EXMC_nand_bank) = BANK1_2_NPATCFG_RESET;
}

/*!
    简介:      initialize EXMC_norsram_Parameter_Struct with the default values
    参数[输入]:  无
    参数[输出]: the initialized struct EXMC_norsram_Parameter_Struct pointer
    返回值:      无
*/
void EXMC_nand_Struct_Para_Init(EXMC_nand_Parameter_Struct *EXMC_nand_init_Struct) {
    /* configure the structure with default values */
    EXMC_nand_init_Struct->nand_bank = EXMC_BANK1_NAND;
    EXMC_nand_init_Struct->wait_feature = DISABLE;
    EXMC_nand_init_Struct->databus_width = EXMC_NAND_DATABUS_Width_8B;
    EXMC_nand_init_Struct->ecc_logic = DISABLE;
    EXMC_nand_init_Struct->ecc_size = EXMC_ECC_SIZE_256BYTES;
    EXMC_nand_init_Struct->ctr_latency = 0x0U;
    EXMC_nand_init_Struct->atr_latency = 0x0U;
    EXMC_nand_init_Struct->common_space_timing->setuptime = 0xFCU;
    EXMC_nand_init_Struct->common_space_timing->waittime = 0xFCU;
    EXMC_nand_init_Struct->common_space_timing->holdtime = 0xFCU;
    EXMC_nand_init_Struct->common_space_timing->databus_hiztime = 0xFCU;
    EXMC_nand_init_Struct->attribute_space_timing->setuptime = 0xFCU;
    EXMC_nand_init_Struct->attribute_space_timing->waittime = 0xFCU;
    EXMC_nand_init_Struct->attribute_space_timing->holdtime = 0xFCU;
    EXMC_nand_init_Struct->attribute_space_timing->databus_hiztime = 0xFCU;
}

/*!
    简介:      initialize EXMC NAND bank
    参数[输入]:  EXMC_nand_Parameter_Struct: configure the EXMC NAND parameter
                  nand_bank: EXMC_BANK1_NAND,EXMC_BANK2_NAND
                  ecc_size: EXMC_ECC_SIZE_xBYTES,x=256,512,1024,2048,4096
                  atr_latency: EXMC_ALE_RE_Delay_x_HCLK,x=1..16
                  ctr_latency: EXMC_CLE_RE_Delay_x_HCLK,x=1..16
                  ecc_logic: ENABLE or DISABLE
                  databus_width: EXMC_NAND_DATABUS_Width_8B,EXMC_NAND_DATABUS_Width_16B
                  wait_feature: ENABLE or DISABLE
                  common_space_timing: struct EXMC_nand_pccard_timing_Parameter_Struct set the time
                    databus_hiztime: 0x01U~0xFFU
                    holdtime: 0x01U~0xFEU
                    waittime: 0x02U~0xFFU
                    setuptime: 0x01U~0xFFU
                  attribute_space_timing: struct EXMC_nand_pccard_timing_Parameter_Struct set the time
                    databus_hiztime: 0x00U~0xFEU
                    holdtime: 0x01U~0xFEU
                    waittime: 0x02U~0xFFU
                    setuptime: 0x01U~0xFFU
    参数[输出]:  无
    返回值:      无
*/
void EXMC_nand_Init(EXMC_nand_Parameter_Struct *EXMC_nand_init_Struct) {
    uint32_t npctl = 0x00000000U, npctcfg = 0x00000000U, npatcfg = 0x00000000U;

    npctl = (uint32_t)(EXMC_nand_init_Struct->wait_feature << NPCTL_NDWTEN_OFFSET) |
            EXMC_NPCTL_NDTP |
            EXMC_nand_init_Struct->databus_width |
            (EXMC_nand_init_Struct->ecc_logic << NPCTL_ECCEN_OFFSET) |
            EXMC_nand_init_Struct->ecc_size |
            EXMC_nand_init_Struct->ctr_latency |
            EXMC_nand_init_Struct->atr_latency;

    npctcfg = (uint32_t)((EXMC_nand_init_Struct->common_space_timing->setuptime - 1U) & EXMC_NPCTCFG_COMSET) |
              (((EXMC_nand_init_Struct->common_space_timing->waittime - 1U) << NPCTCFG_COMWAIT_OFFSET) & EXMC_NPCTCFG_COMWAIT) |
              ((EXMC_nand_init_Struct->common_space_timing->holdtime << NPCTCFG_COMHLD_OFFSET) & EXMC_NPCTCFG_COMHLD) |
              (((EXMC_nand_init_Struct->common_space_timing->databus_hiztime - 1U) << NPCTCFG_COMHIZ_OFFSET) & EXMC_NPCTCFG_COMHIZ);

    npatcfg = (uint32_t)((EXMC_nand_init_Struct->attribute_space_timing->setuptime - 1U) & EXMC_NPATCFG_ATTSET) |
              (((EXMC_nand_init_Struct->attribute_space_timing->waittime - 1U) << NPATCFG_ATTWAIT_OFFSET) & EXMC_NPATCFG_ATTWAIT) |
              ((EXMC_nand_init_Struct->attribute_space_timing->holdtime << NPATCFG_ATTHLD_OFFSET) & EXMC_NPATCFG_ATTHLD) |
              ((EXMC_nand_init_Struct->attribute_space_timing->databus_hiztime << NPATCFG_ATTHIZ_OFFSET) & EXMC_NPATCFG_ATTHIZ);

    /* initialize EXMC_BANK1_NAND or EXMC_BANK2_NAND */
    EXMC_NPCTL(EXMC_nand_init_Struct->nand_bank) = npctl;
    EXMC_NPCTCFG(EXMC_nand_init_Struct->nand_bank) = npctcfg;
    EXMC_NPATCFG(EXMC_nand_init_Struct->nand_bank) = npatcfg;
}

/*!
    简介:      enable NAND bank
    参数[输入]:  EXMC_nand_bank: specify the NAND bank
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANKx_NAND(x=1,2)
    参数[输出]:  无
    返回值:      无
*/
void EXMC_nand_Enable(uint32_t EXMC_nand_bank) {
    EXMC_NPCTL(EXMC_nand_bank) |= EXMC_NPCTL_NDBKEN;
}

/*!
    简介:      disable NAND bank
    参数[输入]:  EXMC_nand_bank: specify the NAND bank
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANKx_NAND(x=1,2)
    参数[输出]:  无
    返回值:      无
*/
void EXMC_nand_Disable(uint32_t EXMC_nand_bank) {
    EXMC_NPCTL(EXMC_nand_bank) &= ~EXMC_NPCTL_NDBKEN;
}

/*!
    简介:      deinitialize EXMC PC card bank
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void EXMC_pccard_DeInit(void) {
    /* EXMC_BANK3_PCCARD */
    EXMC_NPCTL3 = BANK3_NPCTL_RESET;
    EXMC_NPINTEN3 = BANK3_NPINTEN_RESET;
    EXMC_NPCTCFG3 = BANK3_NPCTCFG_RESET;
    EXMC_NPATCFG3 = BANK3_NPATCFG_RESET;
    EXMC_PIOTCFG3 = BANK3_PIOTCFG3_RESET;
}

/*!
    简介:      initialize EXMC_pccard_Parameter_Struct with the default values
    参数[输入]:  无
    参数[输出]: the initialized struct EXMC_pccard_Parameter_Struct pointer
    返回值:      无
*/
void EXMC_pccard_Struct_Para_Init(EXMC_pccard_Parameter_Struct *EXMC_pccard_init_Struct) {
    /* configure the structure with default values */
    EXMC_pccard_init_Struct->wait_feature = DISABLE;
    EXMC_pccard_init_Struct->ctr_latency = 0x0U;
    EXMC_pccard_init_Struct->atr_latency = 0x0U;
    EXMC_pccard_init_Struct->common_space_timing->setuptime = 0xFCU;
    EXMC_pccard_init_Struct->common_space_timing->waittime = 0xFCU;
    EXMC_pccard_init_Struct->common_space_timing->holdtime = 0xFCU;
    EXMC_pccard_init_Struct->common_space_timing->databus_hiztime = 0xFCU;
    EXMC_pccard_init_Struct->attribute_space_timing->setuptime = 0xFCU;
    EXMC_pccard_init_Struct->attribute_space_timing->waittime = 0xFCU;
    EXMC_pccard_init_Struct->attribute_space_timing->holdtime = 0xFCU;
    EXMC_pccard_init_Struct->attribute_space_timing->databus_hiztime = 0xFCU;
    EXMC_pccard_init_Struct->io_space_timing->setuptime = 0xFCU;
    EXMC_pccard_init_Struct->io_space_timing->waittime = 0xFCU;
    EXMC_pccard_init_Struct->io_space_timing->holdtime = 0xFCU;
    EXMC_pccard_init_Struct->io_space_timing->databus_hiztime = 0xFCU;
}

/*!
    简介:      initialize EXMC PC card bank
    参数[输入]:  EXMC_pccard_Parameter_Struct: configure the EXMC NAND parameter
                  atr_latency: EXMC_ALE_RE_Delay_x_HCLK,x=1..16
                  ctr_latency: EXMC_CLE_RE_Delay_x_HCLK,x=1..16
                  wait_feature: ENABLE or DISABLE
                  common_space_timing: struct EXMC_nand_pccard_timing_Parameter_Struct set the time
                    databus_hiztime: 0x01U~0xFFU
                    holdtime: 0x01U~0xFEU
                    waittime: 0x02U~0xFFU
                    setuptime: 0x01U~0xFFU
                  attribute_space_timing: struct EXMC_nand_pccard_timing_Parameter_Struct set the time
                    databus_hiztime: 0x00U~0xFEU
                    holdtime: 0x01U~0xFEU
                    waittime: 0x02U~0xFFU
                    setuptime: 0x01U~0xFFU
                  io_space_timing: EXMC_nand_pccard_timing_Parameter_Struct set the time
                    databus_hiztime: 0x00U~0xFFU
                    holdtime: 0x01U~0xFFU
                    waittime: 0x02U~0x100U
                    setuptime: 0x01U~0x100U
    参数[输出]:  无
    返回值:      无
*/
void EXMC_pccard_Init(EXMC_pccard_Parameter_Struct *EXMC_pccard_init_Struct) {
    /* configure the EXMC bank3 PC card control register */
    EXMC_NPCTL3 = (uint32_t)(EXMC_pccard_init_Struct->wait_feature << NPCTL_NDWTEN_OFFSET) |
                  EXMC_NAND_DATABUS_Width_16B |
                  EXMC_pccard_init_Struct->ctr_latency |
                  EXMC_pccard_init_Struct->atr_latency ;

    /* configure the EXMC bank3 PC card common space timing configuration register */
    EXMC_NPCTCFG3 = (uint32_t)((EXMC_pccard_init_Struct->common_space_timing->setuptime - 1U) & EXMC_NPCTCFG_COMSET) |
                    (((EXMC_pccard_init_Struct->common_space_timing->waittime - 1U) << NPCTCFG_COMWAIT_OFFSET) & EXMC_NPCTCFG_COMWAIT) |
                    ((EXMC_pccard_init_Struct->common_space_timing->holdtime << NPCTCFG_COMHLD_OFFSET) & EXMC_NPCTCFG_COMHLD) |
                    (((EXMC_pccard_init_Struct->common_space_timing->databus_hiztime - 1U) << NPCTCFG_COMHIZ_OFFSET) & EXMC_NPCTCFG_COMHIZ);

    /* configure the EXMC bank3 PC card attribute space timing configuration register */
    EXMC_NPATCFG3 = (uint32_t)((EXMC_pccard_init_Struct->attribute_space_timing->setuptime - 1U) & EXMC_NPATCFG_ATTSET) |
                    (((EXMC_pccard_init_Struct->attribute_space_timing->waittime - 1U) << NPATCFG_ATTWAIT_OFFSET) & EXMC_NPATCFG_ATTWAIT) |
                    ((EXMC_pccard_init_Struct->attribute_space_timing->holdtime << NPATCFG_ATTHLD_OFFSET) & EXMC_NPATCFG_ATTHLD) |
                    ((EXMC_pccard_init_Struct->attribute_space_timing->databus_hiztime << NPATCFG_ATTHIZ_OFFSET) & EXMC_NPATCFG_ATTHIZ);

    /* configure the EXMC bank3 PC card io space timing configuration register */
    EXMC_PIOTCFG3 = (uint32_t)((EXMC_pccard_init_Struct->io_space_timing->setuptime - 1U) & EXMC_PIOTCFG3_IOSET) |
                    (((EXMC_pccard_init_Struct->io_space_timing->waittime - 1U) << PIOTCFG_IOWAIT_OFFSET) & EXMC_PIOTCFG3_IOWAIT) |
                    ((EXMC_pccard_init_Struct->io_space_timing->holdtime << PIOTCFG_IOHLD_OFFSET) & EXMC_PIOTCFG3_IOHLD) |
                    ((EXMC_pccard_init_Struct->io_space_timing->databus_hiztime << PIOTCFG_IOHIZ_OFFSET) & EXMC_PIOTCFG3_IOHIZ);
}

/*!
    简介:      enable PC Card Bank
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void EXMC_pccard_Enable(void) {
    EXMC_NPCTL3 |= EXMC_NPCTL_NDBKEN;
}

/*!
    简介:      disable PC Card Bank
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void EXMC_pccard_Disable(void) {
    EXMC_NPCTL3 &= ~EXMC_NPCTL_NDBKEN;
}

/*!
    简介:      deinitialize EXMC SDRAM device
   参数[输入]:   EXMC_sdram_device: select the SRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_SDRAM_DEVICEx(x=0, 1)
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sdram_DeInit(uint32_t EXMC_sdram_device) {
    /* reset SDRAM registers */
    EXMC_SDCTL(EXMC_sdram_device) = SDRAM_DEVICE_SDCTL_RESET;
    EXMC_SDTCFG(EXMC_sdram_device) = SDRAM_DEVICE_SDTCFG_RESET;
    EXMC_SDCMD = SDRAM_DEVICE_SDCMD_RESET;
    EXMC_SDARI = SDRAM_DEVICE_SDARI_RESET;
    EXMC_SDRSCTL = SDRAM_DEVICE_SDRSCTL_RESET;
}

/*!
    简介:      initialize EXMC_sdram_Parameter_Struct with the default values
    参数[输入]:  无
    参数[输出]: the initialized struct EXMC_pccard_Parameter_Struct pointer
    返回值:      无
*/
void EXMC_sdram_Struct_Para_Init(EXMC_sdram_Parameter_Struct *EXMC_sdram_init_Struct) {
    /* configure the structure with default values */
    EXMC_sdram_init_Struct->sdram_device = EXMC_SDRAM_DEVICE0;
    EXMC_sdram_init_Struct->column_Address_width = EXMC_SDRAM_COW_Address_8;
    EXMC_sdram_init_Struct->row_Address_width = EXMC_SDRAM_ROW_Address_11;
    EXMC_sdram_init_Struct->data_width = EXMC_SDRAM_DATABUS_Width_16B;
    EXMC_sdram_init_Struct->internal_bank_number = EXMC_SDRAM_4_INTER_BANK;
    EXMC_sdram_init_Struct->cas_latency = EXMC_CAS_LATENCY_1_SDCLK;
    EXMC_sdram_init_Struct->write_Protection = ENABLE;
    EXMC_sdram_init_Struct->sdclock_Config = EXMC_SDCLK_DISABLE;
    EXMC_sdram_init_Struct->burst_read_switch = DISABLE;
    EXMC_sdram_init_Struct->pipeline_read_delay = EXMC_PIPELINE_Delay_0_HCLK;

    EXMC_sdram_init_Struct->timing->load_Mode_Register_delay = 16U;
    EXMC_sdram_init_Struct->timing->exit_selfrefresh_delay = 16U;
    EXMC_sdram_init_Struct->timing->row_Address_Select_delay = 16U;
    EXMC_sdram_init_Struct->timing->auto_refresh_delay = 16U;
    EXMC_sdram_init_Struct->timing->write_recovery_delay = 16U;
    EXMC_sdram_init_Struct->timing->row_precharge_delay = 16U;
    EXMC_sdram_init_Struct->timing->row_to_column_delay = 16U;
}

/*!
    简介:      initialize EXMC SDRAM device
    参数[输入]:  EXMC_sdram_Parameter_Struct: configure the EXMC SDRAM parameter
                  sdram_device: EXMC_SDRAM_DEVICE0,EXMC_SDRAM_DEVICE1
                  pipeline_read_delay: EXMC_PIPELINE_Delay_x_HCLK,x=0..2
                  burst_read_switch: ENABLE or DISABLE
                  sdclock_Config: EXMC_SDCLK_DISABLE,EXMC_SDCLK_PERIODS_2_HCLK,EXMC_SDCLK_PERIODS_3_HCLK
                  write_Protection: ENABLE or DISABLE
                  cas_latency: EXMC_CAS_LATENCY_x_SDCLK,x=1..3
                  internal_bank_number: EXMC_SDRAM_2_INTER_BANK,EXMC_SDRAM_4_INTER_BANK
                  data_width: EXMC_SDRAM_DATABUS_Width_8B,EXMC_SDRAM_DATABUS_Width_16B,EXMC_SDRAM_DATABUS_Width_32B
                  row_Address_width: EXMC_SDRAM_ROW_Address_x,x=11..13
                  column_Address_width: EXMC_SDRAM_COW_Address_x,x=8..11
                  timing: EXMC_sdram_timing_Parameter_Struct set the time
                    row_to_column_delay: 1U~16U
                    row_precharge_delay: 1U~16U
                    write_recovery_delay: 1U~16U
                    auto_refresh_delay: 1U~16U
                    row_Address_Select_delay: 1U~16U
                    exit_selfrefresh_delay: 1U~16U
                    load_Mode_Register_delay: 1U~16U
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sdram_Init(EXMC_sdram_Parameter_Struct *EXMC_sdram_init_Struct) {
    uint32_t sdctl0, sdctl1, sdtcfg0, sdtcfg1;

    /* configure EXMC_SDCTL0 or EXMC_SDCTL1 */
    if(EXMC_SDRAM_DEVICE0 == EXMC_sdram_init_Struct->sdram_device) {
        /* configure EXMC_SDCTL0 */
        EXMC_SDCTL(EXMC_SDRAM_DEVICE0)  = (uint32_t)(EXMC_sdram_init_Struct->column_Address_width |
                                          EXMC_sdram_init_Struct->row_Address_width |
                                          EXMC_sdram_init_Struct->data_width |
                                          EXMC_sdram_init_Struct->internal_bank_number |
                                          EXMC_sdram_init_Struct->cas_latency |
                                          (EXMC_sdram_init_Struct->write_Protection << SDCTL_WPEN_OFFSET) |
                                          EXMC_sdram_init_Struct->sdclock_Config |
                                          (EXMC_sdram_init_Struct->burst_read_switch << SDCTL_BRSTRD_OFFSET) |
                                          EXMC_sdram_init_Struct->pipeline_read_delay);

        /* configure EXMC_SDTCFG0 */
        EXMC_SDTCFG(EXMC_SDRAM_DEVICE0) = (uint32_t)((EXMC_sdram_init_Struct->timing->load_Mode_Register_delay) - 1U) |
                                          (((EXMC_sdram_init_Struct->timing->exit_selfrefresh_delay) - 1U) << SDTCFG_XSRD_OFFSET) |
                                          (((EXMC_sdram_init_Struct->timing->row_Address_Select_delay) - 1U) << SDTCFG_RASD_OFFSET) |
                                          (((EXMC_sdram_init_Struct->timing->auto_refresh_delay) - 1U) << SDTCFG_ARFD_OFFSET) |
                                          (((EXMC_sdram_init_Struct->timing->write_recovery_delay) - 1U) << SDTCFG_WRD_OFFSET) |
                                          (((EXMC_sdram_init_Struct->timing->row_precharge_delay) - 1U) << SDTCFG_RPD_OFFSET) |
                                          (((EXMC_sdram_init_Struct->timing->row_to_column_delay) - 1U) << SDTCFG_RCD_OFFSET);
    } else {
        /* configure EXMC_SDCTL0 and EXMC_SDCTL1 */
        /* some bits in the EXMC_SDCTL1 register are reserved */
        sdctl0 = EXMC_SDCTL(EXMC_SDRAM_DEVICE0) & (~(EXMC_SDCTL_PIPED | EXMC_SDCTL_BRSTRD | EXMC_SDCTL_SDCLK));

        sdctl0 |= (uint32_t)(EXMC_sdram_init_Struct->sdclock_Config |
                             (EXMC_sdram_init_Struct->burst_read_switch << SDCTL_BRSTRD_OFFSET) |
                             EXMC_sdram_init_Struct->pipeline_read_delay);

        sdctl1 = (uint32_t)(EXMC_sdram_init_Struct->column_Address_width |
                            EXMC_sdram_init_Struct->row_Address_width |
                            EXMC_sdram_init_Struct->data_width |
                            EXMC_sdram_init_Struct->internal_bank_number |
                            EXMC_sdram_init_Struct->cas_latency |
                            (EXMC_sdram_init_Struct->write_Protection << SDCTL_WPEN_OFFSET));

        EXMC_SDCTL(EXMC_SDRAM_DEVICE0) = sdctl0;
        EXMC_SDCTL(EXMC_SDRAM_DEVICE1) = sdctl1;

        /* configure EXMC_SDTCFG0 and EXMC_SDTCFG1 */
        /* some bits in the EXMC_SDTCFG1 register are reserved */
        sdtcfg0 = EXMC_SDTCFG(EXMC_SDRAM_DEVICE0) & (~(EXMC_SDTCFG_RPD | EXMC_SDTCFG_WRD | EXMC_SDTCFG_ARFD));

        sdtcfg0 |= (uint32_t)((((EXMC_sdram_init_Struct->timing->auto_refresh_delay) - 1U) << SDTCFG_ARFD_OFFSET) |
                              (((EXMC_sdram_init_Struct->timing->row_precharge_delay) - 1U) << SDTCFG_RPD_OFFSET) |
                              (((EXMC_sdram_init_Struct->timing->write_recovery_delay) - 1U) << SDTCFG_WRD_OFFSET));

        sdtcfg1 = (uint32_t)(((EXMC_sdram_init_Struct->timing->load_Mode_Register_delay) - 1U) |
                             (((EXMC_sdram_init_Struct->timing->exit_selfrefresh_delay) - 1U) << SDTCFG_XSRD_OFFSET) |
                             (((EXMC_sdram_init_Struct->timing->row_Address_Select_delay) - 1U) << SDTCFG_RASD_OFFSET) |
                             (((EXMC_sdram_init_Struct->timing->row_to_column_delay) - 1U) << SDTCFG_RCD_OFFSET));

        EXMC_SDTCFG(EXMC_SDRAM_DEVICE0) = sdtcfg0;
        EXMC_SDTCFG(EXMC_SDRAM_DEVICE1) = sdtcfg1;
    }
}

/*!
    简介:      initialize EXMC_sdram_Struct_command_Para_init with the default values
    参数[输入]:  无
    参数[输出]: the initialized struct EXMC_sdram_Struct_command_Para_init pointer
    返回值:      无
*/
void EXMC_sdram_Struct_command_Para_Init(EXMC_sdram_command_Parameter_Struct *EXMC_sdram_command_init_Struct) {
    /* configure the structure with default value */
    EXMC_sdram_command_init_Struct->mode_Register_content = 0U;
    EXMC_sdram_command_init_Struct->auto_refresh_number = EXMC_SDRAM_Auto_REFLESH_1_SDCLK;
    EXMC_sdram_command_init_Struct->bank_select = EXMC_SDRAM_DEVICE0_SELECT;
    EXMC_sdram_command_init_Struct->command = EXMC_SDRAM_Normal_OPERATION;
}

/*!
    简介:      deinitialize exmc SQPIPSRAM
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sqpipsram_DeInit(void) {
    /* reset the registers */
    EXMC_SINIT = BANK0_SQPI_SINIT_RESET;
    EXMC_SRCMD = BANK0_SQPI_SRCMD_RESET;
    EXMC_SWCMD = BANK0_SQPI_SWCMD_RESET;
    EXMC_SIDL = BANK0_SQPI_SIDL_RESET;
    EXMC_SIDH = BANK0_SQPI_SIDH_RESET;
}

/*!
    简介:      initialize EXMC_sqpipsram_Parameter_Struct with the default values
    参数[输入]:  the struct EXMC_sqpipsram_Parameter_Struct pointer
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sqpipsram_Struct_Para_Init(EXMC_sqpipsram_Parameter_Struct *EXMC_sqpipsram_init_Struct) {
    /* configure the structure with default values */
    EXMC_sqpipsram_init_Struct->sample_polarity = EXMC_SQPIPSRAM_Sample_RISING_EDGE;
    EXMC_sqpipsram_init_Struct->id_length = EXMC_SQPIPSRAM_ID_Length_64B;
    EXMC_sqpipsram_init_Struct->address_bits = EXMC_SQPIPSRAM_ADDR_Length_24B;
    EXMC_sqpipsram_init_Struct->command_bits = EXMC_SQPIPSRAM_COMMAND_Length_8B;
}

/*!
    简介:      initialize EXMC SQPIPSRAM
    参数[输入]:  EXMC_sqpipsram_Parameter_Struct: configure the EXMC SQPIPSRAM parameter
                  sample_polarity: EXMC_SQPIPSRAM_Sample_RISING_EDGE,EXMC_SQPIPSRAM_Sample_FALLING_EDGE
                  id_length: EXMC_SQPIPSRAM_ID_Length_xB,x=8,16,32,64
                  address_bits: EXMC_SQPIPSRAM_ADDR_Length_xB,x=1..26
                  command_bits: EXMC_SQPIPSRAM_COMMAND_Length_xB,x=4,8,16
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sqpipsram_Init(EXMC_sqpipsram_Parameter_Struct *EXMC_sqpipsram_init_Struct) {
    /* initialize SQPI controller */
    EXMC_SINIT = (uint32_t)EXMC_sqpipsram_init_Struct->sample_polarity |
                 EXMC_sqpipsram_init_Struct->id_length |
                 EXMC_sqpipsram_init_Struct->address_bits |
                 EXMC_sqpipsram_init_Struct->command_bits;
}

/*!
    简介:      configure consecutive clock
    参数[输入]:  clock_mode: specify when the clock is generated
                only one parameter can be selected which is shown as below:
      参数:        EXMC_Clock_SYN_MODE: the clock is generated only during synchronous access
      参数:        EXMC_Clock_UNCONDITIONALLY: the clock is generated unconditionally
    参数[输出]:  无
    返回值:      无
*/
void EXMC_norsram_consecutive_Clock_Config(uint32_t clock_mode) {
    if(EXMC_Clock_UNCONDITIONALLY == clock_mode) {
        EXMC_SNCTL(EXMC_BANK0_NORSRAM_REGION0) |= EXMC_Clock_UNCONDITIONALLY;
    } else {
        EXMC_SNCTL(EXMC_BANK0_NORSRAM_REGION0) &= ~EXMC_Clock_UNCONDITIONALLY;
    }
}

/*!
    简介:      configure CRAM page size
    参数[输入]:  EXMC_norsram_region: select the region of bank0
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK0_NORSRAM_REGIONx(x=0..3)
    参数[输入]:  page_size: CRAM page size
                only one parameter can be selected which is shown as below:
      参数:        EXMC_CRAM_Auto_SPLIT: the clock is generated only during synchronous access
      参数:        EXMC_CRAM_PAGE_SIZE_128_BYTES: page size is 128 bytes
      参数:        EXMC_CRAM_PAGE_SIZE_256_BYTES: page size is 256 bytes
      参数:        EXMC_CRAM_PAGE_SIZE_512_BYTES: page size is 512 bytes
      参数:        EXMC_CRAM_PAGE_SIZE_1024_BYTES: page size is 1024 bytes
    参数[输出]:  无
    返回值:      无
*/
void EXMC_norsram_page_size_Config(uint32_t EXMC_norsram_region, uint32_t page_size) {
    /* reset the bits */
    EXMC_SNCTL(EXMC_norsram_region) &= ~EXMC_SNCTL_CPS;

    /* set the CPS bits */
    EXMC_SNCTL(EXMC_norsram_region) |= page_size;
}

/*!
    简介:      enable or disable the EXMC NAND ECC function
    参数[输入]:  EXMC_nand_bank: specify the NAND bank
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANKx_NAND(x=1,2)
    参数[输入]:  newvalue: ENABLE or DISABLE
    参数[输出]:  无
    返回值:      无
*/
void EXMC_nand_ecc_Config(uint32_t EXMC_nand_bank, ControlStatus newvalue) {
    if(ENABLE == newvalue) {
        /* enable the selected NAND bank ECC function */
        EXMC_NPCTL(EXMC_nand_bank) |= EXMC_NPCTL_ECCEN;
    } else {
        /* disable the selected NAND bank ECC function */
        EXMC_NPCTL(EXMC_nand_bank) &= ~EXMC_NPCTL_ECCEN;
    }
}

/*!
    简介:      get the EXMC ECC value
    参数[输入]:  EXMC_nand_bank: specify the NAND bank
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANKx_NAND(x=1,2)
    参数[输出]:  无
    返回值:     the error correction code(ECC) value
*/
uint32_t EXMC_ecc_Get(uint32_t EXMC_nand_bank) {
    return(EXMC_NECC(EXMC_nand_bank));
}

/*!
    简介:      enable or disable read sample
    参数[输入]:  newvalue: ENABLE or DISABLE
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sdram_readsample_Enable(ControlStatus newvalue) {
    if(ENABLE == newvalue) {
        EXMC_SDRSCTL |=  EXMC_SDRSCTL_RSEN;
    } else {
        EXMC_SDRSCTL &= (uint32_t)(~EXMC_SDRSCTL_RSEN);
    }
}

/*!
    简介:      configure the delayed sample clock of read data
    参数[输入]:  delay_cell: SDRAM the delayed sample clock of read data
                only one parameter can be selected which is shown as below:
      参数:        EXMC_SDRAM_x_Delay_CELL(x=0..15)
    参数[输入]:  extra_hclk: sample cycle of read data
                only one parameter can be selected which is shown as below:
      参数:        EXMC_SDRAM_READSAMPLE_x_EXTRAHCLK(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sdram_readsample_Config(uint32_t delay_cell, uint32_t extra_hclk) {
    uint32_t sdrsctl = 0U;

    /* reset the bits */
    sdrsctl = EXMC_SDRSCTL & (~(EXMC_SDRSCTL_SDSC | EXMC_SDRSCTL_SSCR));
    /* set the bits */
    sdrsctl |= (uint32_t)(delay_cell | extra_hclk);
    EXMC_SDRSCTL = sdrsctl;
}

/*!
    简介:      configure the SDRAM memory command
    参数[输入]:  EXMC_sdram_command_init_Struct: initialize EXMC SDRAM command
                  mode_Register_content:
                  auto_refresh_number: EXMC_SDRAM_Auto_REFLESH_x_SDCLK, x=1..15
                  bank_select: EXMC_SDRAM_DEVICE0_SELECT, EXMC_SDRAM_DEVICE1_SELECT, EXMC_SDRAM_DEVICE0_1_SELECT
                  command: EXMC_SDRAM_Normal_OPERATION, EXMC_SDRAM_Clock_ENABLE, EXMC_SDRAM_PRECHARGE_ALL,
                           EXMC_SDRAM_Auto_REFRESH, EXMC_SDRAM_LOAD_Mode_REGISTER, EXMC_SDRAM_SELF_REFRESH,
                           EXMC_SDRAM_POWERDOWN_ENTRY
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sdram_command_Config(EXMC_sdram_command_Parameter_Struct *EXMC_sdram_command_init_Struct) {
    /* configure command register */
    EXMC_SDCMD = (uint32_t)((EXMC_sdram_command_init_Struct->command) |
                            (EXMC_sdram_command_init_Struct->bank_select) |
                            ((EXMC_sdram_command_init_Struct->auto_refresh_number)) |
                            ((EXMC_sdram_command_init_Struct->mode_Register_content) << SDCMD_MRC_OFFSET));
}

/*!
    简介:      set auto-refresh interval
    参数[输入]:  EXMC_count: the number SDRAM clock cycles unit between two successive auto-refresh commands, 0x0000~0x1FFF
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sdram_refresh_count_Set(uint32_t EXMC_count) {
    uint32_t sdari;
    sdari = EXMC_SDARI & (~EXMC_SDARI_ARINTV);
    EXMC_SDARI = sdari | (uint32_t)((EXMC_count << SDARI_ARINTV_OFFSET) & EXMC_SDARI_ARINTV);
}

/*!
    简介:      set the number of successive auto-refresh command
    参数[输入]:  EXMC_number: the number of successive Auto-refresh cycles will be send, 1~15
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sdram_autorefresh_Number_Set(uint32_t EXMC_number) {
    uint32_t sdcmd;
    sdcmd = EXMC_SDCMD & (~EXMC_SDCMD_NARF);
    EXMC_SDCMD = sdcmd | (uint32_t)((EXMC_number << SDCMD_NARF_OFFSET) & EXMC_SDCMD_NARF) ;
}

/*!
    简介:      configure the write protection function
    参数[输入]:  EXMC_sdram_device: specify the SDRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_SDRAM_DEVICEx(x=0,1)
    参数[输入]:  newvalue: ENABLE or DISABLE
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sdram_Write_Protection_Config(uint32_t EXMC_sdram_device, ControlStatus newvalue) {
    if(ENABLE == newvalue) {
        EXMC_SDCTL(EXMC_sdram_device) |= (uint32_t)EXMC_SDCTL_WPEN;
    } else {
        EXMC_SDCTL(EXMC_sdram_device) &= ~((uint32_t)EXMC_SDCTL_WPEN);
    }

}

/*!
    简介:      get the status of SDRAM device0 or device1
    参数[输入]:  EXMC_sdram_device: specify the SDRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_SDRAM_DEVICEx(x=0,1)
    参数[输出]:  无
    返回值:     the status of SDRAM device
*/
uint32_t EXMC_sdram_bankstatus_Get(uint32_t EXMC_sdram_device) {
    uint32_t sdstat = 0U;

    if(EXMC_SDRAM_DEVICE0 == EXMC_sdram_device) {
        sdstat = ((uint32_t)(EXMC_SDSTAT & EXMC_SDSDAT_STA0) >> SDSTAT_STA0_OFFSET);
    } else {
        sdstat = ((uint32_t)(EXMC_SDSTAT & EXMC_SDSDAT_STA1) >> SDSTAT_STA1_OFFSET);
    }

    return sdstat;
}

/*!
    简介:      set the read command
    参数[输入]:  read_command_mode: configure SPI PSRAM read command mode
                only one parameter can be selected which is shown as below:
      参数:        EXMC_SQPIPSRAM_READ_Mode_DISABLE: not SPI mode
      参数:        EXMC_SQPIPSRAM_READ_Mode_SPI: SPI mode
      参数:        EXMC_SQPIPSRAM_READ_Mode_SQPI: SQPI mode
      参数:        EXMC_SQPIPSRAM_READ_Mode_QPI: QPI mode
    参数[输入]:  read_Wait_cycle: wait cycle number after address phase,0..15
    参数[输入]:  read_command_code: read command for AHB read transfer
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sqpipsram_read_command_Set(uint32_t read_command_mode, uint32_t read_Wait_cycle, uint32_t read_command_code) {
    uint32_t srcmd;

    srcmd = (uint32_t) read_command_mode |
            ((read_Wait_cycle << SRCMD_RWAITCYCLE_OFFSET) & EXMC_SRCMD_RWAITCYCLE) |
            ((read_command_code & EXMC_SRCMD_RCMD));
    EXMC_SRCMD = srcmd;
}

/*!
    简介:      set the write command
    参数[输入]:  write_command_mode: configure SPI PSRAM write command mode
                only one parameter can be selected which is shown as below:
      参数:        EXMC_SQPIPSRAM_Write_Mode_DISABLE: not SPI mode
      参数:        EXMC_SQPIPSRAM_Write_Mode_SPI: SPI mode
      参数:        EXMC_SQPIPSRAM_Write_Mode_SQPI: SQPI mode
      参数:        EXMC_SQPIPSRAM_Write_Mode_QPI: QPI mode
    参数[输入]:  write_Wait_cycle: wait cycle number after address phase,0..15
    参数[输入]:  write_command_code: read command for AHB read transfer
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sqpipsram_Write_command_Set(uint32_t write_command_mode, uint32_t write_Wait_cycle, uint32_t write_command_code) {
    uint32_t swcmd;

    swcmd = (uint32_t) write_command_mode |
            ((write_Wait_cycle << SWCMD_WWAITCYCLE_OFFSET) & EXMC_SWCMD_WWAITCYCLE) |
            ((write_command_code & EXMC_SWCMD_WCMD));
    EXMC_SWCMD = swcmd;
}

/*!
    简介:      send SPI read ID command
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sqpipsram_read_ID_command_send(void) {
    EXMC_SRCMD |= EXMC_SRCMD_RDID;
}

/*!
    简介:      send SPI special command which does not have address and data phase
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void EXMC_sqpipsram_Write_cmd_send(void) {
    EXMC_SWCMD |= EXMC_SWCMD_SC;
}

/*!
    简介:      get the EXMC SPI ID low data
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the ID low data
*/
uint32_t EXMC_sqpipsram_Low_ID_Get(void) {
    return (EXMC_SIDL);
}

/*!
    简介:      get the EXMC SPI ID high data
    参数[输入]:  无
    参数[输出]:  无
    返回值:     the ID high data
*/
uint32_t EXMC_sqpipsram_high_ID_Get(void) {
    return (EXMC_SIDH);
}

/*!
    简介:      get the bit value of EXMC send write command bit or read ID command
    参数[输入]:  send_command_flag: the send command flag
                only one parameter can be selected which is shown as below:
      参数:        EXMC_Send_COMMAND_Flag_RDID: EXMC_SRCMD_RDID flag bit
      参数:        EXMC_Send_COMMAND_Flag_SC: EXMC_SWCMD_SC flag bit
    参数[输出]:  无
    返回值:     the new value of send command flag
*/
FlagStatus EXMC_sqpipsram_Send_command_State_Get(uint32_t send_command_flag) {
    uint32_t flag = 0x00000000U;

    if(EXMC_Send_COMMAND_Flag_RDID == send_command_flag) {
        flag = EXMC_SRCMD;
    } else if(EXMC_Send_COMMAND_Flag_SC == send_command_flag) {
        flag = EXMC_SWCMD;
    } else {
    }

    if(flag & send_command_flag) {
        /* flag is set */
        return SET;
    } else {
        /* flag is reset */
        return RESET;
    }
}

/*!
    简介:      enable EXMC interrupt
    参数[输入]:  EXMC_bank: specify the NAND bank,PC card bank or SDRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK1_NAND: the NAND bank1
      参数:        EXMC_BANK2_NAND: the NAND bank2
      参数:        EXMC_BANK3_PCCARD: the PC card bank
      参数:        EXMC_SDRAM_DEVICE0: the SDRAM device0
      参数:        EXMC_SDRAM_DEVICE1: the SDRAM device1
    参数[输入]:  interrupt: specify EXMC interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        EXMC_NAND_PCCARD_INT_Flag_RISE: rising edge interrupt and flag
      参数:        EXMC_NAND_PCCARD_INT_Flag_LEVEL: high-level interrupt and flag
      参数:        EXMC_NAND_PCCARD_INT_Flag_FALL: falling edge interrupt and flag
      参数:        EXMC_SDRAM_INT_Flag_REFRESH: refresh error interrupt and flag
    参数[输出]:  无
    返回值:      无
*/
void EXMC_Interrupt_Enable(uint32_t EXMC_bank, uint32_t interrupt) {
    if((EXMC_BANK1_NAND == EXMC_bank) || (EXMC_BANK2_NAND == EXMC_bank) || (EXMC_BANK3_PCCARD == EXMC_bank)) {
        /* NAND bank1,bank2 or PC card bank3 */
        EXMC_NPINTEN(EXMC_bank) |= interrupt;
    } else {
        /* SDRAM device0 or device1 */
        EXMC_SDARI |= EXMC_SDARI_REIE;
    }
}

/*!
    简介:      disable EXMC interrupt
    参数[输入]:  EXMC_bank: specify the NAND bank , PC card bank or SDRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK1_NAND: the NAND bank1
      参数:        EXMC_BANK2_NAND: the NAND bank2
      参数:        EXMC_BANK3_PCCARD: the PC card bank
      参数:        EXMC_SDRAM_DEVICE0: the SDRAM device0
      参数:        EXMC_SDRAM_DEVICE1: the SDRAM device1
    参数[输入]:  interrupt: specify EXMC interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        EXMC_NAND_PCCARD_INT_Flag_RISE: rising edge interrupt and flag
      参数:        EXMC_NAND_PCCARD_INT_Flag_LEVEL: high-level interrupt and flag
      参数:        EXMC_NAND_PCCARD_INT_Flag_FALL: falling edge interrupt and flag
      参数:        EXMC_SDRAM_INT_Flag_REFRESH: refresh error interrupt and flag
    参数[输出]:  无
    返回值:      无
*/
void EXMC_Interrupt_Disable(uint32_t EXMC_bank, uint32_t interrupt) {
    if((EXMC_BANK1_NAND == EXMC_bank) || (EXMC_BANK2_NAND == EXMC_bank) || (EXMC_BANK3_PCCARD == EXMC_bank)) {
        /* NAND bank1,bank2 or PC card bank3 */
        EXMC_NPINTEN(EXMC_bank) &= ~interrupt;
    } else {
        /* SDRAM device0 or device1 */
        EXMC_SDARI &= ~EXMC_SDARI_REIE;
    }
}

/*!
    简介:      get EXMC flag status
    参数[输入]:  EXMC_bank: specify the NAND bank , PC card bank or SDRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK1_NAND: the NAND bank1
      参数:        EXMC_BANK2_NAND: the NAND bank2
      参数:        EXMC_BANK3_PCCARD: the PC Card bank
      参数:        EXMC_SDRAM_DEVICE0: the SDRAM device0
      参数:        EXMC_SDRAM_DEVICE1: the SDRAM device1
    参数[输入]:  flag: EXMC status and flag
                only one parameter can be selected which is shown as below:
      参数:        EXMC_NAND_PCCARD_Flag_RISE: interrupt rising edge status
      参数:        EXMC_NAND_PCCARD_Flag_LEVEL: interrupt high-level status
      参数:        EXMC_NAND_PCCARD_Flag_FALL: interrupt falling edge status
      参数:        EXMC_NAND_PCCARD_Flag_FIFOE: FIFO empty flag
      参数:        EXMC_SDRAM_Flag_REFRESH: refresh error interrupt flag
      参数:        EXMC_SDRAM_Flag_NREADY: not ready status
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus EXMC_Flag_Get(uint32_t EXMC_bank, uint32_t flag) {
    uint32_t status = 0x00000000U;

    if((EXMC_BANK1_NAND == EXMC_bank) || (EXMC_BANK2_NAND == EXMC_bank) || (EXMC_BANK3_PCCARD == EXMC_bank)) {
        /* NAND bank1,bank2 or PC card bank3 */
        status = EXMC_NPINTEN(EXMC_bank);
    } else {
        /* SDRAM device0 or device1 */
        status = EXMC_SDSTAT;
    }

    if((status & flag) != (uint32_t)flag) {
        /* flag is reset */
        return RESET;
    } else {
        /* flag is set */
        return SET;
    }
}

/*!
    简介:      clear EXMC flag status
    参数[输入]:  EXMC_bank: specify the NAND bank , PCCARD bank or SDRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK1_NAND: the NAND bank1
      参数:        EXMC_BANK2_NAND: the NAND bank2
      参数:        EXMC_BANK3_PCCARD: the PC card bank
      参数:        EXMC_SDRAM_DEVICE0: the SDRAM device0
      参数:        EXMC_SDRAM_DEVICE1: the SDRAM device1
    参数[输入]:  flag: EXMC status and flag
                only one parameter can be selected which is shown as below:
      参数:        EXMC_NAND_PCCARD_Flag_RISE: interrupt rising edge status
      参数:        EXMC_NAND_PCCARD_Flag_LEVEL: interrupt high-level status
      参数:        EXMC_NAND_PCCARD_Flag_FALL: interrupt falling edge status
      参数:        EXMC_NAND_PCCARD_Flag_FIFOE: FIFO empty flag
      参数:        EXMC_SDRAM_Flag_REFRESH: refresh error interrupt flag
      参数:        EXMC_SDRAM_Flag_NREADY: not ready status
    参数[输出]:  无
    返回值:      无
*/
void EXMC_Flag_Clear(uint32_t EXMC_bank, uint32_t flag) {
    if((EXMC_BANK1_NAND == EXMC_bank) || (EXMC_BANK2_NAND == EXMC_bank) || (EXMC_BANK3_PCCARD == EXMC_bank)) {
        /* NAND bank1,bank2 or PC card bank3 */
        EXMC_NPINTEN(EXMC_bank) &= ~flag;
    } else {
        /* SDRAM device0 or device1 */
        EXMC_SDSTAT &= ~flag;
    }
}

/*!
    简介:      get EXMC interrupt flag
    参数[输入]:  EXMC_bank: specify the NAND bank , PC card bank or SDRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK1_NAND: the NAND bank1
      参数:        EXMC_BANK2_NAND: the NAND bank2
      参数:        EXMC_BANK3_PCCARD: the PC card bank
      参数:        EXMC_SDRAM_DEVICE0: the SDRAM device0
      参数:        EXMC_SDRAM_DEVICE1: the SDRAM device1
    参数[输入]:  interrupt: EXMC interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        EXMC_NAND_PCCARD_INT_Flag_RISE: rising edge interrupt and flag
      参数:        EXMC_NAND_PCCARD_INT_Flag_LEVEL: high-level interrupt and flag
      参数:        EXMC_NAND_PCCARD_INT_Flag_FALL: falling edge interrupt and flag
      参数:        EXMC_SDRAM_INT_Flag_REFRESH: refresh error interrupt and flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus EXMC_Interrupt_Flag_Get(uint32_t EXMC_bank, uint32_t interrupt) {
    uint32_t status = 0x00000000U, interrupt_enable = 0x00000000U, interrupt_state = 0x00000000U;

    if((EXMC_BANK1_NAND == EXMC_bank) || (EXMC_BANK2_NAND == EXMC_bank) || (EXMC_BANK3_PCCARD == EXMC_bank)) {
        /* NAND bank1,bank2 or PC card bank3 */
        status = EXMC_NPINTEN(EXMC_bank);
        interrupt_state = (status & (interrupt >> INTEN_INTS_OFFSET));
    } else {
        /* SDRAM device0 or device1 */
        status = EXMC_SDARI;
        interrupt_state = (EXMC_SDSTAT & EXMC_SDSDAT_REIF);
    }

    interrupt_enable = (status & interrupt);

    if((interrupt_enable) && (interrupt_state)) {
        /* interrupt flag is set */
        return SET;
    } else {
        /* interrupt flag is reset */
        return RESET;
    }
}

/*!
    简介:      clear EXMC interrupt flag
    参数[输入]:  EXMC_bank: specify the NAND bank , PC card bank or SDRAM device
                only one parameter can be selected which is shown as below:
      参数:        EXMC_BANK1_NAND: the NAND bank1
      参数:        EXMC_BANK2_NAND: the NAND bank2
      参数:        EXMC_BANK3_PCCARD: the PC card bank
      参数:        EXMC_SDRAM_DEVICE0: the SDRAM device0
      参数:        EXMC_SDRAM_DEVICE1: the SDRAM device1
    参数[输入]:  interrupt: EXMC interrupt flag
                only one parameter can be selected which is shown as below:
      参数:        EXMC_NAND_PCCARD_INT_Flag_RISE: rising edge interrupt and flag
      参数:        EXMC_NAND_PCCARD_INT_Flag_LEVEL: high-level interrupt and flag
      参数:        EXMC_NAND_PCCARD_INT_Flag_FALL: falling edge interrupt and flag
      参数:        EXMC_SDRAM_INT_Flag_REFRESH: refresh error interrupt and flag
    参数[输出]:  无
    返回值:      无
*/
void EXMC_Interrupt_Flag_Clear(uint32_t EXMC_bank, uint32_t interrupt) {
    if((EXMC_BANK1_NAND == EXMC_bank) || (EXMC_BANK2_NAND == EXMC_bank) || (EXMC_BANK3_PCCARD == EXMC_bank)) {
        /* NAND bank1,bank2 or PC card bank3 */
        EXMC_NPINTEN(EXMC_bank) &= ~(interrupt >> INTEN_INTS_OFFSET);
    } else {
        /* SDRAM device0 or device1 */
        EXMC_SDARI |= EXMC_SDARI_REC;
    }
}
