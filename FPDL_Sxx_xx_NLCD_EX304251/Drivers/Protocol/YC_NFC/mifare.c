
#include <string.h> 
#include "mifare.h"
#include "mifare_crypto1.h"
#include "yc_nfc_contactless_l1.h"
#include "Project.h"

#ifdef Function_NFCUsed_YC5018
static u16 transceive_buffer[MIFARE_TRANSMIT_BUFFER_SIZE];
static crypto1_state mifareCipherState;

static u8 mifareCipherActive;
static u16 crcUpdateCcitt(u16 crc, u8 dat);
void calculateParity(u16 *data_m, u32 length);

u16 mifareCalculateIso14443aCrc(const u8 *buffer, u16 length);

unsigned char  ODD_PARITY(unsigned char i)
{
	return 	 (( (i) ^ (i)>>1 ^ (i)>>2 ^ (i)>>3 ^ (i)>>4 ^ (i)>>5 ^ (i)>>6 ^ (i)>>7 ^ 1) & 0x01);
}


uint8_t getbit(uint16_t *source,uint8_t n)
{
    return source[n/9] & (1 << (n % 9)) ? 1:0;
}

u8 u16tobit(u16 *source, u8 *des, u8 snum)
{
    uint8_t bitnum;
    uint8_t i;
    if (snum > 28)
    {
        return 1;
    }
    bitnum = snum * 8 + snum;
    for (i = 0; i < (bitnum / 8 + 1); i++)
    {
        *des = (getbit(source,i * 8)) | (getbit(source,i * 8 + 1) << 1) | (getbit(source,i * 8 + 2) << 2) | (getbit(source,i * 8 + 3) << 3) \
               | (getbit(source,i * 8 + 4) << 4) | (getbit(source,i * 8 + 5) << 5) | (getbit(source,i * 8 + 6) << 6) | (getbit(source,i * 8 + 7) << 7);

        des++;
    }
    return 0;
}

u8 getbit1(u8 *source,u8 n)
{
    return source[n/8] & (1 << (n % 8)) ? 1:0;
}
u8 bittou16(u8 *source, u16 *des, u8 snum)
{
    u8 i, j;
    u16 check;

    if (snum > 32)
    {
        return 1;
    }

    i = snum * 8 / 9;

    for (j = 0; j < i; j++)
    {
        *des = getbit1(source,j * 9) | (getbit1(source,j * 9 + 1) << 1) | (getbit1(source,j * 9 + 2) << 2) | (getbit1(source,j * 9 + 3) << 3) | (getbit1(source,j * 9 + 4) << 4)\
               | (getbit1(source,j * 9 + 5) << 5) | (getbit1(source,j * 9 + 6) << 6) | (getbit1(source,j * 9 + 7) << 7);

        check = (getbit1(source,j * 9 + 8) << 8);
        *des += check;
        des++;
    }
    return 0;
}

void mifareSetKey(const u8 *key)
{
   s16 i;
   uint64emu_storage_t   corrected_key[2] = {0};
// uint64emu_storage_t corrected_key;

    if (!crypto1_new(&mifareCipherState, CRYPTO1_CIPHER_READER, CRYPTO1_IMPLEMENTATION_CLEAN))
    {
// MIFARE_DEBUG("Initialization failed \n");
    }

    for (i = 0; i < 6; i++)
    {
        uint64emu_setbyte(corrected_key, 5 - i, key[i]);
    }

    crypto1_init(&mifareCipherState, corrected_key);
// MIFARE_DEBUG("Initialization ok\n");
}


void mifareResetCipher(void)
{
    mifareCipherActive = FALSE;
}

