#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>
int time;

// ����֡ͷ֡β
#define FRAME_HEADER 0xA5
#define FRAME_TAIL   0x5A

// ֡���ȶ���
#define FRAME_LEN    3

// ���ջ�����������
uint8_t Serial_RxData;					// ���ڽ������ݱ���
uint8_t Serial_RxFlag;					// ���ڽ��ձ�־λ
uint8_t rx_buf[FRAME_LEN];				// ֡���ջ�����
uint8_t rx_idx = 0;						// ��������
uint8_t received_number = 0;			// ���յ�������
uint8_t frame_received = 0;				// ֡������ɱ�־





/**
  * ���ڳ�ʼ��������USART3���ô���
  */
void Serial_Init(void)
{
	/* ����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	// ����USART3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	// ����GPIOBʱ��
	
	/* GPIO��ʼ����PB10=TX��PB11=RX�� */
	GPIO_InitTypeDef GPIO_InitStructure;
	// �������ţ�PB10���������������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				
	// �������ţ�PB11���������룩
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);				
	
	/* USART3��ʼ������������ͳһΪUSART3�� */
	USART_InitTypeDef USART_InitStructure;					
	USART_InitStructure.USART_BaudRate = 115200;              // ������115200
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;	// ͬʱʹ���շ�
	USART_InitStructure.USART_Parity = USART_Parity_No;     // ��У��
	USART_InitStructure.USART_StopBits = USART_StopBits_1;  // 1λֹͣλ
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;		// 8λ����λ
	USART_Init(USART3, &USART_InitStructure);  // ����ΪUSART3
	
	/* ʹ�ܽ����ж� */
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			
	
	/* NVIC���� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);			
	NVIC_InitTypeDef NVIC_InitStructure;					
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;       // USART3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;      
	NVIC_Init(&NVIC_InitStructure);                         
	
	/* ʹ��USART3 */
	USART_Cmd(USART3, ENABLE);							
}

// �����ֽں��������ֲ��䣩
void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART3, Byte);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}

// ���ձ�־λ�����ݻ�ȡ���������ֲ��䣩
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
		time++;  // ������������֤�жϴ�����
		Serial_RxData = USART_ReceiveData(USART3);  // ��ȡ��������
		Serial_RxFlag = 1;  // �����ֽڽ��ձ�־���ɱ������ڵ��ԣ�
		
		// ֡�����߼���������������һ֡��У��ͨ����������ݣ�
		if (rx_idx == 0) 
		{
			// ��1���ֽڣ�У��֡ͷ
			if (Serial_RxData == FRAME_HEADER)
			{
				rx_buf[rx_idx] = Serial_RxData;  // ����֡ͷ
				rx_idx++;  // ׼�����յ�2���ֽ�
			}
			// ������֡ͷ�����������ȴ���һ��֡ͷ��
		}
		else if (rx_idx == 1)
		{
			// ��2���ֽڣ��������ݲ���
			rx_buf[rx_idx] = Serial_RxData;
			rx_idx++;  // ׼�����յ�3���ֽ�
		}
		else if (rx_idx == 2)
		{
			// ��3���ֽڣ�У��֡β
			rx_buf[rx_idx] = Serial_RxData;
			if (rx_buf[rx_idx] == FRAME_TAIL)  // ֡βУ��ͨ��
			{
				received_number = rx_buf[1];  // ��ȡ��Ч���֣���2���ֽڣ�
				frame_received = 1;  // ��λ֡��ɱ�־��֪ͨ������
			}
			rx_idx = 0;  // ����������׼����һ֡
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);  // ����жϱ�־
	}
}
