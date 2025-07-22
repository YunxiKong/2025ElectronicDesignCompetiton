#include "stm32f10x.h"                  // Device header

//GPIO引脚初始化
void SENSOR_GPIO_Init(void)
{		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
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
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11);			//返回PB4输入寄存器的状态PA10
}
