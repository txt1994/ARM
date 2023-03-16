/**
  ******************************************************************************
  * 文件:    stm32f4xx_sdio.c
  * 作者:    MCD Application Team
  * 版本:    V1.8.0
  * 日期:    04-November-2016
  * 简介:    此文件提供固件功能，以管理安全数字输入/输出接口(SDIO)外围设备的以下功能:
  *           + 初始化和配置
  *           + Command path state machine (CPSM) management
  *           + Data path state machine (DPSM) management
  *           + SDIO IO Cards mode management
  *           + CE-ATA mode management
  *           + DMA传输管理
  *           + 中断和标志管理
  *
@verbatim

 ===================================================================
                 ##### 如何使用这个驱动程序 #####
 ===================================================================
 [..]
   (#) SDIO时钟(SDIOCLK=48 MHz)来自PLL(PLL48CLK)的特定输出。
            在开始使用SDIO外围设备之前，请确保PLL配置良好。
       SDIO外围设备使用两个时钟信号:
       (++) SDIO适配器时钟(SDIOCLK=48 MHz)
       (++) APB2总线时钟(PCLK2)

       -@@- PCLK2和SDIO_CK时钟频率必须符合以下条件:
           Frequency(PCLK2) >= (3 / 8 x Frequency(SDIO_CK))

   (#) 使用RCC_APB2PeriphClockCmd(RCC_AB2Periph_SDIO，Enable)启用外围时钟。

   (#) 根据SDIO模式，使用 RCC_AHB1PeriphClockCmd()函数。
       I/O可以是以下配置之一:
       (++) 1位数据长度:SDIO_CMD、SDIO_CK和D0。
       (++) 4位数据长度:SDIO_CMD、SDIO_CK和D[3:0]。
       (++) 8位数据长度:SDIO_CMD、SDIO_CK和D[7:0]。

   (#) 外围备用功能:
       (++) 使用GPIO_PinAFConfig()函数将管脚连接到所需外围设备的备用功能(AF)
       (++) 通过以下方式在备用功能中配置所需引脚:
           GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF
       (++) 通过GPIO_PuPd、GPIO_OType和GPIO_speed成员选择类型、上拉/下拉和输出速度
       (++) 调用GPIO_Init()函数

   (#) 使用SDIO_Init()函数对时钟边缘、时钟旁路、时钟节能、总线范围、硬件、
        流量控制和时钟分配器进行编程。

   (#) 使用SDIO_SetPowerState(SDIO_PowerState_ON)函数启用开机状态。

   (#) 使用SDIO_ClockCmd()函数启用时钟。

   (#) 如果需要使用中断模式，请使用函数SDIO_ITConfig()启用NVIC和相应的中断。

   (#) 使用DMA模式时
(++)使用DMA_Init()函数配置DMA
(++)使用SDIO_DMACmd()函数激活所需的通道请求

   (#) 使用DMA模式时，使用DMA_Cmd()函数启用DMA。

   (#) 要控制CPSM(命令路径状态机)并向卡发送命令，请使用SDIO_SendCommand()、
        SDIO_GetCommandResponse()和SDIO_GetResponse()函数。
        首先，用户必须根据要发送的选定命令填充命令结构(指向SDIO_CmdInitTypeDef的指针)。
       应填写的参数包括:
        (++)命令参数
        (++)命令索引
        (++)命令响应类型
        (++)命令等待
        (++)CPSM状态(启用或禁用)。

       -@@- 要检查命令是否接收良好，请使用SDIO_GetCommandResponse()读取SDIO_CMDRESP寄存器。
           SDIO响应寄存器(SDIO_RESP1到SDIO_RESP2)使用SDIO_GetResponse()函数。

   (#) 要控制DPSM(数据路径状态机)并向/从卡发送/接收数据，请使用SDIO_DataConfig()、
        SDIO_GetDataCounter()、SD IO_ReadData()、S DIO_WriteData()和
        SDIO_GetFIFOCount()函数。

 *** 读取操作 ***
 =======================
 [..]
   (#) 首先，用户必须根据要接收的选定数据类型填充数据结构(指向SDIO_DataInitTypeDef的指针)。
       应填写的参数包括:
        (++)数据超时
        (++)数据长度
        (++)数据块大小
        (++)数据传输方向:应来自卡(到SDIO)
        (++)数据传输模式
        (++)DPSM状态(启用或禁用)

   (#) 配置SDIO资源以根据所选传输模式从卡接收数据(请参阅步骤8、9和10)。

   (#)  发送选定的读取命令(请参阅步骤11)。

   (#) 使用SDIO标志/中断检查传输状态。

 *** 写入操作 ***
 ========================
 [..]
   (#) 首先，用户必须根据要接收的选定数据类型填充数据结构(指向SDIO_DataInitTypeDef的指针)。
       应填写的参数包括:
        (++)数据超时
        (++)数据长度
        (++)数据块大小
        (++)数据传输方向:应该是到卡(到卡)
        (++)数据传输模式
        (++)DPSM状态(启用或禁用)

   (#) 配置SDIO资源以根据所选传输模式将数据发送到卡(请参阅步骤8、9和10)。

   (#) 发送选定的写入命令(请参阅步骤11)。

   (#) 使用SDIO标志/中断检查传输状态。


@endverbatim
  *
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
#include "stm32f4xx_sdio.h"
#include "stm32f4xx_rcc.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @defgroup SDIO
  * 简介: SDIO 驱动模块
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* 私有宏 ------------------------------------------------------------*/

