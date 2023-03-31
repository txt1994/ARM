/**
  ******************************************************************************
  * 文件:    stm32f4xx_i2c.h
  * 作者:    MCD Application Team
  * 版本:    V1.8.0
  * 日期:    04-November-2016
  * 简介:    该文件包含 I2C 固件库的所有功能原型.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4xx_I2C_H
#define __STM32F4xx_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */

/** @addtogroup I2C
  * @{
  */

/* Exported types ------------------------------------------------------------*/

/**
  * 简介:  I2C 初始化结构定义
  */

typedef struct {
    uint32_t I2C_ClockSpeed;          /*!< 指定时钟频率。
                                          此参数必须设置为低于 400kHz 的值 */

    uint16_t I2C_Mode;                /*!< 指定 I2C 模式。
                                          该参数可以是@ref I2C_mode 的值 */

    uint16_t I2C_DutyCycle;           /*!< 指定 I2C 快速模式占空比。
                                          该参数可以是@ref I2C_duty_cycle_in_fast_mode 的值 */

    uint16_t I2C_OwnAddress1;         /*!< 指定第一个设备自己的地址。
                                          该参数可以是 7 位或 10 位地址。 */

    uint16_t I2C_Ack;                 /*!< 启用或禁用确认。
                                          该参数可以是@ref I2C_acknowledgement 的值*/

    uint16_t I2C_AcknowledgedAddress; /*!< 指定是否确认 7 位或 10 位地址。
                                          该参数可以是@ref I2C_acknowledged_address 的值 */
} I2C_InitTypeDef;

/* Exported constants --------------------------------------------------------*/


/** @defgroup I2C_Exported_Constants
  * @{
  */
#define IS_I2C_ALL_PERIPH(PERIPH) (((PERIPH) == I2C1) || \
                                   ((PERIPH) == I2C2) || \
                                   ((PERIPH) == I2C3))
/** @defgroup I2C_Digital_Filter
  * @{
  */

#define IS_I2C_DIGITAL_FILTER(FILTER)   ((FILTER) <= 0x0000000F)
/**
  * @}
  */


/** @defgroup I2C_mode
  * @{
  */

#define I2C_Mode_I2C                    ((uint16_t)0x0000)
#define I2C_Mode_SMBusDevice            ((uint16_t)0x0002)
#define I2C_Mode_SMBusHost              ((uint16_t)0x000A)
#define IS_I2C_MODE(MODE) (((MODE) == I2C_Mode_I2C) || \
                           ((MODE) == I2C_Mode_SMBusDevice) || \
                           ((MODE) == I2C_Mode_SMBusHost))
/**
  * @}
  */

/** @defgroup I2C_duty_cycle_in_fast_mode
  * @{
  */

#define I2C_DutyCycle_16_9              ((uint16_t)0x4000) /*!< I2C 快速模式 Tlow/Thigh = 16/9 */
#define I2C_DutyCycle_2                 ((uint16_t)0xBFFF) /*!< I2C 快速模式 Tlow/Thigh = 2 */
#define IS_I2C_DUTY_CYCLE(CYCLE) (((CYCLE) == I2C_DutyCycle_16_9) || \
                                  ((CYCLE) == I2C_DutyCycle_2))
/**
  * @}
  */

/** @defgroup I2C_acknowledgement
  * @{
  */

#define I2C_Ack_Enable                  ((uint16_t)0x0400)
#define I2C_Ack_Disable                 ((uint16_t)0x0000)
#define IS_I2C_ACK_STATE(STATE) (((STATE) == I2C_Ack_Enable) || \
                                 ((STATE) == I2C_Ack_Disable))
/**
  * @}
  */

/** @defgroup I2C_transfer_direction
  * @{
  */

#define  I2C_Direction_Transmitter      ((uint8_t)0x00)
#define  I2C_Direction_Receiver         ((uint8_t)0x01)
#define IS_I2C_DIRECTION(DIRECTION) (((DIRECTION) == I2C_Direction_Transmitter) || \
                                     ((DIRECTION) == I2C_Direction_Receiver))
