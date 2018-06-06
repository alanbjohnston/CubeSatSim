#include "ax8052.h"
#include "libmftypes.h"

#if defined SDCC

/**
 * \brief fast memset
 *
 */
__reentrantb void fmemset(void __generic *p, char c, uint16_t n) __reentrant __naked
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

	mov	r0,sp
	dec	r0
	dec	r0
	mov	ar5,@r0
	dec	r0
	mov	ar7,@r0
	dec	r0
	mov	a,@r0
	mov	r6,a
	jnz	00001$
	orl	a,r7
	jnz	00000$
00002$:	ret
00001$:	inc	r7
00000$:	;; decide memory space
	mov	a,r5
	jb	_B_7,00002$		; >0x80 code
	jnb	_B_6,xdataloop$		; <0x40 far
	mov	r0,dpl
	jb	_B_5,pdataloop$		; >0x60 pdata
idataloop$:
	mov	@r0,a
	inc	r0
	djnz	r6,idataloop$
	djnz	r7,idataloop$
	ret
pdataloop$:
	movx	@r0,a
	inc	r0
	djnz	r6,pdataloop$
	djnz	r7,pdataloop$
	ret
xdataloop$:
	movx	@dptr,a
	inc	dptr
	djnz	r6,xdataloop$
	djnz	r7,xdataloop$
	ret
	__endasm;
}

#elif defined __ICC8051__

__reentrantb void fmemset(void __generic *p, char c, uint16_t n) __reentrant
{
	union ptr {
		void __generic *p;
		uint8_t __data *pd;
		uint8_t __xdata *px;
		uint8_t b[3];
	};
	union ptr pp;
	uint8_t pa;
	uint8_t n0 = n, n1 = n >> 8;
	if (n0)
		++n1;
	else if (!n1)
		return;
	pp.p = p;
	pa = pp.b[2];
	if (pa & 0x80)
		return;
	if (pa & 0x01) {
		uint8_t __data *p1 = pp.pd;
		do {
			do {
				*p1++ = c;
			} while (--n0);
		} while (--n1);
	} else {
		uint8_t __xdata *p1 = pp.px;
		do {
			do {
				*p1++ = c;
			} while (--n0);
		} while (--n1);
	}
}

#else

__reentrantb void fmemset(void __generic *p, char c, uint16_t n) __reentrant
{
	char __generic *pp = (char __generic *)p;
	for (; n; --n)
		*pp++ = c;
}

#endif
