#include "MFC.h"
#include "GUI_Function.h"
#include "Project.h"
#include "LCD.h"
#include "Font_Menu.h"
#include "Log.h"
#include "global_variable.h"
#include "Basic_Function.h"

#include "EEPROM.h"
#include "BeepMgr.h"
#include "MFC_MF17622.h"
#include "yc_nfc_contactless_l1.h"

extern DataInputMgr_t DataInputMgr;

#define Def_UserSwingCardTimeDelay	Def_GuiTimeDelayCnt5s

void ReadCardUserMemoryFromEEPROM(void)//从EEPROM读取卡片用户
{
	EEPROM_ReadSequential(CardUserMemoryStartAddr,&CardMemoryMgr[0].UserID,(6*DEF_MAX_CARDUSER));
}

void WriteCardUserMemoryToEEPROM(void)//向EEPROM写入卡片用户
{
	EEPROM_WriteSequential(CardUserMemoryStartAddr,&CardMemoryMgr[0].UserID,(6*DEF_MAX_CARDUSER));
}

void CardUserIdentify(void)//卡片用户认证
{
	status_t GetCardID;
	uint8_t UserID;
	if (CardIdentifyMgr.Status == ReadingCardID)
	{
		GetCardID = MFC_Auto_Reader(CardIdentifyMgr.CID);
		if ( GetCardID == S_SUCCESS )
		{
			CardIdentifyMgr.Status = CheckCardIDifRegistered;
			RefreshSystemSleepTime();
		}
	}
	else if ( CardIdentifyMgr.Status == CheckCardIDifRegistered)
	{
		UserID = CompareCardIDwithMemory(CardIdentifyMgr.CID); 
		if ( UserID == 0x00 )		//Card ID is not registered
		{
			CardIdentifyMgr.Status = Fail;
			CardIdentifyMgr.TimeCnt = Def_IdendtifyFailScreenTimeDelay;
		}
		else
		{
			CardIdentifyMgr.UserID = UserID;
			CardIdentifyMgr.Status = Success;	
			CardIdentifyMgr.TimeCnt = Def_IdendtifySuccessScreenTimeDelay;
		}
	}
}

uint8_t CompareCardIDwithMemory(uint8_t *Point)//将新卡片ID与已有ID进行比较
{
	uint8_t i;
	uint8_t CardUserID;

	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{
		if ( CardMemoryMgr[i].Status == CIDisValid )
		{
			if (  (CardMemoryMgr[i].CID[0] == *Point)
				&&(CardMemoryMgr[i].CID[1] == *(Point+1))
				&&(CardMemoryMgr[i].CID[2] == *(Point+2))
				&&(CardMemoryMgr[i].CID[3] == *(Point+3))
				//&&(CardMemoryMgr[i].CID[4] == *(Point+4))
				 )
			{
				CardUserID = CardMemoryMgr[i].UserID;
				return CardUserID;
			}
		}
	}
	return 0;
}

uint8_t Get_Availabe_CardUserID(void)//获取已注册的卡片ID
{
	uint8_t i;
	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{
		if ( CardMemoryMgr[i].Status != CIDisValid ){
			return (i+1);
		}
	}
	return 0;
}

status_t SaveCardUserToMemory(uint8_t *Point,uint8_t UserID)//保存卡片用户到EEPROM
{
	uint8_t j;

	for (j=0;j<4;j++)
	{
		CardMemoryMgr[UserID-1].CID[j] = *(Point+j);
	}
	CardMemoryMgr[UserID-1].UserID = UserID;
	CardMemoryMgr[UserID-1].Status = CIDisValid;
	
	WriteCardUserMemoryToEEPROM();
	return S_SUCCESS;

}

void DeleteCardUserfromMemory(uint8_t UserID)//从EEPROM中删除卡片用户
{
	uint8_t j;

	CardMemoryMgr[UserID-1].Status = CIDisInvalid;
	CardMemoryMgr[UserID-1].UserID = 0xFF;
	for (j=0;j<4;j++)
	{
		CardMemoryMgr[UserID-1].CID[j] = 0xFF;
	}
	WriteCardUserMemoryToEEPROM();
}

void DeleteAllCardUserfromMemory(void)//从EEPROM中删除所有卡片用户
{
	uint8_t i,j;

	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{

		CardMemoryMgr[i].Status = CIDisInvalid;
		CardMemoryMgr[i].UserID = 0xFF;
		for (j=0;j<4;j++)
		{
			CardMemoryMgr[i].CID[j] = 0xFF;
		}

	}	
	WriteCardUserMemoryToEEPROM();
}

bool_t IfCardUserIDisRegistered(uint8_t CardUserID)//判断卡片用户ID是否注册
{
	uint8_t i;

	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{
		if ( CardMemoryMgr[i].Status == CIDisValid )
			{
				if (  CardMemoryMgr[i].UserID == CardUserID)
				{
					return bTRUE;
				}
			}
	}
	return bFALSE;
}

uint8_t CheckHowManyRegisteredCardUser( void )//检查已注册卡片用户数量
{
	uint8_t i,UserNum;
	UserNum =0;
	for (i=0;i<DEF_MAX_CARDUSER;i++)
	{
		if ( CardMemoryMgr[i].Status == CIDisValid )
		{
			UserNum++;
		}
	}
	return UserNum;
}

bool_t IfSystemIsNoCardUser(void)
{
	if ( CheckHowManyRegisteredCardUser() == 0 )
	{
		return bTRUE;
	}
	else
	{
		return bFALSE;
	}
}

