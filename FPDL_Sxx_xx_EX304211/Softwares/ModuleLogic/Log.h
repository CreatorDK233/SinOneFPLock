#ifndef _Log_H_
#define _Log_H_

#include "StdTypes.h"

extern code uint8_t DEF_LogFlagHighByte;
extern code uint8_t DEF_LogFlagLowByte;
extern code uint16_t DEF_MaxRecordedLogID;  //128*4,total 4 Pages
extern code uint16_t DEF_MaxSearchLogID;		

extern void LogMgr_WriteLog(uint16_t LogPoint);
extern void LogMgr_ReadLog(uint16_t LogPoint,uint8_t *point);

extern status_t JudgeLogSaved(uint16_t LogPoint);
extern status_t DeleteAllLog(void);

extern void SearchEventLogBySequence(void);
extern void GUI_CreatAndSaveLog(EventLogType_t LogType);
extern void DisplayOneEventLog(uint16_t EventLogID);
//extern void SearchEventLogByDate(void);

extern void LogMgr_Init(void);

#endif