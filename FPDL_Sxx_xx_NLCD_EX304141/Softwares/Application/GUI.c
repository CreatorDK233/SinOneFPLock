#include "IO.h"
//Protocol layer
#include "RTC_PCF8563.h"
#include "Fingerprint.h"
#include "BeepMgr.h"
#include "LCD_HFG12864.h"
#include "TuyaWIFI.h"
//Logic layer
#include "AppUnlock.h"
#include "KeyScan.h"
#include "Motor.h"
#include "Log.h"
#include "FP.h"
#include "LCD.h"
#include "MFC.h"
#include "RTC.h"
//Application layer
#include "GUI.h"
#include "GUI_Function.h"
#include "Basic_Function.h"
#include "Voice_Menu.h"
#include "Font_Menu.h"
#include "Font.h"

struct
{
	enum{
		SetYearMajor=0,
		SetYearMinor=1,
		SetMonthMajor,
		SetMonthMinor,
		SetDateMajor,
		SetDateMinor,
		SetHourMajor,
		SetHourMinor,
		SetMinuteMajor,
		SetMinuteMinor,
		SetSecondMajor,
		SetSecondMinor
	}Status;

}TimeSettingMgr;

screen_t LastScreen;
VoiceMenuMgr_t VoiceMenuMgr;
DataInputMgr_t DataInputMgr;
PasscodeInputMgr_t PasscodeInputMgr;
uint8_t GUI_TimeCnt;
uint8_t GUI_ToggleFlag_1s;
uint8_t GUI_ToggleFlag_05s;
bool_t GUI_Flag_RefreshLCD;

keycode_t gui_keycode = KEY_NONE;

uint8_t PasscodeBUFF1[12];

/*******************************************************/
/*******************************************************
/*******************************************************/
void ShowIdentifySuccessPage(void)//认证成功页面
{
	#ifdef Function_ScreenDisplay
	uint8_t DoorLockedStr[]={ZF_xiaoyuhao,HZ_yi,HZ_guanbi,HZ_suomen,ZF_dayuhao,HZ_end};
//	uint8_t DoorLockedStrEn[]={"<Closed>"};	
	uint8_t DoorUnlockedStr[]={ZF_xiaoyuhao,HZ_yi,HZ_kai,HZ_suomen,ZF_dayuhao,HZ_end};
//	uint8_t DoorUnlockedStrEn[]={"<Opened>"};	
	uint8_t InitialStatusStr[]={HZ_chuzhong,HZ_shizhong,HZ_zhuang,HZ_tai,HZ_end};
//	uint8_t InitialStatusStrEn[]={"Initial"};
	#endif 	

	if ( MotorMgr.MotorStatus != IDLE )
	{
		#ifdef Function_ScreenDisplay
		DisImage(0,72,52,40,Icon_Unlocked,NormalDisplay);
		
		if ( UserIdentifyResultMgr.IdentifyType == CARD )
		{
			DisHZ16x14Str(2,0,CardUserStr,NormalDisplay);
			DisHZ16x14Str(4,0,UserIDStr,NormalDisplay);
			DisDigital16x8Str(4,40,UserIdentifyResultMgr.CardUserID,NormalDisplay);
		}
		else if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT )
		{
			DisHZ16x14Str(2,0,FpUserStr,NormalDisplay);
			DisHZ16x14Str(4,0,UserIDStr,NormalDisplay);
			DisDigital16x8Str(4,40,UserIdentifyResultMgr.FPUserID,NormalDisplay);
		}
		else if (UserIdentifyResultMgr.IdentifyType == PASSCODE )
		{
			/*if ( UserIdentifyResultMgr.PasscodeType == OnlinePasscode )
			{
					DisHZ16x14Str(2,0,TemporaryPasswordStr,NormalDisplay);
					DisHZ16x14Str(4,0,UserIDStr,NormalDisplay);
			}
			else*/
			{
				DisHZ16x14Str(2,0,PasscodeUserStr,NormalDisplay);
				DisHZ16x14Str(4,0,UserIDStr,NormalDisplay);
			}
			DisDigital16x8Str(4,40,UserIdentifyResultMgr.PasscodeUserID,NormalDisplay);
		}
		else if (UserIdentifyResultMgr.IdentifyType == FINGERPRINTandCARD )
		{
			
			DisHZ16x14Str(0,0,FpUserStr,NormalDisplay);
			DisHZ16x14Str(2,0,UserIDStr,NormalDisplay);
			DisDigital16x8Str(2,36,UserIdentifyResultMgr.FPUserID,NormalDisplay);
			DisHZ16x14Str(4,0,CardUserStr,NormalDisplay);
			DisHZ16x14Str(6,0,UserIDStr,NormalDisplay);
			DisDigital16x8Str(6,40,UserIdentifyResultMgr.CardUserID,NormalDisplay);		
		}
		else if (UserIdentifyResultMgr.IdentifyType == FINGERPRINTandPASSCODE )
		{
			DisHZ16x14Str(0,0,FpUserStr,NormalDisplay);
			DisHZ16x14Str(2,0,UserIDStr,NormalDisplay);
			DisDigital16x8Str(2,40,UserIdentifyResultMgr.FPUserID,NormalDisplay);
			DisHZ16x14Str(4,0,PasscodeUserStr,NormalDisplay);
			DisHZ16x14Str(6,0,UserIDStr,NormalDisplay);
			DisDigital16x8Str(6,40,UserIdentifyResultMgr.PasscodeUserID,NormalDisplay);	
		}
		else if (UserIdentifyResultMgr.IdentifyType == CARDandPASSCODE )
		{
			DisHZ16x14Str(0,0,CardUserStr,NormalDisplay);
			DisHZ16x14Str(2,0,UserIDStr,NormalDisplay);
			DisDigital16x8Str(2,40,UserIdentifyResultMgr.CardUserID,NormalDisplay);
			DisHZ16x14Str(4,0,PasscodeUserStr,NormalDisplay);
			DisHZ16x14Str(6,0,UserIDStr,NormalDisplay);
			DisDigital16x8Str(6,40,UserIdentifyResultMgr.PasscodeUserID,NormalDisplay);	
		}
		else if (UserIdentifyResultMgr.IdentifyType == INITIALSTATUS )
		{
		
				DisHZ16x14Str(3,0,InitialStatusStr,NormalDisplay);
		}
		DisHZ16x14Str(6,64,DoorUnlockedStr,NormalDisplay);
		
		#endif 
		
		if (( MotorMgr.MotorStatus == CWWAIT )&&(MotorMgr.CWWAITTimer==(Def_MotorCWWAITTime-5)) )
		{
			PLAY_VOICE_ONESEGMENT(VOICE_DoorOpened);
			
			#ifdef Function_TuyaWifi
			if( UserIdentifyResultMgr.IdentifyType == WIFITEMPPASSCODE )
			{
				WifiMgr.PostMgr.Flag.Bits.TempPasswordUnlock = 1;
			}
			else if( UserIdentifyResultMgr.IdentifyType != RemoteUnlock )
			{
				WifiMgr.PostMgr.Flag.Bits.Unlock = 1;
			}
			#endif
			
			#ifdef Function_FPMbreathingLed
			//if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT )
			//{
				SetFPMbreathingLed(FPMbreathingLed_Green);
			//}
			#endif	
						
		}
		else if ( MotorMgr.MotorStatus == CCWWAIT ) 
		{
			if ( MotorMgr.CCWWAITTimer==(Def_MotorCCWWAITTime-5) )
			{
				PLAY_VOICE_ONESEGMENT(VOICE_DoorClosed);	
				UserIdentifyResultMgr.TimeCnt = Def_GuiTimeDelayCnt3s;
				SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt3s;
				#ifdef Function_EventLog
				GUI_CreatAndSaveLog(OpenDoor);
				#endif
			}
			else if ( MotorMgr.CCWWAITTimer == 1)
			{
				//EMAGNET_RelaseTrigger();
			}
		}
	}
	else
	{
		#ifdef Function_ScreenDisplay
		GUI_Flag_RefreshLCD = bTRUE;
		DisImage(0,44,40,40,Icon_Locked,NormalDisplay);
    	DisHZ16x14Str(6,36,DoorLockedStr,NormalDisplay);
		#endif
		
		if ( UserIdentifyResultMgr.TimeCnt > 0 )
		{
			if (UserIdentifyResultMgr.TimeCnt == 64)
			{
				SystemPowerMgr.SleepDelayTimerCnt = 65;	
			}
			UserIdentifyResultMgr.TimeCnt--;
		}
		else
		{
			UserIdentifyResultMgr.CardIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.FPIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.PasscodeIdentifyStatus = S_FAIL;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
			CardIdentifyMgr.Status = ReadingCardID;
			FpIdentifyMgr.Status = FPMcmdStart;
			CurrentScreen = SCREEN_Main;	
			SystemPowerMgr.SleepDelayTimerCnt = 0x0000;	//set system to sleep
			
//			#ifdef Function_FPMbreathingLed
//			SetFPMbreathingLed(FPMbreathingLed_Red);
//			#endif	
		}
	}

}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowIdentifyFailPage(void)//认证失败页面
{
	#ifdef Function_ScreenDisplay
	DisImage(1,52,24,24,Icon_Incorrect,NormalDisplay);
	
	if ( UserIdentifyResultMgr.IdentifyType == CARD )
	{
		DisHZ16x14Str(5,36,IdentifyFailStr,NormalDisplay);
	}
	else if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT )
	{
		if ( UserIdentifyResultMgr.ErrorType == FPMserialNumberMismatched )
		{
			DisHZ16x14Str(4,14,FPHardwareMismatchStr,RED_COLOR);
			DisHZ16x14Str(6,20,PLSRestoreFactoryStr,RED_COLOR);
		}
		else
		{
			DisHZ16x14Str(5,36,IdentifyFailStr,NormalDisplay);
		}
	}
	else if ( UserIdentifyResultMgr.IdentifyType == PASSCODE )
	{
		DisHZ16x14Str(5,36,IdentifyFailStr,NormalDisplay);
	}
	#endif
	
	if ( UserIdentifyResultMgr.TimeCnt == Def_MessageBoxTimeDelay )
	{
		#ifdef Function_FPMbreathingLed
		//if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT )
		//{
			SetFPMbreathingLed(FPMbreathingLed_Red);
		//}
		#endif
		#ifdef Function_TuyaWifi
		WifiMgr.PostMgr.Flag.Bits.Alarm = 1;
		if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT )
		{
			WifiMgr.PostMgr.AlarmType = FingerErrorAlarm;
		}
		else if ( UserIdentifyResultMgr.IdentifyType == PASSCODE )
		{
			WifiMgr.PostMgr.AlarmType = PasswordErrorAlarm;
		}
		else if ( UserIdentifyResultMgr.IdentifyType == CARD )
		{
			WifiMgr.PostMgr.AlarmType = CardErrorAlarm;
		}
		#endif
	}

	if ( UserIdentifyResultMgr.TimeCnt > 0 )
	{
		UserIdentifyResultMgr.TimeCnt--;
	}
	else
	{
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		CardIdentifyMgr.Status = ReadingCardID;
		FpIdentifyMgr.Status = FPMcmdStart;
		CurrentScreen = SCREEN_Main;
		RefreshSystemSleepTime();
		
		#ifdef Function_FPMbreathingLed
		SetFPMbreathingLed(FPMbreathingLed_Bule);
		#endif
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void DisplayMainPage(void)//界面_主页面
{
	#ifdef Function_ScreenDisplay
	code uint8_t YearMonthDay[]={ZF_2,ZF_0,ZF_0,ZF_0,ZF_hengan,ZF_0,ZF_1,ZF_hengan,ZF_0,ZF_1,HZ_end};
	//code uint8_t Frame[]={ZF_1,ZF_8,ZF_hengan,ZF_0,ZF_1,ZF_hengan,ZF_0,ZF_1,HZ_end};
	code uint8_t Str1[]={HZ_weihe,HZ_le,HZ_nin,HZ_de,HZ_an,HZ_quan,HZ_end};
//	code uint8_t Str1En[]={"Sys without"};
	code uint8_t Str2[]={HZ_qing,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_end};
//	code uint8_t Str2En[]={"admin."};
	code uint8_t Str3[]={HZ_qing,HZ_shu,HZ_ru,HZ_di,HZ_er,HZ_shen,HZ_fen,HZ_end};
//	code uint8_t Str3En[]={"Input 2nd ID"};
	code uint8_t Str4[]={HZ_anya,ZF_xiaoyuhao,ZF_jinghao,ZF_dayuhao,HZ_jinru,HZ_ru,HZ_guan,HZ_li,HZ_mo,HZ_si,HZ_end};
//	code uint8_t Str4En[]={"Press # to menu"};

	uint8_t Empty32X20[80]={
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
	};

	DisHZ16x14Str(0,8,YearMonthDay,NormalDisplay);
	DisBcdDigital16x8(0,24,SystemTime.year,NormalDisplay);
	DisBcdDigital16x8(0,48,SystemTime.month,NormalDisplay);
	DisBcdDigital16x8(0,72,SystemTime.date,NormalDisplay);
	DisImage(0,100,25,16,IconBetteryLevel[BatteryMgr.BatteryLevel],NormalDisplay);
		
		if (	( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle ) )
		{
			if 	( (UserIdentifyResultMgr.UnlockingMode == DoubleMode )&&
					((UserIdentifyResultMgr.FPIdentifyStatus == S_SUCCESS )
					||(UserIdentifyResultMgr.CardIdentifyStatus == S_SUCCESS )
					||(UserIdentifyResultMgr.PasscodeIdentifyStatus == S_SUCCESS ))
				)
			{
				DisHZ16x14Str(3,16,Str3,NormalDisplay);	//please input sencond identity
				GUI_Flag_RefreshLCD = bTRUE;
			}
				
			else if ( g_ASTERISK_PressedOnMainScreen == bTRUE )
			{
				GUI_Flag_RefreshLCD = bTRUE;
				DisHZ16x14Str(3,4,Str4,NormalDisplay);					
			}
			
			else if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			{
				Clear_Screen_Page(2);
				Clear_Screen_Page(3);
				Clear_Screen_Page(4);
				Clear_Screen_Page(5);
				DisImage(2,4,27,24,Icon_Warning,NormalDisplay);
				DisHZ16x14Str(2,40,Str1,NormalDisplay);
				DisHZ16x14Str(4,40,Str2,NormalDisplay);
				GUI_Flag_RefreshLCD = bTRUE;
			}
			else
			{
				GUI_Flag_RefreshLCD = bTRUE;
				DisBcdDigital32x20(2,14,SystemTime.hour,NormalDisplay);
				DisBcdDigital32x20(2,74,SystemTime.minute,NormalDisplay);
//				DisImage(2,54,20,32,ZF32x20[10],NormalDisplay);	
					if (GUI_ToggleFlag_05s == 0 )
					{
						DisImage(2,54,20,32,Empty32X20,NormalDisplay);	
					}
					else
					{
						DisImage(2,54,20,32,ZF32x20[10],NormalDisplay);	
					}
			}
			//DisHZ16x14Str(6,16,FPDLLog,NormalDisplay);
			ShowLockBrand();
		}
	#endif
}

/*******************************************************/
void GUI_UpadteMain(void)//GUI_主界面
{
	if ( SafetyMonitorMgr.SystemLocked == bTRUE )
	{
		CurrentScreen = SCREEN_SystemLocked;
		#ifdef Function_FPMbreathingLed
		SetFPMbreathingLed(FPMbreathingLed_Off);
		#endif
	}
	
	if (( CardIdentifyMgr.CardDetectIntervalTimeCnt == 0 ) )	//FPM Cmd is sent out
	{
		CardIdentifyMgr.CardDetectIntervalTimeCnt = Def_CardDetectIntervalTime;
		CardUserIdentify();
	}

	
	if ( CardIdentifyMgr.Status == Success)
	{
		UserIdentifyResultMgr.CardIdentifyStatus = S_SUCCESS;
		UserIdentifyResultMgr.CardUserID = CardIdentifyMgr.UserID;
		RefreshSystemSleepTime();	//card detected system sleep timer reset
		SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
		if (UserIdentifyResultMgr.UnlockingMode == SingalMode )
		{
			UnlockSuccess(CARD);
		}
		else if (UserIdentifyResultMgr.UnlockingMode == DoubleMode )
		{
			if (( UserIdentifyResultMgr.PasscodeIdentifyStatus == S_SUCCESS ))
			{
				UnlockSuccess(CARDandPASSCODE);
			}
			else if (( UserIdentifyResultMgr.FPIdentifyStatus == S_SUCCESS ))
			{	
				UnlockSuccess(FINGERPRINTandCARD);
			}
			else{	
				if(UserIdentifyResultMgr.VoiceDelay == 0)
				{
					PLAY_VOICE_ONESEGMENT(VOICE_PleaseInputSecondIdentity);
					//CardIdentifyMgr.Status = ReadingCardID;
					UserIdentifyResultMgr.VoiceDelay = Def_MessageBoxTimeDelay;	
				}
				else if ( UserIdentifyResultMgr.VoiceDelay > 0 )
				{
					if( UserIdentifyResultMgr.VoiceDelay == 1 )
					{
						CardIdentifyMgr.Status = ReadingCardID;
					}
					UserIdentifyResultMgr.VoiceDelay--;
				}
			}
		}
	}
	else if ( CardIdentifyMgr.Status == Fail)
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
//			if ( (CardIdentifyMgr.CID[0] == 0x16)
//				&&(CardIdentifyMgr.CID[1] == 0xD4)
//				&&(CardIdentifyMgr.CID[2] == 0xDB)
//				&&(CardIdentifyMgr.CID[3] == 0x3C) )
//			{
//				DEBUG_MARK;
//				if ( LockBrand.BrandChangeTimeDelay == 0 )
//				{
//					WriteLockBrand();
//					if ( LockBrand.GotBrandData == bTRUE )
//					{
//						DEBUG_MARK;
//						PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Success);
//						LockBrand.BrandChangeTimeDelay = Def_GuiTimeDelayCnt3s;
//						RefreshSystemSleepTime();
//					}
//				}
//				CardIdentifyMgr.Status = ReadingCardID;
//			}
//			else
//			{
				UnlockSuccess(INITIALSTATUS);
				RefreshSystemSleepTime();
//			}
		}
		else
		{
			UserIdentifyResultMgr.CardIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.IdentifyType = CARD;
			UserIdentifyResultMgr.CardUserID = 0x00;
			//Enable_KEYLED_IdentifyFail();
			CurrentScreen = SCREEN_IdentifyFail;
			UserIdentifyResultMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VOICE_IdentifyFail);	
			if ( SafetyMonitorMgr.CardIdentifyFailedTimes < DEF_CardIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.CardIdentifyFailedTimes++;
			}
			else
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
			}
		}
	}

	FpUserIdentify();
	if ( FpIdentifyMgr.Status == success)
	{
		UserIdentifyResultMgr.FPIdentifyStatus = S_SUCCESS;
		UserIdentifyResultMgr.FPUserID = FpIdentifyMgr.UserID+1;
		SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
        SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
		if (UserIdentifyResultMgr.UnlockingMode == SingalMode )
		{
			UnlockSuccess(FINGERPRINT);
		}
		else if (UserIdentifyResultMgr.UnlockingMode == DoubleMode )
		{
			if (( UserIdentifyResultMgr.CardIdentifyStatus == S_SUCCESS))
			{
				UnlockSuccess(FINGERPRINTandCARD);
			}
			else if (( UserIdentifyResultMgr.PasscodeIdentifyStatus == S_SUCCESS ))
			{
				UnlockSuccess(FINGERPRINTandPASSCODE);
			}
			else
			{
				if(UserIdentifyResultMgr.VoiceDelay == 0)
				{
					PLAY_VOICE_ONESEGMENT(VOICE_PleaseInputSecondIdentity);
					//FpIdentifyMgr.Status = FPMcmdStart;
					UserIdentifyResultMgr.VoiceDelay = Def_MessageBoxTimeDelay;
				}
				else if ( UserIdentifyResultMgr.VoiceDelay > 0 )
				{
					if( UserIdentifyResultMgr.VoiceDelay == 1 )
					{
						FpIdentifyMgr.Status = FPMcmdStart;
					}
					UserIdentifyResultMgr.VoiceDelay--;
				}
			}
		}
	}
	else if ( FpIdentifyMgr.Status == fail)
	{
		if ( FpIdentifyMgr.ErrorType == Error_SerialNumberMismatched )
		{
			UserIdentifyResultMgr.ErrorType = FPMserialNumberMismatched;
		}
		else{
			UserIdentifyResultMgr.ErrorType = UserUnregistered;
		}
		
		if ( ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			&& (UserIdentifyResultMgr.ErrorType != FPMserialNumberMismatched)
			)
		{
			UnlockSuccess(INITIALSTATUS);
		}
		else
		{
			UserIdentifyResultMgr.FPIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.IdentifyType = FINGERPRINT;
			UserIdentifyResultMgr.FPUserID = 0x00;
			//Enable_KEYLED_IdentifyFail();
			CurrentScreen = SCREEN_IdentifyFail;
			UserIdentifyResultMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VOICE_IdentifyFail);
			
			if ( SafetyMonitorMgr.FpIdentifyFailedTimes < DEF_FpIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.FpIdentifyFailedTimes++;
			}
			else
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
			}
		
		}

	}

	PasscodeUserIdentify();
	if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifySuccess)
	{
		UserIdentifyResultMgr.PasscodeIdentifyStatus = S_SUCCESS;
		UserIdentifyResultMgr.PasscodeUserID = PasscodeUserIdentifyMgr.UserID;
		SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
    SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
    SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
		if ( (UserIdentifyResultMgr.UnlockingMode == SingalMode)
			|| (UserIdentifyResultMgr.PasscodeType == AppPasscode)
			|| (UserIdentifyResultMgr.PasscodeType == WifiTempPasscode) )
		{
			#ifdef Function_TuyaWifi
			if(UserIdentifyResultMgr.PasscodeType == WifiTempPasscode){
				UnlockSuccess(WIFITEMPPASSCODE);
			}
			else
			#endif
			{
				UnlockSuccess(PASSCODE);
			}
		}
		else if (UserIdentifyResultMgr.UnlockingMode == DoubleMode )
		{
			if (( UserIdentifyResultMgr.CardIdentifyStatus == S_SUCCESS ))
			{
				UnlockSuccess(CARDandPASSCODE);
			}
			else if (( UserIdentifyResultMgr.FPIdentifyStatus == S_SUCCESS ))
			{	
				UnlockSuccess(FINGERPRINTandPASSCODE);
			}
			else
			{
				PLAY_VOICE_ONESEGMENT(VOICE_PleaseInputSecondIdentity);
				PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
				UserIdentifyResultMgr.VoiceDelay = Def_MessageBoxTimeDelay;
			}
		}
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyFail )
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			UnlockSuccess(INITIALSTATUS);
		}
		else
		{
			UserIdentifyResultMgr.PasscodeIdentifyStatus = S_FAIL;
			UserIdentifyResultMgr.IdentifyType = PASSCODE;
			UserIdentifyResultMgr.PasscodeUserID = 0x00;
			//Enable_KEYLED_IdentifyFail();
			CurrentScreen = SCREEN_IdentifyFail;
			UserIdentifyResultMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VOICE_IdentifyFail);	
			if ( SafetyMonitorMgr.PasscodeIdentifyFailedTimes < DEF_PasscodeIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.PasscodeIdentifyFailedTimes++;
			}
			else
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
			}
		}
	}
	else if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifyAgingTestSuccess )
	{
		AgingTestMgr.TestTimes=0x0000;		
		CurrentScreen = SCREEN_AgingTest;
	}
	else if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifyCheckVersionSuccess )
	{
		GoToSystemVersion();
	}
	else if (PasscodeUserIdentifyMgr.Status == DeleteWifiTempPasscodeSuccess )
	{
		PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Success);
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
	}
	
	if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
	{
		if 	( (UserIdentifyResultMgr.UnlockingMode == DoubleMode )&&
					(	(UserIdentifyResultMgr.FPIdentifyStatus == S_SUCCESS )
					||(UserIdentifyResultMgr.CardIdentifyStatus == S_SUCCESS )
					||(UserIdentifyResultMgr.PasscodeIdentifyStatus == S_SUCCESS ) )
				)
		{
		}
	}
	#ifdef Function_TuyaWifi
	if( WifiMgr.WifiConnection.Status == JustConnected )
	{
		WifiMgr.WifiConnection.Status = Connected;
		//PLAY_VOICE_TWOSEGMENT(VOICE_NetWorking,VOICE_Success);
	}
	#endif
	#ifdef Function_ScreenDisplay
		DisplayMainPage();
	#endif

	if (SystemPowerMgr.SleepDelayTimerCnt == 0x0000)
	{
		#ifdef Function_ScreenDisplay
		Clear_Screen();
		#endif
		//SET_ALLKEYLED_OFF();
	}
	
}

