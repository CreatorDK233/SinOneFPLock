#include "Function_Init.H"

//ѡ������һģʽ��
#define  Uart1   0
#define  TWI1    1
#define  SPI1    2

#define  SSI1_Mode  Uart1

void Uart1_Init(uint Freq,unsigned long int baud);
void TWI1_Init(void);
void SPI1_Init(void);

bit Uart1SendFlag = 0;    //Uart1�����жϱ�־λ
bit Uart1ReceiveFlag = 0; //Uart1�����жϱ�־λ
bit SPI1Flag = 0;         //SPI1���ݴ�����ɱ�־λ
bit TWI1Flag = 0;         //TWI1�жϱ�־λ

/*****************************************************
*�������ƣ�void SSI1_Test(void)
*�������ܣ�SSI1����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SSI1_Test(void)
{
#if (SSI1_Mode == Uart1)
	Uart1_Init(32,9600);
	while(1)
	{
		US1CON3 = 0xAA;
		while(!Uart1SendFlag);
		Uart1SendFlag = 0;
	}
#endif
	
#if (SSI1_Mode == TWI1)
	TWI1_Init();
	while(1)
	{
		US1CON1 |= 0x20;    //������ʼ����
				while(!TWI1Flag);
        TWI1Flag = 0;		
		US1CON3 = 0x10;	    ////���͵�ַ��д����
        while(!TWI1Flag);
        TWI1Flag = 0;
    US1CON3 = 0x55;
        while(!TWI1Flag);
        TWI1Flag = 0;
		Soft_Delay_us(100);
		US1CON1 |= 0x10;
		Soft_Delay_us(100);
	}
#endif

#if (SSI1_Mode == SPI1)
	SPI1_Init();
	while(1)
	{
		US1CON2 = 0xAA;
		while(!SPI1Flag);
		SPI1Flag = 0;
        Soft_Delay_us(1000);
	}
#endif	
}
/*****************************************************
*�������ƣ�void Uart1_Init(uint Freq,unsigned long int baud)
*�������ܣ�Uart1�жϳ�ʼ��
*��ڲ�����Freq-��Ƶ��baud-������
*���ڲ�����void
*****************************************************/
void Uart1_Init(uint Freq,unsigned long int baud)
{
	P1CON &= 0xF5;    //TX/RX����Ϊ���������
	P1PH  |= 0x0A;
	
	OTCON |= 0xC0;    //���нӿ�SSI1ѡ��Uart1ͨ��
	US1CON0 = 0x50;   //����ͨ�ŷ�ʽΪģʽһ���������
	US1CON1 = Freq*1000000/baud;   //�����ʵ�λ����
	US1CON2 = (Freq*1000000/baud)>>8;   //�����ʸ�λ����
	IE2 |= 0x01;      //����SSI1�ж�
    EA = 1;	
}

/*****************************************************
*�������ƣ�void TWI1_Init(void)
*�������ܣ�TWI1��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void TWI1_Init(void)
{
	OTCON |= 0x80;   //ѡ��TWI1ģʽ
	US1CON0 = 0x80;  //��ģʽ�� ʹ��Ӧ���־λ
	US1CON1 = 0x05;  //---- xxxx  xΪʱ������
	IE2 |= 0x01;
	EA = 1;
}

/*****************************************************
*�������ƣ�void SPI1_Init(void)
*�������ܣ�SPI1��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SPI1_Init(void)
{
	OTCON |= 0X40;    //ѡ��SPI1ģʽ
	US1CON0 = 0x3F;   //����SPI1Ϊ���豸��SCK����ʱ��Ϊ�ߵ�ƽ��SCK���ڵڶ��زɼ����ݣ�ʱ������ΪFsys/128
	US1CON1 = 0x01;   //MSB���ȷ��ͣ�8λ���䣬�������ж�
	US1CON0 |= 0x80;  //����SPI1
	IE2 |= 0x01;
	EA = 1;
}


/*****************************************************
*�������ƣ�void TWI1/PI/UART1_Int() interrupt 7
*�������ܣ�SSI1�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
#if (SSI1_Mode == Uart1)
void Uart1_Int() interrupt 15   //Uart1�жϺ���
{
	if(US1CON0&0x02)    //���ͱ�־λ�ж�
	{
		US1CON0 &= 0xFE;
		Uart1SendFlag = 1;
	}
	if((US1CON0&0x01))  //���ձ�־λ�ж�
	{
		US1CON0 &= 0xFD;
		Uart1ReceiveFlag = 1;
	}	
}
#endif

#if (SSI1_Mode == TWI1)
void TWI1_Int() interrupt 15     //TWI1�жϺ���
{
	if(US1CON0&0x40)
	{
		US1CON0 &= 0xbf;  //�ж�����
		TWI1Flag = 1;
	}	
}
#endif 

#if (SSI1_Mode == SPI1)
void Spi1_Int(void) interrupt 15    //SPI1�жϺ���
{	  
	if(US1CON1&0X08)    //���ͻ������ձ�־�ж�
	{
		US1CON1 &= ~0X08;
	}
	if(US1CON1&0X80)    //���ݴ����־λ�ж�
	{
		US1CON1 &= ~0X80;
		SPI1Flag = 1;
	}
}
#endif 