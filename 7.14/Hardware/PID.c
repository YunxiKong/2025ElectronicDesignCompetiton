#include "stm32f10x.h"                  // Device header
#include "PID.h"


void PID_Update(PID_t *p){
	p->Error1 = p->Error0;					//获取上次误差
	p->Error0 = p->Target - p->Actual;		//获取本次误差，目标值减实际值，即为误差值
	
	if (p->Ki != 0)					//如果Ki不为0
	{
		p->ErrorInt += p->Error0;	//进行误差积分
	}
	else							//否则
	{
		p->ErrorInt = 0;			//误差积分直接归0
	}
	
	/*使用位置式PID公式，计算得到输出值*/
	p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt + p->Kd * (p->Error0 - p->Error1);
	
	/*输出限幅*/
	if (p->Out > p->OutMax) {p->Out = p->OutMax;}	
	if (p->Out < p->OutMin) {p->Out = p->OutMin;}	
}

PID_t leftmotor = {					
	.Kp = 2,				
	.Ki = 0.5,				
	.Kd = 0.12,			
	.OutMax = 100,			
	.OutMin = -100,
	.Target = 0,
	.Actual = 0,
	.Out = 0,
	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
};
PID_t rightmotor = {					
	.Kp = 2,				
	.Ki = 0.5,				
	.Kd = 0.12,			
	.OutMax = 100,			
	.OutMin = -100,
	.Target = 0,
	.Actual = 0,
	.Out = 0,
	.Error0 = 0,
	.Error1 = 0,
	.ErrorInt = 0,
};
