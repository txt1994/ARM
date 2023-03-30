/*!
    文件:    gd32f4xx_ipa.c
    简介:   IPA driver

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

#include "gd32f4xx_ipa.h"

#define IPA_DEFAULT_VALUE   0x00000000U

/*!
    简介:    deinitialize IPA registers
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void IPA_DeInit(void) {
    RCU_Periph_Reset_Enable(RCU_IPARST);
    RCU_Periph_Reset_Disable(RCU_IPARST);
}

/*!
    简介:    enable IPA transfer
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void IPA_Transfer_Enable(void) {
    IPA_CTL |= IPA_CTL_TEN;
}

/*!
    简介:    enable IPA transfer hang up
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void IPA_Transfer_hangup_Enable(void) {
    IPA_CTL |= IPA_CTL_THU;
}

/*!
    简介:    disable IPA transfer hang up
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void IPA_Transfer_hangup_Disable(void) {
    IPA_CTL &= ~(IPA_CTL_THU);
}

/*!
    简介:    enable IPA transfer stop
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void IPA_Transfer_Stop_Enable(void) {
    IPA_CTL |= IPA_CTL_TST;
}

/*!
    简介:    disable IPA transfer stop
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void IPA_Transfer_Stop_Disable(void) {
    IPA_CTL &= ~(IPA_CTL_TST);
}
/*!
    简介:    enable IPA foreground LUT loading
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void IPA_foreground_Lut_loading_Enable(void) {
    IPA_FPCTL |= IPA_FPCTL_FLLEN;
}

/*!
    简介:    enable IPA background LUT loading
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void IPA_background_Lut_loading_Enable(void) {
    IPA_BPCTL |= IPA_BPCTL_BLLEN;
}

/*!
    简介:    set pixel format convert mode, the function is invalid when the IPA transfer is enabled
    参数[输入]:  pfcm: pixel format convert mode
                only one parameter can be selected which is shown as below:
      参数:        IPA_FGTODE: foreground memory to destination memory without pixel format convert
      参数:        IPA_FGTODE_PF_CONVERT: foreground memory to destination memory with pixel format convert
      参数:        IPA_FGBGTODE: blending foreground and background memory to destination memory
      参数:        IPA_FILL_Up_DE: fill up destination memory with specific color
    参数[输出]:  无
    返回值:      无
*/
void IPA_pixel_format_convert_Mode_Set(uint32_t pfcm) {
    IPA_CTL &= ~(IPA_CTL_PFCM);
    IPA_CTL |= pfcm;
}

/*!
    简介:    initialize the structure of IPA foreground parameter struct with the default values, it is
                suggested that call this function after an IPA_foreground_Parameter_Struct structure is defined
    参数[输入]:  无
    参数[输出]: foreground_Struct: the data needed to initialize foreground
                  foreground_memaddr: foreground memory base address
                  foreground_lineoff: foreground line offset
                  foreground_prealpha: foreground pre-defined alpha value
                  foreground_alpha_algorithm: IPA_FG_ALPHA_Mode_0,IPA_FG_ALPHA_Mode_1,IPA_FG_ALPHA_Mode_2
                  foreground_pf: foreground pixel format(FOREGROUND_PPF_ARGB8888,FOREGROUND_PPF_RGB888,FOREGROUND_PPF_RGB565,
                            FOREGROUND_PPF_ARG1555,FOREGROUND_PPF_ARGB4444,FOREGROUND_PPF_L8,FOREGROUND_PPF_AL44,
                            FOREGROUND_PPF_AL88,FOREGROUND_PPF_L4,FOREGROUND_PPF_A8,FOREGROUND_PPF_A4)
                  foreground_prered: foreground pre-defined red value
                  foreground_pregreen: foreground pre-defined green value
                  foreground_preblue: foreground pre-defined blue value
    返回值:      无
*/
void IPA_foreground_Struct_Para_Init(IPA_foreground_Parameter_Struct *foreground_Struct) {
    /* initialize the struct parameters with default values */
    foreground_Struct->foreground_memaddr = IPA_DEFAULT_VALUE;
    foreground_Struct->foreground_lineoff = IPA_DEFAULT_VALUE;
    foreground_Struct->foreground_prealpha = IPA_DEFAULT_VALUE;
    foreground_Struct->foreground_alpha_algorithm = IPA_FG_ALPHA_Mode_0;
    foreground_Struct->foreground_pf = FOREGROUND_PPF_ARGB8888;
    foreground_Struct->foreground_prered = IPA_DEFAULT_VALUE;
    foreground_Struct->foreground_pregreen = IPA_DEFAULT_VALUE;
    foreground_Struct->foreground_preblue = IPA_DEFAULT_VALUE;
}

