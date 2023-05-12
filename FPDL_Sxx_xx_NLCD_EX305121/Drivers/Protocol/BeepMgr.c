#include "IO.h"
#include "global_variable.h"
#include "BeepMgr.h"

void PLAY_VOICE_ONESEGMENT(uint8_t segment)
{
	uint8_t Buff[2];

	Buff[0]= segment;
	Buff[1]= DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(Buff);
}

void PLAY_VOICE_TWOSEGMENT(uint8_t segment1,uint8_t segment2)
{
	uint8_t Buff[3];

	Buff[0]= segment1;
	Buff[1]= segment2;
	Buff[2]= DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(Buff);

}

void PLAY_VOICE_THREESEGMENT(uint8_t segment1,uint8_t segment2,uint8_t segment3)
{
	uint8_t Buff[4];

	Buff[0]= segment1;
	Buff[1]= segment2;
	Buff[2]= segment3;
	Buff[3]= DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS(Buff);
}

void PLAY_VOICE_MULTISEGMENTS(uint8_t BUFF[])
{
	uint8_t SegmentCnt;
	
	if (  ( VoiceMgr.Enable == bFALSE)&&
			(	( CurrentScreen == SCREEN_Main)
				||( CurrentScreen == SCREEN_IdentifySuccess)
				||( CurrentScreen == SCREEN_IdentifyFail)
			)
		)
	{
		return;
	}
	VoiceDataTransferMgr.DataBuff[0] = 0xfe;
	for ( SegmentCnt =0;SegmentCnt<49;SegmentCnt++)
	{
		if ( BUFF[SegmentCnt] == DEF_VoiceSegmentEndFlag ){break;}
		VoiceDataTransferMgr.DataBuff[2*SegmentCnt+1] = 0xF3; //Continue play
		VoiceDataTransferMgr.DataBuff[2*SegmentCnt+2] = BUFF[SegmentCnt]+SystemLanguage; //Continue play
		//VoiceDataTransferMgr.DataBuff[2*SegmentCnt+2] = BUFF[SegmentCnt]; //Continue play
	}
	VoiceDataTransferMgr.BitPoint = 0x00;
	VoiceDataTransferMgr.SendDataNum = 0;
	VoiceDataTransferMgr.TotalDataNum = 2*SegmentCnt+1;
	VoiceDataTransferMgr.Status = SendingDataStart;
	VoiceDataTransferMgr.VoicePlayEndCnt=0;
	VoiceDataTransferMgr.VoicePlayEnd = bFALSE;
}

//使用FIXED后缀时，无需切换中英文
void PLAY_VOICE_ONESEGMENT_FIXED(uint8_t segment)
{
	if (  ( VoiceMgr.Enable == bFALSE)&&
			(	( CurrentScreen == SCREEN_Main )
				||( CurrentScreen == SCREEN_IdentifySuccess)
				||( CurrentScreen == SCREEN_IdentifyFail)
			)
		)
	{
		return;
	}
	VoiceDataTransferMgr.DataBuff[0]= segment;
	VoiceDataTransferMgr.BitPoint = 0x00;
	VoiceDataTransferMgr.SendDataNum = 0;
	VoiceDataTransferMgr.TotalDataNum = 1;
	VoiceDataTransferMgr.Status = SendingDataStart;
	VoiceDataTransferMgr.VoicePlayEndCnt=0;
	VoiceDataTransferMgr.VoicePlayEnd = bFALSE;
}

void PLAY_VOICE_TWOSEGMENT_FIXED(uint8_t segment1,uint8_t segment2)
{
	uint8_t Buff[4];

	Buff[0]= segment1;
	Buff[1]= segment2;
	Buff[2]= DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS_FIXED(Buff);
}

void PLAY_VOICE_THREESEGMENT_FIXED(uint8_t segment1,uint8_t segment2,uint8_t segment3)
{
	uint8_t Buff[4];

	Buff[0]= segment1;
	Buff[1]= segment2;
	Buff[2]= segment3;
	Buff[3]= DEF_VoiceSegmentEndFlag;
	PLAY_VOICE_MULTISEGMENTS_FIXED(Buff);
}

