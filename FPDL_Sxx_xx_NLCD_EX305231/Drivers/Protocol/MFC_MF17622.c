#include "IO.h"
#include "I2cBus.h"
#include "MFC_MF17622.h"
#include "FM17622_LPCD_API.h"
#include "GUI_Function.h"
#include "Basic_Function.h"
#include "global_variable.h"

//******************************************************************/
//                         ��������                                 /
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

extern sint8_t KK[8]; // ���ݼ�����Կ
static uint8_t RF_Buffer[18]; // ��Ƶ�����ݻ�����
static uint8_t UID[5];
static uint8_t Password_Buffer[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
//extern void Des_Encrypt(sint8_t* In, sint8_t* K,sint8_t* Out);
//extern void Des_Decrypt(sint8_t* In, sint8_t* K,sint8_t* Out);
static uint8_t des_on; // DES���ܱ�־

static uint8_t TestVal;
static uint8_t TestBuff[20];

static uint8_t HAL_SPI2_RXBUFF;
#ifdef Function_CardReaderUseSPI
//******************************************************************/
//��    �ܣ���RC522�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//��    �أ�������ֵ
//******************************************************************/
uint8_t ReadRawRC(uint8_t Address)
{
	uint8_t ucResult,ReadData;
	uint8_t TempNFC_TX;
	HAL_StatusTypeDef HAL_STATUS;
	
	SET_MFC_CS_L;
	Hardware_DelayX1us(10);
//	Hardware_DelayMs(10);
	ucResult = (Address<<1)|0x80;
	
	HAL_STATUS = HAL_SPI_Transmit(&hspi2,&ucResult,1,10);
	
	TempNFC_TX=0x00;
	HAL_STATUS = HAL_SPI_TransmitReceive(&hspi2,&TempNFC_TX,&HAL_SPI2_RXBUFF,1,10);
	
	SET_MFC_CS_H;

//	Hardware_DelayMs(10);
  Hardware_DelayX1us(10);
	return HAL_SPI2_RXBUFF;
}

//******************************************************************/
//��    �ܣ�дRC522�Ĵ���
//����˵����Address[IN]:�Ĵ�����ַ
//          value[IN]:д���ֵ
//******************************************************************/
void  WriteRawRC(uint8_t Address, uint8_t value)
{
	uint8_t temp;
	HAL_StatusTypeDef HAL_STATUS;
	
	SET_MFC_CS_L;
  Hardware_DelayX1us(10);
	
	temp = Address<<1;
	HAL_STATUS = HAL_SPI_Transmit(&hspi2,&temp,1,500);
	HAL_STATUS = HAL_SPI_Transmit(&hspi2,&value,1,500);
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
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
//******************************************************************/
void SetBitMask(uint8_t reg,uint8_t mask)  
{
  sint8_t tmp = 0x00;
  tmp = ReadRawRC(reg)| mask;
  WriteRawRC(reg, tmp);  // set bit mask
}

//******************************************************************/
//��    �ܣ���RC522�Ĵ���λ
//����˵����reg[IN]:�Ĵ�����ַ
//          mask[IN]:��λֵ
//******************************************************************/
void ClearBitMask(uint8_t reg,uint8_t mask)  
{
  sint8_t tmp = 0x00;
  tmp = ReadRawRC(reg)&(~mask);
  WriteRawRC(reg, tmp)        ;  // clear bit mask
} 

status_t FM17622_SoftReset(void)
{	
	unsigned char reg_data;
	WriteRawRC(CommandReg,PCD_RESETPHASE);
	Hardware_DelayMs(3); 
	reg_data = ReadRawRC(CommandReg);
	if(reg_data == 0x20)
		return S_SUCCESS;
	else
		return S_FAIL;
}
//******************************************************************/
//��    �ܣ���λRC522
//��    ��: �ɹ�����MI_OK
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
	Hardware_DelayMs(5)                  ;

	WriteRawRC(RFCfgReg,0x58)       ; 

	/* ������� GsNǰ����λ���� CWGsCfg���0x3F ModGsCfg���0x3F*/
#ifdef ProjectIs_AutoBarLock_S11Z04
	
	WriteRawRC(GsNReg,0xFF) 		; 
	WriteRawRC(CWGsCfgReg,0x3F) 	; 
	WriteRawRC(ModGsCfgReg,0x3F)	 ; 
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
//�������߷���  
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
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
//�������߷���  
//ÿ��������ر����շ���֮��Ӧ������1ms�ļ��
//******************************************************************/
void PcdAntennaTestOn(void)
{
//*
  SET_MFC_RST_H                         ;
  Hardware_DelayMs(15); // 2010.10.09 ???? FOR DEBUG
  
  WriteRawRC(TxControlReg,0x02)    ;
}


//******************************************************************/
//�ر����߷���
//******************************************************************/
void PcdAntennaOff(void)
{
	//return;
  	ClearBitMask(TxControlReg, 0x03);
}

//******************************************************************/
//��    �ܣ�ͨ��RC522��ISO14443��ͨѶ
//����˵����Command[IN]:RC522������
//          pInData[IN]:ͨ��RC522���͵���Ƭ������
//          InLenByte[IN]:�������ݵ��ֽڳ���
//          pOutData[OUT]:���յ��Ŀ�Ƭ��������
//          *pOutLenBit[OUT]:�������ݵ�λ����
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
  SetBitMask(FIFOLevelReg,0x80)                 ; // ���FIFO 
  for(i=0; i<InLenByte; i++)
    WriteRawRC(FIFODataReg,pInData[i])          ; // ����д��FIFO 
  WriteRawRC(CommandReg, Command)               ; // ����д������Ĵ���
  if(Command == PCD_TRANSCEIVE)
    SetBitMask(BitFramingReg,0x80)              ; // ��ʼ����     
	
	PcdAntennaOn()																;//�������߷���
	
  i = 100                                     		; //����ʱ��Ƶ�ʵ���������M1�ȴ�ʱ��15ms
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
//��    �ܣ�Ѱ��                                                    /
//����˵��: req_code[IN]:Ѱ����ʽ                                   /
//                0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�           /
//                0x26 = Ѱδ��������״̬�Ŀ�                       /
//          pTagType[OUT]����Ƭ���ʹ���                             /
//                0x4400 = Mifare_UltraLight                        /
//                0x0400 = Mifare_One(S50)                          /
//                0x0200 = Mifare_One(S70)                          /
//                0x0800 = Mifare_Pro(X)                            /
//                0x4403 = Mifare_DESFire                           /
//��    ��: �ɹ�����MI_OK                                           /
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
//��    �ܣ�����ײ                                                  /
//����˵��: pSnr[OUT]:��Ƭ���кţ�4�ֽ�                             /
//��    ��: �ɹ�����MI_OK                                           /
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
         }
         if (snr_check != ucComMF522Buf[i])
         {   status = MI_ERR;    }
    }
    
    SetBitMask(CollReg,0x80);
    return status;
}

