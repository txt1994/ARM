/**
  ******************************************************************************
  * 文件:    stm32f4xx_rcc.c
  * 作者:    MCD Application Team
  * 版本:    V1.8.0
  * 日期:    04-November-2016
  * 简介:    此文件提供固件功能，以管理重置和时钟控制(RCC)外围设备的以下功能:
  *           +内部/外部时钟、PLL、CSS和MCO配置
  *           +系统、AHB和APB总线时钟配置
  *           +外围时钟配置
  *           + 中断和标志管理
  *
 @verbatim
 ===============================================================================
                      ##### RCC特定功能 #####
 ===============================================================================
    [..]
      重置后，设备从内部高速振荡器(HSI 16MHz)运行，Flash 0等待状态，
          Flash预取缓冲区、D-Cache和I-Cache被禁用，除内部SRAM、Flash和JTAG外，
          所有外围设备均关闭。
      (+) 高速(AHB)和低速(APB)总线上没有预分频器；
          这些总线上映射的所有外围设备都以HSI速度运行。
      (+) 除SRAM和FLASH外，所有外围设备的时钟均已关闭。
      (+) 所有GPIO都处于输入浮动状态，但分配用于调试目的的JTAG引脚除外。
    [..]
      一旦设备从重置启动，用户应用程序必须:
      (+) 配置用于驱动系统时钟的时钟源(如果应用程序需要更高的频率/性能)
      (+) 配置系统时钟频率和闪存设置
      (+) 配置AHB和APB总线预分频器
      (+) 启用要使用的外围设备的时钟
      (+) 为时钟不是来自系统时钟(I2S、RTC、ADC、USB OTG FS/SDIO/RNG)的外围设备配置时钟源
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
#include "stm32f4xx_rcc.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @defgroup RCC
  * 简介: RCC 驱动模块
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* 私有宏 ------------------------------------------------------------*/
/* ------------ RCC registers bit address in the alias region ----------- */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)
/* --- CR 寄存器 ---*/
/* Alias word address of HSION 位 */
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BitNumber           0x00
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))
/* Alias word address of CSSON 位 */
#define CSSON_BitNumber           0x13
#define CR_CSSON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (CSSON_BitNumber * 4))
/* Alias word address of PLLON 位 */
#define PLLON_BitNumber           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))
/* Alias word address of PLLI2SON 位 */
#define PLLI2SON_BitNumber        0x1A
#define CR_PLLI2SON_BB            (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLI2SON_BitNumber * 4))

/* Alias word address of PLLSAION 位 */
#define PLLSAION_BitNumber        0x1C
#define CR_PLLSAION_BB            (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLSAION_BitNumber * 4))

/* --- CFGR 寄存器 ---*/
/* Alias word address of I2SSRC 位 */
#define CFGR_OFFSET               (RCC_OFFSET + 0x08)
#define I2SSRC_BitNumber          0x17
#define CFGR_I2SSRC_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (I2SSRC_BitNumber * 4))

/* --- BDCR 寄存器 ---*/
/* Alias word address of RTCEN 位 */
#define BDCR_OFFSET               (RCC_OFFSET + 0x70)
#define RTCEN_BitNumber           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))
/* Alias word address of BDRST 位 */
#define BDRST_BitNumber           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

/* --- CSR 寄存器 ---*/
/* Alias word address of LSION 位 */
#define CSR_OFFSET                (RCC_OFFSET + 0x74)
#define LSION_BitNumber           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

/* --- DCKCFGR 寄存器 ---*/
/* Alias word address of TIMPRE 位 */
#define DCKCFGR_OFFSET            (RCC_OFFSET + 0x8C)
#define TIMPRE_BitNumber          0x18
#define DCKCFGR_TIMPRE_BB         (PERIPH_BB_BASE + (DCKCFGR_OFFSET * 32) + (TIMPRE_BitNumber * 4))

/* --- CFGR Register ---*/
#define RCC_CFGR_OFFSET            (RCC_OFFSET + 0x08)
#if defined(STM32F410xx)
    /* Alias word address of MCO1EN 位 */
    #define RCC_MCO1EN_BIT_NUMBER      0x8
    #define RCC_CFGR_MCO1EN_BB         (PERIPH_BB_BASE + (RCC_CFGR_OFFSET * 32) + (RCC_MCO1EN_BIT_NUMBER * 4))

    /* Alias word address of MCO2EN 位 */
    #define RCC_MCO2EN_BIT_NUMBER      0x9
    #define RCC_CFGR_MCO2EN_BB         (PERIPH_BB_BASE + (RCC_CFGR_OFFSET * 32) + (RCC_MCO2EN_BIT_NUMBER * 4))
#endif /* STM32F410xx */
/* ---------------------- RCC寄存器位掩码 ------------------------ */
/* CFGR寄存器位掩码 */
#define CFGR_MCO2_RESET_MASK      ((uint32_t)0x07FFFFFF)
#define CFGR_MCO1_RESET_MASK      ((uint32_t)0xF89FFFFF)

/* RCC Flag Mask */
#define FLAG_MASK                 ((uint8_t)0x1F)

/* CR register byte 3 (Bits[23:16]) base address */
#define CR_BYTE3_ADDRESS          ((uint32_t)0x40023802)

/* CIR register byte 2 (Bits[15:8]) base address */
#define CIR_BYTE2_ADDRESS         ((uint32_t)(RCC_BASE + 0x0C + 0x01))

/* CIR register byte 3 (Bits[23:16]) base address */
#define CIR_BYTE3_ADDRESS         ((uint32_t)(RCC_BASE + 0x0C + 0x02))

/* BDCR register base address */
#define BDCR_ADDRESS              (PERIPH_BASE + BDCR_OFFSET)

/* 私有宏 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};

/* 私有函数原型 -----------------------------------------------*/
/* 私有函数 ---------------------------------------------------------*/

/** @defgroup RCC_Private_Functions
  * @{
  */

/** @defgroup RCC_Group1 内部和外部时钟、PLL、CSS和MCO配置功能
 *  简介   内部和外部时钟、PLL、CSS和MCO配置功能
 *
@verbatim
 ===================================================================================
 ##### 内部和外部时钟、PLL、CSS和MCO配置功能 #####
 ===================================================================================
    [..]
      本节提供了可以配置内部/外部时钟的功能。PLL、CSS和MCO引脚。

      (#)HSI(高速内部)，16MHz的工厂调整的RC直接使用或通过PLL作为系统时钟源。作为系统时钟源。

      (#) LSI(低速内部)，32KHz低消耗RC用作IWDG和/或RTC 时钟源。

      (#) HSE(高速外部)，4-26MHz晶体振荡器，直接或通过PLL作为系统时钟源使用。也可作为RTC时钟源。

      (#) LSE(低速外部)，32KHz振荡器用作RTC时钟源。

      (#) PLL(由HSI或HSE提供时钟)，具有两个不同的输出时钟。
        (++)第一个输出用于产生高速系统时钟(高达168MHz)
        (++)第二个输出用于产生USB OTG FS(48 MHz)、随机模拟发生器(<=48 MHz)和SDIO(<=48 MHz)的时钟。

      (#) PLLI2S(由HSI或HSE时钟)，用于产生精确的时钟，以便在I2S接口或SAI接口上实现高质量的音频性能，如果是STM32F429x/439x器件。

      (#) PLLSAI由(HSI或HSE)时钟，用于为SAI接口和LCD TFT控制器产生精确的时钟，仅适用于STM32F42xxx/43xxx/446xx/469xx/479xx器件。

      (#) CSS(时钟安全系统)，一旦启用，如果HSE时钟发生故障(HSE直接使用或通过PLL作为系统时钟源)，系统时钟将自动切换到HSI，如果启用则产生一个中断。
         该中断与Cortex-M4的NMI(非屏蔽中断)异常向量相连。

      (#) MCO1(微控制器时钟输出)，用于在PA8引脚上输出HSI、LSE、HSE或PLL时钟(通过一个可配置的预分频器)。

      (#) MCO2(微控制器时钟输出)，用于在PC9引脚上输出HSE、PLL、SYSCLK或PLLI2S时钟(通过一个可配置的预分频器)。

 @endverbatim
  * @{
  */

/**
  * 简介:  将 RCC 时钟配置重置为默认重置状态。
  * 注意:   时钟配置的默认复位状态如下:
  *            - HSI开启并作为系统时钟源使用
  *            - HSE、PLL和PLLI2S关闭
  *            - AHB、APB1和APB2预分频器设置为1。
  *            - CSS、MCO1和MCO2关闭
  *            - 禁用所有中断
  * 注意:   这个函数并不修改配置
  *           - 外围时钟
  *           - LSI、LSE和RTC时钟
  * 参数:  无
  * 返回值: 无
  */
void RCC_DeInit(void) {
    /* Set HSION 位 */
    RCC->CR |= (uint32_t)0x00000001;

    /* Reset CFGR 寄存器 */
    RCC->CFGR = 0x00000000;

    /* Reset HSEON, CSSON, PLLON, PLLI2S and PLLSAI(STM32F42xxx/43xxx/446xx/469xx/479xx devices) 位 */
    RCC->CR &= (uint32_t)0xEAF6FFFF;

    /* Reset PLLCFGR 寄存器 */
    RCC->PLLCFGR = 0x24003010;

    #if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F446xx) || defined(STM32F413_423xx) || defined(STM32F469_479xx)
    /* Reset PLLI2SCFGR 寄存器 */
    RCC->PLLI2SCFGR = 0x20003000;
    #endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F411xE || STM32F446xx || STM32F413_423xx || STM32F469_479xx */

    #if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
    /* Reset PLLSAICFGR register, only available for STM32F42xxx/43xxx/446xx/469xx/479xx devices */
    RCC->PLLSAICFGR = 0x24003000;
    #endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F446xx || STM32F469_479xx */

    /* Reset HSEBYP 位 */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000;

    /* Disable Timers clock prescalers selection, only available for STM32F42/43xxx and STM32F413_423xx devices */
    RCC->DCKCFGR = 0x00000000;

    #if defined(STM32F410xx) || defined(STM32F413_423xx)
    /* Disable LPTIM and FMPI2C clock prescalers selection, only available for STM32F410xx and STM32F413_423xx devices */
    RCC->DCKCFGR2 = 0x00000000;
    #endif /* STM32F410xx || STM32F413_423xx */
}

/**
  * 简介:  配置外部高速振荡器 (HSE)。
  * 注意:  在启用HSE(RCC_HSE_ON或RCC_HSE_Bypass)后，应用软件应等待
  *        HSERDY标志被设置，表明HSE时钟稳定，可用于PLL和/或系统时钟。
  * 注意:  如果直接使用或通过PLL作为系统时钟，HSE状态不能改变。
  *        在这种情况下，你必须选择另一个系统时钟源，然后改变HSE状态(例如，禁用它)。
  * 注意:  当进入STOP和STANDBY模式时，HSE被硬件停止。
  * 注意:  这个函数重置了CSSON位，所以如果时钟安全系统(CSS)
  *        以前是启用的，你必须在调用这个函数后再次启用它。
  * 参数:  RCC_HSE: 规定了HSE的新状态。
  *          此参数可以是以下值之一:
  *            @arg RCC_HSE_OFF: 关闭HSE振荡器，HSERDY标志在6个HSE振荡器时钟周期后变低。
  *            @arg RCC_HSE_ON: 开启HSE振荡器
  *            @arg RCC_HSE_Bypass: 用外部时钟旁路HSE振荡器
  * 返回值: 无
  */
void RCC_HSEConfig(uint8_t RCC_HSE) {
    /* 检查参数 */
    assert_param(IS_RCC_HSE(RCC_HSE));

    /* 配置HSE前，请先重置HSEON和HSEBYP位 ------------------*/
    *(__IO uint8_t *) CR_BYTE3_ADDRESS = RCC_HSE_OFF;

    /* 设置 new HSE 配置 -------------------------------------------*/
    *(__IO uint8_t *) CR_BYTE3_ADDRESS = RCC_HSE;
}

/**
  * 简介:  等待 HSE 启动。
  * 注意:  该函数等待HSERDY标志被设置，如果该标志被设置，则返回SUCCESS，
  *        否则如果达到超时且该标志未被设置，则返回ERROR。超时值由stm32f4xx.h
  *        文件中的常数HSE_STARTUP_TIMEOUT定义。你可以根据你的应用程序中使用的HSE晶体来定制它。
  * 参数:  无
  * 返回值: ErrorStatus枚举值:
  *          - SUCCESS: HSE振荡器稳定，随时可用
  *          - ERROR: HSE振荡器尚未就绪
  */
ErrorStatus RCC_WaitForHSEStartUp(void) {
    __IO uint32_t startupcounter = 0;
    ErrorStatus status = ERROR;
    FlagStatus hsestatus = RESET;

    /* 等待HSE准备就绪，如果达到超时退出 */
    do {
        hsestatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
        startupcounter++;
    } while((startupcounter != HSE_STARTUP_TIMEOUT) && (hsestatus == RESET));

    if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET) {
        status = SUCCESS;
    } else {
        status = ERROR;
    }

    return (status);
}

/**
  * 简介:  调整内部高速振荡器 (HSI) 校准值。
  * 注意:   校准用于补偿影响内部HSI RC频率的电压和温度变化。
  * 参数:  HSICalibrationValue: 指定校准微调值。
  *         此参数必须是介于0和0x1F之间的数字。
  * 返回值: 无
  */
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue) {
    uint32_t tmpreg = 0;
    /* 检查参数 */
    assert_param(IS_RCC_CALIBRATION_VALUE(HSICalibrationValue));

    tmpreg = RCC->CR;

    /* 清除 HSITRIM[4:0] 位 */
    tmpreg &= ~RCC_CR_HSITRIM;

    /* 设置 HSITRIM[4:0] bits according to HSICalibrationValue 值 */
    tmpreg |= (uint32_t)HSICalibrationValue << 3;

    /* 存储新值 */
    RCC->CR = tmpreg;
}

/**
  * 简介:  启用或禁用内部高速振荡器 (HSI)。
  * 注意:  当进入STOP和STANDBY模式时，HSI被硬件停止。
  *        它被用作(由硬件启用)系统时钟源，在复位后启动，从STOP和STANDBY
  *        模式唤醒，或在HSE故障时直接或间接用作系统时钟(如果时钟安全系统CSS被启用)。
  * 注意:  如果HSI被用作系统时钟源，则不能被停止。在这种情况下，
  *        你必须选择另一个系统时钟源，然后停止HSI。
  * 注意。: 启用HSI后，应用软件应等待HSIRDY标志被设置，
  *        表明HSI时钟稳定，可以作为系统时钟源使用。
  * 参数:  NewState: HSI的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 注意:   当HSI停止时，HSIRDY标志在6个HSI振荡器时钟周期后变为低电平。
  * 返回值: 无
  */
void RCC_HSICmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) CR_HSION_BB = (uint32_t)NewState;
}

/**
  * 简介:  配置外部低速振荡器 (LSE)。
  * 注意:   由于LSE位于Backup域中，重置后拒绝对该域进行写访问，
  *         因此在配置LSE之前必须使用PWR_BackupAccessCmd(ENABLE)功能启用写访问(重置后只执行一次)。
  * 注意:   启用LSE (RCC_LSE_ON或RCC_LSE_Bypass)后，
  *         应用软件需要等待LSERDY标志设置完成，表示LSE时钟稳定，可用于RTC时钟。
  * 参数:  RCC_LSE: specifies the 新状态-> LSE.
  *          此参数可以是以下值之一:
  *            @arg RCC_LSE_OFF: 关闭LSE振荡器，LSERDY标志在6个LSE振荡器时钟周期后变低。
  *            @arg RCC_LSE_ON: 打开LSE振荡器
  *            @arg RCC_LSE_Bypass: LSE振荡器用外部时钟旁路
  * 返回值: 无
  */
void RCC_LSEConfig(uint8_t RCC_LSE) {
    /* 检查参数 */
    assert_param(IS_RCC_LSE(RCC_LSE));

    /* 配置LSE前，需要重置LSEON和LSEBYP位 ------------------*/
    /* Reset LSEON 位 */
    *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;

    /* Reset LSEBYP 位 */
    *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;

    /* 配置LSE(上面的代码部分已经介绍了RCC_LSE_OFF) */
    switch (RCC_LSE) {
        case RCC_LSE_ON:
            /* Set LSEON 位 */
            *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_ON;
            break;

        case RCC_LSE_Bypass:
            /* Set LSEBYP and LSEON 位 */
            *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
            break;

        default:
            break;
    }
}

/**
  * 简介:  启用或禁用内部低速振荡器 (LSI)。
  * 注意:   使能LSI后，应用软件需要等待LSIRDY标志设置完成，
  *         表示LSI时钟稳定，可用于IWDG和/或RTC的时钟。
  * 注意:   当IWDG正在运行时，不能禁用LSI。
  * 参数:  NewState: 新状态-> LSI.
  *          此参数可以是:ENABLE或DISABLE。
  * 注意:   当LSI停止时，经过6个LSI振荡器时钟周期后LSIRDY标志位变为low。
  * 返回值: 无
  */
