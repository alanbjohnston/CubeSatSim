#include "libmfcrc.h"

#define crc_msb_byte     crc_crc32_msb_byte
#define crc_msbtable     crc_crc32_msbtable
#define crc_msbtable_asm _crc_crc32_msbtable

#if defined(SDCC)

__reentrantb uint32_t crc_msb_byte(uint32_t crc, uint8_t c) __reentrant __naked
{
	crc;
	c;
        __asm;
	mov	r2,a
	mov	a,sp
	add	a,#-2
	mov	r0,a
	mov	a,@r0
	xrl	a,r2
	rl	a
	rl	a
	mov	r3,a
	anl	a,#0xfc
	add	a,#crc_msbtable_asm
	xch	a,dpl
	mov	r2,a
	mov	a,r3
	anl	a,#0x03
	addc	a,#(crc_msbtable_asm >> 8)
	xch	a,dph
	mov	r3,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r2
	mov	r2,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r3
	mov	r3,a
	mov	a,#3
	movc	a,@a+dptr
	xrl	a,b
	mov	r4,a
	clr	a
	movc	a,@a+dptr
	mov	dpl,a
	mov	dph,r2
	mov	b,r3
	mov	a,r4
	ret
        __endasm;
}

#else

__reentrantb uint32_t crc_msb_byte(uint32_t crc, uint8_t c) __reentrant
{
        return (crc << 8) ^ crc_msbtable[((uint8_t)(crc >> 24) ^ c) & (uint8_t)0xff];
}

#endif
