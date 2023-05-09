#include "Function_Init.H"

//选择三合一模块：
#define  USCI2   2
#define  USCI3   3
#define  USCI4   4
#define  USCI5   5

//选择三合一模式：
#define  UartX   0
#define  TWIX    1
#define  SPIX    2

#define  SSIX_Select USCI3
#define  SSIX_Mode  2

void UARTX_Init(uint Freq, unsigned long int baud);
void TWIX_Init(void);
void SPIX_Init(void);

bit UartXSendFlag = 0;    //UartX发送中断标志位
bit UartXReceiveFlag = 0; //UartX接收中断标志位

bit SPIXFlag = 0;         //SPIX数据传输完成标志位
bit TWIXFlag = 0;         //TWIX中断标志位

uint8_t SSIxx_Select = 2;
uint8_t SSIxx_Mode = 0;

/*****************************************************
*函数名称：void SSIX_Test(void)
*函数功能：SSIX测试
*入口参数：void
*出口参数：void
*****************************************************/
void SSIX_Test(void)
{
  USXINX = SSIX_Select;       //USCIX寄存器指向对应的USCI

//判断三合一模式
#if (SSIX_Mode == UartX)
  {
    UARTX_Init(32, 9600);      //初始化串口，输入主频和波特兰
    while(1)
    {
      USXCON3 = 0xAA;         //发送0xAA
      while(!UartXSendFlag);        //等待发送完成标志位置起
      UartXSendFlag = 0;      //清除发送标志位
    }
  }
#elif (SSIX_Mode == TWIX)
  {
    TWIX_Init();
    while(1)
    {
      USXCON1 |= 0x20;    //产生起始条件
      while(!TWIXFlag);
      TWIXFlag = 0;
      USXCON3 = 0x10;	    //发送地址及写命令
      while(!TWIXFlag);
      TWIXFlag = 0;
      USXCON3 = 0x55;
      while(!TWIXFlag);
      TWIXFlag = 0;
      Soft_Delay_us(100);
      USXCON1 |= 0x10;        //发送结束信号
      Soft_Delay_us(100);
    }
  }
#elif (SSIX_Mode == SPIX)
  {
    SPIX_Init();
    while(1)
    {
      USXCON2 = 0xAA;
      while(!SPIXFlag);
      SPIXFlag = 0;
      Soft_Delay_us(1000);
    }
  }
#endif

}

/*****************************************************
*函数名称：void UARTX_Init(uint Freq,unsigned long int baud)
*函数功能：UartX中断初始化
*入口参数：Freq-主频，baud-波特率
*出口参数：void
*****************************************************/
void UARTX_Init(uint Freq, unsigned long int baud)
{
	TMCON &= 0x3F;
  TMCON |= 0xC0;    //串行接口SSIX选择Uart通信
	if (SSIxx_Select == USCI2)
	{
  P4CON &= ~0x30;
  P4PH |= 0x30;
  IE2 |= 0x02;      //开启中断
	}
	else if (SSIxx_Select == USCI3)
	{
  P2CON &= ~0xC0;
  P2PH |= 0xC0;
  IE2 |= 0x04;      //开启中断
	}
	else if (SSIxx_Select == USCI4)
	{
  P4CON &= ~0xC0;
  P4PH |= 0xC0;
  IE2 |= 0x08;     //开启中断
	}
	else if (SSIxx_Select == USCI5)
	{
  P0CON &= ~0x0C;
  P0PH |= 0x0C;
  IE2 |= 0x10;     //开启中断
	}

  USXCON0 = 0x50;   //设置通信方式为模式一，允许接收
  USXCON1 = Freq * 1000000 / baud; //波特率低位控制
  USXCON2 = (Freq * 1000000 / baud) >> 8; //波特率高位控制

  EA = 1;
}

