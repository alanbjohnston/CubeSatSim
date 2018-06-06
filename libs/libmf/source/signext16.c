#include "libmftypes.h"

#if defined(SDCC)

__reentrantb int32_t signextend16(int16_t x) __reentrant __naked
{
	x;
	__asm;
	mov	a,dph
	rlc	a
	subb	a,acc
	mov	b,a
	ret
	__endasm;
}

#else

__reentrantb int32_t signextend16(int16_t x) __reentrant
{
	return x;
}

	
#endif