/*******************************************************/
void ShowDoorBell(void)
{
	#ifdef Function_ScreenDisplay
	DisImage(1,33,61,40,Icon_DoorBell,NormalDisplay);
	#endif
	if ( DoorBellMgr.TimeCnt > 0 )
	{
		#ifdef Function_TuyaWifi
		if( DoorBellMgr.TimeCnt == 150 )
		{
			WifiMgr.RemoteUnlockMgr.WaitRequestRemoteUnlock = bTRUE;
		}
		#endif
		DoorBellMgr.TimeCnt--;
	}
	else
	{
		CurrentScreen = SCREEN_Main;
	}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowMainMenu(void)//界面-主菜单
{
	#ifdef Function_ScreenDisplay
	uint8_t i,j;
	if( VoiceMenuMgr.MenuPoint<5 )
	{
		for (i=0;i<4;i++)
		{
			DisHZ16x14Str(2*i,0,MainMenuStr[i],NormalDisplay);
		}	
	}
	else
	{
		for (j=0;j<1;j++)
		{
			DisHZ16x14Str(2*j,0,MainMenuStr[4+j],NormalDisplay);
		}	
	}
	GUI_Flag_RefreshLCD = bTRUE;
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(MainMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowLogMenu(void)//界面-日志菜单
{
	#ifdef Function_ScreenDisplay
	uint8_t i;
	for (i=0;i<2;i++)
	{
		DisHZ16x14Str(2*i,0,EventLogMenuStr[i],NormalDisplay);
	}		
	GUI_Flag_RefreshLCD = bTRUE;
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(LogMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowFpMenu(void)//界面-指纹菜单
{
	#ifdef Function_ScreenDisplay
	uint8_t i;
	for (i=0;i<3;i++)
	{
			DisHZ16x14Str(2*i,0,FpMenuStr[i],NormalDisplay);
	}
	GUI_Flag_RefreshLCD = bTRUE;
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(FpMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowPasscodeMenu(void)//界面-密码菜单
{
	#ifdef Function_ScreenDisplay
	uint8_t i;
	for (i=0;i<3;i++)
	{
			DisHZ16x14Str(2*i,0,PassCodeMenuStr[i],NormalDisplay);
	}	
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(PasscodeMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}	

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowCardUserMenu(void)//界面-磁卡菜单
{
	#ifdef Function_ScreenDisplay
	uint8_t i;
	for (i=0;i<2;i++)
	{
			DisHZ16x14Str(2*i,0,CardUserMenuStr[i],NormalDisplay);
	}	
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(CardUserMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowSystemConfigMenu(void)//系统配置菜单
{
	#ifdef Function_ScreenDisplay
	uint8_t i,j;
	if( VoiceMenuMgr.MenuPoint<5 )
	{
		for (i=0;i<4;i++)
		{
			DisHZ16x14Str(2*i,0,SytemConfigMenuStr[i],NormalDisplay);
		}	
	}
	else
	{
		for (j=0;j<2;j++)
		{
			DisHZ16x14Str(2*j,0,SytemConfigMenuStr[4+j],NormalDisplay);
		}	
	}
	GUI_Flag_RefreshLCD = bTRUE;
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(SystemConfigMenuVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}		
}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowNetWorkConnecting(void)//联网中
{
	#ifdef Function_TuyaWifi
	if ( WifiMgr.Link.Status == LinkStart )
	{
		WifiMgr.Link.Status = LinkWait;
		WifiMgr.Link.TimeCnt = 120*64;	//120S
		WifiMgr.PostMgr.Flag.Bits.StartNetwork = 1;
		PLAY_VOICE_ONESEGMENT(VOICE_NetworkingConfiguration);
	}
	else if ( WifiMgr.Link.Status == LinkWait )
	{
		if( WifiMgr.WifiConnection.Status == JustConnected )
		{
			WifiMgr.WifiConnection.Status = Connected;
			WifiMgr.Link.Status = LinkSuccess;
		}
		RefreshSystemSleepTime();
	}
	else if ( WifiMgr.Link.Status == LinkSuccess )
	{
		PLAY_VOICE_TWOSEGMENT(VOICE_NetWorking,VOICE_Success);
		WifiMgr.Link.Status = LinkIdle;
		WifiMgr.Link.TimeCnt = Def_GuiTimeDelayCnt3s;
	}
	else if ( WifiMgr.Link.Status == LinkFail )
	{
		//if ( WifiMgr.Link.errorcode == 0x02 )	//wifi module not be found
		PLAY_VOICE_TWOSEGMENT(VOICE_NetWorking,VOICE_Fail);
		WifiMgr.Link.Status = LinkIdle;
		WifiMgr.Link.TimeCnt = Def_GuiTimeDelayCnt3s;
	}
	
	if( WifiMgr.Link.TimeCnt > 0){
		WifiMgr.Link.TimeCnt--;
	}
	else
	{
		if ( WifiMgr.Link.Status == LinkWait )
		{
			WifiMgr.Link.Status = LinkFail;
			//WifiMgr.Link.errorcode = 0x01;
		}
		else
		{
			GoToMainMenu();
		}
	}
	#endif
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRegisterMasterFp(void)//界面-注册管理员指纹
{
	#ifdef Function_ScreenDisplay
	code uint8_t DuplicateFpStr[]={HZ_kongge,HZ_chong,HZ_fu,HZ_zhi,HZ_wen,HZ_end}; 
//	code uint8_t DuplicateFpStrEn[]={"<Duplicate Fp>"};
	#endif
	if ( FpUserRegisterMgr.Status == StartFpUserRegister )
	{
		if ( CheckMemoryMgr.FpMasterNum < DEF_MAX_FPMASTER )
		{
			FpUserRegisterMgr.Status = AddUserToFPM;
			FpRegisterMgr.Status = FPMcmdStart;
			DataInputMgr.Status = InputIdle;
		}
		else
		{
			FpUserRegisterMgr.Status = RegisterUserFail;
			FpUserRegisterMgr.ErrorType = MemoryIsFull;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VOICE_UsersAreFull);
			Hardware_DelayMs(100);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,UsersFullStr,NormalDisplay);
			#endif
		}
	}
	else if (FpUserRegisterMgr.Status == AddUserToFPM)
	{
		FpUserRegisterMgr.UserID = Get_Availabe_FPmasterID();
		
		RegisterFp(FpUserRegisterMgr.UserID-1);
		
		if (FpRegisterMgr.Status == success)
		{
			FpUserRegisterMgr.Status = RegisterUserSuccess;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Success);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
			#endif
			//GUI_Flag_RefreshLCD = bTRUE;
			CheckMemoryMgr.FpMasterNum+=1;
			FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].UserID = FpUserRegisterMgr.UserID;
			FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].RegisterStatus = Registered;
			FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].UserPriority = Master;
//			#ifdef Function_TuyaWifi
//				WifiMgr.PostMgr.Flag.Bits.FingerSync = 1;
//			#endif
		}
		else if (FpRegisterMgr.Status == fail)
		{
			FpUserRegisterMgr.Status = RegisterUserFail;
			FpUserRegisterMgr.ErrorType = FpRegisterMgr.ErrorType;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			if ( FpUserRegisterMgr.ErrorType == FingerPrintIsRegistered ){
				PLAY_VOICE_TWOSEGMENT(VOICE_Duplicate,VOICE_Fingerprint);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,14,DuplicateFpStr,NormalDisplay);
				#endif
			}
			else
			{
				PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Fail);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
				#endif
			}
			//GUI_Flag_RefreshLCD = bTRUE;
		}
	}

	else if ( FpUserRegisterMgr.Status == RegisterUserSuccess )
	{
		if (FpUserRegisterMgr.TimeCnt-- < 1 )		
		{
			GoToFpMenu();
			RefreshSystemSleepTime();
			#ifdef Function_EventLog
			GUI_CreatAndSaveLog(AddFpUser);
			#endif

			DEBUG_MARK;
		}
	}
	else if ( FpUserRegisterMgr.Status == RegisterUserFail )
	{
		if ( FpUserRegisterMgr.ErrorType == UserIDisRegistered )
		{
			
		}
		
		else if ( FpUserRegisterMgr.ErrorType == FingerPrintIsRegistered )
		{
		
		}
		else if ( FpUserRegisterMgr.ErrorType == MemoryIsFull )
		{
	
		}
		else
		{

		}
		
		if (FpUserRegisterMgr.TimeCnt-- < 1 )		
		{
			GoToFpMenu();
		}
		DEBUG_MARK;
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteMasterFp(void)//删除管理员指纹
{
	//uint8_t i;

	if ( FpUserDeleteMgr.Status == StartFpUserDelete )
	{
		FpUserDeleteMgr.Status = InputUserID;
		DataInputMgr.Status = InputIdle;
		PLAY_VOICE_ONESEGMENT(VOICE_PleaseInputID);
	}
	
	if ( FpUserDeleteMgr.Status == InputUserID )
	{
		
		GUI_DataInputCreat(5,72,2,0x0000);

		if (DataInputMgr.Status == InputEnd)
		{
			FpUserDeleteMgr.UserID = DataInputMgr.Value;
			if ((FpUserDeleteMgr.UserID==0)||( FpUserDeleteMgr.UserID > DEF_MAX_FPMASTER ))				
			{
				DataInputMgr.Status = InputIdle;		//Reinput
			}
			else
			{
				FpUserDeleteMgr.Status = CheckIfFpUserIDisBeUsed;
				CheckIfFpUserIDisRegistered.Status = StartCheckIfFpUserIDisRegistered;
				DataInputMgr.Status = InputIdle;	
			}
		}
		else if  (DataInputMgr.Status == InputExit)
		{
			FpUserDeleteMgr.Status = DeleteUserFail;
			FpUserDeleteMgr.ErrorType = QUIT;
			FpUserDeleteMgr.TimeCnt = 0;	
		}
	}
	else if (FpUserDeleteMgr.Status == CheckIfFpUserIDisBeUsed)
	{
		CheckIfFpUserIsRegistered(FpUserDeleteMgr.UserID-1);
		if ( CheckIfFpUserIDisRegistered.Status == CheckIfFpUserIDisRegisteredSuccess )
		{
			if ( CheckIfFpUserIDisRegistered.UserIDisRegistered == bTRUE )
			{
				if ((CheckMemoryMgr.FpMasterNum == 0x01))
					//&&(CheckMemoryMgr.PasscodeMasterNum==0x00))
				{
					FpUserDeleteMgr.Status = DeleteUserFail;
					FpUserDeleteMgr.ErrorType = CanNotDeleteLastOneMaster;
					FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
					//GUI_Flag_RefreshLCD = bTRUE;
					DEF_Fail_Beep;
				}
				else
				{
					FpUserDeleteMgr.Status = DeleteUserFromFPM;	
					DataInputMgr.Status = InputIdle;	
					//GUI_Flag_RefreshLCD = bTRUE;
				}
			}
			else
			{
				FpUserDeleteMgr.Status = DeleteUserFail;
				FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
				FpUserDeleteMgr.ErrorType = UserIDisNotRegistered;
				PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
				//GUI_Flag_RefreshLCD = bTRUE;
			}
		}
		else if ( CheckIfFpUserIDisRegistered.Status == CheckIfFpUserIDisRegisteredFail )
		{
			FpUserDeleteMgr.Status = DeleteUserFail;
			FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
			FpUserDeleteMgr.ErrorType = QUIT;
			PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
			//GUI_Flag_RefreshLCD = bTRUE;
		}
	}
	else if ( FpUserDeleteMgr.Status == DeleteUserFromFPM)
	{
		FPM_DeleteCharCmd(FpUserDeleteMgr.UserID-1,1);
		FpUserDeleteMgr.Status = WaitForDeleteUserACKfromFPM;
		FpUserDeleteMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		FpmAckMgr.Status = WaitACK;
	}
	else if (FpUserDeleteMgr.Status == WaitForDeleteUserACKfromFPM)
		{
			if (FpmAckMgr.Status == GotACK)
				{
					if ( FpmAckMgr.ErrorCode == Error_NONE)
					{
						FpUserDeleteMgr.Status = DeleteUserSuccess;
						//GUI_Flag_RefreshLCD = bTRUE;
						FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
						if ( CheckMemoryMgr.FpMasterNum > 0 )
						{
							CheckMemoryMgr.FpMasterNum-=1;
							FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserID = 0xFFFF;
							FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].RegisterStatus = UnRegistered;
							FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserPriority = Undefined;
						}
					}
					else
					{
						FpUserDeleteMgr.Status = DeleteUserFail;
						//GUI_Flag_RefreshLCD = bTRUE;
						FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
					}
					DEBUG_MARK;
				}
			else if (--FpUserDeleteMgr.TimeCnt < 1 )
				{
					FpUserDeleteMgr.Status = DeleteUserFail;
					FpmAckMgr.ErrorCode = Error_TimeOut;
					//GUI_Flag_RefreshLCD = bTRUE;
					FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
				}

		}
	else if ( FpUserDeleteMgr.Status == DeleteUserSuccess )
		{
			if (--FpUserDeleteMgr.TimeCnt < 1 )		
			{
				CurrentScreen = SCREEN_MasterFpMenu;
				UnlockModeJudgment();
				#ifdef Function_EventLog
				GUI_CreatAndSaveLog(DeleteFpUser);
				#endif
			}
			DEBUG_MARK;
		}
	else if ( FpUserDeleteMgr.Status == DeleteUserFail )
		{
			if ( FpUserDeleteMgr.ErrorType == UserIDisNotRegistered )
			{
				
			}
			/*
			else if ( FpUserDeleteMgr.ErrorType == TimeOut )
			{
				//DisHZ16x14Str(4,48,TimeOutStr,NormalDisplay);
			}
			*/
			else if ( FpUserDeleteMgr.ErrorType == CanNotDeleteLastOneMaster )
			{
				
			}
			else
			{

			}
			if (FpUserDeleteMgr.TimeCnt-- < 1 )		
			{
				CurrentScreen = SCREEN_MasterFpMenu;
			}
			DEBUG_MARK;
		}
	
}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRegisterUserFp(void)//界面-注册用户指纹
{
	//uint8_t i;
	//uint16_t UserID;	
	//~{Lm<SV8NFSC;'~}
	#ifdef Function_ScreenDisplay
	code uint8_t DuplicateFpStr[]={HZ_kongge,HZ_chong,HZ_fu,HZ_zhi,HZ_wen,HZ_end}; //重复指纹
//	code uint8_t DuplicateFpStrEn[]={"<Duplicate Fp>"};
	#endif
	
	if ( FpUserRegisterMgr.Status == StartFpUserRegister )
	{
		if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
		)
		{
			FpUserRegisterMgr.Status = RegisterUserFail;
			FpUserRegisterMgr.ErrorType = SystemNoMaster;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VOICE_PleaseAddMasterFirst);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,14,AddMasterStr,NormalDisplay);
			#endif
			//GUI_Flag_RefreshLCD = bTRUE;
		}	
		else
		{
			if ( CheckMemoryMgr.FpUserNum < DEF_MAX_FPUSER )
			{
				FpUserRegisterMgr.Status = AddUserToFPM;
				FpRegisterMgr.Status = FPMcmdStart;
				DataInputMgr.Status = InputIdle;
			}
			else
			{
				FpUserRegisterMgr.Status = RegisterUserFail;
				FpUserRegisterMgr.ErrorType = MemoryIsFull;
				FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
				PLAY_VOICE_ONESEGMENT(VOICE_UsersAreFull);
				Hardware_DelayMs(100);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,UsersFullStr,NormalDisplay);
				#endif
				//GUI_Flag_RefreshLCD = bTRUE;
			}
		}
	}
	else if (FpUserRegisterMgr.Status == AddUserToFPM)
	{
		FpUserRegisterMgr.UserID = Get_Availabe_FPuserID();
		
		RegisterFp(FpUserRegisterMgr.UserID-1);
		
		if (FpRegisterMgr.Status == success)
		{
			FpUserRegisterMgr.Status = RegisterUserSuccess;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Success);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
			#endif
			CheckMemoryMgr.FpUserNum+=1;
			FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].UserID = FpUserRegisterMgr.UserID;
			FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].RegisterStatus = Registered;
			FpUserMemoryMgr[FpUserRegisterMgr.UserID-1].UserPriority = User;
