#ifndef _Basic_Function_H_
#define _Basic_Function_H_

#include "StdTypes.h"

extern void Soft_Delay1us(void);
extern void Soft_Delay2us(void);
extern void Soft_Delay3us(void);
extern void Soft_Delay4us(void);
extern void Soft_Delay5us(void);
extern void Soft_Delay_us(unsigned long time);
extern void Soft_Delay_usS(unsigned int time);
extern void Soft_Delay1ms(unsigned int time);
extern void  	Hardware_DelayMs(uint16_t time);
extern void 	Hardware_DelayX1us(uint16_t time);
extern void 	Hardware_DelayX5us(uint16_t time);


#endif