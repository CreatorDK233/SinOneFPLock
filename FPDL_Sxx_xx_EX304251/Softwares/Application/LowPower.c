#include "LowPower.h"
#include "SensorMethod.h"
#include "Project.h"
#include "IO.h"
#include "global_variable.h"
#include "Basic_Function.h"
#include "HallSlide.h"
/*以下为休眠唤醒函数所需头文件*/
#include "spi.h"
#include "adc.h"
#include "KeyScan.h"
#include "GUI.h"
#include "i2c.h"
#include "Fingerprint.h"
#include "MFC_WS1850.h"
#include "BeepMgr.h"
#include "tim.h"
#include "usart.h"
#include "RTC_PCF8563.h"
#include "LCD.h"
#include "Battery.h"

extern bool_t IfSystemIsInFactoryDefaultStatus(void);

extern bit Touch_WakeUpFlag;
#ifdef Function_HallSlideCover
extern 	bit SleepingHallState; 	//休眠时读到霍尔io口的电位值
extern bit HallSleepFlag;				//霍尔休眠标志
#endif

#ifdef Function_USE_Internal_RTC
unsigned	char  SecondCount = 0;
#endif

static uint8_t LastWakeUpKeyValue = 0xff;
bit 		KeyJustWakeUp = 0;

/**************************************************
*函数名称：void IntoLowPowerMode_Init(void)
*函数功能：进入低功耗模式前的的初始化
*入口参数：void
*出口参数：void  
*备注说明：在进入低功耗前，对部分数据进行处理 
**************************************************/
void IntoLowPowerMode_Init(void)
{
    /*进入低功耗前设置*/
	#ifdef Function_HallSlideCover
	SleepingHallState = PINMACRO_HALLSENSOR_STATUS;
	HallSleepFlag = 0;
	#endif
}

/**************************************************
*函数名称：void MX_QuitLowPowerMode(void)
*函数功能退出低功耗模式
*入口参数：void
*出口参数：void  
**************************************************/
void MX_QuitLowPowerMode(void)
{
	TouchKey_QuitLowPowerMode();       
	Touch_WakeUpFlag = 1;
	INT1flag=0x00;
}

/**************************************************
*函数名称：void AntiMisAwakeup(uint8_t KeyValue)
*函数功能防止偶然干扰造成自动唤醒
*入口参数：uint8_t KeyValue
*出口参数：void  
**************************************************/
void AntiMisAwakeup(uint8_t KeyValue)
{
	//uint8_t i=0;
	if(LastWakeUpKeyValue==KeyValue)
	{
		#ifdef Function_HallSlideCover
		if( PINMACRO_HALLSENSOR_STATUS == 0 )
		{
			MX_QuitLowPowerMode();
			KeyJustWakeUp = 1;
			LastWakeUpKeyValue = 0xff;
		}
		#else
		MX_QuitLowPowerMode();
		KeyJustWakeUp = 1;
		LastWakeUpKeyValue = 0xff;
		#endif
	}
	else{
//		LastWakeUpKeyValue = KeyValue;
//		LastWakeUpKeyValue = 0;
//		i++;
		LastWakeUpKeyValue = KeyValue;
	}
//			MX_QuitLowPowerMode();
//			KeyJustWakeUp = 1;
}

/**************************************************
*函数名称：void LowPowerAwakeupDispose(void)
*函数功能低功耗唤醒检测（不带触摸按键检测）
*入口参数：void
*出口参数：void  
**************************************************/
void LowPowerAwakeupDispose(void)
{
	if( PickAlarmEnableMgr.Enable == bTRUE )
	{
		if(( PINMACRO_PICKLOCK_STATUS == 1 )&&( AntiPryingMgr.AntiPryingSignalRelease == bTRUE ))
		{
			MX_QuitLowPowerMode();
		}
	}
	if(( PINMACRO_FPM_TOUCH_STATUS == 1 )||( PINMACRO_ONBOARD_BUTTON_STATUS == 0 ))
	{
		MX_QuitLowPowerMode();
	}
	#ifdef Function_HallSlideCover
	HallAwakeupDispose();
	#endif
	if( INT1flag != 0 )
	{
		MX_QuitLowPowerMode();
	}
}

