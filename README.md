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

## 打开 Ozone 工程

**用 Ozone 打开本工程根目录下的 `F103_HomeWork.jdebug`**（它已经把
`File.Open` 指向 `build/F103_HomeWork.elf`）。

> 不要打开别人的/参考工程的 `.jdebug`：那种工程目录里没有自己的 `build/`，
> Ozone 会报 `Program file not found`；如果对方是在更新版本的 Ozone 里创建的，
> 还会连带报 `unknown identifier "DataGraph.Add"`（本机 V3.26 不支持），
> 两个错一起出现。判断方法很简单——看报错里的行号：本工程第 39 行是
> `File.Open(...)`，没有 DataGraph 调用。


## 目录

```
Core/          CubeMX 生成的初始化代码
Drivers/       HAL 库
Tasks/         业务代码（my_task.hpp / my_task.cpp）
cmake/         工具链配置
docs/          附录图片
```

## 截图注意

程序必须在运行状态；Timeline 里的 `Power` 和 `Code` 要关掉。
CPU 一停，看门狗仍在计数，调试器会掉线。

本机 Ozone 是 **V3.26（2021 版）**，不支持在 `.jdebug` 里用
`DataGraph.Add("tick")` 自动把 `tick` 加进 Timeline（会报
`error (83): Script interpretation failure: unknown identifier "DataGraph.Add"`，
导致整个工程加载失败）。所以 `tick` 需要在 Ozone 里手工加：
打开 View → Data Graph（或 Timeline），点 `+` 添加变量 `tick`，采样率设 1 kHz。