/**
  * @}
  */

/** @defgroup I2C_acknowledged_address
  * @{
  */

#define I2C_AcknowledgedAddress_7bit    ((uint16_t)0x4000)
#define I2C_AcknowledgedAddress_10bit   ((uint16_t)0xC000)
#define IS_I2C_ACKNOWLEDGE_ADDRESS(ADDRESS) (((ADDRESS) == I2C_AcknowledgedAddress_7bit) || \
        ((ADDRESS) == I2C_AcknowledgedAddress_10bit))
/**
  * @}
  */

/** @defgroup I2C_registers
  * @{
  */

#define I2C_Register_CR1                ((uint8_t)0x00)
#define I2C_Register_CR2                ((uint8_t)0x04)
#define I2C_Register_OAR1               ((uint8_t)0x08)
#define I2C_Register_OAR2               ((uint8_t)0x0C)
#define I2C_Register_DR                 ((uint8_t)0x10)
#define I2C_Register_SR1                ((uint8_t)0x14)
#define I2C_Register_SR2                ((uint8_t)0x18)
#define I2C_Register_CCR                ((uint8_t)0x1C)
#define I2C_Register_TRISE              ((uint8_t)0x20)
#define IS_I2C_REGISTER(REGISTER) (((REGISTER) == I2C_Register_CR1) || \
                                   ((REGISTER) == I2C_Register_CR2) || \
                                   ((REGISTER) == I2C_Register_OAR1) || \
                                   ((REGISTER) == I2C_Register_OAR2) || \
                                   ((REGISTER) == I2C_Register_DR) || \
                                   ((REGISTER) == I2C_Register_SR1) || \
                                   ((REGISTER) == I2C_Register_SR2) || \
                                   ((REGISTER) == I2C_Register_CCR) || \
                                   ((REGISTER) == I2C_Register_TRISE))
/**
  * @}
  */

/** @defgroup I2C_NACK_position
  * @{
  */

#define I2C_NACKPosition_Next           ((uint16_t)0x0800)
#define I2C_NACKPosition_Current        ((uint16_t)0xF7FF)
#define IS_I2C_NACK_POSITION(POSITION)  (((POSITION) == I2C_NACKPosition_Next) || \
        ((POSITION) == I2C_NACKPosition_Current))
/**
  * @}
  */

/** @defgroup I2C_SMBus_alert_pin_level
  * @{
  */

#define I2C_SMBusAlert_Low              ((uint16_t)0x2000)
#define I2C_SMBusAlert_High             ((uint16_t)0xDFFF)
#define IS_I2C_SMBUS_ALERT(ALERT) (((ALERT) == I2C_SMBusAlert_Low) || \
                                   ((ALERT) == I2C_SMBusAlert_High))
/**
  * @}
  */

/** @defgroup I2C_PEC_position
  * @{
  */

#define I2C_PECPosition_Next            ((uint16_t)0x0800)
#define I2C_PECPosition_Current         ((uint16_t)0xF7FF)
#define IS_I2C_PEC_POSITION(POSITION) (((POSITION) == I2C_PECPosition_Next) || \
                                       ((POSITION) == I2C_PECPosition_Current))
/**
  * @}
  */

/** @defgroup I2C_interrupts_definition
  * @{
  */

#define I2C_IT_BUF                      ((uint16_t)0x0400)
#define I2C_IT_EVT                      ((uint16_t)0x0200)
#define I2C_IT_ERR                      ((uint16_t)0x0100)
#define IS_I2C_CONFIG_IT(IT) ((((IT) & (uint16_t)0xF8FF) == 0x00) && ((IT) != 0x00))
/**
  * @}
  */

/** @defgroup I2C_interrupts_definition
  * @{
  */

