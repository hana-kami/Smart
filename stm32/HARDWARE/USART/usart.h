#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"
#define USART_REC_LEN4  			200  	//定义最大接收字节数 200
extern u8  USART_RX_BUF4[USART_REC_LEN4]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA4;         		//接收状态标记	

#define USART_DEBUG		USART1		//调试打印所使用的串口组


void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void USART_SendString1(USART_TypeDef* USARTx, char *DataString);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif
