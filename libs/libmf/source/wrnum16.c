#include "wrnum.h"

static const uint16_t __code subtbl[4*4] = {
	-80, -40, -20, -10,
	-800, -400, -200, -100,
	-8000, -4000, -2000, -1000,
	0, -40000, -20000, -10000
};

#if defined(SDCC)

__reentrantb uint16_t libmf_num16_digit(uint16_t val, uint8_t __auto *dp) __reentrant __naked
{
	__asm;
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	push	ar3
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	mov	a,sp
	add	a,#-7
	mov	r0,a
	mov	a,@r0
	mov	r0,a
	mov	a,@r0
	add	a,#-6
	jnc	00001$
	clr	a
	mov	@r0,a
	sjmp	00000$
00001$:	add	a,#4
	jc	00002$
	mov	a,dpl
	mov	@r0,a
	mov	dptr,#0
	sjmp	00000$
00002$:	swap	a
	rr	a
	mov	r7,dpl
	mov	r6,dph
	mov	dptr,#_subtbl
	add	a,dpl
	mov	dpl,a
	clr	a
	addc	a,dph
	mov	dph,a
	clr	a
	mov	@r0,a
	mov	a,#8
00010$:	mov	r1,a
	clr	a
	movc	a,@a+dptr
	inc	dptr
	add	a,r7
	mov	r3,a
	clr	a
	movc	a,@a+dptr
	inc	dptr
	addc	a,r6
	jnc	00011$
	mov	r6,a
	mov	a,r3
	mov	r7,a
	mov	a,@r0
	orl	a,r1
	mov	@r0,a
00011$:	mov	a,r1
	clr	c
	rrc	a
	jnz	00010$
	mov	dpl,r7
	mov	dph,r6
00000$:	pop	ar1
	pop	ar0
	pop	ar7
	pop	ar6
	pop	ar3
	ret
	__endasm;
}

#else

__reentrantb uint16_t libmf_num16_digit(uint16_t val, uint8_t __auto *dp) __reentrant
{
	const uint16_t __code *s;
	uint8_t cnt, r;
	{
		uint8_t i = *dp;
		if (i > 5) {
			*dp = 0;
			return val;
		}
		i -= 2;
		if (i >= 4) {
			*dp = val;
			return 0;
		}
		s = &subtbl[4*i];
	}
	cnt = 8;
	r = 0;
	do {
		uint16_t rv = val + *s++;
		if (rv < val) {
			val = rv;
			r |= cnt;
		}
		cnt >>= 1;
	} while (cnt);
	*dp = r;
	return val;
}

#endif
