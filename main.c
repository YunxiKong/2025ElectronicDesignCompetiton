#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "PWM.h"
#include "Sensor.h"



int main(void)
{
	/*模块初始化*/
	Motor_Init();//包含了PWM的初始化PWM_Init
	SENSOR_GPIO_Init();
	
	motor(50,50);
	
	while (1){
		
		void track_zhixian();
	}
}
