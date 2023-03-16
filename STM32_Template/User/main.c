/**
  ************************* Copyright **********************
  *
  *          (C) Copyright 2022,txt1994,China, GCU.
  *                    All Rights Reserved
  *
  *                 https://github.com/txt1994
  *			        email:linguangyuan88@gmail.com
  *
  * FileName     : main.c
  * Version      : v1.0
  * Author       : txt1994
  * Date         : 2022-10-18
  * Description  :
  * Function List:

  **********************************************************
 */

#include "stm32f4xx_conf.h"
void RCC_HSE_Config(uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq);
int main(void) {

    Led_Init();
    delay_init(168);

    RCC_HSE_Config(8, 336, 4, 7);

    while(1) {

        delay_ms(100);
        GPIO_ResetBits(GPIOF, GPIO_Pin_9);
        //GPIO_SetBits(GPIOF, GPIO_Pin_10);
        delay_ms(100);
        GPIO_SetBits(GPIOF, GPIO_Pin_9);
        //GPIO_ResetBits(GPIOF, GPIO_Pin_10);
    }
}


void RCC_HSE_Config(uint32_t pllm, uint32_t plln, uint32_t pllp, uint32_t pllq) {
    RCC_DeInit(); // 将外设 RCC 寄存器重置为缺省值
    RCC_HSEConfig(RCC_HSE_ON); // 设置外部高速晶振(HSE)

    if(RCC_WaitForHSEStartUp() == SUCCESS) { // 等待 HSE 起振
        RCC_HCLKConfig(RCC_SYSCLK_Div1);// 设置AHB时钟
        RCC_PCLK1Config(RCC_HCLK_Div2);// 设置低速 APB2 时钟(PCLK2)
        RCC_PCLK2Config(RCC_HCLK_Div4);// 设置低速 APB1 时钟(PCLK1)
        RCC_PLLConfig(RCC_PLLSource_HSE, pllm, plln, pllp, pllq); // 设置PLL时钟源及倍频系数
        RCC_PLLCmd(ENABLE); // PLL使能或失能

        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);// 检查指定的RCC标志位设置与否，PLL就绪

        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);// 设置系统时钟👈

        while(RCC_GetSYSCLKSource() != 0x08);// 返回用作系统时钟的时钟源，0x08:PLL作为系统时钟
    }
}
