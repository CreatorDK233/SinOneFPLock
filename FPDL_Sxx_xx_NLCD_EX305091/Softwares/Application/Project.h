#ifndef __Project__
#define __Project__	1

#include "SC95F861xB_C.H"

//#define DEBUG_MODE

/*无屏系类不需要RTC，配置中不需要更改外部32K晶振配置
目前所有版本复位电路都是拉电源，不需要更改复位配置*/
//#define ProjectIs_BarLock_S7702			//383F 		指纹验证8次 (6902去屏加灯板)
//#define ProjectIs_BarLock_S6421			//383FPC 	指纹验证8次
//#define ProjectIs_BarLock_S6428			//383FM 	指纹验证8次	(同6421)
//#define ProjectIs_BarLock_S6428v2		//383FM 	指纹验证8次	6428基础上将刷卡芯片换成YC5018
//#define ProjectIs_BarLock_S6430			//FJ05		指纹验证5次	6428v2基础上加TuyaWiFi
//#define ProjectIs_BarLock_S1734			//383F		指纹验证8次	(1728去屏)
//#define ProjectIs_BarLock_S1735			//583F		指纹验证8次	1734基础上加TuyaWiFi
//#define ProjectIs_BarLock_S1736			//FJ04		指纹验证5次
//#define ProjectIs_BarLock_S8106			//FJ04		指纹验证5次	(1728去屏)
//#define ProjectIs_BarLock_S6904			//383F		指纹验证8次	
//#define ProjectIs_BarLock_S6904v2		//383F		指纹验证8次	6904基础上将刷卡芯片换成YC5018
//#define ProjectIs_BarLock_S6117			//383F		指纹验证8次	
//#define ProjectIs_BarLock_S6117v2		//383F		指纹验证8次	6117基础上将刷卡芯片换成YC5018
//#define ProjectIs_BarLock_S0607			//FJ04		指纹验证5次
//#define ProjectIs_BarLock_S7203			//FJ04		指纹验证5次 (1728去屏)
//#define ProjectIs_BarLock_S9901			//FJ04		指纹验证5次
//#define ProjectIs_BarLock_S7705			//FJ04		指纹验证5次
//#define ProjectIs_BarLock_S3008			//FJ04		指纹验证5次
#define ProjectIs_BarLock_S4914			//FJ04		指纹验证5次

#if defined ProjectIs_BarLock_S7702
//#define LED_HFG12864_SHOW
#define DEF_CustomerNumber	77		//should be less than 256
#define DEF_ModelNumber			02		//should be less than 256
#define DEF_HardwareVerion	101		//should be less than 256
#define DEF_FirmwareVerion	200		//should be less than 256
#define SystemHardwareVersion	{VOICE_Seven,VOICE_Seven,VOICE_Zero,VOICE_Two,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_One,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6421
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber			21		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	201		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_Four,VOICE_Two,VOICE_One,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6428
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber			28		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	202		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_Four,VOICE_Two,VOICE_Eight,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Two,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6428v2
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber			28		//should be less than 256
#define DEF_HardwareVerion	101		//should be less than 256
#define DEF_FirmwareVerion	200		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_Four,VOICE_Two,VOICE_Eight,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_One,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
//#define Function_NFCUsed_1850X
#define Function_NFCUsed_YC5018
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6430
#define DEF_CustomerNumber	64		//should be less than 256
#define DEF_ModelNumber			30		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	208		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_Four,VOICE_Three,VOICE_Zero,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Eight,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
//#define Function_NFCUsed_1850X
#define Function_NFCUsed_YC5018
#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN
#define Function_VoiceEnabledEnglish

#elif defined ProjectIs_BarLock_S1734
#define DEF_CustomerNumber	17		//should be less than 256
#define DEF_ModelNumber			34		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	205		//should be less than 256
#define SystemHardwareVersion	{VOICE_One,VOICE_Seven,VOICE_Three,VOICE_Four,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Five,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S1735
#define DEF_CustomerNumber	17		//should be less than 256
#define DEF_ModelNumber			35		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	203		//should be less than 256
#define SystemHardwareVersion	{VOICE_One,VOICE_Seven,VOICE_Three,VOICE_Five,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Three,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN
#define Function_VoiceEnabledEnglish

