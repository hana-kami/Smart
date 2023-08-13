#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
									  
//////////////////////////////////////////////////////////////////////////////////

#define DHT11_IO    GPIOA
#define DHT11_PIN   GPIO_Pin_0
//IO��������
#define DHT11_IO_IN()  {DHT11_IO->CRL&=0XFFFFFFF0;DHT11_IO->CRL|=8<<0;}
#define DHT11_IO_OUT() {DHT11_IO->CRL&=0XFFFFFFF0;DHT11_IO->CRL|=3<<0;}
////IO��������											   
#define	DHT11_DQ_OUT PAout(0) //���ݶ˿�	PB4
#define	DHT11_DQ_IN  PAin(0)  //���ݶ˿�	PB4


u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
#endif















