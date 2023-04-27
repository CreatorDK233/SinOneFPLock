#include "adc.h"
#include "SC95F861xB_C.H"

bit Adc_Flag = 0;
extern uint16_t a2d_data[1];

void ADC_Init(uint8_t Channel)
{
	a2d_data[0]=0x0929;
	ADCCON = 0X80|Channel;		//开启ADC，ADC采样频率为2M ,选择Channel位ADC采样口
	if(Channel<8)
	{
		ADCCFG0 = 1<<Channel;   //设置Channel作为采样口
	}
	else
	{
		ADCCFG1 = 1<<(Channel-8);   //设置Channel作为采样口
	}
	ADCCFG2 = 0x10;			//ADC采样时间为三个时钟
	IE |= 0X40;        //开启ADC中断
	EA = 1;
}
void ADC_DeInit(void)
{
	ADCCON &= (~0X80);
	IE &= (~0X40);        
}
uint16_t ADC_Value_Got(void)
{
	uint16_t ADC_Value = 0x0000;
	ADCCON |= 0X40;   //开始ADC转换
	while(!Adc_Flag);	         		 //等待 ADC转换完成;
	Adc_Flag = 0;
	ADC_Value = (ADCVH<<4)+(ADCVL>>4);
	return ADC_Value;
}

//根据硬件的不同 该函数系数也需要重新测算
uint8_t VoltageGet(uint16_t A2D_Data)
{
	return ( A2D_Data*2 + 467 )/100;
}

/* ADC1 init function */
void MX_ADC1_Init(void)
{
	ADC_Init(AIN11);
}

void ADC_Interrupt(void) interrupt 6
{
	ADCCON &= ~(0X20);  //清中断标志位
	Adc_Flag = 1;
}
