#include "ax8052.h"
#include "libmfbch.h"

uint32_t bch3121_decode(uint32_t cw)
{
	uint16_t __autodata x = bch3121_syndrome(cw);
	uint8_t __autodata p;
	x = bch3121_syndrometable[x];
	// check for decode failure
	if (((int16_t)x) < 0)
		return cw | 1UL;
	p = x & 0x1F;
	cw ^= (1UL << p);
	x >>= 5;
	p = x & 0x1F;
	cw ^= (1UL << p);
	return cw & ~1UL;
}
