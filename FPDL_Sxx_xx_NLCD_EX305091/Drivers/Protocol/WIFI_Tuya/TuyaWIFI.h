#ifndef _TuyaWIFI_h_
#define _TuyaWIFI_h_

#include "Project.h"
#include "StdTypes.h"

//=============================================================================
//֡���ֽ�˳��
//=============================================================================
#define         HEAD_FIRST                      0
#define         HEAD_SECOND                     1        
#define         PROTOCOL_VERSION                2
#define         FRAME_TYPE                      3
#define         LENGTH_HIGH                     4
#define         LENGTH_LOW                      5
#define         DATA_START                      6

//=============================================================================
//����֡����
//=============================================================================
#define         PRODUCT_INFO_CMD                0x01                            //��Ʒ��Ϣ
#define         WIFI_STATE_CMD                  0x02                            //wifi����״̬	
#define         WIFI_RESET_CMD                  0x03                            //����wifi
#define         WIFI_MODE_CMD                   0x04                            //ѡ��smartconfig/APģʽ
#ifdef STATE_FAST_UPLOAD_ENABLE
#define         STATE_UPLOAD_CMD                0x18                            //ʵʱ״̬�����ϱ�
#define         STATE_RC_UPLOAD_CMD             0x19                            //��¼��״̬�����ϱ�
#else
#define         STATE_UPLOAD_CMD                0x05                            //ʵʱ״̬�ϱ�
#define         STATE_RC_UPLOAD_CMD             0x08                            //��¼��״̬�ϱ�
#endif
#define         GET_LOCAL_TIME_CMD              0x06                            //��ȡ����ʱ��
#define         WIFI_TEST_CMD                   0x07                            //wifi���ܲ���
#define         DATA_QUERT_CMD                  0x09                            //�����·�
#define         WIFI_UG_REQ_CMD                 0x0a                            //MCU����WIFIģ��̼�����
#define         ROUTE_RSSI_CMD                  0x0b                            //��ѯ��ǰ����·�����ź�ǿ��
#define         MCU_UG_REQ_CMD                  0x0c                            //MCU����MCU�̼�����
#define         UPDATE_START_CMD                0x0d                            //MCU������ʼ
#define         UPDATE_TRANS_CMD                0x0e                            //MCU��������
#define         GET_GL_TIME_CMD                 0x10                            //��ȡ����ʱ��
#define         GET_TMP_PWD_CMD           			0x14                            //�����ƶ���ʱ����
#define         GET_DP_CACHE_CMD                0x15                            //��ȡdp����ָ��
#define         GET_TMP_PWD_OFFLINE_CMD         0x16                            //�������߶�̬����
#define         REPORTED_MCU_SN_CMD             0x17                            //�ϱ�MCU��SN
#ifdef LOCK_KEEP_ALIVE
#define         GET_WIFI_STATE_CMD              0x1A                            //��ȡWiFi״̬
#endif
#define         WIFI_RESET_NOTICE_CMD           0x25                            //ģ������״̬֪ͨ



//=============================================================================
#define MCU_RX_VER              0x00                                            //MCU����Э��汾��
#define MCU_TX_VER              0x00                                            //MCU����Э��汾��
#define PROTOCOL_HEAD           0x07                                            //�̶�Э��ͷ����
#define FIRM_UPDATA_SIZE        256                                             //��������С
#define FRAME_FIRST             0x55
#define FRAME_SECOND            0xaa

#define PowerLevelHigh          0x00
#define PowerLevelMedium        0x01
#define PowerLevelLow           0x02
#define PowerLevelEmpty         0x03

typedef enum
{
	DpIdFingerUnlock				=	0x01,	// ָ�ƽ���
	DpIdPasswordUnlock			=	0x02,	// ��ͨ�������
	DpIdTMPPWDUnlock				=	0x03,	// ��ʱ�������
	DpIdCardUnlock    			=	0x05,	// ��Ƭ����
	DpIdAlarm								= 0x08,	// �澯
	DpIdAPPUnlock						= 0x09,	// APP����
	DpIdBatteryLevel				= 0x0b,	// ��ص���
	DpIdFingerSync 					= 0x19,	// ָ��ͬ��
	DpIdPasswordSync 				= 0x1a,	// ����ͬ��
	DpIdCardSync 						= 0x1b,	// ��Ƭͬ��
	DpIdOfflineTmpPWDUnlock = 0x20,	// ������ʱ�������
	DpIdOfflineDeleteCode 	= 0x21,	// ����������ϱ�
	DpIdRemoteUnlock   			= 0x32, // ������Զ�̿���-����Կ

}WifiDpId_t;



typedef enum
{
	RAW,		// ����͸�� ����N
	BOOL,		// value��Χ��0x00/0x01 ����1
	VALUE,	// ��Ӧint�� ����4
	STRING,	// �ַ��� ����N
	ENUM,		// ö�� 0~255 ����1
	BITMAP,	// ����1/2/4

}WifiDataType_t;


#ifdef Function_TuyaWifi
#define WIFIPowerOpened 	1    
#define WIFIPowerClosed		0
#define Def_RemoteUnlockCountdown	30	//����ʱ30s
extern status_t IfWifiIsLinked(void);
extern void WifiRemoteUnlockCountdown(void);
extern void WifiRemoteUnlockStop(void);
extern void Wifi_Mgr_Task(void);
extern void Wifi_Rst(void);
extern void Wifi_Init(void);
extern void Wifi_DeInit(void);
#endif

#endif