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

code  uint8_t PleaseSwingCardStr[]={HZ_qing,HZ_shua,HZ_ka,HZ_end};//��ˢ��
//code  uint8_t PleaseSwingCardStrEn[]={"PLS swing card"};
 
code uint8_t CardCIDisBeUsedStr[]={HZ_chong,HZ_fu,HZ_ka,HZ_hao,HZ_end};
//code uint8_t CardCIDisBeUsedStrEn[]={"Repeated card"}; 
 
code uint8_t AddMasterStr[]={HZ_kongge,HZ_qing,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_end}; 
//code uint8_t AddMasterStrEn[]={"Add Master First"}; 

code uint8_t UsersFullStr[]={HZ_yong,HZ_hu,HZ_yi,HZ_man,HZ_end}; 
//code uint8_t UsersFullStrEn[]={"Users Are Full"};

code uint8_t ConfirmRestoreFactoryDefaultStr1[]={HZ_anya,ZF_jinghao,HZ_que,HZ_ding,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_end};
code uint8_t ConfirmRestoreFactoryDefaultStr2[]={HZ_anya,ZF_xinghao,HZ_fan,HZ_huijia,HZ_end};
//code uint8_t PressConfirm[]={"Press # to cfm"}; //ȷ��ɾ��
//code uint8_t PressReturn[]={"Press * to RTN"};	//��*����
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
	{ZF_1,ZF_xiaoshudian,HZ_zhi,HZ_wen,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_end}, 					//ָ���û�����		
	{ZF_2,ZF_xiaoshudian,HZ_mi,HZ_ma,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_end},						//�����û�����		
	{ZF_3,ZF_xiaoshudian,HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_guan,HZ_li,HZ_end}, 					//��Ƭ�û����� 
	{ZF_4,ZF_xiaoshudian,HZ_xi,HZ_tong,HZ_she,HZ_zhiwei,HZ_end},			//ϵͳ�趨	
	{ZF_5,ZF_xiaoshudian,HZ_ji,HZ_lu,HZ_cha,HZ_xun,HZ_end},				//��¼��ѯ
	//{HZ_kongge,HZ_kongge,HZ_kongge,HZ_lian,HZ_wang,HZ_kongge,HZ_kongge,HZ_kongge,HZ_end}		//����				
};	
//code uint8_t MainMenuStrEn[][16]= 
//{ 
//	{"1.FP user"},				//ָ���û�����		
//	{"2.PW user"},				//�����û�����		
//	{"3.Card user"},				//��Ƭ�û����� 
//	{"4.System CFG"},			 	//ϵͳ�趨	
//	//{"   Event log   "},				//������¼	
//	//{"    NetWork"}					//��������
//	
//};


code uint8_t FpMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_zhi,HZ_wen,HZ_end}, 		//��ӹ���Աָ��
	{ZF_2,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_yong,HZ_hu,HZ_zhi,HZ_wen,HZ_end},			//����û�ָ��			
	//{HZ_kongge,HZ_xie,HZ_po,HZ_yong,HZ_hu,HZ_zhi,HZ_wen,HZ_kongge,HZ_end}				//в���û�ָ�ƹ���
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_yong,HZ_hu,HZ_zhi,HZ_wen,HZ_end}					//ɾ������ָ��
};	
//code uint8_t FpMenuStrEn[][12]= 
//{ 
//	{"1.FP Admin."},					//����Աָ�ƹ���
//	{"2.FP User"}, 				//��ͨ�û�ָ�ƹ���			
//	//{" FP Special"} 				//в���û�ָ�ƹ���	
//	{"3.Del all FP"} 				//ɾ������ָ��		
//};


code uint8_t CardUserMenuStr[][10]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_ka,HZ_pian,HZ_yong,HZ_hu,HZ_end}, 				//��ӿ�Ƭ�û�
	//{HZ_kongge,HZ_shan,HZ_chufa,HZ_dan,HZ_yier,HZ_yong,HZ_hu,HZ_kongge,HZ_end}, 				//ɾ�������û�				   
	{ZF_2,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_ka,HZ_pian,HZ_end},					//ɾ�����п�Ƭ�û�				   
};	
//code uint8_t CardUserMenuStrEn[][16]= 
//{ 
//	{"1.Add user"},					//������û�
//	//{"Delete user	 "},					//ɾ�������û�				   
//	{"2.Del all user"},					//ɾ�������û�				   
//};	

