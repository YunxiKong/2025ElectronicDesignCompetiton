#include "myfile.h"

int main(void){
    Motor_Init();
    SENSOR_GPIO_Init();
	Timer_Init();
	Encoder_Init();
    Key_Init();
	OLED_Init();
	LED_Init();
	
	
	while(1){
		
		ONTrack();
		
	}
	
	
	
	
	
	
	

}

