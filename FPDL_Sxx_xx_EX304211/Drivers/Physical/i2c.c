#include "i2c.h"
#include "SC95F861xB_C.H"
//bit TWI0_Flag = 0;

void MX_I2C_Init()
{
	P2CON &= ~0x30;  //设置P24 P25为高阻模式
	P2PH  &= ~0x30;
	
//	OTCON |= 0x20;   //选择TWI0模式
//	US0CON0 = 0x88;  //主模式， 使能应答标志位
//	US0CON1 = 0x01;  //---- xxxx  x为时钟速率
//	IE1 |= 0x01;
//	EA = 1;
}

/*****************************************************
*函数名称：void TWI0/PI/UART1_Int() interrupt 7
*函数功能：SSI0中断函数
*入口参数：void
*出口参数：void
*****************************************************/
//void TWI0_Int() interrupt 7     //TWI0中断函数
//{
//	if(US0CON0&0x40)
//	{
//		US0CON0 &= 0xbf;  //中断清零
//		TWI0_Flag = 1;
//	}	
//}
