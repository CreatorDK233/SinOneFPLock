#include "H/Function_Init.H" 

void CMP_Init(uchar CMPIS, uchar CMPRF);

/*************ģ��Ƚ������˱Ƚϵ�ѹѡ��****************/
enum CMPRF {CMPRF=0,CMPRF1,CMPRF2,CMPRF3,CMPRF4,CMPRF5,CMPRF6,CMPRF7,CMPRF8,CMPRF9,CMPRF10,CMPRF11,CMPRF12,CMPRF13,CMPRF14,CMPRF15};

/*************ģ��Ƚ�����������ͨ��ѡ��****************/
enum CMPIS {CMP0=0,CMP1,CMP2,CMP3};

bit CMPSTA = 0;

/*****************************************************
*�������ƣ�void CPM_Test(void)
*�������ܣ�ģ��Ƚ�������
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void CMP_Test(void)
{
	CMP_Init(CMP1,CMPRF8);  //ѡ������ͨ��1�����˵�ѹѡ��1/16VDD
	while(1)
	{
        P04 = ~P04;
		if(CMPCON&0x20)     //�ж�ģ�������״̬
		{
			CMPSTA = 1;     //V+ > V-
		}
		else
		{
			CMPSTA = 0;     //V- > V+
		}
	}
}

/*****************************************************
*�������ƣ�void CMP_Init(uchar CMPIS, uchar CMPRF)
*�������ܣ�ģ��Ƚ�����ʼ��
*��ڲ�����CMPIS=����ͨ����CMPRF=���˵�ѹ
*���ڲ�����void
*****************************************************/
void CMP_Init(uchar CMPIS, uchar CMPRF)
{
	CMPCON = 0x80;   //����ģ��Ƚ�����Դ
	CMPCON |= CMPRF; //ѡ�񸺶˵�ѹ
	CMPCFG = 0x04;   //�������ж�
	CMPCFG |= CMPIS; //��������ͨ��ѡ��	
    
    IE1 |= 0x20;
    EA = 1;
}

void CMP_INTERRUPT() interrupt 12
{
	CMPCON &= ~0x40;	//���жϱ�־λ
    P05 = ~P05;
}