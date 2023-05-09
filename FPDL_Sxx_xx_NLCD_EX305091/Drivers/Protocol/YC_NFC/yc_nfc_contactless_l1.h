/**
  ******************************************************************************
  * @file    yc_NFC_contactless_l1.h
  * @author  YiChip Application Team
  * @version V2.6
  * @date    10-May-2019
  * @brief   This file contains all the functions prototypes for NFC 2.6
  *                      contactless level 1 interface
  ******************************************************************************
  */

#ifndef _YC_NFC_CONTACTLESS_L1_H_
#define _YC_NFC_CONTACTLESS_L1_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "StdTypes.h"
#include "I2cBus.h"
#include "Project.h"


/* Modulation index */
#define MOD_INDEX       0x10
/* TX POWER*/
#define PA_GAIN_VAL     0x1F
/*RX MINLEVEL*/
#define RX_MINLEVEL     0X18A

//#define READ_RxVpp

/* Interface to configure the registers of YC5018 */
extern void WRITEREG(unsigned char addr, unsigned char data_m);
extern uint8_t READREG(unsigned char addr);

/****************************** yc5018 reg table ***************************/
#ifndef __YC5018_REG_TABLE_H_
#define __YC5018_REG_TABLE_H_
#define REG_PAGE_SELECT                 0
#define REG_TOP_CTRL                    1
#define REG_COMMAND                     2
#define REG_FIFO_CTRL                   3
#define REG_FIFO_LENGTH                 4
#define REG_WATER_LEVEL                 5
#define REG_FIFO_DATA                   6
#define REG_IRQ0_STATE                  7
#define REG_IRQ1_STATE                  8
#define REG_IRQ0_CLEAR                  9
#define REG_IRQ1_CLEAR                  10
#define REG_IRQ0_EN                     11
#define REG_IRQ1_EN                     12
#define REG_ERROR_FLAG                  13
#define REG_PROTOCOL_ERROR              14 
#define REG_T_CTRL                      15
#define REG_T_PRESCAL_HI                16
#define REG_T_PRESCAL_LO                17
#define REG_T_RELOAD_HI                 18
#define REG_T_RELOAD_LO                 19
#define REG_T_VALUE_HI                  20
#define REG_T_VALUE_LO                  21
#define REG_TX_WAIT_CTRL                22
#define REG_TX_WAIT_FREQ                23
#define REG_TX_WAIT                     24
#define REG_TX_CODER_CTRL               25
#define REG_TX_CRC_CTRL                 26
#define REG_MOD_WIDTH                   27
#define REG_TX_TYPE_B_FRAMING           28
#define REG_FRAM_CON                    29
#define REG_RX_WAIT                     30
#define REG_RX_WAIT_FREQ                31
#define REG_RX_CODER_CTRL               32
#define REG_RX_CRC_CTRL                 33
#define REG_RX_BIT_CTRL                 34
#define REG_RX_COLL                     35
#define REG_RX_TYPE_B_FRAMING           36//数据手册无描述
#define REG_RX_TOLERANCE                37
#define REG_RX_LAST_BITS                38
#define REG_EMD_CONDITION_MASK0         39
#define REG_EMD_CONDITION_MASK1         40
#define REG_EMD_STATUS0                 41
#define REG_EMD_STATUS1                 42
#define REG_EMD_FRAME_LENGTH            43
#define REG_TX_FALL_GAIN0               44
#define REG_TX_FALL_GAIN1               45
#define REG_TX_FALL_GAIN2               46
#define REG_TX_FALL_GAIN3               47
#define REG_TX_FALL_GAIN4               48
#define REG_TX_FALL_GAIN5               49
#define REG_TX_FALL_GAIN6               50
#define REG_TX_FALL_GAIN7               51
#define REG_TX_RISE_GAIN0               52
#define REG_TX_RISE_GAIN1               53
#define REG_TX_RISE_GAIN2               54
#define REG_TX_RISE_GAIN3               55
#define REG_TX_RISE_GAIN4               56
#define REG_TX_RISE_GAIN5               57
#define REG_TX_RISE_GAIN6               58
#define REG_TX_RISE_GAIN7               59
#define REG_DEMOD_CTRL                  60//数据手册无描述
#define REG_DEMOD_TYPEA_CTRL            61
#define REG_MIN_LEVEL_LOW               62
#define REG_MIN_LEVEL_HIGH              63
#define REG_COLL_LEVEL                  64
#define REG_RSSI_CALC_CONFIG            65
#define REG_RSSI_CALC_CTRL              66
#define REG_AVG_POW                     67
#define REG_AVG_I                       68
#define REG_AVG_Q                       69
#define REG_DC_I                        70
#define REG_DC_Q                        71
#define REG_PMU_STATE                   72
#define REG_CTRL_STATE                  73
#define REG_DEMOD_STATE                 74
#define REG_TOP_CTRL2                   80
#define REG_WUC_PERIOD                  81
#define REG_CLK_CTRL                    82
#define REG_ADC_CTRL                    83
#define REG_ADC_VALUE_I                 84
#define REG_ADC_VALUE_Q                 85
#define REG_CAP_CTRL1                   86
#define REG_CAP_CTRL2                   87
#define REG_DEBUG_CTRL                  88
#define REG_PIN_MUX_CTRL                89
#define REG_PIN_PE_CTRL                 90
#define REG_METAL_FIX_CTRL              91
#define REG_METAL_FIX_REG_13P56         92
#define REG_METAL_FIX_REG_32K           93
#define REG_RESERVED_REG                94
#define REG_GPIO_DS_CTRL                95
#define REG_TX_PA_GAIN                  97
#define REG_TX_PA_MOD_GAIN              98
#define REG_TX_PA_GAIN_CTRL             99 //数据手册无描述
#define REG_XTAL_STABLE_TIME            104
#define REG_MODEM_STATE_FORCE_MODE      105
#define REG_LPCD0_CNT_TIME              106
#define REG_LPCD0_CNT_THRESHOLD_HIGH    107
#define REG_LPCD0_CNT_THRESHOLD_LOW     108
#define REG_LPCD1_PA_SETTLE_TIME        109
#define REG_LPCD1_DC_CALC_TIME          110
#define REG_LPCD1_DC_THRESHOLD          111
#define REG_LDO_CORE_LPO_CLK_CTRL       129//数据手册无描述
#define REG_LDO_ADC_CLKGEN_LPO_CTRL     130//数据手册无描述
#define REG_LDO_RX_LPCD_LPO_CTRL        131
#define REG_LPO_XTAL_CTRL               132
#define REG_XTAL_CTRL0                  133
#define REG_XTAL_CTRL1                  134
#define REG_XTAL_CTRL2                  135
#define REG_XTAL_CTRL3                  136
#define REG_XTAL_CTRL4                  137
#define REG_ADC_CTRL0                   138
#define REG_ADC_CTRL1                   139
#define REG_ADC_CTRL2                   140
#define REG_ADC_CTRL3                   141
#define REG_ADC_CRTL4                   142
#define REG_BBPLL_CTRL0                 143
#define REG_BBPLL_CTRL1                 144
#define REG_BBPLL_CTRL2                 145
#define REG_BBPLL_CTRL3                 146
#define REG_CLKGEN_CTRL                 147
#define REG_RX_CTRL0                    148
#define REG_RX_CTRL1                    149
#define REG_RX_CTRL2                    150
#define REG_RX_CTRL3                    151//数据手册无描述
#define REG_RX_CTRL4                    152//数据手册无描述
#define REG_RX_CTRL5                    153//数据手册无描述
#define REG_RX_TEST_CTRL                154
#define REG_TEST_PA_CTRL                155//数据手册无描述
#define REG_DA_RX_FLT_GRCK_EN           171//数据手册无描述
#define REG_DA_TX_PA_EN                 174//数据手册无描述

