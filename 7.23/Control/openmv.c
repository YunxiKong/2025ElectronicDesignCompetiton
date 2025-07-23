#include "myfile.h"

uint8_t RxData;
int Room_Num = 0;
extern uint8_t received_number;
extern uint8_t frame_received;

int openmv(void)
{
    // 关键：使用帧完成标志判断，而非单个字节标志
    if (frame_received == 1)  // 改用帧接收完成的标志
    {
        RxData = received_number;    // 此时received_number已通过帧校验，有效
    }

    // 匹配接收的数据（0x01到0x08）
    switch (RxData)
    {
        case 0x01:  // 注意：0x1等价于0x01，建议写全8位格式
            Room_Num = 1;
            break;
        case 0x02:
            Room_Num = 2;
            break;
        case 0x03:
            Room_Num = 3;
            break;
        case 0x04:
            Room_Num = 4;
            break;
        case 0x05:
            Room_Num = 5;
            break;
        case 0x06:
            Room_Num = 6;
            break;
        case 0x07:
            Room_Num = 7;
            break;
        case 0x08:
            Room_Num = 8;
            break;
        default:
            // 无匹配数据时，保持上一次的值（或显式设为0）
            // Room_Num = 0;  // 根据需求决定是否清零
            break;
    }
	return Room_Num;
}
