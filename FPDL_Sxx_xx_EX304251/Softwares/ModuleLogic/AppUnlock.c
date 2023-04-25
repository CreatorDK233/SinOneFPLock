
#include <string.h>
#ifdef PLAT_GCC 
#include <stdio.h>  
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#else
#include "global_variable.h"
#include "IO.h"
#include "Motor.h"
#include "ISR.h"
#endif

#define C2D(x) (x-0x30)
#define D2C(x) (x+0x30)

#define true 1
#define false 0
//#define NULL 0
#define s_int  signed int
#define u_int  unsigned int
#define s_char signed char
#define u_char unsigned char
#define bool   unsigned char
u_char version[]="sl_generic_v1.0.0.003";

//void MyPrintf(char *format, ...);
#ifdef PLAT_GCC
#define DEBUG_GCC
#define DEBUG_MARK usleep(1)
#define FIFO_PATH "/lxk/tj/backup/lockpipe"
bool  debug_singleline=false;
u_int debug_minutes_test = 0;
#endif

typedef struct _itime
{
	u_int  year;
	u_int  month;
	u_int  day;
	u_int  hour;
	u_int  minute;
	
	u_int  yday;	//一年中的第几天[0-365] 
	u_int  dmin;    //一天中的第几分钟[0-1440]
	u_int  aday;    //从2019.1.1算起的第几天

	u_int  aday_h;  // aday/6  密码中时间的高4位
	u_int  aday_l;  // aday%6 密码中时间的低3位
	u_int  dminute_l; // aday%6  

	u_int  yday_max;  //该年最多几天[365|366]
#ifdef DEBUG_GCC
	/*测试用，模拟不同时间*/
	u_int  year_test;
	u_int  month_test;
	u_int  day_test;
	u_int  hour_test;
	u_int  minute_test;  
#endif
}itime;

#define mode_forever (u_char)0      //永久有效
#define mode_count (u_char)1		//计数模式(999)
#define mode_temporary (u_char)2    //15分钟有效
#define mode_period (u_char)3       //时间段(h-h)
#define mode_limit (u_char)4        //限时到某年月日
typedef struct _ipwd
{
	u_char  lockpwd[13]; 	//开锁密码
	u_int   lockpwd_len;	//开锁密码的长度
	u_int   iscorrect;		//密码是否正确
	u_char  mode;			//mode_count、mode_temporary、  mode_period、mode_limit
	u_int   rand;			//随机数
	u_int   count;		 	//计数模式的次数
	itime   limit_day;		//限时模式的期限(从2019.1.1开始的天数)
	u_int   period_start;   //时间段模式的开始时间(0-23小时)
	u_int   period_end;		//时间段模式的结束时间(0-23小时)

	u_char  userpwd[13];  	//用户密码
	u_int   userpwd_len;  	//用户密码的长度
	u_int   userpwd_hash[13];  //用户密码hash结果
	u_int   userpwd_hash_len;  //用户密码hash的长度
	u_int   userpwd_iscorrect; //开锁密码中包含的用户密码是否正确
	u_char  isencrypt;
#ifdef DEBUG_GCC
	u_char  userpwd_debug[14]; //调试打印使用
	u_char  lockpwd_debug[13]; //调试打印使用
	u_char  process_debug[6][14]; //调试打印使用
#endif
}ipwd;

