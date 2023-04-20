#include "TuyaWIFI.h"

#include "usart.h"
#include "IO.h"
#include "Basic_Function.h"
#include "global_variable.h"

#include "FP.h"
#include "MFC.h"
#include "GUI_Function.h"

#include "stdlib.h"
#include <string.h>


#ifdef Function_TuyaWifi

#define Def_WifiSendTime Def_GuiTimeDelayCnt025s

extern PasscodeInputMgr_t PasscodeInputMgr;

/**
 * @brief  计算校验和
 * @param[in] {pack} 数据源指针
 * @param[in] {pack_len} 计算校验和长度
 * @return 校验和
 */
uint8_t GetCheckSum(uint8_t *pack, uint8_t pack_len)
{
    uint8_t i;
    uint8_t check_sum = 0;
    
    for(i = 0; i < pack_len; i ++) {
        check_sum += *pack ++;
    }
    
    return check_sum;
}

/**
 * @brief  串口发送一段数据
 * @param[in] {in} 发送缓存指针
 * @param[in] {len} 数据发送长度
 * @return Null
 */
static void TuyaWIFISendDatas(uint8_t *in, uint16_t SendLen)
{
	Uart1SendStr(in,SendLen);
}

/**
 * @brief  向wifi串口发送一帧数据
 * @param[in] {fr_type} 帧类型
 * @param[in] {*DataBuff} 数据
 * @param[in] {DataLen} 数据长度
 * @return Null
 */
void TuyaWIFISendFrame(uint8_t fr_type, uint8_t *DataBuff,uint16_t DataLen)
{
	uint8_t check_sum = 0;
	uint16_t i = 0;
	uint8_t TuyaWIFI_tx_buf[UART_MAX_LEN];
	
	TuyaWIFI_tx_buf[HEAD_FIRST] = FRAME_FIRST;
	TuyaWIFI_tx_buf[HEAD_SECOND] = FRAME_SECOND;
	TuyaWIFI_tx_buf[PROTOCOL_VERSION] = MCU_TX_VER;
	TuyaWIFI_tx_buf[FRAME_TYPE] = fr_type;
	TuyaWIFI_tx_buf[LENGTH_HIGH] = DataLen >> 8;
	TuyaWIFI_tx_buf[LENGTH_LOW] = DataLen & 0xff;

	for( i=0; i<DataLen; i++ )
	{
		TuyaWIFI_tx_buf[i+DATA_START] = *DataBuff;
		DataBuff++;
	}
	
	DataLen += PROTOCOL_HEAD;
	check_sum = GetCheckSum((uint8_t *)TuyaWIFI_tx_buf, DataLen - 1);
	TuyaWIFI_tx_buf[DataLen - 1] = check_sum;

	TuyaWIFISendDatas((uint8_t *)TuyaWIFI_tx_buf, DataLen);
}

/**
 * @brief  判断WIFI是否已连接
 * @param[in] void
 * @return status_t
 */
status_t IfWifiIsLinked(void)
{
	if( (WifiMgr.WifiConnection.Status == Connected) || (WifiMgr.WifiConnection.Status == JustConnected) )
	{
		return S_SUCCESS;
	}
	return S_FAIL;
}

/*	ASCII
{ 0x7b		} 0x7d		" 0x22		: 0x3a		, 0x2c 		. 0x2e
*/
// 产品信息：{"p":"ffxpgjqdnqalmkdk","v":"1.0.0","cap":11}
uint8_t infoData[] = "{\"p\":\"sh0xpcipym5yiyck\",\"v\":\"1.0.0\",\"cap\":0}";

// 发送产品信息
void Wifi_SendProjectInfo(void)
{
	uint16_t DataLen = sizeof(infoData)-1;
	uint8_t DataBuff[55] = {0};
	uint8_t i = 0;
	
	memset(DataBuff, 0, DataLen);
	
	for(i = 0; i < DataLen; i++) {
		DataBuff[i] = infoData[i];
	}

//	if(DataBuff != NULL) {
		TuyaWIFISendFrame(PRODUCT_INFO_CMD,DataBuff,DataLen);
//		free(DataBuff);
//		DataBuff = NULL;
//	}
}

// 回复模块发送的联网状态信息
void Wifi_SendLinkNetworkAck(void)
{
//	uint8_t buff[7]={0x55,0xAA,0x00,0x02,0x00,0x00,0x01};
	uint16_t DataLen = 0;
	uint8_t DataBuff[] = {0x00};
	TuyaWIFISendFrame(WIFI_STATE_CMD,DataBuff,DataLen);
}

// 重置WiFi
void Wifi_SendResetNetworkConfig(void)
{
	uint16_t DataLen = 0;
	uint8_t DataBuff[] = {0x00};
	TuyaWIFISendFrame(WIFI_RESET_CMD,DataBuff,DataLen);
}

// Smart配网
void Wifi_SendSmartNetworkConfig(void)
{
	uint16_t DataLen = 1;
	uint8_t DataBuff[] = {0x00};	// Smart: 0x00		AP: 0x01
	TuyaWIFISendFrame(WIFI_MODE_CMD,DataBuff,DataLen);
	WifiMgr.Ack.TimeOutCnt++;
}

// 实时状态上报
// lenth 为buf长度
// buf 是需要发送的几个pid的数据组合
// 如 PID1 LEN1_H LEN1_L DATA1_1 DATA1_2 ... PID2 LEN2_H LEN2_L DATA2_1 DATA2_2 ...
void Wifi_SendCurrentState(uint8_t lenth, uint8_t* buf)
{ 
	uint16_t DataLen = lenth;
	uint8_t* DataBuff = buf;

	TuyaWIFISendFrame(STATE_UPLOAD_CMD,DataBuff,DataLen);
}

// 记录型状态上报
// lenth 为buf长度
// buf 是需要发送的几个pid的数据组合
// 如 PID1 LEN1_H LEN1_L DATA1_1 DATA1_2 ... PID2 LEN2_H LEN2_L DATA2_1 DATA2_2 ...
void Wifi_SendRecordState(uint8_t lenth, uint8_t* buf)
{ 
	buf[0] = 0x02;	// 0 时间无效；	1 有效 当地时间；	2 有效 格林时间
	memcpy(buf+1, WifiMgr.OfflineTempPassword.GreenTime, 6);
	TuyaWIFISendFrame(STATE_RC_UPLOAD_CMD,buf,lenth);
}

