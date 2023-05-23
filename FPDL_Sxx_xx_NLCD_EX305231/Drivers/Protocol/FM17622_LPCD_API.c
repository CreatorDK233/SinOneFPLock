/*********************************************************************
*                                                                    *
*   Copyright (c) 2010 Shanghai FuDan MicroElectronic Inc, Ltd.      *
*   All rights reserved. Licensed Software Material.                 *
*                                                                    *
*   Unauthorized use, duplication, or distribution is strictly       *
*   prohibited by law.                                               *
*                                                                    *
*********************************************************************/

/********************************************************************/
/* 	FM17622 LPCD API 函数库V01(FM17610可以完全兼容)									*/
/* 	主要功能:						        																		*/
/* 	    实现17622芯片内部ADC_LPCD功能，主要包含两个函数        			*/
/*			(1)Lpcd_Init_Register()函数，进行LPCD功能参数配置						*/
/*			(2)Lpcd_IRQ_Event()函数，处理LPCD中断事件										*/
/* 	编制:宋耀海 																										*/
/* 	编制时间:2021年10月28日																					*/
/* 																																	*/
/********************************************************************/
#include "FM17622_LPCD_API.h"
#include "MFC_MF17622.h"
#include "StdTypes.h"
#include "Project.h"
//***********************************************
//函数名称：GetReg_Ext(unsigned char ExtRegAddr,unsigned char* ExtRegData)
//函数功能：读取扩展寄存器值
//入口参数：ExtRegAddr:扩展寄存器地址   ExtRegData:读取的值
//出口参数：unsigned char  TRUE：读取成功   FALSE:失败
//***********************************************
void GetReg_Ext(unsigned char ExtRegAddr,unsigned char* ExtRegData)
{
	uint8_t tempdata;
	WriteRawRC(JREG_EXT_REG_ENTRANCE,JBIT_EXT_REG_RD_ADDR + ExtRegAddr);
	tempdata = ReadRawRC(JREG_EXT_REG_ENTRANCE);
	*ExtRegData = tempdata;
}

//***********************************************
//函数名称：SetReg_Ext(unsigned char ExtRegAddr,unsigned char* ExtRegData)
//函数功能：写扩展寄存器
//入口参数：ExtRegAddr:扩展寄存器地址   ExtRegData:要写入的值
//出口参数：unsigned char  TRUE：写成功   FALSE:写失败
//***********************************************
void SetReg_Ext(unsigned char ExtRegAddr,unsigned char ExtRegData)
{
	WriteRawRC(JREG_EXT_REG_ENTRANCE,JBIT_EXT_REG_WR_ADDR + ExtRegAddr);
	WriteRawRC(JREG_EXT_REG_ENTRANCE,JBIT_EXT_REG_WR_DATA + ExtRegData);
}
//***********************************************
//函数名称：Lpcd_Init_Register()
//函数功能：LPCD寄存器初始化配置
//入口参数：
//出口参数：SUCCESS：配置完成 
//***********************************************
status_t Lpcd_Init_Register(void)
{
	uint8_t TryTimes=3;

	do 
	{
		if(FM17622_SoftReset() == S_SUCCESS)
		{
			SetReg_Ext(0x24, 0x14);//请勿修改
			SetReg_Ext(0x25, 0x3A);//请勿修改
			SetReg_Ext(JREG_LPCDCTRLMODE, (RF_DET_ENABLE|RF_DET_SEN_00|LPCD_ENABLE));//
			//SetReg_Ext(JREG_LPCDDETECTMODE, LPCD_TXSCALE_4|LPCD_RX_CHANGE_MODE);

			SetReg_Ext(JREG_LPCDSLEEPTIMER, LPCD_SLEEPTIME);
			SetReg_Ext(JREG_LPCDRFTIMER, LPCD_IRQINV_ENABLE|LPCD_IRQ_PUSHPULL|LPCD_RFTIME_5us);//探测时间配置使用5us
			SetReg_Ext(JREG_LPCDTHRESH_L, LPCD_THRSH_L);
			SetReg_Ext(JREG_LPCDTXCTRL2, LPCD_CWP);
			SetReg_Ext(JREG_LPCDTXCTRL3, LPCD_CWN);
			
			SetReg_Ext(JREG_LPCDREQATIMER,LPCD_REQA_TIME_5ms);//REQA操作时间,5ms兼容手机方案
			
			SetReg_Ext(JREG_LPCDREQAANA,LPCD_RXGAIN_33DB | LPCD_MINLEVEL_9 | LPCD_MODWIDTH_38);
			SetReg_Ext(JREG_LPCDDETECTMODE,LPCD_TXSCALE_4|LPCD_COMBINE_MODE);//配置REQA检测功能
			break;
		}
		TryTimes--;
		DEBUG_MARK;
	}while (TryTimes>0);

	if ( TryTimes == 0)
	{
		return S_FAIL;
	}
	else
	{
		return S_SUCCESS;
	}
}

//***********************************************
//函数名称：Lpcd_Get_ADC_Value()
//函数功能：Lpcd_Get_ADC_Value读取LPCD的ADC数据
//入口参数：
//出口参数：
//***********************************************
void Lpcd_Get_ADC_Value(void)
{
	unsigned char reg,reg1;
	unsigned char lpcd_delta1,lpcd_delta2;
	{
		GetReg_Ext(JREG_LPCDDELTA_HI, &reg);
		GetReg_Ext(JREG_LPCDDELTA_LO, &reg1);
		lpcd_delta1 = (reg<<6) + reg1;
		lpcd_delta2 = ((reg<<6)>>8);
	}	
	return;	
}

void Lpcd_ClearIrq(void)
{
	unsigned char reg;
	GetReg_Ext(JREG_LPCDIRQ, &reg);//读取LPCD中断标志
	SetReg_Ext(JREG_LPCDIRQ, reg);//CLEAR LPCD IRQ
}
//***********************************************
//函数名称：Lpcd_IRQ_Event()
//函数功能：LPCD中断处理
//入口参数：
//出口参数：
//***********************************************
void Lpcd_IRQ_Event(void)
{
	//unsigned char reg;
	
//		Lpcd_Set_Mode(FM17622_LPCD_DISABLE); 		//NPD = 1,FM17622退出休眠模式	
//		
//		GetReg_Ext(JREG_LPCDIRQ, &reg);//读取LPCD中断标志
//		SetReg_Ext(JREG_LPCDIRQ, reg);//CLEAR LPCD IRQ
//	
//		if(reg & 0x08) 
//		{
//		}
//		if(reg & 0x04)
//		{
//			Lpcd_Get_ADC_Value();//用于LPCD功能调试					
//		}
//		if(reg & 0x02)
//		{
//		}
//		
//		if((reg & 0x08) ||(reg & 0x04)||(reg & 0x02))
//		{
//			Lpcd_Card_Event();//卡片操作处理流程 

//		}
//		
//		Lpcd_Init_Register();//LPCD初始化  
//		Lpcd_Set_Mode(FM17622_LPCD_ENABLE);//返回LPCD模式	
	return;	
}


unsigned char Lpcd_Card_Event(void)
{
	unsigned char result;
	return result;
}





