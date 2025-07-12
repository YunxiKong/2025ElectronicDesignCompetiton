#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "Motor.h"
#include "Sensor.h"

void track_zhixian(void);
void Dashed_Line_Handler(void);
void TIM1_UP_IRQHandler(void);
unsigned char lukou_num = 0; //全局变量定义检测到路口的次数	
int lastDirection; //状态：-1右转，0直走，1左转


int main(void)
{
	Motor_Init();			
	Timer_Init();		

	while (1)
	{
		Dashed_Line_Handler();
		track_zhixian();
	}
}


void track_zhixian(void)
{   
	
	unsigned char num = 0,i;  //num个灯压线认为是到达路口
	for(i=0;i<2;i++) //循环检测路口2次
	{
		if(D1 == 0)num++;	
		if(D2 == 0)num++;	
	    if(D3 == 0)num++;	
		if(D4 == 0)num++;
		if(D5 == 0)num++;	
		if(D6 == 0)num++;	
		if(D7 == 0)num++;	
		if(D8 == 0)num++;
		if(num == 8){ 
			lukou_num++; 
			if(lukou_num == 1)	Delay_ms(10); //第一次检测到延时10ms，消抖操作
		}  
		num = 0;
	}
	if(lukou_num >= 2){ 
		lukou_num = 0; 
		TIM1_UP_IRQHandler();
	}
	if(D1&&D2&&D3&&D4&&D5&&D6&&D7&&D8 == 1){
		TIM1_UP_IRQHandler();
	}  
		
	if((D1 == 1)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))        {lastDirection=-1; motor(40,0);}    //0000 0001 
	else if((D1 == 1)&&(D2 == 1)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {lastDirection=-1;motor(35,5);}     //0000 0011
	else if((D1 == 0)&&(D2 == 1)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {lastDirection=-1;motor(35,10);}    //0000 0010
	else if((D1 == 0)&&(D2 == 1)&&(D3 == 1)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {lastDirection=-1;motor(30,20);}    //0000 0110
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 1)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {lastDirection=-1;motor(30,15);}    //0000 0100
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 1)&&(D4 == 1)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {lastDirection=-1;motor(28,17);}    //0000 1100
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 1)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {lastDirection= 0;motor(25,20);}    //0000 1000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 1)&&(D5 == 1)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {lastDirection= 0;motor(20,20);}    //0001 1000 //正中间位置
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 1)&&(D6 == 0)&&(D7 == 0)&&(D8 == 0))   {lastDirection= 0;motor(20,25);}    //0001 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 1)&&(D6 == 1)&&(D7 == 0)&&(D8 == 0))   {lastDirection= 1;motor(17,28);}    //0011 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 1)&&(D7 == 0)&&(D8 == 0))   {lastDirection= 1;motor(15,30);}    //0010 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 1)&&(D7 == 1)&&(D8 == 0))   {lastDirection= 1;motor(20,30);}    //0110 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 1)&&(D8 == 0))   {lastDirection= 1;motor(10,35);}    //0100 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 1)&&(D8 == 1))   {lastDirection= 1;motor(5,35);}     //1100 0000
	else if((D1 == 0)&&(D2 == 0)&&(D3 == 0)&&(D4 == 0)&&(D5 == 0)&&(D6 == 0)&&(D7 == 0)&&(D8 == 1))   {lastDirection= 1;motor(0,40);}     //1000 0000
	else motor(25,25);
}


void Dashed_Line_Handler(void) {
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
    track_zhixian(); // 返回正常循迹
  }
  
  // 检测返回
  if (D1||D2||D3||D4||D5||D6||D7||D8 == 1) {
    dashTimer = 0;
    track_zhixian();
  }
}


void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		motor(0,0);
		Delay_ms(5000);
		if(D1&&D2&&D3&&D4&&D5&&D6&&D7&&D8 == 0){//在车库内，出库
			motor(30,5);
			Delay_ms(60);
			motor(0,0);
		}
		if(D1&&D2&&D3&&D4&&D5&&D6&&D7&&D8 == 1){//在车库外，入库
			motor(5,30);
			Delay_ms(60);
			motor(0,0);
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
