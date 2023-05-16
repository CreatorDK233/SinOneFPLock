#ifndef GUI_H
#define GUI_H

#include "global_variable.h"

#define Def_CardDetectIntervalTime	Def_GuiTimeDelayCnt05s
#define Def_WaitUserInputPasscodeTimeDelay	Def_GuiTimeDelayCnt10s

#define DEF_FpIdentifyFailedTimesLimited	4
#define DEF_CardIdentifyFailedTimesLimited	4
#define DEF_PasscodeIdentifyFailedTimesLimited  4
#define DEF_ManagerPasscodeIdentifyFailedTimesLimited 5

#ifdef Function_NFCUsed_YC5018
#define DEF_SystemLockedTime	11250	//180s
#define DEF_AntiPryingTime		10660	//180s
#else
#define DEF_SystemLockedTime	11250	//180s
#define DEF_AntiPryingTime		11250	//180s
#endif

extern void GUI_Task(void);
extern void GUI_Init(void);

#endif