//			#ifdef Function_TuyaWifi
//				WifiMgr.PostMgr.Flag.Bits.FingerSync = 1;
//			#endif
		}
		else if (FpRegisterMgr.Status == fail)
		{
			FpUserRegisterMgr.Status = RegisterUserFail;
			FpUserRegisterMgr.ErrorType = FpRegisterMgr.ErrorType;
			FpUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			if ( FpUserRegisterMgr.ErrorType == FingerPrintIsRegistered ){
				PLAY_VOICE_TWOSEGMENT(VOICE_Duplicate,VOICE_Fingerprint);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,14,DuplicateFpStr,NormalDisplay);
				#endif
				//GUI_Flag_RefreshLCD = bTRUE;
			}
			else
			{
				PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Fail);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
				#endif
				//GUI_Flag_RefreshLCD = bTRUE;
			}
		}
	}
	else if ( FpUserRegisterMgr.Status == RegisterUserSuccess )
	{
		
		if (--FpUserRegisterMgr.TimeCnt < 1 )		
		{
			//FpUserRegisterMgr.Status = StartFpUserRegister;
			GoToFpMenu();
			RefreshSystemSleepTime();
			#ifdef Function_EventLog
			GUI_CreatAndSaveLog(AddFpUser);
			#endif
		}
		DEBUG_MARK;
	}
	else if ( FpUserRegisterMgr.Status == RegisterUserFail )
	{		
		if ( FpUserRegisterMgr.ErrorType == UserIDisRegistered )
		{
		
		}
		else if ( FpUserRegisterMgr.ErrorType == SystemNoMaster )
		{
			
		}
		else if ( FpUserRegisterMgr.ErrorType == FingerPrintIsRegistered )
		{
			
		}
		else if ( FpUserRegisterMgr.ErrorType == MemoryIsFull )
		{
			
		}
		else
		{
			
		}
		if (FpUserRegisterMgr.TimeCnt-- < 1 )		
		{
			GoToFpMenu();
		}
		DEBUG_MARK;
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteUserFp(void)//界面-删除用户指纹
{
	if ( FpUserDeleteMgr.Status == StartFpUserDelete )
	{
		FpUserDeleteMgr.Status = InputUserID;
		DataInputMgr.Status = InputIdle;
		PLAY_VOICE_ONESEGMENT(VOICE_PleaseInputID);
	}
	if ( FpUserDeleteMgr.Status == InputUserID )
	{
			
			GUI_DataInputCreat(5,80,3,0x0000);
	
			if (DataInputMgr.Status == InputEnd)
			{
				FpUserDeleteMgr.UserID = DataInputMgr.Value;
				if ((FpUserDeleteMgr.UserID >DEF_MAX_FPMASTER )&&( FpUserDeleteMgr.UserID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER+1)) )
				{
					FpUserDeleteMgr.Status = CheckIfFpUserIDisBeUsed;
					CheckIfFpUserIDisRegistered.Status = StartCheckIfFpUserIDisRegistered;
					DataInputMgr.Status = InputIdle;
				}
				else				
				{
					DataInputMgr.Status = InputIdle;		//Reinput
				}

			}
			else if  (DataInputMgr.Status == InputExit)
			{
				FpUserDeleteMgr.Status = DeleteUserFail;
				FpUserDeleteMgr.ErrorType = QUIT;
				FpUserDeleteMgr.TimeCnt = 0;
			}
	}
	else if (FpUserDeleteMgr.Status == CheckIfFpUserIDisBeUsed)
	{
		CheckIfFpUserIsRegistered(FpUserDeleteMgr.UserID-1);
		if ( CheckIfFpUserIDisRegistered.Status == CheckIfFpUserIDisRegisteredSuccess )
		{
			if ( CheckIfFpUserIDisRegistered.UserIDisRegistered == bTRUE )
			{
				FpUserDeleteMgr.Status = DeleteUserFromFPM;	
				DataInputMgr.Status = InputIdle;	
				//GUI_Flag_RefreshLCD = bTRUE;
			}
			else
			{
				FpUserDeleteMgr.Status = DeleteUserFail;
				FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
				FpUserDeleteMgr.ErrorType = UserIDisNotRegistered;
				PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
				//GUI_Flag_RefreshLCD = bTRUE;
			}
		}
		else if ( CheckIfFpUserIDisRegistered.Status == CheckIfFpUserIDisRegisteredFail )
		{
			FpUserDeleteMgr.Status = DeleteUserFail;
			FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
			FpUserDeleteMgr.ErrorType = QUIT;
			PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
			//GUI_Flag_RefreshLCD = bTRUE;
		}
	}
	else if ( FpUserDeleteMgr.Status == DeleteUserFromFPM)
	{
		FPM_DeleteCharCmd(FpUserDeleteMgr.UserID-1,1);
		FpUserDeleteMgr.Status = WaitForDeleteUserACKfromFPM;
		FpUserDeleteMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		FpmAckMgr.Status = WaitACK;
	}
	else if (FpUserDeleteMgr.Status == WaitForDeleteUserACKfromFPM)
		{
			if (FpmAckMgr.Status == GotACK)
				{
					if ( FpmAckMgr.ErrorCode == Error_NONE)
					{
						FpUserDeleteMgr.Status = DeleteUserSuccess;
						//GUI_Flag_RefreshLCD = bTRUE;
						FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Success);
						if ( CheckMemoryMgr.FpUserNum > 0 )
						{
							CheckMemoryMgr.FpUserNum-=1;
							FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserID = 0xFFFF;
							FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].RegisterStatus = UnRegistered;
							FpUserMemoryMgr[FpUserDeleteMgr.UserID-1].UserPriority = Undefined;
						}
					}
					else
					{
						FpUserDeleteMgr.Status = DeleteUserFail;
						//GUI_Flag_RefreshLCD = bTRUE;
						FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
					}
					DEBUG_MARK;
				}
			else if (--FpUserDeleteMgr.TimeCnt < 1 )
				{
					FpUserDeleteMgr.Status = DeleteUserFail;
					FpmAckMgr.ErrorCode = Error_TimeOut;
					//GUI_Flag_RefreshLCD = bTRUE;
					FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
					PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
				}

		}
	else if ( FpUserDeleteMgr.Status == DeleteUserSuccess )
		{
			
			if (--FpUserDeleteMgr.TimeCnt < 1 )		
			{
				CurrentScreen = SCREEN_UserFpMenu;
				UnlockModeJudgment();
				#ifdef Function_EventLog
				GUI_CreatAndSaveLog(DeleteFpUser);
				#endif
			}
			DEBUG_MARK;
		}
	else if ( FpUserDeleteMgr.Status == DeleteUserFail )
		{
			if ( FpUserDeleteMgr.ErrorType == UserIDisNotRegistered )
			{

			}
			else
			{

			}
			if (FpUserDeleteMgr.TimeCnt-- < 1 )		
			{
				CurrentScreen = SCREEN_UserFpMenu;
			}
			DEBUG_MARK;
		}
	
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteAllUserFp(void)//界面-删除所有用户指纹
{
	uint16_t i;
	
	if ( AllUserFpDeleteMgr.Status == StartAllUserFpDelete )
	{
		AllUserFpDeleteMgr.Status = WaitForUserConfirmDeleteAllFP;
		AllUserFpDeleteMgr.Selection = NO;
		//PLAY_VOICE_ONESEGMENT(VOICE_ConfirmToDelete);
		PLAY_VOICE_MULTISEGMENTS(ConfirmDeleteVoiceBuff);
		#ifdef Function_ScreenDisplay
		DisHZ16x14Str(2,16,ConfirmDEL,NormalDisplay);
		DisHZ16x14Str(4,16,CancelDEL,NormalDisplay);
		#endif
	}
	else if ( AllUserFpDeleteMgr.Status == WaitForUserConfirmDeleteAllFP )
	{
		//if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
		//{
			
		//}
	}
	else if ( AllUserFpDeleteMgr.Status == SendDeleteAllUserFpCmdToFPM )
	{
		
		FPM_DeleteCharCmd(DEF_MAX_FPMASTER,DEF_MAX_FPUSER);
		AllUserFpDeleteMgr.Status = WaitForDeleteAllUserFpCmdACKfromFPM;
		AllUserFpDeleteMgr.TimeCnt =Def_FPMcmdTimeOutDelay;
		//GUI_Flag_RefreshLCD = bTRUE;
		FpmAckMgr.Status = WaitACK;
	}
	
	else if ( AllUserFpDeleteMgr.Status == WaitForDeleteAllUserFpCmdACKfromFPM )
		  {
			  if (FpmAckMgr.Status == GotACK)
				{
					if ( FpmAckMgr.ErrorCode == Error_NONE)
					{
						AllUserFpDeleteMgr.Status = DeleteAllFpUserSuccess;
						//GUI_Flag_RefreshLCD = bTRUE;
						FpUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Success);
						CheckMemoryMgr.FpUserNum=0;
						for (i=DEF_MAX_FPMASTER;i<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER);i++)
						{
							FpUserMemoryMgr[i].UserID = 0xFFFF;
							FpUserMemoryMgr[i].RegisterStatus = UnRegistered;
							FpUserMemoryMgr[i].UserPriority = Undefined;
						}
					}
					else
					{
						AllUserFpDeleteMgr.Status = DeleteAllFpUserFail;
						//GUI_Flag_RefreshLCD = bTRUE;
						AllUserFpDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
					}
					DEBUG_MARK;
				}
			else if (--FpUserDeleteMgr.TimeCnt < 1 )
				{
					AllUserFpDeleteMgr.Status = DeleteAllFpUserFail;
					FpmAckMgr.ErrorCode = Error_TimeOut;
					//GUI_Flag_RefreshLCD = bTRUE;
					AllUserFpDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
					PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
				}
		  }
	
	else if ( AllUserFpDeleteMgr.Status == DeleteAllFpUserSuccess )
		{
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
			#endif
			if (--AllUserFpDeleteMgr.TimeCnt < 1 )
			{
				GoToFpMenu();
				#ifdef Function_EventLog
				GUI_CreatAndSaveLog(DeleteAllFpUser);
				#endif
			}
		}
	else if ( AllUserFpDeleteMgr.Status == DeleteAllFpUserFail )
		{
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
			#endif
			if (--AllUserFpDeleteMgr.TimeCnt < 1 )
			{
				GoToFpMenu();
			}
		}
	else if ( AllUserFpDeleteMgr.Status == EXIT )
		{
			GoToFpMenu();
		}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteCardUser(void)//界面-删除磁卡用户
{
	if ( CardUserDeleteMgr.Status == StartCardUserDelete )
	{
		CardUserDeleteMgr.Status = InputCardUserID;
		DataInputMgr.Status = InputIdle;
		PLAY_VOICE_ONESEGMENT(VOICE_PleaseInputID);
	}
	if ( CardUserDeleteMgr.Status == InputCardUserID )
	{
		//DisEN16x8Str(6,0,CardUserIDRangeStrEn,NormalDisplay);
//		DisOneDigital16x8(6,24,DEF_MAX_CARDUSER/100,NormalDisplay);
//		DisOneDigital16x8(6,32,DEF_MAX_CARDUSER%100/10,NormalDisplay);
//		DisOneDigital16x8(6,40,DEF_MAX_CARDUSER%10,NormalDisplay);
		
		GUI_DataInputCreat(6,68,3,0x0000);

		if (DataInputMgr.Status == InputEnd)
		{
			CardUserDeleteMgr.UserID = DataInputMgr.Value;
			if ( CardUserDeleteMgr.UserID > DEF_MAX_CARDUSER )				
			{
				DataInputMgr.Status = InputIdle;		//Reinput
			}
			else if ( IfCardUserIDisRegistered(CardUserDeleteMgr.UserID) == bTRUE )
			{
				DeleteCardUserfromMemory(CardUserDeleteMgr.UserID);
				CardUserDeleteMgr.Status = Success;	
				CardUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				//GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Success);
				if ( CheckMemoryMgr.CardUserNum > 0 )
				{
					CheckMemoryMgr.CardUserNum-=1;
				}
					
			}
			else
			{
				CardUserDeleteMgr.Status = Fail;
				CardUserDeleteMgr.ErrorType = UserIDisNotRegistered;
				CardUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				//GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
			}
		}
		else if  (DataInputMgr.Status == InputExit)
		{
			CardUserDeleteMgr.Status = Fail;
			CardUserDeleteMgr.ErrorType = QUIT;
			CardUserDeleteMgr.TimeCnt = 0;
		}
	}
	
	else if  ( CardUserDeleteMgr.Status == Success)
	{
		
		if (--CardUserDeleteMgr.TimeCnt < 1 )
		{
			CurrentScreen = SCREEN_CardUserMenu;
			UnlockModeJudgment();
//			#ifdef Function_TuyaWifi
//			WifiMgr.PostMgr.Flag.Bits.CardSync = 1;
//			#endif
			#ifdef Function_EventLog
			GUI_CreatAndSaveLog(DeleteCardUser);
			#endif
		}
	}
	else if ( CardUserDeleteMgr.Status == Fail)
	{
		if (CardUserDeleteMgr.ErrorType == UserIDisNotRegistered)
		{

		}
		else
		{
		

		}
		if ( CardUserDeleteMgr.TimeCnt-- < 1 )
		{
			CurrentScreen = SCREEN_CardUserMenu;
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteAllCardUser(void)//界面-删除全部磁卡用户
{
	
	if ( AllCardUserDeleteMgr.Status == StartAllCardUserDelete )
	{
		AllCardUserDeleteMgr.Status = WaitForUserConfirmDeleteAllCard;
		AllCardUserDeleteMgr.Selection = NO;
		//PLAY_VOICE_ONESEGMENT(VOICE_ConfirmToDelete);
		PLAY_VOICE_MULTISEGMENTS(ConfirmDeleteVoiceBuff);
	}
	else if ( AllCardUserDeleteMgr.Status == WaitForUserConfirmDeleteAllCard )
	{	
		#ifdef Function_ScreenDisplay
			DisHZ16x14Str(2,16,ConfirmDEL,NormalDisplay);
			DisHZ16x14Str(4,16,CancelDEL,NormalDisplay);
		#endif
	}
	else if ( AllCardUserDeleteMgr.Status == DeletingAllCardUser )
	{
		
		DeleteAllCardUserfromMemory();
		AllCardUserDeleteMgr.TimeCnt =Def_MessageBoxTimeDelay;
		AllCardUserDeleteMgr.Status = Success;
		//GUI_Flag_RefreshLCD = bTRUE;
		PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Success);
		CheckMemoryMgr.CardUserNum =0;
	}
	else if ( AllCardUserDeleteMgr.Status == Success )
	{
		#ifdef Function_ScreenDisplay
		Clear_Screen();
		DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
		#endif
		if (AllCardUserDeleteMgr.TimeCnt-- < 1 )
		{
			GoToCardMenu();
			UnlockModeJudgment();
			#ifdef Function_EventLog
			GUI_CreatAndSaveLog(DeleteAllCardUser);
			#endif
		}
	}
	else if ( AllCardUserDeleteMgr.Status == CardUserEXIT )
	{
		GoToCardMenu();
	}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowRegisterPasscodeUser(void)//注册密码用户
{
	uint8_t i;
	
	if (PasscodeUserRegisterMgr.Status == StartPasscodeUserRegister)
	{
		if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(PasscodeUserRegisterMgr.UserPriority == User)
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
			)
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = SystemNoMaster;
			PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VOICE_PleaseAddMasterFirst);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,14,AddMasterStr,NormalDisplay);
			#endif
		}
		else if ( 	((PasscodeUserRegisterMgr.UserPriority == Master)&&(!(CheckMemoryMgr.PasscodeMasterNum < DEF_MAX_PASSCODEMASTER)))
					||((PasscodeUserRegisterMgr.UserPriority == User)&&(!(CheckMemoryMgr.PasscodeUserNum < DEF_MAX_PASSCODEUSER)))	)
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = MemoryIsFull;
			PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VOICE_UsersAreFull);
			Hardware_DelayMs(100);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,UsersFullStr,NormalDisplay);
			#endif
		}
		else
		{
			PasscodeUserRegisterMgr.Status = InputPasscodeUserID;
			DataInputMgr.Status = InputIdle;
		}
	}
	else if  (PasscodeUserRegisterMgr.Status == InputPasscodeUserID)
	{
		PasscodeUserRegisterMgr.Status = InputFirstPasscode;
		PasscodeUserRegisterMgr.OverTimeCnt = Def_GuiTimeDelayCnt5s;
		DataInputMgr.Status = InputIdle;
		//PasscodeUserRegisterMgr.UserID = DataInputMgr.Value;
		if (PasscodeUserRegisterMgr.UserPriority == Master)
		{
			PasscodeUserRegisterMgr.UserID = Get_Availabe_PasscodeMasterID();
		}
		else
		{
			PasscodeUserRegisterMgr.UserID = Get_Availabe_PasscodeUserID();
		}
		
		PasscodeInputMgr.Point = 0x00;
		PasscodeInputMgr.PasscodeLen = 12;
		PasscodeInputMgr.Status = PasscodeInputStart;
		for (i=0;i<12;i++)
		{
			PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
		}
		PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;		
		//GUI_Flag_RefreshLCD = bTRUE;
		if (PasscodeUserRegisterMgr.UserPriority == Master)
		{
			PLAY_VOICE_THREESEGMENT(VOICE_PleaseEnter,VOICE_Administrator,VOICE_Password);
		}
		else
		{
			PLAY_VOICE_THREESEGMENT(VOICE_PleaseEnter,VOICE_User,VOICE_Password);
		}
		#ifdef Function_ScreenDisplay
		Clear_Screen();
		DisHZ16x14Str(3,30,InputPasscodeStr,NormalDisplay);
		#endif
	}
	else if  ( PasscodeUserRegisterMgr.Status == InputFirstPasscode)
	{
		#ifdef Function_ScreenDisplay
		Clear_Screen();
		GUI_PasscodeInputCreat(5,0);
		#endif
		if (PasscodeInputMgr.Status == PasscodeInputEnd)
		{
			if ( PasscodeInputMgr.Point > 5 )
			{
				for (i=0;i<12;i++)
				{
					PasscodeBUFF1[i] = PasscodeInputMgr.InputBuff[i];
				}
				PasscodeUserRegisterMgr.Status = InputSecondPasscode;
				PasscodeInputMgr.Point = 0x00;
				PasscodeInputMgr.PasscodeLen = 12;
				PasscodeInputMgr.Status = PasscodeInputStart;
				for (i=0;i<12;i++)
				{
					PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
				}
				PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;	
				//GUI_Flag_RefreshLCD = bTRUE;
				if (PasscodeUserRegisterMgr.UserPriority == Master)
				{
					PLAY_VOICE_THREESEGMENT(VOICE_Again,VOICE_Administrator,VOICE_Password);
				}
				else
				{
					PLAY_VOICE_THREESEGMENT(VOICE_Again,VOICE_User,VOICE_Password);
				}
			}
			else
			{
				PasscodeUserRegisterMgr.Status = InputFirstPasscode;
				PasscodeInputMgr.Point = 0x00;
				PasscodeInputMgr.PasscodeLen = 12;
				PasscodeInputMgr.Status = PasscodeInputStart;
				for (i=0;i<12;i++)
				{
					PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
				}
				PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;
				//GUI_Flag_RefreshLCD = bTRUE;
			}
		}
		else if ( PasscodeInputMgr.Status == PasscodeInputExit )
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = QUIT;
			PasscodeUserRegisterMgr.TimeCnt = 1;
		}
		#ifdef Function_ScreenDisplay
		DisHZ16x14Str(3,30,InputPasscodeStr,NormalDisplay);
		#endif
		else if ( PasscodeUserRegisterMgr.OverTimeCnt-- < 1 )
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = TimeOut;
			PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
			PLAY_VOICE_TWOSEGMENT(VOICE_Operation, VOICE_Fail);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
			#endif
		}	
	}
	else if ( PasscodeUserRegisterMgr.Status == InputSecondPasscode )
	{
		GUI_PasscodeInputCreat(5,0);

		if (PasscodeInputMgr.Status == PasscodeInputEnd)
		{
			if ( PasscodeInputMgr.Point > 5 )
				{
					PasscodeUserRegisterMgr.Status = CompareTwoPasscode;	
					//GUI_Flag_RefreshLCD = bTRUE;
				}
			else
				{
					PasscodeUserRegisterMgr.Status = InputSecondPasscode;
					PasscodeInputMgr.Point = 0x00;
					PasscodeInputMgr.PasscodeLen = 12;
					PasscodeInputMgr.Status = PasscodeInputStart;
					for (i=0;i<12;i++)
					{
						PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
					}
					PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;	
					//GUI_Flag_RefreshLCD = bTRUE;
				}
		}
		else if ( PasscodeInputMgr.Status == PasscodeInputExit )
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = QUIT;
			PasscodeUserRegisterMgr.TimeCnt = 1;
		}
		else if ( PasscodeUserRegisterMgr.OverTimeCnt-- < 1 )
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = TimeOut;
			PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_TWOSEGMENT(VOICE_Operation, VOICE_Fail);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
			#endif
		}	
	}
	else if ( PasscodeUserRegisterMgr.Status == CompareTwoPasscode )
	{
		if (GUI_CompareTwoPasscodes(PasscodeBUFF1,PasscodeInputMgr.InputBuff) == bTRUE)
		{
			if (PasscodeIdendify(PasscodeInputMgr.InputBuff) == 0x00 )	//passcode is not be used
			{
				if (SavePasscodeUserToMemory(PasscodeInputMgr.InputBuff,PasscodeUserRegisterMgr.UserID) == S_SUCCESS)
				{
					PasscodeUserRegisterMgr.Status = RegisterPasscodeUserSuccess;
					PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
					//GUI_Flag_RefreshLCD = bTRUE;
					PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Success);
					#ifdef Function_ScreenDisplay
					Clear_Screen();
					DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
					#endif
					if (PasscodeUserRegisterMgr.UserPriority == Master){
						CheckMemoryMgr.PasscodeMasterNum+=1;
					}
					else{
						CheckMemoryMgr.PasscodeUserNum+=1;
					}
				}
				else
				{
					//目前不返回失败
				}
			}
			else
			{
				PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
				PasscodeUserRegisterMgr.ErrorType = PasscodeIsBeUsed;
				PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
				PLAY_VOICE_TWOSEGMENT(VOICE_Duplicate,VOICE_Password);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,PasscodeisBeUsedStr,NormalDisplay);
				#endif
			}
		}
		else
		{
			PasscodeUserRegisterMgr.Status = RegisterPasscodeUserFail;
			PasscodeUserRegisterMgr.ErrorType = TwoPasscodesDoNotMatch;
			PasscodeUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
			PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Fail);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
			#endif
		}
	}
	else if ( PasscodeUserRegisterMgr.Status ==  RegisterPasscodeUserSuccess )
	{
		if ( PasscodeUserRegisterMgr.TimeCnt-- < 1 )
		{
			#ifdef DirectIntoMainMenuInFactoryDefaultStatus
			PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
			#else
			
			if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(PasscodeUserRegisterMgr.UserPriority == Master)
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x01)){
				/*
				FpUserRegisterMgr.Status = AddUserToFPM;	
				FpRegisterMgr.Status = FPMcmdStart;
				FpUserRegisterMgr.UserID=0x01;
				CurrentScreen = SCREEN_RegisterMasterFp;
				*/
				GoToMainMenu();
			}
			else
			{
				GoToPasswordMenu();
				//PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
			}
			#endif
			
