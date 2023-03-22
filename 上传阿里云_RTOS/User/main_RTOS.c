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

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
/*
*************************************************************************
*                             ������
*************************************************************************
*/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */

static TaskHandle_t AppTaskCreate_Handle = NULL;  /* ���������� */
static TaskHandle_t LED_Task_Handle = NULL;	    	/* LED������ */
static TaskHandle_t KEY_Task_Handle = NULL; 			/* KEY������ */
static TaskHandle_t BEEP_Task_Handle = NULL; 			/* BEEP������ */
static TaskHandle_t ESP_Task_Handle = NULL; 			/* ESP������ */
static TaskHandle_t DHT11_Task_Handle = NULL; 		/* DHT11������ */

/*
*************************************************************************
*                           �ں˶�����
*************************************************************************
*/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */
 QueueHandle_t KEY_Queue_Handle =NULL;
 
 /******************************* �궨�� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩ�궨�塣
 */
#define  QUEUE_LEN    4   /* ���еĳ��ȣ����ɰ������ٸ���Ϣ */
#define  QUEUE_SIZE   4   /* ������ÿ����Ϣ��С���ֽڣ� */


/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */
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



/* ������Ӳ����ʼ�� */
void BSP_Init(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
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

#ifdef RTOS
#else
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
#endif
int main()
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
    /* ������Ӳ����ʼ�� */
    BSP_Init();
    /* ������Ϣ */
    if(Esp_SUB(subtopic) == 1)
    {
        printf("subscribe failed\n");
    }else{
        printf("subscribe successed\n");
    }
    /* ����AppTaskCreate���� */
    xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
    /* ����������� */           
    if(pdPASS == xReturn)
        vTaskStartScheduler();   /* �������񣬿������� */
    else
        return -1;  

    while(1);   /* ��������ִ�е����� */  
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
    BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

    taskENTER_CRITICAL();           //�����ٽ���
    
//    /* ����KEY_Queue���� */
//    KEY_Queue_Handle = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* ��Ϣ���еĳ��� */
//                        (UBaseType_t ) QUEUE_SIZE);/* ��Ϣ�Ĵ�С */
//    if(NULL != KEY_Queue_Handle)
//    printf("����KEY_Queue��Ϣ���гɹ�!\r\n");

    /* ����LED_Task���� */
    xReturn = xTaskCreate((TaskFunction_t )LED_Task, /* ������ں��� */
                        (const char*    )"LED_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,  /* ������ں������� */
                        (UBaseType_t    )1,     /* ��������ȼ� */
                        (TaskHandle_t*  )&LED_Task_Handle);/* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
    printf("����LED_Task����ɹ�!\r\n");
    
    /* ����KEY_Task���� */
    xReturn = xTaskCreate((TaskFunction_t )KEY_Task, /* ������ں��� */
                        (const char*    )"KEY_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,  /* ������ں������� */
                        (UBaseType_t    )1,     /* ��������ȼ� */
                        (TaskHandle_t*  )&KEY_Task_Handle);/* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
    printf("����KEY_Task����ɹ�!\r\n");
    
    /* ����DHT11_Task���� */
    xReturn = xTaskCreate((TaskFunction_t )DHT11_Task, /* ������ں��� */
                        (const char*    )"DHT11_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,  /* ������ں������� */
                        (UBaseType_t    )1,     /* ��������ȼ� */
                        (TaskHandle_t*  )&DHT11_Task_Handle);/* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
    printf("����DHT11_Task����ɹ�!\r\n");

    /* ����ESP_Task���� */
    xReturn = xTaskCreate((TaskFunction_t )ESP_Task, /* ������ں��� */
                        (const char*    )"ESP_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,  /* ������ں������� */
                        (UBaseType_t    )3,     /* ��������ȼ� */
                        (TaskHandle_t*  )&ESP_Task_Handle);/* ������ƿ�ָ�� */
    if(pdPASS == xReturn)
    printf("����ESP_Task����ɹ�!\r\n");
    
    vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����

    taskEXIT_CRITICAL();            //�˳��ٽ���
}


/***********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
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
  * @ ������  �� KEY_Task
  * @ ����˵���� KEY_Task
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void KEY_Task(void)
{
    while (1)
    {
        if((Get_KEY_Status(KEY1_PORT,KEY1_PIN) == KEY_STATUS_PRESS))
        {/* K1 ������  led����*/
            switch_led = 1;
            Led_B_On();
            printf("on\n");
        } 
        if((Get_KEY_Status(KEY2_PORT,KEY2_PIN) == KEY_STATUS_PRESS))
        {/* K2 ������  led�ص�*/
            switch_led = 0;
            Led_B_Off();
            printf("off\n");
        }
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
    while(1)
    {
        /* �ϱ����ݵ������� */
        if(Esp_PUB() == 0){
            printf("\r\npublish success\r\n");
        }else{
            printf("\r\npublish failed\r\n");
        }
        vTaskDelay(200);
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
    float temp = 0.0;
    float humi = 0.0;
    while(1)
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
}
