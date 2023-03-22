#include "stdarg.h"
#include "string.h"
#include "SYSTEM/USART/inc/usart.h"
#include "BSP/LED/inc/led.h"
#include "BSP/ESP32/inc/esp.h"
char RECS[250];
unsigned char i;

uint8_t usart1_BufIndex  = 0;
usart_RxTx_Flag usart1_RxC = NO;
uint8_t usart1_RecvBuf[USART_BUFLEN]; 
uint8_t usart1_SendBuf[USART_BUFLEN];

uint8_t usart2_BufIndex  = 0;
usart_RxTx_Flag usart2_RxC = NO;
uint8_t usart2_RecvBuf[USART_BUFLEN];
uint8_t usart2_SendBuf[USART_BUFLEN];
static void NVIC_Usart1_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruce;
    NVIC_InitStruce.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStruce.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruce.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStruce.NVIC_IRQChannelSubPriority=3;
    NVIC_Init(&NVIC_InitStruce);
}
static void NVIC_Usart2_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruce;
    NVIC_InitStruce.NVIC_IRQChannel=USART2_IRQn;
    NVIC_InitStruce.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruce.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStruce.NVIC_IRQChannelSubPriority=3;
    NVIC_Init(&NVIC_InitStruce);
}
static void NVIC_Usart3_Init(void)
{
    NVIC_InitTypeDef NVIC_InitStruce;
    NVIC_InitStruce.NVIC_IRQChannel=USART3_IRQn;
    NVIC_InitStruce.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStruce.NVIC_IRQChannelPreemptionPriority=3;
    NVIC_InitStruce.NVIC_IRQChannelSubPriority=3;
    NVIC_Init(&NVIC_InitStruce);
}
void Usart1_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStruce;
    USART_InitTypeDef USART_InitStruce;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    
    GPIO_InitStruce.GPIO_Pin=USART1_TX_PIN;
    GPIO_InitStruce.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStruce.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(USART1_TX_PORT, &GPIO_InitStruce);
    GPIO_InitStruce.GPIO_Pin=USART1_RX_PIN;
    GPIO_InitStruce.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART1_RX_PORT, &GPIO_InitStruce);
    
    NVIC_Usart1_Init();
    
    USART_InitStruce.USART_BaudRate=115200;
    USART_InitStruce.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStruce.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruce.USART_Parity=USART_Parity_No;
    USART_InitStruce.USART_StopBits=USART_StopBits_1;
    USART_InitStruce.USART_WordLength=USART_WordLength_8b;
    USART_Init(USART1, &USART_InitStruce);
    
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
}

void Usart2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruce;
    USART_InitTypeDef USART_InitStruce;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    
    GPIO_InitStruce.GPIO_Pin=USART2_TX_PIN;
    GPIO_InitStruce.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStruce.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(USART2_TX_PORT, &GPIO_InitStruce);
    GPIO_InitStruce.GPIO_Pin=USART2_RX_PIN;
    GPIO_InitStruce.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART2_RX_PORT, &GPIO_InitStruce);
    
    NVIC_Usart2_Init();
    
    USART_InitStruce.USART_BaudRate=115200;
    USART_InitStruce.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStruce.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruce.USART_Parity=USART_Parity_No;
    USART_InitStruce.USART_StopBits=USART_StopBits_1;
    USART_InitStruce.USART_WordLength=USART_WordLength_8b;
    USART_Init(USART2, &USART_InitStruce);
    
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
}

void Usart3_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruce;
    USART_InitTypeDef USART_InitStruce;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    
    GPIO_InitStruce.GPIO_Pin=USART3_TX_PIN;
    GPIO_InitStruce.GPIO_Mode=GPIO_Mode_AF_PP;
    GPIO_InitStruce.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(USART3_TX_PORT, &GPIO_InitStruce);
    GPIO_InitStruce.GPIO_Pin=USART3_RX_PIN;
    GPIO_InitStruce.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_Init(USART3_RX_PORT, &GPIO_InitStruce);
    
    NVIC_Usart3_Init();
    
    USART_InitStruce.USART_BaudRate=115200;
    USART_InitStruce.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStruce.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruce.USART_Parity=USART_Parity_No;
    USART_InitStruce.USART_StopBits=USART_StopBits_1;
    USART_InitStruce.USART_WordLength=USART_WordLength_8b;
    USART_Init(USART3, &USART_InitStruce);
    
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART3, ENABLE);
}

void Usart_Init(void)
{
    Usart1_Init();
    Usart2_Init();
    Usart3_Init();
}

void USART1_IRQHandler(void)
{
    uint8_t data;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET){
        data = USART_ReceiveData(USART1);
        USART_SendData(USART1,data);
        USART_ClearFlag(USART1, USART_IT_RXNE);
    }
}



void USART2_IRQHandler()
{
    if(USART_GetITStatus(USART2,USART_IT_RXNE))
    {
        RECS[i++]=USART_ReceiveData(USART2);
        if((RECS[i-2]=='\r')|(RECS[i-1]=='\n'))  
        {
            RECS[i-2]='\0';
            i = 0;
//            CommandAnalyse();
        }
    }
}

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1,  ch);
    return (ch);
}







void Usart_Printf(USART_TypeDef *USARTx, char *fmt, ...)
{
    static unsigned char Usartx_TxBuff[200];
    unsigned int i, length;

    va_list ap; //va_list 可变参数列表，存参数地址

    va_start(ap, fmt); //获取可变参数地址 fmt地址赋给ap

    /*使用参数列表发送格式化输出到字符串,函数功能将可变参数格式化输出到一个字符数组*/
    vsprintf((char *)Usartx_TxBuff, fmt, ap); //fmt中内容赋给Usart3_TxBuff

    va_end(ap); //清空参数列表

    length = strlen((const char *)Usartx_TxBuff);
    while ((USARTx->SR & 0X40) == 0)
        ;
    for (i = 0; i < length; i++)
    {
        USARTx->DR = Usartx_TxBuff[i];
        while ((USARTx->SR & 0X40) == 0);
    }
}




