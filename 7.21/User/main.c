#include "myfile.h"

extern int target_Position;
extern PID_t InnerL, InnerR, Outer;
int SpeedR, SpeedL, LocationL, LocationR, SpeedLOUT, SpeedROUT;
int target_Position = 0;
int Basic_Speed = 0;    //基础速度，在这里修改速度



int main(void){
	//OLED_Init();
	
	Key_Init();
    Motor_Init();
	Encoder_Init();
    SENSOR_GPIO_Init();
	LED_Init();
	
	Serial_Init();
	Timer_Init();
	
	
	while(1){
//		target_Position = 10000;
		
		motor(100,100);
//		Outer.Target = target_Position;
		Serial_Printf("%f,%f,%f,%f,%f,%f\r\n", InnerL.Target, InnerL.Actual, InnerL.Out, Outer.Target, Outer.Actual, Outer.Out);
//		Serial_Printf("%f,%f,%f,%f,%f,%f\r\n", InnerL.Target, InnerL.Actual, InnerL.Out, InnerR.Target, InnerR.Actual, InnerR.Out);
//		Serial_Printf("%d,%d\r\n", SpeedL, SpeedR);
//		
//		Serial_Printf("%d,%d\r\n", LocationL, LocationR);
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
			
			InnerL.Target = 100;
			SpeedL = Encoder_GetL();		//获取编码器增量，得到实际速度
			LocationL += SpeedL;			//实际速度累加，得到实际位置
			InnerL.Actual = SpeedL;			//内环为速度环，实际值为速度值
			Inner_PID_Update(&InnerL);
			SpeedLOUT = (int)InnerL.Out;

			InnerR.Target = 100;
			SpeedR = Encoder_GetR();
			LocationR += SpeedR;
			InnerR.Actual = SpeedR;
			Inner_PID_Update(&InnerR);
			SpeedROUT = (int)InnerR.Out;
			
			//MotorL_SetSpeed(SpeedLOUT);
			//MotorR_SetSpeed(SpeedROUT);
		}
		
//		/*外环计次分频*/
//		Count2 ++;				//计次自增
//		if (Count2 >= 40)		//如果计次40次，则if成立，即if每隔40ms进一次
//		{
//			Count2 = 0;			//计次清零，便于下次计次
//			
//			Outer.Actual = LocationL;		//外环为位置环，实际值为位置值
//			
//			Outer_PID_Update(&Outer);			//调用封装好的函数，一步完成PID计算和更新
//			
//			InnerL.Target = Outer.Out;
//			InnerR.Target = Outer.Out;
//		}
//		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
