#include "Function_Init.H"

//ѡ������һģ�飺
#define  USCI2   2
#define  USCI3   3
#define  USCI4   4
#define  USCI5   5

//ѡ������һģʽ��
#define  UartX   0
#define  TWIX    1
#define  SPIX    2

#define  SSIX_Select USCI3
#define  SSIX_Mode  2

void UARTX_Init(uint Freq, unsigned long int baud);
void TWIX_Init(void);
void SPIX_Init(void);

bit UartXSendFlag = 0;    //UartX�����жϱ�־λ
bit UartXReceiveFlag = 0; //UartX�����жϱ�־λ

bit SPIXFlag = 0;         //SPIX���ݴ�����ɱ�־λ
bit TWIXFlag = 0;         //TWIX�жϱ�־λ

uint8_t SSIxx_Select = 2;
uint8_t SSIxx_Mode = 0;

/*****************************************************
*�������ƣ�void SSIX_Test(void)
*�������ܣ�SSIX����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SSIX_Test(void)
{
  USXINX = SSIX_Select;       //USCIX�Ĵ���ָ���Ӧ��USCI

//�ж�����һģʽ
#if (SSIX_Mode == UartX)
  {
    UARTX_Init(32, 9600);      //��ʼ�����ڣ�������Ƶ�Ͳ�����
    while(1)
    {
      USXCON3 = 0xAA;         //����0xAA
      while(!UartXSendFlag);        //�ȴ�������ɱ�־λ����
      UartXSendFlag = 0;      //������ͱ�־λ
    }
  }
#elif (SSIX_Mode == TWIX)
  {
    TWIX_Init();
    while(1)
    {
      USXCON1 |= 0x20;    //������ʼ����
      while(!TWIXFlag);
      TWIXFlag = 0;
      USXCON3 = 0x10;	    //���͵�ַ��д����
      while(!TWIXFlag);
      TWIXFlag = 0;
      USXCON3 = 0x55;
      while(!TWIXFlag);
      TWIXFlag = 0;
      Soft_Delay_us(100);
      USXCON1 |= 0x10;        //���ͽ����ź�
      Soft_Delay_us(100);
    }
  }
#elif (SSIX_Mode == SPIX)
  {
    SPIX_Init();
    while(1)
    {
      USXCON2 = 0xAA;
      while(!SPIXFlag);
      SPIXFlag = 0;
      Soft_Delay_us(1000);
    }
  }
#endif

}

/*****************************************************
*�������ƣ�void UARTX_Init(uint Freq,unsigned long int baud)
*�������ܣ�UartX�жϳ�ʼ��
*��ڲ�����Freq-��Ƶ��baud-������
*���ڲ�����void
*****************************************************/
void UARTX_Init(uint Freq, unsigned long int baud)
{
	TMCON &= 0x3F;
  TMCON |= 0xC0;    //���нӿ�SSIXѡ��Uartͨ��
	if (SSIxx_Select == USCI2)
	{
  P4CON &= ~0x30;
  P4PH |= 0x30;
  IE2 |= 0x02;      //�����ж�
	}
	else if (SSIxx_Select == USCI3)
	{
  P2CON &= ~0xC0;
  P2PH |= 0xC0;
  IE2 |= 0x04;      //�����ж�
	}
	else if (SSIxx_Select == USCI4)
	{
  P4CON &= ~0xC0;
  P4PH |= 0xC0;
  IE2 |= 0x08;     //�����ж�
	}
	else if (SSIxx_Select == USCI5)
	{
  P0CON &= ~0x0C;
  P0PH |= 0x0C;
  IE2 |= 0x10;     //�����ж�
	}

  USXCON0 = 0x50;   //����ͨ�ŷ�ʽΪģʽһ���������
  USXCON1 = Freq * 1000000 / baud; //�����ʵ�λ����
  USXCON2 = (Freq * 1000000 / baud) >> 8; //�����ʸ�λ����

  EA = 1;
}