/*!
    简介:    initialize foreground parameters
    参数[输入]:  foreground_Struct: the data needed to initialize foreground
                  foreground_memaddr: foreground memory base address
                  foreground_lineoff: foreground line offset
                  foreground_prealpha: foreground pre-defined alpha value
                  foreground_alpha_algorithm: IPA_FG_ALPHA_Mode_0,IPA_FG_ALPHA_Mode_1,IPA_FG_ALPHA_Mode_2
                  foreground_pf: foreground pixel format(FOREGROUND_PPF_ARGB8888,FOREGROUND_PPF_RGB888,FOREGROUND_PPF_RGB565,
                            FOREGROUND_PPF_ARG1555,FOREGROUND_PPF_ARGB4444,FOREGROUND_PPF_L8,FOREGROUND_PPF_AL44,
                            FOREGROUND_PPF_AL88,FOREGROUND_PPF_L4,FOREGROUND_PPF_A8,FOREGROUND_PPF_A4)
                  foreground_prered: foreground pre-defined red value
                  foreground_pregreen: foreground pre-defined green value
                  foreground_preblue: foreground pre-defined blue value
    参数[输出]:  无
    返回值:      无
*/
void IPA_foreground_Init(IPA_foreground_Parameter_Struct *foreground_Struct) {
    FlagStatus tempflag = RESET;

    if(RESET != (IPA_CTL & IPA_CTL_TEN)) {
        tempflag = SET;
        /* reset the TEN in order to configure the following bits */
        IPA_CTL &= ~IPA_CTL_TEN;
    }

    /* foreground memory base address configuration */
    IPA_FMADDR &= ~(IPA_FMADDR_FMADDR);
    IPA_FMADDR = foreground_Struct->foreground_memaddr;
    /* foreground line offset configuration */
    IPA_FLOFF &= ~(IPA_FLOFF_FLOFF);
    IPA_FLOFF = foreground_Struct->foreground_lineoff;
    /* foreground pixel format pre-defined alpha, alpha calculation algorithm configuration */
    IPA_FPCTL &= ~(IPA_FPCTL_FPDAV | IPA_FPCTL_FAVCA | IPA_FPCTL_FPF);
    IPA_FPCTL |= (foreground_Struct->foreground_prealpha << 24U);
    IPA_FPCTL |= foreground_Struct->foreground_alpha_algorithm;
    IPA_FPCTL |= foreground_Struct->foreground_pf;
    /* foreground pre-defined red green blue configuration */
    IPA_FPV &= ~(IPA_FPV_FPDRV | IPA_FPV_FPDGV | IPA_FPV_FPDBV);
    IPA_FPV |= ((foreground_Struct->foreground_prered << 16U) | (foreground_Struct->foreground_pregreen << 8U)
                | (foreground_Struct->foreground_preblue));

    if(SET == tempflag) {
        /* restore the state of TEN */
        IPA_CTL |= IPA_CTL_TEN;
    }
}

