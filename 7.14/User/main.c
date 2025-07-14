#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Motor.h"
#include "Sensor.h"
#include "PID.h"
#include "Track.h"
#include "Encoder.h"
#include "Serial.h"

int16_t SpeedR, LocationR, SpeedL, LocationL;		//速度，位置
extern PID_t leftmotor;
extern PID_t rightmotor;

int main(void){
    Motor_Init();
    SENSOR_GPIO_Init();
	Timer_Init();
	Encoder_Init();
	Serial_Init();
    
    while (1){
		/*
        // 先检查是否虚线（全白）
        if(!D1 && !D2 && !D3 && !D4 && !D5 && !D6 && !D7 && !D8){
            //Dashed_Line_Handler();
			track_zhixian();
        }
        // 再检查是否路口（全黑）
        else if(D1 && D2 && D3 && D4 && D5 && D6 && D7 && D8) {
            handle_intersection(); // 路口处理函数
        }
        // 正常循迹
        else {
            track_zhixian(); // 调用循迹函数
        }
		*/
		
		track_zhixian();

		//leftmotor.Target = 50;
		//rightmotor.Target = 50;
		Serial_Printf("%f,%f,%f\r\n", rightmotor.Target, rightmotor.Actual, rightmotor.Out);
		Serial_Printf("%f,%f,%f\r\n", leftmotor.Target, leftmotor.Actual, leftmotor.Out);
    }
}


void TIM4_IRQHandler(void){
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) == SET){
		
		static uint16_t Count1;
		
		Count1 ++;				//计次自增
		if (Count1 >= 10)		//如果计次40次，则if成立，即if每隔40ms进一次
		{
			Count1 = 0;			//计次清零，便于下次计次
			
			/*获取实际速度值和实际位置值*/
			/*Encoder_Get函数，可以获取两次读取编码器的计次值增量*/
			/*此值正比于速度，所以可以表示速度，但它的单位并不是速度的标准单位*/
			/*此处每隔40ms获取一次计次值增量，电机旋转一周的计次值增量约为408*/
			/*因此如果想转换为标准单位，比如转/秒*/
			/*则可将此句代码改成Speed = Encoder_Get() / 408.0 / 0.04;*/
			SpeedR = Encoder_GetR()/1.425;		
			LocationR += SpeedR;			
			SpeedL = Encoder_GetL()/1.425;		
			LocationL += SpeedL;

			rightmotor.Actual = SpeedR;		
			leftmotor.Actual = SpeedL;

			PID_Update(&rightmotor);			
			PID_Update(&leftmotor);

			motorSpeed(leftmotor.Out, rightmotor.Out);
		}
		
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}



