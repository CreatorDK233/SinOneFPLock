#include "gpio.h"
//Protocol layer
#include "MFC_MF17622.h"
#include "EEPROM.h"
#include "BeepMgr.h"
#include "TuyaWIFI.h"
#include "FingerPrint.h"
//Logic layer
#include "AppUnlock.h"
#include "Motor.h"
#include "FP.h"
#include "LCD.h"
#include "MFC.h"
//Application layer
#include "GUI_Function.h"
#include "global_variable.h"
#include "Font.h"
#include "Voice_Menu.h"
#include "Font_Menu.h"


extern screen_t CurrentScreen;
extern DataInputMgr_t DataInputMgr;
extern VoiceMenuMgr_t VoiceMenuMgr;
extern PasscodeInputMgr_t PasscodeInputMgr;
extern uint8_t LEDsCtrlSwitch;
extern uint8_t GUI_ToggleFlag_05s;
extern keycode_t gui_keycode;



void GoToMainMenu(void)//进入主菜单
{
	VoiceMenuMgr.MenuPoint=0;
	#ifdef Function_TuyaWifi
	VoiceMenuMgr.TotalMenuNum = 6;
	#else
	VoiceMenuMgr.TotalMenuNum = 5;
	#endif
	CurrentScreen = SCREEN_MainMenu;
}

void GoToFpMenu(void)//进入指纹菜单
{
	VoiceMenuMgr.MenuPoint=0;
	#ifdef Function_Enable_ID_Del
	VoiceMenuMgr.TotalMenuNum = 5;
	#else
	VoiceMenuMgr.TotalMenuNum = 4;
	#endif
	CurrentScreen = SCREEN_FpMenu;
}
void GoToPasswordMenu(void)//进入密码菜单
{
	VoiceMenuMgr.MenuPoint=0;
	#ifdef Function_Enable_ID_Del
	VoiceMenuMgr.TotalMenuNum = 5;
	#else
	VoiceMenuMgr.TotalMenuNum = 4;
	#endif
	CurrentScreen = SCREEN_PasscodeMenu;
}
void GoToCardMenu(void)//进入卡片菜单
{
	VoiceMenuMgr.MenuPoint=0;
	#ifdef Function_Enable_ID_Del
	VoiceMenuMgr.TotalMenuNum = 4;
	#else
	VoiceMenuMgr.TotalMenuNum = 3;
	#endif
	CurrentScreen = SCREEN_CardUserMenu;
}
void GoToSystemConfigMenu(void)//进入系统设置菜单
{
	VoiceMenuMgr.MenuPoint=0;
	#ifdef Function_VoiceEnabledEnglish
	VoiceMenuMgr.TotalMenuNum = 7;
	#else
	VoiceMenuMgr.TotalMenuNum = 6;
	#endif
	CurrentScreen = SCREEN_SystemConfigMenu;
}
void GoToSystemVersion(void)//进入系统版本号菜单
{
	VoiceMenuMgr.MenuPoint=0;
	VoiceMenuMgr.TotalMenuNum = 2;
	CurrentScreen = SCREEN_SystemVersion;
}

void GotoLogMenu(void)//进入日志菜单
{
	VoiceMenuMgr.MenuPoint=0;
	VoiceMenuMgr.TotalMenuNum = 3;
	CurrentScreen = SCREEN_EventLogMenu;
}

