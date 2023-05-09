#include "AppUnlock.h"
#include "Project.h"
#include "EEPROM.h"
#include "global_variable.h"


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
u_char version[]="sl_generic_v1.0.0.007";

#define mode_serial_temp (u_char)6       //Â∫èÂàóÂè∑Ê®°ÂºèÔºåÊó†Â±èÁâà‰ΩøÁî®Ôºå1Ê¨°ÊúâÊïà‰ΩøÁî®Ê¨°Êï?
#define mode_serial_count (u_char)7      //Â∫èÂàóÂè∑Ê®°ÂºèÔºåÊó†Â±èÁâà‰ΩøÁî®Ôºå‰ΩøÁî®Ê¨°Êï∞1-999Ê¨?

#define USER_PWD_MAX_LEN  12
#define LOCK_PWD_MAX_LEN   12
#define LOCK_PWD_NOLED_MAX_LEN 13
#define DEF_MAX_SERIAL_NUM   (9999)
#define DEF_MAX_LIMIT_SERIAL_NUM   (11)
#define DEF_MAX_COUNT_NUM (999)

typedef struct _ipwd
{
	u_char  lockpwd[LOCK_PWD_NOLED_MAX_LEN+1]; 	//ÂºÄÈîÅÂØÜÁ†?
	u_int   lockpwd_len;	//ÂºÄÈîÅÂØÜÁ†ÅÁöÑÈïøÂ∫¶
	u_int   iscorrect;		//ÂØÜÁ†ÅÊòØÂê¶Ê≠£Á°Æ
	u_char  mode;					//mode_count„ÄÅmode_temporary„Ä? mode_period„ÄÅmode_limit
	u_int   rand;					//ÈöèÊú∫Êï?
	u_int   serialnum;
	u_int   count;		 		//ËÆ°Êï∞Ê®°ÂºèÁöÑÊ¨°Êï?
	u_char  userpwd[USER_PWD_MAX_LEN+1];  	//Áî®Êà∑ÂØÜÁ†Å
	u_int   userpwd_len;  	//Áî®Êà∑ÂØÜÁ†ÅÁöÑÈïøÂ∫?
	u_int   userpwd_hash[USER_PWD_MAX_LEN+1];  //Áî®Êà∑ÂØÜÁ†ÅhashÁªìÊûú
	u_int   userpwd_hash_len;  //Áî®Êà∑ÂØÜÁ†ÅhashÁöÑÈïøÂ∫?
	u_int   userpwd_iscorrect; //ÂºÄÈîÅÂØÜÁ†Å‰∏≠ÂåÖÂê´ÁöÑÁî®Êà∑ÂØÜÁ†ÅÊòØÂê¶Ê≠£Á°?
	u_char  isencrypt;
}ipwd;

