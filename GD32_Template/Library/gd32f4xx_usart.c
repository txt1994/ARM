/*!
    文件:    gd32f4xx_usart.c
    简介:   USART driver

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

#include "gd32f4xx_usart.h"

/* USART register bit offset */
#define GP_GUAT_OFFSET            ((uint32_t)8U)       /* bit offset of GUAT in USART_GP */
#define CTL3_SCRTNUM_OFFSET       ((uint32_t)1U)       /* bit offset of SCRTNUM in USART_CTL3 */
#define RT_BL_OFFSET              ((uint32_t)24U)      /* bit offset of BL in USART_RT */

/*!
    简介:    reset USART/UART
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_DeInit(uint32_t USART_periph) {
    switch(USART_periph) {
    case USART0:
        RCU_Periph_Reset_Enable(RCU_USART0RST);
        RCU_Periph_Reset_Disable(RCU_USART0RST);
        break;

    case USART1:
        RCU_Periph_Reset_Enable(RCU_USART1RST);
        RCU_Periph_Reset_Disable(RCU_USART1RST);
        break;

    case USART2:
        RCU_Periph_Reset_Enable(RCU_USART2RST);
        RCU_Periph_Reset_Disable(RCU_USART2RST);
        break;

    case USART5:
        RCU_Periph_Reset_Enable(RCU_USART5RST);
        RCU_Periph_Reset_Disable(RCU_USART5RST);
        break;

    case UART3:
        RCU_Periph_Reset_Enable(RCU_UART3RST);
        RCU_Periph_Reset_Disable(RCU_UART3RST);
        break;

    case UART4:
        RCU_Periph_Reset_Enable(RCU_UART4RST);
        RCU_Periph_Reset_Disable(RCU_UART4RST);
        break;

    case UART6:
        RCU_Periph_Reset_Enable(RCU_UART6RST);
        RCU_Periph_Reset_Disable(RCU_UART6RST);
        break;

    case UART7:
        RCU_Periph_Reset_Enable(RCU_UART7RST);
        RCU_Periph_Reset_Disable(RCU_UART7RST);
        break;

    default:
        break;
    }
}

/*!
    简介:    configure USART baud rate value
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  baudval: baud rate value
    参数[输出]:  无
    返回值:      无
*/
void USART_Baudrate_Set(uint32_t USART_periph, uint32_t baudval) {
    uint32_t uclk = 0U, intdiv = 0U, fradiv = 0U, udiv = 0U;

    switch(USART_periph) {
    /* get clock frequency */
    case USART0:
        uclk = RCU_Clock_Freq_Get(CK_APB2);
        break;

    case USART5:
        uclk = RCU_Clock_Freq_Get(CK_APB2);
        break;

    case USART1:
        uclk = RCU_Clock_Freq_Get(CK_APB1);
        break;

    case USART2:
        uclk = RCU_Clock_Freq_Get(CK_APB1);
        break;

    case UART3:
        uclk = RCU_Clock_Freq_Get(CK_APB1);
        break;

    case UART4:
        uclk = RCU_Clock_Freq_Get(CK_APB1);
        break;

    case UART6:
        uclk = RCU_Clock_Freq_Get(CK_APB1);
        break;

    case UART7:
        uclk = RCU_Clock_Freq_Get(CK_APB1);
        break;

    default:
        break;
    }

    if(USART_CTL0(USART_periph) & USART_CTL0_OVSMOD) {
        /* when oversampling by 8, configure the value of USART_BAUD */
        udiv = ((2U * uclk) + baudval / 2U) / baudval;
        intdiv = udiv & 0xfff0U;
        fradiv = (udiv >> 1U) & 0x7U;
        USART_BAUD(USART_periph) = ((USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv));
    } else {
        /* when oversampling by 16, configure the value of USART_BAUD */
        udiv = (uclk + baudval / 2U) / baudval;
        intdiv = udiv & 0xfff0U;
        fradiv = udiv & 0xfU;
        USART_BAUD(USART_periph) = ((USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv));
    }
}

