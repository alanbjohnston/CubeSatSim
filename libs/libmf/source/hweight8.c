#include "libmftypes.h"

#if defined(SDCC)

__reentrantb uint8_t hweight8(uint8_t x) __reentrant __naked
{
	x;
	__asm;
	mov	a,dpl
	anl	a,#0x55
	xch	a,dpl
	rr	a
	anl	a,#0x55
	add	a,dpl
	mov	dpl,a
	anl	a,#0x33
	xch	a,dpl
	rr	a
	rr	a
	anl	a,#0x33
	add	a,dpl
	mov	dpl,a
	anl	a,#0x0f
	xch	a,dpl
	swap	a
	anl	a,#0x0f
	add	a,dpl
	mov	dpl,a	
	ret
	__endasm;
}

#else

__reentrantb uint8_t hweight8(uint8_t x) __reentrant
{
	x = (x & 0x55) + ((x >> 1) & 0x55);
	x = (x & 0x33) + ((x >> 2) & 0x33);
	x = (x & 0x0F) + ((x >> 4) & 0x0F);
	return x;
}
	
#endif
