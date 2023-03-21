/*!
    文件:    gd32f4xx_i2c.c
    简介:   I2C driver

    版本: 2016-08-15, V1.0.0, firmware for GD32F4xx
    版本: 2018-12-12, V2.0.0, firmware for GD32F4xx
    版本: 2019-04-16, V2.0.2, firmware for GD32F4xx
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

#include "gd32f4xx_i2c.h"

/* I2C register bit mask */
#define I2CCLK_MAX                    ((uint32_t)0x0000003CU)             /*!< i2cclk maximum value */
#define I2CCLK_MIN                    ((uint32_t)0x00000002U)             /*!< i2cclk minimum value */
#define I2C_FLAG_MASK                 ((uint32_t)0x0000FFFFU)             /*!< i2c flag mask */
#define I2C_ADDRESS_MASK              ((uint32_t)0x000003FFU)             /*!< i2c address mask */
#define I2C_ADDRESS2_MASK             ((uint32_t)0x000000FEU)             /*!< the second i2c address mask */

/* I2C register bit offset */
#define STAT1_PECV_OFFSET             ((uint32_t)0x00000008U)             /* bit offset of PECV in I2C_STAT1 */

/*!
    简介:      reset I2C
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_deinit(uint32_t i2c_periph) {
    switch(i2c_periph) {
    case I2C0:
        /* reset I2C0 */
        rcu_periph_reset_enable(RCU_I2C0RST);
        rcu_periph_reset_disable(RCU_I2C0RST);
        break;

    case I2C1:
        /* reset I2C1 */
        rcu_periph_reset_enable(RCU_I2C1RST);
        rcu_periph_reset_disable(RCU_I2C1RST);
        break;

    case I2C2:
        /* reset I2C2 */
        rcu_periph_reset_enable(RCU_I2C2RST);
        rcu_periph_reset_disable(RCU_I2C2RST);
        break;

    default:
        break;
    }
}

/*!
    简介:      configure I2C clock
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  clkspeed: I2C clock speed, supports standard mode (up to 100 kHz), fast mode (up to 400 kHz)
    参数[输入]:  dutycyc: duty cycle in fast mode
                only one parameter can be selected which is shown as below:
      参数:        I2C_DTCY_2: T_low/T_high = 2 in fast mode
      参数:        I2C_DTCY_16_9: T_low/T_high = 16/9 in fast mode
    参数[输出]:  无
    返回值:      无
*/
void i2c_clock_config(uint32_t i2c_periph, uint32_t clkspeed, uint32_t dutycyc) {
    uint32_t pclk1, clkc, freq, risetime;
    uint32_t temp;

    pclk1 = rcu_clock_freq_get(CK_APB1);
    /* I2C peripheral clock frequency */
    freq = (uint32_t)(pclk1 / 1000000U);

    if(freq >= I2CCLK_MAX) {
        freq = I2CCLK_MAX;
    }

    temp = I2C_CTL1(i2c_periph);
    temp &= ~I2C_CTL1_I2CCLK;
    temp |= freq;

    I2C_CTL1(i2c_periph) = temp;

    if(100000U >= clkspeed) {
        /* the maximum SCL rise time is 1000ns in standard mode */
        risetime = (uint32_t)((pclk1 / 1000000U) + 1U);

        if(risetime >= I2CCLK_MAX) {
            I2C_RT(i2c_periph) = I2CCLK_MAX;
        } else if(risetime <= I2CCLK_MIN) {
            I2C_RT(i2c_periph) = I2CCLK_MIN;
        } else {
            I2C_RT(i2c_periph) = risetime;
        }

        clkc = (uint32_t)(pclk1 / (clkspeed * 2U));

        if(clkc < 0x04U) {
            /* the CLKC in standard mode minmum value is 4 */
            clkc = 0x04U;
        }

        I2C_CKCFG(i2c_periph) |= (I2C_CKCFG_CLKC & clkc);

    } else if(400000U >= clkspeed) {
        /* the maximum SCL rise time is 300ns in fast mode */
        I2C_RT(i2c_periph) = (uint32_t)(((freq * (uint32_t)300U) / (uint32_t)1000U) + (uint32_t)1U);

        if(I2C_DTCY_2 == dutycyc) {
            /* I2C duty cycle is 2 */
            clkc = (uint32_t)(pclk1 / (clkspeed * 3U));
            I2C_CKCFG(i2c_periph) &= ~I2C_CKCFG_DTCY;
        } else {
            /* I2C duty cycle is 16/9 */
            clkc = (uint32_t)(pclk1 / (clkspeed * 25U));
            I2C_CKCFG(i2c_periph) |= I2C_CKCFG_DTCY;
        }

        if(0U == (clkc & I2C_CKCFG_CLKC)) {
            /* the CLKC in fast mode minmum value is 1 */
            clkc |= 0x0001U;
        }

        I2C_CKCFG(i2c_periph) |= I2C_CKCFG_FAST;
        I2C_CKCFG(i2c_periph) |= clkc;
    } else {
    }
}

