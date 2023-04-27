#ifndef __tim_H
#define __tim_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "StdTypes.h"
	 
typedef struct
{
	uint16_t Timer0Cnt;
	uint16_t Timer1Cnt;
	uint16_t Timer3Cnt;
}TimerCount_t;

void MX_TIM_Init(void);
void MX_TIM_DeInit(void);
void MX_TIM0_Init(void);
void MX_TIM1_Init(void);
void MX_TIM3_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ tim_H */

