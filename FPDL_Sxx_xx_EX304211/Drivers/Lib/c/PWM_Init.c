#include "H/Function_Init.H"

unsigned int xdata PWMREG[14] _at_ 0x2034;	//PWM占空比调节寄存器

void PWM_Init(void);
/*****************************************************
*函数名称：void PWM_Test(void)
*函数功能：PWM测试
*入口参数：void
*出口参数：void
*****************************************************/
void PWM_Test(void)
{
  PWM_Init();
  while(1)
  {
  }
}

/*****************************************************
*函数名称：void PWM_Init(void)
*函数功能：PWM独立模式初始化
*入口参数：void
*出口参数：void
*****************************************************/
void PWM_Init(void)
{
  //设置了周期为500us，占空比为50%的PWM0波形
  //pwm00，PWM01独立输出
  PWMCON0 = 0x00;     //分频系数设置为1分频，独立模式，边沿对齐
  PWMCFG |= 0x02;     //PWM00不反向，PWM01反向
  PWMCON1 |= 0x03;      //PWM00，PMW01输出
  PWMPDL = 0x80;      //周期配置低8位
  PWMPDH = 0x0C;      //周期配置高8位
  PWMREG[6] = 0x640;      //PWM00占空比
  PWMREG[7] = 0x640;      //PWM01占空比
  PWMCON0 |= 0x80;     //使能PWM0
  IE1 |= 0x02;        //开启中断标志位

  //设置了周期为1000us，占空比为50%的PWM2波形
  TXINX = 0x02;     //TimerX寄存器组指向TIM2
  THX = 0X00;     //计数器高8位清零
  TLX =  0x00;    //计数器低8位清零
  TXMOD |= 0x30;      //使能PWM20，PWM21波形输出
  TXMOD |= 0x08;      //PWM21输出反向
  RCAPXH = 0x3E;      //配置周期寄存器高8位
  RCAPXL = 0x80;      //配置周期寄存器低8位
  PWMREG[0] = 0x1F40;      //配置PWM20占空比
  PWMREG[1] = 0x1F40;      //配置PWM21占空比
  TXCON |= 0x04;        //使能PWM2
  ET2 = 0;        //关闭PWM2中断

  //设置了周期为2000us，占空比为50%的PWM3波形
  TXINX = 0x03;     //TimerX寄存器组指向TIM3
  THX = 0X00;     //计数器高8位清零
  TLX =  0x00;    //计数器低8位清零
  TXMOD |= 0x30;      //使能PWM30，PWM31波形输出
  TXMOD |= 0x08;      //PWM31输出反向
  RCAPXH = 0x7D;      //配置周期寄存器高8位
  RCAPXL = 0x00;      //配置周期寄存器低8位
  PWMREG[2] = 0x3E80;      //配置PWM30占空比
  PWMREG[3] = 0x3E80;      //配置PWM31占空比
  TXCON |= 0x04;        //使能PWM3
  IE1 &= ~0x40;         //关闭PWM3中断

  //设置了周期为2000us，占空比为50%的PWM4波形
  TXINX = 0x04;     //TimerX寄存器组指向TIM4
  THX = 0X00;     //计数器高8位清零
  TLX =  0x00;    //计数器低8位清零
  TXMOD |= 0x30;      //使能PWM40，PWM41波形输出
  TXMOD |= 0x80;      //PWM41输出反向
  RCAPXH = 0xFA;      //配置周期寄存器高8位
  RCAPXL = 0x00;      //配置周期寄存器低8位
  PWMREG[4] = 0x7D00;      //配置PWM40占空比
  PWMREG[5] = 0x7D00;      //配置PWM41占空比
  TXCON |= 0x04;        //使能PWM4
  IE1 &= ~0x40;         //关闭PWM4中断

  EA = 1;       //总中断开启
}

/*****************************************************
*函数名称：void PWM_Init(void)
*函数功能：PWM0互补模式初始化
*入口参数：void
*出口参数：void
*****************************************************
void PWM_Init(void)
{
	//设置了周期为50us，占空比为50%的PWM0波形
  //pwm00，PWM01为互补输出
  PWMCON0 = 0x32;     //分频系数设置为8分频，独立模式，边沿对齐
  PWMCFG |= 0x02;     //PWM00不反向，PWM01反向
  PWMCON1 |= 0x03;      //PWM00，PMW01输出
  PWMPDL = 0x40;      //周期配置低8位
  PWMPDH = 0x9C;      //周期配置高8位
  PWMREG[12] = 0x4E;      //PWM00占空比
  IE1 |= 0x02;        //开启中断标志位
  EA = 1;
}
*/

/*****************************************************
*函数名称：void PWM_Init(void)
*函数功能：PWM0故障检测初始化
*入口参数：void
*出口参数：void
*****************************************************/
void PWM_FLT_Init()
{
  PWMFLT = 0xB3;       //开启中断检测，立即模式，故障检测高电平有效，滤波时间为16us
}


/*****************************************************
*函数名称：void PWM_Interrup() interrupt 8
*函数功能：PWM0中断服务函数
*入口参数：void
*出口参数：void
*****************************************************/
void PWM0_Interrup() interrupt 8
{
  if(PWMCON0 & 0x40)
  {
    PWMCON0 &= ~0x40;  //清除中断标志位
    P04 = ~P04;
  }
}