/*!
    简介:      configure I2C address
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  mode:
                only one parameter can be selected which is shown as below:
      参数:        I2C_I2CMODE_ENABLE: I2C mode
      参数:        I2C_SMBUSMODE_ENABLE: SMBus mode
    参数[输入]:  addformat: 7bits or 10bits
                only one parameter can be selected which is shown as below:
      参数:        I2C_ADDFORMAT_7BITS: address format is 7 bits
      参数:        I2C_ADDFORMAT_10BITS: address format is 10 bits
    参数[输入]:  addr: I2C address
    参数[输出]:  无
    返回值:      无
*/
void i2c_mode_addr_config(uint32_t i2c_periph, uint32_t mode, uint32_t addformat, uint32_t addr) {
    /* SMBus/I2C mode selected */
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_SMBEN);
    ctl |= mode;
    I2C_CTL0(i2c_periph) = ctl;
    /* configure address */
    addr = addr & I2C_ADDRESS_MASK;
    I2C_SADDR0(i2c_periph) = (addformat | addr);
}

/*!
    简介:      select SMBus type
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  type:
                only one parameter can be selected which is shown as below:
      参数:        I2C_SMBUS_DEVICE: SMBus mode device type
      参数:        I2C_SMBUS_HOST: SMBus mode host type
    参数[输出]:  无
    返回值:      无
*/
void i2c_smbus_type_config(uint32_t i2c_periph, uint32_t type) {
    if(I2C_SMBUS_HOST == type) {
        I2C_CTL0(i2c_periph) |= I2C_CTL0_SMBSEL;
    } else {
        I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_SMBSEL);
    }
}

/*!
    简介:      whether or not to send an ACK
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  ack:
                only one parameter can be selected which is shown as below:
      参数:        I2C_ACK_ENABLE: ACK will be sent
      参数:        I2C_ACK_DISABLE: ACK will not be sent
    参数[输出]:  无
    返回值:      无
*/
void i2c_ack_config(uint32_t i2c_periph, uint32_t ack) {
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_ACKEN);
    ctl |= ack;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      configure I2C POAP position
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  pos:
                only one parameter can be selected which is shown as below:
      参数:        I2C_ACKPOS_CURRENT: ACKEN bit decides whether or not to send ACK or not for the current byte
      参数:        I2C_ACKPOS_NEXT: ACKEN bit decides whether or not to send ACK for the next byte
    参数[输出]:  无
    返回值:      无
*/
void i2c_ackpos_config(uint32_t i2c_periph, uint32_t pos) {
    uint32_t ctl = 0U;
    /* configure I2C POAP position */
    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_POAP);
    ctl |= pos;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      master sends slave address
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  addr: slave address
    参数[输入]:  trandirection: transmitter or receiver
                only one parameter can be selected which is shown as below:
      参数:        I2C_TRANSMITTER: transmitter
      参数:        I2C_RECEIVER: receiver
    参数[输出]:  无
    返回值:      无
*/
void i2c_master_addressing(uint32_t i2c_periph, uint32_t addr, uint32_t trandirection) {
    /* master is a transmitter or a receiver */
    if(I2C_TRANSMITTER == trandirection) {
        addr = addr & I2C_TRANSMITTER;
    } else {
        addr = addr | I2C_RECEIVER;
    }

    /* send slave address */
    I2C_DATA(i2c_periph) = addr;
}

/*!
    简介:      enable dual-address mode
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  addr: the second address in dual-address mode
    参数[输出]:  无
    返回值:      无
*/
void i2c_dualaddr_enable(uint32_t i2c_periph, uint32_t addr) {
    /* configure address */
    addr = addr & I2C_ADDRESS2_MASK;
    I2C_SADDR1(i2c_periph) = (I2C_SADDR1_DUADEN | addr);
}

