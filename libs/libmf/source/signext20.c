#include "libmftypes.h"

#if defined(SDCC)

__reentrantb int32_t signextend20(int32_t x) __reentrant __naked
{
	x;
	__asm;
	mov	a,b
	anl	a,#0x0F
	mov	b,a
	anl	a,#0x08
	cpl	a
	inc	a
	orl	a,b
	mov	b,a
	rlc	a
	subb	a,acc
	ret
	__endasm;
}

#else

__reentrantb int32_t signextend20(int32_t x) __reentrant
{
	x &= 0xFFFFF;
	x |= -(x & 0x80000);
	return x;
}
	
#endif
