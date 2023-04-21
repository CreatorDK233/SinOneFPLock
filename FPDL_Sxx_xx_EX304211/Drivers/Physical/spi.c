#include "spi.h"
#include "SC95F861xB_C.H"

bit SPI0_Flag = 0;         //SPIX���ݴ�����ɱ�־λ
//#define		SPI0_send_OK	while(!SPI0_Flag); SPI0_Flag = 0
#define		SPI0_send_OK 	while(US0CON1 & 0X20);US0CON1 &= ~0X20
void MX_SPI0_Init(void)
{
	SPI0_Init();
}
void SPI0_DeInit(void)
{
	US0CON0 &= ~0x80;  //�ر�SPI0
	IE1 &= ~0x01;			//���ж�
}

/*******************************SPI�����ֽ�*/
void SPI0SendByte(uint8_t dat)
{
  US0CON2 = dat;
	SPI0_send_OK;
	//Soft_Delay_usS(3);
}
/******************************SPI�����ַ���*/
void SPI0SendStr(uint8_t *pstr,uint8_t strlen)
{
  if((NULL == pstr)||(0 == strlen))
  {
    return;
  }
  while(strlen--)
  {
		SPI0SendByte(*pstr);
//    US0CON2 = *pstr;
//		SPI0_send_OK;
    pstr++;
  }
}

//void SPI0_Int(void) //interrupt 7    //SPI0�жϺ���
//{
//  if(US0CON1 & 0X08)  //���ͻ������ձ�־�ж�
//  {
//    US0CON1 &= ~0X08;
//  }
//  if(US0CON1 & 0X80)  //���ݴ����־λ�ж�
//  {
//    US0CON1 &= ~0X80;
//    SPI0_Flag = 1;
//  }
//}

