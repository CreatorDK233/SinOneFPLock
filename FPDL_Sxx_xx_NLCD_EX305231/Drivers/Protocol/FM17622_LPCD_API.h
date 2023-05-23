#ifndef _FM17622_LPCD_API_H
#define _FM17622_LPCD_API_H
#include "StdTypes.h"

#define FM17622_LPCD_ENABLE		1
#define FM17622_LPCD_DISABLE		0


#define	LPCD_THRSH_H         0x00//µÕπ¶∫ƒ—∞ø®¡È√Ù∂»
#define	LPCD_THRSH_L         0x10//µÕπ¶∫ƒ—∞ø®¡È√Ù∂»
#define LPCD_CWP     		     4   //LPCD PMOS ‰≥ˆ«˝∂Ø 0~63
#define	LPCD_CWN    		     4   //LPCD NMOS ‰≥ˆ«˝∂Ø 0~15
#define	LPCD_SLEEPTIME       16  //LPCD ªΩ–—º‰∏Ù ±º‰£¨√ø“ªµµŒ™32ms£¨–›√ﬂ ±º‰£∫(16+1)*32=544ms




//============================================================================
#define		JREG_EXT_REG_ENTRANCE		     0x0F	//ext register entrance
//============================================================================
#define		JBIT_EXT_REG_WR_ADDR		     0X40	//wrire address cycle
#define		JBIT_EXT_REG_RD_ADDR		     0X80	//read address cycle
#define		JBIT_EXT_REG_WR_DATA		     0XC0	//write data cycle
#define		JBIT_EXT_REG_RD_DATA		     0X00	//read data cycle

//============================================================================
#define   JREG_LPCDDELTA_HI            0x26
//============================================================================

//============================================================================
#define   JREG_LPCDDELTA_LO            0x27
//============================================================================

//============================================================================
#define   JREG_LPCDAUX                 0x30
//============================================================================
#define   IBN2U                        0x00
#define   TEST_BG                      0x01
#define   TEST_SAMPLE_I                0x02
#define   TEST_AMP_OUT_IN              0x03
#define   TEST_AMP_OUT_IP              0x04
#define   TEST_AMP_OUT_QN              0x05
#define   TEST_AMP_OUT_QP              0x06
#define   OSC_64K                      0x07
#define   VBN2                         0x08
#define   VBN1                         0x09
#define   TEST_BG_VREF                 0x0A
#define   AVSS                         0x0B
#define   TEST_SAMPLE_Q                0x0C
#define   DVDD                         0x0D
#define   TEST_CRYSTAL_VDD             0x0E
#define   AVDD                         0x0F
#define   FLOAT_IN                     0x10
//============================================================================
#define   JREG_LPCDSLEEPTIMER          0x33
//============================================================================

//============================================================================
#define   JREG_LPCDTHRESH_H            0x34
//============================================================================

//============================================================================
#define   JREG_LPCDTHRESH_L            0x35
//============================================================================

//============================================================================
#define   JREG_LPCDREQATIMER           0x37
//============================================================================
#define   LPCD_REQA_TIME_80us          0x00
#define   LPCD_REQA_TIME_100us         0x01
#define   LPCD_REQA_TIME_120us         0x02
#define   LPCD_REQA_TIME_150us         0x03
#define   LPCD_REQA_TIME_200us         0x04
#define   LPCD_REQA_TIME_250us         0x05
#define   LPCD_REQA_TIME_300us         0x06
#define   LPCD_REQA_TIME_400us         0x07
#define   LPCD_REQA_TIME_500us         0x08
#define   LPCD_REQA_TIME_600us         0x09
#define   LPCD_REQA_TIME_800us         0x0A
#define   LPCD_REQA_TIME_1ms           0x0B
#define   LPCD_REQA_TIME_1ms2          0x0C
#define   LPCD_REQA_TIME_1ms6          0x0D
#define   LPCD_REQA_TIME_2ms           0x0E
#define   LPCD_REQA_TIME_2ms5          0x0F
#define   LPCD_REQA_TIME_3ms           0x10
#define   LPCD_REQA_TIME_3ms5          0x11
#define   LPCD_REQA_TIME_4ms           0x12
#define   LPCD_REQA_TIME_5ms           0x13
#define   LPCD_REQA_TIME_7ms           0x14

//============================================================================
#define   JREG_LPCDREQAANA             0x38
//============================================================================
#define   LPCD_RXGAIN_23DB             0x00
#define   LPCD_RXGAIN_33DB             0x10                //default
#define   LPCD_RXGAIN_38DB             0x20
#define   LPCD_RXGAIN_43DB             0x30