bool_t IfSystemIsInFactoryDefaultStatus(void)//如果系统处于出厂默认状态
{
	if ( ( CheckMemoryMgr.FpUserNum == 0x00 )
			&&(CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(CheckMemoryMgr.CardUserNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeUserNum == 0x00 )	)
	{
		return bTRUE;
	}
	else
	{
		return bFALSE;
	}
}

bool_t IfSystemWithoutSecondIdentity(void)//如果系统没有第二身份
{
	if  ( (((CheckMemoryMgr.FpUserNum+CheckMemoryMgr.FpMasterNum) == 0x00 )&&((CheckMemoryMgr.PasscodeMasterNum + CheckMemoryMgr.PasscodeUserNum)== 0x00))
		||(((CheckMemoryMgr.FpUserNum+CheckMemoryMgr.FpMasterNum) == 0x00 )&&(CheckMemoryMgr.CardUserNum == 0x00 ))
		||(((CheckMemoryMgr.PasscodeMasterNum + CheckMemoryMgr.PasscodeUserNum)== 0x00)&&(CheckMemoryMgr.CardUserNum == 0x00 ))
		)
	{
		return bTRUE;
	}
	else
	{
		return bFALSE;
	}
}

bool_t CompareTwoArrayIsSame(uint8_t Point1[],uint8_t Point2[],uint8_t Lenth)//比较两个数组是否相同
{
	uint8_t i;
	for (i=0;i<Lenth;i++)
	{
		if (Point1[i]!=Point2[i]){
			return bFALSE;
		}
	}
	return bTRUE;
}

void UnlockModeJudgment(void)//开锁模式有效性判断
{
	if	( IfSystemWithoutSecondIdentity() == bTRUE )
	{
		UserIdentifyResultMgr.UnlockingMode = SingalMode;
		EEPROM_WriteSequential(UnlockModeStartAddr,&UserIdentifyResultMgr.UnlockingMode,1);
	}
}

bool_t is_valid_date(uint8_t year, uint8_t month, uint8_t date)//判断是不是有效日期
{
    uint8_t monttbuffer[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		uint16_t y;
		uint8_t m,d;
		y=((year/16)*10)+(year%16)+2000;
		m = ((month/16)*10)+(month%16);
		d = ((date/16)*10) + (date%16);
        if (((y % 4 == 0) && (y % 100 != 0)) 
			|| (y % 400 == 0)
			)
        {
                monttbuffer[1] = 29; 
		}
        if ( (m > 0) 
			&& (m < 13)
			&& (d > 0) 
			&& (d <= monttbuffer[m - 1])
			)
        {
			return bTRUE;
		}
		else{
			return bFALSE;
		}
}

void Config_AntiPrying_Interrupt(void)//设置防撬
{
	if (PickAlarmEnableMgr.Enable == bTRUE)
	{
		EnableAntiPrying_Interrupt();
	}
	else{
		DisableAntiPrying_Interrupt();
	}
}

void UnlockSuccess(uint8_t UnlockType)//开锁成功
{
	UserIdentifyResultMgr.IdentifyType = UnlockType;
	GUI_SetMotorToOpenAndCloseDoor();
	//Enable_KEYLED_WATERLIGHT(); 
	CurrentScreen = SCREEN_IdentifySuccess;
	UserIdentifyResultMgr.TimeCnt = Def_MessageBoxTimeDelay;
}

void RefreshSystemSleepTime(void)//更新系统休眠时间
{
	if ( ( CurrentScreen == SCREEN_Main )
		 ||( CurrentScreen ==SCREEN_Initialization)
		 ||( CurrentScreen ==SCREEN_LowBattery )
		)
	{
		SystemPowerMgr.SleepDelayTimerCnt = DEF_SystemSleepDelayTime_MainScreen;
	}
	else
	{
		SystemPowerMgr.SleepDelayTimerCnt = DEF_SystemSleepDelayTime_MenuScreen;
	}
}

void SystemConfigSave(void)//系统设置保存
{
	#ifdef Function_VoiceEnabledEnglish
    EEPROM_WriteSequential(SystemLanguageStartAddr, (uint8_t*)&SystemLanguage, 1);
	#endif
    EEPROM_WriteSequential(UnlockModeStartAddr, (uint8_t*)&UserIdentifyResultMgr.UnlockingMode, 1);
    EEPROM_WriteSequential(VolumeSwitchStartAddr, (uint8_t*)&VoiceMgr.Enable, 1);
    EEPROM_WriteSequential(PickAlarmSwitchStartAddr, (uint8_t*)&PickAlarmEnableMgr.Enable, 1);
}

void SystemConfigLoad(void)//系统设置加载
{
	uint8_t temp;
	bool_t temp_BOOL;
	#ifdef Function_VoiceEnabledEnglish
	EEPROM_ReadSequential(SystemLanguageStartAddr,&temp,1);
	if (temp == Chinese)
	{
		SystemLanguage = Chinese;
	}
	else
	{
		SystemLanguage = English;
	}
	#else
	SystemLanguage = Chinese;
	#endif
	EEPROM_ReadSequential(UnlockModeStartAddr,&temp,1);
	if (temp == DoubleMode)
	{
		UserIdentifyResultMgr.UnlockingMode = DoubleMode;
	}
	else
	{
		UserIdentifyResultMgr.UnlockingMode = SingalMode;
	}
	
	EEPROM_ReadSequential(VolumeSwitchStartAddr,(uint8_t*)&temp_BOOL,1);

	if (temp_BOOL == bFALSE)
	{
    	VoiceMgr.Enable = bFALSE;
	}
	else
	{
			VoiceMgr.Enable = bTRUE;
	}
	
	EEPROM_ReadSequential(PickAlarmSwitchStartAddr,(uint8_t*)&temp_BOOL,1);
		
	#ifdef Function_AntiPryingDefaultDisabled
	if (temp_BOOL == bTRUE)
	{
    	PickAlarmEnableMgr.Enable = bTRUE;
	}
	else
	{
			PickAlarmEnableMgr.Enable = bFALSE;
	}
	#else
	if (temp_BOOL == bFALSE)
	{
    	PickAlarmEnableMgr.Enable = bFALSE;
	}
	else
	{
			PickAlarmEnableMgr.Enable = bTRUE;
	}
	#endif
		
	Config_AntiPrying_Interrupt();
	
}

/****************************************************************/
void ReadLockBrand(void)//读取锁品牌
{
	uint8_t temp;
	
	EEPROM_ReadSequential(LockBrandStartAddr,&temp,1);
	if ( temp == 0x55 )
	{
		EEPROM_ReadSequential(LockBrandStartAddr+1,&LockBrand.LockBrandDisDataBuff[0],224);
		LockBrand.LockBrandDataValid = bTRUE;
	}
	else{
		LockBrand.LockBrandDataValid = bFALSE;
	}	
}

void WriteLockBrand(void)//写入锁品牌
{	
	#ifdef Function_ScreenDisplay
	uint8_t temp;
	temp = 0x55;
	LockBrand.GotBrandData = bFALSE;
	if ( MFC_ReadLockBrandData(&LockBrand.LockBrandDisDataBuff[0]) == S_SUCCESS )
	{
		EEPROM_WriteSequential(LockBrandStartAddr,&temp,1);
		
//		if ( LockBrand.BrandType == SmallBrand )
//		{
			EEPROM_WriteSequential(LockBrandStartAddr+1,&LockBrand.LockBrandDisDataBuff[0],224);
//		}
//		else if ( LockBrand.BrandType == BigBrand )
//		{
//			EEPROM_WriteSequential(LockBrandStartAddr+1,&LockBrand.LockBrandDisDataBuff[0],512);
//		}
		LockBrand.GotBrandData = bTRUE;
		LockBrand.LockBrandDataValid = bTRUE;
	}
	#endif
}

void SystemConfigReset(void)//系统设置恢复出厂
{
	UserIdentifyResultMgr.UnlockingMode = SingalMode;
	VoiceMgr.Enable = bTRUE;
	
	#ifdef Function_AntiPryingDefaultDisabled
		PickAlarmEnableMgr.Enable = bFALSE;
	#else
		PickAlarmEnableMgr.Enable = bTRUE;
	#endif
	
	Config_AntiPrying_Interrupt();

	SystemConfigSave();
}

//ID输入效果显示生成
void GUI_DataInputCreat(uint8_t StartPage,uint8_t StartColumn,uint8_t InputNum,uint16_t DefaultValue)
{
	#ifdef Function_ScreenDisplay
	uint8_t temp,i;
	#endif
	uint16_t value;

	if( DataInputMgr.Status == InputIdle )
	{
		DataInputMgr.Status = InputStart;
		DataInputMgr.Value = DefaultValue;
		DataInputMgr.InputNum = InputNum;

		if ( DefaultValue == 0x0000 ){
			DataInputMgr.InputPoint = 0;
		}
		else{
			DataInputMgr.InputPoint = InputNum;
		}
	}
		
	else if ( DataInputMgr.Status == InputStart )
	{
		if (DataInputMgr.InputPoint > DataInputMgr.InputNum ){
			return;
		}

		value = DataInputMgr.Value;
		#ifdef Function_ScreenDisplay
		for (i=0;i<DataInputMgr.InputPoint;i++)
		{	
			temp = value%10;
			DisOneDigital16x8(StartPage,StartColumn+(8*(DataInputMgr.InputPoint-i-1)),temp,NormalDisplay);
			value/=10;
		}

		for (i=DataInputMgr.InputPoint;i<(DataInputMgr.InputNum+1);i++)		//"+1" for clear underline
		{
			DisZF16x8(StartPage,StartColumn+(8*i),ZF_kongge,NormalDisplay);
		}
		
		if ( GUI_ToggleFlag_05s == 1 )
		{
			DisZF16x8(StartPage,StartColumn+(8*DataInputMgr.InputPoint),ZF_kongge,NormalDisplay);
		}
		else{
			DisZF16x8(StartPage,StartColumn+(8*DataInputMgr.InputPoint),ZF_underline,NormalDisplay);
		}
		#else
		StartPage = 0; StartColumn = 0;//无意义，注掉不影响，会报warming
		#endif
	}

}


void GUI_UserIDinputButtonMonitor(keycode_t keycode)//GUI_用户ID输入按钮监视器
{
	if ( (keycode < KEY_NINE)||(keycode == KEY_NINE) )
	{
		DEF_ButtonPress_Voice;
		PasscodeUserRegisterMgr.OverTimeCnt = Def_GuiTimeDelayCnt5s;
		if ( DataInputMgr.InputPoint < DataInputMgr.InputNum )
		{
			DataInputMgr.Value = DataInputMgr.Value*10+gui_keycode;
			DataInputMgr.InputPoint++;
		}
	}
	else if ( keycode == KEY_ASTERISK )
	{
		DEF_ButtonPress_Voice;
		PasscodeUserRegisterMgr.OverTimeCnt = Def_GuiTimeDelayCnt5s;
		if ( DataInputMgr.InputPoint > 0 )
		{
			DataInputMgr.InputPoint--;
			DataInputMgr.Value/=10;
		}
		else
		{
			DataInputMgr.Status = InputExit;
		}
	}
	else if ( keycode == KEY_POUNDSIGN )
	{
		//DEF_ButtonPress_Voice;
		PasscodeUserRegisterMgr.OverTimeCnt = Def_GuiTimeDelayCnt5s;
		if ( DataInputMgr.InputPoint == 0 )
		{
			DataInputMgr.Status = InputExit;
		}
		else
		{
			DataInputMgr.Status = InputEnd;
		}
	}
}

void GUI_PasscodeInputCreat(uint8_t StartPage,uint8_t StartColumn)//GUI密码输入显示效果生成
{
	uint8_t i=0;
	StartColumn=0;//无意义，注掉不影响，会报warming
	if (PasscodeInputMgr.Point > 16)
	{
		return;
	}
	#ifdef Function_ScreenDisplay
	Clear_Screen_Page(StartPage);
	Clear_Screen_Page(StartPage+1);
	for (i=0;i<PasscodeInputMgr.Point;i++)
	{
		DisZF16x8(StartPage,(8*i)+(64-(PasscodeInputMgr.Point*4)),ZF_xinghao,NormalDisplay);
	}
	#else
	StartPage=0;//无意义，注掉不影响，会报warming
	#endif
}

void GUI_PasscodeInputButtonMonitor(keycode_t keycode)//GUI按键密码输入监视器
{
	if ( (keycode < KEY_NINE)||(keycode == KEY_NINE) )//0-9
	{
		PasscodeUserRegisterMgr.OverTimeCnt = Def_GuiTimeDelayCnt5s;
		if ( CurrentScreen != SCREEN_PickLockAlarm ){
			DEF_ButtonPress_Voice;
		}
		if ( PasscodeInputMgr.Point <  PasscodeInputMgr.PasscodeLen )
		{
			PasscodeInputMgr.InputBuff[PasscodeInputMgr.Point] = gui_keycode;
			PasscodeInputMgr.Point++;
		}
	}
	else if ( keycode == KEY_ASTERISK )
	{
		PasscodeUserRegisterMgr.OverTimeCnt = Def_GuiTimeDelayCnt5s;
		if ( CurrentScreen != SCREEN_PickLockAlarm && CurrentScreen != SCREEN_ManagerIdentify){
			DEF_ButtonPress_Voice;
		}
		
		if ( PasscodeInputMgr.Point > 0 )
		{
			PasscodeInputMgr.Status = PasscodeInputExit;	
		}
		else
		{
			PasscodeInputMgr.Status = PasscodeInputExit;
		}
	}
	else if ( keycode == KEY_POUNDSIGN )
	{
		PasscodeUserRegisterMgr.OverTimeCnt = Def_GuiTimeDelayCnt5s;
		if ( CurrentScreen != SCREEN_PickLockAlarm ){
			DEF_ButtonPress_Voice;
		}
		if ( PasscodeInputMgr.Point == 0 )
		{
			PasscodeInputMgr.Status = PasscodeInputExit;
		}
		else
		{
			PasscodeInputMgr.Status = PasscodeInputEnd;
		}
	}
}

void PasscodeUserIdentify(void)//密码用户认证
{
	if ( PasscodeUserIdentifyMgr.Status != PasscodeIdentifyIdle )
	{
		if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifyStart )
		{
			//PasscodeUserIdentifyMgr.TimeCnt = 240;
			PasscodeUserIdentifyMgr.Status = PasscodeIdentifyPasscodeInput;
		}
		else if (PasscodeUserIdentifyMgr.Status == PasscodeIdentifyPasscodeInput)
		{
			#ifdef Function_ScreenDisplay
			Clear_Screen_Page(3);
			Clear_Screen_Page(6);
			Clear_Screen_Page(7);
			GUI_PasscodeInputCreat(4,0);
			#endif
			if (PasscodeInputMgr.Status == PasscodeInputEnd)
			{
				if ( IfSystemIsInFactoryDefaultStatus()==bTRUE )
				{
					if( (PasscodeInputMgr.InputBuff[0]==0x01)
						&&(PasscodeInputMgr.InputBuff[1]==0x01)
						&&(PasscodeInputMgr.InputBuff[2]==0x01)
						&&(PasscodeInputMgr.InputBuff[3]==0x01)
						&&(PasscodeInputMgr.InputBuff[4]==0x01)
						&&(PasscodeInputMgr.InputBuff[5]==0x01)
						&&(PasscodeInputMgr.InputBuff[6]==0x01)
						&&(PasscodeInputMgr.InputBuff[8]==0x01)
						&&(PasscodeInputMgr.InputBuff[9]==0x01)
					  )
					{
						PasscodeUserIdentifyMgr.Status = PasscodeIdentifyAgingTestSuccess;
					}
					else if ((PasscodeInputMgr.InputBuff[0]==0x01)
							&&(PasscodeInputMgr.InputBuff[1]==0x03)
							&&(PasscodeInputMgr.InputBuff[2]==0x05)
							&&(PasscodeInputMgr.InputBuff[3]==0x07)
							&&(PasscodeInputMgr.Point == 4)
							)
					{
						PasscodeUserIdentifyMgr.Status = PasscodeIdentifyCheckVersionSuccess;
					}
					else
					{
						PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
					}
				}
				else
				{
					PasscodeUserIdentifyMgr.UserID = PasscodeIdendify(PasscodeInputMgr.InputBuff);
					if ( PasscodeUserIdentifyMgr.UserID != 0x00 )//Identify success
					{
						PasscodeUserIdentifyMgr.Status = PasscodeIdentifySuccess;
						//PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifySuccessScreenTimeDelay;
						UserIdentifyResultMgr.PasscodeType = LocalPasscode;
						//GUI_Flag_RefreshLCD = bTRUE;
					}
					else
					{
						if ( PasscodeInputMgr.Point == 10 )
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
							//PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifyFailScreenTimeDelay;
							#ifdef Function_TuyaWifi
							WifiMgr.PostMgr.Flag.Bits.RequestGreenTime = 1;
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
							#endif
						}
						else if ((PasscodeInputMgr.Point == 12)||( PasscodeInputMgr.Point == 13 ))
						{
							#ifdef Function_AppUnlock
 							PasscodeUserIdentifyMgr.UserID = AppPasscodeIdentify(PasscodeInputMgr.InputBuff,PasscodeInputMgr.Point);
							if ( PasscodeUserIdentifyMgr.UserID != 0x00 )//App unlock success
							{
								PasscodeUserIdentifyMgr.Status = PasscodeIdentifySuccess;
								//PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifySuccessScreenTimeDelay;
								UserIdentifyResultMgr.PasscodeType = AppPasscode;
								//GUI_Flag_RefreshLCD = bTRUE;
							}
							else
							#endif
							{
								PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
								//PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifyFailScreenTimeDelay;
								//GUI_Flag_RefreshLCD = bTRUE;
							}
						}
						else
						{
							PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
							//PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifyFailScreenTimeDelay;
							//GUI_Flag_RefreshLCD = bTRUE;
						}
					}
				}
			}
			else if (PasscodeInputMgr.Status == PasscodeInputExit)
			{
				PasscodeUserIdentifyMgr.Status = PasscodeIdentifyIdle;
				PasscodeUserRegisterMgr.TimeCnt = 0;	
				//GUI_Flag_RefreshLCD = bTRUE;
			}
		}
	}
}

