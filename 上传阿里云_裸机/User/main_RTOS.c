#include "stm32f10x.h"
#include "BSP/LED/inc/led.h"
#include "SYSTEM/DELAY/inc/delay.h"
#include "BSP/KEY/inc/key.h"
#include "BSP/BEEP/inc/beep.h"
#include "BSP/EXTI/inc/exti.h"
#include "SYSTEM/USART/inc/usart.h"
#include "BSP/ESP32/inc/esp.h"
#include "BSP\TIMER\inc\timer.h" 
//������ 
#include "BSP/DHT11/inc/dht11.h"

#include <stdio.h>
#include <string.h>



/*
*************************************************************************
*                             ��������
*************************************************************************
*/

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */
static void LED_Task(void);/* LED_Task����ʵ�� */
static void KEY_Task(void);/* KEY_Task����ʵ�� */
static void BEEP_Task(void);/* BEEP_Task����ʵ�� */
static void ESP_Task(void);/* ESP_Task����ʵ�� */
static void DHT11_Task(void);/* DHT11_Task����ʵ�� */


extern const char* subtopic;
float temp;
float humi;
uint8_t switch_led = 0;     //led���� 0���أ�1����
uint8_t switch_beep = 0;    //beep����0���أ�1����
uint32_t cnt;

uint32_t dht11_flag;
uint32_t esp_flag;



/* ������Ӳ����ʼ�� */
void BSP_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SysTick_Config(SystemCoreClock/1000);
    uint16_t Judge=0;
//    BASIC_TIM_Init();//������ʱ����ʼ��
    Usart_Init();//���ڳ�ʼ��
    Led_Init();
    Key_Init();
    DHT11_Init();
    printf("��ӭʹ��Ұ��STM32������\r\n");
    printf("Linking...\r\n");

    do
    {
        Judge = esp_Init();
        printf("\nerror code:    %d\r\n",Judge);
    }while(Judge);  //���Ӱ�����ֱ���ɹ�
}


int main()
{
    /* ������Ӳ����ʼ�� */
    BSP_Init();
    /* ������Ϣ */
    if(Esp_SUB(subtopic) == 1)
    {
        printf("subscribe failed\n");
    }else{
        printf("subscribe successed\n");
    }

    while(1)
    {
        KEY_Task();
        LED_Task();
        if(dht11_flag== 1)
        {
            dht11_flag = 0;
            DHT11_Task();
        }
        if(esp_flag == 1)
        {
            esp_flag = 0;
            ESP_Task();
        }
    }
}


/***********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void LED_Task(void)
{
    if(switch_led == 1)
    {
        Led_B_On();
    }
    if(switch_led == 0)
    {
        Led_B_Off();
    }
}


/***********************************************************************
  * @ ������  �� KEY_Task
  * @ ����˵���� KEY_Task
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void KEY_Task(void)
{
    if((Get_KEY_Status(KEY1_PORT,KEY1_PIN) == KEY_STATUS_PRESS))
    {/* K1 ������  led����*/
        switch_led = 1;
        printf("on\n");
    } 
    if((Get_KEY_Status(KEY2_PORT,KEY2_PIN) == KEY_STATUS_PRESS))
    {/* K2 ������  led�ص�*/
        switch_led = 0;
        printf("off\n");
    }
}


/***********************************************************************
  * @ ������  �� BEEP_Task
  * @ ����˵���� BEEP_Task
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void BEEP_Task(void)
{
    if(switch_beep == 0){
        Beep_Off();
    }else if(switch_beep == 1){
        Beep_On();
    }
}


/***********************************************************************
  * @ ������  �� ESP_Task
  * @ ����˵���� ESP_Task
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void ESP_Task(void)
{
    /* �ϱ����ݵ������� */
    if(Esp_PUB() == 0){
        printf("\r\npublish success\r\n");
    }else{
        printf("\r\npublish failed\r\n");
    }
}


/***********************************************************************
  * @ ������  �� DHT11_Task
  * @ ����˵���� DHT11_Task
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void DHT11_Task(void)
{
    /*����DHT11_Read_TempAndHumidity��ȡ��ʪ�ȣ����ɹ����������Ϣ*/
    if( DHT11_Get_Humiture (&humi,&temp) == SUCCESS)
    {
        printf("\r\n��ȡDHT11�ɹ�!      ʪ��Ϊ%f ��RH ���¶�Ϊ %f�� \r\n",humi,temp);
    }
    else
    {
        printf("Read DHT11 ERROR!\r\n");
    }
}
