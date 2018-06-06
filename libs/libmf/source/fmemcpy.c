#include "ax8052.h"
#include "libmftypes.h"

#if defined SDCC

/**
 * \brief fast memset
 *
 */
__reentrantb void fmemcpy(void __generic *d, const void __generic *s, uint16_t n) __reentrant __naked
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
	mov	ar3,@r0
	dec	r0
	mov	ar2,@r0
	dec	r0
	mov	ar1,@r0
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
	mov	a,r3
	jb	_B_7,00002$		; >0x80 code
	jnb	_B_6,xdataptr$		; <0x40 far
	mov	r0,dpl
	mov	dpl,r1
	mov	dph,r2
	jnb	_B_5,idataptr$		; >0x60 pdata
	ljmp	pdataptr$
idataptr$:
	jb	acc.7,idatacodeloop$	; >0x80 code
	jnb	acc.6,idataxdataloop$	; <0x40 far
	jb	acc.5,idatapdataloop$	; >0x60 pdata
idataidataloop$:
	mov	a,@r1
	inc	r1
	mov	@r0,a
	inc	r0
	djnz	r6,idataidataloop$
	djnz	r7,idataidataloop$
	ret
idatapdataloop$:
	movx	a,@r1
	inc	r1
	mov	@r0,a
	inc	r0
	djnz	r6,idatapdataloop$
	djnz	r7,idatapdataloop$
	ret
idataxdataloop$:
	movx	a,@dptr
	inc	dptr
	mov	@r0,a
	inc	r0
	djnz	r6,idataxdataloop$
	djnz	r7,idataxdataloop$
	ret
idatacodeloop$:
	clr	a
	movc	a,@a+dptr
	inc	dptr
	mov	@r0,a
	inc	r0
	djnz	r6,idatacodeloop$
	djnz	r7,idatacodeloop$
	ret
xdataptr$:
	jb	acc.7,xdatacodeloop$	; >0x80 code
	jnb	acc.6,xdataxdataloop$	; <0x40 far
	jb	acc.5,xdatapdataloop$	; >0x60 pdata
xdataidataloop$:
	mov	a,@r1
	inc	r1
	movx	@dptr,a
	inc	dptr
	djnz	r6,xdataidataloop$
	djnz	r7,xdataidataloop$
	ret
xdatapdataloop$:
	movx	a,@r1
	inc	r1
	movx	@dptr,a
	inc	dptr
	djnz	r6,xdatapdataloop$
	djnz	r7,xdatapdataloop$
	ret
xdataxdataloop$:
	push	_IE
	clr	EA
	push	_DPL1
	push	_DPH1
	mov	_DPL1,r1
	mov	_DPH1,r2
xdataxdataloop1$:
	xrl	_DPS,#1
	movx	a,@dptr
	inc	dptr
	xrl	_DPS,#1
	movx	@dptr,a
	inc	dptr
	djnz	r6,xdataxdataloop1$
	djnz	r7,xdataxdataloop1$
	pop	_DPH1
	pop	_DPL1
	pop	_IE
	ret
xdatacodeloop$:
	push	_IE
	clr	EA
	push	_DPL1
	push	_DPH1
	mov	_DPL1,r1
	mov	_DPH1,r2
xdatacodeloop1$:
	xrl	_DPS,#1
	clr	a
	movc	a,@a+dptr
	xrl	_DPS,#1
	inc	dptr
	movx	@dptr,a
	inc	dptr
	djnz	r6,xdatacodeloop1$
	djnz	r7,xdatacodeloop1$
	pop	_DPH1
	pop	_DPL1
	pop	_IE
	ret
pdataptr$:
	jb	acc.7,pdatacodeloop$	; >0x80 code
	jnb	acc.6,pdataxdataloop$	; <0x40 far
	jb	acc.5,pdatapdataloop$	; >0x60 pdata
pdataidataloop$:
	mov	a,@r1
	inc	r1
	movx	@r0,a
	inc	r0
	djnz	r6,pdataidataloop$
	djnz	r7,pdataidataloop$
	ret
pdatapdataloop$:
	movx	a,@r1
	inc	r1
	movx	@r0,a
	inc	r0
	djnz	r6,pdatapdataloop$
	djnz	r7,pdatapdataloop$
	ret
pdataxdataloop$:
	movx	a,@dptr
	inc	dptr
	movx	@r0,a
	inc	r0
	djnz	r6,pdataxdataloop$
	djnz	r7,pdataxdataloop$
	ret
pdatacodeloop$:
	clr	a
	movc	a,@a+dptr
	inc	dptr
	movx	@r0,a
	inc	r0
	djnz	r6,pdatacodeloop$
	djnz	r7,pdatacodeloop$
	ret
	__endasm;
}

#elif defined __ICC8051__

__reentrantb void fmemcpy(void __generic *d, const void __generic *s, uint16_t n) __reentrant
{
	uint8_t da = ((uint32_t)d)>>16;
	uint8_t sa = ((uint32_t)s)>>16;
	if (da & 0x80)
		return;
	if (da & 0x01) {
		uint8_t __data *pd = (uint8_t __data *)(uint8_t)d;
		if (sa & 0x80) {
			const uint8_t __code *ps = (const uint8_t __code *)(uint16_t)s;
			for (; n; --n)
				*pd++ = *ps++;
		} else if (sa & 0x01) {
			const uint8_t __data *ps = (const uint8_t __data *)(uint8_t)s;
			for (; n; --n)
				*pd++ = *ps++;
		} else {
			const uint8_t __xdata *ps = (const uint8_t __xdata *)(uint16_t)s;
			for (; n; --n)
				*pd++ = *ps++;
		}
	} else {
		uint8_t __xdata *pd = (uint8_t __xdata *)(uint16_t)d;
		if (sa & 0x80) {
			const uint8_t __code *ps = (const uint8_t __code *)(uint16_t)s;
			for (; n; --n)
				*pd++ = *ps++;
		} else if (sa & 0x01) {
			const uint8_t __data *ps = (const uint8_t __data *)(uint8_t)s;
			for (; n; --n)
				*pd++ = *ps++;
		} else {
			const uint8_t __xdata *ps = (const uint8_t __xdata *)(uint16_t)s;
			for (; n; --n)
				*pd++ = *ps++;
		}
	}
}

#else

__reentrantb void fmemcpy(void __generic *d, const void __generic *s, uint16_t n) __reentrant
{
	char __generic *pd = (char __generic *)d;
	const char __generic *ps = (const char __generic *)s;
	for (; n; --n)
		*pd++ = *ps++;
}

#endif
