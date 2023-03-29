/*!
    文件:    gd32f4xx_gpio.c
    简介:   GPIO driver

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

#include "gd32f4xx_gpio.h"

/*!
    简介:      reset GPIO port
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输出]:  无
    返回值:      无
*/
void GPIO_DeInit(uint32_t GPIO_periph) {
    switch(GPIO_periph) {
    case GPIOA:
        /* reset GPIOA */
        RCU_Periph_Reset_Enable(RCU_GPIOARST);
        RCU_Periph_Reset_Disable(RCU_GPIOARST);
        break;

    case GPIOB:
        /* reset GPIOB */
        RCU_Periph_Reset_Enable(RCU_GPIOBRST);
        RCU_Periph_Reset_Disable(RCU_GPIOBRST);
        break;

    case GPIOC:
        /* reset GPIOC */
        RCU_Periph_Reset_Enable(RCU_GPIOCRST);
        RCU_Periph_Reset_Disable(RCU_GPIOCRST);
        break;

    case GPIOD:
        /* reset GPIOD */
        RCU_Periph_Reset_Enable(RCU_GPIODRST);
        RCU_Periph_Reset_Disable(RCU_GPIODRST);
        break;

    case GPIOE:
        /* reset GPIOE */
        RCU_Periph_Reset_Enable(RCU_GPIOERST);
        RCU_Periph_Reset_Disable(RCU_GPIOERST);
        break;

    case GPIOF:
        /* reset GPIOF */
        RCU_Periph_Reset_Enable(RCU_GPIOFRST);
        RCU_Periph_Reset_Disable(RCU_GPIOFRST);
        break;

    case GPIOG:
        /* reset GPIOG */
        RCU_Periph_Reset_Enable(RCU_GPIOGRST);
        RCU_Periph_Reset_Disable(RCU_GPIOGRST);
        break;

    case GPIOH:
        /* reset GPIOH */
        RCU_Periph_Reset_Enable(RCU_GPIOHRST);
        RCU_Periph_Reset_Disable(RCU_GPIOHRST);
        break;

    case GPIOI:
        /* reset GPIOI */
        RCU_Periph_Reset_Enable(RCU_GPIOIRST);
        RCU_Periph_Reset_Disable(RCU_GPIOIRST);
        break;

    default:
        break;
    }
}

/*!
    简介:      set GPIO mode
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  mode: GPIO pin mode
      参数:        GPIO_MODE_INPUT: input mode
      参数:        GPIO_MODE_OUTPUT: output mode
      参数:        GPIO_MODE_AF: alternate function mode
      参数:        GPIO_MODE_ANALOG: analog mode
    参数[输入]:  pull_up_down: GPIO pin with pull-up or pull-down resistor
      参数:        GPIO_PUPD_NONE: floating mode, no pull-up and pull-down resistors
      参数:        GPIO_PUPD_PULLUP: with pull-up resistor
      参数:        GPIO_PUPD_PULLDOWN:with pull-down resistor
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:      无
*/
void GPIO_mode_set(uint32_t GPIO_periph, uint32_t mode, uint32_t pull_up_down, uint32_t pin) {
    uint16_t i;
    uint32_t ctl, pupd;

    ctl = GPIO_CTL(GPIO_periph);
    pupd = GPIO_PUD(GPIO_periph);

    for(i = 0U; i < 16U; i++) {
        if((1U << i) & pin) {
            /* clear the specified pin mode bits */
            ctl &= ~GPIO_MODE_MASK(i);
            /* set the specified pin mode bits */
            ctl |= GPIO_MODE_SET(i, mode);

            /* clear the specified pin pupd bits */
            pupd &= ~GPIO_PUPD_MASK(i);
            /* set the specified pin pupd bits */
            pupd |= GPIO_PUPD_SET(i, pull_up_down);
        }
    }

    GPIO_CTL(GPIO_periph) = ctl;
    GPIO_PUD(GPIO_periph) = pupd;
}

