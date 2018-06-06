#include "wrnum.h"

#include "libmfuart1.h"
#define uart_tx(x)           do { uart1_tx(x); } while (0)
#define _uart_tx             _uart1_tx
#define uart_writenum32      uart1_writenum32

#if defined(SDCC) && UART != 3

#define WRNUM_BIT_SIGNED   _B_0
#define WRNUM_BIT_PLUS     _B_1
#define WRNUM_BIT_ZEROPLUS _B_2
#define WRNUM_BIT_PADZERO  _B_3
#define WRNUM_BIT_TSDSEP   _B_4
#define WRNUM_BIT_LCHEX    _B_5
#define WRNUM_BIT_DIGSET   _B_6
#define WRNUM_BIT_DIGCONT  _B_7

#define ASCII_PLUS         0x2b
#define ASCII_MINUS        0x2d
#define ASCII_SPACE        0x20
#define ASCII_0            0x30
#define ASCII_APOSTROPHE   0x27

__reentrantb uint8_t uart_writenum32(uint32_t val, uint8_t nrdig1, uint8_t flags1) __reentrant __naked
{
	val;
	nrdig1;
	flags1;
	__asm;
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	mov	r6,a
	mov	r7,b
	; uint8_t flags = flags1;
	mov	a,sp
	add	a,#-3
	mov	r0,a
	mov	a,@r0
	inc	r0
	anl	a,#WRNUM_MASK
	mov	b,a
	; uint8_t nrdig = nrdig1;
	mov	a,@r0
	mov	r2,a
	; char ch = 0
	mov	r0,#0
	; uint8_t d;
	; --> r4
	; uint8_t cnt = 10;
	mov	r3,#10
	; if ((flags & WRNUM_SIGNED) && (int32_t)val < 0) {
	; 	val = -val;
	; 	ch = '-';
	; } else if ((flags & WRNUM_ZEROPLUS) || ((flags & WRNUM_PLUS) && val)) {
	; 	ch = '+';
	; }
	jnb	WRNUM_BIT_SIGNED,00011$
	mov	a,r6
	jnb	acc.7,00011$
	clr	c
	clr	a
	subb	a,dpl
	mov	dpl,a
	clr	a
	subb	a,dph
	mov	dph,a
	clr	a
	subb	a,r7
	mov	r7,a
	clr	a
	subb	a,r6
	mov	r6,a
	mov	r0,#ASCII_MINUS
	sjmp	00010$
00011$:	jb	WRNUM_BIT_ZEROPLUS,00012$
	jnb	WRNUM_BIT_PLUS,00010$
	mov	a,dph
	orl	a,dpl
	orl	a,r7
	orl	a,r6
	jz	00010$
00012$:	mov	r0,#ASCII_PLUS
00010$:	
	; if (ch && nrdig > 0)
	; 	--nrdig;
	mov	a,r0
	jz	00018$
	mov	a,r2
	jz	00018$
	dec	a
	mov	r2,a
00018$:
	; if (flags & WRNUM_TSDSEP) {
	; 	if (nrdig > 9)
	; 		--nrdig;
	; 	if (nrdig > 6)
	; 		--nrdig;
	; 	if (nrdig > 3)
	; 		--nrdig;
	; }
	jnb	WRNUM_BIT_TSDSEP,00022$
	mov	a,r2
	add	a,#-10
	jnc	00020$
	dec	r2
00020$:	mov	a,r2
	add	a,#-7
	jnc	00021$
	dec	r2
00021$:	mov	a,r2
	add	a,#-4
	jnc	00022$
	dec	r2
00022$:
	; flags &= WRNUM_MASK;
	; if (cnt < nrdig)
	; 	cnt = nrdig;
	clr	c
	mov	a,r3
	subb	a,r2
	jnc	00028$
	mov	a,r2
	mov	r3,a
00028$:	
	; do {
00030$:	
	; 	d = cnt;
	push	ar3
	; 	val = libmf_num32_digit(val, (uint8_t __auto *)&d);
	mov	a,sp
	push	b
	push	acc
	mov	b,r7
	mov	a,r6
	lcall	_libmf_num32_digit
	mov	r6,a
	mov	r7,b
	dec	sp
	pop	b
	pop	acc

	; 	if (!d && cnt != 1 && !(flags & WRNUM_DIGCONT)) {
	mov	r4,a
	jnz	00040$
	mov	a,r3
	dec	a
	jz	00040$
	jb	WRNUM_BIT_DIGCONT,00040$
	; 		if (cnt > nrdig)
	; 			continue;
	mov	a,r2
	clr	c
	subb	a,r3
	jc	00038$
	; 		if (!(flags & WRNUM_PADZERO)) {
	jb	WRNUM_BIT_PADZERO,00040$
	; 			if (!(flags & WRNUM_DIGSET)) {
	jb	WRNUM_BIT_DIGSET,00041$
	; 				nrdig = cnt;
	mov	a,r3
	mov	r2,a
	; 				flags |= WRNUM_DIGSET;
	setb	WRNUM_BIT_DIGSET
	; 			}
00041$:
	; 			uart_tx(' ');
	push	b
	push	dpl
	push	dph
	mov	dpl,#ASCII_SPACE
	lcall	_uart_tx
	pop	dph
	pop	dpl
	pop	b
	; 			if ((flags & WRNUM_TSDSEP) && (cnt == 4 || cnt == 7 || cnt == 10)) {
	jnb	WRNUM_BIT_TSDSEP,00038$
	mov	a,r3
	add	a,#-4
	jz	00042$
	add	a,#-3
	jz	00042$
	add	a,#-3
	jnz	00038$
	; 				uart_tx(' ');
00042$:	push	b
	push	dpl
	push	dph
	mov	dpl,#ASCII_SPACE
	lcall	_uart_tx
	pop	dph
	pop	dpl
	pop	b
	; 				++nrdig;
	inc	r2
	; 			}
	; 			continue;
	;sjmp	00038$
	; 		}
	; 	}

	; } while (--cnt);
00038$:	djnz	r3,00030$
	; return nrdig;
	mov	dpl,r2
	ret

00040$:
	; 	if (!(flags & WRNUM_DIGCONT)) {
	jb	WRNUM_BIT_DIGCONT,00048$
	; 		if (!(flags & WRNUM_DIGSET))
	jb	WRNUM_BIT_DIGSET,00049$
	; 			nrdig = cnt;
	mov	a,r3
	mov	r2,a
00049$:
	; 		flags |= WRNUM_PADZERO | WRNUM_DIGCONT | WRNUM_DIGSET;
	mov	a,#WRNUM_PADZERO | WRNUM_DIGCONT | WRNUM_DIGSET
	orl	b,a
	; 		if (ch) {
	mov	a,r0
	jz	00048$
	; 			uart_tx(ch);
	push	b
	push	dpl
	push	dph
	mov	dpl,r0
	lcall	_uart_tx
	pop	dph
	pop	dpl
	pop	b
	; 			++nrdig;
	inc	r2
	; 		}
	; 	}
00048$:
	; 	uart_tx('0' + d);
	push	b
	push	dpl
	push	dph
	mov	a,r4
	add	a,#ASCII_0
	mov	dpl,a
	lcall	_uart_tx
	pop	dph
	pop	dpl
	pop	b
	; 	if ((flags & WRNUM_TSDSEP) && (cnt == 4 || cnt == 7 || cnt == 10)) {
	jnb	WRNUM_BIT_TSDSEP,00038$
	mov	a,r3
	add	a,#-4
	jz	00050$
	add	a,#-3
	jz	00050$
	add	a,#-3
	jnz	00038$
	; 		uart_tx('\'');
00050$:	push	b
	push	dpl
	push	dph
	mov	dpl,#ASCII_APOSTROPHE
	lcall	_uart_tx
	pop	dph
	pop	dpl
	pop	b
	; 		++nrdig;
	inc	r2
	; 	}
	sjmp	00038$
	__endasm;
}