void PLAY_VOICE_MULTISEGMENTS_FIXED(uint8_t BUFF[])
{
	uint8_t SegmentCnt;
	
	if (  ( VoiceMgr.Enable == bFALSE)&&
			(	( CurrentScreen == SCREEN_Main)
				||( CurrentScreen == SCREEN_IdentifySuccess)
				||( CurrentScreen == SCREEN_IdentifyFail)
			)
		)
	{
		return;
	}
	VoiceDataTransferMgr.DataBuff[0] = 0xfe;
	for ( SegmentCnt =0;SegmentCnt<49;SegmentCnt++)
	{
		if ( BUFF[SegmentCnt] == DEF_VoiceSegmentEndFlag ){break;}
		VoiceDataTransferMgr.DataBuff[2*SegmentCnt+1] = 0xF3; //Continue play
		VoiceDataTransferMgr.DataBuff[2*SegmentCnt+2] = BUFF[SegmentCnt]; //Continue play
	}
	VoiceDataTransferMgr.BitPoint = 0x00;
	VoiceDataTransferMgr.SendDataNum = 0;
	VoiceDataTransferMgr.TotalDataNum = 2*SegmentCnt+1;
	VoiceDataTransferMgr.Status = SendingDataStart;
	VoiceDataTransferMgr.VoicePlayEndCnt=0;
	VoiceDataTransferMgr.VoicePlayEnd = bFALSE;
}

void SET_VOLUME(uint8_t volume)
{
	PLAY_VOICE_ONESEGMENT_FIXED(0xE0+volume);
}

void STOP_VOICEPLAY(void)
{
	PLAY_VOICE_ONESEGMENT_FIXED(0xFE);
}

void VoicePlayerPowerDown(void)
{
	SET_VOICEDATA_SLEEP;
	SET_VOICEBUSY_SLEEP;
}

void PLAY_VOICE_DOORBELL(void)
{
	if ( VoiceDataTransferMgr.Status != VoiceIdle )
	{
		return;
	}
	VoiceDataTransferMgr.DataBuff[0] = 0xfe;
	VoiceDataTransferMgr.DataBuff[1]= 0xF3;
	VoiceDataTransferMgr.DataBuff[2]= VOICE_Doorbell;
	VoiceDataTransferMgr.DataBuff[3]= 0xF3;
	VoiceDataTransferMgr.DataBuff[4]= VOICE_Doorbell;
	VoiceDataTransferMgr.BitPoint = 0x00;
	VoiceDataTransferMgr.SendDataNum = 0;
	VoiceDataTransferMgr.TotalDataNum = 5;
	VoiceDataTransferMgr.Status = SendingDataStart;
	VoiceDataTransferMgr.VoicePlayEndCnt=0;
	VoiceDataTransferMgr.VoicePlayEnd = bFALSE;
}

/*void PLAY_VOICE2_ONESEGMENT(uint8_t segment)
{
	VoiceDataTransferMgr2.BitPoint = 0x00;
	VoiceDataTransferMgr2.DataBuff[0] = segment;
	VoiceDataTransferMgr2.SendDataNum = 1;
	VoiceDataTransferMgr2.Status = SendingDataStart;
}

void PLAY_VOICE2_TWOSEGMENT(uint8_t segment1,uint8_t segment2)
{
	if ( VoiceMgr.Enable == bTRUE)
	{
		VoiceDataTransferMgr2.BitPoint = 0x00;
		VoiceDataTransferMgr2.DataBuff[3] = 0xF3;
		VoiceDataTransferMgr2.DataBuff[2] = segment1;
		VoiceDataTransferMgr2.DataBuff[1] = 0xF3;
		VoiceDataTransferMgr2.DataBuff[0] = segment2;
		VoiceDataTransferMgr2.SendDataNum = 4;
		VoiceDataTransferMgr2.Status = SendingDataStart;
	}
}

void PLAY_VOICE2_DOORBELL(void)
{
	VoiceDataTransferMgr2.BitPoint = 0x00;
	VoiceDataTransferMgr2.DataBuff[3] = 0xF3;
	VoiceDataTransferMgr2.DataBuff[2] = VOICE_Doorbell;
	VoiceDataTransferMgr2.DataBuff[1] = 0xF3;
	VoiceDataTransferMgr2.DataBuff[0] = VOICE_Doorbell;
	VoiceDataTransferMgr2.SendDataNum = 4;
	VoiceDataTransferMgr2.Status = SendingDataStart;
}

void PLAY_VOICE2_CONTINUE(uint8_t segment)
{
	VoiceDataTransferMgr2.BitPoint = 0x00;
	VoiceDataTransferMgr2.DataBuff[1] = segment;
	VoiceDataTransferMgr2.DataBuff[0] = 0xF2;
	VoiceDataTransferMgr2.SendDataNum = 2;
	VoiceDataTransferMgr2.Status = SendingDataStart;
}

void STOP_VOICEPLAY2(void)
{
	VoiceDataTransferMgr2.BitPoint = 0x00;
	VoiceDataTransferMgr2.DataBuff[0] = 0xFE;
	VoiceDataTransferMgr2.SendDataNum = 1;
	VoiceDataTransferMgr2.Status = SendingDataStart;
}*/

