#ifndef __PID_H
#define __PID_H

#define Min_Max(x, min, max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

typedef struct {
	float Target;
	float Actual;
	float Out;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Error0;
	float Error1;
	float ErrorInt;
	
	float OutMax;
	float OutMin;
}PID_t;

int Inner_PID_Update(PID_t *p);
int Outer_PID_Update(PID_t *p);
void Control(void);
void Navigation_LineTrace(void);

#endif
