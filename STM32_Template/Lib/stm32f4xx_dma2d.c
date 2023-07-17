/**
  ******************************************************************************
  * 文件:    stm32f4xx_dma2d.c
  * 作者:    MCD Application Team
  * 版本:    V1.8.0
  * 日期:    04-November-2016
  * 简介:    此文件提供固件功能，以管理DMA2D控制器(DMA2D)外围设备的以下功能:
  *           + 初始化和配置
  *           + 中断和标志管理
  *
  @verbatim
 ===============================================================================
                      ##### 如何使用这个驱动程序 #####
 ===============================================================================
    [..]
        (#)启用DMA2D时钟使用
            RCC_APB2PeriphResetCmd(RCC_ABP2PeripH_DMA2D，ENABLE)函数。
            (#)配置DMA2D
            (++)传输模式
            (++)像素格式，行_编号，像素_每行
            (++)输出内存地址
            (++)alpha值
            (++)输出偏移
            (++)默认颜色(RGB)
        (#)配置前景或/和背景
            (++)内存地址
            (++)alpha值
            (++)偏移和默认颜色
         (#)调用DMA2D_Start()以启用DMA2D控制器。

    @endverbatim

  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2016 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_dma2d.h"
#include "stm32f4xx_rcc.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @defgroup DMA2D
  * 简介: DMA2D 驱动模块
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* 私有宏 ------------------------------------------------------------*/
/* 私有宏 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------*/
/* 私有函数 ---------------------------------------------------------*/

#define CR_MASK                     ((uint32_t)0xFFFCE0FC)  /* DMA2D CR Mask */
#define PFCCR_MASK                  ((uint32_t)0x00FC00C0)  /* DMA2D FGPFCCR Mask */
#define DEAD_MASK                   ((uint32_t)0xFFFF00FE)  /* DMA2D DEAD Mask */

/** @defgroup DMA2D_Private_Functions
  * @{
  */

/** @defgroup DMA2D_Group1 初始化和配置函数
 *  简介   初始化和配置函数
 *
@verbatim
 ===============================================================================
            ##### 初始化和配置函数 #####
 ===============================================================================
    [..]  本节提供的功能允许:
      (+)初始化和配置DMA2D
      (+)启动/中止/挂起传输
      (+)初始化、配置和设置前景和背景
      (+)配置并启用DeadTime
      (+)配置线条水印


@endverbatim
  * @{
  */

/**
  * 简介:  将DMA2D外围寄存器取消初始化为其默认重置值。
  * 参数:  无
  * 返回值: 无
  */

void DMA2D_DeInit(void) {
    /* 使能DMA2D 复位状态 */
    RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA2D, ENABLE);
    /* 释放DMA2D from 复位状态 */
    RCC_AHB1PeriphResetCmd(RCC_AHB1Periph_DMA2D, DISABLE);
}


/**
  * 简介:  根据 DMA2D_InitStruct 中指定的参数初始化 DMA2D 外设。
  * 注意:   仅当DMA2D被禁用时，才能使用此函数。
  * 参数:  DMA2D_InitStruct: 指向DMA2D_InitTypeDef结构的指针，该结构包含指定DMA2D的配置信息外设.
  * 返回值: 无
  */
