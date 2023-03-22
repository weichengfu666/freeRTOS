#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include "SYSTEM/USART/inc/usart.h"
#include "SYSTEM/DELAY/inc/delay.h"
#include "BSP/ESP32/inc/esp.h"
#include "BSP/LED/inc/led.h"

extern float temp;
extern float humi;
extern uint8_t switch_led;
extern uint8_t switch_beep;
const char* str_temp="temp";
const char* str_humi="humi";
const char* str_led="switch_led";
const char* str_beep="switch_beep";
extern char RECS[250];
const char* WIFI ="Xiaomi_6906";
const char* WIFIASSWORD="201515Wei@";
const char* ClintID="a1ZDlryHT3x.light2|securemode=2\\,signmethod=hmacsha256\\,timestamp=1677759023866|";
const char* username="light2&a1ZDlryHT3x";
const char* passwd="063d47f65e79dfac147b784696648a59986957f3fee76f585174e7bce4b24e64";
const char* Url="a1ZDlryHT3x.iot-as-mqtt.cn-shanghai.aliyuncs.com";
const char* pubtopic="/sys/a1ZDlryHT3x/light2/thing/event/property/post";
const char* subtopic="/sys/a1ZDlryHT3x/light2/thing/service/property/set";


char esp_Init(void)
{
    memset(RECS,0,sizeof(RECS));
    Usart_Printf(USART2,"AT+RST\r\n");  //重启
    Delay_SW_Ms(2000);
    
    memset(RECS,0,sizeof(RECS));

    Usart_Printf(USART2,"ATE0\r\n");    //关闭回显
    Delay_SW_Ms(10);

    if(strcmp(RECS,"OK"))
        return 1;
    
    Usart_Printf(USART2,"AT+CWMODE=1\r\n"); //Station模式
    Delay_SW_Ms(1000);
    if(strcmp(RECS,"OK"))
        return 2;
    
    memset(RECS,0,sizeof(RECS));
    Usart_Printf(USART2,"AT+CWJAP=\"%s\",\"%s\"\r\n",WIFI,WIFIASSWORD); //连接热点
    Delay_SW_Ms(2000);
    if(strcmp(RECS,"OK"))
        return 3;
    
    memset(RECS,0,sizeof(RECS));
    Usart_Printf(USART2,"AT+MQTTUSERCFG=0,1,\"%s\",\"%s\",\"%s\",0,0,\"\"\r\n",ClintID,username,passwd);//用户信息配置
    Delay_SW_Ms(10);
    if(strcmp(RECS,"OK"))
        return 4;
    
    memset(RECS,0,sizeof(RECS));
    Usart_Printf(USART2,"AT+MQTTCONN=0,\"%s\",1883,1\r\n",Url); //连接服务器
    Delay_SW_Ms(1000);
    if(strcmp(RECS,"OK"))
        return 5;
    
    Usart_Printf(USART2,"AT+MQTTSUB=0,\"%s\",1\r\n",subtopic); //订阅消息
    Delay_SW_Ms(500);
    if(strcmp(RECS,"OK"))
        return 5;
    memset(RECS,0,sizeof(RECS));
    return 0;
}
//功能：esp发送消息
//参数：无
//返回值：0：发送成功；1：发送失败
char Esp_PUB(void)
{
    memset(RECS,0,sizeof(RECS));
    Usart_Printf(USART2,"AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"humiture:temp\\\":%f\\,\\\"humiture:humi\\\":%f\\,\\\"switch:switch_led\\\":%d\\,\\\"switch:switch_beep\\\":%d}}\",0,0\r\n",pubtopic,temp,humi,switch_led,switch_beep);

//    Usart_Printf(USART2,"AT+MQTTPUB=0,\"%s\",\"{\\\"method\\\":\\\"thing.event.property.post\\\"\\,\\\"params\\\":{\\\"%s\\\":%f\\,\\\"%s\\\":%f}}\",0,0\r\n",pubtopic,"RoomTemp",26.1,"RoomHumidity",47.2);
    while(RECS[0]);//等待ESP返回数据
    Delay_SW_Ms(200);//延时等待数据接收完成
    printf("\r\n%s\r\n",RECS);
    if(strcmp(RECS,"ERROR")==0)
        return 1;
    return 0;
}

char Esp_SUB(const char* subtopic)
{
    memset(RECS,0,sizeof(RECS));
    Usart_Printf(USART2,"AT+MQTTSUB=0,\"%s\",1\r\n",subtopic); //订阅消息
    while(RECS[0]);//等待ESP返回数据
    Delay_SW_Ms(200);//延时等待数据接收完成
    if(strcmp(RECS,"ERROR")==0)
        return 1;
    return 0;
}
/* 阿里云下发的 led 和 beep 开关命令解析 */
void CommandAnalyse(void)
{
    if(strncmp(RECS,"+MQTTSUBRECV:",13)==0)
    {
        uint8_t i=0;
        while(RECS[i++] != '\0')             
        {
            if(strncmp((RECS+i),str_led,strlen(str_led))==0)
            {
                while(RECS[i++] != ':');       
                switch_led=RECS[i]-'0';
            }
            if(strncmp((RECS+i),str_beep,strlen(str_beep))==0)
            {
                while(RECS[i++] != ':');
                switch_beep=RECS[i]-'0';
            }
        }
    }
}






