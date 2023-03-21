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
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_deinit(uint32_t usart_periph) {
    switch(usart_periph) {
    case USART0:
        rcu_periph_reset_enable(RCU_USART0RST);
        rcu_periph_reset_disable(RCU_USART0RST);
        break;

    case USART1:
        rcu_periph_reset_enable(RCU_USART1RST);
        rcu_periph_reset_disable(RCU_USART1RST);
        break;

    case USART2:
        rcu_periph_reset_enable(RCU_USART2RST);
        rcu_periph_reset_disable(RCU_USART2RST);
        break;

    case USART5:
        rcu_periph_reset_enable(RCU_USART5RST);
        rcu_periph_reset_disable(RCU_USART5RST);
        break;

    case UART3:
        rcu_periph_reset_enable(RCU_UART3RST);
        rcu_periph_reset_disable(RCU_UART3RST);
        break;

    case UART4:
        rcu_periph_reset_enable(RCU_UART4RST);
        rcu_periph_reset_disable(RCU_UART4RST);
        break;

    case UART6:
        rcu_periph_reset_enable(RCU_UART6RST);
        rcu_periph_reset_disable(RCU_UART6RST);
        break;

    case UART7:
        rcu_periph_reset_enable(RCU_UART7RST);
        rcu_periph_reset_disable(RCU_UART7RST);
        break;

    default:
        break;
    }
}

/*!
    简介:    configure USART baud rate value
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  baudval: baud rate value
    参数[输出]:  无
    返回值:      无
*/
void usart_baudrate_set(uint32_t usart_periph, uint32_t baudval) {
    uint32_t uclk = 0U, intdiv = 0U, fradiv = 0U, udiv = 0U;

    switch(usart_periph) {
    /* get clock frequency */
    case USART0:
        uclk = rcu_clock_freq_get(CK_APB2);
        break;

    case USART5:
        uclk = rcu_clock_freq_get(CK_APB2);
        break;

    case USART1:
        uclk = rcu_clock_freq_get(CK_APB1);
        break;

    case USART2:
        uclk = rcu_clock_freq_get(CK_APB1);
        break;

    case UART3:
        uclk = rcu_clock_freq_get(CK_APB1);
        break;

    case UART4:
        uclk = rcu_clock_freq_get(CK_APB1);
        break;

    case UART6:
        uclk = rcu_clock_freq_get(CK_APB1);
        break;

    case UART7:
        uclk = rcu_clock_freq_get(CK_APB1);
        break;

    default:
        break;
    }

    if(USART_CTL0(usart_periph) & USART_CTL0_OVSMOD) {
        /* when oversampling by 8, configure the value of USART_BAUD */
        udiv = ((2U * uclk) + baudval / 2U) / baudval;
        intdiv = udiv & 0xfff0U;
        fradiv = (udiv >> 1U) & 0x7U;
        USART_BAUD(usart_periph) = ((USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv));
    } else {
        /* when oversampling by 16, configure the value of USART_BAUD */
        udiv = (uclk + baudval / 2U) / baudval;
        intdiv = udiv & 0xfff0U;
        fradiv = udiv & 0xfU;
        USART_BAUD(usart_periph) = ((USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv));
    }
}

/*!
    简介:   configure USART parity function
    参数[输入]: usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]: paritycfg: configure USART parity
               only one parameter can be selected which is shown as below:
      参数:       USART_PM_NONE: no parity
      参数:       USART_PM_EVEN: even parity
      参数:       USART_PM_ODD:  odd parity
    参数[输出]:  无
    返回值:      无
*/
void usart_parity_config(uint32_t usart_periph, uint32_t paritycfg) {
    /* clear USART_CTL0 PM,PCEN Bits */
    USART_CTL0(usart_periph) &= ~(USART_CTL0_PM | USART_CTL0_PCEN);
    /* configure USART parity mode */
    USART_CTL0(usart_periph) |= paritycfg ;
}

