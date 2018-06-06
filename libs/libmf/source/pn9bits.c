#include "libmfcrc.h"

#if defined(SDCC)

__reentrantb uint16_t pn9_advance_bits(uint16_t pn9, uint16_t bits) __reentrant __naked
{
	pn9;
	bits;
	__asm
	mov	a,sp
	add	a,#-3
	mov	r0,a
	mov	a,@r0
	mov	r2,a
	inc	r0
	mov	a,@r0
	mov	r3,a
	orl	a,r2
	jz	00000$
	mov	a,r2
	jz	00003$
	inc	r3
00003$:	mov	a,dph
	rrc	a
	mov	a,dpl
00001$:	rrc	a
	jnb	acc.4,00002$
	cpl	c
00002$:	djnz	r2,00001$
	djnz	r3,00001$
	mov	dpl,a
	clr	a
	rlc	a
	mov	dph,a
00000$:	ret
	__endasm;
}

#else

__reentrantb uint16_t pn9_advance_bits(uint16_t pn9, uint16_t bits) __reentrant
{
	if (!bits)
		return pn9;
	do {
		pn9 = (uint8_t)(pn9 >> 1) | (((pn9 << 3) ^ (pn9 << 8)) & 0x100);
	} while (--bits);
	return pn9;
}

#endif
