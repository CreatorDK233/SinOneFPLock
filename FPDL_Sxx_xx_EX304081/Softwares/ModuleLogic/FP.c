#include "FingerPrint.h"
#include "EEPROM.h"
#include "BeepMgr.h"
#include "LCD.h"
#include "FP.h"
#include "GUI_Function.h"
#include "global_variable.h"
#include "Basic_Function.h"
#include "Font_Menu.h"

void RegisterFp(uint16_t UserID)//注册指纹
{
	
	FpRegisterMgr.UserID = UserID;
	
	if ( FpRegisterMgr.Status == FPMcmdStart )
	{
		FpRegisterMgr.Status = SendGetImageCmd;
		FpRegisterMgr.TimeCnt = Def_WaitUserPutFingerTimeDelay; 	
		FpRegisterMgr.EnrollSuccessTimes = 1;
		FpRegisterMgr.EnrollFailTimes=0;
		#ifdef Function_NoDuplicateFp
		FpRegisterMgr.DuplicateCheck = bFALSE;
		#else
		FpRegisterMgr.DuplicateCheck = bTRUE;
		#endif
		PLAY_VOICE_ONESEGMENT(VOICE_PleasePutFinger);
		Hardware_DelayMs(150);
		#ifdef Function_ScreenDisplay
		DisHZ16x14Str(3,14,PutFingerStr,NormalDisplay);
		#endif
	}
	else if (FpRegisterMgr.Status == SendGetImageCmd)
	{
		
		FpRegisterMgr.Status = WaitForGetImageCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendGetEnrollImageCmd();
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForGetImageCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			DEBUG_MARK;
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpRegisterMgr.Status = SendGenCharCmd;
				RefreshSystemSleepTime();
			}
			else
			{
				if ( (FpmAckMgr.ErrorCode == Error_GetImage)
					||(FpmAckMgr.ErrorCode == Error_NoFinger)
					||(FpmAckMgr.ErrorCode == Error_BadImage)
					)
				{
					FpRegisterMgr.Status = SendGetImageCmd;
					if ( FpmAckMgr.ErrorCode == Error_NoFinger )
					{
						--FpRegisterMgr.TimeCnt;
						if ( --FpRegisterMgr.TimeCnt < 1 )
						{
							FpRegisterMgr.Status = fail;
							FpRegisterMgr.ErrorType = TimeOut;
						}
					}
				}
				else
				{
					FpRegisterMgr.Status = fail;
					FpRegisterMgr.ErrorType = FpRegisterFail;
					DEBUG_MARK;
				}
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}

	}
	else if ( FpRegisterMgr.Status == SendGenCharCmd )
	{
		
		FpRegisterMgr.Status = WaitForGenCharCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendGenCharCmd(FpRegisterMgr.EnrollSuccessTimes);		//BuffID start from 0x01
		FpRegisterMgr.TimeCnt = Def_FPMcmdTimeOutDelay; 	
		DEBUG_MARK;
	}

	else if ( FpRegisterMgr.Status == WaitForGenCharCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{	
				if ((FpRegisterMgr.EnrollSuccessTimes == 1 )&&(FpRegisterMgr.DuplicateCheck == bTRUE ))
				{
					FpRegisterMgr.EnrollSuccessTimes+=1;
					FpRegisterMgr.Status = SendSearchCmd;
					FpRegisterMgr.DuplicateCheck = bFALSE;	
					FpRegisterMgr.TimeCnt =  Def_FPMcmdTimeOutDelay;
				}
				else if (FpRegisterMgr.EnrollSuccessTimes < DEF_FPM_EnrollTimes )
				{
					FpRegisterMgr.Status = SendDetectFingerRemoveCmd;
					FpRegisterMgr.TimeCnt = Def_WaitUserRemoveFingerTimeDelay;	
					FpRegisterMgr.EnrollSuccessTimes+=1;
					PLAY_VOICE_ONESEGMENT(VOICE_PleasePutFingerAgain);
					Hardware_DelayMs(150);
					#ifdef Function_ScreenDisplay
					DisHZ16x14Str(3,14,PutFingerStr,NormalDisplay);
					#endif
				}
				else 
				{
					FpRegisterMgr.Status = SendRegModelCmd;
					FpRegisterMgr.TimeCnt =  Def_FPMcmdTimeOutDelay;
				}
				RefreshSystemSleepTime();
			}
			else if ((FpmAckMgr.ErrorCode == Error_GenChar)
								||(FpmAckMgr.ErrorCode == Error_GetImage)	
							)
			{
				if ( FpRegisterMgr.EnrollFailTimes < DEF_FPM_EnrollFailTimes_Limited)
				{
					FpRegisterMgr.Status = SendDetectFingerRemoveCmd;
					FpRegisterMgr.TimeCnt = Def_WaitUserRemoveFingerTimeDelay;	
					RefreshSystemSleepTime();
					PLAY_VOICE_ONESEGMENT(VOICE_PleasePutFingerAgain);
					Hardware_DelayMs(150);
					#ifdef Function_ScreenDisplay
					DisHZ16x14Str(3,14,PutFingerStr,NormalDisplay);
					#endif
				}
				else
				{
					FpRegisterMgr.Status = fail;
					FpRegisterMgr.ErrorType = FpRegisterFail;
				}
				FpRegisterMgr.EnrollFailTimes+=1;
			}
			else
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}

	else if ( FpRegisterMgr.Status == SendSearchCmd )
	{
		FpRegisterMgr.Status = WaitForSearchCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendSearchCmd(0x01,0x0000,(DEF_MAX_FPMASTER+DEF_MAX_FPUSER));		//BuffID is 0x01,from Page 1~~100
		DEBUG_MARK;
	}

	else if ( FpRegisterMgr.Status == WaitForSearchCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE) 	//if fingerprint is registered
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FingerPrintIsRegistered;
			}
			else if ( FpmAckMgr.ErrorCode == Error_UnRegistered)	
			{
				FpRegisterMgr.Status = SendDetectFingerRemoveCmd;
				FpRegisterMgr.TimeCnt = Def_WaitUserRemoveFingerTimeDelay;	
				//GUI_Flag_RefreshLCD = bTRUE;
				RefreshSystemSleepTime();
				PLAY_VOICE_ONESEGMENT(VOICE_PleasePutFingerAgain);
				Hardware_DelayMs(150);
				#ifdef Function_ScreenDisplay
				DisHZ16x14Str(3,14,PutFingerStr,NormalDisplay);
				#endif
			}
			else
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}

	else if ( FpRegisterMgr.Status == SendDetectFingerRemoveCmd )
	{
		FpRegisterMgr.Status = WaitForDetectFingerRemoveCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendGetEnrollImageCmd();
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForDetectFingerRemoveCmdACK )
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode != Error_NoFinger)
			{
				FpRegisterMgr.Status = SendDetectFingerRemoveCmd;
				RefreshSystemSleepTime();
			}
			else
			{
				FpRegisterMgr.Status =	SendGetImageCmd;
				FpRegisterMgr.TimeCnt = Def_WaitUserPutFingerTimeDelay; 			
				//GUI_Flag_RefreshLCD = bTRUE;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}

	else if ( FpRegisterMgr.Status == SendRegModelCmd )
	{
		FpRegisterMgr.Status = WaitForRegModelCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendRegModelCmd();	
		FpRegisterMgr.TimeCnt = Def_FPMcmdTimeOutDelay; 	
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForRegModelCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			DEBUG_MARK;
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpRegisterMgr.Status = SendStoreCharCmd;
			}
			else
			{
				DEBUG_MARK;
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}
	else if ( FpRegisterMgr.Status == SendStoreCharCmd )
	{
		FpRegisterMgr.Status = WaitForStoreCharCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendStoreCharCmd(0x01,FpRegisterMgr.UserID);		//BuffID is 0x01 ,Page ID is UserID
		FpRegisterMgr.TimeCnt = Def_FPMcmdTimeOutDelay; 
		DEBUG_MARK;
	}
	else if ( FpRegisterMgr.Status == WaitForStoreCharCmdACK )
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpRegisterMgr.Status = success;
			}
			else
			{
				FpRegisterMgr.Status = fail;
				FpRegisterMgr.ErrorType = FpRegisterFail;
			}
			DEBUG_MARK;
		}
		else if (--FpRegisterMgr.TimeCnt < 1 )
		{
			FpRegisterMgr.Status = fail;
			FpRegisterMgr.ErrorType = TimeOut;
		}
	}
}

