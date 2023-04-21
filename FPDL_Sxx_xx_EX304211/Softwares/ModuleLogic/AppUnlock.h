#ifndef AppUnlock_H
#define AppUnlock_H

extern unsigned int getNowTime(void);
extern unsigned int pwd_decrypt_checkmode(unsigned char userpwd[],unsigned int userpwdlen,unsigned char lockpwd[],unsigned int lockpwd_len);
extern void AppUnlockTest(void);

#endif