void RCC_LSICmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) CSR_LSION_BB = (uint32_t)NewState;
}

#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
/**
  * 简介:  配置主PLL时钟源、乘法和除法系数。
  * 注意:   这个函数必须在主PLL被禁用时才使用。
  *
  * 参数:  RCC_PLLSource: PLL表项时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_PLLSource_HSI: 选择HSI振荡器时钟作为锁相环时钟入口
  *            @arg RCC_PLLSource_HSE: 选择HSE振荡器时钟作为锁相环时钟入口
  * 注意:   该时钟源(RCC_PLLSource)是主PLL和PLLI2S的通用时钟源。
  *
  * 参数:  PLLM: specifies the division factor for PLL VCO input clock
  *          此参数必须是介于 0 and 63.
  * 注意:   You have to set the PLLM parameter correctly to ensure that the VCO input
  *         frequency ranges from 1 to 2 MHz. It is recommended to select a frequency
  *         of 2 MHz to limit PLL jitter.
  *
  * 参数:  PLLN: specifies the multiplication factor for PLL VCO output clock
  *          必须为50 ~ 432之间的数字。
  * 注意:   You have to set the PLLN parameter correctly to ensure that the VCO
  *         output frequency is between 100 and 432 MHz.
  *
  * 参数:  PLLP: specifies the division factor for main system clock (SYSCLK)
  *          此参数必须是范围{2、4、6或8}中的数字。
  * 注意:   You have to set the PLLP parameter correctly to not exceed 168 MHz on
  *         the System clock frequency.
  *
  * 参数:  PLLQ: specifies the division factor for OTG FS, SDIO and RNG clocks
  *          此参数必须是介于 4 and 15.
  *
  * 参数:  PLLR: specifies the division factor for I2S, SAI, SYSTEM, SPDIF in STM32F446xx devices
  *          此参数必须是介于2和7之间的数字。
  *
  * 注意:   If the USB OTG FS is used in your application, you have to set the
  *         PLLQ parameter correctly to have 48 MHz clock for the USB. However,
  *         the SDIO and RNG need a frequency lower than or equal to 48 MHz to work
  *         correctly.
  *
  * 返回值: 无
  */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ, uint32_t PLLR) {
    /* 检查参数 */
    assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
    assert_param(IS_RCC_PLLM_VALUE(PLLM));
    assert_param(IS_RCC_PLLN_VALUE(PLLN));
    assert_param(IS_RCC_PLLP_VALUE(PLLP));
    assert_param(IS_RCC_PLLQ_VALUE(PLLQ));
    assert_param(IS_RCC_PLLR_VALUE(PLLR));

    RCC->PLLCFGR = PLLM | (PLLN << 6) | (((PLLP >> 1) - 1) << 16) | (RCC_PLLSource) |
                   (PLLQ << 24) | (PLLR << 28);
}
#endif /* STM32F410xx || STM32F412xG || STM32F413_423xx || STM32F446xx || STM32F469_479xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F411xE)
/**
  * 简介:  配置主PLL时钟源、乘法和除法因子。
  * 注意:   这个函数必须在主PLL被禁用时才使用。
  *
  * 参数:  RCC_PLLSource: 指定PLL入口时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_PLLSource_HSI: HSI 选择振荡器时钟作为PLL时钟点
  *            @arg RCC_PLLSource_HSE: HSE 选择振荡器时钟作为PLL时钟点
  * 注意:   该时钟源(RCC_PLLSource)对于主PLL和PLLI2S是通用的。
  *
  * 参数:  PLLM: 指定PLL VCO输入时钟的分频因子
  *          此参数必须是介于 0 and 63.
  * 注意:   您必须正确设置PLLM参数，以确保VCO输入频率
			在1到2 MHz之间。
			建议选择2 MHz的频率来限制PLL抖动。
  *
  * 参数:  PLLN: 指定PLL VCO输出时钟的倍增因子
  *          必须为50 ~ 432之间的数字。
  * 注意:   您必须正确设置PLLN参数，
  *			以确保VCO输出频率在100和432 MHz之间。
  *
  * 参数:  PLLP: 指定主系统时钟(SYSCLK)的分频因子
  *          此参数必须是范围{2、4、6或8}中的数字。
  * 注意:   您必须正确设置PLLP参数，
  *			使其在系统时钟频率上不超过168 MHz。
  *
  * 参数:  PLLQ: 指定OTG FS、SDIO和RNG时钟的分频因子
  *          此参数必须是介于 4 and 15.
  * 注意:   如果您的应用程序中使用USB OTG FS，
			则必须正确设置PLLQ参数，使USB具有48 MHz时钟。
			然而，SDIO和RNG需要低于或等于48MHz的频率才能正常工作。
  *
  * 返回值: 无
  */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t PLLM, uint32_t PLLN, uint32_t PLLP, uint32_t PLLQ) {
    /* 检查参数 */
    assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
    assert_param(IS_RCC_PLLM_VALUE(PLLM));
    assert_param(IS_RCC_PLLN_VALUE(PLLN));
    assert_param(IS_RCC_PLLP_VALUE(PLLP));
    assert_param(IS_RCC_PLLQ_VALUE(PLLQ));

    RCC->PLLCFGR = PLLM | (PLLN << 6) | (((PLLP >> 1) - 1) << 16) | (RCC_PLLSource) |
                   (PLLQ << 24);
}
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F411xE */

/**
  * 简介:  启用或禁用main PLL.
  * 注意:   启用主PLL后，应用软件应等待PLLRDY标志
			被设置，指示PLL时钟稳定，可以用作系统时钟源。
  * 注意:   如果主PLL用作系统时钟源，则不能禁用它
  * 注意:   当进入STOP(停止)和STANDBY(待机)模式时，主PLL被硬件禁用。
  * 参数:  NewState: 新状态-> main PLL. 此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_PLLCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    *(__IO uint32_t *) CR_PLLON_BB = (uint32_t)NewState;
}

#if defined(STM32F40_41xxx) || defined(STM32F401xx)
/**
  * 简介:  配置PLLI2S时钟乘除因子。
  *
  * 注意:   This function can be used only for STM32F405xx/407xx, STM32F415xx/417xx
  *         or STM32F401xx devices.
  *
  * 注意:   该功能仅在禁用PLLI2S的情况下使用。
  * 注意:   PLLI2S时钟源与主锁相环共用(在RCC_PLLConfig函数中配置)
  *
  * 参数:  PLLI2SN: 指定PLLI2S VCO输出时钟的乘法因子
  *          必须为50 ~ 432之间的数字。
  * 注意:   必须正确设置PLLI2SN参数，以确保VCO输出频率在100 ~ 432mhz之间。
  *
  * 参数:  PLLI2SR: I2S时钟的除法因子
  *          此参数必须是介于2和7之间的数字。
  * 注意:   在I2S时钟频率上，必须将PLLI2SR参数正确设置为不超过192mhz。
  *
  * 返回值: 无
  */
void RCC_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SR) {
    /* 检查参数 */
    assert_param(IS_RCC_PLLI2SN_VALUE(PLLI2SN));
    assert_param(IS_RCC_PLLI2SR_VALUE(PLLI2SR));

    RCC->PLLI2SCFGR = (PLLI2SN << 6) | (PLLI2SR << 28);
}
#endif /* STM32F40_41xxx || STM32F401xx */

#if defined(STM32F411xE)
/**
  * 简介:  配置PLLI2S时钟乘除因子。
  *
  * 注意:   该功能仅支持STM32F411xE设备。
  *
  * 注意:   该功能仅在禁用PLLI2S的情况下使用。
  * 注意:   PLLI2S时钟源与主锁相环共用(在RCC_PLLConfig函数中配置)
  *
  * 参数:  PLLI2SM: 指定PLLI2S VCO输入时钟的除法因子
  *         该参数必须为Min_Data = 2 ~ Max_Data = 63之间的数字。
  * 注意:   为了保证VCO输入频率在1 ~ 2mhz范围内，
  *         必须正确设置PLLI2SM参数。建议选择2mhz的频率，以限制PLLI2S抖动。
  *
  * 参数:  PLLI2SN: 指定PLLI2S VCO输出时钟的乘法因子
  *          必须为50 ~ 432之间的数字。
  * 注意:   必须正确设置PLLI2SN参数，以确保VCO输出频率在100 ~ 432mhz之间。
  *
  * 参数:  PLLI2SR: I2S时钟的除法因子
  *          此参数必须是介于2和7之间的数字。
  * 注意:   在I2S时钟频率上，必须将PLLI2SR参数正确设置为不超过192mhz。
  *
  * 返回值: 无
  */
void RCC_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SR, uint32_t PLLI2SM) {
    /* 检查参数 */
    assert_param(IS_RCC_PLLI2SN_VALUE(PLLI2SN));
    assert_param(IS_RCC_PLLI2SM_VALUE(PLLI2SM));
    assert_param(IS_RCC_PLLI2SR_VALUE(PLLI2SR));

    RCC->PLLI2SCFGR = (PLLI2SN << 6) | (PLLI2SR << 28) | PLLI2SM;
}
#endif /* STM32F411xE */

#if defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx)
/**
  * 简介:  配置PLLI2S时钟乘除因子。
  *
  * 注意:   此功能只能用于STM32F42xxx/43xxx设备
  *
  * 注意:   该功能仅在禁用PLLI2S的情况下使用。
  * 注意:   PLLI2S时钟源与主锁相环共用(在RCC_PLLConfig函数中配置)
  *
  * 参数:  PLLI2SN: 指定PLLI2S VCO输出时钟的乘法因子
  *          必须为50 ~ 432之间的数字。
  * 注意:   必须正确设置PLLI2SN参数，以确保VCO输出频率在100 ~ 432mhz之间。
  *
  * 参数:  PLLI2SQ: 指定SAI1时钟的分割因子
  *          此参数必须是介于2和15之间的数字。
  *
  * 参数:  PLLI2SR: I2S时钟的除法因子
  *          此参数必须是介于2和7之间的数字。
  * 注意:   在I2S时钟频率上，必须将PLLI2SR参数正确设置为不超过192mhz。
  *
  * 返回值: 无
  */
void RCC_PLLI2SConfig(uint32_t PLLI2SN, uint32_t PLLI2SQ, uint32_t PLLI2SR) {
    /* 检查参数 */
    assert_param(IS_RCC_PLLI2SN_VALUE(PLLI2SN));
    assert_param(IS_RCC_PLLI2SQ_VALUE(PLLI2SQ));
    assert_param(IS_RCC_PLLI2SR_VALUE(PLLI2SR));

    RCC->PLLI2SCFGR = (PLLI2SN << 6) | (PLLI2SQ << 24) | (PLLI2SR << 28);
}
#endif /* STM32F427_437xx || STM32F429_439xx || STM32F469_479xx */

#if defined(STM32F412xG ) || defined(STM32F413_423xx) || defined(STM32F446xx)
/**
  * 简介:  配置PLLI2S时钟乘除因子。
  *
  * 注意:   此功能只能用于STM32F446xx设备
  *
  * 注意:   该功能仅在禁用PLLI2S的情况下使用。
  * 注意:   PLLI2S时钟源与主锁相环共用(在RCC_PLLConfig函数中配置)
  *
  * 参数:  PLLI2SM: 指定PLLI2S VCO输入时钟的除法因子
  *         该参数必须为Min_Data = 2 ~ Max_Data = 63之间的数字。
  * 注意:   为了保证VCO输入频率在1 ~ 2mhz范围内，必须正确设置PLLI2SM参数。
  *         建议选择2mhz的频率，以限制PLLI2S抖动。
  *
  * 参数:  PLLI2SN: 指定PLLI2S VCO输出时钟的乘法因子
  *          必须为50 ~ 432之间的数字。
  * 注意:   必须正确设置PLLI2SN参数，以确保VCO输出频率在100 ~ 432mhz之间。
  *
  * 参数:  PLLI2SP: 指定PLL 48Mhz时钟输出的分频因数
  *          此参数必须是范围{2、4、6或8}中的数字。
  *
  * 参数:  PLLI2SQ: 指定SAI1时钟的分割因子
  *          此参数必须是介于2和15之间的数字。
  *
  * 参数:  PLLI2SR: I2S时钟的除法因子
  *          此参数必须是介于2和7之间的数字。
  * 注意:   在I2S时钟频率上，必须将PLLI2SR参数正确设置为不超过192mhz。
  * 注意:   PLLI2SR参数仅适用于STM32F42xxx/43xxx设备。
  *
  * 返回值: 无
  */
void RCC_PLLI2SConfig(uint32_t PLLI2SM, uint32_t PLLI2SN, uint32_t PLLI2SP, uint32_t PLLI2SQ, uint32_t PLLI2SR) {
    /* 检查参数 */
    assert_param(IS_RCC_PLLI2SM_VALUE(PLLI2SM));
    assert_param(IS_RCC_PLLI2SN_VALUE(PLLI2SN));
    assert_param(IS_RCC_PLLI2SP_VALUE(PLLI2SP));
    assert_param(IS_RCC_PLLI2SQ_VALUE(PLLI2SQ));
    assert_param(IS_RCC_PLLI2SR_VALUE(PLLI2SR));

    RCC->PLLI2SCFGR =  PLLI2SM | (PLLI2SN << 6) | (((PLLI2SP >> 1) - 1) << 16) | (PLLI2SQ << 24) | (PLLI2SR << 28);
}
#endif /* STM32F412xG || STM32F413_423xx || STM32F446xx */

/**
  * 简介:  启用或禁用PLLI2S.
  * 注意:   在进入STOP和STANDBY模式时，PLLI2S被硬件禁用。
  * 参数:  NewState: 新状态-> PLLI2S. 此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_PLLI2SCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    *(__IO uint32_t *) CR_PLLI2SON_BB = (uint32_t)NewState;
}

#if defined(STM32F469_479xx)
/**
  * 简介:  配置PLLSAI时钟乘法和除法因子。
  *
  * 注意:   此功能只能用于STM32F469_479xx设备
  *
  * 注意:   仅当PLLSAI被禁用时，才能使用此函数。
  * 注意:   PLLSAI时钟源与主PLL共用(在RCC_PLLConfig功能中配置)
  *
  * 参数:  PLLSAIN: 指定PLLSAI VCO输出时钟的倍增系数。此参数必须是50到432之间的数字。
  * 注意:   您必须正确设置PLLSAIN参数，以确保VCO输出频率在100到432 MHz之间。
  *
  * 参数:  PLLSAIP: 指定PLL 48Mhz时钟输出的分频因数
  *         此参数必须是范围{2、4、6或8}中的数字。.
  *
  * 参数:  PLLSAIQ: 指定SAI1时钟的分割因子
  *         此参数必须是介于2和15之间的数字。
  *
  * 参数:  PLLSAIR: 指定LTDC时钟的分割因子
  *          此参数必须是介于2和7之间的数字。
  *
  * 返回值: 无
  */
void RCC_PLLSAIConfig(uint32_t PLLSAIN, uint32_t PLLSAIP, uint32_t PLLSAIQ, uint32_t PLLSAIR) {
    /* 检查参数 */
    assert_param(IS_RCC_PLLSAIN_VALUE(PLLSAIN));
    assert_param(IS_RCC_PLLSAIP_VALUE(PLLSAIP));
    assert_param(IS_RCC_PLLSAIQ_VALUE(PLLSAIQ));
    assert_param(IS_RCC_PLLSAIR_VALUE(PLLSAIR));

    RCC->PLLSAICFGR = (PLLSAIN << 6) | (((PLLSAIP >> 1) - 1) << 16) | (PLLSAIQ << 24) | (PLLSAIR << 28);
}
#endif /* STM32F469_479xx */

#if defined(STM32F446xx)
/**
  * 简介:  配置PLLSAI时钟乘法和除法因子。
  *
  * 注意:   此功能只能用于STM32F446xx设备
  *
  * 注意:   仅当PLLSAI被禁用时，才能使用此函数。
  * 注意:   PLLSAI时钟源与主PLL共用(在RCC_PLLConfig功能中配置)
  *
  * 参数:  PLLSAIM: 指定PLLSAI VCO输入时钟的分割因子。此参数必须是介于Min_Data=2和Max_Data=63之间的数字。
  * 注意:   您必须正确设置PLLSAIM参数，以确保VCO输入频率范围为1到2 MHz。建议选择2 MHz的频率以限制PLLSAI抖动。
  *
  * 参数:  PLLSAIN: 指定PLLSAI VCO输出时钟的倍增系数。此参数必须是50到432之间的数字。
  * 注意:   您必须正确设置PLLSAIN参数，以确保VCO输出频率在100到432 MHz之间。
  *
  * 参数:  PLLSAIP: 指定PLL 48Mhz时钟输出的分频因数
  *         此参数必须是范围{2、4、6或8}中的数字。
  *
  * 参数:  PLLSAIQ: 指定SAI1时钟的分割因子
  *         此参数必须是介于2和15之间的数字。
  *
  * 返回值: 无
  */
