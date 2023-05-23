#ifndef _LowPower_H_
#define _LowPower_H_

#include "StdTypes.h"

extern void MX_QuitLowPowerMode(void);
extern void AntiMisAwakeup(uint8_t KeyValue);
extern void IntoLowPowerMode_Init(void);
extern void LowPowerAwakeupDispose(void);

extern void System_PowerDown(void);
extern void System_Awake(void);

extern void LowPowerTimer_ISR(void);

#endif