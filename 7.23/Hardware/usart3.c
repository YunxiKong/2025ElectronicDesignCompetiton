#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
int time;

// 定义帧头帧尾
#define FRAME_HEADER 0xA5
#define FRAME_TAIL   0x5A

// 帧长度定义
#define FRAME_LEN    3

// 接收缓冲区和索引
uint8_t Serial_RxData;					// 串口接收数据变量
uint8_t Serial_RxFlag;					// 串口接收标志位
uint8_t rx_buf[FRAME_LEN];				// 帧接收缓冲区
uint8_t rx_idx = 0;						// 接收索引
uint8_t received_number = 0;			// 接收到的数字
uint8_t frame_received = 0;				// 帧接收完成标志





/**
  * 串口初始化（修正USART3配置错误）
  */
void Serial_Init(void)
{
	/* 开启时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	// 开启USART3时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// 开启GPIOB时钟
	
	/* GPIO初始化（PB10=TX，PB11=RX） */
	GPIO_InitTypeDef GPIO_InitStructure;
	// 发送引脚：PB10（复用推挽输出）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				
	// 接收引脚：PB11（上拉输入）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				
	
	/* USART3初始化（所有配置统一为USART3） */
	USART_InitTypeDef USART_InitStructure;					
	USART_InitStructure.USART_BaudRate = 115200;              // 波特率115200
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	// 同时使能收发
	USART_InitStructure.USART_Parity = USART_Parity_No;     // 无校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  // 1位停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		// 8位数据位
	USART_Init(USART3, &USART_InitStructure);  // 修正为USART3
	
	/* 使能接收中断 */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			
	
	/* NVIC配置 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			
	NVIC_InitTypeDef NVIC_InitStructure;					
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       // USART3中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      
	NVIC_Init(&NVIC_InitStructure);                         
	
	/* 使能USART3 */
	USART_Cmd(USART3, ENABLE);							
}

// 发送字节函数（保持不变）
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

// 接收标志位和数据获取函数（保持不变）
uint8_t Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)		
	{
		Serial_RxFlag = 0;
		return 1;					
	}
	return 0;						
}

uint8_t Serial_GetRxData(void)
{
	return Serial_RxData;			
}

void USART3_IRQHandler(void)
{
	if (USART_GetITStatus(USART3, USART_IT_RXNE) == SET)	
	{
		time++;  // 计数正常（验证中断触发）
		Serial_RxData = USART_ReceiveData(USART3);  // 读取接收数据
		Serial_RxFlag = 1;  // 单个字节接收标志（可保留用于调试）
		
		// 帧接收逻辑（仅在完整接收一帧且校验通过后更新数据）
		if (rx_idx == 0) 
		{
			// 第1个字节：校验帧头
			if (Serial_RxData == FRAME_HEADER)
			{
				rx_buf[rx_idx] = Serial_RxData;  // 存入帧头
				rx_idx++;  // 准备接收第2个字节
			}
			// 若不是帧头，不做处理（等待下一个帧头）
		}
		else if (rx_idx == 1)
		{
			// 第2个字节：存入数据部分
			rx_buf[rx_idx] = Serial_RxData;
			rx_idx++;  // 准备接收第3个字节
		}
		else if (rx_idx == 2)
		{
			// 第3个字节：校验帧尾
			rx_buf[rx_idx] = Serial_RxData;
			if (rx_buf[rx_idx] == FRAME_TAIL)  // 帧尾校验通过
			{
				received_number = rx_buf[1];  // 提取有效数字（第2个字节）
				frame_received = 1;  // 置位帧完成标志（通知主程序）
			}
			rx_idx = 0;  // 重置索引，准备下一帧
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);  // 清除中断标志
	}
}