/* ------------ SDIO registers bit address in the alias region ----------- */
#define SDIO_OFFSET                (SDIO_BASE - PERIPH_BASE)

/* --- CLKCR 寄存器 ---*/
/* Alias word address of CLKEN 位 */
#define CLKCR_OFFSET              (SDIO_OFFSET + 0x04)
#define CLKEN_BitNumber           0x08
#define CLKCR_CLKEN_BB            (PERIPH_BB_BASE + (CLKCR_OFFSET * 32) + (CLKEN_BitNumber * 4))

/* --- CMD 寄存器 ---*/
/* Alias word address of SDIOSUSPEND 位 */
#define CMD_OFFSET                (SDIO_OFFSET + 0x0C)
#define SDIOSUSPEND_BitNumber     0x0B
#define CMD_SDIOSUSPEND_BB        (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (SDIOSUSPEND_BitNumber * 4))

/* Alias word address of ENCMDCOMPL 位 */
#define ENCMDCOMPL_BitNumber      0x0C
#define CMD_ENCMDCOMPL_BB         (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (ENCMDCOMPL_BitNumber * 4))

/* Alias word address of NIEN 位 */
#define NIEN_BitNumber            0x0D
#define CMD_NIEN_BB               (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (NIEN_BitNumber * 4))

/* Alias word address of ATACMD 位 */
#define ATACMD_BitNumber          0x0E
#define CMD_ATACMD_BB             (PERIPH_BB_BASE + (CMD_OFFSET * 32) + (ATACMD_BitNumber * 4))

/* --- DCTRL 寄存器 ---*/
/* Alias word address of DMAEN 位 */
#define DCTRL_OFFSET              (SDIO_OFFSET + 0x2C)
#define DMAEN_BitNumber           0x03
#define DCTRL_DMAEN_BB            (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (DMAEN_BitNumber * 4))

/* Alias word address of RWSTART 位 */
#define RWSTART_BitNumber         0x08
#define DCTRL_RWSTART_BB          (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWSTART_BitNumber * 4))

/* Alias word address of RWSTOP 位 */
#define RWSTOP_BitNumber          0x09
#define DCTRL_RWSTOP_BB           (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWSTOP_BitNumber * 4))

/* Alias word address of RWMOD 位 */
#define RWMOD_BitNumber           0x0A
#define DCTRL_RWMOD_BB            (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (RWMOD_BitNumber * 4))

/* Alias word address of SDIOEN 位 */
#define SDIOEN_BitNumber          0x0B
#define DCTRL_SDIOEN_BB           (PERIPH_BB_BASE + (DCTRL_OFFSET * 32) + (SDIOEN_BitNumber * 4))

/* ---------------------- SDIO寄存器位掩码 ------------------------ */
/* --- CLKCR 寄存器 ---*/
/* CLKCR register clear mask */
#define CLKCR_CLEAR_MASK         ((uint32_t)0xFFFF8100)

/* --- PWRCTRL 寄存器 ---*/
/* SDIO PWRCTRL Mask */
#define PWR_PWRCTRL_MASK         ((uint32_t)0xFFFFFFFC)

/* --- DCTRL 寄存器 ---*/
/* SDIO DCTRL Clear Mask */
#define DCTRL_CLEAR_MASK         ((uint32_t)0xFFFFFF08)

/* --- CMD 寄存器 ---*/
/* CMD Register clear mask */
#define CMD_CLEAR_MASK           ((uint32_t)0xFFFFF800)

/* SDIO RESP Registers Address */
#define SDIO_RESP_ADDR           ((uint32_t)(SDIO_BASE + 0x14))

/* 私有宏 -------------------------------------------------------------*/
/* 私有变量 ---------------------------------------------------------*/
/* 私有函数原型 -----------------------------------------------*/
/* 私有函数 ---------------------------------------------------------*/

/** @defgroup SDIO_Private_Functions
  * @{
  */

/** @defgroup SDIO_Group1 初始化和配置函数
 *  简介   初始化和配置函数
 *
@verbatim
 ===============================================================================
              ##### 初始化和配置函数 #####
 ===============================================================================

@endverbatim
  * @{
  */

/**
  * 简介:  将SDIO外设寄存器去初始化为其默认复位值。
  * 参数: 无
  * 返回值: 无
  */
void SDIO_DeInit(void) {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SDIO, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SDIO, DISABLE);
}

