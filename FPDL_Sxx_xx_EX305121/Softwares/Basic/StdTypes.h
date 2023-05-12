
#ifndef STDTYPES_H
#define STDTYPES_H

//* ********************** FILE HEADER *************************************

//* *************************************************************************
//*  BASIC TYPE DEFINTIONS
#define uchar unsigned char
#define uint unsigned int
#define uint8_t unsigned char
#define uint16_t unsigned int
#define uint32_t unsigned long int
#define int8_t char
#define int16_t int
#define int32_t long int

typedef signed   char     sint8_t;
typedef signed int        sint16_t;
typedef signed long int   sint32_t;

#define false 0
#define true 1
#ifndef NULL    
#define NULL ((void *)0) 
#endif 

typedef enum {bTRUE=0, bFALSE=~bTRUE} bool_t;
typedef enum {S_FAIL=0, S_SUCCESS=~S_FAIL} status_t;
typedef enum {YES=0, NO=1} selection_t;

//+++++++++++++++++++++++++++ Bit Computing ++++++++++++++++++++++++++++++++++//
//SET BIT.    Example: a |= SETBIT0
enum
{
	SETBIT0 = 0x01,  SETBIT1 = 0x02,	SETBIT2 = 0x04,	 SETBIT3 = 0x08,
	SETBIT4 = 0x10,	 SETBIT5 = 0x20,	SETBIT6 = 0x40,	 SETBIT7 = 0x80,
	SETBIT8 = 0x0100,	 SETBIT9 = 0x0200,	SETBIT10 = 0x0400, SETBIT11 = 0x0800,
	SETBIT12 = 0x1000, SETBIT13 = 0x2000,	SETBIT14 = 0x4000, SETBIT15 = 0x8000		
};
//CLR BIT.    Example: a &= CLRBIT0
enum
{
	CLRBIT0 = 0xFFFE,  CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	 CLRBIT3 = 0xFFF7,	
	CLRBIT4 = 0xFFEF,	 CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	 CLRBIT7 = 0xFF7F,
	CLRBIT8 = 0xFEFF,	 CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF, CLRBIT11 = 0xF7FF,
	CLRBIT12 = 0xEFFF, CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF, CLRBIT15 = 0x7FFF
};
//CHOSE BIT.  Example: a = b&CHSBIT0
enum
{
	CHSBIT0 = 0x0001,  CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,	 CHSBIT3 = 0x0008,
	CHSBIT4 = 0x0010,	 CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	 CHSBIT7 = 0x0080,
	CHSBIT8 = 0x0100,	 CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400, CHSBIT11 = 0x0800,
	CHSBIT12 = 0x1000, CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000, CHSBIT15 = 0x8000		
};

//+++++++++++++++++++++++++++++ Others ++++++++++++++++++++++++++++++++++++//
//TAST RUN STEPS.
enum
{
	STEP0 = 0,         STEP1 = 1,         STEP2 = 2,         STEP3 = 3,
	STEP4 = 4,         STEP5 = 5,         STEP6 = 6,         STEP7 = 7,
	STEP8 = 8,         STEP9 = 9,         STEP10 = 10,       STEP11 = 11,
	STEP12 = 12,       STEP13 = 13,       STEP14 = 14,       STEP15 = 15,
	STEP16 = 16,       STEP17 = 17,       STEP18 = 18,       STEP19 = 19,
	STEP20 = 20,       STEP21 = 21,       STEP22 = 22,       STEP23 = 23,
	STEP24 = 24,       STEP25 = 25,       STEP26 = 26,       STEP27 = 27,
	STEP28 = 28,       STEP29 = 29,       STEP30 = 30,       STEP31 = 31,
	STEP32 = 32,       STEP33 = 33,       STEP34 = 34,       STEP35 = 35
};

typedef struct
{
	uint8_t second;	///
	uint8_t minute;
	uint8_t hour;	
	uint8_t date;
	uint8_t day;
	uint8_t month;
	uint8_t year;
}systemtime_t;

typedef enum
{
	Undefined=0,
	Master=1,
	User
}UserPriority_t;

typedef enum
{
	FPUSER = 0,
	PASSCODEUSER=1,
	CARDUSER,
	REMOTEGUEST,
	ONLINEPASSCODEUSER
}UserType_t;