void FpUserIdentify(void)//指纹用户确认
{
	if (FpIdentifyMgr.Status == FPMcmdStart )
	{
		FpIdentifyMgr.Status = SendGetImageCmd;
	}
	else if (FpIdentifyMgr.Status == SendGetImageCmd)
	{
		FpIdentifyMgr.Status = WaitForGetImageCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendGetImageCmd();
		FpIdentifyMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpIdentifyMgr.Status == WaitForGetImageCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				DEBUG_MARK;
				#ifdef Function_FPMserialNumberCheck
				FpIdentifyMgr.Status = SendGetSerialNumberCmd;
				#else
				FpIdentifyMgr.Status = SendGenCharCmd;
				//GUI_Flag_RefreshLCD = bTRUE;
				#endif
				RefreshSystemSleepTime();	//if finger detected system sleep timer reset
			}
			else
			{
				if (FpmAckMgr.ErrorCode == Error_NoFinger)
				{
					FpIdentifyMgr.Status = FPMcmdStart;
				}
				else
				{
					FpIdentifyMgr.Status = FPMcmdStart;
					DEBUG_MARK;
				}
			}
		}
		else 
		{
			if (--FpIdentifyMgr.TimeCnt < 1)
			{
				FpIdentifyMgr.Status = FPMcmdStart;
																			DEBUG_MARK;
			}
		}
	}
	else if ( FpIdentifyMgr.Status == SendGetSerialNumberCmd )
	{
		FpIdentifyMgr.Status = WaitForGetSerialNumberCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendGetChipSerialNumberCmd();
		FpIdentifyMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpIdentifyMgr.Status == WaitForGetSerialNumberCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				if ( CheckFPMserialNumberIsMatch(&FpmAckMgr.Buff[10]) == bTRUE )
				{
					FpIdentifyMgr.Status = SendGenCharCmd;
					//GUI_Flag_RefreshLCD = bTRUE;
					FpIdentifyMgr.TimeCnt = Def_IdendtifySuccessScreenTimeDelay;	
				}
				else
				{
					FpIdentifyMgr.Status = fail;
					FpIdentifyMgr.ErrorType = Error_SerialNumberMismatched;
					FpIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
					DEBUG_MARK;
				}
			}
			else
			{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_TimeOut;
				FpIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
				DEBUG_MARK;
			}
		}
		else 
		{
			if (--FpIdentifyMgr.TimeCnt < 1 )
			{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_TimeOut;
				FpIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
				DEBUG_MARK;
			}
		}
	}
	else if ( FpIdentifyMgr.Status == SendGenCharCmd )
	{
		FpIdentifyMgr.Status = WaitForGenCharCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendGenCharCmd(0x01);		//BuffID is 0x01
		FpIdentifyMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpIdentifyMgr.Status == WaitForGenCharCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpIdentifyMgr.Status = SendSearchCmd;
				//GUI_Flag_RefreshLCD = bTRUE;
			}
			else
			{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_GenChar;
				FpIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
				DEBUG_MARK;
			}
			DEBUG_MARK;
		}
		else 
		{
			if (--FpIdentifyMgr.TimeCnt < 1)
			{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_TimeOut;
				FpIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
				DEBUG_MARK;
			}
		}
	}
	else if ( FpIdentifyMgr.Status == SendSearchCmd )
	{
		FpIdentifyMgr.Status = WaitForSearchCmdACK;
		FpmAckMgr.Status = WaitACK;
		FPM_SendSearchCmd(0x01,0x0000,(DEF_MAX_FPMASTER+DEF_MAX_FPUSER));		//BuffID is 0x01,from Page 1~~100
		FpIdentifyMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
		DEBUG_MARK;
	}
	else if ( FpIdentifyMgr.Status == WaitForSearchCmdACK)
	{
		if (FpmAckMgr.Status == GotACK)
		{
			if ( FpmAckMgr.ErrorCode == Error_NONE)
			{
				FpIdentifyMgr.Status = success;
				FpIdentifyMgr.UserID = FpmAckMgr.Para1;
				//GUI_Flag_RefreshLCD = bTRUE;
				FpIdentifyMgr.TimeCnt = Def_IdendtifySuccessScreenTimeDelay;	
			}
			else if (FpmAckMgr.ErrorCode == Error_FingerNotRelease )
			{
				FpIdentifyMgr.Status = FPMcmdStart;
			}
			else{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_NotMatch;
				FpIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
			}
			DEBUG_MARK;
		}
		else 
		{
			if (--FpIdentifyMgr.TimeCnt < 1)
			{
				FpIdentifyMgr.Status = fail;
				FpIdentifyMgr.ErrorType = Error_TimeOut;
				FpIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
			}
		}
	}
}

