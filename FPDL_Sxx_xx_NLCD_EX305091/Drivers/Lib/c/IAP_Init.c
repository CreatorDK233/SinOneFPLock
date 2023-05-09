#include "Function_Init.H"
#include "IAP_Option_EW.h"
unsigned char code* POINT;	     //该指针指向0x0000

#define IAP_APROM 0x00
#define IAP_UniqueID 0x01
#define IAP_EEPROM 0x02
#define IAP_LDROM 0x03

//unsigned char IAP_Read(unsigned long IAP_IapAddr, unsigned char area);

void IAP_Test(void)
{
  unsigned char IapReadData1,IapReadData2;				//定义储存读出数据的变量
	
  IAP_CodeSectorEraseOption(0xFFFE);			//往CODE 0x1FFFE地址块擦
  IAP_CodeProgramByteOption(0xFFFE, 0x55);			//往CODE 0x1FFFE地址写入0x55
  IapReadData1 = IAP_Read(0xFFFE, IAP_APROM);	//读取CODE 0x1FFFE地址数据
	
  IAP_EEPROMSectorEraseOption(0x1000);			//往EEPROM 0x1000地址块擦
  IAP_EEPROMProgramByteOption(0x1000, 0xAA);			//往EEPROM 0x1000地址写入0xAA
  IapReadData2 = IAP_Read(0x1000, IAP_EEPROM);		//读取EEPROM 0x1000地址数据
  while(1)
  {
    if((IapReadData1 == 0x55) && (IapReadData2 == 0xAA))
    {
      P02 = ~P02;
    }
    else
    {
      P03 = ~P03;
    }
  }
}

/**************************************************
*函数名称：unsigned char IAP_Read(unsigned long Add,unsigned char ADER)
*函数功能：单Byte读取
*入口参数：Add ：需要擦除的地址(0X00~0X1FFFF（FLASH物理地址）
*         Data：要写入的Byte,ADER:操作对象 APROM为00,EEPROM为02
*出口参数：void
**************************************************/
unsigned char IAP_Read(unsigned long Add, unsigned char Iapade)
{
  unsigned char IAP_IapData;
  //保存ROMBNK、IAPADE、EA
  unsigned char tempADER = IAPADE;
  unsigned char tempEA = EA;

  unsigned char code* point = 0;

  EA = 0;//关闭中断

  IAPADE = Iapade;

  IAP_IapData = *(point + Add);

  //操作结束恢复ROMBNK、IAPADE、EA，避免MOVC位置出错
  IAPADE = tempADER;
  EA = tempEA;

  return IAP_IapData;

}




