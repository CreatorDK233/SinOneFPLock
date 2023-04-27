#include "Function_Init.H"

//选择三合一模式：
#define  Uart1   0
#define  TWI1    1
#define  SPI1    2

#define  SSI1_Mode  Uart1

void Uart1_Init(uint Freq,unsigned long int baud);
void TWI1_Init(void);
void SPI1_Init(void);

bit Uart1SendFlag = 0;    //Uart1发送中断标志位
bit Uart1ReceiveFlag = 0; //Uart1接收中断标志位
bit SPI1Flag = 0;         //SPI1数据传输完成标志位
bit TWI1Flag = 0;         //TWI1中断标志位

/*****************************************************
*函数名称：void SSI1_Test(void)
*函数功能：SSI1测试
*入口参数：void
*出口参数：void
*****************************************************/
void SSI1_Test(void)
{
#if (SSI1_Mode == Uart1)
	Uart1_Init(32,9600);
	while(1)
	{
		US1CON3 = 0xAA;
		while(!Uart1SendFlag);
		Uart1SendFlag = 0;
	}
#endif
	
#if (SSI1_Mode == TWI1)
	TWI1_Init();
	while(1)
	{
		US1CON1 |= 0x20;    //产生起始条件
				while(!TWI1Flag);
        TWI1Flag = 0;		
		US1CON3 = 0x10;	    ////发送地址及写命令
        while(!TWI1Flag);
        TWI1Flag = 0;
    US1CON3 = 0x55;
        while(!TWI1Flag);
        TWI1Flag = 0;
		Soft_Delay_us(100);
		US1CON1 |= 0x10;
		Soft_Delay_us(100);
	}
#endif

#if (SSI1_Mode == SPI1)
	SPI1_Init();
	while(1)
	{
		US1CON2 = 0xAA;
		while(!SPI1Flag);
		SPI1Flag = 0;
        Soft_Delay_us(1000);
	}
#endif	
}
/*****************************************************
*函数名称：void Uart1_Init(uint Freq,unsigned long int baud)
*函数功能：Uart1中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart1_Init(uint Freq,unsigned long int baud)
{
	P1CON &= 0xF5;    //TX/RX设置为输入带上拉
	P1PH  |= 0x0A;
	
	OTCON |= 0xC0;    //串行接口SSI1选择Uart1通信
	US1CON0 = 0x50;   //设置通信方式为模式一，允许接收
	US1CON1 = Freq*1000000/baud;   //波特率低位控制
	US1CON2 = (Freq*1000000/baud)>>8;   //波特率高位控制
	IE2 |= 0x01;      //开启SSI1中断
    EA = 1;	
}

/*****************************************************
*函数名称：void TWI1_Init(void)
*函数功能：TWI1初始化
*入口参数：void
*出口参数：void
*****************************************************/
void TWI1_Init(void)
{
	OTCON |= 0x80;   //选择TWI1模式
	US1CON0 = 0x80;  //主模式， 使能应答标志位
	US1CON1 = 0x05;  //---- xxxx  x为时钟速率
	IE2 |= 0x01;
	EA = 1;
}

/*****************************************************
*函数名称：void SPI1_Init(void)
*函数功能：SPI1初始化
*入口参数：void
*出口参数：void
*****************************************************/
void SPI1_Init(void)
{
	OTCON |= 0X40;    //选择SPI1模式
	US1CON0 = 0x3F;   //设置SPI1为主设备，SCK空闲时间为高电平，SCK周期第二沿采集数据，时钟速率为Fsys/128
	US1CON1 = 0x01;   //MSB优先发送，8位传输，允许发送中断
	US1CON0 |= 0x80;  //开启SPI1
	IE2 |= 0x01;
	EA = 1;
}


/*****************************************************
*函数名称：void TWI1/PI/UART1_Int() interrupt 7
*函数功能：SSI1中断函数
*入口参数：void
*出口参数：void
*****************************************************/
#if (SSI1_Mode == Uart1)
void Uart1_Int() interrupt 15   //Uart1中断函数
{
	if(US1CON0&0x02)    //发送标志位判断
	{
		US1CON0 &= 0xFE;
		Uart1SendFlag = 1;
	}
	if((US1CON0&0x01))  //接收标志位判断
	{
		US1CON0 &= 0xFD;
		Uart1ReceiveFlag = 1;
	}	
}
#endif

#if (SSI1_Mode == TWI1)
void TWI1_Int() interrupt 15     //TWI1中断函数
{
	if(US1CON0&0x40)
	{
		US1CON0 &= 0xbf;  //中断清零
		TWI1Flag = 1;
	}	
}
#endif 

#if (SSI1_Mode == SPI1)
void Spi1_Int(void) interrupt 15    //SPI1中断函数
{	  
	if(US1CON1&0X08)    //发送缓存器空标志判断
	{
		US1CON1 &= ~0X08;
	}
	if(US1CON1&0X80)    //数据传输标志位判断
	{
		US1CON1 &= ~0X80;
		SPI1Flag = 1;
	}
}
#endif 