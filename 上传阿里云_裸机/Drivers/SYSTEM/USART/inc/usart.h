#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include <stdio.h>
// ´®¿Ú1-USART1
#define USART1_TX_PORT  GPIOA
#define USART1_TX_PIN   GPIO_Pin_9
#define USART1_RX_PORT  GPIOA
#define USART1_RX_PIN   GPIO_Pin_10

// ´®¿Ú2-USART2
#define USART2_TX_PORT  GPIOA
#define USART2_TX_PIN   GPIO_Pin_2
#define USART2_RX_PORT  GPIOA
#define USART2_RX_PIN   GPIO_Pin_3

// ´®¿Ú3-USART3
#define USART3_TX_PORT  GPIOB
#define USART3_TX_PIN   GPIO_Pin_10
#define USART3_RX_PORT  GPIOB
#define USART3_RX_PIN   GPIO_Pin_11

#define USART_BUFLEN    10
typedef enum usart_RxTx_Flag
{   
    YES,
    NO
}usart_RxTx_Flag;
extern uint8_t usart2_BufIndex;
extern usart_RxTx_Flag usart2_RxC;
extern uint8_t usart2_RecvBuf[USART_BUFLEN];
extern uint8_t usart2_SendBuf[USART_BUFLEN];

extern uint8_t usart1_BufIndex;
extern usart_RxTx_Flag usart1_RxC;
extern uint8_t usart1_RecvBuf[USART_BUFLEN];
extern uint8_t usart1_SendBuf[USART_BUFLEN];

void Usart_Init(void);
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);
void Usart_Printf(USART_TypeDef *USARTx, char *fmt, ...);
#endif