/*!
    简介:    initialize the structure of IPA background parameter struct with the default values, it is
                suggested that call this function after an IPA_background_Parameter_Struct structure is defined
    参数[输入]:  无
    参数[输出]: background_Struct: the data needed to initialize background
                  background_memaddr: background memory base address
                  background_lineoff: background line offset
                  background_prealpha: background pre-defined alpha value
                  background_alpha_algorithm: IPA_BG_ALPHA_Mode_0,IPA_BG_ALPHA_Mode_1,IPA_BG_ALPHA_Mode_2
                  background_pf: background pixel format(BACKGROUND_PPF_ARGB8888,BACKGROUND_PPF_RGB888,BACKGROUND_PPF_RGB565,
                            BACKGROUND_PPF_ARG1555,BACKGROUND_PPF_ARGB4444,BACKGROUND_PPF_L8,BACKGROUND_PPF_AL44,
                            BACKGROUND_PPF_AL88,BACKGROUND_PPF_L4,BACKGROUND_PPF_A8,BACKGROUND_PPF_A4)
                  background_prered: background pre-defined red value
                  background_pregreen: background pre-defined green value
                  background_preblue: background pre-defined blue value
    返回值:      无
*/
void IPA_background_Struct_Para_Init(IPA_background_Parameter_Struct *background_Struct) {
    /* initialize the struct parameters with default values */
    background_Struct->background_memaddr = IPA_DEFAULT_VALUE;
    background_Struct->background_lineoff = IPA_DEFAULT_VALUE;
    background_Struct->background_prealpha = IPA_DEFAULT_VALUE;
    background_Struct->background_alpha_algorithm = IPA_BG_ALPHA_Mode_0;
    background_Struct->background_pf = BACKGROUND_PPF_ARGB8888;
    background_Struct->background_prered = IPA_DEFAULT_VALUE;
    background_Struct->background_pregreen = IPA_DEFAULT_VALUE;
    background_Struct->background_preblue = IPA_DEFAULT_VALUE;
}

/*!
    简介:    initialize background parameters
    参数[输入]:  background_Struct: the data needed to initialize background
                  background_memaddr: background memory base address
                  background_lineoff: background line offset
                  background_prealpha: background pre-defined alpha value
                  background_alpha_algorithm: IPA_BG_ALPHA_Mode_0,IPA_FG_ALPHA_Mode_1,IPA_FG_ALPHA_Mode_2
                  background_pf: background pixel format(BACKGROUND_PPF_ARGB8888,BACKGROUND_PPF_RGB888,BACKGROUND_PPF_RGB565,
                            BACKGROUND_PPF_ARG1555,BACKGROUND_PPF_ARGB4444,BACKGROUND_PPF_L8,BACKGROUND_PPF_AL44,
                            BACKGROUND_PPF_AL88,BACKGROUND_PPF_L4,BACKGROUND_PPF_A8,BACKGROUND_PPF_A4)
                  background_prered: background pre-defined red value
                  background_pregreen: background pre-defined green value
                  background_preblue: background pre-defined blue value
    参数[输出]:  无
    返回值:      无
*/
void IPA_background_Init(IPA_background_Parameter_Struct *background_Struct) {
    FlagStatus tempflag = RESET;

    if(RESET != (IPA_CTL & IPA_CTL_TEN)) {
        tempflag = SET;
        /* reset the TEN in order to configure the following bits */
        IPA_CTL &= ~IPA_CTL_TEN;
    }

    /* background memory base address configuration */
    IPA_BMADDR &= ~(IPA_BMADDR_BMADDR);
    IPA_BMADDR = background_Struct->background_memaddr;
    /* background line offset configuration */
    IPA_BLOFF &= ~(IPA_BLOFF_BLOFF);
    IPA_BLOFF = background_Struct->background_lineoff;
    /* background pixel format pre-defined alpha, alpha calculation algorithm configuration */
    IPA_BPCTL &= ~(IPA_BPCTL_BPDAV | IPA_BPCTL_BAVCA | IPA_BPCTL_BPF);
    IPA_BPCTL |= (background_Struct->background_prealpha << 24U);
    IPA_BPCTL |= background_Struct->background_alpha_algorithm;
    IPA_BPCTL |= background_Struct->background_pf;
    /* background pre-defined red green blue configuration */
    IPA_BPV &= ~(IPA_BPV_BPDRV | IPA_BPV_BPDGV | IPA_BPV_BPDBV);
    IPA_BPV |= ((background_Struct->background_prered << 16U) | (background_Struct->background_pregreen << 8U)
                | (background_Struct->background_preblue));

    if(SET == tempflag) {
        /* restore the state of TEN */
        IPA_CTL |= IPA_CTL_TEN;
    }
}