/*!
    简介:   configure USART word length
    参数[输入]: usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]: wlen: USART word length configure
               only one parameter can be selected which is shown as below:
      参数:       USART_WL_8BIT: 8 bits
      参数:       USART_WL_9BIT: 9 bits
    参数[输出]:  无
    返回值:      无
*/
void usart_word_length_set(uint32_t usart_periph, uint32_t wlen) {
    /* clear USART_CTL0 WL bit */
    USART_CTL0(usart_periph) &= ~USART_CTL0_WL;
    /* configure USART word length */
    USART_CTL0(usart_periph) |= wlen;
}

/*!
    简介:   configure USART stop bit length
    参数[输入]: usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]: stblen: USART stop bit configure
               only one parameter can be selected which is shown as below:
      参数:       USART_STB_1BIT:   1 bit
      参数:       USART_STB_0_5BIT: 0.5 bit(not available for UARTx(x=3,4,6,7))
      参数:       USART_STB_2BIT:   2 bits
      参数:       USART_STB_1_5BIT: 1.5 bits(not available for UARTx(x=3,4,6,7))
    参数[输出]:  无
    返回值:      无
*/
void usart_stop_bit_set(uint32_t usart_periph, uint32_t stblen) {
    /* clear USART_CTL1 STB bits */
    USART_CTL1(usart_periph) &= ~USART_CTL1_STB;
    /* configure USART stop bits */
    USART_CTL1(usart_periph) |= stblen;
}
/*!
    简介:    enable USART
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_enable(uint32_t usart_periph) {
    USART_CTL0(usart_periph) |= USART_CTL0_UEN;
}

/*!
    简介:   disable USART
    参数[输入]: usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_disable(uint32_t usart_periph) {
    USART_CTL0(usart_periph) &= ~(USART_CTL0_UEN);
}

/*!
    简介:    configure USART transmitter
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  txconfig: enable or disable USART transmitter
                only one parameter can be selected which is shown as below:
      参数:        USART_TRANSMIT_ENABLE: enable USART transmission
      参数:        USART_TRANSMIT_DISABLE: enable USART transmission
    参数[输出]:  无
    返回值:      无
*/
void usart_transmit_config(uint32_t usart_periph, uint32_t txconfig) {
    uint32_t ctl = 0U;

    ctl = USART_CTL0(usart_periph);
    ctl &= ~USART_CTL0_TEN;
    ctl |= txconfig;
    /* configure transfer mode */
    USART_CTL0(usart_periph) = ctl;
}

/*!
    简介:    configure USART receiver
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  rxconfig: enable or disable USART receiver
                only one parameter can be selected which is shown as below:
      参数:        USART_RECEIVE_ENABLE: enable USART reception
      参数:        USART_RECEIVE_DISABLE: disable USART reception
    参数[输出]:  无
    返回值:      无
*/
void usart_receive_config(uint32_t usart_periph, uint32_t rxconfig) {
    uint32_t ctl = 0U;

    ctl = USART_CTL0(usart_periph);
    ctl &= ~USART_CTL0_REN;
    ctl |= rxconfig;
    /* configure transfer mode */
    USART_CTL0(usart_periph) = ctl;
}

/*!
    简介:    data is transmitted/received with the LSB/MSB first
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  msbf: LSB/MSB
                only one parameter can be selected which is shown as below:
      参数:        USART_MSBF_LSB: LSB first
      参数:        USART_MSBF_MSB: MSB first
    参数[输出]:  无
    返回值:      无
*/
void usart_data_first_config(uint32_t usart_periph, uint32_t msbf) {
    uint32_t ctl = 0U;

    ctl = USART_CTL3(usart_periph);
    ctl &= ~(USART_CTL3_MSBF);
    ctl |= msbf;
    /* configure data transmitted/received mode */
    USART_CTL3(usart_periph) = ctl;
}

