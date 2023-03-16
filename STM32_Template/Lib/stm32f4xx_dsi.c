/**
  ******************************************************************************
  * 文件:    stm32f4xx_dsi.c
  * 作者:    MCD Application Team
  * 版本:    V1.8.0
  * 日期:    04-November-2016
  * 简介:    此文件提供固件功能，用于管理显示串行接口(DSI)的以下功能:
  *           + 初始化和配置
  *           + 数据传输管理功能
  *           + 低功耗功能
  *           + 中断和标志管理
  *
@verbatim

 ===================================================================
                  ##### 如何使用这个驱动程序 #####
 ===================================================================
 [..]

@endverbatim
  *
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
#include "stm32f4xx_dsi.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */
/** @addtogroup DSI
  * 简介: DSI 驱动模块
  * @{
  */
#if defined(STM32F469_479xx)

/* Private types -------------------------------------------------------------*/
/* 私有宏s -----------------------------------------------------------*/
/** @addtogroup DSI_Private_Constants
  * @{
  */
#define DSI_TIMEOUT_VALUE ((uint32_t)1000)  /* 1s */

#define DSI_ERROR_ACK_MASK (DSI_ISR0_AE0 | DSI_ISR0_AE1 | DSI_ISR0_AE2 | DSI_ISR0_AE3 | \
                            DSI_ISR0_AE4 | DSI_ISR0_AE5 | DSI_ISR0_AE6 | DSI_ISR0_AE7 | \
                            DSI_ISR0_AE8 | DSI_ISR0_AE9 | DSI_ISR0_AE10 | DSI_ISR0_AE11 | \
                            DSI_ISR0_AE12 | DSI_ISR0_AE13 | DSI_ISR0_AE14 | DSI_ISR0_AE15)
#define DSI_ERROR_PHY_MASK (DSI_ISR0_PE0 | DSI_ISR0_PE1 | DSI_ISR0_PE2 | DSI_ISR0_PE3 | DSI_ISR0_PE4)
#define DSI_ERROR_TX_MASK  DSI_ISR1_TOHSTX
#define DSI_ERROR_RX_MASK  DSI_ISR1_TOLPRX
#define DSI_ERROR_ECC_MASK (DSI_ISR1_ECCSE | DSI_ISR1_ECCME)
#define DSI_ERROR_CRC_MASK DSI_ISR1_CRCE
#define DSI_ERROR_PSE_MASK DSI_ISR1_PSE
#define DSI_ERROR_EOT_MASK DSI_ISR1_EOTPE
#define DSI_ERROR_OVF_MASK DSI_ISR1_LPWRE
#define DSI_ERROR_GEN_MASK (DSI_ISR1_GCWRE | DSI_ISR1_GPWRE | DSI_ISR1_GPTXE | DSI_ISR1_GPRDE | DSI_ISR1_GPRXE)

#define DSI_MAX_RETURN_PKT_SIZE ((uint32_t)0x00000037) /*!< Maximum return packet 配置*/
/**
  * @}
  */

/* 私有变量 ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/* 私有宏s ------------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------*/
static void DSI_ConfigPacketHeader(DSI_TypeDef *DSIx, uint32_t ChannelID, uint32_t DataType, uint32_t Data0, uint32_t Data1);
/* 私有函数 ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/** @addtogroup DSI_Exported_Functions
  * @{
  */

/** @defgroup DSI_Group1 初始化和配置函数
 *  简介   初始化和配置函数
 *
@verbatim
 ===============================================================================
                ##### 初始化和配置函数 #####
 ===============================================================================
    [..]  本节提供的功能允许:
      (+) 初始化并配置DSI
      (+) 解除对DSI的初始化

@endverbatim
  * @{
  */

/**
  * 简介:  将 DSI 外设寄存器取消初始化为其默认复位值。
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 返回值: 无
  */
void DSI_DeInit(DSI_TypeDef *DSIx) {
    /* 禁用 DSI wrapper */
    DSIx->WCR &= ~DSI_WCR_DSIEN;

    /* 禁用 DSI host */
    DSIx->CR &= ~DSI_CR_EN;

    /* D-PHY clock and digital disable */
    DSIx->PCTLR &= ~(DSI_PCTLR_CKE | DSI_PCTLR_DEN);

    /* Turn off the DSI PLL */
    DSIx->WRPCR &= ~DSI_WRPCR_PLLEN;

    /* 禁用 regulator */
    DSIx->WRPCR &= ~DSI_WRPCR_REGEN;

    /* 检查参数 */
    assert_param(IS_DSI_ALL_PERIPH(DSIx));

    if(DSIx == DSI) {
        /* Enable DSI reset state */
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_DSI, ENABLE);
        /* 释放DSI from reset state */
        RCC_APB2PeriphResetCmd(RCC_APB2Periph_DSI, DISABLE);
    }
}

/**
  * 简介: 将 DSIx 外设寄存器取消初始化为其默认复位值。
  * 参数: DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数: DSI_InitStruct: 指向DSI_InitTypeDef结构的指针。
  *                       该结构包含指定DSI外设的配置信息。
  * 参数: DSI_InitTIMStruct: 指向DSI_TIMTypeDef结构的指针，
  *                          该结构包含指定DSI定时器的配置信息。
  * 返回值: 无
  */
void DSI_Init(DSI_TypeDef *DSIx, DSI_InitTypeDef* DSI_InitStruct, DSI_PLLInitTypeDef *PLLInit) {
    uint32_t unitIntervalx4 = 0;
    uint32_t tempIDF = 0;

    /* 检查函数参数 */
    assert_param(IS_DSI_PLL_NDIV(PLLInit->PLLNDIV));
    assert_param(IS_DSI_PLL_IDF(PLLInit->PLLIDF));
    assert_param(IS_DSI_PLL_ODF(PLLInit->PLLODF));
    assert_param(IS_DSI_AUTO_CLKLANE_CONTROL(DSI_InitStruct->AutomaticClockLaneControl));
    assert_param(IS_DSI_NUMBER_OF_LANES(DSI_InitStruct->NumberOfLanes));

    /**************** Turn on the regulator and enable the DSI PLL ****************/

    /* 启用 regulator */
    DSIx->WRPCR |= DSI_WRPCR_REGEN;

    /* Wait until the regulator is ready */
    while(DSI_GetFlagStatus(DSIx, DSI_FLAG_RRS) == RESET )
    {}

    /* 设置 PLL division factors */
    DSIx->WRPCR &= ~(DSI_WRPCR_PLL_NDIV | DSI_WRPCR_PLL_IDF | DSI_WRPCR_PLL_ODF);
    DSIx->WRPCR |= (((PLLInit->PLLNDIV) << 2) | ((PLLInit->PLLIDF) << 11) | ((PLLInit->PLLODF) << 16));

    /* 启用 DSI PLL */
    DSIx->WRPCR |= DSI_WRPCR_PLLEN;

    /* Wait for the lock of the PLL */
    while(DSI_GetFlagStatus(DSIx, DSI_FLAG_PLLLS) == RESET)
    {}

    /*************************** Set the PHY parameters ***************************/

    /* D-PHY clock and digital enable*/
    DSIx->PCTLR |= (DSI_PCTLR_CKE | DSI_PCTLR_DEN);

    /* Clock lane 配置*/
    DSIx->CLCR &= ~(DSI_CLCR_DPCC | DSI_CLCR_ACR);
    DSIx->CLCR |= (DSI_CLCR_DPCC | DSI_InitStruct->AutomaticClockLaneControl);

    /* 配置 number of active data lanes */
    DSIx->PCONFR &= ~DSI_PCONFR_NL;
    DSIx->PCONFR |= DSI_InitStruct->NumberOfLanes;

    /************************ Set the DSI clock parameters ************************/
    /* 设置 TX escape clock division factor */
    DSIx->CCR &= ~DSI_CCR_TXECKDIV;
    DSIx->CCR = DSI_InitStruct->TXEscapeCkdiv;

    /* Calculate the bit period in high-speed mode in unit of 0.25 ns (UIX4) */
    /* The equation is : UIX4 = IntegerPart( (1000/F_PHY_Mhz) * 4 )          */
    /* Where : F_PHY_Mhz = (NDIV * HSE_Mhz) / (IDF * ODF)                    */
    tempIDF = (PLLInit->PLLIDF > 0) ? PLLInit->PLLIDF : 1;
    unitIntervalx4 = (4000000 * tempIDF * (1 << PLLInit->PLLODF)) / ((HSE_VALUE / 1000) * PLLInit->PLLNDIV);

    /* 设置 bit period in high-speed mode */
    DSIx->WPCR[0] &= ~DSI_WPCR0_UIX4;
    DSIx->WPCR[0] |= unitIntervalx4;

    /****************************** Error management *****************************/
    /* Disable all error interrupts */
    DSIx->IER[0] = 0;
    DSIx->IER[1] = 0;
}

