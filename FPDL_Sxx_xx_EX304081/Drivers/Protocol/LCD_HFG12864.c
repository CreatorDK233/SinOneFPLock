#include "IO.h"
#include "spi.h"
#include "Project.h"
#include "Basic_Function.h"

uint8_t G_DisRAM[1024]={0};

#ifdef Function_DisplayUsed_HFG12864 

static uint8_t SwitchPageNumber;

void WriteComm(uint8_t cmd)
{
	SET_LCD_DS_L;
	Soft_Delay_usS(1);
	SET_LCD_CS_L;
	Soft_Delay_usS(1);
	SPI0SendByte(cmd);
	Soft_Delay_usS(1);
	SET_LCD_CS_H;	
	//__asm ("nop");__asm ("nop");__asm ("nop");__asm ("nop");__asm ("nop");
}

void Display_Init(void)
{
	uint8_t i;
	
	SET_LCD_RST_H;  
	Soft_Delay1ms(1);
	
	SET_LCD_RST_L;
	Soft_Delay1ms(1);

	SET_LCD_RST_H;
	Soft_Delay1ms(1);
	
//	SET_LCD_DS_L;
//	Soft_Delay1ms(1);
//	
//	SET_LCD_CS_L;
//	Soft_Delay1ms(1);
 
	WriteComm(0xe2); //reset
	WriteComm(0xa3); //LCD BIAS=1/9行写入数据地址加1，但读入不加

#if defined ProjectIs_BarLock_S0608 || defined ProjectIs_BarLock_S5514 \
 || defined ProjectIs_BarLock_S9201 || defined ProjectIs_BarLock_S3007
	WriteComm(0xa0); //0xA0 or 0xA1, 列地址移动方向:ADC=0(SEG1->SEG132)
	WriteComm(0xc0); //0xC0 or 0xC8, 行扫描方向:SHL=1(COM64->COM1)
#else
	WriteComm(0xa1); //0xA0 or 0xA1, 列地址移动方向:ADC=0(SEG1->SEG132)
	WriteComm(0xc8); //0xC0 or 0xC8, 行扫描方向:SHL=1(COM64->COM1)
#endif

	
	WriteComm(0x2c); //VC ON
	Soft_Delay1ms(1);
	WriteComm(0x2e); //VR ON
	Soft_Delay1ms(1);
	
	WriteComm(0x2f); //电源控制模式：VC,VR,VF=111
	Soft_Delay1ms(1);
	
	WriteComm(0x81); //对比度寄存器模式
	//Hardware_DelayMs(1);
	WriteComm(0x20); //对比度调整
	Soft_Delay1ms(1);
	
	WriteComm(0x23); //设置电阻分压
	Soft_Delay1ms(1);


	for (i=0;i<8;i++)
	{
		WriteComm(0x10);// set start and end column addr
		#if defined ProjectIs_BarLock_S0608 || defined ProjectIs_BarLock_S5514 \
		 || defined ProjectIs_BarLock_S9201 || defined ProjectIs_BarLock_S3007
		WriteComm(0x01);// set start column addr	
		#else
		WriteComm(0x05);// set start column addr
		#endif		

		WriteComm(0xB0+i);// set start page addr	
		
		SET_LCD_DS_H;
		Soft_Delay_usS(1);
		SET_LCD_CS_L;
		Soft_Delay_usS(1);
		
		SPI0SendStr(&G_DisRAM[128*i],128);
		SET_LCD_CS_H;
	}
	
	//WriteComm(0x10);// set start and end column addr
	//WriteComm(0x00);// set start column addr

	WriteComm(0xaf); //开显示
	Soft_Delay1ms(1);

	SwitchPageNumber = 0x00;
	
}

		 
void Display_Task(void)
{
	//uint8_t i,j,k;
	
	//WriteComm(0x40);// set start Line addr

	if ( SwitchPageNumber >= 7 )
	{
		SwitchPageNumber = 0x00;
	}
	else SwitchPageNumber++;
//
	WriteComm(0x10);// set start and end column addr
	
	#if defined ProjectIs_BarLock_S0608 || defined ProjectIs_BarLock_S5514 \
	 || defined ProjectIs_BarLock_S9201 || defined ProjectIs_BarLock_S3007
	WriteComm(0x00);// set start column addr	
	#else
	WriteComm(0x04);// set start column addr
	#endif	
	
	WriteComm(0xB0+SwitchPageNumber);// set start page addr	
	
	SET_LCD_DS_H;
	Soft_Delay_usS(1);
	SET_LCD_CS_L;
	Soft_Delay_usS(1);

	SPI0SendStr(&G_DisRAM[128*SwitchPageNumber],128);
	SET_LCD_CS_H;
}


#endif

