
/*
 * Philips/NXP Mifare Crypto-1 implementation v1.0
*/
#include "mifare_uint64emu.h"
#include "types.h"

//#define ODD_PARITY(i) (( (i) ^ (i)>>1 ^ (i)>>2 ^ (i)>>3 ^ (i)>>4 ^ (i)>>5 ^ (i)>>6 ^ (i)>>7 ^ 1) & 0x01)

struct _crypto1_state;

// ToDo: integrate reauthentication step 1 into the crypto1 framework
void crypto1_clean_mutual_1_2(struct _crypto1_state * const state, const u32 uid, const u32 card_challange);

struct _crypto1_ops {
  void(*init)(struct _crypto1_state *state, const uint64emu_t key);
  void(*mutual_1)(struct _crypto1_state *state, const u32 uid, const u32 card_challenge);
  void(*mutual_2_reader)(struct _crypto1_state *state, u16 *reader_response);
  long (*mutual_3_reader)(struct _crypto1_state *state, const u16 *card_response);
  long (*mutual_2_card)(struct _crypto1_state *state, const u16 *reader_response);
  void(*mutual_3_card)(struct _crypto1_state *state, u16 *card_response);
  void(*transcrypt_bits)(struct _crypto1_state *state, u16 *data_m, unsigned long bytes, unsigned long bits);
};

typedef struct _crypto1_state {
  uint64emu_storage_t lfsr[2]; /* The 48 bit LFSR for the main cipher state and keystream generation */
  u16 prng; /* The 16 bit LFSR for the card PRNG state, also used during authentication. */
  
  u8 is_card; /* Boolean whether this instance should perform authentication in card mode. */
  
  const struct _crypto1_ops *ops;
} crypto1_state;

enum crypto1_cipher_implementation {
  CRYPTO1_IMPLEMENTATION_CLEAN,
  CRYPTO1_IMPLEMENTATION_OPTIMIZED
};

enum crypto1_cipher_type {
  CRYPTO1_CIPHER_READER = 0,
  CRYPTO1_CIPHER_CARD = 1
};

long  crypto1_new(crypto1_state *state, enum crypto1_cipher_type, enum crypto1_cipher_implementation implementation);
void crypto1_init(crypto1_state *state, uint64emu_t key);
void crypto1_mutual_1(crypto1_state *state, u32 uid, u32 card_challenge);
long  crypto1_mutual_2(crypto1_state *state, u16 *reader_response);
long  crypto1_mutual_3(crypto1_state *state, u16 *card_response);
void crypto1_transcrypt(crypto1_state *state, u16 *data_m, unsigned long length);
void crypto1_transcrypt_bits(crypto1_state *state, u16 *data_m, unsigned long bytes, unsigned long bits);

long _crypto1_new_opt(crypto1_state *state);
long _crypto1_new_clean(crypto1_state *state);

/* Reverse the bit order in the 8 bit value x */
#define rev8(x)			((((x)>>7)&1)^((((x)>>6)&1)<<1)^((((x)>>5)&1)<<2)^((((x)>>4)&1)<<3)^((((x)>>3)&1)<<4)^((((x)>>2)&1)<<5)^((((x)>>1)&1)<<6)^(((x)&1)<<7))
/* Reverse the bit order in the 16 bit value x */
#define rev16(x)		(rev8 (x)^(rev8 (x>> 8)<< 8))
/* Reverse the bit order in the 32 bit value x */
#define rev32(x)		(rev16(x)^(rev16(x>>16)<<16))
/* Return the nth bit from x */
#define bit(x,n)		(((x)>>(n))&1)

/**
 * Convert 4 array entries (a[0], a[1], a[2] and a[3]) into a 32 bit integer, where 
 * a[0] is the MSByte and a[3] is the LSByte
 */
#define ARRAY_TO_UINT32(a) ( ((u32)((a)[0]&0xff) << 24) | ((u32)((a)[1]&0xff) << 16) | \
	((u32)((a)[2]&0xff) << 8) | ((u32)((a)[3]&0xff) << 0) )

/* Copy an u32 i into 4 array entries (a[0], a[1], a[2] and a[3]), where a[0] is the MSByte */
#define UINT32_TO_ARRAY(i, a) do{ \
	(a)[0] = ((i)>>24) & 0xff; \
	(a)[1] = ((i)>>16) & 0xff; \
	(a)[2] = ((i)>> 8) & 0xff; \
	(a)[3] = ((i)>> 0) & 0xff; \
}while(0);