/*!
    简介:      disable dual-address mode
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_dualaddr_disable(uint32_t i2c_periph) {
    I2C_SADDR1(i2c_periph) &= ~(I2C_SADDR1_DUADEN);
}

/*!
    简介:      enable I2C
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_enable(uint32_t i2c_periph) {
    I2C_CTL0(i2c_periph) |= I2C_CTL0_I2CEN;
}

/*!
    简介:      disable I2C
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_disable(uint32_t i2c_periph) {
    I2C_CTL0(i2c_periph) &= ~(I2C_CTL0_I2CEN);
}

/*!
    简介:      generate a START condition on I2C bus
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_start_on_bus(uint32_t i2c_periph) {
    I2C_CTL0(i2c_periph) |= I2C_CTL0_START;
}

/*!
    简介:      generate a STOP condition on I2C bus
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_stop_on_bus(uint32_t i2c_periph) {
    I2C_CTL0(i2c_periph) |= I2C_CTL0_STOP;
}

/*!
    简介:      I2C transmit data function
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  data: data of transmission
    参数[输出]:  无
    返回值:      无
*/
void i2c_data_transmit(uint32_t i2c_periph, uint8_t data) {
    I2C_DATA(i2c_periph) = DATA_TRANS(data);
}

/*!
    简介:      I2C receive data function
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:     data of received
*/
uint8_t i2c_data_receive(uint32_t i2c_periph) {
    return (uint8_t)DATA_RECV(I2C_DATA(i2c_periph));
}

/*!
    简介:      configure I2C DMA mode
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  dmastate:
                only one parameter can be selected which is shown as below:
      参数:        I2C_DMA_ON: enable DMA mode
      参数:        I2C_DMA_OFF: disable DMA mode
    参数[输出]:  无
    返回值:      无
*/
void i2c_dma_config(uint32_t i2c_periph, uint32_t dmastate) {
    /* configure I2C DMA function */
    uint32_t ctl = 0U;

    ctl = I2C_CTL1(i2c_periph);
    ctl &= ~(I2C_CTL1_DMAON);
    ctl |= dmastate;
    I2C_CTL1(i2c_periph) = ctl;
}

/*!
    简介:      configure whether next DMA EOT is DMA last transfer or not
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  dmalast:
                only one parameter can be selected which is shown as below:
      参数:        I2C_DMALST_ON: next DMA EOT is the last transfer
      参数:        I2C_DMALST_OFF: next DMA EOT is not the last transfer
    参数[输出]:  无
    返回值:      无
*/
void i2c_dma_last_transfer_config(uint32_t i2c_periph, uint32_t dmalast) {
    /* configure DMA last transfer */
    uint32_t ctl = 0U;

    ctl = I2C_CTL1(i2c_periph);
    ctl &= ~(I2C_CTL1_DMALST);
    ctl |= dmalast;
    I2C_CTL1(i2c_periph) = ctl;
}

/*!
    简介:      whether to stretch SCL low when data is not ready in slave mode
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  stretchpara:
                only one parameter can be selected which is shown as below:
      参数:        I2C_SCLSTRETCH_ENABLE: enable SCL stretching
      参数:        I2C_SCLSTRETCH_DISABLE: disable SCL stretching
    参数[输出]:  无
    返回值:      无
*/
void i2c_stretch_scl_low_config(uint32_t i2c_periph, uint32_t stretchpara) {
    /* configure I2C SCL strerching */
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_SS);
    ctl |= stretchpara;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      whether or not to response to a general call
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  gcallpara:
                only one parameter can be selected which is shown as below:
      参数:        I2C_GCEN_ENABLE: slave will response to a general call
      参数:        I2C_GCEN_DISABLE: slave will not response to a general call
    参数[输出]:  无
    返回值:      无
*/
void i2c_slave_response_to_gcall_config(uint32_t i2c_periph, uint32_t gcallpara) {
    /* configure slave response to a general call enable or disable */
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_GCEN);
    ctl |= gcallpara;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      configure software reset of I2C
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  sreset:
                only one parameter can be selected which is shown as below:
      参数:        I2C_SRESET_SET: I2C is under reset
      参数:        I2C_SRESET_RESET: I2C is not under reset
    参数[输出]:  无
    返回值:      无
