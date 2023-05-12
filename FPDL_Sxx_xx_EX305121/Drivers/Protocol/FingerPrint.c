#include "global_variable.h"
#include "Project.h"
#include "FingerPrint.h"
#include "usart.h"


#define CMD_TEST_CONNECTION 	0x0001 		//进行与设备的通讯测试
#define CMD_SET_PARAM 				0x0002 		//设 置 设 备 参 数 (Device ID, Security Level, Baudrate,Duplication Check, 		 
																				//AutoLearn,TimeOut)注：TimeOut只适用于
#define CMD_GET_PARAM 				0x0003 		//获 取 设 备 参 数 (Device ID, Security Level, Baudrate,Duplication Check,
																				//AutoLearn，TimeOut)注：TimeOut只适用于滑动采集器
#define CMD_GET_DEVICE_INFO 	0x0004 		//获取设备信息
#define CMD_ENTER_IAP_MODE 		0x0005 		//将设备设置为 IAP状态
#define CMD_GET_IMAGE 				0x0020 		//从采集器采集指纹图像并保存于 ImageBuffer 中
#define CMD_FINGER_DETECT 		0x0021 		//检测指纹输入状态
#define CMD_UP_IMAGE 					0x0022 		//将保存于 ImageBuffer 中的指纹图像上传至 HOST
#define CMD_DOWN_IMAGE 				0x0023 		//HOST下载指纹图像到模块的 ImageBuffer 中
#define CMD_SLED_CTRL 				0x0024 		//控制采集器背光灯的开/关（注：半导体传感器不用此功能）
#define CMD_STORE_CHAR 				0x0040 		//将指定编号 Ram Buffer中的 Template，注册到指定编号的库中
#define CMD_LOAD_CHAR 				0x0041 		//读取库中指定编号中的 Template到指定编号的 Ram Buffer
#define CMD_UP_CHAR 					0x0042 		//将保存于指定编号的 Ram Buffer 中的 Template 上传至 HOST
#define CMD_DOWN_CHAR 				0x0043 		//从 HOST下载 Template到模块指定编号的 Ram Buffer 中
#define CMD_DEL_CHAR 					0x0044 		//删除指定编号范围内的 Template 。
#define CMD_GET_EMPTY_ID 			0x0045 		//获取指定范围内可注册的（没有注册的）第一个模板编号。
#define CMD_GET_STATUS 				0x0046 		//获取指定编号的模板注册状态。
#define CMD_GET_BROKEN_ID 		0x0047 		//检查指定编号范围内的所有指纹模板是否存在坏损的情况
#define CMD_GET_ENROLL_COUNT 	0x0048 		//获取指定编号范围内已注册的模板个数。
#define CMD_GENERATE 					0x0060 		//将 ImageBuffer 中的指纹图像生成模板数据１４嬗谥付ū嗪诺? Ram Buffer 中。
#define CMD_MERGE 						0x0061 		//将保存于 Ram Buffer 中的两或三个模板数据融合成一个模板数据

#define CMD_MATCH 						0x0062 		//指定 Ram Buffer 中的两个指纹模板之间进行 1:1 比对
#define CMD_SEARCH 						0x0063 		//指定 Ram Buffer 中的模板与指纹库中指定
																				//定编号范围内的所有模板之间进行 1:N 比对
#define CMD_VERIFY 						0x0064 		//指定 Ram Buffer 中的治颇０逵胫肝瓶庵兄付ū嗪诺闹肝颇０逯浣? 1:1比对
#define CMD_SET_MODULE_SN 		0x0008 		//在设备中设置模块序列号信息（Module SN）
#define CMD_GET_MODULE_SN 		0x0009		//获取本设备的模块序列海 Module SN）
#define CMD_FP_CANCEL 				0x0025 		//取消指纹采集操作（只适用于带 TimeOut 参数的滑动传感器）
#define CMD_GET_ENROLLED_ID_LIST 	0x0049 //获取已注册 User ID 列表
#define CMD_ENTER_STANDY_STATE 		0x000C //使模块进入休眠状獭注：有些模块不支持菝吖δ埽淙荒？橄煊Ω弥噶罘祷爻晒?


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
        UART2_Mgr.Status = Idle; //处理模式
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
	//Bit0锛? 0-鍏抽棴鎶楁畫鐣欏姛鑳斤紝1-鎵撳紑鎶楁畫鐣欏姛鑳斤紙榛樿鐘舵?侊級銆?
	//Bit1锛? 0-鍏抽棴鎸囩汗鑶滆鍋囩畻娉曪紝1-鎵撳紑鎸囩汗鑶滆鍋囩畻娉曪紙榛樿鐘舵?侊級銆?
	//Bit2锛? 0-鍏抽棴瀛︿範鍔熻兘锛?1-鎵撳紑瀛︿範鍔熻兘锛堥粯璁ょ姸鎬侊級銆?
	//Bit3~ Bit7锛氫繚鐣欙紙榛樿涓? 0锛夈??

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

