void DMA2D_Init(DMA2D_InitTypeDef* DMA2D_InitStruct) {

    uint32_t outgreen = 0;
    uint32_t outred   = 0;
    uint32_t outalpha = 0;
    uint32_t pixline  = 0;

    /* 检查参数 */
    assert_param(IS_DMA2D_MODE(DMA2D_InitStruct->DMA2D_Mode));
    assert_param(IS_DMA2D_CMODE(DMA2D_InitStruct->DMA2D_CMode));
    assert_param(IS_DMA2D_OGREEN(DMA2D_InitStruct->DMA2D_OutputGreen));
    assert_param(IS_DMA2D_ORED(DMA2D_InitStruct->DMA2D_OutputRed));
    assert_param(IS_DMA2D_OBLUE(DMA2D_InitStruct->DMA2D_OutputBlue));
    assert_param(IS_DMA2D_OALPHA(DMA2D_InitStruct->DMA2D_OutputAlpha));
    assert_param(IS_DMA2D_OUTPUT_OFFSET(DMA2D_InitStruct->DMA2D_OutputOffset));
    assert_param(IS_DMA2D_LINE(DMA2D_InitStruct->DMA2D_NumberOfLine));
    assert_param(IS_DMA2D_PIXEL(DMA2D_InitStruct->DMA2D_PixelPerLine));

    /* Configures the DMA2D operation mode */
    DMA2D->CR &= (uint32_t)CR_MASK;
    DMA2D->CR |= (DMA2D_InitStruct->DMA2D_Mode);

    /* Configures the color mode of the output image */
    DMA2D->OPFCCR &= ~(uint32_t)DMA2D_OPFCCR_CM;
    DMA2D->OPFCCR |= (DMA2D_InitStruct->DMA2D_CMode);

    /* 配置输出 color */

    if (DMA2D_InitStruct->DMA2D_CMode == DMA2D_ARGB8888) {
        outgreen = DMA2D_InitStruct->DMA2D_OutputGreen << 8;
        outred = DMA2D_InitStruct->DMA2D_OutputRed << 16;
        outalpha = DMA2D_InitStruct->DMA2D_OutputAlpha << 24;
    } else

        if (DMA2D_InitStruct->DMA2D_CMode == DMA2D_RGB888) {
            outgreen = DMA2D_InitStruct->DMA2D_OutputGreen << 8;
            outred = DMA2D_InitStruct->DMA2D_OutputRed << 16;
            outalpha = (uint32_t)0x00000000;
        }

        else

            if (DMA2D_InitStruct->DMA2D_CMode == DMA2D_RGB565) {
                outgreen = DMA2D_InitStruct->DMA2D_OutputGreen << 5;
                outred = DMA2D_InitStruct->DMA2D_OutputRed << 11;
                outalpha = (uint32_t)0x00000000;
            }

            else

                if (DMA2D_InitStruct->DMA2D_CMode == DMA2D_ARGB1555) {
                    outgreen = DMA2D_InitStruct->DMA2D_OutputGreen << 5;
                    outred = DMA2D_InitStruct->DMA2D_OutputRed << 10;
                    outalpha = DMA2D_InitStruct->DMA2D_OutputAlpha << 15;
                }

                else { /* DMA2D_CMode = DMA2D_ARGB4444 */
                    outgreen = DMA2D_InitStruct->DMA2D_OutputGreen << 4;
                    outred = DMA2D_InitStruct->DMA2D_OutputRed << 8;
                    outalpha = DMA2D_InitStruct->DMA2D_OutputAlpha << 12;
                }

    DMA2D->OCOLR |= ((outgreen) | (outred) | (DMA2D_InitStruct->DMA2D_OutputBlue) | (outalpha));

    /* 配置输出 memory address */
    DMA2D->OMAR = (DMA2D_InitStruct->DMA2D_OutputMemoryAdd);

    /* Configure  the line Offset */
    DMA2D->OOR &= ~(uint32_t)DMA2D_OOR_LO;
    DMA2D->OOR |= (DMA2D_InitStruct->DMA2D_OutputOffset);

    /* 配置 number of line and pixel per line */
    pixline = DMA2D_InitStruct->DMA2D_PixelPerLine << 16;
    DMA2D->NLR &= ~(DMA2D_NLR_NL | DMA2D_NLR_PL);
    DMA2D->NLR |= ((DMA2D_InitStruct->DMA2D_NumberOfLine) | (pixline));

}

/**
  * 简介:  用默认值填充每个DMA2D_InitStruct成员。
  * 参数:  DMA2D_InitStruct: 指向将被初始化的DMA2D_InitTypeDef结构的指针。
  * 返回值: 无
  */
