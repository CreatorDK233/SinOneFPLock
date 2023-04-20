#ifndef __Project__
#define __Project__	1

#include "SC95F861xB_C.H"

//#define DEBUG_MODE

//#define ProjectIs_BarLock_S1728		//583F 			ָ����֤5��
//#define ProjectIs_BarLock_S1732		//383FM			ָ����֤8��(ͬ1728)(����������Ϊ�ǿ���)
//#define ProjectIs_BarLock_S6902		//������  	ָ����֤8��
//#define ProjectIs_BarLock_S8103		//������  	ָ����֤8��(ͬ1728)
#define ProjectIs_BarLock_S5514		//		�ڲ�rtc	 	ָ����֤5��
//#define ProjectIs_BarLock_S9201		//		�ڲ�rtc		ָ����֤5��(ͬ5514)
//#define ProjectIs_BarLock_S3007		//		�ڲ�rtc		ָ����֤5��(ͬ5514)
//#define ProjectIs_BarLock_S3009		//FJ04			ָ����֤5��
//#define ProjectIs_BarLock_S0608		//FJ04		ָ����֤5��
//#define ProjectIs_BarLock_S7703		//383F			ָ����֤8��(ͬ6902��ȥ�ư�)
//#define ProjectIs_BarLock_S8705		//583F			ָ����֤5��(ͬ1728)(��������ص�)
//#define ProjectIs_BarLock_S8706		//������		ָ����֤5��(ͬ1728)(��������ص�)
//#define ProjectIs_BarLock_S7204		//FJ04			ָ����֤5��(ͬ1728)
//#define ProjectIs_BarLock_S6431		//FJ04			ָ����֤5��
//#define ProjectIs_BarLock_S9702		//FJ04		ָ����֤5��(ͬ1728)


#ifdef ProjectIs_BarLock_S1728
#define DEF_CustomerNumber	17			//should be less than 256
#define DEF_ModelNumber			28			//should be less than 256
#define DEF_HardwareVerion	200			//should be less than 256
#define DEF_FirmwareVerion	204			//should be less than 256
#define SystemHardwareVersion	{VOICE_One,VOICE_Seven,VOICE_Two,VOICE_Eight,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_Two,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Four,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S1732
#define DEF_CustomerNumber	17			//should be less than 256
#define DEF_ModelNumber			32			//should be less than 256
#define DEF_HardwareVerion	100			//should be less than 256
#define DEF_FirmwareVerion	200			//should be less than 256
#define SystemHardwareVersion	{VOICE_One,VOICE_Seven,VOICE_Three,VOICE_Two,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S8103
#define DEF_CustomerNumber	81			//should be less than 256
#define DEF_ModelNumber			03			//should be less than 256
#define DEF_HardwareVerion	102			//should be less than 256
#define DEF_FirmwareVerion	200			//should be less than 256
#define SystemHardwareVersion	{VOICE_Eight,VOICE_One,VOICE_Zero,VOICE_Three,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Two,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S0608
#define DEF_CustomerNumber	06			//should be less than 256
#define DEF_ModelNumber			8			//should be less than 256
#define DEF_HardwareVerion	100			//should be less than 256
#define DEF_FirmwareVerion	201			//should be less than 256
#define SystemHardwareVersion	{VOICE_Zero,VOICE_Six,VOICE_Zero,VOICE_Eight,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6902
#define DEF_CustomerNumber	69		//should be less than 256
#define DEF_ModelNumber			02		//should be less than 256
#define DEF_HardwareVerion	101		//should be less than 256
#define DEF_FirmwareVerion	200		//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_Nine,VOICE_Zero,VOICE_Two,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_One,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_NoDuplicateFp
#define Function_EventLog
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S7703
#define DEF_CustomerNumber	77		//should be less than 256
#define DEF_ModelNumber			03		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	200		//should be less than 256
#define SystemHardwareVersion	{VOICE_Seven,VOICE_Seven,VOICE_Zero,VOICE_Three,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_NoDuplicateFp
#define Function_EventLog
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S5514
#define DEF_CustomerNumber	55		//should be less than 256
#define DEF_ModelNumber			14		//should be less than 256
#define DEF_HardwareVerion	200		//should be less than 256
#define DEF_FirmwareVerion	205		//should be less than 256
#define SystemHardwareVersion	{VOICE_Five,VOICE_Five,VOICE_One,VOICE_Four,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_Two,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Five,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN
#define Function_HallSlideCover

