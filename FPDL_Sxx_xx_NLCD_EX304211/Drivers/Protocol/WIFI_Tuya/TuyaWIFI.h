#ifndef _TuyaWIFI_h_
#define _TuyaWIFI_h_

#include "Project.h"
#include "StdTypes.h"

//=============================================================================
//帧的字节顺序
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//数据帧类型
//=============================================================================
#define         PRODUCT_INFO_CMD                0x01                            //产品信息
#define         WIFI_STATE_CMD                  0x02                            //wifi工作状态	
#define         WIFI_RESET_CMD                  0x03                            //重置wifi
#define         WIFI_MODE_CMD                   0x04                            //选择smartconfig/AP模式
#ifdef STATE_FAST_UPLOAD_ENABLE
#define         STATE_UPLOAD_CMD                0x18                            //实时状态快速上报
#define         STATE_RC_UPLOAD_CMD             0x19                            //记录型状态快速上报
#else
#define         STATE_UPLOAD_CMD                0x05                            //实时状态上报
#define         STATE_RC_UPLOAD_CMD             0x08                            //记录型状态上报
#endif
#define         GET_LOCAL_TIME_CMD              0x06                            //获取本地时间
#define         WIFI_TEST_CMD                   0x07                            //wifi功能测试
#define         DATA_QUERT_CMD                  0x09                            //命令下发
#define         WIFI_UG_REQ_CMD                 0x0a                            //MCU请求WIFI模块固件升级
#define         ROUTE_RSSI_CMD                  0x0b                            //查询当前连接路由器信号强度
#define         MCU_UG_REQ_CMD                  0x0c                            //MCU请求MCU固件升级
#define         UPDATE_START_CMD                0x0d                            //MCU升级开始
#define         UPDATE_TRANS_CMD                0x0e                            //MCU升级传输
#define         GET_GL_TIME_CMD                 0x10                            //获取格林时间
#define         GET_TMP_PWD_CMD           			0x14                            //请求云端临时密码
#define         GET_DP_CACHE_CMD                0x15                            //获取dp缓存指令
#define         GET_TMP_PWD_OFFLINE_CMD         0x16                            //请求离线动态密码
#define         REPORTED_MCU_SN_CMD             0x17                            //上报MCU的SN
#ifdef LOCK_KEEP_ALIVE
#define         GET_WIFI_STATE_CMD              0x1A                            //获取WiFi状态
#endif
#define         WIFI_RESET_NOTICE_CMD           0x25                            //模块重置状态通知



//=============================================================================
#define MCU_RX_VER              0x00                                            //MCU接收协议版本号
#define MCU_TX_VER              0x00                                            //MCU发送协议版本号
#define PROTOCOL_HEAD           0x07                                            //固定协议头长度
#define FIRM_UPDATA_SIZE        256                                             //升级包大小
#define FRAME_FIRST             0x55
#define FRAME_SECOND            0xaa

#define PowerLevelHigh          0x00
#define PowerLevelMedium        0x01
#define PowerLevelLow           0x02
#define PowerLevelEmpty         0x03

typedef enum
{
	DpIdFingerUnlock				=	0x01,	// 指纹解锁
	DpIdPasswordUnlock			=	0x02,	// 普通密码解锁
	DpIdTMPPWDUnlock				=	0x03,	// 临时密码解锁
	DpIdCardUnlock    			=	0x05,	// 卡片解锁
	DpIdAlarm								= 0x08,	// 告警
	DpIdAPPUnlock						= 0x09,	// APP解锁
	DpIdBatteryLevel				= 0x0b,	// 电池电量
	DpIdFingerSync 					= 0x19,	// 指纹同步
	DpIdPasswordSync 				= 0x1a,	// 密码同步
	DpIdCardSync 						= 0x1b,	// 卡片同步
	DpIdOfflineTmpPWDUnlock = 0x20,	// 离线临时密码解锁
	DpIdOfflineDeleteCode 	= 0x21,	// 离线清空码上报
	DpIdRemoteUnlock   			= 0x32, // 新免密远程开门-带秘钥

}WifiDpId_t;



typedef enum
{
	RAW,		// 数据透传 长度N
	BOOL,		// value范围：0x00/0x01 长度1
	VALUE,	// 对应int型 长度4
	STRING,	// 字符串 长度N
	ENUM,		// 枚举 0~255 长度1
	BITMAP,	// 长度1/2/4

}WifiDataType_t;


#ifdef Function_TuyaWifi
#define WIFIPowerOpened 	1    
#define WIFIPowerClosed		0
#define Def_RemoteUnlockCountdown	30	//倒计时30s
extern status_t IfWifiIsLinked(void);
extern void WifiRemoteUnlockCountdown(void);
extern void WifiRemoteUnlockStop(void);
extern void Wifi_Mgr_Task(void);
extern void Wifi_Rst(void);
extern void Wifi_Init(void);
extern void Wifi_DeInit(void);
#endif

#endif