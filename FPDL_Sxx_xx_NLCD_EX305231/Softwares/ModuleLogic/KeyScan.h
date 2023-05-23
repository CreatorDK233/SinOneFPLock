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

#define NoTKChannel		0x0000	//用于初始化数组
//用于提供TK通道生效时的键值
#define KeyValueTK0 	0x0001
#define KeyValueTK1 	0x0002
#define KeyValueTK2 	0x0004
#define KeyValueTK3 	0x0008
#define KeyValueTK4 	0x0010
#define KeyValueTK5 	0x0020
#define KeyValueTK6 	0x0040
#define KeyValueTK7 	0x0080
#define KeyValueTK8 	0x0100
#define KeyValueTK9		0x0200
#define KeyValueTK10	0x0400
#define KeyValueTK11	0x0800
#define KeyValueTK12	0x1000
#define KeyValueTK13	0x2000
#define KeyValueTK14	0x4000
#define KeyValueTK15	0x8000
//#define KeyValueTK16	0x00010000
//#define KeyValueTK17	0x00020000
//#define KeyValueTK18	0x00040000

extern TouchKeyStatus_t TouchKeyStatus;

extern keycode_t Key_Scan(void);
extern void Key_Init(void);

#endif
