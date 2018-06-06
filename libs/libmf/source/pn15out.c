#include "libmfcrc.h"

__reentrantb uint8_t pn15_output(uint16_t pn15) __reentrant
{
	return pn15_out_table[pn15 & 0xff];
}