#elif defined ProjectIs_BarLock_S3007
#define DEF_CustomerNumber	30		//should be less than 256
#define DEF_ModelNumber			07		//should be less than 256
#define DEF_HardwareVerion	200		//should be less than 256
#define DEF_FirmwareVerion	201		//should be less than 256
#define SystemHardwareVersion	{VOICE_Three,VOICE_Zero,VOICE_Zero,VOICE_Seven,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_Two,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN
#define Function_HallSlideCover

#elif defined ProjectIs_BarLock_S9201
#define DEF_CustomerNumber	92		//should be less than 256
#define DEF_ModelNumber			01		//should be less than 256
#define DEF_HardwareVerion	103		//should be less than 256
#define DEF_FirmwareVerion	200		//should be less than 256
#define SystemHardwareVersion	{VOICE_Nine,VOICE_Two,VOICE_Zero,VOICE_One,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Three,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN
#define Function_HallSlideCover

#elif defined ProjectIs_BarLock_S8705
#define DEF_CustomerNumber	87			//should be less than 256
#define DEF_ModelNumber			05			//should be less than 256
#define DEF_HardwareVerion	100			//should be less than 256
#define DEF_FirmwareVerion	203			//should be less than 256
#define SystemHardwareVersion	{VOICE_Eight,VOICE_Seven,VOICE_Zero,VOICE_Five,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Three,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S8706
#define DEF_CustomerNumber	87			//should be less than 256
#define DEF_ModelNumber			06			//should be less than 256
#define DEF_HardwareVerion	100			//should be less than 256
#define DEF_FirmwareVerion	203			//should be less than 256
#define SystemHardwareVersion	{VOICE_Eight,VOICE_Seven,VOICE_Zero,VOICE_Six,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Three,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S7204
#define DEF_CustomerNumber	72			//should be less than 256
#define DEF_ModelNumber			04			//should be less than 256
#define DEF_HardwareVerion	105			//should be less than 256
#define DEF_FirmwareVerion	204			//should be less than 256
#define SystemHardwareVersion	{VOICE_Seven,VOICE_Two,VOICE_Zero,VOICE_Four,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Five,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Four,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S6431
#define DEF_CustomerNumber	64			//should be less than 256
#define DEF_ModelNumber			31			//should be less than 256
#define DEF_HardwareVerion	100			//should be less than 256
#define DEF_FirmwareVerion	202			//should be less than 256
#define SystemHardwareVersion	{VOICE_Six,VOICE_Four,VOICE_Three,VOICE_One,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Two,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S3009
#define DEF_CustomerNumber	30			//should be less than 256
#define DEF_ModelNumber			9			//should be less than 256
#define DEF_HardwareVerion	100			//should be less than 256
#define DEF_FirmwareVerion	200			//should be less than 256
#define SystemHardwareVersion	{VOICE_Three,VOICE_Zero,VOICE_Zero,VOICE_Nine,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Zero,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#elif defined ProjectIs_BarLock_S9702
#define DEF_CustomerNumber	97		//should be less than 256
#define DEF_ModelNumber			02		//should be less than 256
#define DEF_HardwareVerion	100		//should be less than 256
#define DEF_FirmwareVerion	203		//should be less than 256
#define SystemHardwareVersion	{VOICE_Nine,VOICE_Seven,VOICE_Zero,VOICE_Two,DEF_VoiceSegmentEndFlag}
#define SystemFirmwareVersion	{VOICE_One,VOICE_Zero,VOICE_Zero,VOICE_Mute20ms,VOICE_Two,VOICE_Zero,VOICE_Three,DEF_VoiceSegmentEndFlag}
//#define Function_CardReaderUseSPI
#define Function_AppUnlock
#define Function_ScreenDisplay
#define Function_DisplayUsed_HFG12864
#define Function_EventLog
#define Function_NoDuplicateFp
//#define Function_FPMserialNumberCheck
#define Function_FPMbreathingLed
#define Function_AntiPryingDefaultDisabled
//#define Function_USE_Internal_RTC
//#define Function_DoorBellTouchIndependented
#define Function_DoorBellTouchUsedPOUNDSIGN

#endif

#ifdef DEBUG_MODE 
#define DEBUG_MARK {Soft_Delay_us(1);}
#define EnableWDT()	//
					//Enable watch dog  //Disable Protection  //1/256		//862ms			//Refresh WDG Timer and Enable Protection
#define CLRWDT()	//
#else
#define DEBUG_MARK //_nop_(); 
#define EnableWDT()	 WDTCON &= 0x00;   //Utilites����DEBUG Setting customer����WDTʹ�� bit4��CLRWDT Biy[2:0] WDTCKS 500ms
					
#define CLRWDT()   WDTCON |= 0x10;
#endif



#endif