bool_t CheckFPMserialNumberIsMatch(uint8_t buff[])//检查FPM序列号是否匹配
{
	//uint8_t SavedFPMserialNumber[32];

	EEPROM_ReadSequential(FPMserialNumberStartAddr,FPMserialNumberManager.SN,32);
	if( CompareTwoArrayIsSame(buff,FPMserialNumberManager.SN,32) == bTRUE )	
	{
		FPMserialNumberManager.SNmatched = IsMatched;
		DEBUG_MARK;
		return bTRUE;
	}
	else{
		FPMserialNumberManager.SNmatched = IsNotMatched;
		DEBUG_MARK;
		return bFALSE;
	}
	
}

uint8_t Get_Availabe_FPmasterID(void)//获取有效指纹管理员ID
{
	uint8_t i;
	for (i=0;i<DEF_MAX_FPMASTER;i++)
	{
		if ( FpUserMemoryMgr[i].RegisterStatus == UnRegistered ){
			return (i+1);
		}
	}
	return 0;
}

uint8_t Get_Availabe_FPuserID(void)//获取有效指纹用户ID
{
	uint8_t i;
	for (i=(DEF_MAX_FPMASTER);i<(DEF_MAX_FPUSER+DEF_MAX_FPMASTER);i++)
	{
		if ( FpUserMemoryMgr[i].RegisterStatus == UnRegistered ){
			return (i+1);
		}
	}
	return 0;
}