/*!
    简介:    configure USART inversion
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  invertpara: refer to enum USART_INVERT_CONFIG
                only one parameter can be selected which is shown as below:
      参数:        USART_DINV_ENABLE: data bit level inversion
      参数:        USART_DINV_DISABLE: data bit level not inversion
      参数:        USART_TXPIN_ENABLE: TX pin level inversion
      参数:        USART_TXPIN_DISABLE: TX pin level not inversion
      参数:        USART_RXPIN_ENABLE: RX pin level inversion
      参数:        USART_RXPIN_DISABLE: RX pin level not inversion
    参数[输出]:  无
    返回值:      无
*/
void usart_invert_config(uint32_t usart_periph, usart_invert_enum invertpara) {
    /* inverted or not the specified siginal */
    switch(invertpara) {
    case USART_DINV_ENABLE:
        USART_CTL3(usart_periph) |= USART_CTL3_DINV;
        break;

    case USART_TXPIN_ENABLE:
        USART_CTL3(usart_periph) |= USART_CTL3_TINV;
        break;

    case USART_RXPIN_ENABLE:
        USART_CTL3(usart_periph) |= USART_CTL3_RINV;
        break;

    case USART_DINV_DISABLE:
        USART_CTL3(usart_periph) &= ~(USART_CTL3_DINV);
        break;

    case USART_TXPIN_DISABLE:
        USART_CTL3(usart_periph) &= ~(USART_CTL3_TINV);
        break;

    case USART_RXPIN_DISABLE:
        USART_CTL3(usart_periph) &= ~(USART_CTL3_RINV);
        break;

    default:
        break;
    }
}

/*!
    简介:    configure the USART oversample mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  oversamp: oversample value
                only one parameter can be selected which is shown as below:
      参数:        USART_OVSMOD_8: 8 bits
      参数:        USART_OVSMOD_16: 16 bits
    参数[输出]:  无
    返回值:      无
*/
void usart_oversample_config(uint32_t usart_periph, uint32_t oversamp) {
    /*  clear OVSMOD bit */
    USART_CTL0(usart_periph) &= ~(USART_CTL0_OVSMOD);
    USART_CTL0(usart_periph) |= oversamp;
}

/*!
    简介:    configure sample bit method
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  obsm: sample bit
                only one parameter can be selected which is shown as below:
      参数:        USART_OSB_1bit: 1 bit
      参数:        USART_OSB_3bit: 3 bits
    参数[输出]:  无
    返回值:      无
*/
void usart_sample_bit_config(uint32_t usart_periph, uint32_t obsm) {
    USART_CTL2(usart_periph) &= ~(USART_CTL2_OSB);
    USART_CTL2(usart_periph) |= obsm;
}

/*!
    简介:    enable receiver timeout of USART
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void usart_receiver_timeout_enable(uint32_t usart_periph) {
    USART_CTL3(usart_periph) |= USART_CTL3_RTEN;
}

/*!
    简介:    disable receiver timeout of USART
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void usart_receiver_timeout_disable(uint32_t usart_periph) {
    USART_CTL3(usart_periph) &= ~(USART_CTL3_RTEN);
}

/*!
    简介:    set the receiver timeout threshold of USART
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  rtimeout: 0-0x00FFFFFF
    参数[输出]:  无
    返回值:      无
*/
void usart_receiver_timeout_threshold_config(uint32_t usart_periph, uint32_t rtimeout) {
    USART_RT(usart_periph) &= ~(USART_RT_RT);
    USART_RT(usart_periph) |= rtimeout;
}

/*!
    简介:    USART transmit data function
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  data: data of transmission
    参数[输出]:  无
    返回值:      无
*/
void usart_data_transmit(uint32_t usart_periph, uint32_t data) {
    USART_DATA(usart_periph) = ((uint16_t)USART_DATA_DATA & data);
}

/*!
    简介:    USART receive data function
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:     data of received
*/
uint16_t usart_data_receive(uint32_t usart_periph) {
    return (uint16_t)(GET_BITS(USART_DATA(usart_periph), 0U, 8U));
}

/*!
    简介:    configure the address of the USART in wake up by address match mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  addr: address of USART/UART
    参数[输出]:  无
    返回值:      无
*/
void usart_address_config(uint32_t usart_periph, uint8_t addr) {
    USART_CTL1(usart_periph) &= ~(USART_CTL1_ADDR);
    USART_CTL1(usart_periph) |= (USART_CTL1_ADDR & addr);
}