typedef enum
{
	KEY_ZERO=0,
	KEY_ONE=1,
	KEY_TWO,
	KEY_THREE,
	KEY_FOUR,
	KEY_FIVE,    
	KEY_SIX,
	KEY_SEVEN,
	KEY_EIGHT,
	KEY_NINE,
	KEY_ASTERISK,
	KEY_POUNDSIGN,
	KEY_DOORBELL,	
	KEY_INSIDEBUTTON,
	KEY_PROX,
	KEY_ONE_HOLD,
	KEY_TWO_HOLD,
	KEY_THREE_HOLD,
	KEY_FOUR_HOLD,
	KEY_FIVE_HOLD,   
	KEY_SIX_HOLD,
	KEY_SEVEN_HOLD,
	KEY_EIGHT_HOLD,
	KEY_NIGHT_HOLD,
	KEY_ZERO_HOLD,
	KEY_ASTERISK_HOLD,
	KEY_POUNDSIGN_HOLD,
	KEY_INSIDEBUTTON_HOLD_LONG,
	KEY_NONE=255
}keycode_t;

typedef enum
{
	SCREEN_Main=0,
	SCREEN_MainMenu=1,
	SCREEN_FpMenu,
	SCREEN_UserFpMenu,
	SCREEN_MasterFpMenu,
	SCREEN_CardUserMenu,
	SCREEN_PasscodeMenu,
	SCREEN_PasscodeUserMenu,
	SCREEN_PasscodeMasterMenu,
	SCREEN_EventLogMenu,
	SCREEN_NetWorkLink,
	SCREEN_SystemConfigMenu,
	SCREEN_RegisterUserFp,
	SCREEN_DeleteUserFp,
	SCREEN_DeleteAllUserFp,
	SCREEN_RegisterMasterFp,
	SCREEN_DeleteMasterFp,
	SCREEN_DeleteAllMasterFp,	//0x10
	SCREEN_RegisterCardUser,
	SCREEN_DeleteCardUser,
	SCREEN_DeleteAllCardUser,
	SCREEN_RegisterPasscodeUser,
	SCREEN_DeletePasscodeUser,
	SCREEN_DeleteAllPasscodeUser,
	SCREEN_TimeSetting,
	SCREEN_ManagerPasscodeModify,
	SCREEN_RestoreFactoryDefault,
	SCREEN_LanguageSetting,
	SCREEN_PickAlarmEnableSetting,
	SCREEN_VolumeSetting,
	SCREEN_MemoryCheck,
	SCREEN_ManagerIdentify,
	SCREEN_DoorBell,
	SCREEN_Initialization,
	SCREEN_LowBattery,	
	SCREEN_IdentifySuccess,//0x20
	SCREEN_IdentifyFail,
	SCREEN_SelfTest,
	SCREEN_AgingTest,
	SCREEN_SystemLocked,
	SCREEN_UnlockingModeSetting,
	SCREEN_CheckEventLogBySequence,
	SCREEN_CheckEventLogByDate,
	SCREEN_DeleteEventLog,
	SCREEN_PickLockAlarm,
	SCREEN_ContinueOpenSetting,
	SCREEN_SystemVersion,
}screen_t;	


typedef struct
{
	uint8_t T1024Hz;	   
	uint8_t T256Hz;    /*                            */
	uint8_t T128Hz;    /*                            */
	uint8_t T64Hz;     /*                            */
	uint8_t T16Hz;     /*                            */
	uint8_t T8Hz;
	uint8_t T2Hz;
	uint8_t T1Hz;     /*                            */
}Timebase_t;


typedef struct
{
	//uint8_t TxBUFFER[20];
	uint8_t TxPoint;
	uint8_t TxLength;
	
	uint8_t RX_Buffer[100];
	uint8_t RX_DataPoint;
	 enum 
	{
		Idle =0,
		ReceivingData=1,
		GotNewCmd
  }Status;
}UartRxMgr_t;