#define I2C_IT_SMBALERT                 ((uint32_t)0x01008000)
#define I2C_IT_TIMEOUT                  ((uint32_t)0x01004000)
#define I2C_IT_PECERR                   ((uint32_t)0x01001000)
#define I2C_IT_OVR                      ((uint32_t)0x01000800)
#define I2C_IT_AF                       ((uint32_t)0x01000400)
#define I2C_IT_ARLO                     ((uint32_t)0x01000200)
#define I2C_IT_BERR                     ((uint32_t)0x01000100)
#define I2C_IT_TXE                      ((uint32_t)0x06000080)
#define I2C_IT_RXNE                     ((uint32_t)0x06000040)
#define I2C_IT_STOPF                    ((uint32_t)0x02000010)
#define I2C_IT_ADD10                    ((uint32_t)0x02000008)
#define I2C_IT_BTF                      ((uint32_t)0x02000004)
#define I2C_IT_ADDR                     ((uint32_t)0x02000002)
#define I2C_IT_SB                       ((uint32_t)0x02000001)

#define IS_I2C_CLEAR_IT(IT) ((((IT) & (uint16_t)0x20FF) == 0x00) && ((IT) != (uint16_t)0x00))

#define IS_I2C_GET_IT(IT) (((IT) == I2C_IT_SMBALERT) || ((IT) == I2C_IT_TIMEOUT) || \
                           ((IT) == I2C_IT_PECERR) || ((IT) == I2C_IT_OVR) || \
                           ((IT) == I2C_IT_AF) || ((IT) == I2C_IT_ARLO) || \
                           ((IT) == I2C_IT_BERR) || ((IT) == I2C_IT_TXE) || \
                           ((IT) == I2C_IT_RXNE) || ((IT) == I2C_IT_STOPF) || \
                           ((IT) == I2C_IT_ADD10) || ((IT) == I2C_IT_BTF) || \
                           ((IT) == I2C_IT_ADDR) || ((IT) == I2C_IT_SB))
/**
  * @}
  */

/** @defgroup I2C_flags_definition
  * @{
  */

/**
  * 简介:  SR2 register flags
  */

#define I2C_FLAG_DUALF                  ((uint32_t)0x00800000)
#define I2C_FLAG_SMBHOST                ((uint32_t)0x00400000)
#define I2C_FLAG_SMBDEFAULT             ((uint32_t)0x00200000)
#define I2C_FLAG_GENCALL                ((uint32_t)0x00100000)
#define I2C_FLAG_TRA                    ((uint32_t)0x00040000)
#define I2C_FLAG_BUSY                   ((uint32_t)0x00020000)
#define I2C_FLAG_MSL                    ((uint32_t)0x00010000)

/**
  * 简介:  SR1 register flags
  */

#define I2C_FLAG_SMBALERT               ((uint32_t)0x10008000)
#define I2C_FLAG_TIMEOUT                ((uint32_t)0x10004000)
#define I2C_FLAG_PECERR                 ((uint32_t)0x10001000)
#define I2C_FLAG_OVR                    ((uint32_t)0x10000800)
#define I2C_FLAG_AF                     ((uint32_t)0x10000400)
#define I2C_FLAG_ARLO                   ((uint32_t)0x10000200)
#define I2C_FLAG_BERR                   ((uint32_t)0x10000100)
#define I2C_FLAG_TXE                    ((uint32_t)0x10000080)
#define I2C_FLAG_RXNE                   ((uint32_t)0x10000040)
#define I2C_FLAG_STOPF                  ((uint32_t)0x10000010)
#define I2C_FLAG_ADD10                  ((uint32_t)0x10000008)
#define I2C_FLAG_BTF                    ((uint32_t)0x10000004)
#define I2C_FLAG_ADDR                   ((uint32_t)0x10000002)
#define I2C_FLAG_SB                     ((uint32_t)0x10000001)

#define IS_I2C_CLEAR_FLAG(FLAG) ((((FLAG) & (uint16_t)0x20FF) == 0x00) && ((FLAG) != (uint16_t)0x00))

