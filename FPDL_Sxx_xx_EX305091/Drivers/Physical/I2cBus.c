#include "bsp_io_i2c.h"
#include "I2cBus.h"
#include "Basic_Function.h"

void I2C_WriteRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t Value)
{
	uint8_t i;
	uint8_t Data_send[2];
	Data_send[0] = DeviceAddr&0xFE;	    //发送地址及写命令
  Data_send[1] = RegisterAddr;	  //发送寄存器地址
	for(i=0;i<10;i++)
	{
		if( IIC_SEND(Data_send[0],Data_send[1],&Value,1) != 0 )
			break;
		Soft_Delay_us(100);
	}
}

void I2C_WriteSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num)
{
	uint8_t i;
  uint8_t Data_send[2];
	Data_send[0] = DeviceAddr&0xFE;	    //发送地址及写命令
  Data_send[1] = StartAddr;	  //发送寄存器地址
	for(i=0;i<10;i++)
	{
		if( IIC_SEND(Data_send[0],Data_send[1],BUFF,Num) != 0 )
			break;
		Soft_Delay_us(100);
	}
}

void I2C_ReadRandom(uint8_t DeviceAddr,uint8_t RegisterAddr,uint8_t *Point)
{
	uint8_t i;
	uint8_t Data_send[3];
	Data_send[0] = DeviceAddr&0xFE;	    //发送地址及写命令
  Data_send[1] = RegisterAddr;	  //发送寄存器地址
	Data_send[2] = DeviceAddr|0x01;	    //发送地址及读命令
	for(i=0;i<10;i++)
	{
		if( IIC_READ(Data_send[0],Data_send[1],Data_send[2],Point,1) != 0 )
			break;
		Soft_Delay_us(100);
	}
}

void I2C_ReadSequential(uint8_t DeviceAddr,uint8_t StartAddr,uint8_t *BUFF,uint8_t Num)
{
	uint8_t i;
	uint8_t Data_send[3];
	Data_send[0] = DeviceAddr&0xFE;	    //发送地址及写命令
  Data_send[1] = StartAddr;	  //发送寄存器地址
	Data_send[2] = DeviceAddr|0x01;	    //发送地址及读命令
	for(i=0;i<10;i++)
	{
		if( IIC_READ(Data_send[0],Data_send[1],Data_send[2],BUFF,Num) != 0 )
			break;
	}
}
