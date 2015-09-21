#ifndef __CANTOR_H__
#define __CANTOR_H__

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef u8 Error;

enum {
	ERROK = 0,
	ERRALLOC,
	ERRNILP,
	ERRBOUNDARIES
};

typedef struct {
	u8 *bits; 
	u32 size;
	u32 start; /* virtual start */
	u32 end;  /* virtual end */
} BitSet;

/**
 * bitsetinit initializes the BitSet structure 
 */
Error bitset_init(BitSet *, u32);
Error bitset_initrange(BitSet *, u32, u32);
Error bitset_cprange(BitSet *from, BitSet *to, u32 start, u32 end);
void bitset_free(BitSet *);
void bitset_clear(BitSet *);
void bitset_dbg(BitSet *);
Error bitset_rangedbg(BitSet *, u32, u32);
Error bitset_set(BitSet *, u32);
Error bitset_unset(BitSet *, u32);
u8 bitset_get(BitSet *, u32);
u32 bitset_sizeof(BitSet *);

#endif