*/
void i2c_software_reset_config(uint32_t i2c_periph, uint32_t sreset) {
    /* modify CTL0 and configure software reset I2C state */
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_SRESET);
    ctl |= sreset;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      configure I2C PEC calculation
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  pecstate:
                only one parameter can be selected which is shown as below:
      参数:        I2C_PEC_ENABLE: PEC calculation on
      参数:        I2C_PEC_DISABLE: PEC calculation off
    参数[输出]:  无
    返回值:      无
*/
void i2c_pec_config(uint32_t i2c_periph, uint32_t pecstate) {
    /* on/off PEC calculation */
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_PECEN);
    ctl |= pecstate;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      configure whether to transfer PEC value
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  pecpara:
                only one parameter can be selected which is shown as below:
      参数:        I2C_PECTRANS_ENABLE: transfer PEC value
      参数:        I2C_PECTRANS_DISABLE: not transfer PEC value
    参数[输出]:  无
    返回值:      无
*/
void i2c_pec_transfer_config(uint32_t i2c_periph, uint32_t pecpara) {
    /* whether to transfer PEC */
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_PECTRANS);
    ctl |= pecpara;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      get packet error checking value
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:     PEC value
*/
uint8_t i2c_pec_value_get(uint32_t i2c_periph) {
    return (uint8_t)((I2C_STAT1(i2c_periph) & I2C_STAT1_PECV) >> STAT1_PECV_OFFSET);
}

/*!
    简介:      configure I2C alert through SMBA pin
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  smbuspara:
                only one parameter can be selected which is shown as below:
      参数:        I2C_SALTSEND_ENABLE: issue alert through SMBA pin
      参数:        I2C_SALTSEND_DISABLE: not issue alert through SMBA pin
    参数[输出]:  无
    返回值:      无
*/
void i2c_smbus_alert_config(uint32_t i2c_periph, uint32_t smbuspara) {
    /* configure smubus alert through SMBA pin */
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_SALT);
    ctl |= smbuspara;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      configure I2C ARP protocol in SMBus
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  arpstate:
                only one parameter can be selected which is shown as below:
      参数:        I2C_ARP_ENABLE: enable ARP
      参数:        I2C_ARP_DISABLE: disable ARP
    参数[输出]:  无
    返回值:      无
*/
void i2c_smbus_arp_config(uint32_t i2c_periph, uint32_t arpstate) {
    /* enable or disable I2C ARP protocol*/
    uint32_t ctl = 0U;

    ctl = I2C_CTL0(i2c_periph);
    ctl &= ~(I2C_CTL0_ARPEN);
    ctl |= arpstate;
    I2C_CTL0(i2c_periph) = ctl;
}

/*!
    简介:      disable analog noise filter
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_analog_noise_filter_disable(uint32_t i2c_periph) {
    I2C_FCTL(i2c_periph) |= I2C_FCTL_AFD;
}

/*!
    简介:      enable analog noise filter
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_analog_noise_filter_enable(uint32_t i2c_periph) {
    I2C_FCTL(i2c_periph) &= ~(I2C_FCTL_AFD);
}

/*!
    简介:      configure digital noise filter
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  dfilterpara: refer to i2c_digital_filter_enum
                only one parameter can be selected which is shown as below:
      参数:        I2C_DF_DISABLE: disable digital noise filter
      参数:        I2C_DF_1PCLK: enable digital noise filter and the maximum filtered spiker's length 1 PCLK1
      参数:        I2C_DF_2PCLK: enable digital noise filter and the maximum filtered spiker's length 2 PCLK1
      参数:        I2C_DF_3PCLK: enable digital noise filter and the maximum filtered spiker's length 3 PCLK1
      参数:        I2C_DF_4PCLK: enable digital noise filter and the maximum filtered spiker's length 4 PCLK1
      参数:        I2C_DF_5PCLK: enable digital noise filter and the maximum filtered spiker's length 5 PCLK1
      参数:        I2C_DF_6PCLK: enable digital noise filter and the maximum filtered spiker's length 6 PCLK1
      参数:        I2C_DF_7PCLK: enable digital noise filter and the maximum filtered spiker's length 7 PCLK1
      参数:        I2C_DF_8PCLK: enable digital noise filter and the maximum filtered spiker's length 8 PCLK1
      参数:        I2C_DF_9PCLK: enable digital noise filter and the maximum filtered spiker's length 9 PCLK1
      参数:        I2C_DF_10PCLK: enable digital noise filter and the maximum filtered spiker's length 10 PCLK1
      参数:        I2C_DF_11CLK: enable digital noise filter and the maximum filtered spiker's length 11 PCLK1
      参数:        I2C_DF_12CLK: enable digital noise filter and the maximum filtered spiker's length 12 PCLK1
      参数:        I2C_DF_13PCLK: enable digital noise filter and the maximum filtered spiker's length 13 PCLK1
      参数:        I2C_DF_14PCLK: enable digital noise filter and the maximum filtered spiker's length 14 PCLK1
      参数:        I2C_DF_15PCLK: enable digital noise filter and the maximum filtered spiker's length 15 PCLK1
    参数[输出]:  无
    返回值:      无
*/
void i2c_digital_noise_filter_config(uint32_t i2c_periph, i2c_digital_filter_enum dfilterpara) {
    I2C_FCTL(i2c_periph) |= dfilterpara;
}