//			#ifdef Function_TuyaWifi
//			WifiMgr.PostMgr.Flag.Bits.PasswordSync = 1;
//			#endif
			
			#ifdef Function_EventLog
			GUI_CreatAndSaveLog(AddPasscodeUser);
			#endif
		}
	}
	else if ( PasscodeUserRegisterMgr.Status == RegisterPasscodeUserFail )
	{
		if ( PasscodeUserRegisterMgr.ErrorType == UserIDisRegistered)
		{

		}
		else if ( PasscodeUserRegisterMgr.ErrorType == SystemNoMaster )
		{

		}
		else if ( PasscodeUserRegisterMgr.ErrorType == PasscodeIsBeUsed )
		{

		}
		else if ( PasscodeUserRegisterMgr.ErrorType == MemoryIsFull )
		{

		}
		else if ( PasscodeUserRegisterMgr.ErrorType == QUIT )
		{
		
		}
		else
		{

		}
	
		if ( PasscodeUserRegisterMgr.TimeCnt-- < 1 )
		{
			if 	( 	(PasscodeUserRegisterMgr.ErrorType == QUIT )
					&&(CheckMemoryMgr.FpMasterNum == 0x00 )
					&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
				)
			{
				CurrentScreen = SCREEN_Main;
				PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
				FpIdentifyMgr.Status = FPMcmdStart;
				CardIdentifyMgr.Status = ReadingCardID;
			}
			else if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(PasscodeUserRegisterMgr.UserPriority == Master)
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
			)
			{
				CurrentScreen = SCREEN_Main;
				PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
				FpIdentifyMgr.Status = FPMcmdStart;
				CardIdentifyMgr.Status = ReadingCardID;
			}
			else if ( (PasscodeUserRegisterMgr.ErrorType == MemoryIsFull )
				||(PasscodeUserRegisterMgr.ErrorType == QUIT )
				||( PasscodeUserRegisterMgr.ErrorType == SystemNoMaster )
				)
			{
				if (PasscodeUserRegisterMgr.UserPriority == Master)
				{
					GoToPasswordMenu();
				}
				else{
					GoToPasswordMenu();
				}
			}
			else
			{
				//PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
				GoToPasswordMenu();
			}
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeletePasscodeUser(void)//界面-删除密码用户
{
	if ( PasscodeUserDeleteMgr.Status == StartPasscodeUserDelete )
	{
		PasscodeUserDeleteMgr.Status = InputPasscodeUserID;
		DataInputMgr.Status = InputIdle;
		PLAY_VOICE_ONESEGMENT(VOICE_PleaseInputID);
	}
	else if ( PasscodeUserDeleteMgr.Status == InputPasscodeUserID )
		{
			if (PasscodeUserDeleteMgr.UserPriority == Master)
			{
//				DisOneDigital16x8(6,8,1,NormalDisplay);
//				DisOneDigital16x8(6,32,DEF_MAX_PASSCODEMASTER%10,NormalDisplay);
			}
			else
			{
//				DisOneDigital16x8(6,8,(DEF_MAX_PASSCODEMASTER+1)%10,NormalDisplay);
//				DisOneDigital16x8(6,24,(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER)/10,NormalDisplay);
//				DisOneDigital16x8(6,32,(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER)%10,NormalDisplay);
			}
			
			GUI_DataInputCreat(6,56,2,0x0000);

			if (DataInputMgr.Status == InputEnd)
			{
				PasscodeUserDeleteMgr.UserID = DataInputMgr.Value;
				if (  (((PasscodeUserDeleteMgr.UserID>(DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER))||(PasscodeUserDeleteMgr.UserID<(DEF_MAX_PASSCODEMASTER+1)))&&(PasscodeUserDeleteMgr.UserPriority == User))		
				  ||(((PasscodeUserDeleteMgr.UserID > DEF_MAX_PASSCODEMASTER)||(PasscodeUserDeleteMgr.UserID<1))&&(PasscodeUserDeleteMgr.UserPriority == Master))
			   )			
				{
					DataInputMgr.Status = InputIdle;		//Reinput
				}
				else
				{
					PasscodeUserDeleteMgr.Status = CheckIfPasscodeUserIDisRegistered;
					DataInputMgr.Status = InputIdle;	
				}
			}
			else if  (DataInputMgr.Status == InputExit)
			{
				PasscodeUserDeleteMgr.Status = DeletePasscodeUserFail;
				PasscodeUserDeleteMgr.ErrorType = QUIT;
				PasscodeUserDeleteMgr.TimeCnt = 0;
			}
		}
	else if ( PasscodeUserDeleteMgr.Status == CheckIfPasscodeUserIDisRegistered)
	{
		if ( IfPasscodeUserIDisRegistered(PasscodeUserDeleteMgr.UserID) == bTRUE )
			{
				if (PasscodeUserDeleteMgr.UserPriority == Master)
				{
					if ((CheckMemoryMgr.PasscodeMasterNum==0x01))
					//&&(CheckMemoryMgr.FpMasterNum == 0x00))
					{
						PasscodeUserDeleteMgr.Status = DeletePasscodeUserFail;
						PasscodeUserDeleteMgr.ErrorType = CanNotDeleteLastOneMaster;
						PasscodeUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
						//GUI_Flag_RefreshLCD = bTRUE;
						DEF_Fail_Beep;
					}
					else
					{
						DeletePasscodeUserfromMemory(PasscodeUserDeleteMgr.UserID);
						PasscodeUserDeleteMgr.Status = DeletePasscodeUserSuccess;	
						PasscodeUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
						//GUI_Flag_RefreshLCD = bTRUE;
						PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Success);
						if ( CheckMemoryMgr.PasscodeMasterNum > 0 )
						{
							CheckMemoryMgr.PasscodeMasterNum-=1;
						}
					}
				}
				else
				{
					DeletePasscodeUserfromMemory(PasscodeUserDeleteMgr.UserID);
					PasscodeUserDeleteMgr.Status = DeletePasscodeUserSuccess;	
					PasscodeUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
					//GUI_Flag_RefreshLCD = bTRUE;
					PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Success);
					if ( CheckMemoryMgr.PasscodeUserNum > 0 )
					{
						CheckMemoryMgr.PasscodeUserNum-=1;
					}
				}
					
			}
		else
			{
				PasscodeUserDeleteMgr.Status = DeletePasscodeUserFail;
				PasscodeUserDeleteMgr.ErrorType = UserIDisNotRegistered;
				PasscodeUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				//GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Fail);
			}
	}
	else if  ( PasscodeUserDeleteMgr.Status == DeletePasscodeUserSuccess)
		{
			
			if (--PasscodeUserDeleteMgr.TimeCnt < 1 )
				{
					//CurrentScreen = SCREEN_PasscodeUserMenu;
					PasscodeUserDeleteMgr.Status = StartPasscodeUserDelete;
					//GUI_Flag_RefreshLCD = bTRUE;
					UnlockModeJudgment();
					#ifdef Function_EventLog
					GUI_CreatAndSaveLog(DeletePasscodeUser);
					#endif
				}
		}
	else if ( PasscodeUserDeleteMgr.Status == DeletePasscodeUserFail)
		{
			if ( PasscodeUserDeleteMgr.ErrorType == UserIDisNotRegistered)
			{

			}
			else if ( PasscodeUserDeleteMgr.ErrorType == CanNotDeleteLastOneMaster )
			{

			}
			else
			{
	
			}
			
			if (PasscodeUserDeleteMgr.TimeCnt-- < 1 )
			{
				if ( PasscodeUserDeleteMgr.ErrorType == QUIT )
				{
					if (PasscodeUserDeleteMgr.UserPriority == Master)
					{
						CurrentScreen = SCREEN_PasscodeMasterMenu;
					}
					else{
						CurrentScreen = SCREEN_PasscodeUserMenu;
					}
				}
				else
				{
					PasscodeUserDeleteMgr.Status = StartPasscodeUserDelete;
					//GUI_Flag_RefreshLCD = bTRUE;
				}
			}
		}
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowDeleteAllPasscodeUser(void)//界面-删除全部密码用户
{
	//uint8_t i;
	
	if ( AllPasscodeUserDeleteMgr.Status == StartAllPasscodeUserDelete )
	{
		AllPasscodeUserDeleteMgr.Status = WaitForDeleteAllPasscodeUserConfirm;
		AllPasscodeUserDeleteMgr.Selection = NO;
		PLAY_VOICE_MULTISEGMENTS(ConfirmDeleteVoiceBuff);
	}
	else if ( AllPasscodeUserDeleteMgr.Status == WaitForDeleteAllPasscodeUserConfirm )
	{
		#ifdef Function_ScreenDisplay
		DisHZ16x14Str(2,16,ConfirmDEL,NormalDisplay);
		DisHZ16x14Str(4,16,CancelDEL,NormalDisplay);
		#endif
	}
	else if ( AllPasscodeUserDeleteMgr.Status == DeletingAllPasscodeUser )
	{
		DeleteAllPasscodeUserfromMemory();
		AllPasscodeUserDeleteMgr.TimeCnt = Def_MessageBoxTimeDelay;
		AllPasscodeUserDeleteMgr.Status = DeleteAllPasscodeUserSuccess;
		GUI_Flag_RefreshLCD = bTRUE;
		PLAY_VOICE_TWOSEGMENT(VOICE_Delete,VOICE_Success);
		CheckMemoryMgr.PasscodeUserNum = 0;
//		#ifdef Function_TuyaWifi
//			WifiMgr.PostMgr.Flag.Bits.PasswordSync = 1;
//		#endif
	}
	else if ( AllPasscodeUserDeleteMgr.Status == DeleteAllPasscodeUserSuccess )
		{
			#ifdef Function_ScreenDisplay
			DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
			#endif
			if (AllPasscodeUserDeleteMgr.TimeCnt-- < 1 )
			{
				GoToPasswordMenu();
				UnlockModeJudgment();
				#ifdef Function_EventLog
				GUI_CreatAndSaveLog(DeleteAllPasscodeUser);
				#endif
			}
		}
	else if  ( AllPasscodeUserDeleteMgr.Status == PasscodeUserEXIT )
		{
			GoToPasswordMenu();
		}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowTimeSetting(void)//界面-时间设定
{
	#ifdef Function_ScreenDisplay
	code uint8_t TitleStr[]={HZ_shi,HZ_jian,HZ_she,HZ_ding,HZ_end};  						//时间设定

	code uint8_t YearMonthDayStrEn[]={"2020-01-01"};//初始值
	
	code uint8_t TimeStrEn[]={"23:01:01"};

	DisHZ16x14Str(0,28,TitleStr,NormalDisplay);

	DisEN16x8Str(3,24,YearMonthDayStrEn,NormalDisplay);//显示字符串2020-01-01
	DisBcdDigital16x8(3,40,TempSystemTime.year,NormalDisplay);//40-24=16，即把2019的19覆盖掉，20不覆盖 
	DisBcdDigital16x8(3,64,TempSystemTime.month,NormalDisplay);
	DisBcdDigital16x8(3,88,TempSystemTime.date,NormalDisplay);
	
	DisEN16x8Str(6,24,TimeStrEn,NormalDisplay);
	DisBcdDigital16x8(6,24,TempSystemTime.hour,NormalDisplay);
	DisBcdDigital16x8(6,48,TempSystemTime.minute,NormalDisplay);
	DisBcdDigital16x8(6,72,TempSystemTime.second,NormalDisplay);	

	if ( (TimeSettingMgr.Status == SetYearMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,40,TempSystemTime.year>>4,InverseDisplay);//高4位移至低4位显示			
	}
	else if ( (TimeSettingMgr.Status == SetYearMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,48,TempSystemTime.year&0x0F,InverseDisplay);//取低4位显示
	}
	else if ( (TimeSettingMgr.Status == SetMonthMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,64,TempSystemTime.month>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetMonthMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,72,TempSystemTime.month&0x0F,InverseDisplay);	
	}
	else if ( (TimeSettingMgr.Status == SetDateMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,88,TempSystemTime.date>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetDateMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(3,96,TempSystemTime.date&0x0F,InverseDisplay);	
	}
	
	else if ( (TimeSettingMgr.Status == SetHourMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,24,TempSystemTime.hour>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetHourMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,32,TempSystemTime.hour&0x0F,InverseDisplay);	
	}
	else if ( (TimeSettingMgr.Status == SetMinuteMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,48,TempSystemTime.minute>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetMinuteMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,56,TempSystemTime.minute&0x0F,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetSecondMajor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,72,TempSystemTime.second>>4,InverseDisplay);		
	}
	else if ( (TimeSettingMgr.Status == SetSecondMinor)&&(GUI_ToggleFlag_05s==0x01) )
	{
		DisOneDigital16x8(6,80,TempSystemTime.second&0x0F,InverseDisplay);	
	}
	#endif
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
#ifdef Function_ScreenDisplay
//code uint8_t ReplaceBatteryStr1En[]={"Low Power,Pls"};
//code uint8_t ReplaceBatteryStr2En[]={"Replace Battery"};
#endif
void ShowLowBattery(void)//低电压界面
{
	#ifdef Function_ScreenDisplay
	DisImage(0,50,27,24,Icon_Warning,NormalDisplay);
	DisImage(5,4,120,24,Icon_ReplaceBattery,NormalDisplay);
	#endif 	
	if ( BatteryMgr.LowBatteryProtectionEnabled == bFALSE)
	{
		if (--BatteryMgr.TimeCnt < 1 )
		{
			CurrentScreen = SCREEN_Main;
			FpIdentifyMgr.Status = FPMcmdStart;
			CardIdentifyMgr.Status = ReadingCardID;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
			if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
			{
				RefreshSystemSleepTime();
				PLAY_VOICE_ONESEGMENT(VOICE_PleaseAddMasterFirst);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,14,AddMasterStr,NormalDisplay);
				#endif
			}
			else
			{
				//PLAY_VOICE_ONESEGMENT(VOICE_PleasePutFingerOrCardOrPasscode);
			}
		}
	}
}


/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowLanguageSetting(void)//语言设定
{
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			if ( VoiceMenuMgr.MenuPoint < 2 )
			{
				PLAY_VOICE_MULTISEGMENTS_FIXED(VolumeSetLanguageBuff[VoiceMenuMgr.MenuPoint]);
			}
			else
			{
				PLAY_VOICE_MULTISEGMENTS(VolumeSetLanguageBuff[VoiceMenuMgr.MenuPoint]);
			}
			VoiceMenuMgr.MenuPoint++;
		}
	}
}