/*!
    简介:      set GPIO output type and speed
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  otype: GPIO pin output mode
      参数:        GPIO_OTYPE_PP: push pull mode
      参数:        GPIO_OTYPE_OD: open drain mode
    参数[输入]:  speed: GPIO pin output max speed
      参数:        GPIO_OSPEED_2MHZ: output max speed 2MHz
      参数:        GPIO_OSPEED_25MHZ: output max speed 25MHz
      参数:        GPIO_OSPEED_50MHZ: output max speed 50MHz
      参数:        GPIO_OSPEED_MAX: output max speed more than 50MHz
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:      无
*/
void GPIO_output_options_set(uint32_t GPIO_periph, uint8_t otype, uint32_t speed, uint32_t pin) {
    uint16_t i;
    uint32_t ospeedr;

    if(GPIO_OTYPE_OD == otype) {
        GPIO_OMODE(GPIO_periph) |= (uint32_t)pin;
    } else {
        GPIO_OMODE(GPIO_periph) &= (uint32_t)(~pin);
    }

    /* get the specified pin output speed bits value */
    ospeedr = GPIO_OSPD(GPIO_periph);

    for(i = 0U; i < 16U; i++) {
        if((1U << i) & pin) {
            /* clear the specified pin output speed bits */
            ospeedr &= ~GPIO_OSPEED_MASK(i);
            /* set the specified pin output speed bits */
            ospeedr |= GPIO_OSPEED_SET(i, speed);
        }
    }

    GPIO_OSPD(GPIO_periph) = ospeedr;
}

/*!
    简介:    set GPIO pin bit
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:      无
*/
void GPIO_bit_set(uint32_t GPIO_periph, uint32_t pin) {
    GPIO_BOP(GPIO_periph) = (uint32_t)pin;
}

/*!
    简介:      reset GPIO pin bit
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:      无
*/
void GPIO_bit_reset(uint32_t GPIO_periph, uint32_t pin) {
    GPIO_BC(GPIO_periph) = (uint32_t)pin;
}

/*!
    简介:      write data to the specified GPIO pin
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输入]:  bit_value: SET or RESET
      参数:        RESET: clear the port pin
      参数:        SET: set the port pin
    参数[输出]:  无
    返回值:      无
*/
void GPIO_bit_write(uint32_t GPIO_periph, uint32_t pin, bit_status bit_value) {
    if(RESET != bit_value) {
        GPIO_BOP(GPIO_periph) = (uint32_t)pin;
    } else {
        GPIO_BC(GPIO_periph) = (uint32_t)pin;
    }
}

/*!
    简介:      write data to the specified GPIO port
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  data: specify the value to be written to the port output control register
    参数[输出]:  无
    返回值:      无
*/
void GPIO_port_write(uint32_t GPIO_periph, uint16_t data) {
    GPIO_OCTL(GPIO_periph) = (uint32_t)data;
}

