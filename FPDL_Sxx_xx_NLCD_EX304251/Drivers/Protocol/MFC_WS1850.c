#include "IO.h"
#include "I2cBus.h"
#include "StdTypes.h"
#include "Basic_Function.h"

#ifdef Function_NFCUsed_1850X
#define MAXRLEN        18
#define MIN_STRENGTH  228
//******************************************************************/
//                    RC522 FIFO长度定义                            /
//******************************************************************/
#define DEF_FIFO_LENGTH             64           //FIFO size=64byte

//******************************************************************/
//                       RC522命令字                                /
//******************************************************************/
#define PCD_SLEEP                 0x10           //取消当前命令
#define PCD_IDLE                  0x00           //取消当前命令
#define PCD_AUTHENT               0x0E           //验证密钥
#define PCD_RECEIVE               0x08           //接收数据
#define PCD_TRANSMIT              0x04           //发送数据
#define PCD_TRANSCEIVE            0x0C           //发送并接收数据
#define PCD_RESETPHASE            0x0F           //复位
#define PCD_CALCCRC               0x03           //CRC计算

//******************************************************************/
//                   Mifare_One卡片命令字                          */
//******************************************************************/
#define PICC_REQIDL               0x26           //寻天线区内未进入休眠状态
#define PICC_REQALL               0x52           //寻天线区内全部卡
#define PICC_ANTICOLL1            0x93           //防冲撞
#define PICC_ANTICOLL2            0x95           //防冲撞
#define PICC_AUTHENT1A            0x60           //验证A密钥
#define PICC_AUTHENT1B            0x61           //验证B密钥
#define PICC_READ                 0x30           //读块
#define PICC_WRITE                0xA0           //写块
#define PICC_DECREMENT            0xC0           //扣款
#define PICC_INCREMENT            0xC1           //充值
#define PICC_RESTORE              0xC2           //调块数据到缓冲区
#define PICC_TRANSFER             0xB0           //保存缓冲区中数据
#define PICC_HALT                 0x50           //休眠

//******************************************************************/
//                        MF522寄存器定义                           /
//******************************************************************/
// PAGE 0
#define     RFU00                 0x00    
#define     CommandReg            0x01    
#define     ComIEnReg             0x02    
#define     DivlEnReg             0x03    
#define     ComIrqReg             0x04    
#define     DivIrqReg             0x05
#define     ErrorReg              0x06    
#define     Status1Reg            0x07    
#define     Status2Reg            0x08    
#define     FIFODataReg           0x09
#define     FIFOLevelReg          0x0A
#define     WaterLevelReg         0x0B
#define     ControlReg            0x0C
#define     BitFramingReg         0x0D
#define     CollReg               0x0E
#define     RFU0F                 0x0F
// PAGE 1     
#define     RFU10                 0x10
#define     ModeReg               0x11
#define     TxModeReg             0x12
#define     RxModeReg             0x13
#define     TxControlReg          0x14
#define     TxASKReg              0x15
#define     TxSelReg              0x16
#define     RxSelReg              0x17
#define     RxThresholdReg        0x18
#define     DemodReg              0x19
#define     RFU1A                 0x1A
#define     RFU1B                 0x1B
#define     MifareReg             0x1C
#define     RFU1D                 0x1D
#define     RFU1E                 0x1E
#define     SerialSpeedReg        0x1F
// PAGE 2    
#define     RFU20                 0x20  
#define     CRCResultRegM         0x21
#define     CRCResultRegL         0x22
#define     RFU23                 0x23
#define     ModWidthReg           0x24
#define     RFU25                 0x25
#define     RFCfgReg              0x26
#define     GsNReg                0x27
#define     CWGsCfgReg            0x28
#define     ModGsCfgReg           0x29
#define     TModeReg              0x2A
#define     TPrescalerReg         0x2B
#define     TReloadRegH           0x2C
#define     TReloadRegL           0x2D
#define     TCounterValueRegH     0x2E
#define     TCounterValueRegL     0x2F
// PAGE 3      
#define     RFU30                 0x30
#define     TestSel1Reg           0x31
#define     TestSel2Reg           0x32
#define     TestPinEnReg          0x33
#define     TestPinValueReg       0x34
#define     TestBusReg            0x35
#define     AutoTestReg           0x36
#define     VersionReg            0x37
#define     AnalogTestReg         0x38
#define     TestDAC1Reg           0x39  
#define     TestDAC2Reg           0x3A   
#define     TestADCReg            0x3B   
#define     RFU3C                 0x3C   
#define     RFU3D                 0x3D   
#define     RFU3E                 0x3E   
#define     RFU3F		  						0x3F

//******************************************************************/
//                    RC522通讯返回错误代码                         /
//******************************************************************/
#define MI_ERR                      0xFE 
//#define MI_ERR                         //(-2)


// Mifare Error Codes 
// Each function returns a status value, which corresponds to the 
// mifare error codes. 