u_int code hash_user[5][100]={ //‰ΩøÁî®Â§ö‰∏™Êï∞ÁªÑÂìàÂ∏åÁî®Êà∑ÂØÜÁ†ÅÔºåÂ¶ÇÊûú‰ΩøÁî?‰∏™Êï∞ÁªÑÂØπÂ∫?-12‰ΩçÊâÄÊúâÁöÑÁî®Êà∑ÂØÜÁ†ÅÔºåÂìàÂ∏åÈáçÂ§çÁöÑÊ¶ÇÁéá‰ºöÈ´òÂæàÂ§ö
		//6‰ΩçÁî®Êà∑ÂØÜÁ†?
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
			//7‰ΩçÁî®Êà∑ÂØÜÁ†?
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
			//8‰ΩçÁî®Êà∑ÂØÜÁ†?
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
			//9‰ΩçÁî®Êà∑ÂØÜÁ†?
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
			//10-12‰ΩçÁî®Êà∑ÂØÜÁ†ÅÔºå10‰ΩçÂØÜÁ†ÅÊó∂ÔºåÈÅáÂà?ÂàôÂØπÂ∫îÁöÑÊò?0Ôº?11‰ΩçÂØÜÁ†ÅÊó∂ÔºåÈÅáÂà?ÂàôÂØπÂ∫îÁöÑÊò?0ÔºåÈÅáÂà?ÂàôÂØπÂ∫îÁöÑÊò?1Ôº?12‰ΩçÂØÜÁ†ÅÊó∂ÔºåÈÅáÂà?ÂàôÂØπÂ∫îÁöÑÊò?0ÔºåÈÅáÂà?ÂàôÂØπÂ∫îÁöÑÊò?1ÔºåÈÅáÂà?ÂàôÂØπÂ∫îÁöÑÊò?2
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

u_int code hash_6bit[100]={47915,13054,41259,18369,40178,38207,25638,46817,46083,30419,
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

bool pwd_prepare(ipwd *pwd,u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len);
bool pwd_shiftr_d(ipwd *pwd);
bool pwd_shiftl_d(ipwd *pwd);
bool pwd_userpwd_select_d(ipwd *pwd,u_char userpwd[],u_int userpwdlen);
bool pwd_confuse_d(ipwd *pwd);
bool pwd_hash_d(ipwd *pwd);
bool pwd_cross_d(ipwd *pwd);
bool pwd_unpack_rd(ipwd *pwd);
bool pwd_unpack(ipwd *pwd,u_char userpwd[],u_int userId,u_int userpwdlen);

unsigned char pwd_decrypt(unsigned char userpwd[],unsigned int userId,unsigned int userpwdlen,unsigned char lockpwd[],unsigned int lockpwd_len,unsigned int *pmode,unsigned int *presult);

#if (0)
void debug_start(u_char *str)
{
#ifdef DEBUG_GCC
	printf( " %s :",str);
	debug_singleline = true;
#endif
}

void debug_print(u_char *re_str,...)
{
#ifdef DEBUG_GCC
     char buff[512]={0};
	 char cmd[550]={0};
	 u_int y=0,m=0,d=0,h=0,s=0;
	 
     va_list args;
     va_start(args, re_str);

     vsnprintf(buff,1024 ,re_str, args);
     va_end(args);

	 printf(buff);

	 u_int len = strlen(buff);
	 if( buff[len-1]==0x0A )
	 	buff[len-1] = 0;
	 //system_time(&y,&m,&d,&h,&s);
	// sprintf(cmd,"echo '%04d-%02d-%02d %02d:%02d  %s' >> /lxk/tj/backup/elog",y,m,d,h,s,buff);
	 system(cmd);
#endif
}

void debug_print_array(u_char buf[],u_int _len)
{
#ifdef DEBUG_GCC
	if( _len==12 )
		debug_print("buf=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11]);
	else if( _len==13 )
		debug_print("buf=0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x,0x%x\n",buf[0],buf[1],buf[2],buf[3],buf[4],buf[5],buf[6],buf[7],buf[8],buf[9],buf[10],buf[11],buf[12]);
#endif
}


void debug_end()
{
#ifdef DEBUG_GCC
	printf( "\n");
	debug_singleline = false;
#endif
}
#endif


void re_memcpy(u_char dst[],u_char src[],u_int len)
{
	u_int i;
	for(i=0;i<len;i++)
		dst[i] = src[i];
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
bool pwd_prepare(ipwd *pwd,u_char userpwd[],u_int userpwdlen,u_char lockpwd[],u_int lockpwd_len)
{
	if( pwd==NULL )
		return false;
	if( userpwdlen<6 || userpwdlen>USER_PWD_MAX_LEN|| lockpwd_len<LOCK_PWD_MAX_LEN|| lockpwd_len>LOCK_PWD_NOLED_MAX_LEN)
		return false;
	pwd->lockpwd_len = lockpwd_len;
	re_memcpy(pwd->lockpwd,lockpwd,pwd->lockpwd_len);
	pwd->lockpwd[lockpwd_len]=0;
	pwd->userpwd_len = userpwdlen;
	re_memcpy(pwd->userpwd,userpwd,pwd->userpwd_len);
	pwd->userpwd[userpwdlen]=0;
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

bool  pwd_userpwd_safe_d(ipwd *pwd,u_char userpwd[],u_int userpwdlen)
{
	if( pwd->mode==mode_serial_temp || pwd->mode==mode_serial_count )
	{
		u_int i=0,checksum=0;
		u_char h1,h2,h3,h4;
		for(i=0;i<userpwdlen;i++)
			checksum += userpwd[i];
		h1=(checksum%100)/10;
		h2=checksum%10;
		h3=userpwd[userpwdlen-1];
		h4=userpwd[userpwdlen-2];

		pwd->lockpwd[1] = dad(pwd->lockpwd[1],h1);
		pwd->lockpwd[2] = dad(pwd->lockpwd[2],h2);
		pwd->lockpwd[3] = dad(pwd->lockpwd[3],h3);
		pwd->lockpwd[4] = dad(pwd->lockpwd[4],h4);
		return true;
	}
   return false;
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

	pwd_userpwd_safe_d(pwd,userpwd,userpwdlen);
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
	if(pwd->mode==mode_serial_temp)
	{
		u_int d5 = 0;
		for(i=userpwdlen-1;i>=0;i--){
			if( i!=d1 && i!=d2 && i!=d3 && i!=d4 )
			{
				d5 = i;
				break;
			}
		}
		pwd->userpwd_hash[0] = pwd->lockpwd[1];
		pwd->userpwd_hash[1] = pwd->lockpwd[2];
		pwd->userpwd_hash[2] = pwd->lockpwd[3];
		pwd->userpwd_hash[3] = pwd->lockpwd[4];
		pwd->userpwd_hash[4] = pwd->lockpwd[5];
		pwd->userpwd_hash_len = 5;
		
		if( userpwd[d1]==pwd->userpwd_hash[0] && userpwd[d2]==pwd->userpwd_hash[1] && userpwd[d3]==pwd->userpwd_hash[2] && userpwd[d4]==pwd->userpwd_hash[3] &&  userpwd[d5]==pwd->userpwd_hash[4] )
			pwd->userpwd_iscorrect = 1;
	}
	else if(pwd->mode==mode_serial_count)
	{
		for(i=userpwdlen-1;i>=0;i--)
		{
			if( i!=d1 && i!=d2 && i!=d3 )
			{
				d4 = i;
				break;
			}
		}

		pwd->userpwd_hash[0] = pwd->lockpwd[1];
		pwd->userpwd_hash[1] = pwd->lockpwd[2];
		pwd->userpwd_hash[2] = pwd->lockpwd[3];
		pwd->userpwd_hash[3] = pwd->lockpwd[4];
		pwd->userpwd_hash_len = 4;


		if( userpwd[d1]==pwd->userpwd_hash[0] && userpwd[d2]==pwd->userpwd_hash[1] && userpwd[d3]==pwd->userpwd_hash[2] && userpwd[d4]==pwd->userpwd_hash[3] )
			pwd->userpwd_iscorrect = 1;

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
	u_char d1,d2,d3,d4,d5,d6,i;
	d1 = (u_char)(_data/10000);
	d2 = (u_char)((_data%10000)/1000);
	d3 = (u_char)((_data%1000)/100);
	d4 = (u_char)((_data%100)/10);
	d5 = (u_char)(_data%10);
	d6 = 0;
	i=0;
	
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
	if( pwd->lockpwd_len==LOCK_PWD_NOLED_MAX_LEN )
		pwd->mode=mode_serial_count;
	switch(pwd->mode)
	{
		case mode_serial_temp:
			pwd->rand = pwd->lockpwd[10]*10+pwd->lockpwd[11];
		break;
		case mode_serial_count:
			pwd->rand = pwd->lockpwd[0]*10+pwd->lockpwd[12]; 
		break;
	
		default:
			return false;
			break;
			
	}
	return  true;
}

void init_BackupsManagerdata(void)
{
             //µ•¥Œ ⁄»®√‹¬Î
		EEPROM_ReadSequential(BackupsSerialNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].Serialnum,4);
		if((UserIdentifyResultMgr.BackupsManagerID[0].Serialnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].Serialnum[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].Serialnum[2]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].Serialnum[3]==0xFF)
		)
		{
			UserIdentifyResultMgr.BackupsManagerID[0].Serialnum[0]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].Serialnum[1]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].Serialnum[2]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].Serialnum[3]=0x00;
			EEPROM_WriteSequential(BackupsSerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].Serialnum,4);
		}
		EEPROM_ReadSequential(BackupsSerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].Serialnum,4);
		if((UserIdentifyResultMgr.BackupsManagerID[1].Serialnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].Serialnum[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].Serialnum[2]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].Serialnum[3]==0xFF)
		)
		{
		      	UserIdentifyResultMgr.BackupsManagerID[1].Serialnum[0]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].Serialnum[1]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].Serialnum[2]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].Serialnum[3]=0x00;
			EEPROM_WriteSequential(BackupsSerialNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].Serialnum,4);
		}

		//∂‡¥Œ ⁄»®√‹¬Î
		EEPROM_ReadSequential(BackupsCountSerialNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum,4);

		if((UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum[2]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum[3]==0xFF)
		)
		{
			UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum[0]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum[1]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum[2]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum[3]=0x00;
			EEPROM_WriteSequential(BackupsCountSerialNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].CountSerialnum,4);
		}
		EEPROM_ReadSequential(BackupsCountSerialNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum,4);
		if((UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum[2]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum[3]==0xFF)
		)
		{
			UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum[0]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum[1]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum[2]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum[3]=0x00;
			EEPROM_WriteSequential(BackupsCountSerialNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].CountSerialnum,4);
		}
		
		EEPROM_ReadSequential(BackupsTempCountNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].TempCountnum,3);
		if((UserIdentifyResultMgr.BackupsManagerID[0].TempCountnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].TempCountnum[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].TempCountnum[2]==0xFF)
		)
		{
			UserIdentifyResultMgr.BackupsManagerID[0].TempCountnum[0]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].TempCountnum[1]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].TempCountnum[2]=0x00;
			EEPROM_WriteSequential(BackupsTempCountNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].TempCountnum,3);
		}
		EEPROM_ReadSequential(BackupsTempCountNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].TempCountnum,3);
		if((UserIdentifyResultMgr.BackupsManagerID[1].TempCountnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].TempCountnum[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].TempCountnum[2]==0xFF)
		)
		{
			UserIdentifyResultMgr.BackupsManagerID[1].TempCountnum[0]= 0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].TempCountnum[1]= 0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].TempCountnum[2]= 0x00;
			EEPROM_WriteSequential(BackupsTempCountNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].TempCountnum,3);
		}
		
		EEPROM_ReadSequential(BackupsCountRandNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].CountRandnum,2);
		if((UserIdentifyResultMgr.BackupsManagerID[0].CountRandnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].CountRandnum[1]==0xFF)
		)
		{
                	EEPROM_WriteSequential(BackupsCountRandNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].CountRandnum,2);

		}	
		EEPROM_ReadSequential(BackupsCountRandNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].CountRandnum,2);
		if((UserIdentifyResultMgr.BackupsManagerID[1].CountRandnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].CountRandnum[1]==0xFF)
		)
		{
                	EEPROM_WriteSequential(BackupsCountRandNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].CountRandnum,2);

		}
		EEPROM_ReadSequential(CountNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].Countnum,3);
		if((UserIdentifyResultMgr.BackupsManagerID[0].Countnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].Countnum[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].Countnum[2]==0xFF)
		)
		{
	         	EEPROM_WriteSequential(BackupsCountNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].Countnum,3);	
		}
		
		EEPROM_ReadSequential(BackupsCountNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].Countnum,3);
		if((UserIdentifyResultMgr.BackupsManagerID[1].Countnum[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].Countnum[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].Countnum[2]==0xFF)
		)
		{
	         	EEPROM_WriteSequential(BackupsCountNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].Countnum,3);	
		}
             //π‹¿Ì‘±√‹¬Î±∏≥ı ºªØ

       EEPROM_ReadSequential(BackupsManagerPasscodeMemoryStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff,12);
		if((UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[2]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[3]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[4]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[5]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[6]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[7]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[8]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[9]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[10]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[11]==0xFF)
		)
		{
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[0]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[1]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[2]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[3]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[4]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[5]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[6]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[7]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[8]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[9]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[10]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[11]=0x00;

			EEPROM_WriteSequential(BackupsManagerPasscodeMemoryStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff,12);	
		}

             	EEPROM_ReadSequential(BackupsManagerPasscodeMemoryStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff,12);
		if((UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[0]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[1]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[2]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[3]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[4]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[5]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[6]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[7]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[8]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[9]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[10]==0xFF)
		||(UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[11]==0xFF)
		)
		{

			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[0]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[1]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[2]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[3]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[4]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[5]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[6]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[7]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[8]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[9]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[10]=0x00;
			UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[11]=0x00;

	         	EEPROM_WriteSequential(BackupsManagerPasscodeMemoryStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff,12);	
		}

			 
}