void DeleteAllFpFromFPM(void)//从FPM中删除所有指纹
{
	//uint8_t i;
	
	if ( AllUserFpDeleteMgr.Status == StartAllUserFpDelete )
		{
			AllUserFpDeleteMgr.Status = SendDeleteAllUserFpCmdToFPM;
		}
    else if ( AllUserFpDeleteMgr.Status == SendDeleteAllUserFpCmdToFPM )
		{
			FPM_DeleteAllCharCmd();
			AllUserFpDeleteMgr.Status = WaitForDeleteAllUserFpCmdACKfromFPM;
			AllUserFpDeleteMgr.TimeCnt = Def_FPMcmdTimeOutDelay;
			FpmAckMgr.Status = WaitACK;
		}
	else if ( AllUserFpDeleteMgr.Status == WaitForDeleteAllUserFpCmdACKfromFPM )
	  {
		  if (FpmAckMgr.Status == GotACK)
			{
				if ( FpmAckMgr.ErrorCode == Error_NONE)
				{
					AllUserFpDeleteMgr.Status = DeleteAllFpUserSuccess;
				}
				else
				{
					AllUserFpDeleteMgr.Status = DeleteAllFpUserFail;
				}
				DEBUG_MARK;
			}
		else if (--AllUserFpDeleteMgr.TimeCnt < 1 )
			{
				AllUserFpDeleteMgr.Status = DeleteAllFpUserFail;
				FpmAckMgr.ErrorCode = Error_TimeOut;
			}
	  }	
}