void DMA2D_StructInit(DMA2D_InitTypeDef* DMA2D_InitStruct) {
    /* 初始化 transfer mode 成员 */
    DMA2D_InitStruct->DMA2D_Mode = DMA2D_M2M;

    /* 初始化 output color mode members */
    DMA2D_InitStruct->DMA2D_CMode = DMA2D_ARGB8888;

    /* 初始化 alpha and RGB values */
    DMA2D_InitStruct->DMA2D_OutputGreen = 0x00;
    DMA2D_InitStruct->DMA2D_OutputBlue = 0x00;
    DMA2D_InitStruct->DMA2D_OutputRed = 0x00;
    DMA2D_InitStruct->DMA2D_OutputAlpha = 0x00;

    /* 初始化 output memory address */
    DMA2D_InitStruct->DMA2D_OutputMemoryAdd = 0x00;

    /* 初始化 output offset */
    DMA2D_InitStruct->DMA2D_OutputOffset = 0x00;

    /* 初始化 number of line and the number of pixel per line */
    DMA2D_InitStruct->DMA2D_NumberOfLine = 0x00;
    DMA2D_InitStruct->DMA2D_PixelPerLine = 0x00;
}

/**
  * 简介:  启动 DMA2D 传输。
  * 参数:
  * 返回值: 无
  */

void DMA2D_StartTransfer(void) {
    /* Start DMA2D transfer by setting START 位 */
    DMA2D->CR |= (uint32_t)DMA2D_CR_START;
}

/**
  * 简介:  中止DMA2D传输。
  * 参数:
  * 返回值: 无
  */

void DMA2D_AbortTransfer(void) {
    /* Start DMA2D transfer by setting START 位 */
    DMA2D->CR |= (uint32_t)DMA2D_CR_ABORT;

}

/**
  * 简介:  停止或继续 DMA2D 传输。
  * 参数:  NewState: DMA2D的新状态外设.
  *   此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void DMA2D_Suspend(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Suspend DMA2D transfer by setting STOP 位 */
        DMA2D->CR |= (uint32_t)DMA2D_CR_SUSP;
    } else {
        /* Continue DMA2D transfer by clearing STOP 位 */
        DMA2D->CR &= ~(uint32_t)DMA2D_CR_SUSP;
    }
}

/**
  * 简介:  根据 DMA2D_FGStruct 中的指定参数配置 Foreground。
  * 注意:   仅当传输被禁用时，才能使用此功能。
  * 参数:  DMA2D_FGStruct: 指向DMA2D_FGTypeDef结构的指针，该结构包含指定背景的配置信息。
  * 返回值: 无
  */
