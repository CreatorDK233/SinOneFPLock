#include "Function_Init.H"

//ѡ������һģʽ��
#define  Uart0   0
#define  TWI0    1
#define  SPI0    2

#define  SSI0_Mode  TWI0

void Uart0_Init(uint Freq,unsigned long int baud);
void TWI0_Init(void);
void SPI0_Init(void);

bit Uart0SendFlag = 0;    //Uart0�����жϱ�־λ
bit Uart0ReceiveFlag = 0; //Uart0�����жϱ�־λ
bit SPI0Flag = 0;         //SPI0���ݴ�����ɱ�־λ
bit TWI0Flag = 0;         //TWI0�жϱ�־λ

/*****************************************************
*�������ƣ�void SSI0_Test(void)
*�������ܣ�SSI0����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SSI0_Test(void)
{
#if (SSI0_Mode == Uart0)
	Uart0_Init(32,9600);
	while(1)
	{
		US0CON3 = 0xAA;
		while(!Uart0SendFlag);
		Uart0SendFlag = 0;
	}
#endif
	
#if (SSI0_Mode == TWI0)
	TWI0_Init();
	while(1)
	{
		US0CON1 |= 0x20;    //������ʼ����
		while(!TWI0Flag);
        TWI0Flag = 0;		
		US0CON3 = 0x10;	    //���͵�ַ��д����
        while(!TWI0Flag);
        TWI0Flag = 0;
        US0CON3 = 0x55;	    //��������
        while(!TWI0Flag);
        TWI0Flag = 0;
		Soft_Delay_us(100);
		US0CON1 |= 0x10;
		Soft_Delay_us(100);
	}
#endif

#if (SSI0_Mode == SPI0)
	SPI0_Init();
	while(1)
	{
		US0CON2 = 0xAA;
		while(!SPI0Flag);
		SPI0Flag = 0;
        Soft_Delay_us(1000);
	}
#endif	
}
/*****************************************************
*�������ƣ�void Uart0_Init(uint Freq,unsigned long int baud)
*�������ܣ�Uart0�жϳ�ʼ��
*��ڲ�����Freq-��Ƶ��baud-������
*���ڲ�����void
*****************************************************/
void Uart0_Init(uint Freq,unsigned long int baud)
{
	P0CON &= 0xCF;    //TX/RX����Ϊ���������
	P0PH  |= 0x30;
	
	OTCON |= 0x30;    //���нӿ�SSI0ѡ��Uart0ͨ��
	US0CON0 = 0x50;   //����ͨ�ŷ�ʽΪģʽһ���������
	US0CON1 = Freq*1000000/baud;   //�����ʵ�λ����
	US0CON2 = (Freq*1000000/baud)>>8;   //�����ʸ�λ����
	IE1 |= 0x01;      //����SSI0�ж�
    EA = 1;	
}

/*****************************************************
*�������ƣ�void TWI0_Init(void)
*�������ܣ�TWI0��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void TWI0_Init(void)
{
	OTCON |= 0x20;   //ѡ��TWI0ģʽ
	US0CON0 = 0x80;  //��ģʽ�� ʹ��Ӧ���־λ
	US0CON1 = 0x05;  //---- xxxx  xΪʱ������
	IE1 |= 0x01;
	EA = 1;
}

/*****************************************************
*�������ƣ�void SPI0_Init(void)
*�������ܣ�SPI0��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void SPI0_Init(void)
{
	OTCON |= 0X10;    //ѡ��SPI0ģʽ
	US0CON0 = 0x3B;   //����SPI0Ϊ���豸��SCK����ʱ��Ϊ�ߵ�ƽ��SCK���ڵڶ��زɼ����ݣ�ʱ������ΪFsys/8
	US0CON1 = 0x01;   //MSB���ȷ��ͣ�8λ���䣬�������ж�
	US0CON0 |= 0x80;  //����SPI0
	//IE1 |= 0x01;
	EA = 1;
}


/*****************************************************
*�������ƣ�void TWI0/PI/UART1_Int() interrupt 7
*�������ܣ�SSI0�жϺ���
*��ڲ�����void
*���ڲ�����void
*****************************************************/
#if (0)
void Uart0_Int() //interrupt 7   //Uart0�жϺ���
{
	if(US0CON0&0x02)    //���ͱ�־λ�ж�
	{
		US0CON0 &= 0xFE;
		Uart0SendFlag = 1;
	}
	if((US0CON0&0x01))  //���ձ�־λ�ж�
	{
		US0CON0 &= 0xFD;
		Uart0ReceiveFlag = 1;
	}	
}
#endif

#if (0)
void TWI0_Int() //interrupt 7     //TWI0�жϺ���
{
	if(US0CON0&0x40)
	{
		US0CON0 &= 0xbf;  //�ж�����
		TWI0Flag = 1;
	}	
}
#endif 

#if (0)
void Spi0_Int(void) //interrupt 7    //SPI0�жϺ���
{	  
	if(US0CON1&0X08)    //���ͻ������ձ�־�ж�
	{
		US0CON1 &= ~0X08;
	}
	if(US0CON1&0X80)    //���ݴ����־λ�ж�
	{
		US0CON1 &= ~0X80;
		SPI0Flag = 1;
	}
}
#endif 