/**
  * 简介: 用默认值填充每个 DSI_InitStruct 成员。
  * 参数: DSI_InitStruct: 指向DSI_InitTypeDef结构的指针，该结构将被初始化。
  * 返回值: 无
  */
void DSI_StructInit(DSI_InitTypeDef* DSI_InitStruct, DSI_HOST_TimeoutTypeDef* DSI_HOST_TimeoutInitStruct) {
    /*--------------- Reset DSI init structure parameters values ---------------*/
    /* 初始化 AutomaticClockLaneControl 成员 */
    DSI_InitStruct->AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
    /* 初始化 NumberOfLanes 成员 */
    DSI_InitStruct->NumberOfLanes = DSI_ONE_DATA_LANE;
    /* Initialize  the TX Escape clock division */
    DSI_InitStruct->TXEscapeCkdiv = 0;

    /*--------------- Reset DSI timings init structure parameters values -------*/
    /* 初始化 TimeoutCkdiv 成员 */
    DSI_HOST_TimeoutInitStruct->TimeoutCkdiv = 0;
    /* 初始化 HighSpeedTransmissionTimeout 成员 */
    DSI_HOST_TimeoutInitStruct->HighSpeedTransmissionTimeout = 0;
    /* 初始化 LowPowerReceptionTimeout 成员 */
    DSI_HOST_TimeoutInitStruct->LowPowerReceptionTimeout = 0;
    /* 初始化 HighSpeedReadTimeout 成员 */
    DSI_HOST_TimeoutInitStruct->HighSpeedReadTimeout = 0;
    /* 初始化 LowPowerReadTimeout 成员 */
    DSI_HOST_TimeoutInitStruct->LowPowerReadTimeout = 0;
    /* 初始化 HighSpeedWriteTimeout 成员 */
    DSI_HOST_TimeoutInitStruct->HighSpeedWriteTimeout = 0;
    /* 初始化 HighSpeedWritePrespMode 成员 */
    DSI_HOST_TimeoutInitStruct->HighSpeedWritePrespMode = 0;
    /* 初始化 LowPowerWriteTimeout 成员 */
    DSI_HOST_TimeoutInitStruct->LowPowerWriteTimeout = 0;
    /* 初始化 BTATimeout 成员 */
    DSI_HOST_TimeoutInitStruct->BTATimeout = 0;
}

/**
  * 简介:  配置通用接口回读的虚拟通道ID。
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  VirtualChannelID: 虚拟通道ID
  * 返回值: 无
  */
void DSI_SetGenericVCID(DSI_TypeDef *DSIx, uint32_t VirtualChannelID) {
    /* Update the GVCID 寄存器 */
    DSIx->GVCIDR &= ~DSI_GVCIDR_VCID;
    DSIx->GVCIDR |= VirtualChannelID;
}

/**
  * 简介:  选择视频模式并配置相应的参数
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  VidCfg: 指向一个包含DSI视频模式配置参数的DSI_VidCfgTypeDef结构的指针。
  * 返回值: 无
  */
void DSI_ConfigVideoMode(DSI_TypeDef *DSIx, DSI_VidCfgTypeDef *VidCfg) {
    /* 检查参数 */
    assert_param(IS_DSI_COLOR_CODING(VidCfg->ColorCoding));
    assert_param(IS_DSI_VIDEO_MODE_TYPE(VidCfg->Mode));
    assert_param(IS_DSI_LP_COMMAND(VidCfg->LPCommandEnable));
    assert_param(IS_DSI_LP_HFP(VidCfg->LPHorizontalFrontPorchEnable));
    assert_param(IS_DSI_LP_HBP(VidCfg->LPHorizontalBackPorchEnable));
    assert_param(IS_DSI_LP_VACTIVE(VidCfg->LPVerticalActiveEnable));
    assert_param(IS_DSI_LP_VFP(VidCfg->LPVerticalFrontPorchEnable));
    assert_param(IS_DSI_LP_VBP(VidCfg->LPVerticalBackPorchEnable));
    assert_param(IS_DSI_LP_VSYNC(VidCfg->LPVerticalSyncActiveEnable));
    assert_param(IS_DSI_FBTAA(VidCfg->FrameBTAAcknowledgeEnable));
    assert_param(IS_DSI_DE_POLARITY(VidCfg->DEPolarity));
    assert_param(IS_DSI_VSYNC_POLARITY(VidCfg->VSPolarity));
    assert_param(IS_DSI_HSYNC_POLARITY(VidCfg->HSPolarity));

    /* 检查 the LooselyPacked variant only in 18-bit mode */
    if(VidCfg->ColorCoding == DSI_RGB666) {
        assert_param(IS_DSI_LOOSELY_PACKED(VidCfg->LooselyPacked));
    }

    /* Select video mode by resetting CMDM and DSIM 位 */
    DSIx->MCR &= ~DSI_MCR_CMDM;
    DSIx->WCFGR &= ~DSI_WCFGR_DSIM;

    /* 配置 video mode transmission type */
    DSIx->VMCR &= ~DSI_VMCR_VMT;
    DSIx->VMCR |= VidCfg->Mode;

    /* 配置 video packet size */
    DSIx->VPCR &= ~DSI_VPCR_VPSIZE;
    DSIx->VPCR |= VidCfg->PacketSize;

    /* 设置 chunks number to be transmitted through the DSI link */
    DSIx->VCCR &= ~DSI_VCCR_NUMC;
    DSIx->VCCR |= VidCfg->NumberOfChunks;

    /* 设置 size of the null packet */
    DSIx->VNPCR &= ~DSI_VNPCR_NPSIZE;
    DSIx->VNPCR |= VidCfg->NullPacketSize;

    /* 选择这个virtual channel for the LTDC interface traffic */
    DSIx->LVCIDR &= ~DSI_LVCIDR_VCID;
    DSIx->LVCIDR |= VidCfg->VirtualChannelID;

    /* 配置 polarity of control signals */
    DSIx->LPCR &= ~(DSI_LPCR_DEP | DSI_LPCR_VSP | DSI_LPCR_HSP);
    DSIx->LPCR |= (VidCfg->DEPolarity | VidCfg->VSPolarity | VidCfg->HSPolarity);

    /* 选择这个color coding for the host */
    DSIx->LCOLCR &= ~DSI_LCOLCR_COLC;
    DSIx->LCOLCR |= VidCfg->ColorCoding;

    /* 选择这个color coding for the wrapper */
    DSIx->WCFGR &= ~DSI_WCFGR_COLMUX;
    DSIx->WCFGR |= ((VidCfg->ColorCoding) << 1);

    /* Enable/disable the loosely packed variant to 18-bit 配置*/
    if(VidCfg->ColorCoding == DSI_RGB666) {
        DSIx->LCOLCR &= ~DSI_LCOLCR_LPE;
        DSIx->LCOLCR |= VidCfg->LooselyPacked;
    }

    /* 设置 Horizontal Synchronization Active (HSA) in lane byte clock cycles */
    DSIx->VHSACR &= ~DSI_VHSACR_HSA;
    DSIx->VHSACR |= VidCfg->HorizontalSyncActive;

    /* 设置 Horizontal Back Porch (HBP) in lane byte clock cycles */
    DSIx->VHBPCR &= ~DSI_VHBPCR_HBP;
    DSIx->VHBPCR |= VidCfg->HorizontalBackPorch;

    /* 设置 total line time (HLINE=HSA+HBP+HACT+HFP) in lane byte clock cycles */
    DSIx->VLCR &= ~DSI_VLCR_HLINE;
    DSIx->VLCR |= VidCfg->HorizontalLine;

    /* 设置 Vertical Synchronization Active (VSA) */
    DSIx->VVSACR &= ~DSI_VVSACR_VSA;
    DSIx->VVSACR |= VidCfg->VerticalSyncActive;

    /* 设置 Vertical Back Porch (VBP)*/
    DSIx->VVBPCR &= ~DSI_VVBPCR_VBP;
    DSIx->VVBPCR |= VidCfg->VerticalBackPorch;

    /* 设置 Vertical Front Porch (VFP)*/
    DSIx->VVFPCR &= ~DSI_VVFPCR_VFP;
    DSIx->VVFPCR |= VidCfg->VerticalFrontPorch;

    /* 设置 Vertical Active period*/
    DSIx->VVACR &= ~DSI_VVACR_VA;
    DSIx->VVACR |= VidCfg->VerticalActive;

    /* 配置 command transmission mode */
    DSIx->VMCR &= ~DSI_VMCR_LPCE;
    DSIx->VMCR |= VidCfg->LPCommandEnable;

    /* Low power largest packet size */
    DSIx->LPMCR &= ~DSI_LPMCR_LPSIZE;
    DSIx->LPMCR |= ((VidCfg->LPLargestPacketSize) << 16);

    /* Low power VACT largest packet size */
    DSIx->LPMCR &= ~DSI_LPMCR_VLPSIZE;
    DSIx->LPMCR |= VidCfg->LPVACTLargestPacketSize;

    /* Enable LP transition in HFP period */
    DSIx->VMCR &= ~DSI_VMCR_LPHFPE;
    DSIx->VMCR |= VidCfg->LPHorizontalFrontPorchEnable;

    /* Enable LP transition in HBP period */
    DSIx->VMCR &= ~DSI_VMCR_LPHBPE;
    DSIx->VMCR |= VidCfg->LPHorizontalBackPorchEnable;

    /* Enable LP transition in VACT period */
    DSIx->VMCR &= ~DSI_VMCR_LPVAE;
    DSIx->VMCR |= VidCfg->LPVerticalActiveEnable;

    /* Enable LP transition in VFP period */
    DSIx->VMCR &= ~DSI_VMCR_LPVFPE;
    DSIx->VMCR |= VidCfg->LPVerticalFrontPorchEnable;

    /* Enable LP transition in VBP period */
    DSIx->VMCR &= ~DSI_VMCR_LPVBPE;
    DSIx->VMCR |= VidCfg->LPVerticalBackPorchEnable;

    /* Enable LP transition in vertical sync period */
    DSIx->VMCR &= ~DSI_VMCR_LPVSAE;
    DSIx->VMCR |= VidCfg->LPVerticalSyncActiveEnable;

    /* 启用 request for an acknowledge response at the end of a frame */
    DSIx->VMCR &= ~DSI_VMCR_FBTAAE;
    DSIx->VMCR |= VidCfg->FrameBTAAcknowledgeEnable;
}