#define MI_OK                          0 
#define MI_CHK_OK                      0 
#define MI_CRC_ZERO                    0 

#define MI_CRC_NOTZERO                 1 

#define MI_NOTAGERR                 0xFF 
#define MI_CHK_S_FAILED             0xFF 
#define MI_CRCERR                   0xFE 
#define MI_CHK_COMPERR              0xFE 
#define MI_EMPTY                    0xFD 
#define MI_AUTHERR                  0xFC 
#define MI_PARITYERR                0xFB 
#define MI_CODEERR                  0xFA 

#define MI_SERNRERR                 0xF8 
#define MI_KEYERR                   0xF7 
#define MI_NOTAUTHERR               0xF6 
#define MI_BITCOUNTERR              0xF5 
#define MI_BYTECOUNTERR             0xF4 
#define MI_IDLE                     0xF3 
#define MI_TRANSERR                 0xF2 
#define MI_WRITEERR                 0xF1 
#define MI_INCRERR                  0xF0 
#define MI_DECRERR                  0xEF 
#define MI_READERR                  0xEE 
#define MI_OVFLERR                  0xED 
#define MI_POLLING                  0xEC 
#define MI_FRAMINGERR               0xEB 
#define MI_ACCESSERR                0xEA 
#define MI_UNKNOWN_COMMAND          0xE9 
#define MI_COLLERR                  0xE8 
#define MI_RESETERR                 0xE7 
#define MI_INITERR                  0xE7 
#define MI_INTERFACEERR             0xE7 
#define MI_ACCESSTIMEOUT            0xE5 
#define MI_NOBITWISEANTICOLL        0xE4 
#define MI_QUIT                     0xE2 

#define MI_RECBUF_OVERFLOW          0xCF 
#define MI_SENDBYTENR               0xCE 

#define MI_SENDBUF_OVERFLOW         0xCC 
#define MI_BAUDRATE_NOT_SUPPORTED   0xCB 
#define MI_SAME_BAUDRATE_REQUIRED   0xCA 

#define MI_WRONG_PARAMETER_VALUE    0xC5 

#define MI_BREAK                    0x9E 
#define MI_NY_IMPLEMENTED           0x9D 
#define MI_NO_MFRC                  0x9C 
#define MI_MFRC_NOTAUTH             0x9B 
#define MI_WRONG_DES_MODE           0x9A 
#define MI_HOST_AUTH_S_FAILED         0x99 

#define MI_WRONG_LOAD_MODE          0x97 
#define MI_WRONG_DESKEY             0x96 
#define MI_MKLOAD_S_FAILED            0x95 
#define MI_FIFOERR                  0x94 
#define MI_WRONG_ADDR               0x93 
#define MI_DESKEYLOAD_S_FAILED        0x92 

#define MI_WRONG_SEL_CNT            0x8F 
#define MI_RC531_WRONG_READVALUE    0x8E //LI ADDED 09-4-24 
#define MI_WRONG_TEST_MODE          0x8C 
#define MI_TEST_S_FAILED              0x8B 
#define MI_TOC_ERROR                0x8A 
#define MI_COMM_ABORT               0x89 
#define MI_INVALID_BASE             0x88 
#define MI_MFRC_RESET               0x87 
#define MI_WRONG_VALUE              0x86 
#define MI_VALERR                   0x85
//******************************************************************/
//                         函数定义                                 /
//******************************************************************/
sint8_t PcdHalt(void);
sint8_t PcdReset(void);
void PcdAntennaOn(void);
void PcdAntennaOff(void);
sint8_t PcdRequest(uint8_t req_code,uint8_t *pTagType);   
sint8_t PcdAnticoll(uint8_t *pSnr);
sint8_t PcdSelect(uint8_t *pSnr);
sint8_t PcdAuthState(uint8_t auth_mode,uint8_t addr,
                  uint8_t *pKey,uint8_t *pSnr);     
sint8_t PcdRead(uint8_t addr,uint8_t *pData);     
sint8_t PcdWrite(uint8_t addr,uint8_t *pData);    
sint8_t PcdValue(uint8_t dd_mode,uint8_t addr,uint8_t *pValue);   
sint8_t PcdBakValue(uint8_t sourceaddr, uint8_t goaladdr);                                 
sint8_t PcdComMF522(uint8_t Command, uint8_t *pInData, 
                 uint8_t InLenByte,uint8_t *pOutData, 
                 uint16_t  *pOutLenBit);
void CalulateCRC(uint8_t *pIndata,uint8_t len,
                 uint8_t *pOutData);
void WriteRawRC(uint8_t Address,uint8_t value);
uint8_t ReadRawRC(uint8_t Address); 
void SetBitMask(uint8_t reg,uint8_t mask); 
void ClearBitMask(uint8_t reg,uint8_t mask); 
sint8_t Read_Block(uint8_t Block,uint8_t *Buf);
sint8_t Write_Block(uint8_t Block);
void PcdAntennaTestOn(void);

