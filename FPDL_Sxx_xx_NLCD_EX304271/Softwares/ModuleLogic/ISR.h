#ifndef __ISR__
#define __ISR__	1

//extern uint8_t UART2_RXBUFFER[2];

//extern void TIM1_ISR(void);
extern void TIM3_ISR(void);
extern void SysTick_ISR(void);
extern void UART1_ISR(void);
extern void UART2_ISR(void);
extern void UART3_ISR(void);

#endif