void init_Managerdata(void)
{
             //µ•¥Œ ⁄»®√‹¬Î
		EEPROM_ReadSequential(SerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].Serialnum,4);
		if((UserIdentifyResultMgr.ManagerID[0].Serialnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].Serialnum[1]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].Serialnum[2]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].Serialnum[3]==0xFF)
		)
		{
			UserIdentifyResultMgr.ManagerID[0].Serialnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[0].Serialnum[1]=0x00;
			UserIdentifyResultMgr.ManagerID[0].Serialnum[2]=0x00;
			UserIdentifyResultMgr.ManagerID[0].Serialnum[3]=0x00;
			EEPROM_WriteSequential(SerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].Serialnum,4);
		}
		EEPROM_ReadSequential(SerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].Serialnum,4);
		if((UserIdentifyResultMgr.ManagerID[1].Serialnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].Serialnum[1]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].Serialnum[2]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].Serialnum[3]==0xFF)
		)
		{
		      	UserIdentifyResultMgr.ManagerID[1].Serialnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[1].Serialnum[1]=0x00;
			UserIdentifyResultMgr.ManagerID[1].Serialnum[2]=0x00;
			UserIdentifyResultMgr.ManagerID[1].Serialnum[3]=0x00;
			EEPROM_WriteSequential(SerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].Serialnum,4);
		}

		//∂‡¥Œ ⁄»®√‹¬Î
		EEPROM_ReadSequential(CountSerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].CountSerialnum,4);

		if((UserIdentifyResultMgr.ManagerID[0].CountSerialnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].CountSerialnum[1]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].CountSerialnum[2]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].CountSerialnum[3]==0xFF)
		)
		{
			UserIdentifyResultMgr.ManagerID[0].CountSerialnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[0].CountSerialnum[1]=0x00;
			UserIdentifyResultMgr.ManagerID[0].CountSerialnum[2]=0x00;
			UserIdentifyResultMgr.ManagerID[0].CountSerialnum[3]=0x00;
			EEPROM_WriteSequential(CountSerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].CountSerialnum,4);
		}
		EEPROM_ReadSequential(CountSerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].CountSerialnum,4);
		if((UserIdentifyResultMgr.ManagerID[1].CountSerialnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].CountSerialnum[1]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].CountSerialnum[2]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].CountSerialnum[3]==0xFF)
		)
		{
			UserIdentifyResultMgr.ManagerID[1].CountSerialnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[1].CountSerialnum[1]=0x00;
			UserIdentifyResultMgr.ManagerID[1].CountSerialnum[2]=0x00;
			UserIdentifyResultMgr.ManagerID[1].CountSerialnum[3]=0x00;
			EEPROM_WriteSequential(CountSerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].CountSerialnum,4);
		}
		
		EEPROM_ReadSequential(TempCountNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].TempCountnum,3);
		if((UserIdentifyResultMgr.ManagerID[0].TempCountnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].TempCountnum[1]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].TempCountnum[2]==0xFF)
		)
		{
			UserIdentifyResultMgr.ManagerID[0].TempCountnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[0].TempCountnum[1]=0x00;
			UserIdentifyResultMgr.ManagerID[0].TempCountnum[2]=0x00;
			EEPROM_WriteSequential(TempCountNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].TempCountnum,3);
		}
		EEPROM_ReadSequential(TempCountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].TempCountnum,3);
		if((UserIdentifyResultMgr.ManagerID[1].TempCountnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].TempCountnum[1]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].TempCountnum[2]==0xFF)
		)
		{
			UserIdentifyResultMgr.ManagerID[1].TempCountnum[0]= 0x00;
			UserIdentifyResultMgr.ManagerID[1].TempCountnum[1]= 0x00;
			UserIdentifyResultMgr.ManagerID[1].TempCountnum[2]= 0x00;
			EEPROM_WriteSequential(TempCountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].TempCountnum,3);
		}
		
		EEPROM_ReadSequential(CountRandNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].CountRandnum,2);
		if((UserIdentifyResultMgr.ManagerID[0].CountRandnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].CountRandnum[1]==0xFF)
		)
		{
		       UserIdentifyResultMgr.ManagerID[0].CountRandnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[0].CountRandnum[1]=0x00;
                	EEPROM_WriteSequential(CountRandNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].CountRandnum,2);

		}	
		EEPROM_ReadSequential(CountRandNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].CountRandnum,2);
		if((UserIdentifyResultMgr.ManagerID[1].CountRandnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].CountRandnum[1]==0xFF)
		)
		{
			UserIdentifyResultMgr.ManagerID[1].CountRandnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[1].CountRandnum[1]=0x00;

                	EEPROM_WriteSequential(CountRandNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].CountRandnum,2);

		}
		EEPROM_ReadSequential(CountNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].Countnum,3);
		if((UserIdentifyResultMgr.ManagerID[0].Countnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].Countnum[1]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[0].Countnum[2]==0xFF)
		)
		{
			UserIdentifyResultMgr.ManagerID[0].Countnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[0].Countnum[1]=0x00;
			UserIdentifyResultMgr.ManagerID[0].Countnum[2]=0x00;
	         	EEPROM_WriteSequential(CountNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].Countnum,3);	
		}
		
		EEPROM_ReadSequential(CountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].Countnum,3);
		if((UserIdentifyResultMgr.ManagerID[1].Countnum[0]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].Countnum[1]==0xFF)
		||(UserIdentifyResultMgr.ManagerID[1].Countnum[2]==0xFF)
		)
		{
			UserIdentifyResultMgr.ManagerID[1].Countnum[0]=0x00;
			UserIdentifyResultMgr.ManagerID[1].Countnum[1]=0x00;
			UserIdentifyResultMgr.ManagerID[1].Countnum[2]=0x00;       
	         	EEPROM_WriteSequential(CountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].Countnum,3);	
		}
          init_BackupsManagerdata();

}