/*****************************************************
*�������ƣ�void TWIX_Init(void)
*�������ܣ�TWIX��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void TWIX_Init(void)
{
	TMCON &= 0x3F;
  TMCON |= 0x80;    //���нӿ�SSIXѡ��TWIͨ��
	if (SSIxx_Select == USCI2)
	{
		P4CON &= ~0x18;
		P4PH |= 0x18;
		IE2 |= 0x02;      //�����ж�
	}
	else if (SSIxx_Select == USCI3)
	{
		P2CON &= ~0x60;
		P2PH |= 0x60;
		IE2 |= 0x04;      //�����ж�
	}
	else if (SSIxx_Select == USCI4)
	{
  P5CON &= ~0x20;
  P5PH |= 0x20;
  P4CON &= ~0x70;
  P4PH |= 0x70;
  IE2 |= 0x08;     //�����ж�
	}
	else if (SSIxx_Select == USCI5)
	{
  P0CON &= ~0x06;
  P0PH |= 0x06;
  IE2 |= 0x10;     //�����ж�
	}

  TMCON |= 0x80;   //ѡ��TWIXģʽ
  USXCON0 = 0x80;  //��ģʽ�� ʹ��Ӧ���־λ
  USXCON1 = 0x05;  //---- xxxx  xΪʱ������
  IE2 |= 1 << (SSIxx_Select - 1);    //�����ж�
  EA = 1;
}

/*****************************************************
*�������ƣ�void SPIX_Init(void)
*�������ܣ�SPIX��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SPIX_Init(void)
{
	TMCON &= 0x3F;
  TMCON |= 0x40;    //���нӿ�SSIXѡ��SPIͨ��
	if (SSIxx_Select == USCI2)
	{
  P4CON &= ~0x38;
  P4PH |= 0x38;
  IE2 |= 0x02;      //�����ж�
	}
	else if (SSIxx_Select == USCI3)
	{
  P2CON &= ~0xE0;
  P2PH |= 0xE0;
  //IE2 |= 0x04;      //�����ж�
	}
	else if (SSIxx_Select == USCI4)
	{
  P5CON &= ~0x20;
  P5PH |= 0x20;
  P4CON &= ~0xC0;
  P4PH |= 0xC0;
  IE2 |= 0x08;     //�����ж�
	}
	else if (SSIxx_Select == USCI5)
	{
  P0CON &= ~0x0E;
  P0PH |= 0x0E;
  IE2 |= 0x10;     //�����ж�
	}

  TMCON |= 0X40;    //ѡ��SPIXģʽ
  USXCON0 = 0x3B;   //����SPIXΪ���豸��SCK����ʱ��Ϊ�ߵ�ƽ��SCK���ڵڶ��زɼ����ݣ�ʱ������ΪFsys/8
  USXCON1 = 0x01;   //MSB���ȷ��ͣ�8λ���䣬���������ж�
  USXCON0 |= 0x80;  //����SPIX
  //IE2 |= 1 << (SSIxx_Select - 1);    //�����ж�
  EA = 1;
}

#if (0)
/*****************************************************
*�������ƣ�void TWI2/SPI2/UART2_Int() interrupt 7
*�������ܣ�SSI2�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
#if (SSIX_Mode == UartX)
void UART2_Int() //interrupt 16   //UartX�жϺ���
{
  if(USXCON0 & 0x02)  //���ͱ�־λ�ж�
  {
    USXCON0 &= 0xFE;
    UartXSendFlag = 1;
  }
  if((USXCON0 & 0x01)) //���ձ�־λ�ж�
  {
    USXCON0 |= 0xFD;
    UartXReceiveFlag = 1;
  }
}
#elif (SSIX_Mode == TWIX)
void TWI2_Int() //interrupt 16     //TWIX�жϺ���
{
  if(USXCON0 & 0x40)
  {
    USXCON0 &= 0xbf;  //�ж�����
    TWIXFlag = 1;
  }
}
#elif (SSIX_Mode == SPIX)
void SPI2Int(void) //interrupt 16    //SPIX�жϺ���
{	  
	if(USXCON1&0X08)    //���ͻ������ձ�־�ж�
	{
		USXCON1 &= ~0X08;
	}
	if(USXCON1&0X80)    //���ݴ����־λ�ж�
	{
		USXCON1 &= ~0X80;
		SPIXFlag = 1;
	}
}
#endif
#elif (0)
/*****************************************************
*�������ƣ�void TWI3/SPI3/UART3_Int() interrupt 7
*�������ܣ�SSI2�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
#if (SSIX_Mode == UartX)
void UART3_Int() //interrupt 17   //UartX�жϺ���
{
  if(USXCON0 & 0x02)  //���ͱ�־λ�ж�
  {
    USXCON0 &= 0xFE;
    UartXSendFlag = 1;
  }
  if((USXCON0 & 0x01)) //���ձ�־λ�ж�
  {
    USXCON0 |= 0xFD;
    UartXReceiveFlag = 1;
  }
}
#elif (SSIX_Mode == TWIX)
void TWI3_Int() //interrupt 17     //TWIX�жϺ���
{
  if(USXCON0 & 0x40)
  {
    USXCON0 &= 0xbf;  //�ж�����
    TWIXFlag = 1;
  }
}
#elif (SSIX_Mode == SPIX)
void SPI3_Int(void) //interrupt 17    //SPIX�жϺ���
{	  
   if(USXCON1&0X08)    //���ͻ������ձ�־�ж�
   {
      USXCON1 &= ~0X08;
   }
   if(USXCON1&0X80)    //���ݴ����־λ�ж�
   {
      USXCON1 &= ~0X80;
      SPIXFlag = 1;
   }
}
#endif
#elif (0)
/*****************************************************
*�������ƣ�void TWI4/SPI4/UART4_Int() interrupt 7
*�������ܣ�SSI2�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
#if (SSIX_Mode == UartX)
void UART4_Int() //interrupt 18   //UartX�жϺ���
{
  if(USXCON0 & 0x02)  //���ͱ�־λ�ж�
  {
    USXCON0 &= 0xFE;
    UartXSendFlag = 1;
  }
  if((USXCON0 & 0x01)) //���ձ�־λ�ж�
  {
    USXCON0 |= 0xFD;
    UartXReceiveFlag = 1;
  }
}
#elif (SSIX_Mode == TWIX)
void TWI4_Int() //interrupt 18     //TWIX�жϺ���
{
  if(USXCON0 & 0x40)
  {
    USXCON0 &= 0xbf;  //�ж�����
    TWIXFlag = 1;
  }
}
#elif (SSIX_Mode == SPIX)
void SPI4_Int(void) //interrupt 18   //SPIX�жϺ���
{	  
	if(USXCON1&0X08)    //���ͻ������ձ�־�ж�
	{
		USXCON1 &= ~0X08;
	}
	if(USXCON1&0X80)    //���ݴ����־λ�ж�
	{
		USXCON1 &= ~0X80;
		SPIXFlag = 1;
	}
}
#endif

#elif (0)
#if (SSIX_Mode == UartX)
void UART5_Int() //interrupt 19   //UartX�жϺ���
{
  if(USXCON0 & 0x02)  //���ͱ�־λ�ж�
  {
    USXCON0 &= 0xFE;
    UartXSendFlag = 1;
  }
  if((USXCON0 & 0x01)) //���ձ�־λ�ж�
  {
    USXCON0 |= 0xFD;
    UartXReceiveFlag = 1;
  }
}
#elif (SSIX_Mode == TWIX)
void TWI5_Int() //interrupt 19     //TWIX�жϺ���
{
  if(USXCON0 & 0x40)
  {
    USXCON0 &= 0xbf;  //�ж�����
    TWIXFlag = 1;
  }
}
#elif (SSIX_Mode == SPIX)
void SPI5_Int(void) //interrupt 19   //SPIX�жϺ���
{	  
	if(USXCON1&0X08)    //���ͻ������ձ�־�ж�
	{
		USXCON1 &= ~0X08;
	}
	if(USXCON1&0X80)    //���ݴ����־λ�ж�
	{
		USXCON1 &= ~0X80;
		SPIXFlag = 1;
	}
}
#endif

#endif