/**
  * 简介:  选择适应的命令模式并配置相应的参数
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  CmdCfg: 指向一个包含DSI命令模式配置参数的DSI_CmdCfgTypeDef结构的指针。
  * 返回值: 无
  */
void DSI_ConfigAdaptedCommandMode(DSI_TypeDef *DSIx, DSI_CmdCfgTypeDef *CmdCfg) {
    /* 检查参数 */
    assert_param(IS_DSI_COLOR_CODING(CmdCfg->ColorCoding));
    assert_param(IS_DSI_TE_SOURCE(CmdCfg->TearingEffectSource));
    assert_param(IS_DSI_TE_POLARITY(CmdCfg->TearingEffectPolarity));
    assert_param(IS_DSI_AUTOMATIC_REFRESH(CmdCfg->AutomaticRefresh));
    assert_param(IS_DSI_VS_POLARITY(CmdCfg->VSyncPol));
    assert_param(IS_DSI_TE_ACK_REQUEST(CmdCfg->TEAcknowledgeRequest));
    assert_param(IS_DSI_DE_POLARITY(CmdCfg->DEPolarity));
    assert_param(IS_DSI_VSYNC_POLARITY(CmdCfg->VSPolarity));
    assert_param(IS_DSI_HSYNC_POLARITY(CmdCfg->HSPolarity));

    /* Select command mode by setting CMDM and DSIM 位 */
    DSIx->MCR |= DSI_MCR_CMDM;
    DSIx->WCFGR &= ~DSI_WCFGR_DSIM;
    DSIx->WCFGR |= DSI_WCFGR_DSIM;

    /* 选择这个virtual channel for the LTDC interface traffic */
    DSIx->LVCIDR &= ~DSI_LVCIDR_VCID;
    DSIx->LVCIDR |= CmdCfg->VirtualChannelID;

    /* 配置 polarity of control signals */
    DSIx->LPCR &= ~(DSI_LPCR_DEP | DSI_LPCR_VSP | DSI_LPCR_HSP);
    DSIx->LPCR |= (CmdCfg->DEPolarity | CmdCfg->VSPolarity | CmdCfg->HSPolarity);

    /* 选择这个color coding for the host */
    DSIx->LCOLCR &= ~DSI_LCOLCR_COLC;
    DSIx->LCOLCR |= CmdCfg->ColorCoding;

    /* 选择这个color coding for the wrapper */
    DSIx->WCFGR &= ~DSI_WCFGR_COLMUX;
    DSIx->WCFGR |= ((CmdCfg->ColorCoding) << 1);

    /* 配置 maximum allowed size for write memory command */
    DSIx->LCCR &= ~DSI_LCCR_CMDSIZE;
    DSIx->LCCR |= CmdCfg->CommandSize;

    /* 配置 tearing effect source and polarity and select the refresh mode */
    DSIx->WCFGR &= ~(DSI_WCFGR_TESRC | DSI_WCFGR_TEPOL | DSI_WCFGR_AR | DSI_WCFGR_VSPOL);
    DSIx->WCFGR |= (CmdCfg->TearingEffectSource | CmdCfg->TearingEffectPolarity | CmdCfg->AutomaticRefresh | CmdCfg->VSyncPol);

    /* 配置 tearing effect acknowledge request */
    DSIx->CMCR &= ~DSI_CMCR_TEARE;
    DSIx->CMCR |= CmdCfg->TEAcknowledgeRequest;

    /* 启用 Tearing Effect interrupt */
    DSI_ITConfig(DSIx, DSI_IT_TE, ENABLE);
    /* 启用 End of Refresh interrupt */
    DSI_ITConfig(DSIx, DSI_IT_ER, ENABLE);
}

/**
  * 简介:  配置命令传输模式:高速或低功率，并在数据包传输后启用/禁用确认请求
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  LPCmd: 指向包含DSI命令传输模式配置参数的DSI_LPCmdTypeDef结构的指针
  * 返回值: 无
  */
void DSI_ConfigCommand(DSI_TypeDef *DSIx, DSI_LPCmdTypeDef *LPCmd) {
    assert_param(IS_DSI_LP_GSW0P(LPCmd->LPGenShortWriteNoP));
    assert_param(IS_DSI_LP_GSW1P(LPCmd->LPGenShortWriteOneP));
    assert_param(IS_DSI_LP_GSW2P(LPCmd->LPGenShortWriteTwoP));
    assert_param(IS_DSI_LP_GSR0P(LPCmd->LPGenShortReadNoP));
    assert_param(IS_DSI_LP_GSR1P(LPCmd->LPGenShortReadOneP));
    assert_param(IS_DSI_LP_GSR2P(LPCmd->LPGenShortReadTwoP));
    assert_param(IS_DSI_LP_GLW(LPCmd->LPGenLongWrite));
    assert_param(IS_DSI_LP_DSW0P(LPCmd->LPDcsShortWriteNoP));
    assert_param(IS_DSI_LP_DSW1P(LPCmd->LPDcsShortWriteOneP));
    assert_param(IS_DSI_LP_DSR0P(LPCmd->LPDcsShortReadNoP));
    assert_param(IS_DSI_LP_DLW(LPCmd->LPDcsLongWrite));
    assert_param(IS_DSI_LP_MRDP(LPCmd->LPMaxReadPacket));
    assert_param(IS_DSI_ACK_REQUEST(LPCmd->AcknowledgeRequest));

    /* Select High-speed or Low-power for command transmission */
    DSIx->CMCR &= ~(DSI_CMCR_GSW0TX | \
                    DSI_CMCR_GSW1TX | \
                    DSI_CMCR_GSW2TX | \
                    DSI_CMCR_GSR0TX | \
                    DSI_CMCR_GSR1TX | \
                    DSI_CMCR_GSR2TX | \
                    DSI_CMCR_GLWTX  | \
                    DSI_CMCR_DSW0TX | \
                    DSI_CMCR_DSW1TX | \
                    DSI_CMCR_DSR0TX | \
                    DSI_CMCR_DLWTX  | \
                    DSI_CMCR_MRDPS);
    DSIx->CMCR |= (LPCmd->LPGenShortWriteNoP  | \
                   LPCmd->LPGenShortWriteOneP | \
                   LPCmd->LPGenShortWriteTwoP | \
                   LPCmd->LPGenShortReadNoP   | \
                   LPCmd->LPGenShortReadOneP  | \
                   LPCmd->LPGenShortReadTwoP  | \
                   LPCmd->LPGenLongWrite      | \
                   LPCmd->LPDcsShortWriteNoP  | \
                   LPCmd->LPDcsShortWriteOneP | \
                   LPCmd->LPDcsShortReadNoP   | \
                   LPCmd->LPDcsLongWrite      | \
                   LPCmd->LPMaxReadPacket);

    /* 配置 acknowledge request after each packet transmission */
    DSIx->CMCR &= ~DSI_CMCR_ARE;
    DSIx->CMCR |= LPCmd->AcknowledgeRequest;
}

