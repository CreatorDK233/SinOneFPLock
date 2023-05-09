#include "adc.h"
#include "SC95F861xB_C.H"

bit Adc_Flag = 0;
extern uint16_t a2d_data[1];

void ADC_Init(uint8_t Channel)
{
	a2d_data[0]=0x0929;
	ADCCON = 0X80|Channel;		//����ADC��ADC����Ƶ��Ϊ2M ,ѡ��ChannelλADC������
	if(Channel<8)
	{
		ADCCFG0 = 1<<Channel;   //����Channel��Ϊ������
	}
	else
	{
		ADCCFG1 = 1<<(Channel-8);   //����Channel��Ϊ������
	}
	ADCCFG2 = 0x10;			//ADC����ʱ��Ϊ����ʱ��
	IE |= 0X40;        //����ADC�ж�
	EA = 1;
}
void ADC_DeInit(void)
{
	ADCCON &= (~0X80);
	IE &= (~0X40);        
}
uint16_t ADC_Value_Got(void)
{
	uint16_t ADC_Value = 0x0000;
	ADCCON |= 0X40;   //��ʼADCת��
	while(!Adc_Flag);	         		 //�ȴ� ADCת�����;
	Adc_Flag = 0;
	ADC_Value = (ADCVH<<4)+(ADCVL>>4);
	return ADC_Value;
}

//����Ӳ���Ĳ�ͬ �ú���ϵ��Ҳ��Ҫ���²���
uint8_t VoltageGet(uint16_t A2D_Data)
{
	return ( A2D_Data*2 + 467 )/100;
}

/* ADC1 init function */
void MX_ADC1_Init(void)
{
	ADC_Init(AIN11);
}

void ADC_Interrupt(void) interrupt 6
{
	ADCCON &= ~(0X20);  //���жϱ�־λ
	Adc_Flag = 1;
}
