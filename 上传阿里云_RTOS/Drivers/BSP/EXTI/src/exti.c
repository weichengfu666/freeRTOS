#include "BSP/EXTI/inc/exti.h"
static void Exti_Nvic_Init(void)
{
    NVIC_InitTypeDef NVIC_InitSture;
    //配置优先级分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //初始化中断优先级
    NVIC_InitSture.NVIC_IRQChannel=KEY1_IRQn;
    NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitSture.NVIC_IRQChannelSubPriority=2;
    NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitSture);
    
    NVIC_InitSture.NVIC_IRQChannel=KEY2_IRQn;
    NVIC_Init(&NVIC_InitSture);
}

/*
*   外部中断配置步骤：
*       1.打开对应GPIO时钟和APIO时钟，初始化GPIOx, 将GPIOx连接到外部中断线LINEx
*       2.配置NVIC优先级
*       3.初始化EXTIx
*       4.实现EXTIx_IRQHandler外部中断函数
*/
void Exti_Key_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStruce;
    GPIO_InitTypeDef GPIO_InitStrue;
    //1.打开对应GPIO时钟和APIO时钟，初始化GPIOx, 将GPIOx连接到外部中断线LINEx
    /*
    *   注意：使用复用功能时，一定要打开复用IO时钟
    */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE); 
    //初始化KEYx的GPIO
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStrue.GPIO_Pin=KEY1_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY1_PORT, &GPIO_InitStrue);
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStrue.GPIO_Pin=KEY2_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY2_PORT, &GPIO_InitStrue);
    //配置中断线
    GPIO_EXTILineConfig(KEY1_PORTSOURCE, KEY1_PINSOURCE);
    GPIO_EXTILineConfig(KEY2_PORTSOURCE, KEY2_PINSOURCE);
   
    //2.配置NVIC优先级
    Exti_Nvic_Init();
    //3.初始化EXTIx
    EXTI_InitStruce.EXTI_Line=KEY1_LINE;
    EXTI_InitStruce.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruce.EXTI_Trigger=EXTI_Trigger_Rising;
    EXTI_InitStruce.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStruce);
    EXTI_InitStruce.EXTI_Line=KEY2_LINE;
    EXTI_Init(&EXTI_InitStruce);
}

//4.实现EXTIx_IRQHandler外部中断函数
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(KEY1_LINE) == SET)
    {
        Led_R_On();
        EXTI_ClearITPendingBit(KEY1_LINE);
    }
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(KEY2_LINE) == SET)
    {
        Led_G_On();
        EXTI_ClearITPendingBit(KEY2_LINE);
    }
}

