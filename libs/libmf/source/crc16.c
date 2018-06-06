#include "libmfcrc.h"

#if CRCMODE == 0
#define crc_byte      crc_crc16_byte
#define crc_table     crc_crc16_table
#define crc_table_asm _crc_crc16_table
#elif CRCMODE == 1
#define crc_byte      crc_crc16dnp_byte
#define crc_table     crc_crc16dnp_table
#define crc_table_asm _crc_crc16dnp_table
#elif CRCMODE == 2
#define crc_byte      crc_ccitt_byte
#define crc_table     crc_ccitt_table
#define crc_table_asm _crc_ccitt_table
#else
#error "invalid CRCMODE"
#endif

#if defined(SDCC)

__reentrantb uint16_t crc_byte(uint16_t crc, uint8_t c) __reentrant __naked
{
	crc;
	c;
        __asm;
	mov	a,sp
	add	a,#-2
	mov	r0,a
	mov	a,@r0
	xrl	a,dpl
	clr	c
	rlc	a
	mov	r2,a
	clr	a
	rlc	a
	mov	r3,a
	mov	a,#crc_table_asm
	add	a,r2
	mov	dpl,a
	mov	a,#(crc_table_asm >> 8)
	addc	a,r3
	xch	a,dph
	mov	r2,a
	clr	a
	movc	a,@a+dptr
	xrl	a,r2
	mov	r2,a
	mov	a,#1
	movc	a,@a+dptr
	mov	dph,a
	mov	dpl,r2
	ret
        __endasm;
}

#else

__reentrantb uint16_t crc_byte(uint16_t crc, uint8_t c) __reentrant
{
        return (crc >> 8) ^ crc_table[((uint8_t)crc ^ c) & (uint8_t)0xff];
}

#endif
