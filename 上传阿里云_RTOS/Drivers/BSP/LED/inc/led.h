#ifndef __LED_H
#define __LED_H
#include "stm32f10x.h"
#define LED_R_PORT  GPIOB
#define LED_R_PIN   GPIO_Pin_5
#define LED_G_PORT  GPIOB
#define LED_G_PIN   GPIO_Pin_0
#define LED_B_PORT  GPIOB
#define LED_B_PIN   GPIO_Pin_1
void Led_Init(void);
void Led_R_On(void);
void Led_R_Off(void);
void Led_G_On(void);
void Led_G_Off(void);
void Led_B_On(void);
void Led_B_Off(void);
#endif