extern sint8_t KK[8]; // 数据加密密钥
static uint8_t RF_Buffer[18]; // 射频卡数据缓冲区
static uint8_t UID[5];
static uint8_t Password_Buffer[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//extern void Des_Encrypt(sint8_t* In, sint8_t* K,sint8_t* Out);
//extern void Des_Decrypt(sint8_t* In, sint8_t* K,sint8_t* Out);
static uint8_t des_on; // DES加密标志

static uint8_t TestVal;
static uint8_t TestBuff[20];

static uint8_t HAL_SPI2_RXBUFF;
#ifdef Function_CardReaderUseSPI
//******************************************************************/
//功    能：读RC522寄存器
//参数说明：Address[IN]:寄存器地址
//返    回：读出的值
//******************************************************************/
uint8_t ReadRawRC(uint8_t Address)
{
	uint8_t ucResult;
	HAL_StatusTypeDef HAL_STATUS;
	
	SET_MFC_CS_L;
	Hardware_DelayX1us(10);
//	Hardware_DelayMs(10);
	ucResult = (Address<<1)|0x80;
	
	HAL_STATUS = HAL_SPI_Transmit(&hspi1,&ucResult,1,10);
	HAL_STATUS = HAL_SPI_Receive(&hspi1,&HAL_SPI2_RXBUFF,1,10);
	SET_MFC_CS_H;

//	Hardware_DelayMs(10);
  Hardware_DelayX1us(10);
	return HAL_SPI2_RXBUFF;
}

//******************************************************************/
//功    能：写RC522寄存器
//参数说明：Address[IN]:寄存器地址
//          value[IN]:写入的值
//******************************************************************/
void  WriteRawRC(uint8_t Address, uint8_t value)
{
	uint8_t temp;
	HAL_StatusTypeDef HAL_STATUS;
	
	SET_MFC_CS_L;
  Hardware_DelayX1us(10);
	
	temp = Address<<1;
	HAL_STATUS = HAL_SPI_Transmit(&hspi1,&temp,1,500);
	HAL_STATUS = HAL_SPI_Transmit(&hspi1,&value,1,500);
	SET_MFC_CS_H;
  //Hardware_DelayX1us(100);
}

#else
uint8_t ReadRawRC(uint8_t Address)
{
  uint8_t ucResult;
  I2C_ReadRandom(I2CADDR_MFC,Address,&ucResult);
  return ucResult;
}
void WriteRawRC(uint8_t Address, uint8_t value)
{  
  I2C_WriteRandom(I2CADDR_MFC,Address,value);
}
#endif

//******************************************************************/
//功    能：置RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:置位值
//******************************************************************/
void SetBitMask(uint8_t reg,uint8_t mask)  
{
  sint8_t tmp = 0x00;
  tmp = ReadRawRC(reg)| mask;
  WriteRawRC(reg, tmp);  // set bit mask
}

//******************************************************************/
//功    能：清RC522寄存器位
//参数说明：reg[IN]:寄存器地址
//          mask[IN]:清位值
//******************************************************************/
void ClearBitMask(uint8_t reg,uint8_t mask)  
{
  sint8_t tmp = 0x00;
  tmp = ReadRawRC(reg)&(~mask);
  WriteRawRC(reg, tmp)        ;  // clear bit mask
} 

//******************************************************************/
//功    能：复位RC522
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t PcdReset(void)
{
	//SET_MFC_RST_H;
	//Hardware_DelayMs(1);
	SET_MFC_RST_L;
	Hardware_DelayX1us(10);
	SET_MFC_RST_H;
	Hardware_DelayX1us(50);
	
  WriteRawRC(CommandReg,PCD_RESETPHASE);
  Hardware_DelayMs(5);
	
	WriteRawRC(RFCfgReg,0x48)       ; 
  /* 增益调整 GsN前后两位独立 CWGsCfg最大0x3F ModGsCfg最大0x3F*/
	#ifdef ProjectIs_BarLock_S4914
		WriteRawRC(GsNReg,0x33) 		; 
	WriteRawRC(CWGsCfgReg,0x03) 	; 
	WriteRawRC(ModGsCfgReg,0x03)	;
	#else
	WriteRawRC(GsNReg,0xFF); 
	WriteRawRC(CWGsCfgReg,0x3F); 
	WriteRawRC(ModGsCfgReg,0x3F); 
	#endif
	
  WriteRawRC(ModeReg,0x3D);
  WriteRawRC(TReloadRegL,30);
  WriteRawRC(TReloadRegH,0);
  
  WriteRawRC(TModeReg,0x8D);
  WriteRawRC(TPrescalerReg,0x3E);  
		
//  WriteRawRC(TxASKReg,0x40)            ; // FOR DEBUG AND TEST
  return MI_OK; 
}

//******************************************************************/
//开启天线发射  
//每次启动或关闭天险发射之间应至少有1ms的间隔
//******************************************************************/
void PcdAntennaOn(void)
{
  //uint8_t i;
	/*
  WriteRawRC(TxASKReg,0x40)       ;
  Hardware_DelayMs(3)            ;
  i = ReadRawRC(TxControlReg)     ;
  if(!(i&0x03))
    SetBitMask(TxControlReg, 0x03);
  i=ReadRawRC(TxASKReg)       ;
	*/
	WriteRawRC(TxASKReg,0x40)       ;
	SetBitMask(TxControlReg, 0x03);
}

//******************************************************************/
//开启天线发射  
//每次启动或关闭天险发射之间应至少有1ms的间隔
//******************************************************************/
void PcdAntennaTestOn(void)
{
//*
  SET_MFC_RST_H                         ;
  Hardware_DelayMs(15); // 2010.10.09 ???? FOR DEBUG
  
  WriteRawRC(TxControlReg,0x02)    ;
}


//******************************************************************/
//关闭天线发射
//******************************************************************/
void PcdAntennaOff(void)
{
	//return;
  	ClearBitMask(TxControlReg, 0x03);
}

//******************************************************************/
//功    能：通过RC522和ISO14443卡通讯
//参数说明：Command[IN]:RC522命令字
//          pInData[IN]:通过RC522发送到卡片的数据
//          InLenByte[IN]:发送数据的字节长度
//          pOutData[OUT]:接收到的卡片返回数据
//          *pOutLenBit[OUT]:返回数据的位长度
//******************************************************************/
sint8_t PcdComMF522(uint8_t Command  ,uint8_t *pInData , 
                 uint8_t InLenByte,uint8_t *pOutData, 
                 uint16_t  *pOutLenBit                       )
{
  sint8_t status = MI_ERR                          ;
  uint8_t irqEn   = 0x00                  ;
  uint8_t waitFor = 0x00                  ;
  uint8_t lastBits                        ;
  uint8_t n                               ;
  uint16_t  i                               ;
  switch (Command)
  {
    case PCD_AUTHENT:
      irqEn   = 0x12                            ;
      waitFor = 0x10                            ;
      break                                     ;
    case PCD_TRANSCEIVE:
      irqEn   = 0x20;//0x77                            ;
      waitFor = 0x30                            ;
      break                                     ;
    default:
      break                                     ;
  }
  WriteRawRC(ComIEnReg,irqEn|0x80)              ; //
  ClearBitMask(ComIrqReg,0x80)                  ;
  WriteRawRC(CommandReg,PCD_IDLE)               ;
  SetBitMask(FIFOLevelReg,0x80)                 ; // 清空FIFO 
  for(i=0; i<InLenByte; i++)
    WriteRawRC(FIFODataReg,pInData[i])          ; // 数据写入FIFO 
  WriteRawRC(CommandReg, Command)               ; // 命令写入命令寄存器
  if(Command == PCD_TRANSCEIVE)
    SetBitMask(BitFramingReg,0x80)              ; // 开始发送     
	
	PcdAntennaOn()																;//开启天线发射
	
  i = 100                                     		; //根据时钟频率调整，操作M1等待时间15ms
  do 
  {
    n = ReadRawRC(ComIrqReg)                    ;
    i--                                         ;
  }
  while((i!=0)&&!(n&0x01)&&!(n&waitFor))        ;
  ClearBitMask(BitFramingReg,0x80)              ;
  if(i!=0)
  {
    if(!(ReadRawRC(ErrorReg)&0x1B))
    {
      status = MI_OK                            ;
      if (n&irqEn&0x01)
        status = MI_NOTAGERR                    ;
      if(Command==PCD_TRANSCEIVE)
      {
        n = ReadRawRC(FIFOLevelReg)             ;
        lastBits = ReadRawRC(ControlReg)&0x07   ;
        if(lastBits)
          *pOutLenBit = (n-1)*8 + lastBits      ;
        else
          *pOutLenBit = n*8                     ;
        if(n==0)
          n = 1                                 ;
        if(n>MAXRLEN)
          n = MAXRLEN                           ;
				
       for (i=0; i<n; i++)
				{ 
					pOutData[i]= ReadRawRC(FIFODataReg)  ; 
				}
				
				DEBUG_MARK;
      }
    }
    else
      status = MI_ERR;        
  }
	else{
		PcdAntennaOff();
	}
  SetBitMask(ControlReg,0x80);// stop timer now
  WriteRawRC(CommandReg,PCD_IDLE); 
  return status;
}

//******************************************************************/
//功    能：寻卡                                                    /
//参数说明: req_code[IN]:寻卡方式                                   /
//                0x52 = 寻感应区内所有符合14443A标准的卡           /
//                0x26 = 寻未进入休眠状态的卡                       /
//          pTagType[OUT]：卡片类型代码                             /
//                0x4400 = Mifare_UltraLight                        /
//                0x0400 = Mifare_One(S50)                          /
//                0x0200 = Mifare_One(S70)                          /
//                0x0800 = Mifare_Pro(X)                            /
//                0x4403 = Mifare_DESFire                           /
//返    回: 成功返回MI_OK                                           /
//******************************************************************/
sint8_t PcdRequest(uint8_t req_code,uint8_t *pTagType)
{
  sint8_t status;  
  uint16_t  unLen;
  uint8_t ucComMF522Buf[MAXRLEN]; 

  ClearBitMask(Status2Reg,0x08);
  WriteRawRC(BitFramingReg,0x07);
  SetBitMask(TxControlReg,0x03);
 
  ucComMF522Buf[0] = req_code;

  status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,
                       1,ucComMF522Buf,&unLen);
  if ((status == MI_OK) && (unLen == 0x10))
  {    
    *pTagType     = ucComMF522Buf[0];
    *(pTagType+1) = ucComMF522Buf[1];
  }
  else
    status = MI_ERR;
  return status;
}

