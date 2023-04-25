//#include "Battery.h"
#include "adc.h"
#include "global_variable.h"
#include "Project.h"

#define a2d_databuffer_vbat	0

#define VBAT_FULL_A2D	558			//30V, A2D = (VBAT/11/5)*1024
#define VBAT_EMPTY_A2D	372			//20V, A2D = (VBAT/11/5)*1024

uint16_t a2d_data[1];

/* ***************************************************************************
*  Battery Voltage A2D  Filter
*
*/
void HardwareBatVoltageA2dFilter(uint16_t NewA2d)
{
	
	if ( NewA2d > a2d_data[a2d_databuffer_vbat] )
	{
		if ( NewA2d > (10+a2d_data[a2d_databuffer_vbat]) ){
			a2d_data[a2d_databuffer_vbat]+= 5;
		}
		else if ( NewA2d > (5+a2d_data[a2d_databuffer_vbat]) ){
			a2d_data[a2d_databuffer_vbat]+= 1;
		}
	}
	else if ( a2d_data[a2d_databuffer_vbat] > NewA2d )
	{
		if ( a2d_data[a2d_databuffer_vbat] > (NewA2d+10) ){
			a2d_data[a2d_databuffer_vbat]-= 5;
		}
		else if ( a2d_data[a2d_databuffer_vbat] > (NewA2d+5) )
		{
			a2d_data[a2d_databuffer_vbat]-= 1;
		}
	}
	
	//a2d_data[a2d_databuffer_vbat] = NewA2d;
}

void HardwareBatteryMgr_Task(void)
{
	BatteryMgr.BatteryVoltage = VoltageGet(a2d_data[a2d_databuffer_vbat]);	//in 0.1v, R1=15K, R2=10K
	
	//BatteryMgr.BatteryVoltage-=4;		//Battery voltage will drop 0.3V as serial connected SS34

 if ( BatteryMgr.BatteryVoltage > 55 ){		
		BatteryMgr.BatteryLevel = LEVEL_4;	
	}
 #if (defined ProjectIs_BarLock_S8705) || (defined ProjectIs_BarLock_S8706)
	else if ( BatteryMgr.BatteryVoltage > 50 ){		
		BatteryMgr.BatteryLevel = LEVEL_3;	
	}
	else if ( BatteryMgr.BatteryVoltage > 45 ){		
		BatteryMgr.BatteryLevel = LEVEL_2;	
	}
	else if ( BatteryMgr.BatteryVoltage > 43 ){		
		BatteryMgr.BatteryLevel = LEVEL_1;
	}
	#else
	else if ( BatteryMgr.BatteryVoltage > 51 ){		
		BatteryMgr.BatteryLevel = LEVEL_3;	
	}
	else if ( BatteryMgr.BatteryVoltage > 47 ){		
		BatteryMgr.BatteryLevel = LEVEL_2;	
	}
	else if ( BatteryMgr.BatteryVoltage > 43 ){		
		BatteryMgr.BatteryLevel = LEVEL_1;
	}
	#endif
	else {		
		BatteryMgr.BatteryLevel = LEVEL_0;	
	}
												//BatteryMgr.BatteryLevel = LEVEL_4;
	
	if ( BatteryMgr.BatteryLevel == LEVEL_0 )
	{
		if ( BatteryMgr.ProtectVoltageTriggerTimes < 3 )
		{
			BatteryMgr.ProtectVoltageTriggerTimes++;	
		}
		else{
			BatteryMgr.LowBatteryProtectionEnabled = bFALSE;
		}
	}
	else
	{
		if ( BatteryMgr.ProtectVoltageTriggerTimes > 0 )
		{
			BatteryMgr.ProtectVoltageTriggerTimes--;	
		}
		else{
			BatteryMgr.LowBatteryProtectionEnabled = bFALSE;
		}
	}	
}


/*************************************************************************
*   Hardware_Task_Analog() - Acquires A/D Data, processes touch          *
************************************************************************** 
*
*   Enter:      nothing                     
*   Calls:      Hardware_Touch_State(), Hardware_A2D_init()
*               Hardware_A2D_start(), Hardware_A2D_Fetch()                                                                                   
*   Returns:    data in local storage for call backs
*   Uses:       lots
*   General:    This task runs at the main loop 64 hertz task rate
*               and controls acquisition of the five channels of A/D 
*               data.
*               Touch screen are acquired fifteen times followed by
*               one acquisition of temp and humidity.  So temp and
*               humidity acquire twice per second, and touch acquires
*               30 times per second.
*/

void Hardware_Task_Analog( void)
{
	uint16_t temp_a2d;
	temp_a2d = ADC_Value_Got();
	HardwareBatVoltageA2dFilter(temp_a2d);
}