void RCC_PLLSAIConfig(uint32_t PLLSAIM, uint32_t PLLSAIN, uint32_t PLLSAIP, uint32_t PLLSAIQ) {
    /* 检查参数 */
    assert_param(IS_RCC_PLLSAIM_VALUE(PLLSAIM));
    assert_param(IS_RCC_PLLSAIN_VALUE(PLLSAIN));
    assert_param(IS_RCC_PLLSAIP_VALUE(PLLSAIP));
    assert_param(IS_RCC_PLLSAIQ_VALUE(PLLSAIQ));

    RCC->PLLSAICFGR = PLLSAIM | (PLLSAIN << 6) | (((PLLSAIP >> 1) - 1) << 16)  | (PLLSAIQ << 24);
}
#endif /* STM32F446xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F411xE)
/**
  * 简介:  配置PLLSAI时钟乘法和除法因子。
  *
  * 注意:   此功能只能用于STM32F42xxx/43xxx设备
  *
  * 注意:   仅当PLLSAI被禁用时，才能使用此函数。
  * 注意:   PLLSAI时钟源与主PLL共用(在RCC_PLLConfig功能中配置)
  *
  * 参数:  PLLSAIN: 指定PLLSAI VCO输出时钟的倍增系数。此参数必须是50到432之间的数字。
  * 注意:   您必须正确设置PLLSAIN参数，以确保VCO输出频率在100到432 MHz之间。
  *
  * 参数:  PLLSAIQ: 指定SAI1时钟的分割因子
  *          此参数必须是介于2和15之间的数字。
  *
  * 参数:  PLLSAIR: 指定LTDC时钟的分割因子
  *          此参数必须是介于2和7之间的数字。
  *
  * 返回值: 无
  */
void RCC_PLLSAIConfig(uint32_t PLLSAIN, uint32_t PLLSAIQ, uint32_t PLLSAIR) {
    /* 检查参数 */
    assert_param(IS_RCC_PLLSAIN_VALUE(PLLSAIN));
    assert_param(IS_RCC_PLLSAIR_VALUE(PLLSAIR));
    assert_param(IS_RCC_PLLSAIQ_VALUE(PLLSAIQ));

    RCC->PLLSAICFGR = (PLLSAIN << 6) | (PLLSAIQ << 24) | (PLLSAIR << 28);
}
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F411xE */

/**
  * 简介:  启用或禁用PLLSAI.
  *
  * 注意:   该功能只支持STM32F42xxx/43xxx/446xx/469xx/479xx设备
  *
  * 注意:   在进入STOP和STANDBY模式时，PLLSAI被硬件禁用。
  * 参数:  NewState: 新状态-> PLLSAI. 此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_PLLSAICmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    *(__IO uint32_t *) CR_PLLSAION_BB = (uint32_t)NewState;
}

/**
  * 简介:  启用或禁用Clock Security System.
  * 注意:   如果在HSE振荡器时钟上检测到故障，该振荡器将自动禁用，
			并生成一个中断来通知软件故障(时钟安全系统中断，CSSI)，
			从而允许MCU执行救援操作。
			CSSI链接到Cortex-M4 NMI(非屏蔽中断)异常向量。
  * 参数:  NewState: 新状态-> Clock Security System.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_ClockSecuritySystemCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    *(__IO uint32_t *) CR_CSSON_BB = (uint32_t)NewState;
}

/**
  * 简介:  选择要在MCO1引脚(PA8)上输出的时钟源。
  * 注意:   应在备用功能模式下配置PA8。
  * 参数:  RCC_MCO1Source: 指定要输出的时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_MCO1Source_HSI: 选择HSI时钟作为MCO1源
  *            @arg RCC_MCO1Source_LSE: 选择LSE时钟作为MCO1源
  *            @arg RCC_MCO1Source_HSE: 选择HSE时钟作为MCO1源
  *            @arg RCC_MCO1Source_PLLCLK: 选择主PLL时钟作为MCO1源
  * 参数:  RCC_MCO1Div: 指定MCO1预分频器。
  *          此参数可以是以下值之一:
  *            @arg RCC_MCO1Div_1: MCO1时钟没有分频
  *            @arg RCC_MCO1Div_2: 除以2应用于MCO1时钟
  *            @arg RCC_MCO1Div_3: 除以3应用于MCO1时钟
  *            @arg RCC_MCO1Div_4: 除以4应用于MCO1时钟
  *            @arg RCC_MCO1Div_5: 除以5应用于MCO1时钟
  * 返回值: 无
  */
void RCC_MCO1Config(uint32_t RCC_MCO1Source, uint32_t RCC_MCO1Div) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_MCO1SOURCE(RCC_MCO1Source));
    assert_param(IS_RCC_MCO1DIV(RCC_MCO1Div));

    tmpreg = RCC->CFGR;

    /* 清除 MCO1[1:0] and MCO1PRE[2:0] 位 */
    tmpreg &= CFGR_MCO1_RESET_MASK;

    /* Select MCO1 clock source and prescaler */
    tmpreg |= RCC_MCO1Source | RCC_MCO1Div;

    /* 存储新值 */
    RCC->CFGR = tmpreg;

    #if defined(STM32F410xx)
    RCC_MCO1Cmd(ENABLE);
    #endif /* STM32F410xx */
}

/**
  * 简介:  选择要在MCO2引脚(PC9)上输出的时钟源。
  * 注意:   应在备用功能模式下配置PC9。
  * 参数:  RCC_MCO2Source: 指定要输出的时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_MCO2Source_SYSCLK: System clock (SYSCLK) selected as MCO2 source
  *            @arg RCC_MCO2SOURCE_PLLI2SCLK: PLLI2S clock selected as MCO2 source, available for all STM32F4 devices except STM32F410xx
  *            @arg RCC_MCO2SOURCE_I2SCLK: I2SCLK clock selected as MCO2 source, available only for STM32F410xx devices
  *            @arg RCC_MCO2Source_HSE: HSE clock selected as MCO2 source
  *            @arg RCC_MCO2Source_PLLCLK: main PLL clock selected as MCO2 source
  * 参数:  RCC_MCO2Div: specifies the MCO2 prescaler.
  *          此参数可以是以下值之一:
  *            @arg RCC_MCO2Div_1: no division applied to MCO2 clock
  *            @arg RCC_MCO2Div_2: division by 2 applied to MCO2 clock
  *            @arg RCC_MCO2Div_3: division by 3 applied to MCO2 clock
  *            @arg RCC_MCO2Div_4: division by 4 applied to MCO2 clock
  *            @arg RCC_MCO2Div_5: division by 5 applied to MCO2 clock
  * 注意:  For STM32F410xx devices to output I2SCLK clock on MCO2 you should have
  *        at last one of the SPI clocks enabled (SPI1, SPI2 or SPI5).
  * 返回值: 无
  */
void RCC_MCO2Config(uint32_t RCC_MCO2Source, uint32_t RCC_MCO2Div) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_MCO2SOURCE(RCC_MCO2Source));
    assert_param(IS_RCC_MCO2DIV(RCC_MCO2Div));

    tmpreg = RCC->CFGR;

    /* 清除 MCO2 and MCO2PRE[2:0] 位 */
    tmpreg &= CFGR_MCO2_RESET_MASK;

    /* Select MCO2 clock source and prescaler */
    tmpreg |= RCC_MCO2Source | RCC_MCO2Div;

    /* 存储新值 */
    RCC->CFGR = tmpreg;

    #if defined(STM32F410xx)
    RCC_MCO2Cmd(ENABLE);
    #endif /* STM32F410xx */
}

/**
  * @}
  */

/** @defgroup RCC_Group2 系统、AHB和APB总线时钟配置功能
 *  简介   系统、AHB和APB总线时钟配置功能
 *
@verbatim
 ===============================================================================
      ##### 系统、AHB和APB总线时钟配置功能 #####
 ===============================================================================
    [..]
      本节提供的功能允许配置系统，AHB，APB1和APB2 总线时钟。

      (#) 多个时钟源可用于驱动系统时钟(SYSCLK):HSI、HSE和PLL。
          AHB时钟(HCLK)由系统时钟通过可配置的预分频器派生而来，用于为映射在AHB总线(DMA, GPIO…)
          上的CPU，内存和外设计时。APB1 (PCLK1)和APB2 (PCLK2)时钟是由AHB时钟通过可配置的预分频器派生出来的，
          用于为映射在这些总线上的外围设备计时。您可以使用“RCC_GetClocksFreq()”函数来检索这些时钟的频率。

      -@- 所有的外围时钟都来源于系统时钟(SYSCLK)，除了:
        (+@) I2S: I2S时钟可以从特定的锁相环(PLLI2S)或映射在I2S_CKIN引脚上的外部时钟派生。
             您必须使用RCC_I2SCLKConfig()函数来配置此时钟。
        (+@) RTC: RTC时钟可由LSI、LSE或HSE时钟除以2 ~ 31得到。
             你必须使用RCC_RTCCLKConfig()和RCC_RTCCLKCmd()函数来配置这个时钟。
        (+@) USB OTG FS, SDIO和RTC: USB OTG FS需要等于48 MHz的频率才能正常工作，
             而SDIO需要等于或低于48的频率。该时钟由主锁相环通过锁相环q分频器派生而来。
        (+@) IWDG时钟，总是LSI时钟。

      (#) 对于STM32F405xx/407xx和STM32F415xx/417xx设备，SYSCLK和HCLK的最大频率为168 MHz,
          PCLK2为84 MHz, PCLK1为42 MHz。根据设备电压范围的不同，应相应地调整最大频率:
 +-------------------------------------------------------------------------------------+
 | Latency       |                HCLK clock frequency (MHz)                           |
 |               |---------------------------------------------------------------------|
 |               |     电压范围   |    电压范围     | voltage range   | voltage range   |
 |               | 2.7 V - 3.6 V  | 2.4 V - 2.7 V  | 2.1 V - 2.4 V   | 1.8 V - 2.1 V   |
 |---------------|----------------|----------------|-----------------|-----------------|
 |0WS(1CPU cycle)|0 < HCLK <= 30  |0 < HCLK <= 24  |0 < HCLK <= 22   |0 < HCLK <= 20   |
 |---------------|----------------|----------------|-----------------|-----------------|
 |1WS(2CPU cycle)|30 < HCLK <= 60 |24 < HCLK <= 48 |22 < HCLK <= 44  |20 < HCLK <= 40  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |2WS(3CPU cycle)|60 < HCLK <= 90 |48 < HCLK <= 72 |44 < HCLK <= 66  |40 < HCLK <= 60  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |3WS(4CPU cycle)|90 < HCLK <= 120|72 < HCLK <= 96 |66 < HCLK <= 88  |60 < HCLK <= 80  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |4WS(5CPU cycle)|120< HCLK <= 150|96 < HCLK <= 120|88 < HCLK <= 110 |80 < HCLK <= 100 |
 |---------------|----------------|----------------|-----------------|-----------------|
 |5WS(6CPU cycle)|150< HCLK <= 168|120< HCLK <= 144|110 < HCLK <= 132|100 < HCLK <= 120|
 |---------------|----------------|----------------|-----------------|-----------------|
 |6WS(7CPU cycle)|      NA        |144< HCLK <= 168|132 < HCLK <= 154|120 < HCLK <= 140|
 |---------------|----------------|----------------|-----------------|-----------------|
 |7WS(8CPU cycle)|      NA        |      NA        |154 < HCLK <= 168|140 < HCLK <= 160|
 +---------------|----------------|----------------|-----------------|-----------------+
      (#) For STM32F42xxx/43xxx/469xx/479xx devices, the maximum frequency of the SYSCLK and HCLK is 180 MHz,
          PCLK2 90 MHz and PCLK1 45 MHz. Depending on the device voltage range, the maximum
          frequency should be adapted accordingly:
 +-------------------------------------------------------------------------------------+
 | Latency       |                HCLK clock frequency (MHz)                           |
 |               |---------------------------------------------------------------------|
 |               |     电压范围   |    电压范围     | voltage range   | voltage range   |
 |               | 2.7 V - 3.6 V  | 2.4 V - 2.7 V  | 2.1 V - 2.4 V   | 1.8 V - 2.1 V   |
 |---------------|----------------|----------------|-----------------|-----------------|
 |0WS(1CPU cycle)|0 < HCLK <= 30  |0 < HCLK <= 24  |0 < HCLK <= 22   |0 < HCLK <= 20   |
 |---------------|----------------|----------------|-----------------|-----------------|
 |1WS(2CPU cycle)|30 < HCLK <= 60 |24 < HCLK <= 48 |22 < HCLK <= 44  |20 < HCLK <= 40  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |2WS(3CPU cycle)|60 < HCLK <= 90 |48 < HCLK <= 72 |44 < HCLK <= 66  |40 < HCLK <= 60  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |3WS(4CPU cycle)|90 < HCLK <= 120|72 < HCLK <= 96 |66 < HCLK <= 88  |60 < HCLK <= 80  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |4WS(5CPU cycle)|120< HCLK <= 150|96 < HCLK <= 120|88 < HCLK <= 110 |80 < HCLK <= 100 |
 |---------------|----------------|----------------|-----------------|-----------------|
 |5WS(6CPU cycle)|120< HCLK <= 180|120< HCLK <= 144|110 < HCLK <= 132|100 < HCLK <= 120|
 |---------------|----------------|----------------|-----------------|-----------------|
 |6WS(7CPU cycle)|      NA        |144< HCLK <= 168|132 < HCLK <= 154|120 < HCLK <= 140|
 |---------------|----------------|----------------|-----------------|-----------------|
 |7WS(8CPU cycle)|      NA        |168< HCLK <= 180|154 < HCLK <= 176|140 < HCLK <= 160|
 |---------------|----------------|----------------|-----------------|-----------------|
 |8WS(9CPU cycle)|      NA        |      NA        |176 < HCLK <= 180|160 < HCLK <= 168|
 +-------------------------------------------------------------------------------------+

      (#) For STM32F401xx devices, the maximum frequency of the SYSCLK and HCLK is 84 MHz,
          PCLK2 84 MHz and PCLK1 42 MHz. Depending on the device voltage range, the maximum
          frequency should be adapted accordingly:
 +-------------------------------------------------------------------------------------+
 | Latency       |                HCLK clock frequency (MHz)                           |
 |               |---------------------------------------------------------------------|
 |               |     电压范围   |    电压范围     | voltage range   | voltage range   |
 |               | 2.7 V - 3.6 V  | 2.4 V - 2.7 V  | 2.1 V - 2.4 V   | 1.8 V - 2.1 V   |
 |---------------|----------------|----------------|-----------------|-----------------|
 |0WS(1CPU cycle)|0 < HCLK <= 30  |0 < HCLK <= 24  |0 < HCLK <= 22   |0 < HCLK <= 20   |
 |---------------|----------------|----------------|-----------------|-----------------|
 |1WS(2CPU cycle)|30 < HCLK <= 60 |24 < HCLK <= 48 |22 < HCLK <= 44  |20 < HCLK <= 40  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |2WS(3CPU cycle)|60 < HCLK <= 84 |48 < HCLK <= 72 |44 < HCLK <= 66  |40 < HCLK <= 60  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |3WS(4CPU cycle)|      NA        |72 < HCLK <= 84 |66 < HCLK <= 84  |60 < HCLK <= 80  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |4WS(5CPU cycle)|      NA        |      NA        |      NA         |80 < HCLK <= 84  |
 +-------------------------------------------------------------------------------------+

      (#) For STM32F410xx/STM32F411xE devices, the maximum frequency of the SYSCLK and HCLK is 100 MHz,
          PCLK2 100 MHz and PCLK1 50 MHz. Depending on the device voltage range, the maximum
          frequency should be adapted accordingly:
 +-------------------------------------------------------------------------------------+
 | Latency       |                HCLK clock frequency (MHz)                           |
 |               |---------------------------------------------------------------------|
 |               |     电压范围   |    电压范围     | voltage range   | voltage range   |
 |               | 2.7 V - 3.6 V  | 2.4 V - 2.7 V  | 2.1 V - 2.4 V   | 1.8 V - 2.1 V   |
 |---------------|----------------|----------------|-----------------|-----------------|
 |0WS(1CPU cycle)|0 < HCLK <= 30  |0 < HCLK <= 24  |0 < HCLK <= 18   |0 < HCLK <= 16   |
 |---------------|----------------|----------------|-----------------|-----------------|
 |1WS(2CPU cycle)|30 < HCLK <= 64 |24 < HCLK <= 48 |18 < HCLK <= 36  |16 < HCLK <= 32  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |2WS(3CPU cycle)|64 < HCLK <= 90 |48 < HCLK <= 72 |36 < HCLK <= 54  |32 < HCLK <= 48  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |3WS(4CPU cycle)|90 < HCLK <= 100|72 < HCLK <= 96 |54 < HCLK <= 72  |48 < HCLK <= 64  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |4WS(5CPU cycle)|      NA        |96 < HCLK <= 100|72 < HCLK <= 90  |64 < HCLK <= 80  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |5WS(6CPU cycle)|      NA        |       NA       |90 < HCLK <= 100 |80 < HCLK <= 96  |
 |---------------|----------------|----------------|-----------------|-----------------|
 |6WS(7CPU cycle)|      NA        |       NA       |        NA       |96 < HCLK <= 100 |
 +-------------------------------------------------------------------------------------+

      -@- On STM32F405xx/407xx and STM32F415xx/417xx devices:
           (++) when VOS = '0', 最大值 fHCLK = 144MHz.
           (++) when VOS = '1', 最大值 fHCLK = 168MHz.
          [..]
          On STM32F42xxx/43xxx/469xx/479xx devices:
           (++) when VOS[1:0] = '0x01', fHCLK的最大值为120MHz。
           (++) when VOS[1:0] = '0x10', 最大值 fHCLK is 144MHz.
           (++) when VOS[1:0] = '0x11', 最大值 f  is 168MHz
          [..]
          On STM32F401x devices:
           (++) when VOS[1:0] = '0x01', fHCLK的最大值为64MHz。
           (++) when VOS[1:0] = '0x10', fHCLK的最大值为84MHz。
          On STM32F410xx/STM32F411xE devices:
           (++) when VOS[1:0] = '0x01' fHCLK的最大值为64MHz。
           (++) when VOS[1:0] = '0x10' fHCLK的最大值为84MHz。
           (++) when VOS[1:0] = '0x11' fHCLK的最大值为100MHz。

       You can use PWR_MainRegulatorModeConfig() function to control VOS bits.

@endverbatim
  * @{
  */

