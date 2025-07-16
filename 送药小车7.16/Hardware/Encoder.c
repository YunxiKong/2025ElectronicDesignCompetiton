#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//MotorR
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;/////////??
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							
		
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;	
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);             
	
	TIM_ICInitTypeDef TIM_ICInitStructure;							
	TIM_ICStructInit(&TIM_ICInitStructure);                         
	                                                                                                         
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                         
	TIM_ICInit(TIM3, &TIM_ICInitStructure);                         
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;                
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                         
	TIM_ICInit(TIM3, &TIM_ICInitStructure);                         
	
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
																	
	TIM_Cmd(TIM3, ENABLE);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);			//MotorL
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
                                                                            
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                         
	TIM_ICInit(TIM1, &TIM_ICInitStructure);                         
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;                
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                         
	TIM_ICInit(TIM1, &TIM_ICInitStructure);   
	
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
																	
	TIM_Cmd(TIM1, ENABLE);	
}


int16_t Encoder_GetR(void)
{
	//使用Temp变量作为中继，目的是返回CNT后将其清零
	int16_t Temp1;
	Temp1 = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp1;
}

int16_t Encoder_GetL(void)
{
	//使用Temp变量作为中继，目的是返回CNT后将其清零
	int16_t Temp2;
	Temp2 = TIM_GetCounter(TIM1);
	TIM_SetCounter(TIM1, 0);
	return Temp2;
}


// 计算行驶距离 (单位: cm)
float Encoder_GetDistance(void) {
    static float distance = 0.0f;
    
    // 获取左右编码器增量
    int32_t left_delta = Encoder_GetL();
    int32_t right_delta = Encoder_GetR();
    
    // 计算平均增量 (脉冲数)
    float avg_delta = (left_delta + right_delta) / 2.0f;
    
    // 转换为距离 (假设每厘米脉冲数)
	/*获取实际速度值和实际位置值*/
			/*Encoder_Get函数，可以获取两次读取编码器的计次值增量*/
			/*此值正比于速度，所以可以表示速度，但它的单位并不是速度的标准单位*/
			/*此处每隔40ms获取一次计次值增量，电机旋转一周的计次值增量约为408*/
			/*因此如果想转换为标准单位，比如转/秒*/
			/*则可将此句代码改成Speed = Encoder_Get() / 408.0 / 0.04;*/
    float distance_delta = avg_delta * 21.36283 / 408.0 / 0.01;//厘米每秒
    
    // 累加总距离
    distance += distance_delta;
    
    return distance;
}


// 重置距离测量
void Encoder_ResetDistance(void) {
    TIM3->CNT = 0;
    TIM4->CNT = 0;
}