uint8_t PasscodeIdendify(uint8_t *BUFF1)//密码验证
{
		uint8_t i,k;
	int j;
	uint8_t PasscodeLen;
	for (i=0;i<(DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER);i++)
	{
		if ( PasscodeMemoryMgr[i].Status != PasscodeIsValid ){
			continue;
		}

		PasscodeLen = 0;
		for (j=11;j>=0;j--)
		{
			if(j%2==1)
			{
				if ( (PasscodeMemoryMgr[i].PasscodeBuff[j/2]&0xf) != 0xF )
				{
					PasscodeLen = j+1;
					break;
				}
			}
			else
			{
				if ( ((PasscodeMemoryMgr[i].PasscodeBuff[j/2]>>4)&0xf) != 0xF )
				{
					PasscodeLen = j+1;
					break;
				}
			}
		}
		
		for (j=0;j<(16-PasscodeLen+1);j++)
		{
			for (k=0;k<PasscodeLen;k++)
			{
				if(k%2==1)
				{
					if (((PasscodeMemoryMgr[i].PasscodeBuff[k/2]&0xf) != *(BUFF1+k+j)))
					{
						break;
					}
				}
			    else
				{
					if ((((PasscodeMemoryMgr[i].PasscodeBuff[k/2]>>4)&0xf) != *(BUFF1+k+j)))
					{
						break;
					}
				}
			}
			if (k == PasscodeLen){
				return PasscodeMemoryMgr[i].UserID;
			}
		}
	}
	return 0;
}