void DMA2D_FGConfig(DMA2D_FG_InitTypeDef* DMA2D_FG_InitStruct) {

    uint32_t fg_clutcolormode = 0;
    uint32_t fg_clutsize = 0;
    uint32_t fg_alpha_mode = 0;
    uint32_t fg_alphavalue = 0;
    uint32_t fg_colorgreen = 0;
    uint32_t fg_colorred = 0;

    assert_param(IS_DMA2D_FGO(DMA2D_FG_InitStruct->DMA2D_FGO));
    assert_param(IS_DMA2D_FGCM(DMA2D_FG_InitStruct->DMA2D_FGCM));
    assert_param(IS_DMA2D_FG_CLUT_CM(DMA2D_FG_InitStruct->DMA2D_FG_CLUT_CM));
    assert_param(IS_DMA2D_FG_CLUT_SIZE(DMA2D_FG_InitStruct->DMA2D_FG_CLUT_SIZE));
    assert_param(IS_DMA2D_FG_ALPHA_MODE(DMA2D_FG_InitStruct->DMA2D_FGPFC_ALPHA_MODE));
    assert_param(IS_DMA2D_FG_ALPHA_VALUE(DMA2D_FG_InitStruct->DMA2D_FGPFC_ALPHA_VALUE));
    assert_param(IS_DMA2D_FGC_BLUE(DMA2D_FG_InitStruct->DMA2D_FGC_BLUE));
    assert_param(IS_DMA2D_FGC_GREEN(DMA2D_FG_InitStruct->DMA2D_FGC_GREEN));
    assert_param(IS_DMA2D_FGC_RED(DMA2D_FG_InitStruct->DMA2D_FGC_RED));

    /* Configures the FG memory address */
    DMA2D->FGMAR = (DMA2D_FG_InitStruct->DMA2D_FGMA);

    /* Configures the FG offset */
    DMA2D->FGOR &= ~(uint32_t)DMA2D_FGOR_LO;
    DMA2D->FGOR |= (DMA2D_FG_InitStruct->DMA2D_FGO);

    /* Configures foreground Pixel Format Convertor */
    DMA2D->FGPFCCR &= (uint32_t)PFCCR_MASK;
    fg_clutcolormode = DMA2D_FG_InitStruct->DMA2D_FG_CLUT_CM << 4;
    fg_clutsize = DMA2D_FG_InitStruct->DMA2D_FG_CLUT_SIZE << 8;
    fg_alpha_mode = DMA2D_FG_InitStruct->DMA2D_FGPFC_ALPHA_MODE << 16;
    fg_alphavalue = DMA2D_FG_InitStruct->DMA2D_FGPFC_ALPHA_VALUE << 24;
    DMA2D->FGPFCCR |= (DMA2D_FG_InitStruct->DMA2D_FGCM | fg_clutcolormode | fg_clutsize | \
                       fg_alpha_mode | fg_alphavalue);

    /* Configures foreground color */
    DMA2D->FGCOLR &= ~(DMA2D_FGCOLR_BLUE | DMA2D_FGCOLR_GREEN | DMA2D_FGCOLR_RED);
    fg_colorgreen = DMA2D_FG_InitStruct->DMA2D_FGC_GREEN << 8;
    fg_colorred = DMA2D_FG_InitStruct->DMA2D_FGC_RED << 16;
    DMA2D->FGCOLR |= (DMA2D_FG_InitStruct->DMA2D_FGC_BLUE | fg_colorgreen | fg_colorred);

    /* Configures foreground CLUT memory address */
    DMA2D->FGCMAR = DMA2D_FG_InitStruct->DMA2D_FGCMAR;
}

/**
  * 简介:  用默认值填充每个DMA2D_FGStruct成员。
  * 参数:  DMA2D_FGStruct: 指向将被初始化的DMA2D_FGTypeDef结构的指针。
  * 返回值: 无
  */
void DMA2D_FG_StructInit(DMA2D_FG_InitTypeDef* DMA2D_FG_InitStruct) {
    /*!< Initialize the DMA2D foreground memory address */
    DMA2D_FG_InitStruct->DMA2D_FGMA = 0x00;

    /*!< Initialize the DMA2D foreground offset */
    DMA2D_FG_InitStruct->DMA2D_FGO = 0x00;

    /*!< Initialize the DMA2D foreground color mode */
    DMA2D_FG_InitStruct->DMA2D_FGCM = CM_ARGB8888;

    /*!< Initialize the DMA2D foreground CLUT color mode */
    DMA2D_FG_InitStruct->DMA2D_FG_CLUT_CM = CLUT_CM_ARGB8888;

    /*!< Initialize the DMA2D foreground CLUT size */
    DMA2D_FG_InitStruct->DMA2D_FG_CLUT_SIZE = 0x00;

    /*!< Initialize the DMA2D foreground alpha mode */
    DMA2D_FG_InitStruct->DMA2D_FGPFC_ALPHA_MODE = NO_MODIF_ALPHA_VALUE;

    /*!< Initialize the DMA2D foreground alpha 值 */
    DMA2D_FG_InitStruct->DMA2D_FGPFC_ALPHA_VALUE = 0x00;

    /*!< Initialize the DMA2D foreground blue 值 */
    DMA2D_FG_InitStruct->DMA2D_FGC_BLUE = 0x00;

    /*!< Initialize the DMA2D foreground green 值 */
    DMA2D_FG_InitStruct->DMA2D_FGC_GREEN = 0x00;

    /*!< Initialize the DMA2D foreground red 值 */
    DMA2D_FG_InitStruct->DMA2D_FGC_RED = 0x00;

    /*!< Initialize the DMA2D foreground CLUT memory address */
    DMA2D_FG_InitStruct->DMA2D_FGCMAR = 0x00;
}