/**
  * 简介:  根据SDIO_InitStruct中指定的参数初始化SDIO外设。
  * 参数:  SDIO_InitStruct : pointer to a SDIO_InitTypeDef structure
  *         that contains the configuration information for the SDIO 外设.
  * 返回值: 无
  */
void SDIO_Init(SDIO_InitTypeDef* SDIO_InitStruct) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_SDIO_CLOCK_EDGE(SDIO_InitStruct->SDIO_ClockEdge));
    assert_param(IS_SDIO_CLOCK_BYPASS(SDIO_InitStruct->SDIO_ClockBypass));
    assert_param(IS_SDIO_CLOCK_POWER_SAVE(SDIO_InitStruct->SDIO_ClockPowerSave));
    assert_param(IS_SDIO_BUS_WIDE(SDIO_InitStruct->SDIO_BusWide));
    assert_param(IS_SDIO_HARDWARE_FLOW_CONTROL(SDIO_InitStruct->SDIO_HardwareFlowControl));

    /*---------------------------- SDIO CLKCR 配置 ------------------------*/
    /* 获取SDIO CLKCR 值 */
    tmpreg = SDIO->CLKCR;

    /* 清除 CLKDIV, PWRSAV, BYPASS, WIDBUS, NEGEDGE, HWFC_EN 位 */
    tmpreg &= CLKCR_CLEAR_MASK;

    /* Set CLKDIV bits according to SDIO_ClockDiv 值 */
    /* Set PWRSAV bit according to SDIO_ClockPowerSave 值 */
    /* Set BYPASS bit according to SDIO_ClockBypass 值 */
    /* Set WIDBUS bits according to SDIO_BusWide 值 */
    /* Set NEGEDGE bits according to SDIO_ClockEdge 值 */
    /* Set HWFC_EN bits according to SDIO_HardwareFlowControl 值 */
    tmpreg |= (SDIO_InitStruct->SDIO_ClockDiv  | SDIO_InitStruct->SDIO_ClockPowerSave |
               SDIO_InitStruct->SDIO_ClockBypass | SDIO_InitStruct->SDIO_BusWide |
               SDIO_InitStruct->SDIO_ClockEdge | SDIO_InitStruct->SDIO_HardwareFlowControl);

    /* 写入SDIO CLKCR */
    SDIO->CLKCR = tmpreg;
}

/**
  * 简介:  用每个SDIO_InitStruct成员的默认值填充其默认值。
  * 参数:  SDIO_InitStruct: pointer to an SDIO_InitTypeDef structure which
  *         will be initialized.
  * 返回值: 无
  */
void SDIO_StructInit(SDIO_InitTypeDef* SDIO_InitStruct) {
    /* SDIO_InitStruct members default 值 */
    SDIO_InitStruct->SDIO_ClockDiv = 0x00;
    SDIO_InitStruct->SDIO_ClockEdge = SDIO_ClockEdge_Rising;
    SDIO_InitStruct->SDIO_ClockBypass = SDIO_ClockBypass_Disable;
    SDIO_InitStruct->SDIO_ClockPowerSave = SDIO_ClockPowerSave_Disable;
    SDIO_InitStruct->SDIO_BusWide = SDIO_BusWide_1b;
    SDIO_InitStruct->SDIO_HardwareFlowControl = SDIO_HardwareFlowControl_Disable;
}

/**
  * 简介:  启用或禁用 SDIO 时钟。
  * 参数:  NewState: 新状态-> SDIO Clock.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_ClockCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) CLKCR_CLKEN_BB = (uint32_t)NewState;
}

/**
  * 简介:  设置控制器的电源状态。
  * 参数:  SDIO_PowerState: 新状态-> Power state.
  *          此参数可以是以下值之一:
  *            @arg SDIO_PowerState_OFF: SDIO Power OFF
  *            @arg SDIO_PowerState_ON: SDIO Power ON
  * 返回值: 无
  */
void SDIO_SetPowerState(uint32_t SDIO_PowerState) {
    /* 检查参数 */
    assert_param(IS_SDIO_POWER_STATE(SDIO_PowerState));

    SDIO->POWER = SDIO_PowerState;
}

/**
  * 简介:  获取控制器的电源状态。
  * 参数: 无
  * 返回值: Power status of the controller. The returned value can be one of the
  *         following values:
  *            - 0x00: Power OFF
  *            - 0x02: Power UP
  *            - 0x03: Power ON
  */
uint32_t SDIO_GetPowerState(void) {
    return (SDIO->POWER & (~PWR_PWRCTRL_MASK));
}

/**
  * @}
  */

/** @defgroup SDIO_Group2 Command path state machine (CPSM) management functions
 *  简介   Command path state machine (CPSM) management functions
 *
@verbatim
 ===============================================================================
        ##### Command path state machine (CPSM) management functions #####
 ===============================================================================

  This section provide functions allowing to program and read the Command path
  state machine (CPSM).

@endverbatim
  * @{
  */