void BackupsManagerPassword(void)
{
	uint8_t i;
	//ReadPasscodeUserMemoryFromEEPROM();
	for(i=0;i<12;i++)
	{
		UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff[i]=PasscodeMemoryMgr[0].PasscodeBuff[i];
	}
	UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.UserID=PasscodeMemoryMgr[0].UserID;
	EEPROM_WriteSequential(BackupsManagerPasscodeMemoryStartAddr,UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.PasscodeBuff,12);
	EEPROM_WriteSequential(BackupsManagerPwdUserIdStartAddr,&UserIdentifyResultMgr.BackupsManagerID[0].ManagerPswUserId.UserID,1);

	//ReadPasscodeMaster02MemoryFromEEPROM();
	for(i=0;i<12;i++)
	{
	UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff[i]=PasscodeMemoryMgr[1].PasscodeBuff[i];
	}
	UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.UserID=PasscodeMemoryMgr[1].UserID;
	EEPROM_WriteSequential(BackupsManagerPasscodeMemoryStartAddr1,UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.PasscodeBuff,12);
	EEPROM_WriteSequential(BackupsManagerPwdUserIdStartAddr1,&UserIdentifyResultMgr.BackupsManagerID[1].ManagerPswUserId.UserID,1);




}


void  RestoreFactory_Managerdata(void)
{
	uint8_t i;

             //µ•¥Œ ⁄»®√‹¬Î
             
			for (i=0;i<4;i++)
			{
				UserIdentifyResultMgr.ManagerID[0].Serialnum[i]= 0xFF;
			      UserIdentifyResultMgr.ManagerID[1].Serialnum[i]= 0xFF;
				UserIdentifyResultMgr.ManagerID[0].CountSerialnum[i]= 0xFF;
				UserIdentifyResultMgr.ManagerID[1].CountSerialnum[i]= 0xFF;

			}
			EEPROM_WriteSequential(SerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].Serialnum,4);
			EEPROM_WriteSequential(SerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].Serialnum,4);
			EEPROM_WriteSequential(CountSerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].CountSerialnum,4);			
			EEPROM_WriteSequential(CountSerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].CountSerialnum,4);

		//∂‡¥Œ ⁄»®√‹¬Î
			for (i=0;i<3;i++)
			{
				UserIdentifyResultMgr.ManagerID[0].TempCountnum[i]= 0xFF;
				UserIdentifyResultMgr.ManagerID[1].TempCountnum[i]= 0xFF;
				UserIdentifyResultMgr.ManagerID[0].Countnum[i]=0xFF;
				UserIdentifyResultMgr.ManagerID[1].Countnum[i]=0xFF;
			}
			EEPROM_WriteSequential(TempCountNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].TempCountnum,3);
			EEPROM_WriteSequential(TempCountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].TempCountnum,3);
			EEPROM_WriteSequential(CountNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].Countnum,3);	
			EEPROM_WriteSequential(CountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].Countnum,3);	

			for (i=0;i<2;i++)
			{
				UserIdentifyResultMgr.ManagerID[0].CountRandnum[i]= 0xFF;
				UserIdentifyResultMgr.ManagerID[1].CountRandnum[i]= 0xFF;
			}
                	EEPROM_WriteSequential(CountRandNumberStartAddr,UserIdentifyResultMgr.ManagerID[0].CountRandnum,2);
                	EEPROM_WriteSequential(CountRandNumberStartAddr1,UserIdentifyResultMgr.ManagerID[1].CountRandnum,2);				
                  BackupsManagerPassword();
		

}

