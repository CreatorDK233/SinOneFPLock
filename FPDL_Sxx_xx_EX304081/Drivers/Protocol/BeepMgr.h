#ifndef BeepMgr_h
#define BeepMgr_h

#include "StdTypes.h"
	
#define DEF_VoiceSegmentEndFlag 0xFA

#define VOICE_Mute20ms 					0
#define VOICE_WaterDrop 					1
#define VOICE_Beep					2
#define VOICE_Alarm					3
#define VOICE_Doorbell					4
#define VOICE_VolumeAdjust					5
#define VOICE_POWERON					6
#define VOICE_FailVoice					7
#define VOICE_Language					8
#define VOICE_Chinese					10
#define VOICE_English					11
#define VOICE_Zero					12
#define VOICE_One					14
#define VOICE_Two					16
#define VOICE_Three					18
#define VOICE_Four					20
#define VOICE_Five					22
#define VOICE_Six					24
#define VOICE_Seven					26
#define VOICE_Eight					28
#define VOICE_Nine					30
#define VOICE_Press					32
#define VOICE_Management					34
#define VOICE_SystemSetting					36
#define VOICE_InputError					38
#define VOICE_SingleUnlock					40
#define VOICE_CombinationUnlock					42
#define VOICE_DoorOpened					44
#define VOICE_DoorClosed					46
#define VOICE_SystemLocked					48
#define VOICE_Success					50
#define VOICE_Fail					52
#define VOICE_Confirm					54
#define VOICE_PressPoundKey					56
#define VOICE_PressAsteriskKey					58
#define VOICE_ConfirmToDelete					60
#define VOICE_PleaseAddMasterFirst					62
#define VOICE_IdentifyFail					64
#define VOICE_RestoreFactoryDefault					66
#define VOICE_Operation					68
#define VOICE_Add					70
#define VOICE_Delete					72
#define VOICE_All					74
#define VOICE_Administrator					76
#define VOICE_User					78
#define VOICE_Fingerprint					80
#define VOICE_Password					82
#define VOICE_Card					84
#define VOICE_Duplicate					86
#define VOICE_UsersAreFull					88
#define VOICE_IdentifyMaster					90
#define VOICE_PleaseTryAgain					92
#define VOICE_ReturnPreviousMenu					94
#define VOICE_ExitMenu					96
#define VOICE_PleaseSelectDigitalOperation					98
#define VOICE_PleaseInputID					100
#define VOICE_PleaseEnter					102
#define VOICE_Again					104
#define VOICE_UserIDisBeUsed					106
#define VOICE_PleasePutFinger					108
#define VOICE_PleasePutFingerAgain					110
#define VOICE_PleaseSwingCard					112
#define VOICE_Voice					114
#define VOICE_PleaseReplaceTheBattery					116
#define VOICE_Time					118
#define VOICE_NetworkingConfiguration					120
#define VOICE_PleaseInputSecondIdentity					122
#define VOICE_NormallyOpen					124
#define VOICE_UnlockingMode					126
#define VOICE_AntiSkidAlarm					128
#define VOICE_Setting					130
#define VOICE_Opened					132
#define VOICE_Closed					134
#define VOICE_Open					136
#define VOICE_Close					138

#define VOICE_Log					140
#define VOICE_Query					142
#define VOICE_Type					144
#define VOICE_ID					146
#define VOICE_Unlock				148
#define VOICE_No					150
#define VOICE_Previous				152
#define VOICE_Next					154

extern void PLAY_VOICE_ONESEGMENT(uint8_t segment);
extern void PLAY_VOICE_TWOSEGMENT(uint8_t segment1,uint8_t segment2);
extern void PLAY_VOICE_THREESEGMENT(uint8_t segment1,uint8_t segment2,uint8_t segment3);
extern void PLAY_VOICE_MULTISEGMENTS(uint8_t BUFF[]);
extern void PLAY_VOICE_ONESEGMENT_FIXED(uint8_t segment);
extern void PLAY_VOICE_TWOSEGMENT_FIXED(uint8_t segment1,uint8_t segment2);
extern void PLAY_VOICE_THREESEGMENT_FIXED(uint8_t segment1,uint8_t segment2,uint8_t segment3);
extern void PLAY_VOICE_MULTISEGMENTS_FIXED(uint8_t BUFF[]);

extern void SET_VOLUME(uint8_t volume);
extern void STOP_VOICEPLAY(void);
extern void VoicePlayerPowerDown(void);
extern void PLAY_VOICE_DOORBELL(void);


//extern void PLAY_VOICE2_ONESEGMENT(uint8_t segment);
//extern void PLAY_VOICE2_TWOSEGMENT(uint8_t segment1,uint8_t segment2);
//extern void PLAY_VOICE2_DOORBELL(void);
//extern void PLAY_VOICE2_CONTINUE(uint8_t segment);
//extern void STOP_VOICEPLAY2(void);
extern void VOICE_Init(void);
extern void BeepMgrTask(void);

#define DEF_ButtonPress_Voice  PLAY_VOICE_ONESEGMENT_FIXED(VOICE_WaterDrop)
#define DEF_Fail_Beep 				 PLAY_VOICE_ONESEGMENT_FIXED(VOICE_FailVoice)


#endif
