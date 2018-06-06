#include "libmfdbglink.h"
#define uart_txfree		dbglink_txfree
#define uart_rxcount		dbglink_rxcount
#define uart_wait_txfree	dbglink_wait_txfree
#define _uart_wait_txfree	_dbglink_wait_txfree
#define uart_wait_rxcount	dbglink_wait_rxcount
#define uart_txbufptr		dbglink_txbufptr
#define _uart_txbufptr		_dbglink_txbufptr
#define uart_txfreelinear	dbglink_txfreelinear
#define _uart_txfreelinear 	_dbglink_txfreelinear
#define uart_rxpeek		dbglink_rxpeek
#define uart_txpokehex		dbglink_txpokehex
#define uart_txpoke		dbglink_txpoke
#define uart_rxadvance		dbglink_rxadvance
#define uart_txadvance		dbglink_txadvance
#define _uart_txadvance		_dbglink_txadvance
#define uart_rx			dbglink_rx
#define uart_tx			dbglink_tx
#define uart_writestr		dbglink_writestr
#define uart_writehexu16	dbglink_writehexu16
#define uart_writehexu32	dbglink_writehexu32
#define uart_writeu16		dbglink_writeu16
#define uart_writeu32		dbglink_writeu32

#if defined(SDCC)

__reentrantb void uart_writestr(const char __generic *ch) __reentrant
{
	ch;
	__asm;
	mov	r0,dpl
	mov	r7,dph
	clr	a
	mov	r3,a
	mov	r2,a
00000$:	jb	_B_7,00010$		; >0x80 code
	jnb	_B_6,00011$		; <0x40 far
	jb	_B_5,00012$		; >0x60 pdata
	;; idata
	mov	a,@r0
	inc	r0
	sjmp	00013$
00010$: ;; code
	mov	dpl,r0
	mov	dph,r7
	clr	a
	movc	a,@a+dptr
	inc	dptr
	mov	r0,dpl
	mov	r7,dph
	sjmp	00013$
00011$:	;; xdata
	mov	dpl,r0
	mov	dph,r7
	movx	a,@dptr
	inc	dptr
	mov	r0,dpl
	mov	r7,dph
	sjmp	00013$
00012$:	;; pdata
	movx	a,@r0
	inc	r0
00013$:	jz	00001$
	mov	r1,a
	mov	a,r3
	jnz	00002$
	mov	a,r2
	jz	00003$
	mov	dpl,a
	lcall	_uart_txadvance
00003$:	lcall	_uart_txfreelinear
	mov	a,dpl
	jnz	00004$
	mov	r4,b
	mov	dpl,#1
	lcall	_uart_wait_txfree
	mov	b,r4
	lcall	_uart_txfreelinear
	mov	a,dpl
00004$:	mov	r3,a
	clr	a
	mov	r2,a
	mov	dpl,a
	lcall	_uart_txbufptr
	mov	r4,dpl
	mov	r5,dph
00002$:	mov	dpl,r4
	mov	dph,r5
	mov	a,r1
	movx	@dptr,a
	inc	dptr
	mov	r4,dpl
	mov	r5,dph
	inc	r2
	dec	r3
	sjmp	00000$
00001$:	mov	a,r2
	jz	00005$
	mov	dpl,a
	lcall	_uart_txadvance
00005$:
	__endasm;
}

#else

__reentrantb void uart_writestr(const char __generic *ch) __reentrant
{
	uint8_t __xdata *p;
	uint8_t f = 0;
	uint8_t a = 0;
	for (;;) {
		char c = *ch++;
		if (!c)
			break;
		if (!f) {
			if (a)
				uart_txadvance(a);
			f = uart_txfreelinear();
			if (!f) {
				uart_wait_txfree(1);
				f = uart_txfreelinear();
			}
			p = uart_txbufptr(0);
			a = 0;
		}
		*p++ = c;
		++a;
		--f;
	}
	if (a)
		uart_txadvance(a);
}

#endif