#endif /*__YC5018_REG_TABLE_H_*/

#define TX_BAUD_RATE_106        0
#define TX_BAUD_RATE_212        1
#define TX_BAUD_RATE_424        2
#define TX_BAUD_RATE_848        3

#define RX_BAUD_RATE_106        0
#define RX_BAUD_RATE_212        1
#define RX_BAUD_RATE_424        2
#define RX_BAUD_RATE_848        3

/* Error code reported to terminal */
#define NFC_ERR_NONE                    0
#define NFC_ERR_PARA                    1
#define NFC_ERR_NO_CARD                 2
#define NFC_ERR_COLLISION               3
#define NFC_ERR_TIME_OUT                4
#define NFC_ERR_TRANSMISSION            5
#define NFC_ERR_PROTOCOL                6
#define NFC_ERR_OVERFLOW                7
#define EOT_IND_REMOVE                  8
#define EOT_IND_POWER_OFF               9
#define EOT_SHOUT                       10
#define NFC_IDENTITY_CARD		        11
/*RX WAIT TIME*/
#define	WAIT_ATQA_PRESCALER    0
#define	WAIT_ATQA_RELOAD       2400   //176us  

#define	WAIT_ATQB_PRESCALER    0
#define	WAIT_ATQB_RELOAD       21240

