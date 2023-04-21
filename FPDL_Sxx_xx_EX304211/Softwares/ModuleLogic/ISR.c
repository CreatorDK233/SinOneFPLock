/* 头文件包含 INCLUDES */
#include "Project.h"
#include "IO.h"
#include "global_variable.h"
#include "BeepMgr.h"

uint16_t TimeCunt1024Hz = 0;
uint8_t UART2_RXBUFFER[2];

void SysTick_ISR(void)
{
		G_tflagbits.T1024Hz =1;
		TimeCunt1024Hz++;

		//SET_DEBUG_TX_TOGGLE;
	
		if ( TimeCunt1024Hz%4==0 )
		{
			G_tflagbits.T256Hz =1;
			
			if ( TimeCunt1024Hz%8 ==0 )
			{
				G_tflagbits.T128Hz =1;
				if ( TimeCunt1024Hz%16 ==0 )
				{
					G_tflagbits.T64Hz =1;
					if ( TimeCunt1024Hz%64 ==0 )
					{
						G_tflagbits.T16Hz =1;
						if ( TimeCunt1024Hz%128==0 )
						{
							G_tflagbits.T8Hz =1;
							if ( TimeCunt1024Hz%512==0 )
							{
								G_tflagbits.T2Hz =1;	
								if ( TimeCunt1024Hz==1024 )
								{	
									TimeCunt1024Hz = 0;
									G_tflagbits.T1Hz =1;
								}
							}
						}
					}	
				}
			}
		}
		DEBUG_MARK;
		return;
  /* USER CODE END SysTick_IRQn 1 */
}


//void TIM1_ISR(void)
//{
//	DEBUG_MARK;
//	UART2_TX_Task();
//}

void TIM3_ISR(void)	//300us interval
{
		DEBUG_MARK;
	BeepMgrTask();
		
}

void UART2_ISR(void)
{
	UART2_RXBUFFER[0]= USXCON3;
	if (	UART2_Mgr.Status == GotNewCmd )
	{
		//do nothing
	}
	else if ( (UART2_Mgr.Status == Idle )&&(UART2_RXBUFFER[0]==0xEF) )
	{
		UART2_Mgr.RX_Buffer[UART2_Mgr.RX_DataPoint] = UART2_RXBUFFER[0];
		UART2_Mgr.RX_DataPoint++;
		UART2_Mgr.Status = ReceivingData;
	}
	else if (UART2_Mgr.Status == ReceivingData)
	{
		UART2_Mgr.RX_Buffer[UART2_Mgr.RX_DataPoint] = UART2_RXBUFFER[0];
		UART2_Mgr.RX_DataPoint++;
		if ((UART2_Mgr.RX_Buffer[0]==0xEF)&&( UART2_Mgr.RX_DataPoint == (UART2_Mgr.RX_Buffer[8]+9))&&( UART2_Mgr.RX_DataPoint >8 ))
		{
			UART2_Mgr.Status = GotNewCmd;
		}
		else if ( UART2_Mgr.RX_DataPoint > 50)
		{
			UART2_Mgr.RX_DataPoint = 0x00;
			UART2_Mgr.Status = Idle;
		}
	}
			
	return;
}

 

