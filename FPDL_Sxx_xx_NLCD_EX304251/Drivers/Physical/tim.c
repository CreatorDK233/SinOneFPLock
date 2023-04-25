#include "tim.h"
#include "ISR.h"
#include "SC95F861xB_C.H"

TimerCount_t TimerCount;
bit Tim3_Flag = 0; 
#ifdef Function_NFCUsed_YC5018
extern bit NFC_TIM_FLAG;
extern uint16_t NFC_TIM_COUNT;
uint16_t NFC_TIM_COUNT_MS;
#endif

void MX_TIM_Init(void)
{
	TMCON |= 0X01;
	MX_TIM0_Init();
	MX_TIM1_Init();
	//MX_TIM3_Init();
	TimerCount.Timer0Cnt=0;
	TimerCount.Timer1Cnt=0;
	//TimerCount.Timer3Cnt=0;
}

void MX_TIM_DeInit(void)
{
	//T0
	ET0 = 0;//定时器0允许
	TR0 = 0;//打开定时器0
	//T1
	ET1 = 0;//定时器1允许
	TR1 = 0;//打开定时器1
}

void MX_TIM0_Init(void)
{
  //T0设置，频率为1KHZ(主频：32M)
	TMOD |= 0x01;                 //0000 0001;Timer0设置工作方式1
	TL0 = (65536 - 32000)%256;    //时钟为Fsys，则溢出时间=32000*（1/Fsys）;
	TH0 = (65536 - 32000)/256;
	TR0 = 0;
	ET0 = 1;//定时器0允许
	TR0 = 1;//打开定时器0
}

void MX_TIM1_Init(void)
{
  //T1设置，频率为200KHZ(主频：32M)
	TMOD |= 0x20;            //0010 0000;Timer1设置工作方式2
	TL1 = 256 - 160;         //时钟为Fsys，则溢出时间=160*（1/Fsys）;
	TH1 = 256 - 160;
	TR1 = 0;
	IPT1 =1;//设置Timer1中断优先级为高
	ET1 = 1;//定时器1允许
	TR1 = 1;//打开定时器1
}

void MX_TIM3_Init(void)
{
  //T3设置，频率为1KHZ(主频：32M)
  TXINX = 0x03;    //选择定时器T3
	TXMOD = 0x80;    
	TXCON = 0x00;	 //设置为16位重载寄存器
	RCAPXH = (65536-32000)/256;     //时钟为Fsys，则溢出时间=32000*（1/Fsys）;
	RCAPXL = (65536-32000)%256;
	TRX = 0;
	IE1 |= 0x40; //定时器3允许
  IP1 |= 0X40;
	TRX = 1;     //打开定时器3	
}

//1ms
void timer0() interrupt 1
{
  TL0 = (65536 - 32000)%256;			//手动重装载值低8位
	TH0 = (65536 - 32000)/256;			//手动重装载值高8位
//	TimerCount.Timer0Cnt+=1;
//	if(TimerCount.Timer0Cnt>=1000)
//	{
//		TimerCount.Timer0Cnt=0;
//	}
	SysTick_ISR();
	#ifdef Function_NFCUsed_YC5018
	if( NFC_TIM_FLAG )
	{
		NFC_TIM_COUNT_MS += 1;
		if( NFC_TIM_COUNT_MS >= 1000 )
		{
			NFC_TIM_COUNT_MS = 0;
			NFC_TIM_COUNT += 1;
			if( NFC_TIM_COUNT >2000 )
			{
				NFC_TIM_COUNT = 0;
			}
		}
	}
	#endif
}

//60us
void timer1() interrupt 3
{
	TimerCount.Timer1Cnt+=1;
	if(TimerCount.Timer1Cnt>=5)//300us
	{
		TimerCount.Timer1Cnt=0;
		TIM3_ISR();
	}
}

//1ms
void Timer3Int(void) interrupt 13
{		
  TXINX = 0x03;   //选择定时器T3
	TFX = 0;        //溢出清零

	TimerCount.Timer3Cnt+=1;
	if(TimerCount.Timer3Cnt>=1000)
	{
		TimerCount.Timer3Cnt=0;
		Tim3_Flag=1;
	}
}