/////////////////////////////////////////////////////////////////////
//��    �ܣ�ѡ����Ƭ
//����˵��: pSnr[IN]:��Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ���֤��Ƭ����
//����˵��: auth_mode[IN]: ������֤ģʽ
//                 0x60 = ��֤A��Կ
//                 0x61 = ��֤B��Կ 
//          addr[IN]�����ַ
//          pKey[IN]������
//          pSnr[IN]����Ƭ���кţ�4�ֽ�
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
sint8_t PcdRead(uint8_t addr,uint8_t *pData)
{
    sint8_t status                                  ;
    uint16_t  unLen                                 ;
    uint8_t i																				;
		uint8_t ucComMF522Buf[MAXRLEN]               		; 

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
//��    �ܣ���ȡM1��һ������
//����˵��: addr[IN]�����ַ
//          pData[OUT]�����������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
sint8_t Read_Block(uint8_t Block,uint8_t *Buf)
{
  sint8_t result                                             ;

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
  return MI_OK                                          ; 
}

//******************************************************************/
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/
sint8_t PcdWrite(uint8_t addr,uint8_t *pData)
{
  sint8_t status                                             ;
  uint16_t  unLen                                     ;
  uint8_t i;uint8_t ucComMF522Buf[MAXRLEN]                  ; 
    
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
//��    �ܣ�д���ݵ�M1��һ��
//����˵��: addr[IN]�����ַ
//          pData[IN]��д������ݣ�16�ֽ�
//��    ��: �ɹ�����MI_OK
//******************************************************************/

sint8_t Write_Block(uint8_t Block)
{
  sint8_t result                                             ;
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
//��    �ܣ��ۿ�ͳ�ֵ
//����˵��: dd_mode[IN]��������
//               0xC0 = �ۿ�
//               0xC1 = ��ֵ
//          addr[IN]��Ǯ����ַ
//          pValue[IN]��4�ֽ���(��)ֵ����λ��ǰ
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ�����Ǯ��
//����˵��: sourceaddr[IN]��Դ��ַ
//          goaladdr[IN]��Ŀ���ַ
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
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
//��    �ܣ����Ƭ��������״̬
//��    ��: �ɹ�����MI_OK
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
//��MF522����CRC16����
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
 
 //ClearBitMask(ComIrqReg,0x80);		//release IRQ PIN
	ClearBitMask(Status2Reg,0x08);
	WriteRawRC(BitFramingReg,0x07);
	SetBitMask(TxControlReg,0x03);
	Hardware_DelayMs(6);
  irqEn	 = 0x20;//0x77;
  waitFor = 0x30;

	WriteRawRC(ComIEnReg,irqEn|0x80); //
	ClearBitMask(ComIrqReg,0x80);
	WriteRawRC(CommandReg,PCD_IDLE);
	SetBitMask(FIFOLevelReg,0x80); // ���FIFO 
	WriteRawRC(FIFODataReg,0x52); // ����д��FIFO 
	WriteRawRC(CommandReg, PCD_TRANSCEIVE); // ����д������Ĵ���

	SetBitMask(BitFramingReg,0x80); // ��ʼ����	 
	 
	PcdAntennaOn();//�������߷���
   
	i = 15; //����ʱ��Ƶ�ʵ���������M1�ȴ�ʱ��15ms
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
	//TOUCH_SUSPEND();
	if ( Find_Card() == S_SUCCESS )
	{
		DEBUG_MARK;
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
				//TOUCH_RESUME();		
				DEBUG_MARK;
				return S_SUCCESS;
			}
		}
	}
	PcdAntennaOff();
	SetBitMask(ControlReg,0x80);// stop timer now
  	WriteRawRC(CommandReg,PCD_IDLE); 
	//TOUCH_RESUME();
	return S_FAIL;
}

