#include "libmfcrc.h"

#define crc_byte      crc_crc8onewire_byte
#define crc_table     crc_crc8onewire_table
#define crc_table_asm _crc_crc8onewire_table

#if defined(SDCC)

__reentrantb uint8_t crc_byte(uint8_t crc, uint8_t c) __reentrant __naked
{
	crc;
	c;
        __asm;
	mov	a,sp
	add	a,#-2
	mov	r0,a
	mov	a,@r0
	xrl	a,dpl
	mov	dptr,#crc_table_asm
	movc	a,@a+dptr
	mov	dpl,a
	ret
        __endasm;
}

#else

__reentrantb uint8_t crc_byte(uint8_t crc, uint8_t c) __reentrant
{
        return crc_table[crc ^ c];
}

#endif
