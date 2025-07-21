#include "myfile.h"

extern PID_t InnerL, InnerR, Outer, Differ;
float SpeedR, SpeedL, LocationL, LocationR, SpeedLOUT, SpeedROUT;
float target_Position = 0;
static int Basic_Speed = 0;    //基础速度，在这里修改速度
static float filteredL = 0, filteredR = 0;
float speed_diff;

int main(void){
//	OLED_Init();
	
	Key_Init();
    Motor_Init();
	Encoder_Init();
    SENSOR_GPIO_Init();
	LED_Init();
	
	Serial_Init();
	Timer_Init();
	
	
	while(1){
		target_Position = 468.1027739;//位置系数：0.1068141502厘米每单位,轮子转一圈200单位。
		
//		motor(50,50);
//		motor(0,50);
//		Outer.Target = target_Position;
//		Serial_Printf("%f,%f,%f,%f,%f,%f\r\n", InnerL.Target, InnerL.Actual, InnerL.Out, Outer.Target, Outer.Actual, Outer.Out);
		Serial_Printf("%f,%f,%f,%f,%f,%f,%f,%f,%f\r\n", InnerL.Target, InnerL.Actual, InnerL.Out, InnerR.Target, InnerR.Actual, InnerR.Out, Outer.Target, Outer.Actual, Outer.Out);
//		Serial_Printf("%d,%d\r\n", SpeedL, SpeedR);
//		
//		Serial_Printf("%f,%f\r\n", LocationL, LocationR);
//		ONTrack();
	}
	
}



void TIM1_UP_IRQHandler(void)
{
	static uint16_t Count1, Count2;		//分别用于内环和外环的计次分频
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		/*每隔1ms，程序执行到这里一次*/
		
//		Key_Tick();			//调用按键的Tick函数
		
		
		/*内环计次分频*/
		Count1 ++;				//计次自增
		if (Count1 >= 40)		//如果计次40次，则if成立，即if每隔40ms进一次
		{
			Count1 = 0;			//计次清零，便于下次计次
			
			filteredL = 0.7 * filteredL + 0.3 * Encoder_GetL()/5.2;
			filteredR = 0.7 * filteredR + 0.3 * Encoder_GetR()/5.2;
			
			SpeedL = filteredL;				//获取编码器增量，得到实际速度
			SpeedR = filteredR;
			
			LocationL += SpeedL;			//实际速度累加，得到实际位置
			LocationR += SpeedR;
			
			speed_diff = filteredL - filteredR;
			Differ.Error0 = -speed_diff;
			
			Differ_PID_Update(&Differ);
			
			InnerL.Target = Outer.Out - Differ.Out;
			InnerR.Target = Outer.Out + Differ.Out;
			
			InnerL.Actual = SpeedL;			//内环为速度环，实际值为速度值
			InnerR.Actual = SpeedR;
			
			Inner_PID_Update(&InnerL);
			Inner_PID_Update(&InnerR);
			
			SpeedLOUT = (int)InnerL.Out;
			SpeedROUT = (int)InnerR.Out;

			if(SpeedLOUT > 0){SpeedLOUT += 7;}
			else if(SpeedLOUT < 0){SpeedLOUT -= 7;}
			else SpeedLOUT = 0;
			if(SpeedROUT > 0){SpeedROUT += 7;}
			else if(SpeedROUT < 0){SpeedROUT -= 7;}
			else SpeedROUT = 0;
			
			MotorL_SetSpeed(SpeedLOUT);
			MotorR_SetSpeed(SpeedROUT);
			
//			MotorL_SetSpeed(100);
//			MotorR_SetSpeed(100);
		}
		
		/*外环计次分频*/
		Count2 ++;				//计次自增
		if (Count2 >= 40)		//如果计次40次，则if成立，即if每隔40ms进一次
		{
			Count2 = 0;			//计次清零，便于下次计次
			
			Outer.Target = target_Position;
			
			Outer.Actual = LocationL;			//外环为位置环，实际值为位置值
			
			Outer_PID_Update(&Outer);			//调用封装好的函数，一步完成PID计算和更新
			
		}
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
