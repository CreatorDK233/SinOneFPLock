#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "StdTypes.h"
	 
#define UART3_MAX_LEN 128
extern uint16_t uart3_rx_count;
extern uint8_t xdata Uart3_Rx[UART3_MAX_LEN];	 
	 
extern void UARTX_Init(uint Freq, unsigned long int baud);
extern uint8_t SSIxx_Select;
extern uint8_t SSIxx_Mode;

void MX_UART2_Init(void);
void MX_UART2_DeInit(void);
void MX_UART3_Init(void);
void MX_UART3_DeInit(void);

void Uart2SendByte(uint8_t dat);
void Uart2SendStr(uint8_t *pstr,uint16_t strlen);
void Uart3SendByte(uint8_t dat);
void Uart3SendStr(uint8_t *pstr,uint16_t strlen);


#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