/**
  * 简介:  配置流量控制参数
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  FlowControl: 要启用的流量控制功能。
  *                      此参数可以是@ref DSI_FlowControl的任意组合。
  * 返回值: 无
  */
void DSI_ConfigFlowControl(DSI_TypeDef *DSIx, uint32_t FlowControl) {
    /* 检查参数 */
    assert_param(IS_DSI_FLOW_CONTROL(FlowControl));

    /* 设置 DSI Host Protocol Configuration 寄存器 */
    DSIx->PCR &= ~DSI_FLOW_CONTROL_ALL;
    DSIx->PCR |= FlowControl;
}

/**
  * 简介:  配置DSI PHY计时器参数
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  PhyTimers: 包含DSI PHY计时参数的DSI_PHY_TimerTypeDef结构
  * 返回值: 无
  */
void DSI_ConfigPhyTimer(DSI_TypeDef *DSIx, DSI_PHY_TimerTypeDef *PhyTimers) {
    uint32_t maxTime = 0;

    maxTime = (PhyTimers->ClockLaneLP2HSTime > PhyTimers->ClockLaneHS2LPTime) ? PhyTimers->ClockLaneLP2HSTime : PhyTimers->ClockLaneHS2LPTime;

    /* Clock lane timer 配置*/
    /* In Automatic Clock Lane control mode, the DSI Host can turn off the clock lane between two
       High-Speed transmission.
       To do so, the DSI Host calculates the time required for the clock lane to change from HighSpeed
       to Low-Power and from Low-Power to High-Speed.
       This timings are configured by the HS2LP_TIME and LP2HS_TIME in the DSI Host Clock Lane Timer Configuration Register (DSI_CLTCR).
       But the DSI Host is not calculating LP2HS_TIME + HS2LP_TIME but 2 x HS2LP_TIME.

       Workaround : Configure HS2LP_TIME and LP2HS_TIME with the same value being the max of HS2LP_TIME or LP2HS_TIME.
    */
    DSIx->CLTCR &= ~(DSI_CLTCR_LP2HS_TIME | DSI_CLTCR_HS2LP_TIME);
    DSIx->CLTCR |= (maxTime | ((maxTime) << 16));

    /* Data lane timer 配置*/
    DSIx->DLTCR &= ~(DSI_DLTCR_MRD_TIME | DSI_DLTCR_LP2HS_TIME | DSI_DLTCR_HS2LP_TIME);
    DSIx->DLTCR |= (PhyTimers->DataLaneMaxReadTime | ((PhyTimers->DataLaneLP2HSTime) << 16) | ((PhyTimers->DataLaneHS2LPTime) << 24));

    /* 配置 wait period to request HS transmission after a stop state */
    DSIx->PCONFR &= ~DSI_PCONFR_SW_TIME;
    DSIx->PCONFR |= ((PhyTimers->StopWaitTime) << 8);
}

/**
  * 简介:  配置DSI HOST超时参数
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  HostTimeouts: 包含DSI主机超时参数的DSI_HOST_TimeoutTypeDef结构
  * 返回值: 无
  */
void DSI_ConfigHostTimeouts(DSI_TypeDef *DSIx, DSI_HOST_TimeoutTypeDef *HostTimeouts) {
    /* 设置 timeout clock division factor */
    DSIx->CCR &= ~DSI_CCR_TOCKDIV;
    DSIx->CCR = ((HostTimeouts->TimeoutCkdiv) << 8);

    /* High-speed transmission timeout */
    DSIx->TCCR[0] &= ~DSI_TCCR0_HSTX_TOCNT;
    DSIx->TCCR[0] |= ((HostTimeouts->HighSpeedTransmissionTimeout) << 16);

    /* Low-power reception timeout */
    DSIx->TCCR[0] &= ~DSI_TCCR0_LPRX_TOCNT;
    DSIx->TCCR[0] |= HostTimeouts->LowPowerReceptionTimeout;

    /* High-speed read timeout */
    DSIx->TCCR[1] &= ~DSI_TCCR1_HSRD_TOCNT;
    DSIx->TCCR[1] |= HostTimeouts->HighSpeedReadTimeout;

    /* Low-power read timeout */
    DSIx->TCCR[2] &= ~DSI_TCCR2_LPRD_TOCNT;
    DSIx->TCCR[2] |= HostTimeouts->LowPowerReadTimeout;

    /* High-speed write timeout */
    DSIx->TCCR[3] &= ~DSI_TCCR3_HSWR_TOCNT;
    DSIx->TCCR[3] |= HostTimeouts->HighSpeedWriteTimeout;

    /* High-speed write presp mode */
    DSIx->TCCR[3] &= ~DSI_TCCR3_PM;
    DSIx->TCCR[3] |= HostTimeouts->HighSpeedWritePrespMode;

    /* Low-speed write timeout */
    DSIx->TCCR[4] &= ~DSI_TCCR4_LPWR_TOCNT;
    DSIx->TCCR[4] |= HostTimeouts->LowPowerWriteTimeout;

    /* BTA timeout */
    DSIx->TCCR[5] &= ~DSI_TCCR5_BTA_TOCNT;
    DSIx->TCCR[5] |= HostTimeouts->BTATimeout;
}

/**
  * 简介:  启动DSI模块
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  *               DSI的配置信息。
  * 返回值: 无
  */
void DSI_Start(DSI_TypeDef *DSIx) {
    /* 启用 DSI host */
    DSIx->CR |= DSI_CR_EN;
    /* 启用 DSI wrapper */
    DSIx->WCR |= DSI_WCR_DSIEN;
}

/**
  * 简介:  停止DSI模块
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 返回值: 无
  */
void DSI_Stop(DSI_TypeDef *DSIx) {
    /* 禁用 DSI host */
    DSIx->CR &= ~DSI_CR_EN;

    /* 禁用 DSI wrapper */
    DSIx->WCR &= ~DSI_WCR_DSIEN;
}

/**
  * 简介:  在命令模式下刷新显示
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  *               DSI的配置信息。
  * 返回值: 无
  */
void DSI_Refresh(DSI_TypeDef *DSIx) {
    /* Update the display */
    DSIx->WCR |= DSI_WCR_LTDCEN;
}

/**
  * 简介:  控制视频模式中的显示颜色模式
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  ColorMode: 颜色模式(全色或8色)。
  *                    此参数可以是@ref DSI_Color_Mode的任何值
  * 返回值: 无
  */
void DSI_ColorMode(DSI_TypeDef *DSIx, uint32_t ColorMode) {
    /* 检查参数 */
    assert_param(IS_DSI_COLOR_MODE(ColorMode));

    /* Update the display color mode */
    DSIx->WCR &= ~DSI_WCR_COLM;
    DSIx->WCR |= ColorMode;
}

/**
  * 简介:  控制视频模式下的显示器关闭
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  Shutdown: 关闭(显示器打开或显示器关闭)。
  *                   此参数可以是@ref DSI_ShutDown的任何值
  * 返回值: 无
  */
void DSI_Shutdown(DSI_TypeDef *DSIx, uint32_t Shutdown) {
    /* 检查参数 */
    assert_param(IS_DSI_SHUT_DOWN(Shutdown));

    /* Update the display Shutdown */
    DSIx->WCR &= ~DSI_WCR_SHTDN;
    DSIx->WCR |= Shutdown;
}

/**
  * @}
  */

/** @defgroup 数据传输管理功能
 *  简介    DSI 数据传输管理功能
 *
@verbatim
 ===============================================================================
                #####  数据传输管理功能  #####
 ===============================================================================
@endverbatim
  * @{
  */

/**
  * 简介:  DCS或通用短写命令
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  ChannelID: 虚拟通道ID.
  * 参数:  Mode: DSI短分组数据类型。
  *               此参数可以是@ref DSI_SHORT_WRITE_PKT_Data_Type.
  * 参数:  Param1: DSC命令或第一个通用参数。
  *                 此参数可以是@ref DSI_DCS_Command 或通用命令代码.
  * 参数:  Param2: DSC参数或第二个通用参数。
  * 返回值: 无
  */
void DSI_ShortWrite(DSI_TypeDef *DSIx,
                    uint32_t ChannelID,
                    uint32_t Mode,
                    uint32_t Param1,
                    uint32_t Param2) {
    /* 检查参数 */
    assert_param(IS_DSI_SHORT_WRITE_PACKET_TYPE(Mode));

    /* Wait for Command FIFO Empty */
    while((DSIx->GPSR & DSI_GPSR_CMDFE) == 0)
    {}

    /* 配置 packet to send a short DCS command with 0 or 1 parameter */
    DSI_ConfigPacketHeader(DSIx,
                           ChannelID,
                           Mode,
                           Param1,
                           Param2);
}

