#include "myfile.h"

extern PID_t InnerL, InnerR, Outer, Differ;
volatile uint32_t g_systick = 0;
char systemState = 1;
extern int nowtime;

int main(void){
	OLED_Init();
	
	Key_Init();
    Motor_Init();
	Encoder_Init();
    SENSOR_GPIO_Init();
	LED_Init();
	
	Serial_Init();
	Timer_Init();
	
	
	while(1){
//		target_Position = 468.1027739;//位置系数：0.1068141502厘米每单位,轮子转一圈200单位。
		
		ToRoom1();
//		LED1_ON();
//		motor(50,50);
//		motor(0,50);
//		Outer.Target = target_Position;
		Serial_Printf("%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", InnerL.Target, InnerL.Actual, InnerL.Out, InnerR.Target, InnerR.Actual, InnerR.Out, Outer.Target, Outer.Actual, Outer.Out);
//		Serial_Printf("%d,%d\r\n", Encoder_GetL(), Encoder_GetR());		
//		Serial_Printf("%f,%f\r\n", LocationL, LocationR);
//		Serial_Printf("%f\r\n", Differ.Out);	
		
		/*OLED显示*/
		OLED_Printf(0, 16, OLED_8X16, "sy:%d", g_systick);			//显示Kp
		OLED_Printf(0, 32, OLED_8X16, "nt:%d", nowtime);			//显示Ki
		OLED_Printf(0, 48, OLED_8X16, "Kd:%4.2f", Outer.Kd);			//显示Kd
		
		OLED_Printf(64, 16, OLED_8X16, "Tar:%+04.0f", Outer.Target);	//显示目标值
		OLED_Printf(64, 32, OLED_8X16, "Act:%+04.0f", Outer.Actual);	//显示实际值
		OLED_Printf(64, 48, OLED_8X16, "Out:%+04.0f", Outer.Out);		//显示输出值
		
		OLED_Update();	//OLED更新，调用显示函数后必须调用此函数更新，否则显示的内容不会更新到OLED上
	}
	
}



void TIM1_UP_IRQHandler(void)
{

	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET){
		g_systick++; 						// 每1ms增加1,用作系统时钟
		if(systemState  == 1){
		Control();
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
