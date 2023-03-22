#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"
#include "../../SYSTEM/DELAY/inc/delay.h"
#define KEY1_PORT   GPIOA
#define KEY1_PIN    GPIO_Pin_0
#define KEY2_PORT   GPIOC
#define KEY2_PIN    GPIO_Pin_13
typedef enum{
    KEY_STATUS_RAISE=0, //抬起按键
    KEY_STATUS_PRESS    //按下按键
}KEYStatus_TypeDef;
void Key_Init(void);
KEYStatus_TypeDef Get_KEY_Status(GPIO_TypeDef* KEYx_PORT, uint16_t KEYx_PIN);
#endif

