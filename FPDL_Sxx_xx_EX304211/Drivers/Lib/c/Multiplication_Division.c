#include "Function_Init.H"

//unsigned long product = 0;//�˻�
unsigned long quotient = 0;//��
unsigned int remainder = 0;//����

typedef struct			
{
  unsigned char MDU_EXA3Reg;	 //EXA3�Ĵ���
  unsigned char MDU_EXA2Reg;	 //EXA2�Ĵ���
  unsigned char MDU_EXA1Reg;	 //EXA1�Ĵ���
  unsigned char MDU_EXA0Reg;	 //EXA0�Ĵ���
} MDU_EXAxReg_Typedef;	

typedef union
{
  MDU_EXAxReg_Typedef MDU_EXAxReg;
  unsigned long MDU_Temp;
} MDU_Temp_Union;			
//Result_union result;

unsigned long Multiplication(unsigned int faciend, unsigned int Multiplier);
void Division(unsigned long dividend,unsigned int divisor);

void  Multiplier_Divider_Test()
{
	while(1)
	{
		Multiplication(15,16);
//		if(product == 240)
//		{
//			//P03 = ~P03;
//		}
		Division(16,5);
//		if((quotient == 3) && (remainder == 1))
//		{
//			//P04 = ~P04;
//		}
	}
}

/****************************************************
*�������ƣ�Multiplication(unsigned int faciend, unsigned int Multiplier)
*�������ܣ��˷�����
*��ڲ�����faciend��Multiplier
*���ڲ�����product
****************************************************/
unsigned long Multiplication(unsigned int faciend, unsigned int Multiplier)
{
	unsigned long result;
	
  MDU_Temp_Union MDU_ProductTemp;
    OPERCON &= ~0x40;   //ѡ��˷�
    
	EXA0 = faciend;
	EXA1 = faciend>>8;
	EXBL = Multiplier;
	EXBH = Multiplier>>8;

	OPERCON |= 0x80;      //��ʼ����
	while(OPERCON & 0x80);			//�ȴ��������

  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA0Reg = EXA0;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA1Reg = EXA1;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA2Reg = EXA2;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA3Reg = EXA3;
	
  result =  MDU_ProductTemp.MDU_Temp;			//��ȡ�˻�
	
	return result;
}	

/****************************************************
*�������ƣ�Division(unsigned long dividend,unsigned int divisor)
*�������ܣ���������
*��ڲ�����dividend��divisor
*���ڲ�������
****************************************************/
void Division(unsigned long dividend,unsigned int divisor)
{
  MDU_Temp_Union MDU_ProductTemp;
	MDU_ProductTemp.MDU_Temp = dividend;

	OPERCON &= ~0x40;
	OPERCON |= 0x40;    //ѡ�����
	
  EXA0 = MDU_ProductTemp.MDU_EXAxReg.MDU_EXA0Reg;			//�������
  EXA1 = MDU_ProductTemp.MDU_EXAxReg.MDU_EXA1Reg;
  EXA2 = MDU_ProductTemp.MDU_EXAxReg.MDU_EXA2Reg;
  EXA3 = MDU_ProductTemp.MDU_EXAxReg.MDU_EXA3Reg;

	EXBL = divisor;			//���뱻����
	EXBH = divisor>>8;

	OPERCON |= 0xC0;      //��ʼ����
	while(OPERCON & 0x80);			//�ȴ��������

  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA0Reg = EXA0;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA1Reg = EXA1;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA2Reg = EXA2;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA3Reg = EXA3;

	remainder = EXBH*256+ EXBL;			//����
	quotient  = MDU_ProductTemp.MDU_Temp;			//��
}

unsigned long DivisionToQuotient(unsigned long dividend,unsigned int divisor)//��
{
	Division(dividend,divisor);
	return quotient;
}

unsigned int DivisionToRemainder(unsigned long dividend,unsigned int divisor)//����
{
	Division(dividend,divisor);
	return remainder;
}


