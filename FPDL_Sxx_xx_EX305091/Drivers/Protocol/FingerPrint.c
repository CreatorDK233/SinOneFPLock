#include "global_variable.h"
#include "Project.h"
#include "FingerPrint.h"
#include "usart.h"


#define CMD_TEST_CONNECTION 	0x0001 		//�������豸��ͨѶ����
#define CMD_SET_PARAM 				0x0002 		//�� �� �� �� �� �� (Device ID, Security Level, Baudrate,Duplication Check, 		 
																				//AutoLearn,TimeOut)ע��TimeOutֻ������
#define CMD_GET_PARAM 				0x0003 		//�� ȡ �� �� �� �� (Device ID, Security Level, Baudrate,Duplication Check,
																				//AutoLearn��TimeOut)ע��TimeOutֻ�����ڻ����ɼ���
#define CMD_GET_DEVICE_INFO 	0x0004 		//��ȡ�豸��Ϣ
#define CMD_ENTER_IAP_MODE 		0x0005 		//���豸����Ϊ IAP״̬
#define CMD_GET_IMAGE 				0x0020 		//�Ӳɼ����ɼ�ָ��ͼ�񲢱����� ImageBuffer ��
#define CMD_FINGER_DETECT 		0x0021 		//���ָ������״̬
#define CMD_UP_IMAGE 					0x0022 		//�������� ImageBuffer �е�ָ��ͼ���ϴ��� HOST
#define CMD_DOWN_IMAGE 				0x0023 		//HOST����ָ��ͼ��ģ��� ImageBuffer ��
#define CMD_SLED_CTRL 				0x0024 		//���Ʋɼ�������ƵĿ�/�أ�ע���뵼�崫�������ô˹��ܣ�
#define CMD_STORE_CHAR 				0x0040 		//��ָ����� Ram Buffer�е� Template��ע�ᵽָ����ŵĿ���
#define CMD_LOAD_CHAR 				0x0041 		//��ȡ����ָ������е� Template��ָ����ŵ� Ram Buffer
#define CMD_UP_CHAR 					0x0042 		//��������ָ����ŵ� Ram Buffer �е� Template �ϴ��� HOST
#define CMD_DOWN_CHAR 				0x0043 		//�� HOST���� Template��ģ��ָ����ŵ� Ram Buffer ��
#define CMD_DEL_CHAR 					0x0044 		//ɾ��ָ����ŷ�Χ�ڵ� Template ��
#define CMD_GET_EMPTY_ID 			0x0045 		//��ȡָ����Χ�ڿ�ע��ģ�û��ע��ģ���һ��ģ���š�
#define CMD_GET_STATUS 				0x0046 		//��ȡָ����ŵ�ģ��ע��״̬��
#define CMD_GET_BROKEN_ID 		0x0047 		//���ָ����ŷ�Χ�ڵ�����ָ��ģ���Ƿ���ڻ�������
#define CMD_GET_ENROLL_COUNT 	0x0048 		//��ȡָ����ŷ�Χ����ע���ģ�������
#define CMD_GENERATE 					0x0060 		//�� ImageBuffer �е�ָ��ͼ������ģ�����ݣ������ָ����ŵ? Ram Buffer �С�
#define CMD_MERGE 						0x0061 		//�������� Ram Buffer �е���������ģ�������ںϳ�һ��ģ������

#define CMD_MATCH 						0x0062 		//ָ�� Ram Buffer �е�����ָ��ģ��֮����� 1:1 �ȶ�
#define CMD_SEARCH 						0x0063 		//ָ�� Ram Buffer �е�ģ����ָ�ƿ���ָ��
																				//����ŷ�Χ�ڵ�����ģ��֮����� 1:N �ȶ�
#define CMD_VERIFY 						0x0064 		//ָ�� Ram Buffer �е����ģ����ָ�ƿ���ָ����ŵ�ָ��ģ��֮����? 1:1�ȶ�
#define CMD_SET_MODULE_SN 		0x0008 		//���豸������ģ�����к���Ϣ��Module SN��
#define CMD_GET_MODULE_SN 		0x0009		//��ȡ���豸��ģ�����к� Module SN��
#define CMD_FP_CANCEL 				0x0025 		//ȡ��ָ�Ʋɼ�������ֻ�����ڴ� TimeOut �����Ļ�����������
#define CMD_GET_ENROLLED_ID_LIST 	0x0049 //��ȡ��ע�� User ID �б�
#define CMD_ENTER_STANDY_STATE 		0x000C //ʹģ���������״̡ע����Щģ�鲻֧����߹��ܣ���Ȼģ����Ӧ��ָ��سɹ?


#define FPM_SID	0x01
#define FPM_DID	0x02
//extern uint8_t system_IC_busy_Flag;

void FPcmd_Init(void)
{
	UART2_Mgr.TxPoint = 0;
	UART2_Mgr.TxLength = 0;
}


void FPM_ResetRX(void)
{
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;
	//system_IC_busy_Flag = 1;
}


void FPMcmd_Excute(void)
{
	uint16_t i;
	uint16_t CmdLenth,CKS,TempCKS;
	
	if ( UART2_Mgr.Status != GotNewCmd ){
		return;
	}
	CmdLenth = (UART2_Mgr.RX_Buffer[7]*256) + UART2_Mgr.RX_Buffer[8] + 9;		
	
	if (CmdLenth >= 50) {
        UART2_Mgr.RX_DataPoint = 0x00;
        UART2_Mgr.Status = Idle; //����ģʽ
        return;
    }
	
	for (i=0;i<CmdLenth;i++)
	{
		FpmAckMgr.Buff[i] = UART2_Mgr.RX_Buffer[i];		
	}
	
	UART2_Mgr.RX_DataPoint = 0x00;
	UART2_Mgr.Status = Idle;
	
	CKS=0x0000;
	for ( i=6;i<(CmdLenth-2);i++ )
	{
		CKS = CKS+FpmAckMgr.Buff[i];	
	}

	TempCKS = (FpmAckMgr.Buff[CmdLenth-2]*256)+FpmAckMgr.Buff[CmdLenth-1];

	DEBUG_MARK;
	
	if ( CKS!=TempCKS )
	{
		DEBUG_MARK;
		FpmAckMgr.Status = GotACK;	
		FpmAckMgr.ErrorCode = Error_CheckSum;
		return;		//if check sum is failed, ignore this data strin
	}
	
	if (FpmAckMgr.Buff[6]==0x07)
	{
		FpmAckMgr.Status = GotACK;
    //if(system_IC_busy_Flag == 1)system_IC_busy_Flag =0;		
		//FpmAckMgr.Status = GotACK;
	//	FpmAckMgr.Status = 0x01;
		FpmAckMgr.ErrorCode = FpmAckMgr.Buff[9];
		FpmAckMgr.Para1 = FpmAckMgr.Buff[10]*256 + FpmAckMgr.Buff[11];
		FpmAckMgr.Para2 = FpmAckMgr.Buff[12]*256 + FpmAckMgr.Buff[13];
	}
	else
	{
		FpmAckMgr.Status = GotACK;	
 		FpmAckMgr.ErrorCode = Error_Fail;		//data package fail
		DEBUG_MARK;
	}
}

void FPM_SendGetImageCmd(void)
{
	uint8_t buff[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x01,0x00,0x05};

	//UART2_Write_TxBUFFER(&buff[0],12);
	Uart2SendStr(buff,12);

	FPM_ResetRX();
}


void FPM_SendGetEnrollImageCmd(void)
{
	uint8_t buff[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x29,0x00,0x2D};

	//UART2_Write_TxBUFFER(&buff[0],12);
	Uart2SendStr(buff,12);
	
	FPM_ResetRX();
}


