#include "IO.h"
#include "KeyScan.h"
#include "global_variable.h"
#include "Key_SinOne.h"

#define Def_KeyHoldTimeLimited 96			//1.5s
#define Def_KeyHoldLongTimeLimited 	192		//3s

keycode_t NewKeycode,LastKeycode;
uint16_t KeyHoldTimeCnt=0;
uint8_t KEYSCANTIMECNT=0;
extern bit KeyJustWakeUp;

TouchKeyStatus_t TouchKeyStatus;

void Key_Init(void)
{
	TouchKeyStatus.KEY_ZERO_Pressed = bFALSE;
	TouchKeyStatus.KEY_ONE_Pressed = bFALSE;
	TouchKeyStatus.KEY_TWO_Pressed = bFALSE;
	TouchKeyStatus.KEY_THREE_Pressed = bFALSE;
	TouchKeyStatus.KEY_FOUR_Pressed = bFALSE;
	TouchKeyStatus.KEY_FIVE_Pressed = bFALSE;
	TouchKeyStatus.KEY_SIX_Pressed = bFALSE;
	TouchKeyStatus.KEY_SEVEN_Pressed = bFALSE;
	TouchKeyStatus.KEY_EIGHT_Pressed = bFALSE;
	TouchKeyStatus.KEY_NINE_Pressed = bFALSE;
	TouchKeyStatus.KEY_POUNDSIGN_Pressed = bFALSE;
	TouchKeyStatus.KEY_ASTERISK_Pressed = bFALSE;
	//TouchKeyStatus.KEY_DOORCLOSE_Pressed = bFALSE;
	TouchKeyStatus.KEY_INSIDEBUTTON_Pressed = bFALSE;
}

