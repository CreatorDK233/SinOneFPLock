#include "Project.h"
#include "RTC.h"
#include "LCD.h"
#include "Font.h"
#include "Font_Menu.h"
#include "Flash.h"

code uint8_t DEF_LogFlagHighByte = 0x55;
code uint8_t DEF_LogFlagLowByte = 0xAA;

code uint16_t DEF_MaxRecordedLogID = 512;  //512*8=4K   TOTAL 4K
code uint16_t DEF_MaxSearchLogID = 500;	

#define DEF_EachLogAddrIncrease 64/DEF_InternalFlash_MemoryBit

void LogMgr_WriteLog(uint16_t LogPoint)
{
#ifdef Function_EventLog
	uint16_t Addr;
	uint8_t *point;
	
	Addr = LogPoint*DEF_EachLogAddrIncrease+DEF_InternalFlash_LogMemoryStartAddr;
	point = &LogMgr.NewLog.FlagHighByte;
	FLASH_WriteSequential(Addr,point,DEF_EachLogAddrIncrease);
#endif
}

void LogMgr_ReadLog(uint16_t LogPoint,uint8_t *point)
{
#ifdef Function_EventLog
	uint16_t Addr;

	Addr = LogPoint*DEF_EachLogAddrIncrease+DEF_InternalFlash_LogMemoryStartAddr;
	FLASH_ReadSequential(Addr,point,DEF_EachLogAddrIncrease);
#endif
}


status_t JudgeLogSaved(uint16_t LogPoint)
{
	#ifndef Function_EventLog
            return S_FAIL;
	#else
	Log_t TempLog;
	LogMgr_ReadLog(LogPoint,&TempLog.FlagHighByte);
	if ( (TempLog.FlagHighByte == DEF_LogFlagHighByte)
		 &&(TempLog.FlagLowByte == DEF_LogFlagLowByte) )
	{
		return S_SUCCESS;
	}
	return S_FAIL;
	#endif
}

status_t DeleteAllLog(void)
{
	#ifndef Function_EventLog
            return S_FAIL;
	#else
	uint8_t i;
	uint16_t SectorAddr;
	
	for (i=0;i<DEF_InternalFlash_SectorNum;i++)
	{
		SectorAddr=(DEF_InternalFlash_LogMemoryStartPage+i)*DEF_InternalFlash_SectorSize;
		if ( Flash_EraseOnePage(SectorAddr) == S_FAIL )
		{
			DEBUG_MARK;
			return S_FAIL;
		}
	}
	return S_SUCCESS;
	#endif
}

void SearchEventLogBySequence(void)
{
	#ifndef Function_EventLog
            return;
	#else
	uint16_t LogID,i;
	Log_t TempLog; 

	if ( LogMgr.LastPoint >0 )
	{
		LogID = LogMgr.LastPoint-1;
	}
	else{
		LogID = DEF_MaxRecordedLogID-1;
	}

	CheckEventLogBySequenceMgr.MatchedEventLogNum = 0;
	
	for (i=0;i<DEF_MaxSearchLogID;i++)
	{
		CLRWDT();
		
		LogMgr_ReadLog(LogID,&TempLog.FlagHighByte);
		if ( 	(TempLog.FlagHighByte == DEF_LogFlagHighByte)
				&&(TempLog.FlagLowByte == DEF_LogFlagLowByte)
			)
		{	
			if ( CheckEventLogBySequenceMgr.MatchedEventLogNum == 0 )
			{
				CheckEventLogBySequenceMgr.StartEventLogPoint = LogID;	//first matched Log, saved to start point memory
			}
			CheckEventLogBySequenceMgr.MatchedEventLogNum++;
		}
		else if ( (TempLog.FlagHighByte != DEF_LogFlagHighByte)
				||(TempLog.FlagLowByte != DEF_LogFlagLowByte)
				)
		{
			break;
		}

		if (LogID>0)
		{
			LogID--;
		}
		else
		{
			if ( JudgeLogSaved(DEF_MaxRecordedLogID-1) == S_SUCCESS )
			{
				LogID = DEF_MaxRecordedLogID-1;
			}
			else
			{
				break;
			}
		}
	}
	#endif
}

