#include "ax8052.h"
#include "libmftypes.h"
#include "libmfradio.h"

#if defined(SDCC)

__reentrantb void radio_write16(uint16_t addr, uint16_t d) __reentrant __naked
{
	addr;
	d;
        __asm;
	ar2=0x02
	ar3=0x03
	ar4=0x04
	ar5=0x05
	ar6=0x06
	ar7=0x07
	ar0=0x00
	ar1=0x01

	mov	a,dph
	anl	a,#0x0F
	orl	a,#(AX8052_RADIOBASE >> 8)
	mov	dph,a
	mov	a,sp
	add	a,#-3
	push	ar0
	mov	r0,a
	mov	a,_IE
	rlc	a
	clr	_EA
	mov	a,_RADIOACC
	mov	b,a
	anl	a,#0xFC
	orl	a,#0x01
	mov	_RADIOACC,a
	mov	_RADIODATA2,@r0
	inc	r0
	mov	a,@r0
	movx	@dptr,a
	mov	_RADIOACC,b
	mov	_EA,c
	pop	ar0
	ret
        __endasm;
}

#else

__reentrantb void radio_write16(uint16_t addr, uint16_t d) __reentrant
{
	uint8_t iesave, racc;
	addr &= 0xFFF;
	addr |= AX8052_RADIOBASE;
	iesave = IE & 0x80;
	EA = 0;
	racc = RADIOACC;
	RADIOACC = (racc & 0xFC) | 0x01;
	RADIODATA2 = d;
	d >>= 8;
	*(uint8_t __xdata *)addr = d;
	RADIOACC = racc;
	IE |= iesave;
}

#endif