keycode_t Key_Scan(void)
{
	uint32_t key_value,temp;
	
	keycode_t PostKeycode;
	
	key_value = KeyScanDrive();
	
	if(key_value!=0)
	{
		temp = key_value;
	}
	
	if (SystemPowerMgr.AwakeTimerCnt < 32 )
	{
		return KEY_NONE;
	}
	
	if( KeyJustWakeUp == 1 )
	{
		if( key_value == 0 )
		{
			KeyJustWakeUp = 0;
		}
		return KEY_NONE;
	}

	KeyPressedJudge(key_value);

	if ( PINMACRO_ONBOARD_BUTTON_STATUS == 0 )
	{
		DEBUG_MARK;
		TouchKeyStatus.KEY_INSIDEBUTTON_Pressed = bTRUE;
	}
	#ifdef INSIDEBUTTONINTOMAINMENU
	else if (SystemPowerMgr.AwakeSource == SystemResetKey)		
		{
				TouchKeyStatus.KEY_INSIDEBUTTON_Pressed = bTRUE;
				SystemPowerMgr.AwakeSource = DoNotCare;	
				//CurrentScreen = SCREEN_ManagerIdentify;
				//ManagerIdentifyMgr.Status = StartManagerIdentify;
		}
	#endif
	else{
		TouchKeyStatus.KEY_INSIDEBUTTON_Pressed = bFALSE;
	}

		
	if ( (TouchKeyStatus.KEY_ZERO_Pressed == bTRUE )&&(LastKeycode == KEY_ZERO)){
		NewKeycode = KEY_ZERO;
	}
	else if ( (TouchKeyStatus.KEY_ONE_Pressed == bTRUE )&&(LastKeycode == KEY_ONE)){
		NewKeycode = KEY_ONE;
	}
	else if ( (TouchKeyStatus.KEY_TWO_Pressed == bTRUE )&&(LastKeycode == KEY_TWO)){
		NewKeycode = KEY_TWO;
	}
	else if ( (TouchKeyStatus.KEY_THREE_Pressed == bTRUE )&&(LastKeycode == KEY_THREE)){
		NewKeycode = KEY_THREE;
	}
	else if ( (TouchKeyStatus.KEY_FOUR_Pressed == bTRUE )&&(LastKeycode == KEY_FOUR)){
		NewKeycode = KEY_FOUR;
	}
	else if ( (TouchKeyStatus.KEY_FIVE_Pressed == bTRUE )&&(LastKeycode == KEY_FIVE)){
		NewKeycode = KEY_FIVE;
	}
	else if ( (TouchKeyStatus.KEY_SIX_Pressed == bTRUE )&&(LastKeycode == KEY_SIX)){
		NewKeycode = KEY_SIX;
	}
	else if ( (TouchKeyStatus.KEY_SEVEN_Pressed == bTRUE )&&(LastKeycode == KEY_SEVEN)){
		NewKeycode = KEY_SEVEN;
	}
	else if ( (TouchKeyStatus.KEY_EIGHT_Pressed == bTRUE )&&(LastKeycode == KEY_EIGHT)){
		NewKeycode = KEY_EIGHT;
	}
	else if ( (TouchKeyStatus.KEY_NINE_Pressed == bTRUE )&&(LastKeycode == KEY_NINE)){
		NewKeycode = KEY_NINE;
	}
	else if ( (TouchKeyStatus.KEY_POUNDSIGN_Pressed == bTRUE )&&(LastKeycode == KEY_POUNDSIGN)){
		NewKeycode = KEY_POUNDSIGN;
	}
	else if ( (TouchKeyStatus.KEY_ASTERISK_Pressed == bTRUE )&&(LastKeycode == KEY_ASTERISK)){
		NewKeycode = KEY_ASTERISK;
	}
	else if ( (TouchKeyStatus.KEY_INSIDEBUTTON_Pressed == bTRUE )&&(LastKeycode == KEY_INSIDEBUTTON)){
		NewKeycode = KEY_INSIDEBUTTON;
	}
//	else if ((TouchKeyStatus.KEY_DOORBELL_Pressed == bTRUE )&&(LastKeycode == KEY_DOORBELL))
//	{
//		NewKeycode = KEY_DOORBELL;
//		DEBUG_MARK;
//	}
	
	else if ( TouchKeyStatus.KEY_ZERO_Pressed == bTRUE ){
		NewKeycode = KEY_ZERO;
	}
	else if ( TouchKeyStatus.KEY_ONE_Pressed == bTRUE ){
		NewKeycode = KEY_ONE;
	}
	else if ( TouchKeyStatus.KEY_TWO_Pressed == bTRUE ){
		NewKeycode = KEY_TWO;
	}
	else if ( TouchKeyStatus.KEY_THREE_Pressed == bTRUE ){
		NewKeycode = KEY_THREE;
	}
	else if ( TouchKeyStatus.KEY_FOUR_Pressed == bTRUE ){
		NewKeycode = KEY_FOUR;
	}
	else if ( TouchKeyStatus.KEY_FIVE_Pressed == bTRUE ){
		NewKeycode = KEY_FIVE;
	}
	else if ( TouchKeyStatus.KEY_SIX_Pressed == bTRUE ){
		NewKeycode = KEY_SIX;
	}
	else if ( TouchKeyStatus.KEY_SEVEN_Pressed == bTRUE ){
		NewKeycode = KEY_SEVEN;
	}
	else if ( TouchKeyStatus.KEY_EIGHT_Pressed == bTRUE ){
		NewKeycode = KEY_EIGHT;
	}
	else if ( TouchKeyStatus.KEY_NINE_Pressed == bTRUE ){
		NewKeycode = KEY_NINE;
	}
	else if ( TouchKeyStatus.KEY_POUNDSIGN_Pressed == bTRUE ){
		NewKeycode = KEY_POUNDSIGN;
	}
	else if ( TouchKeyStatus.KEY_ASTERISK_Pressed == bTRUE ){
		NewKeycode = KEY_ASTERISK;
	}
	else if ( TouchKeyStatus.KEY_INSIDEBUTTON_Pressed == bTRUE ){
		NewKeycode = KEY_INSIDEBUTTON;
	}
//	else if (TouchKeyStatus.KEY_DOORBELL_Pressed == bTRUE )
//	{
//		NewKeycode = KEY_DOORBELL;
//	}
	
	else {
		NewKeycode = KEY_NONE;
	}	

	LEDsMgr.Keycode = NewKeycode;

	if ( NewKeycode != KEY_NONE )
	{
		if ( NewKeycode !=LastKeycode )
		{
			if (NewKeycode != KEY_INSIDEBUTTON)
			{
				PostKeycode = NewKeycode;
			}
			else{
				PostKeycode = KEY_NONE; 
				DEBUG_MARK;
			}
			LastKeycode = NewKeycode;
			KeyHoldTimeCnt=0;
		}
		else
		{
			if ( KeyHoldTimeCnt <= Def_KeyHoldLongTimeLimited )
			{
				KeyHoldTimeCnt++;
				PostKeycode = KEY_NONE;			
			}

			if ( KeyHoldTimeCnt > Def_KeyHoldTimeLimited )
			{
				if ((KeyHoldTimeCnt >= Def_KeyHoldLongTimeLimited )&&( NewKeycode == KEY_INSIDEBUTTON))
				{
					PostKeycode = KEY_INSIDEBUTTON_HOLD_LONG;				
				}
				else{
					PostKeycode = KEY_NONE;
				}
				
				if ( ++KEYSCANTIMECNT > 127 ){
					KEYSCANTIMECNT=0;
				}
			}
			else{
				PostKeycode = KEY_NONE;
			}
		}
	}
	else
	{
		if (LastKeycode == KEY_INSIDEBUTTON){
			PostKeycode = LastKeycode;
		}
		else{
			PostKeycode = KEY_NONE;	
		}
		LastKeycode = NewKeycode;
		KeyHoldTimeCnt=0;
	}

	DEBUG_MARK;

	return PostKeycode;

}