/**
  * 简介:  根据 DMA2D_BGStruct 中的指定参数配置背景。
  * 注意:   仅当传输被禁用时，才能使用此功能。
  * 参数:  DMA2D_BGStruct: 指向DMA2D_BGTypeDef结构的指针，该结构包含指定后台的配置信息。
  * 返回值: 无
  */
void DMA2D_BGConfig(DMA2D_BG_InitTypeDef* DMA2D_BG_InitStruct) {

    uint32_t bg_clutcolormode = 0;
    uint32_t bg_clutsize = 0;
    uint32_t bg_alpha_mode = 0;
    uint32_t bg_alphavalue = 0;
    uint32_t bg_colorgreen = 0;
    uint32_t bg_colorred = 0;

    assert_param(IS_DMA2D_BGO(DMA2D_BG_InitStruct->DMA2D_BGO));
    assert_param(IS_DMA2D_BGCM(DMA2D_BG_InitStruct->DMA2D_BGCM));
    assert_param(IS_DMA2D_BG_CLUT_CM(DMA2D_BG_InitStruct->DMA2D_BG_CLUT_CM));
    assert_param(IS_DMA2D_BG_CLUT_SIZE(DMA2D_BG_InitStruct->DMA2D_BG_CLUT_SIZE));
    assert_param(IS_DMA2D_BG_ALPHA_MODE(DMA2D_BG_InitStruct->DMA2D_BGPFC_ALPHA_MODE));
    assert_param(IS_DMA2D_BG_ALPHA_VALUE(DMA2D_BG_InitStruct->DMA2D_BGPFC_ALPHA_VALUE));
    assert_param(IS_DMA2D_BGC_BLUE(DMA2D_BG_InitStruct->DMA2D_BGC_BLUE));
    assert_param(IS_DMA2D_BGC_GREEN(DMA2D_BG_InitStruct->DMA2D_BGC_GREEN));
    assert_param(IS_DMA2D_BGC_RED(DMA2D_BG_InitStruct->DMA2D_BGC_RED));

    /* Configures the BG memory address */
    DMA2D->BGMAR = (DMA2D_BG_InitStruct->DMA2D_BGMA);

    /* Configures the BG offset */
    DMA2D->BGOR &= ~(uint32_t)DMA2D_BGOR_LO;
    DMA2D->BGOR |= (DMA2D_BG_InitStruct->DMA2D_BGO);

    /* Configures background Pixel Format Convertor */
    DMA2D->BGPFCCR &= (uint32_t)PFCCR_MASK;
    bg_clutcolormode = DMA2D_BG_InitStruct->DMA2D_BG_CLUT_CM << 4;
    bg_clutsize = DMA2D_BG_InitStruct->DMA2D_BG_CLUT_SIZE << 8;
    bg_alpha_mode = DMA2D_BG_InitStruct->DMA2D_BGPFC_ALPHA_MODE << 16;
    bg_alphavalue = DMA2D_BG_InitStruct->DMA2D_BGPFC_ALPHA_VALUE << 24;
    DMA2D->BGPFCCR |= (DMA2D_BG_InitStruct->DMA2D_BGCM | bg_clutcolormode | bg_clutsize | \
                       bg_alpha_mode | bg_alphavalue);

    /* Configures background color */
    DMA2D->BGCOLR &= ~(DMA2D_BGCOLR_BLUE | DMA2D_BGCOLR_GREEN | DMA2D_BGCOLR_RED);
    bg_colorgreen = DMA2D_BG_InitStruct->DMA2D_BGC_GREEN << 8;
    bg_colorred = DMA2D_BG_InitStruct->DMA2D_BGC_RED << 16;
    DMA2D->BGCOLR |= (DMA2D_BG_InitStruct->DMA2D_BGC_BLUE | bg_colorgreen | bg_colorred);

    /* Configures background CLUT memory address */
    DMA2D->BGCMAR = DMA2D_BG_InitStruct->DMA2D_BGCMAR;

}

