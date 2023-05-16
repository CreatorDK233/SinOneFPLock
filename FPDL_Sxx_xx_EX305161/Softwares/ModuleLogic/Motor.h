#ifndef _Motor_H_
#define _Motor_H_	1

#include "StdTypes.h"
#include "Project.h"

#if (defined ProjectIs_BarLock_S8705) || (defined ProjectIs_BarLock_S8706)
#define Def_MotorCWTime  9		//0.15s
#define Def_MotorCWWAITTime  Def_IdendtifySuccessScreenTimeDelay		//5s
#define Def_MotorCCWTime  9		//0.15s
#define Def_MotorCCWWAITTime  8	//0.125S
#else
#define Def_MotorCWTime  18		//0.31s
#define Def_MotorCWWAITTime  Def_IdendtifySuccessScreenTimeDelay		//5s
#define Def_MotorCCWTime  18		//0.31s
#define Def_MotorCCWWAITTime  8	//0.125S
#endif

extern uint8_t Hardware_MotorDriverTimeCompasention(void);
extern void GUI_SetMotorToOpenAndCloseDoor(void);
extern void GUI_MotorAgingTest(void);
extern void GUI_MotorContinueOpen(void);
extern void Hardware_MotorMgr_Init( void );
extern void Hardware_MotorMgr_Task( void );

#endif