/*****************************************************
*函数名称：void TWIX_Init(void)
*函数功能：TWIX初始化
*入口参数：void
*出口参数：void
*****************************************************/
void TWIX_Init(void)
{
	TMCON &= 0x3F;
  TMCON |= 0x80;    //串行接口SSIX选择TWI通信
	if (SSIxx_Select == USCI2)
	{
		P4CON &= ~0x18;
		P4PH |= 0x18;
		IE2 |= 0x02;      //开启中断
	}
	else if (SSIxx_Select == USCI3)
	{
		P2CON &= ~0x60;
		P2PH |= 0x60;
		IE2 |= 0x04;      //开启中断
	}
	else if (SSIxx_Select == USCI4)
	{
  P5CON &= ~0x20;
  P5PH |= 0x20;
  P4CON &= ~0x70;
  P4PH |= 0x70;
  IE2 |= 0x08;     //开启中断
	}
	else if (SSIxx_Select == USCI5)
	{
  P0CON &= ~0x06;
  P0PH |= 0x06;
  IE2 |= 0x10;     //开启中断
	}

  TMCON |= 0x80;   //选择TWIX模式
  USXCON0 = 0x80;  //主模式， 使能应答标志位
  USXCON1 = 0x05;  //---- xxxx  x为时钟速率
  IE2 |= 1 << (SSIxx_Select - 1);    //开启中断
  EA = 1;
}

/*****************************************************
*函数名称：void SPIX_Init(void)
*函数功能：SPIX初始化
*入口参数：void
*出口参数：void
*****************************************************/
void SPIX_Init(void)
{
	TMCON &= 0x3F;
  TMCON |= 0x40;    //串行接口SSIX选择SPI通信
	if (SSIxx_Select == USCI2)
	{
  P4CON &= ~0x38;
  P4PH |= 0x38;
  IE2 |= 0x02;      //开启中断
	}
	else if (SSIxx_Select == USCI3)
	{
  P2CON &= ~0xE0;
  P2PH |= 0xE0;
  //IE2 |= 0x04;      //开启中断
	}
	else if (SSIxx_Select == USCI4)
	{
  P5CON &= ~0x20;
  P5PH |= 0x20;
  P4CON &= ~0xC0;
  P4PH |= 0xC0;
  IE2 |= 0x08;     //开启中断
	}
	else if (SSIxx_Select == USCI5)
	{
  P0CON &= ~0x0E;
  P0PH |= 0x0E;
  IE2 |= 0x10;     //开启中断
	}

  TMCON |= 0X40;    //选择SPIX模式
  USXCON0 = 0x3B;   //设置SPIX为主设备，SCK空闲时间为高电平，SCK周期第二沿采集数据，时钟速率为Fsys/8
  USXCON1 = 0x01;   //MSB优先发送，8位传输，不允许发送中断
  USXCON0 |= 0x80;  //开启SPIX
  //IE2 |= 1 << (SSIxx_Select - 1);    //开启中断
  EA = 1;
}

