#include "gpio.h"
#include "Project.h"
#include "global_variable.h"
#include "SC95F861xB_C.H"
#include "IO.h"
uint8_t INT1flag = 0x00;

void GPIO_Config(GPIO_Port_type Port,Pin_type Pin,GPIO_Mode_type Mode)
{
	if((Port>=0&&Port<=5&&Pin>=0&&Pin<8)!=1)return;
	switch (Port)
	{
	case 0:
		switch (Mode)
        {
        case GPIO_Mode_OUT_PP:
	        P0CON |=(1<<Pin);
	        P0PH  &=~(1<<Pin);
	        break;
        case GPIO_Mode_INPUT_PU:
            P0CON &=~(1<<Pin);
	        P0PH  |= (1<<Pin);
	        break;
        case GPIO_Mode_INPUT_HZ:
          P0CON &=~(1<<Pin);
	        P0PH  &=~(1<<Pin);
	        break;
        default:
	        break;
        }
		break;
	case 1:
		switch (Mode)
        {
        case GPIO_Mode_OUT_PP:
	        P1CON |=(1<<Pin);
	        P1PH  &=~(1<<Pin);
	        break;
        case GPIO_Mode_INPUT_PU:
          P1CON &=~(1<<Pin);
	        P1PH  |= (1<<Pin);
	        break;
        case GPIO_Mode_INPUT_HZ:
            P1CON &=~(1<<Pin);
	        P1PH  &=~(1<<Pin);
	        break;
        default:
	        break;
        }
		break;
	case 2:
		switch (Mode)
        {
        case GPIO_Mode_OUT_PP:
	        P2CON |=(1<<Pin);
	        P2PH  &=~(1<<Pin);
	        break;
        case GPIO_Mode_INPUT_PU:
            P2CON &=~(1<<Pin);
	        P2PH  |= (1<<Pin);
	        break;
        case GPIO_Mode_INPUT_HZ:
            P2CON &=~(1<<Pin);
	        P2PH  &=~(1<<Pin);
	        break;
        default:
	        break;
        }
		break;
	
	case 3:
		switch (Mode)
        {
        case GPIO_Mode_OUT_PP:
	        P3CON |=(1<<Pin);
	        P3PH  &=~(1<<Pin);
	        break;
        case GPIO_Mode_INPUT_PU:
            P3CON &=~(1<<Pin);
	        P3PH  |= (1<<Pin);
	        break;
        case GPIO_Mode_INPUT_HZ:
            P3CON &=~(1<<Pin);
	        P3PH  &=~(1<<Pin);
	        break;
        default:
	        break;
        }
		break;
	
	case 4:
		switch (Mode)
        {
        case GPIO_Mode_OUT_PP:
	        P4CON |=(1<<Pin);
	        P4PH  &=~(1<<Pin);
	        break;
        case GPIO_Mode_INPUT_PU:
            P4CON &=~(1<<Pin);
	        P4PH  |= (1<<Pin);
	        break;
        case GPIO_Mode_INPUT_HZ:
            P4CON &=~(1<<Pin);
	        P4PH  &=~(1<<Pin);
	        break;
        default:
	        break;
        }
		break;
	
	case 5:
		switch (Mode)
        {
        case GPIO_Mode_OUT_PP:
	        P5CON |=(1<<Pin);
	        P5PH  &=~(1<<Pin);
	        break;
        case GPIO_Mode_INPUT_PU:
          P5CON &=~(1<<Pin);
	        P5PH  |= (1<<Pin);
	        break;
        case GPIO_Mode_INPUT_HZ:
          P5CON &=~(1<<Pin);
	        P5PH  &=~(1<<Pin);
	        break;
        default:
	        break;
        }
		break;

	default:
		break;
	}
}