uint8_t AppPasscodeIdentify(uint8_t BUFF[],unsigned int pwd_len )//APP密码识别
{
	uint8_t i,j;
	uint8_t UserPasscodeLen;
	uint8_t temp[12];
	unsigned int mode=0,result=0;
	
	ReadPasscodeUserMemoryFromEEPROM();
       //ReadPasscodeMaster02MemoryFromEEPROM();

	for (i=0;i<DEF_MAX_PASSCODEMASTER;i++)
	{
		UserPasscodeLen = 0;
		for(j=0;j<6;j++)
		{
			temp[2*j]=PasscodeMemoryMgr[i].PasscodeBuff[j]>>4;
			temp[2*j+1]=PasscodeMemoryMgr[i].PasscodeBuff[j]&0x0f;
		}
		for (j=0;j<pwd_len;j++)
		{
			if ( temp[j] < 10)
			{
				UserPasscodeLen++;
			}
			else
			{
				break;
			}
		}
		if(pwd_decrypt(temp,i,UserPasscodeLen,BUFF,pwd_len,&mode,&result)>0)             
		{
			return i+1;
		}
		
	}
	return 0;
}

void ShowLockBrand(void)//显示锁品牌
{
	#ifdef Function_ScreenDisplay
	code uint8_t Welcome_Str[8]={HZ_huan,HZ_yingjie,HZ_shiyong,HZ_yong,HZ_zhineng,HZ_neng,HZ_suomen,HZ_end};

	if ( LockBrand.LockBrandDataValid == bTRUE )
	{
		DisImage_RAM(6,8,112,16,&LockBrand.LockBrandDisDataBuff[0],NormalDisplay);
	}
	else if( LockBrand.LockBrandDataValid == bFALSE )
	{
		DisHZ16x14Str(6,16,Welcome_Str,NormalDisplay);
	}
	#endif
}

