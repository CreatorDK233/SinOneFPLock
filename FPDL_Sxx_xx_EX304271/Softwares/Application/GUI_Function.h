#ifndef _GUI_Function_H_
#define _GUI_Function_H_

#include "StdTypes.h"

#define DEF_SystemSleepDelayTime_MainScreen		640		//10s

#define DEF_SystemSleepDelayTime_MenuScreen		1920	//30s

void GoToMainMenu(void);
void GoToFpMenu(void);
void GoToPasswordMenu(void);
void GoToCardMenu(void);
void GoToSystemConfigMenu(void);
void GoToSystemVersion(void);
void GotoLogMenu(void);

bool_t IfSystemIsInFactoryDefaultStatus(void);
bool_t IfSystemWithoutSecondIdentity(void);
bool_t CompareTwoArrayIsSame(uint8_t Point1[],uint8_t Point2[],uint8_t Lenth);
void UnlockModeJudgment(void);
bool_t is_valid_date(uint8_t year, uint8_t month, uint8_t date);
void Config_AntiPrying_Interrupt(void);

void UnlockSuccess(uint8_t UnlockType);
void RefreshSystemSleepTime(void);
void SystemConfigSave(void);
void SystemConfigLoad(void);
void ReadLockBrand(void);
void WriteLockBrand(void);
void SystemConfigReset(void);
void GUI_DataInputCreat(uint8_t StartPage,uint8_t StartColumn,uint8_t InputNum,uint16_t DefaultValue);
void GUI_UserIDinputButtonMonitor(keycode_t keycode);
void GUI_PasscodeInputCreat(uint8_t StartPage,uint8_t StartColumn);
void GUI_PasscodeInputButtonMonitor(keycode_t keycode);
void PasscodeUserIdentify(void);
uint8_t PasscodeIdendify(uint8_t *BUFF1);
uint8_t AppPasscodeIdentify(uint8_t BUFF[]);
void ShowLockBrand(void);
void GUI_Update_Version(void);

void ReadPasscodeUserMemoryFromEEPROM(void);
void WritePasscodeUserMemoryToEEPROM(void);	
bool_t GUI_CompareTwoPasscodes(uint8_t *BUFF1,uint8_t *BUFF2);
bool_t IfPasscodeUserIDisRegistered(uint8_t UserID);
uint8_t CheckHowManyRegisteredPasscodeMaster( void );
uint8_t CheckHowManyRegisteredPasscodeUser( void );
void DeletePasscodeUserfromMemory(uint8_t UserID);
void DeleteAllPasscodeMasterfromMemory(void);
void DeleteAllPasscodeUserfromMemory(void);
status_t SavePasscodeUserToMemory(uint8_t *Point,uint8_t UserID);
uint8_t Get_Availabe_PasscodeMasterID(void);
uint8_t Get_Availabe_PasscodeUserID(void);
void GUI_GetUserNumList(void);
uint8_t TranslateNumberToVoice(uint8_t value);	
void SaveSystemTime(void);
void GotSystemTime(void);

typedef struct
{
	//uint8_t StartPage;
	//uint8_t StartColumn;
	uint16_t Value;
	uint8_t InputPoint;
	uint8_t InputNum;
	enum
	{
		InputIdle=0,
		InputStart=1,
		InputEnd,
		InputExit
	}Status;

}DataInputMgr_t;

typedef struct
{
	uint8_t TotalMenuNum;
	uint8_t MenuPoint;
}VoiceMenuMgr_t;

typedef struct
{
	uint8_t InputBuff[16];
	uint8_t PasscodeLen;
	uint8_t Point;
	enum
		{
			PasscodeInputIdle=0,
			PasscodeInputStart=1,
			PasscodeInputEnd,
			PasscodeInputExit
		}Status;
}PasscodeInputMgr_t;


























#endif