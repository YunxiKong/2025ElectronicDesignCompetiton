#include "myfile.h"

uint8_t RxData;
int Room_Num = 0;
extern uint8_t received_number;
extern uint8_t frame_received;

int openmv(void)
{
    // �ؼ���ʹ��֡��ɱ�־�жϣ����ǵ����ֽڱ�־
    if (frame_received == 1)  // ����֡������ɵı�־
    {
        RxData = received_number;    // ��ʱreceived_number��ͨ��֡У�飬��Ч
    }

    // ƥ����յ����ݣ�0x01��0x08��
    switch (RxData)
    {
        case 0x01:  // ע�⣺0x1�ȼ���0x01������дȫ8λ��ʽ
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
            // ��ƥ������ʱ��������һ�ε�ֵ������ʽ��Ϊ0��
            // Room_Num = 0;  // ������������Ƿ�����
            break;
    }
	return Room_Num;
}