/*!
    简介:      enable SAM_V interface
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_sam_enable(uint32_t i2c_periph) {
    I2C_SAMCS(i2c_periph) |= I2C_SAMCS_SAMEN;
}

/*!
    简介:      disable SAM_V interface
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_sam_disable(uint32_t i2c_periph) {
    I2C_SAMCS(i2c_periph) &= ~(I2C_SAMCS_SAMEN);
}

/*!
    简介:      enable SAM_V interface timeout detect
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_sam_timeout_enable(uint32_t i2c_periph) {
    I2C_SAMCS(i2c_periph) |= I2C_SAMCS_STOEN;
}

/*!
    简介:      disable SAM_V interface timeout detect
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输出]:  无
    返回值:      无
*/
void i2c_sam_timeout_disable(uint32_t i2c_periph) {
    I2C_SAMCS(i2c_periph) &= ~(I2C_SAMCS_STOEN);
}

/*!
    简介:      get I2C flag status
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  flag: I2C flags, refer to i2c_flag_enum
                only one parameter can be selected which is shown as below:
      参数:        I2C_FLAG_SBSEND: start condition sent out in master mode
      参数:        I2C_FLAG_ADDSEND: address is sent in master mode or received and matches in slave mode
      参数:        I2C_FLAG_BTC: byte transmission finishes
      参数:        I2C_FLAG_ADD10SEND: header of 10-bit address is sent in master mode
      参数:        I2C_FLAG_STPDET: stop condition detected in slave mode
      参数:        I2C_FLAG_RBNE: I2C_DATA is not empty during receiving
      参数:        I2C_FLAG_TBE: I2C_DATA is empty during transmitting
      参数:        I2C_FLAG_BERR: a bus error occurs indication a unexpected start or stop condition on I2C bus
      参数:        I2C_FLAG_LOSTARB: arbitration lost in master mode
      参数:        I2C_FLAG_AERR: acknowledge error
      参数:        I2C_FLAG_OUERR: over-run or under-run situation occurs in slave mode
      参数:        I2C_FLAG_PECERR: PEC error when receiving data
      参数:        I2C_FLAG_SMBTO: timeout signal in SMBus mode
      参数:        I2C_FLAG_SMBALT: SMBus alert status
      参数:        I2C_FLAG_MASTER: a flag indicating whether I2C block is in master or slave mode
      参数:        I2C_FLAG_I2CBSY: busy flag
      参数:        I2C_FLAG_TR: whether the I2C is a transmitter or a receiver
      参数:        I2C_FLAG_RXGC: general call address (00h) received
      参数:        I2C_FLAG_DEFSMB: default address of SMBus device
      参数:        I2C_FLAG_HSTSMB: SMBus host header detected in slave mode
      参数:        I2C_FLAG_DUMOD: dual flag in slave mode indicating which address is matched in dual-address mode
      参数:        I2C_FLAG_TFF: txframe fall flag
      参数:        I2C_FLAG_TFR: txframe rise flag
      参数:        I2C_FLAG_RFF: rxframe fall flag
      参数:        I2C_FLAG_RFR: rxframe rise flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus i2c_flag_get(uint32_t i2c_periph, i2c_flag_enum flag) {
    if(RESET != (I2C_REG_VAL(i2c_periph, flag) & BIT(I2C_BIT_POS(flag)))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:      clear I2C flag status
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  flag: I2C flags, refer to i2c_flag_enum
                only one parameter can be selected which is shown as below:
      参数:       I2C_FLAG_SMBALT: SMBus alert status
      参数:       I2C_FLAG_SMBTO: timeout signal in SMBus mode
      参数:       I2C_FLAG_PECERR: PEC error when receiving data
      参数:       I2C_FLAG_OUERR: over-run or under-run situation occurs in slave mode
      参数:       I2C_FLAG_AERR: acknowledge error
      参数:       I2C_FLAG_LOSTARB: arbitration lost in master mode
      参数:       I2C_FLAG_BERR: a bus error occurs indication a unexpected start or stop condition on I2C bus
      参数:       I2C_FLAG_ADDSEND: address is sent in master mode or received and matches in slave mode
      参数:       I2C_FLAG_TFF: txframe fall flag
      参数:       I2C_FLAG_TFR: txframe rise flag
      参数:       I2C_FLAG_RFF: rxframe fall flag
      参数:       I2C_FLAG_RFR: rxframe rise flag
    参数[输出]:  无
    返回值:      无
*/
void i2c_flag_clear(uint32_t i2c_periph, i2c_flag_enum flag) {
    if(I2C_FLAG_ADDSEND == flag) {
        /* read I2C_STAT0 and then read I2C_STAT1 to clear ADDSEND */
        I2C_STAT0(i2c_periph);
        I2C_STAT1(i2c_periph);
    } else {
        I2C_REG_VAL(i2c_periph, flag) &= ~BIT(I2C_BIT_POS(flag));
    }
}

