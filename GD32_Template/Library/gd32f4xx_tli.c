/*!
    文件:    gd32f4xx_tli.c
    简介:   TLI driver

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

#include "gd32f4xx_tli.h"

#define TLI_DEFAULT_VALUE   0x00000000U
#define TLI_OPAQUE_VALUE    0x000000FFU

/*!
    简介:    deinitialize TLI registers
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void TLI_DeInit(void) {
    RCU_Periph_Reset_Enable(RCU_TLIRST);
    RCU_Periph_Reset_Disable(RCU_TLIRST);
}

/*!
    简介:    initialize the parameters of TLI parameter structure with the default values, it is suggested
                that call this function after a TLI_Parameter_Struct structure is defined
    参数[输入]:  无
    参数[输出]: TLI_Struct: the data needed to initialize TLI
                  synpsz_vpsz: size of the vertical synchronous pulse
                  synpsz_hpsz: size of the horizontal synchronous pulse
                  backpsz_vbpsz: size of the vertical back porch plus synchronous pulse
                  backpsz_hbpsz: size of the horizontal back porch plus synchronous pulse
                  activesz_vasz: size of the vertical active area width plus back porch and synchronous pulse
                  activesz_hasz: size of the horizontal active area width plus back porch and synchronous pulse
                  totalsz_vtsz: vertical total size of the display, including active area, back porch, synchronous
                  totalsz_htsz: vorizontal total size of the display, including active area, back porch, synchronous
                  backcolor_red: background value red
                  backcolor_green: background value green
                  backcolor_blue: background value blue
                  signalpolarity_hs: TLI_HSYN_ACTLIVE_LOW,TLI_HSYN_ACTLIVE_HIGHT
                  signalpolarity_vs: TLI_VSYN_ACTLIVE_LOW,TLI_VSYN_ACTLIVE_HIGHT
                  signalpolarity_de: TLI_DE_ACTLIVE_LOW,TLI_DE_ACTLIVE_HIGHT
                  signalpolarity_pixelck: TLI_PIXEL_Clock_TLI,TLI_PIXEL_Clock_INVERTEDTLI
    返回值:      无
*/
void TLI_Struct_Para_Init(TLI_Parameter_Struct *TLI_Struct) {
    /* initialize the struct parameters with default values */
    TLI_Struct->synpsz_vpsz = TLI_DEFAULT_VALUE;
    TLI_Struct->synpsz_hpsz = TLI_DEFAULT_VALUE;
    TLI_Struct->backpsz_vbpsz = TLI_DEFAULT_VALUE;
    TLI_Struct->backpsz_hbpsz = TLI_DEFAULT_VALUE;
    TLI_Struct->activesz_vasz = TLI_DEFAULT_VALUE;
    TLI_Struct->activesz_hasz = TLI_DEFAULT_VALUE;
    TLI_Struct->totalsz_vtsz = TLI_DEFAULT_VALUE;
    TLI_Struct->totalsz_htsz = TLI_DEFAULT_VALUE;
    TLI_Struct->backcolor_red = TLI_DEFAULT_VALUE;
    TLI_Struct->backcolor_green = TLI_DEFAULT_VALUE;
    TLI_Struct->backcolor_blue = TLI_DEFAULT_VALUE;
    TLI_Struct->signalpolarity_hs = TLI_HSYN_ACTLIVE_LOW;
    TLI_Struct->signalpolarity_vs = TLI_VSYN_ACTLIVE_LOW;
    TLI_Struct->signalpolarity_de = TLI_DE_ACTLIVE_LOW;
    TLI_Struct->signalpolarity_pixelck = TLI_PIXEL_Clock_TLI;
}

