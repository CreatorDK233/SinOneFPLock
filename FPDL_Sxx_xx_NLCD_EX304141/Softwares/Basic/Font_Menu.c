#include "Font_Menu.h"
#include "Project.h"
#include "Font.h"

#ifdef Function_ScreenDisplay
code uint8_t FpUserStr[]={HZ_zhi,HZ_wen,HZ_yong,HZ_hu,HZ_end};
//code uint8_t FpUserStrEn[]={"FP user"};

code uint8_t CardUserStr[]={HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_end};
//code uint8_t CardUserStrEn[]={"CD user"};

code uint8_t PasscodeUserStr[]={HZ_mi,HZ_ma,HZ_yong,HZ_hu,HZ_end};
//code uint8_t PasscodeUserStrEn[]={"PW user"};

code uint8_t IdentifyFailStr[]={HZ_yan,HZ_zheng,HZ_shibai,HZ_bai,HZ_end};
//code uint8_t IdentifyFailStrEn[]={"Identify Fail"};

//code uint8_t RemoteUnlockStr[]={HZ_yuanchu,HZ_chengxu,HZ_kai,HZ_suomen,HZ_end};
//code uint8_t RemoteUnlockStrEn[]={"Remote"};

code	uint8_t FPHardwareMismatchStr[]={HZ_zhi,HZ_wen,HZ_ying,HZ_jianshu,HZ_bu,HZ_pi,HZ_pei,HZ_end};
code	uint8_t PLSRestoreFactoryStr[]={HZ_qing,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_end};

//code uint8_t TemporaryPasswordStr[]={HZ_lin,HZ_shi,HZ_mi,HZ_ma,HZ_end};
//code uint8_t TemporaryPasswordStrEn[]={"Temp PW"};

code uint8_t UserIDStr[]={HZ_bian,HZ_hao,ZF_maohao,HZ_end};
//code uint8_t UserIDStrEn[]={"ID:"};

//code uint8_t Yes[]={HZ_kongge,HZ_shifou,HZ_kongge,HZ_end};
//code uint8_t YesEn[]={"YES"};

//code uint8_t No[]={HZ_kongge,HZ_fou,HZ_kongge,HZ_end};
//code uint8_t NoEn[]={"NO"};

code uint8_t PutFingerStr[]={HZ_kongge,HZ_qing,HZ_fang,HZ_zhiwei,HZ_shou,HZ_zhi,HZ_end}; 
//	code uint8_t PutFingerStrEn[]={"<Put Finger>"};

code uint8_t InputPasscodeStr[]={HZ_qing,HZ_shu,HZ_ru,HZ_mi,HZ_ma,HZ_end};
//code uint8_t InputPasscodeStrEn[]={"PLS input PW"};
code uint8_t ConfirmPasscode[]={HZ_qing,HZ_que,HZ_ding,HZ_mi,HZ_ma,HZ_end};
//code uint8_t ConfirmPasscodeEn[]={"Confirm PW"};

code uint8_t OperationSuccessStr[]={HZ_cao,HZ_zuo,HZ_cheng,HZ_gong,HZ_end}; 
//code uint8_t OperationSuccessStrEn[]={"Operation Success"};
code uint8_t OperationFailStr[]={HZ_cao,HZ_zuo,HZ_shibai,HZ_bai,HZ_end}; 
//code uint8_t OperationFailStrEn[]={"Operation Fail"};
code uint8_t PasscodeisBeUsedStr[]={HZ_chong,HZ_fu,HZ_mi,HZ_ma,HZ_end}; 

code uint8_t InputUserIDStr[]={HZ_qing,HZ_shu,HZ_ru,HZ_bian,HZ_hao,HZ_end};
//code uint8_t InputUserIDStrEn[]={"PLS input ID"};

code  uint8_t PleaseSwingCardStr[]={HZ_qing,HZ_shua,HZ_ka,HZ_end};//请刷卡
//code  uint8_t PleaseSwingCardStrEn[]={"PLS swing card"};
 
code uint8_t CardCIDisBeUsedStr[]={HZ_chong,HZ_fu,HZ_ka,HZ_hao,HZ_end};
//code uint8_t CardCIDisBeUsedStrEn[]={"Repeated card"}; 
 