/*!
    简介:    initialize the structure of IPA destination parameter struct with the default values, it is
                suggested that call this function after an IPA_destination_Parameter_Struct structure is defined
    参数[输入]:  无
    参数[输出]: destination_Struct: the data needed to initialize destination parameter
                  destination_pf: IPA_DPF_ARGB8888,IPA_DPF_RGB888,IPA_DPF_RGB565,IPA_DPF_ARGB1555,
                              IPA_DPF_ARGB4444,refer to IPA_dpf_enum
                  destination_lineoff: destination line offset
                  destination_prealpha: destination pre-defined alpha value
                  destination_prered: destination pre-defined red value
                  destination_pregreen: destination pre-defined green value
                  destination_preblue: destination pre-defined blue value
                  destination_memaddr: destination memory base address
                  image_width: width of the image to be processed
                  image_height: height of the image to be processed
    返回值:      无
*/
void IPA_destination_Struct_Para_Init(IPA_destination_Parameter_Struct *destination_Struct) {
    /* initialize the struct parameters with default values */
    destination_Struct->destination_pf = IPA_DPF_ARGB8888;
    destination_Struct->destination_lineoff = IPA_DEFAULT_VALUE;
    destination_Struct->destination_prealpha = IPA_DEFAULT_VALUE;
    destination_Struct->destination_prered = IPA_DEFAULT_VALUE;
    destination_Struct->destination_pregreen = IPA_DEFAULT_VALUE;
    destination_Struct->destination_preblue = IPA_DEFAULT_VALUE;
    destination_Struct->destination_memaddr = IPA_DEFAULT_VALUE;
    destination_Struct->image_width = IPA_DEFAULT_VALUE;
    destination_Struct->image_height = IPA_DEFAULT_VALUE;
}

