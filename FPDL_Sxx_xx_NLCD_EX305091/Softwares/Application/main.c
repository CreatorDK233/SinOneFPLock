#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "IO.h"
#include "LCD_HFG12864.h"
#include <string.h>
#include "RTC_PCF8563.h"
#include "RTC.h"
#include "adc.h"
#include "BeepMgr.h"
#include "Key_SinOne.h"
#include "MFC_WS1850.h"
#include "yc_nfc_contactless_l1.h"
#include "TuyaWIFI.h"
#include "GUI_Function.h"
#include "Basic_Function.h"
#include "Flash.h"
#include "FingerPrint.h"
#include "LEDsMgr.h"
#include "Motor.h"
#include "Battery.h"
#include "HallSlide.h"
#include "Log.h"
#include "LowPower.h"
#include "FP.h"
#include "GUI.h"

/**************************************************************
说明：
***************************************************************/
uint8_t ReadRTC = 0;
uint8_t TextBuff[10]={0x11,0x22,0x33};
extern void test_fun();

//uint8_t system_IC_busy_Flag = 0;
void Main_System_Init(void)
{
	EnableWDT();
	MX_GPIO_Init();
	SET_MFC_RST_H;
	MX_ADC1_Init();
	MX_TIM_Init();
	MX_SPI0_Init();
	MX_UART2_Init();
	
	#ifdef Function_TuyaWifi
	//Wifi_Init();
	//MX_UART1_Init();
	Wifi_Rst();
	#endif
	
	FPcmd_Init();
	MX_I2C_Init();
	SYSPOWER_ON;  
	#ifdef Function_USE_External_RTC
	PCF8563_Init();
	Hardware_DelayMs(10);
	#endif	
	FPMpowerMgr.Status = FPMpowerOn;
	VOICE_Init();
	MFC_Init();
	Hardware_DelayMs(10);
	Touch_Init();
	Hardware_DelayMs(10);
	SystemTime.year   =		0x20;
	SystemTime.month 	= 	1;
	SystemTime.date   = 	1;
	SystemTime.day		= 	3;
	SystemTime.hour    	= 	0;
	SystemTime.minute 	= 	0;
	SystemTime.second 	= 	0;
	#ifdef Function_USE_Internal_RTC
	G_SystemUTCTime = SystemTimeToUTC(SystemTime);
	#elif defined Function_USE_External_RTC
	PCF8563_WriteTime();
	#endif
	GUI_Init();
	
	SafetyMonitorMgr.FpIdentifyFailedTimes = 0;
  SafetyMonitorMgr.CardIdentifyFailedTimes = 0;
  SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0;
	SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes = 0;
	SafetyMonitorMgr.SystemLocked = bFALSE;
	UserIdentifyResultMgr.FlagContinuedOpenEnabled = bFALSE;
	
	/* Configure The Touch*/
	#ifdef Function_EventLog
	LogMgr_Init();
	#endif
	/* Configure The Motor*/
	Hardware_MotorMgr_Init();
	
	CurrentScreen = SCREEN_Initialization;
	InitializationMgr.Status = StartInitialization;
	BatteryMgr.ProtectVoltageTriggerTimes = 0;
	BatteryMgr.LowBatteryProtectionEnabled = bFALSE;
	BatteryMgr.BatteryVoltage = 0;
	VoiceMgr.volume = 20; // default volume
	SET_VOLUME(VoiceMgr.volume);
	
	AntiPryingMgr.AntiPryingTrigger = bFALSE;
	AntiPryingMgr.AntiPryingSignalRelease = bFALSE;	
	#ifdef Function_HallSlide
	HallSlideCover_Init();
	#endif
	#ifdef Function_TuyaWifi
	WifiMgr.PowerState = WIFIPowerClosed;
	#endif
	RefreshSystemSleepTime();
}
void main(void)
{
	uint8_t i;
	uint32_t Awake_key=0;
	
	FLASH_OB_Config();			//v121和中微有差异
	Main_System_Init();
	
	for (i = 0; i < 50; i++) 
	{
			Hardware_Task_Analog();
			HardwareBatteryMgr_Task();
	} //更新50次ADC
	
	#ifdef Function_FPMbreathingLed
	SetFPMbreathingLed(FPMbreathingLed_Bule);
	#endif
	Hardware_MotorMgr_Init(); //电机初始化
	
	while (1)
  {
		if ( G_tflagbits.T1024Hz == 1 )
		{
			G_tflagbits.T1024Hz =0;
		}

		if ( G_tflagbits.T256Hz == 1 )
		{
			G_tflagbits.T256Hz =0;			
		}
		
		if ( G_tflagbits.T64Hz == 1 )
		{
			G_tflagbits.T64Hz = 0;
	
			FPM_Mgr_Task();
			
			#if defined (Function_TuyaWifi)
			Wifi_Mgr_Task();
			#endif

			GUI_Task();
			
			Hardware_MotorMgr_Task();	

			#if (defined ProjectIs_BarLock_S7702) || (defined ProjectIs_BarLock_S4914)
			LEDsMgr_Task();
			#endif

			if ( SystemPowerMgr.SleepDelayTimerCnt > 0 )
			{
        SystemPowerMgr.SleepDelayTimerCnt--;
			}
			
			if (SystemPowerMgr.AwakeTimerCnt < 60)
			{
				SystemPowerMgr.AwakeTimerCnt++;
			}
			#ifdef Function_HallSlide
			HallSleep_Task();
			#endif
			if (( MotorMgr.MotorStatus == IDLE )
				//&&( PINMACRO_KB_IRQ_STATUS != 0x00 )	 
				&&( CurrentScreen != SCREEN_Initialization )
				&&( CurrentScreen != SCREEN_PickLockAlarm )
				&&( CurrentScreen != SCREEN_SystemLocked )
				&&( CurrentScreen != SCREEN_AgingTest )
//				&&(CurrentScreen != SCREEN_SelfTest)
				&&((SystemPowerMgr.SleepDelayTimerCnt == 0x0000)	
					) //casing closed
				)
			{
				System_PowerDown();
				System_Awake();
				CLRWDT();
				RefreshSystemSleepTime();
				SystemPowerMgr.AwakeTimerCnt=0x0000;
			}
		}
		
//		if ( G_tflagbits.T16Hz == 1 )
//		{
//			
//		}
		
		if ( G_tflagbits.T8Hz == 1 )
		{
			G_tflagbits.T8Hz =0;
			CLRWDT();
			#ifdef Function_ScreenDisplay
			//if(system_IC_busy_Flag == 0)
			{
			  for(i=0;i<8;i++)
		    {	
			    Display_Task();
		   	}
		  }
			#endif	
			//Uart3SendStr(TextBuff,3);
		}

		if ( G_tflagbits.T2Hz == 1 )
		{
			G_tflagbits.T2Hz =0;
		}
		
		if ( G_tflagbits.T1Hz == 1 )
		{
			G_tflagbits.T1Hz =0;
			//SystemTimeUpdate();	
			#ifdef Function_TuyaWifi			
			WifiRemoteUnlockCountdown();
			#endif

			//EEPROM_TESTING();
			//MFC_Test();
			//Flash_Test();
			//AppUnlockTest();
			#ifdef Function_USE_Internal_RTC
			SystemTime = UTCToSystemtime(G_SystemUTCTime);
			#elif defined Function_USE_External_RTC
			if( ReadRTC == 0 )
			{
				PCF8563_ReadTime();
			}
			ReadRTC += 1;
			if(ReadRTC >= 5)
			ReadRTC = 0;
			#endif
			DEBUG_MARK;
			
		}
  }
}




