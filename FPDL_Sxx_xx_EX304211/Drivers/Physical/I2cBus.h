#ifndef I2cBus_h
#define I2cBus_h

#include "StdTypes.h"

#define I2CADDR_CSK14S		0x2A
#define I2CADDR_TSM12		0xD0
#define I2CADDR_TSM12S		0xD0
#define I2CADDR_MPR121 	0xB4
//#define I2CADDR_MPR121 	0xB8
#define I2CADDR_RTC			0xA2
#define I2CADDR_E2PROM 	0xA8	//0xA0
//#define I2CADDR_MFC			0x5E
#define I2CADDR_MFC			0x50

void I2C_WriteRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t Value);
void I2C_WriteSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num);
void I2C_ReadRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t *Point);
void I2C_ReadSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num);

#endif
