#include "libmfcrc.h"

__reentrantb uint16_t pn15_advance(uint16_t pn15) __reentrant
{
	pn15 &= 0x7FFF;
        return (pn15 >> 8) ^ pn15_adv_table[pn15 & 0xff];
}
