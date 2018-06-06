#include "libmftypes.h"

#if defined(SDCC)

__reentrantb int32_t signextend24(int32_t x) __reentrant __naked
{
	x;
	__asm;
	mov	a,b
	rlc	a
	subb	a,acc
	ret
	__endasm;
}

#else

__reentrantb int32_t signextend24(int32_t x) __reentrant
{
	x &= 0xFFFFFF;
	x |= -(x & 0x800000);
	return x;
}
	
#endif
