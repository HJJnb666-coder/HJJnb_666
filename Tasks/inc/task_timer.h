/**
 ******************************************************************************
 * @file    task_timer.h
 * @brief   第 2 题（1 ms tick）+ 第 3 题（看门狗）—— 对外接口
 *
 * 说明：本文件放在 Tasks/inc/ 下，task_timer.c 放在 Tasks/src/ 下。
 ******************************************************************************
 */

#ifndef TASK_TIMER_H
#define TASK_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/* 第 2 题要求的全局毫秒计数：
 * 必须叫 tick、必须是全局的 volatile uint32_t，Ozone 的 Watch/Timeline 才能
 * 看到它一直增加（不加 volatile 会被 -O 优化掉，看窗里永远是 0）。 */
extern volatile uint32_t tick;

/**
 * @brief  启动 TIM2 的 1 ms 更新中断。
 * @note   TIM2 的参数（PSC=71、ARR=999）由 CubeMX 在 Core/Src/tim.c 里配好：
 *         定时器时钟 72 MHz → (71+1)*(999+1)/72MHz = 0.001 s = 1 ms。
 *         调用位置：main.c 的 USER CODE BEGIN 2。
 */
void Task_Timer_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* TASK_TIMER_H */