//******************************************************************/
//功    能：防冲撞                                                  /
//参数说明: pSnr[OUT]:卡片序列号，4字节                             /
//返    回: 成功返回MI_OK                                           /
//******************************************************************/
sint8_t PcdAnticoll(uint8_t *pSnr)
{
    sint8_t status;
    uint8_t i,snr_check=0;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
	//return MI_OK;
	
    ClearBitMask(Status2Reg,0x08);
    WriteRawRC(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x20;

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,2,ucComMF522Buf,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr+i)  = ucComMF522Buf[i];
             snr_check ^= ucComMF522Buf[i];
					 TestVal = snr_check;
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//功    能：选定卡片
//参数说明: pSnr[IN]:卡片序列号，4字节
//返    回: 成功返回MI_OK
/////////////////////////////////////////////////////////////////////
sint8_t PcdSelect(uint8_t *pSnr)
{
    sint8_t status;
    uint8_t i;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = PICC_ANTICOLL1;
    ucComMF522Buf[1] = 0x70;
    ucComMF522Buf[6] = 0;
    for (i=0; i<4; i++)
    {
    	ucComMF522Buf[i+2] = *(pSnr+i);
    	ucComMF522Buf[6]  ^= *(pSnr+i);
    }
    CalulateCRC(ucComMF522Buf,7,&ucComMF522Buf[7]);
  
    ClearBitMask(Status2Reg,0x08);

    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,9,ucComMF522Buf,&unLen);
    
    if ((status == MI_OK) && (unLen == 0x18))
    {   status = MI_OK;  }
    else
    {   status = MI_ERR;    }

    return status;
}

