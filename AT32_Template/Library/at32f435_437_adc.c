/**
  **************************************************************************
  * @file     at32f435_437_adc.c
  * @brief    contains all the functions for the adc firmware library
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

/** @defgroup ADC
  * @brief ADC driver modules
  * @{
  */

#ifdef ADC_MODULE_EnableD

/** @defgroup ADC_private_functions
  * @{
  */

/**
  * @brief  deinitialize the adc peripheral registers to their default reset values.
  * @param  none
  * @retval none
  */
void ADC_Reset(void) {
    crm_periph_Reset(CRM_ADC_PERIPH_Reset, TRUE);
    crm_periph_Reset(CRM_ADC_PERIPH_Reset, FALSE);
}

/**
  * @brief  enable or disable the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of a/d converter.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl2_bit.adcen = new_state;
}

/**
  * @brief  adc base default para init.
  * @param  sequence_mode: set the state of adc sequence mode.
  *         this parameter can be:TRUE or FALSE
  * @param  repeat_mode: set the state of adc repeat conversion mode.
  *         this parameter can be:TRUE or FALSE
  * @param  data_align: set the state of adc data alignment.
  *         this parameter can be one of the following values:
  *         - ADC_RIGHT_ALIGNMENT
  *         - ADC_LEFT_ALIGNMENT
  * @param  ordinary_Channel_length: configure the adc ordinary channel sequence length.
  *         this parameter can be:
  *         - (0x1~0xf)
  * @retval none
  */
void ADC_base_Default_Para_Init(ADC_base_Config_Type *ADC_base_struct) {
    ADC_base_struct->sequence_mode = FALSE;
    ADC_base_struct->repeat_mode = FALSE;
    ADC_base_struct->data_align = ADC_RIGHT_ALIGNMENT;
    ADC_base_struct->ordinary_Channel_length = 1;
}

/**
  * @brief  initialize the adc peripheral according to the specified parameters.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  sequence_mode: set the state of adc sequence mode.
  *         this parameter can be:TRUE or FALSE
  * @param  repeat_mode: set the state of adc repeat conversion mode.
  *         this parameter can be:TRUE or FALSE
  * @param  data_align: set the state of adc data alignment.
  *         this parameter can be one of the following values:
  *         - ADC_RIGHT_ALIGNMENT
  *         - ADC_LEFT_ALIGNMENT
  * @param  ordinary_Channel_length: configure the adc ordinary channel sequence length.
  *         this parameter can be:
  *         - (0x1~0xf)
  * @retval none
  */
void ADC_base_Config(ADC_Type *ADC_x, ADC_base_Config_Type *ADC_base_struct) {
    ADC_x->ctrl1_bit.sqen = ADC_base_struct->sequence_mode;
    ADC_x->ctrl2_bit.rpen = ADC_base_struct->repeat_mode;
    ADC_x->ctrl2_bit.dtalign = ADC_base_struct->data_align;
    ADC_x->osq1_bit.oclen = ADC_base_struct->ordinary_Channel_length - 1;
}

/**
  * @brief  adc common default para init.
  * @param  combine_mode: configure the adc combine_mode mode.
  *         this parameter can be one of the following values:
  *         - ADC_INDEPENDENT_MODE                       - ADC_Ordinary_SMLT_Preempt_SMLT_ONESLAVE_MODE   - ADC_Ordinary_SMLT_Preempt_INTERLTRIG_ONESLAVE_MODE
  *         - ADC_Preempt_SMLT_ONLY_ONESLAVE_MODE        - ADC_Ordinary_SMLT_ONLY_ONESLAVE_MODE           - ADC_Ordinary_SHIFT_ONLY_ONESLAVE_MODE
  *         - ADC_Preempt_INTERLTRIG_ONLY_ONESLAVE_MODE  - ADC_Ordinary_SMLT_Preempt_SMLT_TWOSLAVE_MODE   - ADC_Ordinary_SMLT_Preempt_INTERLTRIG_TWOSLAVE_MODE
  *         - ADC_Preempt_SMLT_ONLY_TWOSLAVE_MODE        - ADC_Ordinary_SMLT_ONLY_TWOSLAVE_MODE           - ADC_Ordinary_SHIFT_ONLY_TWOSLAVE_MODE
  *         - ADC_Preempt_INTERLTRIG_ONLY_TWOSLAVE_MODE
  * @param  div: configure the adc division.
  *         this parameter can be one of the following values:
  *         - ADC_HCLK_DIV_2             - ADC_HCLK_DIV_3             - ADC_HCLK_DIV_4             - ADC_HCLK_DIV_5
  *         - ADC_HCLK_DIV_6             - ADC_HCLK_DIV_7             - ADC_HCLK_DIV_8             - ADC_HCLK_DIV_9
  *         - ADC_HCLK_DIV_10            - ADC_HCLK_DIV_11            - ADC_HCLK_DIV_12            - ADC_HCLK_DIV_13
  *         - ADC_HCLK_DIV_14            - ADC_HCLK_DIV_15            - ADC_HCLK_DIV_16            - ADC_HCLK_DIV_17
  * @param  common_DMA_mode: configure the adc common dma mode.
  *         this parameter can be one of the following values:
  *         - ADC_Common_DMAMODE_Disable
  *         - ADC_Common_DMAMODE_1 <mode 1 can be used: all ordinary simultaneous mode,all ordinary shifting mode>
  *         - ADC_Common_DMAMODE_2 <mode 2 can be used: one slaver ordinary simultaneous mode,all ordinary shifting mode>
  *         - ADC_Common_DMAMODE_3 <mode 3 can be used: one slaver ordinary simultaneous mode in 6/8 bit resolution,ordinary shifting mode in 6/8 bit resolution>
  *         - ADC_Common_DMAMODE_4 <mode 4 can be used: two slaver ordinary simultaneous mode in 6/8 bit resolution,two slave ordinary shifting mode in 6/8 bit resolution>
  *         - ADC_Common_DMAMODE_5 <mode 5 can be used: all two slaver ordinary simultaneous mode,all two slave ordinary shifting mode>
  * @param  common_DMA_Request_repeat_state: set the adc common dma request repeat state.
  *         this parameter can be:TRUE or FALSE
  * @param  sampling_interval: configure the ordinary shifting mode adjacent adc sampling interval.
  *         this parameter can be one of the following values:
  *         - ADC_SAMPLING_INTERVAL_5CYCLES     - ADC_SAMPLING_INTERVAL_6CYCLES     - ADC_SAMPLING_INTERVAL_7CYCLES     - ADC_SAMPLING_INTERVAL_8CYCLES
  *         - ADC_SAMPLING_INTERVAL_9CYCLES     - ADC_SAMPLING_INTERVAL_10CYCLES    - ADC_SAMPLING_INTERVAL_11CYCLES    - ADC_SAMPLING_INTERVAL_12CYCLES
  *         - ADC_SAMPLING_INTERVAL_13CYCLES    - ADC_SAMPLING_INTERVAL_14CYCLES    - ADC_SAMPLING_INTERVAL_15CYCLES    - ADC_SAMPLING_INTERVAL_16CYCLES
  *         - ADC_SAMPLING_INTERVAL_17CYCLES    - ADC_SAMPLING_INTERVAL_18CYCLES    - ADC_SAMPLING_INTERVAL_19CYCLES    - ADC_SAMPLING_INTERVAL_20CYCLES
  * @param  tempervintrv_state: set the adc temperature sensor and vintrv state.
  *         this parameter can be:TRUE or FALSE
  * @param  vbat_state: set the adc voltage battery state.
  *         this parameter can be:TRUE or FALSE
  * @retval none
  */
