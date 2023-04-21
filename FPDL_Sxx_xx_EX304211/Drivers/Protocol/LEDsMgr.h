#ifndef LEDSMGR_h
#define LEDSMGR_h

#include "StdTypes.h"

#define DEF_RGB_LED_R	0x01
#define DEF_RGB_LED_G	0x02
#define DEF_RGB_LED_B	0x03

extern uint8_t LEDsCtrlSwitch;
extern void SET_ALLKEYLED_ON(void);
extern void SET_ALLKEYLED_OFF(void);

extern void SET_KEYLED_OFF(keycode_t key);
extern void SET_KEYLED_ON(keycode_t key);
extern void Enable_KEYLED_IdentifyFail(void);
extern void KEYLED_IdentifyFail_Task(void);
extern void Enable_KEYLED_WATERLIGHT(void);
extern void KEYLED_WATERLIGHT_Task(void);
extern void KEYLED_ASTERISK_Flash(void);
extern void KEYLED_KEY5_Flash(void);



extern void SET_OneSmileLED_ON(uint8_t lednumber);
extern void SET_AllSmileLED_OFF(void);
extern void SET_AP_LED_ON(void);
extern void SET_12A_LED_ON(void);
extern void SET_123A_LED_ON(void);
extern void SET_1234A_LED_ON(void);
extern void SET_123456A_LED_ON(void);

extern void LEDsMgr_Task(void);

#endif
