#ifndef LIBMFUART0_H
#define LIBMFUART0_H

#include "libmftypes.h"

/*
 * UART 0
 */
#if defined SDCC

#define UART0_DEFINE_RXBUFFER(sz)                               \
static void uart0_define_rxbuffer(void) __naked                 \
{                                                               \
	__asm                                                   \
	.area XSEG    (XDATA)                                   \
	.globl  _uart0_rxbuffer                                 \
_uart0_rxbuffer:                                                \
	.ds	sz                                              \
                                                                \
	.area HOME    (CODE)                                    \
	.area UART0S0 (CODE)                                    \
	.area UART0S1 (CODE)                                    \
	.area UART0S2 (CODE)                                    \
	.area UART0S3 (CODE)                                    \
	.area UART0S4 (CODE)                                    \
	.area UART0S5 (CODE)                                    \
                                                                \
	.area UART0S2 (CODE)                                    \
	.db	256-sz                                          \
	.db	257-sz                                          \
                                                                \
	.area UART0S5 (CODE)                                    \
	.db	sz                                              \
                                                                \
	.area CSEG    (CODE)                                    \
	__endasm;                                               \
}

#define UART0_DEFINE_TXBUFFER(sz)                               \
static void uart0_define_txbuffer(void) __naked                 \
{                                                               \
	__asm                                                   \
	.area XSEG    (XDATA)                                   \
	.globl  _uart0_txbuffer                                 \
_uart0_txbuffer:                                                \
	.ds	sz                                              \
                                                                \
	.area HOME    (CODE)                                    \
	.area UART0S0 (CODE)                                    \
	.area UART0S1 (CODE)                                    \
	.area UART0S2 (CODE)                                    \
	.area UART0S3 (CODE)                                    \
	.area UART0S4 (CODE)                                    \
	.area UART0S5 (CODE)                                    \
                                                                \
	.area UART0S1 (CODE)                                    \
	.db	256-sz                                          \
	.db	257-sz                                          \
                                                                \
	.area UART0S4 (CODE)                                    \
	.db	sz                                              \
                                                                \
	.area CSEG    (CODE)                                    \
	__endasm;                                               \
}

#elif defined __CX51__ || defined __C51__

#define UART0_DEFINE_RXBUFFER(sz)                               \
uint8_t __xdata uart0_rxbuffer[sz];                             \
const uint8_t __code uart0_rxbuffer_size[] = { sz, 256-(sz), 257-(sz) };

#define UART0_DEFINE_TXBUFFER(sz)                               \
uint8_t __xdata uart0_txbuffer[sz];                             \
const uint8_t __code uart0_txbuffer_size[] = { sz, 256-(sz), 257-(sz) };

#else

#define UART0_DEFINE_RXBUFFER(sz)                               \
uint8_t __xdata uart0_rxbuffer[sz];                             \
const uint8_t __code uart0_rxbuffer_size[] = { sz };

#define UART0_DEFINE_TXBUFFER(sz)                               \
uint8_t __xdata uart0_txbuffer[sz];                             \
const uint8_t __code uart0_txbuffer_size[] = { sz };

#endif

#if defined SDCC
#pragma callee_saves uart0_txbufptr,uart0_txfreelinear,uart0_rxbufptr,uart0_rxcountlinear
#pragma callee_saves uart0_txfree,uart0_rxcount,uart0_wait_txfree,uart0_wait_rxcount
#pragma callee_saves uart0_txbuffersize,uart0_rxbuffersize,uart0_rxpeek,uart0_txpoke,uart0_txpokehex
#pragma callee_saves uart0_init,uart0_poll,uart0_wait_txfree,uart0_wait_rxcount,uart0_rx,uart0_tx
extern __reentrantb void uart0_irq(void) __interrupt(11) __naked;
#elif defined __CX51__ || defined __C51__
#elif defined __ICC8051__
#else
#error "Compiler unsupported"
#endif

extern __reentrantb uint8_t uart0_poll(void) __reentrant __naked;
extern __reentrantb uint8_t __xdata *uart0_txbufptr(uint8_t idx) __reentrant __naked;
extern __reentrantb uint8_t uart0_txfreelinear(void) __reentrant __naked;
extern __reentrantb uint8_t uart0_txidle(void) __reentrant __naked;
extern __reentrantb uint8_t uart0_txfree(void) __reentrant __naked;
extern __reentrantb const uint8_t __xdata *uart0_rxbufptr(uint8_t idx) __reentrant __naked;
extern __reentrantb uint8_t uart0_rxcountlinear(void) __reentrant __naked;
extern __reentrantb uint8_t uart0_rxcount(void) __reentrant __naked;
extern __reentrantb uint8_t uart0_txbuffersize(void) __reentrant __naked;
extern __reentrantb uint8_t uart0_rxbuffersize(void) __reentrant __naked;
extern __reentrantb uint8_t uart0_rxpeek(uint8_t idx) __reentrant __naked;
extern __reentrantb void uart0_txpoke(uint8_t idx, uint8_t ch) __reentrant __naked;
extern __reentrantb void uart0_txpokehex(uint8_t idx, uint8_t ch) __reentrant __naked;
extern __reentrantb void uart0_rxadvance(uint8_t idx) __reentrant __naked;
extern __reentrantb void uart0_txadvance(uint8_t idx) __reentrant __naked;

extern void uart0_init(uint8_t timernr, uint8_t wl, uint8_t stop);
extern void uart0_stop(void);
extern __reentrantb void uart0_wait_txdone(void) __reentrant;
extern __reentrantb void uart0_wait_txfree(uint8_t v) __reentrant;
extern __reentrantb void uart0_wait_rxcount(uint8_t v) __reentrant;
extern __reentrantb uint8_t uart0_rx(void) __reentrant;
extern __reentrantb void uart0_tx(uint8_t v) __reentrant;
extern __reentrantb void uart0_writestr(const char __generic *ch) __reentrant;
extern __reentrantb uint8_t uart0_writenum16(uint16_t val, uint8_t nrdig, uint8_t flags) __reentrant;
extern __reentrantb uint8_t uart0_writehex16(uint16_t val, uint8_t nrdig, uint8_t flags) __reentrant;
#if defined(SDCC)
extern __reentrantb uint8_t uart0_writenum32(uint32_t val, uint8_t nrdig, uint8_t flags) __reentrant;
extern __reentrantb uint8_t uart0_writehex32(uint32_t val, uint8_t nrdig, uint8_t flags) __reentrant;
#else
extern uint8_t uart0_writenum32(uint32_t val, uint8_t nrdig, uint8_t flags);
extern uint8_t uart0_writehex32(uint32_t val, uint8_t nrdig, uint8_t flags);
#endif

/* legacy */
extern __reentrantb void uart0_writehexu16(uint16_t val, uint8_t nrdig) __reentrant;
extern void uart0_writehexu32(uint32_t val, uint8_t nrdig);
extern __reentrantb void uart0_writeu16(uint16_t val, uint8_t nrdig) __reentrant;
extern void uart0_writeu32(uint32_t val, uint8_t nrdig);


#endif /* LIBMFUART0_H */
