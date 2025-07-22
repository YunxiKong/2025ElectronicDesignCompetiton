#include "myfile.h"

float sensor_err = 0;
extern PID_t InnerL, InnerR, Outer, Differ;
float SpeedR, SpeedL, LocationL, LocationR, SpeedLOUT, SpeedROUT;
extern float target_Position;
//static int Basic_Speed = 0;    //基础速度，在这里修改速度
static float filteredL = 0, filteredR = 0;



/*定义PID结构体变量*/
PID_t InnerL = {				//内环PID结构体变量，定义的时候同时给部分成员赋初值
	.Kp = 0.6,					//比例项权重
	.Ki = 0.02,					//积分项权重
	.Kd = 0.1,					//微分项权重
	.Target = 0
};

PID_t InnerR = {				//内环PID结构体变量，定义的时候同时给部分成员赋初值
	.Kp = 0.6,					//比例项权重
	.Ki = 0.02,					//积分项权重
	.Kd = 0.1,					//微分项权重
	.Target = 0
};

PID_t Outer = {					//外环PID结构体变量，定义的时候同时给部分成员赋初值
	.Kp = 0.3,					//比例项权重
	.Ki = 0.03,					//积分项权重
	.Kd = 0.1,					//微分项权重
};

PID_t Differ = {				//差速环PID结构体变量，定义的时候同时给部分成员赋初值
	.Kp = 0.8,					//比例项权重
	.Ki = 0.3,					//积分项权重
	.Kd = 0.1,					//微分项权重
};


void Control(){	
	static uint16_t Count1, Count2;			//分别用于内环和外环的计次分频
		/*每隔1ms，程序执行到这里一次*/

//		Key_Tick();							//调用按键的Tick函数
		
		
		/*内环计次分频*/
		Count1 ++;							//计次自增
		if (Count1 >= 40){					//如果计次40次，则if成立，即if每隔40ms进一次
			Count1 = 0;						//计次清零，便于下次计次
			
			Navigation_LineTrace();
	
			filteredL = 0.7 * filteredL + 0.3 * Encoder_GetL()/5.2;
			filteredR = 0.7 * filteredR + 0.3 * Encoder_GetR()/5.2;
			
			SpeedL = filteredL;				//获取编码器增量，得到实际速度
			SpeedR = filteredR;
			
			LocationL += SpeedL;			//实际速度累加，得到实际位置
			LocationR += SpeedR;
		
			Differ.Error0 = - SpeedL + SpeedR;
			
			Differ_PID_Update(&Differ);
			
			InnerL.Target = Outer.Out;
			InnerR.Target = Outer.Out;
			
			InnerL.Actual = SpeedL;			//内环为速度环，实际值为速度值
			InnerR.Actual = SpeedR;
			
			Inner_PID_Update(&InnerL);
			Inner_PID_Update(&InnerR);
			
			SpeedLOUT = (int)InnerL.Out;
			SpeedROUT = (int)InnerR.Out;

			if(SpeedLOUT > 0){SpeedLOUT += 7;}
			else if(SpeedLOUT < 0){SpeedLOUT -= 7;}
			else SpeedLOUT = 0;
			if(SpeedROUT > 0){SpeedROUT += 7;}
			else if(SpeedROUT < 0){SpeedROUT -= 7;}
			else SpeedROUT = 0;
			
			MotorL_SetSpeed(SpeedLOUT);
			MotorR_SetSpeed(SpeedROUT);
			
//			MotorL_SetSpeed(100);
//			MotorR_SetSpeed(100);
		}
		
		/*外环计次分频*/
		Count2 ++;				//计次自增
		if (Count2 >= 40)		//如果计次40次，则if成立，即if每隔40ms进一次
		{
			Count2 = 0;			//计次清零，便于下次计次
			
			Outer.Target = target_Position;
			
			Outer.Actual = (LocationL + LocationR)/2;			//外环为位置环，实际值为位置值
			
			Outer_PID_Update(&Outer);			//调用封装好的函数，一步完成PID计算和更新
			
		}
		 
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

/************************位置式差速环pd***************************/
void Differ_PID_Update(PID_t *p) 				//外环PD控制位置环
{
	p->Error1 = p->Error0;					//获取上次误差
		
	// 积分项累加（积分限幅防饱和、变速积分）
	p->ErrorInt += p->Error0;
	p->ErrorInt = Min_Max(p->ErrorInt, -20, 20); 
    
	p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt + p->Kd * (p->Error0 - p->Error1);
}


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
    
	
	p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt + p->Kd * (p->Error0 - p->Error1);
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
    p->Out = p->Kp * p->Error0 + p->Ki * p->ErrorInt + p->Kd * (p->Error0 - p->Error1);
 
	//输出值限幅
	p->Out = Min_Max(p->Out, -25, 25);
}


// 计算位置偏差 (基于传感器状态)
void Navigation_LineTrace(){
    // 传感器布局: [S1][S2][S3][S4][S5][S6][S7][S8]
    // 理想情况: 00011000 (传感器4和5检测到线)
    if(D4 && D5) {
        sensor_err = 0.0f;  // 居中
    }
    else if(D3 && D4) {
        sensor_err = -1.8f; // 轻微左偏
    }
    else if(D5 && D6) {
        sensor_err = 1.8f;  // 轻微右偏
    }
    else if(D2 && D3) {
        sensor_err = -2.3f; // 左偏
    }
    else if(D6 && D7) {
        sensor_err = 2.3f;  // 右偏
    }
    else if(D1 || D2) {
        sensor_err = -3.5f; // 严重左偏
    }
    else if(D7 || D8) {
        sensor_err = 3.0f;  // 严重右偏
    }
    else {
        // 未检测到线，保持上次偏差
    }
}


