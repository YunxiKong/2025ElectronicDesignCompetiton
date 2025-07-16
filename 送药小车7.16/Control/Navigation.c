#include "myfile.h"

// ����״̬
typedef enum {
    NAV_STATE_ARRIVED,
	TURN_LEFT,
	TURN_RIGHT,
} NavState;

NavState nav_state = NAV_STATE_ARRIVED;
extern int target_Position;
extern PID_t Outer;

void Turn_Around(void){
	motor(-20, 20);
	Delay_ms(2400);
}


// ����״̬��
void Navigation_Update(void) {
    switch(nav_state) {
        case NAV_STATE_ARRIVED:
            motor(0, 0);
            break;
            
		case TURN_LEFT:
            motor(5, 35);
			Delay_ms(600);
            break;
		
		case TURN_RIGHT:
            motor(35, 5);
			Delay_ms(600);
            break;
		
        default:
            motor(0, 0);// ����״̬����ִ���κβ���
            break;
    }
}


void ToRoom1(void) {
	Encoder_ResetDistance();
	target_Position = 75;
	Control();
    
    // ��һ·�ڼ��
    if(Outer.Error0 < 1.0f) {
        nav_state = TURN_LEFT;
		Navigation_Update();
    }
	
	Encoder_ResetDistance();
	target_Position = 55;
	Control();
	
	// �ڶ�·�ڼ��
    if(Outer.Error0 < 1.0f) {
        nav_state = NAV_STATE_ARRIVED;
		Navigation_Update();
    }
	LED1_ON();
}


void ReturnRoom1(void) {
	Encoder_ResetDistance();
	target_Position = 55;
	Control();
    
    // ��һ·�ڼ��
    if(Outer.Error0 < 1.0f) {
        nav_state = TURN_RIGHT;
		Navigation_Update();
    }
	
	Encoder_ResetDistance();
	target_Position = 75;
	Control();
	
	// �ڶ�·�ڼ��
    if(Outer.Error0 < 1.0f) {
		Turn_Around();
        nav_state = NAV_STATE_ARRIVED;
		Navigation_Update();
    }
	LED2_ON();
}