/*******************************************************/
/*******************************************************/
//uint8_t VoiceWait=0;
/*******************************************************/
void ShowVolumeSetting(void)//声音设定
{	
	#ifdef Function_ScreenDisplay
	code uint8_t VoiceOnStr[]={ZF_1,ZF_xiaoshudian,HZ_yuyan,HZ_yin,HZ_kongge,HZ_kai,HZ_end};	//声音开	
//	code uint8_t VoiceOnStrEn[]={"1.Voice On"};							//声音开	
	code uint8_t VoiceOffStr[]={ZF_2,ZF_xiaoshudian,HZ_yuyan,HZ_yin,HZ_kongge,HZ_guanbi,HZ_end};	//声音关	
//	code uint8_t VoiceOffStrEn[]={"2.Voice Off"};							//声音关	
	
	DisHZ16x14Str(0,4,VoiceOnStr,NormalDisplay);
	DisHZ16x14Str(2,4,VoiceOffStr,NormalDisplay);
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		{
			if ( VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
			{
				PLAY_VOICE_MULTISEGMENTS(VolumeSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
				VoiceMenuMgr.MenuPoint++;
			}
		}
	}	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowMemoryUsage(void)//内存使用情况
{
	
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowManagerIdentify(void)//管理员认证
{
	uint8_t i;
	#ifdef Function_ScreenDisplay
	code uint8_t IdentifyManagerStr[]={HZ_yan,HZ_zheng,HZ_guan,HZ_li,HZ_yuan,HZ_end};
//	code uint8_t IdentifyManagerStrEn[]={"Identify Manager"};
	code uint8_t IdentifySuccessStr[]={HZ_yan,HZ_zheng,HZ_cheng,HZ_gong,HZ_end};
//	code uint8_t IdentifySuccessStrEn[]={"Identify Success"};		
	//code uint8_t InputAdminPasscodeStr[]={HZ_qing,HZ_shu,HZ_ru,HZ_guan,HZ_li,HZ_yuan,HZ_mi,HZ_ma,HZ_end};
	#endif
	if ( ManagerIdentifyMgr.Status == StartManagerIdentify )
	{
		if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
			)
		{
			#ifdef DirectIntoMainMenuInFactoryDefaultStatus
			ManagerIdentifyMgr.Status = ManagerIdentifySuccess;
			#else
			//ManagerIdentifyMgr.Status = ManagerIdentifySuccess;
			CurrentScreen = SCREEN_RegisterPasscodeUser;
			PasscodeUserRegisterMgr.OverTimeCnt = Def_GuiTimeDelayCnt5s;
			PasscodeUserRegisterMgr.Status = InputFirstPasscode;
			DataInputMgr.Status = InputIdle;
			PasscodeUserRegisterMgr.UserID = 0x01;
			PasscodeUserRegisterMgr.UserPriority = Master;
			
			PasscodeInputMgr.Point = 0x00;
			PasscodeInputMgr.PasscodeLen = 12;
			PasscodeInputMgr.Status = PasscodeInputStart;
			for (i=0;i<12;i++)
			{
				PasscodeInputMgr.InputBuff[i] = 0xFF;		//Initial passcode buffer
			}
			PasscodeUserRegisterMgr.TimeCnt = Def_WaitUserInputPasscodeTimeDelay;		
			//GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_THREESEGMENT(VOICE_PleaseEnter,VOICE_Administrator,VOICE_Password);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,30,InputPasscodeStr,NormalDisplay);
				#endif
			#endif
			
		}
		else
		{
			PasscodeInputMgr.Point = 0x00;
			PasscodeInputMgr.PasscodeLen = 12;
			PasscodeInputMgr.Status = PasscodeInputStart;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyPasscodeInput;
			//PasscodeUserIdentifyMgr.TimeCnt = 240;	//
			for (i=0;i<PasscodeInputMgr.PasscodeLen;i++)
			{
				PasscodeInputMgr.InputBuff[i]=0xff;
			}
			ManagerIdentifyMgr.Status = WaiForManagerPasscodeOrFP;
			FpIdentifyMgr.Status = FPMcmdStart;
			PLAY_VOICE_ONESEGMENT(VOICE_IdentifyMaster);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,30,IdentifyManagerStr,NormalDisplay);
			#endif
		}
	}
	else if ( ManagerIdentifyMgr.Status == WaiForManagerPasscodeOrFP )
	{

		GUI_PasscodeInputCreat(6,16);
		
		if (PasscodeInputMgr.Status == PasscodeInputEnd)
		{	
			ManagerIdentifyMgr.Status = ManagerIdentifyFail;	
			ManagerIdentifyMgr.PasscodeUserID = PasscodeIdendify(PasscodeInputMgr.InputBuff);
			if ( (ManagerIdentifyMgr.PasscodeUserID != 0x00 )
				&&(ManagerIdentifyMgr.PasscodeUserID < (DEF_MAX_PASSCODEMASTER+1) )
			   )
			{
				ManagerIdentifyMgr.Status = ManagerIdentifySuccess;
			}
			//GUI_Flag_RefreshLCD = bTRUE;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;

			if ( ManagerIdentifyMgr.Status == ManagerIdentifyFail )
			{
				PLAY_VOICE_ONESEGMENT(VOICE_IdentifyFail);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,IdentifyFailStr,NormalDisplay);
				#endif
				ManagerIdentifyMgr.TimeCnt = Def_MessageBoxTimeDelay;	
				if ( SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes < DEF_ManagerPasscodeIdentifyFailedTimesLimited )
				{
					SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes++;
				}	
			}
			else
			{
				PLAY_VOICE_TWOSEGMENT(VOICE_IdentifyMaster,VOICE_Success);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,IdentifySuccessStr,NormalDisplay);
				#endif
			}
			
		}
		else if (PasscodeInputMgr.Status == PasscodeInputExit)
		{
			ManagerIdentifyMgr.Status = ManagerIdentifyExit;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		}

		FpUserIdentify();
		if ( (FpIdentifyMgr.Status == success)
			&&(FpIdentifyMgr.UserID < (DEF_MAX_FPMASTER) )
			)
		{
			ManagerIdentifyMgr.Status = ManagerIdentifySuccess;
			FpIdentifyMgr.Status = FPMcmdStart;
			PLAY_VOICE_TWOSEGMENT(VOICE_IdentifyMaster,VOICE_Success);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,IdentifySuccessStr,NormalDisplay);
			#endif
			#ifdef Function_FPMbreathingLed
			FpIdentifyMgr.BreathingLED = 1;
			SetFPMbreathingLed(FPMbreathingLed_Green);
			#endif	
		}
		else if (( FpIdentifyMgr.Status == fail)
				||( (FpIdentifyMgr.Status == success)&&((FpIdentifyMgr.UserID+1) > DEF_MAX_FPMASTER ))
			)
		{
			if (FpIdentifyMgr.ErrorType == Error_SerialNumberMismatched ){
				ManagerIdentifyMgr.ErrorType = FPMserialNumberMismatched;
			}
			else{
				ManagerIdentifyMgr.ErrorType = MasterMismatched;
			}
			FpIdentifyMgr.Status = FPMcmdStart;
			ManagerIdentifyMgr.Status = ManagerIdentifyFail;
			ManagerIdentifyMgr.TimeCnt = Def_MessageBoxTimeDelay;
			//GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_ONESEGMENT(VOICE_IdentifyFail);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,IdentifyFailStr,NormalDisplay);
			#endif
			#ifdef Function_FPMbreathingLed
			SetFPMbreathingLed(FPMbreathingLed_Red);
			#endif
			if ( SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes < DEF_ManagerPasscodeIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes++;
			} 
		}
	}
	else if ( ManagerIdentifyMgr.Status == ManagerIdentifySuccess )
	{
		GoToMainMenu();
		SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes=0x00;
	}
	else if ( ManagerIdentifyMgr.Status == ManagerIdentifyFail )
	{
		if (--ManagerIdentifyMgr.TimeCnt < 1 )
		{
			if ( SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes >= DEF_ManagerPasscodeIdentifyFailedTimesLimited )
			{
				SafetyMonitorMgr.SystemLocked = bTRUE;
				SafetyMonitorMgr.SystemLockedTimeDelay = DEF_SystemLockedTime;
			}
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
			CardIdentifyMgr.Status = ReadingCardID;
			FpIdentifyMgr.Status = FPMcmdStart;
			CurrentScreen = SCREEN_Main;
			#ifdef Function_FPMbreathingLed
			SetFPMbreathingLed(FPMbreathingLed_Bule);
			#endif
		}
	}
	else if ( ManagerIdentifyMgr.Status == ManagerIdentifyExit )
	{
		PLAY_VOICE_ONESEGMENT(VOICE_ExitMenu);
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		CardIdentifyMgr.Status = ReadingCardID;
		FpIdentifyMgr.Status = FPMcmdStart;
		CurrentScreen = SCREEN_Main;
		#ifdef Function_FPMbreathingLed
		SetFPMbreathingLed(FPMbreathingLed_Bule);
		#endif
	}

}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowUnlockingModeSetting(void)//界面-解锁模式设置
{
	#ifdef Function_ScreenDisplay
	code uint8_t SingalModeStr[]={ZF_1,ZF_xiaoshudian,HZ_dan,HZ_yier,HZ_kai,HZ_suomen,HZ_end};
//	code uint8_t SingalModeStrEn[]={"1.Single"}; 
	code uint8_t DoubleModeStr[]={ZF_2,ZF_xiaoshudian,HZ_zu,HZ_he,HZ_kai,HZ_suomen,HZ_end};
//	code uint8_t DoubleModeStrEn[]={"2.Combination"};	
	code uint8_t SystemNoSecondIdentityStr[]={HZ_qing,HZ_shu,HZ_ru,HZ_di,HZ_er,HZ_shen,HZ_fen,HZ_end};
//	code uint8_t SystemNoSecondIdentityStrEn[]={"No 2nd Identity"};
	#endif
	if ( UnlockingModeMgr.Status == UnlockingModeSettingStart )
	{
		UnlockingModeMgr.Status = WaitForUserConfirmUnlockingMode;
	}
	else if ( UnlockingModeMgr.Status == WaitForUserConfirmUnlockingMode )
	{	
			#ifdef Function_ScreenDisplay
			DisHZ16x14Str(0,4,SingalModeStr,NormalDisplay);
			DisHZ16x14Str(2,4,DoubleModeStr,NormalDisplay);
			#endif
			if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
			{
				if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
				{
					PLAY_VOICE_MULTISEGMENTS(UnlockModeSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
					VoiceMenuMgr.MenuPoint++;
				}
			}
	}
	else if ( UnlockingModeMgr.Status == UserConfirmedUnlockingMode )//UnlockingModeSettingSuccess )
	{
		UnlockingModeMgr.TimeCnt = 0;
		if (UserIdentifyResultMgr.UnlockingMode == DoubleMode )
		{	
			if  ( IfSystemWithoutSecondIdentity() == bTRUE )
			{
				#ifdef Function_ScreenDisplay
					Clear_Screen();
					DisHZ16x14Str(3,16,SystemNoSecondIdentityStr,NormalDisplay);
				#endif
				UnlockingModeMgr.TimeCnt = Def_MessageBoxTimeDelay;
				UserIdentifyResultMgr.UnlockingMode = SingalMode;
			}
		}
		UnlockingModeMgr.Status = UnlockingModeSettingSuccess;
	}
	if ( UnlockingModeMgr.Status == UnlockingModeSettingSuccess )
	{
		if (UnlockingModeMgr.TimeCnt > 0)
		{
			UnlockingModeMgr.TimeCnt--;
		}
		else
		{
			SystemConfigSave();
			GoToSystemConfigMenu();
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowContinuedOpenModeSetting(void)//界面-组合开锁模式设置
{
	#ifdef Function_ScreenDisplay
	//code uint8_t TitleStr[]={HZ_changchang,HZ_kai,HZ_mo,HZ_si,HZ_end};							//常开模式
//	code uint8_t TitleStrEn[]={"Often Open"};							//常开模式	
	code uint8_t EnabledStr[]={ZF_1,ZF_xiaoshudian,HZ_changchang,HZ_kai,HZ_mo,HZ_si,HZ_kongge,HZ_kai,HZ_end};
//	code uint8_t EnabledStrEn[]={"1. ON"};
	code uint8_t DisabledStr[]={ZF_2,ZF_xiaoshudian,HZ_changchang,HZ_kai,HZ_mo,HZ_si,HZ_kongge,HZ_guanbi,HZ_end};
//	code uint8_t DisabledStrEn[]={"2. OFF"};

	DisHZ16x14Str(0,4,EnabledStr,NormalDisplay);
	DisHZ16x14Str(2,4,DisabledStr,NormalDisplay);	
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(ContinuedOpenModeSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowPickAlarmEnableSetting(void)//界面-防撬告警使能设置
{
	#ifdef Function_ScreenDisplay
	code uint8_t AntiPryingEnableStr[]={ZF_1,ZF_xiaoshudian,HZ_fangzi,HZ_qiao,HZ_bao,HZ_jing,HZ_kongge,HZ_kai,HZ_end};
//	code uint8_t AntiPryingEnableStrEn[]={"1.Enable"}; 
	code uint8_t AntiPryingDisableStr[]={ZF_2,ZF_xiaoshudian,HZ_fangzi,HZ_qiao,HZ_bao,HZ_jing,HZ_kongge,HZ_guanbi,HZ_end};
//	code uint8_t AntiPryingDisableStrEn[]={"2.Disable"};

	DisHZ16x14Str(0,4,AntiPryingEnableStr,NormalDisplay);
	DisHZ16x14Str(2,4,AntiPryingDisableStr,NormalDisplay);
	#endif
	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(PickAlarmSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
		/*
		PLAY_VOICE_MULTISEGMENTS(PickAlarmSetVoiceBuff[VoiceMenuMgr.MenuPoint]);
		
		if ( ++VoiceMenuMgr.MenuPoint >= VoiceMenuMgr.TotalMenuNum )
		{
			VoiceMenuMgr.MenuPoint = 0;
		}
		*/
	}

	
	if ( PickAlarmEnableMgr.Status == StartPickAlarmEnableSetting )
		{
			PickAlarmEnableMgr.Status = WaitForPickAlarmEnableSettingUserConfirm;
		}
	else if ( PickAlarmEnableMgr.Status == WaitForPickAlarmEnableSettingUserConfirm )
		{
			
		}
	else if ( PickAlarmEnableMgr.Status == PickAlarmEnableSettingSuccess )
		{			
			AntiPryingMgr.AntiPryingTrigger = bFALSE;
			SystemConfigSave();
			Config_AntiPrying_Interrupt();
			GoToSystemConfigMenu();	
		}
}

/*******************************************************/
/*******************************************************/
extern uint16_t KeyHoldTimeCnt;
/*******************************************************/
void ShowRestoreFactoryDefault(void)//界面-恢复出厂
{
	uint8_t i;
	#ifdef Function_ScreenDisplay
	code uint8_t RestoreFactoryDoingStr[]={HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_she,HZ_zhiwei,ZF_xiaoshudian,ZF_xiaoshudian,ZF_xiaoshudian,HZ_end};
//	code uint8_t RestoreFactoryDoingStrEn[]={"Restoring..."};
	#endif
	//RestoreFactoryDefaultMgr.Status = RestoreFactoryDefaultEXIT;

	if ( RestoreFactoryDefaultMgr.Status == StartRestoreFactoryDefault )
	{
		RestoreFactoryDefaultMgr.Status = WaitForRestoreFactoryDefaultUserConfirm;
		//GUI_Flag_RefreshLCD = bTRUE;
		RestoreFactoryDefaultMgr.Selection = NO;
		//SET_LRandConfirmLED_ON();
		//SET_AP_LED_ON();
	}
	else if ( RestoreFactoryDefaultMgr.Status == WaitForRestoreFactoryDefaultUserConfirm )
	{
		#ifdef Function_ScreenDisplay
		Clear_Screen();
		DisHZ16x14Str(2,10,ConfirmRestoreFactoryDefaultStr1,NormalDisplay);	
		DisHZ16x14Str(4,10,ConfirmRestoreFactoryDefaultStr2,NormalDisplay);
		#endif
		if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
		{
			if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
			{
				PLAY_VOICE_MULTISEGMENTS(RestoreFactoryDefaultVoiceBuff[VoiceMenuMgr.MenuPoint]);
				VoiceMenuMgr.MenuPoint++;
			}
		}
	}
	else if ( RestoreFactoryDefaultMgr.Status == ConfirmedToRestoreFactoryDefault )
	{	
		#ifdef Function_ScreenDisplay
		Clear_Screen();
		DisHZ16x14Str(3,14,RestoreFactoryDoingStr,NormalDisplay);	
		#endif
		//SET_ALLKEYLED_OFF();
		
		RestoreFactoryDefaultMgr.Status = SaveFPMserialNumber;

		PLAY_VOICE_ONESEGMENT(VOICE_RestoreFactoryDefault);

	}
	else if ( RestoreFactoryDefaultMgr.Status == SaveFPMserialNumber )
	{
		if ( SaveFPMserialNumberToMemory() == S_SUCCESS )
			{
				RestoreFactoryDefaultMgr.Status = ResetFPMuser;
				AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
				RestoreFactoryDefaultMgr.FailTimes = 0x00;
			}
		else{	
			RestoreFactoryDefaultMgr.Status = RestoreFactoryFail;
			PLAY_VOICE_TWOSEGMENT(VOICE_RestoreFactoryDefault,VOICE_Fail);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
			#endif
		}
	}	
	else if ( RestoreFactoryDefaultMgr.Status == ResetFPMuser )
		{
	
			DeleteAllFpFromFPM();
			if ( AllUserFpDeleteMgr.Status == DeleteAllFpUserSuccess )
			{
				RestoreFactoryDefaultMgr.Status = ResetCardUser;
				RestoreFactoryDefaultMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
				CheckMemoryMgr.FpUserNum = 0x00;
				CheckMemoryMgr.FpMasterNum = 0x00;	
				for (i=0;i<(DEF_MAX_FPMASTER+DEF_MAX_FPUSER);i++)
				{
					FpUserMemoryMgr[i].UserID = 0xFFFF;
					FpUserMemoryMgr[i].RegisterStatus = UnRegistered;
					FpUserMemoryMgr[i].UserPriority = Undefined;
				}
			}
			else if ( AllUserFpDeleteMgr.Status == DeleteAllFpUserFail )
			{
				if ( RestoreFactoryDefaultMgr.FailTimes < 4 )
				{
					RestoreFactoryDefaultMgr.FailTimes++;
					AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
					RefreshSystemSleepTime();
				}
				else
				{
					RestoreFactoryDefaultMgr.Status = RestoreFactoryFail;
					PLAY_VOICE_TWOSEGMENT(VOICE_RestoreFactoryDefault,VOICE_Fail);
					#ifdef Function_ScreenDisplay
					Clear_Screen();
					DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
					#endif
				}
			}	
		}
	
	else if ( RestoreFactoryDefaultMgr.Status == ResetCardUser )
		{			
			if (RestoreFactoryDefaultMgr.TimeCnt-- < 1 )
			{
				DeleteAllCardUserfromMemory();
 				RestoreFactoryDefaultMgr.Status = ResetPasscodeUser;
				RestoreFactoryDefaultMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
				CheckMemoryMgr.CardUserNum = 0x00;
			}	
		}
	else if ( RestoreFactoryDefaultMgr.Status == ResetPasscodeUser )
		{	
			if (RestoreFactoryDefaultMgr.TimeCnt-- < 1 )
			{
				RestoreFactory_Managerdata();
				DeleteAllPasscodeMasterfromMemory();
				DeleteAllPasscodeUserfromMemory();
				SystemConfigReset();
				#ifdef Function_EventLog
				DeleteAllLog();
				#endif
				
				RestoreFactoryDefaultMgr.Status = RestoreFactorySuccess;
				RestoreFactoryDefaultMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
				CheckMemoryMgr.PasscodeMasterNum = 0x00;
				CheckMemoryMgr.PasscodeUserNum = 0x00;
				//GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_TWOSEGMENT(VOICE_RestoreFactoryDefault,VOICE_Success);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
				#endif
			}
		}
	
	else if ( RestoreFactoryDefaultMgr.Status == RestoreFactorySuccess )
		{

			if (RestoreFactoryDefaultMgr.TimeCnt-- < 1 )
			{
				CurrentScreen = SCREEN_Main;
				RefreshSystemSleepTime();
				KeyHoldTimeCnt=0;
				PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
				FpIdentifyMgr.Status = FPMcmdStart;
				CardIdentifyMgr.Status = ReadingCardID;

			}
		}
	else if ( RestoreFactoryDefaultMgr.Status == RestoreFactoryFail )
		{
			if (RestoreFactoryDefaultMgr.TimeCnt-- < 1 )
			{
				CurrentScreen = SCREEN_Main;
				KeyHoldTimeCnt=0;
				PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
				FpIdentifyMgr.Status = FPMcmdStart;
				CardIdentifyMgr.Status = ReadingCardID;
			}
		}
	else if ( RestoreFactoryDefaultMgr.Status == RestoreFactoryDefaultEXIT )
		{
			GoToSystemConfigMenu();
		}

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowInitialization(void)//界面-初始化
{
	
	uint8_t i;
	#ifdef Function_ScreenDisplay
	code uint8_t SystenRebootStr[]={HZ_xi,HZ_tong,HZ_qidong,HZ_dong,HZ_end};
//	code uint8_t SystenRebootStr_En[]={"[Rebooting]"};
	code uint8_t FingprintStr[]={HZ_zhi,HZ_wen,HZ_end}; 
//	code uint8_t FingprintStr_En[]={"Fingerprint"}; 
	code uint8_t CardReaderStr[]={HZ_du,HZ_ka,HZ_end}; 
//	code uint8_t CardReaderStr_En[]={"Card Reader"}; 	
	code uint8_t EepromStr[]={HZ_nei,HZ_cun,HZ_end}; 
//	code uint8_t EepromStr_En[]={"Memory"}; 
	code uint8_t TouchStr[]={HZ_chumo,HZ_motou,HZ_end}; 
//	code uint8_t TouchStr_En[]={"Touch"}; 
	code uint8_t SystemCheckPassStr[]={HZ_xi,HZ_tong,HZ_zhengque,HZ_changchang,HZ_end}; 
//	code uint8_t SystemCheckPassStr_En[]={"System OK"}; 
	code uint8_t SystemCheckFailStr[]={HZ_xi,HZ_tong,HZ_bu,HZ_zhengque,HZ_changchang,HZ_end}; 
//	code uint8_t SystemCheckFailStr_En[]={"System NG"};
	#endif 	
	
	if ( InitializationMgr.Status == StartInitialization )
	{
		//SET_ALLKEYLED_OFF();
		#ifdef Function_ScreenDisplay
		DisEN16x8Str(1,28,"[",NormalDisplay);	//display "["
		DisHZ16x14Str(1,36,SystenRebootStr,NormalDisplay);	//display "reboot"
		DisEN16x8Str(1,92,"]",NormalDisplay);	//display "]"
		#endif
		CheckMemoryMgr.Status = StartCheckMemory;
		InitializationMgr.Status = MemoryCheck;
	}
	else if ( InitializationMgr.Status == MemoryCheck )
	{
		#ifdef Function_ScreenDisplay
		DisHZ16x14Str(4,36,EepromStr,NormalDisplay);//display "EEPROM TEST"
		#endif
		SystemConfigLoad();
		ReadLockBrand();
		InitializationMgr.MemoryCheckPass = bTRUE;
		InitializationMgr.Status = MemoryCheckWait;
		InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
		PLAY_VOICE_ONESEGMENT_FIXED(VOICE_Beep);
	}
	else if ( InitializationMgr.Status == MemoryCheckWait )
	{
		#ifdef Function_ScreenDisplay
		if (InitializationMgr.MemoryCheckPass == bTRUE)
		{	
			DisImage(4,75,16,16,Icon_Pass,NormalDisplay);
		}
		else
		{
			DisImage(4,75,16,16,Icon_Fail,NormalDisplay);
		}
		#endif
		if (--InitializationMgr.TimeCnt < 1 )
		{
			InitializationMgr.Status = FingerPrintCheck;
			#ifdef Function_ScreenDisplay
			Clear_Screen_Page(4);
			Clear_Screen_Page(5);
			DisHZ16x14Str(4,36,FingprintStr,NormalDisplay);	//display "FPM TEST"
			#endif
		}
	}
	else if ( InitializationMgr.Status == FingerPrintCheck )
	{

		GUI_GetUserNumList();		//read user number
		if ( CheckMemoryMgr.Status == CheckMemorySuccess )
		{
			//SET_KEYLED_ON(KEY_TWO);
			#ifdef Function_FPMbreathingLed
			FpmAckMgr.Status = WaitACK;
			FPM_SetSecurityLevel(1); 	//
			for (i=0;i<25;i++)
			{
				Hardware_DelayMs(10);
				FPM_Mgr_Task();
				if ( FpmAckMgr.Status == GotACK )
				{
					break;
				}
			}
			#endif

			InitializationMgr.FingerPrintCheckPass = bTRUE;
			InitializationMgr.Status = FingerPrintCheckWait;
			InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
			PLAY_VOICE_ONESEGMENT_FIXED(VOICE_Beep);
			
		}
		else if  ( CheckMemoryMgr.Status == CheckMemoryFail )
		{	
			//CheckMemoryMgr.Status = StartCheckMemory;
			//InitializationMgr.Status = StartInitialization;//redo
			InitializationMgr.FingerPrintCheckPass = bFALSE;
			InitializationMgr.Status = FingerPrintCheckWait;
			InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
			DEF_Fail_Beep;
		}
	}
	else if  ( InitializationMgr.Status == FingerPrintCheckWait )
	{
		#ifdef Function_ScreenDisplay
		if (InitializationMgr.FingerPrintCheckPass == bTRUE)
		{	
			DisImage(4,75,16,16,Icon_Pass,NormalDisplay);
		}
		else
		{
			DisImage(4,75,16,16,Icon_Fail,NormalDisplay);
		}
		#endif
		if (--InitializationMgr.TimeCnt < 1 )
		{
			InitializationMgr.Status = CardReaderCheck;
			#ifdef Function_ScreenDisplay
			Clear_Screen_Page(4);
			Clear_Screen_Page(5);	
			DisHZ16x14Str(4,36,CardReaderStr,NormalDisplay); 
			#endif
		}
	}
	else if ( InitializationMgr.Status == CardReaderCheck )
	{
		//SET_KEYLED_ON(KEY_THREE);
		InitializationMgr.CardReaderCheckPass = bTRUE;
		InitializationMgr.Status = CardReaderCheckWait;
		InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
		PLAY_VOICE_ONESEGMENT_FIXED(VOICE_Beep);
	}
	else if ( InitializationMgr.Status == CardReaderCheckWait )
	{
		#ifdef Function_ScreenDisplay
		if (InitializationMgr.CardReaderCheckPass == bTRUE)
		{	
			DisImage(4,75,16,16,Icon_Pass,NormalDisplay);
		}
		else
		{
			DisImage(4,75,16,16,Icon_Fail,NormalDisplay);
		}
		#endif
		if (--InitializationMgr.TimeCnt < 1 )
		{
			InitializationMgr.Status = TouchCheck;
			#ifdef Function_ScreenDisplay
			Clear_Screen_Page(4);
			Clear_Screen_Page(5);	
			DisHZ16x14Str(4,36,TouchStr,NormalDisplay);
			#endif
		}
	}
	else if ( InitializationMgr.Status == TouchCheck )
		{
			//SET_KEYLED_ON(KEY_FOUR);
			InitializationMgr.TouchCheckPass = bTRUE;
			InitializationMgr.Status = TouchCheckWait;
			InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
 
			PLAY_VOICE_ONESEGMENT_FIXED(VOICE_Beep);
		}
	else if ( InitializationMgr.Status == TouchCheckWait )
		{
			#ifdef Function_ScreenDisplay
			if (InitializationMgr.TouchCheckPass == bTRUE)
			{	
				DisImage(4,75,16,16,Icon_Pass,NormalDisplay);
			}
			else
			{
				DisImage(4,75,16,16,Icon_Fail,NormalDisplay);
			}
			#endif
			if (--InitializationMgr.TimeCnt < 1 )
			{
				InitializationMgr.Status = CheckFinished;
				InitializationMgr.TimeCnt = Def_GuiTimeDelayCnt1s;
				#ifdef Function_ScreenDisplay
				Clear_Screen_Page(4);
				Clear_Screen_Page(5);
				if( (InitializationMgr.FingerPrintCheckPass == bTRUE )
					&&(InitializationMgr.MemoryCheckPass == bTRUE )
					&&(InitializationMgr.CardReaderCheckPass == bTRUE )
					&&(InitializationMgr.TouchCheckPass == bTRUE ) )
				{
					DisHZ16x14Str(4,36,SystemCheckPassStr,NormalDisplay);
				}
				else
				{
					DisHZ16x14Str(4,28,SystemCheckFailStr,NormalDisplay);
				}
				#endif
			}
		}

	else if ( InitializationMgr.Status == CheckFinished )
	{
		if (--InitializationMgr.TimeCnt < 1 )
		{
			if (( BatteryMgr.BatteryLevel == LEVEL_0 )||( BatteryMgr.BatteryLevel == LEVEL_1 ))
			{
				PLAY_VOICE_ONESEGMENT(VOICE_PleaseReplaceTheBattery);
				BatteryMgr.PostLowBattery = bFALSE;
				BatteryMgr.TimeCnt = Def_MessageBoxTimeDelay;
				CurrentScreen = SCREEN_LowBattery;	
			}
			else
			{
				CurrentScreen = SCREEN_Main;
				PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
				FpIdentifyMgr.Status = FPMcmdStart;
				CardIdentifyMgr.Status = ReadingCardID;
              	PLAY_VOICE_ONESEGMENT_FIXED(VOICE_POWERON);
			}
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowSystemLocked(void)//界面-系统锁定
{
	#ifdef Function_ScreenDisplay
	uint16_t Temp;
	code uint8_t SystemLockedStr[]={HZ_xi,HZ_tong,HZ_suomen,HZ_ding,HZ_end};
//	code uint8_t SystemLockedStrEn[]={"System Lock"};
	#endif
	if ( SafetyMonitorMgr.SystemLockedTimeDelay > 0x0000 )
	{
		#ifdef Function_TuyaWifi
		if( SafetyMonitorMgr.SystemLockedTimeDelay == DEF_SystemLockedTime )
		{
			WifiMgr.PostMgr.Flag.Bits.Alarm = 1;
			WifiMgr.PostMgr.AlarmType = ErrorAlarm;
		}
		#endif
		SafetyMonitorMgr.SystemLockedTimeDelay--;
		if ( SafetyMonitorMgr.SystemLockedTimeDelay > (DEF_SystemLockedTime-625))
		{
			if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
			{
				if(VoiceMgr.Enable == bFALSE)
				{
					VoiceMgr.Enable = bTRUE;
					PLAY_VOICE_THREESEGMENT(VOICE_Alarm,VOICE_Alarm,VOICE_Alarm);
					VoiceMgr.Enable = bFALSE;
				}
				else
				{
					PLAY_VOICE_THREESEGMENT(VOICE_Alarm,VOICE_Alarm,VOICE_Alarm);
				}
			}
		}
	}
	else
	{
		SafetyMonitorMgr.FpIdentifyFailedTimes = 0x00;
    SafetyMonitorMgr.CardIdentifyFailedTimes = 0x00;
    SafetyMonitorMgr.PasscodeIdentifyFailedTimes = 0x00;
		SafetyMonitorMgr.ManagerPasscodeIdentifyFailedTimes = 0x00;
		SafetyMonitorMgr.SystemLocked = bFALSE;
		CurrentScreen = SCREEN_Main;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		FpIdentifyMgr.Status = FPMcmdStart;
		CardIdentifyMgr.Status = ReadingCardID;
		STOP_VOICEPLAY();
	}
	#ifdef Function_ScreenDisplay
	DisImage(0,50,27,24,Icon_Warning,RED_COLOR);
	DisHZ16x14Str(4,36,SystemLockedStr,NormalDisplay);
	Temp = (SafetyMonitorMgr.SystemLockedTimeDelay*2)/125;
	DisOneDigital16x8(6,52,Temp/100,NormalDisplay);
	DisOneDigital16x8(6,60,(Temp%100)/10,NormalDisplay);
	DisOneDigital16x8(6,68,Temp%10,NormalDisplay);
	#endif
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
#ifdef Function_ScreenDisplay
 code uint8_t AntiPryingAlarmStr[]={HZ_fangzi,HZ_qiao,HZ_bao,HZ_jing,HZ_end};
// code uint8_t AntiPryingAlarmStrEn[]={"Alarm"};
 //uint8_t RemovalAlarmStr1[]={HZ_qing,HZ_shu,HZ_ru,HZ_ren,HZ_lianbu,ZF_douhao,HZ_zhi,HZ_wen,HZ_end}; //请输入人脸
 //uint8_t RemovalAlarmStr1En[]={"Enter Identity"};	
 code uint8_t RemovalAlarmStr2[]={HZ_kongge,HZ_zhi,HZ_wen,HZ_mi,HZ_ma,HZ_huo,HZ_ka,HZ_pian,HZ_kongge,HZ_kongge,HZ_tui,HZ_chu,HZ_end};		
// code uint8_t RemovalAlarmStr2En[]={"to Removal"};
#endif
void ShowPickLockAlarm(void)//界面-防撬告警
{
	bool_t Flag_ExitPickLockAlarm;

	RefreshSystemSleepTime();		//reset system sleep time

	#ifdef Function_ScreenDisplay
	if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
	{
		DisHZ16x14Str(0,36,AntiPryingAlarmStr,NormalDisplay);
		Clear_Screen_Page(5);
		DisHZ16x14Str(3,0,RemovalAlarmStr2,NormalDisplay); 
	}
	#endif 	
	
	Flag_ExitPickLockAlarm = bFALSE;
	if (( UART2_Mgr.TxLength == 0x00 )&& ( CardIdentifyMgr.CardDetectIntervalTimeCnt ==0 ) )	//FPM Cmd is sent out
	{
		CardIdentifyMgr.CardDetectIntervalTimeCnt = Def_CardDetectIntervalTime;
		
		CardUserIdentify();
	}

		
	if ( CardIdentifyMgr.Status == Success)
	{
		Flag_ExitPickLockAlarm = bTRUE;
	}
	else if ( CardIdentifyMgr.Status == Fail)
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			Flag_ExitPickLockAlarm = bTRUE;
		}
		else
		{
			CardIdentifyMgr.Status = ReadingCardID;
		}
	}

	FpUserIdentify();
	if ( FpIdentifyMgr.Status == success)
	{
		Flag_ExitPickLockAlarm = bTRUE;	
	}
	else if ( FpIdentifyMgr.Status == fail)
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			Flag_ExitPickLockAlarm = bTRUE;		
		}
		else
		{
			FpIdentifyMgr.Status = FPMcmdStart;
		}
	}

	PasscodeUserIdentify();
	if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifySuccess)
	{
		Flag_ExitPickLockAlarm = bTRUE;
	}
	else if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyFail )
	{
		if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
		{
			Flag_ExitPickLockAlarm = bTRUE;
		}
		else
		{
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		}
	}

	if ( AntiPryingMgr.TimeCnt > 0 )
	{
		#ifdef Function_TuyaWifi
		if( AntiPryingMgr.TimeCnt == DEF_AntiPryingTime )
		{
			WifiMgr.PostMgr.Flag.Bits.Alarm = 1;
			WifiMgr.PostMgr.AlarmType = PryAlarm;
		}
		#endif
		AntiPryingMgr.TimeCnt--;
	}
	else{
		Flag_ExitPickLockAlarm = bTRUE;
	}
	
	if ( Flag_ExitPickLockAlarm == bTRUE )
	{
		STOP_VOICEPLAY();
		#ifdef Function_FPMbreathingLed
		SetFPMbreathingLed(FPMbreathingLed_Bule);
		#endif
		INT1_RST();
		AntiPryingMgr.AntiPryingTrigger = bFALSE;
		AntiPryingMgr.AntiPryingSignalRelease = bFALSE;
		UserIdentifyResultMgr.CardIdentifyStatus = S_FAIL;
		UserIdentifyResultMgr.FPIdentifyStatus = S_FAIL;
		UserIdentifyResultMgr.PasscodeIdentifyStatus = S_FAIL;
		CurrentScreen = SCREEN_Main;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		FpIdentifyMgr.Status = FPMcmdStart;
		CardIdentifyMgr.Status = ReadingCardID;
		RefreshSystemSleepTime();		//reset system sleep time
	}
	else
	{
		if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
		{
			PLAY_VOICE_THREESEGMENT(VOICE_Alarm,VOICE_Alarm,VOICE_Alarm);
		}
	}
}

/*******************************************************/
/*******************************************************/
void ShowAgingTest(void)//界面-老化测试
{
	#ifdef Function_ScreenDisplay
	uint8_t Str1[]={"AGING TEST"}; 
	uint8_t Str2[]={"MOTOR:"}; 
	uint8_t Str3[]={"INPUT:  . V"};
	uint8_t Str4[]={"SLIDE:"};

	DisEN16x8Str(0,16,Str1,NormalDisplay);
	#endif
	GUI_MotorAgingTest();
	#ifdef Function_ScreenDisplay
	DisEN16x8Str(4,0,Str2,NormalDisplay);
	DisOneDigital16x8(4,56,AgingTestMgr.TestTimes/1000,NormalDisplay);
	DisOneDigital16x8(4,64,AgingTestMgr.TestTimes%1000/100,NormalDisplay);
	DisOneDigital16x8(4,72,AgingTestMgr.TestTimes%100/10,NormalDisplay);
	DisOneDigital16x8(4,80,AgingTestMgr.TestTimes%10,NormalDisplay);

	DisEN16x8Str(6,0,Str3,NormalDisplay);	
	DisOneDigital16x8(6,54,BatteryMgr.BatteryVoltage%100/10,NormalDisplay);
	DisOneDigital16x8(6,70,BatteryMgr.BatteryVoltage%10,NormalDisplay);
	#endif
}
/*******************************************************/
/*******************************************************/
/*******************************************************/

void ShowEventLogBySequence(void)//界面-按顺序界面-事件日志
{
	#ifdef Function_EventLog
	code uint8_t VoiceReportLogBuff[][5]=
	{	
		{VOICE_Press,VOICE_Two,VOICE_Previous,DEF_VoiceSegmentEndFlag},
		{VOICE_Press,VOICE_Eight,VOICE_Next,DEF_VoiceSegmentEndFlag},
		{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag},
		{VOICE_Mute20ms,DEF_VoiceSegmentEndFlag}
	};
		
	uint8_t VoiceStr_NoLog[]={VOICE_No,VOICE_Log,DEF_VoiceSegmentEndFlag};
	uint8_t NoEventLogStr[]={HZ_wu,HZ_ji,HZ_lu,HZ_end};

	if (CheckEventLogBySequenceMgr.Status == SearchEventLogWithSequence)
	{
		SearchEventLogBySequence();
		CheckEventLogBySequenceMgr.Status = VoiceReportEventLogWithSequence;
		if ( CheckEventLogBySequenceMgr.MatchedEventLogNum > 0x0000 )
		{
			LogMgr.DisplayPoint = CheckEventLogBySequenceMgr.StartEventLogPoint;
			LogMgr.SavedDisplayPoint = LogMgr.DisplayPoint+1;	//for reload Display Log
			CheckEventLogBySequenceMgr.OffsetEventLogNum = 0x0000;
			VoiceMenuMgr.MenuPoint = 0;
			VoiceMenuMgr.TotalMenuNum = 3;		
		}
		else
		{
			PLAY_VOICE_MULTISEGMENTS(VoiceStr_NoLog);
		}
	}
	else if (CheckEventLogBySequenceMgr.Status == VoiceReportEventLogWithSequence )
	{
		if ( CheckEventLogBySequenceMgr.MatchedEventLogNum == 0x0000 )
		{
				DisHZ16x14Str(3,40,NoEventLogStr,NormalDisplay);
		}
		else
		{
			if ( LogMgr.SavedDisplayPoint != LogMgr.DisplayPoint )
			{
				LogMgr_ReadLog(LogMgr.DisplayPoint,&LogMgr.DisplayLog.FlagHighByte);
				LogMgr.SavedDisplayPoint = LogMgr.DisplayPoint;
				VoiceReportLogMgr.Status = ReportLogInit;
				Clear_Screen();
			}
			
			DisplayOneEventLog(CheckEventLogBySequenceMgr.OffsetEventLogNum+1);
			
			if ( VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
			{
				if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
				{
					PLAY_VOICE_MULTISEGMENTS(VoiceReportLogBuff[VoiceMenuMgr.MenuPoint]);
					VoiceMenuMgr.MenuPoint++;
				}
				SystemPowerMgr.SleepDelayTimerCnt = Def_GuiTimeDelayCnt45s;
			}
			else
			{
			 //	VoiceReportOneEventLog(CheckEventLogBySequenceMgr.OffsetEventLogNum+1);
			}
		}
	}
	#endif
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void ShowClearEventLog(void)//界面-清除事件日志
{
	#ifdef Function_EventLog
	uint8_t DELing[]={HZ_shan,HZ_chufa,HZ_zhong,ZF_xiaoshudian,ZF_xiaoshudian,ZF_xiaoshudian,HZ_end};
	if (LogDeleteMgr.Status == StartLogDelete)
	{
		LogDeleteMgr.DeleteSectorNum = 0;
		LogDeleteMgr.Percentage = 0;
		LogDeleteMgr.Status = WaitforLogDeleteCofirm;
		PLAY_VOICE_MULTISEGMENTS(ConfirmDeleteVoiceBuff);
	}
	else if (LogDeleteMgr.Status == WaitforLogDeleteCofirm)
	{
		#ifdef Function_ScreenDisplay
		DisHZ16x14Str(2,16,ConfirmDEL,NormalDisplay);
		DisHZ16x14Str(4,16,CancelDEL,NormalDisplay);
		#endif
	}
	else if (LogDeleteMgr.Status == LogDeleting)
	{
		if ( LogDeleteMgr.Percentage < 100 )
		{
			if ( LogDeleteMgr.Percentage == 0 )
			{
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,30,DELing,NormalDisplay);
				#endif
				if ( DeleteAllLog() != S_SUCCESS )
				{
					if ( DeleteAllLog() != S_SUCCESS )
					{
						DeleteAllLog();
					}
				}
			}
			LogDeleteMgr.Percentage++;
		}
		else
		{
			LogDeleteMgr.Status = LogDeleteSuccess;
			LogDeleteMgr.Percentage = 0;
			RefreshSystemSleepTime();
			PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Success);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
			#endif
		}
	}
	else if (LogDeleteMgr.Status == LogDeleteSuccess)
	{
		if ( LogDeleteMgr.Percentage < 100 )
		{
			LogDeleteMgr.Percentage++;
		}
		else 
		{
			LogDeleteMgr.Status = QuitLogDelete;
		}
	}
	else
	{
		GotoLogMenu();
	}
	#endif
}

/*******************************************************/
/*******************************************************/


/*******************************************************/
/*******************************************************/
void GUI_Button_Monitor(void)//GUI按钮监视器
{
	//DEBUG_MARK;
	uint8_t i;
	uint8_t Offset;
	if (CardIdentifyMgr.CardDetectIntervalTimeCnt > (Def_CardDetectIntervalTime-8))
	{
		//return;
	}
	
	if ( MotorMgr.MotorStatus != IDLE )
	{
		return;
	}

	gui_keycode = Key_Scan();

	DEBUG_MARK;

	if ( gui_keycode != KEY_NONE )
	{
		RefreshSystemSleepTime();
	}
	switch ( CurrentScreen)
	{
		case SCREEN_Main:

			if ( gui_keycode == KEY_INSIDEBUTTON )
			{
				#ifdef INSIDEBUTTONINTOMAINMENU
				DEF_ButtonPress_Voice;
				CurrentScreen = SCREEN_ManagerIdentify;
				ManagerIdentifyMgr.Status = StartManagerIdentify;
				#endif
			}
			else if ( gui_keycode == KEY_INSIDEBUTTON_HOLD_LONG )
			{
				CurrentScreen = SCREEN_RestoreFactoryDefault;
				//RestoreFactoryDefaultMgr.Status = StartRestoreFactoryDefault;
				RestoreFactoryDefaultMgr.Status = ConfirmedToRestoreFactoryDefault;
			}
			#ifdef Function_DoorBellTouchIndependented
			else if ( gui_keycode == KEY_DOORBELL )
			{	
				PLAY_VOICE_DOORBELL();
				DoorBellMgr.TimeCnt = 150;
				CurrentScreen = SCREEN_DoorBell;
			}
			#endif

			if  ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
			{
				if ( gui_keycode < 10 )		//from KEY0~~KEY9
				{
					if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
					{
						
						PasscodeInputMgr.Point = 0x01;
						PasscodeInputMgr.PasscodeLen = 16;
						PasscodeInputMgr.Status = PasscodeInputStart;
						PasscodeInputMgr.InputBuff[0] = gui_keycode;
						for (i=1;i<PasscodeInputMgr.PasscodeLen;i++)
						{
							PasscodeInputMgr.InputBuff[i]=0xff;
						}
						
						PasscodeUserIdentifyMgr.Status = PasscodeIdentifyStart;
						DEF_ButtonPress_Voice;
					}
				 }
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					if ( g_ASTERISK_PressedOnMainScreen == bFALSE )
					{
						g_ASTERISK_PressedOnMainScreen = bTRUE;
					}
					else 
					{
						SystemPowerMgr.SleepDelayTimerCnt = 0x0000;
						SystemPowerMgr.SleepSource = UserForced;
					}
				}
				else if ( gui_keycode == KEY_POUNDSIGN )
				{
					if ( g_ASTERISK_PressedOnMainScreen == bTRUE )
					{	
						 DEF_ButtonPress_Voice;
						 CurrentScreen = SCREEN_ManagerIdentify;
						 ManagerIdentifyMgr.Status = StartManagerIdentify;
						 g_ASTERISK_PressedOnMainScreen = bFALSE;
					}
					else
					{
						#ifdef Function_DoorBellTouchUsedPOUNDSIGN
						PLAY_VOICE_DOORBELL();	//DOORBELL
						DoorBellMgr.TimeCnt = 150;
						CurrentScreen = SCREEN_DoorBell;
						#else
						DEF_ButtonPress_Voice;
						#endif
					}	
				}
			}
			else if (( gui_keycode != KEY_NONE )		//from KEY0~~KEY9
								&&( gui_keycode != KEY_DOORBELL )
							)
			{
				g_ASTERISK_PressedOnMainScreen = bFALSE;
				if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput )
				{
					DEF_ButtonPress_Voice;
					//PasscodeUserIdentifyMgr.TimeCnt = 240;	//
					GUI_PasscodeInputButtonMonitor(gui_keycode);//GUI_密码输入按钮监视器
				}
			}
			
			break;
			
		case SCREEN_MainMenu:

				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToFpMenu();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToPasswordMenu();
				}
				else if ( gui_keycode == KEY_THREE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToCardMenu();
				}
				else if ( gui_keycode == KEY_FOUR )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToSystemConfigMenu();
				}
				#ifdef Function_TuyaWifi
				else if (gui_keycode == KEY_FIVE)
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_NetWorkLink;
					WifiMgr.Link.Status = LinkStart;
					Wifi_Rst();
				}
				#endif
//				else if ( gui_keycode == KEY_FIVE )
//				{
//					DEF_ButtonPress_Voice;
//					Hardware_DelayMs(100);
//					GotoLogMenu();
//				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					#ifdef Function_FPMbreathingLed
					FpIdentifyMgr.BreathingLED = 0;
					SetFPMbreathingLed(FPMbreathingLed_Bule);
					#endif
					//DEF_ButtonPress_Voice;
					//FpmAckMgr.Status = WaitACK;
					PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
					CardIdentifyMgr.Status = ReadingCardID;
					FpIdentifyMgr.Status = FPMcmdStart;
					PLAY_VOICE_ONESEGMENT(VOICE_ExitMenu);
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_Main;
					RefreshSystemSleepTime();
				}

				break;
				
		case SCREEN_EventLogMenu:
			
				#ifdef Function_EventLog
				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_CheckEventLogBySequence;
					CheckEventLogBySequenceMgr.Status = SearchEventLogWithSequence;
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_DeleteEventLog;
					LogDeleteMgr.Status = StartLogDelete;
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToMainMenu();
				}
				#endif
				
				break;	
				
		case SCREEN_FpMenu:
				if( gui_keycode == KEY_ONE || gui_keycode == KEY_TWO || gui_keycode == KEY_THREE )
				{
					#ifdef Function_FPMbreathingLed
					FpIdentifyMgr.BreathingLED = 0;
					SetFPMbreathingLed(FPMbreathingLed_Bule);
					#endif
				}
			
				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					FpUserRegisterMgr.Status = StartFpUserRegister;
					CurrentScreen = SCREEN_RegisterMasterFp;
					
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					FpUserRegisterMgr.Status = StartFpUserRegister;
					CurrentScreen = SCREEN_RegisterUserFp;
				}
				else if ( gui_keycode == KEY_THREE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					AllUserFpDeleteMgr.Status = StartAllUserFpDelete;
					CurrentScreen = SCREEN_DeleteAllUserFp;
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToMainMenu();
				}
				break;
					
		case SCREEN_CardUserMenu:

				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_RegisterCardUser;
					CardUserRegisterMgr.Status = StartCardUserRegister;
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_DeleteAllCardUser;
					AllCardUserDeleteMgr.Status = StartAllCardUserDelete;
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToMainMenu();	
				}
				break;
				
		case SCREEN_PasscodeMenu:	

				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_RegisterPasscodeUser;
					PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
					PasscodeUserRegisterMgr.UserPriority = Master;
				}
				
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_RegisterPasscodeUser;
					PasscodeUserRegisterMgr.Status = StartPasscodeUserRegister;
					PasscodeUserRegisterMgr.UserPriority = User;
				}
				else if ( gui_keycode == KEY_THREE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					CurrentScreen = SCREEN_DeleteAllPasscodeUser;
					AllPasscodeUserDeleteMgr.Status = StartAllPasscodeUserDelete;
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToMainMenu();
				}
			break;
					
		case SCREEN_SystemConfigMenu:
			
				Offset = 0;
					
