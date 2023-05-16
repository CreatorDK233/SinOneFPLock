#ifndef Flash_H
#define Flash_H

#include "global_variable.h"

//FLASH storage requires 4K space
#define DEF_InternalFlash_MemoryBit						8				//Depends on Flash _bit(8/16/32/64)
#define DEF_InternalFlash_SectorSize 					0x0200	//Required _size of a sector
#define DEF_InternalFlash_SectorNum 					8				//FLASH use 8 sectors (total 4K)
#define DEF_InternalFlash_LogMemoryStartAddr 	0x0800	//Start Addr
#define DEF_InternalFlash_LogMemoryStartPage 	4				//5th sector

extern void FLASH_ReadSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num);
extern void FLASH_WriteSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num);
extern void FLASH_PageWrite(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num);
extern status_t Flash_EraseOnePage(uint16_t PageAddr);

extern void FLASH_OB_Config(void);

extern void Flash_Test(void);

#endif