s8 mifareAuthenticateStep1(u8 keySelect,
        u8 block,
        const u8 *uid,
        u8 uidLength,
        const u8 *key)
{
    s8 err = M1_ERR_NONE;
    u8 authenticationCommand[5];
    u8 authenticationResponse[FSD];
    u16 crc;
    size_t index;
    u16 numReceivedBytes;
    u32 uid_as_u32;
    u32 tag_nonce;
		u16 revbuf[5];

    authenticationCommand[0] = keySelect;
    authenticationCommand[1] = block;//&0xfc;
    /* Append CRC. */
    crc = mifareCalculateIso14443aCrc(authenticationCommand, 2);
    authenticationCommand[2] = crc & 0xFF;
    authenticationCommand[3] = (crc >> 8) & 0xFF;

    /* Convert authenticate_command to parity_data_t. */
    for(index = 0; index < 4; index++)
        transceive_buffer[index] = authenticationCommand[index];

    calculateParity(transceive_buffer, 4);
  /*UID too short. Abort*/
    if(uidLength < 4)
    {
        return M1_ERR_PARAM;   //-2
    }

    if(mifareCipherActive)
	{
        crypto1_transcrypt(&mifareCipherState, transceive_buffer, 4);
	}
	
	u16tobit(transceive_buffer,authenticationCommand,4);
	err = NFC_Transceive(CMD_MAUTH1, authenticationCommand, 4*9, 0, authenticationResponse, &numReceivedBytes, 0);

    if(numReceivedBytes != 5)
    {
        mifareResetCipher();
        return M1_ERR_NOTFOUND;   //-3
    }
	
    uid_as_u32 = ARRAY_TO_UINT32(uid);
	bittou16(authenticationResponse,revbuf,5);
    tag_nonce = ARRAY_TO_UINT32(revbuf);
    if(mifareCipherActive)
    {
        mifareResetCipher();
        mifareSetKey(key);
        crypto1_clean_mutual_1_2(&mifareCipherState, uid_as_u32, tag_nonce);
    }
    else
    {
        mifareResetCipher();
        mifareSetKey(key);
        crypto1_mutual_1(&mifareCipherState, uid_as_u32, tag_nonce);
    }

    return err;
}


s8 mifareAuthenticateStep2(u32 readerNonce)
{
    s8 err = M1_ERR_NONE;
    u16 auth2Command[8];
	u8 auth2Command_b[10];
    u8 auth2Response[5];
    u16 numReceivedBytes;

    UINT32_TO_ARRAY_WITH_PARITY( readerNonce, auth2Command);
    crypto1_mutual_2(&mifareCipherState, auth2Command);
	
	u16tobit(auth2Command,auth2Command_b,8);
	
	NFC_Transceive(CMD_MAUTH2, auth2Command_b, 8*9, 0, auth2Response, &numReceivedBytes, 0);

    if (numReceivedBytes != 5)
    {

		mifareResetCipher();  
        return M1_ERR_NOTFOUND;
    }
	
	bittou16(auth2Response,transceive_buffer,5);

    if(!crypto1_mutual_3(&mifareCipherState, transceive_buffer))
    {
		mifareResetCipher(); 
        return M1_ERR_NOMSG;
    }
  
    mifareCipherActive = TRUE;

    return err;
}


s8 mifareSendRequest(const u8 *request, u16 requestLength
    , u8 *response, u16 maxResponseLength, u16 *responseLength, u16 timeout, bool fourBitResponse)
{
    s8 err = M1_ERR_NONE;
    size_t index;
    u16 crc;
	u8 sendbit = 0;
	u8 transceive_buffer_b[32];
	maxResponseLength = 0;
	timeout = 0;
	fourBitResponse = 0;


    /* Copy request into parity_data_t transmit buffer. */
    for(index = 0; index < requestLength; index++)
    {
        transceive_buffer[index] = request[index];
    }

    /* Append CRC. */
    crc = mifareCalculateIso14443aCrc(request, requestLength);
    transceive_buffer[requestLength] = ((u8) (crc & 0xFF));
    transceive_buffer[requestLength+1] = ((u8) ((crc>>8) & 0xFF));

    // Calculate Parity
    calculateParity(&transceive_buffer[0], requestLength+2);

    // Encrypt if cipher is in use
    if(mifareCipherActive)
        crypto1_transcrypt(&mifareCipherState, transceive_buffer, requestLength+2);
	
	u16tobit(transceive_buffer,transceive_buffer_b,requestLength+2);
	sendbit = (requestLength+2)*9;
	err = NFC_Transceive(CMD_MTRANSFER, transceive_buffer_b, sendbit, 0, response, responseLength, 0);

	if (err != 0)
	{
		return err;
	}
  
    // Decrypt message in transceive buffer if cipher is in use.
    if(mifareCipherActive)
    {
        // If a response with a length of 0 or 1 byte is received it is asumed that this
        // actually was an ACK,NACK and 4 bits are fed into the cipher
        if((*responseLength == 0) || (*responseLength == 1))
        {
			transceive_buffer[0] = response[0];
            crypto1_transcrypt_bits(&mifareCipherState, &transceive_buffer[0], 0, 4);
        }
        else
		{
			bittou16(response,transceive_buffer,*responseLength);
            crypto1_transcrypt_bits(&mifareCipherState, transceive_buffer, (*responseLength *8/9), 0);
			
			*responseLength = *responseLength*8/9;
		}
    }

    /* Copy decrypted message back into response buffer. */
    /* ToDo: Check why not only numReceivedBytes bytes are copied. */
    for(index = 0; index < *responseLength; index++)
        response[index] = (unsigned char) transceive_buffer[index];

    return err;
}