/**************************************************
*函数名称：void System_PowerDown(void)
*函数功能系统休眠开始
*入口参数：void
*出口参数：void  
**************************************************/
void System_PowerDown(void)
{
  uint8_t i;
	
	static uint16_t FPtimeout_times,FP_SleepFailedTimes;
	
	CLRWDT();
	DEBUG_MARK;
	
	#ifdef Function_ScreenDisplay
	Clear_Screen();
	#endif
	
	#ifndef DEBUG_MODE
	SET_LOGLED_OFF;
	#endif
	
	ADC_DeInit();
	
	MX_TIM_DeInit();

	SPI0_DeInit();

	MFC_POWERDOWN();	

	VoicePlayerPowerDown();

	CLRWDT();

	FPtimeout_times=0;
	FP_SleepFailedTimes=0;
	while(1)
	{
		FpmAckMgr.Status = WaitACK;
		FPM_SendSleepCmd();
		for (i=0;i<25;i++)
		{
			CLRWDT();
			Hardware_DelayMs(20);
			FPM_Mgr_Task();
			if ( FpmAckMgr.Status == GotACK )
			{
				if (  ( FpmAckMgr.ErrorCode == Error_NONE)
					&&(PINMACRO_FPM_TOUCH_STATUS == 0 )
					)
				{
					FPMpowerMgr.Status = FPMpowerDown;	
					DEBUG_MARK;
					break;
				}
				else
				{
					FP_SleepFailedTimes++;
					DEBUG_MARK;
					
					if (FP_SleepFailedTimes>350)		//FP not released more than 10s
					{
						FPMpowerMgr.Status = FPMpowerDown;	
					}
					break;
				}
				
			}
			else
			{
				if (i>23)		//time out,FP failed
				{
					FPtimeout_times++;
					if (FPtimeout_times>2)
					{
						FPMpowerMgr.Status = FPMpowerDown;	
						break;
					}
				}
			}
		}
		CLRWDT();
		if ( FPMpowerMgr.Status == FPMpowerDown)
		{
			DEBUG_MARK;
			MX_UART2_DeInit();
			SET_FPMPOWER_OFF;	
			Hardware_DelayMs(3);//Wait for Power line lost power
			break;
		}
	}
	#ifdef ProjectIs_BarLock_S6902
		LEDsCtrlSwitch = 0;
	#endif
		//wait for user release touch button
	if ( SystemPowerMgr.SleepSource == UserForced )
	{
		for (i=0;i<10;i++)
		{
			CLRWDT();
			Hardware_DelayMs(1);
		}
	}

	MX_GPIO_DeInit();
	
	Enable_EX_Interrupt();
	
	IntoLowPowerMode_Init();

	TouchKey_IntoLowPowerMode();
	i=0;
	while(i!=1)
	{
		if(GetLowPowerScanFlag() == 0)	//正常运行模式 
		{
			i=1;
		}
		else						//低功耗运行模式
		{
			LowPower_Touchkey_Scan();
		}	
	}
}

/**************************************************
*函数名称：void System_Awake(void)
*函数功能系统唤醒开始
*入口参数：void
*出口参数：void  
**************************************************/
void System_Awake(void)
{
	uint8_t i;
	
	CLRWDT();

	Hardware_DelayMs(1);		//wait for system clock stable//
	
	MX_GPIO_Init();
	
	SET_MFC_RST_H;
	
	MX_ADC1_Init();
	
	MX_TIM_Init();
	
	MX_SPI0_Init();
	
	MX_I2C_Init();
	
	#ifndef DEBUG_MODE
	SET_LOGLED_ON;
	#endif
	
	#if (defined ProjectIs_BarLock_S6902)
	SET_LEDKEYs_OFF;
	#endif
	
	MX_UART2_Init();
	
	SET_POWER_ON;        
	FPMpowerMgr.Status = FPMpowerOn;
	Hardware_DelayMs(5);		//wait for OLED Driver power on reset
	
	MFC_Init();
	Hardware_DelayMs(5);	
	
	TouchKeyRestart();
	Key_Init();
	
	GUI_Init();	

	SET_VOLUME(VoiceMgr.volume);

	Hardware_DelayMs(1);
	
	for (i=0;i<100;i++)
	{
		if( i%10 == 0 )CLRWDT();
		Hardware_Task_Analog();
		HardwareBatteryMgr_Task();
	}
	if (( BatteryMgr.BatteryLevel == LEVEL_1 )||( BatteryMgr.BatteryLevel == LEVEL_0 ))
	{
		PLAY_VOICE_ONESEGMENT(VOICE_PleaseReplaceTheBattery);
		BatteryMgr.TimeCnt = Def_MessageBoxTimeDelay;
		CurrentScreen = SCREEN_LowBattery;		
	}
	#ifdef INSIDEBUTTONINTOMAINMENU
	else if (SystemPowerMgr.AwakeSource == SystemResetKey)		
	{
		if ( PINMACRO_ONBOARD_BUTTON_STATUS != 0 )
		{
			//TouchKeyStatus.KEY_INSIDEBUTTON_Pressed = bTRUE;
			//SystemPowerMgr.AwakeSource = DoNotCare;	
			CurrentScreen = SCREEN_ManagerIdentify;
			ManagerIdentifyMgr.Status = StartManagerIdentify;
		}

	}	
	#endif
	else
	{
		CurrentScreen = SCREEN_Main;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		FpIdentifyMgr.Status = FPMcmdStart;
		CardIdentifyMgr.Status = ReadingCardID;
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			PLAY_VOICE_ONESEGMENT(VOICE_PleaseAddMasterFirst);
		}
		else
		{
			if (  (SystemPowerMgr.AwakeSource == CasingOpen)
				||(SystemPowerMgr.AwakeSource == KeyBoardTouch)
				)
			{
				PLAY_VOICE_ONESEGMENT_FIXED(VOICE_POWERON);	
			}
		}
	}
	
	SystemPowerMgr.SleepSource = SleepTimer;

#ifdef Function_FPMbreathingLed
	for(i=0;i<25;i++)
	{
		CLRWDT();
		FpmAckMgr.Status = WaitACK;
		FPM_SetBreathingLED(1,1,1,255);		//Blue LED breathing
		Hardware_DelayMs(20);
		FPM_Mgr_Task();
		if ( FpmAckMgr.Status == GotACK )
		{
			break;
		}
	}
#endif
	
}

/**************************************************
*函数名称：void LowPowerTimer_ISR(void)
*函数功能低功耗模式下定时器中断服务函数（125ms调用一次）
*入口参数：void
*出口参数：void  
**************************************************/
void LowPowerTimer_ISR(void) 
{
	#ifdef Function_USE_Internal_RTC
	SecondCount += 1;
	if( SecondCount >= 8 )
	{
		SecondCount = 0;
		G_SystemUTCTime += 1;
	}
	#endif
}



