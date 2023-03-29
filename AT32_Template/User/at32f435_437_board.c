/**
  **************************************************************************
  * @file     at32f435_437_board.c
  * @brief    set of firmware functions to manage leds and push-button.
  *           initialize delay function.
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f435_437_board.h"

/** @addtogroup AT32F435_437_board
  * @{
  */

/** @defgroup BOARD
  * @brief onboard periph driver
  * @{
  */

/* delay macros */
#define STEP_DELAY_MS                    50

/* at-start led resouce array */
gpio_Type *led_gpio_port[LED_Num]        = {LED2_GPIO, LED3_GPIO, LED4_GPIO};
uint16_t led_gpio_pin[LED_Num]           = {LED2_PIN, LED3_PIN, LED4_PIN};
crm_periph_Clock_Type led_gpio_crm_clk[LED_Num] = {LED2_GPIO_CRM_CLK, LED3_GPIO_CRM_CLK, LED4_GPIO_CRM_CLK};

/* delay variable */
static __IO uint32_t fac_us;
static __IO uint32_t fac_ms;

/* support printf function, usemicrolib is unnecessary */
#if (__ARMCC_VERSION > 6000000)
__asm (".global __use_no_semihosting\n\t");
void _sys_Exit(int x) {
    x = x;
}
/* __use_no_semihosting was requested, but _ttywrch was */
void _ttywrch(int ch) {
    ch = ch;
}
FILE __stdout;
#else
#ifdef __CC_ARM
#pragma import(__use_no_semihosting)
struct __FILE {
    int handle;
};
FILE __stdout;
void _sys_Exit(int x) {
    x = x;
}
/* __use_no_semihosting was requested, but _ttywrch was */
void _ttywrch(int ch) {
    ch = ch;
}
#endif
#endif

#if defined (__GNUC__) && !defined (__clang__)
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/**
  * @brief  retargets the c library printf function to the usart.
  * @param  none
  * @retval none
  */
PUTCHAR_PROTOTYPE {
    while(usart_Flag_Get(PRINT_UART, USART_TDBE_FLAG) == RESET);

    usart_Data_transmit(PRINT_UART, ch);
    return ch;
}

#if defined (__GNUC__) && !defined (__clang__)
int _Write(int fd, char *pbuffer, int size) {
    for(int i = 0; i < size; i ++) {
        __io_putchar(*pbuffer++);
    }

    return size;
}
#endif

/**
  * @brief  initialize uart
  * @param  baudrate: uart baudrate
  * @retval none
  */
void uart_print_Init(uint32_t baudrate) {
    gpio_Init_Type gpio_Init_struct;

    #if defined (__GNUC__) && !defined (__clang__)
    setvbuf(stdout, NULL, _IONBF, 0);
    #endif

    /* enable the uart and gpio clock */
    crm_periph_Clock_Enable(PRINT_UART_CRM_CLK, TRUE);
    crm_periph_Clock_Enable(PRINT_UART_TX_GPIO_CRM_CLK, TRUE);

    GPIO_Default_Para_Init(&gpio_Init_struct);

    /* configure the uart tx pin */
    gpio_Init_struct.GPIO_Drive_Strength = GPIO_Drive_Strength_STRONGER;
    gpio_Init_struct.GPIO_Out_Type  = GPIO_OutPut_PUSH_PULL;
    gpio_Init_struct.GPIO_Mode = GPIO_Mode_MUX;
    gpio_Init_struct.GPIO_Pins = PRINT_UART_TX_PIN;
    gpio_Init_struct.GPIO_Pull = GPIO_Pull_NONE;
    gpio_Init(PRINT_UART_TX_GPIO, &gpio_Init_struct);

    GPIO_Pin_Mux_Config(PRINT_UART_TX_GPIO, PRINT_UART_TX_Pin_SOURCE, PRINT_UART_TX_Pin_MUX_Num);

    /* configure uart param */
    usart_Init(PRINT_UART, baudrate, USART_Data_8BITS, USART_Stop_1_BIT);
    usart_transmitter_Enable(PRINT_UART, TRUE);
    usart_Enable(PRINT_UART, TRUE);
}

/**
  * @brief  board initialize interface init led and button
  * @param  none
  * @retval none
  */
void at32_board_Init() {
    /* initialize delay function */
    delay_Init();

    /* configure led in at_Start_board */
    at32_led_Init(LED2);
    at32_led_Init(LED3);
    at32_led_Init(LED4);
    at32_led_off(LED2);
    at32_led_off(LED3);
    at32_led_off(LED4);

    /* configure button in at_Start board */
    at32_button_Init();
}

/**
  * @brief  configure button gpio
  * @param  button: specifies the button to be configured.
  * @retval none
  */
void at32_button_Init(void) {
    gpio_Init_Type gpio_Init_struct;

    /* enable the button clock */
    crm_periph_Clock_Enable(USER_BUTTON_CRM_CLK, TRUE);

    /* set default parameter */
    GPIO_Default_Para_Init(&gpio_Init_struct);

    /* configure button pin as input with pull-up/pull-down */
    gpio_Init_struct.GPIO_Drive_Strength = GPIO_Drive_Strength_STRONGER;
    gpio_Init_struct.GPIO_Out_Type  = GPIO_OutPut_PUSH_PULL;
    gpio_Init_struct.GPIO_Mode = GPIO_Mode_INPUT;
    gpio_Init_struct.GPIO_Pins = USER_BUTTON_PIN;
    gpio_Init_struct.GPIO_Pull = GPIO_Pull_DOWN;
    gpio_Init(USER_BUTTON_PORT, &gpio_Init_struct);
}