void GUI_CreatAndSaveLog(EventLogType_t LogType)//GUI创建和保存日志
{
	uint32_t TempUTC;
	if ( LogType == OpenDoor )
	{
		if ( UserIdentifyResultMgr.IdentifyType == CARD ){
			LogMgr.NewLog.LogOrType._Bit.UserType = CARDUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.CardUserID;
			LogMgr.NewLog.LogOrType._Bit.EventType = OpenDoor;
			//LogMgr.NewLog.EventParameter = 0x00;
		}
		else if ( UserIdentifyResultMgr.IdentifyType == FINGERPRINT ){
			LogMgr.NewLog.LogOrType._Bit.UserType = FPUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.FPUserID;
			LogMgr.NewLog.LogOrType._Bit.EventType = OpenDoor;
			//LogMgr.NewLog.EventParameter = 0x00;
		}
		else if (UserIdentifyResultMgr.IdentifyType == PASSCODE )
		{
			LogMgr.NewLog.LogOrType._Bit.UserType = PASSCODEUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.PasscodeUserID;
			LogMgr.NewLog.LogOrType._Bit.EventType = OpenDoor;
			//LogMgr.NewLog.EventParameter = 0x00;	
		}
		else if (UserIdentifyResultMgr.IdentifyType == FINGERPRINTandCARD ){
			LogMgr.NewLog.LogOrType._Bit.UserType = FPUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.FPUserID;
			LogMgr.NewLog.LogOrType._Bit.EventType = OpenDoor;
			//LogMgr.NewLog.EventParameter = 0x00;	
		}
		else if (UserIdentifyResultMgr.IdentifyType == FINGERPRINTandPASSCODE ){
			LogMgr.NewLog.LogOrType._Bit.UserType = FPUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.FPUserID;
			LogMgr.NewLog.LogOrType._Bit.EventType = OpenDoor;
			//LogMgr.NewLog.EventParameter = 0x00;

		}
		else if (UserIdentifyResultMgr.IdentifyType == CARDandPASSCODE ){
			LogMgr.NewLog.LogOrType._Bit.UserType = PASSCODEUSER;
			LogMgr.NewLog.UserID = UserIdentifyResultMgr.PasscodeUserID;
			LogMgr.NewLog.LogOrType._Bit.EventType = OpenDoor;
			//LogMgr.NewLog.EventParameter = 0x00;	
		}
		else if (UserIdentifyResultMgr.IdentifyType == INITIALSTATUS )
		{
			return;		//do nothing
		}
		else
		{
			return;		//do nothing
		}
	}
	else if ( LogType == AddFpUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = FPUSER;
		LogMgr.NewLog.UserID = FpUserRegisterMgr.UserID;
		LogMgr.NewLog.LogOrType._Bit.EventType = AddFpUser;
		//LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteFpUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = FPUSER;
		LogMgr.NewLog.UserID = FpUserDeleteMgr.UserID;
		LogMgr.NewLog.LogOrType._Bit.EventType = DeleteFpUser;
		//LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteAllFpUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = FPUSER;
		LogMgr.NewLog.UserID = 0x00;
		LogMgr.NewLog.LogOrType._Bit.EventType = DeleteAllFpUser;
//		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteAllStressFpUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = FPUSER;
		LogMgr.NewLog.UserID = 0x00;
		LogMgr.NewLog.LogOrType._Bit.EventType = DeleteAllStressFpUser;
//		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == AddPasscodeUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = PASSCODEUSER;
		LogMgr.NewLog.UserID = PasscodeUserRegisterMgr.UserID;
		LogMgr.NewLog.LogOrType._Bit.EventType = AddPasscodeUser;
//		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeletePasscodeUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = PASSCODEUSER;
		LogMgr.NewLog.UserID = PasscodeUserDeleteMgr.UserID;
		LogMgr.NewLog.LogOrType._Bit.EventType = DeletePasscodeUser;
//		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteAllPasscodeUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = PASSCODEUSER;
		LogMgr.NewLog.UserID = 0x00;
		LogMgr.NewLog.LogOrType._Bit.EventType = DeleteAllPasscodeUser;
//		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == AddCardUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = CARDUSER;
		LogMgr.NewLog.UserID = CardUserRegisterMgr.UserID;
		LogMgr.NewLog.LogOrType._Bit.EventType = AddCardUser;
//		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteCardUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = CARDUSER;
		LogMgr.NewLog.UserID = CardUserDeleteMgr.UserID;
		LogMgr.NewLog.LogOrType._Bit.EventType = DeleteCardUser;
//		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else if ( LogType == DeleteAllCardUser )
	{
		LogMgr.NewLog.LogOrType._Bit.UserType = CARDUSER;
		LogMgr.NewLog.UserID = 0x00;
		LogMgr.NewLog.LogOrType._Bit.EventType = DeleteAllCardUser;
//		LogMgr.NewLog.EventParameter = 0x00;	
	}
	else
	{
		return;		
		//do nothing
	}
	TempUTC = SystemTimeToUTC(SystemTime);
	LogMgr.NewLog.UTC = TempUTC;

	LogMgr.NewLog.FlagHighByte = DEF_LogFlagHighByte;
	LogMgr.NewLog.FlagLowByte = DEF_LogFlagLowByte;
	
	LogMgr_WriteLog(LogMgr.LastPoint);
		
	if ( LogMgr.LastPoint < (DEF_MaxRecordedLogID-1) )
	{
		LogMgr.LastPoint++;
		//LogMgr.DisplayPoint = LogMgr.LastPoint-1;
	}
	else
	{
		LogMgr.NewLog.LogOrType._Bit.LogIDmajor = ~ LogMgr.NewLog.LogOrType._Bit.LogIDmajor;
		LogMgr.LastPoint=0x0000;
		//LogMgr.DisplayPoint = 999;
	}
	
}

void DisplayOneEventLog(uint16_t EventLogID)//显示一个事件日志
{
	#ifdef Function_ScreenDisplay
	systemtime_t LogTime;
	
	if ( LogMgr.DisplayLog.LogOrType._Bit.EventType == OpenDoor ) 
	{
			DisHZ16x14Str(0,24,UnlockLogStr,NormalDisplay);
	}
	else if (( LogMgr.DisplayLog.LogOrType._Bit.EventType == AddFpUser )
			||( LogMgr.DisplayLog.LogOrType._Bit.EventType == AddPasscodeUser )
			||( LogMgr.DisplayLog.LogOrType._Bit.EventType == AddCardUser ))
	{
			DisHZ16x14Str(0,24,AddUserStr,NormalDisplay);
	}

	else if (( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteFpUser )
			||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteAllStressFpUser )
			||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeletePasscodeUser )
			||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteCardUser )
			||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteAllFpUser )
			||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteAllCardUser )
			||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteAllPasscodeUser ) 
			)
	{
			DisHZ16x14Str(0,24,DeleteUserStr,NormalDisplay);
	}

	DisDigital16x8Str(0,88,EventLogID,NormalDisplay);
	
	LogTime = UTCToSystemtime(LogMgr.DisplayLog.UTC);
	DisBcdDigital16x8(2,28,LogTime.year,NormalDisplay);
	DisZF16x8(2,44,ZF_hengan,NormalDisplay);
	DisBcdDigital16x8(2,52,LogTime.month,NormalDisplay);
	DisZF16x8(2,68,ZF_hengan,NormalDisplay);
	DisBcdDigital16x8(2,76,LogTime.date,NormalDisplay);
	
	DisBcdDigital16x8(4,28,LogTime.hour,NormalDisplay);
	DisZF16x8(4,44,ZF_maohao,NormalDisplay);
	DisBcdDigital16x8(4,52,LogTime.minute,NormalDisplay);
	DisZF16x8(4,68,ZF_maohao,NormalDisplay);
	DisBcdDigital16x8(4,76,LogTime.second,NormalDisplay);

	if ( LogMgr.DisplayLog.LogOrType._Bit.UserType == CARDUSER )
	{
			DisHZ16x14Str(6,20,CardUserStr,NormalDisplay);
	}
	else if ( LogMgr.DisplayLog.LogOrType._Bit.UserType ==  FPUSER )
	{
			DisHZ16x14Str(6,20,FpUserStr,NormalDisplay);
	}
	else if (LogMgr.DisplayLog.LogOrType._Bit.UserType ==	PASSCODEUSER )
	{
			DisHZ16x14Str(6,20,PasscodeUserStr,NormalDisplay);
	}
	else if (LogMgr.DisplayLog.LogOrType._Bit.UserType == ONLINEPASSCODEUSER )
	{
			DisHZ16x14Str(6,20,TemporaryPasswordStr,NormalDisplay);
	}
	DisEN16x8Str(6,80,":",NormalDisplay);
	
	if (( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteAllFpUser )
		||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteAllStressFpUser )
		||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteAllCardUser )
		||( LogMgr.DisplayLog.LogOrType._Bit.EventType == DeleteAllPasscodeUser )
		)
	{
			DisHZ16x14Str(6,92,LotStr,NormalDisplay);
	}
	else
	{
		DisDigital16x8Str(6,92,LogMgr.DisplayLog.UserID,NormalDisplay);
	}
	#endif
}

