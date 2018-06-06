#include "libmfcrc.h"

#define crc_buf       crc_crc8ccitt_msb
#define crc_byte      crc_crc8ccitt_msb_byte
#define crc_table_asm _crc_crc8ccitt_msbtable
#define CRCMSB        1

#if defined(SDCC)

__reentrantb uint8_t crc_buf(const uint8_t __generic *buf, uint16_t buflen, uint8_t crc) __reentrant __naked
{
	buf;
	buflen;
	crc;
	__asm;
	mov	a,sp
	add	a,#-4
	mov	r0,a
	mov	a,@r0
	mov	r4,a
	inc	r0
	mov	a,@r0
	mov	r2,a
	inc	r0
	mov	a,@r0
	mov	r3,a
	orl	a,r2
	jz	00001$
	mov	a,r2
	jz	00000$
	inc	r3
00000$:	jb	_B_7,00010$		; >0x80 code
	jnb	_B_6,00020$		; <0x40 far
	mov	r0,dpl
	jb	_B_5,00030$		; >0x60 pdata
	;; idata
00040$:	mov	a,@r0
	inc	r0
	;; lsb/msb: crc = crc_table[crc ^ c]
	xrl	a,r4
	mov	dptr,#crc_table_asm
	movc	a,@a+dptr
	mov	r4,a
	;; loop
	djnz	r2,00040$
	djnz	r3,00040$
	sjmp	00001$

00030$:	movx	a,@r0
	inc	r0
	;; lsb/msb: crc = crc_table[crc ^ c]
	xrl	a,r4
	mov	dptr,#crc_table_asm
	movc	a,@a+dptr
	mov	r4,a
	;; loop
	djnz	r2,00030$
	djnz	r3,00030$

00001$:
	mov	dpl,r4
	ret

00020$:	movx	a,@dptr
	inc	dptr
	mov	r0,dpl
	mov	r1,dph
	;; lsb/msb: crc = crc_table[crc ^ c]
	xrl	a,r4
	mov	dptr,#crc_table_asm
	movc	a,@a+dptr
	mov	r4,a
	;; loop
	mov	dph,r1
	mov	dpl,r0
	djnz	r2,00020$
	djnz	r3,00020$
	sjmp	00001$

00010$:	clr	a
	movc	a,@a+dptr
	inc	dptr
	mov	r0,dpl
	mov	r1,dph
	;; lsb/msb: crc = crc_table[crc ^ c]
	xrl	a,r4
	mov	dptr,#crc_table_asm
	movc	a,@a+dptr
	mov	r4,a
	;; loop
	mov	dph,r1
	mov	dpl,r0
	djnz	r2,00010$
	djnz	r3,00010$
	sjmp	00001$
	__endasm;
}

#else

__reentrantb uint8_t crc_buf(const uint8_t __generic *buf, uint16_t buflen, uint8_t crc) __reentrant
{
	if (!buflen)
		return crc;
	do {
		crc = crc_byte(crc, *buf++);
	} while (--buflen);
	return crc;
}

#endif