/*!
    简介:   configure USART parity function
    参数[输入]: USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]: paritycfg: configure USART parity
               only one parameter can be selected which is shown as below:
      参数:       USART_PM_NONE: no parity
      参数:       USART_PM_EVEN: even parity
      参数:       USART_PM_ODD:  odd parity
    参数[输出]:  无
    返回值:      无
*/
void USART_Parity_Config(uint32_t USART_periph, uint32_t paritycfg) {
    /* clear USART_CTL0 PM,PCEN Bits */
    USART_CTL0(USART_periph) &= ~(USART_CTL0_PM | USART_CTL0_PCEN);
    /* configure USART parity mode */
    USART_CTL0(USART_periph) |= paritycfg ;
}

/*!
    简介:   configure USART word length
    参数[输入]: USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]: wlen: USART word length configure
               only one parameter can be selected which is shown as below:
      参数:       USART_WL_8BIT: 8 bits
      参数:       USART_WL_9BIT: 9 bits
    参数[输出]:  无
    返回值:      无
*/
void USART_Word_Length_Set(uint32_t USART_periph, uint32_t wlen) {
    /* clear USART_CTL0 WL bit */
    USART_CTL0(USART_periph) &= ~USART_CTL0_WL;
    /* configure USART word length */
    USART_CTL0(USART_periph) |= wlen;
}

/*!
    简介:   configure USART stop bit length
    参数[输入]: USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]: stblen: USART stop bit configure
               only one parameter can be selected which is shown as below:
      参数:       USART_STB_1BIT:   1 bit
      参数:       USART_STB_0_5BIT: 0.5 bit(not available for UARTx(x=3,4,6,7))
      参数:       USART_STB_2BIT:   2 bits
      参数:       USART_STB_1_5BIT: 1.5 bits(not available for UARTx(x=3,4,6,7))
    参数[输出]:  无
    返回值:      无
*/
void USART_Stop_Bit_Set(uint32_t USART_periph, uint32_t stblen) {
    /* clear USART_CTL1 STB bits */
    USART_CTL1(USART_periph) &= ~USART_CTL1_STB;
    /* configure USART stop bits */
    USART_CTL1(USART_periph) |= stblen;
}
/*!
    简介:    enable USART
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_Enable(uint32_t USART_periph) {
    USART_CTL0(USART_periph) |= USART_CTL0_UEN;
}

/*!
    简介:   disable USART
    参数[输入]: USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_Disable(uint32_t USART_periph) {
    USART_CTL0(USART_periph) &= ~(USART_CTL0_UEN);
}

/*!
    简介:    configure USART transmitter
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  txconfig: enable or disable USART transmitter
                only one parameter can be selected which is shown as below:
      参数:        USART_Transmit_Enable: enable USART transmission
      参数:        USART_Transmit_Disable: enable USART transmission
    参数[输出]:  无
    返回值:      无
*/
void USART_Transmit_Config(uint32_t USART_periph, uint32_t txconfig) {
    uint32_t ctl = 0U;

    ctl = USART_CTL0(USART_periph);
    ctl &= ~USART_CTL0_TEN;
    ctl |= txconfig;
    /* configure transfer mode */
    USART_CTL0(USART_periph) = ctl;
}

/*!
    简介:    configure USART receiver
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  rxconfig: enable or disable USART receiver
                only one parameter can be selected which is shown as below:
      参数:        USART_Receive_Enable: enable USART reception
      参数:        USART_Receive_Disable: disable USART reception
    参数[输出]:  无
    返回值:      无
*/
void USART_Receive_Config(uint32_t USART_periph, uint32_t rxconfig) {
    uint32_t ctl = 0U;

    ctl = USART_CTL0(USART_periph);
    ctl &= ~USART_CTL0_REN;
    ctl |= rxconfig;
    /* configure transfer mode */
    USART_CTL0(USART_periph) = ctl;
}