/*!
    简介:    enable mute mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_mute_mode_enable(uint32_t usart_periph) {
    USART_CTL0(usart_periph) |= USART_CTL0_RWU;
}

/*!
    简介:    disable mute mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_mute_mode_disable(uint32_t usart_periph) {
    USART_CTL0(usart_periph) &= ~(USART_CTL0_RWU);
}

/*!
    简介:    configure wakeup method in mute mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  wmehtod: two method be used to enter or exit the mute mode
                only one parameter can be selected which is shown as below:
      参数:        USART_WM_IDLE: idle line
      参数:        USART_WM_ADDR: address mask
    参数[输出]:  无
    返回值:      无
*/
void usart_mute_mode_wakeup_config(uint32_t usart_periph, uint32_t wmehtod) {
    USART_CTL0(usart_periph) &= ~(USART_CTL0_WM);
    USART_CTL0(usart_periph) |= wmehtod;
}

/*!
    简介:    enable LIN mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_lin_mode_enable(uint32_t usart_periph) {
    USART_CTL1(usart_periph) |= USART_CTL1_LMEN;
}

/*!
    简介:    disable LIN mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_lin_mode_disable(uint32_t usart_periph) {
    USART_CTL1(usart_periph) &= ~(USART_CTL1_LMEN);
}

/*!
    简介:    configure lin break frame length
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  lblen: lin break frame length
                only one parameter can be selected which is shown as below:
      参数:        USART_LBLEN_10B: 10 bits
      参数:        USART_LBLEN_11B: 11 bits
    参数[输出]:  无
    返回值:      无
*/
void usart_lin_break_detection_length_config(uint32_t usart_periph, uint32_t lblen) {
    USART_CTL1(usart_periph) &= ~(USART_CTL1_LBLEN);
    USART_CTL1(usart_periph) |= (USART_CTL1_LBLEN & lblen);
}

/*!
    简介:    send break frame
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_send_break(uint32_t usart_periph) {
    USART_CTL0(usart_periph) |= USART_CTL0_SBKCMD;
}

/*!
    简介:    enable half duplex mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_halfduplex_enable(uint32_t usart_periph) {
    USART_CTL2(usart_periph) |= USART_CTL2_HDEN;
}

/*!
    简介:    disable half duplex mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_halfduplex_disable(uint32_t usart_periph) {
    USART_CTL2(usart_periph) &= ~(USART_CTL2_HDEN);
}

/*!
    简介:    enable CK pin in synchronous mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void usart_synchronous_clock_enable(uint32_t usart_periph) {
    USART_CTL1(usart_periph) |= USART_CTL1_CKEN;
}

/*!
    简介:    disable CK pin in synchronous mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void usart_synchronous_clock_disable(uint32_t usart_periph) {
    USART_CTL1(usart_periph) &= ~(USART_CTL1_CKEN);
}

/*!
    简介:    configure USART synchronous mode parameters
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
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
void usart_synchronous_clock_config(uint32_t usart_periph, uint32_t clen, uint32_t cph, uint32_t cpl) {
    uint32_t ctl = 0U;

    /* read USART_CTL1 register */
    ctl = USART_CTL1(usart_periph);
    ctl &= ~(USART_CTL1_CLEN | USART_CTL1_CPH | USART_CTL1_CPL);
    /* set CK length, CK phase, CK polarity */
    ctl |= (USART_CTL1_CLEN & clen) | (USART_CTL1_CPH & cph) | (USART_CTL1_CPL & cpl);

    USART_CTL1(usart_periph) = ctl;
}

/*!
    简介:    configure guard time value in smartcard mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  guat: guard time value, 0-0xFF
    参数[输出]:  无
    返回值:      无
*/
void usart_guard_time_config(uint32_t usart_periph, uint32_t guat) {
    USART_GP(usart_periph) &= ~(USART_GP_GUAT);
    USART_GP(usart_periph) |= (USART_GP_GUAT & ((guat) << GP_GUAT_OFFSET));
}

/*!
    简介:    enable smartcard mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void usart_smartcard_mode_enable(uint32_t usart_periph) {
    USART_CTL2(usart_periph) |= USART_CTL2_SCEN;
}

/*!
    简介:    disable smartcard mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void usart_smartcard_mode_disable(uint32_t usart_periph) {
    USART_CTL2(usart_periph) &= ~(USART_CTL2_SCEN);
}

/*!
    简介:    enable NACK in smartcard mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void usart_smartcard_mode_nack_enable(uint32_t usart_periph) {
    USART_CTL2(usart_periph) |= USART_CTL2_NKEN;
}

/*!
    简介:    disable NACK in smartcard mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输出]:  无
    返回值:      无
*/
void usart_smartcard_mode_nack_disable(uint32_t usart_periph) {
    USART_CTL2(usart_periph) &= ~(USART_CTL2_NKEN);
}

