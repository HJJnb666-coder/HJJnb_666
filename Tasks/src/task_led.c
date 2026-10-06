/**
 ******************************************************************************
 * @file    task_led.c
 * @brief   第 1 题：板载 LED（PC13）—— 实现
 *
 * 作业要求：
 *   F103 最小系统板：把 PC13 配成推挽输出，在 Tasks 的初始化里写成低电平
 *   （低电平点亮板载灯），不要在 while(1) 里翻转。
 *
 * 说明：
 *   PC13 的“推挽输出”配置在 CubeMX 里完成（Pinout & Configuration → GPIO →
 *   PC13 → GPIO_Output，User Label = LED_PC13），生成的代码在 Core/Src/gpio.c
 *   的 MX_GPIO_Init() 里，并给出了 LED_PC13_Pin / LED_PC13_GPIO_Port 两个宏。
 *   本文件只做业务动作：把它写成低电平。
 ******************************************************************************
 */

#include "task_led.h"

void Task_Led_Init(void)
{
    /* 低电平点亮板载灯（PC13 上的 LED 是接 3.3V 的，所以低电平亮） */
    HAL_GPIO_WritePin(LED_PC13_GPIO_Port, LED_PC13_Pin, GPIO_PIN_RESET);
}
