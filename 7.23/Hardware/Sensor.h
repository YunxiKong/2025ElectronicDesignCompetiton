#ifndef __SENSOR_H
#define __SENSOR_H

#define D1 Sensor1_Get()
#define D2 Sensor2_Get()
#define D3 Sensor3_Get()
#define D4 Sensor4_Get()
#define D5 Sensor5_Get()
#define D6 Sensor6_Get()
#define D7 Sensor7_Get()
#define D8 Sensor8_Get()

void SENSOR_GPIO_Init(void);
uint8_t Sensor1_Get(void);
uint8_t Sensor2_Get(void);
uint8_t Sensor3_Get(void);
uint8_t Sensor4_Get(void);
uint8_t Sensor5_Get(void);
uint8_t Sensor6_Get(void);
uint8_t Sensor7_Get(void);
uint8_t Sensor8_Get(void);				  

#endif 