/**
  * 简介:  配置系统时钟 (SYSCLK)。
  * 注意:   HSI在Reset模式启动、STOP模式唤醒和STANDBY模式唤醒后作为系统时钟源(硬件开启)，
  *         或者在HSE故障时直接或间接作为系统时钟(时钟安全系统CSS开启)。
  * 注意:   从一个时钟源切换到另一个时钟源只有在目标时钟源准备就绪(启动延迟或锁相锁紧后时钟稳定)时才会发生。
  *          如果选择了一个尚未就绪的时钟源，则在时钟源准备就绪时进行切换。
  *         您可以使用RCC_GetSYSCLKSource()函数来了解当前使用哪个时钟作为系统时钟源。
  * 参数:  RCC_SYSCLKSource: 指定用作系统时钟的时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_SYSCLKSource_HSI: HSI selected as system clock source
  *            @arg RCC_SYSCLKSource_HSE: HSE selected as system clock source
  *            @arg RCC_SYSCLKSource_PLLCLK: PLL selected as system clock source (RCC_SYSCLKSource_PLLPCLK for STM32F446xx devices)
  *            @arg RCC_SYSCLKSource_PLLRCLK: PLL R selected as system clock source only for STM32F412xG, STM32F413_423xx and STM32F446xx devices
  * 返回值: 无
  */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));

    tmpreg = RCC->CFGR;

    /* 清除 SW[1:0] 位 */
    tmpreg &= ~RCC_CFGR_SW;

    /* Set SW[1:0] bits according to RCC_SYSCLKSource 值 */
    tmpreg |= RCC_SYSCLKSource;

    /* 存储新值 */
    RCC->CFGR = tmpreg;
}

/**
  * 简介:  返回用作系统时钟的时钟源。
  * 参数:  无
  * 返回值: 用作系统时钟的时钟源。返回的值可以是以下值之一:
  *              - 0x00: HSI used as system clock
  *              - 0x04: HSE used as system clock
  *              - 0x08: PLL used as system clock (PLL P for STM32F446xx devices)
  *              - 0x0C: PLL R用作系统时钟(仅适用于STM32F412xG、STM32F413_423xx和STM32F446xx设备)
  */
uint8_t RCC_GetSYSCLKSource(void) {
    return ((uint8_t)(RCC->CFGR & RCC_CFGR_SWS));
}

/**
  * 简介:  配置 AHB 时钟 (HCLK)。
  * 注意:   根据设备电压范围，软件必须正确设置这些位，
  *         以确保HCLK不超过允许的最大频率
  *         (有关更多详细信息，请参阅上文“CPU、AHB和APB总线时钟配置功能")
  * 参数:  RCC_SYSCLK: 定义AHB时钟分频器。该时钟源自系统时钟(SYSCLK)。
  *          此参数可以是以下值之一:
  *            @arg RCC_SYSCLK_Div1: AHB clock = SYSCLK
  *            @arg RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
  *            @arg RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
  *            @arg RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
  *            @arg RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
  *            @arg RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
  *            @arg RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
  *            @arg RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
  *            @arg RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
  * 返回值: 无
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_HCLK(RCC_SYSCLK));

    tmpreg = RCC->CFGR;

    /* 清除 HPRE[3:0] 位 */
    tmpreg &= ~RCC_CFGR_HPRE;

    /* Set HPRE[3:0] bits according to RCC_SYSCLK 值 */
    tmpreg |= RCC_SYSCLK;

    /* 存储新值 */
    RCC->CFGR = tmpreg;
}

/**
  * 简介:  配置低速 APB 时钟 (PCLK1)。
  * 参数:  RCC_HCLK: 定义APB1时钟分频器。该时钟源自AHB时钟(HCLK)。
  *          此参数可以是以下值之一:
  *            @arg RCC_HCLK_Div1:  APB1 clock = HCLK
  *            @arg RCC_HCLK_Div2:  APB1 clock = HCLK/2
  *            @arg RCC_HCLK_Div4:  APB1 clock = HCLK/4
  *            @arg RCC_HCLK_Div8:  APB1 clock = HCLK/8
  *            @arg RCC_HCLK_Div16: APB1 clock = HCLK/16
  * 返回值: 无
  */
void RCC_PCLK1Config(uint32_t RCC_HCLK) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_PCLK(RCC_HCLK));

    tmpreg = RCC->CFGR;

    /* 清除 PPRE1[2:0] 位 */
    tmpreg &= ~RCC_CFGR_PPRE1;

    /* Set PPRE1[2:0] bits according to RCC_HCLK 值 */
    tmpreg |= RCC_HCLK;

    /* 存储新值 */
    RCC->CFGR = tmpreg;
}

/**
  * 简介: 配置高速 APB 时钟 (PCLK2)。
  * 参数:  RCC_HCLK: 定义APB2时钟分频器。该时钟源自AHB时钟(HCLK)。
  *          此参数可以是以下值之一:
  *            @arg RCC_HCLK_Div1:  APB2 clock = HCLK
  *            @arg RCC_HCLK_Div2:  APB2 clock = HCLK/2
  *            @arg RCC_HCLK_Div4:  APB2 clock = HCLK/4
  *            @arg RCC_HCLK_Div8:  APB2 clock = HCLK/8
  *            @arg RCC_HCLK_Div16: APB2 clock = HCLK/16
  * 返回值: 无
  */
void RCC_PCLK2Config(uint32_t RCC_HCLK) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_PCLK(RCC_HCLK));

    tmpreg = RCC->CFGR;

    /* 清除 PPRE2[2:0] 位 */
    tmpreg &= ~RCC_CFGR_PPRE2;

    /* Set PPRE2[2:0] bits according to RCC_HCLK 值 */
    tmpreg |= RCC_HCLK << 3;

    /* 存储新值 */
    RCC->CFGR = tmpreg;
}

/**
  * 简介:  返回不同片上时钟的频率;SYSCLK，HCLK，PCLK1和PCLK2。
  *
  * 注意:   此函数计算的系统频率不是芯片中的实际频率。
  *         它是根据预定义的常数和选定的时钟源计算的:
  * 注意:     如果SYSCLK源是HSI，则函数返回基于HSI_VALUE(*)的值
  * 注意:     如果SYSCLK源是HSE，则函数返回基于HSE_VALUE(**)的值
  * 注意:     如果SYSCLK源是PLL，则函数返回基于HSE_VALUE(**)或HSI_VALE(*)乘以/除以PLL因子的值。
  * 注意:     (*) HSI_VALUE是stm32f44xx.h文件中定义的常量(默认值为16 MHz)，
  *              但实际值可能会根据电压和温度的变化而变化。
  * 注意:     (**) HSE_VALUE是stm32f4xx.h文件中定义的常数(默认值25 MHz)，
  *              用户必须确保HSE_VALU与所用晶体的实际频率相同。
  *              否则，此函数可能会产生错误的结果。
  *
  * 注意:   当使用HSE晶体的分数值时，该函数的结果可能不正确。
  *
  * 参数:  RCC_Clocks: 指向RCC_ClocksTypeDef结构的指针，该结构将保持时钟频率。
  *
  * 注意:   用户应用程序可以使用此功能来计算通信外设的波特率或配置其他参数。
  * 注意:   每当SYSCLK、HCLK、PCLK1和/或PCLK2时钟发生变化时，
  *         必须调用此函数来更新结构的字段。否则，基于此功能的任何配置都将不正确。
  *
  * 返回值: 无
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks) {
    uint32_t tmp = 0, presc = 0, pllvco = 0, pllp = 2, pllsource = 0, pllm = 2;
    #if defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
    uint32_t pllr = 2;
    #endif /* STM32F412xG || STM32F413_423xx || STM32F446xx */

    /* Get SYSCLK source -------------------------------------------------------*/
    tmp = RCC->CFGR & RCC_CFGR_SWS;

    switch (tmp) {
        case 0x00:  /* HSI used as system clock source */
            RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
            break;

        case 0x04:  /* HSE used as system clock  source */
            RCC_Clocks->SYSCLK_Frequency = HSE_VALUE;
            break;

        case 0x08:  /* PLL P used as system clock  source */

            /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
            SYSCLK = PLL_VCO / PLLP
            */
            pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
            pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

            if (pllsource != 0) {
                /* HSE used as PLL clock source */
                pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
            } else {
                /* HSI used as PLL clock source */
                pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
            }

            pllp = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLP) >> 16) + 1 ) * 2;
            RCC_Clocks->SYSCLK_Frequency = pllvco / pllp;
            break;

            #if defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)

        case 0x0C:  /* PLL R used as system clock  source */
            /* PLL_VCO = (HSE_VALUE or HSI_VALUE / PLLM) * PLLN
            SYSCLK = PLL_VCO / PLLR
            */
            pllsource = (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) >> 22;
            pllm = RCC->PLLCFGR & RCC_PLLCFGR_PLLM;

            if (pllsource != 0) {
                /* HSE used as PLL clock source */
                pllvco = (HSE_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
            } else {
                /* HSI used as PLL clock source */
                pllvco = (HSI_VALUE / pllm) * ((RCC->PLLCFGR & RCC_PLLCFGR_PLLN) >> 6);
            }

            pllr = (((RCC->PLLCFGR & RCC_PLLCFGR_PLLR) >> 28) + 1 ) * 2;
            RCC_Clocks->SYSCLK_Frequency = pllvco / pllr;
            break;
            #endif /* STM32F412xG || STM32F413_423xx || STM32F446xx */

        default:
            RCC_Clocks->SYSCLK_Frequency = HSI_VALUE;
            break;
    }

    /* Compute HCLK, PCLK1 and PCLK2 clocks frequencies ------------------------*/

    /* Get HCLK prescaler */
    tmp = RCC->CFGR & RCC_CFGR_HPRE;
    tmp = tmp >> 4;
    presc = APBAHBPrescTable[tmp];
    /* HCLK clock frequency */
    RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;

    /* Get PCLK1 prescaler */
    tmp = RCC->CFGR & RCC_CFGR_PPRE1;
    tmp = tmp >> 10;
    presc = APBAHBPrescTable[tmp];
    /* PCLK1 clock frequency */
    RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;

    /* Get PCLK2 prescaler */
    tmp = RCC->CFGR & RCC_CFGR_PPRE2;
    tmp = tmp >> 13;
    presc = APBAHBPrescTable[tmp];
    /* PCLK2 clock frequency */
    RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
}

/**
  * @}
  */

/** @defgroup RCC_Group3 外围时钟配置函数
 *  简介   外围时钟配置函数
 *
@verbatim
 ===============================================================================
              ##### 外围时钟配置函数 #####
 ===============================================================================
    [..] 本节提供允许配置外围时钟的功能。

      (#) 从LSI、LSE或HSE时钟导出的RTC时钟除以2至31。

      (#) 从Reset(重置)重新启动或从STANDBY(待机)唤醒后，除内部SRAM、Flash和JTAG外，
          所有外围设备均关闭。在开始使用外围设备之前，您必须启用其接口时钟。
          可以使用RCC_AHBPeriphClockCmd()、RCC_APB2PeriphClockCmd()和RCC_APB1PeriphClock Cmd(函数执行此操作。

      (#) 要重置外围设备配置(设备重置后为默认状态)，可以使用
          RCC_AHBPeriphResetCmd()、RCC_APB2PeriphResetCmd()和RCC_APB1PeriphResedCmd()函数。

      (#) 为了进一步降低SLEEP模式下的功耗，可以在执行WFI或WFE指令之前禁用外围时钟。
          可以使用RCC_AHBPeriphClockLPModeCmd()、RCC_APB2PeriphClockLP ModeCmd)和RCC_APB1PeriphClock LPModeCmd()函数执行此操作。

@endverbatim
  * @{
  */

/**
  * 简介:  配置 RTC 时钟 (RTCCLK)。
  * 注意:   由于RTC时钟配置位在备份域中，并且重置后对此域的写入访问被拒绝，
  *         因此您必须在配置RTC时钟源之前使用PWR_BackupAccessCmd(enable)功能启用写入访问(重置后执行一次)。
  * 注意:   配置RTC时钟后，除非使用RCC_BackupResetCmd()函数或通电重置(POR)重置备份域，否则无法更改。
  *
  * 参数:  RCC_RTCCLKSource: 指定RTC时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_RTCCLKSource_LSE: LSE selected as RTC clock
  *            @arg RCC_RTCCLKSource_LSI: LSI selected as RTC clock
  *            @arg RCC_RTCCLKSource_HSE_Divx: HSE clock divided by x selected
  *                                            as RTC clock, where x:[2,31]
  *
  * 注意:   如果LSE或LSI用作RTC时钟源，RTC将继续在STOP和STANDBY模式下工作，并可用作唤醒源。
  *          然而，当HSE时钟用作RTC时钟源时，RTC不能在STOP和STANDBY模式下使用。
  * 注意:   RTC的最大输入时钟频率为1MHz(当使用HSE作为RTC时钟源时)。
  *
  * 返回值: 无
  */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));

    if ((RCC_RTCCLKSource & 0x00000300) == 0x00000300) {
        /* If HSE is selected as RTC clock source, configure HSE division factor for RTC clock */
        tmpreg = RCC->CFGR;

        /* 清除 RTCPRE[4:0] 位 */
        tmpreg &= ~RCC_CFGR_RTCPRE;

        /* Configure HSE division factor for RTC clock */
        tmpreg |= (RCC_RTCCLKSource & 0xFFFFCFF);

        /* 存储新值 */
        RCC->CFGR = tmpreg;
    }

    /* 选择这个RTC clock source */
    RCC->BDCR |= (RCC_RTCCLKSource & 0x00000FFF);
}

/**
  * 简介:  启用或禁用RTC clock.
  * 注意:   This function must be used only after the RTC clock source was selected
  *         using the RCC_RTCCLKConfig function.
  * 参数:  NewState: 新状态-> RTC clock. 此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_RTCCLKCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) BDCR_RTCEN_BB = (uint32_t)NewState;
}

/**
  * 简介:  强制或释放备份域重置。
  * 注意:   This function resets the RTC peripheral (including the backup registers)
  *         and the RTC clock source selection in RCC_CSR register.
  * 注意:   The BKPSRAM is not affected by this reset.
  * 参数:  NewState: 新状态-> Backup domain reset.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_BackupResetCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));
    *(__IO uint32_t *) BDCR_BDRST_BB = (uint32_t)NewState;
}

#if defined (STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
/**
  * 简介:  Configures the I2S clock source (I2SCLK).
  * 注意:   This function must be called before enabling the I2S APB clock.
  *
  * 参数:  RCC_I2SAPBx: specifies the APBx I2S clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_I2SBus_APB1: I2S peripheral instance is on APB1 Bus
  *            @arg RCC_I2SBus_APB2: I2S peripheral instance is on APB2 Bus
  *
  * 参数:  RCC_I2SCLKSource: specifies the I2S clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_I2SCLKSource_PLLI2S: PLLI2S clock used as I2S clock source
  *            @arg RCC_I2SCLKSource_Ext: External clock mapped on the I2S_CKIN pin
  *                                        used as I2S clock source
  *            @arg RCC_I2SCLKSource_PLL: PLL clock used as I2S clock source
  *            @arg RCC_I2SCLKSource_HSI_HSE: HSI or HSE depends on PLLSRC used as I2S clock source
  * 返回值: 无
  */