code uint8_t AddMasterStr[]={HZ_kongge,HZ_qing,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_end}; 
//code uint8_t AddMasterStrEn[]={"Add Master First"}; 

code uint8_t UsersFullStr[]={HZ_yong,HZ_hu,HZ_yi,HZ_man,HZ_end}; 
//code uint8_t UsersFullStrEn[]={"Users Are Full"};

code uint8_t ConfirmRestoreFactoryDefaultStr1[]={HZ_anya,ZF_jinghao,HZ_que,HZ_ding,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_end};
code uint8_t ConfirmRestoreFactoryDefaultStr2[]={HZ_anya,ZF_xinghao,HZ_fan,HZ_huijia,HZ_end};
//code uint8_t PressConfirm[]={"Press # to cfm"}; //确认删除
//code uint8_t PressReturn[]={"Press * to RTN"};	//按*返回
code uint8_t ConfirmDEL[]={HZ_anya,ZF_jinghao,HZ_que,HZ_ding,HZ_shan,HZ_chufa,HZ_end};
code uint8_t CancelDEL[]={HZ_anya,ZF_xinghao,HZ_fan,HZ_huijia,HZ_end};



code uint8_t UnlockLogStr[]={HZ_kai,HZ_suomen,HZ_ji,HZ_lu,ZF_hengan,HZ_end};
//const uint8_t UnlockLogStrEn[]={"Unlock-"}; 
code uint8_t AddUserStr[]={HZ_tian,HZ_jia,HZ_yong,HZ_hu,ZF_hengan,HZ_end};
//const uint8_t AddUserStrEn[]={"Add User-"};
code uint8_t DeleteUserStr[]={HZ_shan,HZ_chufa,HZ_yong,HZ_hu,ZF_hengan,HZ_end};
//const uint8_t DeleteUserStrEn[]={"Del. User-"};
code uint8_t LotStr[]={HZ_piliang,HZ_liang,HZ_end};
//const uint8_t LotStrEn[]={"Lot"};
code uint8_t TemporaryPasswordStr[]={HZ_lin,HZ_shi,HZ_mi,HZ_ma,HZ_end};

code uint8_t MainMenuStr[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_zhi,HZ_wen,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_end}, 					//指纹用户管理		
	{ZF_2,ZF_xiaoshudian,HZ_mi,HZ_ma,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_end},						//密码用户管理		
	{ZF_3,ZF_xiaoshudian,HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_end}, 					//卡片用户管理 
	{ZF_4,ZF_xiaoshudian,HZ_xi,HZ_tong,HZ_she,HZ_zhiwei,HZ_end},			//系统设定	
	{ZF_5,ZF_xiaoshudian,HZ_ji,HZ_lu,HZ_cha,HZ_xun,HZ_end},				//记录查询
	//{HZ_kongge,HZ_kongge,HZ_kongge,HZ_lian,HZ_wang,HZ_kongge,HZ_kongge,HZ_kongge,HZ_end}		//联网				
};	
//code uint8_t MainMenuStrEn[][16]= 
//{ 
//	{"1.FP user"},				//指纹用户管理		
//	{"2.PW user"},				//密码用户管理		
//	{"3.Card user"},				//卡片用户管理 
//	{"4.System CFG"},			 	//系统设定	
//	//{"   Event log   "},				//开锁记录	
//	//{"    NetWork"}					//联网配置
//	
//};


code uint8_t FpMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_zhi,HZ_wen,HZ_end}, 		//添加管理员指纹
	{ZF_2,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_yong,HZ_hu,HZ_zhi,HZ_wen,HZ_end},			//添加用户指纹			
	//{HZ_kongge,HZ_xie,HZ_po,HZ_yong,HZ_hu,HZ_zhi,HZ_wen,HZ_kongge,HZ_end}				//胁迫用户指纹管理
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_yong,HZ_hu,HZ_zhi,HZ_wen,HZ_end}					//删除所有指纹
};	
//code uint8_t FpMenuStrEn[][12]= 
//{ 
//	{"1.FP Admin."},					//管理员指纹管理
//	{"2.FP User"}, 				//普通用户指纹管理			
//	//{" FP Special"} 				//胁迫用户指纹管理	
//	{"3.Del all FP"} 				//删除所有指纹		
//};


