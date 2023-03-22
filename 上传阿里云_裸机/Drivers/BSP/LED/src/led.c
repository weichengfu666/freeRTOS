#include "BSP/LED/inc/led.h"
void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStrue;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    //初始化红灯，并关闭
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStrue.GPIO_Pin=LED_R_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LED_R_PORT, &GPIO_InitStrue);
    GPIO_SetBits(LED_R_PORT, LED_R_PIN);
    //初始化绿灯，并关闭
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStrue.GPIO_Pin=LED_G_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LED_G_PORT, &GPIO_InitStrue);
    GPIO_SetBits(LED_G_PORT, LED_G_PIN);
    //初始化蓝灯，并关闭
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStrue.GPIO_Pin=LED_B_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(LED_G_PORT, &GPIO_InitStrue);
    GPIO_SetBits(LED_B_PORT, LED_B_PIN);
}

void Led_R_On(void)
{
    GPIO_ResetBits(LED_R_PORT, LED_R_PIN);
    GPIO_SetBits(LED_G_PORT, LED_G_PIN);
    GPIO_SetBits(LED_B_PORT, LED_B_PIN);
}

void Led_R_Off(void)
{
    GPIO_SetBits(LED_R_PORT, LED_R_PIN);
    GPIO_SetBits(LED_G_PORT, LED_G_PIN);
    GPIO_SetBits(LED_B_PORT, LED_B_PIN);
}

void Led_G_On(void)
{
    GPIO_SetBits(LED_R_PORT, LED_R_PIN);
    GPIO_ResetBits(LED_G_PORT, LED_G_PIN);
    GPIO_SetBits(LED_B_PORT, LED_B_PIN);
}

void Led_G_Off(void)
{
    GPIO_SetBits(LED_R_PORT, LED_R_PIN);
    GPIO_SetBits(LED_G_PORT, LED_G_PIN);
    GPIO_SetBits(LED_B_PORT, LED_B_PIN);
}

void Led_B_On(void)
{
    GPIO_SetBits(LED_R_PORT, LED_R_PIN);
    GPIO_SetBits(LED_G_PORT, LED_G_PIN);
    GPIO_ResetBits(LED_B_PORT, LED_B_PIN);
}

void Led_B_Off(void)
{
    GPIO_SetBits(LED_R_PORT, LED_R_PIN);
    GPIO_SetBits(LED_G_PORT, LED_G_PIN);
    GPIO_SetBits(LED_B_PORT, LED_B_PIN);
}

