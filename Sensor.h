#ifndef __SENSOR_H
#define __SENSOR_H

#define D1 digtal(1)
#define D2 digtal(2)
#define D3 digtal(3)
#define D4 digtal(4)
#define D5 digtal(5)
#define D6 digtal(6)
#define D7 digtal(7)
#define D8 digtal(8)


void SENSOR_GPIO_Init(void);
uint8_t Sensor1_Get(void);
uint8_t Sensor2_Get(void);
uint8_t Sensor3_Get(void);
uint8_t Sensor4_Get(void);
uint8_t Sensor5_Get(void);
uint8_t Sensor6_Get(void);
uint8_t Sensor7_Get(void);
uint8_t Sensor8_Get(void);
u8 digtal(u8 channel);  					  


#endif 
