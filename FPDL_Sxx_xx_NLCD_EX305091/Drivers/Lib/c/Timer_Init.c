#include "H/Function_Init.H"

void Timer_Init(void);
uint count = 0;
uint count1=0,count2=0;
uchar flag=0;
/*****************************************************
*函数名称：void Timer_Test(void);
*函数功能：T0/T1/T2测试
*入口参数：void
*出口参数：void
*****************************************************/
void Timer_Test(void)
{
	Timer_Init();
	while(1)
	{
	}
}
/*****************************************************
*函数名称：void Timer_Init(void)
*函数功能：T0/T1/T2初始化
*入口参数：void
*出口参数：void
*****************************************************/
void Timer_Init(void)
{ 
	TMCON = 0X07;    //------111 ;Timer0、Tiemr1和Tiemr2选择时钟Fsys
	
	//T0设置，现象：P02输出频率为1K的方波(主频：32M)
	TMOD |= 0x01;                 //0000 0001;Timer0设置工作方式1
	TL0 = (65536 - 16000)%256;    //时钟为Fsys，则溢出时间=16000*（1/Fsys）;
	TH0 = (65536 - 16000)/256;
	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0
    
	//T1设置，现象为P03输出频率为100K的方波(主频：32M)
	TMOD |= 0x20;            //0010 0000;Timer1设置工作方式2
	TL1 = 256 - 160;         //时钟为Fsys，则溢出时间=160*（1/Fsys）;
	TH1 = 256 - 160;
	TR1 = 0;
	ET1 = 1;//定时器1允许
	TR1 = 1;//打开定时器1
    
	//T2设置，设置为捕获模式，读取变量count可以知道P22第10次到第11次下降沿的定时器计数值
	TXINX = 0x02;   //选择定时器T2
	TXCON = 0X09;   //使能EXT2，16位捕获模式
	TXMOD = 0X80;
	THX = 0X00;
	TLX = 0X00;
	RCAPXH = 0X00;
	RCAPXL = 0X00;
  ET2 = 1;
	TRX = 1;//打开定时器2

	//T3设置，现象：P02输出频率为1K的方波(主频：32M)
  TXINX = 0x03;    //选择定时器T3
	TXMOD = 0x80;    
	TXCON = 0x00;	 //设置为16位重载寄存器
	RCAPXH = (65536-16000)/256;     //时钟为Fsys，则溢出时间=32000*（1/Fsys）;
	RCAPXL = (65536-16000)%256;
	TRX = 0;
	IE1 |= 0x40; //定时器3允许
  IP1 |= 0X40;
	TRX = 1;     //打开定时器3	
    
  //T4设置
  TXINX = 0x04;    //选择定时器T4
	TXMOD = 0x80;    
	TXCON = 0x00;	 //设置为16位重载寄存器
	RCAPXH = (65536-32000)/256;     //时钟为Fsys，则溢出时间=32000*（1/Fsys）;
	RCAPXL = (65536-32000)%256;
	TRX = 0;
	IE1 |= 0x80;//定时器4允许
  IP1 |= 0X80;
	TRX = 1;    //打开定时器4	
    
	EA = 1;	
}

/**************************************************
*函数名称：void timer0/1/2/3/4() interrupt 1/3/5/13/14
*函数功能：定时器中断产生方波
*入口参数：void
*出口参数：void
**************************************************/
#if (0)
void timer0() interrupt 1
{
  TL0 = (65536 - 16000)%256;			//手动重装载值低8位
	TH0 = (65536 - 16000)/256;			//手动重装载值高8位
	P02 = ~P02;			//翻转IO口
}

void timer1() interrupt 3
{
	P03 = ~P03;			//翻转IO口
}

void Timer2Int(void) interrupt 5
{		
  TXINX = 0x02;            //选择定时器T2
	if((TXCON & 0x80))			//是否发生溢出中断
	{
		TXCON &= 0x7F;			//清除溢出中断
		THX = 0;
		TLX = 0;
	}
	
  if((TXCON & 0x40))		//是否发生外部捕获中断
	{
		P07 = ~P07;			//IO翻转
		TXCON &= ~0X40;      //清除标志位
		if(++flag >= 12)     //捕获到第12个电平清零
		{
			flag = 0;

		}
		if(flag == 10)      //捕获到第10个电平，记录计数值1
		{
			count1 = ((uint)(RCAPXH << 8) + RCAPXL);	//捕获到第11个电平，记录计数值2
		}
		if(flag == 11)
		{
			count2 = ((uint)(RCAPXH << 8) + RCAPXL);
			count = count2 - count1;      //计数值2-计数值1=第10个下降沿到第11个下降沿的时间计数值
		}
	}
}

void Timer3Int(void) interrupt 13
{		
    TXINX = 0x03;   //选择定时器T3
		TFX = 0;        //溢出清零
    P05 = ~P05;
}

void Timer4Int(void) interrupt 14
{		
    TXINX = 0x04;   //选择定时器T4
		TFX = 0;        //溢出清零
    P06 = ~P06;
}
#endif
