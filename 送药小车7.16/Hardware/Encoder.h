#ifndef __ENCODER_H
#define __ENCODER_H

void Encoder_Init(void);
int16_t Encoder_GetR(void);
int16_t Encoder_GetL(void);
float Encoder_GetDistance(void);
void Encoder_ResetDistance(void);

#endif
