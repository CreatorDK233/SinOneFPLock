

#ifndef __types_h
#define __types_h

typedef   signed           char INT8;
typedef   signed      		 int  INT16;
typedef   signed  long     int  INT32;

    /* exact-width unsigned integer types */
typedef unsigned   		char 					UINT8;
typedef unsigned 			char      		u8;
typedef unsigned      int  					UINT16;
typedef unsigned      int  					u16;
typedef unsigned   		long    int  	UINT32;
typedef unsigned   		long    int  	u32;
typedef signed 				long 		int		SINT32;

//typedef enum {FALSE = 0, TRUE = 1} Boolean;
//#define IS_BOOLEAN(bool) ((bool == FALSE) || (bool == TRUE))

//typedef unsigned long int uint32_t;
//typedef unsigned int 			uint16_t;
//typedef unsigned char 		uint8_t;

typedef unsigned           char BYTE;
typedef unsigned 			     int  WORD;
typedef unsigned  long     int  DWORD;
typedef unsigned           char * PBYTE;
typedef unsigned 			     int  * PWORD;
typedef unsigned  long     int  * PDWORD;

typedef volatile unsigned	char VUINT8;
typedef volatile unsigned	char vu8;
typedef volatile unsigned	int  VUINT16;
typedef volatile unsigned	int  vu16;
typedef volatile unsigned	long	int  VUINT32;
typedef volatile unsigned	long	int  vu32;

//typedef signed long      s32;
typedef signed int     s16;
typedef signed char      s8;

typedef enum
{
  FALSE = 0, TRUE  = !FALSE
}
bool;

#endif 




