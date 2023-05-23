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
/* 	FM17622 LPCD API ������V01(FM17610������ȫ����)									*/
/* 	��Ҫ����:						        																		*/
/* 	    ʵ��17622оƬ�ڲ�ADC_LPCD���ܣ���Ҫ������������        			*/
/*			(1)Lpcd_Init_Register()����������LPCD���ܲ�������						*/
/*			(2)Lpcd_IRQ_Event()����������LPCD�ж��¼�										*/
/* 	����:��ҫ�� 																										*/
/* 	����ʱ��:2021��10��28��																					*/
/* 																																	*/
/********************************************************************/
#include "FM17622_LPCD_API.h"
#include "MFC_MF17622.h"
#include "StdTypes.h"
#include "Project.h"
//***********************************************
//�������ƣ�GetReg_Ext(unsigned char ExtRegAddr,unsigned char* ExtRegData)
//�������ܣ���ȡ��չ�Ĵ���ֵ
//��ڲ�����ExtRegAddr:��չ�Ĵ�����ַ   ExtRegData:��ȡ��ֵ
//���ڲ�����unsigned char  TRUE����ȡ�ɹ�   FALSE:ʧ��
//***********************************************
void GetReg_Ext(unsigned char ExtRegAddr,unsigned char* ExtRegData)
{
	uint8_t tempdata;
	WriteRawRC(JREG_EXT_REG_ENTRANCE,JBIT_EXT_REG_RD_ADDR + ExtRegAddr);
	tempdata = ReadRawRC(JREG_EXT_REG_ENTRANCE);
	*ExtRegData = tempdata;
}

//***********************************************
//�������ƣ�SetReg_Ext(unsigned char ExtRegAddr,unsigned char* ExtRegData)
//�������ܣ�д��չ�Ĵ���
//��ڲ�����ExtRegAddr:��չ�Ĵ�����ַ   ExtRegData:Ҫд���ֵ
//���ڲ�����unsigned char  TRUE��д�ɹ�   FALSE:дʧ��
//***********************************************
void SetReg_Ext(unsigned char ExtRegAddr,unsigned char ExtRegData)
{
	WriteRawRC(JREG_EXT_REG_ENTRANCE,JBIT_EXT_REG_WR_ADDR + ExtRegAddr);
	WriteRawRC(JREG_EXT_REG_ENTRANCE,JBIT_EXT_REG_WR_DATA + ExtRegData);
}
//***********************************************
//�������ƣ�Lpcd_Init_Register()
//�������ܣ�LPCD�Ĵ�����ʼ������
//��ڲ�����
//���ڲ�����SUCCESS��������� 
//***********************************************
status_t Lpcd_Init_Register(void)
{
	uint8_t TryTimes=3;

	do 
	{
		if(FM17622_SoftReset() == S_SUCCESS)
		{
			SetReg_Ext(0x24, 0x14);//�����޸�
			SetReg_Ext(0x25, 0x3A);//�����޸�
			SetReg_Ext(JREG_LPCDCTRLMODE, (RF_DET_ENABLE|RF_DET_SEN_00|LPCD_ENABLE));//
			//SetReg_Ext(JREG_LPCDDETECTMODE, LPCD_TXSCALE_4|LPCD_RX_CHANGE_MODE);

			SetReg_Ext(JREG_LPCDSLEEPTIMER, LPCD_SLEEPTIME);
			SetReg_Ext(JREG_LPCDRFTIMER, LPCD_IRQINV_ENABLE|LPCD_IRQ_PUSHPULL|LPCD_RFTIME_5us);//̽��ʱ������ʹ��5us
			SetReg_Ext(JREG_LPCDTHRESH_L, LPCD_THRSH_L);
			SetReg_Ext(JREG_LPCDTXCTRL2, LPCD_CWP);
			SetReg_Ext(JREG_LPCDTXCTRL3, LPCD_CWN);
			
			SetReg_Ext(JREG_LPCDREQATIMER,LPCD_REQA_TIME_5ms);//REQA����ʱ��,5ms�����ֻ�����
			
			SetReg_Ext(JREG_LPCDREQAANA,LPCD_RXGAIN_33DB | LPCD_MINLEVEL_9 | LPCD_MODWIDTH_38);
			SetReg_Ext(JREG_LPCDDETECTMODE,LPCD_TXSCALE_4|LPCD_COMBINE_MODE);//����REQA��⹦��
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
//�������ƣ�Lpcd_Get_ADC_Value()
//�������ܣ�Lpcd_Get_ADC_Value��ȡLPCD��ADC����
//��ڲ�����
//���ڲ�����
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
	GetReg_Ext(JREG_LPCDIRQ, &reg);//��ȡLPCD�жϱ�־
	SetReg_Ext(JREG_LPCDIRQ, reg);//CLEAR LPCD IRQ
}
//***********************************************
//�������ƣ�Lpcd_IRQ_Event()
//�������ܣ�LPCD�жϴ���
//��ڲ�����
//���ڲ�����
//***********************************************
void Lpcd_IRQ_Event(void)
{
	//unsigned char reg;
	
//		Lpcd_Set_Mode(FM17622_LPCD_DISABLE); 		//NPD = 1,FM17622�˳�����ģʽ	
//		
//		GetReg_Ext(JREG_LPCDIRQ, &reg);//��ȡLPCD�жϱ�־
//		SetReg_Ext(JREG_LPCDIRQ, reg);//CLEAR LPCD IRQ
//	
//		if(reg & 0x08) 
//		{
//		}
//		if(reg & 0x04)
//		{
//			Lpcd_Get_ADC_Value();//����LPCD���ܵ���					
//		}
//		if(reg & 0x02)
//		{
//		}
//		
//		if((reg & 0x08) ||(reg & 0x04)||(reg & 0x02))
//		{
//			Lpcd_Card_Event();//��Ƭ������������ 

//		}
//		
//		Lpcd_Init_Register();//LPCD��ʼ��  
//		Lpcd_Set_Mode(FM17622_LPCD_ENABLE);//����LPCDģʽ	
	return;	
}


unsigned char Lpcd_Card_Event(void)
{
	unsigned char result;
	return result;
}