#define IS_I2C_GET_FLAG(FLAG) (((FLAG) == I2C_FLAG_DUALF) || ((FLAG) == I2C_FLAG_SMBHOST) || \
                               ((FLAG) == I2C_FLAG_SMBDEFAULT) || ((FLAG) == I2C_FLAG_GENCALL) || \
                               ((FLAG) == I2C_FLAG_TRA) || ((FLAG) == I2C_FLAG_BUSY) || \
                               ((FLAG) == I2C_FLAG_MSL) || ((FLAG) == I2C_FLAG_SMBALERT) || \
                               ((FLAG) == I2C_FLAG_TIMEOUT) || ((FLAG) == I2C_FLAG_PECERR) || \
                               ((FLAG) == I2C_FLAG_OVR) || ((FLAG) == I2C_FLAG_AF) || \
                               ((FLAG) == I2C_FLAG_ARLO) || ((FLAG) == I2C_FLAG_BERR) || \
                               ((FLAG) == I2C_FLAG_TXE) || ((FLAG) == I2C_FLAG_RXNE) || \
                               ((FLAG) == I2C_FLAG_STOPF) || ((FLAG) == I2C_FLAG_ADD10) || \
                               ((FLAG) == I2C_FLAG_BTF) || ((FLAG) == I2C_FLAG_ADDR) || \
                               ((FLAG) == I2C_FLAG_SB))
/**
  * @}
  */

/** @defgroup I2C_Events
  * @{
  */

/**
 ===============================================================================
               I2C 主事件(按通信顺序分组的事件)
 ===============================================================================
 */

/**
  * 简介:  通讯开始
  *
  * 发送 START 条件(I2C_GenerateSTART() 函数)后，主机必须等待此事件。
  *	这意味着启动条件已在 I2C 总线上正确释放(总线空闲，没有其他设备在通信)。
  *
  */
/* --EV5 */
#define  I2C_EVENT_MASTER_MODE_SELECT                      ((uint32_t)0x00030001)  /* BUSY, MSL and SB flag */

/**
  * 简介:  Address Acknowledge
  *
  * After checking on EV5 (start condition correctly released on the bus), the
  * master sends the address of the slave(s) with which it will communicate
  * (I2C_Send7bitAddress() function, it also determines the direction of the communication:
  * Master transmitter or Receiver). Then the master has to wait that a slave acknowledges
  * his address. If an acknowledge is sent on the bus, one of the following events will
  * be set:
  *
  *  1) In case of Master Receiver (7-bit addressing): the I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED
  *     event is set.
  *
  *  2) In case of Master Transmitter (7-bit addressing): the I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED
  *     is set
  *
  *  3) In case of 10-Bit addressing mode, the master (just after generating the START
  *  and checking on EV5) has to send the header of 10-bit addressing mode (I2C_SendData()
  *  function). Then master should wait on EV9. It means that the 10-bit addressing
  *  header has been correctly sent on the bus. Then master should send the second part of
  *  the 10-bit address (LSB) using the function I2C_Send7bitAddress(). Then master
  *  should wait for event EV6.
  *
  */

/* --EV6 */
#define  I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED        ((uint32_t)0x00070082)  /* BUSY, MSL, ADDR, TXE and TRA flags */
#define  I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED           ((uint32_t)0x00030002)  /* BUSY, MSL and ADDR flags */
/* --EV9 */
#define  I2C_EVENT_MASTER_MODE_ADDRESS10                   ((uint32_t)0x00030008)  /* BUSY, MSL and ADD10 flags */