//					if ( gui_keycode == KEY_ONE  )
//					{
//						DEF_ButtonPress_Voice;
//						Hardware_DelayMs(100);
//						CurrentScreen = SCREEN_TimeSetting;
//							#ifdef Function_USE_Internal_RTC
//							SystemTime = UTCToSystemtime(G_SystemUTCTime);
//							#elif defined Function_USE_External_RTC
//							PCF8563_ReadTime();
//							#endif
//							GotSystemTime();	
//					}
		#ifdef Function_VoiceEnabledEnglish
					Offset = 1;
					if ( gui_keycode == KEY_ONE )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						CurrentScreen = SCREEN_LanguageSetting;
						VoiceMenuMgr.MenuPoint=0;
						VoiceMenuMgr.TotalMenuNum = 3;
					}	
		#endif
					if ( gui_keycode == KEY_ONE+Offset )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						CurrentScreen = SCREEN_VolumeSetting;
						VoiceMenuMgr.MenuPoint=0;
						VoiceMenuMgr.TotalMenuNum = 3;
					}					
					else if ( gui_keycode == KEY_TWO+Offset )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						CurrentScreen = SCREEN_ContinueOpenSetting;
						VoiceMenuMgr.MenuPoint=0;
						VoiceMenuMgr.TotalMenuNum = 3;
					}
					else if ( gui_keycode == KEY_THREE+Offset )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						CurrentScreen = SCREEN_UnlockingModeSetting;
						UnlockingModeMgr.Status = UnlockingModeSettingStart;
						VoiceMenuMgr.MenuPoint=0;
						VoiceMenuMgr.TotalMenuNum = 3;
					}
					else if ( gui_keycode == KEY_FOUR+Offset )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						CurrentScreen = SCREEN_PickAlarmEnableSetting;
						PickAlarmEnableMgr.Status = StartPickAlarmEnableSetting;
						VoiceMenuMgr.MenuPoint=0;
						VoiceMenuMgr.TotalMenuNum = 3;
					}
					else if ( gui_keycode == KEY_FIVE+Offset )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						CurrentScreen = SCREEN_RestoreFactoryDefault;
						RestoreFactoryDefaultMgr.Status = StartRestoreFactoryDefault;
						VoiceMenuMgr.MenuPoint=0;
						VoiceMenuMgr.TotalMenuNum = 2;	
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						GoToMainMenu();	
					}

				break;
				
		#ifdef Function_TuyaWifi					
		case SCREEN_NetWorkLink:

			if (gui_keycode == KEY_ASTERISK)
			{
				DEF_ButtonPress_Voice;
				Hardware_DelayMs(100);
				WifiMgr.Link.Status = LinkIdle;
				GoToMainMenu();
			}

			break;
		#endif

		case SCREEN_RegisterUserFp:
				
				if ( FpUserRegisterMgr.Status == InputUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}
				if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToFpMenu();	
				}
				
			break;
			
		case SCREEN_DeleteUserFp:
				if ( FpUserDeleteMgr.Status == InputUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}

			break;

		case SCREEN_DeleteAllUserFp:
			
			if ( AllUserFpDeleteMgr.Status == WaitForUserConfirmDeleteAllFP )
				{
					 if ( gui_keycode == KEY_POUNDSIGN )
					{
						AllUserFpDeleteMgr.Selection = YES;
						AllUserFpDeleteMgr.Status = SendDeleteAllUserFpCmdToFPM;
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						GoToFpMenu();	
					}
				}

			break;

		case SCREEN_RegisterMasterFp:
				
				if ( FpUserRegisterMgr.Status == InputUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}
				if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToFpMenu();	
				}
				
			break;
			
		case SCREEN_DeleteMasterFp:
				if ( FpUserDeleteMgr.Status == InputUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}

			break;

		case SCREEN_DeleteAllMasterFp:
			
				/*if ( AllUserFpDeleteMgr.Status == WaitForUserConfirmDeleteAllFP )
				{
					if ( gui_keycode == KEY_FOUR )
					{
						DEF_ButtonPress_Voice;
						AllUserFpDeleteMgr.Selection = YES;
					}
					else if ( gui_keycode == KEY_SIX )
					{
						DEF_ButtonPress_Voice;
						AllUserFpDeleteMgr.Selection = NO;
					}
					else if ( gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						 if (  AllUserFpDeleteMgr.Selection == YES ){
							AllUserFpDeleteMgr.Status = SendDeleteAllUserFpCmdToFPM;
						}
						else{
							AllUserFpDeleteMgr.Status = EXIT;
						}
					}
				}*/

			break;

		case SCREEN_RegisterCardUser:
				if ( CardUserRegisterMgr.Status == InputCardUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}
				if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToCardMenu();	
				}

			break;
			
		case SCREEN_DeleteCardUser:

				if ( CardUserDeleteMgr.Status == InputCardUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}
			break;

		case SCREEN_DeleteAllCardUser:
				
				if ( AllCardUserDeleteMgr.Status == WaitForUserConfirmDeleteAllCard )
				{
					if ( gui_keycode == KEY_POUNDSIGN )
					{
						AllCardUserDeleteMgr.Selection = YES;
						AllCardUserDeleteMgr.Status = DeletingAllCardUser;
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						GoToCardMenu();	
					}
				}

			break;	


		case SCREEN_RegisterPasscodeUser:

				if ( PasscodeUserRegisterMgr.Status == InputPasscodeUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}
				else if ( PasscodeUserRegisterMgr.Status == InputFirstPasscode )
				{
					GUI_PasscodeInputButtonMonitor(gui_keycode);
				}
				else if ( PasscodeUserRegisterMgr.Status == InputSecondPasscode )
				{
					GUI_PasscodeInputButtonMonitor(gui_keycode);
				}
				
				if ( ( CheckMemoryMgr.FpMasterNum != 0x00 )
					 ||( CheckMemoryMgr.PasscodeMasterNum != 0x00) )
				{
					if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						GoToPasswordMenu();	
					}
				}
		
			break;
			
		case SCREEN_DeletePasscodeUser:

			if ( PasscodeUserDeleteMgr.Status == InputPasscodeUserID )
				{
					GUI_UserIDinputButtonMonitor(gui_keycode);
				}

			break;

		case SCREEN_DeleteAllPasscodeUser:
			
			if ( AllPasscodeUserDeleteMgr.Status == WaitForDeleteAllPasscodeUserConfirm )
			{
				if ( gui_keycode == KEY_POUNDSIGN )
				{
					AllPasscodeUserDeleteMgr.Selection = YES;
					AllPasscodeUserDeleteMgr.Status = DeletingAllPasscodeUser;
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToPasswordMenu();	
				}
			}
			break;
			

		case SCREEN_TimeSetting:
			
		/*if ( TimeSettingMgr.Status == SetYearMajor)
				{
					if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.year = (TempSystemTime.year&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetYearMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetYearMinor)
				{
					if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
					}
					else if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.year =(TempSystemTime.year&0xF0)+gui_keycode;
						TimeSettingMgr.Status = SetMonthMajor;
					}
				}
				
				else if ( TimeSettingMgr.Status == SetMonthMajor)
				{
					if ( (gui_keycode < KEY_TWO) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.month= (TempSystemTime.month&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetMonthMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetMonthMinor)
				{
					if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
					}
					else if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.month=(TempSystemTime.month&0xF0)+gui_keycode;
						if ( TempSystemTime.month > 0x12 ){
							TempSystemTime.month = 0x12;
						}
						if(TempSystemTime.month == 0x00)
						{
						 TempSystemTime.month = 0x01;
						}
						TimeSettingMgr.Status = SetDateMajor;
					}
				}
				
				else if ( TimeSettingMgr.Status == SetDateMajor)
				{
					if ( gui_keycode == KEY_ASTERISK ){
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
					}
					else if ( (gui_keycode < KEY_FOUR) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.date= (TempSystemTime.date&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetDateMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetDateMinor)
				{
					if ( gui_keycode == KEY_ASTERISK ){
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
					}
					else if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.date=(TempSystemTime.date&0xF0)+gui_keycode;
						if ( TempSystemTime.date > 0x31 ){
							TempSystemTime.date = 0x31;
						}
						if(TempSystemTime.date == 0x00)
						{
						 TempSystemTime.date = 0x01;
						}
						TimeSettingMgr.Status = SetHourMajor;
					}
				}

				else if ( TimeSettingMgr.Status == SetHourMajor)
				{
					if ( gui_keycode == KEY_ASTERISK ){
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
					}
					else if ( (gui_keycode < KEY_THREE )){
						DEF_ButtonPress_Voice;
						TempSystemTime.hour= (TempSystemTime.hour&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetHourMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetHourMinor)
				{
					if ( gui_keycode == KEY_ASTERISK ){
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
					}
					else if( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) )
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.hour=(TempSystemTime.hour&0xF0)+gui_keycode;
						if (TempSystemTime.hour > 0x23){
							TempSystemTime.hour = 0x23;
						}
						TimeSettingMgr.Status = SetMinuteMajor;
					}
				}


				else if ( TimeSettingMgr.Status == SetMinuteMajor)
				{
					if ( gui_keycode == KEY_ASTERISK ){
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
					}
					else if ( gui_keycode < KEY_SIX)
					{
						DEF_ButtonPress_Voice;
						TempSystemTime.minute= (TempSystemTime.minute&0x0F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetMinuteMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetMinuteMinor)
				{
					if ( gui_keycode == KEY_ASTERISK ){
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
//						PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_PleaseInputMinute);
					}
					else if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) ){
						DEF_ButtonPress_Voice;
						TempSystemTime.minute=(TempSystemTime.minute&0xF0)+gui_keycode;
						TimeSettingMgr.Status = SetSecondMajor;
					}
				}

				else if ( TimeSettingMgr.Status == SetSecondMajor)
				{
					if ( gui_keycode == KEY_ASTERISK ){
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
					}
					else if ( gui_keycode < KEY_SIX){
						DEF_ButtonPress_Voice;
						TempSystemTime.second= (TempSystemTime.second&0x8F)+(gui_keycode<<4);
						TimeSettingMgr.Status = SetSecondMinor;
					}
				}
				else if ( TimeSettingMgr.Status == SetSecondMinor)
				{
					if ( gui_keycode == KEY_ASTERISK ){
						DEF_ButtonPress_Voice;
						TimeSettingMgr.Status = SetYearMajor;
//						PLAY_VOICE_MULTISEGMENTS(VoiceMgr.volume,VoiceStr_PleaseInputSecond);
					}
					else if ( (gui_keycode < KEY_NINE )||(gui_keycode == KEY_NINE) ){
						DEF_ButtonPress_Voice;
						TempSystemTime.second=(TempSystemTime.second&0xF0)+gui_keycode;
						TimeSettingMgr.Status = SetYearMajor;
					}
				}

				if ( gui_keycode == KEY_POUNDSIGN )
				{
					if (is_valid_date(TempSystemTime.year, TempSystemTime.month, TempSystemTime.date)==bTRUE)
					{
						DEF_ButtonPress_Voice;
						SaveSystemTime();
						#ifdef Function_USE_Internal_RTC
						G_SystemUTCTime = SystemTimeToUTC(SystemTime);
						#elif defined Function_USE_External_RTC
						PCF8563_WriteTime();
						#endif
						Hardware_DelayMs(100);
						PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Success);
					}
					else{
						Hardware_DelayMs(100);
						PLAY_VOICE_THREESEGMENT(VOICE_Time,VOICE_Setting,VOICE_Fail);
					}
						
					GoToSystemConfigMenu();					
				}
				else if ( gui_keycode == KEY_ASTERISK )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToSystemConfigMenu();		

					#ifdef Function_USE_Internal_RTC
					SystemTime = UTCToSystemtime(G_SystemUTCTime);
					#elif defined Function_USE_External_RTC
					PCF8563_ReadTime();
					#endif
	
					//GotSystemTime();			
				
					TimeSettingMgr.Status = SetYearMajor;
				}*/

			break;

			case SCREEN_RestoreFactoryDefault:

				if ( RestoreFactoryDefaultMgr.Status == WaitForRestoreFactoryDefaultUserConfirm )
				{
					if ( gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						RestoreFactoryDefaultMgr.Selection = YES ;
						RestoreFactoryDefaultMgr.Status = ConfirmedToRestoreFactoryDefault;
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						GoToSystemConfigMenu();	
					}
				}

				break;

		#ifdef Function_VoiceEnabledEnglish
		case SCREEN_LanguageSetting:

				if ( gui_keycode == KEY_ONE )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					SystemLanguage = Chinese;				
					SystemConfigSave();
					GoToSystemConfigMenu();
				}
				else if ( gui_keycode == KEY_TWO )
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					SystemLanguage = English;					
					SystemConfigSave();
					GoToSystemConfigMenu();
				}
				else if ( gui_keycode == KEY_ASTERISK)
				{
					DEF_ButtonPress_Voice;
					Hardware_DelayMs(100);
					GoToSystemConfigMenu();
				}
			break;
		#endif

		case SCREEN_VolumeSetting:

			if ( gui_keycode == KEY_ONE )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceStr_VoiceOpened);
				Hardware_DelayMs(100);
				VoiceMgr.Enable = bTRUE;				
				SystemConfigSave();
				GoToSystemConfigMenu();
			}
			else if ( gui_keycode == KEY_TWO )
			{
				PLAY_VOICE_MULTISEGMENTS(VoiceStr_VoiceClosed);
				Hardware_DelayMs(100);
				VoiceMgr.Enable = bFALSE;	
				SystemConfigSave();
				GoToSystemConfigMenu();
			}
			else if ( gui_keycode == KEY_ASTERISK )
			{
				DEF_ButtonPress_Voice;
				Hardware_DelayMs(100);
				GoToSystemConfigMenu();
			}
			break;	
