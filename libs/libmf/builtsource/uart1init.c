#include "ax8052.h"
#include "libmfuart.h"

#include "libmfuart1.h"
#define UARTS0               UART1S0
#define UARTS1               UART1S1
#define UARTS2               UART1S2
#define UARTS3               UART1S3
#define UARTS4               UART1S4
#define UARTS5               UART1S5
#define USHREG               U1SHREG
#define UMODE                U1MODE
#define UCTRL                U1CTRL
#define USTATUS              U1STATUS
#define _USHREG              _U1SHREG
#define _UMODE               _U1MODE
#define _UCTRL               _U1CTRL
#define _USTATUS             _U1STATUS
#define IRQENA               EIE_5
#define uart_irq_nr          12
#define uart_vector_addr     0x63
#define uart_init            uart1_init
#define uart_stop            uart1_stop
#define uart_iocore          uart1_iocore
#define _uart_iocore         _uart1_iocore
#define uart_irq             uart1_irq
#define uart_poll            uart1_poll
#define uart_rxbufptr        uart1_rxbufptr
#define _uart_rxbufptr       _uart1_rxbufptr
#define uart_txbufptr        uart1_txbufptr
#define _uart_txbufptr       _uart1_txbufptr
#define uart_txfreelinear    uart1_txfreelinear
#define _uart_txidle	     _uart1_txidle
#define uart_txidle	     uart1_txidle
#define uart_txfree	     uart1_txfree
#define uart_rxcountlinear   uart1_rxcountlinear
#define uart_rxcount	     uart1_rxcount
#define uart_txbuffersize    uart1_txbuffersize
#define uart_rxbuffersize    uart1_rxbuffersize
#define uart_wait_txfree     uart1_wait_txfree
#define uart_wait_rxcount    uart1_wait_rxcount
#define uart_rxpeek	     uart1_rxpeek
#define uart_txpokehex	     uart1_txpokehex
#define uart_txpoke	     uart1_txpoke
#define uart_rxadvance	     uart1_rxadvance
#define uart_txadvance	     uart1_txadvance
#define uart_rx		     uart1_rx
#define uart_tx              uart1_tx
#define _uart_txpoke	     _uart1_txpoke
#define uart_rxbuffer        uart1_rxbuffer
#define _uart_rxbuffer       _uart1_rxbuffer
#define uart_txbuffer        uart1_txbuffer
#define _uart_txbuffer       _uart1_txbuffer
#define _uart_buffer_size    _uart1_buffer_size
#define _uart_buffer_negsize _uart1_buffer_negsize
#define uart_rxbuffer_size   uart1_rxbuffer_size
#define _uart_rxbuffer_size  _uart1_rxbuffer_size
#define uart_txbuffer_size   uart1_txbuffer_size
#define _uart_txbuffer_size  _uart1_txbuffer_size
#define uart_fiforxwr        uart1_fiforxwr
#define _uart_fiforxwr       _uart1_fiforxwr
#define uart_fiforxrd	     uart1_fiforxrd
#define _uart_fiforxrd       _uart1_fiforxrd
#define uart_fifotxwr	     uart1_fifotxwr
#define _uart_fifotxwr       _uart1_fifotxwr
#define uart_fifotxrd        uart1_fifotxrd
#define _uart_fifotxrd       _uart1_fifotxrd

static volatile uint8_t __data uart_fiforxwr;
static volatile uint8_t __data uart_fiforxrd;
static volatile uint8_t __data uart_fifotxwr;
static volatile uint8_t __data uart_fifotxrd;

extern uint8_t __xdata uart_rxbuffer[];
extern uint8_t __xdata uart_txbuffer[];

#if !defined(SDCC)
extern const uint8_t __code uart_rxbuffer_size[];
extern const uint8_t __code uart_txbuffer_size[];
#endif

#if defined(SDCC)

static void dummy0(void) __naked
{
	__asm;	
	.area HOME   (CODE)
	.area UARTS0 (CODE)
	.area UARTS1 (CODE)
	.area UARTS2 (CODE)
	.area UARTS3 (CODE)
	.area UARTS4 (CODE)
	.area UARTS5 (CODE)

	.area HOME   (CODE)
	__endasm;
}