typedef enum
{
	Error_NONE=0,						//		00H ：表示指令执行完毕或无错误
	Error_Fail=1,						//		01H ：表示数据包接收错误； 
	Error_NoFinger, 					//		02H ：表示传感器上没有手指；
	Error_GetImage, 					//		03H ：表示录入指纹图像失败；
	Error_FingerTooDry, 				//		04H ：表示指纹图像太干、淡而生不成特征；
	Error_FingerTooDamp,				//		05H ：表示指纹图像太湿、糊而生不成特征；
	Error_GenChar,						//		06H ：表示指纹图像太乱而生不成特征；
	Error_GenChar1, 					//		07H ：表示指纹图像正常，但特征点太少（或面积小）而生不成；
	Error_NotMatch, 					//		08H ：表示指纹不匹配
	Error_UnRegistered, 				//		09H ：表示 没搜索到指纹；
	Error_Combine,						//		0aH ：表示特征合并失败；
	Error_FpAddrOutRange,				//		0bH ：表示访问指纹库时地址序号超出范围；
	Error_ReadTemplate, 				//		0cH ：表示从指纹库读模板出错或无效；
	Error_UploadChar,					//		0dH ：表示上传特征失败； 
	Error_GetData,						//		0eH ：表示模块不能接后续数据包
	Error_UploadImage,					//		0fH ：表示上传图像失败
	Error_DeleteTemplate,				//		10H ：表示删除模板失败；
	Error_DeleteAll,					//		11H ：表示清空指纹库失败；
	Error_GoToSleep,					//		12H ：表示不能进入低功耗状态；
	Error_Passcode, 					//		13H ：表示口令不正确；
	Error_SystemReset,					//		14H ：表示系统复位失败；
	Error_BufferData,					//		15H ：表示缓冲区内没有效原始图而生不成像；
	Error_OnlineUpdate, 				//		16H ：表示在线升级失败；
	Error_FingerNotRelease, 			//		17H ：表示残留指纹或两次采集之间手没有移动过；
	Error_RWFlash,						//		18H ：表示读写 表示读写FLASH出错；
	Error_GenRandomNumber,				//		19H ：随机数生成失败
	Error_RegisterInvalid,				//		1aH ：无效寄存器号；
	Error_ValueOfRegister,				//		1bH ：寄存器设定内容错误号；
	Error_NotePage, 					//		1cH ：记事本页码指定错误；
	Error_PortOperation,				//		1dH ：端口操作失败；
	Error_AutoEnroll,					//		1eH ：自动注册（enroll）失败；
	Error_MemoryIsFull, 				//		1fH ：指纹库满；
	Error_DeviceAddress,				//		20H ：设备地址错误
	Error_Password, 					//		21H ：密码有误； 
	Error_TemplateIsNotEmpty,			//		22H ：指纹模板非空； 
	Error_TemplateIsEmpty,				//		23H ：指纹模板为空；
	Error_FpMemoryIsEmpty,				//		24H ：指纹库为空； 
	Error_EnrollTimes,					//		25H ：录入次数设置错误；
	Error_TimeOut,						//		26H ：超时；
	Error_TemplateIsRegistered, 		//		27H ：指纹已存在
	Error_TemplateIsUnion,				//		28H ：指纹模板有关联
	Error_Sensor,						//		29H ：传感器初始化失败；
	Error_Reserve,						//		2AH ：Reserved
	Error_SerialNumberMismatched,
	Error_DataPackageF0=0xF0,				//		f0H ：有后续数据包的指令，正确接收用0xf0应答
	Error_DataPackageF1=0xF1,				//		f1H ：有后续数据包的指令，命令包用0xf1应答；
	Error_FlashChecksum=0xF2,			//		f2H ：表示烧写内部FLASH时，校验和错误
	Error_FlashPackageHead=0xF3,		//		f3H ：表示烧写内部FLASH时，包标识错误
	Error_FlashPackageLength=0xF4,		//		f4H ：表示烧写内部FLASH时，包长度错误
	Error_FlashHexFileTooLong=0xF5, 	//		f5H ：表示烧写内部FLASH时，代码长度太长
	Error_FlashWriteFail=0xF6,			//		f6H ：表示烧写内部FLASH时，烧写时，烧写FLASH失败 ；
	Error_BadImage=0xF9,			//		f9H ：采集到的图像不清晰，魔力FPC指纹头
	Error_CheckSum
}FPMcmdErrorType_t;
	
typedef enum
{
	FPMcmdStart=0,
	SendFirstGetImageCmd,
	WaitForFirstGetImageCmdACK,
	SendFirstGenCharCmd,
	WaitForFirstGenCharCmdACK,
	SendDetectFingerFirstTimeRemoveCmd,
	WaitForDetectFingerFirstTimeRemoveCmdACK,
	SendSecondGetImageCmd,
	WaitForSecondGetImageCmdACK,
	SendSecondGenCharCmd,
	WaitForSecondGenCharCmdACK,
	SendDetectFingerSecondTimeRemoveCmd,
	WaitForDetectFingerSecondTimeRemoveCmdACK,
	SendThirdGetImageCmd,
	WaitForThirdGetImageCmdACK,
	SendThirdGenCharCmd,
	WaitForThirdGenCharCmdACK,
	SendDetectFingerThirdTimeRemoveCmd,
	WaitForDetectFingerThirdTimeRemoveCmdACK,
	SendFourthGetImageCmd,
	WaitForFourthGetImageCmdACK,
	SendFourthGenCharCmd,
	WaitForFourthGenCharCmdACK, 	
	SendRegModelCmd,
	WaitForRegModelCmdACK,
	SendStoreCharCmd,
	WaitForStoreCharCmdACK,
	//StartFpUserDelete,		
	SendDeleteCharCmd,
	WaitForDeleteCharCmdACK,
	StartFpUserIdentify,
	SendGetImageCmd,
	WaitForGetImageCmdACK,
	SendGenCharCmd,
	WaitForGenCharCmdACK,
	SendDetectFingerRemoveCmd,
	WaitForDetectFingerRemoveCmdACK,
	SendSearchCmd,
	WaitForSearchCmdACK,
	SendGetSerialNumberCmd,
	WaitForGetSerialNumberCmdACK,
	//StartAllUserFpDelete,
//		SendDeleteAllUserFpCmd,
//		WaitForDeleteAllUserFpCmdACK,
	success,
	fail

}FPMcmdStatus_t;