/**
  * 简介:  根据SDIO_CmdInitStruct中指定的参数初始化SDIO命令并发送命令。
  * 参数:  SDIO_CmdInitStruct : pointer to a SDIO_CmdInitTypeDef
  *         structure that contains the configuration information for the SDIO
  *         command.
  * 返回值: 无
  */
void SDIO_SendCommand(SDIO_CmdInitTypeDef *SDIO_CmdInitStruct) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_SDIO_CMD_INDEX(SDIO_CmdInitStruct->SDIO_CmdIndex));
    assert_param(IS_SDIO_RESPONSE(SDIO_CmdInitStruct->SDIO_Response));
    assert_param(IS_SDIO_WAIT(SDIO_CmdInitStruct->SDIO_Wait));
    assert_param(IS_SDIO_CPSM(SDIO_CmdInitStruct->SDIO_CPSM));

    /*---------------------------- SDIO ARG 配置 ------------------------*/
    /* 设置 SDIO Argument 值 */
    SDIO->ARG = SDIO_CmdInitStruct->SDIO_Argument;

    /*---------------------------- SDIO CMD 配置 ------------------------*/
    /* 获取SDIO CMD 值 */
    tmpreg = SDIO->CMD;
    /* 清除 CMDINDEX, WAITRESP, WAITINT, WAITPEND, CPSMEN 位 */
    tmpreg &= CMD_CLEAR_MASK;
    /* Set CMDINDEX bits according to SDIO_CmdIndex 值 */
    /* Set WAITRESP bits according to SDIO_Response 值 */
    /* Set WAITINT and WAITPEND bits according to SDIO_Wait 值 */
    /* Set CPSMEN bits according to SDIO_CPSM 值 */
    tmpreg |= (uint32_t)SDIO_CmdInitStruct->SDIO_CmdIndex | SDIO_CmdInitStruct->SDIO_Response
              | SDIO_CmdInitStruct->SDIO_Wait | SDIO_CmdInitStruct->SDIO_CPSM;

    /* 写入SDIO CMD */
    SDIO->CMD = tmpreg;
}

/**
  * 简介:  用每个SDIO_CmdInitStruct成员的默认值填充该成员。
  * 参数:  SDIO_CmdInitStruct: pointer to an SDIO_CmdInitTypeDef
  *         structure which will be initialized.
  * 返回值: 无
  */
void SDIO_CmdStructInit(SDIO_CmdInitTypeDef* SDIO_CmdInitStruct) {
    /* SDIO_CmdInitStruct members default 值 */
    SDIO_CmdInitStruct->SDIO_Argument = 0x00;
    SDIO_CmdInitStruct->SDIO_CmdIndex = 0x00;
    SDIO_CmdInitStruct->SDIO_Response = SDIO_Response_No;
    SDIO_CmdInitStruct->SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStruct->SDIO_CPSM = SDIO_CPSM_Disable;
}

/**
  * 简介:  返回接收到响应的最后一个命令的命令索引
  * 参数: 无
  * 返回值: Returns the command index of the last command response received.
  */
uint8_t SDIO_GetCommandResponse(void) {
    return (uint8_t)(SDIO->RESPCMD);
}

/**
  * 简介:  返回从卡收到的对最后一个命令的响应。
  * 参数:  SDIO_RESP: Specifies the SDIO response register.
  *          此参数可以是以下值之一:
  *            @arg SDIO_RESP1: Response Register 1
  *            @arg SDIO_RESP2: Response Register 2
  *            @arg SDIO_RESP3: Response Register 3
  *            @arg SDIO_RESP4: Response Register 4
  * 返回值: The Corresponding response register value.
  */
uint32_t SDIO_GetResponse(uint32_t SDIO_RESP) {
    __IO uint32_t tmp = 0;

    /* 检查参数 */
    assert_param(IS_SDIO_RESP(SDIO_RESP));

    tmp = SDIO_RESP_ADDR + SDIO_RESP;

    return (*(__IO uint32_t *) tmp);
}

/**
  * @}
  */

/** @defgroup SDIO_Group3 Data path state machine (DPSM) management functions
 *  简介   Data path state machine (DPSM) management functions
 *
@verbatim
 ===============================================================================
         ##### Data path state machine (DPSM) management functions #####
 ===============================================================================

  This section provide functions allowing to program and read the Data path
  state machine (DPSM).

@endverbatim
  * @{
  */

/**
  * 简介:  根据SDIO_DataInitStruct中指定的参数初始化 SDIO 数据路径。
  * 参数:  SDIO_DataInitStruct : pointer to a SDIO_DataInitTypeDef structure
  *         that contains the configuration information for the SDIO command.
  * 返回值: 无
  */
