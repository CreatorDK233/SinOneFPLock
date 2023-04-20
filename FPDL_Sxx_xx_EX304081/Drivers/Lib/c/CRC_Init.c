#include "H/Function_Init.H"

char Array[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
typedef struct
{
    char a3;        //��λ��ַ
    char a2;        //�θߵ�ַ
    char a1;        //�ε͵�ַ
    char a0;        //��λ��ַ
}Value_Typedef;    
typedef union
{
    Value_Typedef reg; 
    unsigned long int result;  //���ս��
}Result_Typedef;

unsigned long int temp = 0x00;  //���ս��
Result_Typedef CRC_Result;

void CRC_Hardware_Init(void);
void CRC_Software_Init(char array[]);

/*****************************************************
*�������ƣ�void CRC_Test(void)
*�������ܣ�CRC����
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void CRC_Test(void)
{
    //Ӳ��APPROM����У��
    CRC_Hardware_Init();
    //����Զ�������У��
    CRC_Software_Init(Array);
	
		while(1);
}

/*****************************************************
*�������ƣ�void CRC_Hardware_Init(void)
*�������ܣ�CRCӲ��У���ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void CRC_Hardware_Init(void)
{
    bit EABIT = EA;
    EA = 0;
    OPERCON |= 0x01;  //����Ӳ��У��
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    CRCINX = 0x00;
    CRC_Result.reg.a0 = CRCREG;   //����byte������
    CRC_Result.reg.a1 = CRCREG;   //�ε���byte������
    CRC_Result.reg.a2 = CRCREG;   //�θ���byte������
    CRC_Result.reg.a3 = CRCREG;   //����byte������
    temp = CRC_Result.result;     //����������
    EA = EABIT;
}

/*****************************************************
*�������ƣ�void CRC_Software_Init(char array[])
*�������ܣ�CRC���У���ʼ��
*��ڲ�����void
*���ڲ�����void
*****************************************************/
void CRC_Software_Init(char array[])
{
    int i = 0;
    bit EABIT = EA;
    EA = 0;
    OPERCON |= 0x02;             //�������У��
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();  
    for(i=0; i<16; i++)      //У�鷶Χ
    {
        CRCREG = array[i];   //��ҪУ��ĵ�ֵַ�ŵ��Ĵ���
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();
        _nop_();            
    }
    CRCINX = 0x00;
    CRC_Result.reg.a0 = CRCREG;   //����byte������
    CRC_Result.reg.a1 = CRCREG;   //�ε���byte������
    CRC_Result.reg.a2 = CRCREG;   //�θ���byte������
    CRC_Result.reg.a3 = CRCREG;   //����byte������
    temp = CRC_Result.result;     //����������
    EA = EABIT;
}