/*!
    简介:    initialize TLI display 定时参数
    参数[输入]:  TLI_Struct: the data needed to initialize TLI
                  synpsz_vpsz: size of the vertical synchronous pulse
                  synpsz_hpsz: size of the horizontal synchronous pulse
                  backpsz_vbpsz: size of the vertical back porch plus synchronous pulse
                  backpsz_hbpsz: size of the horizontal back porch plus synchronous pulse
                  activesz_vasz: size of the vertical active area width plus back porch and synchronous pulse
                  activesz_hasz: size of the horizontal active area width plus back porch and synchronous pulse
                  totalsz_vtsz: vertical total size of the display, including active area, back porch, synchronous
                  totalsz_htsz: vorizontal total size of the display, including active area, back porch, synchronous
                  backcolor_red: background value red
                  backcolor_green: background value green
                  backcolor_blue: background value blue
                  signalpolarity_hs: TLI_HSYN_ACTLIVE_LOW,TLI_HSYN_ACTLIVE_HIGHT
                  signalpolarity_vs: TLI_VSYN_ACTLIVE_LOW,TLI_VSYN_ACTLIVE_HIGHT
                  signalpolarity_de: TLI_DE_ACTLIVE_LOW,TLI_DE_ACTLIVE_HIGHT
                  signalpolarity_pixelck: TLI_PIXEL_Clock_TLI,TLI_PIXEL_Clock_INVERTEDTLI
    参数[输出]:  无
    返回值:      无
*/
void TLI_Init(TLI_Parameter_Struct *TLI_Struct) {
    /* synchronous pulse size configuration */
    TLI_SPSZ &= ~(TLI_SPSZ_VPSZ | TLI_SPSZ_HPSZ);
    TLI_SPSZ = (uint32_t)((uint32_t)TLI_Struct->synpsz_vpsz | ((uint32_t)TLI_Struct->synpsz_hpsz << 16U));
    /* back-porch size configuration */
    TLI_BPSZ &= ~(TLI_BPSZ_VBPSZ | TLI_BPSZ_HBPSZ);
    TLI_BPSZ = (uint32_t)((uint32_t)TLI_Struct->backpsz_vbpsz | ((uint32_t)TLI_Struct->backpsz_hbpsz << 16U));
    /* active size configuration */
    TLI_ASZ &= ~(TLI_ASZ_VASZ | TLI_ASZ_HASZ);
    TLI_ASZ = (TLI_Struct->activesz_vasz | (TLI_Struct->activesz_hasz << 16U));
    /* total size configuration */
    TLI_TSZ &= ~(TLI_TSZ_VTSZ | TLI_TSZ_HTSZ);
    TLI_TSZ = (TLI_Struct->totalsz_vtsz | (TLI_Struct->totalsz_htsz << 16U));
    /* background color configuration */
    TLI_BGC &= ~(TLI_BGC_BVB | (TLI_BGC_BVG) | (TLI_BGC_BVR));
    TLI_BGC = (TLI_Struct->backcolor_blue | (TLI_Struct->backcolor_green << 8U) | (TLI_Struct->backcolor_red << 16U));
    TLI_CTL &= ~(TLI_CTL_HPPS | TLI_CTL_VPPS | TLI_CTL_DEPS | TLI_CTL_CLKPS);
    TLI_CTL |= (TLI_Struct->signalpolarity_hs | TLI_Struct->signalpolarity_vs | \
                TLI_Struct->signalpolarity_de | TLI_Struct->signalpolarity_pixelck);

}

/*!
    简介:    configure TLI dither function
    参数[输入]:  dither_stat
                only one parameter can be selected which is shown as below:
      参数:        TLI_Dither_ENABLE
      参数:        TLI_Dither_DISABLE
    参数[输出]:  无
    返回值:      无
*/
void TLI_Dither_Config(uint8_t dither_stat) {
    if(TLI_Dither_ENABLE == dither_stat) {
        TLI_CTL |= TLI_CTL_DFEN;
    } else {
        TLI_CTL &= ~(TLI_CTL_DFEN);
    }
}

/*!
    简介:    enable TLI
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void TLI_Enable(void) {
    TLI_CTL |= TLI_CTL_TLIEN;
}

/*!
    简介:    disable TLI
    参数[输入]:  无
    参数[输出]:  无
    返回值:      无
*/
void TLI_Disable(void) {
    TLI_CTL &= ~(TLI_CTL_TLIEN);
}

/*!
    简介:    configure TLI reload mode
    参数[输入]:  reload_mod
                only one parameter can be selected which is shown as below:
      参数:        TLI_Frame_BLANK_Reload_EN
      参数:        TLI_Request_Reload_EN
    参数[输出]:  无
    返回值:      无
*/
void TLI_Reload_Config(uint8_t reload_mod) {
    if(TLI_Frame_BLANK_Reload_EN == reload_mod) {
        /* the layer configuration will be reloaded at frame blank */
        TLI_RL |= TLI_RL_FBR;
    } else {
        /* the layer configuration will be reloaded after this bit sets */
        TLI_RL |= TLI_RL_RQR;
    }
}