/*!
    简介:      get GPIO pin input status
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:     input status of GPIO pin: SET or RESET
*/
FlagStatus GPIO_input_bit_get(uint32_t GPIO_periph, uint32_t pin) {
    if((uint32_t)RESET != (GPIO_ISTAT(GPIO_periph) & (pin))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:      get GPIO all pins input status
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输出]:  无
    返回值:     input status of GPIO all pins
*/
uint16_t GPIO_input_port_get(uint32_t GPIO_periph) {
    return ((uint16_t)GPIO_ISTAT(GPIO_periph));
}

/*!
    简介:      get GPIO pin output status
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:     output status of GPIO pin: SET or RESET
*/
FlagStatus GPIO_output_bit_get(uint32_t GPIO_periph, uint32_t pin) {
    if((uint32_t)RESET != (GPIO_OCTL(GPIO_periph) & (pin))) {
        return SET;
    } else {
        return RESET;
    }
}

/*!
    简介:      get GPIO port output status
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输出]:  无
    返回值:     output status of GPIO all pins
*/
uint16_t GPIO_output_port_get(uint32_t GPIO_periph) {
    return ((uint16_t)GPIO_OCTL(GPIO_periph));
}

/*!
    简介:      set GPIO alternate function
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  alt_func_num: GPIO pin af function
      参数:        GPIO_AF_0: SYSTEM
      参数:        GPIO_AF_1: TIMER0, TIMER1
      参数:        GPIO_AF_2: TIMER2, TIMER3, TIMER4
      参数:        GPIO_AF_3: TIMER7, TIMER8, TIMER9, TIMER10
      参数:        GPIO_AF_4: I2C0, I2C1, I2C2
      参数:        GPIO_AF_5: SPI0, SPI1, SPI2, SPI3, SPI4, SPI5
      参数:        GPIO_AF_6: SPI2, SPI3, SPI4
      参数:        GPIO_AF_7: USART0, USART1, USART2, SPI1, SPI2
      参数:        GPIO_AF_8: UART3, UART4, USART5, UART6, UART7
      参数:        GPIO_AF_9: CAN0, CAN1, TLI, TIMER11, TIMER12, TIMER13, I2C1, I2C2, CTC
      参数:        GPIO_AF_10: USB_FS, USB_HS
      参数:        GPIO_AF_11: ENET
      参数:        GPIO_AF_12: EXMC, SDIO, USB_HS
      参数:        GPIO_AF_13: DCI
      参数:        GPIO_AF_14: TLI
      参数:        GPIO_AF_15: EVENTOUT
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:      无
*/
void GPIO_af_set(uint32_t GPIO_periph, uint32_t alt_func_num, uint32_t pin) {
    uint16_t i;
    uint32_t afrl, afrh;

    afrl = GPIO_AFSEL0(GPIO_periph);
    afrh = GPIO_AFSEL1(GPIO_periph);

    for(i = 0U; i < 8U; i++) {
        if((1U << i) & pin) {
            /* clear the specified pin alternate function bits */
            afrl &= ~GPIO_AFR_MASK(i);
            afrl |= GPIO_AFR_SET(i, alt_func_num);
        }
    }

    for(i = 8U; i < 16U; i++) {
        if((1U << i) & pin) {
            /* clear the specified pin alternate function bits */
            afrh &= ~GPIO_AFR_MASK(i - 8U);
            afrh |= GPIO_AFR_SET(i - 8U, alt_func_num);
        }
    }

    GPIO_AFSEL0(GPIO_periph) = afrl;
    GPIO_AFSEL1(GPIO_periph) = afrh;
}

/*!
    简介:      lock GPIO pin bit
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:      无
*/
void GPIO_pin_lock(uint32_t GPIO_periph, uint32_t pin) {
    uint32_t lock = 0x00010000U;
    lock |= pin;

    /* lock key writing sequence: write 1->write 0->write 1->read 0->read 1 */
    GPIO_LOCK(GPIO_periph) = (uint32_t)lock;
    GPIO_LOCK(GPIO_periph) = (uint32_t)pin;
    GPIO_LOCK(GPIO_periph) = (uint32_t)lock;
    lock = GPIO_LOCK(GPIO_periph);
    lock = GPIO_LOCK(GPIO_periph);
}

/*!
    简介:      toggle GPIO pin status
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)
    参数[输入]:  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      参数:        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    参数[输出]:  无
    返回值:      无
*/
void GPIO_bit_toggle(uint32_t GPIO_periph, uint32_t pin) {
    GPIO_TG(GPIO_periph) = (uint32_t)pin;
}

/*!
    简介:      toggle GPIO port status
    参数[输入]:  GPIO_periph: GPIO port
                only one parameter can be selected which is shown as below:
      参数:        GPIOx(x = A,B,C,D,E,F,G,H,I)

    参数[输出]:  无
    返回值:      无
*/
void GPIO_port_toggle(uint32_t GPIO_periph) {
    GPIO_TG(GPIO_periph) = 0x0000FFFFU;
}