void Write_Serialnum(ipwd *pwd,u_char mode,u_int userId)
{
    if( pwd==NULL )
	return;
     if(mode==mode_serial_temp)
     {

		UserIdentifyResultMgr.ManagerID[userId].Serialnum[0]=pwd->lockpwd[6];
		UserIdentifyResultMgr.ManagerID[userId].Serialnum[1]=pwd->lockpwd[7];
		UserIdentifyResultMgr.ManagerID[userId].Serialnum[2]=pwd->lockpwd[8];
		UserIdentifyResultMgr.ManagerID[userId].Serialnum[3]=pwd->lockpwd[9];
		
		UserIdentifyResultMgr.BackupsManagerID[userId].Serialnum[0]= UserIdentifyResultMgr.ManagerID[userId].Serialnum[0];
		UserIdentifyResultMgr.BackupsManagerID[userId].Serialnum[1]= UserIdentifyResultMgr.ManagerID[userId].Serialnum[1];
		UserIdentifyResultMgr.BackupsManagerID[userId].Serialnum[2]= UserIdentifyResultMgr.ManagerID[userId].Serialnum[2];
		UserIdentifyResultMgr.BackupsManagerID[userId].Serialnum[3]= UserIdentifyResultMgr.ManagerID[userId].Serialnum[3];
             
		if(userId==0)
		{
			EEPROM_WriteSequential(SerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].Serialnum,4);
			EEPROM_WriteSequential(BackupsSerialNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[userId].Serialnum,4);

		}
		else if(userId==1)
		{
			EEPROM_WriteSequential(SerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].Serialnum,4);
			EEPROM_WriteSequential(BackupsSerialNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[userId].Serialnum,4);

		}
  
     }
    else if(mode==mode_serial_count)
    {
		UserIdentifyResultMgr.ManagerID[userId].CountSerialnum[0]=pwd->lockpwd[8];
		UserIdentifyResultMgr.ManagerID[userId].CountSerialnum[1]=pwd->lockpwd[9];
		UserIdentifyResultMgr.ManagerID[userId].CountSerialnum[2]=pwd->lockpwd[10];
		UserIdentifyResultMgr.ManagerID[userId].CountSerialnum[3]=pwd->lockpwd[11];

		UserIdentifyResultMgr.ManagerID[userId].TempCountnum[0]=pwd->lockpwd[5];
		UserIdentifyResultMgr.ManagerID[userId].TempCountnum[1]=pwd->lockpwd[6];
		UserIdentifyResultMgr.ManagerID[userId].TempCountnum[2]=pwd->lockpwd[7];
		
		UserIdentifyResultMgr.ManagerID[userId].CountRandnum[0]=pwd->lockpwd[0];
		UserIdentifyResultMgr.ManagerID[userId].CountRandnum[1]=pwd->lockpwd[12];

		UserIdentifyResultMgr.BackupsManagerID[userId].CountSerialnum[0]=pwd->lockpwd[8];
		UserIdentifyResultMgr.BackupsManagerID[userId].CountSerialnum[1]=pwd->lockpwd[9];
		UserIdentifyResultMgr.BackupsManagerID[userId].CountSerialnum[2]=pwd->lockpwd[10];
		UserIdentifyResultMgr.BackupsManagerID[userId].CountSerialnum[3]=pwd->lockpwd[11];

		UserIdentifyResultMgr.BackupsManagerID[userId].TempCountnum[0]=pwd->lockpwd[5];
		UserIdentifyResultMgr.BackupsManagerID[userId].TempCountnum[1]=pwd->lockpwd[6];
		UserIdentifyResultMgr.BackupsManagerID[userId].TempCountnum[2]=pwd->lockpwd[7];
		
		UserIdentifyResultMgr.BackupsManagerID[userId].CountRandnum[0]=pwd->lockpwd[0];
		UserIdentifyResultMgr.BackupsManagerID[userId].CountRandnum[1]=pwd->lockpwd[12];

		
		if(userId==0)
		{
			EEPROM_WriteSequential(CountSerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].CountSerialnum,4);
			EEPROM_WriteSequential(TempCountNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].TempCountnum,3);
			EEPROM_WriteSequential(CountRandNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].CountRandnum,2);

			EEPROM_WriteSequential(BackupsCountSerialNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[userId].CountSerialnum,4);
			EEPROM_WriteSequential(BackupsTempCountNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[userId].TempCountnum,3);
			EEPROM_WriteSequential(BackupsCountRandNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[userId].CountRandnum,2);
		
		}
		else if(userId==1)
		{
			EEPROM_WriteSequential(CountSerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].CountSerialnum,4);
			EEPROM_WriteSequential(TempCountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].TempCountnum,3);
			EEPROM_WriteSequential(CountRandNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].CountRandnum,2);

			EEPROM_WriteSequential(BackupsCountSerialNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[userId].CountSerialnum,4);
			EEPROM_WriteSequential(BackupsTempCountNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[userId].TempCountnum,3);
			EEPROM_WriteSequential(BackupsCountRandNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[userId].CountRandnum,2);
			
		}

    }


}


