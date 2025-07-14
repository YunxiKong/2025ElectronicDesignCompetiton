#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Motor.h"
#include "Sensor.h"
#include "PID.h"


unsigned char lukou_num = 0; //全局变量定义检测到路口的次数	
extern PID_t leftmotor;
extern PID_t rightmotor;



void motor(int TargetL, int TargetR){
	leftmotor.Target = TargetL;	
	rightmotor.Target = TargetR;		
}


void track_zhixian(void){
	if((D1 == 1)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		leftmotor.Target = 5;
		rightmotor.Target = 45;
	}//0000 0001 
	else if((D1 == 1)&&(D2 == 1)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		leftmotor.Target = 10;
		rightmotor.Target = 40;
	}//0000 0011
	else if((D1 == 0)&&(D2 == 1)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		motor(13,38);
	}//0000 0010
	else if((D1 == 0)&&(D2 == 1)&&(D3 == 1)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		motor(15,35);
	}//0000 0110
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 1)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		motor(17,32);
	}//0000 0100
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 1)&&(D4 == 1)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		motor(20,30);
	}    //0000 1100
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 1)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		motor(22,28);
	}//0000 1000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 1)&&(D5 == 1)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		motor(25,25);
	}//0001 1000 //正中间位置
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 1)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){
		motor(28,22);
	}//0001 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 1)&&(D6 == 1)&&(D7 == 0)&&(D8 == 0)){
		motor(30,20);
	}//0011 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 1)&&(D7 == 0)&&(D8 == 0)){
		motor(32,17);
	}//0010 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 1)&&(D7 == 1)&&(D8 == 0)){
		motor(35,15);
	}//0110 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 1)&&(D8 == 0)){
		motor(38,13);
	}//0100 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 1)&&(D8 == 1)){
		motor(40,10);
	}//1100 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 1)){
		motor(45,5);
	}//1000 0000
	//else if(D8 == 1){
	//	motor(70,5);
	//	Delay_ms(60);
	//}
	else motor(20,20);
}

void handle_intersection(void) {
    static uint8_t detect_count = 0;
    
    // 持续检测100ms
    Delay_ms(100);
    if(D1 && D2 && D3 && D4 && D5 && D6 && D7 && D8) {
        detect_count++;
    } else {
        detect_count = 0;
        return;
    }

    // 连续检测到2次全黑才算路口
    if(detect_count >= 2) {
        lukou_num++;
        motor(0, 0);  // 直行通过路口
        Delay_ms(3000);   // 通过时间
        
        detect_count = 0;
    }
}

void Dashed_Line_Handler(void){
	static uint8_t detect_count1 = 0;
    
    // 持续检测100ms
    Delay_ms(200);
    if(D1 && D2 && D3 && D4 && D5 && D6 && D7 && D8) {
        detect_count1++;
    } else {
        detect_count1 = 0;
        return;
    }

    // 连续检测到2次全白才算路口
    if(detect_count1 >= 2) {
	
		motor(5,30);
		Delay_ms(600);
	}
}                                                                                                     