/**
  * 简介:  DCS或通用长写命令
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  ChannelID: 虚拟通道ID.
  * 参数:  Mode: DSI长数据包数据类型。
  *               此参数可以是@ref DSI_LONG_WRITE_PKT_Data_Type.
  * 参数:  NbParams: 参数的数量。
  * 参数:  Param1: DSC命令或第一个通用参数。
  *                 此参数可以是@ref DSI_DCS_Command 或通用命令代码
  * 参数:  ParametersTable: 指向参数值表的指针。
  * 返回值: 无
  */
void DSI_LongWrite(DSI_TypeDef *DSIx,
                   uint32_t ChannelID,
                   uint32_t Mode,
                   uint32_t NbParams,
                   uint32_t Param1,
                   uint8_t* ParametersTable) {
    uint32_t uicounter = 0;

    /* 检查参数 */
    assert_param(IS_DSI_LONG_WRITE_PACKET_TYPE(Mode));

    /* Wait for Command FIFO Empty */
    while((DSIx->GPSR & DSI_GPSR_CMDFE) == 0)
    {}

    /* 设置 DCS code hexadecimal on payload byte 1, and the other parameters on the write FIFO command*/
    while(uicounter < NbParams) {
        if(uicounter == 0x00) {
            DSIx->GPDR = (Param1 | \
                          ((uint32_t)(*(ParametersTable + uicounter)) << 8) | \
                          ((uint32_t)(*(ParametersTable + uicounter + 1)) << 16) | \
                          ((uint32_t)(*(ParametersTable + uicounter + 2)) << 24));
            uicounter += 3;
        } else {
            DSIx->GPDR = ((*(ParametersTable + uicounter)) | \
                          ((uint32_t)(*(ParametersTable + uicounter + 1)) << 8) | \
                          ((uint32_t)(*(ParametersTable + uicounter + 2)) << 16) | \
                          ((uint32_t)(*(ParametersTable + uicounter + 3)) << 24));
            uicounter += 4;
        }
    }

    /* 配置 packet to send a long DCS command */
    DSI_ConfigPacketHeader(DSIx,
                           ChannelID,
                           Mode,
                           ((NbParams + 1) & 0x00FF),
                           (((NbParams + 1) & 0xFF00) >> 8));
}

/**
  * 简介:  读取命令(DCS或通用)
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  ChannelNbr: 虚拟通道ID
  * 参数:  Array: 指向存储回读操作有效负载的缓冲区的指针。
  * 参数:  Size:要读取的数据大小(字节)。
  * 参数:  Mode:DSI读取数据包数据类型。
  *               此参数可以是@ref DSI_SHORT_READ_PKT_Data_Type.
  * 参数:  DCSCmd: DCS get/read command.
  * 参数:  ParametersTable: 指向参数值表的指针。
  * 返回值: 无
  */
void DSI_Read(DSI_TypeDef *DSIx,
              uint32_t ChannelNbr,
              uint8_t* Array,
              uint32_t Size,
              uint32_t Mode,
              uint32_t DCSCmd,
              uint8_t* ParametersTable) {

    /* 检查参数 */
    assert_param(IS_DSI_READ_PACKET_TYPE(Mode));

    if(Size > 2) {
        /* set max return packet size */
        DSI_ShortWrite(DSIx, ChannelNbr, DSI_MAX_RETURN_PKT_SIZE, ((Size) & 0xFF), (((Size) >> 8) & 0xFF));
    }

    /* 配置 packet to read command */
    if (Mode == DSI_DCS_SHORT_PKT_READ) {
        DSI_ConfigPacketHeader(DSIx, ChannelNbr, Mode, DCSCmd, 0);
    } else if (Mode == DSI_GEN_SHORT_PKT_READ_P0) {
        DSI_ConfigPacketHeader(DSIx, ChannelNbr, Mode, 0, 0);
    } else if (Mode == DSI_GEN_SHORT_PKT_READ_P1) {
        DSI_ConfigPacketHeader(DSIx, ChannelNbr, Mode, ParametersTable[0], 0);
    } else { /* DSI_GEN_SHORT_PKT_READ_P2 */
        DSI_ConfigPacketHeader(DSIx, ChannelNbr, Mode, ParametersTable[0], ParametersTable[1]);
    }

    /* 检查 that the payload read FIFO is not empty */
    while((DSIx->GPSR & DSI_GPSR_PRDFE) == DSI_GPSR_PRDFE)
    {}

    /* 获取first byte */
    *((uint32_t *)Array) = (DSIx->GPDR);

    if (Size > 4) {
        Size -= 4;
        Array += 4;
    }

    /* 获取remaining bytes if any */
    while(((int)(Size)) > 0) {
        if((DSIx->GPSR & DSI_GPSR_PRDFE) == 0) {
            *((uint32_t *)Array) = (DSIx->GPDR);
            Size -= 4;
            Array += 4;
        }
    }
}

/**
  * 简介:  通用DSI数据包头配置
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 参数:  ChannelID: 虚拟通道头数据包的ID
  * 参数:  DataType: 头数据包的数据包数据类型
  *                   此参数可以是以下任意值:
  *                      @ref DSI_SHORT_WRITE_PKT_Data_Type
  *                   or @ref DSI_LONG_WRITE_PKT_Data_Type
  *                   or @ref DSI_SHORT_READ_PKT_Data_Type
  *                   or DSI_MAX_RETURN_PKT_SIZE
  * 参数:  Data0: 字数LSB
  * 参数:  Data1: 字数MSB
  * 返回值: 无
  */
static void DSI_ConfigPacketHeader(DSI_TypeDef *DSIx,
                                   uint32_t ChannelID,
                                   uint32_t DataType,
                                   uint32_t Data0,
                                   uint32_t Data1) {
    /* 用新信息更新DSI数据包头 */
    DSIx->GHCR = (DataType | (ChannelID << 6) | (Data0 << 8) | (Data1 << 16));
}

/**
  * @}
  */

/** @defgroup DSI_Group3 低功耗功能
 *  简介    DSI低功耗管理功能
 *
@verbatim
 ===============================================================================
                   ##### 低功耗函数 #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * 简介:  在D-PHY PLL运行的情况下进入ULPM(超低功率模式)(只有数据通道在ULPM中)
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 返回值: 无
  */
void DSI_EnterULPMData(DSI_TypeDef *DSIx) {
    /* ULPS Request on Data Lanes */
    DSIx->PUCR |= DSI_PUCR_URDL;


    /* Wait until the D-PHY active lanes enter into ULPM */
    if((DSIx->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE) {
        while((DSIx->PSR & DSI_PSR_UAN0) != 0)
        {}
    } else { /* DSI_TWO_DATA_LANES */
        while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1)) != 0)
        {}
    }
}

/**
  * 简介:  在D-PHY PLL运行的情况下退出ULPM(超低功率模式)(只有数据通道在ULPM中)
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 返回值: 无
  */
void DSI_ExitULPMData(DSI_TypeDef *DSIx) {
    /* Exit ULPS on Data Lanes */
    DSIx->PUCR |= DSI_PUCR_UEDL;

    /* Wait until all active lanes exit ULPM */
    if((DSIx->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE) {
        while((DSIx->PSR & DSI_PSR_UAN0) != DSI_PSR_UAN0)
        {}
    } else { /* DSI_TWO_DATA_LANES */
        while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1)) != (DSI_PSR_UAN0 | DSI_PSR_UAN1))
        {}
    }

    /* De-assert the ULPM requests and the ULPM exit 位 */
    DSIx->PUCR = 0;
}

/**
  * 简介:  在D-PHY PLL关闭的情况下进入ULPM(超低功率模式)(数据和时钟通道都在ULPM中)
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 返回值: 无
  */
void DSI_EnterULPM(DSI_TypeDef *DSIx) {
    /* Clock lane configuration: no more HS request */
    DSIx->CLCR &= ~DSI_CLCR_DPCC;

    /* Use system PLL as byte lane clock source before stopping DSIPHY clock source */
    RCC_DSIClockSourceConfig(RCC_DSICLKSource_PLLR);

    /* ULPS Request on Clock and Data Lanes */
    DSIx->PUCR |= (DSI_PUCR_URCL | DSI_PUCR_URDL);

    /* Wait until all active lanes exit ULPM */
    if((DSIx->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE) {
        while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UANC)) != 0)
        {}
    } else { /* DSI_TWO_DATA_LANES */
        while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)) != 0)
        {}
    }

    /* Turn off the DSI PLL */
    DSIx->WRPCR &= ~DSI_WRPCR_PLLEN;
}

/**
  * 简介:  在D-PHY PLL关闭的情况下退出ULPM(超低功率模式)(数据和时钟通道都在ULPM中)
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 返回值: 无
  */
