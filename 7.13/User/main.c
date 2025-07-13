#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Motor.h"
#include "Sensor.h"

void track(void);
void track_zhixian(void);
void handle_intersection(void);
void Dashed_Line_Handler(void);
//void TIM1_UP_IRQHandler(void);
unsigned char lukou_num = 0; //全局变量定义检测到路口的次数	
int lastDirection = 0; //状态：-1右转，0直走，1左转
//unsigned int state = 1;




int main(void){
    Motor_Init();
    SENSOR_GPIO_Init();
    
    while (1){
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
    }
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

/*
	if(sensorFlag == 0){ 
		motor(0,0);
		Delay_ms(5000);
		if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0)){//在车库内，左转出库
		lukou_num = 0; 
		motor(30,5);
		Delay_ms(60);
		}
	}
	if(sensorFlag == 1){
		motor(0,0);
		Delay_ms(5000);
		if((D1 == 1)&&(D2 == 1)&&(D3 == 1)&&(D4 == 1)&&(D5 == 1)&&(D6 == 1)&&(D7 == 1)&&(D8 == 1)){//在车库外，右转入库
			motor(5,30);
			Delay_ms(60);
		}
	}  
	*/

void track_zhixian(){
	if((D1 == 1)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))        {//lastDirection=-1;
	motor(5,45);}    //0000 0001 
	else if((D1 == 1)&&(D2 == 1)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {//lastDirection=-1;
	motor(10,40);}     //0000 0011
	else if((D1 == 0)&&(D2 == 1)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {//lastDirection=-1;
	motor(13,38);}    //0000 0010
	else if((D1 == 0)&&(D2 == 1)&&(D3 == 1)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {//lastDirection=-1;
	motor(15,35);}    //0000 0110
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 1)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {//lastDirection=-1;
	motor(17,32);}    //0000 0100
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 1)&&(D4 == 1)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {//lastDirection=-1;
	motor(20,30);}    //0000 1100
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 1)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {//lastDirection= 0;
	motor(22,28);}    //0000 1000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 1)&&(D5 == 1)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {//lastDirection= 0;
	motor(25,25);}    //0001 1000 //正中间位置
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 1)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {//lastDirection= 0;
	motor(28,22);}    //0001 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 1)&&(D6 == 1)&&(D7 == 0)&&(D8 == 0))   {//lastDirection= 1;
	motor(30,20);}   //0011 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 1)&&(D7 == 0)&&(D8 == 0))   {//lastDirection= 1;
	motor(32,17);}    //0010 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 1)&&(D7 == 1)&&(D8 == 0))   {//lastDirection= 1;
	motor(35,15);}    //0110 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 1)&&(D8 == 0))   {//lastDirection= 1;
	motor(38,13);}  //0100 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 1)&&(D8 == 1))   {//lastDirection= 1;
	motor(40,10); }   //1100 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 1))   {//lastDirection= 1;
	motor(45,0); }   //1000 0000
	else if(D8 == 1){
	motor(70,5);Delay_ms(60);}
	else motor(20,20);
}

/*
void Dashed_Line_Handler(void) {	//状态保持
  static uint32_t dashTimer = 0;
  const uint32_t DASH_DELAY = 100; // 100ms延时
  
  switch (lastDirection) {
    case 1: //左转
      motor(40, 20); 
      break;
    case -1: //右转
      motor(20, 40); 
      break;
    default:
      motor(25, 25); 
      break;
  }
  
  // 延时返回
  dashTimer++;
  if (dashTimer >= DASH_DELAY) {
    dashTimer = 0;
	  return;
  }
  
  // 检测返回
  if (D1||D2||D3||D4||D5||D6||D7||D8 == 1) {
    dashTimer = 0;
	  return;
  }
}


void TIM1_UP_IRQHandler(void){
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET){
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
		if(lukou_num >= 2 ){ 
		sensorFlag = 1;
		}
		if ((D1 == 1)&&(D2 == 1)&&(D3 == 1)&&(D4 == 1)&&(D5 == 1)&&(D6 == 1)&&(D7 == 1)&&(D8 == 1)) {
		sensorFlag = 0;
		}
	}
}
*/
