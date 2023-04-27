#include "EEPROM.h"
#include "IAP_Option_EW.h"
#include "Function_Init.H"

#define DEF_InternalEEPROM_SectorSize 					512	//Required _size of a sector

void EEPROM_ReadSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)
{
	while( Num-- )
	{
		*BUFF = IAP_Read(StartAddr, IAP_EEPROM);		//读取EEPROM数据
		BUFF++;
		StartAddr++;
	}
}

void EEPROM_WriteSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)
{
	uint8_t	SectorDATA[DEF_InternalEEPROM_SectorSize]={0};
	uint16_t i=0;
	uint16_t OffsetByte;
	uint16_t CurrentByte;
	uint16_t SavedByteNum;
	uint16_t Addr;
	SavedByteNum = 0;
	Addr=StartAddr;
	
	for (;;)
	{
			if ( Num > SavedByteNum)
			{
					OffsetByte = Addr%DEF_InternalEEPROM_SectorSize;
					if ( Num > (SavedByteNum+DEF_InternalEEPROM_SectorSize-OffsetByte) )//判断写数据是否超出当前扇区
					{
						if( OffsetByte != 0 )
						{
							EEPROM_ReadSequential((Addr-OffsetByte),SectorDATA,DEF_InternalEEPROM_SectorSize);//取出Sector原数据
							Soft_Delay1ms(1);
						}
			
						CurrentByte=OffsetByte;
						for( i=0 ; i<( DEF_InternalEEPROM_SectorSize-OffsetByte ) ; i++ )//更新该扇区需要存储的数据
						{
							SectorDATA[CurrentByte]=*BUFF;
							CurrentByte++;
							BUFF++;
						}
						IAP_EEPROMSectorEraseOption(Addr-OffsetByte);//擦除当前扇区所有数据
						EEPROM_PageWrite(Addr-OffsetByte,SectorDATA,DEF_InternalEEPROM_SectorSize);//写入当前扇区新数据
						SavedByteNum+=(DEF_InternalEEPROM_SectorSize-OffsetByte);
						Addr+=(DEF_InternalEEPROM_SectorSize-OffsetByte);
						Soft_Delay1ms(5);
					}
					else
					{
						EEPROM_ReadSequential((Addr-OffsetByte),SectorDATA,DEF_InternalEEPROM_SectorSize);//取出Sector原数据
						Soft_Delay1ms(1);
						CurrentByte=OffsetByte;
						for( i=0 ; i<( Num-SavedByteNum ) ; i++ )//更新该扇区需要存储的数据
						{
							SectorDATA[CurrentByte]=*BUFF;
							CurrentByte++;
							BUFF++;
						}
						IAP_EEPROMSectorEraseOption(Addr-OffsetByte);//擦除当前扇区所有数据
						EEPROM_PageWrite(Addr-OffsetByte,SectorDATA,DEF_InternalEEPROM_SectorSize);//写入当前扇区新数据
						SavedByteNum = Num;
						Soft_Delay1ms(5);
						break;
					}
			}
	}	
}

void EEPROM_PageWrite(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)//使用前必须先擦再写
{
	while( Num-- )
	{
		IAP_EEPROMProgramByteOption(StartAddr, *BUFF);		//往EEPROM 地址写入
		BUFF++;
		StartAddr++;
	}
}


void EEPROM_TESTING(void)
{
	//uint16_t i;

	//EEPROM_WriteSequential(VolumeSwitchStartAddr,&BUFFA,2);
	
	//EEPROM_ReadSequential(VolumeSwitchStartAddr,&BUFFB,1);
	
	//EEPROM_ReadSequential(VolumeSwitchStartAddr,&BUFFB,2);

}


