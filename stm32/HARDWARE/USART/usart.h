#ifndef _USART_H_
#define _USART_H_


#include "stm32f10x.h"
#define USART_REC_LEN4  			200  	//�����������ֽ��� 200
extern u8  USART_RX_BUF4[USART_REC_LEN4]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA4;         		//����״̬���	

#define USART_DEBUG		USART1		//���Դ�ӡ��ʹ�õĴ�����


void Usart1_Init(unsigned int baud);

void Usart2_Init(unsigned int baud);

void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
void USART_SendString1(USART_TypeDef* USARTx, char *DataString);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);

#endif
