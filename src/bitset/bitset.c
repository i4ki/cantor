/**
 * Only experiments with fixed-size bitmap vectors.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cantor/cantor.h>

#define BIT(x,n) (((x) >> (n)) & 1)


Error bitset_init(BitSet *bset, u32 maxNumber)
{
	if(NULL == bset) {
		return ERRNILP;;
	}

	bset->size = maxNumber/8;
	bset->bits = (u8 *)malloc(bset->size);
	bset->start = 0;
	bset->end = maxNumber-1;

	if(NULL == bset->bits) {
		return ERRALLOC;
	}

	bitset_clear(bset);

	return ERROK;
}

Error bitset_initrange(BitSet *bset, u32 start, u32 end)
{
	if(NULL == bset) {
		return ERRNILP;
	}

	if(end < start) {
		return ERRBOUNDARIES;
	}

	bset->size = (end-start)/8;
	bset->bits = (u8 *) malloc(bset->size);

	if(bset->bits == NULL) {
		return ERRALLOC;
	}

	bset->start = start;
	bset->end = end;

	bitset_clear(bset);

	return ERROK;
}

void bitset_free(BitSet *bset)
{
	bset->size = 0;
	bset->start = 0;
	bset->end = 0;
	free(bset->bits);
}

void bitset_clear(BitSet *b)
{
	u32 i = 0;

	for(i = 0; i < b->size; i++) {
		b->bits[i] = 0;
	}
}

void bitset_dbg(BitSet *b)
{
	u32 i = 0, j = 0;

	for(i = 0; i < b->size; i++) {
		for(j = 0; j < 8; j++) {
			printf("%d", BIT(b->bits[i], j));
		}
	}

	puts("");
}

// O(end-start)
Error bitset_rangedbg(BitSet *b, u32 start, u32 end)
{
	u32 	i = 0, 
		j = 0, 
		elemstart = start/8,
		elemend = end/8,
		firstoffstart = start%8,
		offsetend = end%8;

	if(end < start ||
		start < b->start || 
		start >= b->end || 
		end > b->end ||
		end < b->start) {
		return ERRBOUNDARIES;
	}

	// run the first byte isolated for the case when 'start' is in the middle of the first byte
	for(j = firstoffstart; j < (end - start >= 8) ? 8 : offsetend; j++) {
		printf("%d", BIT(b->bits[elemstart], j));
	}

	// run all of inner loops
	for(i = elemstart + 1; i < elemend; i++) { /* do not run the first neither last iteration */
		for(j = 0; j < 8; j++) {
			printf("%d", BIT(b->bits[i], j));
		}
	}

	// run the last case for when 'end' is in the middle of last byte
	for(j = 0; j < offsetend; j++) {
		printf("%d", BIT(b->bits[elemend], j));
	}

	return ERROK;
}

Error bitset_cprange(BitSet *from, BitSet *to, u32 start, u32 end)
{
	Error err;
	u32 	i = 0, 
		j = 0, 
		elemstart = start/8,
		elemend = end/8,
		firstoffstart = start%8,
		offsetend = end%8;

	if(start > from->end || end > from->end || end < start) {
		return ERRBOUNDARIES;
	}

	err = bitset_initrange(to, start, end);

	if(err != ERROK) {
		return err;
	}

	// run the first byte isolated for the case when 'start' is in the middle of the first byte
	for(j = firstoffstart; j < (end - start >= 8) ? 8 : offsetend; j++) {
		to->bits[0] |= from->bits[elemstart] & (1 << j);
	}

	// run all of inner loops
	for(i = elemstart + 1; i < elemend; i++) { /* do not run the first neither last iteration */
		for(j = 0; j < 8; j++) {
			to->bits[i] |= from->bits[i] & (1<<j);
		}
	}

	// run the last case for when 'end' is in the middle of last byte
	for(j = 0; j < offsetend; j++) {
		to->bits[elemend] = from->bits[elemend] & (1<<j);
	}

	return ERROK;
}

Error bitset_set(BitSet *b, u32 pos)
{
	u32 element = 0, offset = 0;

	if(pos >= b->size*8) {
		return ERRBOUNDARIES;
	}

	element = pos/8;
	offset = pos % 8;

	b->bits[element] |= (1 << offset);

	return ERROK;
}

Error bitset_unset(BitSet *b, u32 pos) 
{
	u32 element = 0, offset = 0;

	if(pos >= b->size*8) {
		return ERRBOUNDARIES;
	}

	element = pos/8;
	offset = pos % 8;

	u8 old = b->bits[element];
	u8 cleanshift = 8 - offset;

	// OR of left part and right part skiping the 'pos'
	b->bits[element] = (old >> cleanshift) << cleanshift | (old << offset + 1) >> offset + 1;

	return ERROK;
}


u8 bitset_get(BitSet *b, u32 pos) 
{
	u32 element = 0, offset = 0;

	if(pos >= b->size*8) {
		return ERRBOUNDARIES;
	}

	element = pos/8;
	offset = pos%8;

	return BIT(b->bits[element], offset);
}

u32 bitset_sizeof(BitSet *b)
{
	return b->size * sizeof(u8) + sizeof(BitSet);
}