typedef enum
{
	
	UserUnregistered=0,
	MasterMismatched=1,
	FPMserialNumberMismatched
}UserIdentifyErrorType_t;

typedef struct
{
	uint8_t item_id;
	uint8_t content;

} VoiceMenuItem_t;

typedef struct
{
	uint16_t Para1;
	uint16_t Para2;
	uint8_t Buff[30];
	enum
	{
		WaitACK=0,
		GotACK=1
	}Status;
	enum
	{
		GetImageCmd=0,
		GenCharCmd=1,
		RegModelCmd,
		StoreCharCmd
	}Type;
	FPMcmdErrorType_t ErrorCode;
}FpmAckMgr_t;


typedef enum
{
	ErrorNone=0,
	TimeOut=1,
	UserIDisRegistered,
	UserIDisNotRegistered,
	PasscodeIsBeUsed,
	CardCIDisBeUsed,
	MemoryIsFull,
	SystemNoMaster,
	CanNotDeleteLastOneMaster,
	TwoPasscodesDoNotMatch,
	FingerPrintIsRegistered,
	FpRegisterFail,
	QUIT
}UserMgrErrorType_t;


		
typedef struct
{
	uint16_t UserID;
	int16_t TimeCnt;
	uint8_t EnrollSuccessTimes;
	uint8_t EnrollFailTimes;
	bool_t DuplicateCheck;
	FPMcmdStatus_t Status;
	UserMgrErrorType_t ErrorType;
}FpRegisterMgr_t;


typedef enum
{
	StartFpUserRegister=0,
	InputUserID=1,	
	CheckIfFpUserIDisBeUsed,
	AddUserToFPM,
	DeleteUserFromFPM,
	WaitForDeleteUserACKfromFPM,
	DeleteAllUserFromFPM,
	WaitForDeleteAllUserACKfromFPM,
	WaitForUserConfirmDeleteAllFP,
	SendDeleteAllUserFpCmdToFPM,
	WaitForDeleteAllUserFpCmdACKfromFPM,
	RegisterUserSuccess,
	RegisterUserFail,
	DeleteUserSuccess,
	DeleteUserFail,	
	StartFpIdentify,
	StartFpUserDelete,
	StartAllUserFpDelete,
	DeleteAllFpUserSuccess,
	DeleteAllFpUserFail,
	EXIT
}FpUserMgrStatus_t;

typedef struct
{
	uint16_t UserID;
	uint8_t TimeCnt;
	FpUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
}FpUserRegisterMgr_t;

typedef struct
{
	uint16_t UserID;
	uint8_t TimeCnt;
	FpUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
}FpUserDeleteMgr_t;

typedef struct
{
	FpUserMgrStatus_t Status;
	selection_t Selection;
	uint8_t TimeCnt;
}AllUserFpDeleteMgr_t;

typedef struct
{
	uint16_t UserID;
	uint16_t TimeCnt;
	uint8_t BreathingLED;
	FPMcmdStatus_t Status;
	FPMcmdErrorType_t ErrorType;
}FpIdentifyMgr_t;



typedef struct
{
	uint8_t UserID;
	uint8_t CID[4];
	enum
	{
		CIDisValid=0xAA,
		CIDisInvalid=0xFF
	}Status;
}CardMemoryMgr_t;


typedef enum
{
	StartCardUserRegister=0,
	InputCardUserID=1,
	CheckCardUserIDisRegistered,
	SavedCardID,
	Success,
	Fail,
	StartCardUserIdentify,
	ReadingCardID,
	CheckCardIDifRegistered,
	StartCardUserDelete,
	StartAllCardUserDelete,
	WaitForUserConfirmDeleteAllCard,
	DeletingAllCardUser,
	CardUserEXIT
}CardUserMgrStatus_t;

