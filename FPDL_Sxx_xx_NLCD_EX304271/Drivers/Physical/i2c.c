#include "i2c.h"
#include "SC95F861xB_C.H"
//bit TWI0_Flag = 0;

void MX_I2C_Init()
{
	P2CON &= ~0x30;  //����P24 P25Ϊ����ģʽ
	P2PH  &= ~0x30;
	
//	OTCON |= 0x20;   //ѡ��TWI0ģʽ
//	US0CON0 = 0x88;  //��ģʽ�� ʹ��Ӧ���־λ
//	US0CON1 = 0x01;  //---- xxxx  xΪʱ������
//	IE1 |= 0x01;
//	EA = 1;
}

/*****************************************************
*�������ƣ�void TWI0/PI/UART1_Int() interrupt 7
*�������ܣ�SSI0�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
//void TWI0_Int() interrupt 7     //TWI0�жϺ���
//{
//	if(US0CON0&0x40)
//	{
//		US0CON0 &= 0xbf;  //�ж�����
//		TWI0_Flag = 1;
//	}	
//}
