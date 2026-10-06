/**
 ******************************************************************************
 * @file    task_led.h
 * @brief   第 1 题：板载 LED（PC13）初始化 —— 对外接口
 ******************************************************************************
 */

#ifndef TASK_LED_H
#define TASK_LED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"   /* HAL_GPIO_WritePin / LED_PC13_Pin / LED_PC13_GPIO_Port */

/**
 * @brief  第 1 题：把 PC13 输出低电平，点亮 F103 最小系统板的板载灯。
 * @note   PC13 的推挽输出方向已经由 CubeMX 生成的 MX_GPIO_Init() 配好，
 *         这里只负责把它写成题目要求的那一个电平，不要在 while(1) 里翻转。
 */
void Task_Led_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* TASK_LED_H */