// 回复模块的命令
void Wifi_SendGotCMDAck(void)
{
	uint16_t DataLen = 0;
	uint8_t DataBuff[] = {0x00};
	TuyaWIFISendFrame(DATA_QUERT_CMD,DataBuff,DataLen);
}

// 查询网络时间
void Wifi_SendLocalTimeRequest(void)
{
	uint16_t DataLen = 0;
	uint8_t DataBuff[] = {0x00};
	TuyaWIFISendFrame(GET_LOCAL_TIME_CMD,DataBuff,DataLen);
}

// 查询格林时间
void Wifi_SendGreenTimeRequest(void)
{
	uint16_t DataLen = 0;
	uint8_t DataBuff[] = {0x00};
	TuyaWIFISendFrame(GET_GL_TIME_CMD,DataBuff,DataLen);
}

// 查询当前连接的路由器的信号强度
void Wifi_SendNetworkRequest(void)
{
	uint16_t DataLen = 0;
	uint8_t DataBuff[] = {0x00};
	TuyaWIFISendFrame(ROUTE_RSSI_CMD,DataBuff,DataLen);
}

// 获取云端临时密码
void Wifi_SendTmpPwdRequest(void)
{
	uint16_t DataLen = 0;
	uint8_t DataBuff[] = {0x00};
	TuyaWIFISendFrame(GET_TMP_PWD_CMD,DataBuff,DataLen);
}

// 获取DP缓存
//void Wifi_SendDPCacheRequest(uint16_t num, uint8_t* id)
//{
//	uint16_t DataLen = num + 1;
//	uint8_t* DataBuff = (uint8_t *) malloc(DataLen);
//	uint8_t i = 0;
//	
//	memset(DataBuff, 0, DataLen);
//	
//	DataBuff[0] = num;	// DP 个数
//	for(i = 1; i <= num; i++) {
//		DataBuff[i] = id[i-1];
//	}

//	if(DataBuff != NULL) {
//		TuyaWIFISendFrame(GET_DP_CACHE_CMD,DataBuff,DataLen);
//		free(DataBuff);
//		DataBuff = NULL;
//	}
//}

// 离线动态密码
// lenth 为pwd长度
// pwd 离线密码
void Wifi_SendOfflineTmpPWD(uint8_t* pwd, uint8_t lenth)
{ 
	uint16_t DataLen = lenth + 7;
	uint8_t* DataBuff = (uint8_t *) malloc(DataLen);
	uint8_t i = 0;
	
	memset(DataBuff, 0, DataLen);
	memcpy(DataBuff, WifiMgr.OfflineTempPassword.GreenTime, 6);
	DataBuff[6] = lenth;	// 密码长度
	
	for(i = 0; i < lenth; i++) {
		DataBuff[7+i] = pwd[i];
	}
	if(DataBuff != NULL) {
		TuyaWIFISendFrame(GET_TMP_PWD_OFFLINE_CMD,DataBuff,DataLen);
		free(DataBuff);
		DataBuff = NULL;
	}
}

//离线临时密码上报
void Wifi_ReportTempPasscode(uint8_t DpID)
{
	//TuyaWIFISendFrame(STATE_UPLOAD_CMD,DataBuff,DataLen);
	uint16_t DataLen = 7 + 20;
	uint8_t DataBuff[7 + 20] = {0x00};
	DataBuff[7] = DpID;			// DP ID
	DataBuff[8] = 0x00;			// 数据类型
	DataBuff[9] = 0x00;			// 数据长度H
	DataBuff[10] = 0x10;		// 数据长度L
	memcpy(DataBuff+11, WifiMgr.OfflineTempPassword.DecryptingData, 16);
	//Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
	Wifi_SendRecordState(DataLen, DataBuff);			// 记录型
}

// 上报SN
// lenth 为SN长度
// sn SN
//void Wifi_Send_MCU_SN(uint8_t lenth, uint8_t* sn)
//{
//	uint16_t DataLen = lenth + 1;
//	uint8_t* DataBuff = (uint8_t *) malloc(DataLen);
//	uint8_t i = 0;
//	memset(DataBuff, 0, DataLen);
//	DataBuff[0] = lenth;	// SN 长度
//	for(i = 1; i <= lenth; i++) {
//		DataBuff[i] = sn[i-1];
//	}
//	if(DataBuff != NULL) {
//		TuyaWIFISendFrame(REPORTED_MCU_SN_CMD,DataBuff,DataLen);
//		free(DataBuff);
//		DataBuff = NULL;
//	}
//}

// 回复模块重置命令
void Wifi_SendResetNoticeAck(void)
{
	uint16_t DataLen = 0;
	uint8_t DataBuff[] = {0x00};
	TuyaWIFISendFrame(WIFI_RESET_NOTICE_CMD,DataBuff,DataLen);
}

// 获取WiFi状态
//#ifdef LOCK_KEEP_ALIVE
//void Wifi_SendGetWIFIState(void)
//{
//	uint16_t DataLen = 0;
//	uint8_t DataBuff[] = {0x00};
//	TuyaWIFISendFrame(GET_WIFI_STATE_CMD,DataBuff,DataLen);
//}
//#endif

//上报 解锁
void Wifi_ReportUnlockInfo(uint8_t DpId, uint16_t id)
{
	uint16_t DataLen = 7 + 8;
	uint8_t DataBuff[7 + 8] = {0x00};
	
	DataBuff[7] = DpId;						// DP ID
	DataBuff[8] = VALUE;					// 数据类型
	DataBuff[9] = 0x00;						// 数据长度H
	DataBuff[10] = 0x04;					// 数据长度L
	DataBuff[11] = 0x00;					// 数据
	DataBuff[12] = 0x00;					// 数据
	DataBuff[13] = id>>8;					// 数据
	DataBuff[14] = id;						// 数据
	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//	Wifi_SendRecordState(DataLen, DataBuff);			// 记录型
}

//上报 新免密远程开门-带秘钥
void Wifi_ReportRemoteUnlockInfo(uint8_t DpId, uint16_t id)
{
	uint16_t DataLen = 7 + 7;
	uint8_t DataBuff[7 + 7] = {0x00};
	
	DataBuff[7] = DpId;						// DP ID
	DataBuff[8] = RAW;					// 数据类型
	DataBuff[9] = 0x00;						// 数据长度H
	DataBuff[10] = 0x03;					// 数据长度L
	DataBuff[11] = 0x00;					// 成功
	DataBuff[12] = id>>8;					// 密匙编号
	DataBuff[13] = id;						// 密匙编号
	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//	Wifi_SendRecordState(DataLen, DataBuff);			// 记录型
}