/*!
    简介:      enable I2C interrupt
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  interrupt: I2C interrupts, refer to i2c_interrupt_enum
                only one parameter can be selected which is shown as below:
      参数:        I2C_INT_ERR: error interrupt
      参数:        I2C_INT_EV: event interrupt
      参数:        I2C_INT_BUF: buffer interrupt
      参数:        I2C_INT_TFF: txframe fall interrupt
      参数:        I2C_INT_TFR: txframe rise interrupt
      参数:        I2C_INT_RFF: rxframe fall interrupt
      参数:        I2C_INT_RFR: rxframe rise interrupt
    参数[输出]:  无
    返回值:      无
*/
void i2c_interrupt_enable(uint32_t i2c_periph, i2c_interrupt_enum interrupt) {
    I2C_REG_VAL(i2c_periph, interrupt) |= BIT(I2C_BIT_POS(interrupt));
}

/*!
    简介:      disable I2C interrupt
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  interrupt: I2C interrupts, refer to i2c_interrupt_enum
                only one parameter can be selected which is shown as below:
      参数:        I2C_INT_ERR: error interrupt
      参数:        I2C_INT_EV: event interrupt
      参数:        I2C_INT_BUF: buffer interrupt
      参数:        I2C_INT_TFF: txframe fall interrupt
      参数:        I2C_INT_TFR: txframe rise interrupt
      参数:        I2C_INT_RFF: rxframe fall interrupt
      参数:        I2C_INT_RFR: rxframe rise interrupt
    参数[输出]:  无
    返回值:      无
*/
void i2c_interrupt_disable(uint32_t i2c_periph, i2c_interrupt_enum interrupt) {
    I2C_REG_VAL(i2c_periph, interrupt) &= ~BIT(I2C_BIT_POS(interrupt));
}