void calculateParity(u16 *data_m, u32 length)
{
    u32 index;

    for (index = 0; index < length; index++)
    {			
        if(ODD_PARITY(data_m[index]))
        {
            data_m[index] |= (u16) 0x0100;
        }
        else
        {
            data_m[index] &= ~((u16) 0x0100);
        }
    }
}


u16 crcCalculateCcitt(u16 preloadValue, const u8* buf, u16 length)
{
    u16 crc = preloadValue;
    u16 index;

    for (index = 0; index < length; index++)
    {
        crc = crcUpdateCcitt(crc, buf[index]);
    }

    return crc;
}



u16 crcUpdateCcitt(u16 crc, u8 dat)
{
    dat ^= ((u8)crc) & 0xFF;
    dat ^= dat << 4;

    crc = (crc >> 8)^(((u16) dat) << 8)^(((u16) dat) << 3)^(((u16) dat) >> 4);

    return crc;
}





u16 mifareCalculateIso14443aCrc(const u8 *buffer, u16 length)
{
    return crcCalculateCcitt(0x6363, buffer, length);
}

u16 Mif_read (u8 blockAddr, u8 * response, u16 *responseLength)
{
	u8 luc_send_data[18]; 
	s8 err  = 0x00; 
	*responseLength = 0x00 ;
	luc_send_data[0] = MIFARE_READ_BLOCK;
	luc_send_data[1] = blockAddr; //blockAddr 
	err = mifareSendRequest(luc_send_data,
							2,
							response,
							20,
							responseLength,
							6000, //timeout (ms)5
							FALSE);
	if((NFC_ERR_TIME_OUT ==err )||(*responseLength !=18))
		 return MIF_ERROR;
 
	else if (err!=M1_ERR_NONE)
		return MIF_ERROR ;
	else
	{ 
		if((*responseLength==0x01)&&(err==M1_ERR_NONE) )
			return MIF_NOTAUTHERR ; 
		
		return MIF_OK ; 
	}
	
}

//u16 Mif_write(u8 blockAddr, u8 * send_value, u16 send_length)
//{
//	s8 err ; 
//	u8 luc_send_data[18]; 
//	u8 luc_p_response[18]={0x00};
//	u16 lus_responseLength = 0x00 ;
//	
//	if(send_length!=16)
//		return MIF_LENGTH_ERR ;

//	luc_send_data[0] = MIFARE_WRITE_BLOCK;
//	luc_send_data[1] = blockAddr;  
//	err = mifareSendRequest(luc_send_data,
//							2,
//							luc_p_response,
//							20,
//							&lus_responseLength,
//							15000, //timeout (5ms)
//							TRUE); //返回4bit
////	if((err == 0)&&(luc_p_response[0]==0x0A))
//	if(err == 0)
//	{
//		err = mifareSendRequest(send_value,
//								send_length,
//								luc_p_response,
//								1,
//								&lus_responseLength ,
//								10000, //timeout (10ms)
//								TRUE);	 //返回4bit
//		if(NFC_ERR_TIME_OUT ==err )
//		{
//			return MIF_TIMEOVER ; 
//		}
//		else if (err!=M1_ERR_NONE)
//		{
//		
//			return MIF_WRITEERR ;	  /*6ff3*/
//		}
//		
//		else 
//			return MIF_OK ; 
//		
//	}

//	else if (err !=0)
//	{
//		return MIF_WRITEERR;
//	}

//	else 
//	{
//		return MIF_ERROR   ;   /*6ff0*/
//	}
//	
//	
//}


