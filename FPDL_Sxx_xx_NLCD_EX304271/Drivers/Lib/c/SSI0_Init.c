#include "Function_Init.H"

//选择三合一模式：
#define  Uart0   0
#define  TWI0    1
#define  SPI0    2

#define  SSI0_Mode  TWI0

void Uart0_Init(uint Freq,unsigned long int baud);
void TWI0_Init(void);
void SPI0_Init(void);

bit Uart0SendFlag = 0;    //Uart0发送中断标志位
bit Uart0ReceiveFlag = 0; //Uart0接收中断标志位
bit SPI0Flag = 0;         //SPI0数据传输完成标志位
bit TWI0Flag = 0;         //TWI0中断标志位

/*****************************************************
*函数名称：void SSI0_Test(void)
*函数功能：SSI0测试
*入口参数：void
*出口参数：void
*****************************************************/
void SSI0_Test(void)
{
#if (SSI0_Mode == Uart0)
	Uart0_Init(32,9600);
	while(1)
	{
		US0CON3 = 0xAA;
		while(!Uart0SendFlag);
		Uart0SendFlag = 0;
	}
#endif
	
#if (SSI0_Mode == TWI0)
	TWI0_Init();
	while(1)
	{
		US0CON1 |= 0x20;    //产生起始条件
		while(!TWI0Flag);
        TWI0Flag = 0;		
		US0CON3 = 0x10;	    //发送地址及写命令
        while(!TWI0Flag);
        TWI0Flag = 0;
        US0CON3 = 0x55;	    //发送数据
        while(!TWI0Flag);
        TWI0Flag = 0;
		Soft_Delay_us(100);
		US0CON1 |= 0x10;
		Soft_Delay_us(100);
	}
#endif

#if (SSI0_Mode == SPI0)
	SPI0_Init();
	while(1)
	{
		US0CON2 = 0xAA;
		while(!SPI0Flag);
		SPI0Flag = 0;
        Soft_Delay_us(1000);
	}
#endif	
}
/*****************************************************
*函数名称：void Uart0_Init(uint Freq,unsigned long int baud)
*函数功能：Uart0中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void Uart0_Init(uint Freq,unsigned long int baud)
{
	P0CON &= 0xCF;    //TX/RX设置为输入带上拉
	P0PH  |= 0x30;
	
	OTCON |= 0x30;    //串行接口SSI0选择Uart0通信
	US0CON0 = 0x50;   //设置通信方式为模式一，允许接收
	US0CON1 = Freq*1000000/baud;   //波特率低位控制
	US0CON2 = (Freq*1000000/baud)>>8;   //波特率高位控制
	IE1 |= 0x01;      //开启SSI0中断
    EA = 1;	
}

/*****************************************************
*函数名称：void TWI0_Init(void)
*函数功能：TWI0初始化
*入口参数：void
*出口参数：void
*****************************************************/
void TWI0_Init(void)
{
	OTCON |= 0x20;   //选择TWI0模式
	US0CON0 = 0x80;  //主模式， 使能应答标志位
	US0CON1 = 0x05;  //---- xxxx  x为时钟速率
	IE1 |= 0x01;
	EA = 1;
}

/*****************************************************
*函数名称：void SPI0_Init(void)
*函数功能：SPI0初始化
*入口参数：void
*出口参数：void
*****************************************************/
void SPI0_Init(void)
{
	OTCON |= 0X10;    //选择SPI0模式
	US0CON0 = 0x3B;   //设置SPI0为主设备，SCK空闲时间为高电平，SCK周期第二沿采集数据，时钟速率为Fsys/8
	US0CON1 = 0x01;   //MSB优先发送，8位传输，允许发送中断
	US0CON0 |= 0x80;  //开启SPI0
	//IE1 |= 0x01;
	EA = 1;
}


/*****************************************************
*函数名称：void TWI0/PI/UART1_Int() interrupt 7
*函数功能：SSI0中断函数
*入口参数：void
*出口参数：void
*****************************************************/
#if (0)
void Uart0_Int() //interrupt 7   //Uart0中断函数
{
	if(US0CON0&0x02)    //发送标志位判断
	{
		US0CON0 &= 0xFE;
		Uart0SendFlag = 1;
	}
	if((US0CON0&0x01))  //接收标志位判断
	{
		US0CON0 &= 0xFD;
		Uart0ReceiveFlag = 1;
	}	
}
#endif

#if (0)
void TWI0_Int() //interrupt 7     //TWI0中断函数
{
	if(US0CON0&0x40)
	{
		US0CON0 &= 0xbf;  //中断清零
		TWI0Flag = 1;
	}	
}
#endif 

#if (0)
void Spi0_Int(void) //interrupt 7    //SPI0中断函数
{	  
	if(US0CON1&0X08)    //发送缓存器空标志判断
	{
		US0CON1 &= ~0X08;
	}
	if(US0CON1&0X80)    //数据传输标志位判断
	{
		US0CON1 &= ~0X80;
		SPI0Flag = 1;
	}
}
#endif 