typedef struct
{
	CardUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	int TimeCnt;
	uint8_t UserID;
	uint8_t CID[5];
}CardRegisterMgr_t;

typedef struct
{
	CardUserMgrStatus_t Status;
	uint16_t TimeCnt;
	uint8_t CID[5];
	uint16_t UserID;
	uint8_t CardDetectIntervalTimeCnt; 
}CardIdentifyMgr_t;

typedef struct
{
	CardUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint8_t TimeCnt;
	uint8_t CID[5];
	uint8_t UserID;
}CardUserDeleteMgr_t;

typedef struct
{
	CardUserMgrStatus_t Status;
	selection_t Selection;
	uint8_t TimeCnt;
}AllCardUserDeleteMgr_t;


typedef struct
{
	uint8_t UserID;
	uint8_t PasscodeBuff[6];
	enum
	{
		PasscodeIsValid=0xAA,
		PasscodeIsInvalid=0xFF
	}Status;
}PasscodeMemoryMgr_t;


typedef enum
{
	StartPasscodeUserRegister=0,
	InputPasscodeUserID=1,
	CheckIfPasscodeUserIDisRegistered,
	InputFirstPasscode,
	InputSecondPasscode,
	CompareTwoPasscode,
	SavePasscode,
	PasscodeIdentifyIdle,
	PasscodeIdentifyStart,
	PasscodeIdentifyPasscodeInput,
	PasscodeIdentifySuccess,
	PasscodeIdentifyFail,
	PasscodeIdentifyAgingTestSuccess,
	PasscodeIdentifyCheckVersionSuccess,
	StartPasscodeUserDelete,
	StartAllPasscodeUserDelete,
	WaitForDeleteAllPasscodeUserConfirm,
	DeletingAllPasscodeUser,
	RegisterPasscodeUserSuccess,
	RegisterPasscodeUserFail,
	DeletePasscodeUserSuccess,
	DeletePasscodeUserFail,
	DeleteAllPasscodeUserSuccess,
	DeleteAllPasscodeUserFail,
	PasscodeUserEXIT,
	ModifyManagerPasscodeStart,
	ModifyManagerPasscodeFail,
	ModifyManagerPasscodeSuccess
}PasscodeUserMgrStatus_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint16_t TimeCnt;
	uint8_t UserID;
	uint8_t Passcode[6];
	UserPriority_t UserPriority;
	int16_t OverTimeCnt;
}PasscodeUserRegisterMgr_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint8_t TimeCnt;
	//uint8_t Passcode[6];
	uint8_t UserID;
	UserPriority_t UserPriority;
}PasscodeUserDeleteMgr_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	selection_t Selection;
	uint8_t TimeCnt;
}AllPasscodeUserDeleteMgr_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint16_t TimeCnt;
	uint8_t Passcode[10];
	uint8_t UserID;
}PasscodeUserIdentifyMgr_t;




typedef struct
{
	uint16_t CWTimer;
	uint16_t CCWTimer;
	uint16_t CWWAITTimer;
	uint16_t CCWWAITTimer;
	bool_t 	CWSoftStart;
	bool_t 	CCWSoftStart;
	enum
		{
			IDLE=0,
			CW=1,
			CCW,
			CWWAIT,
			CCWWAIT
		}MotorStatus;
}MotorMgr_t;

typedef struct
{
	uint16_t SleepDelayTimerCnt;
	uint16_t AwakeTimerCnt;
	enum
	{
		RUN=0,
		SLEEP=1
	}Status;
	enum
	{
		SleepTimer=0,
		UserForced=1
	}SleepSource;
	enum
	{
		FingerTouch=0,
		KeyBoardTouch=1,
		CasingOpen,
		RTCwakeupTimer,
		SystemResetKey,
		AntiPrying,
		DoNotCare
	}AwakeSource;
		
	uint8_t	SavedCasingStatus;
}SystemPowerMgr_t;

typedef struct
{
	uint8_t BatteryVoltage;
	uint8_t ProtectVoltageTriggerTimes;
	bool_t LowBatteryProtectionEnabled;
	bool_t PostLowBattery;
	enum
	{
		LEVEL_0=0,
		LEVEL_1=1,
		LEVEL_2,
		LEVEL_3,
		LEVEL_4,
		LEVEL_5,
		LEVEL_6,
		LEVEL_7
	}BatteryLevel;
	uint8_t TimeCnt;
}BatteryMgr_t;

typedef struct
{
	enum{
		F1K=0,
		F2K,
		F4K
	}HZ;
	uint8_t TimeCnt;	
}BeepMgr_t;