//上报 组合开锁
void Wifi_ReportCombinedUnlockInfo(uint8_t *DpId, uint16_t *id)
{
	uint16_t DataLen = 7 + 16;
	uint8_t DataBuff[7 + 16] = {0x00};
	uint8_t i;
	uint8_t InfoNum = 2;
	
	for( i=0; i<InfoNum; i++ )
	{
		DataBuff[8*i+7] = *DpId;					// DP ID
		DataBuff[8*i+8] = VALUE;					// 数据类型
		DataBuff[8*i+9] = 0x00;						// 数据长度H
		DataBuff[8*i+10] = 0x04;					// 数据长度L
		DataBuff[8*i+11] = 0x00;					// 数据
		DataBuff[8*i+12] = 0x00;					// 数据
		DataBuff[8*i+13] = *id>>8;				// 数据
		DataBuff[8*i+14] = *id;						// 数据
		if( i < (InfoNum-1) )
		{
			DpId++;
			id++;
		}
	}
	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//	Wifi_SendRecordState(DataLen, DataBuff);			// 记录型
}

//上报 告警
void Wifi_ReportAlarmInfo(WifiAlarmType_t AlarmType)
{
	uint16_t DataLen = 7 + 5;
	uint8_t DataBuff[7 + 5] = {0x00};
	
	DataBuff[7] = DpIdAlarm;			// DP ID
	DataBuff[8] = ENUM;						// 数据类型
	DataBuff[9] = 0x00;						// 数据长度H
	DataBuff[10] = 0x01;						// 数据长度L
	DataBuff[11] = AlarmType;				// 数据

	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//	Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
}

//上报 远程开锁请求倒计时
void Wifi_ReportRemoteUnlockRequestCountdown(uint16_t time)
{
	uint16_t DataLen = 7 + 8;
	uint8_t DataBuff[7 + 8] = {0x00};
	
	DataBuff[7] = DpIdAPPUnlock;	// DP ID
	DataBuff[8] = VALUE;					// 数据类型
	DataBuff[9] = 0x00;						// 数据长度H
	DataBuff[10] = 0x04;						// 数据长度L
	DataBuff[11] = 0x00;						// 数据
	DataBuff[12] = 0x00;						// 数据
	DataBuff[13] = time>>8;					// 数据
	DataBuff[14] = time;						// 数据

	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//		Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
}

//上报 电池电量状态
void Wifi_ReportBatteryLevel(uint8_t BatteryLevel)
{
	uint16_t DataLen = 7 + 5;
	uint8_t DataBuff[7 + 5] = {0x00};
	
	DataBuff[7] = DpIdBatteryLevel;	// DP ID
	DataBuff[8] = ENUM;							// 数据类型
	DataBuff[9] = 0x00;							// 数据长度H
	DataBuff[10] = 0x01;						// 数据长度L
	DataBuff[11] = BatteryLevel;				// 数据

	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//	Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
}

//上报 APP远程解锁
void Wifi_ReportAPPRemoteUnlock(uint16_t value)
{
	uint16_t DataLen = 7 + 8;
	uint8_t DataBuff[7 + 8] = {0x00};
	
	DataBuff[7] = 0x0f;						// DP ID
	DataBuff[8] = VALUE;					// 数据类型
	DataBuff[9] = 0x00;						// 数据长度H
	DataBuff[10] = 0x04;						// 数据长度L
	DataBuff[11] = 0x00;						// 数据
	DataBuff[12] = 0x00;						// 数据
	DataBuff[13] = value>>8;					// 数据
	DataBuff[14] = value;					// 数据
	
	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//	Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
}

//上报 被胁迫
//void Wifi_ReportStress(uint8_t stress)
//{
//	uint16_t DataLen = 7 + 5;
//	uint8_t DataBuff[7 + 5] = {0x00};
//	
//	DataBuff[7] = 0x10;						// DP ID
//	DataBuff[8] = BOOL;						// 数据类型
//	DataBuff[9] = 0x00;						// 数据长度H
//	DataBuff[10] = 0x01;						// 数据长度L
//	DataBuff[11] = stress;				// 数据

//	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
////	Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
//}

//上报 门铃呼叫
//void Wifi_ReportDoorBellAwake(uint8_t on)
//{
//	uint16_t DataLen = 7 + 5;
//	uint8_t DataBuff[7 + 5] = {0x00};
//	
//	DataBuff[7] = 0x13;						// DP ID
//	DataBuff[8] = BOOL;						// 数据类型
//	DataBuff[9] = 0x00;						// 数据长度H
//	DataBuff[10] = 0x01;						// 数据长度L
//	DataBuff[11] = on;						// 数据

//	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
////	Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
//}

//上报 请求远程开门密匙
void Wifi_ReportRequestRemoteUnlockPassword()
{
	uint16_t DataLen = 7 + 5;
	uint8_t DataBuff[7 + 5] = {0x00};
	
	DataBuff[7] = 0x31;						// DP ID
	DataBuff[8] = RAW;						// 数据类型
	DataBuff[9] = 0x00;						// 数据长度H
	DataBuff[10] = 0x01;						// 数据长度L
	DataBuff[11] = 0x01;						// 数据

	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//	Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
}

//上报 保存密匙成功
void Wifi_ReportSaveKeySuccess()
{
	uint16_t DataLen = 7 + 7;
	uint8_t DataBuff[7 + 7] = {0x00};
	
	DataBuff[7] = 0x31;						// DP ID
	DataBuff[8] = RAW;						// 数据类型
	DataBuff[9] = 0x00;						// 数据长度H
	DataBuff[10] = 0x03;						// 数据长度L
	DataBuff[11] = 0x00;						// 成功
	DataBuff[12] = 0x00;						// 密匙编号
	DataBuff[13] = 0x01;						// 密匙编号

	Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//	Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
}

//上报 同步所有 指纹0x19 密码0x1a 卡号0x1b ID
void Wifi_ReportAllUserDeviceId(uint8_t DpId, uint8_t* id, uint16_t lenth)
{
	uint16_t DataLen = 7 + 4 + lenth;
	uint8_t* DataBuff = (uint8_t *) malloc(DataLen);
	uint8_t i = 0;
	memset(DataBuff, 0, DataLen);

	DataBuff[7] = DpId;						// DP ID
	DataBuff[8] = RAW;						// 数据类型
	DataBuff[9] = lenth>>8;					// 数据长度H
	DataBuff[10] = lenth;					// 数据长度L

//	for(i = 4; i < DataLen; i++) {
//		DataBuff[i] = id[i-4];				// 数据
//	}
	memcpy(DataBuff+4+7, id, lenth);		// 数据
	
	if(DataBuff != NULL) {
		Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
//		Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
		free(DataBuff);
		DataBuff = NULL;
	}
}