static __reentrantb void uart_iocore(void) __reentrant __naked;

void uart_irq(void) __interrupt(uart_irq_nr) __naked
{
	__asm;
	push	acc
	push	psw
	push	_DPS
	push	dpl
	push	dph
	push	b
	mov	_DPS,#0
	mov	psw,#0
	lcall	_uart_iocore
	pop	b
	pop	dph
	pop	dpl
	pop	_DPS
	pop	psw
	pop	acc
	reti
	__endasm;
}

uint8_t uart_poll(void) __reentrant __naked
{
	__asm;
	mov	a,#0x80
	anl	a,_IE
	rl	a
	mov	b,a
	clr	_EA
	lcall	_uart_iocore
	mov	a,b
	clr	c
	rrc	a
	mov	_EA,c
	mov	dpl,a
	ret

	.area HOME   (CODE)
	.area UARTS0 (CODE)
	.area UARTS1 (CODE)
	.area UARTS2 (CODE)
	.area UARTS3 (CODE)
	.area UARTS4 (CODE)
	.area UARTS5 (CODE)

	.area UARTS0 (CODE)
	__endasm;
}

static __reentrantb void uart_iocore(void) __reentrant __naked
{
	__asm;
	ar2=0x02
	ar3=0x03
	ar4=0x04
	ar5=0x05
	ar6=0x06
	ar7=0x07
	ar0=0x00
	ar1=0x01

	mov	a,_USTATUS
	jnb	acc.0,iocnorx
	mov	dptr,#_uart_rxbuffer
	mov	a,_uart_fiforxwr
	add	a,dpl
	mov	dpl,a
	clr	a
	addc	a,dph
	mov	dph,a
	mov	a,#_uart_buffer_negsize+3-00010$
	movc	a,@a+pc
00010$:	add	a,_uart_fiforxwr
	jc	00001$
	mov	a,_uart_fiforxwr
	inc	a
00001$:	cjne	a,_uart_fiforxrd,00000$
	anl	_UCTRL,#~0x04
	sjmp	iocnorx
00000$:	mov	_uart_fiforxwr,a
	mov	a,_USHREG
	movx	@dptr,a
	orl	_UCTRL,#0x04
	setb	_B_1
iocnorx:
	mov	a,_USTATUS
	jnb	acc.2,iocnotx
	mov	a,_uart_fifotxrd
	cjne	a,_uart_fifotxwr,00000$
	anl	_UCTRL,#~0x08
	sjmp	iocnotx
00000$:	mov	dptr,#_uart_txbuffer
	add	a,dpl
	mov	dpl,a
	clr	a
	addc	a,dph
	mov	dph,a
	movx	a,@dptr
	mov	_USHREG,a
	mov	a,#_uart_buffer_negsize+1-00010$
	movc	a,@a+pc
00010$:	add	a,_uart_fifotxrd
	jc	00001$
	mov	a,_uart_fifotxrd
	inc	a
00001$:	mov	_uart_fifotxrd,a
	orl	_UCTRL,#0x08
	setb	_B_2
iocnotx:
	ret
	__endasm;
}

void uart_rxadvance(uint8_t idx) __reentrant __naked
{
	idx;
	__asm;
	mov	a,#_uart_buffer_negsize+2-00003$
	movc	a,@a+pc
00003$:	xch	a,dpl
	jz	00000$
	add	a,_uart_fiforxrd
	jnc	00002$
	add	a,dpl
	sjmp	00001$
00002$:	xch	a,dpl
	add	a,dpl
	jc	00001$
	mov	a,dpl
00001$:	mov	_uart_fiforxrd,a
	orl	_UCTRL,#0x04
00000$:	ret
	__endasm;
}

void uart_txadvance(uint8_t idx) __reentrant __naked
{
	idx;
	__asm;
	mov	a,#_uart_buffer_negsize-00003$
	movc	a,@a+pc
00003$:	xch	a,dpl
	jz	00000$
	add	a,_uart_fifotxwr
	jnc	00002$
	add	a,dpl
	sjmp	00001$
00002$:	xch	a,dpl
	add	a,dpl
	jc	00001$
	mov	a,dpl
00001$:	mov	_uart_fifotxwr,a
	orl	_UCTRL,#0x08
00000$:	ret
	__endasm;
}

