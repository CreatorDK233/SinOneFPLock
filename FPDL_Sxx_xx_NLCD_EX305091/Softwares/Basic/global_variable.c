#include "global_variable.h"

Timebase_t G_tflagbits;
systemtime_t SystemTime;
systemtime_t TempSystemTime;
uint32_t G_SystemUTCTime;
UartRxMgr_t UART1_Mgr;
UartRxMgr_t UART2_Mgr;
uint8_t G_TEST;
screen_t CurrentScreen;
FpmAckMgr_t FpmAckMgr;
FpUserRegisterMgr_t FpUserRegisterMgr;
AllUserFpDeleteMgr_t	AllUserFpDeleteMgr;
FpUserDeleteMgr_t	FpUserDeleteMgr;
FpIdentifyMgr_t FpIdentifyMgr;
CardMemoryMgr_t CardMemoryMgr[DEF_MAX_CARDUSER];
CardRegisterMgr_t CardUserRegisterMgr;
CardIdentifyMgr_t CardIdentifyMgr;
CardUserDeleteMgr_t CardUserDeleteMgr;
AllCardUserDeleteMgr_t AllCardUserDeleteMgr;

FpRegisterMgr_t FpRegisterMgr;

PasscodeMemoryMgr_t PasscodeMemoryMgr[DEF_MAX_PASSCODEMASTER+DEF_MAX_PASSCODEUSER];
PasscodeUserRegisterMgr_t PasscodeUserRegisterMgr;
PasscodeUserDeleteMgr_t PasscodeUserDeleteMgr;
AllPasscodeUserDeleteMgr_t AllPasscodeUserDeleteMgr;
PasscodeUserIdentifyMgr_t PasscodeUserIdentifyMgr;
BatteryMgr_t BatteryMgr;

MotorMgr_t	MotorMgr;
//EMAGNETMgr_t EMAGNETMgr;

SystemPowerMgr_t SystemPowerMgr;

BeepMgr_t BeepMgr;

VoiceDataTransferMgr_t VoiceDataTransferMgr;
//VoiceDataTransferMgr_t VoiceDataTransferMgr2;

FpUserMemoryMgr_t FpUserMemoryMgr[DEF_MAX_FPMASTER+DEF_MAX_FPUSER];

RestoreFactoryDefaultMgr_t RestoreFactoryDefaultMgr;

VoiceMgr_t VoiceMgr;
PickAlarmEnableMgr_t PickAlarmEnableMgr;

CheckHomManyRegisteredFPuser_t CheckHomManyRegisteredFPuser;
CheckIfFpUserIDisRegistered_t CheckIfFpUserIDisRegistered;
CheckMemoryMgr_t CheckMemoryMgr;
ManagerIdentifyMgr_t ManagerIdentifyMgr;
DisplayMemoryUsageMgr_t DisplayMemoryUsageMgr;
InitializationMgr_t InitializationMgr;
ManagerPasscodeModifyMgr_t	ManagerPasscodeModifyMgr;
UserIdentifyResultMgr_t	UserIdentifyResultMgr;
SelfTestMgr_t SelfTestMgr;

SafetyMonitorMgr_t SafetyMonitorMgr;
UnlockingModeMgr_t UnlockingModeMgr;

FPMpowerMgr_t FPMpowerMgr;

bool_t g_ASTERISK_PressedOnMainScreen;
AgingTestMgr_t AgingTestMgr;

LogMgr_t LogMgr;

LockBrand_t LockBrand;

CheckEventLogByDateMgr_t CheckEventLogByDateMgr;

CheckEventLogBySequenceMgr_t CheckEventLogBySequenceMgr;

TouchPowerMgr_t TouchPowerMgr;

SystemLanguage_t SystemLanguage;

AntiPryingMgr_t AntiPryingMgr;

LogDeleteMgr_t LogDeleteMgr;

FPMserialNumberManager_t FPMserialNumberManager;

LEDsMgr_t LEDsMgr;

//FpLEDState_t FpLEDState;

VoiceReportLogMgr_t VoiceReportLogMgr;

DriverBoardVersion_t DriverBoardVersion;

DoorBellMgr_t DoorBellMgr;

WifiMgr_t WifiMgr;



