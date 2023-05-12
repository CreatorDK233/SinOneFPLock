#ifndef _BSP_IO_I2C_H_
#define _BSP_IO_I2C_H_

#include "gpio.h"
#include "StdTypes.h"
#include "SC95F861xB_C.H"

#define I2CADDR_E2PROM 	0xA8//0xA0
#define IIC_SDA_PORT PORT2
#define IIC_SCL_PORT PORT2
#define IIC_SDA_PIN PIN5
#define IIC_SCL_PIN PIN4
#define IO_IIC_SCL PORT2,PIN4
#define IO_IIC_SDA PORT2,PIN5

#define IO_IIC_SCL_H P2CON &=~(1<<4)
#define IO_IIC_SCL_L P2CON |=(1<<4)
#define IO_IIC_SDA_H P2CON &=~(1<<5)
#define IO_IIC_SDA_L P2CON |=(1<<5)

#define SDA_IO P25

void IIC_START(void);
void IIC_STOP(void);
uint8_t IIC_Wait_ACK(void);
void IIC_ACK(void);
void IIC_NACK(void);
void IIC_SEND_8Bits(uint8_t IIC_Data_8b);
uint8_t IIC_READ_8Bits(void);
uint8_t IIC_READ(uint8_t send1,uint8_t send2,uint8_t send3,uint8_t *Pdata,uint16_t Num);
//uint8_t IIC_READ2(uint8_t send1,uint8_t send2,uint8_t send3,uint8_t send4,uint8_t *Pdata,uint16_t Num);
uint8_t IIC_SEND(uint8_t send1,uint8_t send2,uint8_t *_data,uint16_t Num);
//uint8_t IIC_SEND(uint8_t send1,uint8_t send2,uint8_t _data);
//uint8_t IIC_SEND2(uint8_t send1,uint8_t send2,uint8_t send3,uint8_t _data);

#endif