__reentrantb const uint8_t __xdata *uart_rxbufptr(uint8_t idx) __reentrant __naked
{
	idx;
	__asm;
	mov	a,#_uart_buffer_negsize+2-00003$
	movc	a,@a+pc
00003$:	xch	a,dpl
	add	a,_uart_fiforxrd
	jnc	00002$
	add	a,dpl
	sjmp	00001$
00002$:	xch	a,dpl
	add	a,dpl
	jc	00001$
	mov	a,dpl
00001$:	mov	dptr,#_uart_rxbuffer
	add	a,dpl
	mov	dpl,a
	clr	a
	addc	a,dph
	mov	dph,a
	ret
	__endasm;
}

__reentrantb uint8_t __xdata *uart_txbufptr(uint8_t idx) __reentrant __naked
{
	idx;
	__asm;
	mov	a,#_uart_buffer_negsize-00003$
	movc	a,@a+pc
00003$:	xch	a,dpl
	add	a,_uart_fifotxwr
	jnc	00002$
	add	a,dpl
	sjmp	00001$
00002$:	xch	a,dpl
	add	a,dpl
	jc	00001$
	mov	a,dpl
00001$:	mov	dptr,#_uart_txbuffer
	add	a,dpl
	mov	dpl,a
	clr	a
	addc	a,dph
	mov	dph,a
	ret

_uart_buffer_negsize:
	.area UARTS3 (CODE)
	__endasm;
}

uint8_t uart_txfreelinear(void) __reentrant __naked
{
	__asm;
	mov	a,_uart_fifotxrd
	setb	c
	subb	a,_uart_fifotxwr
	jnc	00000$
	mov	a,_uart_fifotxrd
	add	a,#0xff
	cpl	c
	mov	a,#_uart_buffer_size-00001$
	movc	a,@a+pc
00001$:	subb	a,_uart_fifotxwr
00000$:	mov	dpl,a
	ret
	__endasm;
}

uint8_t uart_txfree(void) __reentrant __naked
{
	__asm;
	mov	a,_uart_fifotxrd
	setb	c
	subb	a,_uart_fifotxwr
	mov	dpl,a
	jnc	00000$
	mov	a,#_uart_buffer_size-00001$
	movc	a,@a+pc
00001$:	add	a,dpl
	mov	dpl,a
00000$:	ret
	__endasm;
}

uint8_t uart_rxcountlinear(void) __reentrant __naked
{
	__asm;
	mov	a,_uart_fiforxwr
	clr	c
	subb	a,_uart_fiforxrd
	jnc	00000$
	mov	a,#_uart_buffer_size+1-00001$
	movc	a,@a+pc
00001$:	clr	c
	subb	a,_uart_fiforxrd
00000$:	mov	dpl,a
	ret
	__endasm;
}

uint8_t uart_rxcount(void) __reentrant __naked
{
	__asm;
	mov	a,_uart_fiforxwr
	clr	c
	subb	a,_uart_fiforxrd
	mov	dpl,a
	jnc	00000$
	mov	a,#_uart_buffer_size+1-00001$
	movc	a,@a+pc
00001$:	add	a,dpl
	mov	dpl,a
00000$:	ret
	__endasm;
}

uint8_t uart_txbuffersize(void) __reentrant __naked
{
	__asm;
	mov	a,#_uart_buffer_size-00000$
	movc	a,@a+pc
00000$:	dec	a
	mov	dpl,a
	ret
	__endasm;
}

uint8_t uart_rxbuffersize(void) __reentrant __naked
{
	__asm;
	mov	a,#_uart_buffer_size+1-00000$
	movc	a,@a+pc
00000$:	dec	a
	mov	dpl,a
	ret

_uart_buffer_size:
	.area CSEG    (CODE)
	__endasm;
}

uint8_t uart_rxpeek(uint8_t idx) __reentrant __naked
{
	idx;
	__asm;
	lcall	_uart_rxbufptr
	movx	a,@dptr
	mov	dpl,a
	ret
	__endasm;
}