/*!
    简介:    initialize destination parameters
    参数[输入]:  destination_Struct: the data needed to initialize destination parameters
                  destination_pf: IPA_DPF_ARGB8888,IPA_DPF_RGB888,IPA_DPF_RGB565,IPA_DPF_ARGB1555,
                                IPA_DPF_ARGB4444,refer to IPA_dpf_enum
                  destination_lineoff: destination line offset
                  destination_prealpha: destination pre-defined alpha value
                  destination_prered: destination pre-defined red value
                  destination_pregreen: destination pre-defined green value
                  destination_preblue: destination pre-defined blue value
                  destination_memaddr: destination memory base address
                  image_width: width of the image to be processed
                  image_height: height of the image to be processed
    参数[输出]:  无
    返回值:      无
*/
void IPA_destination_Init(IPA_destination_Parameter_Struct *destination_Struct) {
    uint32_t destination_pixelformat;
    FlagStatus tempflag = RESET;

    if(RESET != (IPA_CTL & IPA_CTL_TEN)) {
        tempflag = SET;
        /* reset the TEN in order to configure the following bits */
        IPA_CTL &= ~IPA_CTL_TEN;
    }

    /* destination pixel format configuration */
    IPA_DPCTL &= ~(IPA_DPCTL_DPF);
    IPA_DPCTL = destination_Struct->destination_pf;
    destination_pixelformat = destination_Struct->destination_pf;

    /* destination pixel format ARGB8888 */
    switch(destination_pixelformat) {
    case IPA_DPF_ARGB8888:
        IPA_DPV &= ~(IPA_DPV_DPDBV_0 | (IPA_DPV_DPDGV_0) | (IPA_DPV_DPDRV_0) | (IPA_DPV_DPDAV_0));
        IPA_DPV = (destination_Struct->destination_preblue | (destination_Struct->destination_pregreen << 8U)
                   | (destination_Struct->destination_prered << 16U)
                   | (destination_Struct->destination_prealpha << 24U));
        break;

    /* destination pixel format RGB888 */
    case IPA_DPF_RGB888:
        IPA_DPV &= ~(IPA_DPV_DPDBV_1 | (IPA_DPV_DPDGV_1) | (IPA_DPV_DPDRV_1));
        IPA_DPV = (destination_Struct->destination_preblue | (destination_Struct->destination_pregreen << 8U)
                   | (destination_Struct->destination_prered << 16U));
        break;

    /* destination pixel format RGB565 */
    case IPA_DPF_RGB565:
        IPA_DPV &= ~(IPA_DPV_DPDBV_2 | (IPA_DPV_DPDGV_2) | (IPA_DPV_DPDRV_2));
        IPA_DPV = (destination_Struct->destination_preblue | (destination_Struct->destination_pregreen << 5U)
                   | (destination_Struct->destination_prered << 11U));
        break;

    /* destination pixel format ARGB1555 */
    case IPA_DPF_ARGB1555:
        IPA_DPV &= ~(IPA_DPV_DPDBV_3 | (IPA_DPV_DPDGV_3) | (IPA_DPV_DPDRV_3) | (IPA_DPV_DPDAV_3));
        IPA_DPV = (destination_Struct->destination_preblue | (destination_Struct->destination_pregreen << 5U)
                   | (destination_Struct->destination_prered << 10U)
                   | (destination_Struct->destination_prealpha << 15U));
        break;

    /* destination pixel format ARGB4444 */
    case IPA_DPF_ARGB4444:
        IPA_DPV &= ~(IPA_DPV_DPDBV_4 | (IPA_DPV_DPDGV_4) | (IPA_DPV_DPDRV_4) | (IPA_DPV_DPDAV_4));
        IPA_DPV = (destination_Struct->destination_preblue | (destination_Struct->destination_pregreen << 4U)
                   | (destination_Struct->destination_prered << 8U)
                   | (destination_Struct->destination_prealpha << 12U));
        break;

    default:
        break;
    }

    /* destination memory base address configuration */
    IPA_DMADDR &= ~(IPA_DMADDR_DMADDR);
    IPA_DMADDR = destination_Struct->destination_memaddr;
    /* destination line offset configuration */
    IPA_DLOFF &= ~(IPA_DLOFF_DLOFF);
    IPA_DLOFF = destination_Struct->destination_lineoff;
    /* image size configuration */
    IPA_IMS &= ~(IPA_IMS_HEIGHT | IPA_IMS_WIDTH);
    IPA_IMS |= ((destination_Struct->image_width << 16U) | (destination_Struct->image_height));

    if(SET == tempflag) {
        /* restore the state of TEN */
        IPA_CTL |= IPA_CTL_TEN;
    }
}

/*!
    简介:    initialize IPA foreground LUT parameters
    参数[输入]:  fg_Lut_num: foreground LUT number of pixel
    参数[输入]:  fg_Lut_pf: foreground LUT pixel format(IPA_Lut_PF_ARGB8888, IPA_Lut_PF_RGB888)
    参数[输入]:  fg_Lut_addr: foreground LUT memory base address
    参数[输出]:  无
    返回值:      无
*/
void IPA_foreground_Lut_Init(uint8_t fg_Lut_num, uint8_t fg_Lut_pf, uint32_t fg_Lut_addr) {
    FlagStatus tempflag = RESET;

    if(RESET != (IPA_FPCTL & IPA_FPCTL_FLLEN)) {
        tempflag = SET;
        /* reset the FLLEN in order to configure the following bits */
        IPA_FPCTL &= ~IPA_FPCTL_FLLEN;
    }

    /* foreground LUT number of pixel configuration */
    IPA_FPCTL |= ((uint32_t)fg_Lut_num << 8U);

    /* foreground LUT pixel format configuration */
    if(IPA_Lut_PF_RGB888 == fg_Lut_pf) {
        IPA_FPCTL |= IPA_FPCTL_FLPF;
    } else {
        IPA_FPCTL &= ~(IPA_FPCTL_FLPF);
    }

    /* foreground LUT memory base address configuration */
    IPA_FLMADDR &= ~(IPA_FLMADDR_FLMADDR);
    IPA_FLMADDR = fg_Lut_addr;

    if(SET == tempflag) {
        /* restore the state of FLLEN */
        IPA_FPCTL |= IPA_FPCTL_FLLEN;
    }
}