//上报 设置新免密远程开锁密钥
//void Wifi_ReportNewRemoteUnlockPwd(uint8_t* newPwd, uint16_t lenth)
//{
//	uint16_t DataLen = 7 + 4 + lenth;
//	uint8_t* DataBuff = (uint8_t *) malloc(DataLen);
//	uint8_t i = 0;
//	memset(DataBuff, 0, DataLen);
//	
//	DataBuff[7] = 0x31;						// DP ID
//	DataBuff[8] = RAW;						// 数据类型
//	DataBuff[9] = lenth>>8;					// 数据长度H
//	DataBuff[10] = lenth;					// 数据长度L

//	memcpy(DataBuff+4+7, newPwd, lenth);		// 数据

//	if(DataBuff != NULL) {
//		Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
////		Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
//		free(DataBuff);
//		DataBuff = NULL;
//	}
//}

//上报 免密远程开锁
//void Wifi_ReportRemoteUnlock(uint8_t* Pwd, uint16_t lenth)
//{
//	uint16_t DataLen = 7 + 4 + lenth;
//	uint8_t* DataBuff = (uint8_t *) malloc(DataLen);
//	uint8_t i = 0;
//	memset(DataBuff, 0, DataLen);
//	
//	DataBuff[7] = 0x32;						// DP ID
//	DataBuff[8] = RAW;						// 数据类型
//	DataBuff[9] = lenth>>8;					// 数据长度H
//	DataBuff[10] = lenth;					// 数据长度L

//	memcpy(DataBuff+4+7, Pwd, lenth);		// 数据

//	if(DataBuff != NULL) {
//		Wifi_SendCurrentState(DataLen - 7, DataBuff + 7);	// 实时
////		Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
//		free(DataBuff);
//		DataBuff = NULL;
//	}
//}

//上报 主动消息推送
//void Wifi_ReportMessage(uint8_t* Msg, uint16_t lenth)
//{
//	uint16_t DataLen = 7 + 4 + lenth;
//	uint8_t* DataBuff = (uint8_t *) malloc(DataLen);
//	uint8_t i = 0;
//	memset(DataBuff, 0, DataLen);
//	
//	DataBuff[7] = 0xd4;						// DP ID
//	DataBuff[8] = RAW;						// 数据类型
//	DataBuff[9] = lenth>>8;					// 数据长度H
//	DataBuff[10] = lenth;					// 数据长度L

//	memcpy(DataBuff+4+7, Msg, lenth);		// 数据

//	if(DataBuff != NULL) {
//		Wifi_SendCurrentState(DataLen- 7, DataBuff + 7);	// 实时
////		Wifi_SendRecordState(DataLen, DataBuff);	// 记录型
//		free(DataBuff);
//		DataBuff = NULL;
//	}
//}

/**
 * @brief  数据帧处理
 * @param[in] {offset} 数据起始位
 * @return Null
 */
//uint8_t rssi = 0; // WiFi信号强度
//uint8_t tmppwd[12] = 0; // WiFi信号强度

