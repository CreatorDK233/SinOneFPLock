#include "Voice_Menu.h"
#include "BeepMgr.h"
#include "Project.h"


code uint8_t MainMenuVoiceBuff[][6]=
{	
	{VOICE_Press,VOICE_One,VOICE_Fingerprint,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Password,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Card,VOICE_User,VOICE_Management,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_SystemSetting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Five,VOICE_Log,VOICE_Query,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ExitMenu,DEF_VoiceSegmentEndFlag}
};

	
code uint8_t FpMenuVoiceBuff[][7] = {
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Administrator,VOICE_Fingerprint,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Add,VOICE_User,VOICE_Fingerprint,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Delete,VOICE_All,VOICE_User,VOICE_Fingerprint,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};
code uint8_t PasscodeMenuVoiceBuff[][7]={
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Administrator,VOICE_Password,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Add,VOICE_User,VOICE_Password,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_Delete,VOICE_All,VOICE_User,VOICE_Password,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};

code uint8_t CardUserMenuVoiceBuff[][7]={
	{VOICE_Press,VOICE_One,VOICE_Add,VOICE_Card,VOICE_User,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Delete,VOICE_All,VOICE_Card,VOICE_User,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};

code uint8_t LogMenuVoiceBuff[][7]={
	{VOICE_Press,VOICE_One,VOICE_Log,VOICE_Query,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Delete,VOICE_Log,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};

code uint8_t SystemConfigMenuVoiceBuff[][5]={
	{VOICE_Press,VOICE_One,VOICE_Time,VOICE_Setting,DEF_VoiceSegmentEndFlag},
//	{VOICE_Press,VOICE_Two,VOICE_Language,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Voice,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Three,VOICE_NormallyOpen,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Four,VOICE_UnlockingMode,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Five,VOICE_AntiSkidAlarm,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Six,VOICE_RestoreFactoryDefault,VOICE_Setting,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};

//code uint8_t VolumeSetLanguageBuff[][4]={	
//	{VOICE_Press,VOICE_One,VOICE_Chinese,DEF_VoiceSegmentEndFlag},
////	{VOICE_Press+1,VOICE_Two+1,VOICE_English,DEF_VoiceSegmentEndFlag},
//	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
//};


code uint8_t VolumeSetVoiceBuff[][5]={	
	{VOICE_Press,VOICE_One,VOICE_Voice,VOICE_Open,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_Voice,VOICE_Close,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};
code uint8_t VoiceStr_VoiceOpened[] = {VOICE_Voice,VOICE_Opened,DEF_VoiceSegmentEndFlag};
code uint8_t VoiceStr_VoiceClosed[] = {VOICE_Voice,VOICE_Closed,DEF_VoiceSegmentEndFlag};

code uint8_t ContinuedOpenModeSetVoiceBuff[][6]={	
	{VOICE_Press,VOICE_One,VOICE_NormallyOpen,VOICE_Open,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_NormallyOpen,VOICE_Close,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};
code uint8_t VoiceStr_NormallyOpenModeOpened[] = {VOICE_NormallyOpen,VOICE_Opened,DEF_VoiceSegmentEndFlag};
code uint8_t VoiceStr_NormallyOpenModeClosed[] = {VOICE_NormallyOpen,VOICE_Closed,DEF_VoiceSegmentEndFlag};


code uint8_t UnlockModeSetVoiceBuff[][4]={	
	{VOICE_Press,VOICE_One,VOICE_SingleUnlock,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_CombinationUnlock,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};
code uint8_t VoiceStr_SingleUnlockModeOpened[] = {VOICE_SingleUnlock,VOICE_Opened,DEF_VoiceSegmentEndFlag};
code uint8_t VoiceStr_CombinationUnlockModeOpened[] = {VOICE_CombinationUnlock,VOICE_Opened,DEF_VoiceSegmentEndFlag};


code uint8_t PickAlarmSetVoiceBuff[][5]={	
	{VOICE_Press,VOICE_One,VOICE_AntiSkidAlarm,VOICE_Open,DEF_VoiceSegmentEndFlag},
	{VOICE_Press,VOICE_Two,VOICE_AntiSkidAlarm,VOICE_Close,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};
code uint8_t VoiceStr_PickAlarmOpened[] = {VOICE_AntiSkidAlarm,VOICE_Opened,DEF_VoiceSegmentEndFlag};
code uint8_t VoiceStr_PickAlarmClosed[] = {VOICE_AntiSkidAlarm,VOICE_Closed,DEF_VoiceSegmentEndFlag};
	
code uint8_t RestoreFactoryDefaultVoiceBuff[][4]={	
	{VOICE_PressPoundKey,VOICE_Confirm,VOICE_RestoreFactoryDefault,DEF_VoiceSegmentEndFlag},
	{VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag}
};
	

code uint8_t ConfirmDeleteVoiceBuff[]={VOICE_PressPoundKey,VOICE_ConfirmToDelete,VOICE_PressAsteriskKey,VOICE_ReturnPreviousMenu,DEF_VoiceSegmentEndFlag};

code uint8_t SystemVersionVoiceBuff[][8]={
	SystemHardwareVersion,
	SystemFirmwareVersion
};