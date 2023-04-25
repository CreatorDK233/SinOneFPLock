#include "Function_Init.H" 

void Uart_Init(uint Freq,unsigned long int baud);
bit UartSendFlag = 0; //发送中断标志位
bit UartReceiveFlag = 0; //接收中断标志位
/*****************************************************
*函数名称：void Uart_Test(void)
*函数功能：Uart测试
*入口参数：void
*出口参数：void
*****************************************************/
void Uart_Test(void)
{
	Uart_Init(32,9600);
	while(1)
	{
		SBUF = 0x05;
		while(!UartSendFlag);
		UartSendFlag = 0;
		Soft_Delay_us(1000);
	}
}
/*****************************************************
*函数名称：void Uart_Init(uint Freq,unsigned long int baud)
*函数功能：Uart中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart_Init(uint Freq,unsigned long int baud)    //选择Timer1作为波特率信号发生器
{
	P2CON &= 0xFC;   //TX/RX设置为输入带上拉
	P2PH  |= 0x03;
	SCON  |= 0X50;   //设置通信方式为模式一，允许接收
	TXCON |= 0X03;	//UART0时钟来源为定时器1
	TH1 = (Freq*1000000/baud)>>8;	  //波特率为T1的溢出时间；
	TL1 = Freq*1000000/baud;
	TR1 = 0;	//SC95F861xB系列UARTO使用定时器1作为时钟源，TR1为0
	ET1 = 0;	
	EUART = 1;     //开启Uart中断
	EA = 1;
}

/*
void Uart_Init(uint Freq,unsigned long int baud)    //选择Timer2作为波特率信号发生器
{
	P2CON &= 0xFC;   //TX/RX设置为输入带上拉
	P2PH  |= 0x03;

	SCON  |= 0X50;   //设置通信方式为模式一，允许接收
    TXINX = 0x02;
	TMCON |= 0X04;
	TXMOD = 0X00;
	TXCON = 0X30;
	RCAPXH = Freq*1000000/baud/256;
	RCAPXL = Freq*1000000/baud%256;
	TRX = 0;
	ET2 = 0;
	EUART = 1;     //开启Uart中断
	EA = 1;
}
*/

/*****************************************************
*函数名称：void UartInt(void) interrupt 4
*函数功能：Uart中断函数
*入口参数：void
*出口参数：void
*****************************************************/
void UartInt(void) interrupt 4
{
	if(TI) //UART0发送中断
	{
		TI = 0;	
		UartSendFlag = 1;		
	}
	if(RI) //UART0接收中断
	{
		RI = 0;	
		UartReceiveFlag = 1;
	}	
}
