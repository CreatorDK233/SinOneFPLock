#include "bsp_io_i2c.h"
#include "Basic_Function.h"

#define Hardware_Delay1us Soft_Delay1us()
#define Hardware_Delay5us	Soft_Delay2us()
#define Hardware_Delay9us	Soft_Delay3us()
#define Hardware_Delay10us Soft_Delay4us()
#define Hardware_Delay20us Soft_Delay4us();Soft_Delay4us()


void IIC_START()
{
	IO_IIC_SCL_H;
	IO_IIC_SDA_H;
	Hardware_Delay10us;
	IO_IIC_SDA_L;
	Hardware_Delay5us;
	IO_IIC_SCL_L;
	Hardware_Delay5us;
}
void IIC_STOP()
{
	IO_IIC_SDA_L;
	IO_IIC_SCL_L;
	Hardware_Delay5us;
	IO_IIC_SDA_L;
	IO_IIC_SCL_H;
	Hardware_Delay5us;
	IO_IIC_SDA_H;
	Hardware_Delay5us;
}
uint8_t IIC_Wait_ACK()
{
	uint16_t WaitTime = 0;
	IO_IIC_SDA_H;//SDAÊäÈë
	Hardware_Delay10us;
	IO_IIC_SCL_H;
	Hardware_Delay10us;
	while(SDA_IO==1)
	{
		WaitTime++;
		if(WaitTime>250)
		{
			IIC_STOP();
			return 0;
	  }
	}
	IO_IIC_SCL_L;

	return 1;
}
void IIC_ACK()
{
	IO_IIC_SCL_L;
	IO_IIC_SDA_L;
	Hardware_Delay10us;
	IO_IIC_SCL_H;
	Hardware_Delay10us;
	IO_IIC_SCL_L;
	IO_IIC_SDA_H;
	Hardware_Delay1us;
}
void IIC_NACK()
{	
	IO_IIC_SCL_L;
	IO_IIC_SDA_H;
	Hardware_Delay10us;
	IO_IIC_SCL_H;
	Hardware_Delay10us;
	IO_IIC_SCL_L;
	IO_IIC_SDA_H;
	Hardware_Delay1us;
}
void IIC_SEND_8Bits(uint8_t IIC_Data_8b)
{
	uint8_t t;
	for(t=0;t<8;t++)
	{
		IO_IIC_SCL_L;
		Hardware_Delay1us;
	  if(IIC_Data_8b&0x80)
	  {
	  	IO_IIC_SDA_H;
	  }
	  else
	  {
	  	IO_IIC_SDA_L;
	  }
	  IIC_Data_8b<<=1;
	  Hardware_Delay9us;
	  IO_IIC_SCL_H;
		Hardware_Delay10us;
	  IO_IIC_SCL_L;
  }
}
uint8_t IIC_READ_8Bits()
{
	uint8_t i;
	uint8_t receive8Bits = 0x00;
	IO_IIC_SDA_H;//SDAÊäÈë
	
	for(i=0;i<8;i++)
	{
		IO_IIC_SCL_L;
		Hardware_Delay10us;
		IO_IIC_SCL_H;
		receive8Bits = (receive8Bits<<1)|SDA_IO;
		Hardware_Delay10us;
	}
	IO_IIC_SCL_L;
	return receive8Bits;
}
uint8_t IIC_READ(uint8_t send1,uint8_t send2,uint8_t send3,uint8_t *Pdata,uint16_t Num)
{
	uint8_t t;
	    IIC_START();
	    IIC_SEND_8Bits(send1);
	    if(IIC_Wait_ACK()==0)return 0;
	    IIC_SEND_8Bits(send2);
	    if(IIC_Wait_ACK()==0)return 0;	
			Hardware_Delay20us;
	    IO_IIC_SCL_H;
	    Hardware_Delay20us;
	    IO_IIC_SDA_L;
	    Hardware_Delay5us;
	    IO_IIC_SCL_L;
			Hardware_Delay10us;	
			IIC_SEND_8Bits(send3);
			if(IIC_Wait_ACK()==0)return 0;
			IO_IIC_SDA_H;
	   for(t=0;t<Num;t++)
     {
	  	*(Pdata + t)=IIC_READ_8Bits();
	  	if(t==(Num-1))
	  	{
	  		IIC_NACK();
	  	}
	  	else
  		{
			  IIC_ACK();
	  	} 	
	   }
			Hardware_Delay20us;
			IIC_STOP();
		 return 1;
}
/*void IIC_READ2(uint8_t send1,uint8_t send2,uint8_t send3,uint8_t send4,uint8_t *Pdata,uint16_t Num)
{
	uint8_t t;
	    IIC_START();
	    IIC_SEND_8Bits(send1);
	    if(IIC_Wait_ACK()==0)return;
	    IIC_SEND_8Bits(send2);
	    if(IIC_Wait_ACK()==0)return;	
	    IIC_SEND_8Bits(send3);
	    if(IIC_Wait_ACK()==0)return;	
			Hardware_Delay20us;
	    IO_IIC_SCL_H;
	    Hardware_Delay20us;
	    IO_IIC_SDA_L;
	    Hardware_Delay5us;
	    IO_IIC_SCL_L;
			Hardware_Delay10us;		
			IIC_SEND_8Bits(send4);
			if(IIC_Wait_ACK()==0)return;
			IO_IIC_SDA_H;
	   for(t=0;t<Num;t++)
     {
	  	*(Pdata + t)=IIC_READ_8Bits();
	  	if(t==(Num-1))
	  	{
	  		IIC_NACK();
	  	}
	  	else
  		{
			  IIC_ACK();
	  	} 	
	   }
			Hardware_Delay20us;
			IIC_STOP();
}*/
uint8_t IIC_SEND(uint8_t send1,uint8_t send2,uint8_t *_data,uint16_t Num)
{
	uint8_t t;
	IIC_START();
	IIC_SEND_8Bits(send1);
  if(IIC_Wait_ACK()==0)return 0;
	IIC_SEND_8Bits(send2);
	if(IIC_Wait_ACK()==0)return 0;	
	for(t=0;t<Num;t++)
	{
		IIC_SEND_8Bits(*_data);
		_data++;
		if(IIC_Wait_ACK()==0)return 0;
	}
	IIC_STOP();	
	return 1;
}
/*
uint8_t  IIC_SEND2(uint8_t send1,uint8_t send2,uint8_t send3,uint8_t _data)
{
	IIC_START();
	IIC_SEND_8Bits(send1);
	if(IIC_Wait_ACK()==0)return 0;
	IIC_SEND_8Bits(send2);
	if(IIC_Wait_ACK()==0)return 0;
  IIC_SEND_8Bits(send3);
	if(IIC_Wait_ACK()==0)return 0;	
	IIC_SEND_8Bits(_data);
	if(IIC_Wait_ACK()==0)return 0;
	IIC_STOP();	
	return 1;
}*/
