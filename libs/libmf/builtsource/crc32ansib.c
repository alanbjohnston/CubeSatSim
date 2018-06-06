#include "libmfcrc.h"

#define crc_buf       crc_crc32
#define crc_byte      crc_crc32_byte
#define crc_table_asm _crc_crc32_table
#define CRCMSB        0

#if defined(SDCC)

__reentrantb uint32_t crc_buf(const uint8_t __generic *buf, uint16_t buflen, uint32_t crc) __reentrant __naked
{
	buf;
	buflen;
	crc;
	__asm;
	mov	a,sp
	add	a,#-7
	mov	r0,a
	mov	a,@r0
	mov	r4,a
	inc	r0
	mov	a,@r0
	mov	r5,a
	inc	r0
	mov	a,@r0
	mov	r6,a
	inc	r0
	mov	a,@r0
	mov	r7,a
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
00000$:	jb	_B_7,00011$		; >0x80 code
	jnb	_B_6,00020$		; <0x40 far
	mov	r0,dpl
	jb	_B_5,00030$		; >0x60 pdata
	;; idata
00040$:	mov	a,@r0
	inc	r0
#if !CRCMSB
	;; lsb: crc = (crc >> 8) ^ crc_table[((uint8_t)crc ^ c) & (uint8_t)0xff]
	xrl	a,r4
	rl	a
	rl	a
	mov	dpl,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	xch	a,dpl
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	xrl	a,r5
	mov	r4,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r6
	mov	r5,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r7
	mov	r6,a
	mov	a,#3
	movc	a,@a+dptr
	mov	r7,a
#else
	;; msb: crc = (crc << 8) ^ crc_table[((uint8_t)(crc >> 24) ^ c) & (uint8_t)0xff]
	xrl	a,r7
	rl	a
	rl	a
	mov	dpl,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	xch	a,dpl
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	mov	dph,a
	mov	a,#3
	movc	a,@a+dptr
	xrl	a,r6
	mov	r7,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r5
	mov	r6,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r4
	mov	r5,a
	clr	a
	movc	a,@a+dptr
	mov	r4,a
#endif
	;; loop
	djnz	r2,00040$
	djnz	r3,00040$
	sjmp	00001$

00030$:	movx	a,@r0
	inc	r0
#if !CRCMSB
	;; lsb: crc = (crc >> 8) ^ crc_table[((uint8_t)crc ^ c) & (uint8_t)0xff]
	xrl	a,r4
	rl	a
	rl	a
	mov	dpl,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	xch	a,dpl
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	xrl	a,r5
	mov	r4,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r6
	mov	r5,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r7
	mov	r6,a
	mov	a,#3
	movc	a,@a+dptr
	mov	r7,a
#else
	;; msb: crc = (crc << 8) ^ crc_table[((uint8_t)(crc >> 24) ^ c) & (uint8_t)0xff]
	xrl	a,r7
	rl	a
	rl	a
	mov	dpl,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	xch	a,dpl
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	mov	dph,a
	mov	a,#3
	movc	a,@a+dptr
	xrl	a,r6
	mov	r7,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r5
	mov	r6,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r4
	mov	r5,a
	clr	a
	movc	a,@a+dptr
	mov	r4,a
#endif
	;; loop
	djnz	r2,00030$
	djnz	r3,00030$

00001$:
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r7
	ret

00011$:
	sjmp	00010$

00020$:	movx	a,@dptr
	inc	dptr
	mov	r0,dpl
	mov	r1,dph
#if !CRCMSB
	;; lsb: crc = (crc >> 8) ^ crc_table[((uint8_t)crc ^ c) & (uint8_t)0xff]
	xrl	a,r4
	rl	a
	rl	a
	mov	dpl,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	xch	a,dpl
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	xrl	a,r5
	mov	r4,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r6
	mov	r5,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r7
	mov	r6,a
	mov	a,#3
	movc	a,@a+dptr
	mov	r7,a
#else
	;; msb: crc = (crc << 8) ^ crc_table[((uint8_t)(crc >> 24) ^ c) & (uint8_t)0xff]
	xrl	a,r7
	rl	a
	rl	a
	mov	dpl,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	xch	a,dpl
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	mov	dph,a
	mov	a,#3
	movc	a,@a+dptr
	xrl	a,r6
	mov	r7,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r5
	mov	r6,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r4
	mov	r5,a
	clr	a
	movc	a,@a+dptr
	mov	r4,a
#endif
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
#if !CRCMSB
	;; lsb: crc = (crc >> 8) ^ crc_table[((uint8_t)crc ^ c) & (uint8_t)0xff]
	xrl	a,r4
	rl	a
	rl	a
	mov	dpl,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	xch	a,dpl
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	mov	dph,a
	clr	a
	movc	a,@a+dptr
	xrl	a,r5
	mov	r4,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r6
	mov	r5,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r7
	mov	r6,a
	mov	a,#3
	movc	a,@a+dptr
	mov	r7,a
#else
	;; msb: crc = (crc << 8) ^ crc_table[((uint8_t)(crc >> 24) ^ c) & (uint8_t)0xff]
	xrl	a,r7
	rl	a
	rl	a
	mov	dpl,a
	anl	a,#0xfc
	add	a,#crc_table_asm
	xch	a,dpl
	anl	a,#0x03
	addc	a,#(crc_table_asm >> 8)
	mov	dph,a
	mov	a,#3
	movc	a,@a+dptr
	xrl	a,r6
	mov	r7,a
	mov	a,#2
	movc	a,@a+dptr
	xrl	a,r5
	mov	r6,a
	mov	a,#1
	movc	a,@a+dptr
	xrl	a,r4
	mov	r5,a
	clr	a
	movc	a,@a+dptr
	mov	r4,a
#endif
	;; loop
	mov	dph,r1
	mov	dpl,r0
	djnz	r2,00010$
	djnz	r3,00010$
	sjmp	00001$
	__endasm;
}

#else

__reentrantb uint32_t crc_buf(const uint8_t __generic *buf, uint16_t buflen, uint32_t crc) __reentrant
{
	if (!buflen)
		return crc;
	do {
		crc = crc_byte(crc, *buf++);
	} while (--buflen);
	return crc;
}

#endif
