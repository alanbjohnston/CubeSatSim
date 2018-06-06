#include "libmftypes.h"

#if defined(SDCC)

__reentrantb int32_t signedlimit32(int32_t x, int32_t lim) __reentrant __naked
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
	jnb	acc.7,00000$
	dec	r0
	dec	r0
	dec	r0
	clr	c
	clr	a
	subb	a,@r0
	inc	r0
	mov	dpl,a
	clr	a
	subb	a,@r0
	inc	r0
	mov	dph,a
	clr	a
	subb	a,@r0
	inc	r0
	mov	b,a
	clr	a
	subb	a,@r0
	sjmp	00001$
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
	jb	acc.7,00000$
	dec	r0
	dec	r0
	dec	r0
	mov	a,@r0
	inc	r0
	mov	dpl,a
	mov	a,@r0
	inc	r0
	mov	dph,a
	mov	a,@r0
	inc	r0
	mov	b,a
	mov	a,@r0
	sjmp	00001$
00000$:	mov	a,r1
00001$:	pop	ar1
	pop	ar0
	ret
	__endasm;
}

#else

#include <stdlib.h>

__reentrantb int32_t signedlimit32(int32_t x, int32_t lim) __reentrant
{
	if (x < 0) {
		int32_t xx = x + lim;
		if (xx >= 0)
			return x;
		return -lim;
	}
	{
		int32_t xx = x - lim;
		if (xx <= 0)
			return x;
		return lim;
	}
}

#endif