/*!
    简介:    data is transmitted/received with the LSB/MSB first
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  msbf: LSB/MSB
                only one parameter can be selected which is shown as below:
      参数:        USART_MSBF_LSB: LSB first
      参数:        USART_MSBF_MSB: MSB first
    参数[输出]:  无
    返回值:      无
*/
void USART_Data_First_Config(uint32_t USART_periph, uint32_t msbf) {
    uint32_t ctl = 0U;

    ctl = USART_CTL3(USART_periph);
    ctl &= ~(USART_CTL3_MSBF);
    ctl |= msbf;
    /* configure data transmitted/received mode */
    USART_CTL3(USART_periph) = ctl;
}

/*!
    简介:    configure USART inversion
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  invertpara: refer to enum USART_Invert_Config
                only one parameter can be selected which is shown as below:
      参数:        USART_DINV_Enable: data bit level inversion
      参数:        USART_DINV_Disable: data bit level not inversion
      参数:        USART_TXPIN_Enable: TX pin level inversion
      参数:        USART_TXPIN_Disable: TX pin level not inversion
      参数:        USART_RXPIN_Enable: RX pin level inversion
      参数:        USART_RXPIN_Disable: RX pin level not inversion
    参数[输出]:  无
    返回值:      无
*/
void USART_Invert_Config(uint32_t USART_periph, USART_Invert_enum invertpara) {
    /* inverted or not the specified siginal */
    switch(invertpara) {
    case USART_DINV_Enable:
        USART_CTL3(USART_periph) |= USART_CTL3_DINV;
        break;

    case USART_TXPIN_Enable:
        USART_CTL3(USART_periph) |= USART_CTL3_TINV;
        break;

    case USART_RXPIN_Enable:
        USART_CTL3(USART_periph) |= USART_CTL3_RINV;
        break;

    case USART_DINV_Disable:
        USART_CTL3(USART_periph) &= ~(USART_CTL3_DINV);
        break;

    case USART_TXPIN_Disable:
        USART_CTL3(USART_periph) &= ~(USART_CTL3_TINV);
        break;

    case USART_RXPIN_Disable:
        USART_CTL3(USART_periph) &= ~(USART_CTL3_RINV);
        break;

    default:
        break;
    }
}

/*!
    简介:    configure the USART oversample mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  oversamp: oversample value
                only one parameter can be selected which is shown as below:
      参数:        USART_OVSMOD_8: 8 bits
      参数:        USART_OVSMOD_16: 16 bits
    参数[输出]:  无
    返回值:      无
*/
void USART_OverSample_Config(uint32_t USART_periph, uint32_t oversamp) {
    /*  clear OVSMOD bit */
    USART_CTL0(USART_periph) &= ~(USART_CTL0_OVSMOD);
    USART_CTL0(USART_periph) |= oversamp;
}

/*!
    简介:    configure sample bit method
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  obsm: sample bit
                only one parameter can be selected which is shown as below:
      参数:        USART_OSB_1bit: 1 bit
      参数:        USART_OSB_3bit: 3 bits
    参数[输出]:  无
    返回值:      无
*/
void USART_Sample_Bit_Config(uint32_t USART_periph, uint32_t obsm) {
    USART_CTL2(USART_periph) &= ~(USART_CTL2_OSB);
    USART_CTL2(USART_periph) |= obsm;
}

/*!
    简介:    enable receiver timeout of USART
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void USART_Receiver_Timeout_Enable(uint32_t USART_periph) {
    USART_CTL3(USART_periph) |= USART_CTL3_RTEN;
}

/*!
    简介:    disable receiver timeout of USART
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void USART_Receiver_Timeout_Disable(uint32_t USART_periph) {
    USART_CTL3(USART_periph) &= ~(USART_CTL3_RTEN);
}

/*!
    简介:    set the receiver timeout threshold of USART
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  rtimeout: 0-0x00FFFFFF
    参数[输出]:  无
    返回值:      无
*/
void USART_Receiver_Timeout_Threshold_Config(uint32_t USART_periph, uint32_t rtimeout) {
    USART_RT(USART_periph) &= ~(USART_RT_RT);
    USART_RT(USART_periph) |= rtimeout;
}

