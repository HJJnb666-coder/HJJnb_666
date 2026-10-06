# 电控第一次作业

STM32F103C8T6（F103 最小系统板），三题在同一个 CubeMX 工程内，业务代码放在 `Tasks/`。

## 题目

| 题 | 内容 |
|---|---|
| 1 | PC13 推挽输出，初始化里置低电平点亮板载 LED |
| 2 | TIM2 更新中断周期 1ms，`tick` 自增并喂狗 |
| 3 | 去掉喂狗，观察 `tick` 涨到约 2000 后归零 |

## 参数

时钟：HSE 8 MHz，PLL ×9，SYSCLK = 72 MHz，
APB1 分频 = 2，APB1 定时器时钟 = 72 MHz。

TIM2：PSC = 71，ARR = 999
```
计数时钟 = 72 MHz / (71 + 1) = 1 MHz
中断周期 = 1000 / 1 MHz = 1 ms
```

IWDG：分频 64，Reload 1249，LSI 约 40 kHz
```
超时 = (1249 + 1) × 64 / 40000 = 2.0 s
```

## 第 2 题 / 第 3 题切换

`Tasks/src/my_task.cpp`：

```c
#define FEED_WATCHDOG   1    /* 1 = 喂狗(第2题)，0 = 不喂狗(第3题) */
```

## 构建

VS Code 选 Debug，按 F7；或命令行：

```
cmake --preset Debug
cmake --build --preset Debug
```

产物在 `build/`，用 Ozone 打开 `build/F103_HomeWork.elf`。

## 目录

```
Core/          CubeMX 生成的初始化代码
Drivers/       HAL 库
Tasks/         业务代码（my_task.hpp / my_task.cpp）
cmake/         工具链配置
docs/          附录图片
```

## 截图注意

Timeline 里的 `Power` 和 `Code` 要关掉；程序必须在运行状态。
CPU 一停，看门狗仍在计数，调试器会掉线。