/*!
    简介:    initialize the parameters of TLI layer structure with the default values, it is suggested
                that call this function after a TLI_Layer_Parameter_Struct structure is defined
    参数[输入]:  无
    参数[输出]: layer_Struct: TLI Layer parameter struct
                  layer_Window_rightpos: window right position
                  layer_Window_leftpos: window left position
                  layer_Window_bottompos: window bottom position
                  layer_Window_toppos: window top position
                  layer_ppf: LAYER_PPF_ARGB8888,LAYER_PPF_RGB888,LAYER_PPF_RGB565,
                                 LAYER_PPF_ARG1555,LAYER_PPF_ARGB4444,LAYER_PPF_L8,
                                 LAYER_PPF_AL44,LAYER_PPF_AL88
                  layer_sa: specified alpha
                  layer_default_alpha: the default color alpha
                  layer_default_red: the default color red
                  layer_default_green: the default color green
                  layer_default_blue: the default color blue
                  layer_acf1: LAYER_ACF1_SA,LAYER_ACF1_PASA
                  layer_acf2: LAYER_ACF2_SA,LAYER_ACF2_PASA
                  layer_Frame_bufaddr: frame buffer base address
                  layer_Frame_buf_stride_offset: frame buffer stride offset
                  layer_Frame_Line_length: frame line length
                  layer_Frame_total_Line_number: frame total line number
    返回值:      无
*/
void TLI_Layer_Struct_Para_Init(TLI_Layer_Parameter_Struct *layer_Struct) {
    /* initialize the struct parameters with default values */
    layer_Struct->layer_Window_rightpos = TLI_DEFAULT_VALUE;
    layer_Struct->layer_Window_leftpos = TLI_DEFAULT_VALUE;
    layer_Struct->layer_Window_bottompos = TLI_DEFAULT_VALUE;
    layer_Struct->layer_Window_toppos = TLI_DEFAULT_VALUE;
    layer_Struct->layer_ppf = LAYER_PPF_ARGB8888;
    layer_Struct->layer_sa = TLI_OPAQUE_VALUE;
    layer_Struct->layer_default_alpha = TLI_DEFAULT_VALUE;
    layer_Struct->layer_default_red = TLI_DEFAULT_VALUE;
    layer_Struct->layer_default_green = TLI_DEFAULT_VALUE;
    layer_Struct->layer_default_blue = TLI_DEFAULT_VALUE;
    layer_Struct->layer_acf1 = LAYER_ACF1_PASA;
    layer_Struct->layer_acf2 = LAYER_ACF2_PASA;
    layer_Struct->layer_Frame_bufaddr = TLI_DEFAULT_VALUE;
    layer_Struct->layer_Frame_buf_stride_offset = TLI_DEFAULT_VALUE;
    layer_Struct->layer_Frame_Line_length = TLI_DEFAULT_VALUE;
    layer_Struct->layer_Frame_total_Line_number = TLI_DEFAULT_VALUE;
}

