#include "Motor.h"
#include "IO.h"
#include "global_variable.h"
#include "Basic_Function.h"

/********************************************************************************************************
*  Function: 				                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
uint8_t Hardware_MotorDriverTimeCompasention(void)
{
	uint8_t CompasentionTime;
	if ( BatteryMgr.BatteryVoltage > 58 )	//if battery voltage more than 5.8V
	{
		CompasentionTime = 0;
	}
	else if ( BatteryMgr.BatteryVoltage < 45 ) //if battery voltage less than 4.5v
	{
		CompasentionTime = 6;		//6*16 = 96ms
	}
	else
	{
		CompasentionTime = (58-BatteryMgr.BatteryVoltage)/2;	//from 0~96ms
	}

	return CompasentionTime;
}
/********************************************************************************************************
*  Function: 	GUI_SetMotorToOpenAndCloseDoor(void)			                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  GUI控制电机开/关门
********************************************************************************************************/
void GUI_SetMotorToOpenAndCloseDoor(void)
{
	if ( (UserIdentifyResultMgr.UnlockingMode == SingalMode)||(UserIdentifyResultMgr.UnlockingMode == DoubleMode) )
	{
		if ( MotorMgr.MotorStatus == IDLE )
		{
			if ( UserIdentifyResultMgr.FlagContinuedOpenEnabled == bTRUE )
			{	
				UserIdentifyResultMgr.FlagContinuedOpenEnabled = bFALSE;
			}
				MotorMgr.CWTimer = Def_MotorCWTime+Hardware_MotorDriverTimeCompasention();
				MotorMgr.CWWAITTimer =  Def_MotorCWWAITTime;	
				MotorMgr.CCWTimer = Def_MotorCCWTime+Hardware_MotorDriverTimeCompasention();
				MotorMgr.CCWWAITTimer =  Def_MotorCCWWAITTime;	
				MotorMgr.MotorStatus = CW;
				MotorMgr.CWSoftStart = bTRUE;
				MotorMgr.CCWSoftStart = bTRUE;
		}
	}
}
/********************************************************************************************************
*  Function: 	GUI_MotorAgingTest(void)			                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  老化测试电机部分
********************************************************************************************************/
void GUI_MotorAgingTest(void)
{
	if (	( MotorMgr.MotorStatus == IDLE )
			&&(AgingTestMgr.TestTimes < 5000 )
		)
	{
		MotorMgr.MotorStatus = CW;
		MotorMgr.CWTimer = Def_MotorCWTime+Hardware_MotorDriverTimeCompasention();
		MotorMgr.CWWAITTimer =  32;	
		MotorMgr.CCWTimer = Def_MotorCCWTime+Hardware_MotorDriverTimeCompasention();
		MotorMgr.CCWWAITTimer =  32;	
		AgingTestMgr.TestTimes++;
		MotorMgr.CWSoftStart = bTRUE;
		MotorMgr.CCWSoftStart = bTRUE;
	}

	if (MotorMgr.MotorStatus == CW)
	{
		//Enable_KEYLED_WATERLIGHT(); 
	}
}
/********************************************************************************************************
*  Function: 	GUI_MotorContinueOpen(void)			                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  开常开模式电机配置
********************************************************************************************************/
void GUI_MotorContinueOpen(void)
{
	MotorMgr.CWTimer = Def_MotorCWTime+Hardware_MotorDriverTimeCompasention();
	MotorMgr.CWWAITTimer =  Def_MotorCCWTime;	
	MotorMgr.CCWTimer = 0;
	MotorMgr.CCWWAITTimer =  0;	
	MotorMgr.MotorStatus = CW;	
	MotorMgr.CWSoftStart = bTRUE;
}
/********************************************************************************************************
*  Function: 				                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无
********************************************************************************************************/
void Hardware_MotorMgr_Init( void )
{
	MotorMgr.CWTimer = 0;
	MotorMgr.CWWAITTimer = 0;
	MotorMgr.CCWTimer = Def_MotorCCWTime+Hardware_MotorDriverTimeCompasention();	
  	MotorMgr.CCWWAITTimer = Def_MotorCCWWAITTime;		// Reset motor when power on
	MotorMgr.MotorStatus = CCW;
	MotorMgr.CCWSoftStart = bTRUE;
}
/********************************************************************************************************
*  Function: 				                                                           
*  Object: 
*  输入： 无
*  输出： 无 	                                     
*  备注:  无,TEST
********************************************************************************************************/
void Hardware_MotorMgr_Task( void )
{
	uint16_t i;
	if ( MotorMgr.MotorStatus == CW )
	{
		if ( MotorMgr.CWTimer > 0 )
		{
			MotorMgr.CWTimer--;
		}
		else
		{
			if ( MotorMgr.CWWAITTimer > 0 )
			{
				MotorMgr.MotorStatus = CWWAIT;
			}
			else
			{
				MotorMgr.MotorStatus = IDLE;
			}
		}
	}
	else if ( MotorMgr.MotorStatus == CWWAIT )
	{
		if ( MotorMgr.CWWAITTimer > 0 )
		{
			MotorMgr.CWWAITTimer--; 
		}
		else 
		{
			if ( MotorMgr.CCWTimer > 0 ){
				MotorMgr.MotorStatus = CCW;
			}
			else
			{
				MotorMgr.MotorStatus = IDLE;
			}
		}
	}	
	else if ( MotorMgr.MotorStatus == CCW )
	{
		if ( MotorMgr.CCWTimer > 0 )
		{
			MotorMgr.CCWTimer--;
		}
		else
		{
			if ( MotorMgr.CCWWAITTimer > 0 ){
				MotorMgr.MotorStatus = CCWWAIT;
			}
			else
			{
				MotorMgr.MotorStatus = IDLE;
			}
		}
	}
	else if ( MotorMgr.MotorStatus == CCWWAIT )
	{
		if ( MotorMgr.CCWWAITTimer > 0 )
		{
			MotorMgr.CCWWAITTimer--;
		}
		else
		{
			MotorMgr.MotorStatus = IDLE;
		}
	}
	else
	{
		MotorMgr.MotorStatus = IDLE;
	}
	
	if ( MotorMgr.MotorStatus == CCW )
	{	
		SET_MOTOR_INA_L;
		if ( MotorMgr.CCWSoftStart == bTRUE )
		{
			for (i=0;i<200;i++)
			{
				SET_MOTOR_INB_H;
				Hardware_DelayX1us(i);
				SET_MOTOR_INB_L;
				Hardware_DelayX1us(200-i);
			}
			MotorMgr.CCWSoftStart = bFALSE;
			SET_MOTOR_INB_H;
		}
		else
		{
			SET_MOTOR_INB_H;
		}
		DEBUG_MARK;
	}
	else if ( MotorMgr.MotorStatus == CW )
	{
		SET_MOTOR_INB_L;
		if ( MotorMgr.CWSoftStart == bTRUE )
		{
			for (i=0;i<200;i++)
			{
				SET_MOTOR_INA_H;
				Hardware_DelayX1us(i);
				SET_MOTOR_INA_L;
				Hardware_DelayX1us(200-i);
			}
			MotorMgr.CWSoftStart = bFALSE;
			SET_MOTOR_INA_H;
		}
		else
		{
			SET_MOTOR_INA_H;
		}
		DEBUG_MARK;
	}
	else if ( MotorMgr.MotorStatus == CWWAIT )
	{
		SET_MOTOR_INA_H;
		SET_MOTOR_INB_H;
		DEBUG_MARK;
	}
	else if ( MotorMgr.MotorStatus == CCWWAIT )
	{
		SET_MOTOR_INA_H;
		SET_MOTOR_INB_H;
		DEBUG_MARK;
	}
	else
	{
		SET_MOTOR_INA_L;
		SET_MOTOR_INB_L;
		DEBUG_MARK;
	}
}


