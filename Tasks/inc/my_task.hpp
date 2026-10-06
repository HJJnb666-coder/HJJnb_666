#ifndef MY_TASK_HPP
#define MY_TASK_HPP

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

extern volatile uint32_t tick;

void MyTaskInit(void);

#ifdef __cplusplus
}
#endif

#endif