//******************************************************************/
//功    能：验证卡片密码
//参数说明: auth_mode[IN]: 密码验证模式
//                 0x60 = 验证A密钥
//                 0x61 = 验证B密钥 
//          addr[IN]：块地址
//          pKey[IN]：密码
//          pSnr[IN]：卡片序列号，4字节
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t PcdAuthState(uint8_t auth_mode,uint8_t addr,
                  uint8_t *pKey,uint8_t *pSnr    )
{
    sint8_t status;
    uint16_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = auth_mode;
    ucComMF522Buf[1] = addr;
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+2] = *(pKey+i);   }
    for (i=0; i<6; i++)
    {    ucComMF522Buf[i+8] = *(pSnr+i);   }
    
    status = PcdComMF522(PCD_AUTHENT,ucComMF522Buf,12,ucComMF522Buf,&unLen);
    if ((status != MI_OK) || (!(ReadRawRC(Status2Reg) & 0x08)))
    {   status = MI_ERR;   }
    
    return status;
}

//******************************************************************/
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t PcdRead(uint8_t addr,uint8_t *pData)
{
    sint8_t status                                          ;
    uint16_t  unLen                                  ;
    uint8_t i;uint8_t ucComMF522Buf[MAXRLEN]               ; 

    ucComMF522Buf[0] = PICC_READ                         ;
    ucComMF522Buf[1] = addr                              ;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2])       ;   
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,
                         ucComMF522Buf,&unLen           );
    if ((status == MI_OK) && (unLen == 0x90))
    {
        for (i=0; i<16; i++)
            *(pData+i) = ucComMF522Buf[i];   
    }
    else
      status = MI_ERR;       
    return status;
}

//******************************************************************/
//功    能：读取M1卡一块数据
//参数说明: addr[IN]：块地址
//          pData[OUT]：读出的数据，16字节
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t Read_Block(uint8_t Block,uint8_t *Buf)
{
  sint8_t result                                          ;

  result = PcdAuthState(0x60,Block,Password_Buffer,UID)   ;
  if(result!=MI_OK)
    return result                                         ;
  result = PcdRead(Block,Buf)                             ;
//  return result; // 2011.01.03
  
  if(result!=MI_OK)     return   result                   ;
  if(Block!=0x00&&des_on)
  {
		/*
    Des_Decrypt((sint8_t *)Buf,KK,(sint8_t *)Buf);
    Des_Decrypt((sint8_t *)&Buf[8],KK,(sint8_t *)&Buf[8]);  
		*/
  }
  return MI_OK                                            ; 
}