/*!
    简介:    initialize TLI layer
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输入]:  layer_Struct: TLI Layer parameter struct
                  layer_Window_rightpos: window right position
                  layer_Window_leftpos: window left position
                  layer_Window_bottompos: window bottom position
                  layer_Window_toppos: window top position
                  layer_ppf: LAYER_PPF_ARGB8888,LAYER_PPF_RGB888,LAYER_PPF_RGB565,
                                 LAYER_PPF_ARG1555,LAYER_PPF_ARGB4444,LAYER_PPF_L8,
                                 LAYER_PPF_AL44,LAYER_PPF_AL88
                  layer_sa: specified alpha
                  layer_default_alpha: the default color alpha
                  layer_default_red: the default color red
                  layer_default_green: the default color green
                  layer_default_blue: the default color blue
                  layer_acf1: LAYER_ACF1_SA,LAYER_ACF1_PASA
                  layer_acf2: LAYER_ACF2_SA,LAYER_ACF2_PASA
                  layer_Frame_bufaddr: frame buffer base address
                  layer_Frame_buf_stride_offset: frame buffer stride offset
                  layer_Frame_Line_length: frame line length
                  layer_Frame_total_Line_number: frame total line number
    参数[输出]:  无
    返回值:      无
*/
void TLI_Layer_Init(uint32_t layerx, TLI_Layer_Parameter_Struct *layer_Struct) {
    /* configure layer window horizontal position */
    TLI_LxHPOS(layerx) &= ~(TLI_LxHPOS_WLP | (TLI_LxHPOS_WRP));
    TLI_LxHPOS(layerx) = (uint32_t)((uint32_t)layer_Struct->layer_Window_leftpos | ((uint32_t)layer_Struct->layer_Window_rightpos << 16U));
    /* configure layer window vertical position */
    TLI_LxVPOS(layerx) &= ~(TLI_LxVPOS_WTP | (TLI_LxVPOS_WBP));
    TLI_LxVPOS(layerx) = (uint32_t)((uint32_t)layer_Struct->layer_Window_toppos | ((uint32_t)layer_Struct->layer_Window_bottompos << 16U));
    /* configure layer packeted pixel format */
    TLI_LxPPF(layerx) &= ~(TLI_LxPPF_PPF);
    TLI_LxPPF(layerx) = layer_Struct->layer_ppf;
    /* configure layer specified alpha */
    TLI_LxSA(layerx) &= ~(TLI_LxSA_SA);
    TLI_LxSA(layerx) = layer_Struct->layer_sa;
    /* configure layer default color */
    TLI_LxDC(layerx) &= ~(TLI_LxDC_DCB | (TLI_LxDC_DCG) | (TLI_LxDC_DCR) | (TLI_LxDC_DCA));
    TLI_LxDC(layerx) = (uint32_t)((uint32_t)layer_Struct->layer_default_blue | ((uint32_t)layer_Struct->layer_default_green << 8U)
                                  | ((uint32_t)layer_Struct->layer_default_red << 16U)
                                  | ((uint32_t)layer_Struct->layer_default_alpha << 24U));

    /* configure layer alpha calculation factors */
    TLI_LxBLEND(layerx) &= ~(TLI_LxBLEND_ACF2 | (TLI_LxBLEND_ACF1));
    TLI_LxBLEND(layerx) = ((layer_Struct->layer_acf2) | (layer_Struct->layer_acf1));
    /* configure layer frame buffer base address */
    TLI_LxFBADDR(layerx) &= ~(TLI_LxFBADDR_FBADD);
    TLI_LxFBADDR(layerx) = (layer_Struct->layer_Frame_bufaddr);
    /* configure layer frame line length */
    TLI_LxFLLEN(layerx) &= ~(TLI_LxFLLEN_FLL | (TLI_LxFLLEN_STDOFF));
    TLI_LxFLLEN(layerx) = (uint32_t)((uint32_t)layer_Struct->layer_Frame_Line_length | ((uint32_t)layer_Struct->layer_Frame_buf_stride_offset << 16U));
    /* configure layer frame total line number */
    TLI_LxFTLN(layerx) &= ~(TLI_LxFTLN_FTLN);
    TLI_LxFTLN(layerx) = (uint32_t)(layer_Struct->layer_Frame_total_Line_number);

}

/*!
    简介:    reconfigure window position
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输入]:  offset_x: new horizontal offset
    参数[输入]:  offset_y: new vertical offset
    参数[输出]:  无
    返回值:      无
*/
void TLI_Layer_Window_Offset_Modify(uint32_t layerx, uint16_t offset_x, uint16_t offset_y) {
    /* configure window start position */
    uint32_t layer_ppf, line_num, hstart, vstart;
    uint32_t line_length = 0U;
    TLI_LxHPOS(layerx) &= ~(TLI_LxHPOS_WLP | (TLI_LxHPOS_WRP));
    TLI_LxVPOS(layerx) &= ~(TLI_LxVPOS_WTP | (TLI_LxVPOS_WBP));
    hstart = (uint32_t)offset_x + (((TLI_BPSZ & TLI_BPSZ_HBPSZ) >> 16U) + 1U);
    vstart = (uint32_t)offset_y + ((TLI_BPSZ & TLI_BPSZ_VBPSZ) + 1U);
    line_num = (TLI_LxFTLN(layerx) & TLI_LxFTLN_FTLN);
    layer_ppf = (TLI_LxPPF(layerx) & TLI_LxPPF_PPF);

    /* the bytes of a line equal TLI_LxFLLEN_FLL bits value minus 3 */
    switch(layer_ppf) {
    case LAYER_PPF_ARGB8888:
        /* each pixel includes 4bytes, when pixel format is ARGB8888 */
        line_length = (((TLI_LxFLLEN(layerx) & TLI_LxFLLEN_FLL) - 3U) / 4U);
        break;

    case LAYER_PPF_RGB888:
        /* each pixel includes 3bytes, when pixel format is RGB888 */
        line_length = (((TLI_LxFLLEN(layerx) & TLI_LxFLLEN_FLL) - 3U) / 3U);
        break;

    case LAYER_PPF_RGB565:
    case LAYER_PPF_ARGB1555:
    case LAYER_PPF_ARGB4444:
    case LAYER_PPF_AL88:
        /* each pixel includes 2bytes, when pixel format is RGB565,ARG1555,ARGB4444 or AL88 */
        line_length = (((TLI_LxFLLEN(layerx) & TLI_LxFLLEN_FLL) - 3U) / 2U);
        break;

    case LAYER_PPF_L8:
    case LAYER_PPF_AL44:
        /* each pixel includes 1byte, when pixel format is L8 or AL44 */
        line_length = (((TLI_LxFLLEN(layerx) & TLI_LxFLLEN_FLL) - 3U));
        break;

    default:
        break;
    }

    /* reconfigure window position */
    TLI_LxHPOS(layerx) = (hstart | ((hstart + line_length - 1U) << 16U));
    TLI_LxVPOS(layerx) = (vstart | ((vstart + line_num - 1U) << 16U));
}