void RCC_I2SCLKConfig(uint32_t RCC_I2SAPBx, uint32_t RCC_I2SCLKSource) {
    /* 检查参数 */
    assert_param(IS_RCC_I2SCLK_SOURCE(RCC_I2SCLKSource));
    assert_param(IS_RCC_I2S_APBx(RCC_I2SAPBx));

    if(RCC_I2SAPBx == RCC_I2SBus_APB1) {
        /* 清除 APB1 I2Sx clock source selection 位 */
        RCC->DCKCFGR &= ~RCC_DCKCFGR_I2S1SRC;
        /* Set new APB1 I2Sx clock source*/
        RCC->DCKCFGR |= RCC_I2SCLKSource;
    } else {
        /* 清除 APB2 I2Sx clock source selection  位 */
        RCC->DCKCFGR &= ~RCC_DCKCFGR_I2S2SRC;
        /* Set new APB2 I2Sx clock source */
        RCC->DCKCFGR |= (RCC_I2SCLKSource << 2);
    }
}
#if defined(STM32F446xx)
/**
  * 简介:  Configures the SAIx clock source (SAIxCLK).
  * 注意:   This function must be called before enabling the SAIx APB clock.
  *
  * 参数:  RCC_SAIInstance: specifies the SAIx clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_SAIInstance_SAI1: SAI1 clock source selection
  *            @arg RCC_SAIInstance_SAI2: SAI2 clock source selections
  *
  * 参数:  RCC_SAICLKSource: specifies the SAI clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_SAICLKSource_PLLSAI: PLLSAI clock used as SAI clock source
  *            @arg RCC_SAICLKSource_PLLI2S: PLLI2S clock used as SAI clock source
  *            @arg RCC_SAICLKSource_PLL: PLL clock used as SAI clock source
  *            @arg RCC_SAICLKSource_HSI_HSE: HSI or HSE depends on PLLSRC used as SAI clock source
  * 返回值: 无
  */
void RCC_SAICLKConfig(uint32_t RCC_SAIInstance, uint32_t RCC_SAICLKSource) {
    /* 检查参数 */
    assert_param(IS_RCC_SAICLK_SOURCE(RCC_SAICLKSource));
    assert_param(IS_RCC_SAI_INSTANCE(RCC_SAIInstance));

    if(RCC_SAIInstance == RCC_SAIInstance_SAI1) {
        /* 清除 SAI1 clock source selection 位 */
        RCC->DCKCFGR &= ~RCC_DCKCFGR_SAI1SRC;
        /* Set new SAI1 clock source */
        RCC->DCKCFGR |= RCC_SAICLKSource;
    } else {
        /* 清除 SAI2 clock source selection 位 */
        RCC->DCKCFGR &= ~RCC_DCKCFGR_SAI2SRC;
        /* Set new SAI2 clock source */
        RCC->DCKCFGR |= (RCC_SAICLKSource << 2);
    }
}
#endif /* STM32F446xx */

#if defined(STM32F413_423xx)
/**
  * 简介:  Configures SAI1BlockA clock source selection.
  * 注意:   This function must be called before enabling PLLSAI, PLLI2S and
  *         the SAI clock.
  * 参数:  RCC_SAIBlockACLKSource: specifies the SAI Block A clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_SAIACLKSource_PLLI2SR: PLLI2SR clock used as SAI clock source
  *            @arg RCC_SAIACLKSource_PLLI2S: PLLI2S clock used as SAI clock source
  *            @arg RCC_SAIACLKSource_PLL: PLL clock used as SAI clock source
  *            @arg RCC_SAIACLKSource_HSI_HSE: HSI or HSE depends on PLLSRC used as SAI clock source
  * 返回值: 无
  */
void RCC_SAIBlockACLKConfig(uint32_t RCC_SAIBlockACLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_SAIACLK_SOURCE(RCC_SAIBlockACLKSource));

    tmpreg = RCC->DCKCFGR;

    /* 清除 RCC_DCKCFGR_SAI1ASRC[1:0] 位 */
    tmpreg &= ~RCC_DCKCFGR_SAI1ASRC;

    /* Set SAI Block A source selection 值 */
    tmpreg |= RCC_SAIBlockACLKSource;

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}

/**
  * 简介:  Configures SAI1BlockB clock source selection.
  * 注意:   This function must be called before enabling PLLSAI, PLLI2S and
  *         the SAI clock.
  * 参数:  RCC_SAIBlockBCLKSource: specifies the SAI Block B clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_SAIBCLKSource_PLLI2SR: PLLI2SR clock used as SAI clock source
  *            @arg RCC_SAIBCLKSource_PLLI2S: PLLI2S clock used as SAI clock source
  *            @arg RCC_SAIBCLKSource_PLL: PLL clock used as SAI clock source
  *            @arg RCC_SAIBCLKSource_HSI_HSE: HSI or HSE depends on PLLSRC used as SAI clock source
  * 返回值: 无
  */
void RCC_SAIBlockBCLKConfig(uint32_t RCC_SAIBlockBCLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_SAIBCLK_SOURCE(RCC_SAIBlockBCLKSource));

    tmpreg = RCC->DCKCFGR;

    /* 清除 RCC_DCKCFGR_SAI1ASRC[1:0] 位 */
    tmpreg &= ~RCC_DCKCFGR_SAI1BSRC;

    /* Set SAI Block B source selection 值 */
    tmpreg |= RCC_SAIBlockBCLKSource;

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}
#endif /* STM32F413_423xx */
#endif /* STM32F412xG || STM32F413_423xx || STM32F446xx */

#if defined(STM32F410xx)
/**
  * 简介:  Configures the I2S clock source (I2SCLK).
  * 注意:   This function must be called before enabling the I2S clock.
  *
  * 参数:  RCC_I2SCLKSource: specifies the I2S clock source.
  *         此参数可以是以下值之一:
  *            @arg RCC_I2SAPBCLKSOURCE_PLLR: PLL VCO output clock divided by PLLR.
  *            @arg RCC_I2SAPBCLKSOURCE_EXT: External clock mapped on the I2S_CKIN pin.
  *            @arg RCC_I2SAPBCLKSOURCE_PLLSRC: HSI/HSE depends on PLLSRC.
  * 返回值: 无
  */
void RCC_I2SCLKConfig(uint32_t RCC_I2SCLKSource) {
    /* 检查参数 */
    assert_param(IS_RCC_I2SCLK_SOURCE(RCC_I2SCLKSource));

    /* 清除 I2Sx clock source selection 位 */
    RCC->DCKCFGR &= ~RCC_DCKCFGR_I2SSRC;
    /* Set new I2Sx clock source*/
    RCC->DCKCFGR |= RCC_I2SCLKSource;
}
#endif /* STM32F410xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F401xx) || defined(STM32F411xE) || defined(STM32F469_479xx)
/**
  * 简介:  Configures the I2S clock source (I2SCLK).
  * 注意:   This function must be called before enabling the I2S APB clock.
  * 参数:  RCC_I2SCLKSource: specifies the I2S clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_I2S2CLKSource_PLLI2S: PLLI2S clock used as I2S clock source
  *            @arg RCC_I2S2CLKSource_Ext: External clock mapped on the I2S_CKIN pin
  *                                        used as I2S clock source
  * 返回值: 无
  */
void RCC_I2SCLKConfig(uint32_t RCC_I2SCLKSource) {
    /* 检查参数 */
    assert_param(IS_RCC_I2SCLK_SOURCE(RCC_I2SCLKSource));

    *(__IO uint32_t *) CFGR_I2SSRC_BB = RCC_I2SCLKSource;
}
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F401xx || STM32F411xE || STM32F469_479xx */

#if defined(STM32F40_41xxx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F469_479xx)
/**
  * 简介:  Configures SAI1BlockA clock source selection.
  *
  * 注意:   This function can be used only for STM32F42xxx/43xxx/469xx/479xx devices.
  *
  * 注意:   This function must be called before enabling PLLSAI, PLLI2S and
  *         the SAI clock.
  * 参数:  RCC_SAIBlockACLKSource: specifies the SAI Block A clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_SAIACLKSource_PLLI2S: PLLI2S_Q clock divided by PLLI2SDIVQ used
  *                                           as SAI1 Block A clock
  *            @arg RCC_SAIACLKSource_PLLSAI: PLLISAI_Q clock divided by PLLSAIDIVQ used
  *                                           as SAI1 Block A clock
  *            @arg RCC_SAIACLKSource_Ext: External clock mapped on the I2S_CKIN pin
  *                                        used as SAI1 Block A clock
  * 返回值: 无
  */
void RCC_SAIBlockACLKConfig(uint32_t RCC_SAIBlockACLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_SAIACLK_SOURCE(RCC_SAIBlockACLKSource));

    tmpreg = RCC->DCKCFGR;

    /* 清除 RCC_DCKCFGR_SAI1ASRC[1:0] 位 */
    tmpreg &= ~RCC_DCKCFGR_SAI1ASRC;

    /* Set SAI Block A source selection 值 */
    tmpreg |= RCC_SAIBlockACLKSource;

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}

/**
  * 简介:  Configures SAI1BlockB clock source selection.
  *
  * 注意:   This function can be used only for STM32F42xxx/43xxx/469xx/479xx devices.
  *
  * 注意:   This function must be called before enabling PLLSAI, PLLI2S and
  *         the SAI clock.
  * 参数:  RCC_SAIBlockBCLKSource: specifies the SAI Block B clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_SAIBCLKSource_PLLI2S: PLLI2S_Q clock divided by PLLI2SDIVQ used
  *                                           as SAI1 Block B clock
  *            @arg RCC_SAIBCLKSource_PLLSAI: PLLISAI_Q clock divided by PLLSAIDIVQ used
  *                                           as SAI1 Block B clock
  *            @arg RCC_SAIBCLKSource_Ext: External clock mapped on the I2S_CKIN pin
  *                                        used as SAI1 Block B clock
  * 返回值: 无
  */
void RCC_SAIBlockBCLKConfig(uint32_t RCC_SAIBlockBCLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_SAIBCLK_SOURCE(RCC_SAIBlockBCLKSource));

    tmpreg = RCC->DCKCFGR;

    /* 清除 RCC_DCKCFGR_SAI1BSRC[1:0] 位 */
    tmpreg &= ~RCC_DCKCFGR_SAI1BSRC;

    /* Set SAI Block B source selection 值 */
    tmpreg |= RCC_SAIBlockBCLKSource;

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}
#endif /* STM32F40_41xxx || STM32F427_437xx || STM32F429_439xx || STM32F469_479xx */

/**
  * 简介:  Configures the SAI clock Divider coming from PLLI2S.
  *
  * 注意:   该功能只支持STM32F42xxx/43xxx/446xx/469xx/479xx设备.
  *
  * 注意:   This function must be called before enabling the PLLI2S.
  *
  * 参数:  RCC_PLLI2SDivQ: specifies the PLLI2S division factor for SAI1 clock .
  *          此参数必须是介于 1 and 32.
  *          SAI1 clock frequency = f(PLLI2S_Q) / RCC_PLLI2SDivQ
  *
  * 返回值: 无
  */
void RCC_SAIPLLI2SClkDivConfig(uint32_t RCC_PLLI2SDivQ) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_PLLI2S_DIVQ_VALUE(RCC_PLLI2SDivQ));

    tmpreg = RCC->DCKCFGR;

    /* 清除 PLLI2SDIVQ[4:0] 位 */
    tmpreg &= ~(RCC_DCKCFGR_PLLI2SDIVQ);

    /* Set PLLI2SDIVQ values */
    tmpreg |= (RCC_PLLI2SDivQ - 1);

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}

/**
  * 简介:  Configures the SAI clock Divider coming from PLLSAI.
  *
  * 注意:   该功能只支持STM32F42xxx/43xxx/446xx/469xx/479xx设备.
  *
  * 注意:   This function must be called before enabling the PLLSAI.
  *
  * 参数:  RCC_PLLSAIDivQ: specifies the PLLSAI division factor for SAI1 clock .
  *          此参数必须是介于 1 and 32.
  *          SAI1 clock frequency = f(PLLSAI_Q) / RCC_PLLSAIDivQ
  *
  * 返回值: 无
  */
void RCC_SAIPLLSAIClkDivConfig(uint32_t RCC_PLLSAIDivQ) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_PLLSAI_DIVQ_VALUE(RCC_PLLSAIDivQ));

    tmpreg = RCC->DCKCFGR;

    /* 清除 PLLI2SDIVQ[4:0] and PLLSAIDIVQ[4:0] 位 */
    tmpreg &= ~(RCC_DCKCFGR_PLLSAIDIVQ);

    /* Set PLLSAIDIVQ values */
    tmpreg |= ((RCC_PLLSAIDivQ - 1) << 8);

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}

#if defined(STM32F413_423xx)
/**
  * 简介:  Configures the SAI clock Divider coming from PLLI2S.
  *
  * 注意:   This function can be used only for STM32F413_423xx
  *
  * 参数:   RCC_PLLI2SDivR: specifies the PLLI2S division factor for SAI1 clock.
  *          此参数必须是介于 1 and 32.
  *          SAI1 clock frequency = f(PLLI2SR) / RCC_PLLI2SDivR
  * 返回值: 无
  */
void RCC_SAIPLLI2SRClkDivConfig(uint32_t RCC_PLLI2SDivR) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_PLLI2S_DIVR_VALUE(RCC_PLLI2SDivR));

    tmpreg = RCC->DCKCFGR;

    /* 清除 PLLI2SDIVR[4:0] 位 */
    tmpreg &= ~(RCC_DCKCFGR_PLLI2SDIVR);

    /* Set PLLI2SDIVR values */
    tmpreg |= (RCC_PLLI2SDivR - 1);

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}

/**
  * 简介:  Configures the SAI clock Divider coming from PLL.
  *
  * 注意:   This function can be used only for STM32F413_423xx
  *
  * 注意:   This function must be called before enabling the PLLSAI.
  *
  * 参数:  RCC_PLLDivR: specifies the PLL division factor for SAI1 clock.
  *          此参数必须是介于 1 and 32.
  *          SAI1 clock frequency = f(PLLR) / RCC_PLLDivR
  *
  * 返回值: 无
  */
void RCC_SAIPLLRClkDivConfig(uint32_t RCC_PLLDivR) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_PLL_DIVR_VALUE(RCC_PLLDivR));

    tmpreg = RCC->DCKCFGR;

    /* 清除 PLLDIVR[12:8] */
    tmpreg &= ~(RCC_DCKCFGR_PLLDIVR);

    /* Set PLLDivR values */
    tmpreg |= ((RCC_PLLDivR - 1 ) << 8);

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}
#endif /* STM32F413_423xx */

/**
  * 简介:  Configures the LTDC clock Divider coming from PLLSAI.
  *
  * 注意:   The LTDC peripheral is only available with STM32F42xxx/43xxx/446xx/469xx/479xx Devices.
  *
  * 注意:   This function must be called before enabling the PLLSAI.
  *
  * 参数:  RCC_PLLSAIDivR: specifies the PLLSAI division factor for LTDC clock .
  *          LTDC clock frequency = f(PLLSAI_R) / RCC_PLLSAIDivR
  *          此参数可以是以下值之一:
  *            @arg RCC_PLLSAIDivR_Div2: LTDC clock = f(PLLSAI_R)/2
  *            @arg RCC_PLLSAIDivR_Div4: LTDC clock = f(PLLSAI_R)/4
  *            @arg RCC_PLLSAIDivR_Div8: LTDC clock = f(PLLSAI_R)/8
  *            @arg RCC_PLLSAIDivR_Div16: LTDC clock = f(PLLSAI_R)/16
  *
  * 返回值: 无
  */
void RCC_LTDCCLKDivConfig(uint32_t RCC_PLLSAIDivR) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_PLLSAI_DIVR_VALUE(RCC_PLLSAIDivR));

    tmpreg = RCC->DCKCFGR;

    /* 清除 PLLSAIDIVR[2:0] 位 */
    tmpreg &= ~RCC_DCKCFGR_PLLSAIDIVR;

    /* Set PLLSAIDIVR values */
    tmpreg |= RCC_PLLSAIDivR;

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}

#if defined(STM32F412xG) || defined(STM32F413_423xx)
/**
  * 简介:  配置DFSDM时钟源(DFSDMCLK)。
  * 注意:   在启用DFSDM APB时钟之前，必须调用此函数。
  * 参数:  RCC_DFSDMCLKSource: 指定DFSDM时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_DFSDMCLKSource_APB: APB clock used as DFSDM clock source.
  *            @arg RCC_DFSDMCLKSource_SYS: System clock used as DFSDM clock source.
  *
  * 返回值: 无
  */
