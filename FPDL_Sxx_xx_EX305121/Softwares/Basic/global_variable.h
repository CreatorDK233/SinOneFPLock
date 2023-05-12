#ifndef _global_variable_H_
#define _global_variable_H_

#include "StdTypes.h"

#define DEF_MAX_CARDUSER		100
#define DEF_MAX_PASSCODEMASTER  2
#define DEF_MAX_PASSCODEUSER	8
#define DEF_MAX_FPMASTER	10
#define DEF_MAX_FPUSER		40

#define Def_GuiTimeDelayCnt05s	32
#define Def_GuiTimeDelayCnt1s	64
#define Def_GuiTimeDelayCnt1s5	96
#define Def_GuiTimeDelayCnt2s	128
#define Def_GuiTimeDelayCnt2s5	160
#define Def_GuiTimeDelayCnt3s	192
#define Def_GuiTimeDelayCnt4s	256
#define Def_GuiTimeDelayCnt5s	320
#define Def_GuiTimeDelayCnt10s	640
#define Def_GuiTimeDelayCnt15s	960
#define Def_GuiTimeDelayCnt45s			2880		//45s

#define FPMbreathingLed_Bule		1
#define FPMbreathingLed_Red			2
#define FPMbreathingLed_Green		3
#define FPMbreathingLed_Off  		4

#define Def_IdendtifyFailScreenTimeDelay	Def_GuiTimeDelayCnt3s
#define Def_IdendtifySuccessScreenTimeDelay	 Def_GuiTimeDelayCnt5s
#define Def_MessageBoxTimeDelay	 Def_GuiTimeDelayCnt2s

GLOBAL Timebase_t G_tflagbits;

GLOBAL uint8_t G_BatLevel;
GLOBAL uint16_t G_BatVoltage;

GLOBAL systemtime_t SystemTime;
GLOBAL systemtime_t TempSystemTime;
GLOBAL uint32_t G_SystemUTCTime;

GLOBAL  UartRxMgr_t UART2_Mgr;

GLOBAL uint8_t G_TEST;

GLOBAL screen_t CurrentScreen;

GLOBAL  FpmAckMgr_t FpmAckMgr;
GLOBAL  FpUserRegisterMgr_t FpUserRegisterMgr;
GLOBAL  AllUserFpDeleteMgr_t	AllUserFpDeleteMgr;
GLOBAL  FpUserDeleteMgr_t	FpUserDeleteMgr;
GLOBAL  FpIdentifyMgr_t FpIdentifyMgr;

GLOBAL  CardMemoryMgr_t CardMemoryMgr[DEF_MAX_CARDUSER];
GLOBAL  CardRegisterMgr_t CardUserRegisterMgr;
GLOBAL  CardIdentifyMgr_t CardIdentifyMgr;
GLOBAL  CardUserDeleteMgr_t CardUserDeleteMgr;
GLOBAL  AllCardUserDeleteMgr_t AllCardUserDeleteMgr;

GLOBAL  FpRegisterMgr_t FpRegisterMgr;

GLOBAL  PasscodeMemoryMgr_t PasscodeMemoryMgr[DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER];
GLOBAL  PasscodeUserRegisterMgr_t PasscodeUserRegisterMgr;
GLOBAL  PasscodeUserDeleteMgr_t PasscodeUserDeleteMgr;
GLOBAL  AllPasscodeUserDeleteMgr_t AllPasscodeUserDeleteMgr;
GLOBAL  PasscodeUserIdentifyMgr_t PasscodeUserIdentifyMgr;
GLOBAL  BatteryMgr_t BatteryMgr;

GLOBAL  MotorMgr_t	MotorMgr;
//GLOBAL  EMAGNETMgr_t EMAGNETMgr;

GLOBAL  SystemPowerMgr_t SystemPowerMgr;

GLOBAL  BeepMgr_t BeepMgr;

GLOBAL  VoiceDataTransferMgr_t VoiceDataTransferMgr;
GLOBAL  VoiceDataTransferMgr_t VoiceDataTransferMgr2;

GLOBAL  FpUserMemoryMgr_t FpUserMemoryMgr[DEF_MAX_FPMASTER+DEF_MAX_FPUSER];

GLOBAL  RestoreFactoryDefaultMgr_t RestoreFactoryDefaultMgr;

GLOBAL  VoiceMgr_t VoiceMgr;
GLOBAL  PickAlarmEnableMgr_t PickAlarmEnableMgr;

GLOBAL  CheckHomManyRegisteredFPuser_t CheckHomManyRegisteredFPuser;
GLOBAL  CheckIfFpUserIDisRegistered_t CheckIfFpUserIDisRegistered;
GLOBAL  CheckMemoryMgr_t CheckMemoryMgr;
GLOBAL  ManagerIdentifyMgr_t ManagerIdentifyMgr;
GLOBAL  DisplayMemoryUsageMgr_t DisplayMemoryUsageMgr;
GLOBAL  InitializationMgr_t InitializationMgr;
GLOBAL  ManagerPasscodeModifyMgr_t	ManagerPasscodeModifyMgr;
GLOBAL  UserIdentifyResultMgr_t	UserIdentifyResultMgr;
GLOBAL  SelfTestMgr_t SelfTestMgr;

GLOBAL  SafetyMonitorMgr_t SafetyMonitorMgr;
GLOBAL  UnlockingModeMgr_t UnlockingModeMgr;

GLOBAL  FPMpowerMgr_t FPMpowerMgr;

GLOBAL  bool_t g_ASTERISK_PressedOnMainScreen;
GLOBAL  AgingTestMgr_t AgingTestMgr;

GLOBAL  LogMgr_t LogMgr;

GLOBAL LockBrand_t LockBrand;

GLOBAL CheckEventLogByDateMgr_t CheckEventLogByDateMgr;

GLOBAL CheckEventLogBySequenceMgr_t CheckEventLogBySequenceMgr;

GLOBAL TouchPowerMgr_t TouchPowerMgr;

GLOBAL SystemLanguage_t SystemLanguage;

GLOBAL AntiPryingMgr_t AntiPryingMgr;

GLOBAL LogDeleteMgr_t LogDeleteMgr;

GLOBAL FPMserialNumberManager_t FPMserialNumberManager;

GLOBAL LEDsMgr_t LEDsMgr;

GLOBAL VoiceReportLogMgr_t VoiceReportLogMgr;

GLOBAL DriverBoardVersion_t DriverBoardVersion;

GLOBAL DoorBellMgr_t DoorBellMgr;

GLOBAL AwakeSystemKeyMgr_t AwakeSystemKeyMgr;

#endif