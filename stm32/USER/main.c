#include "stm32f10x.h"
#include "sys.h"
#include "led.h"
#include "stdio.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include "oled.h"
#include "dht11.h"
void Hardware_Init(void)
{

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�жϿ�������������
	delay_init();									//systick��ʼ��
	LED_Init();				//LED��ʼ��
	Usart1_Init(115200);							//����1����ӡ��Ϣ��
	OLED_Init();
	OLED_Clear();	
	while(DHT11_Init())	//DHT11��ʼ��	
	{
		delay_ms(200);
	}
	
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	OLED_ShowCHinese(0,4,0);
	OLED_ShowCHinese(16,4,1);
	OLED_ShowCHinese(32,4,2);
	OLED_ShowCHinese(48,4,3);
	OLED_ShowCHinese(64,4,4);
	OLED_ShowCHinese(80,4,5);
}
int main(void)
{  
	u8 t=1;
	Hardware_Init();				//��ʼ����ΧӲ��
	extern u8 temperature;  	    
	extern u8 humidity;  
	
	while(1)
	{
		if(t%254==0)			//ÿ100ms��ȡһ��
		{
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ			
			OLED_Float(1,0,temperature,3);	
			OLED_Float(3,0,humidity,3);
		}
		t++;
		if(t==255)
		{
			t=0;
		}
	}
}
