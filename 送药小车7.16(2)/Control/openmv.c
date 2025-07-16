#include "myfile.h"

uint8_t RxData;
uint8_t Room_Num;

int openmv(void)
{
	
	//OLED_ShowString(1,1,"RxData",OLED_8X16);
	
		if(Serial_GetRxFlag() == 1)
		{
			RxData = Serial_GetRxData();	
		}
		//OLED_ShowChar(1,8,RxData, OLED_8X16);
		if(RxData == '1') {
			Room_Num = 1;
			RxData = 0;
		}
		else if(RxData == '2') {
			Room_Num = 2;
			RxData = 0;
		}
		else if(RxData == '3') {
			Room_Num = 3;
			RxData = 0;
		}
		else if(RxData == '4') {
			Room_Num = 4;
			RxData = 0;
		}
		else if(RxData == '5') {
			Room_Num = 5;
			RxData = 0;
		}
		else if(RxData == '6') {
			Room_Num = 6;
			RxData = 0;
		}
		else if(RxData == '7') {
			Room_Num = 7;
			RxData = 0;
		}
		else if(RxData == '8') {
			Room_Num = 8;
			RxData = 0;
		}
	return Room_Num;
}