/**
  * 简介: Communication events
  *
  * If a communication is established (START condition generated and slave address
  * acknowledged) then the master has to check on one of the following events for
  * communication procedures:
  *
  * 1) Master 接收器模式: The master has to wait on the event EV7 then to read
  *    the data received from the slave (I2C_ReceiveData() function).
  *
  * 2) Master 发射器模式: The master has to send data (I2C_SendData()
  *    function) then to wait on event EV8 or EV8_2.
  *    These two events are similar:
  *     - EV8 means that the data has been written in the data register and is
  *       being shifted out.
  *     - EV8_2 means that the data has been physically shifted out and output
  *       on the bus.
  *     In most cases, using EV8 is sufficient for the application.
  *     Using EV8_2 leads to a slower communication but ensure more reliable test.
  *     EV8_2 is also more suitable than EV8 for testing on the last data transmission
  *     (before Stop condition generation).
  *
  *  @note In case the  user software does not guarantee that this event EV7 is
  *        managed before the current byte end of transfer, then user may check on EV7
  *        and BTF flag at the same time (ie. (I2C_EVENT_MASTER_BYTE_RECEIVED | I2C_FLAG_BTF)).
  *        In this case the communication may be slower.
  *
  */

/* Master 接收器模式 -----------------------------*/
/* --EV7 */
#define  I2C_EVENT_MASTER_BYTE_RECEIVED                    ((uint32_t)0x00030040)  /* BUSY, MSL and RXNE flags */

/* Master 发射器模式 --------------------------*/
/* --EV8 */
#define I2C_EVENT_MASTER_BYTE_TRANSMITTING                 ((uint32_t)0x00070080) /* TRA, BUSY, MSL, TXE flags */
/* --EV8_2 */
#define  I2C_EVENT_MASTER_BYTE_TRANSMITTED                 ((uint32_t)0x00070084)  /* TRA, BUSY, MSL, TXE and BTF flags */


/**
 ===============================================================================
               I2C Slave Events (Events grouped in order of communication)
 ===============================================================================
 */


/**
  * 简介:  Communication start events
  *
  * Wait on one of these events at the start of the communication. It means that
  * the I2C peripheral detected a Start condition on the bus (generated by master
  * device) followed by the peripheral address. The peripheral generates an ACK
  * condition on the bus (if the acknowledge feature is enabled through function
  * I2C_AcknowledgeConfig()) and the events listed above are set :
  *
  * 1) In normal case (only one address managed by the slave), when the address
  *   sent by the master matches the own address of the peripheral (configured by
  *   I2C_OwnAddress1 field) the I2C_EVENT_SLAVE_XXX_ADDRESS_MATCHED event is set
  *   (where XXX could be TRANSMITTER or RECEIVER).
  *
  * 2) In case the address sent by the master matches the second address of the
  *   peripheral (configured by the function I2C_OwnAddress2Config() and enabled
  *   by the function I2C_DualAddressCmd()) the events I2C_EVENT_SLAVE_XXX_SECONDADDRESS_MATCHED
  *   (where XXX could be TRANSMITTER or RECEIVER) are set.
  *
  * 3) In case the address sent by the master is General Call (address 0x00) and
  *   if the General Call is enabled for the peripheral (using function I2C_GeneralCallCmd())
  *   the following event is set I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED.
  *
  */

/* --EV1  (all the events below are variants of EV1) */
/* 1) Case of One Single Address managed by the slave */
#define  I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED          ((uint32_t)0x00020002) /* BUSY and ADDR flags */
#define  I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED       ((uint32_t)0x00060082) /* TRA, BUSY, TXE and ADDR flags */

/* 2) Case of Dual address managed by the slave */
#define  I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED    ((uint32_t)0x00820000)  /* DUALF and BUSY flags */
#define  I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED ((uint32_t)0x00860080)  /* DUALF, TRA, BUSY and TXE flags */

/* 3) Case of General Call enabled for the slave */
#define  I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED        ((uint32_t)0x00120000)  /* GENCALL and BUSY flags */

