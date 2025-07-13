#include "stm32f10x.h"                  // Device header

//GPIO引脚初始化
void SENSOR_GPIO_Init(void)
{		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
}

uint8_t Sensor1_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);			//返回PB11输入寄存器的状态
}
uint8_t Sensor2_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);			//返回PA10输入寄存器的状态
}
uint8_t Sensor3_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);			//返回PB9输入寄存器的状态
}
uint8_t Sensor4_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8);			//返回PB10输入寄存器的状态
}
uint8_t Sensor5_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9);			//返回PB5输入寄存器的状态
}
uint8_t Sensor6_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);			//返回PB8输入寄存器的状态
}
uint8_t Sensor7_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);			//返回PB0输入寄存器的状态
}
uint8_t Sensor8_Get(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10);			//返回PB4输入寄存器的状态
}

unsigned char digtal(unsigned char channel)
{
	u8 value = 0;
	switch(channel) 
	{
		case 1:  
			if(Sensor1_Get() == 1) value = 1;
			else value = 0;  
			break;  
		case 2: 
			if(Sensor2_Get() == 1) value = 1;
			else value = 0;  
			break;  
		case 3: 
			if(Sensor3_Get() == 1) value = 1;
			else value = 0;  
			break;   
		case 4:  
			if(Sensor4_Get() == 1) value = 1;
			else value = 0;  
			break;   
		case 5:
			if(Sensor5_Get() == 1) value = 1;
			else value = 0;  
			break;
		case 6:  
			if(Sensor6_Get() == 1) value = 1;
			else value = 0; 
			break;  
		case 7: 
			if(Sensor7_Get() == 1) value = 1;
			else value = 0;  
			break;  
		case 8: 
			if(Sensor8_Get() == 1) value = 1;
			else value = 0;  
			break;  
	}
	return value; 
}