#else

uint8_t uart_writenum32(uint32_t val, uint8_t nrdig1, uint8_t flags1)
{
	char __autodata ch = 0;
	uint8_t __autodata d;
	uint8_t __autodata cnt = 10;
	uint8_t __autodata flags = flags1;
	uint8_t __autodata nrdig = nrdig1;

	if ((flags & WRNUM_SIGNED) && ((int32_t)val) < 0) {
		val = -val;
		ch = '-';
	} else if ((flags & WRNUM_ZEROPLUS) || ((flags & WRNUM_PLUS) && val)) {
		ch = '+';
	}
	if (ch && nrdig > 0)
		--nrdig;
	if (flags & WRNUM_TSDSEP) {
		if (nrdig > 9)
			--nrdig;
		if (nrdig > 6)
			--nrdig;
		if (nrdig > 3)
			--nrdig;
	}
	flags &= WRNUM_MASK;
	if (cnt < nrdig)
		cnt = nrdig;
	do {
		d = cnt;
		val = libmf_num32_digit(val, (uint8_t __auto *)&d);
		if (!d && cnt != 1 && !(flags & WRNUM_DIGCONT)) {
			if (cnt > nrdig)
				continue;
			if (!(flags & WRNUM_PADZERO)) {
				if (!(flags & WRNUM_DIGSET)) {
					nrdig = cnt;
					flags |= WRNUM_DIGSET;
				}
				uart_tx(' ');
				if ((flags & WRNUM_TSDSEP) && (cnt == 4 || cnt == 7 || cnt == 10)) {
					uart_tx(' ');
					++nrdig;
				}
				continue;
			}
		}
		if (!(flags & WRNUM_DIGCONT)) {
			if (!(flags & WRNUM_DIGSET))
				nrdig = cnt;
			flags |= WRNUM_PADZERO | WRNUM_DIGCONT | WRNUM_DIGSET;
			if (ch) {
				uart_tx(ch);
				++nrdig;
			}
		}
		uart_tx('0' + d);
		if ((flags & WRNUM_TSDSEP) && (cnt == 4 || cnt == 7 || cnt == 10)) {
			uart_tx('\'');
			++nrdig;
		}
	} while (--cnt);
	return nrdig;
}

#endif