u_int const hash_user[5][100]={ //使用多个数组哈希用户密码，如果使用1个数组对应6-12位所有的用户密码，哈希重复的概率会高很多
		//6位用户密码
		 {315,420,254,105,513,410,450,425,415,350,
			523,125,210,143,134,201,314,250,102,214,
			451,135,130,435,104,153,401,304,452,234,
			204,421,301,140,132,405,325,520,305,430,
			352,241,413,253,302,145,120,240,412,532,
			501,251,324,402,321,345,310,503,231,213,
			502,351,543,142,354,524,235,534,312,510,
			203,340,154,514,215,243,542,530,150,512,
			540,453,230,541,124,504,403,341,521,342,
			432,431,103,205,423,531,123,245,320,152
			},
			//7位用户密码
			{206,564,653,416,632,140,521,640,520,245,
			634,164,126,420,603,230,124,102,506,501,
			512,610,136,530,106,463,513,235,523,154,
			326,130,320,605,145,426,310,315,461,341,
			264,524,641,504,150,403,526,405,563,502,
			253,432,406,421,650,364,561,436,265,134,
			430,104,301,435,431,243,625,542,142,156,
			615,261,160,240,306,624,162,614,654,205,
			143,630,623,214,541,643,510,165,324,560,
			152,412,532,213,356,210,645,503,602,251
			},
			//8位用户密码
			{523,547,510,356,175,264,316,214,642,704,
			207,140,452,352,253,526,453,514,603,367,
			752,731,307,673,471,326,165,764,106,713,
			760,450,413,105,243,527,631,475,632,634,
			732,605,401,423,741,576,126,306,350,162,
			617,725,160,503,246,257,163,231,201,650,
			467,645,412,532,342,345,513,156,210,765,
			107,256,407,403,520,357,670,437,271,312,
			247,561,235,564,157,754,674,463,723,320,
			170,215,472,364,740,402,371,567,365,274
			},
			//9位用户密码
			{743,503,631,628,416,286,783,247,672,364,
			380,612,468,712,162,346,680,857,675,807,
			237,316,361,142,613,736,158,837,648,351,
			430,352,734,128,356,543,270,815,420,825,
			817,718,702,415,841,678,428,624,603,170,
			521,106,405,873,308,601,701,526,716,750,
			154,516,846,164,263,617,368,487,413,568,
			146,876,432,561,786,472,560,417,431,510,
			745,507,125,806,482,378,847,637,765,706,
			256,845,476,570,321,326,513,238,746,763
			},
			//10-12位用户密码，10位密码时，遇到0则对应的是10； 11位密码时，遇到0则对应的是10，遇到1则对应的是11； 12位密码时，遇到0则对应的是10，遇到1则对应的是11，遇到2则对应的是12
			{174,192,210,957,720,859,178,758,958,105,
			346,563,419,594,892,581,208,274,791,348,
			520,371,735,601,951,158,752,207,825,815,
			860,398,980,703,840,361,628,214,436,248,
			218,843,571,412,906,159,962,713,201,647,
			970,230,841,831,894,478,902,385,580,167,
			190,591,874,659,189,675,764,186,359,365,
			148,420,940,482,590,384,736,754,871,968,
			370,945,435,932,410,519,870,392,469,310,
			694,391,386,314,983,908,376,802,297,913
			},
	};

u_int const hash_6bit[100]={47915,13054,41259,18369,40178,38207,25638,46817,46083,30419,
								 50986,10896,36704,10563,18506,28065,50342,21086,19537,30698,
								 53109,37046,30852,27564,51967,47385,50612,48059,40981,31760,
								 50162,56901,46805,42069,42970,59167,52317,27980,14682,31780,
								 52183,53819,56328,14976,19547,57182,16280,37081,32197,36125,
								 13540,21659,12468,48951,40278,10829,14907,54921,29630,17634,
								 10298,37018,43259,24157,32765,20453,28015,31684,28345,35018,
								 48176,52041,47921,40917,54819,14208,25789,34708,42817,14563,
								 39746,20874,58164,19734,32401,10495,48375,48175,37602,21854,
								 48397,27564,58192,41362,56024,23704,14650,26048,47309,28619
};

u_int system_time(u_int *pyear,u_int *pmonth, u_int *pday,u_int *phour,u_int *pminute);

bool time_dyear(itime *it);
u_int time_yday_max(u_int year);
void time_print(itime it,u_char label[]);
u_int time_findyear(u_int year/*2019*/,u_int days/*1-366*/,u_int *pmonth,u_int *pday);
bool time_cur(itime *it);
bool time_set(itime *it,u_int days,u_int minute);
u_int time_adays(itime *it);
s_int  time_diff_minute(itime t1,itime t2);
s_int  time_diff_day(itime t1,itime t2);
void time_format_cur(u_char lockpwd[],u_int len);


bool pwd_prepare(ipwd *pwd,u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len);
bool pwd_shiftr_d(ipwd *pwd);
bool pwd_shiftl_d(ipwd *pwd);
bool pwd_userpwd_select_d(ipwd *pwd,u_char userpwd[],u_int userpwdlen);
bool pwd_confuse_d(ipwd *pwd);
bool pwd_hash_d(ipwd *pwd);
bool pwd_cross_d(ipwd *pwd);
bool pwd_unpack_rd(ipwd *pwd);
bool pwd_unpack(ipwd *pwd,u_char userpwd[],u_int userpwdlen);
ipwd *pwd_decrypt(u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len);
unsigned int pwd_decrypt_checkmode(u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len);

