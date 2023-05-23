#ifndef _MFC_LOGID_H_
#define _MFC_LOGID_H_

#include "StdTypes.h"

void ReadCardUserMemoryFromEEPROM(void);
void WriteCardUserMemoryToEEPROM(void);

void CardUserIdentify(void);
uint8_t CompareCardIDwithMemory(uint8_t *Point);
uint8_t Get_Availabe_CardUserID(void);
status_t SaveCardUserToMemory(uint8_t *Point,uint8_t UserID);
void DeleteCardUserfromMemory(uint8_t UserID);
void DeleteAllCardUserfromMemory(void);
bool_t IfCardUserIDisRegistered(uint8_t CardUserID);
uint8_t CheckHowManyRegisteredCardUser( void );
bool_t IfSystemIsNoCardUser(void);

void ShowRegisterCardUser(void);





















#endif