void SDIO_DataConfig(SDIO_DataInitTypeDef* SDIO_DataInitStruct) {
    uint32_t tmpreg = 0;

    /* 检查参数 */
    assert_param(IS_SDIO_DATA_LENGTH(SDIO_DataInitStruct->SDIO_DataLength));
    assert_param(IS_SDIO_BLOCK_SIZE(SDIO_DataInitStruct->SDIO_DataBlockSize));
    assert_param(IS_SDIO_TRANSFER_DIR(SDIO_DataInitStruct->SDIO_TransferDir));
    assert_param(IS_SDIO_TRANSFER_MODE(SDIO_DataInitStruct->SDIO_TransferMode));
    assert_param(IS_SDIO_DPSM(SDIO_DataInitStruct->SDIO_DPSM));

    /*---------------------------- SDIO DTIMER 配置 ---------------------*/
    /* 设置 SDIO Data TimeOut 值 */
    SDIO->DTIMER = SDIO_DataInitStruct->SDIO_DataTimeOut;

    /*---------------------------- SDIO DLEN 配置 -----------------------*/
    /* 设置 SDIO DataLength 值 */
    SDIO->DLEN = SDIO_DataInitStruct->SDIO_DataLength;

    /*---------------------------- SDIO DCTRL 配置 ----------------------*/
    /* 获取SDIO DCTRL 值 */
    tmpreg = SDIO->DCTRL;
    /* 清除 DEN, DTMODE, DTDIR and DBCKSIZE 位 */
    tmpreg &= DCTRL_CLEAR_MASK;
    /* Set DEN bit according to SDIO_DPSM 值 */
    /* Set DTMODE bit according to SDIO_TransferMode 值 */
    /* Set DTDIR bit according to SDIO_TransferDir 值 */
    /* Set DBCKSIZE bits according to SDIO_DataBlockSize 值 */
    tmpreg |= (uint32_t)SDIO_DataInitStruct->SDIO_DataBlockSize | SDIO_DataInitStruct->SDIO_TransferDir
              | SDIO_DataInitStruct->SDIO_TransferMode | SDIO_DataInitStruct->SDIO_DPSM;

    /* 写入SDIO DCTRL */
    SDIO->DCTRL = tmpreg;
}

/**
  * 简介:  用每个SDIO_DataInitStruct成员的默认值填充每个成员。
  * 参数:  SDIO_DataInitStruct: pointer to an SDIO_DataInitTypeDef structure
  *         which will be initialized.
  * 返回值: 无
  */
void SDIO_DataStructInit(SDIO_DataInitTypeDef* SDIO_DataInitStruct) {
    /* SDIO_DataInitStruct members default 值 */
    SDIO_DataInitStruct->SDIO_DataTimeOut = 0xFFFFFFFF;
    SDIO_DataInitStruct->SDIO_DataLength = 0x00;
    SDIO_DataInitStruct->SDIO_DataBlockSize = SDIO_DataBlockSize_1b;
    SDIO_DataInitStruct->SDIO_TransferDir = SDIO_TransferDir_ToCard;
    SDIO_DataInitStruct->SDIO_TransferMode = SDIO_TransferMode_Block;
    SDIO_DataInitStruct->SDIO_DPSM = SDIO_DPSM_Disable;
}

/**
  * 简介:  返回要传输的剩余数据字节数。
  * 参数: 无
  * 返回值: Number of remaining data bytes to be transferred
  */
uint32_t SDIO_GetDataCounter(void) {
    return SDIO->DCOUNT;
}

/**
  * 简介:  从Rx FIFO读取一个数据字。
  * 参数: 无
  * 返回值: Data received
  */
uint32_t SDIO_ReadData(void) {
    return SDIO->FIFO;
}

/**
  * 简介:  将一个数据字写入 Tx FIFO。
  * 参数:  Data: 32-bit data word to write.
  * 返回值: 无
  */
void SDIO_WriteData(uint32_t Data) {
    SDIO->FIFO = Data;
}

/**
  * 简介:  返回要写入 FIFO 或从 FIFO 读取的剩余字数。
  * 参数: 无
  * 返回值: Remaining number of words.
  */
uint32_t SDIO_GetFIFOCount(void) {
    return SDIO->FIFOCNT;
}

/**
  * @}
  */

/** @defgroup SDIO_Group4 SDIO IO卡模式管理功能
 *  简介   SDIO IO卡模式管理功能
 *
@verbatim
 ===============================================================================
               ##### SDIO IO卡模式管理功能 #####
 ===============================================================================

  本节提供允许编程和读取SDIO IO卡的功能。

@endverbatim
  * @{
  */

/**
  * 简介:  启动 SD I/O 读取等待操作。
  * 参数:  NewState: 新状态-> Start SDIO Read Wait operation.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_StartSDIOReadWait(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) DCTRL_RWSTART_BB = (uint32_t) NewState;
}

/**
  * 简介:  停止 SD I/O 读取等待操作。
  * 参数:  NewState: 新状态-> Stop SDIO Read Wait operation.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_StopSDIOReadWait(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) DCTRL_RWSTOP_BB = (uint32_t) NewState;
}

/**
  * 简介:  设置插入读取等待间隔的两个选项之一。
  * 参数:  SDIO_ReadWaitMode: SD I/O Read Wait operation mode.
  *          这个参数可以是:
  *            @arg SDIO_ReadWaitMode_CLK: Read Wait control by stopping SDIOCLK
  *            @arg SDIO_ReadWaitMode_DATA2: Read Wait control using SDIO_DATA2
  * 返回值: 无
  */