void WifiDataHandle(uint8_t *offset)
{
	uint8_t cmd_type,i = 0;
	uint8_t TempPswd[8] = {0};
	uint16_t cmd_lenth = 0;
	uint32_t Temp_sendingflag = 0;

	if(offset == NULL){return;}
	cmd_type = offset[3];
	cmd_lenth &= offset[4] << 8;
	cmd_lenth += offset[5];

	switch(cmd_type) 
	{
		case PRODUCT_INFO_CMD:	// 模块查询MCU信息
			WifiMgr.Ack.Status = GotProjectInfoCmdACK;
			break;
		
		case WIFI_STATE_CMD:	// 模块上报设备状态
			if(cmd_lenth > 0) 
			{
				if(offset[6] == 0x04) 
				{
					WifiMgr.WifiConnection.Status = JustConnected;
					WifiMgr.PostMgr.Flag.Bits.PowerLevel = 1;
					WifiMgr.RemoteUnlockMgr.RequestPasswordTimeCnt = Def_GuiTimeDelayCnt1s;
				}
			}
			WifiMgr.Ack.Status = WaitWifiStateCmdACK;
			break;
		
		case WIFI_RESET_CMD:	// 模块回复重置WiFi指令
			WifiMgr.WifiConnection.Status = NotConnected;
			break;
		case WIFI_MODE_CMD:	// 重置WiFi 选择配置模式
			if(offset[5] == 0x00) 
			{
				WifiMgr.PostMgr.Flag.Bits.StartNetwork = 0;
				WifiMgr.Ack.TimeOutCnt = 0;
			}
			break;
		case ROUTE_RSSI_CMD:	// 模块回复WiFi信号强度
			if(cmd_lenth > 0) {
				if(offset[6] == 0x01) {	// 成功
					//rssi = offset[7];	// WiFi信号强度 0 ~ 100
				}
				else
				{
					// 模块未连接路由器
				}
			}
			break;

		case STATE_UPLOAD_CMD:	// 模块回复实时状态指令
			if(cmd_lenth > 0) {
				if(offset[6] == 0x00) {
					// 执行成功
					Temp_sendingflag = WifiMgr.PostMgr.SendingFlag.TotalFlagBits;
					WifiMgr.PostMgr.Flag.TotalFlagBits  &= ~Temp_sendingflag;
					WifiMgr.Ack.TimeOutCnt = 0;
				} else if(offset[6] == 0x01) {
					// 执行失败

				}
			}		
			break;
			
		case STATE_RC_UPLOAD_CMD:	// 模块回复记录型状态指令
			if(cmd_lenth > 0) {
				Temp_sendingflag = WifiMgr.PostMgr.SendingFlag.TotalFlagBits;
				WifiMgr.PostMgr.Flag.TotalFlagBits  &= ~Temp_sendingflag;
				WifiMgr.Ack.TimeOutCnt = 0;
//				if(offset[6] == 0x00) {
//					// 记录成功
//				} else if(offset[6] == 0x01) {
//					// 当前记录上报成功，且还有滞留记录需要上报
//				} else if(offset[6] == 0x02) {
//					// 记录失败
//				}
			}		
			break;
			
		case GET_GL_TIME_CMD:
			if(cmd_lenth > 0) 
			{
				if(offset[6] == 0x01)
				{
					memcpy(WifiMgr.OfflineTempPassword.GreenTime, offset+7, 6);
					WifiMgr.PostMgr.Flag.Bits.RequestGreenTime = 0;
					WifiMgr.Ack.TimeOutCnt = 0;
					WifiMgr.PostMgr.Flag.Bits.TempPasswordCheck = 1;
				}
			}
			break;

		case DATA_QUERT_CMD:	// 模块命令下发
			if(cmd_lenth > 0) 
			{
				if( (offset[5] == 0x19) &&  (offset[6] == 0x31) && (offset[9] == 0x15) ) 
				{
					if( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestPassword )
					{
						memcpy(WifiMgr.RemoteUnlockMgr.Passward, offset+23, 8);
						WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockStandby;
						WifiMgr.PostMgr.Flag.Bits.SaveKeySuccess = 1;
					}
				}
				else if( offset[6] == 0x32 ) 
				{
					if( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestWait )
					{
						if( offset[5] != 0x11 )
						{
							WifiMgr.RemoteUnlockMgr.Result = IsFail;
						}
						else
						{
							memcpy(TempPswd, offset+13, 8);
							for(i=0;i<8;i++)
							{
								if( TempPswd[i] != WifiMgr.RemoteUnlockMgr.Passward[i] )
								{
									WifiMgr.RemoteUnlockMgr.Result = IsFail;
									break;
								}
								if( i == 7 )
								{
									WifiMgr.RemoteUnlockMgr.Result = IsSuccess;
								}
							}
						}
					}
				}
				WifiMgr.Ack.Status = WifiAnswerCmdACK;
			}
			break;
			
	//	case GET_TMP_PWD_CMD:	// 模组回复一组临时密码
	//		if(cmd_lenth > 0) {
	//			if(offset[6] == 0x01) {	// 成功
	//				tmppwd[0] = offset[7];	// 年
	//				tmppwd[1] = offset[8];	// 月
	//				tmppwd[2] = offset[9];	// 日
	//				tmppwd[3] = offset[10];	// 时
	//				tmppwd[4] = offset[11];	// 分
	//				tmppwd[5] = offset[12];	// 秒
	//				// 模拟发送该指令，没有收到测试工具返回密码，后续添加
	//			}
	//			else
	//			{
	//				// 获取失败
	//			}
	//		}
	//		
	//		dataTest[0] = cmd_lenth;
	//		Wifi_SendTestAck(1);
	//		break;
	
		case GET_TMP_PWD_OFFLINE_CMD:
			if(cmd_lenth > 0) 
			{
				if( offset[6] == 0x00 )
				{
					if( offset[7] == 0x00 || offset[7] == 0x01 )
					{
						//开锁成功
						if( WifiMgr.OfflineTempPassword.Status == TempPswdChecking )
						{
							memcpy(WifiMgr.OfflineTempPassword.DecryptingData, offset+9, 16);
							WifiMgr.OfflineTempPassword.Status = TempPswdCorrect;
						}
					}
					else if( offset[7] == 0x02 )
					{
						//操作成功
						if( WifiMgr.OfflineTempPassword.Status == TempPswdChecking )
						{
							if( offset[8] == 0x10 )
							{
								memcpy(WifiMgr.OfflineTempPassword.DecryptingData, offset+9, 16);
							}
							WifiMgr.OfflineTempPassword.Status = ClearCodeCorrect;
						}
					}
				}
				else
				{
					//验证失败
					if( WifiMgr.OfflineTempPassword.Status == TempPswdChecking )
					{
						WifiMgr.OfflineTempPassword.Status = TempPswdWrong;
					}
				}
				
			}
			break;
		
		default:
			break;	
	}
}

//获取开锁类型和用户编号
void GotIdentifyResult(uint8_t *Type, uint16_t *ID)
{
	switch(UserIdentifyResultMgr.IdentifyType)
	{
		case CARD:
			*Type ++ = DpIdCardUnlock;
			*Type = 0;
			*ID ++ = (uint16_t)UserIdentifyResultMgr.CardUserID;
			*ID = 0;
			break;
		case PASSCODE:
			*Type ++ = DpIdPasswordUnlock;
			*Type = 0;
			*ID ++ = (uint16_t)UserIdentifyResultMgr.PasscodeUserID;
			*ID = 0;
			break;
		case FINGERPRINT:
			*Type ++ = DpIdFingerUnlock;
			*Type = 0;
			*ID ++ = (uint16_t)UserIdentifyResultMgr.FPUserID;
			*ID = 0;
			break;
		case FINGERPRINTandCARD:
			*Type ++ = DpIdFingerUnlock;
			*Type = DpIdCardUnlock;
			*ID ++ = (uint16_t)UserIdentifyResultMgr.FPUserID;
			*ID = (uint16_t)UserIdentifyResultMgr.CardUserID;
			break;
		case FINGERPRINTandPASSCODE:
			*Type ++ = DpIdFingerUnlock;
			*Type = DpIdPasswordUnlock;
			*ID ++ = (uint16_t)UserIdentifyResultMgr.FPUserID;
			*ID = (uint16_t)UserIdentifyResultMgr.PasscodeUserID;
			break;
		case CARDandPASSCODE:
			*Type ++ = DpIdPasswordUnlock;
			*Type = DpIdCardUnlock;
			*ID ++ = (uint16_t)UserIdentifyResultMgr.PasscodeUserID;
			*ID = (uint16_t)UserIdentifyResultMgr.CardUserID;
			break;
		//case WIFITEMPPASSCODE:
			
		default:
			break;
	}
}

