#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "StdTypes.h"

extern void SPI0_Init(void);
	 
void MX_SPI0_Init(void);
void SPI0_DeInit(void);	 
void SPI0SendByte(uint8_t dat);
void SPI0SendStr(uint8_t *pstr,uint8_t strlen);

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */
