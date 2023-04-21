#ifndef GUI_H
#define GUI_H

#include "global_variable.h"

#define Def_CardDetectIntervalTime	Def_GuiTimeDelayCnt05s
#define Def_WaitUserInputPasscodeTimeDelay	Def_GuiTimeDelayCnt10s

#define DEF_FpIdentifyFailedTimesLimited	4
#define DEF_CardIdentifyFailedTimesLimited	4
#define DEF_PasscodeIdentifyFailedTimesLimited  4
#define DEF_ManagerPasscodeIdentifyFailedTimesLimited 5

#define DEF_SystemLockedTime	11250	//180s
#define DEF_AntiPryingTime		11250	//180s

extern void GUI_Task(void);
extern void GUI_Init(void);

#endif