//******************************************************************/
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t PcdWrite(uint8_t addr,uint8_t *pData)
{
  sint8_t status                                          ;
  uint16_t  unLen                                     ;
  uint8_t i;uint8_t ucComMF522Buf[MAXRLEN]                ; 
    
  ucComMF522Buf[0] = PICC_WRITE                           ;
  ucComMF522Buf[1] = addr                                 ;
  CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2])          ;
  status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,
                       ucComMF522Buf,&unLen          )    ;
  if(  ( status != MI_OK)||(unLen != 4)
     ||((ucComMF522Buf[0]&0x0F)!= 0x0A))
    status = MI_ERR                                       ;           
  if (status == MI_OK)
  {
    for (i=0; i<16; i++)
      ucComMF522Buf[i] = *(pData+i)                       ;  
    CalulateCRC(ucComMF522Buf,16,&ucComMF522Buf[16])      ;
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,
                         18,ucComMF522Buf,&unLen     )    ;
    if(  (status != MI_OK)||(unLen != 4 )
       ||((ucComMF522Buf[0]&0x0F)!= 0x0A))
      status = MI_ERR                                     ;   
  }    
  return status                                           ;
}
//******************************************************************/
//功    能：写数据到M1卡一块
//参数说明: addr[IN]：块地址
//          pData[IN]：写入的数据，16字节
//返    回: 成功返回MI_OK
//******************************************************************/

sint8_t Write_Block(uint8_t Block)
{
  sint8_t result                                          ;
  if(des_on)
  {
/*    Des_Encrypt((sint8_t *)RF_Buffer    ,KK,
                (sint8_t *)RF_Buffer        )                ;// for debug
    Des_Encrypt((sint8_t *)&RF_Buffer[8],KK,
                (sint8_t *)&RF_Buffer[8]    )                ;// for debug  
*/								
  }
  result = PcdAuthState(0x60,Block,Password_Buffer,UID)   ;
  if(result!=MI_OK)
    return result                                         ;  
  result = PcdWrite(Block,RF_Buffer)                      ;
  return result                                           ;  
}

//******************************************************************/
//功    能：扣款和充值
//参数说明: dd_mode[IN]：命令字
//               0xC0 = 扣款
//               0xC1 = 充值
//          addr[IN]：钱包地址
//          pValue[IN]：4字节增(减)值，低位在前
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t PcdValue(uint8_t dd_mode,uint8_t addr,uint8_t *pValue)
{
    sint8_t status;
    uint16_t  unLen;
    uint8_t i,ucComMF522Buf[MAXRLEN]; 
    
    ucComMF522Buf[0] = dd_mode;
    ucComMF522Buf[1] = addr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
        
    if (status == MI_OK)
    {
        for (i=0; i<16; i++)
        {    ucComMF522Buf[i] = *(pValue+i);   }
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
        unLen = 0;
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = PICC_TRANSFER;
        ucComMF522Buf[1] = addr;
        CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]); 
   
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

        if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
        {   status = MI_ERR;   }
    }
    return status;
}

//******************************************************************/
//功    能：备份钱包
//参数说明: sourceaddr[IN]：源地址
//          goaladdr[IN]：目标地址
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t PcdBakValue(uint8_t sourceaddr, uint8_t goaladdr)
{
    sint8_t status;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_RESTORE;
    ucComMF522Buf[1] = sourceaddr;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }
    
    if (status == MI_OK)
    {
        ucComMF522Buf[0] = 0;
        ucComMF522Buf[1] = 0;
        ucComMF522Buf[2] = 0;
        ucComMF522Buf[3] = 0;
        CalulateCRC(ucComMF522Buf,4,&ucComMF522Buf[4]);
 
        status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,6,ucComMF522Buf,&unLen);
        if (status != MI_ERR)
        {    status = MI_OK;    }
    }
    
    if (status != MI_OK)
    {    return MI_ERR;   }
    
    ucComMF522Buf[0] = PICC_TRANSFER;
    ucComMF522Buf[1] = goaladdr;

    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);

    if ((status != MI_OK) || (unLen != 4) || ((ucComMF522Buf[0] & 0x0F) != 0x0A))
    {   status = MI_ERR;   }

    return status;
}


//******************************************************************/
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t PcdHalt(void)
{
    sint8_t status;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return status;
//    return MI_OK;
}