//开锁信息上报
void WifiUnlockInfoExecute(void)
{
	uint8_t UnlockType[2]={0};
	uint16_t UnlockID[2]={0};
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		GotIdentifyResult(UnlockType,UnlockID);
		if( UnlockType[1] != 0 )
		{
			Wifi_ReportCombinedUnlockInfo(UnlockType,UnlockID);
		}
		else if( UnlockType[0] != 0 )
		{
			Wifi_ReportUnlockInfo(UnlockType[0],UnlockID[0]);
		}
		else
		{
			WifiMgr.PostMgr.Flag.Bits.Unlock = 0;
		}
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//请求远程开锁密匙
void WifiRequestRemoteUnlockKeyExecute()
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		Wifi_ReportRequestRemoteUnlockPassword();
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//远程开锁密匙保存成功上报
void WifiSaveKeySuccessExecute()
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		Wifi_ReportSaveKeySuccess();
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//远程开锁信息上报
void WifiRemoteUnlockSuccessExecute()
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		Wifi_ReportRemoteUnlockInfo(DpIdRemoteUnlock,1);
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//发送请求远程开锁倒计时
void WifiRemoteUnlockTimeSendExecute(uint16_t time)
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		Wifi_ReportRemoteUnlockRequestCountdown(time);
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//上报电量等级
void WifiPowerLevelExecute(void)
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		switch(BatteryMgr.BatteryLevel)
		{
			case LEVEL_4:
				Wifi_ReportBatteryLevel(PowerLevelHigh);
				break;
			case LEVEL_3:
			case LEVEL_2:
				Wifi_ReportBatteryLevel(PowerLevelMedium);
				break;
			case LEVEL_1:
				Wifi_ReportBatteryLevel(PowerLevelLow);
				break;
			case LEVEL_0:
				Wifi_ReportBatteryLevel(PowerLevelEmpty);
				break;
			default:
				break;
		}
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//系统锁定报警
void WifiSysLockExecute(WifiAlarmType_t AlarmLock)
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		Wifi_ReportAlarmInfo(AlarmLock);
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//wifi请求格林时间
void WifiRequestGreenTimeExecute()
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		Wifi_SendGreenTimeRequest();
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//wifi临时密码认证
void WifiTempPasscodeIdentify(uint8_t BUFF[],uint8_t LEN)
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		Wifi_SendOfflineTmpPWD(BUFF,LEN);
	}
}

//wifi离线临时密码上报
void WifiTempPasscodeExecute(uint8_t DpID)
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		Wifi_ReportTempPasscode(DpID);
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//和云端同步管理员和用户
void WifiSyncExecute(uint8_t SyncType)
{
	uint8_t MasterNum, UserNum, TotalNum;
	uint8_t AvailabeID[100]={0};
	uint8_t i;
	
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		if( SyncType == DpIdFingerSync )
		{
			MasterNum = Get_Availabe_FPmasterID()-1;
			UserNum = Get_Availabe_FPuserID()-11;
			TotalNum = MasterNum + UserNum;
			for( i = 0; i < MasterNum; i++ )
			{
				AvailabeID[i] = i+1;
			}
			for( i = 0; i < UserNum; i++ )
			{
				AvailabeID[ MasterNum + i ] = i+11;
			}
			Wifi_ReportAllUserDeviceId(DpIdFingerSync,AvailabeID,(uint16_t)TotalNum);
		}
		else if( SyncType == DpIdPasswordSync )
		{
			MasterNum = CheckHowManyRegisteredPasscodeMaster();
			UserNum = CheckHowManyRegisteredPasscodeUser();
			TotalNum = MasterNum + UserNum;
			for( i = 0; i < MasterNum; i++ )
			{
				AvailabeID[i] = i+1;
			}
			for( i = 0; i < UserNum; i++ )
			{
				AvailabeID[ MasterNum + i ] = i+3;
			}
			Wifi_ReportAllUserDeviceId(DpIdPasswordSync,AvailabeID,(uint16_t)TotalNum);
		}
		else if( SyncType == DpIdCardSync )
		{
			TotalNum = CheckHowManyRegisteredCardUser();
			for( i = 0; i < TotalNum; i++ )
			{
				AvailabeID[i] = i+1;
			}
			Wifi_ReportAllUserDeviceId(DpIdCardSync,AvailabeID,(uint16_t)TotalNum);
		}
		WifiMgr.Ack.TimeOutCnt++;
	}
}

//远程开锁倒计时上报
void WifiRemoteUnlockCountdown(void)
{
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		if( WifiMgr.PostMgr.RemoteUnlockCountdown > 0 )
		{
			WifiMgr.PostMgr.RemoteUnlockCountdown--;
			WifiMgr.PostMgr.Flag.Bits.DoorBell = 1;
		}
	}
}
	
