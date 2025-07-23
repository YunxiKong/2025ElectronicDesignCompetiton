#include "myfile.h"

extern PID_t OuterL, OuterR;
volatile uint32_t g_systick = 0;
extern int nowtime, key,time;
float SpeedR, SpeedL;
static float filteredL = 0, filteredR = 0;
extern int Room_Num;


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
		
		openmv();
//		CloseTrack();
//		track_zhixian();
//		Serial_Printf("%f,%f,%f,%f,%f,%f\r\n", OuterL.Target, OuterL.Actual, OuterL.Out, OuterR.Target, OuterR.Actual, OuterR.Out);
//		Serial_Printf("%d,%d\r\n", Encoder_GetL(), Encoder_GetR());		
//		Serial_Printf("%f,%f\r\n", LocationL, LocationR);
//		Serial_Printf("%f\r\n", Differ.Out);	
		
		/*OLED显示*/
		OLED_Printf(0, 16, OLED_8X16, "sy:%d", time);			
		OLED_Printf(0, 32, OLED_8X16, "nt:%d", g_systick);			
//		OLED_Printf(0, 48, OLED_8X16, "SR:%+04.0f", SpeedR);
//		OLED_Printf(64, 48, OLED_8X16, "SL:%+04.0f", SpeedL);		

		OLED_Printf(64, 16, OLED_8X16, "State:%d", Room_Num);	//显示目标值
//		OLED_Printf(64, 32, OLED_8X16, "Act:%+04.0f", Outer.Actual);	//显示实际值
//		OLED_Printf(64, 48, OLED_8X16, "Out:%+04.0f", Outer.Out);		//显示输出值
//		
		OLED_Update();	//OLED更新，调用显示函数后必须调用此函数更新，否则显示的内容不会更新到OLED
		
	
	}
	
}

void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		g_systick++;
		Key_Tick();
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}