void uart_txpoke(uint8_t idx, uint8_t ch) __reentrant __naked
{
	idx;
	ch;
	__asm;
	push	ar0
	mov	a,sp
	add	a,#-3
	mov	r0,a
_uart_txpoke_hexentry:
	lcall	_uart_txbufptr
	mov	a,@r0
	movx	@dptr,a
	pop	ar0
	ret
	__endasm;
}

void uart_txpokehex(uint8_t idx, uint8_t ch) __reentrant __naked
{
	idx;
	ch;
	__asm;
	push	ar0
	mov	a,sp
	add	a,#-3
	mov	r0,a
	mov	a,@r0
	anl	a,#0x0F
	add	a,#256-10
	jnc	00000$
	add	a,#'A-'9-1
00000$:	add	a,#10+'0	; '
	mov	@r0,a
	ljmp	_uart_txpoke_hexentry
	__endasm;
}

uint8_t uart_txidle(void) __reentrant __naked
{
	__asm;
	mov	a,_UCTRL
	jnb	acc.1,00001$
	anl	a,#0x08
	jnz	00000$
	mov	a,_USTATUS
	anl	a,#0x40
	jz	00000$
00001$:	mov	dpl,#1
	ret
00000$:	mov	dpl,#0
	ret
	__endasm;
}

static void wtimer_cansleep_dummy(void) __naked
{
	__asm
	.area HOME      (CODE)
	.area WTCANSLP0 (CODE)
	.area WTCANSLP1 (CODE)
	.area WTCANSLP2 (CODE)

	.area WTCANSLP1 (CODE)
	lcall	_uart_txidle
	mov	a,dpl
	jnz	00000$
	ret
00000$:
	.area CSEG      (CODE)
	__endasm;
}

#elif defined __CX51__ || defined __C51__

static __reentrantb void uart_iocore(void) __reentrant __naked;

void uart_irq(void) interrupt uart_irq_nr
{
#pragma asm
	push	acc
	push	psw
	push	DPS
	push	dpl
	push	dph
	push	b
	mov	DPS,#0
	mov	psw,#0
	lcall	_uart_iocore
	pop	b
	pop	dph
	pop	dpl
	pop	DPS
	pop	psw
	pop	acc
#pragma endasm
}

__reentrantb uint8_t uart_poll(void) __reentrant __naked
{
#pragma asm
	mov	a,#0x80
	anl	a,IE
	rl	a
	mov	b,a
	clr	EA
	lcall	_uart_iocore
	mov	a,b
	clr	c
	rrc	a
	mov	EA,c
	mov	r7,a
#pragma endasm
}

static __reentrantb void uart_iocore(void) __reentrant __naked
{
	uart_rxbuffer[0];
	uart_rxbuffer_size[0];
	uart_txbuffer[0];
	uart_txbuffer_size[0];
#pragma asm
;ar2	equ	0x02
;ar3	equ	0x03
;ar4	equ	0x04
;ar5	equ	0x05
;ar6	equ	0x06
;ar7	equ	0x07
;ar0	equ	0x00
;ar1	equ	0x01

_uart_iocore:
	mov	a,USTATUS
	jnb	acc.0,iocnorx
	clr	a
	mov	dptr,#uart_rxbuffer_size+2
	movc	a,@a+dptr
	push	acc
	mov	dptr,#uart_rxbuffer
	mov	a,uart_fiforxwr
	add	a,dpl
	mov	dpl,a
	clr	a
	addc	a,dph
	mov	dph,a
	pop	acc
	add	a,uart_fiforxwr
	jc	ioc1
	mov	a,uart_fiforxwr
	inc	a
ioc1:	cjne	a,uart_fiforxrd,ioc0
	anl	UCTRL,#~0x04
	sjmp	iocnorx
ioc0:	mov	uart_fiforxwr,a
	mov	a,USHREG
	movx	@dptr,a
	orl	UCTRL,#0x04
	setb	B_1
iocnorx:
	mov	a,USTATUS
	jnb	acc.2,iocnotx
	mov	a,uart_fifotxrd
	cjne	a,uart_fifotxwr,ioc2
	anl	UCTRL,#~0x08
	sjmp	iocnotx
ioc2:	mov	dptr,#uart_txbuffer
	add	a,dpl
	mov	dpl,a
	clr	a
	addc	a,dph
	mov	dph,a
	movx	a,@dptr
	mov	USHREG,a
	clr	a
	mov	dptr,#uart_txbuffer_size+2
	movc	a,@a+dptr
	add	a,uart_fifotxrd
	jc	ioc3
	mov	a,uart_fifotxrd
	inc	a
ioc3:	mov	uart_fifotxrd,a
	orl	UCTRL,#0x08
	setb	B_2
iocnotx:
#pragma endasm
}

