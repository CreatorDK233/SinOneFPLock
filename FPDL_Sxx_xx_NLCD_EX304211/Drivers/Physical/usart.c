#include "usart.h"
#include "ISR.h"
#include "Project.h"
#include "SC95F861xB_C.H"

//ѡ������һģ�飺
#define  USCI2   2
#define  USCI3   3
#define  USCI4   4
#define  USCI5   5

//ѡ������һģʽ��
#define  UartX   0
#define  TWIX    1
#define  SPIX    2

//uint16_t uart3_rx_count=0;
//uint8_t xdata Uart3_Rx[UART_MAX_LEN];
//uint16_t uart1_rx_count=0;
//uint8_t xdata Uart1_Rx[UART_MAX_LEN];
bit Uart1SendFlag = 0;    //Uart1�����жϱ�־λ
bit Uart1ReceiveFlag = 0; //Uart1�����жϱ�־λ
bit Uart2SendFlag = 0;    //Uart2�����жϱ�־λ
bit Uart2ReceiveFlag = 0; //Uart2�����жϱ�־λ
bit Uart3SendFlag = 0;
bit Uart3ReceiveFlag = 0;

#define 	Choose_SSI2 	SSIxx_Select = USCI2; /*SSIxx_Mode = UartX;*/ USXINX = SSIxx_Select
#define 	Choose_SSI3 	SSIxx_Select = USCI3; /*SSIxx_Mode = UartX;*/ USXINX = SSIxx_Select

#define		Uart1_send_OK		while(!Uart1SendFlag); Uart1SendFlag = 0
#define		Uart2_send_OK		while(!Uart2SendFlag); Uart2SendFlag = 0//while( !(USXCON0 & 0x02) ); USXCON0 |= 0x02
#define		Uart3_send_OK		while(!Uart3SendFlag); Uart3SendFlag = 0//while( !(USXCON0 & 0x02) ); USXCON0 |= 0x02

#define 	USXINX_Clear	//USXINX = 0
//uart1,wifi
void MX_UART1_Init(void)
{
	Uart1_Init(32,115200);
}
void MX_UART1_DeInit(void)
{
	OTCON &=~ 0xC0;//�ر�USART0
	IE2   &=~ 0x01;//�رմ����ж�
}
//uart2,FPM
void MX_UART2_Init(void)
{
	Choose_SSI2;
	UARTX_Init(32, 57600);
	//IP2 |= 0x02;
	USXINX_Clear;
}
void MX_UART2_DeInit(void)
{
	Choose_SSI2;
	TMCON &= 0x3F;
	IE2 &= ~0x02;  
	//IP2 &= ~0x02;
	USXINX_Clear;
}
//uart3
void MX_UART3_Init(void)
{
  Choose_SSI3;
	UARTX_Init(32, 115200);
	USXINX_Clear;
}
void MX_UART3_DeInit(void)
{
	Choose_SSI3;
	TMCON &= 0x3F;
	IE2 &= ~0x04;  
	USXINX_Clear;	
}
////uart5
//void MX_USART5_UART_Init(void)
//{
//  Choose_SSI5;
//	uart5_rx_count=0;
//	UARTX_Init(32, 115200);
//	USXINX_Clear;
//}
//void UART5_DeInit(void)
//{
//	Choose_SSI5;
//	TMCON &= 0x3F;
//	IE2 &= ~0x10;  
//	USXINX_Clear;	
//}
/*******************************����1���͵��ֽ�*/
void Uart1SendByte(uint8_t dat)
{
	 US1CON3 = dat;
	 Uart1_send_OK;
}
/******************************����1�����ַ���*/
void Uart1SendStr(uint8_t *pstr,uint16_t strlen)
{
	if((NULL == pstr)||(0 == strlen))
  {
    return;
  }
  while(strlen--)
  {
		Uart1SendByte(*pstr);
    pstr++;
  }
}
/******************************����1��ȡ���ֽ�*/
void Uart1ReadByte(uint8_t *dat)
{
	 *dat = US1CON3;
}
/*******************************����2�����ֽ�*/
void Uart2SendByte(uint8_t dat)
{
	Choose_SSI2;
  USXCON3 = dat;
	Uart2_send_OK;
	//Soft_Delay_usS(5);
	USXINX_Clear;
}
/******************************����2�����ַ���*/
void Uart2SendStr(uint8_t *pstr,uint16_t strlen)
{
  if((NULL == pstr)||(0 == strlen))
  {
    return;
  }
//	Choose_SSI2;
  while(strlen--)
  {
		Uart2SendByte(*pstr);
//		USXCON3 = *pstr;
//		Uart2_send_OK;
    pstr++;
  }
//	USXINX_Clear;
}
/******************************����2��ȡ���ֽ�*/
void Uart2ReadByte(uint8_t *dat)
{
	Choose_SSI2;
	*dat = USXCON3;
	USXINX_Clear;
}
/*******************************����3�����ֽ�*/
void Uart3SendByte(uint8_t dat)
{
	Choose_SSI3;
  USXCON3 = dat;
	Uart3_send_OK;
	USXINX_Clear;
}
/******************************����3�����ַ���*/
void Uart3SendStr(uint8_t *pstr,uint16_t strlen)
{
  if((NULL == pstr)||(0 == strlen))
  {
    return;
  }
  while(strlen--)
  {
		Uart3SendByte(*pstr);

    pstr++;
  }
}
/******************************����3��ȡ���ֽ�*/
void Uart3ReadByte(uint8_t *dat)
{
	Choose_SSI3;
	*dat = USXCON3;
	USXINX_Clear;
}
///*******************************����5�����ֽ�*/
//void Uart5SendByte(uint8_t dat)
//{
//	Choose_SSI5;
//  USXCON3 = dat;
//	Uart5_send_OK;
//	USXINX_Clear;
//}
///******************************����5�����ַ���*/
//void Uart5SendStr(uint8_t *pstr,uint16_t strlen)
//{
//  if((NULL == pstr)||(0 == strlen))
//  {
//    return;
//  }
////	Choose_SSI5;
//  while(strlen--)
//  {
//		Uart5SendByte(*pstr);
////    USXCON3 = *pstr;
////		Uart5_send_OK;
//    pstr++;
//  }
////	USXINX_Clear;
//}

