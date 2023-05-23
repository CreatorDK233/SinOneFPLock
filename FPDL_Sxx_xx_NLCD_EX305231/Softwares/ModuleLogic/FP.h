#ifndef _FPL_LOGID_H_
#define _FPL_LOGID_H_

#include "StdTypes.h"
#include "Project.h"

#define DEF_FPM_EnrollFailTimes_Limited 3

#define Def_WaitUserPutFingerTimeDelay		320		//5s

#define Def_WaitUserRemoveFingerTimeDelay		320		//5s	

#define Def_FPMcmdTimeOutDelay	128		//2s
#define DEF_FpmAckTimeoutTime   128

#if defined ProjectIs_BarLock_S7702 || defined ProjectIs_BarLock_S6421 || \
		defined ProjectIs_BarLock_S6428 || defined ProjectIs_BarLock_S6428v2 || \
		defined ProjectIs_BarLock_S1734 || defined ProjectIs_BarLock_S1735 || \
		defined ProjectIs_BarLock_S6904 || defined ProjectIs_BarLock_S6904v2 || \
		defined ProjectIs_BarLock_S6117 || defined ProjectIs_BarLock_S6117v2  
#define DEF_FPM_EnrollTimes 8
#else
#define DEF_FPM_EnrollTimes 5
#endif

#ifdef Function_FPMBreathingLed
void SetFPMbreathingLed(uint8_t color);
#endif

void RegisterFp(uint16_t UserID);
void FpUserIdentify(void);
bool_t CheckFPMserialNumberIsMatch(uint8_t buff[]);
uint8_t Get_Availabe_FPmasterID(void);
uint8_t Get_Availabe_FPuserID(void);
void DeleteAllFpFromFPM(void);
status_t Get_FPM_SerialNumber(uint8_t buff[]);
void CheckHowManyRegistereFPuser(void);
void CheckIfFpUserIsRegistered(uint16_t USERID);
status_t SaveFPMserialNumberToMemory(void);
void AddFPuserIdToList(uint16_t FPuserID);
void DeleteFPuserIdFromList(uint16_t FPuserID);
void DeleteAllFPuserIdFromList(void);
void ReadFPuserIdListFromEEPROM(void);
void WriteFPuserIdListToEEPROM(void);
void ResetFPuserIdListInEEPROM(void);

void SetFPMbreathingLed(uint8_t color);



#endif