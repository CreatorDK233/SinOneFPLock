#ifndef KeyScan_h
#define KeyScan_h

#include "StdTypes.h"

typedef struct
{
	bool_t KEY_ZERO_Pressed;
	bool_t KEY_ONE_Pressed;
	bool_t KEY_TWO_Pressed;
	bool_t KEY_THREE_Pressed;
	bool_t KEY_FOUR_Pressed;
	bool_t KEY_FIVE_Pressed;
	bool_t KEY_SIX_Pressed;
	bool_t KEY_SEVEN_Pressed;
	bool_t KEY_EIGHT_Pressed;
	bool_t KEY_NINE_Pressed;
	bool_t KEY_POUNDSIGN_Pressed;
	bool_t KEY_ASTERISK_Pressed;
	bool_t KEY_INSIDEBUTTON_Pressed;
	bool_t KEY_SLIDETOUCH_Pressed;
	bool_t KEY_DOORBELLTOUCH_Pressed;
	bool_t KEY_DOORCLOSE_Pressed;
	bool_t KEY_DOORBELL_Pressed;
}TouchKeyStatus_t;

extern TouchKeyStatus_t TouchKeyStatus;

extern keycode_t Key_Scan(void);
extern void Key_Init(void);

#endif