#ifdef PLAT_GCC
u_char  getrand();
bool pwd_init(ipwd *pwd,u_char mode,u_char userpwd[],u_int userpwdlen);
bool pwd_rand(ipwd *pwd);
void pwd_snap(ipwd *pwd,u_int index);
bool pwd_pack(ipwd *pwd);
void pwd_print(ipwd pwd);
void pwd_shiftr(ipwd *pwd);
void pwd_shiftl(ipwd *pwd);
void pwd_confuse(ipwd *pwd);
u_int pwd_hash(ipwd *pwd);
bool pwd_cross(ipwd *pwd);
u_char *pwd_encrypt(ipwd *pwd,u_char lockpwd[]);
bool pwd_userpwd_select(ipwd *pwd,u_char userpwd[],u_int userpwdlen);
u_char *pwd_make_count(u_char userpwd[],u_int userpwdlen,u_int count,u_char lockpwd[],u_int lockpwd_len);
u_char *pwd_make_temporary(u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len);
u_char *pwd_make_period(u_char userpwd[],u_int userpwdlen,u_int hour_start,u_int hour_end,u_char lockpwd[],u_int lockpwd_len);
u_char *pwd_make_limit(u_char userpwd[],u_int userpwdlen,u_int year,u_int month,u_int day,u_int hour,u_char lockpwd[],u_int lockpwd_len);
#endif

#ifdef DEBUG_GCC
void debug_start(u_char *str)
{
	printf( " %s ：",str);
	debug_singleline = true;
}

void debug_print(u_char *str,...)
{
     char buff[512]={0};
	 char cmd[550]={0};
	 u_int y=0,m=0,d=0,h=0,s=0;
	 
     va_list args;
     va_start(args, str);

     vsnprintf(buff,1024 ,str, args);
     va_end(args);

	 printf(buff);

	 u_int len = strlen(buff);
	 if( buff[len-1]==0x0A )
	 	buff[len-1] = 0;
	 system_time(&y,&m,&d,&h,&s);
	 sprintf(cmd,"echo '%04d-%02d-%02d %02d:%02d  %s' >> /lxk/tj/backup/elog",y,m,d,h,s,buff);
	 system(cmd);
}
#endif

