#include "ax8052.h"
#include "libmftypes.h"
#include "libmfradio.h"

#if defined(SDCC)

__reentrantb uint32_t radio_read32(uint16_t addr) __reentrant __naked
{
	addr;
        __asm;
	mov	a,dph
	anl	a,#0x0F
	orl	a,#(AX8052_RADIOBASE >> 8)
	mov	dph,a
	mov	a,_IE
	rlc	a
	clr	_EA
	mov	a,_RADIOACC
	mov	b,a
	anl	a,#0xFC
	orl	a,#0x03
	mov	_RADIOACC,a
	movx	a,@dptr
	mov	_RADIOACC,b
	mov	b,_RADIODATA2
	mov	dph,_RADIODATA1
	mov	dpl,_RADIODATA0
	mov	_EA,c
	ret
        __endasm;
}

#else

__reentrantb uint32_t radio_read32(uint16_t addr) __reentrant
{
	uint8_t iesave, racc;
	uint32_t r;
	addr &= 0xFFF;
	addr |= AX8052_RADIOBASE;
	iesave = IE & 0x80;
	EA = 0;
	racc = RADIOACC;
	RADIOACC = (racc & 0xFC) | 0x03;
	r = *(uint8_t __xdata *)addr;
	r <<= 8;
	r |= RADIODATA2;
	r <<= 8;
	r |= RADIODATA1;
	r <<= 8;
	r |= RADIODATA0;
	RADIOACC = racc;
	IE |= iesave;
	return r;
}

#endif
