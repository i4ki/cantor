/**
 * Only experiments with fixed-size bitmap vectors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cantor/cantor.h>


Error bitsetinit(BitSet *bset, u32 maxNumber)
{
	if(NULL == bset) {
		return ERRNILP;;
	}

	bset->size = maxNumber/8;
	bset->bits = (u8 *)malloc(maxNumber/8);

	if(NULL == bset->bits) {
		return ERRALLOC;
	}

	bitsetclear(bset);

	return ERROK;
}

void bitsetfree(BitSet *bset)
{
	bset->size = 0;
	free(bset->bits);
}

void bitsetclear(BitSet *b)
{
	u32 i = 0;

	for(i = 0; i < b->size; i++) {
		b->bits[i] = 0;
	}
}

void bitsetdbg(BitSet *b)
{
	u32 i = 0, j = 0;

	for(i = 0; i < b->size; i++) {
		for(j = 0; j < 8; j++) {
			if(b->bits[i] & (1<<j)) {
				printf("1");
			} else {
				printf("0");
			}
		}
	}

	puts("");
}

Error bitsetset(BitSet *b, u32 pos)
{
	u32 element = 0, offset = 0;

	element = pos/8;
	offset = pos % 8;

	b->bits[element] |= (1 << offset);

	return ERROK;
}

u8 bitsetget(BitSet *b, u32 pos) 
{
	u32 element = 0, offset = 0;

	element = pos/8;
	offset = pos%8;

	return b->bits[element] & (1 << offset);
}
