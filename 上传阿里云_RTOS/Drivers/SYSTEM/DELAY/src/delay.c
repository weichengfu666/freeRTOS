#include "SYSTEM/DELAY/inc/delay.h"
uint32_t time_count;

//粗延时函数，微秒
void Delay_SW_Us(u16 us)
{
    u16 i=0;
    while(us--)
    {
        i=10;//自己定义
        while(i--) ;
    }
}

//粗延时函数，毫秒
void Delay_SW_Ms(u16 ms)
{
    u16 i=0;
    while(ms--)
    {
        i=12000;//自己定义
        while(i--) ;
    }
}

/*  定时器延时函数，毫秒 */
void Delay_TIME_Ms(uint32_t ms)  //timer定时器设置文件timer.c
{
    time_count = ms;
    while(time_count);
}