u_int Get_Serialnum(u_int userId)
{    
     u_int temp_serial_num=0;
	if(userId==0)
	{
	      EEPROM_ReadSequential(SerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].Serialnum,4);
	}
	else if(userId==1)
	{
		EEPROM_ReadSequential(SerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].Serialnum,4);
	}
	temp_serial_num=UserIdentifyResultMgr.ManagerID[userId].Serialnum[0]*1000+UserIdentifyResultMgr.ManagerID[userId].Serialnum[1]*100+UserIdentifyResultMgr.ManagerID[userId].Serialnum[2]*10+UserIdentifyResultMgr.ManagerID[userId].Serialnum[3];
	return  temp_serial_num;
}
u_int Get_CountSerialnum(u_int userId)
{    
     u_int temp_countserial_num=0;
	 if(userId==0)
	{
		EEPROM_ReadSequential(CountSerialNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].CountSerialnum,4);
	}
	else if(userId==1)
	{
		EEPROM_ReadSequential(CountSerialNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].CountSerialnum,4);
	}
	

	temp_countserial_num=UserIdentifyResultMgr.ManagerID[userId].CountSerialnum[0]*1000+UserIdentifyResultMgr.ManagerID[userId].CountSerialnum[1]*100+UserIdentifyResultMgr.ManagerID[userId].CountSerialnum[2]*10+UserIdentifyResultMgr.ManagerID[userId].CountSerialnum[3];
	return  temp_countserial_num;
}
 
u_int Get_CountSerialnum_TempCount(u_int userId)
{    
     u_int temp_countserialnum_Tempcount=0;
	 if(userId==0)
	{
		EEPROM_ReadSequential(TempCountNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].TempCountnum,3);
	}
	else if(userId==1)
	{
		EEPROM_ReadSequential(TempCountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].TempCountnum,3);
	}
	temp_countserialnum_Tempcount=UserIdentifyResultMgr.ManagerID[userId].TempCountnum[0]*100+UserIdentifyResultMgr.ManagerID[userId].TempCountnum[1]*10+UserIdentifyResultMgr.ManagerID[userId].TempCountnum[2];
	return  temp_countserialnum_Tempcount;
}