/*!
    简介:    initialize the parameters of TLI layer LUT structure with the default values, it is suggested
                that call this function after a TLI_Layer_Lut_Parameter_Struct structure is defined
    参数[输入]:  无
    参数[输出]: lut_Struct: TLI layer LUT parameter struct
                  layer_table_addr: look up table write address
                  layer_Lut_Channel_red: red channel of a LUT entry
                  layer_Lut_Channel_green: green channel of a LUT entry
                  layer_Lut_Channel_blue: blue channel of a LUT entry
    返回值:      无
*/
void TLI_Lut_Struct_Para_Init(TLI_Layer_Lut_Parameter_Struct *lut_Struct) {
    /* initialize the struct parameters with default values */
    lut_Struct->layer_table_addr = TLI_DEFAULT_VALUE;
    lut_Struct->layer_Lut_Channel_red = TLI_DEFAULT_VALUE;
    lut_Struct->layer_Lut_Channel_green = TLI_DEFAULT_VALUE;
    lut_Struct->layer_Lut_Channel_blue = TLI_DEFAULT_VALUE;
}

/*!
    简介:    initialize TLI layer LUT
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输入]:  lut_Struct: TLI layer LUT parameter struct
                  layer_table_addr: look up table write address
                  layer_Lut_Channel_red: red channel of a LUT entry
                  layer_Lut_Channel_green: green channel of a LUT entry
                  layer_Lut_Channel_blue: blue channel of a LUT entry
    参数[输出]:  无
    返回值:      无
*/
void TLI_Lut_Init(uint32_t layerx, TLI_Layer_Lut_Parameter_Struct *lut_Struct) {
    TLI_LxLUT(layerx) = (uint32_t)(((uint32_t)lut_Struct->layer_Lut_Channel_blue) | ((uint32_t)lut_Struct->layer_Lut_Channel_green << 8U)
                                   | ((uint32_t)lut_Struct->layer_Lut_Channel_red << 16U
                                      | ((uint32_t)lut_Struct->layer_table_addr << 24U)));
}

/*!
    简介:    initialize TLI layer color key
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输入]:  redkey: color key red
    参数[输入]:  greenkey: color key green
    参数[输入]:  bluekey: color key blue
    参数[输出]:  无
    返回值:      无
*/
void TLI_Color_Key_Init(uint32_t layerx, uint8_t redkey, uint8_t greenkey, uint8_t bluekey) {
    TLI_LxCKEY(layerx) = (((uint32_t)bluekey) | ((uint32_t)greenkey << 8U) | ((uint32_t)redkey << 16U));
}

/*!
    简介:    enable TLI layer
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void TLI_Layer_Enable(uint32_t layerx) {
    TLI_LxCTL(layerx) |= TLI_LxCTL_LEN;
}

/*!
    简介:    disable TLI layer
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void TLI_Layer_Disable(uint32_t layerx) {
    TLI_LxCTL(layerx) &= ~(TLI_LxCTL_LEN);
}

/*!
    简介:    enable TLI layer color keying
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void TLI_Color_Key_Enable(uint32_t layerx) {
    TLI_LxCTL(layerx) |= TLI_LxCTL_CKEYEN;
}

/*!
    简介:    disable TLI layer color keying
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void TLI_Color_Key_Disable(uint32_t layerx) {
    TLI_LxCTL(layerx) &= ~(TLI_LxCTL_CKEYEN);
}

/*!
    简介:    enable TLI layer LUT
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void TLI_Lut_Enable(uint32_t layerx) {
    TLI_LxCTL(layerx) |= TLI_LxCTL_LUTEN;
}

/*!
    简介:    disable TLI layer LUT
    参数[输入]:  layerx: LAYERx(x=0,1)
    参数[输出]:  无
    返回值:      无
*/
void TLI_Lut_Disable(uint32_t layerx) {
    TLI_LxCTL(layerx) &= ~(TLI_LxCTL_LUTEN);
}

