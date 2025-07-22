#include "myfile.h"

float target_Position = 0;
extern PID_t Outer, InnerR, InnerL;
extern uint32_t g_systick;
extern char systemState;
extern float target_Position;
int nowtime;

typedef enum {
    TO_ROOM1_IDLE,
    TO_ROOM1_TURN_LEFT,
    TO_ROOM1_STOP2,
    TO_ROOM1_RUN_55CM,
} ToRoom1State;

static ToRoom1State room1_state = TO_ROOM1_IDLE;

void ToRoom1(void) {
	  switch(room1_state) {
        case TO_ROOM1_IDLE:
        target_Position = 75 / 0.07539822369;
		if(Outer.Actual > target_Position - 5){
			systemState = 0;
			Outer.Target = 0;Outer.Actual = 0;Outer.ErrorInt = 0;
			nowtime = g_systick;
			room1_state = TO_ROOM1_TURN_LEFT;
		}
            break;
            
        case TO_ROOM1_TURN_LEFT:
        motor(-20, 20);
        if(g_systick - nowtime > 500) {
			room1_state = TO_ROOM1_STOP2;
        }
            break;
            
        case TO_ROOM1_STOP2:
		systemState = 1;
		target_Position = 55 / 0.07539822369;
        if(Outer.Actual > target_Position - 5){
			systemState= 0;
			Outer.Target = 0;Outer.Actual = 0;Outer.ErrorInt = 0;
			room1_state = TO_ROOM1_RUN_55CM;
		}
            break;
            
        case TO_ROOM1_RUN_55CM:
			motor(0, 0);
			break;
	}
}
	  
