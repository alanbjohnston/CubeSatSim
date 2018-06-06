#include "libmftypes.h"

__reentrantb uint8_t rev8(uint8_t x) __reentrant
{
	x = ((x >> 4) & 0x0F) | ((x << 4) & 0xF0);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xCC);
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xAA);
	return x;
}
