#ifndef __CANTOR_H__
#define __CANTOR_H__

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef u8 Error;

enum {
	ERROK = 0,
	ERRALLOC,
	ERRNILP
};

typedef struct {
	u8 *bits; 
	u32 size;
} BitSet;

/**
 * bitsetinit initializes the BitSet structure 
 */
Error bitsetinit(BitSet *, u32);
void bitsetfree(BitSet *);
void bitsetclear(BitSet *);
void bitsetdbg(BitSet *);
Error bitsetset(BitSet *, u32);
u8 bitsetget(BitSet *, u32);
u32 bitsetsizeof(BitSet *);

#endif