/*!
    简介:    USART transmit data function
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  data: data of transmission
    参数[输出]:  无
    返回值:      无
*/
void USART_Data_Transmit(uint32_t USART_periph, uint32_t data) {
    USART_DATA(USART_periph) = ((uint16_t)USART_Data_DATA & data);
}

/*!
    简介:    USART receive data function
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:     data of received
*/
uint16_t USART_Data_Receive(uint32_t USART_periph) {
    return (uint16_t)(GET_BITS(USART_DATA(USART_periph), 0U, 8U));
}

/*!
    简介:    configure the address of the USART in wake up by address match mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  addr: address of USART/UART
    参数[输出]:  无
    返回值:      无
*/
void USART_Address_Config(uint32_t USART_periph, uint8_t addr) {
    USART_CTL1(USART_periph) &= ~(USART_CTL1_ADDR);
    USART_CTL1(USART_periph) |= (USART_CTL1_ADDR & addr);
}

/*!
    简介:    enable mute mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_Mute_Mode_Enable(uint32_t USART_periph) {
    USART_CTL0(USART_periph) |= USART_CTL0_RWU;
}

/*!
    简介:    disable mute mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_Mute_Mode_Disable(uint32_t USART_periph) {
    USART_CTL0(USART_periph) &= ~(USART_CTL0_RWU);
}

/*!
    简介:    configure wakeup method in mute mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  wmehtod: two method be used to enter or exit the mute mode
                only one parameter can be selected which is shown as below:
      参数:        USART_WM_IDLE: idle line
      参数:        USART_WM_ADDR: address mask
    参数[输出]:  无
    返回值:      无
*/
void USART_Mute_Mode_WakeUp_Config(uint32_t USART_periph, uint32_t wmehtod) {
    USART_CTL0(USART_periph) &= ~(USART_CTL0_WM);
    USART_CTL0(USART_periph) |= wmehtod;
}

/*!
    简介:    enable LIN mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_Lin_Mode_Enable(uint32_t USART_periph) {
    USART_CTL1(USART_periph) |= USART_CTL1_LMEN;
}

/*!
    简介:    disable LIN mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_Lin_Mode_Disable(uint32_t USART_periph) {
    USART_CTL1(USART_periph) &= ~(USART_CTL1_LMEN);
}

/*!
    简介:    configure lin break frame length
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  lblen: lin break frame length
                only one parameter can be selected which is shown as below:
      参数:        USART_LBLEN_10B: 10 bits
      参数:        USART_LBLEN_11B: 11 bits
    参数[输出]:  无
    返回值:      无
*/
void USART_Lin_Break_Detection_Length_Config(uint32_t USART_periph, uint32_t lblen) {
    USART_CTL1(USART_periph) &= ~(USART_CTL1_LBLEN);
    USART_CTL1(USART_periph) |= (USART_CTL1_LBLEN & lblen);
}