/*!
    简介:    configure smartcard auto-retry number
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  scrtnum: smartcard auto-retry number
    参数[输出]:  无
    返回值:      无
*/
void usart_smartcard_autoretry_config(uint32_t usart_periph, uint32_t scrtnum) {
    USART_CTL3(usart_periph) &= ~(USART_CTL3_SCRTNUM);
    USART_CTL3(usart_periph) |= (USART_CTL3_SCRTNUM & ((scrtnum) << CTL3_SCRTNUM_OFFSET));
}

/*!
    简介:    configure block length in Smartcard T=1 reception
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  bl: block length
    参数[输出]:  无
    返回值:      无
*/
void usart_block_length_config(uint32_t usart_periph, uint32_t bl) {
    USART_RT(usart_periph) &= ~(USART_RT_BL);
    USART_RT(usart_periph) |= (USART_RT_BL & ((bl) << RT_BL_OFFSET));
}

/*!
    简介:    enable IrDA mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_irda_mode_enable(uint32_t usart_periph) {
    USART_CTL2(usart_periph) |= USART_CTL2_IREN;
}

/*!
    简介:    disable IrDA mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输出]:  无
    返回值:      无
*/
void usart_irda_mode_disable(uint32_t usart_periph) {
    USART_CTL2(usart_periph) &= ~(USART_CTL2_IREN);
}

/*!
    简介:    configure the peripheral clock prescaler in USART IrDA low-power mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  psc: 0-0xFF
    参数[输出]:  无
    返回值:      无
*/
void usart_prescaler_config(uint32_t usart_periph, uint8_t psc) {
    USART_GP(usart_periph) &= ~(USART_GP_PSC);
    USART_GP(usart_periph) |= psc;
}

/*!
    简介:    configure IrDA low-power
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  irlp: IrDA low-power or normal
                only one parameter can be selected which is shown as below:
      参数:        USART_IRLP_LOW: low-power
      参数:        USART_IRLP_NORMAL: normal
    参数[输出]:  无
    返回值:      无
*/
void usart_irda_lowpower_config(uint32_t usart_periph, uint32_t irlp) {
    USART_CTL2(usart_periph) &= ~(USART_CTL2_IRLP);
    USART_CTL2(usart_periph) |= (USART_CTL2_IRLP & irlp);
}

/*!
    简介:    configure hardware flow control RTS
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  rtsconfig: enable or disable RTS
                only one parameter can be selected which is shown as below:
      参数:        USART_RTS_ENABLE:  enable RTS
      参数:        USART_RTS_DISABLE: disable RTS
    参数[输出]:  无
    返回值:      无
*/
void usart_hardware_flow_rts_config(uint32_t usart_periph, uint32_t rtsconfig) {
    uint32_t ctl = 0U;

    ctl = USART_CTL2(usart_periph);
    ctl &= ~USART_CTL2_RTSEN;
    ctl |= rtsconfig;
    /* configure RTS */
    USART_CTL2(usart_periph) = ctl;
}

/*!
    简介:    configure hardware flow control CTS
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  ctsconfig: enable or disable CTS
                only one parameter can be selected which is shown as below:
      参数:        USART_CTS_ENABLE:  enable CTS
      参数:        USART_CTS_DISABLE: disable CTS
    参数[输出]:  无
    返回值:      无
*/
void usart_hardware_flow_cts_config(uint32_t usart_periph, uint32_t ctsconfig) {
    uint32_t ctl = 0U;

    ctl = USART_CTL2(usart_periph);
    ctl &= ~USART_CTL2_CTSEN;
    ctl |= ctsconfig;
    /* configure CTS */
    USART_CTL2(usart_periph) = ctl;
}

/*!
    简介:    configure break frame coherence mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  bcm:
                only one parameter can be selected which is shown as below:
      参数:        USART_BCM_NONE: no parity error is detected
      参数:        USART_BCM_EN:   parity error is detected
    参数[输出]:  无
    返回值:      无
*/
void usart_break_frame_coherence_config(uint32_t usart_periph, uint32_t bcm) {
    USART_CHC(usart_periph) &= ~(USART_CHC_BCM);
    USART_CHC(usart_periph) |= (USART_CHC_BCM & bcm);
}

