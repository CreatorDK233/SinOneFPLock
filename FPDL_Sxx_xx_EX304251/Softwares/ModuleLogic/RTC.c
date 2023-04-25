#include "RTC.h"
#include "Project.h"
#include "global_variable.h"

#define SECPerDay  86400
code uint8_t DayOfMon[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

uint8_t BCD_to_Hex(uint8_t num)
{
	return (num>>4)*10+(num&0x0f);
}

uint8_t Hex_to_BCD(uint8_t num)
{
	return ((num/10)<<4)+(num%10);
}

uint32_t SystemTimeToUTC(systemtime_t Time)
{
	uint32_t Sec_Today = 0;
	uint32_t Day_Total = 0;
	uint16_t year = BCD_to_Hex(Time.year)+2000;
	uint8_t j;
	uint16_t day_at_month;
	while(year > 2000)
	{
	  if((((year-1)%4==0)&&((year-1)%100!=0)) || ((year-1)%400==0)) 
		{
		   Day_Total +=366;
			 year--;
		}
		else
		{
			Day_Total +=365;
			 year--;
		}
	}
	year = BCD_to_Hex(Time.year)+2000;
	for(j=0;j<12;j++)     /* ?????·? */
	{
		if((j==1) && (((year%4==0)&&(year%100!=0)) || (year%400==0)))//闰年2月29天
			day_at_month = 29;
		else
			day_at_month = DayOfMon[j];
		
		if(BCD_to_Hex(SystemTime.month)-1 > j) 
		{
			Day_Total += day_at_month;
		}
		else 
		{
			Day_Total += BCD_to_Hex(SystemTime.date);
			Day_Total -= 1;
			break;
		}
	}
	Sec_Today = (uint32_t)BCD_to_Hex( Time.hour)*3600+(uint32_t)BCD_to_Hex( Time.minute)*60+(uint32_t)BCD_to_Hex( Time.second);
	Sec_Today+=Day_Total*SECPerDay;
	return Sec_Today;
}

systemtime_t UTCToSystemtime(uint32_t UTC)
{
	uint32_t DayToNow = UTC/SECPerDay;
	uint32_t SecOutOfDay = UTC%SECPerDay;
	uint16_t year = 2000;
	systemtime_t TimeResult;
	uint16_t day_at_month;
	uint8_t j;
	while(DayToNow > 365)
	{
		if(((year%4==0)&&(year%100!=0)) || (year%400==0))    /* ???? */
			DayToNow -= 366;
		else
			DayToNow -= 365;
		year++;
	}
	 if((DayToNow == 365) && !(((year%4==0)&&(year%100!=0)) || (year%400==0)))//正好最后一天而且不是闰年
	{
		DayToNow -= 365;
		year++;
	}
	TimeResult.year =Hex_to_BCD((uint8_t)year-2000);
	
	for(j=0;j<12;j++)     /* ?????·? */
  {
		if((j==1) && (((year%4==0)&&(year%100!=0)) || (year%400==0)))//闰年2月29天
			day_at_month = 29;
		else
			day_at_month = DayOfMon[j];
		
		if(DayToNow >= day_at_month) 
			DayToNow -= day_at_month;
		else 
			break;
	}
	TimeResult.month =   Hex_to_BCD(j+1);
	TimeResult.date		=  Hex_to_BCD(DayToNow+1);
	TimeResult.hour =    Hex_to_BCD(((SecOutOfDay/ 3600))%24);
	TimeResult.minute  = Hex_to_BCD((SecOutOfDay % 3600)/60);
	TimeResult.second  = Hex_to_BCD((SecOutOfDay % 3600)%60);
	return TimeResult;
}