void GUI_Update_Version(void)//更新版本号
{
	#ifdef Function_ScreenDisplay
	 code uint8_t SystemVersionStr[]={HZ_xi,HZ_tong,HZ_ban,HZ_ben,HZ_end};
	//code uint8_t FPM_Version[]={"FPM:126"};
	uint8_t ModelStr[]={"Sxxx-xxx"};
	uint8_t VersionStr[]={"100.100"};
	#endif

	if ( VoiceDataTransferMgr.VoicePlayEnd == bTRUE )
	{
		if (VoiceMenuMgr.MenuPoint < VoiceMenuMgr.TotalMenuNum )
		{
			PLAY_VOICE_MULTISEGMENTS(SystemVersionVoiceBuff[VoiceMenuMgr.MenuPoint]);
			VoiceMenuMgr.MenuPoint++;
		}
	}
	
	#ifdef Function_ScreenDisplay
	DisHZ16x14Str(0,32,SystemVersionStr,NormalDisplay);
	ModelStr[1]=DEF_CustomerNumber/100+0x30;
	ModelStr[2]=DEF_CustomerNumber%100/10+0x30;
	ModelStr[3]=DEF_CustomerNumber%10+0x30;
	ModelStr[5]=DEF_ModelNumber/100+0x30;
	ModelStr[6]=DEF_ModelNumber%100/10+0x30;
	ModelStr[7]=DEF_ModelNumber%10+0x30;	
	DisEN16x8Str(3,20,ModelStr,NormalDisplay);
	//Main Board Hardware Version
	VersionStr[0]=DEF_HardwareVerion/100+0x30;
	VersionStr[1]=DEF_HardwareVerion%100/10+0x30;
	VersionStr[2]=DEF_HardwareVerion%10+0x30;
	//Main Board Firmware Version
	VersionStr[4]=DEF_FirmwareVerion/100+0x30;
	VersionStr[5]=DEF_FirmwareVerion%100/10+0x30;
	VersionStr[6]=DEF_FirmwareVerion%10+0x30;

	DisEN16x8Str(5,20,VersionStr,NormalDisplay);
	#endif
}