void FPM_SendGenCharCmd(uint8_t BufferID)
{
	uint8_t i;
	uint16_t CKS;

	uint8_t buff[13]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x04,0x02,0x00,0x00,0x05};

	buff[10]=BufferID;

	CKS=0x0000;
	for (i=6;i<11;i++)
	{
		CKS+=buff[i];
	}
	buff[11]=CKS>>8;
	buff[12]=CKS;

	//UART2_Write_TxBUFFER(&buff[0],13);
	Uart2SendStr(buff,13);

	FPM_ResetRX();
}

void FPM_SendRegModelCmd(void)
{


	uint8_t buff[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x05,0x00,0x09};


	//UART2_Write_TxBUFFER(&buff[0],12);
	Uart2SendStr(buff,12);
	
	FPM_ResetRX();

}

void FPM_SendStoreCharCmd(uint8_t BufferID,uint16_t UserID)
{
	uint8_t i;
	uint16_t CKS;

	uint8_t buff[15]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x06,0x06,0x00,0x00,0x00,0x00,0x00};

	buff[10]=BufferID;
	
	buff[11] = UserID >> 8;
	buff[12] = UserID;
	

	CKS=0x0000;
	for (i=6;i<13;i++)
	{
		CKS+=buff[i];
	}
	buff[13]=CKS>>8;
	buff[14]=CKS;


	//UART2_Write_TxBUFFER(&buff[0],15);
	Uart2SendStr(buff,15);

	FPM_ResetRX();

}

void FPM_SendSearchCmd(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum)
{
	uint8_t i;
	uint16_t CKS;

	uint8_t buff[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

	buff[10]=BufferID;
	
	buff[11] = StartPage >> 8;
	buff[12] = StartPage;
	
	buff[13] = PageNum >> 8;
	buff[14] = PageNum;

	CKS=0x0000;
	for (i=6;i<15;i++)
	{
		CKS+=buff[i];
	}
	buff[15]=CKS>>8;
	buff[16]=CKS;

	//UART2_Write_TxBUFFER(&buff[0],17);
	Uart2SendStr(buff,17);

	FPM_ResetRX();
}

void FPM_DeleteCharCmd(uint16_t StartPageID,uint16_t CharNum)
{
	uint8_t i;
	uint16_t CKS;

	uint8_t buff[16]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x0C,0x00,0x00,0x00,0x00,0x00,0x00};

	buff[10] = StartPageID >> 8;
	buff[11] = StartPageID;
	
	buff[12] = CharNum >> 8;
	buff[13] = CharNum;

	CKS=0x0000;
	for (i=6;i<14;i++)
	{
		CKS+=buff[i];
	}
	buff[14]=CKS>>8;
	buff[15]=CKS;

	//UART2_Write_TxBUFFER(&buff[0],16);
	Uart2SendStr(buff,16);

	FPM_ResetRX();

}
void FPM_DeleteAllCharCmd(void)
{

	uint8_t buff[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x0D,0x00,0x11};

	//UART2_Write_TxBUFFER(&buff[0],12);
	Uart2SendStr(buff,12);

	FPM_ResetRX();

}


void FPM_SendGetSerialNumberCmd(void)
{

	uint8_t buff[13]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x04,0x34,0x00,0x00,0x39};

	//UART2_Write_TxBUFFER(&buff[0],13);
	Uart2SendStr(buff,13);

	FPM_ResetRX();

}

void FPM_GetValidTempleteNumCmd(void)
{

	uint8_t buff[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x1D,0x00,0x21};

	//UART2_Write_TxBUFFER(&buff[0],12);
	Uart2SendStr(buff,12);

	FPM_ResetRX();

}

void FPM_SendReadIndexTableCmd(void)
{
	uint8_t i;
	uint16_t CKS;


	uint8_t buff[13]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x04,0x1F,0x00,0x00,0x39};
	CKS=0x0000;
	for (i=6;i<11;i++)
	{
		CKS+=buff[i];
	}
	buff[11]=CKS>>8;
	buff[12]=CKS;

	//UART2_Write_TxBUFFER(&buff[0],13);
	Uart2SendStr(buff,13);

	FPM_ResetRX();

}