__reentrantb void uart_rxadvance(uint8_t idx) __reentrant
{
	idx;
#pragma asm
	clr	a
	mov	dptr,#uart_rxbuffer_size+1
	movc	a,@a+dptr
	xch	a,r7
	jz	rxad0
	add	a,uart_fiforxrd
	jnc	rxad2
	add	a,r7
	sjmp	rxad1
rxad2:	xch	a,r7
	add	a,r7
	jc	rxad1
	mov	a,r7
rxad1:	mov	uart_fiforxrd,a
	orl	UCTRL,#0x04
rxad0:
#pragma endasm
}

__reentrantb void uart_txadvance(uint8_t idx) __reentrant
{
	idx;
#pragma asm
	clr	a
	mov	dptr,#uart_txbuffer_size+1
	movc	a,@a+dptr
	xch	a,r7
	jz	txad0
	add	a,uart_fifotxwr
	jnc	txad2
	add	a,r7
	sjmp	txad1
txad2:	xch	a,r7
	add	a,r7
	jc	txad1
	mov	a,r7
txad1:	mov	uart_fifotxwr,a
	orl	UCTRL,#0x08
txad0:
#pragma endasm
}

__reentrantb const uint8_t __xdata *uart_rxbufptr(uint8_t idx) __reentrant
{
	idx;
#pragma asm
_uart_rxbufptr:
	clr	a
	mov	dptr,#uart_rxbuffer_size+1
	movc	a,@a+dptr
	xch	a,r7
	add	a,uart_fiforxrd
	jnc	rxp2
	add	a,r7
	sjmp	rxp1
rxp2:	xch	a,r7
	add	a,r7
	jc	rxp1
	mov	a,r7
rxp1:	mov	dptr,#uart_rxbuffer
	add	a,dpl
	mov	r7,a
	clr	a
	addc	a,dph
	mov	r6,a
#pragma endasm
}

__reentrantb uint8_t __xdata *uart_txbufptr(uint8_t idx) __reentrant
{
	idx;
#pragma asm
_uart_txbufptr:
	clr	a
	mov	dptr,#uart_txbuffer_size+1
	movc	a,@a+dptr
	xch	a,r7
	add	a,uart_fifotxwr
	jnc	txp2
	add	a,r7
	sjmp	txp1
txp2:	xch	a,r7
	add	a,r7
	jc	txp1
	mov	a,r7
txp1:	mov	dptr,#uart_txbuffer
	add	a,dpl
	mov	r7,a
	clr	a
	addc	a,dph
	mov	r6,a
#pragma endasm
}

__reentrantb uint8_t uart_txfreelinear(void) __reentrant
{
#pragma asm
	mov	a,uart_fifotxrd
	setb	c
	subb	a,uart_fifotxwr
	mov	r7,a
	jnc	txfrl0
	mov	a,uart_fifotxrd
	add	a,#0xff
	cpl	c
	clr	a
	mov	dptr,#uart_txbuffer_size
	movc	a,@a+dptr
	subb	a,uart_fifotxwr
txfrl0:	mov	r7,a
#pragma endasm
}

__reentrantb uint8_t uart_txfree(void) __reentrant
{
#pragma asm
	mov	a,uart_fifotxrd
	setb	c
	subb	a,uart_fifotxwr
	mov	r7,a
	jnc	txfr0
	clr	a
	mov	dptr,#uart_txbuffer_size
	movc	a,@a+dptr
	add	a,r7
	mov	r7,a
txfr0:
#pragma endasm
}

__reentrantb uint8_t uart_rxcountlinear(void) __reentrant
{
#pragma asm
	mov	a,uart_fiforxwr
	clr	c
	subb	a,uart_fiforxrd
	jnc	rxcl0
	clr	a
	mov	dptr,#uart_rxbuffer_size
	movc	a,@a+dptr
	clr	c
	subb	a,uart_fiforxrd
rxcl0:	mov	r7,a
#pragma endasm
}