typedef struct
{
	enum
	{
		VoiceIdle=0,
		SendingDataStart=1,
		SendingData,
		SendingDataEnd,
		ContinueSendData
	}Status;
	uint8_t BitPoint;
	uint16_t PulseWidthCnt;
	uint8_t Data;
	uint8_t DataBuff[50];
	uint8_t SendDataNum;
	uint8_t TotalDataNum;
	uint16_t VoicePlayEndCnt;
	bool_t VoicePlayEnd;
}VoiceDataTransferMgr_t;

typedef struct
{
	uint16_t UserID;
	enum
	{
		UnRegistered=0,
		Registered=1
	}RegisterStatus;
	UserPriority_t UserPriority;
}
FpUserMemoryMgr_t;
/*
typedef struct
{
	enum
	{
		MagnetIdle=0,
		MagnetBusy=1
	}Status;
	uint8_t TimeCnt;
}EMAGNETMgr_t;
*/

typedef struct
{
	enum
	{
		StartRestoreFactoryDefault = 0,
		WaitForRestoreFactoryDefaultUserConfirm = 1,
		SaveFPMserialNumber,
		ConfirmedToRestoreFactoryDefault,
		ResetFPMuser,
		ResetCardUser,
		ResetPasscodeUser,
		RestoreFactorySuccess,
		RestoreFactoryFail,
		RestoreFactoryDefaultEXIT
	}Status;
	selection_t Selection;
	uint8_t TimeCnt;
	uint8_t FailTimes;
}RestoreFactoryDefaultMgr_t;

typedef struct
{
	enum
	{
		StartVoiceEnableSetting = 0,
		WaitForVoiceEnableSettingUserConfirm = 1,
		VoiceEnableSettingSuccess,
		VoiceEnableSettingEXIT
	}Status;
	bool_t Enable;
	selection_t Selection;
	uint8_t volume;
	uint8_t TimeCnt;
}VoiceMgr_t;

typedef struct
{
	enum
	{
		StartPickAlarmEnableSetting = 0,
		WaitForPickAlarmEnableSettingUserConfirm = 1,
		PickAlarmEnableSettingSuccess,
		PickAlarmEnableSettingEXIT
	}Status;
	bool_t Enable;
	selection_t Selection;
	uint8_t TimeCnt;
}PickAlarmEnableMgr_t;


typedef struct
{
	enum
	{
		StartCheckHowManyRegisteredFPuser = 0,
		WaitForCheckHowManyRegisteredFPuserCmdAck = 1,
		CheckHomManyRegisteredFPuserSuccess,
		CheckHomManyRegisteredFPuserFail
	}Status;
	uint8_t UserNum;
	uint8_t MasterNum;
	uint8_t TimeCnt;
	uint8_t FailTimes;
}CheckHomManyRegisteredFPuser_t;

typedef struct
{
	enum
	{
		StartCheckIfFpUserIDisRegistered = 0,
		WaitForCheckIfFpUserIDisRegisteredCmdAck = 1,
		CheckIfFpUserIDisRegisteredSuccess,
		CheckIfFpUserIDisRegisteredFail
	}Status;
	bool_t UserIDisRegistered;
	uint8_t TimeCnt;
}CheckIfFpUserIDisRegistered_t;


typedef struct
{
	enum
	{
		StartCheckMemory = 0,
		WaitForReadFPuserNum = 1,
		CheckMemorySuccess,
		CheckMemoryFail
	}Status;
	uint8_t TimeCnt;
	uint8_t FpUserNum;
	uint8_t FpMasterNum;
	uint8_t CardUserNum;
	uint8_t PasscodeUserNum;
	uint8_t PasscodeMasterNum;
	bool_t NeedRereadUserFromMemory;
}CheckMemoryMgr_t;

typedef struct
{
	enum
	{
		DisplayFpUserMemoryUsage=0,
		DisplayPasscodeUserMemoryUsage=1,
		DisplayCardUserMemoryUsage,
		DisplayMemoryUsageEnd	
	}Status;

	uint8_t TimeCnt;
}DisplayMemoryUsageMgr_t;

typedef struct
{
	enum
	{
		StartInitialization=0,
		MemoryCheck=1,
		MemoryCheckWait,
		FingerPrintCheck,
		FingerPrintCheckWait,
		CardReaderCheck,
		CardReaderCheckWait,
		TouchCheck,
		TouchCheckWait,
		CheckFinished
	}Status;
	bool_t MemoryCheckPass;
	bool_t FingerPrintCheckPass;
	bool_t CardReaderCheckPass;
	bool_t TouchCheckPass;
	uint16_t TimeCnt;
}InitializationMgr_t;

