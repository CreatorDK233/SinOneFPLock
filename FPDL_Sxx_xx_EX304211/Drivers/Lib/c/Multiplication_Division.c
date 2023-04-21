#include "Function_Init.H"

//unsigned long product = 0;//乘积
unsigned long quotient = 0;//商
unsigned int remainder = 0;//余数

typedef struct			
{
  unsigned char MDU_EXA3Reg;	 //EXA3寄存器
  unsigned char MDU_EXA2Reg;	 //EXA2寄存器
  unsigned char MDU_EXA1Reg;	 //EXA1寄存器
  unsigned char MDU_EXA0Reg;	 //EXA0寄存器
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
*函数名称：Multiplication(unsigned int faciend, unsigned int Multiplier)
*函数功能：乘法运算
*入口参数：faciend，Multiplier
*出口参数：product
****************************************************/
unsigned long Multiplication(unsigned int faciend, unsigned int Multiplier)
{
	unsigned long result;
	
  MDU_Temp_Union MDU_ProductTemp;
    OPERCON &= ~0x40;   //选择乘法
    
	EXA0 = faciend;
	EXA1 = faciend>>8;
	EXBL = Multiplier;
	EXBH = Multiplier>>8;

	OPERCON |= 0x80;      //开始计算
	while(OPERCON & 0x80);			//等待计算完成

  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA0Reg = EXA0;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA1Reg = EXA1;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA2Reg = EXA2;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA3Reg = EXA3;
	
  result =  MDU_ProductTemp.MDU_Temp;			//获取乘积
	
	return result;
}	

/****************************************************
*函数名称：Division(unsigned long dividend,unsigned int divisor)
*函数功能：除法运算
*入口参数：dividend，divisor
*出口参数：无
****************************************************/
void Division(unsigned long dividend,unsigned int divisor)
{
  MDU_Temp_Union MDU_ProductTemp;
	MDU_ProductTemp.MDU_Temp = dividend;

	OPERCON &= ~0x40;
	OPERCON |= 0x40;    //选择除法
	
  EXA0 = MDU_ProductTemp.MDU_EXAxReg.MDU_EXA0Reg;			//输入除数
  EXA1 = MDU_ProductTemp.MDU_EXAxReg.MDU_EXA1Reg;
  EXA2 = MDU_ProductTemp.MDU_EXAxReg.MDU_EXA2Reg;
  EXA3 = MDU_ProductTemp.MDU_EXAxReg.MDU_EXA3Reg;

	EXBL = divisor;			//输入被除数
	EXBH = divisor>>8;

	OPERCON |= 0xC0;      //开始计算
	while(OPERCON & 0x80);			//等待计算完成

  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA0Reg = EXA0;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA1Reg = EXA1;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA2Reg = EXA2;
  MDU_ProductTemp.MDU_EXAxReg.MDU_EXA3Reg = EXA3;

	remainder = EXBH*256+ EXBL;			//余数
	quotient  = MDU_ProductTemp.MDU_Temp;			//商
}

unsigned long DivisionToQuotient(unsigned long dividend,unsigned int divisor)//商
{
	Division(dividend,divisor);
	return quotient;
}

unsigned int DivisionToRemainder(unsigned long dividend,unsigned int divisor)//余数
{
	Division(dividend,divisor);
	return remainder;
}