__reentrantb uint8_t uart_rxcount(void) __reentrant
{
#pragma asm
	mov	a,uart_fiforxwr
	clr	c
	subb	a,uart_fiforxrd
	mov	r7,a
	jnc	rxc0
	clr	a
	mov	dptr,#uart_rxbuffer_size
	movc	a,@a+dptr
	add	a,r7
	mov	r7,a
rxc0:
#pragma endasm
}

__reentrantb uint8_t uart_txbuffersize(void) __reentrant
{
	return uart_txbuffer_size[0]-1;
}

__reentrantb uint8_t uart_rxbuffersize(void) __reentrant
{
	return uart_rxbuffer_size[0]-1;
}

__reentrantb uint8_t uart_rxpeek(uint8_t idx) __reentrant
{
	idx;
#pragma asm
	lcall	_uart_rxbufptr
	mov	dpl,r7
	mov	dph,r6
	movx	a,@dptr
	mov	r7,a
#pragma endasm
}

__reentrantb void uart_txpoke(uint8_t idx, uint8_t ch) __reentrant
{
	idx;
	ch;
#pragma asm
_uart_txpokehex_entry:
	lcall	_uart_txbufptr
	mov	dpl,r7
	mov	dph,r6
	mov	a,r5
	movx	@dptr,a
#pragma endasm
}

__reentrantb void uart_txpokehex(uint8_t idx, uint8_t ch) __reentrant
{
	idx;
	ch;
#pragma asm
	mov	a,r5
	anl	a,#0x0F
	add	a,#256-10
	jnc	txph0
	add	a,#'A'-'9'-1
txph0:	add	a,#10+'0'
	mov	r5,a
	ljmp	_uart_txpokehex_entry
#pragma endasm
}

__reentrantb uint8_t uart_txidle(void) __reentrant
{
#pragma asm
	mov	r7,#0
	mov	a,UCTRL
	jnb	acc.1,txidle
	anl	a,#0x08
	jnz	txnotidle
	mov	a,USTATUS
	anl	a,#0x40
	jz	txnotidle
txidle:
	mov	r7,#1
txnotidle:
#pragma endasm
}

#elif defined __ICC8051__

static __reentrantb uint8_t uart_iocore(void) __reentrant;

#pragma vector=uart_vector_addr
__interrupt void uart_irq(void)
{
	uint8_t __autodata dpssave = DPS;
	DPS = 0;
	uart_iocore();
	DPS = dpssave;
}

__reentrantb uint8_t uart_poll(void) __reentrant
{
	uint8_t flg;
	uint8_t irq = IE & 0x80;
	EA = 0;
	flg = uart_iocore();
	IE |= irq;
	return flg;
}

static __reentrantb uint8_t uart_iocore(void) __reentrant
{
	uint8_t flg = 0;
	if (USTATUS & 0x01) {
		uint8_t wp = uart_fiforxwr + 1;
		uint8_t sz = uart_rxbuffer_size[0];
		if (wp >= sz)
			wp -= sz;
		if (wp != uart_fiforxrd) {
			uart_rxbuffer[uart_fiforxwr] = USHREG;
			uart_fiforxwr = wp;
			UCTRL |= 0x04;
			flg |= 1;
		} else {
			UCTRL &= (uint8_t)~0x04;
		}
	}
	if (USTATUS & 0x04) {
		if (uart_fifotxrd != uart_fifotxwr) {
			uint8_t rp = uart_fifotxrd + 1;
			uint8_t sz = uart_txbuffer_size[0];
			USHREG = uart_txbuffer[uart_fifotxrd];
			if (rp >= sz)
				rp -= sz;
			uart_fifotxrd = rp;
			UCTRL |= 0x08;
			flg |= 2;
		} else {
			UCTRL &= (uint8_t)~0x08;
		}
	}
	return flg;
}