//u16 Mif_Transfer (u8 blockAddr)
//{
//	s8 err ; 
//	u8 luc_send_data[30]; 
//	u8 luc_p_response[20]={0x00};
//	u16 lus_responseLength = 0x00 ;	
//	luc_send_data[0] = MIFARE_TRANSFER_BLOCK;
//	luc_send_data[1] = blockAddr; //blockAddr 
//	err = mifareSendRequest(luc_send_data,
//							2,
//							luc_p_response,
//							1,
//							&lus_responseLength,
//							40000, //timeout (ms)5
//							TRUE);//返回4bit
//			
//	if(NFC_ERR_TIME_OUT ==err )
//		return MIF_TIMEOVER ; 
//	
//	
//	else if (err!=M1_ERR_NONE)
//		return MIF_TRANSFER_ERR ;
//		
//	
//	else 
//		return MIF_OK ; 
//	

//}


//u16 Mif_InitPurchase (u8 blockAddr, u8 * send_value,u16 send_length)
//{
//	u8 luc_send_data[30]; 
//	u8 i = 0 ;
//	
//	if(send_length!=4)
//		return MIF_LENGTH_ERR ; 
//	
//	memcpy(luc_send_data, send_value ,4);
//	
//	for(i =0 ; i<4 ; i++)
//	{
//		luc_send_data[4+i] = ~(*(send_value+i));
//	}
//	
//	memcpy(&luc_send_data[8], send_value ,4);	
//	
//	luc_send_data[12] =  blockAddr ;// blockAddr
//	luc_send_data[13] =  ~blockAddr;//~blockAddr
//	luc_send_data[14] =  blockAddr ;  //blockAddr
//	luc_send_data[15] = ~blockAddr ;	// ~blockAddr
//	
//	return  Mif_write (blockAddr, luc_send_data, 16);
//}



//u16 Mif_Restore (u8 blockAddr,u8 *send_value)
//{
//	s8 err ; 
//	u8 luc_send_data[30]; 
//	u8 luc_p_response[20]={0x00};
//	u16 lus_responseLength = 0x00 ;
//	
//	luc_send_data[0] = MIFARE_RESTORE_BLOCK;
//	luc_send_data[1] = blockAddr; //blockAddr 
//	err = mifareSendRequest(luc_send_data,
//							2,
//							luc_p_response,
//							20,
//							&lus_responseLength,
//							10000, //timeout (ms)5
//							TRUE);//返回4bit
//  // 	if((err == 0)&&(luc_p_response[0]==0x0A))
//	if(err == 0)																	
//	{
//		err = mifareSendRequest(send_value,
//								4,
//								luc_p_response,
//								0,
//								&lus_responseLength,
//								5000, //timeout (ms)5
//								FALSE);	 	
//								
//		if(M1_ERR_NONE ==err ) /*没有出现和接收相关的任何中断*/
//			return MIF_OK ;
//		else
//		  return MIF_VALERR ;
//	}
//	else 
//	{
//		return MIF_VALERR ;
//	}
//	
//	//return MIF_OK ; 
//}


//u16 Mif_increment (u8 blockAddr, u8 *send_value)
//{
//	s8 err ; 
//	u8 luc_send_data[30]; 
//	u8 luc_p_response[20]={0x00};
//	u16 lus_responseLength = 0x00 ;
//	
//	luc_send_data[0] = MIFARE_INCREMENT_BLOCK;
//	luc_send_data[1] = blockAddr; //blockAddr 
//	err = mifareSendRequest(luc_send_data,
//							2,
//							luc_p_response,
//							20,
//							&lus_responseLength,
//							15000, //timeout (ms)5
//							TRUE);//返回4bit
//	if(err == M1_ERR_NONE) 
//	{
//		err = mifareSendRequest(send_value,
//								4,
//								luc_p_response,
//								0,
//								&lus_responseLength,
//								5000, //timeout (ms)5
//								FALSE);

//		if(M1_ERR_NONE !=err )
//			return MIF_OK ;
//		else
//		  	return MIF_VALERR ;
//	}
//	else 
//	{
//		return MIF_VALERR ;
//	}

//}