void RCC_DFSDM1CLKConfig(uint32_t RCC_DFSDMCLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_DFSDM1CLK_SOURCE(RCC_DFSDMCLKSource));

    tmpreg = RCC->DCKCFGR;

    /* 清除 CKDFSDM-SEL  位 */
    tmpreg &= ~RCC_DCKCFGR_CKDFSDM1SEL;

    /* Set CKDFSDM-SEL bit according to RCC_DFSDMCLKSource 值 */
    tmpreg |= (RCC_DFSDMCLKSource << 31) ;

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}

/**
  * 简介:  配置DFSDM音频时钟源(DFSDMACLK)。
  * 注意:   在启用DFSDM APB时钟之前，必须调用此函数。
  * 参数:  RCC_DFSDM1ACLKSource: specifies the DFSDM clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_DFSDM1AUDIOCLKSOURCE_I2SAPB1: APB clock used as DFSDM clock source.
  *            @arg RCC_DFSDM1AUDIOCLKSOURCE_I2SAPB2: System clock used as DFSDM clock source.
  *
  * 返回值: 无
  */
void RCC_DFSDM1ACLKConfig(uint32_t RCC_DFSDM1ACLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_DFSDMACLK_SOURCE(RCC_DFSDM1ACLKSource));

    tmpreg = RCC->DCKCFGR;

    /* 清除 CKDFSDMA SEL  位 */
    tmpreg &= ~RCC_DCKCFGR_CKDFSDM1ASEL;

    /* Set CKDFSDM-SEL   bt according to RCC_DFSDMCLKSource 值 */
    tmpreg |= RCC_DFSDM1ACLKSource;

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}

#if defined(STM32F413_423xx)
/**
  * 简介:  配置DFSDM音频时钟源(DFSDMACLK)。
  * 注意:   在启用DFSDM APB时钟之前，必须调用此函数。
  * 参数:  RCC_DFSDM2ACLKSource: specifies the DFSDM clock source.
  *          此参数可以是以下值之一:
  *            @arg RCC_DFSDM2AUDIOCLKSOURCE_I2SAPB1: APB clock used as DFSDM clock source.
  *            @arg RCC_DFSDM2AUDIOCLKSOURCE_I2SAPB2: System clock used as DFSDM clock source.
  *
  * 返回值: 无
  */
void RCC_DFSDM2ACLKConfig(uint32_t RCC_DFSDMACLKSource) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_RCC_DFSDMCLK_SOURCE(RCC_DFSDMACLKSource));

    tmpreg = RCC->DCKCFGR;

    /* 清除 CKDFSDMA SEL  位 */
    tmpreg &= ~RCC_DCKCFGR_CKDFSDM1ASEL;

    /* Set CKDFSDM-SEL   bt according to RCC_DFSDMCLKSource 值 */
    tmpreg |= RCC_DFSDMACLKSource;

    /* 存储新值 */
    RCC->DCKCFGR = tmpreg;
}
#endif /* STM32F413_423xx */
#endif /* STM32F412xG || STM32F413_423xx */

/**
  * 简介:  配置"计时器时钟"预分频器选择。
  *
  * 注意:   此函数只能用于STM32F42xxx/43xxx和STM32V401xx/411xE设备。
  *
  * 参数:  RCC_TIMCLKPrescaler : 指定计时器时钟预分频器选择
  *         此参数可以是以下值之一:
  *            @arg RCC_TIMPrescDesactivated: 如果PPREx对应于除以1或2，
  *                 则Timers内核时钟预分频器等于HPRE，否则如果PPREx等于除以4或更大，则等于[(HPRE*PPREx)/2]。
  *
  *            @arg RCC_TIMPrescActivated: 如果PPREx对应于除以1、2或4，
  *                 则Timers内核时钟预分频器等于HPRE，否则，如果PPREx相应于除以8或更多，则它等于[(HPRE*PPREx)/4]。
  * 返回值: 无
  */
void RCC_TIMCLKPresConfig(uint32_t RCC_TIMCLKPrescaler) {
    /* 检查参数 */
    assert_param(IS_RCC_TIMCLK_PRESCALER(RCC_TIMCLKPrescaler));

    *(__IO uint32_t *) DCKCFGR_TIMPRE_BB = RCC_TIMCLKPrescaler;
}

/**
  * 简介:  启用或禁用AHB1 外设时钟.
  * 注意:   重置后，外围时钟(用于寄存器读/写访问)被禁用，应用软件必须在使用该时钟之前启用该时钟。
  * 参数:  RCC_AHBPeriph: 指定AHB1外围设备选通其时钟。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_AHB1Periph_GPIOA:       GPIOA clock
  *            @arg RCC_AHB1Periph_GPIOB:       GPIOB clock
  *            @arg RCC_AHB1Periph_GPIOC:       GPIOC clock
  *            @arg RCC_AHB1Periph_GPIOD:       GPIOD clock
  *            @arg RCC_AHB1Periph_GPIOE:       GPIOE clock
  *            @arg RCC_AHB1Periph_GPIOF:       GPIOF clock
  *            @arg RCC_AHB1Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB1Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB1Periph_GPIOI:       GPIOI clock
  *            @arg RCC_AHB1Periph_GPIOJ:       GPIOJ clock (STM32F42xxx/43xxx devices)
  *            @arg RCC_AHB1Periph_GPIOK:       GPIOK clock (STM32F42xxx/43xxx devices)
  *            @arg RCC_AHB1Periph_CRC:         CRC clock
  *            @arg RCC_AHB1Periph_BKPSRAM:     BKPSRAM interface clock
  *            @arg RCC_AHB1Periph_CCMDATARAMEN CCM data RAM interface clock
  *            @arg RCC_AHB1Periph_DMA1:        DMA1 clock
  *            @arg RCC_AHB1Periph_DMA2:        DMA2 clock
  *            @arg RCC_AHB1Periph_DMA2D:       DMA2D clock (STM32F429xx/439xx devices)
  *            @arg RCC_AHB1Periph_ETH_MAC:     Ethernet MAC clock
  *            @arg RCC_AHB1Periph_ETH_MAC_Tx:  Ethernet Transmission clock
  *            @arg RCC_AHB1Periph_ETH_MAC_Rx:  Ethernet Reception clock
  *            @arg RCC_AHB1Periph_ETH_MAC_PTP: Ethernet PTP clock
  *            @arg RCC_AHB1Periph_OTG_HS:      USB OTG HS clock
  *            @arg RCC_AHB1Periph_OTG_HS_ULPI: USB OTG HS ULPI clock
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB1PeriphClockCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB1_CLOCK_PERIPH(RCC_AHB1Periph));

    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB1ENR |= RCC_AHB1Periph;
    } else {
        RCC->AHB1ENR &= ~RCC_AHB1Periph;
    }
}

/**
  * 简介:  启用或禁用AHB2 外设时钟.
  * 注意:   重置后，外围时钟(用于寄存器读/写访问)被禁用，应用软件必须在使用该时钟之前启用该时钟。
  * 参数:  RCC_AHBPeriph: 指定AHB2外围设备以设置时钟门。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_AHB2Periph_DCMI:   DCMI clock
  *            @arg RCC_AHB2Periph_CRYP:   CRYP clock
  *            @arg RCC_AHB2Periph_HASH:   HASH clock
  *            @arg RCC_AHB2Periph_RNG:    RNG clock
  *            @arg RCC_AHB2Periph_OTG_FS: USB OTG FS clock
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB2PeriphClockCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB2_PERIPH(RCC_AHB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB2ENR |= RCC_AHB2Periph;
    } else {
        RCC->AHB2ENR &= ~RCC_AHB2Periph;
    }
}

#if defined(STM32F40_41xxx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
/**
  * 简介:  启用或禁用AHB3 外设时钟.
  * 注意:   重置后，外围时钟(用于寄存器读/写访问)被禁用，应用软件必须在使用该时钟之前启用该时钟。
  * 参数:  RCC_AHBPeriph: 指定AHB3外围设备选通其时钟。
  *          此参数必须为:
  *           - RCC_AHB3Periph_FSMC or RCC_AHB3Periph_FMC (STM32F412xG/STM32F413_423xx/STM32F429x/439x devices)
  *           - RCC_AHB3Periph_QSPI (STM32F412xG/STM32F413_423xx/STM32F446xx/STM32F469_479xx devices)
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB3PeriphClockCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB3_PERIPH(RCC_AHB3Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB3ENR |= RCC_AHB3Periph;
    } else {
        RCC->AHB3ENR &= ~RCC_AHB3Periph;
    }
}
#endif /* STM32F40_41xxx || STM32F412xG || STM32F413_423xx || STM32F427_437xx || STM32F429_439xx || STM32F446xx || STM32F469_479xx */

/**
  * 简介:  启用或禁用Low Speed APB (APB1) 外设时钟.
  * 注意:   重置后，外围时钟(用于寄存器读/写访问)被禁用，应用软件必须在使用该时钟之前启用该时钟。
  * 参数:  RCC_APB1Periph: 指定APB1外围设备选通其时钟。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_APB1Periph_TIM2:   TIM2 clock
  *            @arg RCC_APB1Periph_TIM3:   TIM3 clock
  *            @arg RCC_APB1Periph_TIM4:   TIM4 clock
  *            @arg RCC_APB1Periph_TIM5:   TIM5 clock
  *            @arg RCC_APB1Periph_TIM6:   TIM6 clock
  *            @arg RCC_APB1Periph_TIM7:   TIM7 clock
  *            @arg RCC_APB1Periph_TIM12:  TIM12 clock
  *            @arg RCC_APB1Periph_TIM13:  TIM13 clock
  *            @arg RCC_APB1Periph_TIM14:  TIM14 clock
  *            @arg RCC_APB1Periph_LPTIM1: LPTIM1 clock (STM32F410xx and STM32F413_423xx devices)
  *            @arg RCC_APB1Periph_WWDG:   WWDG clock
  *            @arg RCC_APB1Periph_SPI2:   SPI2 clock
  *            @arg RCC_APB1Periph_SPI3:   SPI3 clock
  *            @arg RCC_APB1Periph_SPDIF:  SPDIF RX clock (STM32F446xx devices)
  *            @arg RCC_APB1Periph_USART2: USART2 clock
  *            @arg RCC_APB1Periph_USART3: USART3 clock
  *            @arg RCC_APB1Periph_UART4:  UART4 clock
  *            @arg RCC_APB1Periph_UART5:  UART5 clock
  *            @arg RCC_APB1Periph_I2C1:   I2C1 clock
  *            @arg RCC_APB1Periph_I2C2:   I2C2 clock
  *            @arg RCC_APB1Periph_I2C3:   I2C3 clock
  *            @arg RCC_APB1Periph_FMPI2C1:FMPI2C1 clock
  *            @arg RCC_APB1Periph_CAN1:   CAN1 clock
  *            @arg RCC_APB1Periph_CAN2:   CAN2 clock
  *            @arg RCC_APB1Periph_CEC:    CEC clock (STM32F446xx devices)
  *            @arg RCC_APB1Periph_PWR:    PWR clock
  *            @arg RCC_APB1Periph_DAC:    DAC clock
  *            @arg RCC_APB1Periph_UART7:  UART7 clock
  *            @arg RCC_APB1Periph_UART8:  UART8 clock
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->APB1ENR |= RCC_APB1Periph;
    } else {
        RCC->APB1ENR &= ~RCC_APB1Periph;
    }
}

/**
  * 简介:  启用或禁用高速APB(APB2)外围时钟。
  * 注意:   重置后，外围时钟(用于寄存器读/写访问)被禁用，应用软件必须在使用该时钟之前启用该时钟。
  * 参数:  RCC_APB2Periph: 指定APB2外围设备选通其时钟。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_APB2Periph_TIM1:   TIM1 clock
  *            @arg RCC_APB2Periph_TIM8:   TIM8 clock
  *            @arg RCC_APB2Periph_USART1: USART1 clock
  *            @arg RCC_APB2Periph_USART6: USART6 clock
  *            @arg RCC_APB2Periph_ADC1:   ADC1 clock
  *            @arg RCC_APB2Periph_ADC2:   ADC2 clock
  *            @arg RCC_APB2Periph_ADC3:   ADC3 clock
  *            @arg RCC_APB2Periph_SDIO:   SDIO clock
  *            @arg RCC_APB2Periph_SPI1:   SPI1 clock
  *            @arg RCC_APB2Periph_SPI4:   SPI4 clock
  *            @arg RCC_APB2Periph_SYSCFG: SYSCFG clock
  *            @arg RCC_APB2Periph_EXTIT:  EXTIIT clock
  *            @arg RCC_APB2Periph_TIM9:   TIM9 clock
  *            @arg RCC_APB2Periph_TIM10:  TIM10 clock
  *            @arg RCC_APB2Periph_TIM11:  TIM11 clock
  *            @arg RCC_APB2Periph_SPI5:   SPI5 clock
  *            @arg RCC_APB2Periph_SPI6:   SPI6 clock
  *            @arg RCC_APB2Periph_SAI1:   SAI1 clock (STM32F42xxx/43xxx/446xx/469xx/479xx/413_423xx devices)
  *            @arg RCC_APB2Periph_SAI2:   SAI2 clock (STM32F446xx devices)
  *            @arg RCC_APB2Periph_LTDC:   LTDC clock (STM32F429xx/439xx devices)
  *            @arg RCC_APB2Periph_DSI:    DSI clock (STM32F469_479xx devices)
  *            @arg RCC_APB2Periph_DFSDM1: DFSDM Clock (STM32F412xG and STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_DFSDM2: DFSDM2 Clock (STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_UART9:  UART9 Clock (STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_UART10: UART10 Clock (STM32F413_423xx Devices)
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->APB2ENR |= RCC_APB2Periph;
    } else {
        RCC->APB2ENR &= ~RCC_APB2Periph;
    }
}

/**
  * 简介:  强制或释放AHB1外围设备复位。
  * 参数:  RCC_AHB1Periph: 指定要重置的AHB1外围设备。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_AHB1Periph_GPIOA:   GPIOA clock
  *            @arg RCC_AHB1Periph_GPIOB:   GPIOB clock
  *            @arg RCC_AHB1Periph_GPIOC:   GPIOC clock
  *            @arg RCC_AHB1Periph_GPIOD:   GPIOD clock
  *            @arg RCC_AHB1Periph_GPIOE:   GPIOE clock
  *            @arg RCC_AHB1Periph_GPIOF:   GPIOF clock
  *            @arg RCC_AHB1Periph_GPIOG:   GPIOG clock
  *            @arg RCC_AHB1Periph_GPIOG:   GPIOG clock
  *            @arg RCC_AHB1Periph_GPIOI:   GPIOI clock
  *            @arg RCC_AHB1Periph_GPIOJ:   GPIOJ clock (STM32F42xxx/43xxx devices)
  *            @arg RCC_AHB1Periph_GPIOK:   GPIOK clock (STM32F42xxx/43xxxdevices)
  *            @arg RCC_AHB1Periph_CRC:     CRC clock
  *            @arg RCC_AHB1Periph_DMA1:    DMA1 clock
  *            @arg RCC_AHB1Periph_DMA2:    DMA2 clock
  *            @arg RCC_AHB1Periph_DMA2D:   DMA2D clock (STM32F429xx/439xx devices)
  *            @arg RCC_AHB1Periph_ETH_MAC: Ethernet MAC clock
  *            @arg RCC_AHB1Periph_OTG_HS:  USB OTG HS clock
  *            @arg RCC_AHB1Periph_RNG:     RNG clock for STM32F410xx devices
  * 参数:  NewState: 新状态-> specified peripheral reset.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB1PeriphResetCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB1_RESET_PERIPH(RCC_AHB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB1RSTR |= RCC_AHB1Periph;
    } else {
        RCC->AHB1RSTR &= ~RCC_AHB1Periph;
    }
}

/**
  * 简介:  强制或释放AHB2外设复位。
  * 参数:  RCC_AHB2Periph: 指定要重置的AHB2外围设备。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_AHB2Periph_DCMI:   DCMI clock
  *            @arg RCC_AHB2Periph_CRYP:   CRYP clock
  *            @arg RCC_AHB2Periph_HASH:   HASH clock
  *            @arg RCC_AHB2Periph_RNG:    RNG clock for STM32F40_41xxx/STM32F412xG/STM32F413_423xx/STM32F427_437xx/STM32F429_439xx/STM32F469_479xx devices
  *            @arg RCC_AHB2Periph_OTG_FS: USB OTG FS clock
  * 参数:  NewState: 新状态-> specified peripheral reset.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB2PeriphResetCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB2_PERIPH(RCC_AHB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB2RSTR |= RCC_AHB2Periph;
    } else {
        RCC->AHB2RSTR &= ~RCC_AHB2Periph;
    }
}

#if defined(STM32F40_41xxx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
/**
  * 简介:  强制或释放AHB3外设复位。
  * 参数:  RCC_AHB3Periph: 指定要重置的AHB3外围设备。
  *          此参数必须为:
  *           - RCC_AHB3Periph_FSMC or RCC_AHB3Periph_FMC (STM32F412xG, STM32F413_423xx and STM32F429x/439x devices)
  *           - RCC_AHB3Periph_QSPI (STM32F412xG/STM32F446xx/STM32F469_479xx devices)
  * 参数:  NewState: 新状态-> specified peripheral reset.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB3PeriphResetCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB3_PERIPH(RCC_AHB3Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB3RSTR |= RCC_AHB3Periph;
    } else {
        RCC->AHB3RSTR &= ~RCC_AHB3Periph;
    }
}
#endif /* STM32F40_41xxx || STM32F412xG || STM32F413_423xx || STM32F427_437xx || STM32F429_439xx || STM32F446xx || STM32F469_479xx */