/*!
    简介:    set line mark value
    参数[输入]:  line_num: line number
    参数[输出]:  无
    返回值:      无
*/
void TLI_Line_Mark_Set(uint16_t line_num) {
    TLI_LM &= ~(TLI_LM_LM);
    TLI_LM = (uint32_t)line_num;
}

/*!
    简介:    get current displayed position
    参数[输入]:  无
    参数[输出]:  无
    返回值:     position of current pixel
*/
uint32_t TLI_Current_Pos_Get(void) {
    return TLI_CPPOS;
}

/*!
    简介:    enable TLI interrupt
    参数[输入]:  int_flag: TLI interrupt flags
                one or more parameters can be selected which are shown as below:
      参数:        TLI_INT_LM: line mark interrupt
      参数:        TLI_INT_FE: FIFO error interrupt
      参数:        TLI_INT_TE: transaction error interrupt
      参数:        TLI_INT_LCR: layer configuration reloaded interrupt
    参数[输出]:  无
    返回值:      无
*/
void TLI_Interrupt_Enable(uint32_t int_flag) {
    TLI_INTEN |= (int_flag);
}

/*!
    简介:    disable TLI interrupt
    参数[输入]:  int_flag: TLI interrupt flags
                one or more parameters can be selected which are shown as below:
      参数:        TLI_INT_LM: line mark interrupt
      参数:        TLI_INT_FE: FIFO error interrupt
      参数:        TLI_INT_TE: transaction error interrupt
      参数:        TLI_INT_LCR: layer configuration reloaded interrupt
    参数[输出]:  无
    返回值:      无
*/
void TLI_Interrupt_Disable(uint32_t int_flag) {
    TLI_INTEN &= ~(int_flag);
}

/*!
    简介:    get TLI interrupt flag
    参数[输入]:  int_flag: TLI interrupt flags
                one or more parameters can be selected which are shown as below:
      参数:        TLI_INT_Flag_LM: line mark interrupt flag
      参数:        TLI_INT_Flag_FE: FIFO error interrupt flag
      参数:        TLI_INT_Flag_TE: transaction error interrupt flag
      参数:        TLI_INT_Flag_LCR: layer configuration reloaded interrupt flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus TLI_Interrupt_Flag_Get(uint32_t int_flag) {
    uint32_t state;
    state = TLI_INTF;

    if(state & int_flag) {
        state = TLI_INTEN;

        if(state & int_flag) {
            return SET;
        }
    }

    return RESET;
}

/*!
    简介:    clear TLI interrupt flag
    参数[输入]:  int_flag: TLI interrupt flags
                one or more parameters can be selected which are shown as below:
      参数:        TLI_INT_Flag_LM: line mark interrupt flag
      参数:        TLI_INT_Flag_FE: FIFO error interrupt flag
      参数:        TLI_INT_Flag_TE: transaction error interrupt flag
      参数:        TLI_INT_Flag_LCR: layer configuration reloaded interrupt flag
    参数[输出]:  无
    返回值:      无
*/
void TLI_Interrupt_Flag_Clear(uint32_t int_flag) {
    TLI_INTC |= (int_flag);
}

/*!
    简介:    get TLI flag or state in TLI_INTF register or TLI_STAT register
    参数[输入]:  flag: TLI flags or states
                only one parameter can be selected which is shown as below:
      参数:        TLI_Flag_VDE: current VDE state
      参数:        TLI_Flag_HDE: current HDE state
      参数:        TLI_Flag_VS: current VS status of the TLI
      参数:        TLI_Flag_HS: current HS status of the TLI
      参数:        TLI_Flag_LM: line mark interrupt flag
      参数:        TLI_Flag_FE: FIFO error interrupt flag
      参数:        TLI_Flag_TE: transaction error interrupt flag
      参数:        TLI_Flag_LCR: layer configuration reloaded interrupt flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus TLI_Flag_Get(uint32_t flag) {
    uint32_t stat;

    /* choose which register to get flag or state */
    if(flag >> 31U) {
        stat = TLI_INTF;
    } else {
        stat = TLI_STAT;
    }

    if(flag & stat) {
        return SET;
    } else {
        return RESET;
    }
}