/*				
		case SCREEN_ManagerPasscodeModify:

			if ( ManagerPasscodeModifyMgr.Status == InputFirstPasscode )
			{
				GUI_PasscodeInputButtonMonitor(gui_keycode);
			}
			
			else if ( ManagerPasscodeModifyMgr.Status == InputSecondPasscode )
			{
				GUI_PasscodeInputButtonMonitor(gui_keycode);
			}
	
			break;
*/				
		case SCREEN_MemoryCheck:
			
			
			break;
			
		case SCREEN_ManagerIdentify:

				if ( ManagerIdentifyMgr.Status == WaiForManagerPasscodeOrFP )
					{
						GUI_PasscodeInputButtonMonitor(gui_keycode);
					}
			break;
				
		case SCREEN_UnlockingModeSetting:
				
				if ( UnlockingModeMgr.Status == WaitForUserConfirmUnlockingMode )
				{
					if ( gui_keycode == KEY_ONE )
					{
						UserIdentifyResultMgr.UnlockingMode = SingalMode;
						UnlockingModeMgr.Status = UserConfirmedUnlockingMode;
						PLAY_VOICE_MULTISEGMENTS(VoiceStr_SingleUnlockModeOpened);
						Hardware_DelayMs(100);
					}
					else if ( gui_keycode == KEY_TWO )
					{
						UserIdentifyResultMgr.UnlockingMode = DoubleMode;
						UnlockingModeMgr.Status = UserConfirmedUnlockingMode;
						if  ( IfSystemWithoutSecondIdentity() == bTRUE )
						{
							PLAY_VOICE_ONESEGMENT(VOICE_PleaseInputSecondIdentity);
							Hardware_DelayMs(100);
						}
						else
						{
							PLAY_VOICE_MULTISEGMENTS(VoiceStr_CombinationUnlockModeOpened);
							Hardware_DelayMs(100);
						}
					}
					else if ((gui_keycode == KEY_ASTERISK))
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						UnlockingModeMgr.Status = UserConfirmedUnlockingMode;
					}
				}
			break;

			
			case SCREEN_ContinueOpenSetting:
			
					if ( gui_keycode == KEY_ONE )
					{
						if ( UserIdentifyResultMgr.FlagContinuedOpenEnabled == bFALSE )
						{
							UserIdentifyResultMgr.FlagContinuedOpenEnabled = bTRUE;
							GUI_MotorContinueOpen();
						}
						PLAY_VOICE_MULTISEGMENTS(VoiceStr_NormallyOpenModeOpened);
						Hardware_DelayMs(100);
						GoToSystemConfigMenu();
					}
					else if ( gui_keycode == KEY_TWO )
					{
						if ( UserIdentifyResultMgr.FlagContinuedOpenEnabled == bTRUE )
						{
							UserIdentifyResultMgr.FlagContinuedOpenEnabled = bFALSE;
							MotorMgr.CWTimer = 0;
							MotorMgr.CWWAITTimer =  0;	
							MotorMgr.CCWTimer = Def_MotorCCWTime+Hardware_MotorDriverTimeCompasention();
							MotorMgr.CCWWAITTimer =  Def_MotorCCWWAITTime;	
							MotorMgr.MotorStatus = CCW;	
							MotorMgr.CCWSoftStart = bTRUE;
						}
						PLAY_VOICE_MULTISEGMENTS(VoiceStr_NormallyOpenModeClosed);
						Hardware_DelayMs(100);
						GoToSystemConfigMenu();
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						GoToSystemConfigMenu();
					}
				break;
					
			case SCREEN_PickAlarmEnableSetting:
			
					if ( PickAlarmEnableMgr.Status == WaitForPickAlarmEnableSettingUserConfirm )
					{
						if ( gui_keycode == KEY_ONE )
						{
							//PickAlarmEnableMgr.Selection = YES;
							PickAlarmEnableMgr.Enable = bTRUE;
							PickAlarmEnableMgr.Status = PickAlarmEnableSettingSuccess;
							PLAY_VOICE_MULTISEGMENTS(VoiceStr_PickAlarmOpened);
							Hardware_DelayMs(100);
		
						}
						else if ( gui_keycode == KEY_TWO )
						{
							//PickAlarmEnableMgr.Selection = NO;
							PickAlarmEnableMgr.Enable = bFALSE;
							PickAlarmEnableMgr.Status = PickAlarmEnableSettingSuccess;
							PLAY_VOICE_MULTISEGMENTS(VoiceStr_PickAlarmClosed);
							Hardware_DelayMs(100);
							
						}
						else if ( gui_keycode == KEY_ASTERISK)
						{
							DEF_ButtonPress_Voice;
							Hardware_DelayMs(100);
							PickAlarmEnableMgr.Status = PickAlarmEnableSettingSuccess;
						}
					}
					break;

			case SCREEN_SystemVersion:

						if ((gui_keycode == KEY_POUNDSIGN )||( gui_keycode == KEY_ASTERISK))
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
							CardIdentifyMgr.Status = ReadingCardID;
							FpIdentifyMgr.Status = FPMcmdStart;
							CurrentScreen = SCREEN_Main;
						}
					break;
						
			case SCREEN_PickLockAlarm:
	
			if  ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
			{
				if ( gui_keycode < 10 )		//from KEY0~~KEY9
				{
					if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyIdle )
					{
						
						PasscodeInputMgr.Point = 0x01;
						PasscodeInputMgr.PasscodeLen = 16;
						PasscodeInputMgr.Status = PasscodeInputStart;
						PasscodeInputMgr.InputBuff[0] = gui_keycode;
						for (i=1;i<PasscodeInputMgr.PasscodeLen;i++)
						{
							PasscodeInputMgr.InputBuff[i]=0xff;
						}
						
						PasscodeUserIdentifyMgr.Status = PasscodeIdentifyStart;
						//DEF_ButtonPress_Voice;
					}
				 }
				else if ( gui_keycode == KEY_ASTERISK )
				{
				}
			}
			else if ( gui_keycode != KEY_NONE )		//from KEY0~~KEY9
			{
				if ( PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput )
				{
					//DEF_ButtonPress_Voice;
					//PasscodeUserIdentifyMgr.TimeCnt = 240;	//
					GUI_PasscodeInputButtonMonitor(gui_keycode);
				}
			}
			
			break;

			case SCREEN_SystemLocked:
				
				if ( gui_keycode != KEY_NONE )	
				{
					PLAY_VOICE_ONESEGMENT(VOICE_SystemLocked);
					Hardware_DelayMs(100);
				}
			break;

			case SCREEN_CheckEventLogBySequence:
				
				#ifdef Function_EventLog
				if ( CheckEventLogBySequenceMgr.Status == VoiceReportEventLogWithSequence )
				{
					if ( gui_keycode == KEY_TWO )
					{	
						if  ( CheckEventLogBySequenceMgr.OffsetEventLogNum > 0 )
						{
							DEF_ButtonPress_Voice;
							CheckEventLogBySequenceMgr.OffsetEventLogNum--;

							if ( LogMgr.DisplayPoint < (DEF_MaxRecordedLogID-1)){
								LogMgr.DisplayPoint++;
							}
							else{
								LogMgr.DisplayPoint = 0;
							}
						}
						else
						{
							DEF_Fail_Beep;
						}
					}
					else if ( gui_keycode == KEY_EIGHT )
					{	
						if ( (CheckEventLogBySequenceMgr.OffsetEventLogNum+1) < CheckEventLogBySequenceMgr.MatchedEventLogNum )
						{
							DEF_ButtonPress_Voice;
							if ( LogMgr.DisplayPoint > 0 )
							{
								LogMgr.DisplayPoint--;
							}
							else if( LogMgr.DisplayPoint == 0 )
							{
								if ( JudgeLogSaved(DEF_MaxRecordedLogID-1) == S_SUCCESS )
								{
									LogMgr.DisplayPoint = DEF_MaxRecordedLogID-1;
								}
							}
							CheckEventLogBySequenceMgr.OffsetEventLogNum++;
						}
						else
						{
							DEF_Fail_Beep;
						}
					}
					else if (gui_keycode == KEY_ASTERISK)
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						GotoLogMenu();
					}
				}
				#endif

			break;

			case SCREEN_DeleteEventLog:
				
				#ifdef Function_EventLog
				if ( LogDeleteMgr.Status == WaitforLogDeleteCofirm )
				{
					if ( gui_keycode == KEY_POUNDSIGN )
					{
						DEF_ButtonPress_Voice;
						LogDeleteMgr.Status = LogDeleting;
					}
					else if ( gui_keycode == KEY_ASTERISK )
					{
						DEF_ButtonPress_Voice;
						Hardware_DelayMs(100);
						GotoLogMenu();
					}
				}
				#endif

			break;
				
		default:

			break;
			
	}
	
}