//******************************************************************/
//功    能：命令卡片进入休眠状态
//返    回: 成功返回MI_OK
//******************************************************************/
sint8_t MIF_Halt(void)
{
    sint8_t status;
    uint16_t  unLen;
    uint8_t ucComMF522Buf[MAXRLEN]; 

    ucComMF522Buf[0] = PICC_HALT;
    ucComMF522Buf[1] = 0;
    CalulateCRC(ucComMF522Buf,2,&ucComMF522Buf[2]);
 
    status = PcdComMF522(PCD_TRANSCEIVE,ucComMF522Buf,4,ucComMF522Buf,&unLen);
    return status ;  
//    return MI_OK;
}



//******************************************************************/
//用MF522计算CRC16函数
//******************************************************************/
void CalulateCRC(uint8_t *pIndata,uint8_t len,uint8_t *pOutData)
{
    uint8_t i,n;
    ClearBitMask(DivIrqReg,0x04);
    WriteRawRC(CommandReg,PCD_IDLE);
    SetBitMask(FIFOLevelReg,0x80);
    for (i=0; i<len; i++)
    {   WriteRawRC(FIFODataReg, *(pIndata+i));   }
    WriteRawRC(CommandReg, PCD_CALCCRC);
    i = 0xFF;
    do 
    {
        n = ReadRawRC(DivIrqReg);
        i--;
    }
    while ((i!=0) && !(n&0x04));
    pOutData[0] = ReadRawRC(CRCResultRegL);
    pOutData[1] = ReadRawRC(CRCResultRegM);
}


status_t Find_Card(void)
{

	//sint8_t status = MI_ERR ;
	uint8_t irqEn	 = 0x00 	 ;
	uint8_t waitFor = 0x00 	 ;
	//uint8_t lastBits				 ;
	uint8_t n								 ;
	uint16_t  i	             ;
 
	//ENTKS = 0;
 //ClearBitMask(ComIrqReg,0x80);		//release IRQ PIN
	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
	Hardware_DelayMs(2);
  irqEn	 = 0x20;//0x77;
  waitFor = 0x30;

	WriteRawRC(ComIEnReg,irqEn|0x80); //
	ClearBitMask(ComIrqReg,0x80);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80); // 清空FIFO 
	WriteRawRC(FIFODataReg,0x52); // 数据写入FIFO 
	WriteRawRC(CommandReg, PCD_TRANSCEIVE); // 命令写入命令寄存器

	SetBitMask(BitFramingReg,0x80); // 开始发送	 
	 
	PcdAntennaOn();//开启天线发射
   
	i = 15; //根据时钟频率调整，操作M1等待时间15ms
	do 
	{
		n = ReadRawRC(ComIrqReg);
		i--;
		if (PINMACRO_NFC_IRQ_STATUS == 0 )
		{
			//PcdAntennaOff();
			return S_SUCCESS;
		}
	}
	while((i!=0)&&!(n&0x01)&&!(n&waitFor));

	//PcdAntennaOff();

	return S_FAIL;
 
}

status_t MFC_Auto_Reader(uint8_t *Point)
{
	uint8_t i;
   
//	return S_FAIL;
		
 	//PcdAntennaOn();//开启天线发射 
//	Hardware_DelayMs(1);

	//PA_ODR |=SETBIT2;
	
	if ( Find_Card() == S_SUCCESS )
	{
		if(PcdAnticoll(UID)==MI_OK)
		{ 	
			DEBUG_MARK;
			if ( PcdSelect(UID) == MI_OK )
			{
				PcdAntennaOff();
				SetBitMask(ControlReg,0x80);// stop timer now
	  			WriteRawRC(CommandReg,PCD_IDLE); 
				
				for (i=0;i<4;i++)
				{
					*Point = UID[i];
					Point++;
				}
				DEBUG_MARK;
				return S_SUCCESS;
			}
		}
	}
	PcdAntennaOff();
	SetBitMask(ControlReg,0x80);// stop timer now
  	WriteRawRC(CommandReg,PCD_IDLE); 
	//WriteRawRC(CommandReg,PCD_SLEEP);

	//PA_ODR &=CLRBIT2; 
	return S_FAIL;
}

status_t MFC_ReadLockBrandData(uint8_t *buff)
{
	uint8_t i;
	uint8_t tempBuff[224];
	status_t TempResult;
	
	TempResult = S_FAIL;
	if ( Find_Card() == S_SUCCESS )
	{
		if(PcdAnticoll(UID)==MI_OK)
		{
			if( PcdSelect(UID)==MI_OK )
			{
				if ( Read_Block(0x04,tempBuff) == MI_OK )	//for save identify byte
				{	
					if (	(tempBuff[0] == 0x11)
							&&(tempBuff[1] == 0x55)
							&&(tempBuff[2] == 0xAA)
							&&(tempBuff[3] == 0xDD)
							&&(tempBuff[4] == 0xBB)
							&&(tempBuff[5] == 0x11)
							&&(tempBuff[6] == 0x55)
							&&(tempBuff[7] == 0xAA)
							&&(tempBuff[8] == 0xDD)
							&&(tempBuff[9] == 0xBB)	)
					{
						for ( i=0;i<7;i++)
						{
							if ( Read_Block(4*i+8,&tempBuff[(2*i)*16]) != MI_OK ){
								break;
							}
							if ( Read_Block(4*i+9,&tempBuff[((2*i)+1)*16]) != MI_OK ){
								break;
							}
							CLRWDT();
						}
						if ( i==7 )
						{
							for (i=0;i<224;i++)
							{
								*(buff+i)=tempBuff[i];
							}
							TempResult = S_SUCCESS;
						}
					}
				}
			}
		}
	}
	PcdAntennaOff();
	SetBitMask(ControlReg,0x80);// stop timer now
  	WriteRawRC(CommandReg,PCD_IDLE); 
	
	return TempResult;
}