/**
  * 简介:  用默认值填充每个 DMA2D_BGStruct 成员。
  * 参数:  DMA2D_BGStruct: 指向将被初始化的DMA2D_BGTypeDef结构的指针。
  * 返回值: 无
  */
void DMA2D_BG_StructInit(DMA2D_BG_InitTypeDef* DMA2D_BG_InitStruct) {
    /*!< Initialize the DMA2D background memory address */
    DMA2D_BG_InitStruct->DMA2D_BGMA = 0x00;

    /*!< Initialize the DMA2D background offset */
    DMA2D_BG_InitStruct->DMA2D_BGO = 0x00;

    /*!< Initialize the DMA2D background color mode */
    DMA2D_BG_InitStruct->DMA2D_BGCM = CM_ARGB8888;

    /*!< Initialize the DMA2D background CLUT color mode */
    DMA2D_BG_InitStruct->DMA2D_BG_CLUT_CM = CLUT_CM_ARGB8888;

    /*!< Initialize the DMA2D background CLUT size */
    DMA2D_BG_InitStruct->DMA2D_BG_CLUT_SIZE = 0x00;

    /*!< Initialize the DMA2D background alpha mode */
    DMA2D_BG_InitStruct->DMA2D_BGPFC_ALPHA_MODE = NO_MODIF_ALPHA_VALUE;

    /*!< Initialize the DMA2D background alpha 值 */
    DMA2D_BG_InitStruct->DMA2D_BGPFC_ALPHA_VALUE = 0x00;

    /*!< Initialize the DMA2D background blue 值 */
    DMA2D_BG_InitStruct->DMA2D_BGC_BLUE = 0x00;

    /*!< Initialize the DMA2D background green 值 */
    DMA2D_BG_InitStruct->DMA2D_BGC_GREEN = 0x00;

    /*!< Initialize the DMA2D background red 值 */
    DMA2D_BG_InitStruct->DMA2D_BGC_RED = 0x00;

    /*!< Initialize the DMA2D background CLUT memory address */
    DMA2D_BG_InitStruct->DMA2D_BGCMAR = 0x00;
}

/**
  * 简介:  开始自动加载 CLUT 或中止传输。
  * 参数:  NewState: DMA2D的新状态外设.
  *   此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */

void DMA2D_FGStart(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Start the automatic loading of the CLUT */
        DMA2D->FGPFCCR |= DMA2D_FGPFCCR_START;
    } else {
        /* abort the transfer */
        DMA2D->FGPFCCR &= (uint32_t)~DMA2D_FGPFCCR_START;
    }
}

/**
  * 简介:  启动CLUT的自动加载或中止传输。
  * 参数:  NewState: DMA2D的新状态外设.
  *   此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */

void DMA2D_BGStart(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Start the automatic loading of the CLUT */
        DMA2D->BGPFCCR |= DMA2D_BGPFCCR_START;
    } else {
        /* abort the transfer */
        DMA2D->BGPFCCR &= (uint32_t)~DMA2D_BGPFCCR_START;
    }
}

/**
  * 简介:  配置 DMA2D 死区时间。
  * 参数:  DMA2D_DeadTime: 指定DMA2D死区时间。
  *   此参数可以是以下值之一:
  * 返回值: 无
  */
void DMA2D_DeadTimeConfig(uint32_t DMA2D_DeadTime, FunctionalState NewState) {
    uint32_t DeadTime;

    /* 检查参数 */
    assert_param(IS_DMA2D_DEAD_TIME(DMA2D_DeadTime));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* 使能and Configures the dead time */
        DMA2D->AMTCR &= (uint32_t)DEAD_MASK;
        DeadTime = DMA2D_DeadTime << 8;
        DMA2D->AMTCR |= (DeadTime | DMA2D_AMTCR_EN);
    } else {
        DMA2D->AMTCR &= ~(uint32_t)DMA2D_AMTCR_EN;
    }
}

