#include "my_task.hpp"
#include "main.h"
#include "tim.h"
#include "iwdg.h"

/* 1 = 第2题，回调里喂狗；0 = 第3题，不喂狗 */
#define FEED_WATCHDOG   1

volatile uint32_t tick = 0;

void MyTaskInit(void)
{
  /* PC13 接板载 LED，低电平点亮 */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  HAL_TIM_Base_Start_IT(&htim2);
}

/* 回调要由 HAL 的 C 代码调用，C++ 里必须加 extern "C" */
extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim == &htim2)
  {
    tick++;

#if FEED_WATCHDOG
    HAL_IWDG_Refresh(&hiwdg);
#endif
  }
}
