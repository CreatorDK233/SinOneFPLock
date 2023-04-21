#include "H/Function_Init.H"

unsigned char xdata LCDRAM[28]  _at_  0x2000;
void LED_Init(void);
void PxVO_Init(void);
	
/*****************************************************
*�������ƣ�void LCD_Test(void)
*�������ܣ�LCD/LED����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void LED_Test(void)
{
	LED_Init();		//LEDģʽ
	while(1)
	{
	}
}

/*****************************************************
*�������ƣ�void LED_Init(void)
*�������ܣ�LED��ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void LED_Init(void)
{
	unsigned int i;
	DDRCON = 0xC0;    //-xyy zzzz  x:LCD/LEDģʽ y:ռ�ձ�1/8:00 1/6:01 1/5:10 1/4:11  z:LCD��ѹ����
	PxVO_Init();
	OTCON |= 0x00;    //---- xxyz  x:LCD��ѹ���� y:��λ�ܽ�ѡ�� z:LCDƫ�õ�ѹ
	for(i=0; i<8; i++)
	{
		LCDRAM[i] = 0xAA;
	}
}

/*****************************************************
*�������ƣ�void PxVO_Init(void)
*�������ܣ���ʾ��������ڳ�ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void PxVO_Init(void)
{
	P0VO = 0xFF;	//��IO��ʹ��ΪLCD/LED�����
	P1VO = 0xFF;
	P2VO = 0xFF;
	P3VO = 0xFF;
}