/*!
    简介:    configure parity check coherence mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  pcm:
                only one parameter can be selected which is shown as below:
      参数:        USART_PCM_NONE: not check parity
      参数:        USART_PCM_EN:   check the parity
    参数[输出]:  无
    返回值:      无
*/
void usart_parity_check_coherence_config(uint32_t usart_periph, uint32_t pcm) {
    USART_CHC(usart_periph) &= ~(USART_CHC_PCM);
    USART_CHC(usart_periph) |= (USART_CHC_PCM & pcm);
}

/*!
    简介:    configure hardware flow control coherence mode
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)
    参数[输入]:  hcm:
                only one parameter can be selected which is shown as below:
      参数:        USART_HCM_NONE: nRTS signal equals to the rxne status register
      参数:        USART_HCM_EN:   nRTS signal is set when the last data bit has been sampled
    参数[输出]:  无
    返回值:      无
*/
void usart_hardware_flow_coherence_config(uint32_t usart_periph, uint32_t hcm) {
    USART_CHC(usart_periph) &= ~(USART_CHC_HCM);
    USART_CHC(usart_periph) |= (USART_CHC_HCM & hcm);
}

/*!
    简介:    configure USART DMA reception
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  dmacmd: enable or disable DMA for reception
                only one parameter can be selected which is shown as below:
      参数:        USART_DENR_ENABLE:  DMA enable for reception
      参数:        USART_DENR_DISABLE: DMA disable for reception
    参数[输出]:  无
    返回值:      无
*/
void usart_dma_receive_config(uint32_t usart_periph, uint32_t dmacmd) {
    uint32_t ctl = 0U;

    ctl = USART_CTL2(usart_periph);
    ctl &= ~USART_CTL2_DENR;
    ctl |= dmacmd;
    /* configure DMA reception */
    USART_CTL2(usart_periph) = ctl;
}

/*!
    简介:    configure USART DMA transmission
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  dmacmd: enable or disable DMA for transmission
                only one parameter can be selected which is shown as below:
      参数:        USART_DENT_ENABLE:  DMA enable for transmission
      参数:        USART_DENT_DISABLE: DMA disable for transmission
    参数[输出]:  无
    返回值:      无
*/
void usart_dma_transmit_config(uint32_t usart_periph, uint32_t dmacmd) {
    uint32_t ctl = 0U;

    ctl = USART_CTL2(usart_periph);
    ctl &= ~USART_CTL2_DENT;
    ctl |= dmacmd;
    /* configure DMA transmission */
    USART_CTL2(usart_periph) = ctl;
}

/*!
    简介:    get flag in STAT0/STAT1/CHC register
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  flag: USART flags, refer to usart_flag_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_FLAG_CTS: CTS change flag
      参数:        USART_FLAG_LBD: LIN break detected flag
      参数:        USART_FLAG_TBE: transmit data buffer empty
      参数:        USART_FLAG_TC: transmission complete
      参数:        USART_FLAG_RBNE: read data buffer not empty
      参数:        USART_FLAG_IDLE: IDLE frame detected flag
      参数:        USART_FLAG_ORERR: overrun error
      参数:        USART_FLAG_NERR: noise error flag
      参数:        USART_FLAG_FERR: frame error flag
      参数:        USART_FLAG_PERR: parity error flag
      参数:        USART_FLAG_BSY: busy flag
      参数:        USART_FLAG_EB: end of block flag
      参数:        USART_FLAG_RT: receiver timeout flag
      参数:        USART_FLAG_EPERR: early parity error flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus usart_flag_get(uint32_t usart_periph, usart_flag_enum flag) {
    if(RESET != (USART_REG_VAL(usart_periph, flag) & BIT(USART_BIT_POS(flag)))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear flag in STAT0/STAT1/CHC register
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  flag: USART flags, refer to usart_flag_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_FLAG_CTS: CTS change flag
      参数:        USART_FLAG_LBD: LIN break detected flag
      参数:        USART_FLAG_TC: transmission complete
      参数:        USART_FLAG_RBNE: read data buffer not empty
      参数:        USART_FLAG_EB: end of block flag
      参数:        USART_FLAG_RT: receiver timeout flag
      参数:        USART_FLAG_EPERR: early parity error flag
    参数[输出]:  无
    返回值:      无
*/
void usart_flag_clear(uint32_t usart_periph, usart_flag_enum flag) {
    USART_REG_VAL(usart_periph, flag) &= ~BIT(USART_BIT_POS(flag));
}