void FPM_SendAutoRegisterCmd(uint16_t UserID)
{
	uint8_t i;
	uint16_t CKS;

	uint8_t buff[17]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x08,0x31,0x00,0x01,0x02,0x00,0xCF,0x00,0x39};

	buff[10]=UserID>>8;
	
	buff[11]=UserID;

	CKS=0x0000;
	for (i=6;i<15;i++)
	{
		CKS+=buff[i];
	}
	buff[15]=CKS>>8;
	buff[16]=CKS;
	//UART2_Write_TxBUFFER(&buff[0],17);
	Uart2SendStr(buff,17);

	FPM_ResetRX();
}

void FPM_SetBreathingLED(uint8_t mode,uint8_t startcolor,uint8_t endcolor,uint8_t looptimes)
{
	uint8_t i;
	uint16_t CKS;

	uint8_t buff[16]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x07,0x3C,0x00,0x00,0x00,0x00,0x00,0x39};

	buff[10]=mode;
	buff[11]=startcolor;
	buff[12]=endcolor;
	buff[13]=looptimes;

	CKS=0x0000;
	for (i=6;i<14;i++)
	{
		CKS+=buff[i];
	}
	buff[14]=CKS>>8;
	buff[15]=CKS;
	//UART2_Write_TxBUFFER(&buff[0],16);
	Uart2SendStr(buff,16);

	FPM_ResetRX();
}

void FPM_SetSecurityLevel(uint8_t Level)			//from 1 ~5
{
	uint8_t i;
	uint16_t CKS;

	uint8_t buff[14]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x05,0x0E,0x05,0x00,0x00,0x39};

	buff[11]=Level;

	CKS=0x0000;
	for (i=6;i<12;i++)
	{
		CKS+=buff[i];
	}
	buff[12]=CKS>>8;
	buff[13]=CKS;

	//UART2_Write_TxBUFFER(&buff[0],14);
	Uart2SendStr(buff,14);

	FPM_ResetRX();
}

void FPM_TurnOffAntiFakeFp(void)			//from 1 ~5
{
	uint8_t i;
	uint16_t CKS;

	uint8_t buff[14]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x05,0x0E,0x08,0x00,0x00,0x39};

	buff[11]=0x05;
	//Bit0�? 0-关闭抗残留功能，1-打开抗残留功能（默认状�?�）�?
	//Bit1�? 0-关闭指纹膜认假算法，1-打开指纹膜认假算法（默认状�?�）�?
	//Bit2�? 0-关闭学习功能�?1-打开学习功能（默认状态）�?
	//Bit3~ Bit7：保留（默认�? 0）�??

	CKS=0x0000;
	for (i=6;i<12;i++)
	{
		CKS+=buff[i];
	}
	buff[12]=CKS>>8;
	buff[13]=CKS;

	//UART2_Write_TxBUFFER(&buff[0],14);
	Uart2SendStr(buff,14);

	FPM_ResetRX();
}


void FPM_SendSleepCmd(void)
{

      uint8_t buff[12]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x03,0x33,0x00,0x37};

			Uart2SendStr(buff,12);
      //UART2_Write_TxBUFFER(&buff[0],12);

	  FPM_ResetRX();

}

void FPM_SendGetChipSerialNumberCmd(void)
{

      uint8_t buff[13]={0xEF,0x01,0xFF,0xFF,0xFF,0xFF,0x01,0x00,0x04,0x34,0x00,0x00,0x39};

			Uart2SendStr(buff,13);
      //UART2_Write_TxBUFFER(&buff[0],13);

	  FPM_ResetRX();
}




void FPM_Mgr_Task(void)
{
	FPMcmd_Excute();
}

















