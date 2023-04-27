
#ifdef MSPGCC
    // least significant word first
    struct uint64emu {
      int data_m[3];
    };
    typedef struct uint64emu * uint64emu_t;
    typedef struct uint64emu uint64emu_storage_t;
#else

  //  typedef  double   * uint64emu_t;
//	typedef  longlong   uint64emu_storage_t;

    typedef  unsigned long   * uint64emu_t;		  //uint64emu_t
	typedef  unsigned long   uint64emu_storage_t;


#endif

void uint64emu_shl(uint64emu_t x, long n);
void uint64emu_shr(uint64emu_t x, long n);
long uint64emu_bit(const uint64emu_t x, long n);
void uint64emu_orbit(uint64emu_t x, long n, long val);
void uint64emu_assign(uint64emu_t x, unsigned long a, unsigned long b, unsigned long c);
unsigned char uint64emu_byte(const uint64emu_t x, long n);
void uint64emu_setbyte(uint64emu_t x, long n, unsigned char val);
void uint64emu_dump(const char * msg, const uint64emu_t);