#if (0)
/*****************************************************
*函数名称：void TWI2/SPI2/UART2_Int() interrupt 7
*函数功能：SSI2中断函数
*入口参数：void
*出口参数：void
*****************************************************/
#if (SSIX_Mode == UartX)
void UART2_Int() //interrupt 16   //UartX中断函数
{
  if(USXCON0 & 0x02)  //发送标志位判断
  {
    USXCON0 &= 0xFE;
    UartXSendFlag = 1;
  }
  if((USXCON0 & 0x01)) //接收标志位判断
  {
    USXCON0 |= 0xFD;
    UartXReceiveFlag = 1;
  }
}
#elif (SSIX_Mode == TWIX)
void TWI2_Int() //interrupt 16     //TWIX中断函数
{
  if(USXCON0 & 0x40)
  {
    USXCON0 &= 0xbf;  //中断清零
    TWIXFlag = 1;
  }
}
#elif (SSIX_Mode == SPIX)
void SPI2Int(void) //interrupt 16    //SPIX中断函数
{	  
	if(USXCON1&0X08)    //发送缓存器空标志判断
	{
		USXCON1 &= ~0X08;
	}
	if(USXCON1&0X80)    //数据传输标志位判断
	{
		USXCON1 &= ~0X80;
		SPIXFlag = 1;
	}
}
#endif
#elif (0)
/*****************************************************
*函数名称：void TWI3/SPI3/UART3_Int() interrupt 7
*函数功能：SSI2中断函数
*入口参数：void
*出口参数：void
*****************************************************/
#if (SSIX_Mode == UartX)
void UART3_Int() //interrupt 17   //UartX中断函数
{
  if(USXCON0 & 0x02)  //发送标志位判断
  {
    USXCON0 &= 0xFE;
    UartXSendFlag = 1;
  }
  if((USXCON0 & 0x01)) //接收标志位判断
  {
    USXCON0 |= 0xFD;
    UartXReceiveFlag = 1;
  }
}
#elif (SSIX_Mode == TWIX)
void TWI3_Int() //interrupt 17     //TWIX中断函数
{
  if(USXCON0 & 0x40)
  {
    USXCON0 &= 0xbf;  //中断清零
    TWIXFlag = 1;
  }
}
#elif (SSIX_Mode == SPIX)
void SPI3_Int(void) //interrupt 17    //SPIX中断函数
{	  
   if(USXCON1&0X08)    //发送缓存器空标志判断
   {
      USXCON1 &= ~0X08;
   }
   if(USXCON1&0X80)    //数据传输标志位判断
   {
      USXCON1 &= ~0X80;
      SPIXFlag = 1;
   }
}
#endif
#elif (0)
/*****************************************************
*函数名称：void TWI4/SPI4/UART4_Int() interrupt 7
*函数功能：SSI2中断函数
*入口参数：void
*出口参数：void
*****************************************************/
#if (SSIX_Mode == UartX)
void UART4_Int() //interrupt 18   //UartX中断函数
{
  if(USXCON0 & 0x02)  //发送标志位判断
  {
    USXCON0 &= 0xFE;
    UartXSendFlag = 1;
  }
  if((USXCON0 & 0x01)) //接收标志位判断
  {
    USXCON0 |= 0xFD;
    UartXReceiveFlag = 1;
  }
}
#elif (SSIX_Mode == TWIX)
void TWI4_Int() //interrupt 18     //TWIX中断函数
{
  if(USXCON0 & 0x40)
  {
    USXCON0 &= 0xbf;  //中断清零
    TWIXFlag = 1;
  }
}
#elif (SSIX_Mode == SPIX)
void SPI4_Int(void) //interrupt 18   //SPIX中断函数
{	  
	if(USXCON1&0X08)    //发送缓存器空标志判断
	{
		USXCON1 &= ~0X08;
	}
	if(USXCON1&0X80)    //数据传输标志位判断
	{
		USXCON1 &= ~0X80;
		SPIXFlag = 1;
	}
}
#endif

#elif (0)
#if (SSIX_Mode == UartX)
void UART5_Int() //interrupt 19   //UartX中断函数
{
  if(USXCON0 & 0x02)  //发送标志位判断
  {
    USXCON0 &= 0xFE;
    UartXSendFlag = 1;
  }
  if((USXCON0 & 0x01)) //接收标志位判断
  {
    USXCON0 |= 0xFD;
    UartXReceiveFlag = 1;
  }
}
#elif (SSIX_Mode == TWIX)
void TWI5_Int() //interrupt 19     //TWIX中断函数
{
  if(USXCON0 & 0x40)
  {
    USXCON0 &= 0xbf;  //中断清零
    TWIXFlag = 1;
  }
}
#elif (SSIX_Mode == SPIX)
void SPI5_Int(void) //interrupt 19   //SPIX中断函数
{	  
	if(USXCON1&0X08)    //发送缓存器空标志判断
	{
		USXCON1 &= ~0X08;
	}
	if(USXCON1&0X80)    //数据传输标志位判断
	{
		USXCON1 &= ~0X80;
		SPIXFlag = 1;
	}
}
#endif

#endif
