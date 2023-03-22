#include "stm32f10x.h"
#include "BSP/LED/inc/led.h"
#include "SYSTEM/DELAY/inc/delay.h"
#include "BSP/KEY/inc/key.h"
#include "BSP/BEEP/inc/beep.h"
#include "BSP/EXTI/inc/exti.h"
#include "SYSTEM/USART/inc/usart.h"
#include "BSP/ESP32/inc/esp.h"
#include "BSP\TIMER\inc\timer.h" 
//传感器 
#include "BSP/DHT11/inc/dht11.h"

#include <stdio.h>
#include <string.h>



/*
*************************************************************************
*                             函数声明
*************************************************************************
*/

static void BSP_Init(void);/* 用于初始化板载相关资源 */
static void LED_Task(void);/* LED_Task任务实现 */
static void KEY_Task(void);/* KEY_Task任务实现 */
static void BEEP_Task(void);/* BEEP_Task任务实现 */
static void ESP_Task(void);/* ESP_Task任务实现 */
static void DHT11_Task(void);/* DHT11_Task任务实现 */


extern const char* subtopic;
float temp;
float humi;
uint8_t switch_led = 0;     //led开关 0：关，1：开
uint8_t switch_beep = 0;    //beep开关0：关，1：开
uint32_t cnt;

uint32_t dht11_flag;
uint32_t esp_flag;



/* 开发板硬件初始化 */
void BSP_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SysTick_Config(SystemCoreClock/1000);
    uint16_t Judge=0;
//    BASIC_TIM_Init();//基本定时器初始化
    Usart_Init();//串口初始化
    Led_Init();
    Key_Init();
    DHT11_Init();
    printf("欢迎使用野火STM32开发板\r\n");
    printf("Linking...\r\n");

    do
    {
        Judge = esp_Init();
        printf("\nerror code:    %d\r\n",Judge);
    }while(Judge);  //连接阿里云直到成功
}


int main()
{
    /* 开发板硬件初始化 */
    BSP_Init();
    /* 订阅消息 */
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
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ： 无  
  * @ 返回值  ： 无
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
  * @ 函数名  ： KEY_Task
  * @ 功能说明： KEY_Task
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void KEY_Task(void)
{
    if((Get_KEY_Status(KEY1_PORT,KEY1_PIN) == KEY_STATUS_PRESS))
    {/* K1 被按下  led开灯*/
        switch_led = 1;
        printf("on\n");
    } 
    if((Get_KEY_Status(KEY2_PORT,KEY2_PIN) == KEY_STATUS_PRESS))
    {/* K2 被按下  led关灯*/
        switch_led = 0;
        printf("off\n");
    }
}


/***********************************************************************
  * @ 函数名  ： BEEP_Task
  * @ 功能说明： BEEP_Task
  * @ 参数    ： 无  
  * @ 返回值  ： 无
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
  * @ 函数名  ： ESP_Task
  * @ 功能说明： ESP_Task
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void ESP_Task(void)
{
    /* 上报数据到阿里云 */
    if(Esp_PUB() == 0){
        printf("\r\npublish success\r\n");
    }else{
        printf("\r\npublish failed\r\n");
    }
}


/***********************************************************************
  * @ 函数名  ： DHT11_Task
  * @ 功能说明： DHT11_Task
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void DHT11_Task(void)
{
    /*调用DHT11_Read_TempAndHumidity读取温湿度，若成功则输出该信息*/
    if( DHT11_Get_Humiture (&humi,&temp) == SUCCESS)
    {
        printf("\r\n读取DHT11成功!      湿度为%f ％RH ，温度为 %f℃ \r\n",humi,temp);
    }
    else
    {
        printf("Read DHT11 ERROR!\r\n");
    }
}
