#ifndef __NAVIGATION_H
#define __NAVIGATION_H

void CloseTrack(void);
void Check(void);
void Navigation_Update(void);

typedef enum {
    STOP,
	TURN_LEFT,
	TURN_RIGHT,
	TURN_AROUND,
	STRAIGHT,
} NavState;

#endif