#define RX_CTRL3        0x3D
#define RX_CTRL4        0x0B
#define RX_CTRL5        0x7C

#define RX_GAIN_6DB         ((0<<3) | (0<<5))
#define RX_GAIN_12DB        ((1<<3) | (0<<5))
#define RX_GAIN_18DB        ((2<<3) | (0<<5))
#define RX_GAIN_24DB        ((3<<3) | (0<<5))
#define RX_GAIN_30DB        ((3<<3) | (1<<5))
#define RX_GAIN_36DB        ((3<<3) | (2<<5))
#define RX_GAIN_42DB        ((3<<3) | (3<<5))
#define RX_GAIN_48DB        ((3<<3) | (4<<5))
#define RX_GAIN_54DB        ((3<<3) | (5<<5))

#define RX_I_R_250                      0
#define RX_I_R_500                      1
#define RX_I_R_1K                       2
#define RX_I_R_2K                       3
#define RX_I_R_OPEN                     4

#define Tp()                            Soft_Delay1ms(6)
#define Tretransmission()               Soft_Delay1ms(8)
#define Tresetdelay()                   Soft_Delay1ms(10)
#define Tpoweroff()                     Soft_Delay1ms(20)

#define FSD             256
#define FSC_CHAINING    256

#define IDLE            0x00
#define RECEIVE         0x01
#define TRANSMIT        0x02
#define TRANSCEIVE      0x03

#define I_BLOCK         0
#define R_BLOCK         2
#define S_BLOCK         3

#define BIT0            0x01
#define BIT1            0x02
#define BIT2            0x04
#define BIT3            0x08
#define BIT4            0x10
#define BIT5            0x20
#define BIT6            0x40
#define BIT7            0x80


typedef enum
{
	CMD_MAUTH1,
	CMD_MTRANSFER,	
	CMD_M0WRITE,
	CMD_M0READ,
	CMD_MAUTH2,
	CMD_WUPA,
	CMD_REQA,
	CMD_ANTICOLLISION,
	CMD_SELECT,
	CMD_RATS,
	CMD_HLTA,
	CMD_WUPB,
	CMD_REQB,
	CMD_ATTRIB,
	CMD_HLTB,
	CMD_IBLOCK,
	CMD_SBLOCK,
	CMD_RBLOCK
} Cmd_t;

typedef struct
{
    unsigned char UID_CLn[3][5];
} YC_NFC_Contactless_L1_PICC_A_pcb_t;
extern YC_NFC_Contactless_L1_PICC_A_pcb_t picc_a;


typedef struct
{
    unsigned char PUPI[4];
} YC_NFC_Contactless_L1_PICC_B_pcb_t;

extern  YC_NFC_Contactless_L1_PICC_B_pcb_t picc_b;

typedef struct
{
	uint8_t Type_A;
	uint8_t Type_B;
} NFC_Poll_Type_t;