void ReadPasscodeUserMemoryFromEEPROM(void)//从EEPROM内存读取密码用户
{
	EEPROM_ReadSequential(PasscodeUserMemoryStartAddr,&PasscodeMemoryMgr[0].UserID,(8*(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER)));
}

void WritePasscodeUserMemoryToEEPROM(void)//从EEPROM内存写入密码用户
{
	EEPROM_WriteSequential(PasscodeUserMemoryStartAddr,&PasscodeMemoryMgr[0].UserID,(8*(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER)));
}

bool_t GUI_CompareTwoPasscodes(uint8_t *BUFF1,uint8_t *BUFF2)//比较两个密码是否相等
{
	if (  ( *BUFF1 == *BUFF2)
		&&(*(BUFF1+1) == *(BUFF2+1))
		&&(*(BUFF1+2) == *(BUFF2+2))
		&&(*(BUFF1+3) == *(BUFF2+3))
		&&(*(BUFF1+4) == *(BUFF2+4))
		&&(*(BUFF1+5) == *(BUFF2+5))
		&&(*(BUFF1+6) == *(BUFF2+6))
		&&(*(BUFF1+7) == *(BUFF2+7))
		&&(*(BUFF1+8) == *(BUFF2+8))
		&&(*(BUFF1+9) == *(BUFF2+9))
		&&(*(BUFF1+10) == *(BUFF2+10))
		&&(*(BUFF1+11) == *(BUFF2+11))
	   )
	{
		return bTRUE;
	}
	else
	{
		return bFALSE;
	}
}

bool_t IfPasscodeUserIDisRegistered(uint8_t UserID)//如果密码用户ID已注册
{
	uint8_t i;
	for (i=0;i<(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER);i++)
	{
		if (PasscodeMemoryMgr[i].UserID == UserID )
		{
			return bTRUE;
		}
	}
	return bFALSE;
}

uint8_t CheckHowManyRegisteredPasscodeMaster( void )//检查注册密码管理员数量
{
	uint8_t i,MasterNum;
	MasterNum =0;
	for (i=0;i<(DEF_MAX_PASSCODEMASTER);i++)
	{
		if ( IfPasscodeUserIDisRegistered(i+1) == bTRUE )
		{
			MasterNum++;
		}
	}
	return MasterNum;
}

uint8_t CheckHowManyRegisteredPasscodeUser( void )//检查注册密码用户数量
{
	uint8_t i,UserNum;
	UserNum =0;
	for (i=DEF_MAX_PASSCODEMASTER;i<(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER);i++)
	{
		if ( IfPasscodeUserIDisRegistered(i+1) == bTRUE )
		{
				UserNum++;
		}
	}
	return UserNum;
}

void DeletePasscodeUserfromMemory(uint8_t UserID)//从EEPROM内存中删除密码用户
{
	uint8_t i,j;

	for (i=0;i<(DEF_MAX_PASSCODEUSER+DEF_MAX_PASSCODEMASTER);i++)
	{
		if (PasscodeMemoryMgr[i].UserID == UserID )
		{
			PasscodeMemoryMgr[i].Status = PasscodeIsInvalid;
			PasscodeMemoryMgr[i].UserID = 0xFF;
			for (j=0;j<6;j++)
			{
				PasscodeMemoryMgr[i].PasscodeBuff[j] = 0xFF;
			}
			break;
		}
	}	

	WritePasscodeUserMemoryToEEPROM();
}

void DeleteAllPasscodeMasterfromMemory(void)//从EEPROM内存中删除全部密码管理员
{
	uint8_t i,j;

	for (i=0;i<DEF_MAX_PASSCODEMASTER;i++)
	{

		PasscodeMemoryMgr[i].Status = PasscodeIsInvalid;
		PasscodeMemoryMgr[i].UserID = 0xFF;
		for (j=0;j<6;j++)
		{
			PasscodeMemoryMgr[i].PasscodeBuff[j] = 0xFF;
		}

	}	
	WritePasscodeUserMemoryToEEPROM();
}

void DeleteAllPasscodeUserfromMemory(void)//从EEPROM内存中删除全部密码用户
{
	uint8_t i,j;

	for (i=DEF_MAX_PASSCODEMASTER;i<(DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER);i++)
	{

		PasscodeMemoryMgr[i].Status = PasscodeIsInvalid;
		PasscodeMemoryMgr[i].UserID = 0xFF;
		for (j=0;j<6;j++)
		{
			PasscodeMemoryMgr[i].PasscodeBuff[j] = 0xFF;
		}

	}	
	WritePasscodeUserMemoryToEEPROM();
}

status_t SavePasscodeUserToMemory(uint8_t *Point,uint8_t UserID)//保存密码用户到内存
{
	uint8_t j;
	for (j=0;j<6;j++)
	{
		PasscodeMemoryMgr[UserID-1].PasscodeBuff[j] = (*(Point+2*j))<<4|((*(Point+2*j+1)&0X0f));
	}
	PasscodeMemoryMgr[UserID-1].UserID = UserID;
	PasscodeMemoryMgr[UserID-1].Status = PasscodeIsValid;
	WritePasscodeUserMemoryToEEPROM();
	return S_SUCCESS;
}

uint8_t Get_Availabe_PasscodeMasterID(void)//获取已注册密码管理员ID
{
	uint8_t i;
	for (i=0;i<DEF_MAX_PASSCODEMASTER;i++)
	{
		if ( PasscodeMemoryMgr[i].Status != PasscodeIsValid ){
			return (i+1);
		}
	}
	return 0;
}