void MFC_POWERDOWN(void)
{
	
	SET_MFC_RST_L;
	Hardware_DelayMs(1);

	//WriteRawRC(CommandReg,PCD_RESETPHASE);
	//Hardware_DelayMs(1) 				 ;
	
	//WriteRawRC(CommandReg,PCD_SLEEP);
}

void MFC_WAKEUP(void)
{	
	SET_MFC_RST_H;
	Hardware_DelayMs(1);
/*	
	WriteRawRC(RFCfgReg,0x78) 	  ; 
	WriteRawRC(GsNReg,0xFF)		  ; 
	WriteRawRC(CWGsCfgReg,0x3F)	  ; 
	WriteRawRC(ModGsCfgReg,0x3F)	   ; 
	  
	  
	WriteRawRC(ModeReg,0x3D)			 ;
	WriteRawRC(TReloadRegL,30)			 ;
	WriteRawRC(TReloadRegH,0)			 ;

	WriteRawRC(TModeReg,0x8D)			 ;
	WriteRawRC(TPrescalerReg,0x3E)		 ;	
*/
	
	DEBUG_MARK;
}


void MFC_Init(void)
{
	PcdReset();//复位RC522
	//PcdAntennaOn();//开启天线发射 
	PcdAntennaOff();
	
	DEBUG_MARK;
}


void MFC_Test(void)
{
       uint8_t i;
        TestVal = 0x00;
       DEBUG_MARK;
	TestVal = ReadRawRC(RFCfgReg);//,0x78)       ; 
	DEBUG_MARK;
	TestVal = ReadRawRC(GsNReg);//,0xFF)       	; 
	DEBUG_MARK;
	TestVal = ReadRawRC(CWGsCfgReg);//,0x3F)     ; 
	DEBUG_MARK;
	TestVal = ReadRawRC(ModGsCfgReg);//,0x3F)     ; 
	DEBUG_MARK;
	
  TestVal = ReadRawRC(ModeReg);//,0x3D)             ;
	DEBUG_MARK;
  TestVal = ReadRawRC(TReloadRegL);//,30)           ;
	DEBUG_MARK;
  TestVal = ReadRawRC(TReloadRegH);//,0)            ;
	DEBUG_MARK;
  
  TestVal = ReadRawRC(TModeReg);//,0x8D)            ;
	DEBUG_MARK;
  TestVal = ReadRawRC(TPrescalerReg);//,0x3E)       ;
	DEBUG_MARK;
	  TestVal = ReadRawRC(TPrescalerReg);//,0x3E)       ;
	DEBUG_MARK;
	  TestVal = ReadRawRC(TPrescalerReg);//,0x3E)       ;
	DEBUG_MARK;
	//PcdAntennaOn();//开启天线发射 
	//Hardware_DelayMs(1);
	//Find_Card();
	//PcdAntennaOff();
	 for(i=0; i<10; i++)
		TestBuff[i]=i;
		
	 for(i=0; i<10; i++)
			WriteRawRC(FIFODataReg,TestBuff[i])          ; // 数据写入FIFO 
	DEBUG_MARK;			
	for(i=0; i<10; i++)
		TestBuff[i]=0xFF;		
	DEBUG_MARK;	
	
	//i=0;	
	
	TestVal = (FIFODataReg<<1)|0x80;
	for(i=0; i<10; i++)
	{
		
		TestBuff[i]	= ReadRawRC(FIFODataReg)          ; //
		/*
		SET_MFC_CS_L;
		Hardware_DelayX1us(100);
	
		HAL_SPI_Transmit(&hspi2,&TestVal,1,10);
		Hardware_DelayMs(10);
		HAL_SPI_Receive(&hspi2,&TestBuff[i],1,10);
	//	HAL_STATUS = HAL_SPI_TransmitReceive(&hspi2,&ucResult,&ReadData,1,500);
		Hardware_DelayMs(10);
		SET_MFC_CS_H;

	//	Hardware_DelayMs(10);
		Hardware_DelayX1us(100);
	*/
	DEBUG_MARK;		
	}		
}
#endif