/**
  * 简介:  强制或释放APB1外设复位。
  * 参数:  RCC_APB1Periph: 指定要重置的APB1外围设备。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_APB1Periph_TIM2:   TIM2 clock
  *            @arg RCC_APB1Periph_TIM3:   TIM3 clock
  *            @arg RCC_APB1Periph_TIM4:   TIM4 clock
  *            @arg RCC_APB1Periph_TIM5:   TIM5 clock
  *            @arg RCC_APB1Periph_TIM6:   TIM6 clock
  *            @arg RCC_APB1Periph_TIM7:   TIM7 clock
  *            @arg RCC_APB1Periph_TIM12:  TIM12 clock
  *            @arg RCC_APB1Periph_TIM13:  TIM13 clock
  *            @arg RCC_APB1Periph_TIM14:  TIM14 clock
  *            @arg RCC_APB1Periph_LPTIM1: LPTIM1 clock (STM32F410xx and STM32F413_423xx devices)
  *            @arg RCC_APB1Periph_WWDG:   WWDG clock
  *            @arg RCC_APB1Periph_SPI2:   SPI2 clock
  *            @arg RCC_APB1Periph_SPI3:   SPI3 clock
  *            @arg RCC_APB1Periph_SPDIF:  SPDIF RX clock (STM32F446xx devices)
  *            @arg RCC_APB1Periph_USART2: USART2 clock
  *            @arg RCC_APB1Periph_USART3: USART3 clock
  *            @arg RCC_APB1Periph_UART4:  UART4 clock
  *            @arg RCC_APB1Periph_UART5:  UART5 clock
  *            @arg RCC_APB1Periph_I2C1:   I2C1 clock
  *            @arg RCC_APB1Periph_I2C2:   I2C2 clock
  *            @arg RCC_APB1Periph_I2C3:   I2C3 clock
  *            @arg RCC_APB1Periph_FMPI2C1:FMPI2C1 clock
  *            @arg RCC_APB1Periph_CAN1:   CAN1 clock
  *            @arg RCC_APB1Periph_CAN2:   CAN2 clock
  *            @arg RCC_APB1Periph_CEC:    CEC clock(STM32F446xx devices)
  *            @arg RCC_APB1Periph_PWR:    PWR clock
  *            @arg RCC_APB1Periph_DAC:    DAC clock
  *            @arg RCC_APB1Periph_UART7:  UART7 clock
  *            @arg RCC_APB1Periph_UART8:  UART8 clock
  * 参数:  NewState: 新状态-> specified peripheral reset.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->APB1RSTR |= RCC_APB1Periph;
    } else {
        RCC->APB1RSTR &= ~RCC_APB1Periph;
    }
}

/**
  * 简介:  强制或释放APB2外设复位。
  * 参数:  RCC_APB2Periph: 指定要重置的APB2外围设备。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_APB2Periph_TIM1:   TIM1 clock
  *            @arg RCC_APB2Periph_TIM8:   TIM8 clock
  *            @arg RCC_APB2Periph_USART1: USART1 clock
  *            @arg RCC_APB2Periph_USART6: USART6 clock
  *            @arg RCC_APB2Periph_ADC1:   ADC1 clock
  *            @arg RCC_APB2Periph_ADC2:   ADC2 clock
  *            @arg RCC_APB2Periph_ADC3:   ADC3 clock
  *            @arg RCC_APB2Periph_SDIO:   SDIO clock
  *            @arg RCC_APB2Periph_SPI1:   SPI1 clock
  *            @arg RCC_APB2Periph_SPI4:   SPI4 clock
  *            @arg RCC_APB2Periph_SYSCFG: SYSCFG clock
  *            @arg RCC_APB2Periph_TIM9:   TIM9 clock
  *            @arg RCC_APB2Periph_TIM10:  TIM10 clock
  *            @arg RCC_APB2Periph_TIM11:  TIM11 clock
  *            @arg RCC_APB2Periph_SPI5:   SPI5 clock
  *            @arg RCC_APB2Periph_SPI6:   SPI6 clock
  *            @arg RCC_APB2Periph_SAI1:   SAI1 clock (STM32F42xxx/43xxx/446xx/469xx/479xx/413_423xx devices)
  *            @arg RCC_APB2Periph_SAI2:   SAI2 clock (STM32F446xx devices)
  *            @arg RCC_APB2Periph_LTDC:   LTDC clock (STM32F429xx/439xx devices)
  *            @arg RCC_APB2Periph_DSI:    DSI clock (STM32F469_479xx devices)
  *            @arg RCC_APB2Periph_DFSDM1: DFSDM Clock (STM32F412xG and STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_DFSDM2: DFSDM2 Clock (STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_UART9:  UART9 Clock (STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_UART10: UART10 Clock (STM32F413_423xx Devices)
  * 参数:  NewState: 新状态-> specified peripheral reset.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_APB2_RESET_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->APB2RSTR |= RCC_APB2Periph;
    } else {
        RCC->APB2RSTR &= ~RCC_APB2Periph;
    }
}

/**
  * 简介:  在低功耗(睡眠)模式下启用或禁用AHB1外设时钟。
  * 注意:   SLEEP模式下的外围时钟门控可用于进一步降低功耗。
  * 注意:   从睡眠模式唤醒后，将再次启用外围时钟。
  * 注意:   默认情况下，在睡眠模式期间启用所有外围时钟。
  * 参数:  RCC_AHBPeriph: 指定AHB1外围设备选通其时钟。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_AHB1Periph_GPIOA:       GPIOA clock
  *            @arg RCC_AHB1Periph_GPIOB:       GPIOB clock
  *            @arg RCC_AHB1Periph_GPIOC:       GPIOC clock
  *            @arg RCC_AHB1Periph_GPIOD:       GPIOD clock
  *            @arg RCC_AHB1Periph_GPIOE:       GPIOE clock
  *            @arg RCC_AHB1Periph_GPIOF:       GPIOF clock
  *            @arg RCC_AHB1Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB1Periph_GPIOG:       GPIOG clock
  *            @arg RCC_AHB1Periph_GPIOI:       GPIOI clock
  *            @arg RCC_AHB1Periph_GPIOJ:       GPIOJ clock (STM32F42xxx/43xxx devices)
  *            @arg RCC_AHB1Periph_GPIOK:       GPIOK clock (STM32F42xxx/43xxx devices)
  *            @arg RCC_AHB1Periph_CRC:         CRC clock
  *            @arg RCC_AHB1Periph_BKPSRAM:     BKPSRAM interface clock
  *            @arg RCC_AHB1Periph_DMA1:        DMA1 clock
  *            @arg RCC_AHB1Periph_DMA2:        DMA2 clock
  *            @arg RCC_AHB1Periph_DMA2D:       DMA2D clock (STM32F429xx/439xx devices)
  *            @arg RCC_AHB1Periph_ETH_MAC:     Ethernet MAC clock
  *            @arg RCC_AHB1Periph_ETH_MAC_Tx:  Ethernet Transmission clock
  *            @arg RCC_AHB1Periph_ETH_MAC_Rx:  Ethernet Reception clock
  *            @arg RCC_AHB1Periph_ETH_MAC_PTP: Ethernet PTP clock
  *            @arg RCC_AHB1Periph_OTG_HS:      USB OTG HS clock
  *            @arg RCC_AHB1Periph_OTG_HS_ULPI: USB OTG HS ULPI clock
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB1PeriphClockLPModeCmd(uint32_t RCC_AHB1Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB1_LPMODE_PERIPH(RCC_AHB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB1LPENR |= RCC_AHB1Periph;
    } else {
        RCC->AHB1LPENR &= ~RCC_AHB1Periph;
    }
}

/**
  * 简介:  启用或禁用AHB2 低功耗(睡眠)模式下的外围时钟。
  * 注意:   SLEEP模式下的外围时钟门控可用于进一步降低功耗。
  * 注意:   从睡眠模式唤醒后，将再次启用外围时钟。
  * 注意:   默认情况下，在睡眠模式期间启用所有外围时钟。
  * 参数:  RCC_AHBPeriph: 指定AHB2外围设备以设置时钟门。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_AHB2Periph_DCMI:   DCMI clock
  *            @arg RCC_AHB2Periph_CRYP:   CRYP clock
  *            @arg RCC_AHB2Periph_HASH:   HASH clock
  *            @arg RCC_AHB2Periph_RNG:    RNG clock
  *            @arg RCC_AHB2Periph_OTG_FS: USB OTG FS clock
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB2PeriphClockLPModeCmd(uint32_t RCC_AHB2Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB2_PERIPH(RCC_AHB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB2LPENR |= RCC_AHB2Periph;
    } else {
        RCC->AHB2LPENR &= ~RCC_AHB2Periph;
    }
}

#if defined(STM32F40_41xxx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F427_437xx) || defined(STM32F429_439xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
/**
  * 简介:  启用或禁用AHB3 低功耗(睡眠)模式下的外围时钟。
  * 注意:   SLEEP模式下的外围时钟门控可用于进一步降低功耗。
  * 注意:   从睡眠模式唤醒后，将再次启用外围时钟。
  * 注意:   默认情况下，在睡眠模式期间启用所有外围时钟。
  * 参数:  RCC_AHBPeriph: 指定AHB3外围设备选通其时钟。
  *          此参数必须为:
  *           - RCC_AHB3Periph_FSMC or RCC_AHB3Periph_FMC (STM32F412xG/STM32F413_423xx/STM32F429x/439x devices)
  *           - RCC_AHB3Periph_QSPI (STM32F412xG/STM32F413_423xx/STM32F446xx/STM32F469_479xx devices)
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB3PeriphClockLPModeCmd(uint32_t RCC_AHB3Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB3_PERIPH(RCC_AHB3Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->AHB3LPENR |= RCC_AHB3Periph;
    } else {
        RCC->AHB3LPENR &= ~RCC_AHB3Periph;
    }
}
#endif /* STM32F40_41xxx || STM32F412xG || STM32F413_423xx || STM32F427_437xx || STM32F429_439xx || STM32F446xx || STM32F469_479xx */

/**
  * 简介:  启用或禁用APB1 低功耗(睡眠)模式下的外围时钟。
  * 注意:   SLEEP模式下的外围时钟门控可用于进一步降低功耗。
  * 注意:   从睡眠模式唤醒后，将再次启用外围时钟。
  * 注意:   默认情况下，在睡眠模式期间启用所有外围时钟。
  * 参数:  RCC_APB1Periph: 指定APB1外围设备选通其时钟。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_APB1Periph_TIM2:   TIM2 clock
  *            @arg RCC_APB1Periph_TIM3:   TIM3 clock
  *            @arg RCC_APB1Periph_TIM4:   TIM4 clock
  *            @arg RCC_APB1Periph_TIM5:   TIM5 clock
  *            @arg RCC_APB1Periph_TIM6:   TIM6 clock
  *            @arg RCC_APB1Periph_TIM7:   TIM7 clock
  *            @arg RCC_APB1Periph_TIM12:  TIM12 clock
  *            @arg RCC_APB1Periph_TIM13:  TIM13 clock
  *            @arg RCC_APB1Periph_TIM14:  TIM14 clock
  *            @arg RCC_APB1Periph_LPTIM1: LPTIM1 clock (STM32F410xx and STM32F413_423xx devices)
  *            @arg RCC_APB1Periph_WWDG:   WWDG clock
  *            @arg RCC_APB1Periph_SPI2:   SPI2 clock
  *            @arg RCC_APB1Periph_SPI3:   SPI3 clock
  *            @arg RCC_APB1Periph_SPDIF:   SPDIF RX clock (STM32F446xx devices)
  *            @arg RCC_APB1Periph_USART2: USART2 clock
  *            @arg RCC_APB1Periph_USART3: USART3 clock
  *            @arg RCC_APB1Periph_UART4:  UART4 clock
  *            @arg RCC_APB1Periph_UART5:  UART5 clock
  *            @arg RCC_APB1Periph_I2C1:   I2C1 clock
  *            @arg RCC_APB1Periph_I2C2:   I2C2 clock
  *            @arg RCC_APB1Periph_I2C3:   I2C3 clock
  *            @arg RCC_APB1Periph_FMPI2C1:   FMPI2C1 clock
  *            @arg RCC_APB1Periph_CAN1:   CAN1 clock
  *            @arg RCC_APB1Periph_CAN2:   CAN2 clock
  *            @arg RCC_APB1Periph_CEC:    CEC clock (STM32F446xx devices)
  *            @arg RCC_APB1Periph_PWR:    PWR clock
  *            @arg RCC_APB1Periph_DAC:    DAC clock
  *            @arg RCC_APB1Periph_UART7:  UART7 clock
  *            @arg RCC_APB1Periph_UART8:  UART8 clock
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_APB1PeriphClockLPModeCmd(uint32_t RCC_APB1Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->APB1LPENR |= RCC_APB1Periph;
    } else {
        RCC->APB1LPENR &= ~RCC_APB1Periph;
    }
}

/**
  * 简介:  启用或禁用APB2 低功耗(睡眠)模式下的外围时钟。
  * 注意:   SLEEP模式下的外围时钟门控可用于进一步降低功耗。
  * 注意:   从睡眠模式唤醒后，将再次启用外围时钟。
  * 注意:   默认情况下，在睡眠模式期间启用所有外围时钟。
  * 参数:  RCC_APB2Periph: 指定APB2外围设备选通其时钟。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_APB2Periph_TIM1:   TIM1 clock
  *            @arg RCC_APB2Periph_TIM8:   TIM8 clock
  *            @arg RCC_APB2Periph_USART1: USART1 clock
  *            @arg RCC_APB2Periph_USART6: USART6 clock
  *            @arg RCC_APB2Periph_ADC1:   ADC1 clock
  *            @arg RCC_APB2Periph_ADC2:   ADC2 clock
  *            @arg RCC_APB2Periph_ADC3:   ADC3 clock
  *            @arg RCC_APB2Periph_SDIO:   SDIO clock
  *            @arg RCC_APB2Periph_SPI1:   SPI1 clock
  *            @arg RCC_APB2Periph_SPI4:   SPI4 clock
  *            @arg RCC_APB2Periph_SYSCFG: SYSCFG clock
  *            @arg RCC_APB2Periph_EXTIT:  EXTIIT clock
  *            @arg RCC_APB2Periph_TIM9:   TIM9 clock
  *            @arg RCC_APB2Periph_TIM10:  TIM10 clock
  *            @arg RCC_APB2Periph_TIM11:  TIM11 clock
  *            @arg RCC_APB2Periph_SPI5:   SPI5 clock
  *            @arg RCC_APB2Periph_SPI6:   SPI6 clock
  *            @arg RCC_APB2Periph_SAI1:   SAI1 clock (STM32F42xxx/43xxx/446xx/469xx/479xx/413_423xx devices)
  *            @arg RCC_APB2Periph_SAI2:   SAI2 clock (STM32F446xx devices)
  *            @arg RCC_APB2Periph_LTDC:   LTDC clock (STM32F429xx/439xx devices)
  *            @arg RCC_APB2Periph_DSI:    DSI clock (STM32F469_479xx devices)
  *            @arg RCC_APB2Periph_DFSDM1: DFSDM Clock (STM32F412xG and STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_DFSDM2: DFSDM2 Clock (STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_UART9:  UART9 Clock (STM32F413_423xx Devices)
  *            @arg RCC_APB2Periph_UART10: UART10 Clock (STM32F413_423xx Devices)
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_APB2PeriphClockLPModeCmd(uint32_t RCC_APB2Periph, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->APB2LPENR |= RCC_APB2Periph;
    } else {
        RCC->APB2LPENR &= ~RCC_APB2Periph;
    }
}

/**
  * 简介: 配置外部低速振荡器模式(LSE 模式)。
  * 注意: 这个模式只为STM32F410xx/STM32F411xx/STM32F446xx/STM32F469_479xx devices.
  * 参数:  Mode: specifies the LSE mode.
  *          此参数可以是以下值之一:
  *            @arg RCC_LSE_LOWPOWER_MODE:  LSE oscillator in low power mode.
  *            @arg RCC_LSE_HIGHDRIVE_MODE: LSE oscillator in High Drive mode.
  * 返回值: 无
  */
void RCC_LSEModeConfig(uint8_t RCC_Mode) {
    /* 检查参数 */
    assert_param(IS_RCC_LSE_MODE(RCC_Mode));

    if(RCC_Mode == RCC_LSE_HIGHDRIVE_MODE) {
        SET_BIT(RCC->BDCR, RCC_BDCR_LSEMOD);
    } else {
        CLEAR_BIT(RCC->BDCR, RCC_BDCR_LSEMOD);
    }
}

