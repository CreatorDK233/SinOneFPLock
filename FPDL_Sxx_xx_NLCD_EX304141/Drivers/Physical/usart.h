#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "StdTypes.h"
	 
#define UART_MAX_LEN 128
	 
extern void Uart1_Init(uint Freq,unsigned long int baud);
extern void UARTX_Init(uint Freq, unsigned long int baud);
extern uint8_t SSIxx_Select;
extern uint8_t SSIxx_Mode;

void MX_UART1_Init(void);
void MX_UART1_DeInit(void);
void MX_UART2_Init(void);
void MX_UART2_DeInit(void);
void MX_UART3_Init(void);
void MX_UART3_DeInit(void);
	 
void Uart1SendByte(uint8_t dat);
void Uart1SendStr(uint8_t *pstr,uint16_t strlen);
void Uart1ReadByte(uint8_t *dat);	 
void Uart2SendByte(uint8_t dat);
void Uart2SendStr(uint8_t *pstr,uint16_t strlen);
void Uart2ReadByte(uint8_t *dat);
void Uart3SendByte(uint8_t dat);
void Uart3SendStr(uint8_t *pstr,uint16_t strlen);
void Uart3ReadByte(uint8_t *dat);


#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

