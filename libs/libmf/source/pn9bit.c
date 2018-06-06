#include "libmfcrc.h"

#if defined(SDCC)

__reentrantb uint16_t pn9_advance_bit(uint16_t pn9) __reentrant __naked
{
	pn9;
	__asm
	mov	a,dph
	rrc	a
	mov	a,dpl
	rrc	a
	jnb	acc.4,00000$
	cpl	c
00000$:	mov	dpl,a
	clr	a
	rlc	a
	mov	dph,a
	ret
	__endasm;
}

#else

__reentrantb uint16_t pn9_advance_bit(uint16_t pn9) __reentrant
{
	return (uint8_t)(pn9 >> 1) | (((pn9 << 3) ^ (pn9 << 8)) & 0x100);
}

#endif