/**
  * 简介:  Communication events
  *
  * Wait on one of these events when EV1 has already been checked and:
  *
  * - Slave 接收器模式:
  *     - EV2: When the application is expecting a data byte to be received.
  *     - EV4: When the application is expecting the end of the communication: master
  *       sends a stop condition and data transmission is stopped.
  *
  * - Slave 发射器模式:
  *    - EV3: When a byte has been transmitted by the slave and the application is expecting
  *      the end of the byte transmission. The two events I2C_EVENT_SLAVE_BYTE_TRANSMITTED and
  *      I2C_EVENT_SLAVE_BYTE_TRANSMITTING are similar. The second one can optionally be
  *      used when the user software doesn't guarantee the EV3 is managed before the
  *      current byte end of transfer.
  *    - EV3_2: When the master sends a NACK in order to tell slave that data transmission
  *      shall end (before sending the STOP condition). In this case slave has to stop sending
  *      data bytes and expect a Stop condition on the bus.
  *
  *  @note In case the  user software does not guarantee that the event EV2 is
  *        managed before the current byte end of transfer, then user may check on EV2
  *        and BTF flag at the same time (ie. (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_BTF)).
  *        In this case the communication may be slower.
  *
  */

/* Slave 接收器模式 --------------------------*/
/* --EV2 */
#define  I2C_EVENT_SLAVE_BYTE_RECEIVED                     ((uint32_t)0x00020040)  /* BUSY and RXNE flags */
/* --EV4  */
#define  I2C_EVENT_SLAVE_STOP_DETECTED                     ((uint32_t)0x00000010)  /* STOPF flag */

/* Slave 发射器模式 -----------------------*/
/* --EV3 */
#define  I2C_EVENT_SLAVE_BYTE_TRANSMITTED                  ((uint32_t)0x00060084)  /* TRA, BUSY, TXE and BTF flags */
#define  I2C_EVENT_SLAVE_BYTE_TRANSMITTING                 ((uint32_t)0x00060080)  /* TRA, BUSY and TXE flags */
/* --EV3_2 */
#define  I2C_EVENT_SLAVE_ACK_FAILURE                       ((uint32_t)0x00000400)  /* AF flag */

/*
 ===============================================================================
                          End of Events Description
 ===============================================================================
 */

#define IS_I2C_EVENT(EVENT) (((EVENT) == I2C_EVENT_SLAVE_TRANSMITTER_ADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_RECEIVER_ADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_TRANSMITTER_SECONDADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_RECEIVER_SECONDADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_GENERALCALLADDRESS_MATCHED) || \
                             ((EVENT) == I2C_EVENT_SLAVE_BYTE_RECEIVED) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_DUALF)) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_RECEIVED | I2C_FLAG_GENCALL)) || \
                             ((EVENT) == I2C_EVENT_SLAVE_BYTE_TRANSMITTED) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_DUALF)) || \
                             ((EVENT) == (I2C_EVENT_SLAVE_BYTE_TRANSMITTED | I2C_FLAG_GENCALL)) || \
                             ((EVENT) == I2C_EVENT_SLAVE_STOP_DETECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_MODE_SELECT) || \
                             ((EVENT) == I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_RECEIVED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_TRANSMITTED) || \
                             ((EVENT) == I2C_EVENT_MASTER_BYTE_TRANSMITTING) || \
                             ((EVENT) == I2C_EVENT_MASTER_MODE_ADDRESS10) || \
                             ((EVENT) == I2C_EVENT_SLAVE_ACK_FAILURE))
/**
  * @}
  */

/** @defgroup I2C_own_address1
  * @{
  */

#define IS_I2C_OWN_ADDRESS1(ADDRESS1) ((ADDRESS1) <= 0x3FF)
/**
  * @}
  */

/** @defgroup I2C_clock_speed
  * @{
  */

#define IS_I2C_CLOCK_SPEED(SPEED) (((SPEED) >= 0x1) && ((SPEED) <= 400000))
/**
  * @}
  */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/*  Function used to set the I2C configuration to the default reset state *****/
void I2C_DeInit(I2C_TypeDef* I2Cx); // 将I2Cx外设寄存器取消初始化为其默认复位值。