void ShowRegisterCardUser(void)//注册磁卡用户
{
	status_t GetCardID;

	if ( CardUserRegisterMgr.Status == StartCardUserRegister )
	{
		if ((CheckMemoryMgr.FpMasterNum == 0x00 )
			&&(CheckMemoryMgr.PasscodeMasterNum == 0x00)
		)
		{
			CardUserRegisterMgr.Status = Fail;
			CardUserRegisterMgr.ErrorType = SystemNoMaster;
			CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
			PLAY_VOICE_ONESEGMENT(VOICE_PleaseAddMasterFirst);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,14,AddMasterStr,NormalDisplay);
			#endif
		}
		else
		{
			if ( CheckMemoryMgr.CardUserNum < DEF_MAX_CARDUSER )
			{
				CardUserRegisterMgr.Status = InputCardUserID;
				DataInputMgr.Status = InputIdle;
				CardUserRegisterMgr.TimeCnt = 0;
				#ifdef Function_Enable_ID_Del
				CardUserRegisterMgr.UserID = Get_Availabe_CardUserID();
				ReportRegisterID(CARDUSER,CardUserRegisterMgr.UserID);
				CardUserRegisterMgr.TimeCnt = Def_GuiTimeDelayCnt4s;
				#endif
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,0,InputUserIDStr,NormalDisplay);
				#endif
			}
			else
			{
				CardUserRegisterMgr.Status = Fail;
				CardUserRegisterMgr.ErrorType = MemoryIsFull;
				CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;
				PLAY_VOICE_ONESEGMENT(VOICE_UsersAreFull);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,UsersFullStr,NormalDisplay);
				#endif
			}
		}
		
	}
	if ( CardUserRegisterMgr.Status == InputCardUserID )
	{
		if ( --CardUserRegisterMgr.TimeCnt < 1 )
		{
			CardUserRegisterMgr.UserID = Get_Availabe_CardUserID();
			CardUserRegisterMgr.Status = ReadingCardID;	
			CardUserRegisterMgr.TimeCnt = Def_UserSwingCardTimeDelay; 	//set to 5s
			PLAY_VOICE_ONESEGMENT(VOICE_PleaseSwingCard);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,40,PleaseSwingCardStr,NormalDisplay);
			#endif
		}
	}
	else if ( CardUserRegisterMgr.Status == ReadingCardID)
	{
		GetCardID = MFC_Auto_Reader(CardUserRegisterMgr.CID);
		if ( GetCardID == S_SUCCESS )
		{
			CardUserRegisterMgr.Status = SavedCardID;
		}
		if ( --CardUserRegisterMgr.TimeCnt < 1 )
		{
			CardUserRegisterMgr.Status = Fail;
			CardUserRegisterMgr.ErrorType = TimeOut;
			CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
			//GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Fail);
			#ifdef Function_ScreenDisplay
			Clear_Screen();
			DisHZ16x14Str(3,36,OperationFailStr,NormalDisplay);
			#endif
		}
	}
	else if ( CardUserRegisterMgr.Status ==  SavedCardID )
	{
		if (CompareCardIDwithMemory(CardUserRegisterMgr.CID) == 0x00 )		//card CID is not be used
		{
			if ( SaveCardUserToMemory(CardUserRegisterMgr.CID,CardUserRegisterMgr.UserID) == S_SUCCESS )
			{
				CardUserRegisterMgr.Status = Success;
				CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				//GUI_Flag_RefreshLCD = bTRUE;
				PLAY_VOICE_TWOSEGMENT(VOICE_Operation,VOICE_Success);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,OperationSuccessStr,NormalDisplay);
				#endif
				CheckMemoryMgr.CardUserNum+=1;
			}
			else
			{
				CardUserRegisterMgr.Status = Fail;
				CardUserRegisterMgr.ErrorType = MemoryIsFull;
				CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay; 	
				PLAY_VOICE_ONESEGMENT(VOICE_UsersAreFull);
				Hardware_DelayMs(100);
				#ifdef Function_ScreenDisplay
				Clear_Screen();
				DisHZ16x14Str(3,36,UsersFullStr,NormalDisplay);
				#endif
			}
		}
		else
		{
			CardUserRegisterMgr.Status = Fail;
			CardUserRegisterMgr.ErrorType = CardCIDisBeUsed;
			CardUserRegisterMgr.TimeCnt = Def_MessageBoxTimeDelay;	
			//GUI_Flag_RefreshLCD = bTRUE;
			PLAY_VOICE_TWOSEGMENT(VOICE_Duplicate,VOICE_Card);
			#ifdef Function_ScreenDisplay
			DisHZ16x14Str(3,36,CardCIDisBeUsedStr,NormalDisplay);
			#endif
		}
	}
	else if ( CardUserRegisterMgr.Status ==  Success )
		{
			if ( --CardUserRegisterMgr.TimeCnt < 1 )
			{
				//CardUserRegisterMgr.Status = StartCardUserRegister;
				GoToCardMenu();
				#ifdef Function_EventLog
				GUI_CreatAndSaveLog(AddCardUser);
				#endif
			}
		}
	else if ( CardUserRegisterMgr.Status == Fail )
		{
//			if (CardUserRegisterMgr.ErrorType == UserIDisRegistered)
//			{
//			
//			}
//			else if ( CardUserRegisterMgr.ErrorType == SystemNoMaster )
//			{
//				
//			}
//			else if ( CardUserRegisterMgr.ErrorType == MemoryIsFull )
//			{
//			
//			}
//			else if ( CardUserRegisterMgr.ErrorType == CardCIDisBeUsed )
//			{
//				
//			}
//			else
//			{
//				
//			}
			if ( CardUserRegisterMgr.TimeCnt-- < 1 )
			{
				GoToCardMenu();
			}
		}
}











