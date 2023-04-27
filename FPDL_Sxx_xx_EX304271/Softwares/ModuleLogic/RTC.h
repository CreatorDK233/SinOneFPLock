#ifndef _RTC_H_
#define _RTC_H_

#include "StdTypes.h"

extern systemtime_t UTCToSystemtime(uint32_t UTC);
extern uint32_t SystemTimeToUTC(systemtime_t Time);

#endif