uint8_t Get_Availabe_PasscodeUserID(void)//获取已注册密码用户ID
{
	uint8_t i;
	for (i=DEF_MAX_PASSCODEMASTER;i<(DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER);i++)
	{
		if ( PasscodeMemoryMgr[i].Status != PasscodeIsValid ){
			return (i+1);
		}
	}
	return 0;
}

void ReadRegisteredFPuserFromLocalList(void)
{
	uint8_t i,j,USERID,Checksum,UserIdByte;
	
	ReadFPuserIdListFromEEPROM();

	Checksum = 0x00;

	for ( i=0;i<32;i++)
	{
		Checksum+=CheckHomManyRegisteredFPuser.UserIdList[i];
	}

	if (Checksum!=CheckHomManyRegisteredFPuser.UserIdListChecksum)
	{
		ResetFPuserIdListInEEPROM();
		CheckHomManyRegisteredFPuser.UserNum = 0x00;
		CheckHomManyRegisteredFPuser.StressUserNum = 0x00;
		CheckHomManyRegisteredFPuser.MasterNum = 0x00;
	}
	else
	{		
		USERID = 0x00;
		CheckHomManyRegisteredFPuser.UserNum = 0x00;
		CheckHomManyRegisteredFPuser.StressUserNum = 0x00;
		CheckHomManyRegisteredFPuser.MasterNum = 0x00;
		for ( i=0;i<32;i++)
		{
			UserIdByte = CheckHomManyRegisteredFPuser.UserIdList[i];
			for (j=0;j<8;j++)
			{	
				if ( (UserIdByte&0x01) !=0 )
				{
					if ( USERID < DEF_MAX_FPMASTER )
					{
						CheckHomManyRegisteredFPuser.MasterNum++;
						FpUserMemoryMgr[USERID].UserID = USERID;
						FpUserMemoryMgr[USERID].RegisterStatus = Registered;
						FpUserMemoryMgr[USERID].UserPriority = Master;
					}
					else if ( USERID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER))
					{
						CheckHomManyRegisteredFPuser.UserNum++;
						FpUserMemoryMgr[USERID].UserID = USERID;
						FpUserMemoryMgr[USERID].RegisterStatus = Registered;
						FpUserMemoryMgr[USERID].UserPriority = User;
					}
					
				}
				else
				{
					if ( USERID < DEF_MAX_FPMASTER )
					{
						FpUserMemoryMgr[USERID].UserID = USERID;
						FpUserMemoryMgr[USERID].RegisterStatus = UnRegistered;
						FpUserMemoryMgr[USERID].UserPriority = Master;
					}
					else if ( USERID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER))
					{
						FpUserMemoryMgr[USERID].UserID = USERID;
						FpUserMemoryMgr[USERID].RegisterStatus = UnRegistered;
						FpUserMemoryMgr[USERID].UserPriority = User;
					}
				}
				UserIdByte>>=1;
				USERID++;
			}
		}
	}
}

/*******************************************************/
/*******************************************************/
/*******************************************************/
void CheckHowManyRegisteredFPuserFromFPM(void)
{
	//uint8_t FPindexTable[32];
	uint8_t i,j,USERID;
	
	if ( CheckHomManyRegisteredFPuser.Status == StartCheckHowManyRegisteredFPuser )
		{
			FPM_SendReadIndexTableCmd();
			CheckHomManyRegisteredFPuser.TimeCnt = Def_GuiTimeDelayCnt05s;//Def_FPMcmdTimeOutDelay;
			CheckHomManyRegisteredFPuser.Status = WaitForCheckHowManyRegisteredFPuserCmdAck;
			FpmAckMgr.Status = WaitACK;
		}
	else if ( CheckHomManyRegisteredFPuser.Status == WaitForCheckHowManyRegisteredFPuserCmdAck)
		{
			if (FpmAckMgr.Status == GotACK)
			{
				if ( FpmAckMgr.ErrorCode == Error_NONE)
				{
					USERID = 0x00;
					CheckHomManyRegisteredFPuser.UserNum = 0x00;
					CheckHomManyRegisteredFPuser.StressUserNum = 0x00;
					CheckHomManyRegisteredFPuser.MasterNum = 0x00;
					for ( i=0;i<32;i++)
						{
							for (j=0;j<8;j++)
							{	
								if ( (FpmAckMgr.Buff[10+i]&0x01) !=0 )
								{
									if ( USERID < DEF_MAX_FPMASTER )
									{
										CheckHomManyRegisteredFPuser.MasterNum++;
										FpUserMemoryMgr[USERID].UserID = USERID;
										FpUserMemoryMgr[USERID].RegisterStatus = Registered;
										FpUserMemoryMgr[USERID].UserPriority = Master;
									}
									else if ( USERID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER))
									{
										CheckHomManyRegisteredFPuser.UserNum++;
										FpUserMemoryMgr[USERID].UserID = USERID;
										FpUserMemoryMgr[USERID].RegisterStatus = Registered;
										FpUserMemoryMgr[USERID].UserPriority = User;
									}
								}
								else
								{
									if ( USERID < DEF_MAX_FPMASTER )
									{
										FpUserMemoryMgr[USERID].UserID = USERID;
										FpUserMemoryMgr[USERID].RegisterStatus = UnRegistered;
										FpUserMemoryMgr[USERID].UserPriority = Master;
									}
									else if ( USERID < (DEF_MAX_FPMASTER+DEF_MAX_FPUSER))
									{
										FpUserMemoryMgr[USERID].UserID = USERID;
										FpUserMemoryMgr[USERID].RegisterStatus = UnRegistered;
										FpUserMemoryMgr[USERID].UserPriority = User;
									}
								}
								
								FpmAckMgr.Buff[10+i]>>=1;
								USERID++;
							}
						}
					CheckHomManyRegisteredFPuser.Status = CheckHomManyRegisteredFPuserSuccess;
				}
				else{
					DEBUG_MARK;
					CheckHomManyRegisteredFPuser.Status = CheckHomManyRegisteredFPuserFail;
				}
			}
			if ( CheckHomManyRegisteredFPuser.TimeCnt-- < 1 )
			{
				CheckHomManyRegisteredFPuser.Status = CheckHomManyRegisteredFPuserFail;
			}
		}
	
}