void SDIO_SetSDIOReadWaitMode(uint32_t SDIO_ReadWaitMode) {
    /* 检查参数 */
    assert_param(IS_SDIO_READWAIT_MODE(SDIO_ReadWaitMode));

    *(__IO uint32_t *) DCTRL_RWMOD_BB = SDIO_ReadWaitMode;
}

/**
  * 简介:  启用或禁用 SD I/O 模式操作。
  * 参数:  NewState: 新状态->  SDIO specific operation.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_SetSDIOOperation(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) DCTRL_SDIOEN_BB = (uint32_t)NewState;
}

/**
  * 简介:  启用或禁用 SD I/O 模式挂起命令发送。
  * 参数:  NewState: 新状态-> SD I/O Mode suspend command.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_SendSDIOSuspendCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) CMD_SDIOSUSPEND_BB = (uint32_t)NewState;
}

/**
  * @}
  */

/** @defgroup SDIO_Group5 CE-ATA模式管理功能
 *  简介   CE-ATA模式管理功能
 *
@verbatim
 ===============================================================================
                  ##### CE-ATA模式管理功能 #####
 ===============================================================================

  This section provide functions allowing to program and read the CE-ATA card.

@endverbatim
  * @{
  */

/**
  * 简介:  启用或禁用命令完成信号。
  * 参数:  NewState: 新状态->  command completion signal.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_CommandCompletionCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) CMD_ENCMDCOMPL_BB = (uint32_t)NewState;
}

/**
  * 简介:  启用或禁用 CE-ATA 中断。
  * 参数:  NewState: 新状态->  CE-ATA interrupt.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_CEATAITCmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) CMD_NIEN_BB = (uint32_t)((~((uint32_t)NewState)) & ((uint32_t)0x1));
}

/**
  * 简介:  发送 CE-ATA 命令 (CMD61)。
  * 参数:  NewState: 新状态->  CE-ATA command.
  *         此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_SendCEATACmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) CMD_ATACMD_BB = (uint32_t)NewState;
}

/**
  * @}
  */

/** @defgroup SDIO_Group6 DMA传输管理 functions
 *  简介   DMA传输管理 functions
 *
@verbatim
 ===============================================================================
                  ##### DMA传输管理 functions #####
 ===============================================================================

  This section provide functions allowing to program SDIO DMA transfer.

@endverbatim
  * @{
  */

/**
  * 简介:  启用或禁用SDIO DMA request.
  * 参数:  NewState: 新状态-> selected SDIO DMA request.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_DMACmd(FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    *(__IO uint32_t *) DCTRL_DMAEN_BB = (uint32_t)NewState;
}

/**
  * @}
  */

/** @defgroup SDIO_Group7 中断和标记管理函数
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
  * 简介:  启用或禁用SDIO 中断.
  * 参数:  SDIO_IT:开启或关闭SDIO中断源。
  *          可以为以下值之一或组合:
  *            @arg SDIO_IT_CCRCFAIL: Command response received (CRC check failed) interrupt
  *            @arg SDIO_IT_DCRCFAIL: Data block sent/received (CRC check failed) interrupt
  *            @arg SDIO_IT_CTIMEOUT: Command response timeout interrupt
  *            @arg SDIO_IT_DTIMEOUT: Data timeout interrupt
  *            @arg SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt
  *            @arg SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt
  *            @arg SDIO_IT_CMDREND:  Command response received (CRC check passed) interrupt
  *            @arg SDIO_IT_CMDSENT:  Command sent (no response required) interrupt
  *            @arg SDIO_IT_DATAEND:  Data end (data counter, SDIDCOUNT, is zero) interrupt
  *            @arg SDIO_IT_STBITERR: Start bit not detected on all data signals in wide
  *                                   bus mode interrupt
  *            @arg SDIO_IT_DBCKEND:  Data block sent/received (CRC check passed) interrupt
  *            @arg SDIO_IT_CMDACT:   Command transfer in progress interrupt
  *            @arg SDIO_IT_TXACT:    Data transmit in progress interrupt
  *            @arg SDIO_IT_RXACT:    Data receive in progress interrupt
  *            @arg SDIO_IT_TXFIFOHE: Transmit FIFO Half Empty interrupt
  *            @arg SDIO_IT_RXFIFOHF: Receive FIFO Half Full interrupt
  *            @arg SDIO_IT_TXFIFOF:  Transmit FIFO full interrupt
  *            @arg SDIO_IT_RXFIFOF:  Receive FIFO full interrupt
  *            @arg SDIO_IT_TXFIFOE:  Transmit FIFO empty interrupt
  *            @arg SDIO_IT_RXFIFOE:  Receive FIFO empty interrupt
  *            @arg SDIO_IT_TXDAVL:   Data available in transmit FIFO interrupt
  *            @arg SDIO_IT_RXDAVL:   Data available in receive FIFO interrupt
  *            @arg SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt
  *            @arg SDIO_IT_CEATAEND: CE-ATA command completion signal received for CMD61 interrupt
  * 参数:  NewState: 新状态-> specified SDIO interrupts.
  *          此参数可以是:ENABLE或DISABLE。
  * 返回值: 无
  */
