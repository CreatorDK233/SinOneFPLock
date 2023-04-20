#ifndef IO_H
#define IO_H

/* ************************ FILE HEADER *************************************/
/* ***************************************************************************
*  INCLUDE FILES
*/
#include "gpio.h"
#include "Project.h"
/* MASS MICROELECTRONICS TARGET BOARD I/O SETTINGS */
#if 1

#define SET_LCD_DS_H 		P03=1 
#define SET_LCD_DS_L 		P03=0 

#define SET_LCD_CS_H 		P01=1 
#define SET_LCD_CS_L 		P01=0 

#define SET_LCD_RST_H 		P02=1 
#define SET_LCD_RST_L 		P02=0 

#if defined ProjectIs_BarLock_S5514 || defined ProjectIs_BarLock_S9201
	#define SET_MFC_RST_H			GPIO_Config(PORT0,PIN0,GPIO_Mode_OUT_PP);P00=1 
	#define SET_MFC_RST_L			P00=0 
	#define PINMACRO_NFC_IRQ_STATUS 		P20

	#define SET_POWER_ON				P36=1 
	#define SET_FPMPOWER_OFF			P36=0 

	#define SYSPOWER_ON				P36=1 
	#define SYSPOWER_OFF			P36=0 

	#define PINMACRO_HALLSENSOR_STATUS 	P40
#else
	#define SET_MFC_RST_H			GPIO_Config(PORT2,PIN0,GPIO_Mode_OUT_PP);P20=1 
	#define SET_MFC_RST_L			P20=0 
	#define PINMACRO_NFC_IRQ_STATUS 		P00

	#define SET_POWER_ON				P40=1 
	#define SET_FPMPOWER_OFF			P40=0 

	#define SYSPOWER_ON				P40=1 
	#define SYSPOWER_OFF			P40=0 
#endif

#define SET_MFC_CS_L	
#define SET_MFC_CS_H	

#define SET_FLASH_CS_H	
#define SET_FLASH_CS_L	

#define SET_OLEDPOWER_ON
#define SET_OLEDPOWER_OFF		


#define PINMACRO_KB_IRQ_STATUS		

#define PINMACRO_ONBOARD_BUTTON_STATUS 	P42
#define PINMACRO_CASINGOPEN_STATUS		

#define PINMACRO_FPM_TOUCH_STATUS 	P43

#define SET_VOICEDATA_H 				P54=1 
#define SET_VOICEDATA_L 				P54=0 
#define STATUS_PINMACRO_VOICEDATA 		P54
#define STATUS_PINMACRO_VOICEBUSY 		P53

#define SET_VOICEDATA_SLEEP 				GPIO_Config(PORT5,PIN4,GPIO_Mode_INPUT_PU);
#define SET_VOICEBUSY_SLEEP 				GPIO_Config(PORT5,PIN3,GPIO_Mode_INPUT_PU);

#define SET_VOICEDATA2_H 							
#define SET_VOICEDATA2_L 							
#define STATUS_PINMACRO_VOICEDATA2 	

#define SET_LOGLED_ON					//
#define SET_LOGLED_OFF				//
#define SET_LEDKEYs_ON				
#define SET_LEDKEYs_OFF				P21=1;P22=1;P23=1
#define SET_LEDRGB_R_ON				P21=0 
#define SET_LEDRGB_R_OFF			P21=1 
#define SET_LEDRGB_G_ON				P22=0 
#define SET_LEDRGB_G_OFF			P22=1 
#define SET_LEDRGB_B_ON				P23=0 
#define SET_LEDRGB_B_OFF			P23=1 

#define PINMACRO_PICKLOCK_STATUS 		P41

#ifdef Function_TuyaWifi
#define MotorIOChange
#define SET_WIFIPOWER_ON			P55=1
#define SET_WIFIPOWER_OFF			P55=0
#endif

#ifdef MotorIOChange

#ifdef ProjectIs_BarLock_S0607
#define SET_MOTOR_INA_H				P46=1 
#define SET_MOTOR_INA_L				P46=0 
#define SET_MOTOR_INB_H				P47=1 
#define SET_MOTOR_INB_L				P47=0
#else
#define SET_MOTOR_INA_H				P47=1 
#define SET_MOTOR_INA_L				P47=0 
#define SET_MOTOR_INB_H				P46=1 
#define SET_MOTOR_INB_L				P46=0
#endif

#else

#define SET_MOTOR_INA_H				P51=1 
#define SET_MOTOR_INA_L				P51=0 
#define SET_MOTOR_INB_H				P50=1 
#define SET_MOTOR_INB_L				P50=0

#endif

#endif

#endif /* #ifndef IO_H */
/* ****************************** END OF FILE ****************************** */
