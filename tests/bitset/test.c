#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cantor/cantor.h>

typedef struct {
	u8* name;
	u8 (*fn)(void);
} TestFn;

TestFn tests[2]; 

u8 testBasicBitSetOneByte()
{
	BitSet bset;
	u32 i = 0;
	u8 bit = 0;
	Error err = ERROK;

	err = bitsetinit(&bset, 8);

	if(err != ERROK) {
		printf("fail: bitset\n");
		return err;
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
	return err;
}

u8 testLargeBitSet()
{
	Error err = ERROK;
	u32 n = 10000000, i = 0;

	BitSet bset;

	err = bitsetinit(&bset, n);

	if(err != ERROK) {
		return err;
	}

	/* tests if all bits are zero'ed */
	for(i = 0; i < n; i++) {
		u8 v = bitsetget(&bset, i);

		if(v != 0) {
			err = 1;
			goto cleanup;
		}
	}

	/* test 1 bit flip */
	bitsetset(&bset, 0);
	u8 v = bitsetget(&bset, 0);

	if(v != 1) {
		err = 1;
		goto cleanup;
	}

cleanup:	
	bitsetfree(&bset);
	return err;
}

void main(void)
{
	u8 i = 0, status = 0;
	TestFn t1, t2;

	t1.name = (u8 *) "testBasicBitSetOneByte";
	t1.fn = testBasicBitSetOneByte;

	t2.name = (u8 *) "testLargeBitSet";
	t2.fn = testLargeBitSet;

	tests[0] = t1;
	tests[1] = t2;

	for(i = 0; i < sizeof(tests)/sizeof(TestFn); i++)
	{
		TestFn t = tests[i];

		printf("Running: %s: ", t.name);
		u8 ret = t.fn();

		if(ret != 0)
		{
			printf("fail\n");
			status = ret;
		} else {
			printf("ok\n");
		}
	}

	exit(status);
}