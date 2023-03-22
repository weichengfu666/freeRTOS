#include "BSP/EXTI/inc/exti.h"
static void Exti_Nvic_Init(void)
{
    NVIC_InitTypeDef NVIC_InitSture;
    //�������ȼ�����
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //��ʼ���ж����ȼ�
    NVIC_InitSture.NVIC_IRQChannel=KEY1_IRQn;
    NVIC_InitSture.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitSture.NVIC_IRQChannelSubPriority=2;
    NVIC_InitSture.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitSture);
    
    NVIC_InitSture.NVIC_IRQChannel=KEY2_IRQn;
    NVIC_Init(&NVIC_InitSture);
}

/*
*   �ⲿ�ж����ò��裺
*       1.�򿪶�ӦGPIOʱ�Ӻ�APIOʱ�ӣ���ʼ��GPIOx, ��GPIOx���ӵ��ⲿ�ж���LINEx
*       2.����NVIC���ȼ�
*       3.��ʼ��EXTIx
*       4.ʵ��EXTIx_IRQHandler�ⲿ�жϺ���
*/
void Exti_Key_Init(void)
{
    EXTI_InitTypeDef EXTI_InitStruce;
    GPIO_InitTypeDef GPIO_InitStrue;
    //1.�򿪶�ӦGPIOʱ�Ӻ�APIOʱ�ӣ���ʼ��GPIOx, ��GPIOx���ӵ��ⲿ�ж���LINEx
    /*
    *   ע�⣺ʹ�ø��ù���ʱ��һ��Ҫ�򿪸���IOʱ��
    */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO, ENABLE); 
    //��ʼ��KEYx��GPIO
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStrue.GPIO_Pin=KEY1_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY1_PORT, &GPIO_InitStrue);
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStrue.GPIO_Pin=KEY2_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY2_PORT, &GPIO_InitStrue);
    //�����ж���
    GPIO_EXTILineConfig(KEY1_PORTSOURCE, KEY1_PINSOURCE);
    GPIO_EXTILineConfig(KEY2_PORTSOURCE, KEY2_PINSOURCE);
   
    //2.����NVIC���ȼ�
    Exti_Nvic_Init();
    //3.��ʼ��EXTIx
    EXTI_InitStruce.EXTI_Line=KEY1_LINE;
    EXTI_InitStruce.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitStruce.EXTI_Trigger=EXTI_Trigger_Rising;
    EXTI_InitStruce.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitStruce);
    EXTI_InitStruce.EXTI_Line=KEY2_LINE;
    EXTI_Init(&EXTI_InitStruce);
}

//4.ʵ��EXTIx_IRQHandler�ⲿ�жϺ���
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