void SDIO_ITConfig(uint32_t SDIO_IT, FunctionalState NewState) {
    /* 检查参数 */
    assert_param(IS_SDIO_IT(SDIO_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE) {
        /* 启用 SDIO interrupts */
        SDIO->MASK |= SDIO_IT;
    } else {
        /* 禁用 SDIO interrupts */
        SDIO->MASK &= ~SDIO_IT;
    }
}

/**
  * 简介:  检查是否设置了指定的 SDIO 标志。
  * 参数:  SDIO_FLAG: 指定要检查的标志。
  *          此参数可以是以下值之一:
  *            @arg SDIO_FLAG_CCRCFAIL: Command response received (CRC check failed)
  *            @arg SDIO_FLAG_DCRCFAIL: Data block sent/received (CRC check failed)
  *            @arg SDIO_FLAG_CTIMEOUT: Command response timeout
  *            @arg SDIO_FLAG_DTIMEOUT: Data timeout
  *            @arg SDIO_FLAG_TXUNDERR: Transmit FIFO underrun error
  *            @arg SDIO_FLAG_RXOVERR:  Received FIFO overrun error
  *            @arg SDIO_FLAG_CMDREND:  Command response received (CRC check passed)
  *            @arg SDIO_FLAG_CMDSENT:  Command sent (no response required)
  *            @arg SDIO_FLAG_DATAEND:  Data end (data counter, SDIDCOUNT, is zero)
  *            @arg SDIO_FLAG_STBITERR: Start bit not detected on all data signals in wide bus mode.
  *            @arg SDIO_FLAG_DBCKEND:  Data block sent/received (CRC check passed)
  *            @arg SDIO_FLAG_CMDACT:   Command transfer in progress
  *            @arg SDIO_FLAG_TXACT:    Data transmit in progress
  *            @arg SDIO_FLAG_RXACT:    Data receive in progress
  *            @arg SDIO_FLAG_TXFIFOHE: Transmit FIFO Half Empty
  *            @arg SDIO_FLAG_RXFIFOHF: Receive FIFO Half Full
  *            @arg SDIO_FLAG_TXFIFOF:  Transmit FIFO full
  *            @arg SDIO_FLAG_RXFIFOF:  Receive FIFO full
  *            @arg SDIO_FLAG_TXFIFOE:  Transmit FIFO empty
  *            @arg SDIO_FLAG_RXFIFOE:  Receive FIFO empty
  *            @arg SDIO_FLAG_TXDAVL:   Data available in transmit FIFO
  *            @arg SDIO_FLAG_RXDAVL:   Data available in receive FIFO
  *            @arg SDIO_FLAG_SDIOIT:   SD I/O interrupt received
  *            @arg SDIO_FLAG_CEATAEND: CE-ATA command completion signal received for CMD61
  * 返回值: 新状态-> SDIO_FLAG (SET or RESET).
  */
FlagStatus SDIO_GetFlagStatus(uint32_t SDIO_FLAG) {
    FlagStatus bitstatus = RESET;

    /* 检查参数 */
    assert_param(IS_SDIO_FLAG(SDIO_FLAG));

    if ((SDIO->STA & SDIO_FLAG) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
  * 简介:  清除 SDIO 的挂起标志。
  * 参数:  SDIO_FLAG: 指定要清除的标志。
  *          可以为以下值之一或组合:
  *            @arg SDIO_FLAG_CCRCFAIL: Command response received (CRC check failed)
  *            @arg SDIO_FLAG_DCRCFAIL: Data block sent/received (CRC check failed)
  *            @arg SDIO_FLAG_CTIMEOUT: Command response timeout
  *            @arg SDIO_FLAG_DTIMEOUT: Data timeout
  *            @arg SDIO_FLAG_TXUNDERR: Transmit FIFO underrun error
  *            @arg SDIO_FLAG_RXOVERR:  Received FIFO overrun error
  *            @arg SDIO_FLAG_CMDREND:  Command response received (CRC check passed)
  *            @arg SDIO_FLAG_CMDSENT:  Command sent (no response required)
  *            @arg SDIO_FLAG_DATAEND:  Data end (data counter, SDIDCOUNT, is zero)
  *            @arg SDIO_FLAG_STBITERR: Start bit not detected on all data signals in wide bus mode
  *            @arg SDIO_FLAG_DBCKEND:  Data block sent/received (CRC check passed)
  *            @arg SDIO_FLAG_SDIOIT:   SD I/O interrupt received
  *            @arg SDIO_FLAG_CEATAEND: CE-ATA command completion signal received for CMD61
  * 返回值: 无
  */
void SDIO_ClearFlag(uint32_t SDIO_FLAG) {
    /* 检查参数 */
    assert_param(IS_SDIO_CLEAR_FLAG(SDIO_FLAG));

    SDIO->ICR = SDIO_FLAG;
}

/**
  * 简介:  检查指定的SDIO中断是否发生。
  * 参数:  SDIO_IT: 指定要检查的SDIO中断源。
  *          此参数可以是以下值之一:
  *            @arg SDIO_IT_CCRCFAIL: Command response received (CRC check failed) interrupt
  *            @arg SDIO_IT_DCRCFAIL: Data block sent/received (CRC check failed) interrupt
  *            @arg SDIO_IT_CTIMEOUT: Command response timeout interrupt
  *            @arg SDIO_IT_DTIMEOUT: Data timeout interrupt
  *            @arg SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt
  *            @arg SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt
  *            @arg SDIO_IT_CMDREND:  Command response received (CRC check passed) interrupt
  *            @arg SDIO_IT_CMDSENT:  Command sent (no response required) interrupt
  *            @arg SDIO_IT_DATAEND:  Data end (data counter, SDIDCOUNT, is zero) interrupt
  *            @arg SDIO_IT_STBITERR: Start bit not detected on all data signals in wide
  *                                   bus mode interrupt
  *            @arg SDIO_IT_DBCKEND:  Data block sent/received (CRC check passed) interrupt
  *            @arg SDIO_IT_CMDACT:   Command transfer in progress interrupt
  *            @arg SDIO_IT_TXACT:    Data transmit in progress interrupt
  *            @arg SDIO_IT_RXACT:    Data receive in progress interrupt
  *            @arg SDIO_IT_TXFIFOHE: Transmit FIFO Half Empty interrupt
  *            @arg SDIO_IT_RXFIFOHF: Receive FIFO Half Full interrupt
  *            @arg SDIO_IT_TXFIFOF:  Transmit FIFO full interrupt
  *            @arg SDIO_IT_RXFIFOF:  Receive FIFO full interrupt
  *            @arg SDIO_IT_TXFIFOE:  Transmit FIFO empty interrupt
  *            @arg SDIO_IT_RXFIFOE:  Receive FIFO empty interrupt
  *            @arg SDIO_IT_TXDAVL:   Data available in transmit FIFO interrupt
  *            @arg SDIO_IT_RXDAVL:   Data available in receive FIFO interrupt
  *            @arg SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt
  *            @arg SDIO_IT_CEATAEND: CE-ATA command completion signal received for CMD61 interrupt
  * 返回值: 新状态-> SDIO_IT (SET or RESET).
  */
ITStatus SDIO_GetITStatus(uint32_t SDIO_IT) {
    ITStatus bitstatus = RESET;

    /* 检查参数 */
    assert_param(IS_SDIO_GET_IT(SDIO_IT));

    if ((SDIO->STA & SDIO_IT) != (uint32_t)RESET) {
        bitstatus = SET;
    } else {
        bitstatus = RESET;
    }

    return bitstatus;
}

/**
  * 简介:  清除 SDIO 的中断挂起位。
  * 参数:  SDIO_IT: 指定要清除的中断等待位。
  *          可以为以下值之一或组合:
  *            @arg SDIO_IT_CCRCFAIL: Command response received (CRC check failed) interrupt
  *            @arg SDIO_IT_DCRCFAIL: Data block sent/received (CRC check failed) interrupt
  *            @arg SDIO_IT_CTIMEOUT: Command response timeout interrupt
  *            @arg SDIO_IT_DTIMEOUT: Data timeout interrupt
  *            @arg SDIO_IT_TXUNDERR: Transmit FIFO underrun error interrupt
  *            @arg SDIO_IT_RXOVERR:  Received FIFO overrun error interrupt
  *            @arg SDIO_IT_CMDREND:  Command response received (CRC check passed) interrupt
  *            @arg SDIO_IT_CMDSENT:  Command sent (no response required) interrupt
  *            @arg SDIO_IT_DATAEND:  Data end (data counter, SDIO_DCOUNT, is zero) interrupt
  *            @arg SDIO_IT_STBITERR: Start bit not detected on all data signals in wide
  *                                   bus mode interrupt
  *            @arg SDIO_IT_SDIOIT:   SD I/O interrupt received interrupt
  *            @arg SDIO_IT_CEATAEND: CE-ATA command completion signal received for CMD61
  * 返回值: 无
  */
void SDIO_ClearITPendingBit(uint32_t SDIO_IT) {
    /* 检查参数 */
    assert_param(IS_SDIO_CLEAR_IT(SDIO_IT));

    SDIO->ICR = SDIO_IT;
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