u_int Get_CountRandnum(u_int userId)
{    
    u_int temp_countrand_num=0;
	 if(userId==0)
	{
		EEPROM_ReadSequential(CountRandNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].CountRandnum,2);
	}
	else if(userId==1)
	{
		EEPROM_ReadSequential(CountRandNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].CountRandnum,2);
	}
	 
	temp_countrand_num=UserIdentifyResultMgr.ManagerID[userId].CountRandnum[0]*10+UserIdentifyResultMgr.ManagerID[userId].CountRandnum[1];
	return  temp_countrand_num;
}

void Write_Count(u_int  count_num,u_int userId)
{    
	UserIdentifyResultMgr.ManagerID[userId].Countnum[0]=(uint8_t)(count_num/100);
	UserIdentifyResultMgr.ManagerID[userId].Countnum[1]=(uint8_t)((count_num%100)/10);
	UserIdentifyResultMgr.ManagerID[userId].Countnum[2]=(uint8_t)((count_num%100)%10);
	
	UserIdentifyResultMgr.BackupsManagerID[userId].Countnum[0]= UserIdentifyResultMgr.ManagerID[userId].Countnum[0];
	UserIdentifyResultMgr.BackupsManagerID[userId].Countnum[1]= UserIdentifyResultMgr.ManagerID[userId].Countnum[1];
	UserIdentifyResultMgr.BackupsManagerID[userId].Countnum[2]= UserIdentifyResultMgr.ManagerID[userId].Countnum[2];

	 if(userId==0)
	{
		EEPROM_WriteSequential(CountNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].Countnum,3);	
		EEPROM_WriteSequential(BackupsCountNumberStartAddr,UserIdentifyResultMgr.BackupsManagerID[userId].Countnum,3);	

	}
	else if(userId==1)
	{		
		EEPROM_WriteSequential(CountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].Countnum,3);	
		EEPROM_WriteSequential(BackupsCountNumberStartAddr1,UserIdentifyResultMgr.BackupsManagerID[userId].Countnum,3);
		
	}
	 

}
u_int Get_Count(u_int userId)
{    
     u_int temp_count_num=0;
	 if(userId==0)
	{
		EEPROM_ReadSequential(CountNumberStartAddr,UserIdentifyResultMgr.ManagerID[userId].Countnum,3);
	}
	else if(userId==1)
	{
		EEPROM_ReadSequential(CountNumberStartAddr1,UserIdentifyResultMgr.ManagerID[userId].Countnum,3);
	}

	temp_count_num=UserIdentifyResultMgr.ManagerID[userId].Countnum[0]*100+UserIdentifyResultMgr.ManagerID[userId].Countnum[1]*10+UserIdentifyResultMgr.ManagerID[userId].Countnum[2];
	return  temp_count_num;
}

