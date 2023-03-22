#include "BSP/BEEP/inc/beep.h"

void Beep_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStrue;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStrue.GPIO_Pin=BEEP_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(BEEP_PORT, &GPIO_InitStrue);
    GPIO_ResetBits(BEEP_PORT, BEEP_PIN); //Ä¬ÈÏ¹Ø±Õ·äÃùÆ÷
}

void Beep_On(void)
{
    GPIO_SetBits(BEEP_PORT, BEEP_PIN);
}

void Beep_Off(void)
{
    GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}


