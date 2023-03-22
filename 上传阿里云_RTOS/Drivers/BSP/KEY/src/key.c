#include "BSP/KEY/inc/key.h"

void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStrue;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
    //初始化KEY1
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStrue.GPIO_Pin=KEY1_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY1_PORT, &GPIO_InitStrue);
    //初始化KEY2
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStrue.GPIO_Pin=KEY2_PIN;
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(KEY2_PORT, &GPIO_InitStrue);
}

KEYStatus_TypeDef Get_KEY_Status(GPIO_TypeDef* KEYx_PORT, uint16_t KEYx_PIN)
{
    KEYStatus_TypeDef key_s;
    if(GPIO_ReadInputDataBit(KEYx_PORT, KEYx_PIN) == 1){
        while(GPIO_ReadInputDataBit(KEYx_PORT, KEYx_PIN) == 1);
        key_s = KEY_STATUS_PRESS;  
    }else{
        key_s = KEY_STATUS_RAISE;
    }
    return key_s;
}