extern NFC_Poll_Type_t Poll_Type;


#define FIFO_LENGTH     64
#define WATER_LEVEL     32

#define CALI_OK         0
#define CALI_FAIL       1
#define DC_ACCEPT_RANGE 3

#define WIN_SIZE_32     0
#define WIN_SIZE_64     1
#define WIN_SIZE_128    2
#define WIN_SIZE_256    3

#define get_dc_i()              READREG(REG_AVG_I)
#define get_dc_q()              READREG(REG_AVG_Q)
#define get_cali_i()            READREG(REG_RX_CTRL1)
#define get_cali_q()            READREG(REG_RX_CTRL2)
#define set_cali_i(rval)        WRITEREG(REG_RX_CTRL1, rval)
#define set_cali_q(rval)        WRITEREG(REG_RX_CTRL2, rval)

//#if 1
//#define PRINT(format,...) MyPrintf(format,##__VA_ARGS__)
//#else
//#define PRINT(format,...)       ((void)0)
//#endif

#define TYPE_A   0
#define TYPE_B   1
#define TYPEA_M1  2
#define TYPEB_IDENTITY  3

#define ON      1
#define OFF     0

#define CL1     0x93
#define CL2     0x95
#define CL3     0x97

/**
  * @brief  Common initialization
  * @param  reg_config: used for external register configuration
  *             @note if not use this value, can input NULL
  * @retval None
  */
void YC_NFC_Contactless_Init(void);

/**
  * @brief      Individual initialization
  * @param  picc_type: specifies which kind of PICC to communicate
  *   This parameter can be one of the values:
  *     @arg TYPE_A: initialize PCD to communicate with type A PICC
  *     @arg TYPE_B: initialize PCD to communicate with type B PICC
  * @retval None
  */
void YC_NFC_Contactless_Switch_Card_Type(unsigned char picc_type);

/**
  * @brief  RF switch on or off
  * @param  on: specifies which switch on or switch off Operating Field
  *   This parameter can be one of the values:
  *     @arg ON:  switch on Operating Field
  *     @arg OFF: switch off Operating Field
  * @retval None
  */
void YC_NFC_Contactless_Rf_Switch(unsigned char on);

/**
  * @brief  WUPA
  * @param  None
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_WUPA(void);

/**
  * @brief  REQA
  * @param  None
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_REQA(void);


/**
  * @brief  Type A Collision Detection
  *
  * @param  None
  * @retval Error code
  */
unsigned char YC_NFC_A_Collision_Detect(void);

/**
  * @brief  ANTICOLLISION
  * @param  cl�� specifies cascade level
  *   This parameter can be one of the values:
  *     @arg CL1: cascase level 1
  *     @arg CL2: cascase level 2
  *     @arg CL2: cascase level 3
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_ANTICOLLISION(unsigned char cl);

/**
  * @brief  SELECT
  * @note   When UID is complete, then check if PICC is compliant with ISO/IEC 14443-4
  * @param  cl�� specifies cascade level
  *   This parameter can be one of the values:
  *     @arg CL1: cascase level 1
  *     @arg CL2: cascase level 2
  *     @arg CL2: cascase level 3
  * @param  UID_complete:
  *                       1: UID not complete
  *                       0: UID complete
  *     @note if not use this value, can input NULL
  * @param  ISOIEC14443_4_compliant:
  *           1: PICC compliant with ISO/IEC 14443-4
  *           0: PICC not compliant with ISO/IEC 14443-4
  *     @note if not use this value, can input NULL
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_SELECT(unsigned char cl,
                unsigned char *UID_complete,
                unsigned char *ISOIEC14443_4_compliant);

/**
  * @brief  RATS
  * @param  None
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_RATS(void);

/**
  * @brief  HLTA
  * @param  None
  * @retval None
  */
void YC_NFC_Contactless_HLTA(void);