#if defined(STM32F410xx) || defined(STM32F413_423xx)
/**
  * 简介: 配置LPTIM1时钟源。
  * 注意: 此功能仅适用于STM32F410xx设备。
  * 参数: RCC_ClockSource: 指定LPTIM1时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_LPTIM1CLKSOURCE_PCLK: LPTIM1 clock from APB1 selected.
  *            @arg RCC_LPTIM1CLKSOURCE_HSI:  LPTIM1 clock from HSI selected.
  *            @arg RCC_LPTIM1CLKSOURCE_LSI:  LPTIM1 clock from LSI selected.
  *            @arg RCC_LPTIM1CLKSOURCE_LSE:  LPTIM1 clock from LSE selected.
  * 返回值: 无
  */
void RCC_LPTIM1ClockSourceConfig(uint32_t RCC_ClockSource) {
    /* 检查参数 */
    assert_param(IS_RCC_LPTIM1_CLOCKSOURCE(RCC_ClockSource));

    /* 清除 LPTIM1 clock source selection source 位 */
    RCC->DCKCFGR2 &= ~RCC_DCKCFGR2_LPTIM1SEL;
    /* Set new LPTIM1 clock source */
    RCC->DCKCFGR2 |= RCC_ClockSource;
}
#endif /* STM32F410xx || STM32F413_423xx */

#if defined(STM32F469_479xx)
/**
  * 简介: 配置DSI时钟源。
  * 注意: 此功能仅适用于STM32F469_479xx devices.
  * 参数: RCC_ClockSource: 配置DSI时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_DSICLKSource_PHY: DSI-PHY used as DSI byte lane clock source (usual case).
  *            @arg RCC_DSICLKSource_PLLR: PLL_R used as DSI byte lane clock source, used in case DSI PLL and DSI-PHY are off (low power mode).
  * 返回值: 无
  */
void RCC_DSIClockSourceConfig(uint8_t RCC_ClockSource) {
    /* 检查参数 */
    assert_param(IS_RCC_DSI_CLOCKSOURCE(RCC_ClockSource));

    if(RCC_ClockSource == RCC_DSICLKSource_PLLR) {
        SET_BIT(RCC->DCKCFGR, RCC_DCKCFGR_DSISEL);
    } else {
        CLEAR_BIT(RCC->DCKCFGR, RCC_DCKCFGR_DSISEL);
    }
}
#endif /*  STM32F469_479xx */

#if defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx) || defined(STM32F469_479xx)
/**
  * 简介: 配置48MHz时钟源。
  * 注意: 此功能仅适用于STM32F446xx/STM32F469_479xx devices.
  * 参数: RCC_ClockSource: specifies the 48MHz clock Source.
  *          此参数可以是以下值之一:
  *            @arg RCC_48MHZCLKSource_PLL: 48MHz from PLL selected.
  *            @arg RCC_48MHZCLKSource_PLLSAI: 48MHz from PLLSAI selected.
  *            @arg RCC_CK48CLKSOURCE_PLLI2SQ : 48MHz from PLLI2SQ
  * 返回值: 无
  */
void RCC_48MHzClockSourceConfig(uint8_t RCC_ClockSource) {
    /* 检查参数 */
    assert_param(IS_RCC_48MHZ_CLOCKSOURCE(RCC_ClockSource));
    #if defined(STM32F469_479xx)

    if(RCC_ClockSource == RCC_48MHZCLKSource_PLLSAI) {
        SET_BIT(RCC->DCKCFGR, RCC_DCKCFGR_CK48MSEL);
    } else {
        CLEAR_BIT(RCC->DCKCFGR, RCC_DCKCFGR_CK48MSEL);
    }

    #elif  defined(STM32F446xx)

    if(RCC_ClockSource == RCC_48MHZCLKSource_PLLSAI) {
        SET_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL);
    } else {
        CLEAR_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL);
    }

    #elif defined(STM32F412xG) || defined(STM32F413_423xx)

    if(RCC_ClockSource == RCC_CK48CLKSOURCE_PLLI2SQ) {
        SET_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL);
    } else {
        CLEAR_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CK48MSEL);
    }

    #else
    #endif /* STM32F469_479xx */
}

/**
  * 简介: 配置SDIO时钟源。
  * 注意: 此功能仅适用于STM32F469_479xx/STM32F446xx devices.
  * 参数: RCC_ClockSource: specifies the SDIO clock Source.
  *          此参数可以是以下值之一:
  *            @arg RCC_SDIOCLKSource_48MHZ: 48MHz clock selected.
  *            @arg RCC_SDIOCLKSource_SYSCLK: system clock selected.
  * 返回值: 无
  */
void RCC_SDIOClockSourceConfig(uint8_t RCC_ClockSource) {
    /* 检查参数 */
    assert_param(IS_RCC_SDIO_CLOCKSOURCE(RCC_ClockSource));
    #if defined(STM32F469_479xx)

    if(RCC_ClockSource == RCC_SDIOCLKSource_SYSCLK) {
        SET_BIT(RCC->DCKCFGR, RCC_DCKCFGR_SDIOSEL);
    } else {
        CLEAR_BIT(RCC->DCKCFGR, RCC_DCKCFGR_SDIOSEL);
    }

    #elif defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)

    if(RCC_ClockSource == RCC_SDIOCLKSource_SYSCLK) {
        SET_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_SDIOSEL);
    } else {
        CLEAR_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_SDIOSEL);
    }

    #else
    #endif /* STM32F469_479xx */
}
#endif /* STM32F412xG || STM32F413_423xx || STM32F446xx || STM32F469_479xx */

#if defined(STM32F446xx)
/**
  * 简介:  启用或禁用指定IP的AHB1时钟门控。
  * 注意: 此功能仅适用于STM32F446xx devices.
  * 参数:  RCC_AHB1ClockGating: specifies the AHB1 clock gating.
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_AHB1ClockGating_APB1Bridge: AHB1 to APB1 clock
  *            @arg RCC_AHB1ClockGating_APB2Bridge: AHB1 to APB2 clock
  *            @arg RCC_AHB1ClockGating_CM4DBG: Cortex M4 ETM clock
  *            @arg RCC_AHB1ClockGating_SPARE: Spare clock
  *            @arg RCC_AHB1ClockGating_SRAM: SRAM controller clock
  *            @arg RCC_AHB1ClockGating_FLITF: Flash interface clock
  *            @arg RCC_AHB1ClockGating_RCC: RCC clock
  * 参数:  NewState: 指定外围时钟的新状态。
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_AHB1ClockGatingCmd(uint32_t RCC_AHB1ClockGating, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_AHB1_CLOCKGATING(RCC_AHB1ClockGating));

    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        RCC->CKGATENR &= ~RCC_AHB1ClockGating;
    } else {
        RCC->CKGATENR |= RCC_AHB1ClockGating;
    }
}

/**
  * 简介: 配置SPDIFRX时钟源。
  * 注意: 此功能仅适用于STM32F446xx devices.
  * 参数: RCC_ClockSource: specifies the SPDIFRX clock Source.
  *          此参数可以是以下值之一:
  *            @arg RCC_SPDIFRXCLKSource_PLLR: SPDIFRX clock from PLL_R selected.
  *            @arg RCC_SPDIFRXCLKSource_PLLI2SP: SPDIFRX clock from PLLI2S_P selected.
  * 返回值: 无
  */
void RCC_SPDIFRXClockSourceConfig(uint8_t RCC_ClockSource) {
    /* 检查参数 */
    assert_param(IS_RCC_SPDIFRX_CLOCKSOURCE(RCC_ClockSource));

    if(RCC_ClockSource == RCC_SPDIFRXCLKSource_PLLI2SP) {
        SET_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_SPDIFRXSEL);
    } else {
        CLEAR_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_SPDIFRXSEL);
    }
}

/**
  * 简介: 配置CEC时钟源。
  * 注意: 此功能仅适用于STM32F446xx devices.
  * 参数: RCC_ClockSource: 指定CEC时钟源。
  *          此参数可以是以下值之一:
  *            @arg RCC_CECCLKSource_HSIDiv488: CEC clock from HSI/488 selected.
  *            @arg RCC_CECCLKSource_LSE: CEC clock from LSE selected.
  * 返回值: 无
  */
void RCC_CECClockSourceConfig(uint8_t RCC_ClockSource) {
    /* 检查参数 */
    assert_param(IS_RCC_CEC_CLOCKSOURCE(RCC_ClockSource));

    if(RCC_ClockSource == RCC_CECCLKSource_LSE) {
        SET_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CECSEL);
    } else {
        CLEAR_BIT(RCC->DCKCFGR2, RCC_DCKCFGR2_CECSEL);
    }
}
#endif /* STM32F446xx */

#if defined(STM32F410xx) || defined(STM32F412xG) || defined(STM32F413_423xx) || defined(STM32F446xx)
/**
  * 简介: 配置FMPI2C1时钟源。
  * 注意: 此功能仅适用于STM32F446xx devices.
  * 参数: RCC_ClockSource: specifies the FMPI2C1 clock Source.
  *          此参数可以是以下值之一:
  *            @arg RCC_FMPI2C1CLKSource_APB1: FMPI2C1 clock from APB1 selected.
  *            @arg RCC_FMPI2C1CLKSource_SYSCLK: FMPI2C1 clock from Sytem clock selected.
  *            @arg RCC_FMPI2C1CLKSource_HSI: FMPI2C1 clock from HSI selected.
  * 返回值: 无
  */
void RCC_FMPI2C1ClockSourceConfig(uint32_t RCC_ClockSource) {
    /* 检查参数 */
    assert_param(IS_RCC_FMPI2C1_CLOCKSOURCE(RCC_ClockSource));

    /* 清除 FMPI2C1 clock source selection source 位 */
    RCC->DCKCFGR2 &= ~RCC_DCKCFGR2_FMPI2C1SEL;
    /* Set new FMPI2C1 clock source */
    RCC->DCKCFGR2 |= RCC_ClockSource;
}
#endif /* STM32F410xx || STM32F412xG || STM32F413_423xx || STM32F446xx */
/**
  * @}
  */

#if defined(STM32F410xx)
/**
  * 简介:  启用或禁用MCO1.
  * 参数:  NewState: 新状态-> MCO1.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_MCO1Cmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) RCC_CFGR_MCO1EN_BB = (uint32_t)NewState;
}

/**
  * 简介:  启用或禁用MCO2.
  * 参数:  NewState: 新状态-> MCO2.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_MCO2Cmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) RCC_CFGR_MCO2EN_BB = (uint32_t)NewState;
}
#endif /* STM32F410xx */

/** @defgroup RCC_Group4 中断和标记管理函数
 *  简介   中断和标记管理函数
 *
@verbatim
 ===============================================================================
                ##### 中断和标记管理函数 #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * 简介:  启用或禁用指定的 RCC interrupts.
  * 参数:  RCC_IT: 指定要启用或禁用的RCC中断源。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_LSERDY: LSE ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSERDY: HSE ready interrupt
  *            @arg RCC_IT_PLLRDY: main PLL ready interrupt
  *            @arg RCC_IT_PLLI2SRDY: PLLI2S ready interrupt
  *            @arg RCC_IT_PLLSAIRDY: PLLSAI ready interrupt (only for STM32F42xxx/43xxx/446xx/469xx/479xx devices)
  * 参数:  NewState: 新状态-> specified RCC interrupts.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_RCC_IT(RCC_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* Perform Byte access to RCC_CIR[14:8] bits to enable the selected interrupts */
        *(__IO uint8_t *) CIR_BYTE2_ADDRESS |= RCC_IT;
    } else {
        /* Perform Byte access to RCC_CIR[14:8] bits to disable the selected interrupts */
        *(__IO uint8_t *) CIR_BYTE2_ADDRESS &= (uint8_t)~RCC_IT;
    }
}

/**
  * 简介:  检查是否设置了指定的 RCC 标志。
  * 参数:  RCC_FLAG: 指定要检查的标志。
  *          此参数可以是以下值之一:
  *            @arg RCC_FLAG_HSIRDY: HSI oscillator clock ready
  *            @arg RCC_FLAG_HSERDY: HSE oscillator clock ready
  *            @arg RCC_FLAG_PLLRDY: main PLL clock ready
  *            @arg RCC_FLAG_PLLI2SRDY: PLLI2S clock ready
  *            @arg RCC_FLAG_PLLSAIRDY: PLLSAI clock ready (only for STM32F42xxx/43xxx/446xx/469xx/479xx devices)
  *            @arg RCC_FLAG_LSERDY: LSE oscillator clock ready
  *            @arg RCC_FLAG_LSIRDY: LSI oscillator clock ready
  *            @arg RCC_FLAG_BORRST: POR/PDR or BOR reset
  *            @arg RCC_FLAG_PINRST: Pin reset
  *            @arg RCC_FLAG_PORRST: POR/PDR reset
  *            @arg RCC_FLAG_SFTRST: Software reset
  *            @arg RCC_FLAG_IWDGRST: Independent Watchdog reset
  *            @arg RCC_FLAG_WWDGRST: Window Watchdog reset
  *            @arg RCC_FLAG_LPWRRST: Low Power reset
  * 返回值: 新状态-> RCC_FLAG (SET or RESET).
  */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG) {
    uint32_t tmp = 0;
    uint32_t statusreg = 0;
    FlagStatus bitstatus = RESET;

    /* 检查参数 */
    assert_param(IS_RCC_FLAG(RCC_FLAG));

    /* 获取RCC register index */
    tmp = RCC_FLAG >> 5;

    if (tmp == 1) {             /* The flag to check is in CR 寄存器 */
        statusreg = RCC->CR;
    } else if (tmp == 2) {      /* The flag to check is in BDCR 寄存器 */
        statusreg = RCC->BDCR;
    } else {                   /* The flag to check is in CSR 寄存器 */
        statusreg = RCC->CSR;
    }

    /* 获取flag position */
    tmp = RCC_FLAG & FLAG_MASK;

    if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    /* 返回标志状态 */
    return bitstatus;
}

/**
  * 简介:  清除 RCC 重置标志。
  *         The reset flags are: RCC_FLAG_PINRST, RCC_FLAG_PORRST,  RCC_FLAG_SFTRST,
  *         RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST
  * 参数:  无
  * 返回值: 无
  */
void RCC_ClearFlag(void) {
    /* Set RMVF bit to clear the reset flags */
    RCC->CSR |= RCC_CSR_RMVF;
}

/**
  * 简介:  检查是否发生了指定的 RCC 中断。
  * 参数:  RCC_IT: 指定要检查的RCC中断源。
  *          此参数可以是以下值之一:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_LSERDY: LSE ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSERDY: HSE ready interrupt
  *            @arg RCC_IT_PLLRDY: main PLL ready interrupt
  *            @arg RCC_IT_PLLI2SRDY: PLLI2S ready interrupt
  *            @arg RCC_IT_PLLSAIRDY: PLLSAI clock ready interrupt (only for STM32F42xxx/43xxx/446xx/469xx/479xx devices)
  *            @arg RCC_IT_CSS: Clock Security System interrupt
  * 返回值: 新状态-> RCC_IT (SET or RESET).
  */
ITStatus RCC_GetITStatus(uint8_t RCC_IT) {
    ITStatus bitstatus = RESET;

    /* 检查参数 */
    assert_param(IS_RCC_GET_IT(RCC_IT));

    /* 检查 the status of the specified RCC interrupt */
    if ((RCC->CIR & RCC_IT) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    /* 返回RCC_IT 状态 */
    return  bitstatus;
}

/**
  * 简介:  清除RCC的中断挂起位。
  * 参数:  RCC_IT: 指定要清除的中断等待位。
  *          此参数可以是以下值的任意组合:
  *            @arg RCC_IT_LSIRDY: LSI ready interrupt
  *            @arg RCC_IT_LSERDY: LSE ready interrupt
  *            @arg RCC_IT_HSIRDY: HSI ready interrupt
  *            @arg RCC_IT_HSERDY: HSE ready interrupt
  *            @arg RCC_IT_PLLRDY: main PLL ready interrupt
  *            @arg RCC_IT_PLLI2SRDY: PLLI2S ready interrupt
  *            @arg RCC_IT_PLLSAIRDY: PLLSAI ready interrupt (only for STM32F42xxx/43xxx/446xx/469xx/479xx devices)
  *            @arg RCC_IT_CSS: Clock Security System interrupt
  * 返回值: 无
  */
void RCC_ClearITPendingBit(uint8_t RCC_IT) {
    /* 检查参数 */
    assert_param(IS_RCC_CLEAR_IT(RCC_IT));

    /* Perform Byte access to RCC_CIR[23:16] bits to clear the selected interrupt
       pending 位 */
    *(__IO uint8_t *) CIR_BYTE3_ADDRESS = RCC_IT;
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