status_t MFC_ReadLockBrandData(uint8_t *buff)
{
	uint16_t i;
	uint8_t BrandBuff[512];
	status_t TempResult;
	
	TempResult = S_FAIL;
	if ( Find_Card() == S_SUCCESS )
	{
		if(PcdAnticoll(UID)==MI_OK)
		{
			if( PcdSelect(UID)==MI_OK )
			{
				if ( Read_Block(0x04,BrandBuff) == MI_OK )	//for save identify byte
				{	
					if (   (BrandBuff[0] == 0x11)
						 &&(BrandBuff[1] == 0x55)
						 &&(BrandBuff[2] == 0xAA)
						 &&(BrandBuff[3] == 0xDD)
						 &&(BrandBuff[4] == 0xBB)
						 &&(BrandBuff[5] == 0x11)
						 &&(BrandBuff[6] == 0x55)
						 &&(BrandBuff[7] == 0xAA)
						 &&(BrandBuff[8] == 0xDD)
						 &&(BrandBuff[9] == 0xBB)
						 )
					{
						/******* samll log, display on buttom of screen *******/
						for ( i=0;i<7;i++)
						{
							if ( Read_Block(4*i+8,&BrandBuff[(2*i)*16]) != MI_OK ){
								break;
							}
							if ( Read_Block(4*i+9,&BrandBuff[((2*i)+1)*16]) != MI_OK ){
								break;
							}
							CLRWDT();
						}
						if ( i==7 )
						{
							for (i=0;i<224;i++)
							{
								*(buff+i)=BrandBuff[i];
							}
							TempResult = S_SUCCESS;
							LockBrand.BrandType = SmallBrand;
						}
					}
					else if (   (BrandBuff[0] == 0x12)
						 &&(BrandBuff[1] == 0x55)
						 &&(BrandBuff[2] == 0xAA)
						 &&(BrandBuff[3] == 0xDD)
						 &&(BrandBuff[4] == 0xBB)
						 &&(BrandBuff[5] == 0x12)
						 &&(BrandBuff[6] == 0x55)
						 &&(BrandBuff[7] == 0xAA)
						 &&(BrandBuff[8] == 0xDD)
						 &&(BrandBuff[9] == 0xBB)
						 )
					{
						/******* big log, display on center of screen *******/
						for ( i=0;i<11;i++)
						{
							if ( Read_Block(4*i+8,&BrandBuff[(3*i)*16]) != MI_OK ){
								break;
							}
							if ( Read_Block(4*i+9,&BrandBuff[((3*i)+1)*16]) != MI_OK ){
								break;
							}
							if ( i<10 )
							{
								if ( Read_Block(4*i+10,&BrandBuff[((3*i)+2)*16]) != MI_OK ){
									break;
								}
							}
							CLRWDT();
						}
						if ( i==11 )
						{
							for (i=0;i<512;i++)
							{
								*(buff+i)=BrandBuff[i];
							}
							TempResult = S_SUCCESS;
							LockBrand.BrandType = BigBrand;
						}
					} 
					else if (   (BrandBuff[0] == 0xEE)
						 &&(BrandBuff[1] == 0x55)
						 &&(BrandBuff[2] == 0xAA)
						 &&(BrandBuff[3] == 0xDD)
						 &&(BrandBuff[4] == 0xBB)
						 &&(BrandBuff[5] == 0xEE)
						 &&(BrandBuff[6] == 0x55)
						 &&(BrandBuff[7] == 0xAA)
						 &&(BrandBuff[8] == 0xDD)
						 &&(BrandBuff[9] == 0xBB)
						 )
					{
						TempResult = S_SUCCESS;
						LockBrand.BrandType = DefaultBigBrand;
					}
					else if (   (BrandBuff[0] == 0xEF)
						 &&(BrandBuff[1] == 0x55)
						 &&(BrandBuff[2] == 0xAA)
						 &&(BrandBuff[3] == 0xDD)
						 &&(BrandBuff[4] == 0xBB)
						 &&(BrandBuff[5] == 0xEF)
						 &&(BrandBuff[6] == 0x55)
						 &&(BrandBuff[7] == 0xAA)
						 &&(BrandBuff[8] == 0xDD)
						 &&(BrandBuff[9] == 0xBB)
						 )
					{
						TempResult = S_SUCCESS;
						LockBrand.BrandType = DefaultSmallBrand;
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

status_t MFC_FrmFunctionConfigSwitch(void)
{
	//uint8_t i;
	uint8_t BrandBuff[224];
	status_t TempResult;
	
	TempResult = S_FAIL;
	if ( Find_Card() == S_SUCCESS )
	{
		if(PcdAnticoll(UID)==MI_OK)
		{
			if( PcdSelect(UID)==MI_OK )
			{
				if ( Read_Block(0x04,BrandBuff) == MI_OK )	//for save identify byte
				{	
					if (   (BrandBuff[0] == 0x22)
						 &&(BrandBuff[1] == 0x66)
						 &&(BrandBuff[2] == 0x88)
						 &&(BrandBuff[3] == 0xCC)
						 &&(BrandBuff[4] == 0x44)
						 &&(BrandBuff[5] == 0x22)
						 &&(BrandBuff[6] == 0x66)
						 &&(BrandBuff[7] == 0x88)
						 &&(BrandBuff[8] == 0xCC)
						 &&(BrandBuff[9] == 0x44)
						 )
					{
						TempResult = S_SUCCESS;
						
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
	#ifdef Function_FM17622_LPCD
	if ( IfSystemIsNoCardUser() == bFALSE )		//System has card user
	{
		if ( Lpcd_Init_Register() == S_SUCCESS )
		{
			//NFC����
			EnableNFCIRQ_Interrupt();
		}
	}
	#endif
	SET_MFC_RST_L;
	Hardware_DelayMs(1);
	SET_MFC_CS_H;
}

void MFC_WAKEUP(void)
{	
	DisableNFCIRQ_Interrupt();

	SET_MFC_RST_H;
	
	Hardware_DelayMs(1);

	SET_MFC_CS_H;

	Lpcd_ClearIrq();
	
	DEBUG_MARK;
}


void MFC_Init(void)
{
	PcdReset();//��λRC522
	//PcdAntennaOn();//�������߷��� 
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
	//PcdAntennaOn();//�������߷��� 
	//Hardware_DelayMs(1);
	//Find_Card();
	//PcdAntennaOff();
	 for(i=0; i<10; i++)
		TestBuff[i]=i;
		
	 for(i=0; i<10; i++)
			WriteRawRC(FIFODataReg,TestBuff[i])          ; // ����д��FIFO 
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