void VOICE_Init(void)
{
	VoiceDataTransferMgr.PulseWidthCnt = 0;
	VoiceDataTransferMgr.Status = VoiceIdle;
	//VoiceDataTransferMgr2.PulseWidthCnt = 0;	
	//VoiceDataTransferMgr2.Status = VoiceIdle; 
}

void BeepMgrTask(void)
{
	if ( VoiceDataTransferMgr.PulseWidthCnt > 0 )
		{
			VoiceDataTransferMgr.PulseWidthCnt--;
		}
		
		if ( VoiceDataTransferMgr.PulseWidthCnt == 0 )
		{
			switch ( VoiceDataTransferMgr.Status )
			{
				default:
					
				case VoiceIdle:
					SET_VOICEDATA_H;

					if ( STATUS_PINMACRO_VOICEBUSY != 0 )
					{ 
						if ( VoiceDataTransferMgr.VoicePlayEndCnt < 1500)
						{
							VoiceDataTransferMgr.VoicePlayEndCnt++;
						}
						else{
							VoiceDataTransferMgr.VoicePlayEnd = bTRUE;
						}
						DEBUG_MARK;
					}
					else
					{
						VoiceDataTransferMgr.VoicePlayEndCnt=0;
						VoiceDataTransferMgr.VoicePlayEnd = bFALSE;
						DEBUG_MARK;
					}
					break;	
						
				case SendingDataStart:
//	
//					if ( STATUS_PINMACRO_VOICEDATA != 0 )
//					{
//						SET_VOICEDATA_L;
//						VoiceDataTransferMgr.PulseWidthCnt = 17;	//5.1ms
//					}
//					else
//					{
//						VoiceDataTransferMgr.Status =  SendingData;//SendingData;
//						VoiceDataTransferMgr.Data = VoiceDataTransferMgr.DataBuff[VoiceDataTransferMgr.SendDataNum];
//					}
						SET_VOICEDATA_L;
						VoiceDataTransferMgr.PulseWidthCnt = 27;
						VoiceDataTransferMgr.Status =  SendingData;//SendingData;
						VoiceDataTransferMgr.Data = VoiceDataTransferMgr.DataBuff[VoiceDataTransferMgr.SendDataNum];
						
					break;
					
				case SendingData:
	
						if ( STATUS_PINMACRO_VOICEDATA == 0 )	//send High level
						{
							SET_VOICEDATA_H;
							if ( (VoiceDataTransferMgr.Data&0x01) != 0x00)
							{
								VoiceDataTransferMgr.PulseWidthCnt = 3;	//900US
								DEBUG_MARK;
							}
							else
							{
								VoiceDataTransferMgr.PulseWidthCnt = 1;	//300US
								DEBUG_MARK;
							}
						}
						else		//send low level
						{
							SET_VOICEDATA_L;
							if ( (VoiceDataTransferMgr.Data&0x01) != 0x00)
							{
								VoiceDataTransferMgr.PulseWidthCnt = 1;	//300US
							}
							else
							{
								VoiceDataTransferMgr.PulseWidthCnt = 3;	//900US
							}
							VoiceDataTransferMgr.Data>>=1;
	
							if ( ++VoiceDataTransferMgr.BitPoint >=8 )
							{
								VoiceDataTransferMgr.Status = SendingDataEnd;
								DEBUG_MARK;
							}
						}
					break;
					
					case SendingDataEnd:	
						
						SET_VOICEDATA_H;
					
						VoiceDataTransferMgr.SendDataNum++;
					
						if (VoiceDataTransferMgr.SendDataNum < VoiceDataTransferMgr.TotalDataNum )
						{
							VoiceDataTransferMgr.Status = ContinueSendData;
							VoiceDataTransferMgr.PulseWidthCnt = 10; //3MS
						}
						else
						{
							VoiceDataTransferMgr.Status = VoiceIdle;
							VoiceDataTransferMgr.PulseWidthCnt = 333; //100MS
						}
						
						
						DEBUG_MARK;
					break;	
					
					case ContinueSendData:	

						//if ( STATUS_PINMACRO_VOICEBUSY != 0 )
						{
							VoiceDataTransferMgr.Status = SendingDataStart;
							VoiceDataTransferMgr.BitPoint = 0x00;
						}

						DEBUG_MARK;
					break;	
					
			}
		}
	return;

}


