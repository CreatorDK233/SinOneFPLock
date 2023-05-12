#ifndef _FPL_LOGID_H_
#define _FPL_LOGID_H_

#include "StdTypes.h"
#include "Project.h"

#define DEF_FPM_EnrollFailTimes_Limited 3

#define Def_WaitUserPutFingerTimeDelay		320		//5s

#define Def_WaitUserRemoveFingerTimeDelay		320		//5s	

#define Def_FPMcmdTimeOutDelay	128		//2s

#if defined ProjectIs_BarLock_S1732 || defined ProjectIs_BarLock_S6902 || \
		defined ProjectIs_BarLock_S8103 || defined ProjectIs_BarLock_S7703 
#define DEF_FPM_EnrollTimes 8
#else
#define DEF_FPM_EnrollTimes 5
#endif

#ifdef Function_FPMbreathingLed
void SetFPMbreathingLed(uint8_t color);
#endif

void RegisterFp(uint16_t UserID);
void FpUserIdentify(void);
bool_t CheckFPMserialNumberIsMatch(uint8_t buff[]);
uint8_t Get_Availabe_FPmasterID(void);
uint8_t Get_Availabe_FPuserID(void);
void DeleteAllFpFromFPM(void);
status_t Get_FPM_SerialNumber(uint8_t buff[]);
status_t Get_FPM_SerialNumber(uint8_t buff[]);
void CheckHowManyRegistereFPuser(void);
void CheckIfFpUserIsRegistered(uint16_t USERID);
status_t SaveFPMserialNumberToMemory(void);

void SetFPMbreathingLed(uint8_t color);



#endif