void DSI_ExitULPM(DSI_TypeDef *DSIx) {
    /* Turn on the DSI PLL */
    DSIx->WRPCR |= DSI_WRPCR_PLLEN;

    /* Wait for the lock of the PLL */
    while(DSI_GetFlagStatus(DSIx, DSI_FLAG_PLLLS) == RESET)
    {}

    /* Exit ULPS on Clock and Data Lanes */
    DSIx->PUCR |= (DSI_PUCR_UECL | DSI_PUCR_UEDL);

    /* Wait until all active lanes exit ULPM */
    if((DSIx->PCONFR & DSI_PCONFR_NL) == DSI_ONE_DATA_LANE) {
        while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UANC)) != (DSI_PSR_UAN0 | DSI_PSR_UANC))
        {}
    } else { /* DSI_TWO_DATA_LANES */
        while((DSIx->PSR & (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC)) != (DSI_PSR_UAN0 | DSI_PSR_UAN1 | DSI_PSR_UANC))
        {}
    }

    /* De-assert the ULPM requests and the ULPM exit 位 */
    DSIx->PUCR = 0;

    /* Switch the lanbyteclock source in the RCC from system PLL to D-PHY */
    RCC_DSIClockSourceConfig(RCC_DSICLKSource_PHY);

    /* Restore clock lane configuration to HS */
    DSIx->CLCR |= DSI_CLCR_DPCC;
}

/**
  * 简介:  开始测试模式生成
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  Mode: 图形生成器模式
  *          此参数可以是以下值之一:
  *           0 : 颜色条(水平或垂直)
  *           1 : BER模式(仅垂直)
  * 参数:  Orientation: 图案生成器方向
  *          此参数可以是以下值之一:
  *           0 : 垂直颜色条
  *           1 : 水平颜色条
  * 返回值: 无
  */
void DSI_PatternGeneratorStart(DSI_TypeDef *DSIx, uint32_t Mode, uint32_t Orientation) {

    /* Configure 图形生成器模式 and orientation */
    DSIx->VMCR &= ~(DSI_VMCR_PGM | DSI_VMCR_PGO);
    DSIx->VMCR |= ((Mode << 20) | (Orientation << 24));

    /* Enable pattern generator by setting PGE 位 */
    DSIx->VMCR |= DSI_VMCR_PGE;

}

/**
  * 简介:  停止测试模式生成
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 返回值: 无
  */
void DSI_PatternGeneratorStop(DSI_TypeDef *DSIx) {
    /* Disable pattern generator by clearing PGE 位 */
    DSIx->VMCR &= ~DSI_VMCR_PGE;
}

/**
  * 简介:  设置回转率和延迟调整
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 参数:  CommDelay: 通信延迟有待调整。
  *                    此参数可以是@ref DSI_Communication_Delay
  * 参数:  Lane: 在时钟或数据通道之间进行选择。
  *               此参数可以是@ref DSI_Lane_Group
  * 参数:  Value: 转换率或延迟的自定义值
  * 返回值: 无
  */
void DSI_SetSlewRateAndDelayTuning(DSI_TypeDef *DSIx, uint32_t CommDelay, uint32_t Lane, uint32_t Value) {
    /* 检查函数参数 */
    assert_param(IS_DSI_COMMUNICATION_DELAY(CommDelay));
    assert_param(IS_DSI_LANE_GROUP(Lane));

    switch(CommDelay) {
        case DSI_SLEW_RATE_HSTX:
            if(Lane == DSI_CLOCK_LANE) {
                /* High-Speed Transmission Slew Rate Control on Clock Lane */
                DSIx->WPCR[1] &= ~DSI_WPCR1_HSTXSRCCL;
                DSIx->WPCR[1] |= Value << 16;
            } else { /* DSI_DATA_LANES */
                /* High-Speed Transmission Slew Rate Control on Data Lanes */
                DSIx->WPCR[1] &= ~DSI_WPCR1_HSTXSRCDL;
                DSIx->WPCR[1] |= Value << 18;
            }

            break;

        case DSI_SLEW_RATE_LPTX:
            if(Lane == DSI_CLOCK_LANE) {
                /* Low-Power transmission Slew Rate Compensation on Clock Lane */
                DSIx->WPCR[1] &= ~DSI_WPCR1_LPSRCCL;
                DSIx->WPCR[1] |= Value << 6;
            } else { /*  DSI_DATA_LANES */
                /* Low-Power transmission Slew Rate Compensation on Data Lanes */
                DSIx->WPCR[1] &= ~DSI_WPCR1_LPSRCDL;
                DSIx->WPCR[1] |= Value << 8;
            }

            break;

        case DSI_HS_DELAY:
            if(Lane == DSI_CLOCK_LANE) {
                /* High-Speed Transmission Delay on Clock Lane */
                DSIx->WPCR[1] &= ~DSI_WPCR1_HSTXDCL;
                DSIx->WPCR[1] |= Value;
            } else { /* DSI_DATA_LANES */
                /* High-Speed Transmission Delay on Data Lanes */
                DSIx->WPCR[1] &= ~DSI_WPCR1_HSTXDDL;
                DSIx->WPCR[1] |= Value << 2;
            }

            break;

        default:
            break;
    }
}

/**
  * 简介:  低功率接收滤波器调谐
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 参数:  Frequency: LPRX输入低通滤波器的截止频率
  * 返回值: 无
  */
void DSI_SetLowPowerRXFilter(DSI_TypeDef *DSIx, uint32_t Frequency) {
    /* Low-Power RX low-pass Filtering Tuning */
    DSIx->WPCR[1] &= ~DSI_WPCR1_LPRXFT;
    DSIx->WPCR[1] |= Frequency << 25;
}

/**
  * 简介:  激活所有车道上的其他当前路径，以满足MIPI D-PHY规范中定义的SDDTx参数
  * 参数:  hdsi: 指向DSI_HandleTypeDef结构的指针，该结构包含
  *               DSI的配置信息。
  * 参数:  State: ENABLE or DISABLE
  * 返回值: 无
  */
void DSI_SetSDD(DSI_TypeDef *DSIx, FunctionalState State) {
    /* 检查函数参数 */
    assert_param(IS_FUNCTIONAL_STATE(State));

    /* Activate/Disactivate additional current path on all lanes */
    DSIx->WPCR[1] &= ~DSI_WPCR1_SDDC;
    DSIx->WPCR[1] |= ((uint32_t)State << 12);
}

/**
  * 简介:  自定义通道管脚配置
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 参数:  CustomLane: 要应用于选定车道的功能。
  *                     此参数可以是@ref DSI_CustomLane
  * 参数:  Lane: 在时钟或数据通道0或数据通道1之间进行选择。
  *               此参数可以是@ref DSI_Lane_Select
  * 参数:  State: ENABLE or DISABLE
  * 返回值: 无
  */
void DSI_SetLanePinsConfiguration(DSI_TypeDef *DSIx, uint32_t CustomLane, uint32_t Lane, FunctionalState State) {
    /* 检查函数参数 */
    assert_param(IS_DSI_CUSTOM_LANE(CustomLane));
    assert_param(IS_DSI_LANE(Lane));
    assert_param(IS_FUNCTIONAL_STATE(State));

    switch(CustomLane) {
        case DSI_SWAP_LANE_PINS:
            if(Lane == DSI_CLOCK_LANE) {
                /* Swap pins on clock lane */
                DSIx->WPCR[0] &= ~DSI_WPCR0_SWCL;
                DSIx->WPCR[0] |= ((uint32_t)State << 6);
            } else if(Lane == DSI_DATA_LANE0) {
                /* Swap pins on data lane 0 */
                DSIx->WPCR[0] &= ~DSI_WPCR0_SWDL0;
                DSIx->WPCR[0] |= ((uint32_t)State << 7);
            } else { /* DSI_DATA_LANE1 */
                /* Swap pins on data lane 1 */
                DSIx->WPCR[0] &= ~DSI_WPCR0_SWDL1;
                DSIx->WPCR[0] |= ((uint32_t)State << 8);
            }

            break;

        case DSI_INVERT_HS_SIGNAL:
            if(Lane == DSI_CLOCK_LANE) {
                /* Invert HS signal on clock lane */
                DSIx->WPCR[0] &= ~DSI_WPCR0_HSICL;
                DSIx->WPCR[0] |= ((uint32_t)State << 9);
            } else if(Lane == DSI_DATA_LANE0) {
                /* Invert HS signal on data lane 0 */
                DSIx->WPCR[0] &= ~DSI_WPCR0_HSIDL0;
                DSIx->WPCR[0] |= ((uint32_t)State << 10);
            } else { /* DSI_DATA_LANE1 */
                /* Invert HS signal on data lane 1 */
                DSIx->WPCR[0] &= ~DSI_WPCR0_HSIDL1;
                DSIx->WPCR[0] |= ((uint32_t)State << 11);
            }

            break;

        default:
            break;
    }
}

