#include "Project.h"
#include "global_variable.h"
#include "IO.h"
#include "LowPower.h"

#ifdef Function_HallSlideCover

bit LastHallState = 1;				//正常运行时上次读到霍尔io口的电位值
bit HallSleepFlag = 0;				//霍尔休眠标志
bit SleepingHallState = 0;		//休眠时读到霍尔io口的电位值

void HallSlideCover_Init(void)
{
	LastHallState = PINMACRO_HALLSENSOR_STATUS;
}

void HallSleep_Task(void)
{
	if( LastHallState == 0 )
	{
		if( PINMACRO_HALLSENSOR_STATUS == 1 )
		{
			HallSleepFlag = 1;
		}
	}
	if( HallSleepFlag == 1 )
	{
		//HallSleepFlag = 0;
		SystemPowerMgr.SleepDelayTimerCnt = 0x0000;
	}
	LastHallState = PINMACRO_HALLSENSOR_STATUS;
}

void HallAwakeupDispose(void)
{
	if( SleepingHallState == 1 )
	{
		if( PINMACRO_HALLSENSOR_STATUS == 0 )
		{
			MX_QuitLowPowerMode();
		}
	}
	else{
		if( PINMACRO_HALLSENSOR_STATUS == 1 )
		{
			SleepingHallState = 1;
			LastHallState = 1;
		}
	}
}

#endif