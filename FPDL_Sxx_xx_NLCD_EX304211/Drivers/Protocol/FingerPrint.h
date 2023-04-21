#ifndef fingerprint_h
#define fingerprint_h

#include "StdTypes.h"
	
void FPcmd_Init(void);
extern void FPM_SendGetImageCmd(void);
extern void FPM_SendGetEnrollImageCmd(void);
extern void FPM_SendGenCharCmd(uint8_t BufferID);
extern void FPM_SendRegModelCmd(void);
extern void FPM_SendStoreCharCmd(uint8_t BufferID,uint16_t UserID);
extern void FPM_SendSearchCmd(uint8_t BufferID,uint16_t StartPage,uint16_t PageNum);
extern void FPM_DeleteCharCmd(uint16_t StartPageID,uint16_t CharNum);
extern void FPM_DeleteAllCharCmd(void);
extern void FPM_GetValidTempleteNumCmd(void);
extern void FPM_SendReadIndexTableCmd(void);
extern void FPM_SendGetSerialNumberCmd(void);
extern void FPM_SendAutoRegisterCmd(uint16_t UserID);
extern void FPM_SetBreathingLED(uint8_t mode,uint8_t startcolor,uint8_t endcolor,uint8_t looptimes);
extern void FPM_SetSecurityLevel(uint8_t Level);			//from 1 ~5
extern void FPM_TurnOffAntiFakeFp(void);
extern void FPM_SendSleepCmd(void);
extern void FPM_SendGetChipSerialNumberCmd(void);
extern void FPMcmd_Excute(void);
extern void FPM_Mgr_Task(void);

#endif
