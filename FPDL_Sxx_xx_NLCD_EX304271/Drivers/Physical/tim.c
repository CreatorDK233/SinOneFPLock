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
	ET0 = 0;//��ʱ��0����
	TR0 = 0;//�򿪶�ʱ��0
	//T1
	ET1 = 0;//��ʱ��1����
	TR1 = 0;//�򿪶�ʱ��1
}

void MX_TIM0_Init(void)
{
  //T0���ã�Ƶ��Ϊ1KHZ(��Ƶ��32M)
	TMOD |= 0x01;                 //0000 0001;Timer0���ù�����ʽ1
	TL0 = (65536 - 32000)%256;    //ʱ��ΪFsys�������ʱ��=32000*��1/Fsys��;
	TH0 = (65536 - 32000)/256;
	TR0 = 0;
	ET0 = 1;//��ʱ��0����
	TR0 = 1;//�򿪶�ʱ��0
}

void MX_TIM1_Init(void)
{
  //T1���ã�Ƶ��Ϊ200KHZ(��Ƶ��32M)
	TMOD |= 0x20;            //0010 0000;Timer1���ù�����ʽ2
	TL1 = 256 - 160;         //ʱ��ΪFsys�������ʱ��=160*��1/Fsys��;
	TH1 = 256 - 160;
	TR1 = 0;
	IPT1 =1;//����Timer1�ж����ȼ�Ϊ��
	ET1 = 1;//��ʱ��1����
	TR1 = 1;//�򿪶�ʱ��1
}

void MX_TIM3_Init(void)
{
  //T3���ã�Ƶ��Ϊ1KHZ(��Ƶ��32M)
  TXINX = 0x03;    //ѡ��ʱ��T3
	TXMOD = 0x80;    
	TXCON = 0x00;	 //����Ϊ16λ���ؼĴ���
	RCAPXH = (65536-32000)/256;     //ʱ��ΪFsys�������ʱ��=32000*��1/Fsys��;
	RCAPXL = (65536-32000)%256;
	TRX = 0;
	IE1 |= 0x40; //��ʱ��3����
  IP1 |= 0X40;
	TRX = 1;     //�򿪶�ʱ��3	
}

//1ms
void timer0() interrupt 1
{
  TL0 = (65536 - 32000)%256;			//�ֶ���װ��ֵ��8λ
	TH0 = (65536 - 32000)/256;			//�ֶ���װ��ֵ��8λ
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
  TXINX = 0x03;   //ѡ��ʱ��T3
	TFX = 0;        //�������

	TimerCount.Timer3Cnt+=1;
	if(TimerCount.Timer3Cnt>=1000)
	{
		TimerCount.Timer3Cnt=0;
		Tim3_Flag=1;
	}
}
