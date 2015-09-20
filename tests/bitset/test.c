#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cantor/cantor.h>

void main(void)
{
	BitSet bset;
	u32 i = 0;
	Error err;

	err = bitsetinit(&bset, 8);

	if(err != ERROK) {
		printf("bitsetinit: failed\n");
		return;
	}

	printf("bitset.size=%d\n", bset.size);

	bitsetdbg(&bset);

	puts("");

	for(i = 0; i < 8; i++) {
		bitsetset(&bset, i);
	}

	bitsetdbg(&bset);

	bitsetfree(&bset);
}
