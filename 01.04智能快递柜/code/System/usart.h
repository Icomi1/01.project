#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"


#define USART_DEBUG		USART1		//调试打印所使用的串口组


void MyUSART_Init(void);

void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);

void Usart_SendString( USART_TypeDef * pUSARTx, char *str);

#endif