/*!
    简介:    send break frame
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_Send_break(uint32_t USART_periph) {
    USART_CTL0(USART_periph) |= USART_CTL0_SBKCMD;
}

/*!
    简介:    enable half duplex mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_HalfDuplex_Enable(uint32_t USART_periph) {
    USART_CTL2(USART_periph) |= USART_CTL2_HDEN;
}

/*!
    简介:    disable half duplex mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_HalfDuplex_Disable(uint32_t USART_periph) {
    USART_CTL2(USART_periph) &= ~(USART_CTL2_HDEN);
}

/*!
    简介:    enable CK pin in synchronous mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void USART_Synchronous_Clock_Enable(uint32_t USART_periph) {
    USART_CTL1(USART_periph) |= USART_CTL1_CKEN;
}

/*!
    简介:    disable CK pin in synchronous mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void USART_Synchronous_Clock_Disable(uint32_t USART_periph) {
    USART_CTL1(USART_periph) &= ~(USART_CTL1_CKEN);
}

/*!
    简介:    configure USART synchronous mode parameters
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  clen: CK length
                only one parameter can be selected which is shown as below:
      参数:        USART_CLEN_NONE: there are 7 CK pulses for an 8 bit frame and 8 CK pulses for a 9 bit frame
      参数:        USART_CLEN_EN:   there are 8 CK pulses for an 8 bit frame and 9 CK pulses for a 9 bit frame
    参数[输入]:  cph: clock phase
                only one parameter can be selected which is shown as below:
      参数:        USART_CPH_1CK: first clock transition is the first data capture edge
      参数:        USART_CPH_2CK: second clock transition is the first data capture edge
    参数[输入]:  cpl: clock polarity
                only one parameter can be selected which is shown as below:
      参数:        USART_CPL_LOW:  steady low value on CK pin
      参数:        USART_CPL_HIGH: steady high value on CK pin
    参数[输出]:  无
    返回值:      无
*/
void USART_Synchronous_Clock_Config(uint32_t USART_periph, uint32_t clen, uint32_t cph, uint32_t cpl) {
    uint32_t ctl = 0U;

    /* read USART_CTL1 register */
    ctl = USART_CTL1(USART_periph);
    ctl &= ~(USART_CTL1_CLEN | USART_CTL1_CPH | USART_CTL1_CPL);
    /* set CK length, CK phase, CK polarity */
    ctl |= (USART_CTL1_CLEN & clen) | (USART_CTL1_CPH & cph) | (USART_CTL1_CPL & cpl);

    USART_CTL1(USART_periph) = ctl;
}

/*!
    简介:    configure guard time value in smartcard mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  guat: guard time value, 0-0xFF
    参数[输出]:  无
    返回值:      无
*/
void USART_Guard_Time_Config(uint32_t USART_periph, uint32_t guat) {
    USART_GP(USART_periph) &= ~(USART_GP_GUAT);
    USART_GP(USART_periph) |= (USART_GP_GUAT & ((guat) << GP_GUAT_OFFSET));
}

/*!
    简介:    enable smartcard mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void USART_SmartCard_Mode_Enable(uint32_t USART_periph) {
    USART_CTL2(USART_periph) |= USART_CTL2_SCEN;
}

/*!
    简介:    disable smartcard mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void USART_SmartCard_Mode_Disable(uint32_t USART_periph) {
    USART_CTL2(USART_periph) &= ~(USART_CTL2_SCEN);
}

/*!
    简介:    enable NACK in smartcard mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void USART_SmartCard_Mode_Nack_Enable(uint32_t USART_periph) {
    USART_CTL2(USART_periph) |= USART_CTL2_NKEN;
}

/*!
    简介:    disable NACK in smartcard mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void USART_SmartCard_Mode_Nack_Disable(uint32_t USART_periph) {
    USART_CTL2(USART_periph) &= ~(USART_CTL2_NKEN);
}

/*!
    简介:    configure smartcard auto-retry number
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  scrtnum: smartcard auto-retry number
    参数[输出]:  无
    返回值:      无
*/
void USART_SmartCard_Autoretry_Config(uint32_t USART_periph, uint32_t scrtnum) {
    USART_CTL3(USART_periph) &= ~(USART_CTL3_SCRTNUM);
    USART_CTL3(USART_periph) |= (USART_CTL3_SCRTNUM & ((scrtnum) << CTL3_SCRTNUM_OFFSET));
}

/*!
    简介:    configure block length in Smartcard T=1 reception
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  bl: block length
    参数[输出]:  无
    返回值:      无
*/
void USART_Block_Length_Config(uint32_t USART_periph, uint32_t bl) {
    USART_RT(USART_periph) &= ~(USART_RT_BL);
    USART_RT(USART_periph) |= (USART_RT_BL & ((bl) << RT_BL_OFFSET));
}

/*!
    简介:    enable IrDA mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_IRDA_Mode_Enable(uint32_t USART_periph) {
    USART_CTL2(USART_periph) |= USART_CTL2_IREN;
}

/*!
    简介:    disable IrDA mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void USART_IRDA_Mode_Disable(uint32_t USART_periph) {
    USART_CTL2(USART_periph) &= ~(USART_CTL2_IREN);
}

/*!
    简介:    configure the peripheral clock prescaler in USART IrDA low-power mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  psc: 0-0xFF
    参数[输出]:  无
    返回值:      无
*/
void USART_Prescaler_Config(uint32_t USART_periph, uint8_t psc) {
    USART_GP(USART_periph) &= ~(USART_GP_PSC);
    USART_GP(USART_periph) |= psc;
}