/**
  * 简介:  为PHY设置自定义定时
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 参数:  Timing: 要调整的PHY定时。
  *                 此参数可以是@ref DSI_PHY_Timing
  * 参数:  State: ENABLE or DISABLE
  * 参数:  Value: 计时的自定义值
  * 返回值: 无
  */
void DSI_SetPHYTimings(DSI_TypeDef *DSIx, uint32_t Timing, FunctionalState State, uint32_t Value) {
    /* 检查函数参数 */
    assert_param(IS_DSI_PHY_TIMING(Timing));
    assert_param(IS_FUNCTIONAL_STATE(State));

    switch(Timing) {
        case DSI_TCLK_POST:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_TCLKPOSTEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 27);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[4] &= ~DSI_WPCR4_TCLKPOST;
                DSIx->WPCR[4] |= Value;
            }

            break;

        case DSI_TLPX_CLK:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_TLPXCEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 26);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[3] &= ~DSI_WPCR3_TLPXC;
                DSIx->WPCR[3] |= Value;
            }

            break;

        case DSI_THS_EXIT:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_THSEXITEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 25);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[3] &= ~DSI_WPCR3_THSEXIT;
                DSIx->WPCR[3] |= Value;
            }

            break;

        case DSI_TLPX_DATA:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_TLPXDEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 24);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[3] &= ~DSI_WPCR3_TLPXD;
                DSIx->WPCR[3] |= Value;
            }

            break;

        case DSI_THS_ZERO:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_THSZEROEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 23);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[3] &= ~DSI_WPCR3_THSZERO;
                DSIx->WPCR[3] |= Value;
            }

            break;

        case DSI_THS_TRAIL:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_THSTRAILEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 22);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[2] &= ~DSI_WPCR2_THSTRAIL;
                DSIx->WPCR[2] |= Value;
            }

            break;

        case DSI_THS_PREPARE:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_THSPREPEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 21);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[2] &= ~DSI_WPCR2_THSPREP;
                DSIx->WPCR[2] |= Value;
            }

            break;

        case DSI_TCLK_ZERO:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_TCLKZEROEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 20);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[2] &= ~DSI_WPCR2_TCLKZERO;
                DSIx->WPCR[2] |= Value;
            }

            break;

        case DSI_TCLK_PREPARE:
            /* Enable/Disable custom timing setting */
            DSIx->WPCR[0] &= ~DSI_WPCR0_TCLKPREPEN;
            DSIx->WPCR[0] |= ((uint32_t)State << 19);

            if(State) {
                /* Set custom 值 */
                DSIx->WPCR[2] &= ~DSI_WPCR2_TCLKPREP;
                DSIx->WPCR[2] |= Value;
            }

            break;

        default:
            break;
    }
}

/**
  * 简介:  强制时钟/数据通道处于TX停止模式
  * 参数:  DSIx: 指向DSI寄存器基址的指针
  * 参数:  Lane: 在时钟或数据通道之间进行选择。
  *               此参数可以是@ref DSI_Lane_Group
  * 参数:  State: ENABLE or DISABLE
  * 返回值: 无
  */
void DSI_ForceTXStopMode(DSI_TypeDef *DSIx, uint32_t Lane, FunctionalState State) {
    /* 检查函数参数 */
    assert_param(IS_DSI_LANE_GROUP(Lane));
    assert_param(IS_FUNCTIONAL_STATE(State));

    if(Lane == DSI_CLOCK_LANE) {
        /* Force/Unforce the Clock Lane in TX Stop Mode */
        DSIx->WPCR[0] &= ~DSI_WPCR0_FTXSMCL;
        DSIx->WPCR[0] |= ((uint32_t)State << 12);
    } else { /* DSI_DATA_LANES */
        /* Force/Unforce the Data Lanes in TX Stop Mode */
        DSIx->WPCR[0] &= ~DSI_WPCR0_FTXSMDL;
        DSIx->WPCR[0] |= ((uint32_t)State << 13);
    }
}

/**
  * 简介:  强制LP接收器处于低功率模式
  * 参数:  hdsi: 指向DSI_HandleTypeDef结构的指针，该结构包含
  *               DSI的配置信息。
  * 参数:  State: ENABLE or DISABLE
  * 返回值: 无
  */
void DSI_ForceRXLowPower(DSI_TypeDef *DSIx, FunctionalState State) {
    /* 检查函数参数 */
    assert_param(IS_FUNCTIONAL_STATE(State));

    /* Force/Unforce LP Receiver in Low-Power Mode */
    DSIx->WPCR[1] &= ~DSI_WPCR1_FLPRXLPM;
    DSIx->WPCR[1] |= ((uint32_t)State << 22);
}

/**
  * 简介:  BTA后强制数据通道处于RX模式
  * 参数:  hdsi: 指向DSI_HandleTypeDef结构的指针，该结构包含
  *               DSI的配置信息。
  * 参数:  State: ENABLE or DISABLE
  * 返回值: 无
  */
void DSI_ForceDataLanesInRX(DSI_TypeDef *DSIx, FunctionalState State) {
    /* 检查函数参数 */
    assert_param(IS_FUNCTIONAL_STATE(State));

    /* Force Data Lanes in RX Mode */
    DSIx->WPCR[0] &= ~DSI_WPCR0_TDDL;
    DSIx->WPCR[0] |= ((uint32_t)State << 16);
}

/**
  * 简介:  启用车道上的下拉按钮，以防止未使用时出现浮动状态
  * 参数:  hdsi: 指向DSI_HandleTypeDef结构的指针，该结构包含
  *               DSI的配置信息。
  * 参数:  State: ENABLE or DISABLE
  * 返回值: 无
  */
void DSI_SetPullDown(DSI_TypeDef *DSIx, FunctionalState State) {
    /* 检查函数参数 */
    assert_param(IS_FUNCTIONAL_STATE(State));

    /* Enable/Disable pull-down on lanes */
    DSIx->WPCR[0] &= ~DSI_WPCR0_PDEN;
    DSIx->WPCR[0] |= ((uint32_t)State << 18);
}

/**
  * 简介:  关闭数据通道上的竞争检测
  * 参数:  hdsi: 指向DSI_HandleTypeDef结构的指针，该结构包含
  *               DSI的配置信息。
  * 参数:  State: ENABLE or DISABLE
  * 返回值: 无
  */
void DSI_SetContentionDetectionOff(DSI_TypeDef *DSIx, FunctionalState State) {
    /* 检查函数参数 */
    assert_param(IS_FUNCTIONAL_STATE(State));

    /* Contention Detection on Data Lanes OFF */
    DSIx->WPCR[0] &= ~DSI_WPCR0_CDOFFDL;
    DSIx->WPCR[0] |= ((uint32_t)State << 14);
}

/**
  * @}
  */

/** @defgroup DSI_Group4 中断和标记管理函数
  *  简介   中断和标记管理函数
  *
@verbatim
 ===============================================================================
            ##### 中断和标记管理函数 #####
 ===============================================================================

 [..] 本节提供了一组函数，允许配置DSI中断源并检查或清除标志或挂起位状态。
      用户应确定在其应用程序中将使用哪种模式来管理通信:轮询模式或中断模式。

 *** 轮询模式 ***
 ====================
[..] 在轮询模式下，DSI通信可以由8个标志管理:
  (#)DSI_FLAG_TE:撕裂效果中断标志
  (#)DSI_FLAG_ER:刷新中断结束标志
  (#)DSI_FLAG_BUSY:忙标志
  (#)DSI_FLAG_PLLLS:PLL锁定状态
  (#)DSI_FLAG_PLLL:PLL锁定中断标志
  (#)DSI_FLAG_PLLU:PLL解锁中断标志
  (#)DSI_FLAG_RRS:调节器就绪状态。
  (#)DSI_FLAG_RR:调节器就绪中断标志。


 [..] 在此模式下，建议使用以下函数:
   (+) FlagStatus DSI_GetFlagStatus(DSI_TypeDef* DSIx, uint32_t DSI_FLAG);
   (+) void DSI_ClearFlag(DSI_TypeDef* DSIx, uint32_t DSI_FLAG);

 *** 中断模式 ***
 ======================
 [..] 在中断模式下，SPI通信可由3个中断源管理和7个挂起位:
  (+)挂起位:
  (##)DSI_IT_TE:撕裂效果中断标志
  (##)DSI_IT_ER:刷新中断结束标志
  (##)DSI_IT_PLLL:PLL锁定中断标志
  (##)DSI_IT_PLLU:PLL解锁中断标志
  (##)DSI_IT_RR:调节器就绪中断标志。
  (+) 中断源:
  (##)DSI_IT_TE:撕裂效果中断启用
  (##)DSI_IT_ER:刷新中断结束启用
  (##)DSI_IT_PLLL:PLL锁定中断启用
  (##)DSI_IT_PLLU:PLL解锁中断启用
  (##)DSI_IT_RR:调节器就绪中断启用
  [..]在此模式下，建议使用以下功能:
   (+) void DSI_ITConfig(DSI_TypeDef* DSIx, uint32_t DSI_IT, FunctionalState NewState);
   (+) ITStatus DSI_GetITStatus(DSI_TypeDef* DSIx, uint32_t DSI_IT);
   (+) void DSI_ClearITPendingBit(DSI_TypeDef* DSIx, uint32_t DSI_IT);

@endverbatim
  * @{
  */

