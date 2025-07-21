#include "myfile.h"

int sensor_err = 0;

/*定义PID结构体变量*/
PID_t InnerL = {				//内环PID结构体变量，定义的时候同时给部分成员赋初值
	.Kp = 1,					//比例项权重
	.Ki = 0,					//积分项权重
	.Kd = 0,					//微分项权重
	.OutMax = 100,				//输出限幅的最大值
	.OutMin = -100,				//输出限幅的最小值
};

PID_t InnerR = {				//内环PID结构体变量，定义的时候同时给部分成员赋初值
	.Kp = 1,					//比例项权重
	.Ki = 0,					//积分项权重
	.Kd = 0,					//微分项权重
	.OutMax = 100,				//输出限幅的最大值
	.OutMin = -100,				//输出限幅的最小值
};

PID_t Outer = {					//外环PID结构体变量，定义的时候同时给部分成员赋初值
	.Kp = 1,					//比例项权重
	.Ki = 0,					//积分项权重
	.Kd = 0,					//微分项权重
	.OutMax = 20,				//输出限幅的最大值
	.OutMin = -20,				//输出限幅的最小值
};


void Control()
{
	Navigation_LineTrace();
			 
}


/*************PID计算公式**************/
/*
void PID_Update(PID_t *p)
	{
	p->Error1 = p->Error0;					//获取上次误差
	p->Error0 = p->Target - p->Actual;		//获取本次误差，目标值减实际值，即为误差值
	
	if (p->Ki != 0)							//如果Ki不为0
	{
		p->ErrorInt += p->Error0;			//进行误差积分
	}
	else									//否则
	{
		p->ErrorInt = 0;					//误差积分直接归0
	}
	
	//使用位置式PID公式，计算得到输出值
	p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt + p->Kd * (p->Error0 - p->Error1);
	
	//输出限幅
	if (p->Out > p->OutMax) {p->Out = p->OutMax;}	
	if (p->Out < p->OutMin) {p->Out = p->OutMin;}	
    }
*/

/************************位置式位置环pd***************************/
void Outer_PID_Update(PID_t *p) 				//外环PD控制位置环
{
	p->Error1 = p->Error0;					//获取上次误差
	p->Error0 = p->Target - p->Actual;		//获取本次误差，目标值减实际值，即为误差值
	
	/*定义一个系数C，表示积分的速度，C的值与误差绝对值大小呈反比，误差绝对值越大，积分速度越慢*/
	float C = 1 / (0.2 * fabs(p->Error0) + 1);			//根据公式计算得到系数C,参数越大积分越不敏感
	
	// 积分项累加（积分限幅防饱和、变速积分）
	p->ErrorInt += C * p->Error0;
	p->ErrorInt = Min_Max(p->ErrorInt, -10, 10); 	// 示例：INTEGRAL_MAX=1000
    
	
	p->Out = p->Kp * p->Error0;
}


/************************位置式速度（角度）环pid***************************/
void Inner_PID_Update(PID_t *p) 				//内环PI控制速度环
{ 	
	p->Error1 = p->Error0;					//获取上次误差
	p->Error0 = p->Target - p->Actual;		//获取本次误差，目标值减实际值，即为误差值
    
	/*定义一个系数C，表示积分的速度，C的值与误差绝对值大小呈反比，误差绝对值越大，积分速度越慢*/
	float C = 1 / (0.2 * fabs(p->Error0) + 1);			//根据公式计算得到系数C,参数越大积分越不敏感
	
	// 积分项累加（积分限幅防饱和、变速积分）
	p->ErrorInt += C * p->Error0;
    p->ErrorInt = Min_Max(p->ErrorInt, -10, 10); 	// 示例：INTEGRAL_MAX=1000
    
	//pid计算
    p->Out = p->Kp * p->Error0;
 
	//输出值限幅
	//p->Out = Min_Max(p->Out, -100, 100);
}


// 计算位置偏差 (基于传感器状态)
void Navigation_LineTrace(){
    // 传感器布局: [S1][S2][S3][S4][S5][S6][S7][S8]
    // 理想情况: 00011000 (传感器4和5检测到线)
    if(D4 && D5) {
        sensor_err = 0.0f; // 居中
    }
    else if(D3 && D4) {
        sensor_err = -1.0f; // 轻微左偏
    }
    else if(D5 && D6) {
        sensor_err = 1.0f; // 轻微右偏
    }
    else if(D2 && D3) {
        sensor_err = -2.0f; // 左偏
    }
    else if(D6 && D7) {
        sensor_err = 2.0f; // 右偏
    }
    else if(D1 || D2) {
        sensor_err = -3.0f; // 严重左偏
    }
    else if(D7 || D8) {
        sensor_err = 3.0f; // 严重右偏
    }
    else {
        // 未检测到线，保持上次偏差
    }
}