code uint8_t PassCodeMenuStr[][11]= 
{ 
	{ZF_1,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_guan,HZ_li,HZ_yuan,HZ_mi,HZ_ma,HZ_end},	//����Ա����
	{ZF_2,ZF_xiaoshudian,HZ_tian,HZ_jia,HZ_yong,HZ_hu,HZ_mi,HZ_ma,HZ_end},				//��ͨ�û�����
	{ZF_3,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_suo,HZ_you,HZ_yong,HZ_hu,HZ_mi,HZ_ma,HZ_end},		//ɾ�������û�		
};	
//code uint8_t PassCodeMenuStrEn[][16]= 
//{ 
//	{"1.PW admin."},			//����Ա����
//	{"2.PW user  "},			//��ͨ�û�����	
//	{"3.Del all user"},			//ɾ�������û�		
//};	

code uint8_t EventLogMenuStr[][9]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_ji,HZ_lu,HZ_cha,HZ_xun,HZ_end},			//��˳���ѯ
		//{ZF_2,ZF_xiaoshudian,HZ_anya,HZ_ri,HZ_qi,HZ_cha,HZ_xun,HZ_end},			//�����ڲ�ѯ
		{ZF_2,ZF_xiaoshudian,HZ_shan,HZ_chufa,HZ_ji,HZ_lu,HZ_end}				//ɾ����¼
};	
//code uint8_t EventLogMenuStrEn[][16]= 
//{ 
//		{"1.Sequence check"},			//��˳���ѯ
//		{"2.Check by date"},			//�����ڲ�ѯ
//		{"3.Delete all log"}				//ɾ����¼
//};		


code uint8_t SytemConfigMenuStr[][9]= 
{ 
		{ZF_1,ZF_xiaoshudian,HZ_shi,HZ_jian,HZ_she,HZ_zhiwei,HZ_end}, 		//ʱ������
//		{ZF_2,ZF_xiaoshudian,HZ_yuyan,HZ_yanyu,HZ_she,HZ_zhiwei,HZ_end}, 		//�����趨
		{ZF_2,ZF_xiaoshudian,HZ_yuyan,HZ_yin,HZ_she,HZ_zhiwei,HZ_end},		//��������
		//{HZ_kongge,HZ_nei,HZ_cun,HZ_guan,HZ_li,HZ_kongge,HZ_kongge,HZ_kongge,HZ_end},			//�ڴ����
		{ZF_3,ZF_xiaoshudian,HZ_changchang,HZ_kai,HZ_mo,HZ_si,HZ_she,HZ_zhiwei,HZ_end}, //��������
		{ZF_4,ZF_xiaoshudian,HZ_kai,HZ_suomen,HZ_mo,HZ_si,HZ_she,HZ_zhiwei,HZ_end},			//����ģʽ
		{ZF_5,ZF_xiaoshudian,HZ_fangzi,HZ_qiao,HZ_bao,HZ_jing,HZ_she,HZ_zhiwei,HZ_end}, 		//���˱�������
		//{HZ_kongge,HZ_xi,HZ_tong,HZ_ban,HZ_ben,HZ_kongge,HZ_kongge,HZ_kongge,HZ_end},			//ϵͳ�汾
		{ZF_6,ZF_xiaoshudian,HZ_hui,HZ_fu,HZ_chu,HZ_chang,HZ_she,HZ_zhiwei,HZ_end}				//�ָ���������
};	
//code uint8_t SytemConfigMenuStrEn[][17]= 
//{ 
//		//{"Time setting   "},			//ʱ���趨
//		{"1.Language"},			//����
//		{"2.Volume"},			//��������
//		//{"Memory Check   "},			//�ڴ����
//		{"3.Often open"},			//��������
//		{"4.Unlock mode"},			//����ģʽ
//		{"5.Anti prying"},			//���˱�������
//		//{"System version "},			//ϵͳ�汾
//		{"6.Factory default"} 			//�ָ���������
//};	
#endif