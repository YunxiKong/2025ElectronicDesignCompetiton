#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "PWM.h"
#include "Sensor.h"
#include "Track.h"



int main(void)
{
	Motor_Init();
	SENSOR_GPIO_Init();
	
	
	while(1){
	track_zhixian();
	}
}