bool pwd_unpack(ipwd *pwd,u_char userpwd[],u_int userId,u_int userpwdlen)
{
	u_int temp_serialnum=0;
	u_int temp_countserialnum=0;
	u_int temp_countnum=0;
	u_int temp_countrandnum=0;
	u_int countserialnum_Tempcount=0;
	
	if( pwd==NULL )
		return false;
	pwd->iscorrect = 0;
	if( pwd_userpwd_select_d(pwd,userpwd,userpwdlen)==false )
		return false;
	switch(pwd->mode)
	{
		case mode_serial_temp:
			pwd->serialnum=pwd->lockpwd[6]*1000+pwd->lockpwd[7]*100+pwd->lockpwd[8]*10+pwd->lockpwd[9];
			if( pwd->userpwd_iscorrect==1 )
			{
			   
			     	temp_serialnum=Get_Serialnum(userId);
			      if(pwd->serialnum<(DEF_MAX_SERIAL_NUM+1))
				{
				          if(temp_serialnum<pwd->serialnum)	
					{
					   pwd->iscorrect = 1;
					}		
					else if((temp_serialnum==DEF_MAX_SERIAL_NUM)&&(pwd->serialnum<DEF_MAX_LIMIT_SERIAL_NUM))
					{				    
	                                 pwd->iscorrect = 1;
					}
					else
					{
	                                 pwd->iscorrect = 0;
					}
			   }

			}
			break;
		case mode_serial_count:
			pwd->count=pwd->lockpwd[5]*100+pwd->lockpwd[6]*10+pwd->lockpwd[7];
			pwd->serialnum=pwd->lockpwd[8]*1000+pwd->lockpwd[9]*100+pwd->lockpwd[10]*10+pwd->lockpwd[11];
			if( pwd->userpwd_iscorrect==1 )
			{
				temp_countserialnum=Get_CountSerialnum(userId);
				temp_countnum=Get_Count(userId);					
				countserialnum_Tempcount=Get_CountSerialnum_TempCount(userId);
				temp_countrandnum=Get_CountRandnum(userId);	
				if(pwd->serialnum<(DEF_MAX_SERIAL_NUM+1))
				{
					if(temp_countserialnum<DEF_MAX_SERIAL_NUM)
					{
						if(temp_countserialnum<pwd->serialnum)
						{
							pwd->iscorrect = 1;	 
							temp_countnum=1;	
							Write_Count(temp_countnum,userId);	
						}
						else if(temp_countserialnum==pwd->serialnum)
						{	
							if(countserialnum_Tempcount !=pwd->count)
							{
								pwd->iscorrect = 1;
								temp_countnum=1;	
								Write_Count(temp_countnum,userId);	
							}
							else
							{
								if(temp_countrandnum!=pwd->rand)
								{
									pwd->iscorrect = 1;	 
									temp_countnum=1;	
									Write_Count(temp_countnum,userId);
								}											
								if(temp_countnum<pwd->count)
								{						
									pwd->iscorrect = 1;
									temp_countnum=temp_countnum+1;
									Write_Count(temp_countnum,userId);	
								}

							}
											  
						}
						else
						{				
							pwd->iscorrect = 0;	    
						}
					}
					else if(temp_countserialnum==DEF_MAX_SERIAL_NUM)
					{  

						if(pwd->serialnum<DEF_MAX_LIMIT_SERIAL_NUM)
						{
							pwd->iscorrect = 1;	 
							temp_countnum=1;	
							Write_Count(temp_countnum,userId);
						}
						if(pwd->serialnum==temp_countserialnum)
						{     
							if(countserialnum_Tempcount !=pwd->count)
							{
								pwd->iscorrect = 1;	 
								temp_countnum=1;	
								Write_Count(temp_countnum,userId);
							}
							else
							{
								if(temp_countrandnum!=pwd->rand)
								{
									pwd->iscorrect = 1;	 
									temp_countnum=1;	
									Write_Count(temp_countnum,userId);
								}
								if(temp_countnum<pwd->count)
								{						
									pwd->iscorrect = 1;
									temp_countnum=temp_countnum+1;
									Write_Count(temp_countnum,userId);	
								}
							}
						      
						}


					}

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


#ifndef PLAT_GCC
unsigned char pwd_decrypt(unsigned char userpwd[],unsigned int userId,unsigned int userpwdlen,unsigned char lockpwd[],unsigned int lockpwd_len,unsigned int *pmode,unsigned int *presult)
{
	ipwd pwd={0};
	u_int temp_count=0;
	if( pwd_prepare(&pwd,userpwd,userpwdlen,lockpwd,lockpwd_len)==false )
		return false;
	pwd_confuse_d(&pwd);
	pwd_cross_d(&pwd);
	pwd_shiftr_d(&pwd);
	pwd_shiftl_d(&pwd);
	if( pwd_unpack_rd(&pwd)==false)
		return false;
	pwd_hash_d(&pwd);
	pwd_unpack(&pwd,userpwd,userId,userpwdlen);
	if( pmode!=NULL && presult!=NULL )
	{
		*pmode = pwd.mode;
		if( pwd.mode==mode_serial_count)
			*presult = pwd.count;
		else
			*presult = 0;
	}
	if(pwd.mode==mode_serial_temp)
	{
		if(1== pwd.iscorrect)
		{
		       //µ•¥Œ ⁄»®√‹¬Î∫Õ∂‡¥Œ ⁄»®√‹¬Î»Áπ˚π≤”√“ª∏ˆ–Ú¡–∫≈±‰¡ø ±£¨“™∏¸–¬º∆ ˝∆˜«Â¡„°£
			//temp_count=0;
			//Write_Count(temp_count);
			Write_Serialnum(&pwd,mode_serial_temp,userId);

		}

	}
	else if(pwd.mode==mode_serial_count)
	{
		if(1== pwd.iscorrect)
		Write_Serialnum(&pwd,mode_serial_count,userId);
		temp_count=Get_Count(userId);
		if(temp_count>pwd.count)
		{ 
		     if(temp_count>DEF_MAX_COUNT_NUM )
		     {
	                   temp_count=0;
		            Write_Count(temp_count,userId);
		    }
			pwd.iscorrect=0;
	      }
		 
	}
	return pwd.iscorrect;
}

void AppUnlockTest(void)
{
	//unsigned int i=0,j=0;
	u_int mode=0,result=0;
	bool  ret = false;
  	//unsigned char lockpwd[12]={1,6,7,3,7,9,9,3,7,6,3,1};
  	//unsigned char userpwd[12]={3,4,5,6,4,5,3,5};  //mode =1
  	//unsigned char lockpwd[12]={9,0,5,0,4,3,7,4,1,1,1,1};
  	//unsigned char userpwd[12]={3,4,5,6,4,5,3,5};  //mode =1
  	
    //unsigned char lockpwd[12]={8,3,5,0,9,9,3,6,0,9,6,2};
  	//unsigned char userpwd[12]={3,4,5,6,4,5,3,5};  //mode =2, 4-13,10:37 

    unsigned char lockpwd[12]={2,0,2,2,9,3,6,9,0,9,5,4};
  	unsigned char userpwd[12]={3,4,5,6,4,5,3,5};  //mode =3, 00:00~11:00 
	
        //userpwdÊòØÈó®ÈîÅ‰∏äËÆæÂÆöÁöÑÁÆ°ÁêÜÂëòÂØÜÁ†ÅÔº?
	ret = pwd_decrypt(userpwd,0,8,lockpwd,12,&mode,&result); //Ëß£Á†ÅÂºÄÈîÅÂØÜÁ†ÅlockpwdÔºå‰º†ÂÖ•Êï¥ÂΩ¢Êï∞ÁªÑÁöÑÁî®Êà∑ÂØÜÁ†Å„ÄÅÁî®Êà∑ÂØÜÁ†ÅÈïøÂ∫¶„ÄÅÊï¥ÂΩ¢Êï∞ÁªÑÁöÑÂºÄÈîÅÂØÜÁ†?

    DEBUG_MARK;
}
#endif
