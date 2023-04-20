#include "KeyScan.h"
#include "SensorMethod.h"
#include "Project.h"

uint8_t TouchKeyBuff[4]={0};
	
#if (defined ProjectIs_BarLock_S6421) || (defined ProjectIs_BarLock_S6421v2)	\
 || (defined ProjectIs_BarLock_S6428) || (defined ProjectIs_BarLock_S6428v2)	\
 || (defined ProjectIs_BarLock_S6430) 
uint16_t ValidKeyValue[]={0x0400,0x8000,0x0008,0x0004,0x4000,0x0010,0x0040,0x2000,0x0020,0x0080,0x1000,0x0100};//0 1 2 3 4 5 6 7 8 9 * # 

#elif defined ProjectIs_BarLock_S6904	|| defined ProjectIs_BarLock_S6904v2
uint16_t ValidKeyValue[]={0x0008,0x0100,0x0400,0x1000,0x0080,0x0020,0x2000,0x0040,0x0010,0x4000,0x0004,0x8000,0x0002};//0 1 2 3 4 5 6 7 8 9 * # bell

#elif defined ProjectIs_BarLock_S6117 || defined ProjectIs_BarLock_S6117v2
uint16_t ValidKeyValue[]={0x0040,0x4000,0x8000,0x0004,0x2000,0x1000,0x0008,0x0100,0x0400,0x0010,0x0080,0x0020};//0 1 2 3 4 5 6 7 8 9 * #

#elif defined ProjectIs_BarLock_S1734 || defined ProjectIs_BarLock_S8106  \
	 || defined ProjectIs_BarLock_S1735 || defined ProjectIs_BarLock_S7203	\
	 || defined ProjectIs_BarLock_S1736 
uint16_t ValidKeyValue[]={0x0080,0x0020,0x0400,0x1000,0x0010,0x0100,0x2000,0x0008,0x0040,0x4000,0x0004,0x8000};//0 1 2 3 4 5 6 7 8 9 * # 

#elif defined ProjectIs_BarLock_S7702	|| defined ProjectIs_BarLock_S7705
uint16_t ValidKeyValue[]={0x0004,0x0020,0x0400,0x1000,0x0040,0x0080,0x2000,0x0010,0x0100,0x4000,0x0008,0x8000};//0 1 2 3 4 5 6 7 8 9 * #

#elif defined ProjectIs_BarLock_S0607
uint16_t ValidKeyValue[]={0x0080,0x8000,0x0400,0x0004,0x4000,0x2000,0x0008,0x1000,0x0040,0x0010,0x0100,0x0020};//0 1 2 3 4 5 6 7 8 9 * #

#elif defined ProjectIs_BarLock_S9901
uint16_t ValidKeyValue[]={0x0400,0x8000,0x0008,0x0004,0x4000,0x0020,0x0010,0x2000,0x0040,0x0080,0x1000,0x0100};

#elif defined ProjectIs_BarLock_S3008
uint16_t ValidKeyValue[]={0x1000,0x0040,0x0004,0x0008,0x8000,0x0020,0x0010,0x4000,0x0080,0x0100,0x2000,0x0400};//0 1 2 3 4 5 6 7 8 9 * #

#endif

uint32_t KeyScanDrive()
{
	uint32_t TK_Value;
	//uint32_t TK_ValueBuff;
	uint8_t i;
	
	if( SOCAPI_TouchKeyStatus & 0x40 )	// bit6-半轮扫描完成标志  1:完成 	0：未完成
	{
		SOCAPI_TouchKeyStatus &= 0xBF;	//清半轮扫描标志位
		TouchKeyRestart();
	}
	else
	{
		if( SOCAPI_TouchKeyStatus & 0x80 )	//bit7-一轮扫描完成标志  1:完成 	0：未完成
		{
			TK_Value = TouchKeyScan();
			//TK_ValueBuff = TK_Value;
			SOCAPI_TouchKeyStatus &= 0x7F;		//清一轮扫描标志位
			for(i=0; i<4; i++)
			{
				TouchKeyBuff[i] = TK_Value >> (24-(8*i));
			}
			TouchKeyRestart();
		}
	}
	return TK_Value;
}

void KeyPressedJudge(uint32_t InputKeyValue)
{
	uint16_t InputValidKeyValue;
	uint8_t i;
	uint8_t TK_Flag;
	
	InputValidKeyValue = InputKeyValue;
	

	
	for(i=0;i<13;i++)//最多有可能有13个键
	{
		TK_Flag = 0;
		if( (InputValidKeyValue & ValidKeyValue[i]) > 0 )
		{
			TK_Flag = 1;
		}
		switch(i)
		{
			case 0:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_ZERO_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_ZERO_Pressed = bFALSE;
				}
				break;
					
			case 1:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_ONE_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_ONE_Pressed = bFALSE;
				}
				break;
					
			case 2:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_TWO_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_TWO_Pressed = bFALSE;
				}
				break;
					
			case 3:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_THREE_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_THREE_Pressed = bFALSE;
				}
				break;
					
			case 4:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_FOUR_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_FOUR_Pressed = bFALSE;
				}
				break;
					
			case 5:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_FIVE_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_FIVE_Pressed = bFALSE;
				}
				break;
					
			case 6:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_SIX_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_SIX_Pressed = bFALSE;
				}
				break;
					
			case 7:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_SEVEN_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_SEVEN_Pressed = bFALSE;
				}
				break;
					
			case 8:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_EIGHT_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_EIGHT_Pressed = bFALSE;
				}
				break;
					
			case 9:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_NINE_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_NINE_Pressed = bFALSE;
				}
				break;
					
			case 10:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_ASTERISK_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_ASTERISK_Pressed = bFALSE;
				}
				break;
					
			case 11:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_POUNDSIGN_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_POUNDSIGN_Pressed = bFALSE;
				}
				break;
				
			case 12:
				if( TK_Flag )
				{
					TouchKeyStatus.KEY_DOORBELL_Pressed = bTRUE;
				}
				else
				{
					TouchKeyStatus.KEY_DOORBELL_Pressed = bFALSE;
				}
				break;

			default:
				break;
		}
	}
}

void Touch_Init(void)
{
	TouchKeyInit();
}
