#include "types.h"
typedef u16 parity_data_t;

/*error number:*/
#define M1_ERR_NONE			0
#define M1_ERR_NOMSG		-1
#define M1_ERR_PARAM		-2
#define M1_ERR_NOTFOUND		-3
#define M1_ERR_TIMEOUT		-4
#define M1_ERR_FIFO   		-5
#define M1_ERR_REQUEST		-6

/*! Size of the intermal MiFare transmit buffer. FIXME: Should be reduced to 18 */
#define MIFARE_TRANSMIT_BUFFER_SIZE         20

#define KEYA 								0
#define KEYB 								1

#define MIFARE_AUTH_KEY_A                   0x60
#define MIFARE_AUTH_KEY_B                   0x61
#define MIFARE_READ_BLOCK                   0x30
#define MIFARE_WRITE_BLOCK                  0xA0
#define MIFARE_TRANSFER_BLOCK      			0xB0    /* transfer command code		*/

#define MIFARE_RESTORE_BLOCK                0xC2	/*restore command code	*/
#define MIFARE_INCREMENT_BLOCK              0xC1    /*increment value*/
#define MIFARE_DECREMENT_BLOCK              0xC0    /*decrement value	*/
#define MIFARE_HALT          				0x50    /* halt				*/


#define MIFARE_ACK                          0x0A
#define MIFARE_NACK_NOT_ALLOWED             0x04
#define MIFARE_NACK_TRANSMISSION_ERROR      0x04

/*error number*/
#define MIF_OK           0x9000
#define MIF_NOTAUTHERR 	 0x6982  
#define MIF_AUTH_ERR     0x6283
#define MIF_CODERR       0x6ff1
#define MIF_BITCOUNTERR  0x6ff2
#define MIF_WRITEERR     0x6ff3
#define MIF_VALERR       0x6ff4
#define MIF_TIMEOVER     0x6ff5
#define MIF_KEYFORMATERR 0x6ff6
#define MIF_TRANSFER_ERR 0x6ff7
#define MIF_LENGTH_ERR	 0x6ff8
#define MIF_CALC_ERR	 0x6ff9 

#define MIF_ERROR        0x6ff0

#define MIFARE_DEFAULT_READER_NONCE             0xAA55AA55

extern unsigned char  ODD_PARITY(unsigned char i);
/*! Calculate the odd parity bit for one byte */
//#define ODD_PARITY(i) (( (i) ^ (i)>>1 ^ (i)>>2 ^ (i)>>3 ^ (i)>>4 ^ (i)>>5 ^ (i)>>6 ^ (i)>>7 ^ 1) & 0x01)

/*!
 * Like UINT32_TO_ARRAY, but put the correct parity in the 8th bit of each array
 * entry (thus the array entries must at least be 16 bit wide)
 */
#define UINT32_TO_ARRAY_WITH_PARITY(i, a) do{ \
	(a)[0] = ((i)>>24) & 0xff; (a)[0] |= ODD_PARITY((a)[0])<<8; \
	(a)[1] = ((i)>>16) & 0xff; (a)[1] |= ODD_PARITY((a)[1])<<8; \
	(a)[2] = ((i)>> 8) & 0xff; (a)[2] |= ODD_PARITY((a)[2])<<8; \
	(a)[3] = ((i)>> 0) & 0xff; (a)[3] |= ODD_PARITY((a)[3])<<8; \
} while(0);

/*!
 *****************************************************************************
 * Set the MiFare key.
 *
 * \note: The array pointed to by \a key must be at least 6 bytes long.
 *
 * \param key Pointer to an array storing the key.
 *****************************************************************************
 */
void mifareSetKey(const u8 *key);//

/*!
 *****************************************************************************
 * \brief Reset the MiFare cipher.
 *
 * Reset the MiFare crypto1 engine and deactive encryption/decryption for
 * communications. This must be called when prior to communicating with a
 * new card.
 *****************************************************************************
 */
 
void mifareResetCipher(void);
u8 Get_Mifare_uid(u8 *uid);//
u16 Mifare_Auth_Fun(u8 *keydata, u8 key_index, u8 *uid, u8 block);
u16 Mif_read (u8 blockAdd, u8 * response, u16 *responseLength);
//u16 Mif_write (u8 blockAdd, u8 * send_value, u16 send_length);
//u16 Mif_Transfer (u8 blockAdd);
//u16 Mif_InitPurchase (u8 blockAdd, u8 * send_value,u16 send_length);
//u16 Mif_Restore (u8 blockAddr,u8 *send_value);
//u16 Mif_increment (u8 blockAdd, u8 * send_value);
//u16 Mif_decrement (u8 blockAdd, u8 * send_value);

//u16 Mifare0_Read(u8 page, u8 * response, u16 *responseLength);
//u16 Mifare0_Write(u8 page, u8 * send_value, u16 send_length);
//u16 Mifare0_Compatibility_Write(u8 page, u8 * send_value, u16 send_length);

