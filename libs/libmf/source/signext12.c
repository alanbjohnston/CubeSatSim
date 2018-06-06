#include "libmftypes.h"

#if defined(SDCC)

__reentrantb int32_t signextend12(int16_t x) __reentrant __naked
{
	x;
	__asm;
	mov	a,dph
	anl	a,#0x0F
	mov	dph,a
	anl	a,#0x08
	cpl	a
	inc	a
	orl	a,dph
	mov	dph,a
	rlc	a
	subb	a,acc
	mov	b,a
	ret
	__endasm;
}

#else

__reentrantb int32_t signextend12(int16_t x) __reentrant
{
	x &= 0xFFF;
	x |= -(x & 0x800);
	return x;
}
	
#endif