/*!
    简介:      get I2C interrupt flag status
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  int_flag: I2C interrupt flags, refer to i2c_interrupt_flag_enum
                only one parameter can be selected which is shown as below:
      参数:        I2C_INT_FLAG_SBSEND: start condition sent out in master mode interrupt flag
      参数:        I2C_INT_FLAG_ADDSEND: address is sent in master mode or received and matches in slave mode interrupt flag
      参数:        I2C_INT_FLAG_BTC: byte transmission finishes interrupt flag
      参数:        I2C_INT_FLAG_ADD10SEND: header of 10-bit address is sent in master mode interrupt flag
      参数:        I2C_INT_FLAG_STPDET: stop condition detected in slave mode interrupt flag
      参数:        I2C_INT_FLAG_RBNE: I2C_DATA is not Empty during receiving interrupt flag
      参数:        I2C_INT_FLAG_TBE: I2C_DATA is empty during transmitting interrupt flag
      参数:        I2C_INT_FLAG_BERR: a bus error occurs indication a unexpected start or stop condition on I2C bus interrupt flag
      参数:        I2C_INT_FLAG_LOSTARB: arbitration lost in master mode interrupt flag
      参数:        I2C_INT_FLAG_AERR: acknowledge error interrupt flag
      参数:        I2C_INT_FLAG_OUERR: over-run or under-run situation occurs in slave mode interrupt flag
      参数:        I2C_INT_FLAG_PECERR: PEC error when receiving data interrupt flag
      参数:        I2C_INT_FLAG_SMBTO: timeout signal in SMBus mode interrupt flag
      参数:        I2C_INT_FLAG_SMBALT: SMBus alert status interrupt flag
      参数:        I2C_INT_FLAG_TFF: txframe fall interrupt flag
      参数:        I2C_INT_FLAG_TFR: txframe rise interrupt flag
      参数:        I2C_INT_FLAG_RFF: rxframe fall interrupt flag
      参数:        I2C_INT_FLAG_RFR: rxframe rise interrupt flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus i2c_interrupt_flag_get(uint32_t i2c_periph, i2c_interrupt_flag_enum int_flag) {
    uint32_t intenable = 0U, flagstatus = 0U, bufie;

    /* check BUFIE */
    bufie = I2C_CTL1(i2c_periph)&I2C_CTL1_BUFIE;

    /* get the interrupt enable bit status */
    intenable = (I2C_REG_VAL(i2c_periph, int_flag) & BIT(I2C_BIT_POS(int_flag)));
    /* get the corresponding flag bit status */
    flagstatus = (I2C_REG_VAL2(i2c_periph, int_flag) & BIT(I2C_BIT_POS2(int_flag)));

    if((I2C_INT_FLAG_RBNE == int_flag) || (I2C_INT_FLAG_TBE == int_flag)) {
        if(intenable && bufie) {
            intenable = 1U;
        } else {
            intenable = 0U;
        }
    }

    if((0U != flagstatus) && (0U != intenable)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:      clear I2C interrupt flag status
    参数[输入]:  i2c_periph: I2Cx(x=0,1,2)
    参数[输入]:  int_flag: I2C interrupt flags, refer to i2c_interrupt_flag_enum
                only one parameter can be selected which is shown as below:
      参数:        I2C_INT_FLAG_ADDSEND: address is sent in master mode or received and matches in slave mode interrupt flag
      参数:        I2C_INT_FLAG_BERR: a bus error occurs indication a unexpected start or stop condition on I2C bus interrupt flag
      参数:        I2C_INT_FLAG_LOSTARB: arbitration lost in master mode interrupt flag
      参数:        I2C_INT_FLAG_AERR: acknowledge error interrupt flag
      参数:        I2C_INT_FLAG_OUERR: over-run or under-run situation occurs in slave mode interrupt flag
      参数:        I2C_INT_FLAG_PECERR: PEC error when receiving data interrupt flag
      参数:        I2C_INT_FLAG_SMBTO: timeout signal in SMBus mode interrupt flag
      参数:        I2C_INT_FLAG_SMBALT: SMBus alert status interrupt flag
      参数:        I2C_INT_FLAG_TFF: txframe fall interrupt flag
      参数:        I2C_INT_FLAG_TFR: txframe rise interrupt flag
      参数:        I2C_INT_FLAG_RFF: rxframe fall interrupt flag
      参数:        I2C_INT_FLAG_RFR: rxframe rise interrupt flag
    参数[输出]:  无
    返回值:      无
*/
void i2c_interrupt_flag_clear(uint32_t i2c_periph, i2c_interrupt_flag_enum int_flag) {
    if(I2C_INT_FLAG_ADDSEND == int_flag) {
        /* read I2C_STAT0 and then read I2C_STAT1 to clear ADDSEND */
        I2C_STAT0(i2c_periph);
        I2C_STAT1(i2c_periph);
    } else {
        I2C_REG_VAL2(i2c_periph, int_flag) &= ~BIT(I2C_BIT_POS2(int_flag));
    }
}
