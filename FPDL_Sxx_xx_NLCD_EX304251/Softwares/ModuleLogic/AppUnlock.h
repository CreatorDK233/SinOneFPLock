#ifndef AppUnlock_H
#define AppUnlock_H
#define NULL    ((void *)0) 
extern void init_Managerdata(void);
extern void  RestoreFactory_Managerdata(void);
extern unsigned char pwd_decrypt(unsigned char userpwd[],unsigned int userId,unsigned int userpwdlen,unsigned char lockpwd[],unsigned int  lockpwd_len,unsigned int  *pmode,unsigned int  *presult);
extern void AppUnlockTest(void);

#endif
