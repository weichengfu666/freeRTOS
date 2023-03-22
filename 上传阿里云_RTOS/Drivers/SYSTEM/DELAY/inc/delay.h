#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"
extern uint32_t time_count;
void Delay_SW_Us(u16 us);
void Delay_SW_Ms(u16 ms);
void Delay_TIME_Ms(uint32_t ms);
#endif
