#include "stm32f10x.h"
#include "BSP/LED/inc/led.h"
#include "SYSTEM/DELAY/inc/delay.h"
#include "BSP/KEY/inc/key.h"
#include "BSP/BEEP/inc/beep.h"
#include "BSP/EXTI/inc/exti.h"
#include "SYSTEM/USART/inc/usart.h"
#include "BSP/ESP32/inc/esp.h"
//传感器
#include "BSP/DHT11/inc/dht11.h"

#include <stdio.h>
#include <string.h>



extern const char* subtopic;
float temp;
float humi;
uint8_t switch_led = 0;     //led开关 0：关，1：开
uint8_t switch_beep = 0;    //beep开关0：关，1：开
uint32_t cnt;

//外设初始化函数
void Init(void)
{
    uint16_t Judge=0;
    Usart_Init();//初始化串口
    Delay_Init();
    Beep_Init();
    Led_Init();
    Key_Init();
    DHT11_Init();
    Usart_SendString(USART1,"欢迎使用野火STM32开发板\r\n");
    Usart_SendString(USART1,"Linking...\r\n");
    
    do
    {
        Judge = esp_Init();
        Usart_SendString(USART1,"\nerror code:    ");
        Usart_SendHalfWord(USART1,Judge);
        Usart_SendString(USART1,"\n\n");
    }while(Judge);  //连接阿里云直到成功

}
//led任务函数
void led_ctl(void)
{
    if(switch_led == 0){
        Led_B_Off();
    }else if(switch_led == 1){
        Led_B_On();
    }

}
//蜂鸣器任务函数
void beep_ctl(void)
{
    if(switch_beep == 0){
        Beep_Off();
    }else if(switch_beep == 1){
        Beep_On();
    }
}
//按键任务函数
void key_ctl(void)
{
    if(Get_KEY_Status(KEY1_PORT,KEY1_PIN) == KEY_STATUS_PRESS)
    {
        switch_led = 1;
    }
    if(Get_KEY_Status(KEY2_PORT,KEY2_PIN) == KEY_STATUS_PRESS)
    {
        switch_led = 0;
    }
}

int main()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    Init();
    //订阅消息
    if(Esp_SUB(subtopic) == 1)
    {
        Usart_SendString(USART1,"subscribe failed\n");
        Delay_IT_Ms(500);
    }else{
        Usart_SendString(USART1,"subscribe successed\n");
        Delay_IT_Ms(500);
    }
    while(1)
    {
        //延时1S
        Delay_IT_Ms(1000);
        //led控制
        led_ctl();
        //按键控制
        key_ctl();
        //蜂鸣器控制
        beep_ctl();
        //读取温湿度数据
        if(DHT11_Get(&temp,&humi) == 1)
        {
            Usart_SendString(USART1,"get humiture failed");
        }
        //上报数据到阿里云
        if(Esp_PUB() == 1)
        {
            Usart_SendString(USART1,"publish failed");
        }
    }
}