typedef struct
{
	enum
	{
		StartManagerIdentify = 0,
		WaiForManagerPasscodeOrFP = 1,
		ManagerIdentifySuccess,
		ManagerIdentifyFail,
		ManagerIdentifyExit
	}Status;

	enum
	{
		Invalid = 0xF0,
		Valid = 0xE8
	}PasscodeValidFlag;
	uint8_t	PasscodeBuff[6];
	uint8_t TimeCnt;
	uint8_t PasscodeUserID;
	UserIdentifyErrorType_t ErrorType;
}ManagerIdentifyMgr_t;

typedef struct
{
	PasscodeUserMgrStatus_t Status;
	UserMgrErrorType_t ErrorType;
	uint16_t TimeCnt;
	uint8_t UserID;
	uint8_t Passcode[6];
}ManagerPasscodeModifyMgr_t;

typedef struct
{
	uint8_t PasscodeBuff[12];
	uint8_t UserID;
}ManagerPasscodeUserId_t;

//typedef struct
//{
//	uint8_t  Serialnum[4];
//	uint8_t  CountSerialnum[4];
//	uint8_t  Countnum[3];
//	uint8_t  TempCountnum[3];
//	uint8_t  CountRandnum[2];
//	ManagerPasscodeUserId_t  ManagerPswUserId;

//}ManagerID_t;


typedef struct
{
	status_t CardIdentifyStatus;
	status_t FPIdentifyStatus;
	status_t PasscodeIdentifyStatus;
	enum
	{
		LocalPasscode=0,
		AppPasscode=1,
		WifiTempPasscode
	}PasscodeType;
	enum
	{
		CARD = 1,
		PASSCODE = 2,
		FINGERPRINT,
		FINGERPRINTandCARD,
		FINGERPRINTandPASSCODE,
		CARDandPASSCODE,
		RemoteUnlock,
		WIFITEMPPASSCODE,
		INITIALSTATUS
	}IdentifyType;
	uint8_t FPUserID;
	uint8_t CardUserID;
	uint8_t PasscodeUserID;
	int TimeCnt;
	uint8_t VoiceDelay;
	enum
	{
		SingalMode = 0,
		DoubleMode = 1
	}UnlockingMode;
	bool_t FlagContinuedOpenEnabled;
	UserIdentifyErrorType_t ErrorType;

 }UserIdentifyResultMgr_t;

typedef struct
{
	enum
	{
		SELFTEST_START = 0,
		OLED_TEST = 1,
		KEYandLED_TEST,
		FPM_TEST,
		MOTOR_TEST,
		EMAGNET_TEST,
		VOICE_TEST,
		EEPROM_TEST,
		FLASH_TEST,
		RFID_TEST,
		VOLTAGE_TEST,
		LOWPOWER_TEST
	}Status;
	enum
	{
		Unknow=0,
		Passed=1,
		Failed
	}TestResult;
	uint16_t TimeCnt;
	uint16_t ButtonStatus;
}SelfTestMgr_t;


typedef struct
{
	uint8_t FpIdentifyFailedTimes;
	uint8_t CardIdentifyFailedTimes;
	uint8_t PasscodeIdentifyFailedTimes;
	uint8_t ManagerPasscodeIdentifyFailedTimes;		
	bool_t SystemLocked;
	uint16_t SystemLockedTimeDelay;
}SafetyMonitorMgr_t;

typedef struct
{
	enum
	{
		UnlockingModeSettingStart = 0,
		WaitForUserConfirmUnlockingMode = 1,
		UserConfirmedUnlockingMode,
		UnlockingModeSettingSuccess
	}Status;
	uint16_t TimeCnt;
}UnlockingModeMgr_t;

typedef struct
{
	enum
	{
		FPMpowerDown = 0,
		FPMpowerOn=1
	}Status;
}FPMpowerMgr_t;

typedef struct
{
	enum
	{
		TouchPowerDown = 0,
		TouchPowerOn=1,
		LowSensitivity,
		HighSensitivity,
		NoSensing
	}Status;
}TouchPowerMgr_t;


typedef struct
{
	uint16_t TestTimes;
	
}AgingTestMgr_t;

typedef enum
{
	OpenDoor = 0,
	ChangSystemConfig=1,
	AddFpUser,
	DeleteFpUser,
	DeleteAllFpUser,
	DeleteAllStressFpUser,
	AddPasscodeUser,
	DeletePasscodeUser,
	DeleteAllPasscodeUser,
	AddCardUser,
	DeleteCardUser,
	DeleteAllCardUser
}EventLogType_t;

