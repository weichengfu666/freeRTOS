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

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/*
*************************************************************************
*                             任务句柄
*************************************************************************
*/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */

static TaskHandle_t AppTaskCreate_Handle = NULL;  /* 创建任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;	    	/* LED任务句柄 */
static TaskHandle_t KEY_Task_Handle = NULL; 			/* KEY任务句柄 */
static TaskHandle_t BEEP_Task_Handle = NULL; 			/* BEEP任务句柄 */
static TaskHandle_t ESP_Task_Handle = NULL; 			/* ESP任务句柄 */
static TaskHandle_t DHT11_Task_Handle = NULL; 		/* DHT11任务句柄 */

/*
*************************************************************************
*                           内核对象句柄
*************************************************************************
*/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */
 QueueHandle_t KEY_Queue_Handle =NULL;
 
 /******************************* 宏定义 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些宏定义。
 */
#define  QUEUE_LEN    4   /* 队列的长度，最大可包含多少个消息 */
#define  QUEUE_SIZE   4   /* 队列中每个消息大小（字节） */


/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */
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



/* 开发板硬件初始化 */
void BSP_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
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

#ifdef RTOS
#else
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
#endif
int main()
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    /* 开发板硬件初始化 */
    BSP_Init();
    /* 订阅消息 */
    if(Esp_SUB(subtopic) == 1)
    {
        printf("subscribe failed\n");
    }else{
        printf("subscribe successed\n");
    }
    /* 创建AppTaskCreate任务 */
    xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
    /* 启动任务调度 */           
    if(pdPASS == xReturn)
        vTaskStartScheduler();   /* 启动任务，开启调度 */
    else
        return -1;  

    while(1);   /* 正常不会执行到这里 */  
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

    taskENTER_CRITICAL();           //进入临界区
    
//    /* 创建KEY_Queue队列 */
//    KEY_Queue_Handle = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* 消息队列的长度 */
//                        (UBaseType_t ) QUEUE_SIZE);/* 消息的大小 */
//    if(NULL != KEY_Queue_Handle)
//    printf("创建KEY_Queue消息队列成功!\r\n");

    /* 创建LED_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t )LED_Task, /* 任务入口函数 */
                        (const char*    )"LED_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,  /* 任务入口函数参数 */
                        (UBaseType_t    )1,     /* 任务的优先级 */
                        (TaskHandle_t*  )&LED_Task_Handle);/* 任务控制块指针 */
    if(pdPASS == xReturn)
    printf("创建LED_Task任务成功!\r\n");
    
    /* 创建KEY_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t )KEY_Task, /* 任务入口函数 */
                        (const char*    )"KEY_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,  /* 任务入口函数参数 */
                        (UBaseType_t    )1,     /* 任务的优先级 */
                        (TaskHandle_t*  )&KEY_Task_Handle);/* 任务控制块指针 */
    if(pdPASS == xReturn)
    printf("创建KEY_Task任务成功!\r\n");
    
    /* 创建DHT11_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t )DHT11_Task, /* 任务入口函数 */
                        (const char*    )"DHT11_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,  /* 任务入口函数参数 */
                        (UBaseType_t    )1,     /* 任务的优先级 */
                        (TaskHandle_t*  )&DHT11_Task_Handle);/* 任务控制块指针 */
    if(pdPASS == xReturn)
    printf("创建DHT11_Task任务成功!\r\n");

    /* 创建ESP_Task任务 */
    xReturn = xTaskCreate((TaskFunction_t )ESP_Task, /* 任务入口函数 */
                        (const char*    )"ESP_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,  /* 任务入口函数参数 */
                        (UBaseType_t    )3,     /* 任务的优先级 */
                        (TaskHandle_t*  )&ESP_Task_Handle);/* 任务控制块指针 */
    if(pdPASS == xReturn)
    printf("创建ESP_Task任务成功!\r\n");
    
    vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务

    taskEXIT_CRITICAL();            //退出临界区
}


/***********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void LED_Task(void)
{
    while (1)
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
}


/***********************************************************************
  * @ 函数名  ： KEY_Task
  * @ 功能说明： KEY_Task
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void KEY_Task(void)
{
    while (1)
    {
        if((Get_KEY_Status(KEY1_PORT,KEY1_PIN) == KEY_STATUS_PRESS))
        {/* K1 被按下  led开灯*/
            switch_led = 1;
            Led_B_On();
            printf("on\n");
        } 
        if((Get_KEY_Status(KEY2_PORT,KEY2_PIN) == KEY_STATUS_PRESS))
        {/* K2 被按下  led关灯*/
            switch_led = 0;
            Led_B_Off();
            printf("off\n");
        }
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
    while(1)
    {
        /* 上报数据到阿里云 */
        if(Esp_PUB() == 0){
            printf("\r\npublish success\r\n");
        }else{
            printf("\r\npublish failed\r\n");
        }
        vTaskDelay(200);
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
    float temp = 0.0;
    float humi = 0.0;
    while(1)
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
}
