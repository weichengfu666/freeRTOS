#include "SYSTEM/DELAY/inc/delay.h"
uint32_t time_count;

//����ʱ������΢��
void Delay_SW_Us(u16 us)
{
    u16 i=0;
    while(us--)
    {
        i=10;//�Լ�����
        while(i--) ;
    }
}

//����ʱ����������
void Delay_SW_Ms(u16 ms)
{
    u16 i=0;
    while(ms--)
    {
        i=12000;//�Լ�����
        while(i--) ;
    }
}

/*  ��ʱ����ʱ���������� */
void Delay_TIME_Ms(uint32_t ms)  //timer��ʱ�������ļ�timer.c
{
    time_count = ms;
    while(time_count);
}