/*!
    简介:    configure IrDA low-power
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  irlp: IrDA low-power or normal
                only one parameter can be selected which is shown as below:
      参数:        USART_IRLP_LOW: low-power
      参数:        USART_IRLP_NORMAL: normal
    参数[输出]:  无
    返回值:      无
*/
void USART_IRDA_Lowpower_Config(uint32_t USART_periph, uint32_t irlp) {
    USART_CTL2(USART_periph) &= ~(USART_CTL2_IRLP);
    USART_CTL2(USART_periph) |= (USART_CTL2_IRLP & irlp);
}

/*!
    简介:    configure hardware flow control RTS
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  rtsconfig: enable or disable RTS
                only one parameter can be selected which is shown as below:
      参数:        USART_RTS_Enable:  enable RTS
      参数:        USART_RTS_Disable: disable RTS
    参数[输出]:  无
    返回值:      无
*/
void USART_Hardware_Flow_RTS_Config(uint32_t USART_periph, uint32_t rtsconfig) {
    uint32_t ctl = 0U;

    ctl = USART_CTL2(USART_periph);
    ctl &= ~USART_CTL2_RTSEN;
    ctl |= rtsconfig;
    /* configure RTS */
    USART_CTL2(USART_periph) = ctl;
}

/*!
    简介:    configure hardware flow control CTS
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  ctsconfig: enable or disable CTS
                only one parameter can be selected which is shown as below:
      参数:        USART_CTS_Enable:  enable CTS
      参数:        USART_CTS_Disable: disable CTS
    参数[输出]:  无
    返回值:      无
*/
void USART_Hardware_Flow_CTS_Config(uint32_t USART_periph, uint32_t ctsconfig) {
    uint32_t ctl = 0U;

    ctl = USART_CTL2(USART_periph);
    ctl &= ~USART_CTL2_CTSEN;
    ctl |= ctsconfig;
    /* configure CTS */
    USART_CTL2(USART_periph) = ctl;
}

/*!
    简介:    configure break frame coherence mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  bcm:
                only one parameter can be selected which is shown as below:
      参数:        USART_BCM_NONE: no parity error is detected
      参数:        USART_BCM_EN:   parity error is detected
    参数[输出]:  无
    返回值:      无
*/
void USART_Break_Frame_Coherence_Config(uint32_t USART_periph, uint32_t bcm) {
    USART_CHC(USART_periph) &= ~(USART_CHC_BCM);
    USART_CHC(USART_periph) |= (USART_CHC_BCM & bcm);
}

/*!
    简介:    configure parity check coherence mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  pcm:
                only one parameter can be selected which is shown as below:
      参数:        USART_PCM_NONE: not check parity
      参数:        USART_PCM_EN:   check the parity
    参数[输出]:  无
    返回值:      无
*/
void USART_Parity_Check_Coherence_Config(uint32_t USART_periph, uint32_t pcm) {
    USART_CHC(USART_periph) &= ~(USART_CHC_PCM);
    USART_CHC(USART_periph) |= (USART_CHC_PCM & pcm);
}

/*!
    简介:    configure hardware flow control coherence mode
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)
    参数[输入]:  hcm:
                only one parameter can be selected which is shown as below:
      参数:        USART_HCM_NONE: nRTS signal equals to the rxne status register
      参数:        USART_HCM_EN:   nRTS signal is set when the last data bit has been sampled
    参数[输出]:  无
    返回值:      无
*/
void USART_Hardware_Flow_Coherence_Config(uint32_t USART_periph, uint32_t hcm) {
    USART_CHC(USART_periph) &= ~(USART_CHC_HCM);
    USART_CHC(USART_periph) |= (USART_CHC_HCM & hcm);
}

