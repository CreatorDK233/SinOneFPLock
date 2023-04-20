#include "Basic_Function.h"
#include "intrins.H"

void Soft_Delay1us(void)
{
	_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();
	_nop_();_nop_();_nop_();_nop_();_nop_();
}

void Soft_Delay2us(void)
{
	Soft_Delay1us();
	Soft_Delay1us();
}
void Soft_Delay3us(void)
{
	Soft_Delay1us();
	Soft_Delay1us();
	Soft_Delay1us();
}
void Soft_Delay4us(void)
{
	Soft_Delay1us();
	Soft_Delay1us();
	Soft_Delay1us();
	Soft_Delay1us();
}
void Soft_Delay5us(void)
{
	Soft_Delay1us();
	Soft_Delay1us();
	Soft_Delay1us();
	Soft_Delay1us();
	Soft_Delay1us();
}

void Soft_Delay_us(unsigned long time)//不准
{
   while(time--);
}

void Soft_Delay_usS(unsigned int time)//不准
{
	while(time--)
	{
		Soft_Delay_us(3);
	}
}

void Soft_Delay1ms(unsigned int time)
{
	while(time--)
	{
		Soft_Delay_us(585);
	}
}

void  Hardware_DelayMs(uint16_t time)
{
	Soft_Delay1ms(time);
}

void Hardware_DelayX1us(uint16_t time)
{
	while(time--);
}

void  Hardware_DelayX5us(uint16_t time)
{
	while(time--)
	{
	Soft_Delay_us(2);
	}
}