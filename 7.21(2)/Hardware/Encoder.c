#include "stm32f10x.h"                  // Device header

void Encoder_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//开启TIM3\4的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);			//开启GPIOA\B的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA\B6\7引脚初始化为上拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               //计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                //预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM3的时基单元
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	/*输入捕获初始化*/
	TIM_ICInitTypeDef TIM_ICInitStructure;							//定义结构体变量
	TIM_ICStructInit(&TIM_ICInitStructure);                         //结构体初始化，若结构体没有完整赋值
	                                                                //则最好执行此函数，给结构体所有成员都赋一个默认值
	                                                                //避免结构体初值不确定的问题
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                //选择配置定时器通道1
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                         //输入滤波器参数，可以过滤信号抖动
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInit(TIM3, &TIM_ICInitStructure);							//将结构体变量交给TIM_ICInit，配置TIM3\4的输入捕获通道
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;                //选择配置定时器通道2
	TIM_ICInitStructure.TIM_ICFilter = 0xF;                         //输入滤波器参数，可以过滤信号抖动
	TIM_ICInit(TIM4, &TIM_ICInitStructure);                         //将结构体变量交给TIM_ICInit，配置TIM3\4的输入捕获通道
	TIM_ICInit(TIM3, &TIM_ICInitStructure); 
	
	/*编码器接口配置*/
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Falling);
																	//配置编码器模式以及两个输入通道是否反相
	                                                                //注意此时参数的Rising和Falling已经不代表上升沿和下降沿了，而是代表是否反相
	                                                                //此函数必须在输入捕获初始化之后进行，否则输入捕获的配置会覆盖此函数的部分配置
	
	/*TIM使能*/
	TIM_Cmd(TIM3, ENABLE);			//使能TIM3，定时器开始运行
	TIM_Cmd(TIM4, ENABLE);	

}


int16_t Encoder_GetL(void)
{
	//使用Temp变量作为中继，目的是返回CNT后将其清零
	int16_t Temp1;
	Temp1 = TIM_GetCounter(TIM4);
	TIM_SetCounter(TIM4, 0);
	return Temp1;
}

int16_t Encoder_GetR(void){
	int16_t Temp2;
	Temp2 = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0);
	return Temp2;
}

// 重置距离测量
void Encoder_ResetDistance(void) {
    TIM3->CNT = 0;
    TIM4->CNT = 0;
}