code uint8_t CardUserMenuStr[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_end}, 				//添加卡片用户
	//{HZ_kongge,HZ_shan,HZ_chufa,HZ_dan,HZ_yier,HZ_yong,HZ_hu,HZ_kongge,HZ_end}, 				//删除已有用户				   
	{ZF_2,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_ka,HZ_pian,HZ_end},					//删除所有卡片用户				   
};	
//code uint8_t CardUserMenuStrEn[][16]= 
//{ 
//	{"1.Add user"},					//添加新用户
//	//{"Delete user	 "},					//删除已有用户				   
//	{"2.Del all user"},					//删除所有用户				   
//};	

code uint8_t PassCodeMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_mi,HZ_ma,HZ_end},	//管理员密码
	{ZF_2,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_yong,HZ_hu,HZ_mi,HZ_ma,HZ_end},				//普通用户密码
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_yong,HZ_hu,HZ_mi,HZ_ma,HZ_end},		//删除所有用户		
};	
//code uint8_t PassCodeMenuStrEn[][16]= 
//{ 
//	{"1.PW admin."},			//管理员密码
//	{"2.PW user  "},			//普通用户密码	
//	{"3.Del all user"},			//删除所有用户		
//};	

code uint8_t EventLogMenuStr[][9]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_ji,HZ_lu,HZ_cha,HZ_xun,HZ_end},			//按顺序查询
		//{ZF_2,ZF_xiaoshudian,HZ_anya,HZ_ri,HZ_qi,HZ_cha,HZ_xun,HZ_end},			//按日期查询
		{ZF_2,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_ji,HZ_lu,HZ_end}				//删除记录
};	
//code uint8_t EventLogMenuStrEn[][16]= 
//{ 
//		{"1.Sequence check"},			//按顺序查询
//		{"2.Check by date"},			//按日期查询
//		{"3.Delete all log"}				//删除记录
//};		


code uint8_t SytemConfigMenuStr[][9]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_shi,HZ_jian,HZ_she,HZ_zhiwei,HZ_end}, 		//时间设置
//		{ZF_2,ZF_xiaoshudian,HZ_yuyan,HZ_yanyu,HZ_she,HZ_zhiwei,HZ_end}, 		//语言设定
		{ZF_2,ZF_xiaoshudian,HZ_yuyan,HZ_yin,HZ_she,HZ_zhiwei,HZ_end},		//语音设置
		//{HZ_kongge,HZ_nei,HZ_cun,HZ_guan,HZ_li,HZ_kongge,HZ_kongge,HZ_kongge,HZ_end},			//内存管理
		{ZF_3,ZF_xiaoshudian,HZ_changchang,HZ_kai,HZ_mo,HZ_si,HZ_she,HZ_zhiwei,HZ_end}, //常开设置
		{ZF_4,ZF_xiaoshudian,HZ_kai,HZ_suomen,HZ_mo,HZ_si,HZ_she,HZ_zhiwei,HZ_end},			//开锁模式
		{ZF_5,ZF_xiaoshudian,HZ_fangzi,HZ_qiao,HZ_bao,HZ_jing,HZ_she,HZ_zhiwei,HZ_end}, 		//防撬报警设置
		//{HZ_kongge,HZ_xi,HZ_tong,HZ_ban,HZ_ben,HZ_kongge,HZ_kongge,HZ_kongge,HZ_end},			//系统版本
		{ZF_6,ZF_xiaoshudian,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_she,HZ_zhiwei,HZ_end}				//恢复出厂设置
};	
//code uint8_t SytemConfigMenuStrEn[][17]= 
//{ 
//		//{"Time setting   "},			//时间设定
//		{"1.Language"},			//语言
//		{"2.Volume"},			//音量调节
//		//{"Memory Check   "},			//内存管理
//		{"3.Often open"},			//常开设置
//		{"4.Unlock mode"},			//开锁模式
//		{"5.Anti prying"},			//防撬报警开关
//		//{"System version "},			//系统版本
//		{"6.Factory default"} 			//恢复出厂设置
//};	
#endif