void ADC_Common_Default_Para_Init(ADC_Common_Config_Type *ADC_Common_struct) {
    ADC_Common_struct->combine_mode = ADC_INDEPENDENT_MODE;
    ADC_Common_struct->div = ADC_HCLK_DIV_2;
    ADC_Common_struct->common_DMA_mode = ADC_Common_DMAMODE_Disable;
    ADC_Common_struct->common_DMA_Request_repeat_state = FALSE;
    ADC_Common_struct->sampling_interval = ADC_SAMPLING_INTERVAL_5CYCLES;
    ADC_Common_struct->tempervintrv_state = FALSE;
    ADC_Common_struct->vbat_state = FALSE;
}

/**
  * @brief  adc common default para init.
  * @param  combine_mode: configure the adc combine_mode mode.
  *         this parameter can be one of the following values:
  *         - ADC_INDEPENDENT_MODE                       - ADC_Ordinary_SMLT_Preempt_SMLT_ONESLAVE_MODE   - ADC_Ordinary_SMLT_Preempt_INTERLTRIG_ONESLAVE_MODE
  *         - ADC_Preempt_SMLT_ONLY_ONESLAVE_MODE        - ADC_Ordinary_SMLT_ONLY_ONESLAVE_MODE           - ADC_Ordinary_SHIFT_ONLY_ONESLAVE_MODE
  *         - ADC_Preempt_INTERLTRIG_ONLY_ONESLAVE_MODE  - ADC_Ordinary_SMLT_Preempt_SMLT_TWOSLAVE_MODE   - ADC_Ordinary_SMLT_Preempt_INTERLTRIG_TWOSLAVE_MODE
  *         - ADC_Preempt_SMLT_ONLY_TWOSLAVE_MODE        - ADC_Ordinary_SMLT_ONLY_TWOSLAVE_MODE           - ADC_Ordinary_SHIFT_ONLY_TWOSLAVE_MODE
  *         - ADC_Preempt_INTERLTRIG_ONLY_TWOSLAVE_MODE
  * @param  div: configure the adc division.
  *         this parameter can be one of the following values:
  *         - ADC_HCLK_DIV_2             - ADC_HCLK_DIV_3             - ADC_HCLK_DIV_4             - ADC_HCLK_DIV_5
  *         - ADC_HCLK_DIV_6             - ADC_HCLK_DIV_7             - ADC_HCLK_DIV_8             - ADC_HCLK_DIV_9
  *         - ADC_HCLK_DIV_10            - ADC_HCLK_DIV_11            - ADC_HCLK_DIV_12            - ADC_HCLK_DIV_13
  *         - ADC_HCLK_DIV_14            - ADC_HCLK_DIV_15            - ADC_HCLK_DIV_16            - ADC_HCLK_DIV_17
  * @param  common_DMA_mode: configure the adc common dma mode.
  *         this parameter can be one of the following values:
  *         - ADC_Common_DMAMODE_Disable
  *         - ADC_Common_DMAMODE_1 <mode 1 can be used: all ordinary simultaneous mode,all ordinary shifting mode>
  *         - ADC_Common_DMAMODE_2 <mode 2 can be used: one slaver ordinary simultaneous mode,all ordinary shifting mode>
  *         - ADC_Common_DMAMODE_3 <mode 3 can be used: one slaver ordinary simultaneous mode in 6/8 bit resolution,ordinary shifting mode in 6/8 bit resolution>
  *         - ADC_Common_DMAMODE_4 <mode 4 can be used: two slaver ordinary simultaneous mode in 6/8 bit resolution,two slave ordinary shifting mode in 6/8 bit resolution>
  *         - ADC_Common_DMAMODE_5 <mode 5 can be used: all two slaver ordinary simultaneous mode,all two slave ordinary shifting mode>
  * @param  common_DMA_Request_repeat_state: set the adc common dma request repeat state.
  *         this parameter can be:TRUE or FALSE
  * @param  sampling_interval: configure the ordinary shifting mode adjacent adc sampling interval.
  *         this parameter can be one of the following values:
  *         - ADC_SAMPLING_INTERVAL_5CYCLES     - ADC_SAMPLING_INTERVAL_6CYCLES     - ADC_SAMPLING_INTERVAL_7CYCLES     - ADC_SAMPLING_INTERVAL_8CYCLES
  *         - ADC_SAMPLING_INTERVAL_9CYCLES     - ADC_SAMPLING_INTERVAL_10CYCLES    - ADC_SAMPLING_INTERVAL_11CYCLES    - ADC_SAMPLING_INTERVAL_12CYCLES
  *         - ADC_SAMPLING_INTERVAL_13CYCLES    - ADC_SAMPLING_INTERVAL_14CYCLES    - ADC_SAMPLING_INTERVAL_15CYCLES    - ADC_SAMPLING_INTERVAL_16CYCLES
  *         - ADC_SAMPLING_INTERVAL_17CYCLES    - ADC_SAMPLING_INTERVAL_18CYCLES    - ADC_SAMPLING_INTERVAL_19CYCLES    - ADC_SAMPLING_INTERVAL_20CYCLES
  * @param  tempervintrv_state: set the adc temperature sensor and vintrv state.
  *         this parameter can be:TRUE or FALSE
  * @param  vbat_state: set the adc voltage battery state.
  *         this parameter can be:TRUE or FALSE
  * @retval none
  */
