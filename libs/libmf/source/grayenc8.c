#include "libmftypes.h"

#if defined(SDCC)

__reentrantb uint8_t gray_encode8(uint8_t x) __reentrant
{
	__asm
	mov	a,dpl
	clr	c
	rrc	a
	xrl	dpl,a
	ret
	__endasm;
}

#else

__reentrantb uint8_t gray_encode8(uint8_t x) __reentrant
{
	x ^= x >> 1;
	return x;
}

#endif