/**
  * @brief  WUPB
  * @param  ISOIEC14443_4_compliant:
  *           1: PICC compliant with ISO/IEC 14443-4
  *           0: PICC not compliant with ISO/IEC 14443-4
  *     @note if not use this value, can input NULL
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_WUPB(unsigned char *ISOIEC14443_4_compliant);

/**
  * @brief  REQB
  * @param  ISOIEC14443_4_compliant:
  *           1: PICC compliant with ISO/IEC 14443-4
  *           0: PICC not compliant with ISO/IEC 14443-4
  *     @note if not use this value, can input NULL
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_REQB(unsigned char *ISOIEC14443_4_compliant);

/**
  * @brief  Type B Collision Detection
  *
  * @param  None
  * @retval Error code
  */
unsigned char YC_NFC_B_Collision_Detect(void);

/**
  * @brief  ATTRIB
  * @param  None
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_ATTRIB(void);

/**
  * @brief  HLTB
  * @param  None
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_HLTB(void);

/**
  * @brief  APDU processing
  * @param  inf: APDU
  * @param  inflen: APDU length
  * @param  res: APDU response
  * @param  reslen: APDU response length
  * @retval Error code
  */
unsigned char YC_NFC_Contactless_APDU_Transceive(const unsigned char *inf,
                uint16_t inflen,
                unsigned char *res,
                uint16_t *reslen);

/**
  * @brief  Enter low power mode
  * @param  None
  * @retval None
  */
void YC_NFC_Contactless_Sleep(void);

/**
  * @brief  Exit low power mode
  * @param  None
  * @retval None
  */
void YC_NFC_Contactless_Wake(void);

/**
  * @brief  Set carrier power
  * @param  PaGain : 0~0xff
  * @retval None
  */
void Set_Carrier_Strength(unsigned char PaGain);

/**
  * @brief  Set receive gain
  * @param  RxGain :    #define RX_GAIN_6DB
                                                #define RX_GAIN_12DB
                                                #define RX_GAIN_18DB
                                                #define RX_GAIN_24DB
                                                #define RX_GAIN_30DB
                                                #define RX_GAIN_36DB
                                                #define RX_GAIN_42DB
                                                #define RX_GAIN_48DB
                                                #define RX_GAIN_54DB

  * @param  RfAtt :    #define RX_I_R_250
                                           #define RX_I_R_500
                                           #define RX_I_R_1K
                                           #define RX_I_R_2K
                                           #define RX_I_R_OPEN
  * @retval None
  */
void Set_Receive_Parameters(unsigned char RxGain,unsigned char RfAtt);

/**
  * @brief  Set Threshold
  * @param  MinLevel : 0x120~0x300
  * @retval None
  */
void Set_Min_Level(uint16_t MinLevel);

/**
  * @brief Interface of transceiving.
  * @param cmd: command
  * @param request: data sent to PICC
  * @param requestlen: length of data sent to PICC
  * @param txalign: number of bit of last byte sent to PICC
  * @param response: data received from PICC
  * @param responselen: length of data received from PICC
  * @param rxalign: the first bit received
  * @retval Error code
  */

unsigned char NFC_Transceive(Cmd_t cmd,
                                    const unsigned char *request, uint16_t requestlen, unsigned char txalign,
                                    unsigned char *response, uint16_t *responselen, unsigned char rxalign);

/** @brief  calibration dc*/
void set_rssi_calc_en(uint8_t en);
void set_rx_gain(uint8_t gn);
uint8_t dc_cali_i(void);
uint8_t dc_cali_q(void);
void force_dc_calc(void);
void DC_cali_alg(uint8_t win_size);
void nfctimer0_enable(unsigned long num);
unsigned char YC_NFC_HwReset(void);
unsigned char  Aset_read_power_xRSSI_RXVPP(void);
#ifdef Function_NFCUsed_YC5018
status_t YC_NFC_M_Code(uint8_t *Point);
extern void MFC_POWERDOWN(void);
extern void MFC_WAKEUP(void);
extern status_t MFC_Auto_Reader(uint8_t *Point);
extern void MFC_Init(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* _YC_NFC_CONTACTLESS_L1_H_ */

/******************* (C) COPYRIGHT 2019 YiChip Microelectronics *******************/
