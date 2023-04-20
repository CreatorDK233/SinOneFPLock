#include "spi.h"
#include "SC95F861xB_C.H"

bit SPI0_Flag = 0;         //SPIX数据传输完成标志位
//#define		SPI0_send_OK	while(!SPI0_Flag); SPI0_Flag = 0
#define		SPI0_send_OK 	while(US0CON1 & 0X20);US0CON1 &= ~0X20
void MX_SPI0_Init(void)
{
	SPI0_Init();
}
void SPI0_DeInit(void)
{
	US0CON0 &= ~0x80;  //关闭SPI0
	IE1 &= ~0x01;			//关中断
}

/*******************************SPI发送字节*/
void SPI0SendByte(uint8_t dat)
{
  US0CON2 = dat;
	SPI0_send_OK;
	//Soft_Delay_usS(3);
}
/******************************SPI发送字符串*/
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

//void SPI0_Int(void) //interrupt 7    //SPI0中断函数
//{
//  if(US0CON1 & 0X08)  //发送缓存器空标志判断
//  {
//    US0CON1 &= ~0X08;
//  }
//  if(US0CON1 & 0X80)  //数据传输标志位判断
//  {
//    US0CON1 &= ~0X80;
//    SPI0_Flag = 1;
//  }
//}