#elif defined ProjectIs_BarLock_S1736
#define DEF_CustomerNumber	17		//should be less than 256
#define DEF_ModelNumber			36		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	202		//should be less than 256
#define SystemHardwareVersion	{VOICE_One,VOICE_Seven,VOICE_Three,VOICE_Six,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Two,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S8106
#define DEF_CustomerNumber	81		//should be less than 256
#define DEF_ModelNumber			06		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	203		//should be less than 256
#define SystemHardwareVersion	{VOICE_Eight,VOICE_One,VOICE_Zero,VOICE_Six,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Three,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6904
#define DEF_CustomerNumber	69		//should be less than 256
#define DEF_ModelNumber			04		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	201		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_Nine,VOICE_Zero,VOICE_Four,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
#define Function_DoorBellTouchIndependented
//#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6904v2
#define DEF_CustomerNumber	69		//should be less than 256
#define DEF_ModelNumber			04		//should be less than 256
#define DEF_HardwareVerion	101		//should be less than 256
#define DEF_FirmwareVerion	201		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_Nine,VOICE_Zero,VOICE_Four,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_One,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
//#define Function_NFCUsed_1850X
#define Function_NFCUsed_YC5018
#define Function_DoorBellTouchIndependented
//#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6117
#define DEF_CustomerNumber	61		//should be less than 256
#define DEF_ModelNumber			17		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	201		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_One,VOICE_One,VOICE_Seven,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6117v2
#define DEF_CustomerNumber	61		//should be less than 256
#define DEF_ModelNumber			17		//should be less than 256
#define DEF_HardwareVerion	101		//should be less than 256
#define DEF_FirmwareVerion	201		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_One,VOICE_One,VOICE_Seven,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_One,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
//#define Function_NFCUsed_1850X
#define Function_NFCUsed_YC5018
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S0607
#define DEF_CustomerNumber	06		//should be less than 256
#define DEF_ModelNumber			07		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	204		//should be less than 256
#define SystemHardwareVersion	{VOICE_Zero,VOICE_Six,VOICE_Zero,VOICE_Seven,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Four,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
#define MotorIOChange
//#define Function_NFCUsed_YC5018
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S7203
#define DEF_CustomerNumber	72		//should be less than 256
#define DEF_ModelNumber			03		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	202		//should be less than 256
#define SystemHardwareVersion	{VOICE_Seven,VOICE_Two,VOICE_Zero,VOICE_Three,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Two,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S9901
#define DEF_CustomerNumber	99		//should be less than 256
#define DEF_ModelNumber			01		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	201		//should be less than 256
#define SystemHardwareVersion	{VOICE_Nine,VOICE_Nine,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define MotorIOChange
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S7705
#define DEF_CustomerNumber	77		//should be less than 256
#define DEF_ModelNumber			05		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	200		//should be less than 256
#define SystemHardwareVersion	{VOICE_Seven,VOICE_Seven,VOICE_Zero,VOICE_Five,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S3008
#define DEF_CustomerNumber	30		//should be less than 256
#define DEF_ModelNumber			08		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	201		//should be less than 256
#define SystemHardwareVersion	{VOICE_Three,VOICE_Zero,VOICE_Zero,VOICE_Eight,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define MotorIOChange
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
//#define Function_TuyaWifi
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S4914
#define DEF_CustomerNumber	49		//should be less than 256
#define DEF_ModelNumber			14		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	202		//should be less than 256
#define SystemHardwareVersion	{VOICE_Four,VOICE_Nine,VOICE_One,VOICE_Four,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Two,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
//#define Function_ScreenDisplay
//#define Function_DisplayUsed_HFG12864
//#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_USE_External_RTC
#define Function_NFCUsed_1850X
//#define Function_NFCUsed_YC5018
#define Function_DoorBellTouchIndependented
//#define Function_DoorBellTouchUsedPOUNDSIGN

#endif

#ifdef DEBUG_MODE 
#define DEBUG_MARK {Soft_Delay_us(1);}
#define EnableWDT()	//
					//Enable watch dog  //Disable Protection  //1/256		//862ms			//Refresh WDG Timer and Enable Protection
#define CLRWDT()	//
#else
#define DEBUG_MARK //_nop_(); 
#define EnableWDT()	 WDTCON &= 0x00;   //Utilites设置DEBUG Setting customer设置WDT使能 bit4：CLRWDT Biy[2:0] WDTCKS 500ms
					
#define CLRWDT()   WDTCON |= 0x10;
#endif



#endif