/**
  * @brief  returns the selected button state
  * @param  none
  * @retval the button gpio pin value
  */
uint8_t at32_button_state(void) {
    return GPIO_Input_Data_Bit_Read(USER_BUTTON_PORT, USER_BUTTON_PIN);
}

/**
  * @brief  returns which button have press down
  * @param  none
  * @retval the button have press down
  */
button_Type at32_button_press() {
    static uint8_t pressed = 1;

    /* get button state in at_Start board */
    if((pressed == 1) && (at32_button_state() != RESET)) {
        /* debounce */
        pressed = 0;
        delay_ms(10);

        if(at32_button_state() != RESET)
            return USER_BUTTON;
    } else if(at32_button_state() == RESET) {
        pressed = 1;
    }

    return NO_BUTTON;
}

/**
  * @brief  configure led gpio
  * @param  led: specifies the led to be configured.
  * @retval none
  */
void at32_led_Init(led_Type led) {
    gpio_Init_Type gpio_Init_struct;

    /* enable the led clock */
    crm_periph_Clock_Enable(led_gpio_crm_clk[led], TRUE);

    /* set default parameter */
    GPIO_Default_Para_Init(&gpio_Init_struct);

    /* configure the led gpio */
    gpio_Init_struct.GPIO_Drive_Strength = GPIO_Drive_Strength_STRONGER;
    gpio_Init_struct.GPIO_Out_Type  = GPIO_OutPut_PUSH_PULL;
    gpio_Init_struct.GPIO_Mode = GPIO_Mode_OUTPUT;
    gpio_Init_struct.GPIO_Pins = led_gpio_pin[led];
    gpio_Init_struct.GPIO_Pull = GPIO_Pull_NONE;
    gpio_Init(led_gpio_port[led], &gpio_Init_struct);
}

/**
  * @brief  turns selected led on.
  * @param  led: specifies the led to be set on.
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval none
  */
void at32_led_on(led_Type led) {
    if(led > (LED_Num - 1))
        return;

    if(led_gpio_pin[led])
        led_gpio_port[led]->clr = led_gpio_pin[led];
}

/**
  * @brief  turns selected led off.
  * @param  led: specifies the led to be set off.
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval none
  */
void at32_led_off(led_Type led) {
    if(led > (LED_Num - 1))
        return;

    if(led_gpio_pin[led])
        led_gpio_port[led]->scr = led_gpio_pin[led];
}

/**
  * @brief  turns selected led toggle.
  * @param  led: specifies the led to be set off.
  *   this parameter can be one of following parameters:
  *     @arg LED2
  *     @arg LED3
  *     @arg LED4
  * @retval none
  */
void at32_led_toggle(led_Type led) {
    if(led > (LED_Num - 1))
        return;

    if(led_gpio_pin[led])
        led_gpio_port[led]->odt ^= led_gpio_pin[led];
}

/**
  * @brief  initialize delay function
  * @param  none
  * @retval none
  */
void delay_Init() {
    /* configure systick */
    systick_Clock_Source_Config(SYSTICK_Clock_Source_AHBCLK_NODIV);
    fac_us = system_core_clock / (1000000U);
    fac_ms = fac_us * (1000U);
}

/**
  * @brief  inserts a delay time.
  * @param  nus: specifies the delay time length, in microsecond.
  * @retval none
  */
void delay_us(uint32_t nus) {
    uint32_t temp = 0;
    SysTick->LOAD = (uint32_t)(nus * fac_us);
    SysTick->VAL = 0x00;
    SysTick->CTRL |= SysTick_CTRL_Enable_Msk ;

    do {
        temp = SysTick->CTRL;
    } while((temp & 0x01) && !(temp & (1 << 16)));

    SysTick->CTRL &= ~SysTick_CTRL_Enable_Msk;
    SysTick->VAL = 0x00;
}

/**
  * @brief  inserts a delay time.
  * @param  nms: specifies the delay time length, in milliseconds.
  * @retval none
  */
void delay_ms(uint16_t nms) {
    uint32_t temp = 0;

    while(nms) {
        if(nms > STEP_DELAY_MS) {
            SysTick->LOAD = (uint32_t)(STEP_DELAY_MS * fac_ms);
            nms -= STEP_DELAY_MS;
        } else {
            SysTick->LOAD = (uint32_t)(nms * fac_ms);
            nms = 0;
        }

        SysTick->VAL = 0x00;
        SysTick->CTRL |= SysTick_CTRL_Enable_Msk;

        do {
            temp = SysTick->CTRL;
        } while((temp & 0x01) && !(temp & (1 << 16)));

        SysTick->CTRL &= ~SysTick_CTRL_Enable_Msk;
        SysTick->VAL = 0x00;
    }
}

/**
  * @brief  inserts a delay time.
  * @param  sec: specifies the delay time, in seconds.
  * @retval none
  */
void delay_sec(uint16_t sec) {
    uint16_t index;

    for(index = 0; index < sec; index++) {
        delay_ms(500);
        delay_ms(500);
    }
}

/**
  * @}
  */

/**
  * @}
  */