status_t Get_FPM_SerialNumber(uint8_t buff[])//获取_FPM_序列号
{
	uint8_t i,j,k;
	uint8_t z=0;
	
	Hardware_DelayMs(100);
	
	for (i=0;i<5;i++)
	{
		FpmAckMgr.Status = WaitACK;
		FPM_SendGetChipSerialNumberCmd();
		for (j=0;j<20;j++)
		{
			CLRWDT();
			Hardware_DelayMs(10);
			FPMcmd_Excute();
			if ( FpmAckMgr.Status == GotACK )
			{
				if ( FpmAckMgr.ErrorCode == Error_NONE)
				{
					for (k=0;k<32;k++)
					{
						buff[k]=FpmAckMgr.Buff[10+k];
					}
					DEBUG_MARK;
					return S_SUCCESS;
				}
				else		//GET ACK with Error,try again
				{
					break; 
				}
			}
		}
	}
	return S_FAIL;
}

status_t SaveFPMserialNumberToMemory(void)//将FPM序列号保存到内存
{
	if ( Get_FPM_SerialNumber(FPMserialNumberManager.SN) == S_SUCCESS )
	{ 
		EEPROM_WriteSequential(FPMserialNumberStartAddr,FPMserialNumberManager.SN,32);
		return S_SUCCESS;
	}
	else{
		return S_FAIL;
	}
}

void CheckIfFpUserIsRegistered(uint16_t USERID)//检查如果指纹用户是已注册
{
	uint8_t ByteOffset,BitOffset;
	
	if ( CheckIfFpUserIDisRegistered.Status == StartCheckIfFpUserIDisRegistered )
		{
			FPM_SendReadIndexTableCmd();
			CheckIfFpUserIDisRegistered.TimeCnt = Def_FPMcmdTimeOutDelay;
			CheckIfFpUserIDisRegistered.Status = WaitForCheckIfFpUserIDisRegisteredCmdAck;
			FpmAckMgr.Status = WaitACK;
		}
	else if ( CheckIfFpUserIDisRegistered.Status == WaitForCheckIfFpUserIDisRegisteredCmdAck)
		{
			if (FpmAckMgr.Status == GotACK)
			{
				if ( FpmAckMgr.ErrorCode == Error_NONE)
				{
					ByteOffset = USERID/8;
					BitOffset = USERID%8;
					CheckIfFpUserIDisRegistered.Status = CheckIfFpUserIDisRegisteredSuccess;
					if ( ((FpmAckMgr.Buff[10+ByteOffset]>>=BitOffset)&0x01) != 0x00 )
					{
						CheckIfFpUserIDisRegistered.UserIDisRegistered = bTRUE;
					}
					else{
						CheckIfFpUserIDisRegistered.UserIDisRegistered = bFALSE;
					}
					
				}
				DEBUG_MARK;
			}
			if ( CheckIfFpUserIDisRegistered.TimeCnt-- < 1 )
			{
				CheckIfFpUserIDisRegistered.Status = CheckIfFpUserIDisRegisteredFail;
			}
		}
	
}

void CheckHowManyRegistereFPuser(void)//检查有多少注册指纹用户
{
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

#ifdef Function_FPMbreathingLed
void SetFPMbreathingLed(uint8_t color)//设置指纹头呼吸灯
{
	uint8_t i;
	FpmAckMgr.Status = WaitACK;
	if( color == FPMbreathingLed_Bule )
	{
		FPM_SetBreathingLED(1,1,1,255);		//Blue LED Breathing
	}
	else if ( color == FPMbreathingLed_Red )
	{
		FPM_SetBreathingLED(1,4,4,255);		//Red LED Breathing
	}
	else if( color == FPMbreathingLed_Green )
	{
		FPM_SetBreathingLED(1,2,2,255);		//Green LED Breathing
	}
	else if( color == FPMbreathingLed_Off )
	{
		FPM_SetBreathingLED(4,0,0,255); 	//ALL LED OFF
	}
	#ifdef ProjectIs_BarLock_S6902
	LEDsCtrlSwitch = color;
	#endif
	for(i=0;i<25;i++)
	{
		if( i > 15 )
		{
			CLRWDT();
		}
		Hardware_DelayMs(10);
		FPM_Mgr_Task();
		if ( FpmAckMgr.Status == GotACK )
		{
			break;
		}
	}
}
#endif