//u16 Mif_decrement (u8 blockAddr, u8 * send_value)
//{
//	s8 err ; 
//	u8 luc_send_data[30]; 
//	u8 luc_p_response[20]={0x00};
//	u16 lus_responseLength = 0x00 ;
//	
//	luc_send_data[0] = MIFARE_DECREMENT_BLOCK;
//	luc_send_data[1] = blockAddr; //blockAddr 
//	err = mifareSendRequest(luc_send_data,
//							2,
//							luc_p_response,
//							20,
//							&lus_responseLength,
//							10000, //timeout (ms)5
//							TRUE);//teturn 4bit
//	if(err == M1_ERR_NONE)
//	{
//		err = mifareSendRequest(send_value,
//								4,
//								luc_p_response,
//								20,
//								&lus_responseLength,
//								10000, //timeout (ms)5
//								FALSE);
//		if(M1_ERR_NONE !=err ) /*not????*/
//			return MIF_OK ;
//		else
//		  	return MIF_VALERR ;
//	}
//	else 
//	{
//		return MIF_VALERR ;
//	}
//}

u8 Get_Mifare_uid(u8 *uid)
{
	* uid = picc_a.UID_CLn[0][0];
	*(uid+1) = picc_a.UID_CLn[0][1];
	*(uid+2) = picc_a.UID_CLn[0][2];
	*(uid+3) = picc_a.UID_CLn[0][3];
	
	mifareResetCipher();
	
	return NFC_ERR_NONE;
}

u16 Mifare_Auth_Fun(u8 *keydata, u8 key_index, u8 *uid, u8 block)
{
	
	u8 apdu_data[2];
	s8 err;	
	if (key_index == KEYA)
	{
		apdu_data[0] = 0x60; 
	}
	else
	{
		apdu_data[0] = 0x61; 
	}
	apdu_data[1] = block;
  err = mifareAuthenticateStep1(apdu_data[0], block, uid, 4, keydata);
	if(err !=M1_ERR_NONE)
	{
	
		return MIF_AUTH_ERR ;
		
	}
  err = mifareAuthenticateStep2(MIFARE_DEFAULT_READER_NONCE);
	if(err != M1_ERR_NONE)
	{
		
		return MIF_AUTH_ERR;
		
	}
	return M1_ERR_NONE;
}

//u16 Mifare0_Read(u8 page, u8 * response, u16 *responseLength)
//{
//	u8 luc_send_data[30]; 
//	u16 send_len = 0;
//	u8 err  = 0x00; 
//	*responseLength = 0x00 ;
//	 
//	luc_send_data[0] = MIFARE_READ_BLOCK;
//	luc_send_data[1] = page; 
//	send_len = 2;
//	err = NFC_Transceive(CMD_M0READ, luc_send_data, send_len, 0, response, responseLength, 0);
//	if(*responseLength == 1) //recv NAK
//	 {
//	  return MIF_ERROR;
//	 }
//	 return err;
//}

//u16 Mifare0_Write(u8 page, u8 * send_value, u16 send_length)
//{
//	u8 luc_send_data[30]; 
//	u16 send_len = 0;
//	u8 err  = 0x00; 
//	u8 i;
//	u8 response[2];
//	u16 responseLength;
//	
//	luc_send_data[0] = 0xa2;
//	luc_send_data[1] = page; 
//	for (i=0;i<send_length;i++)
//	{
//		luc_send_data[2+i] = *send_value++;
//	}
//	send_len = 2+send_length;
//	responseLength = 0;
//	err = NFC_Transceive(CMD_M0WRITE, luc_send_data, send_len, 0, response, &responseLength, 0);
//	if((response[0] == 0x0a) && (responseLength == 1) && (err == 0))
//	{
//			return err;
//	}
//	return MIF_ERROR;
//}

//u16 Mifare0_Compatibility_Write(u8 page, u8 * send_value, u16 send_length)
//{
//	
//	u8 luc_send_data[4]; 
//	u16 send_len = 0;
//	u8 err  = 0x00; 
//	u8 response[2];
//	u16 responseLength;
//	
//	luc_send_data[0] = 0xa0;
//	luc_send_data[1] = page; 
//	send_len = 2;
//	responseLength = 0;
//	err = NFC_Transceive(CMD_M0WRITE, luc_send_data, send_len, 0, response, &responseLength, 0);
//	if((response[0] == 0x0a) && (responseLength == 1) && (err == 0))
//	{
//		err = NFC_Transceive(CMD_M0WRITE, send_value, send_length, 0, response, &responseLength, 0);
//		if((response[0] == 0x0a) && (responseLength == 1) && (err == 0))
//		{
//			return err;
//		}
//	}
//	return MIF_ERROR;
//	
//}

#endif