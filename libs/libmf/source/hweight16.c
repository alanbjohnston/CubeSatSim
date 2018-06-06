#include "libmftypes.h"

#if defined(SDCC)

__reentrantb uint8_t hweight16(uint16_t x) __reentrant __naked
{
	x;
	__asm;
	lcall	_hweight8
	mov	a,dpl
	xch	a,dph
	mov	dpl,a
	lcall	_hweight8
	mov	a,dph
	add	a,dpl
	mov	dpl,a
	ret
	__endasm;
}

#else

__reentrantb uint8_t hweight16(uint16_t x) __reentrant
{
	return hweight8(x) + hweight8(x >> 8);
}
	
#endif
