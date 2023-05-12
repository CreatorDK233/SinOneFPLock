#include "H/Function_Init.H"
/*****************************************************
*函数名称：void IO_Init(void)
*函数功能：IO初始化
*入口参数：void
*出口参数：void
*****************************************************/
void IO_Init(void)
{
	#if (IC_MODEL == SC95F8617B)  //SC92F8617引脚定义
	P0CON = 0xFF;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0xF5;  //设置P1为强推挽模式
	P1PH  = 0x00;
	P2CON = 0x00;  //设置P2为高阻带上拉模式
	P2PH  = 0xFF;
	P3CON = 0xFF;  //设置P3为强推挽模式
	P3PH  = 0x00;
	P4CON = 0xFF;  //设置P4为强推挽模式
	P4PH  = 0x01;
	P5CON = 0xFF;  //设置P5为强推挽模式
	P5PH  = 0x00;
	#endif
	
	#if (IC_MODEL == SC95F8616B)  //SC92F8616引脚定义
	P0CON = 0xFF;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0x00;  //设置P2为高阻带上拉模式
	P2PH  = 0xFF;
	P3CON = 0xFF;  //设置P3为强推挽模式
	P3PH  = 0x00;
	P4CON = 0xFF;  //设置P4为强推挽模式
	P4PH  = 0x01;
	P5CON = 0xFF;  //设置P5为强推挽模式
	P5PH  = 0x00;
	SC95F8616B_NIO_Init(); //未引出IO口配置
	#endif
    
	#if (IC_MODEL == SC95F8615B)  //SC92F8615引脚定义
	P0CON = 0xFF;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0x00;  //设置P2为高阻带上拉模式
	P2PH  = 0xFF;
	P3CON = 0xFF;  //设置P3为强推挽模式
	P3PH  = 0x00;
	P4CON = 0xFF;  //设置P4为强推挽模式
	P4PH  = 0x01;
	P5CON = 0xFF;  //设置P5为强推挽模式
	P5PH  = 0x00;
	SC95F8615B_NIO_Init(); //未引出IO口配置
	#endif
	
	#if (IC_MODEL == SC95F8613B)  //SC92F8613引脚定义
	P0CON = 0xFF;  //设置P0为强推挽模式
	P0PH  = 0x00;
	P1CON = 0x00;  //设置P1为高阻输入模式
	P1PH  = 0x00;
	P2CON = 0x00;  //设置P2为高阻带上拉模式
	P2PH  = 0xFF;
	P3CON = 0xFF;  //设置P3为强推挽模式
	P3PH  = 0x00;
	P4CON = 0xFF;  //设置P4为强推挽模式
	P4PH  = 0x00;
	SC95F8613B_NIO_Init(); //未引出IO口配置
	#endif
}