#ifdef DEBUG_GCC
void debug_print_array(u_char buf[])
{
	debug_print("buf=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
}
#endif


void debug_end()
{
#ifdef DEBUG_GCC
	printf( "\n");
	debug_singleline = false;
#endif
}


#ifdef PLAT_GCC
u_int system_time(u_int *pyear,u_int *pmonth, u_int *pday,u_int *phour,u_int *pminute)
{
	u_int minutes = 0;
	time_t ct;
	struct tm *lt;
	time(&ct);
	lt=localtime(&ct); 
	if( pyear!=NULL )
		*pyear = lt->tm_year+1900;
	if( pmonth!=NULL )
		*pmonth = lt->tm_mon+1;
	if( pday!=NULL )
		*pday = lt->tm_mday;
	if( phour!=NULL )
		*phour = lt->tm_hour;
	if( pminute!=NULL )
		*pminute = lt->tm_min;
	
	if( debug_minutes_test>0 )
	{
		//*pyear += debug_minutes_test;
		minutes = *phour*60+*pminute+debug_minutes_test;
		*phour = minutes/60;
		*pminute = minutes%60;
	}
	
	return lt->tm_hour*60+ lt->tm_min+lt->tm_sec;
}
#else
u_int system_time(u_int *pyear,u_int *pmonth, u_int *pday,u_int *phour,u_int *pminute)
{
        uint16_t y,m,d,hour,min;
        /*
	*pyear = 2019;
	*pmonth = 3;
	*pday = 29;
	*phour = 9;
	*pminute = 53;
        */
        
        y = ((SystemTime.year/16)*10) + (SystemTime.year%16) + 2000;
        m = ((SystemTime.month/16)*10) + (SystemTime.month%16);
        d = ((SystemTime.date/16)*10) + (SystemTime.date%16);
        hour = ((SystemTime.hour/16)*10) + (SystemTime.hour%16);
        min = ((SystemTime.minute/16)*10) + (SystemTime.minute%16);
        
        *pyear = y;
	*pmonth = m;
	*pday = d;
	*phour = hour;
	*pminute = min;
    return *phour*60+ *pminute;
}

//void memset(u_char src[],u_char ch,u_int len)
//{
//	u_int i;
//	for(i=0;i<len;i++)
//		src[i] = ch;
//}

//void memcpy(u_char dst[],u_char src[],u_int len)
//{
//	u_int i;
//	for(i=0;i<len;i++)
//		dst[i] = src[i];
//}

#endif

//年月日->偏移天数         偏移天数->年月日时分
//获取当前时间       该年中第几天       偏移天数
bool time_cur(itime *it)
{
	if( it==NULL )
		return false;
	system_time(&(it->year),&(it->month),&(it->day),&(it->hour),&(it->minute));
	it->year = 2019+(it->year-2019)%15;
	it->dmin=it->hour*60+ it->minute;
	time_adays(it);

	it->aday_h = it->aday/6;
	it->aday_l = it->aday%6;
	it->dminute_l = it->aday_l*1440+it->hour*60+it->minute;

	//time_print(*it,"time_cur");
	return true;
}

bool time_cur2(itime *it)
{
	if( it==NULL )
		return false;
	system_time(&(it->year),&(it->month),&(it->day),&(it->hour),&(it->minute));
	it->dmin=it->hour*60+ it->minute;
	time_adays(it);

	it->aday_h = it->aday/6;
	it->aday_l = it->aday%6;
	it->dminute_l = it->aday_l*1440+it->hour*60+it->minute;

	//time_print(*it,"time_cur");
	return true;
}

//根据密码中偏移天数(从2019.1.1起)获取时间
bool time_set(itime *it,u_int days,u_int minute)
{
//	u_int i=0;
	u_int days_left = 0;
	u_int yday = 0;
	u_int year = 2019,month = 0,day = 0;
	
	if( it==NULL )
		return false;
#ifdef PLAT_GCC
	memset(it,0,sizeof(itime));
#endif
	if( minute>=1440 )
	{
		days += minute/1440;
		minute = minute%1440;
	}
	it->aday = days;
	it->hour = (minute%1440)/60;
	it->minute = minute%60;//+minutes_test%60;
	it->dmin=it->hour*60+ it->minute;

	days_left = days;
	while(true)
	{
		yday = time_findyear(year,days_left,&month,&day);
		days_left -= yday;
		if( days_left<=0 )
			break;
		year++;
	}
	it->year = 2019+(year-2019)%15;
	it->month = month;
	it->day = day;
	it->yday = yday;
	it->yday_max = time_yday_max(year);
	it->aday_h = it->aday/6;
	it->aday_l = it->aday%6;
	it->dminute_l = it->aday_l*1440+it->hour*60+it->minute;

	//time_print(*it,"time_set ");
    return true;
}

bool time_set_ymd(itime *it,u_int year,u_int month,u_int day,u_int hour)
{
	if( it==NULL )
		return false;
	it->year = year;
	it->month = month;
	it->day = day;
	it->hour= hour;
	it->minute=0;
	it->dmin=it->hour*60+ it->minute;
	time_adays(it);

	it->aday_h = it->aday/6;
	it->aday_l = it->aday%6;
	it->dminute_l = it->aday_l*1440+it->hour*60+it->minute;
	
	//time_print(*it,"time_set_ymd");
    return true;
}

#ifdef DEBUG_GCC
void time_print(itime it,u_char label[])
{
	debug_print("time(%s) %04d-%02d-%02d %02d:%02d dyear=%d/%d dmin=%d adays=%d [%03d-%04d.%d]\n",\
		label,it.year,it.month,it.day,it.hour,it.minute,it.yday,it.yday_max,it.dmin,it.aday,\
		it.aday_h,it.dminute_l,it.aday_l);
}
#endif

//查询某日是在该年中的第几天
bool time_dyear(itime *it)
{
	u_int days=0,i;
    u_int daysofmonth[]={31,28,31,30,31,30,31,31,30,31,30,31}; //非闰年

	if( it==NULL )
		return false;
	
    if (it->year%400==0 || (it->year%4==0 && it->year%100!=0))
    {
      daysofmonth[1] = 29;
	  it->yday_max = 366;
    }
	else
	{
		it->yday_max = 365;
	}

	it->yday = 0;
	for(i=0;i<it->month-1;i++)
    {
       it->yday+=daysofmonth[i];
    }
    
    it->yday+=it->day;
    return true;
}

u_int time_yday_max(u_int year)
{
	if (year%400==0 || (year%4==0 && year%100!=0))
		return 366;
	return 365;
}

//从年第几天得出是哪个年月日
//入参year是准备查询的年份，比如偏移天数是700，要按2019、2020...顺序找700天是落在哪年哪月哪日
u_int time_findyear(u_int year/*2019*/,u_int days/*1-366*/,u_int *pmonth,u_int *pday)
{
	u_int sum=0,i=0,maxdays=0;
    u_int daysofmonth[]={31,28,31,30,31,30,31,31,30,31,30,31}; //非闰年
    if (year%400==0 || (year%4==0 && year%100!=0))
    {
      daysofmonth[1] = 29;
	  maxdays = 366;
    }
	else
	  maxdays = 365;

	if( days>maxdays )
	{
		return maxdays;
	}
	for(i=0;i<12;i++)
    {
       sum+=daysofmonth[i];
	   if( sum>=days )
	   {
	   	 *pmonth = i+1;
		 *pday = days-(sum-daysofmonth[i]);
		 break;
	   }
    }
    
    return days;
}



/*从2019.1.1开始的偏移数,itime需要已赋予年月日字段的值*/
u_int time_adays(itime *it)
{
	u_int year=2019;
	u_int days = 0;

	if( it==NULL )
		return 0;
	
	for(year=2019;year<it->year;year++){
		days += time_yday_max(year);
	}
	time_dyear(it);
	it->aday = days + it->yday;
    return days;
}

s_int  time_diff_day(itime t1,itime t2)
{
	s_int diff = t1.aday-t2.aday;
	return diff;
}

s_int  time_diff_minute(itime t1,itime t2)
{
	s_int diff_day,diff_minute;

	diff_day = (t1.aday-t2.aday);
	if (diff_day > 10 )
		diff_day = 10;
	else if (diff_day < -10 )
		diff_day = -10;
	diff_minute = diff_day*1440+(t1.hour-t2.hour)*60+t1.minute-t2.minute;
	return diff_minute;
}

u_char dad(u_char c1,u_char c2)
{
	return (u_char)((c1+c2)%10);
}

u_char dsd(u_char c1,u_char c2)
{
	if( c1>=c2 )
		return (u_char)(c1-c2);
	return (u_char)(c1+10-c2);
}

void c2ds(u_char *src,u_int len)
{
	u_int i=0;
	for(i=0;i<len;i++)
	{
		src[i] = C2D(src[i]);
	}
}

void d2cs(u_char *src,u_int len)
{
	u_int i=0;
	for(i=0;i<len;i++)
	{
		src[i] = D2C(src[i]);
	}
}

u_char *d2cs6(u_char *src)
{
	u_char buf[20]={0};
//	u_int len = strlen(src);
	u_int i=0;
	for(i=0;i<6;i++)
	{
		buf[i] = D2C(src[i]);
	}
	return buf;
}

void d2cs2(u_char *src,u_char *dst,u_int len)
{
	u_int i=0;
	for(i=0;i<len;i++)
	{
		dst[i] = D2C(src[i]);
	}
	dst[len] = 0;
}

bool pwd_prepare(ipwd *pwd,u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len)
{
	if( pwd==NULL )
		return false;
	if( userpwdlen<6 || userpwdlen>12 || lockpwd_len!=12 )
		return false;
#ifdef PLAT_GCC
	memset(pwd,0,sizeof(ipwd));
#endif
	pwd->lockpwd_len = lockpwd_len;
	memcpy(pwd->lockpwd,lockpwd,pwd->lockpwd_len);
	pwd->userpwd_len = userpwdlen;
	memcpy(pwd->userpwd,userpwd,pwd->userpwd_len);
#ifdef DEBUG_GCC
	memcpy(pwd->lockpwd_debug,lockpwd,pwd->lockpwd_len);
#endif
	return true;
}

bool pwd_shiftr_d(ipwd *pwd)
{
	s_int i=0;
	s_int j=0;
	u_int start,end,offset;

	if( pwd==NULL )
		return false;
	start = 1;
	end = pwd->lockpwd_len-1;
	offset = pwd->lockpwd[0];
	offset = (offset%10)%(end-start+1);
	
	if( offset==0 )
		return false;
	
	for(i=0;i<offset;i++)
	{
		u_char tmp = pwd->lockpwd[end];
		for(j=(end-1);j>=0;j--)
		{
			pwd->lockpwd[j+1] = pwd->lockpwd[j];
		}
		pwd->lockpwd[start] = tmp;
	}
	return true;
}


bool pwd_shiftl_d(ipwd *pwd)
{
	s_int i=0;
	s_int j=0;
	u_int start,end,offset;

	if( pwd==NULL )
		return false;
	start = 0;
	end = pwd->lockpwd_len-2;
	offset = pwd->lockpwd[pwd->lockpwd_len-1];
	offset = (offset%10)%(end-start+1);

	if( offset==0 )
		return false;
	
	for(i=0;i<offset;i++)
	{
		u_char tmp = pwd->lockpwd[start];
		for(j=(start+1);j<=end;j++)
		{
			pwd->lockpwd[j-1] = pwd->lockpwd[j];
		}
		pwd->lockpwd[end] = tmp;
	}

	return true;
}


bool pwd_userpwd_select_d(ipwd *pwd,u_char userpwd[],u_int userpwdlen)
{
	u_int checksum = 0,i=0;
	u_int pwdlen,_data,d1,d2,d3,d4,dc,bten;
	pwdlen = userpwdlen; 
	_data = hash_user[(userpwdlen-6)%5][pwd->rand]; ////?????? 
	d1 = _data/100; 
	d2 = (_data%100)/10;
	d3 = _data%10; 
	d4 = 0;
	dc = 0;
	bten = userpwdlen-11;
	
	if( pwd==NULL )
		return false;

	if( pwd->rand<=0 || pwd->rand>=100 )
		return false;

	if( pwdlen<6 || pwdlen>12 )
		return false;
#ifdef DEBUG_GCC
	memset(pwd->userpwd_debug,0x5f,14);
	pwd->userpwd_debug[13] = 0;
#endif
	if( bten>=0 && bten<=2 )
	{
			if( d1<=bten )
				d1 += 10;
			if( d2<=bten )
				d2 += 10;
			if( d3<=bten )
				d3 += 10;
	}
	for(i=1;i<=userpwdlen;i++){
		if( i!=d1 && i!=d2 && i!=d3 )
			{
				d4 = i;
				break;
			}
	}
	
	for(i=0;i<userpwdlen;i++)
		checksum += userpwd[i];

	if(pwd->mode==mode_count || pwd->mode==mode_period)
	{
		pwd->userpwd_hash[0] = pwd->lockpwd[1];
		pwd->userpwd_hash[1] = pwd->lockpwd[2];
		pwd->userpwd_hash[2] = pwd->lockpwd[3];
		pwd->userpwd_hash[3] = pwd->lockpwd[4];
		dc = pwd->lockpwd[5];
		pwd->userpwd_hash_len = 5;
		
		if( userpwd[d1]==pwd->userpwd_hash[0] && userpwd[d2]==pwd->userpwd_hash[1] && userpwd[d3]==pwd->userpwd_hash[2] && userpwd[d4]==pwd->userpwd_hash[3] && (checksum%10)==dc )
			pwd->userpwd_iscorrect = 1;
#ifdef DEBUG_GCC	
		pwd->userpwd_debug[d1] = pwd->userpwd_hash[0]+0x30;
		pwd->userpwd_debug[d2] = pwd->userpwd_hash[1]+0x30;
		pwd->userpwd_debug[d3] = pwd->userpwd_hash[2]+0x30;
		pwd->userpwd_debug[d4] = pwd->userpwd_hash[3]+0x30;
		pwd->userpwd_debug[12] = dc+0x30;
#endif
	}
	else if(pwd->mode==mode_temporary || pwd->mode==mode_limit)
	{
		pwd->userpwd_hash[0] = pwd->lockpwd[1];
		pwd->userpwd_hash[1] = pwd->lockpwd[2];
		dc = pwd->lockpwd[3];
		pwd->userpwd_hash_len = 3;

		if( userpwd[d1]==pwd->userpwd_hash[0] && userpwd[d2]==pwd->userpwd_hash[1] && (checksum%10)==dc )
			pwd->userpwd_iscorrect = 1;
#ifdef DEBUG_GCC
		pwd->userpwd_debug[d1] = pwd->userpwd_hash[0]+0x30;
		pwd->userpwd_debug[d2] = pwd->userpwd_hash[1]+0x30;
		pwd->userpwd_debug[12] = dc+0x30;
#endif
	}
	else
	{
		pwd->userpwd_hash_len = 0;
		pwd->userpwd_iscorrect = 0;
	}
	if( pwd->userpwd_iscorrect==1 )
		return true;
	return false;
}

bool pwd_confuse_d(ipwd *pwd)
{
	s_int i=0,j=0,sum=0;

	if( pwd==NULL )
		return false;
	for(i=0;i<pwd->lockpwd_len-1;i++)
		sum += pwd->lockpwd[i];
	pwd->lockpwd[pwd->lockpwd_len-1] = dsd(pwd->lockpwd[pwd->lockpwd_len-1],sum%10);
	for(i=0;i<pwd->lockpwd_len;i++)
	{
		for(j=0;j<pwd->lockpwd_len;j++)
		{
			if( i!=j )
				pwd->lockpwd[j] = dad(pwd->lockpwd[i],pwd->lockpwd[j]);
			
		}
	}

	return true;
}

bool pwd_hash_d(ipwd *pwd)
{
	u_int _data = 0;
	u_char i=0;
	u_char d1,d2,d3,d4,d5,d6;
	d1 = (u_char)(_data/10000);
	d2 = (u_char)((_data%10000)/1000);
	d3 = (u_char)((_data%1000)/100); 
	d4 = (u_char)((_data%100)/10);
	d5 = (u_char)(_data%10);
	d6 = 0;
	
	if( pwd==NULL )
		return false;
	_data = hash_6bit[pwd->rand];
	
	for(i=1;i<=6;i++){
		if( i!=d1 && i!=d2 && i!=d3 && i!=d4 && i!=d5 )
			{
				d6 = i;
				break;
			}
	}
	
	pwd->lockpwd[1] = dsd(pwd->lockpwd[1],d1);
	pwd->lockpwd[2] = dsd(pwd->lockpwd[2],d2);
	pwd->lockpwd[3] = dsd(pwd->lockpwd[3],d3);
	pwd->lockpwd[4] = dsd(pwd->lockpwd[4],d4);
	pwd->lockpwd[5] = dsd(pwd->lockpwd[5],d5);
	pwd->lockpwd[6] = dsd(pwd->lockpwd[6],d6);
	pwd->lockpwd[7] = dad(pwd->lockpwd[7],d5);
	pwd->lockpwd[8] = dad(pwd->lockpwd[8],d4);
	pwd->lockpwd[9] = dad(pwd->lockpwd[9],d3);
	if( pwd->mode==mode_temporary )
		pwd->lockpwd[10] = dad(pwd->lockpwd[10],d2);

	return true;
}

bool pwd_cross_d(ipwd *pwd)
{
	u_char chx=0;
	
	if( pwd==NULL )
		return false;
	
	chx = dad(pwd->lockpwd[0],pwd->lockpwd[1]);
	pwd->lockpwd[2] = dsd(pwd->lockpwd[2],chx);
	chx = dad(pwd->lockpwd[3],pwd->lockpwd[4]);
	pwd->lockpwd[5] = dsd(pwd->lockpwd[5],chx);
	chx = dad(pwd->lockpwd[6],pwd->lockpwd[7]);
	pwd->lockpwd[8] = dsd(pwd->lockpwd[8],chx);
	chx = dad(pwd->lockpwd[9],pwd->lockpwd[10]);
	pwd->lockpwd[11] = dsd(pwd->lockpwd[11],chx);
	return true;
}


bool pwd_unpack_rd(ipwd *pwd)
{
	if( pwd==NULL )
		return false;
	pwd->mode = pwd->lockpwd[0];
	switch(pwd->mode)
	{
		case mode_count:
		case mode_period:
		case mode_limit:
			pwd->rand = pwd->lockpwd[10]*10+pwd->lockpwd[11];
			break;
		case mode_temporary:
			pwd->rand = pwd->lockpwd[0]*10+pwd->lockpwd[11];
			break;
		default:
			return false;
			break;
			
	}
	return  true;
}

bool pwd_unpack(ipwd *pwd,u_char userpwd[],u_int userpwdlen)
{
	u_int daysonsix = 0;
	u_int year=0,days=0,month=0,day=0,hour=0,minutes=0,i=0,checksum=0;
	s_int td = 0;
	itime ct;

	if( pwd==NULL )
		return false;
	pwd->iscorrect = 0;
	if( pwd_userpwd_select_d(pwd,userpwd,userpwdlen)==false )
		return false;
	
	switch(pwd->mode)
	{
		case mode_count:
			for(i=0;i<pwd->lockpwd_len;i++)
				checksum += pwd->lockpwd[i];
			checksum -= pwd->lockpwd[9];
			if( pwd->lockpwd[9] == checksum%10 )
			{
				pwd->count = pwd->lockpwd[6]*100+pwd->lockpwd[7]*10+pwd->lockpwd[8];
				if( pwd->userpwd_iscorrect==1 )
					pwd->iscorrect = 1;
			}
			break;
		case mode_temporary:
				time_cur(&ct);
				daysonsix = pwd->lockpwd[4]*100+pwd->lockpwd[5]*10+pwd->lockpwd[6];
				minutes = pwd->lockpwd[7]*1000+pwd->lockpwd[8]*100+pwd->lockpwd[9]*10+pwd->lockpwd[10];
				days = daysonsix*6+minutes/1440;
				time_set(&(pwd->limit_day),days,minutes%1440);
				td = time_diff_minute(ct,pwd->limit_day);
				if( pwd->userpwd_iscorrect==1 && td>=(-3) && td<=15 )
					pwd->iscorrect = 1;
			break;
		case mode_period:
			time_cur(&ct);
			for(i=0;i<pwd->lockpwd_len;i++)
			{
				checksum += pwd->lockpwd[i];
			}
			checksum -= pwd->lockpwd[9];
			if( pwd->lockpwd[9] == checksum%10 )
			{
				pwd->period_start = (pwd->lockpwd[7]/3)*10+pwd->lockpwd[6];
				pwd->period_end = (pwd->lockpwd[7]%3)*10+pwd->lockpwd[8];
				if( (pwd->period_start>=0) && (pwd->period_start<=23) &&  (pwd->period_end>pwd->period_start) && (pwd->period_end<=24) )
				{
					if( (pwd->userpwd_iscorrect == 1) && (ct.hour >= pwd->period_start) && (ct.hour < pwd->period_end) ){
						pwd->iscorrect = 1;
					}
				}
			}
			break;
		case mode_limit:
			time_cur2(&ct);
			year = 2019+pwd->lockpwd[4]*10+pwd->lockpwd[5];
			if( year>=ct.year && (year-ct.year)<3 )
			{
				i = pwd->lockpwd[6]*1000+pwd->lockpwd[7]*100+pwd->lockpwd[8]*10+pwd->lockpwd[9];
				if( i<8785 )
				{
					days = i/24;
					hour = i%24;
					time_findyear(year,days,&month,&day);
					time_set_ymd(&(pwd->limit_day),year,month,day,hour);
					td = time_diff_minute(pwd->limit_day,ct);
				 	if( pwd->userpwd_iscorrect==1 && td>0 && (pwd->limit_day.aday-ct.aday)<999 )
						pwd->iscorrect = 1;
				}
			}
			break;
		default:
			pwd->iscorrect = 0;
			break;
	}

	if( pwd->iscorrect==1 )
		return true;
	return false;
}

ipwd *pwd_decrypt(u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len)
{
	ipwd pwd={0};
#ifndef PLAT_GCC
	if( pwd_prepare(&pwd,userpwd,userpwdlen,lockpwd,lockpwd_len)==false ){
		return NULL;
	}
	pwd_confuse_d(&pwd);
	pwd_cross_d(&pwd);
	pwd_shiftr_d(&pwd);
	pwd_shiftl_d(&pwd);
	if( pwd_unpack_rd(&pwd)==false)
		return NULL;
	pwd_hash_d(&pwd);
	pwd_unpack(&pwd,userpwd,userpwdlen);
#else
	if( pwd_prepare(&pwd,userpwd,userpwdlen,lockpwd,lockpwd_len)==false )
		return NULL;
		pwd_snap(&pwd,0);
	pwd_confuse_d(&pwd);
		pwd_snap(&pwd,1);
	pwd_cross_d(&pwd);
		pwd_snap(&pwd,2);
	pwd_shiftr_d(&pwd);
		pwd_snap(&pwd,3);
	pwd_shiftl_d(&pwd);
		pwd_snap(&pwd,4);
	if( pwd_unpack_rd(&pwd)==false)
		return NULL;
	pwd_hash_d(&pwd);
		pwd_snap(&pwd,5);
	pwd_unpack(&pwd,userpwd,userpwdlen);
	//if( pwd.iscorrect==1 )
		pwd_print(pwd);
#endif
	return &pwd;
}
unsigned int pwd_decrypt_checkmode(u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len)
{
	ipwd *pwdtmp;
	pwdtmp=pwd_decrypt(userpwd,userpwdlen,lockpwd,lockpwd_len);
	if( pwdtmp!=NULL )
	{
		if( pwdtmp->iscorrect==1 && pwdtmp->mode==mode_count )
		{
			return pwdtmp->count;
		}
		return pwdtmp->iscorrect;
	}
	//MyPrintf("return 0;\n");
	return 0;
}

#ifndef PLAT_GCC
void AppUnlockTest(void)
{
	unsigned int i=0,j=0;
	//static ipwd *pwd;
	s_int test_result;
  	//unsigned char lockpwd[12]={1,6,7,3,7,9,9,3,7,6,3,1};
  	//unsigned char userpwd[12]={3,4,5,6,4,5,3,5};  //mode =1
  	//unsigned char lockpwd[12]={9,0,5,0,4,3,7,4,1,1,1,1};
  	//unsigned char userpwd[12]={3,4,5,6,4,5,3,5};  //mode =1
  	
    //unsigned char lockpwd[12]={8,3,5,0,9,9,3,6,0,9,6,2};
  	//unsigned char userpwd[12]={3,4,5,6,4,5,3,5};  //mode =2, 4-13,10:37 

    unsigned char lockpwd[12]={2,0,2,2,9,3,6,9,0,9,5,4};
  	unsigned char userpwd[12]={2,2,2,2,2,2};  //mode =3, 00:00~11:00 
	
        //userpwd是门锁上设定的管理员密码；
	test_result = pwd_decrypt_checkmode(userpwd,6,lockpwd,12);   //解码开锁密码lockpwd，传入整形数组的用户密码、用户密码长度、整形数组的开锁密码


    DEBUG_MARK;
}
#endif
