#include "Function_Init.H"
#include "IAP_Option_EW.h"
unsigned char code* POINT;	     //��ָ��ָ��0x0000

#define IAP_APROM 0x00
#define IAP_UniqueID 0x01
#define IAP_EEPROM 0x02
#define IAP_LDROM 0x03

//unsigned char IAP_Read(unsigned long IAP_IapAddr, unsigned char area);

void IAP_Test(void)
{
  unsigned char IapReadData1,IapReadData2;				//���崢��������ݵı���
	
  IAP_CodeSectorEraseOption(0xFFFE);			//��CODE 0x1FFFE��ַ���
  IAP_CodeProgramByteOption(0xFFFE, 0x55);			//��CODE 0x1FFFE��ַд��0x55
  IapReadData1 = IAP_Read(0xFFFE, IAP_APROM);	//��ȡCODE 0x1FFFE��ַ����
	
  IAP_EEPROMSectorEraseOption(0x1000);			//��EEPROM 0x1000��ַ���
  IAP_EEPROMProgramByteOption(0x1000, 0xAA);			//��EEPROM 0x1000��ַд��0xAA
  IapReadData2 = IAP_Read(0x1000, IAP_EEPROM);		//��ȡEEPROM 0x1000��ַ����
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
*�������ƣ�unsigned char IAP_Read(unsigned long Add,unsigned char ADER)
*�������ܣ���Byte��ȡ
*��ڲ�����Add ����Ҫ�����ĵ�ַ(0X00~0X1FFFF��FLASH�����ַ��
*         Data��Ҫд���Byte,ADER:�������� APROMΪ00,EEPROMΪ02
*���ڲ�����void
**************************************************/
unsigned char IAP_Read(unsigned long Add, unsigned char Iapade)
{
  unsigned char IAP_IapData;
  //����ROMBNK��IAPADE��EA
  unsigned char tempADER = IAPADE;
  unsigned char tempEA = EA;

  unsigned char code* point = 0;

  EA = 0;//�ر��ж�

  IAPADE = Iapade;

  IAP_IapData = *(point + Add);

  //���������ָ�ROMBNK��IAPADE��EA������MOVCλ�ó���
  IAPADE = tempADER;
  EA = tempEA;

  return IAP_IapData;

}