void ADC_Common_Config(ADC_Common_Config_Type *ADC_Common_struct) {
    ADCCOM->cctrl_bit.mssel = ADC_Common_struct->combine_mode;
    ADCCOM->cctrl_bit.adcdiv = ADC_Common_struct->div;

    if(ADC_Common_struct->common_DMA_mode & 0x04) {
        ADCCOM->cctrl_bit.msdmasel_h = TRUE;
    } else {
        ADCCOM->cctrl_bit.msdmasel_h = FALSE;
    }

    ADCCOM->cctrl_bit.msdmasel_l = ADC_Common_struct->common_DMA_mode & 0x03;
    ADCCOM->cctrl_bit.msdrcen = ADC_Common_struct->common_DMA_Request_repeat_state;
    ADCCOM->cctrl_bit.asisel = ADC_Common_struct->sampling_interval;
    ADCCOM->cctrl_bit.itsrven = ADC_Common_struct->tempervintrv_state;
    ADCCOM->cctrl_bit.vbaten = ADC_Common_struct->vbat_state;
}

/**
  * @brief  set resolution of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  resolution: set the conversion resolution.
  *         this parameter can be one of the following values:
  *         - ADC_RESOLUTION_12B
  *         - ADC_RESOLUTION_10B
  *         - ADC_RESOLUTION_8B
  *         - ADC_RESOLUTION_6B
  * @retval none
  */
void ADC_resolution_Set(ADC_Type *ADC_x, ADC_resolution_Type resolution) {
    ADC_x->ctrl1_bit.crsel = resolution;
}

/**
  * @brief  enable or disable the adc voltage battery.
  * @param  new_state: new state of the adc voltage battery.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_voltage_battery_Enable(confirm_state new_state) {
    ADCCOM->cctrl_bit.vbaten = new_state;
}

/**
  * @brief  enable or disable the adc dma transfer.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of the adc dma transfer.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_DMA_Mode_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl2_bit.ocdmaen = new_state;
}

/**
  * @brief  enable or disable the adc dma request repeat.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: the adc dma request repeat state.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_DMA_Request_repeat_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl2_bit.ocdrcen = new_state;
}

/**
  * @brief  enable or disable the specified adc interrupts.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_int: specifies the adc interrupt sources to be enabled or disabled.
  *         this parameter can be one of the following values:
  *         - ADC_OCCE_INT
  *         - ADC_VMOR_INT
  *         - ADC_PCCE_INT
  *         - ADC_OCCO_INT
  * @param  new_state: new state of the specified adc interrupts.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Interrupt_Enable(ADC_Type *ADC_x, uint32_t ADC_int, confirm_state new_state) {
    if(new_state == TRUE) {
        ADC_x->ctrl1 |= ADC_int;
    } else if(new_state == FALSE) {
        ADC_x->ctrl1 &= ~ADC_int;
    }
}

/**
  * @brief  set calibration value of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_Calibration_value: calibration value of adc.
  *         this parameter can be:
  *         - (0x00~0x7F)
  * @retval none
  */
void ADC_Calibration_value_Set(ADC_Type *ADC_x, uint8_t ADC_Calibration_value) {
    ADC_x->calval = ADC_Calibration_value;
}

/**
  * @brief  initialize calibration register of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval none
  */
void ADC_Calibration_Init(ADC_Type *ADC_x) {
    ADC_x->ctrl2_bit.adcalinit = TRUE;
}

/**
  * @brief  get calibration register's initialize status of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval the new state of reset calibration register status(SET or RESET).
  */