#define   LPCD_MINLEVEL_3              0x00
#define   LPCD_MINLEVEL_6              0x04
#define   LPCD_MINLEVEL_9              0x08
#define   LPCD_MINLEVEL_C              0x0C

#define   LPCD_MODWIDTH_32             0x00
#define   LPCD_MODWIDTH_38             0x02

//============================================================================
#define   JREG_LPCDDETECTMODE          0x39
//============================================================================
#define   LPCD_TXSCALE_0               0x00                  //1/8 of the TX power configured by CWGSP/CWGSNON
#define   LPCD_TXSCALE_1               0x08                  //1/4 of the TX power configured by CWGSP/CWGSNON
#define   LPCD_TXSCALE_2               0x10                  //1/2 of the TX power configured by CWGSP/CWGSNON
#define   LPCD_TXSCALE_3               0x18                  //3/4 of the TX power configured by CWGSP/CWGSNON
#define   LPCD_TXSCALE_4               0x20                  //equal to of the TX power configured by CWGSP/CWGSNON
#define   LPCD_TXSCALE_5               0x28                  //twice of the TX power configured by CWGSP/CWGSNON
#define   LPCD_TXSCALE_6               0x30                  //3 times of the TX power configured by CWGSP/CWGSNON
#define   LPCD_TXSCALE_7               0x31                  //4 times of the TX power configured by CWGSP/CWGSNON

#define   LPCD_RX_CHANGE_MODE          0x00
#define   LPCD_REQA_MODE               0x01
#define   LPCD_COMBINE_MODE            0x02

//============================================================================
#define   JREG_LPCDCTRLMODE            0x3A
//============================================================================
#define   RF_DET_ENABLE                0x20
#define   RF_DET_DISABLE               0x00

#define   RF_DET_SEN_00                0x00 //∏ﬂ¡È√Ù
#define   RF_DET_SEN_01                0x08
#define   RF_DET_SEN_10                0x10
#define   RF_DET_SEN_11                0x18 //µÕ¡È√Ù

#define   LPCD_DISABLE                 0x00
#define   LPCD_ENABLE                  0x02

//============================================================================
#define   JREG_LPCDIRQ                 0x3B
//============================================================================

//============================================================================
#define   JREG_LPCDRFTIMER             0x3C
//============================================================================
#define   LPCD_IRQINV_ENABLE           0x20
#define   LPCD_IRQINV_DISABLE          0x00

#define   LPCD_IRQ_PUSHPULL            0x10
#define   LPCD_IRQ_OD                  0x00

#define   LPCD_RFTIME_5us              0x00
#define   LPCD_RFTIME_10us             0x01
#define   LPCD_RFTIME_15us             0x02
#define   LPCD_RFTIME_20us             0x03
#define   LPCD_RFTIME_25us             0x04                  //default
#define   LPCD_RFTIME_30us             0x05
#define   LPCD_RFTIME_35us             0x06
#define   LPCD_RFTIME_40us             0x07
#define   LPCD_RFTIME_50us             0x08
#define   LPCD_RFTIME_60us             0x09
#define   LPCD_RFTIME_70us             0x0A
#define   LPCD_RFTIME_80us             0x0B
#define   LPCD_RFTIME_100us            0x0C
#define   LPCD_RFTIME_120us            0x0D
#define   LPCD_RFTIME_150us            0x0E
#define   LPCD_RFTIME_200us            0x0F

//============================================================================
#define   JREG_LPCDTXCTRL1             0x3D
//============================================================================
#define   LPCD_TX2_ENABLE              0x20
#define   LPCD_TX2_DISABLE             0x00

#define   LPCD_TX1_ENABLE              0x10
#define   LPCD_TX1_DISABLE             0x00

#define   LPCD_TX2ON_INV_ENABLE        0x08
#define   LPCD_TX2ON_INV_DISABLE       0x00

#define   LPCD_TX1ON_INV_ENABLE        0x04
#define   LPCD_TX1ON_INV_DISABLE       0x00

#define   LPCD_TX2OFF_INV_ENABLE       0x02
#define   LPCD_TX2OFF_INV_DISABLE      0x00

#define   LPCD_TX1OFF_INV_ENABLE       0x01
#define   LPCD_TX1OFF_INV_DISABLE      0x00

//============================================================================
#define   JREG_LPCDTXCTRL2             0x3E
//============================================================================

//============================================================================
#define   JREG_LPCDTXCTRL3             0x3F
//============================================================================

extern status_t Lpcd_Init_Register(void);
extern void Lpcd_ClearIrq(void);

void Lpcd_IRQ_Event(void);
unsigned char Lpcd_Card_Event(void);
void Lpcd_Get_ADC_Value(void);

#endif


