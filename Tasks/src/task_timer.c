/**
 ******************************************************************************
 * @file    task_timer.c
 * @brief   第 2 题（1 ms tick + 喂狗）+ 第 3 题（不喂狗，观察复位）—— 实现
 *
 * 说明：本文件放在 Tasks/src/ 下，头文件 task_timer.h 放在 Tasks/inc/ 下。
 *
 * 对应作业要求：
 *   第 2 题：用一个定时器的更新中断，周期 1 ms，回调里让全局变量 tick 自增 1，
 *           并调用 HAL_IWDG_Refresh 喂狗。
 *   第 3 题：IWDG 保持启用、分频和超时都不改，tick 仍每 1 ms 加 1，
 *           只是去掉回调里的 HAL_IWDG_Refresh。
 *
 * 定时器时钟从哪来（写说明文档就用这一段）：
 *   时钟树：HSE 8 MHz → PLL ×9 = SYSCLK 72 MHz → AHB ÷1 = HCLK 72 MHz
 *           → APB1 ÷2 = PCLK1 36 MHz，APB2 ÷1 = PCLK2 72 MHz。
 *   TIM2 挂在 APB1 上，而 APB1 分频不为 1 时，定时器时钟是 APB1 的 2 倍，
 *   所以 TIM2 的定时器时钟 = 36 MHz × 2 = 72 MHz。
 *   周期 = (PSC + 1) × (ARR + 1) / 定时器时钟
 *        = (71 + 1) × (999 + 1) / 72 000 000 = 0.001 s = 1 ms。
 *
 * 全局只保留这一份更新回调（HAL_TIM_PeriodElapsedCallback）：
 *   CubeMX 生成的 Core/Src/tim.c 只负责初始化；中断入口 TIM2_IRQHandler 在
 *   Core/Src/stm32f1xx_it.c 里调用 HAL_TIM_IRQHandler(&htim2)，最终进到本文件
 *   的回调。如果 CubeMX 在 main.c 里也生成了同名回调，要删掉那一份，
 *   否则链接会报重复定义。
 ******************************************************************************
 */

#include "task_timer.h"

#include "tim.h"     /* CubeMX 生成：TIM_HandleTypeDef htim2 */
#include "iwdg.h"    /* CubeMX 生成：IWDG_HandleTypeDef hiwdg */

/* ======================= 第 2 题 / 第 3 题的开关 =======================
 * 1 = 回调里调用 HAL_IWDG_Refresh 喂狗   → 第 2 题（tick 一直增加）
 * 0 = 回调里不喂狗                        → 第 3 题（约 2 秒复位一次，
 *                                            tick 从 0 涨到约 2000 再归零）
 * 做完第 2 题、截好图后，把这里改成 0，重新编译下载即可，其它代码都不用动。
 * ====================================================================== */
#define TASK_IWDG_REFRESH       1U

/* ============================ 全局变量 ============================ */

/* 作业要求变量名必须叫 tick，且为全局 volatile uint32_t */
volatile uint32_t tick = 0U;

/* ============================ 公共函数 ============================ */

/**
 * @brief  启动 TIM2 更新中断，周期 1 ms。
 * @note   TIM2 的 PSC/ARR 已经由 CubeMX 生成的 MX_TIM2_Init() 写进 htim2，
 *         这里只需要打开更新中断并启动计数。
 */
void Task_Timer_Init(void)
{
    HAL_TIM_Base_Start_IT(&htim2);
}

/**
 * @brief  TIM2 更新中断回调，每 1 ms 进来一次。
 * @note   HAL_TIM_IRQHandler() 会自己判断是哪个定时器、哪种事件，
 *         再调用这个弱函数（weak）的我们自己的实现。
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        tick++;                     /* 第 2、3 题：每 1 ms 加 1 */

#if TASK_IWDG_REFRESH
        HAL_IWDG_Refresh(&hiwdg);   /* 第 2 题喂狗；第 3 题把开关改成 0 就编译掉这一行 */
#endif
    }
}

/* ============================================================================
 * 做题提示：
 *   第 1 题 —— main.c 里把 Task_Timer_Init() 注释掉再下载，此时没人喂狗，
 *              芯片约 2 秒复位一次，灯仍然亮，赶紧拍照即可。
 *   第 2 题 —— 保持 TASK_IWDG_REFRESH = 1，tick 会一直增加，每秒约 +1000。
 *   第 3 题 —— 把 TASK_IWDG_REFRESH 改成 0，重新编译下载，tick 涨到约 2000
 *              后归零并不断重复（复位后启动代码会把全局变量清 0）。
 *
 * IWDG 的超时（CubeMX 里配的，三题都不要改）：
 *   超时 = (Reload + 1) × 分频 / LSI = (1249 + 1) × 64 / 40000 = 2.0 s
 * ========================================================================== */
