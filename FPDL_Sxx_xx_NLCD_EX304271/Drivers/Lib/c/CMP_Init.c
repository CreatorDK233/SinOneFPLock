#include "H/Function_Init.H" 

void CMP_Init(uchar CMPIS, uchar CMPRF);

/*************模拟比较器负端比较电压选择****************/
enum CMPRF {CMPRF=0,CMPRF1,CMPRF2,CMPRF3,CMPRF4,CMPRF5,CMPRF6,CMPRF7,CMPRF8,CMPRF9,CMPRF10,CMPRF11,CMPRF12,CMPRF13,CMPRF14,CMPRF15};

/*************模拟比较器正端输入通道选择****************/
enum CMPIS {CMP0=0,CMP1,CMP2,CMP3};

bit CMPSTA = 0;

/*****************************************************
*函数名称：void CPM_Test(void)
*函数功能：模拟比较器测试
*入口参数：void
*出口参数：void
*****************************************************/
void CMP_Test(void)
{
	CMP_Init(CMP1,CMPRF8);  //选择正端通道1，负端电压选择1/16VDD
	while(1)
	{
        P04 = ~P04;
		if(CMPCON&0x20)     //判断模拟器输出状态
		{
			CMPSTA = 1;     //V+ > V-
		}
		else
		{
			CMPSTA = 0;     //V- > V+
		}
	}
}

/*****************************************************
*函数名称：void CMP_Init(uchar CMPIS, uchar CMPRF)
*函数功能：模拟比较器初始化
*入口参数：CMPIS=正端通道，CMPRF=负端电压
*出口参数：void
*****************************************************/
void CMP_Init(uchar CMPIS, uchar CMPRF)
{
	CMPCON = 0x80;   //开启模拟比较器电源
	CMPCON |= CMPRF; //选择负端电压
	CMPCFG = 0x04;   //上升沿中断
	CMPCFG |= CMPIS; //正端输入通道选择	
    
    IE1 |= 0x20;
    EA = 1;
}

void CMP_INTERRUPT() interrupt 12
{
	CMPCON &= ~0x40;	//清中断标志位
    P05 = ~P05;
}