#ifndef EEPROM_h
#define EEPROM_h

#include "StdTypes.h"

#define IAP_EEPROM 0x02

#define ManagerPasscodeMemoryStartAddr		0x0000

#define CardUserMemoryStartAddr						0x0020	

#define PasscodeUserMemoryStartAddr			(0x0020+(6*DEF_MAX_CARDUSER))
#define PasscodeUserMemoryStartAddr1		(0x0020+(6*DEF_MAX_CARDUSER+14))
//If a password needs to store 14 bytes, a total of 140 bytes are required

#define FPMserialNumberStartAddr			(0x0312)		//from 0x0312 to 0x0331 total 32bytes

#define CountSerialNumberStartAddr		(0x0332)//(0x0900)
#define CountNumberStartAddr					(0x0337)//(0x0905)
#define TempCountNumberStartAddr	    (0x033B)//(0x0909)
#define CountRandNumberStartAddr	    (0x033F)//(0x090D)

#define CountSerialNumberStartAddr1		(0x0342)//(0x0910)
#define CountNumberStartAddr1					(0x0347)//(0x0915)
#define TempCountNumberStartAddr1	    (0x034B)//(0x0919)
#define CountRandNumberStartAddr1	    (0x034F)//(0x091D)


#define BackupsCountSerialNumberStartAddr		(0x0352)//(0x0920)
#define BackupsCountNumberStartAddr			  	(0x0357)//(0x0925)
#define BackupsTempCountNumberStartAddr	  	(0x035B)//(0x0929)
#define BackupsCountRandNumberStartAddr	  	(0x035F)//(0x092D)

#define BackupsCountSerialNumberStartAddr1	(0x0362)//	(0x0930)
#define BackupsCountNumberStartAddr1				(0x0367)//	(0x0935)
#define BackupsTempCountNumberStartAddr1	  (0x036B)// 	(0x0939)
#define BackupsCountRandNumberStartAddr1	  (0x036F)//  (0x093D)

#define BackupsManagerPasscodeMemoryStartAddr			(0x0373) //(0x0941)
#define BackupsManagerPwdUserIdStartAddr					(0x0380) //(0x094E)
#define BackupsManagerPasscodeMemoryStartAddr1		(0x0385) //(0x0953)
#define BackupsManagerPwdUserIdStartAddr1					(0x0392) //(0x0960)

#define SerialNumberStartAddr									(0x03A0)//(0x1000)
#define SerialNumberStartAddr1								(0x03A6)//(0x1006)
#define BackupsSerialNumberStartAddr			    (0x03AD)//(0x100D)
#define BackupsSerialNumberStartAddr1			    (0x03B4)//(0x1014)

#define LockBrandStartAddr					(0x03C0)//(0x1700)	//from 0x03C0 to 0x04A0 ,225 bytes

#define SystemLanguageStartAddr				(0x07FB)//(0x17FB)
#define PickAlarmSwitchStartAddr			(0x07FC)//(0x17FC)
#define UnlockModeStartAddr						(0x07FD)//(0x17FD)		
#define VolumeSwitchStartAddr					(0x07FE)//(0x17FE)		
#define SelfTestMemoryStartAddr				(0x07FF)//(0x17FF)//from0x0000 to 0x0800 total 2048 bytes(2k bytes)

extern void EEPROM_ReadSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num);

extern void EEPROM_WriteSequential(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num);

extern void EEPROM_PageWrite(uint16_t StartAddr,uint8_t *BUFF,uint16_t Num);

extern void EEPROM_TESTING(void);



#endif