/*!
    简介:    configure USART DMA reception
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  dmacmd: enable or disable DMA for reception
                only one parameter can be selected which is shown as below:
      参数:        USART_DENR_Enable:  DMA enable for reception
      参数:        USART_DENR_Disable: DMA disable for reception
    参数[输出]:  无
    返回值:      无
*/
void USART_DMA_Receive_Config(uint32_t USART_periph, uint32_t dmacmd) {
    uint32_t ctl = 0U;

    ctl = USART_CTL2(USART_periph);
    ctl &= ~USART_CTL2_DENR;
    ctl |= dmacmd;
    /* configure DMA reception */
    USART_CTL2(USART_periph) = ctl;
}

/*!
    简介:    configure USART DMA transmission
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  dmacmd: enable or disable DMA for transmission
                only one parameter can be selected which is shown as below:
      参数:        USART_DENT_Enable:  DMA enable for transmission
      参数:        USART_DENT_Disable: DMA disable for transmission
    参数[输出]:  无
    返回值:      无
*/
void USART_DMA_Transmit_Config(uint32_t USART_periph, uint32_t dmacmd) {
    uint32_t ctl = 0U;

    ctl = USART_CTL2(USART_periph);
    ctl &= ~USART_CTL2_DENT;
    ctl |= dmacmd;
    /* configure DMA transmission */
    USART_CTL2(USART_periph) = ctl;
}

/*!
    简介:    get flag in STAT0/STAT1/CHC register
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  flag: USART flags, refer to USART_Flag_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_Flag_CTS: CTS change flag
      参数:        USART_Flag_LBD: LIN break detected flag
      参数:        USART_Flag_TBE: transmit data buffer empty
      参数:        USART_Flag_TC: transmission complete
      参数:        USART_Flag_RBNE: read data buffer not empty
      参数:        USART_Flag_IDLE: IDLE frame detected flag
      参数:        USART_Flag_ORERR: overrun error
      参数:        USART_Flag_NERR: noise error flag
      参数:        USART_Flag_FERR: frame error flag
      参数:        USART_Flag_PERR: parity error flag
      参数:        USART_Flag_BSY: busy flag
      参数:        USART_Flag_EB: end of block flag
      参数:        USART_Flag_RT: receiver timeout flag
      参数:        USART_Flag_EPERR: early parity error flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus USART_Flag_Get(uint32_t USART_periph, USART_Flag_enum flag) {
    if(RESET != (USART_REG_VAL(USART_periph, flag) & BIT(USART_Bit_POS(flag)))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear flag in STAT0/STAT1/CHC register
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  flag: USART flags, refer to USART_Flag_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_Flag_CTS: CTS change flag
      参数:        USART_Flag_LBD: LIN break detected flag
      参数:        USART_Flag_TC: transmission complete
      参数:        USART_Flag_RBNE: read data buffer not empty
      参数:        USART_Flag_EB: end of block flag
      参数:        USART_Flag_RT: receiver timeout flag
      参数:        USART_Flag_EPERR: early parity error flag
    参数[输出]:  无
    返回值:      无
*/
void USART_Flag_Clear(uint32_t USART_periph, USART_Flag_enum flag) {
    USART_REG_VAL(USART_periph, flag) &= ~BIT(USART_Bit_POS(flag));
}

/*!
    简介:    enable USART interrupt
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  interrupt: USART interrupts, refer to USART_Interrupt_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_INT_PERR: parity error interrupt
      参数:        USART_INT_TBE: transmitter buffer empty interrupt
      参数:        USART_INT_TC: transmission complete interrupt
      参数:        USART_INT_RBNE: read data buffer not empty interrupt and overrun error interrupt
      参数:        USART_INT_IDLE: IDLE line detected interrupt
      参数:        USART_INT_LBD: LIN break detected interrupt
      参数:        USART_INT_ERR: error interrupt
      参数:        USART_INT_CTS: CTS interrupt
      参数:        USART_INT_RT: interrupt enable bit of receive timeout event
      参数:        USART_INT_EB: interrupt enable bit of end of block event
    参数[输出]:  无
    返回值:      无
*/
void USART_Interrupt_Enable(uint32_t USART_periph, USART_Interrupt_enum interrupt) {
    USART_REG_VAL(USART_periph, interrupt) |= BIT(USART_Bit_POS(interrupt));
}