void UART1_Int() interrupt 15   //Uart1�жϺ���
{
	if(US1CON0&0x02)    //���ͱ�־λ�ж�
	{
		US1CON0 |= 0x02;
		Uart1SendFlag = 1;
	}
	if((US1CON0&0x01))  //���ձ�־λ�ж�
	{
		US1CON0 |= 0x01;
		UART1_ISR();
		//Uart1ReceiveFlag = 1;
	}	
}
void UART2_Int() interrupt 16   //Uart2�жϺ���
{
//	uint8_t res;
	Choose_SSI2;
  if(USXCON0 & 0x02)  //���ͱ�־λ�ж�TI
  {
    USXCON0 |= 0x02;
    Uart2SendFlag = 1;
  }
  if((USXCON0 & 0x01)) //���ձ�־λ�ж�RI
  {
    USXCON0 |= 0x01;
		
		UART2_ISR();

    //Uart2ReceiveFlag = 1;
  }
	USXINX_Clear;
}

void UART3_Int() interrupt 17   //Uart3�жϺ���
{
	Choose_SSI3;
  if(USXCON0 & 0x02)  //���ͱ�־λ�ж�TI
  {
    USXCON0 |= 0x02;
    Uart3SendFlag = 1;
  }
  if((USXCON0 & 0x01)) //���ձ�־λ�ж�RI
  {
    USXCON0 |= 0x01;
  }
	USXINX_Clear;
}
/*
void UART5_Int() interrupt 19   //Uart5�жϺ���
{
	uint8_t res;
	Choose_SSI5;
  if(USXCON0 & 0x02)  //���ͱ�־λ�ж�TI
  {
    USXCON0 |= 0x02;
    Uart5SendFlag = 1;
  }
  if((USXCON0 & 0x01)) //���ձ�־λ�ж�RI
  {
    USXCON0 |= 0x01;
		res=USXCON3;
    Uart5_Rx[uart5_rx_count]=res; 
    uart5_rx_count++;
    if (uart5_rx_count >= UART5_MAX_LEN) {
        //��ֹ���
        uart5_rx_count = 0;
		}
    Uart5ReceiveFlag = 1;
  }
	USXINX_Clear;
}*/