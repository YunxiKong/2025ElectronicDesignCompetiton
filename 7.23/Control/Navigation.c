#include "myfile.h"

int nowtime;
extern volatile uint32_t g_systick;
extern uint8_t Room_Num;
NavState nav_state = STOP;
int key;

// 导航状态机
void Navigation_Update(void) {
    switch(nav_state) {
        case STOP:
            motor(0,0);
            break;
            
		case TURN_LEFT:
			track_zhixianL();
            break;
		
		case TURN_RIGHT:
			track_zhixianR();
            break;
		
		case TURN_AROUND:
            motor(20,-20);
			Delay_ms(1333);
			nav_state = STOP;
            break;
		
		case STRAIGHT:
            track_zhixian();
            break;
		
        default:
            motor(0,0);// 空闲状态，不执行任何操作
            break;
    }
}


void CloseTrack(void){
	nav_state = STOP;
	Navigation_Update();

	if(Room_Num ==1){
		nav_state = TURN_LEFT;
		Navigation_Update();
	}
	if(Room_Num ==2){
		nav_state = TURN_RIGHT;
		Navigation_Update();
	}		
}

void Check(void){
	if(Key_GetState()==1){
		key =1;
	}
	if(key==1){
		Room_Num = 2;
	}
}