void GUI_GetUserNumList(void)//获取用户数量
{

	if ( CheckMemoryMgr.Status == StartCheckMemory)
	{
		CheckHomManyRegisteredFPuser.Status = StartCheckHowManyRegisteredFPuser;
		CheckHomManyRegisteredFPuser.FailTimes = 0x00;
		CheckMemoryMgr.Status = WaitForReadFPuserNum;
	}
	else if ( CheckMemoryMgr.Status == WaitForReadFPuserNum )
	{
		//CheckHowManyRegistereFPuser();
		CheckHowManyRegisteredFPuserFromFPM();
		if ( CheckHomManyRegisteredFPuser.Status == CheckHomManyRegisteredFPuserSuccess )
		{
			CheckMemoryMgr.FpUserNum = CheckHomManyRegisteredFPuser.UserNum;
			CheckMemoryMgr.FpMasterNum = CheckHomManyRegisteredFPuser.MasterNum;
			CheckMemoryMgr.CardUserNum = CheckHowManyRegisteredCardUser();
			CheckMemoryMgr.PasscodeMasterNum = CheckHowManyRegisteredPasscodeMaster();
			CheckMemoryMgr.PasscodeUserNum = CheckHowManyRegisteredPasscodeUser();
			CheckMemoryMgr.Status = CheckMemorySuccess;
		}
		else if ( CheckHomManyRegisteredFPuser.Status == CheckHomManyRegisteredFPuserFail )
		{
			if ( CheckHomManyRegisteredFPuser.FailTimes++ < 3 )
			{
				CheckHomManyRegisteredFPuser.Status = StartCheckHowManyRegisteredFPuser;//retry
			}
			else
			{
				ReadRegisteredFPuserFromLocalList();
				CheckMemoryMgr.FpUserNum = CheckHomManyRegisteredFPuser.UserNum;
				CheckMemoryMgr.FpMasterNum = CheckHomManyRegisteredFPuser.MasterNum;
				CheckMemoryMgr.CardUserNum = CheckHowManyRegisteredCardUser();
				CheckMemoryMgr.PasscodeMasterNum = CheckHowManyRegisteredPasscodeMaster();
				CheckMemoryMgr.PasscodeUserNum = CheckHowManyRegisteredPasscodeUser();
				#ifdef Function_FaceRecoginition			
				CheckMemoryMgr.FaceMasterNum = CheckHowManyRegisteredFaceMaster();
				CheckMemoryMgr.FaceUserNum = CheckHowManyRegisteredFaceUser();
				#endif		
				CheckMemoryMgr.Status = CheckMemoryFail;
			}
		}
	}
}

uint8_t TranslateNumberToVoice(uint8_t value)//将数字转换为语音
{
	return 12+(2*value);
}

#ifdef Function_Enable_ID_Del
void ReportRegisterID(uint16_t type, uint16_t ID)
{
	uint8_t voiceType[] = {VOICE_Fingerprint, VOICE_Password, VOICE_Card};
	uint8_t voiceBuf[7] = {0};
	if (type >= 0 && type <= 2 && ID >= 1 && ID <= 999)
	{
		voiceBuf[0] = VOICE_Add;
		voiceBuf[1] =	voiceType[type];
		voiceBuf[2] =	VOICE_ID;
		voiceBuf[3] =	TranslateNumberToVoice(ID / 100);
		voiceBuf[4] =	TranslateNumberToVoice(ID / 10 % 10);
		voiceBuf[5] =	TranslateNumberToVoice(ID % 10);
		voiceBuf[6] =	DEF_VoiceSegmentEndFlag;
		PLAY_VOICE_MULTISEGMENTS(voiceBuf);
	}
}
#endif

void SaveSystemTime( void )
{
	SystemTime.year   =		TempSystemTime.year;
	SystemTime.month 	= 	TempSystemTime.month;
	SystemTime.date   = 	TempSystemTime.date;
	SystemTime.day		= 	TempSystemTime.day;
	SystemTime.hour   = 	TempSystemTime.hour;
	SystemTime.minute = 	TempSystemTime.minute;
	SystemTime.second = 	TempSystemTime.second;
}

void GotSystemTime( void )
{
	TempSystemTime.year		=	SystemTime.year;
	TempSystemTime.month	= SystemTime.month;
	TempSystemTime.date 	= SystemTime.date;
	TempSystemTime.day		= SystemTime.day;
	TempSystemTime.hour		= SystemTime.hour;
	TempSystemTime.minute	= SystemTime.minute;
	TempSystemTime.second	= SystemTime.second;
}