flag_status ADC_Calibration_Init_Status_Get(ADC_Type *ADC_x) {
    if(ADC_x->ctrl2_bit.adcalinit) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  start calibration process of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval none
  */
void ADC_Calibration_start(ADC_Type *ADC_x) {
    ADC_x->ctrl2_bit.adcal = TRUE;
}

/**
  * @brief  get calibration status of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval the new state of calibration status(SET or RESET).
  */
flag_status ADC_Calibration_Status_Get(ADC_Type *ADC_x) {
    if(ADC_x->ctrl2_bit.adcal) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  enable or disable the voltage monitoring on single/all ordinary or preempt channels of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_voltage_monitoring: choose the ADC_voltage_monitoring config.
  *         this parameter can be one of the following values:
  *         - ADC_VMONITOR_SINGLE_ORDINARY
  *         - ADC_VMONITOR_SINGLE_PREEMPT
  *         - ADC_VMONITOR_SINGLE_Ordinary_PREEMPT
  *         - ADC_VMONITOR_All_ORDINARY
  *         - ADC_VMONITOR_All_PREEMPT
  *         - ADC_VMONITOR_All_Ordinary_PREEMPT
  *         - ADC_VMONITOR_NONE
  * @retval none
  */
void ADC_voltage_monitor_Enable(ADC_Type *ADC_x, ADC_voltage_monitoring_Type ADC_voltage_monitoring) {
    ADC_x->ctrl1_bit.ocvmen = FALSE;
    ADC_x->ctrl1_bit.pcvmen = FALSE;
    ADC_x->ctrl1_bit.vmsgen = FALSE;
    ADC_x->ctrl1 |= ADC_voltage_monitoring;
}

/**
  * @brief  set voltage monitoring's high and low thresholds value of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_high_threshold: voltage monitoring's high thresholds value.
  *         this parameter can be:
  *         - (0x000~0xFFF)
  * @param  ADC_low_threshold: voltage monitoring's low thresholds value.
  *         this parameter can be:
  *         - (0x000~0xFFF)
  * @retval none
  */
void ADC_voltage_monitor_threshold_value_Set(ADC_Type *ADC_x, uint16_t ADC_high_threshold, uint16_t ADC_low_threshold) {
    ADC_x->vmhb_bit.vmhb = ADC_high_threshold;
    ADC_x->vmlb_bit.vmlb = ADC_low_threshold;
}

/**
  * @brief  select the voltage monitoring's channel of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_channel: select the channel.
  *         this parameter can be one of the following values:
  *         - ADC_Channel_0    - ADC_Channel_1    - ADC_Channel_2    - ADC_Channel_3
  *         - ADC_Channel_4    - ADC_Channel_5    - ADC_Channel_6    - ADC_Channel_7
  *         - ADC_Channel_8    - ADC_Channel_9    - ADC_Channel_10   - ADC_Channel_11
  *         - ADC_Channel_12   - ADC_Channel_13   - ADC_Channel_14   - ADC_Channel_15
  *         - ADC_Channel_16   - ADC_Channel_17   - ADC_Channel_18
  * @retval none
  */
void ADC_voltage_monitor_single_Channel_Select(ADC_Type *ADC_x, ADC_Channel_Select_Type ADC_channel) {
    ADC_x->ctrl1_bit.vmcsel = ADC_channel;
}

/**
  * @brief  set ordinary channel's corresponding rank in the sequencer and sample time of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_channel: select the channel.
  *         this parameter can be one of the following values:
  *         - ADC_Channel_0    - ADC_Channel_1    - ADC_Channel_2    - ADC_Channel_3
  *         - ADC_Channel_4    - ADC_Channel_5    - ADC_Channel_6    - ADC_Channel_7
  *         - ADC_Channel_8    - ADC_Channel_9    - ADC_Channel_10   - ADC_Channel_11
  *         - ADC_Channel_12   - ADC_Channel_13   - ADC_Channel_14   - ADC_Channel_15
  *         - ADC_Channel_16   - ADC_Channel_17   - ADC_Channel_18
  * @param  ADC_sequence: set rank in the ordinary group sequencer.
  *         this parameter must be:
  *         - between 1 to 16
  * @param  ADC_sampletime: set the sampletime of adc channel.
  *         this parameter can be one of the following values:
  *         - ADC_SAMPLETIME_2_5
  *         - ADC_SAMPLETIME_6_5
  *         - ADC_SAMPLETIME_12_5
  *         - ADC_SAMPLETIME_24_5
  *         - ADC_SAMPLETIME_47_5
  *         - ADC_SAMPLETIME_92_5
  *         - ADC_SAMPLETIME_247_5
  *         - ADC_SAMPLETIME_640_5
  * @retval none
  */
void ADC_Ordinary_Channel_Set(ADC_Type *ADC_x, ADC_Channel_Select_Type ADC_channel, uint8_t ADC_sequence, ADC_sampletime_Select_Type ADC_sampletime) {
    switch(ADC_channel) {
        case ADC_Channel_0:
            ADC_x->spt2_bit.cspt0 = ADC_sampletime;
            break;

        case ADC_Channel_1:
            ADC_x->spt2_bit.cspt1 = ADC_sampletime;
            break;

        case ADC_Channel_2:
            ADC_x->spt2_bit.cspt2 = ADC_sampletime;
            break;

        case ADC_Channel_3:
            ADC_x->spt2_bit.cspt3 = ADC_sampletime;
            break;

        case ADC_Channel_4:
            ADC_x->spt2_bit.cspt4 = ADC_sampletime;
            break;

        case ADC_Channel_5:
            ADC_x->spt2_bit.cspt5 = ADC_sampletime;
            break;

        case ADC_Channel_6:
            ADC_x->spt2_bit.cspt6 = ADC_sampletime;
            break;

        case ADC_Channel_7:
            ADC_x->spt2_bit.cspt7 = ADC_sampletime;
            break;

        case ADC_Channel_8:
            ADC_x->spt2_bit.cspt8 = ADC_sampletime;
            break;

        case ADC_Channel_9:
            ADC_x->spt2_bit.cspt9 = ADC_sampletime;
            break;

        case ADC_Channel_10:
            ADC_x->spt1_bit.cspt10 = ADC_sampletime;
            break;

        case ADC_Channel_11:
            ADC_x->spt1_bit.cspt11 = ADC_sampletime;
            break;

        case ADC_Channel_12:
            ADC_x->spt1_bit.cspt12 = ADC_sampletime;
            break;

        case ADC_Channel_13:
            ADC_x->spt1_bit.cspt13 = ADC_sampletime;
            break;

        case ADC_Channel_14:
            ADC_x->spt1_bit.cspt14 = ADC_sampletime;
            break;

        case ADC_Channel_15:
            ADC_x->spt1_bit.cspt15 = ADC_sampletime;
            break;

        case ADC_Channel_16:
            ADC_x->spt1_bit.cspt16 = ADC_sampletime;
            break;

        case ADC_Channel_17:
            ADC_x->spt1_bit.cspt17 = ADC_sampletime;
            break;

        case ADC_Channel_18:
            ADC_x->spt1_bit.cspt18 = ADC_sampletime;
            break;

        default:
            break;
    }

    switch(ADC_sequence) {
        case 1:
            ADC_x->osq3_bit.osn1 = ADC_channel;
            break;

        case 2:
            ADC_x->osq3_bit.osn2 = ADC_channel;
            break;

        case 3:
            ADC_x->osq3_bit.osn3 = ADC_channel;
            break;

        case 4:
            ADC_x->osq3_bit.osn4 = ADC_channel;
            break;

        case 5:
            ADC_x->osq3_bit.osn5 = ADC_channel;
            break;

        case 6:
            ADC_x->osq3_bit.osn6 = ADC_channel;
            break;

        case 7:
            ADC_x->osq2_bit.osn7 = ADC_channel;
            break;

        case 8:
            ADC_x->osq2_bit.osn8 = ADC_channel;
            break;

        case 9:
            ADC_x->osq2_bit.osn9 = ADC_channel;
            break;

        case 10:
            ADC_x->osq2_bit.osn10 = ADC_channel;
            break;

        case 11:
            ADC_x->osq2_bit.osn11 = ADC_channel;
            break;

        case 12:
            ADC_x->osq2_bit.osn12 = ADC_channel;
            break;

        case 13:
            ADC_x->osq1_bit.osn13 = ADC_channel;
            break;

        case 14:
            ADC_x->osq1_bit.osn14 = ADC_channel;
            break;

        case 15:
            ADC_x->osq1_bit.osn15 = ADC_channel;
            break;

        case 16:
            ADC_x->osq1_bit.osn16 = ADC_channel;
            break;

        default:
            break;
    }
}

/**
  * @brief  set preempt channel lenghth of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_Channel_lenght: set the adc preempt channel lenghth.
  *         this parameter can be:
  *         - (0x1~0x4)
  * @retval none
  */
void ADC_Preempt_Channel_length_Set(ADC_Type *ADC_x, uint8_t ADC_Channel_lenght) {
    ADC_x->psq_bit.pclen =  ADC_Channel_lenght - 1;
}

/**
  * @brief  configure preempt channel's corresponding rank in the sequencer and sample time of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_channel: select the channel.
  *         this parameter can be one of the following values:
  *         - ADC_Channel_0    - ADC_Channel_1    - ADC_Channel_2    - ADC_Channel_3
  *         - ADC_Channel_4    - ADC_Channel_5    - ADC_Channel_6    - ADC_Channel_7
  *         - ADC_Channel_8    - ADC_Channel_9    - ADC_Channel_10   - ADC_Channel_11
  *         - ADC_Channel_12   - ADC_Channel_13   - ADC_Channel_14   - ADC_Channel_15
  *         - ADC_Channel_16   - ADC_Channel_17   - ADC_Channel_18
  * @param  ADC_sequence: set rank in the preempt group sequencer.
  *         this parameter must be:
  *         - between 1 to 4
  * @param  ADC_sampletime: config the sampletime of adc channel.
  *         this parameter can be one of the following values:
  *         - ADC_SAMPLETIME_2_5
  *         - ADC_SAMPLETIME_6_5
  *         - ADC_SAMPLETIME_12_5
  *         - ADC_SAMPLETIME_24_5
  *         - ADC_SAMPLETIME_47_5
  *         - ADC_SAMPLETIME_92_5
  *         - ADC_SAMPLETIME_247_5
  *         - ADC_SAMPLETIME_640_5
  * @retval none
  */
void ADC_Preempt_Channel_Set(ADC_Type *ADC_x, ADC_Channel_Select_Type ADC_channel, uint8_t ADC_sequence, ADC_sampletime_Select_Type ADC_sampletime) {
    uint16_t sequence_index = 0;

    switch(ADC_channel) {
        case ADC_Channel_0:
            ADC_x->spt2_bit.cspt0 = ADC_sampletime;
            break;

        case ADC_Channel_1:
            ADC_x->spt2_bit.cspt1 = ADC_sampletime;
            break;

        case ADC_Channel_2:
            ADC_x->spt2_bit.cspt2 = ADC_sampletime;
            break;

        case ADC_Channel_3:
            ADC_x->spt2_bit.cspt3 = ADC_sampletime;
            break;

        case ADC_Channel_4:
            ADC_x->spt2_bit.cspt4 = ADC_sampletime;
            break;

        case ADC_Channel_5:
            ADC_x->spt2_bit.cspt5 = ADC_sampletime;
            break;

        case ADC_Channel_6:
            ADC_x->spt2_bit.cspt6 = ADC_sampletime;
            break;

        case ADC_Channel_7:
            ADC_x->spt2_bit.cspt7 = ADC_sampletime;
            break;

        case ADC_Channel_8:
            ADC_x->spt2_bit.cspt8 = ADC_sampletime;
            break;

        case ADC_Channel_9:
            ADC_x->spt2_bit.cspt9 = ADC_sampletime;
            break;

        case ADC_Channel_10:
            ADC_x->spt1_bit.cspt10 = ADC_sampletime;
            break;

        case ADC_Channel_11:
            ADC_x->spt1_bit.cspt11 = ADC_sampletime;
            break;

        case ADC_Channel_12:
            ADC_x->spt1_bit.cspt12 = ADC_sampletime;
            break;

        case ADC_Channel_13:
            ADC_x->spt1_bit.cspt13 = ADC_sampletime;
            break;

        case ADC_Channel_14:
            ADC_x->spt1_bit.cspt14 = ADC_sampletime;
            break;

        case ADC_Channel_15:
            ADC_x->spt1_bit.cspt15 = ADC_sampletime;
            break;

        case ADC_Channel_16:
            ADC_x->spt1_bit.cspt16 = ADC_sampletime;
            break;

        case ADC_Channel_17:
            ADC_x->spt1_bit.cspt17 = ADC_sampletime;
            break;

        case ADC_Channel_18:
            ADC_x->spt1_bit.cspt18 = ADC_sampletime;
            break;

        default:
            break;
    }

    sequence_index = ADC_sequence + 3 - ADC_x->psq_bit.pclen;

    switch(sequence_index) {
        case 1:
            ADC_x->psq_bit.psn1 = ADC_channel;
            break;

        case 2:
            ADC_x->psq_bit.psn2 = ADC_channel;
            break;

        case 3:
            ADC_x->psq_bit.psn3 = ADC_channel;
            break;

        case 4:
            ADC_x->psq_bit.psn4 = ADC_channel;
            break;

        default:
            break;
    }
}

/**
  * @brief  set the ordinary channel's external trigger edge and
  *         set external trigger event of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_Ordinary_trig: select the external trigger event.
  *         this parameter can be one of the following values:
  *         - ADC_Ordinary_TRIG_TMR1CH1     - ADC_Ordinary_TRIG_TMR1CH2      - ADC_Ordinary_TRIG_TMR1CH3      - ADC_Ordinary_TRIG_TMR2CH2
  *         - ADC_Ordinary_TRIG_TMR2CH3     - ADC_Ordinary_TRIG_TMR2CH4      - ADC_Ordinary_TRIG_TMR2TRGOUT   - ADC_Ordinary_TRIG_TMR3CH1
  *         - ADC_Ordinary_TRIG_TMR3TRGOUT  - ADC_Ordinary_TRIG_TMR4CH4      - ADC_Ordinary_TRIG_TMR5CH1      - ADC_Ordinary_TRIG_TMR5CH2
  *         - ADC_Ordinary_TRIG_TMR5CH3     - ADC_Ordinary_TRIG_TMR8CH1      - ADC_Ordinary_TRIG_TMR8TRGOUT   - ADC_Ordinary_TRIG_EXINT11
  *         - ADC_Ordinary_TRIG_TMR20TRGOUT - ADC_Ordinary_TRIG_TMR20TRGOUT2 - ADC_Ordinary_TRIG_TMR20CH1     - ADC_Ordinary_TRIG_TMR20CH2
  *         - ADC_Ordinary_TRIG_TMR20CH3    - ADC_Ordinary_TRIG_TMR8TRGOUT2  - ADC_Ordinary_TRIG_TMR1TRGOUT2  - ADC_Ordinary_TRIG_TMR4TRGOUT
  *         - ADC_Ordinary_TRIG_TMR6TRGOUT  - ADC_Ordinary_TRIG_TMR3CH4      - ADC_Ordinary_TRIG_TMR4CH1      - ADC_Ordinary_TRIG_TMR1TRGOUT
  *         - ADC_Ordinary_TRIG_TMR2CH1     - ADC_Ordinary_TRIG_TMR7TRGOUT
  * @param  ADC_Ordinary_trig_edge: ordinary channel conversion's external_Trigger_edge.
  *         this parameter can be one of the following values:
  *         - ADC_Ordinary_TRIG_Edge_NONE
  *         - ADC_Ordinary_TRIG_Edge_RISING
  *         - ADC_Ordinary_TRIG_Edge_FALLING
  *         - ADC_Ordinary_TRIG_Edge_RISING_FALLING
  * @retval none
  */
void ADC_Ordinary_Conversion_Trigger_Set(ADC_Type *ADC_x, ADC_Ordinary_trig_Select_Type ADC_Ordinary_trig, ADC_Ordinary_trig_Edge_Type ADC_Ordinary_trig_edge) {
    if(ADC_Ordinary_trig > ADC_Ordinary_TRIG_EXINT11) {
        ADC_x->ctrl2_bit.octesel_h = 1;
        ADC_x->ctrl2_bit.octesel_l = ADC_Ordinary_trig & 0x0F;
    } else {
        ADC_x->ctrl2_bit.octesel_h = 0;
        ADC_x->ctrl2_bit.octesel_l = ADC_Ordinary_trig & 0x0F;
    }

    ADC_x->ctrl2_bit.ocete = ADC_Ordinary_trig_edge;
}

/**
  * @brief  enable or disable the preempt channel's external trigger and
  *         set external trigger event of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_Preempt_trig: select the external trigger event.
  *         this parameter can be one of the following values:
  *         - ADC_Preempt_TRIG_TMR1CH4      - ADC_Preempt_TRIG_TMR1TRGOUT   - ADC_Preempt_TRIG_TMR2CH1   - ADC_Preempt_TRIG_TMR2TRGOUT
  *         - ADC_Preempt_TRIG_TMR3CH2      - ADC_Preempt_TRIG_TMR3CH4      - ADC_Preempt_TRIG_TMR4CH1   - ADC_Preempt_TRIG_TMR4CH2
  *         - ADC_Preempt_TRIG_TMR4CH3      - ADC_Preempt_TRIG_TMR4TRGOUT   - ADC_Preempt_TRIG_TMR5CH4   - ADC_Preempt_TRIG_TMR5TRGOUT
  *         - ADC_Preempt_TRIG_TMR8CH2      - ADC_Preempt_TRIG_TMR8CH3      - ADC_Preempt_TRIG_TMR8CH4   - ADC_Preempt_TRIG_EXINT15
  *         - ADC_Preempt_TRIG_TMR20TRGOUT  - ADC_Preempt_TRIG_TMR20TRGOUT2 - ADC_Preempt_TRIG_TMR20CH4  - ADC_Preempt_TRIG_TMR1TRGOUT2
  *         - ADC_Preempt_TRIG_TMR8TRGOUT   - ADC_Preempt_TRIG_TMR8TRGOUT2  - ADC_Preempt_TRIG_TMR3CH3   - ADC_Preempt_TRIG_TMR3TRGOUT
  *         - ADC_Preempt_TRIG_TMR3CH1      - ADC_Preempt_TRIG_TMR6TRGOUT   - ADC_Preempt_TRIG_TMR4CH4   - ADC_Preempt_TRIG_TMR1CH3
  *         - ADC_Preempt_TRIG_TMR20CH2     - ADC_Preempt_TRIG_TMR7TRGOUT
  * @param  ADC_Preempt_trig_edge: preempt channel conversion's external_Trigger_edge.
  *         this parameter can be one of the following values:
  *         - ADC_Preempt_TRIG_Edge_NONE
  *         - ADC_Preempt_TRIG_Edge_RISING
  *         - ADC_Preempt_TRIG_Edge_FALLING
  *         - ADC_Preempt_TRIG_Edge_RISING_FALLING
  * @retval none
  */
void ADC_Preempt_Conversion_Trigger_Set(ADC_Type *ADC_x, ADC_Preempt_trig_Select_Type ADC_Preempt_trig, ADC_Preempt_trig_Edge_Type ADC_Preempt_trig_edge) {
    if(ADC_Preempt_trig > ADC_Preempt_TRIG_EXINT15) {
        ADC_x->ctrl2_bit.pctesel_h = 1;
        ADC_x->ctrl2_bit.pctesel_l = ADC_Preempt_trig & 0x0F;
    } else {
        ADC_x->ctrl2_bit.pctesel_h = 0;
        ADC_x->ctrl2_bit.pctesel_l = ADC_Preempt_trig & 0x0F;
    }

    ADC_x->ctrl2_bit.pcete = ADC_Preempt_trig_edge;
}

/**
  * @brief  set preempt channel's conversion value offset of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_Preempt_channel: select the preempt channel.
  *         this parameter can be one of the following values:
  *         - ADC_Preempt_Channel_1
  *         - ADC_Preempt_Channel_2
  *         - ADC_Preempt_Channel_3
  *         - ADC_Preempt_Channel_4
  * @param  ADC_offset_value: set the adc preempt channel's conversion value offset.
  *         this parameter can be:
  *         - (0x000~0xFFF)
  * @retval none
  */
void ADC_Preempt_offset_value_Set(ADC_Type *ADC_x, ADC_Preempt_Channel_Type ADC_Preempt_channel, uint16_t ADC_offset_value) {
    switch(ADC_Preempt_channel) {
        case ADC_Preempt_Channel_1:
            ADC_x->pcdto1_bit.pcdto1 = ADC_offset_value;
            break;

        case ADC_Preempt_Channel_2:
            ADC_x->pcdto2_bit.pcdto2 = ADC_offset_value;
            break;

        case ADC_Preempt_Channel_3:
            ADC_x->pcdto3_bit.pcdto3 = ADC_offset_value;
            break;

        case ADC_Preempt_Channel_4:
            ADC_x->pcdto4_bit.pcdto4 = ADC_offset_value;
            break;

        default:
            break;
    }
}

/**
  * @brief  set partitioned mode channel count of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_Channel_count: configure the adc partitioned mode channel count.
  *         this parameter can be:
  *         - (0x1~0x8)
  * @retval none
  */
void ADC_Ordinary_part_count_Set(ADC_Type *ADC_x, uint8_t ADC_Channel_count) {

    ADC_x->ctrl1_bit.ocpcnt =  ADC_Channel_count - 1;
}

/**
  * @brief  enable or disable the partitioned mode on ordinary channel of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of ordinary channel's partitioned mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Ordinary_part_Mode_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl1_bit.ocpen = new_state;
}

/**
  * @brief  enable or disable the partitioned mode on preempt channel of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of preempt channel's partitioned mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Preempt_part_Mode_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl1_bit.pcpen = new_state;
}

/**
  * @brief  enable or disable automatic preempt group conversion of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of automatic preempt group conversion.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Preempt_Auto_Mode_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl1_bit.pcautoen = new_state;
}

/**
  * @brief  stop the ongoing conversion of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval none
  */
void ADC_Conversion_stop(ADC_Type *ADC_x) {
    ADC_x->ctrl2_bit.adabrt = TRUE;
}

/**
  * @brief  get stop conversion's status of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval the new state of stop conversion's status(SET or RESET).
  */
flag_status ADC_Conversion_Stop_Status_Get(ADC_Type *ADC_x) {
    if(ADC_x->ctrl2_bit.adabrt) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  enable or disable each ordinary channel conversion set occe flag of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of each ordinary channel conversion set occe flag.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_occe_each_Conversion_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl2_bit.eocsfen = new_state;
}

/**
  * @brief  enable or disable ordinary software start conversion of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of ordinary software start conversion.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Ordinary_Software_Trigger_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl2_bit.ocswtrg = new_state;
}

/**
  * @brief  get ordinary software start conversion status of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval the new state of ordinary software start conversion status(SET or RESET).
  */
flag_status ADC_Ordinary_Software_Trigger_Status_Get(ADC_Type *ADC_x) {
    if(ADC_x->ctrl2_bit.ocswtrg) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  enable or disable preempt software start conversion of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of preempt software start conversion.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Preempt_Software_Trigger_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ctrl2_bit.pcswtrg = new_state;
}

/**
  * @brief  get preempt software start conversion status of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval the new state of preempt software start conversion status(SET or RESET).
  */
flag_status ADC_Preempt_Software_Trigger_Status_Get(ADC_Type *ADC_x) {
    if(ADC_x->ctrl2_bit.pcswtrg) {
        return SET;
    } else {
        return RESET;
    }
}

/**
  * @brief  return the last conversion data for ordinary channel of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @retval the last conversion data for ordinary channel.
  */
uint16_t ADC_Ordinary_Conversion_Data_Get(ADC_Type *ADC_x) {
    return (uint16_t)(ADC_x->odt_bit.odt);
}

/**
  * @brief  return the last ordinary conversion data of combine adc.
  * @retval the last conversion data for ordinary channel.
  */
uint32_t ADC_Combine_Ordinary_Conversion_Data_Get(void) {
    return (uint32_t)(ADCCOM->codt);
}

/**
  * @brief  return the conversion data for selection preempt channel of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_Preempt_channel: select the preempt channel.
  *         this parameter can be one of the following values:
  *         - ADC_Preempt_Channel_1
  *         - ADC_Preempt_Channel_2
  *         - ADC_Preempt_Channel_3
  *         - ADC_Preempt_Channel_4
  * @retval the conversion data for selection preempt channel.
  */
uint16_t ADC_Preempt_Conversion_Data_Get(ADC_Type *ADC_x, ADC_Preempt_Channel_Type ADC_Preempt_channel) {
    uint16_t preempt_conv_Data_index = 0;

    switch(ADC_Preempt_channel) {
        case ADC_Preempt_Channel_1:
            preempt_conv_Data_index = (uint16_t)(ADC_x->pdt1_bit.pdt1);
            break;

        case ADC_Preempt_Channel_2:
            preempt_conv_Data_index = (uint16_t)(ADC_x->pdt2_bit.pdt2);
            break;

        case ADC_Preempt_Channel_3:
            preempt_conv_Data_index = (uint16_t)(ADC_x->pdt3_bit.pdt3);
            break;

        case ADC_Preempt_Channel_4:
            preempt_conv_Data_index = (uint16_t)(ADC_x->pdt4_bit.pdt4);
            break;

        default:
            break;
    }

    return preempt_conv_Data_index;
}

/**
  * @brief  get flag of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_flag: select the adc flag.
  *         this parameter can be one of the following values:
  *         - ADC_VMOR_FLAG
  *         - ADC_OCCE_FLAG
  *         - ADC_PCCE_FLAG
  *         - ADC_PCCS_FLAG(no interrupt associated)
  *         - ADC_OCCS_FLAG(no interrupt associated)
  *         - ADC_OCCO_FLAG
  *         - ADC_RDY_FLAG(no interrupt associated)
  * @retval the new state of adc flag status(SET or RESET).
  */
flag_status ADC_Flag_Get(ADC_Type *ADC_x, uint8_t ADC_flag) {
    flag_status status = RESET;

    if((ADC_x->sts & ADC_flag) == RESET) {
        status = RESET;
    } else {
        status = SET;
    }

    return status;
}

/**
  * @brief  clear flag of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         ADC1, ADC2, ADC3.
  * @param  ADC_flag: select the adc flag.
  *         this parameter can be any combination of the following values:
  *         - ADC_VMOR_FLAG
  *         - ADC_OCCE_FLAG(also can clear by reading the ADC_x->odt)
  *         - ADC_PCCE_FLAG
  *         - ADC_PCCS_FLAG
  *         - ADC_OCCS_FLAG
  *         - ADC_OCCO_FLAG
  *         - note:"ADC_RDY_FLAG" cannot be choose!rdy bit is readonly bit,it means the adc is ready to accept conversion request
  * @retval none
  */
void ADC_Flag_Clear(ADC_Type *ADC_x, uint32_t ADC_flag) {
    ADC_x->sts = ~ADC_flag;
}

/**
  * @brief  enable or disable the ordinary oversampling of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of ordinary oversampling.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Ordinary_OverSample_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ovsp_bit.oosen = new_state;
}

/**
  * @brief  enable or disable the preempt oversampling of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of preempt oversampling.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Preempt_OverSample_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ovsp_bit.posen = new_state;
}

/**
  * @brief  config the oversampling ratio and shift of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_OverSample_ratio: set the oversample ratio.
  *         this parameter can be one of the following values:
  *         - ADC_OverSample_Ratio_2
  *         - ADC_OverSample_Ratio_4
  *         - ADC_OverSample_Ratio_8
  *         - ADC_OverSample_Ratio_16
  *         - ADC_OverSample_Ratio_32
  *         - ADC_OverSample_Ratio_64
  *         - ADC_OverSample_Ratio_128
  *         - ADC_OverSample_Ratio_256
  * @param  ADC_OverSample_shift: set the oversample shift.
  *         this parameter can be one of the following values:
  *         - ADC_OverSample_SHIFT_0
  *         - ADC_OverSample_SHIFT_1
  *         - ADC_OverSample_SHIFT_2
  *         - ADC_OverSample_SHIFT_3
  *         - ADC_OverSample_SHIFT_4
  *         - ADC_OverSample_SHIFT_5
  *         - ADC_OverSample_SHIFT_6
  *         - ADC_OverSample_SHIFT_7
  *         - ADC_OverSample_SHIFT_8
  * @retval none
  */
void ADC_OverSample_Ratio_shift_Set(ADC_Type *ADC_x, ADC_OverSample_Ratio_Type ADC_OverSample_ratio, ADC_OverSample_shift_Type ADC_OverSample_shift) {
    ADC_x->ovsp_bit.osrsel = ADC_OverSample_ratio;
    ADC_x->ovsp_bit.osssel = ADC_OverSample_shift;
}

/**
  * @brief  enable or disable the ordinary oversampling trigger mode of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  new_state: new state of ordinary oversampling trigger mode.
  *         this parameter can be: TRUE or FALSE.
  * @retval none
  */
void ADC_Ordinary_OverSample_trig_Enable(ADC_Type *ADC_x, confirm_state new_state) {
    ADC_x->ovsp_bit.oostren = new_state;
}

/**
  * @brief  set ordinary oversample restart mode of the specified adc peripheral.
  * @param  ADC_x: select the adc peripheral.
  *         this parameter can be one of the following values:
  *         - ADC1, ADC2, ADC3.
  * @param  ADC_or_OverSample_restart: ordinary oversample restart mode.
  *         this parameter can be one of the following values:
  *         - ADC_OverSample_CONTINUE
  *         - ADC_OverSample_RESTART
  * @retval none
  */
void ADC_Ordinary_OverSample_restart_Set(ADC_Type *ADC_x, ADC_Ordinary_OverSample_restart_Type ADC_Ordinary_OverSample_restart) {
    ADC_x->ovsp_bit.oosrsel = ADC_Ordinary_OverSample_restart;
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
