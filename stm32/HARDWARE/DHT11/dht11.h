#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   
//////////////////////////////////////////////////////////////////////////////////	 
									  
//////////////////////////////////////////////////////////////////////////////////

#define DHT11_IO    GPIOA
#define DHT11_PIN   GPIO_Pin_0
//IO方向设置
#define DHT11_IO_IN()  {DHT11_IO->CRL&=0XFFFFFFF0;DHT11_IO->CRL|=8<<0;}
#define DHT11_IO_OUT() {DHT11_IO->CRL&=0XFFFFFFF0;DHT11_IO->CRL|=3<<0;}
////IO操作函数											   
#define	DHT11_DQ_OUT PAout(0) //数据端口	PB4
#define	DHT11_DQ_IN  PAin(0)  //数据端口	PB4


u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
#endif















