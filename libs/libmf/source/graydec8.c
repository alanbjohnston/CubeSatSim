#include "libmftypes.h"

#if defined(SDCC)

__reentrantb uint8_t gray_decode8(uint8_t x) __reentrant
{
	__asm
	mov	a,dpl
	rr	a
	anl	a,#0x40
	xrl	dpl,a
	mov	a,dpl
	rr	a
	anl	a,#0x20
	xrl	dpl,a
	mov	a,dpl
	rr	a
	anl	a,#0x10
	xrl	dpl,a
	mov	a,dpl
	rr	a
	anl	a,#0x08
	xrl	dpl,a
	mov	a,dpl
	rr	a
	anl	a,#0x04
	xrl	dpl,a
	mov	a,dpl
	rr	a
	anl	a,#0x02
	xrl	dpl,a
	mov	a,dpl
	rr	a
	anl	a,#0x01
	xrl	dpl,a
	ret
	__endasm;
}

#else

__reentrantb uint8_t gray_decode8(uint8_t x) __reentrant
{
	x ^= (x >> 1) & 0x40;
	x ^= (x >> 1) & 0x20;
	x ^= (x >> 1) & 0x10;
	x ^= (x >> 1) & 0x08;
	x ^= (x >> 1) & 0x04;
	x ^= (x >> 1) & 0x02;
	x ^= (x >> 1) & 0x01;
	return x;
}

#endif
