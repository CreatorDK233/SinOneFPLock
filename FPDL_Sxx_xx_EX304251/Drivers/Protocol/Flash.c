#include "EEPROM.h"
#include "StdTypes.h"
#include "IAP_Option_EW.h"

void FLASH_ReadSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)
{
	EEPROM_ReadSequential(StartAddr,BUFF,Num);
}

void FLASH_WriteSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)
{
	EEPROM_WriteSequential(StartAddr,BUFF,Num);
}

void FLASH_PageWrite(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num)//使用前必须先擦再写
{
	EEPROM_PageWrite(StartAddr,BUFF,Num);
}

status_t Flash_EraseOnePage(uint16_t PageAddr)
{
	IAP_EEPROMSectorEraseOption(PageAddr);
	return S_SUCCESS;
}

void FLASH_OB_Config(void)
{
	return;
}

void Flash_Test(void)
{

}






