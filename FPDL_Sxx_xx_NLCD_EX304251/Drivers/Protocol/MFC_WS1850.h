#ifndef MFC_h
#define MFC_h

#include "StdTypes.h"
#include "Project.h"

#ifdef Function_NFCUsed_1850X
extern void MFC_Init(void);

extern status_t MFC_Auto_Reader(uint8_t *Point);

extern status_t Find_Card(void);

extern status_t MFC_ReadLockBrandData(uint8_t *buff);

extern void MFC_POWERDOWN(void);

extern void MFC_Test(void);
#endif

#endif
