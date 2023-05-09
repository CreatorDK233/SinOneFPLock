#ifndef _Multiplication_Division_C_H
#define _Multiplication_Division_C_H
#include "SC95F873x_C.H"

#define u32 unsigned long int  
#define u16 unsigned  int
#define u8  unsigned char  

typedef struct 
{
	char a3;
	char a2;
	char a1;
	char a0;
}value;
typedef union 
{
	value reg;
	u32 Result;
}Result_union;

extern void Multiplication(u16 faciend, u16 Multiplier);//乘法运算函数
extern void Division(u32 dividend,u16 divisor);//除法运算函数
extern u32 product;//乘积
extern u32 quotient;//商
extern u16 remainder;//余数
#endif

