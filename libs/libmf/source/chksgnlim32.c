#include "libmftypes.h"

#if defined(SDCC)

__reentrantb uint8_t checksignedlimit32(int32_t x, int32_t lim) __reentrant __naked
{
	__asm
	ar2=0x02
	ar3=0x03
	ar4=0x04
	ar5=0x05
	ar6=0x06
	ar7=0x07
	ar0=0x00
	ar1=0x01

	push	ar0
	push	ar1
	mov	r1,a
	mov	a,sp
	add	a,#-7
	mov	r0,a
	mov	a,r1
	jnb	acc.7,00010$
	mov	a,dpl
	add	a,@r0
	inc	r0
	mov	a,dph
	addc	a,@r0
	inc	r0
	mov	a,b
	addc	a,@r0
	inc	r0
	mov	a,r1
	addc	a,@r0
	xrl	a,#0x80
	sjmp	00011$
00010$:	setb	c
	mov	a,dpl
	subb	a,@r0
	inc	r0
	mov	a,dph
	subb	a,@r0
	inc	r0
	mov	a,b
	subb	a,@r0
	inc	r0
	mov	a,r1
	subb	a,@r0
00011$:	rlc	a
	clr	a
	rlc	a
	mov	dpl,a
	pop	ar1
	pop	ar0
	ret
	__endasm;
}

#else

#include <stdlib.h>

__reentrantb uint8_t checksignedlimit32(int32_t x, int32_t lim) __reentrant
{
	if (x < 0) {
		x += lim;
		return x >= 0;
	}
	x -= lim;
	return x <= 0;
}

#endif
