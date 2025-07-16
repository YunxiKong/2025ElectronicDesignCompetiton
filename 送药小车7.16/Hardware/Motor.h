#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void PWM_Init(void);
void MotorL_SetSpeed(int8_t Speed);
void MotorR_SetSpeed(int8_t Speed);
void motor(int TargetL, int TargetR);



#endif
