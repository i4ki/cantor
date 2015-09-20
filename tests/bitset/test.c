#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cantor/cantor.h>

void main(void)
{
	BitSet bset;
	u32 i = 0;
	u8 bit = 0;
	Error err;

	err = bitsetinit(&bset, 8);

	if(err != ERROK) {
		printf("fail: bitset\n");
		return;
	}

	if(bset.size != 1)
	{	
		printf("fail: bitset (wrong size of bitset. Value: %d)\n", bset.size);
		goto cleanup;
	}

	for(i = 0; i < 8; i++) {
		bitsetset(&bset, i);
	}

	for(i = 0; i < 8; i++) {
		bit = bitsetget(&bset, 0);

		if(bit != 1) {
			printf("fail: bitset (bit %d != 1\n", i);
			goto cleanup;
		}
	}

cleanup:
	bitsetfree(&bset);
}
