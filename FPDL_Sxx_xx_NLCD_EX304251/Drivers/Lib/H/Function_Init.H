#ifndef _Function_Init_H_
#define _Function_Init_H_

#include "SC95F861xB_C.H"
#include "intrins.H"
#include "Basic_Function.h"

//extern bit UartXSendFlag;
//extern bit UartXReceiveFlag;

//extern bit SPIXFlag;
extern bit Tim3_Flag;
extern uint8_t SSIxx_Select;
extern uint8_t SSIxx_Mode;

extern void SPI0_Init(void);
extern void UARTX_Init(uint Freq, unsigned long int baud);
extern void SPIX_Init(void);
extern unsigned char IAP_Read(unsigned long Add, unsigned char Iapade);
//extern unsigned long Multiplication(unsigned int faciend, unsigned int Multiplier);
//extern unsigned long DivisionToQuotient(unsigned long dividend,unsigned int divisor);
//extern unsigned int DivisionToRemainder(unsigned long dividend,unsigned int divisor);

/*****************************************
ËµÃ÷£ºÑ¡Ôñ²âÊÔIC
*****************************************/
#define SC95F8617B  0
#define SC95F8616B  1
#define SC95F8615B  2
#define SC95F8613B  3

#define IC_MODEL  SC95F8617B

#endif