#ifndef __EXTI_H
#define __EXTI_H
#include "stm32f10x.h"
#include "../../BSP/LED/inc/led.h"
#include "../../BSP/KEY/inc/key.h"
#define KEY1_PORTSOURCE GPIO_PortSourceGPIOA
#define KEY1_PINSOURCE  GPIO_PinSource0
#define KEY1_IRQn       EXTI0_IRQn
#define KEY1_LINE       EXTI_Line0
#define KEY2_PORTSOURCE GPIO_PortSourceGPIOC
#define KEY2_PINSOURCE  GPIO_PinSource13
#define KEY2_IRQn       EXTI15_10_IRQn
#define KEY2_LINE       EXTI_Line13

void Exti_Key_Init(void);
void EXTI_Key_Config(void);
#endif