/**
  * 简介:  定义线路水印的配置。
  * 参数:  DMA2D_LWatermarkConfig: 线条水印配置。
  * 返回值: 无
  */

void DMA2D_LineWatermarkConfig(uint32_t DMA2D_LWatermarkConfig) {
    /* 检查参数 */
    assert_param(IS_DMA2D_LineWatermark(DMA2D_LWatermarkConfig));

    /* Sets the Line watermark 配置*/
    DMA2D->LWR = (uint32_t)DMA2D_LWatermarkConfig;
}

/**
  * @}
  */

/** @defgroup DMA2D_Group2 中断和标记管理函数
 *  简介   中断和标记管理函数
 *
@verbatim
 ===============================================================================
            ##### 中断和标记管理函数 #####
 ===============================================================================

    [..] 本节提供的功能允许配置DMA2D中断并获取状态、清除标志和中断挂起位。
    [..] DMA2D提供6个中断源和6个标志

    *** 标志 ***
    =============
    [..]
        (+)DMA2D_FLAG_CE:配置错误中断标志
        (+)DMA2D\u FLAG\u CAE:CLUT访问错误中断标志
        (+)DMA2D_FLAG_TW:传输水印中断标志
        (+)DMA2D_FLAG_TC:传输完成中断标志
        (+)DMA2D_FLAG_TE:传输错误中断标志
        (+)DMA2D_FLAG_CTC:CLUT传输完成中断标志

    *** 中断 ***
    ==================
    [..]
        (+)DMA2D_IT_CE:错误时生成配置错误中断检测到配置
        (+)DMA2D\u IT\u CAE:CLUT访问错误中断
        (+)DMA2D_IT_TW:传输水印中断在以下情况下生成达到编程水印
        (+)DMA2D_IT_TE:CPU尝试时生成传输错误中断在CLUT加载或DMA2D1传输时访问CLUT正在进行中
        (+)DMA2D_IT_CTC:CLUT传输完成中断
        (+)DMA2D_IT_TC:传输完成中断
@endverbatim
  * @{
  */
/**
  * 简介:  启用或禁用指定的 DMA2D's 中断
  * 参数:  DMA2D_IT: 指定要启用或禁用的DMA2D中断源。
  *   此参数可以是以下值的任意组合:
  *     @arg DMA2D_IT_CE:配置错误中断启用。
  *     @arg DMA2D_IT_CTC:CLUT传输完成中断启用。
  *     @arg DMA2D_IT_CAE:CLUT访问错误中断启用。
  *     @arg DMA2D_IT_TW:传输水印中断启用。
  *     @arg DMA2D_IT_TC:传输完成中断启用。
  *     @arg DMA2D_IT_TE:传输错误中断启用。
  * 参数: NewState: 指定DMA2D中断的新状态。
  *   此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */

void DMA2D_ITConfig(uint32_t DMA2D_IT, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_DMA2D_IT(DMA2D_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* 启用 selected DMA2D interrupts */
        DMA2D->CR |= DMA2D_IT;
    } else {
        /* 禁用 selected DMA2D interrupts */
        DMA2D->CR &= (uint32_t)~DMA2D_IT;
    }
}

/**
  * 简介:  检查是否设置了指定的 DMA2D 标志。
  * 参数:  DMA2D_FLAG: 指定要检查的标志。
  *   此参数可以是以下值之一:
  *     @arg DMA2D_FLAG_CE:配置错误中断标志。
  *     @arg DMA2D_FLAG_CTC:CLUT传输完成中断标志。
  *     @arg DMA2D_FLAG_CAE:CLUT访问错误中断标志。
  *     @arg DMA2D_FLAG_TW:传输水印中断标志。
  *     @arg DMA2D_FLAG_TC:传输完成中断标志。
  *     @arg DMA2D_FLAG_TE:传输错误中断标志。
  * 返回值: DMA2D_FLAG的新状态(SET或RESET)。
  */