/*******************************************************/
/*******************************************************/
void GUI_Update_Screen(void)//GUI更新屏幕
{
	if (( CurrentScreen !=LastScreen )||(GUI_Flag_RefreshLCD == bTRUE))
	{
		#ifdef Function_ScreenDisplay
		Clear_Screen();
		GUI_Flag_RefreshLCD = bFALSE;
		#endif
		LastScreen =  CurrentScreen;
	}
	switch (CurrentScreen)
	{
		case SCREEN_Initialization:

			//SET_ALLKEYLED_OFF();
			ShowInitialization();
			break;
		
		case SCREEN_LowBattery:

			//SET_ALLKEYLED_OFF();
			ShowLowBattery();
			break;

		case SCREEN_PickLockAlarm:

			//SET_ALLKEYLED_ON();
			ShowPickLockAlarm();
			break;

		case SCREEN_Main:
			
			if (SystemPowerMgr.SleepDelayTimerCnt == 0x0000)
			{
				//SET_ALLKEYLED_OFF();
			}
			else
			{
				//SET_ALLKEYLED_ON();
				GUI_UpadteMain();
			}
			break;

		case SCREEN_IdentifySuccess:
			
			//SET_ALLKEYLED_OFF();
			//KEYLED_WATERLIGHT_Task();
			ShowIdentifySuccessPage();
			break;
		
		case SCREEN_IdentifyFail:
				
			//KEYLED_IdentifyFail_Task();
			ShowIdentifyFailPage();
			break;
		
		case SCREEN_DoorBell:
			
			ShowDoorBell();
			break;
		
		case SCREEN_MainMenu:

			//SET_1234A_LED_ON();
			ShowMainMenu();
			break;
			
		case SCREEN_EventLogMenu:
			
			//SET_12A_LED_ON();
			#ifdef Function_EventLog
			ShowLogMenu();
			#endif
			break;
			
		case SCREEN_FpMenu:
			
			//SET_123A_LED_ON();
			ShowFpMenu();
			break;
		
		case SCREEN_CardUserMenu:

			//SET_12A_LED_ON();
			ShowCardUserMenu();
			break;
		
		case SCREEN_PasscodeMenu:
			
			//SET_123A_LED_ON();
			ShowPasscodeMenu();
			break;
		
		case SCREEN_SystemConfigMenu:

			//SET_123456A_LED_ON();
			ShowSystemConfigMenu();
			break;
		
		#ifdef Function_TuyaWifi
		case SCREEN_NetWorkLink:
			
			ShowNetWorkConnecting();
			break;
		#endif
		
		case SCREEN_RegisterUserFp:
			
			//SET_ALLKEYLED_OFF();
			ShowRegisterUserFp();
			break;
		
		case SCREEN_DeleteUserFp:
			
			//SET_ALLKEYLED_ON();
			//ShowDeleteUserFp();
			break;
		
		case SCREEN_DeleteAllUserFp:
			
			//SET_AP_LED_ON();
			ShowDeleteAllUserFp();
			break;
		
		case SCREEN_RegisterMasterFp:
			
			//SET_ALLKEYLED_OFF();
			ShowRegisterMasterFp();
			break;
			
		case SCREEN_DeleteMasterFp:
			
			//SET_ALLKEYLED_ON();
			//ShowDeleteMasterFp();
			break;
/*
		case SCREEN_DeleteAllMasterFp:
			SET_LRandConfirmLED_ON();
			ShowDeleteAllMasterFp();

			break;
*/
		case SCREEN_RegisterCardUser:
			//KEYLED_KEY5_Flash();
			ShowRegisterCardUser();

			break;
			
		case SCREEN_DeleteCardUser:
			//SET_ALLKEYLED_ON();
			//ShowDeleteCardUser();

			break;

		case SCREEN_DeleteAllCardUser:
			//SET_AP_LED_ON();
			ShowDeleteAllCardUser();

			break;

		case SCREEN_RegisterPasscodeUser:
			//SET_ALLKEYLED_ON();
			ShowRegisterPasscodeUser();

			break;
			
		case SCREEN_DeletePasscodeUser:
			//SET_ALLKEYLED_ON();
			//ShowDeletePasscodeUser();

			break;

		case SCREEN_DeleteAllPasscodeUser:
			//SET_AP_LED_ON();
			ShowDeleteAllPasscodeUser();

			break;


		case SCREEN_TimeSetting:

			//ShowTimeSetting();

			break;
/*
		case SCREEN_ManagerPasscodeModify:
		//	SET_ALLKEYLED_ON();
		//	ShowModifyManagerPasscode();

			break;
*/			
		case SCREEN_RestoreFactoryDefault:
			//SET_AP_LED_ON();
			ShowRestoreFactoryDefault();

			break;
		
		#ifdef Function_VoiceEnabledEnglish
		case SCREEN_LanguageSetting:
			//SET_12A_LED_ON();
			ShowLanguageSetting();

			break;
		#endif
			
		case SCREEN_VolumeSetting:
			//SET_12A_LED_ON();
			ShowVolumeSetting();

			break;

		case SCREEN_MemoryCheck:
			//SET_UDandConfirmLED_ON();
			//ShowMemoryUsage();

			break;
			
		case SCREEN_ManagerIdentify:

			//SET_ALLKEYLED_ON();
			ShowManagerIdentify();

			break;

		case SCREEN_UnlockingModeSetting:
			//SET_12A_LED_ON();
			ShowUnlockingModeSetting();
		
			break;
			
		case SCREEN_ContinueOpenSetting:	
			//SET_12A_LED_ON();
			ShowContinuedOpenModeSetting();
			break;
			
		case SCREEN_PickAlarmEnableSetting:	
			//SET_12A_LED_ON();
			ShowPickAlarmEnableSetting();
			break;
			
		case SCREEN_SystemVersion:	
			//SET_AP_LED_ON();
			GUI_Update_Version();
			break;

//		case SCREEN_SelfTest:

			//SET_ALLKEYLED_ON();
			//ShowSelfTest();
//			break;
		
		case SCREEN_AgingTest:

			//KEYLED_WATERLIGHT_Task();
			ShowAgingTest();
			break;

		case SCREEN_SystemLocked:

			//KEYLED_ASTERISK_Flash();
			ShowSystemLocked();
			break;
			
		case SCREEN_CheckEventLogBySequence:
			#ifdef Function_EventLog
			ShowEventLogBySequence();
			#endif
			break;

		case SCREEN_DeleteEventLog:
						
		#ifdef Function_EventLog
			ShowClearEventLog();
		#endif
			break;
		
		default:
			break;
	}

}
/*******************************************************/
/*******************************************************/
/*******************************************************/
void GUI_Init(void)//
{
	//uint8_t i;

	GUI_ToggleFlag_05s=0x00;
	GUI_ToggleFlag_1s=0x01;
	FpIdentifyMgr.BreathingLED=0x00;

	GUI_Flag_RefreshLCD = bFALSE;

	FpIdentifyMgr.Status = FPMcmdStart;
	CardIdentifyMgr.Status = ReadingCardID;
	TimeSettingMgr.Status = SetYearMajor;
	BatteryMgr.PostLowBattery = bTRUE;
	
	//VoiceMgr.Enable = bTRUE;		//default Enable Voice

	Key_Init();
	
	init_Managerdata();

	ReadCardUserMemoryFromEEPROM();

	ReadPasscodeUserMemoryFromEEPROM();

//	SafetyMonitorMgr.IdentifyFailedTimes = 0x00;
//	SafetyMonitorMgr.SystemLocked = bFALSE;

	UserIdentifyResultMgr.CardIdentifyStatus = S_FAIL;
	UserIdentifyResultMgr.FPIdentifyStatus = S_FAIL;
	UserIdentifyResultMgr.PasscodeIdentifyStatus = S_FAIL;
	UserIdentifyResultMgr.FlagContinuedOpenEnabled = bFALSE;
	UserIdentifyResultMgr.VoiceDelay = 0;

	g_ASTERISK_PressedOnMainScreen = bFALSE;
        
  DEBUG_MARK;
		
	#ifdef Function_USE_Internal_RTC
	SystemTime = UTCToSystemtime(G_SystemUTCTime);
	#elif defined Function_USE_External_RTC
	PCF8563_ReadTime();
	#endif
	
	#ifdef Function_ScreenDisplay
	Display_Init();
	#endif

}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void GUI_Task(void)//
{
	//uint8_t *Point;
  uint8_t i,j;
	if (++GUI_TimeCnt > 63)
	{
		GUI_ToggleFlag_1s^=0x01;
		GUI_TimeCnt =0;
		if ((PINMACRO_ONBOARD_BUTTON_STATUS==0)&&(CurrentScreen == SCREEN_Main))
		{
			if(VoiceMgr.Enable == bFALSE)
			{
				VoiceMgr.Enable = bTRUE;
				DEF_ButtonPress_Voice;
				VoiceMgr.Enable = bFALSE;
			}
			else
			{
				DEF_ButtonPress_Voice;
			}
			RefreshSystemSleepTime();
		}
		#ifdef Function_FPMbreathingLed
		if( FpIdentifyMgr.BreathingLED > 0 )
		{
			FpIdentifyMgr.BreathingLED += 1;
			if( FpIdentifyMgr.BreathingLED == 4 )
			{
				FpIdentifyMgr.BreathingLED = 0;
				SetFPMbreathingLed(FPMbreathingLed_Bule);
			}
		}
		#endif
	}
	
	if ( GUI_TimeCnt %32 == 0x00 ){	
		GUI_ToggleFlag_05s^= 0x01;
	}
	
	if ( CardIdentifyMgr.CardDetectIntervalTimeCnt >  0 )
	{
		CardIdentifyMgr.CardDetectIntervalTimeCnt --;
	}
	if ( LockBrand.BrandChangeTimeDelay > 0 )
	{
		 LockBrand.BrandChangeTimeDelay--;
	}

	if ( PINMACRO_PICKLOCK_STATUS == 1 )
	{
		j=0;
		for (i=0;i<100;i++)
		{
			Hardware_DelayX5us(2);
			if ( PINMACRO_PICKLOCK_STATUS == 1 )
			{
				j++;
			}
		}
		if ((j>90)&&(AntiPryingMgr.AntiPryingSignalRelease == bTRUE))
		{
			AntiPryingMgr.AntiPryingTrigger = bTRUE;
		}
	}
	else
	{
		j=0;
		for (i=0;i<10;i++)
		{
			Hardware_DelayX5us(1);
			if ( PINMACRO_PICKLOCK_STATUS == 0 )
			{
				j++;
			}
		}
		if (j>8)
		{
			AntiPryingMgr.AntiPryingSignalRelease = bTRUE;
		}
	}
	
	if (	( AntiPryingMgr.AntiPryingTrigger == bTRUE )
			&&( CurrentScreen != SCREEN_PickLockAlarm )
			&&( CurrentScreen != SCREEN_Initialization )
			&&( CurrentScreen != SCREEN_PickAlarmEnableSetting )
			&&( CurrentScreen != SCREEN_RestoreFactoryDefault )
			&&( PickAlarmEnableMgr.Enable == bTRUE ))
	{
		CurrentScreen = SCREEN_PickLockAlarm;
		AntiPryingMgr.TimeCnt = DEF_AntiPryingTime;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
		FpIdentifyMgr.Status = FPMcmdStart;
		CardIdentifyMgr.Status = ReadingCardID;
		Hardware_DelayMs(20);		//wait for voice data transfer completed
		#ifdef Function_FPMbreathingLed
		SetFPMbreathingLed(FPMbreathingLed_Off);
		#endif
	}
	
	GUI_Button_Monitor();
	
	GUI_Update_Screen();
	
}
