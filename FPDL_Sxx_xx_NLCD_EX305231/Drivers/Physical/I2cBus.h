#ifndef I2cBus_h
#define I2cBus_h

#include "StdTypes.h"
#include "Project.h"

#define I2CADDR_CSK14S		0x2A
#define I2CADDR_TSM12		0xD0
#define I2CADDR_TSM12S		0xD0
#define I2CADDR_MPR121 	0xB4
//#define I2CADDR_MPR121 	0xB8
#define I2CADDR_RTC			0xA2
#define I2CADDR_E2PROM 	0xA8	//0xA0
//#define I2CADDR_MFC			0x5E
#ifdef Function_NFCUsed_YC5018
#define I2CADDR_MFC			(0x28 << 1)
#else
#define I2CADDR_MFC			0x50
#endif

void I2C_WriteRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t Value);
void I2C_WriteSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num);
void I2C_ReadRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t *Point);
void I2C_ReadSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num);
void WRITEREG(uint8_t addr, uint8_t data_m);
uint8_t READREG(uint8_t addr);

#endif
