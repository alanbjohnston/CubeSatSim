#include "ax8052.h"
#include "libmfbch.h"

uint32_t bch3121_decode_parity(uint32_t cw)
{
	uint8_t __autodata p = cw & 1;
	cw = bch3121_decode(cw);
	if (cw & 1)
		return cw;
	cw |= p ^ parity32(cw);
	return cw;
}
