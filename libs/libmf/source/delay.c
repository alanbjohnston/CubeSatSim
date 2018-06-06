#include "libmftypes.h"

#ifdef SDCC

/**
 * \brief delay (busy waiting)
 *
 */
__reentrantb void delay(uint16_t us) __reentrant __naked
{
	us;
	__asm
	mov	a,dpl
	jz	00002$
	inc	dph
00002$:	nop
	nop
00000$:	mov	a,#3
00001$:	djnz	acc,00001$
	djnz	dpl,00002$
	djnz	dph,00000$
	ret
	__endasm;
}

#elif defined __CX51__ || defined __C51__

__reentrantb void delay(uint16_t us) __reentrant
{
#pragma asm
	mov	a,r7		;
	jz	dly2		;
	inc	r6		;
dly2:	nop			;
	nop			;
dly0:	mov	a,#3		;
dly1:	djnz	acc,dly1	;
	djnz	r7,dly2		;
	djnz	r6,dly0		;
#pragma endasm
}

#else

__reentrantb void delay(uint16_t us) __reentrant
{
	uint8_t x;
	do {
		x = 3;
		do {
			--x;
		} while (x);
		--us;
	} while (us);
}

#endif
