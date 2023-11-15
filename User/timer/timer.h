#ifndef __TIMER_H__
#define __TIMER_H__

#include "fm33le0xx_fl.h"
#include "main.h"

extern volatile uint32_t g_u32SysTickCount;
#define GetSysTickCount()    (g_u32SysTickCount)
void GPTIM_Start(void);
void GPTIM_Loop(void);
void Delay1Ms(uint32_t x);
#endif

