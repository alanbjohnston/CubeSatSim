#include "libmfcrc.h"

#if CRCMODE == 0
#define crc_msb_byte     crc_crc8ccitt_msb_byte
#define crc_msbtable     crc_crc8ccitt_msbtable
#define crc_msbtable_asm _crc_crc8ccitt_msbtable
#elif CRCMODE == 1
#define crc_msb_byte     crc_crc8onewire_msb_byte
#define crc_msbtable     crc_crc8onewire_msbtable
#define crc_msbtable_asm _crc_crc8onewire_msbtable
#else
#error "invalid CRCMODE"
#endif

#if defined(SDCC)

__reentrantb uint8_t crc_msb_byte(uint8_t crc, uint8_t c) __reentrant __naked
{
	crc;
	c;
        __asm;
	mov	a,sp
	add	a,#-2
	mov	r0,a
	mov	a,@r0
	xrl	a,dpl
	mov	dptr,#crc_msbtable_asm
	movc	a,@a+dptr
	mov	dpl,a
	ret
        __endasm;
}

#else

__reentrantb uint8_t crc_msb_byte(uint8_t crc, uint8_t c) __reentrant
{
        return crc_msbtable[crc ^ c];
}

#endif
