#include "libmfcrc.h"

#define crc_byte      crc_crc32_byte
#define crc_table     crc_crc32_table
#define crc_table_asm _crc_crc32_table

#if defined(SDCC)

__reentrantb uint32_t crc_byte(uint32_t crc, uint8_t c) __reentrant __naked
{
	crc;
	c;
        __asm;
	mov	r4,a
	mov	a,sp
	add	a,#-2
	mov	r0,a
	mov	a,@r0
	xrl	a,dpl
	rl	a
	rl	a
	mov	r3,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	mov	dpl,a
	mov	a,r3
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	xch	a,dph
	mov	r2,a
	clr	a
	movc	a,@a+dptr
	xrl	a,r2
	mov	r2,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r4
	mov	r3,a
	mov	a,#3
	movc	a,@a+dptr
	mov	r4,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,b
	mov	dph,a
	mov	dpl,r2
	mov	b,r3
	mov	a,r4
	ret
        __endasm;
}

#else

__reentrantb uint32_t crc_byte(uint32_t crc, uint8_t c) __reentrant
{
        return (crc >> 8) ^ crc_table[((uint8_t)crc ^ c) & (uint8_t)0xff];
}

#endif
