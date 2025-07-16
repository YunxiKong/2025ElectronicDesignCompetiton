#include "myfile.h"


void ONTrack(void){
	if (openmv() == 1){
		Navigation_Update();
		if(Key_GetState() == 1){
		ToRoom1();
		}
		if(Key_GetState() == 0){
		LED1_OFF();
		Turn_Around();
		ReturnRoom1();
		}
	}
	
	
	if (openmv() == 2){
		Navigation_Update();
		if(Key_GetState() == 1){
		ToRoom2();
		}
		if(Key_GetState() == 0){
		LED1_OFF();
		Turn_Around();
		ReturnRoom2();
		}
	}
}
