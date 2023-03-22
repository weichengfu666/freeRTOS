#include "stm32f10x.h"
#include "BSP/LED/inc/led.h"
#include "SYSTEM/DELAY/inc/delay.h"
#include "BSP/KEY/inc/key.h"
#include "BSP/BEEP/inc/beep.h"
#include "BSP/EXTI/inc/exti.h"
#include "SYSTEM/USART/inc/usart.h"
#include "BSP/ESP32/inc/esp.h"
//������
#include "BSP/DHT11/inc/dht11.h"

#include <stdio.h>
#include <string.h>



extern const char* subtopic;
float temp;
float humi;
uint8_t switch_led = 0;     //led���� 0���أ�1����
uint8_t switch_beep = 0;    //beep����0���أ�1����
uint32_t cnt;

//�����ʼ������
void Init(void)
{
    uint16_t Judge=0;
    Usart_Init();//��ʼ������
    Delay_Init();
    Beep_Init();
    Led_Init();
    Key_Init();
    DHT11_Init();
    Usart_SendString(USART1,"��ӭʹ��Ұ��STM32������\r\n");
    Usart_SendString(USART1,"Linking...\r\n");
    
    do
    {
        Judge = esp_Init();
        Usart_SendString(USART1,"\nerror code:    ");
        Usart_SendHalfWord(USART1,Judge);
        Usart_SendString(USART1,"\n\n");
    }while(Judge);  //���Ӱ�����ֱ���ɹ�

}
//led������
void led_ctl(void)
{
    if(switch_led == 0){
        Led_B_Off();
    }else if(switch_led == 1){
        Led_B_On();
    }

}
//������������
void beep_ctl(void)
{
    if(switch_beep == 0){
        Beep_Off();
    }else if(switch_beep == 1){
        Beep_On();
    }
}
//����������
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
    //������Ϣ
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
        //��ʱ1S
        Delay_IT_Ms(1000);
        //led����
        led_ctl();
        //��������
        key_ctl();
        //����������
        beep_ctl();
        //��ȡ��ʪ������
        if(DHT11_Get(&temp,&humi) == 1)
        {
            Usart_SendString(USART1,"get humiture failed");
        }
        //�ϱ����ݵ�������
        if(Esp_PUB() == 1)
        {
            Usart_SendString(USART1,"publish failed");
        }
    }
}