/*!
    简介:    disable USART interrupt
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  interrupt: USART interrupts, refer to USART_Interrupt_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_INT_PERR: parity error interrupt
      参数:        USART_INT_TBE: transmitter buffer empty interrupt
      参数:        USART_INT_TC: transmission complete interrupt
      参数:        USART_INT_RBNE: read data buffer not empty interrupt and overrun error interrupt
      参数:        USART_INT_IDLE: IDLE line detected interrupt
      参数:        USART_INT_LBD: LIN break detected interrupt
      参数:        USART_INT_ERR: error interrupt
      参数:        USART_INT_CTS: CTS interrupt
      参数:        USART_INT_RT: interrupt enable bit of receive timeout event
      参数:        USART_INT_EB: interrupt enable bit of end of block event
    参数[输出]:  无
    返回值:      无
*/
void USART_Interrupt_Disable(uint32_t USART_periph, USART_Interrupt_enum interrupt) {
    USART_REG_VAL(USART_periph, interrupt) &= ~BIT(USART_Bit_POS(interrupt));
}

/*!
    简介:    get USART interrupt and flag status
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  int_flag: USART interrupt flags, refer to USART_Interrupt_Flag_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_INT_Flag_PERR: parity error interrupt and flag
      参数:        USART_INT_Flag_TBE: transmitter buffer empty interrupt and flag
      参数:        USART_INT_Flag_TC: transmission complete interrupt and flag
      参数:        USART_INT_Flag_RBNE: read data buffer not empty interrupt and flag
      参数:        USART_INT_Flag_RBNE_ORERR: read data buffer not empty interrupt and overrun error flag
      参数:        USART_INT_Flag_IDLE: IDLE line detected interrupt and flag
      参数:        USART_INT_Flag_LBD: LIN break detected interrupt and flag
      参数:        USART_INT_Flag_CTS: CTS interrupt and flag
      参数:        USART_INT_Flag_ERR_ORERR: error interrupt and overrun error
      参数:        USART_INT_Flag_ERR_NERR: error interrupt and noise error flag
      参数:        USART_INT_Flag_ERR_FERR: error interrupt and frame error flag
      参数:        USART_INT_Flag_EB: interrupt enable bit of end of block event and flag
      参数:        USART_INT_Flag_RT: interrupt enable bit of receive timeout event and flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus USART_Interrupt_Flag_Get(uint32_t USART_periph, USART_Interrupt_Flag_enum int_flag) {
    uint32_t intenable = 0U, flagstatus = 0U;
    /* get the interrupt enable bit status */
    intenable = (USART_REG_VAL(USART_periph, int_flag) & BIT(USART_Bit_POS(int_flag)));
    /* get the corresponding flag bit status */
    flagstatus = (USART_REG_VAL2(USART_periph, int_flag) & BIT(USART_Bit_POS2(int_flag)));

    if((0U != flagstatus) && (0U != intenable)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear USART interrupt flag in STAT0/STAT1 register
    参数[输入]:  USART_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  int_flag: USART interrupt flags, refer to USART_Interrupt_Flag_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_INT_Flag_CTS: CTS change flag
      参数:        USART_INT_Flag_LBD: LIN break detected flag
      参数:        USART_INT_Flag_TC: transmission complete
      参数:        USART_INT_Flag_RBNE: read data buffer not empty
      参数:        USART_INT_Flag_EB: end of block flag
      参数:        USART_INT_Flag_RT: receiver timeout flag
    参数[输出]:  无
    返回值:      无
*/
void USART_Interrupt_Flag_Clear(uint32_t USART_periph, USART_Interrupt_Flag_enum int_flag) {
    USART_REG_VAL2(USART_periph, int_flag) &= ~BIT(USART_Bit_POS2(int_flag));
}
