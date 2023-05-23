#include "H/Function_Init.H"

char Array[] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
typedef struct
{
    char a3;        //高位地址
    char a2;        //次高地址
    char a1;        //次低地址
    char a0;        //低位地址
}Value_Typedef;    
typedef union
{
    Value_Typedef reg; 
    unsigned long int result;  //最终结果
}Result_Typedef;

unsigned long int temp = 0x00;  //最终结果
Result_Typedef CRC_Result;

void CRC_Hardware_Init(void);
void CRC_Software_Init(char array[]);

/*****************************************************
*函数名称：void CRC_Test(void)
*函数功能：CRC测试
*入口参数：void
*出口参数：void
*****************************************************/
void CRC_Test(void)
{
    //硬件APPROM区域校验
    CRC_Hardware_Init();
    //软件自定义输入校验
    CRC_Software_Init(Array);
	
		while(1);
}

/*****************************************************
*函数名称：void CRC_Hardware_Init(void)
*函数功能：CRC硬件校验初始化
*入口参数：void
*出口参数：void
*****************************************************/
void CRC_Hardware_Init(void)
{
    bit EABIT = EA;
    EA = 0;
    OPERCON |= 0x01;  //启动硬件校验
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    _nop_();

    CRCINX = 0x00;
    CRC_Result.reg.a0 = CRCREG;   //低两byte结果存放
    CRC_Result.reg.a1 = CRCREG;   //次低两byte结果存放
    CRC_Result.reg.a2 = CRCREG;   //次高两byte结果存放
    CRC_Result.reg.a3 = CRCREG;   //高两byte结果存放
    temp = CRC_Result.result;     //最终运算结果
    EA = EABIT;
}

/*****************************************************
*函数名称：void CRC_Software_Init(char array[])
*函数功能：CRC软件校验初始化
*入口参数：void
*出口参数：void
*****************************************************/
void CRC_Software_Init(char array[])
{
    int i = 0;
    bit EABIT = EA;
    EA = 0;
    OPERCON |= 0x02;             //启动软件校验
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
    for(i=0; i<16; i++)      //校验范围
    {
        CRCREG = array[i];   //把要校验的地址值放到寄存器
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
    CRC_Result.reg.a0 = CRCREG;   //低两byte结果存放
    CRC_Result.reg.a1 = CRCREG;   //次低两byte结果存放
    CRC_Result.reg.a2 = CRCREG;   //次高两byte结果存放
    CRC_Result.reg.a3 = CRCREG;   //高两byte结果存放
    temp = CRC_Result.result;     //最终运算结果
    EA = EABIT;
}