/*!
    简介:    initialize IPA background LUT parameters
    参数[输入]:  bg_Lut_num: background LUT number of pixel
    参数[输入]:  bg_Lut_pf: background LUT pixel format(IPA_Lut_PF_ARGB8888, IPA_Lut_PF_RGB888)
    参数[输入]:  bg_Lut_addr: background LUT memory base address
    参数[输出]:  无
    返回值:      无
*/
void IPA_background_Lut_Init(uint8_t bg_Lut_num, uint8_t bg_Lut_pf, uint32_t bg_Lut_addr) {
    FlagStatus tempflag = RESET;

    if(RESET != (IPA_BPCTL & IPA_BPCTL_BLLEN)) {
        tempflag = SET;
        /* reset the BLLEN in order to configure the following bits */
        IPA_BPCTL &= ~IPA_BPCTL_BLLEN;
    }

    /* background LUT number of pixel configuration */
    IPA_BPCTL |= ((uint32_t)bg_Lut_num << 8U);

    /* background LUT pixel format configuration */
    if(IPA_Lut_PF_RGB888 == bg_Lut_pf) {
        IPA_BPCTL |= IPA_BPCTL_BLPF;
    } else {
        IPA_BPCTL &= ~(IPA_BPCTL_BLPF);
    }

    /* background LUT memory base address configuration */
    IPA_BLMADDR &= ~(IPA_BLMADDR_BLMADDR);
    IPA_BLMADDR = bg_Lut_addr;

    if(SET == tempflag) {
        /* restore the state of BLLEN */
        IPA_BPCTL |= IPA_BPCTL_BLLEN;
    }
}

/*!
    简介:    configure IPA line mark
    参数[输入]:  line_num: line number
    参数[输出]:  无
    返回值:      无
*/
void IPA_Line_Mark_Config(uint16_t line_num) {
    IPA_LM &= ~(IPA_LM_LM);
    IPA_LM = line_num;
}

/*!
    简介:    inter-timer enable or disable
    参数[输入]:  TIMER_cfg: IPA_INTER_TIMER_ENABLE,IPA_INTER_TIMER_DISABLE
    参数[输出]:  无
    返回值:      无
*/
void IPA_inter_TIMER_Config(uint8_t TIMER_cfg) {
    if(IPA_INTER_TIMER_ENABLE == TIMER_cfg) {
        IPA_ITCTL |= IPA_ITCTL_ITEN;
    } else {
        IPA_ITCTL &= ~(IPA_ITCTL_ITEN);
    }
}

/*!
    简介:    configure the number of clock cycles interval
    参数[输入]:  clk_num: the number of clock cycles
    参数[输出]:  无
    返回值:      无
*/
void IPA_interval_Clock_num_Config(uint8_t clk_num) {
    /* NCCI[7:0] bits have no meaning if ITEN is '0' */
    IPA_ITCTL &= ~(IPA_ITCTL_NCCI);
    IPA_ITCTL |= ((uint32_t)clk_num << 8U);
}

