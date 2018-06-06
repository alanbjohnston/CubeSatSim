#include "libmfcrc.h"

__reentrantb uint16_t pn9_advance(uint16_t pn9) __reentrant
{
	pn9 &= 0x1FF;
        return (pn9 >> 8) ^ (pn9_table[pn9] << 1);
}