/*!
    简介:    enable USART interrupt
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  interrupt: USART interrupts, refer to usart_interrupt_enum
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
void usart_interrupt_enable(uint32_t usart_periph, usart_interrupt_enum interrupt) {
    USART_REG_VAL(usart_periph, interrupt) |= BIT(USART_BIT_POS(interrupt));
}

/*!
    简介:    disable USART interrupt
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  interrupt: USART interrupts, refer to usart_interrupt_enum
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
void usart_interrupt_disable(uint32_t usart_periph, usart_interrupt_enum interrupt) {
    USART_REG_VAL(usart_periph, interrupt) &= ~BIT(USART_BIT_POS(interrupt));
}

/*!
    简介:    get USART interrupt and flag status
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  int_flag: USART interrupt flags, refer to usart_interrupt_flag_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_INT_FLAG_PERR: parity error interrupt and flag
      参数:        USART_INT_FLAG_TBE: transmitter buffer empty interrupt and flag
      参数:        USART_INT_FLAG_TC: transmission complete interrupt and flag
      参数:        USART_INT_FLAG_RBNE: read data buffer not empty interrupt and flag
      参数:        USART_INT_FLAG_RBNE_ORERR: read data buffer not empty interrupt and overrun error flag
      参数:        USART_INT_FLAG_IDLE: IDLE line detected interrupt and flag
      参数:        USART_INT_FLAG_LBD: LIN break detected interrupt and flag
      参数:        USART_INT_FLAG_CTS: CTS interrupt and flag
      参数:        USART_INT_FLAG_ERR_ORERR: error interrupt and overrun error
      参数:        USART_INT_FLAG_ERR_NERR: error interrupt and noise error flag
      参数:        USART_INT_FLAG_ERR_FERR: error interrupt and frame error flag
      参数:        USART_INT_FLAG_EB: interrupt enable bit of end of block event and flag
      参数:        USART_INT_FLAG_RT: interrupt enable bit of receive timeout event and flag
    参数[输出]:  无
    返回值:     FlagStatus: SET or RESET
*/
FlagStatus usart_interrupt_flag_get(uint32_t usart_periph, usart_interrupt_flag_enum int_flag) {
    uint32_t intenable = 0U, flagstatus = 0U;
    /* get the interrupt enable bit status */
    intenable = (USART_REG_VAL(usart_periph, int_flag) & BIT(USART_BIT_POS(int_flag)));
    /* get the corresponding flag bit status */
    flagstatus = (USART_REG_VAL2(usart_periph, int_flag) & BIT(USART_BIT_POS2(int_flag)));

    if((0U != flagstatus) && (0U != intenable)) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:    clear USART interrupt flag in STAT0/STAT1 register
    参数[输入]:  usart_periph: USARTx(x=0,1,2,5)/UARTx(x=3,4,6,7)
    参数[输入]:  int_flag: USART interrupt flags, refer to usart_interrupt_flag_enum
                only one parameter can be selected which is shown as below:
      参数:        USART_INT_FLAG_CTS: CTS change flag
      参数:        USART_INT_FLAG_LBD: LIN break detected flag
      参数:        USART_INT_FLAG_TC: transmission complete
      参数:        USART_INT_FLAG_RBNE: read data buffer not empty
      参数:        USART_INT_FLAG_EB: end of block flag
      参数:        USART_INT_FLAG_RT: receiver timeout flag
    参数[输出]:  无
    返回值:      无
*/
void usart_interrupt_flag_clear(uint32_t usart_periph, usart_interrupt_flag_enum int_flag) {
    USART_REG_VAL2(usart_periph, int_flag) &= ~BIT(USART_BIT_POS2(int_flag));
}