//void SearchEventLogByDate(void)
//{
//	#ifndef Function_EventLog
//            return;
//	#else
//	uint16_t LogID,i;
//	Log_t TempLog; 

//	if ( LogMgr.LastPoint >0 )
//	{
//		LogID = LogMgr.LastPoint-1;
//	}
//	else{
//		LogID = 511;
//	}

//	CheckEventLogByDateMgr.MatchedEventLogNum = 0;
//	
//	for (i=0;i<DEF_MaxSearchLogID;i++)
//	{
//		LogMgr_ReadLog(LogID,&TempLog.FlagHighByte);
//		if ( 	(TempLog.FlagHighByte == DEF_LogFlagHighByte)
//				&&(TempLog.FlagLowByte == DEF_LogFlagLowByte)
//				&&(TempLog.year == CheckEventLogByDateMgr.year)
//				&&(TempLog.month == CheckEventLogByDateMgr.month)
//				&&(TempLog.date == CheckEventLogByDateMgr.date)
//			)
//		{	
//			if ( CheckEventLogByDateMgr.MatchedEventLogNum == 0 )
//			{
//				CheckEventLogByDateMgr.StartEventLogPoint = LogID;	//first matched Log, saved to start point memory
//			}
//			CheckEventLogByDateMgr.MatchedEventLogNum++;
//		}
//		else if ( (TempLog.FlagHighByte != DEF_LogFlagHighByte)
//				||(TempLog.FlagLowByte != DEF_LogFlagLowByte) )
//		{
//			break;
//		}
//		if (LogID>0)
//		{
//			LogID--;
//		}
//		else
//		{
//			if ( JudgeLogSaved(DEF_MaxRecordedLogID-1) == S_SUCCESS )
//			{
//				LogID = DEF_MaxRecordedLogID-1;
//			}
//			else
//			{
//				break;
//			}
//		}
//	}
//	#endif
//}