/* 初始化和配置功能 *********************************/
void I2C_Init(I2C_TypeDef* I2Cx, I2C_InitTypeDef* I2C_InitStruct); // 根据I2C_InitStruct中的指定参数初始化I2Cx外设。
void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct); // 用每个I2C_InitStruct成员的默认值填充其默认值。
void I2C_Cmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C 外设。
void I2C_DigitalFilterConfig(I2C_TypeDef* I2Cx, uint16_t I2C_DigitalFilter); // 配置 I2C 外设的数字噪声滤波器。
void I2C_AnalogFilterCmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用 I2C 外设的模拟滤波器。
void I2C_GenerateSTART(I2C_TypeDef* I2Cx, FunctionalState NewState); // 生成 I2Cx 通信启动条件。
void I2C_GenerateSTOP(I2C_TypeDef* I2Cx, FunctionalState NewState); // 生成 I2Cx 通信停止条件。
void I2C_Send7bitAddress(I2C_TypeDef* I2Cx, uint8_t Address, uint8_t I2C_Direction); // 传输地址字节以选择从设备。
void I2C_AcknowledgeConfig(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C 确认功能。
void I2C_OwnAddress2Config(I2C_TypeDef* I2Cx, uint8_t Address); // 配置指定的 I2C 自己的地址2。
void I2C_DualAddressCmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C 双寻址模式。
void I2C_GeneralCallCmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C 常规调用功能。
void I2C_SoftwareResetCmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C 软件复位。
void I2C_StretchClockCmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C 时钟扩展。
void I2C_FastModeDutyCycleConfig(I2C_TypeDef* I2Cx, uint16_t I2C_DutyCycle); // 选择指定的 I2C 快速模式占空比。
void I2C_NACKPositionConfig(I2C_TypeDef* I2Cx, uint16_t I2C_NACKPosition); // 在主接收机模式下选择指定的 I2C NACK 位置。
void I2C_SMBusAlertConfig(I2C_TypeDef* I2Cx, uint16_t I2C_SMBusAlert); // 针对指定的 I2C 将 SMBusAlert 引脚驱动至高电平或低电平。
void I2C_ARPCmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C ARP。

/* 数据传输功能 ***************************************************/
void I2C_SendData(I2C_TypeDef* I2Cx, uint8_t Data); // 通过 I2Cx 外设发送一个数据字节。
uint8_t I2C_ReceiveData(I2C_TypeDef* I2Cx); // 返回 I2Cx 外设最近接收到的数据。

/* PEC 管理功能 ***************************************************/
void I2C_TransmitPEC(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C PEC 传输。
void I2C_PECPositionConfig(I2C_TypeDef* I2Cx, uint16_t I2C_PECPosition); // 选择指定的 I2C PEC 位置。
void I2C_CalculatePEC(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用传输字节的 PEC 值计算。
uint8_t I2C_GetPEC(I2C_TypeDef* I2Cx); // 返回指定 I2C 的 PEC 值。

/* DMA传输管理 functions *****************************************/
void I2C_DMACmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 启用或禁用指定的 I2C DMA 请求。
void I2C_DMALastTransferCmd(I2C_TypeDef* I2Cx, FunctionalState NewState); // 指定下一次 DMA 传输是最后一次传输。

/* Interrupts, events and标志管理功能**************************/
uint16_t I2C_ReadRegister(I2C_TypeDef* I2Cx, uint8_t I2C_Register); // 读取指定的 I2C 寄存器并返回其值。
void I2C_ITConfig(I2C_TypeDef* I2Cx, uint16_t I2C_IT, FunctionalState NewState); // 启用或禁用指定的 I2C 中断。

/*
 ==================================================== ==============================
                          I2C 状态监控功能
 ==================================================== ==============================
  此 I2C 驱动程序根据应用要求和约束提供三种不同的 I2C 状态监控方式:


     1.基本状态监控(使用I2C_CheckEvent()函数)
     -------------------------------------------------- ---------
        它将状态寄存器(SR1 和 SR2)的内容与给定事件(可以是一个或多个标志的组合)进行比较。
        如果当前状态包含给定标志，则返回 SUCCESS，如果当前状态中缺少一个或多个标志，则返回 ERROR。

             - 何时使用
             - 此功能适用于大多数应用程序以及启动活动，因为事件在产品参考手册 (RM0090) 中有完整描述。
             - 它也适用于需要定义自己的事件的用户。

          - 限制
             - 如果发生错误(即，除了受监控的标志之外还设置了错误标志)，尽管通信保持或损坏的真实状态，I2C_CheckEvent() 函数可能会返回 SUCCESS。
               在这种情况下，建议使用错误中断来监视错误事件并在中断 IRQ 处理程序中进行处理。

     笔记
         对于错误管理，建议使用以下函数:
           - I2C_ITConfig() 用于配置和启用错误中断 (I2C_IT_ERR)。
           - 发生错误中断时调用的 I2Cx_ER_IRQHandler()。
             其中 x 是外围实例 (I2C1, I2C2 ...)
           - I2C_GetFlagStatus() 或 I2C_GetITStatus() 被调用到
             I2Cx_ER_IRQHandler() 函数以确定发生了哪个错误。
           - I2C_ClearFlag() 或 I2C_ClearITPendingBit() 和/或 I2C_SoftwareResetCmd() 和/或 I2C_GenerateStop() 以清除错误标志和源并返回正确的通信状态。

     2.高级状态监控(使用函数I2C_GetLastEvent())
     -------------------------------------------------- ------------------
        使用函数 I2C_GetLastEvent() 在单个字 (uint32_t) 中返回两个状态寄存器的图像(状态寄存器 2 的值左移 16 位并连接到状态寄存器 1)。

          - 何时使用
             - 此函数适用于上述相同的应用程序，但它允许克服 I2C_GetFlagStatus() 函数的上述限制。
             - 返回值可以与此文件中已定义的事件或用户定义的自定义值进行比较。
               此功能适用于同时监控多个标志的情况。
             - 与 I2C_CheckEvent() 函数相反，此函数允许用户选择何时接受事件(当设置了所有事件标志且未设置其他标志时，或者仅在设置所需标志时，如 I2C_CheckEvent() 函数。

          - 限制
             - 用户可能需要定义自己的事件。
             - 如果用户决定仅检查常规通信标志(并忽略错误标志)，则有关错误管理的相同注释适用于此功能。


     3. 基于标志的状态监控(使用函数I2C_GetFlagStatus())
     -------------------------------------------------- ---------------------

      使用函数 I2C_GetFlagStatus() 简单地返回一个标志的状态(即 I2C_FLAG_RXNE ...)。

          - 何时使用
             - 此功能可用于特定应用程序或调试阶段。
             - 适用于只需要检查一个标志的情况(大多数 I2C 事件通过多个标志进行监控)。
          - 限制:
             - 调用此函数时，访问状态寄存器。
               当访问状态寄存器时，一些标志被清除。
               因此检查一个标志的状态，可能会清除其他标志。
             - 函数可能需要调用两次或更多次才能监控一个事件。
 */

/*
 ===============================================================================
                          1. 基本状态监控
 ===============================================================================
 */
ErrorStatus I2C_CheckEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT); // 检查最后一个 I2Cx 事件是否等于作为参数传递的事件。
/*
 ===============================================================================
                          2. 高级状态监控
 ===============================================================================
 */
uint32_t I2C_GetLastEvent(I2C_TypeDef* I2Cx); // 返回最后一个 I2Cx 事件。
/*
 ===============================================================================
                          3. 基于标志的状态监控
 ===============================================================================
 */
FlagStatus I2C_GetFlagStatus(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG); // 检查是否设置了指定的 I2C 标志。


void I2C_ClearFlag(I2C_TypeDef* I2Cx, uint32_t I2C_FLAG); // 清除 I2Cx 的挂起标志。
ITStatus I2C_GetITStatus(I2C_TypeDef* I2Cx, uint32_t I2C_IT); // 检查是否发生了指定的 I2C 中断。
void I2C_ClearITPendingBit(I2C_TypeDef* I2Cx, uint32_t I2C_IT); // 清除 I2Cx 的中断挂起位。

#ifdef __cplusplus
}
#endif

#endif /*__STM32F4xx_I2C_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