//WIFI指令发送
void WifiCMD_Post(void)
{
	//250ms发一次命令，不同命令通过标志位缓存，依照优先级发送
	if( WifiMgr.PostMgr.WifiPostIntervalCnt <= 0 )
	{
		WifiMgr.PostMgr.SendingFlag.TotalFlagBits = 0;
		if( WifiMgr.PostMgr.Flag.Bits.Info != 0 )
		{
			WifiMgr.PostMgr.Flag.Bits.Info = 0;//回复型指令只发一次，不做失败重发
			Wifi_SendProjectInfo();//发送设备信息
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if( WifiMgr.PostMgr.Flag.Bits.StartNetwork != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.StartNetwork = 1;
			Wifi_SendSmartNetworkConfig();//发送开锁配网指令
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if( WifiMgr.PostMgr.Flag.Bits.WifiState != 0 )
		{
			WifiMgr.PostMgr.Flag.Bits.WifiState = 0;//回复型指令只发一次，不做失败重发
			Wifi_SendLinkNetworkAck();//联网成功应答
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.AnswerCmd != 0 )
		{
			WifiMgr.PostMgr.Flag.Bits.AnswerCmd = 0;//回复型指令只发一次，不做失败重发
			Wifi_SendGotCMDAck();//获取指令应答
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.RequestRemoteUnlockPassword != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.RequestRemoteUnlockPassword = 1;
			WifiRequestRemoteUnlockKeyExecute();//请求远程开锁密匙
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.Alarm != 0 )
		{
			//WifiMgr.PostMgr.Flag.Bits.Alarm = 0;
			WifiMgr.PostMgr.SendingFlag.Bits.Alarm = 1;
			WifiSysLockExecute(WifiMgr.PostMgr.AlarmType);//系统锁定记录上报
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.DoorBell != 0 )
		{
			//WifiMgr.PostMgr.Flag.Bits.DoorBell = 0;
			WifiMgr.PostMgr.SendingFlag.Bits.DoorBell = 1;
			WifiRemoteUnlockTimeSendExecute(WifiMgr.PostMgr.RemoteUnlockCountdown);//发送请求远程开锁倒计时
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.Unlock != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.Unlock = 1;
			WifiUnlockInfoExecute();//开锁信息上报
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.RemoteUnlock != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.RemoteUnlock = 1;
			WifiRemoteUnlockSuccessExecute();//远程开锁信息上报
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.RequestGreenTime != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.RequestGreenTime = 1;
			WifiRequestGreenTimeExecute();//请求格林时间
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.TempPasswordUnlock != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.TempPasswordUnlock = 1;
			WifiTempPasscodeExecute(DpIdOfflineTmpPWDUnlock);//离线临时密码开锁上报
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.DeleteTempPassword != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.DeleteTempPassword = 1;
			WifiTempPasscodeExecute(DpIdOfflineDeleteCode);//离线临时密码清空码上报
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.TempPasswordCheck != 0 )
		{
			//对于只可使用一次密码，重发可能会导致结果出错，此处不做重发判定
			WifiMgr.PostMgr.Flag.Bits.TempPasswordCheck = 0;
			WifiMgr.OfflineTempPassword.Status = TempPswdChecking;
			WifiMgr.OfflineTempPassword.TempPswdCnt = Def_GuiTimeDelayCnt1s5;//1.5s没收到回复视为超时
			WifiTempPasscodeIdentify(PasscodeInputMgr.InputBuff,10);//临时密码认证
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.PowerLevel != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.PowerLevel = 1;
			WifiPowerLevelExecute();//电池电量发送
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.FingerSync != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.FingerSync = 1;
			WifiSyncExecute(DpIdFingerSync);//同步指纹用户
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.PasswordSync != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.PasswordSync = 1;
			WifiSyncExecute(DpIdPasswordSync);//同步密码用户
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.CardSync != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.CardSync = 1;
			WifiSyncExecute(DpIdCardSync);//同步卡片用户
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
		else if ( WifiMgr.PostMgr.Flag.Bits.SaveKeySuccess != 0 )
		{
			WifiMgr.PostMgr.SendingFlag.Bits.SaveKeySuccess = 1;
			WifiSaveKeySuccessExecute();//远程开锁密匙保存成功上报
			WifiMgr.RemoteUnlockMgr.RequestRemoteUnlockisReady = bTRUE;
			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
		}
//		else
//		{
//			//无事件时回复
//			Wifi_SendGotCMDAck();
//			WifiMgr.PostMgr.WifiPostIntervalCnt = Def_WifiSendTime;
//		}
	}
	
	if( WifiMgr.PostMgr.WifiPostIntervalCnt > 0 )
	{
		WifiMgr.PostMgr.WifiPostIntervalCnt--;
	}
}

//WIFI指令处理
void WifiCMD_Excute(void)
{
	uint8_t i;
	uint16_t CmdLenth,CKS,TempCKS;
	
	if ( UART1_Mgr.Status != GotNewCmd ){
		return;
	}
	
	CmdLenth = (UART1_Mgr.RX_Buffer[4]*256) + UART1_Mgr.RX_Buffer[5] + 7;		
	
	if ( CmdLenth >= 80 ) 
	{
		UART1_Mgr.RX_DataPoint = 0x00;
		UART1_Mgr.Status = Idle; //处理模式
		return;
  }
	
	for (i=0;i<CmdLenth;i++)
	{
		WifiMgr.Ack.Buff[i] = UART1_Mgr.RX_Buffer[i];		
	}
	
	UART1_Mgr.RX_DataPoint = 0x00;
	UART1_Mgr.Status = Idle;
	
	CKS = GetCheckSum(WifiMgr.Ack.Buff,CmdLenth-1);

	TempCKS = WifiMgr.Ack.Buff[CmdLenth-1];
	
	if ( CKS!=TempCKS )
	{
		WifiMgr.Ack.Status = WifiAckIdle;	
		return;		//if check sum is failed, ignore this data strin
	}
	WifiMgr.OFFPowerCnt = 0;
	WifiDataHandle(WifiMgr.Ack.Buff);
}

//结束远程开锁流程
void WifiRemoteUnlockStop(void)
{
	WifiMgr.RemoteUnlockMgr.Result = IsUnknow;
	WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockStandby;
	WifiMgr.PostMgr.Flag.Bits.DoorBell = 1;
	WifiMgr.PostMgr.RemoteUnlockCountdown = 0;
}
	
//wifi模块主循环
void Wifi_Handle(void)
{
	//处理ack标志位
	if( WifiMgr.Ack.Status != 0 )
	{
		switch(WifiMgr.Ack.Status)
		{
			case GotProjectInfoCmdACK:
				WifiMgr.PostMgr.Flag.Bits.Info = 1;
				break;
			case WaitWifiStateCmdACK:
				WifiMgr.PostMgr.Flag.Bits.WifiState = 1;
				break;
			case WifiAnswerCmdACK:
				WifiMgr.PostMgr.Flag.Bits.AnswerCmd = 1;
				break;
			/*
			case xxx:
				break;
			*/
			default:
				break;
		}
		WifiMgr.Ack.Status = WifiAckIdle;
	}
	//模块超时未应答处理
	if( WifiMgr.Ack.TimeOutCnt > 20 )
	{
		SET_WIFIPOWER_OFF;
		Soft_Delay1ms(1);
		SET_WIFIPOWER_ON;
		Wifi_Rst();
	}
	//远程开锁密匙请求（联网后1.5s发送请求）
	if( WifiMgr.RemoteUnlockMgr.RequestPasswordTimeCnt > 0 )
	{
		WifiMgr.RemoteUnlockMgr.RequestPasswordTimeCnt--;
		if( WifiMgr.RemoteUnlockMgr.RequestPasswordTimeCnt == 0 )
		{
			WifiMgr.PostMgr.Flag.Bits.RequestRemoteUnlockPassword = 1;
			WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockRequestPassword;
		}
	}
	//远程开锁开始请求开门
	if( ( WifiMgr.RemoteUnlockMgr.WaitRequestRemoteUnlock  == bTRUE )&& \
			( WifiMgr.RemoteUnlockMgr.RequestRemoteUnlockisReady == bTRUE ) )
	{
		WifiMgr.RemoteUnlockMgr.WaitRequestRemoteUnlock = bFALSE;
		WifiMgr.PostMgr.Flag.Bits.DoorBell = 1;
		WifiMgr.RemoteUnlockMgr.Status = RemoteUnlockRequestWait;
		if( SystemPowerMgr.SleepDelayTimerCnt != 0 )
		{
			WifiMgr.PostMgr.RemoteUnlockCountdown = Def_RemoteUnlockCountdown;
		}
		else
		{
			WifiMgr.PostMgr.RemoteUnlockCountdown = 0;
		}
		WifiMgr.RemoteUnlockMgr.Result = IsUnknow;
		SystemPowerMgr.SleepDelayTimerCnt = (Def_RemoteUnlockCountdown+1)*64;
	}
	//远程开锁时开锁判断
	if( WifiMgr.RemoteUnlockMgr.Status == RemoteUnlockRequestWait )
	{
		if( WifiMgr.PostMgr.RemoteUnlockCountdown > 0 )
		{
			if( WifiMgr.RemoteUnlockMgr.Result == IsSuccess )
			{
				//开锁
				UnlockSuccess(RemoteUnlock);
				//远程开锁上报
				WifiMgr.PostMgr.Flag.Bits.RemoteUnlock = 1;
				WifiRemoteUnlockStop();
			}
			else if( WifiMgr.RemoteUnlockMgr.Result == IsFail )
			{
				WifiRemoteUnlockStop();
			}
		}
		else{
			WifiRemoteUnlockStop();
		}
	}
	//离线临时密码开锁
	if( WifiMgr.OfflineTempPassword.Status == TempPswdChecking )
	{
		//超时返回错误
		if( WifiMgr.OfflineTempPassword.TempPswdCnt > 0 )
		{
			WifiMgr.OfflineTempPassword.TempPswdCnt--;
		}
		else{
			WifiMgr.OfflineTempPassword.Status = TempPswdWrong;
		}
	}
	else if( WifiMgr.OfflineTempPassword.Status == TempPswdCorrect )
	{
		WifiMgr.OfflineTempPassword.Status = TempPswdIdle;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifySuccess;
		PasscodeUserIdentifyMgr.UserID = 1;
		//PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifySuccessScreenTimeDelay;
		UserIdentifyResultMgr.PasscodeType = WifiTempPasscode;
	}
	else if( WifiMgr.OfflineTempPassword.Status == ClearCodeCorrect )
	{
		WifiMgr.OfflineTempPassword.Status = TempPswdIdle;
		PasscodeUserIdentifyMgr.Status = DeleteWifiTempPasscodeSuccess;
		WifiMgr.PostMgr.Flag.Bits.DeleteTempPassword = 1;
	}
	else if( WifiMgr.OfflineTempPassword.Status == TempPswdWrong )
	{
		WifiMgr.OfflineTempPassword.Status = TempPswdIdle;
		PasscodeUserIdentifyMgr.Status = PasscodeIdentifyFail;
		//PasscodeUserIdentifyMgr.TimeCnt =Def_IdendtifyFailScreenTimeDelay;
	}
	//超时关闭wifi
	if( IfWifiIsLinked() == S_SUCCESS )
	{
		WifiMgr.OFFPowerCnt++;
		if( WifiMgr.OFFPowerCnt >= Def_GuiTimeDelayCnt5s )
		{
			Wifi_DeInit();
		}
	}
}

void Wifi_Mgr_Task(void)//16ms
{
	if( WifiMgr.PowerState == WIFIPowerClosed )
	{
		if(( WifiMgr.PostMgr.Flag.TotalFlagBits != 0 )||( WifiMgr.RemoteUnlockMgr.WaitRequestRemoteUnlock == bTRUE ))
		{
			Wifi_Init();
		}
	}
	else
	{
		WifiCMD_Post();
		WifiCMD_Excute();
		Wifi_Handle();
	}
}

//重置wifi相关数据
void Wifi_Rst(void)
{
	UART1_Mgr.RX_DataPoint = 0x00;
	UART1_Mgr.Status = Idle;
	WifiMgr.OFFPowerCnt = 0;
	WifiMgr.PostMgr.Flag.TotalFlagBits = 0;
	WifiMgr.PostMgr.SendingFlag.TotalFlagBits = 0;
	WifiMgr.PostMgr.WifiPostIntervalCnt = 0;
	WifiMgr.PostMgr.RemoteUnlockCountdown = 0;
	WifiMgr.Ack.Status = WifiAckIdle;
	WifiMgr.Ack.TimeOutCnt = 0;
	WifiMgr.WifiConnection.Status = NotConnected;
	WifiMgr.RemoteUnlockMgr.RequestPasswordTimeCnt = 0;
	WifiMgr.RemoteUnlockMgr.WaitRequestRemoteUnlock = bFALSE;
	WifiMgr.RemoteUnlockMgr.RequestRemoteUnlockisReady = bFALSE;
	WifiMgr.OfflineTempPassword.Status = TempPswdIdle;
}

void Wifi_Init(void)
{
	SET_WIFIPOWER_ON;
	MX_UART1_Init();
	WifiMgr.PowerState = WIFIPowerOpened;
}

void Wifi_DeInit(void)
{
	SET_WIFIPOWER_OFF;
	MX_UART1_DeInit();
	Wifi_Rst();
	WifiMgr.PowerState = WIFIPowerClosed;
}

/********************Text************************/
//uint8_t UserID[12] = {0x01, 0x12, 0x03, 0x04, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12, 0x12};
//uint8_t NewRemotePwd[6] = {0x01, 0x2, 0x03, 0x04, 0x10, 0x12};
//void test_fun(void)
//{
//	Wifi_SendResetNetworkConfig();
////	Wifi_ReportUnlockInfo(DpIdCardUnlock, 200);
////	Wifi_ReportAlarmInfo(StayAlarm);
////	Wifi_ReportRemoteUnlockRequestCountdown(120);
////	Wifi_ReportBatteryLevel(PowerLevelMedium);
////	Wifi_ReportAPPRemoteUnlock(4);
////	Wifi_ReportStress(1);
////	Wifi_ReportDoorBellAwake(1);
////	Wifi_ReportAllUserDeviceId(0x1b, UserID, 12); //指纹0x19 密码0x1a 卡号0x1b
////	Wifi_ReportNewRemoteUnlockPwd(NewRemotePwd, 6);
////	Wifi_ReportRemoteUnlock(NewRemotePwd, 6);
////	Wifi_ReportMessage(NewRemotePwd, 6);
//}
//uint8_t dataTest[100] = {0};
//void Wifi_SendTestAck(uint8_t lenth)
//{
//	uint16_t DataLen = lenth + 1;
//	uint8_t* DataBuff = (uint8_t *) malloc(DataLen);
//	uint8_t i = 0;
//	
//	memset(DataBuff, 0, DataLen);
//	
//	DataBuff[0] = lenth;

//	for(i = 1; i <= lenth; i++) {
//		//DataBuff[i] = dataTest[i-1];
//	}

//	if(DataBuff != NULL) {
//		TuyaWIFISendFrame(REPORTED_MCU_SN_CMD,DataBuff,DataLen);
//		free(DataBuff);
//		DataBuff = NULL;
//	}
//}
#endif