__reentrantb void uart_rxadvance(uint8_t idx) __reentrant
{
	uint8_t rd;
	uint8_t sz;
	if (!idx)
		return;
	rd = uart_fiforxrd;
	idx += rd;
	sz = uart_rxbuffer_size[0];
	if (idx < rd || idx >= sz)
		idx -= sz;
	uart_fiforxrd = idx;
	UCTRL |= 0x04;
}

__reentrantb void uart_txadvance(uint8_t idx) __reentrant
{
	uint8_t wr;
	uint8_t sz;
	if (!idx)
		return;
	wr = uart_fifotxwr;
	idx += wr;
	sz = uart_txbuffer_size[0];
	if (idx < wr || idx >= sz)
		idx -= sz;
	uart_fifotxwr = idx;
	UCTRL |= 0x08;
}

__reentrantb const uint8_t __xdata *uart_rxbufptr(uint8_t idx) __reentrant
{
	uint8_t rd = uart_fiforxrd;
	uint8_t sz = uart_rxbuffer_size[0];
	idx += rd;
	if (idx < rd || idx >= sz)
		idx -= sz;
	return &uart_rxbuffer[idx];
}

__reentrantb uint8_t __xdata *uart_txbufptr(uint8_t idx) __reentrant
{
	uint8_t wr = uart_fifotxwr;
	uint8_t sz = uart_txbuffer_size[0];
	idx += wr;
	if (idx < wr || idx >= sz)
		idx -= sz;
	return &uart_txbuffer[idx];
}

__reentrantb uint8_t uart_txfreelinear(void) __reentrant
{
	uint8_t rd = uart_fifotxrd;
	uint8_t wr = uart_fifotxwr;
	if (rd <= wr) {
		uint8_t r = uart_txbuffer_size[0] - wr;
		if (!rd)
			--r;
		return r;
	}
	return rd - wr - 1;
}

__reentrantb uint8_t uart_txfree(void) __reentrant
{
	uint8_t rd = uart_fifotxrd;
	uint8_t wr = uart_fifotxwr;
	uint8_t r = rd - wr;
	if (rd <= wr)
		r += uart_txbuffer_size[0];
	--r;
	return r;
}

__reentrantb uint8_t uart_rxcountlinear(void) __reentrant
{
	uint8_t rd = uart_fiforxrd;
	uint8_t wr = uart_fiforxwr;
	if (wr < rd)
		return uart_rxbuffer_size[0] - rd;
	return wr - rd;
}

__reentrantb uint8_t uart_rxcount(void) __reentrant
{
	uint8_t rd = uart_fiforxrd;
	uint8_t wr = uart_fiforxwr;
	uint8_t r = wr - rd;
	if (wr < rd)
		r += uart_rxbuffer_size[0];
	return r;
}

__reentrantb uint8_t uart_txbuffersize(void) __reentrant
{
	return uart_txbuffer_size[0]-1;
}

__reentrantb uint8_t uart_rxbuffersize(void) __reentrant
{
	return uart_rxbuffer_size[0]-1;
}

__reentrantb uint8_t uart_rxpeek(uint8_t idx) __reentrant
{
	const uint8_t __xdata *bp = uart_rxbufptr(idx);
	return *bp;
}

__reentrantb void uart_txpoke(uint8_t idx, uint8_t ch) __reentrant
{
	uint8_t __xdata *bp = uart_txbufptr(idx);
	*bp = ch;
}

__reentrantb void uart_txpokehex(uint8_t idx, uint8_t ch) __reentrant
{
	ch &= 0x0F;
	if (ch >= 10)
		ch += 'A' - '9' - 1;
	ch += '0';
	uart_txpoke(idx, ch);
}

__reentrantb uint8_t uart_txidle(void) __reentrant
{
	if (!(UCTRL & 0x02))
		return 1;
	if ((UCTRL & 0x08) || !(USTATUS & 0x40))
		return 0;
	return 1;
}

#else

#error "Compiler unsupported"

#endif

void uart_init(uint8_t timernr, uint8_t wl, uint8_t stop)
{
	uart_fiforxwr = uart_fiforxrd = uart_fifotxwr = uart_fifotxrd = 0;
	UMODE = ((timernr + 1) & 3) | ((wl & 7) << 2) | ((stop >= 2) << 5) | 0x40;
	UCTRL = 0x07;
	IRQENA = 1;
}