typedef union 
{
	struct
	{
		uint8_t 				LogIDmajor	:1;//低位
		UserType_t 			UserType		:3;
		EventLogType_t 	EventType		:4;//高位
	}_Bit;
	uint8_t _Byte;
}LogOrType_t;

typedef struct
{
	uint8_t FlagHighByte;
	uint8_t FlagLowByte;
	LogOrType_t LogOrType;
	uint32_t UTC;
	uint8_t UserID;
	
	//uint8_t LogIDmajor;
	
//	uint8_t year;
//	uint8_t month;
//	uint8_t date;
//	uint8_t hour;
//	uint8_t minute;
//	uint8_t second;
	
	//UserType_t UserType;
	//EventLogType_t EventType;
	
	//uint8_t EventParameter;
	//uint8_t Remain1;
	//uint8_t Remain2;
	//uint8_t Remain3;
}Log_t;	//total 8 bytes

typedef struct
{
	uint16_t LastPoint;
	uint16_t DisplayPoint;
	uint16_t SavedDisplayPoint;
	Log_t NewLog;
	Log_t DisplayLog;
}LogMgr_t;

typedef struct
{

	uint16_t DeleteSectorNum;
	uint8_t Percentage;
	selection_t Selection;
	enum
	{
		StartLogDelete=0,
		WaitforLogDeleteCofirm=1,
		LogDeleting,
		LogDeleteSuccess,
		QuitLogDelete
	}Status;
}LogDeleteMgr_t;


typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint16_t MatchedEventLogNum;
	uint16_t OffsetEventLogNum;
	uint16_t StartEventLogPoint;
	enum
	{
		SetYear = 0,
		SetMonth=1,
		SetDate,
		SearchEventLogWithDate,
		ShowEventLogWithDate
	}Status;
}CheckEventLogByDateMgr_t;

typedef struct
{
	uint16_t MatchedEventLogNum;
	uint16_t OffsetEventLogNum;
	uint16_t StartEventLogPoint;
	enum
	{
		SearchEventLogWithSequence=0,
		VoiceReportOperation=1,
		VoiceReportEventLogWithSequence,
		VoiceReportNoEventLog
	}Status;
	uint16_t TimeCnt;
}CheckEventLogBySequenceMgr_t;

typedef struct
{
	enum
	{
		ReportLogIdle=0,
		ReportLogInit=1,	
		ReportLogID,
		ReportLogType,
		ReportLogTime,
		ReportLogUser,
		ReportLogEnd
	}Status;
	uint16_t TimeCnt;
}VoiceReportLogMgr_t;


typedef struct
{
	bool_t LockBrandDataValid;
	bool_t GotBrandData;
	uint8_t LockBrandDisDataBuff[224];
	uint8_t BrandChangeTimeDelay;
}LockBrand_t;


typedef struct
{
	bool_t AntiPryingSignalRelease;
	bool_t AntiPryingTrigger;
	uint16_t TimeCnt;
}AntiPryingMgr_t;

typedef struct
{
	enum
	{ 
		IsNotMatched =0,
		IsMatched = 1,
		Unkonw
	}SNmatched;
	bool_t SNavariable;
	uint8_t SN[32];
}FPMserialNumberManager_t;

typedef struct
{
	bool_t Key0ShouldBeOn;
	bool_t Key1ShouldBeOn;
	bool_t Key2ShouldBeOn;
	bool_t Key3ShouldBeOn;
	bool_t Key4ShouldBeOn;
	bool_t Key5ShouldBeOn;
	bool_t Key6ShouldBeOn;
	bool_t Key7ShouldBeOn;	
	bool_t Key8ShouldBeOn;
	bool_t Key9ShouldBeOn;
	bool_t KeyAsteriskShouldBeOn;
	bool_t KeyPoundSignShouldBeOn;
	keycode_t Keycode;
}LEDsMgr_t;

//typedef enum
//{
//	Blue=0,
//	Green=1,
//	Red=2
//}FpLEDState_t;

typedef enum
{
	Chinese=0,
	English=1
}SystemLanguage_t;

typedef struct
{
	uint8_t FWversion;
	uint8_t HWversion;	
}DriverBoardVersion_t;

typedef struct
{
	uint16_t TimeCnt;
}DoorBellMgr_t;

typedef struct
{
	bool_t IsDoorColseKeyAwake;
	bool_t IsDoorBellKeyAwake;
	bool_t IsPoundsignKeyAwake;
}AwakeSystemKeyMgr_t;

#ifdef MAIN_C
#define GLOBAL /*********/
#else
#define GLOBAL extern 
#endif
	
#endif /* #ifndef STDTYPES_H */ 
/* *************** END OF FILE    STDTYPES_H ****************************** */
