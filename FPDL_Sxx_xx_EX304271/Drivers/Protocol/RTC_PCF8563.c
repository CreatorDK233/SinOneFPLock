#include "global_variable.h"
#include "I2cBus.h"
#include <string.h>
#include "Project.h"
#include "Basic_Function.h"

#define PCF8653_CONFIG_REG0 	0x08
#define PCF8653_CONFIG_REG1	  0x00
#define PCF8653_CONFIG_REG2	  0x00
#define PCF8653_CONFIG_REG3	  0x00
#define PCF8653_CONFIG_REG4	  0x00
#define PCF8653_CONFIG_REG5	  0x00
#define PCF8653_CONFIG_REG6	  0x00
#define PCF8653_CONFIG_REG7 	0x00
#define PCF8653_CONFIG_REG8 	0x18
#define PCF8653_CONFIG_REG9 	0x80
#define PCF8653_CONFIG_REG10 	0x80
#define PCF8653_CONFIG_REG11 	0x80
#define PCF8653_CONFIG_REG12 	0x80
#define PCF8653_CONFIG_REG13 	0x00
#define PCF8653_CONFIG_REG14 	0x00
#define PCF8653_CONFIG_REG15 	0x00

void PCF8563_Init(void)
{
	#ifndef Function_USE_Internal_RTC
	uint8_t	PCF8653_CONFIG_REG_BUFF[]=
	{
		PCF8653_CONFIG_REG0,PCF8653_CONFIG_REG1,PCF8653_CONFIG_REG9,PCF8653_CONFIG_REG10,
		PCF8653_CONFIG_REG11,PCF8653_CONFIG_REG12,PCF8653_CONFIG_REG13
	};
	I2C_WriteSequential(I2CADDR_RTC,0x00,PCF8653_CONFIG_REG_BUFF,7);

	Hardware_DelayMs(10);
	#endif
}

void PCF8563_WriteTime(void)
{
	#ifndef Function_USE_Internal_RTC
	uint8_t BUFF[7]={0};

	BUFF[0]= SystemTime.second;	
	BUFF[1]= SystemTime.minute;	
	BUFF[2]= SystemTime.hour;
	BUFF[3]= SystemTime.date;
	BUFF[4]= SystemTime.day;
	BUFF[5]= SystemTime.month;
	BUFF[6]= SystemTime.year;

	I2C_WriteSequential(I2CADDR_RTC,0x02,&BUFF[0],7);
	#endif
}

void PCF8563_ReadTime(void)
{
	#ifndef Function_USE_Internal_RTC
	uint8_t BUFF[7]={0};

	I2C_ReadSequential(I2CADDR_RTC,0x02,&BUFF[0],7);

	//DEBUG_MARK;
	if( (BUFF[0]&0x7F) >= 0x60 || (BUFF[0]&0x0F)>0x09 ||
			(BUFF[1]&0x7F) >= 0x60 || (BUFF[1]&0x0F)>0x09 ||
			(BUFF[2]&0x3F) >= 0x24 || (BUFF[2]&0x0F)>0x09 ||
			(BUFF[3]&0x3F) >  0x31 || (BUFF[3]&0x0F)>0x09 || (BUFF[3]&0x3F) == 0x00 ||
			//(BUFF[4]&0x07) >= 0x08 ||
			(BUFF[5]&0x1F) >  0x12 || (BUFF[5]&0x0F)>0x09	|| (BUFF[5]&0x1F) == 0x00 ||
			 BUFF[6]>0x99 				 || (BUFF[6]&0x0F)>0x09   )
	{
		PCF8563_WriteTime();
		return;
	}
	
	SystemTime.second 	= 	BUFF[0]&0x7F;		//Voltage Low bit is not care
	SystemTime.minute 	= 	BUFF[1]&0x7F;	
	SystemTime.hour    	= 	BUFF[2]&0x3F;	
	SystemTime.date    	= 	BUFF[3]&0x3F;
	SystemTime.day		= 	BUFF[4]&0x07;
	SystemTime.month 	= 	BUFF[5]&0x1F;		//CENTURY BIT is not care 
	SystemTime.year    	=	BUFF[6];
	#endif
	
}

void PCF8563_Test(void)
{
	#if 0
	uint8_t BUFF[7];

	BUFF[0]= 33;	
	BUFF[1]= 22;	
	BUFF[2]= 11;
	BUFF[3]= 4;
	BUFF[4]= 5;
	BUFF[5]= 6;
	BUFF[6]= 7;

	I2C_WriteSequential(I2CADDR_RTC,0x02,&BUFF[0],7);
	BUFF[0]= 0;	
	BUFF[1]= 0;	
	BUFF[2]= 0;
	BUFF[3]= 0;
	BUFF[4]= 0;
	BUFF[5]= 0;
	BUFF[6]= 0;
	
	I2C_ReadSequential(I2CADDR_RTC,0x02,&BUFF[0],7);
	#endif
}
