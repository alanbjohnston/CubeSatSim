#include "libmftypes.h"

#if defined(SDCC)

__reentrantb uint8_t hweight32(uint32_t x) __reentrant __naked
{
	x;
	__asm;
	push	acc
	lcall	_hweight16
	mov	dph,a
	pop	dpl
	lcall	_hweight8
	mov	a,dph
	add	a,dpl
	mov	dph,a
	mov	dpl,b
	lcall	_hweight8
	mov	a,dph
	add	a,dpl
	mov	dpl,a
	ret
	__endasm;
}

#else

__reentrantb uint8_t hweight32(uint32_t x) __reentrant
{
	return hweight8(x) + hweight8(x >> 8) + hweight8(x >> 16) + hweight8(x >> 24);
}
	
#endif
