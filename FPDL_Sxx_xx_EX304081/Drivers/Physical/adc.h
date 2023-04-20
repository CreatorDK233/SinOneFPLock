#ifndef __adc_H
#define __adc_H
#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include "StdTypes.h"
	 
enum Channel {AIN0=0,AIN1,AIN2,AIN3,AIN4,AIN5,AIN6,AIN7,AIN8,AIN9,AIN10,AIN11,AIN12,AIN13,AIN14,AIN15,VDD4=31};

void ADC_Init(uint8_t Channel);
void ADC_DeInit(void);
uint16_t ADC_Value_Got(void);
uint8_t VoltageGet(uint16_t A2D_Data);
void MX_ADC1_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__ adc_H */