void MX_GPIO_Init(void)
{
	P0CON = 0x8F;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0xF5;  //设置P1为强推挽模式
	P1PH  = 0x00;
	P2CON = 0xFF;  //设置P2为强推挽模式
	P2PH  = 0x00;
	P3CON = 0xFF;  //设置P3为强推挽模式
	P3PH  = 0x00;
	P4CON = 0xF1;  //设置P4为强推挽模式
	P4PH  = 0x00;
	P5CON = 0xF7;  //设置P5为强推挽模式
	P5PH  = 0x00;
	#if defined ProjectIs_BarLock_S5514 || defined ProjectIs_BarLock_S9201
	//MFC_IRQ
	GPIO_Config(PORT2,PIN0,GPIO_Mode_INPUT_PU);
	//HALLSENSOR INT
	GPIO_Config(PORT4,PIN0,GPIO_Mode_INPUT_HZ);
	#else
	//MFC_IRQ
	GPIO_Config(PORT0,PIN0,GPIO_Mode_INPUT_PU);
	#endif
	//AD检测
	GPIO_Config(PORT3,PIN7,GPIO_Mode_INPUT_HZ);
	//VOICE BUSY
	GPIO_Config(PORT5,PIN3,GPIO_Mode_INPUT_PU);
	//PRYING INT
	GPIO_Config(PORT4,PIN1,GPIO_Mode_INPUT_HZ);
	//RESET KEY INT
	GPIO_Config(PORT4,PIN2,GPIO_Mode_INPUT_HZ);
	//P42(INT12)下降沿中断
	//INT1F |= 0x04;	
	//P41(INT11)上升沿中断
	INT1R |= 0x02;
	IE  |= 0x04;	//外部中断1中断使能
	IP  |= 0X00;	//配置中断优先级
	IP1 |= 0X00;
	EA = 1;
}

void MX_GPIO_DeInit(void)
{
	#if defined ProjectIs_BarLock_S5514 || defined ProjectIs_BarLock_S9201
	P0CON = 0xff;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0xff;  //设置P1为强推挽模式//f5
	P1PH  = 0x00;
	P2CON = 0xce;  //设置P2为强推挽模式
	P2PH  = 0x01;
	P3CON = 0xff;  //设置P3为强推挽模式//3f
	P3PH  = 0x00;
	P4CON = 0xf0;  //设置P4为强推挽模式
	P4PH  = 0x00;
	P5CON = 0xff;  //设置P5为强推挽模式
	P5PH  = 0x00;
	#else
	P0CON = 0xfe;  //设置P0为强推挽模式
	P0PH  = 0x01;
	P1CON = 0xff;  //设置P1为强推挽模式//f5
	P1PH  = 0x00;
	P2CON = 0xcf;  //设置P2为强推挽模式
	P2PH  = 0x00;
	P3CON = 0xff;  //设置P3为强推挽模式//3f
	P3PH  = 0x00;
	P4CON = 0xf1;  //设置P4为强推挽模式
	P4PH  = 0x00;
	P5CON = 0xff;  //设置P5为强推挽模式
	P5PH  = 0x00;
	#endif
	P0=0x00;
	P1=0x00;
	P2=0x00;
	P3=0x00;
	P4=0x00;
	P5=0x00;
}

void Enable_EX_Interrupt(void)
{
	//PRYING INT
	GPIO_Config(PORT4,PIN1,GPIO_Mode_INPUT_HZ);
	//RESET KEY INT
	GPIO_Config(PORT4,PIN2,GPIO_Mode_INPUT_HZ);
	//TOUCH INT
	GPIO_Config(PORT4,PIN3,GPIO_Mode_INPUT_HZ);
	//P42(INT12)下降沿中断
	//INT1F |= 0x04;	
	//P41(INT11)上升沿中断
	INT1R |= 0x02;	
	IE  |= 0x04;	//外部中断1中断使能
	//IE1 |= 0x10;	//TK中断打开
	IP  |= 0X00;	//配置中断优先级
	IP1 |= 0X00;
	EA = 1;
}

void EnableAntiPrying_Interrupt(void)
{
	GPIO_Config(PORT4,PIN1,GPIO_Mode_INPUT_HZ);
	INT1R |= 0x02;
}

void DisableAntiPrying_Interrupt(void)
{
	GPIO_Config(PORT4,PIN1,GPIO_Mode_INPUT_HZ);
	INT1R &= (~0x02);
}

void INT1_RST(void)
{
	INT1flag = 0x00;
}

void INT1Interrupt() interrupt	2
{
	if(PINMACRO_PICKLOCK_STATUS == 1)
	{
	  INT1flag = 0x20; //INT11产生中断
		if( PickAlarmEnableMgr.Enable == bTRUE )
		{
			AntiPryingMgr.AntiPryingSignalRelease = bTRUE;
		}
	}
}