void LogMgr_Init(void)
{
	#ifndef Function_EventLog
            return;
	#else
	uint16_t LogID=0;
	bit FirstLogIDmajor;
	bool_t OneTurn = bTRUE;
	Log_t TempLog; 
	LogMgr.LastPoint = 0x0000;
	LogMgr_ReadLog(LogID,&TempLog.FlagHighByte);
	FirstLogIDmajor = TempLog.LogOrType._Bit.LogIDmajor;
	LogMgr.NewLog.LogOrType._Bit.LogIDmajor = FirstLogIDmajor;
	
	for (LogID=0;LogID<DEF_MaxRecordedLogID;LogID++)
	{
		LogMgr_ReadLog(LogID,&TempLog.FlagHighByte);
		if 	( (TempLog.FlagHighByte == DEF_LogFlagHighByte)
				&&(TempLog.FlagLowByte == DEF_LogFlagLowByte) )
				//&&(TempLog.LogOrType._Bit.LogIDmajor == FirstLogIDmajor)	)
		{
			//Temp_LogIDmajor = TempLog.LogOrType._Bit.LogIDmajor;
			//LogMgr.NewLog.LogOrType._Bit.LogIDmajor = TempLog.LogOrType._Bit.LogIDmajor;
		}
		else
		{
			if( LogID == 0 )
			{
				LogMgr.NewLog.LogOrType._Bit.LogIDmajor = ~FirstLogIDmajor;
			}
			LogMgr.LastPoint = LogID;
			OneTurn = bFALSE;
			break;
		}
	}
	
	if( OneTurn == bTRUE )
	{
		LogMgr.LastPoint = 0;
		LogMgr.NewLog.LogOrType._Bit.LogIDmajor = ~FirstLogIDmajor;
	}

	DEBUG_MARK;

	#endif

}

