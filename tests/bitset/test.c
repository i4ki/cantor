#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <cantor/cantor.h>

#define TERROR(...) fprintf(stderr, __VA_ARGS__)
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

	err = bitset_init(&bset, 8);

	if(err != ERROK) {
		TERROR("fail: bitset\n");
		return err;
	}

	if(bset.size != 1)
	{	
		TERROR("fail: bitset (wrong size of bitset. Value: %d)\n", bset.size);
		err = 1;
		goto cleanup;
	}

	for(i = 0; i < 8; i++) {
		err = bitset_set(&bset, i);

		if(err != ERROK) {
			goto cleanup;
		}
	}

	for(i = 0; i < 8; i++) {
		bit = bitset_get(&bset, i);

		if(bit != 1) {
			printf("fail: bitset (bit %d != 1\n", bit);
			bitset_dbg(&bset);
			err = 1;
			goto cleanup;
		}
	}

	bitset_clear(&bset);

	/* test 1 bit flip */
	err = bitset_set(&bset, 0);

	if(err != ERROK) {
		TERROR("Failed to set bit\n");
		goto cleanup;
	}

	u8 v = bitset_get(&bset, 0);

	if(v != 1) {
		TERROR("bit flag %d != %d\n", v, 1);
		err = 98;
		goto cleanup;
	}

	/* test bit clear */
	err = bitset_unset(&bset, 0);

	if(err != ERROK) {
		TERROR("bit unset error: %d\n", err);
		goto cleanup;
	}

	v = bitset_get(&bset, 0);

	if(v != 0) {
		TERROR("Failed to clear bit. Still 1\n");
		err = 97;
		goto cleanup;
	}

	/* test if adjacent bit operation mess each other */
	err = bitset_set(&bset, 3);

	if(err != ERROK) {
		TERROR("Failed to set bit 3\n");
		goto cleanup;
	}

	err = bitset_set(&bset, 4);

	if(err != ERROK) {
		TERROR("Failed to set bit 4\n");
		goto cleanup;
	}

	err = bitset_set(&bset, 6);

	if(err != ERROK) {
		TERROR("Failed to set bit 6\n");
		goto cleanup;
	}

	/* clear the middle slot */
	err = bitset_unset(&bset, 5);

	if(err != ERROK) {
		TERROR("Failed to unset the bit 5\n");
		goto cleanup;
	}

	/* test the values */
	u8 v3 = bitset_get(&bset, 3);
	u8 v4 = bitset_get(&bset, 4);
	u8 v5 = bitset_get(&bset, 5);
	u8 v6 = bitset_get(&bset, 6);

	if(v3 != 1 || v4 != 1 || v5 != 0 || v6 != 1) {
		TERROR("Some bit field operations is setting wrong bits\n");
		TERROR("v3=%d, v4=%d, v5=%d, v6=%d\n", v3, v4, v5, v6);
		err = 95;
		goto cleanup;
	}

cleanup:
	bitset_free(&bset);
	return err;
}

u8 testLargeBitSet()
{
	Error err = ERROK;
	u32 n = 10000000, i = 0;

	BitSet bset;

	err = bitset_init(&bset, n);

	if(err != ERROK) {
		TERROR("Failed to init large bitset\n");
		return err;
	}

	/* tests if all bits are zero'ed */
	for(i = 0; i < n; i++) {
		u8 v = bitset_get(&bset, i);

		if(v != 0) {
			err = 99;
			goto cleanup;
		}
	}

	/* test 1 bit flip */
	bitset_set(&bset, 0);
	u8 v = bitset_get(&bset, 0);

	if(v != 1) {
		TERROR("bit flag %d != %d\n", v, 1);
		err = 98;
		goto cleanup;
	}

	/* test out-of-bounds bit flip */
	err = bitset_set(&bset, n+1);

	if(err != ERRBOUNDARIES) {
		fprintf(stderr, "Out of bounds write must fail\n");
		err = 97;
		goto cleanup;
	}

	err = ERROK;

cleanup:	
	bitset_free(&bset);
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
			printf("fail, err code = %ud\n", ret);
			status = ret;
		} else {
			printf("ok\n");
		}
	}

	exit(status);
}