/*!
    简介:    get IPA flag status in IPA_INTF register
    参数[输入]:  flag: IPA flags
                one or more parameters can be selected which are shown as below:
      参数:        IPA_Flag_TAE: transfer access error interrupt flag
      参数:        IPA_Flag_FTF: full transfer finish interrupt flag
      参数:        IPA_Flag_TLM: transfer line mark interrupt flag
      参数:        IPA_Flag_LAC: LUT access conflict interrupt flag
      参数:        IPA_Flag_LLF: LUT loading finish interrupt flag
      参数:        IPA_Flag_WCF: wrong configuration interrupt flag
    参数[输出]:  无
    返回值:      无
*/
FlagStatus IPA_Flag_Get(uint32_t flag) {
    if(RESET != (IPA_INTF & flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear IPA flag in IPA_INTF register
    参数[输入]:  flag: IPA flags
                one or more parameters can be selected which are shown as below:
      参数:        IPA_Flag_TAE: transfer access error interrupt flag
      参数:        IPA_Flag_FTF: full transfer finish interrupt flag
      参数:        IPA_Flag_TLM: transfer line mark interrupt flag
      参数:        IPA_Flag_LAC: LUT access conflict interrupt flag
      参数:        IPA_Flag_LLF: LUT loading finish interrupt flag
      参数:        IPA_Flag_WCF: wrong configuration interrupt flag
    参数[输出]:  无
    返回值:      无
*/
void IPA_Flag_Clear(uint32_t flag) {
    IPA_INTC |= (flag);
}

/*!
    简介:    enable IPA interrupt
    参数[输入]:  int_flag: IPA interrupt flags
                one or more parameters can be selected which are shown as below:
      参数:        IPA_INT_TAE: transfer access error interrupt
      参数:        IPA_INT_FTF: full transfer finish interrupt
      参数:        IPA_INT_TLM: transfer line mark interrupt
      参数:        IPA_INT_LAC: LUT access conflict interrupt
      参数:        IPA_INT_LLF: LUT loading finish interrupt
      参数:        IPA_INT_WCF: wrong configuration interrupt
    参数[输出]:  无
    返回值:      无
*/
void IPA_Interrupt_Enable(uint32_t int_flag) {
    IPA_CTL |= (int_flag);
}

/*!
    简介:    disable IPA interrupt
    参数[输入]:  int_flag: IPA interrupt flags
                one or more parameters can be selected which are shown as below:
      参数:        IPA_INT_TAE: transfer access error interrupt
      参数:        IPA_INT_FTF: full transfer finish interrupt
      参数:        IPA_INT_TLM: transfer line mark interrupt
      参数:        IPA_INT_LAC: LUT access conflict interrupt
      参数:        IPA_INT_LLF: LUT loading finish interrupt
      参数:        IPA_INT_WCF: wrong configuration interrupt
    参数[输出]:  无
    返回值:      无
*/
void IPA_Interrupt_Disable(uint32_t int_flag) {
    IPA_CTL &= ~(int_flag);
}

/*!
    简介:    get IPA interrupt flag
    参数[输入]:  int_flag: IPA interrupt flag flags
                one or more parameters can be selected which are shown as below:
      参数:        IPA_INT_Flag_TAE: transfer access error interrupt flag
      参数:        IPA_INT_Flag_FTF: full transfer finish interrupt flag
      参数:        IPA_INT_Flag_TLM: transfer line mark interrupt flag
      参数:        IPA_INT_Flag_LAC: LUT access conflict interrupt flag
      参数:        IPA_INT_Flag_LLF: LUT loading finish interrupt flag
      参数:        IPA_INT_Flag_WCF: wrong configuration interrupt flag
    参数[输出]:  无
    返回值:      无
*/
FlagStatus IPA_Interrupt_Flag_Get(uint32_t int_flag) {
    if(0U != (IPA_INTF & int_flag)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear IPA interrupt flag
    参数[输入]:  int_flag: IPA interrupt flag flags
                one or more parameters can be selected which are shown as below:
      参数:        IPA_INT_Flag_TAE: transfer access error interrupt flag
      参数:        IPA_INT_Flag_FTF: full transfer finish interrupt flag
      参数:        IPA_INT_Flag_TLM: transfer line mark interrupt flag
      参数:        IPA_INT_Flag_LAC: LUT access conflict interrupt flag
      参数:        IPA_INT_Flag_LLF: LUT loading finish interrupt flag
      参数:        IPA_INT_Flag_WCF: wrong configuration interrupt flag
    参数[输出]:  无
    返回值:      无
*/
void IPA_Interrupt_Flag_Clear(uint32_t int_flag) {
    IPA_INTC |= (int_flag);
}