/**
  * 简介: 启用或禁用指定的DSI中断。
  * 参数: DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数: DSI_IT: 指定要启用或禁用的DSI中断源。
  *          此参数可以是以下值的任意组合:
  *            @arg DSI_IT_TE:撕裂效果中断
  *            @arg DSI_IT_ER:刷新中断结束
  *            @arg DSI_IT_PLLL:PLL锁定中断
  *            @arg DSI_IT_PLLU:PLL解锁中断
  *            @arg DSI_IT_RR:调节器就绪中断
  * 参数:  NewState: 指定DSI中断的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void DSI_ITConfig(DSI_TypeDef* DSIx, uint32_t DSI_IT, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_DSI_ALL_PERIPH(DSIx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    assert_param(IS_DSI_IT(DSI_IT));

    if(NewState != DISABLE) {
        /* 启用 selected DSI interrupt */
        DSIx->WIER |= DSI_IT;
    } else {
        /* 禁用 selected DSI interrupt */
        DSIx->WIER &= ~DSI_IT;
    }
}

/**
  * 简介: 检查是否设置了指定的DSI标志。
  * 参数: DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数: DSI_FLAG: 指定要检查的SPI标志。
  *          此参数可以是以下值之一:
  *          @arg DSI_FLAG_TE:撕裂效果中断标志
  *          @arg DSI_FLAG_ER:刷新中断结束标志
  *          @arg DSI_FLAG_BUSY:忙标志
  *          @arg DSI_FLAG_PLLLS:PLL锁定状态
  *          @arg DSI_FLAG_PLLL:PLL锁定中断标志
  *          @arg DSI_FLAG_PLLU:PLL解锁中断标志
  *          @arg DSI_FLAG_RRS:调节器就绪标志
  *          @arg DSI_FLAG_RR:调节器就绪中断标志
  * 返回值: DSI_FLAG的新状态(SET或RESET)。
  */
FlagStatus DSI_GetFlagStatus(DSI_TypeDef* DSIx, uint16_t DSI_FLAG) {
    FlagStatus bitstatus = RESET;
    /* 检查参数 */
    assert_param(IS_DSI_ALL_PERIPH(DSIx));
    assert_param(IS_DSI_GET_FLAG(DSI_FLAG));

    /* 检查 the status of the specified DSI flag */
    if((DSIx->WISR & DSI_FLAG) != (uint32_t)RESET) {
        /* DSI_FLAG 被设置 */
        bitstatus = SET;
    } else {
        /* DSI_FLAG  被重置 */
        bitstatus = RESET;
    }

    /* 返回DSI_FLAG 状态 */
    return  bitstatus;
}

/**
  * 简介: 清除指定的DSI标志。
  * 参数: DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数: DSI_FLAG: 指定要清除的SPI标志。
  *          此参数可以是以下值之一:
  *            @arg DSI_FLAG_TE   : 撕裂效果中断标志
  *            @arg DSI_FLAG_ER   : 刷新中断结束标志
  *            @arg DSI_FLAG_PLLL:PLL锁定中断标志
  *            @arg DSI_FLAG_PLLU:PLL解锁中断标志
  *            @arg DSI_FLAG_RR:调节器就绪中断标志
  * 返回值: 无
  */
void DSI_ClearFlag(DSI_TypeDef* DSIx, uint16_t DSI_FLAG) {
    /* 检查参数 */
    assert_param(IS_DSI_ALL_PERIPH(DSIx));
    assert_param(IS_DSI_CLEAR_FLAG(DSI_FLAG));

    /* 清除 selected DSI flag */
    DSIx->WIFCR = (uint32_t)DSI_FLAG;
}

/**
  * 简介: 检查指定的DSIx中断是否已发生。
  * 参数: DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数: DSI_IT: 指定要检查的DSI中断源。
  *          此参数可以是以下值之一:
  *          @arg DSI_IT_TE   :撕裂效果中断
  *          @arg DSI_IT_ER    :刷新中断结束
  *          @arg DSI_IT_PLLL  :PLL锁定中断
  *          @arg DSI_IT_PLLU  :PLL解锁中断
  *          @arg DSI_IT_RR:调节器就绪中断
  * 返回值: 新状态-> SPI_I2S_IT (SET or RESET).
  */
ITStatus DSI_GetITStatus(DSI_TypeDef* DSIx, uint32_t DSI_IT) {
    ITStatus bitstatus = RESET;
    uint32_t enablestatus = 0;

    /* 检查参数 */
    assert_param(IS_DSI_ALL_PERIPH(DSIx));
    assert_param(IS_DSI_IT(DSI_IT));

    /* 获取DSI_IT enable bit 状态 */
    enablestatus = (DSIx->WIER & DSI_IT);

    /* 检查 the status of the specified SPI interrupt */
    if (((DSIx->WISR & DSI_IT) != (uint32_t)RESET) && enablestatus) {
        /* DSI_IT 被设置 */
        bitstatus = SET;
    } else {
        /* DSI_IT  被重置 */
        bitstatus = RESET;
    }

    /* 返回DSI_IT 状态 */
    return bitstatus;
}

*            @arg DSI_IT_TE  :
Tearing Effect Interrupt
*            @arg DSI_IT_ER  :
End of Refresh Interrupt
*            @arg DSI_IT_PLLL:
PLL Lock Interrupt
*            @arg DSI_IT_PLLU:
PLL Unlock Interrupt
*            @arg DSI_IT_RR  :
Regulator Ready Interrupt


/**
  * 简介: 清除DSIx中断挂起位。
  * 参数: DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数: DSI_IT: 指定要清除的DSI中断源。
  *        此参数可以是以下值之一:
  *            @arg DSI_IT_TE:撕裂效果中断
  *            @arg DSI_IT_ER:刷新中断结束
  *            @arg DSI_IT_PLLL:PLL锁定中断
  *            @arg DSI_IT_PLLU:PLL解锁中断
  *            @arg DSI_IT_RR:调节器就绪中断
  * 返回值: 无
  */
void DSI_ClearITPendingBit(DSI_TypeDef* DSIx, uint32_t DSI_IT) {
    /* 检查参数 */
    assert_param(IS_DSI_ALL_PERIPH(DSIx));
    assert_param(IS_DSI_IT(DSI_IT));

    /* 清除 selected DSI interrupt pending 位 */
    DSIx->WIFCR = (uint32_t)DSI_IT;
}

/**
  * 简介:  启用错误监视器标志
  * 参数:  DSIx: 要选择DSIx外设，其中x可以是不同的DSI实例
  * 参数:  ActiveErrors: 指示将启用哪些错误中断。
  *                      此参数可以是@ref DSI_Error_Data_Type的任意组合。
  * 返回值: 无
  */
void DSI_ConfigErrorMonitor(DSI_TypeDef *DSIx, uint32_t ActiveErrors) {
    DSIx->IER[0] = 0;
    DSIx->IER[1] = 0;

    if((ActiveErrors & DSI_ERROR_ACK) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[0] |= DSI_ERROR_ACK_MASK;
    }

    if((ActiveErrors & DSI_ERROR_PHY) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[0] |= DSI_ERROR_PHY_MASK;
    }

    if((ActiveErrors & DSI_ERROR_TX) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[1] |= DSI_ERROR_TX_MASK;
    }

    if((ActiveErrors & DSI_ERROR_RX) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[1] |= DSI_ERROR_RX_MASK;
    }

    if((ActiveErrors & DSI_ERROR_ECC) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[1] |= DSI_ERROR_ECC_MASK;
    }

    if((ActiveErrors & DSI_ERROR_CRC) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[1] |= DSI_ERROR_CRC_MASK;
    }

    if((ActiveErrors & DSI_ERROR_PSE) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[1] |= DSI_ERROR_PSE_MASK;
    }

    if((ActiveErrors & DSI_ERROR_EOT) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[1] |= DSI_ERROR_EOT_MASK;
    }

    if((ActiveErrors & DSI_ERROR_OVF) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[1] |= DSI_ERROR_OVF_MASK;
    }

    if((ActiveErrors & DSI_ERROR_GEN) != RESET) {
        /* 启用选定错误的中断生成 */
        DSIx->IER[1] |= DSI_ERROR_GEN_MASK;
    }
}

/**
  * @}
  */

/**
  * @}
  */
#endif /* STM32F469_479xx */
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
