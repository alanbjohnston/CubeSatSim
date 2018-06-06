#include "wrnum.h"

#if defined(SDCC)

static const uint32_t __code subtbl[9*4] = {
	-80, -40, -20, -10,
	-800, -400, -200, -100,
	-8000, -4000, -2000, -1000,
	-80000, -40000, -20000, -10000,
	-800000, -400000, -200000, -100000,
	-8000000, -4000000, -2000000, -1000000,
	-80000000, -40000000, -20000000, -10000000,
	-800000000, -400000000, -200000000, -100000000,
	0, -4000000000, -2000000000, -1000000000
};

__reentrantb uint32_t libmf_num32_digit(uint32_t val, uint8_t __auto *dp) __reentrant __naked
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
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	push	ar0
	push	ar1
	mov	r4,a
	mov	a,sp
	add	a,#-10
	mov	r0,a
	mov	a,@r0
	mov	r0,a
	mov	a,@r0
	add	a,#-11
	jnc	00001$
	clr	a
	mov	@r0,a
	mov	a,r4
	sjmp	00000$
00001$:	add	a,#9
	jc	00002$
	mov	a,dpl
	mov	@r0,a
	clr	a
	mov	b,a
	mov	dptr,#0
	sjmp	00000$
00002$:	swap	a
	mov	r7,dpl
	mov	r6,dph
	mov	r5,b
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
	mov	r2,a
	clr	a
	movc	a,@a+dptr
	inc	dptr
	addc	a,r5
	mov	b,a
	clr	a
	movc	a,@a+dptr
	inc	dptr
	addc	a,r4
	jnc	00011$
	mov	r4,a
	mov	a,b
	mov	r5,a
	mov	a,r2
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
	mov	b,r5
	mov	a,r4
00000$:	pop	ar1
	pop	ar0
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	ret
	__endasm;
}

#else

static const uint32_t __code subtbl[6*4] = {
	-80000, -40000, -20000, -10000,
	-800000, -400000, -200000, -100000,
	-8000000, -4000000, -2000000, -1000000,
	-80000000, -40000000, -20000000, -10000000,
	-800000000, -400000000, -200000000, -100000000,
	0, -4000000000, -2000000000, -1000000000
};

uint32_t libmf_num32_digit(uint32_t val, uint8_t __auto *dp)
{
	const uint32_t __code *s;
	uint8_t __autodata cnt, r;
	{
		uint8_t __autodata i = *dp;
		if (i > 10) {
			*dp = 0;
			return val;
		}
		i -= 5;
		if (i >= 6)
			return libmf_num16_digit(val, dp);
		s = &subtbl[4*i];
	}
	cnt = 8;
	r = 0;
	do {
		uint32_t __autodata rv = val + *s++;
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