FlagStatus DMA2D_GetFlagStatus(uint32_t DMA2D_FLAG) {
    FlagStatus bitstatus = RESET;

    /* 检查参数 */
    assert_param(IS_DMA2D_GET_FLAG(DMA2D_FLAG));

    /* 检查 the status of the specified DMA2D flag */
    if (((DMA2D->ISR) & DMA2D_FLAG) != (uint32_t)RESET) {
        /* DMA2D_FLAG 被设置 */
        bitstatus = SET;
    } else {
        /* DMA2D_FLAG  被重置 */
        bitstatus = RESET;
    }

    /* 返回DMA2D_FLAG 状态 */
    return bitstatus;
}

/**
  * 简介:  清除 DMA2D 的挂起标志。
  * 参数:  DMA2D_FLAG: 指定要清除的标志。
  *   此参数可以是以下值的任意组合:
  *     @arg DMA2D_FLAG_CE:配置错误中断标志。
  *     @arg DMA2D_FLAG_CTC:CLUT传输完成中断标志。
  *     @arg DMA2D\u FLAG\u CAE:CLUT访问错误中断标志。
  *     @arg DMA2D_FLAG_TW:传输水印中断标志。
  *     @arg DMA2D_FLAG_TC:传输完成中断标志。
  *     @arg DMA2D_FLAG_TE:传输错误中断标志。
  * 返回值: 无
  */
void DMA2D_ClearFlag(uint32_t DMA2D_FLAG) {
    /* 检查参数 */
    assert_param(IS_DMA2D_GET_FLAG(DMA2D_FLAG));

    /* 清除 corresponding DMA2D flag */
    DMA2D->IFCR = (uint32_t)DMA2D_FLAG;
}

/**
  * 简介:  检查指定的 DMA2D 的中断是否发生。
  * 参数:  DMA2D_IT: 指定要检查的DMA2D中断源。
  *   此参数可以是以下值之一:
  *     @arg DMA2D_IT_CE:配置错误中断启用。
  *     @arg DMA2D_IT_CTC:CLUT传输完成中断启用。
  *     @arg DMA2D_IT_CAE:CLUT访问错误中断启用。
  *     @arg DMA2D_IT_TW:传输水印中断启用。
  *     @arg DMA2D_IT_TC:传输完成中断启用。
  *     @arg DMA2D_IT_TE:传输错误中断启用。
  * 返回值: DMA2D_IT的新状态(SET或RESET)。
  */
ITStatus DMA2D_GetITStatus(uint32_t DMA2D_IT) {
    ITStatus bitstatus = RESET;
    uint32_t DMA2D_IT_FLAG = DMA2D_IT >> 8;

    /* 检查参数 */
    assert_param(IS_DMA2D_IT(DMA2D_IT));

    if ((DMA2D->ISR & DMA2D_IT_FLAG) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    if (((DMA2D->CR & DMA2D_IT) != (uint32_t)RESET) && (bitstatus != (uint32_t)RESET)) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
  * 简介:  清除 DMA2D 的中断挂起位。
  * 参数:  DMA2D_IT: 指定要清除的中断等待位。
  *   此参数可以是以下值的任意组合:
  *     @arg DMA2D_IT_CE:配置错误中断。
  *     @arg DMA2D_IT_CTC:CLUT传输完成中断。
  *     @arg DMA2D_IT_CAE:CLUT访问错误中断。
  *     @arg DMA2D_IT_TW:传输水印中断。
  *     @arg DMA2D_IT_TC:传输完成中断。
  *     @arg DMA2D_IT_TE:传输错误中断。
  * 返回值: 无
  */
void DMA2D_ClearITPendingBit(uint32_t DMA2D_IT) {
    /* 检查参数 */
    assert_param(IS_DMA2D_IT(DMA2D_IT));
    DMA2D_IT = DMA2D_IT >> 8;

    /* 清除 corresponding DMA2D Interrupt */
    DMA2D->IFCR = (uint32_t)DMA2D_IT;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
