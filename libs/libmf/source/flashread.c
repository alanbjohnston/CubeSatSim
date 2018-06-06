#include "libmfflash.h"

uint16_t flash_read(uint16_t raddr)
{
	const uint16_t __code *p = (const uint16_t __code *)(raddr & ~1U);
	return *p;
}
