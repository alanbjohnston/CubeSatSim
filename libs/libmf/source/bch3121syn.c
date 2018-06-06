#include "ax8052.h"
#include "libmfbch.h"

uint16_t bch3121_syndrome(uint32_t cw)
{
	uint8_t __autodata cnt = 21;
	do {
		if (cw & 0x80000000)
			cw ^= 0xED200000;
		cw <<= 1;
	} while (--cnt);
	return cw >> 22;
}
