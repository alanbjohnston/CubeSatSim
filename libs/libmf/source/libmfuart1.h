#ifndef LIBMFUART1_H
#define LIBMFUART1_H

#include "libmftypes.h"

/*
 * UART 1
 */
#if defined SDCC

#define UART1_DEFINE_RXBUFFER(sz)                               \
static void uart1_define_rxbuffer(void) __naked                 \
{                                                               \
	__asm                                                   \
	.area XSEG    (XDATA)                                   \
	.globl  _uart1_rxbuffer                                 \
_uart1_rxbuffer:                                                \
	.ds	sz                                              \
                                                                \
	.area HOME    (CODE)                                    \
	.area UART1S0 (CODE)                                    \
	.area UART1S1 (CODE)                                    \
	.area UART1S2 (CODE)                                    \
	.area UART1S3 (CODE)                                    \
	.area UART1S4 (CODE)                                    \
	.area UART1S5 (CODE)                                    \
                                                                \
	.area UART1S2 (CODE)                                    \
	.db	256-sz                                          \
	.db	257-sz                                          \
                                                                \
	.area UART1S5 (CODE)                                    \
	.db	sz                                              \
                                                                \
	.area CSEG    (CODE)                                    \
	__endasm;                                               \
}

#define UART1_DEFINE_TXBUFFER(sz)                               \
static void uart1_define_txbuffer(void) __naked                 \
{                                                               \
	__asm                                                   \
	.area XSEG    (XDATA)                                   \
	.globl  _uart1_txbuffer                                 \
_uart1_txbuffer:                                                \
	.ds	sz                                              \
                                                                \
	.area HOME    (CODE)                                    \
	.area UART1S0 (CODE)                                    \
	.area UART1S1 (CODE)                                    \
	.area UART1S2 (CODE)                                    \
	.area UART1S3 (CODE)                                    \
	.area UART1S4 (CODE)                                    \
	.area UART1S5 (CODE)                                    \
                                                                \
	.area UART1S1 (CODE)                                    \
	.db	256-sz                                          \
	.db	257-sz                                          \
                                                                \
	.area UART1S4 (CODE)                                    \
	.db	sz                                              \
                                                                \
	.area CSEG    (CODE)                                    \
	__endasm;                                               \
}

#elif defined __CX51__ || defined __C51__

#define UART1_DEFINE_RXBUFFER(sz)                               \
uint8_t __xdata uart1_rxbuffer[sz];                             \
const uint8_t __code uart1_rxbuffer_size[] = { sz, 256-(sz), 257-(sz) };

#define UART1_DEFINE_TXBUFFER(sz)                               \
uint8_t __xdata uart1_txbuffer[sz];                             \
const uint8_t __code uart1_txbuffer_size[] = { sz, 256-(sz), 257-(sz) };

#else

#define UART1_DEFINE_RXBUFFER(sz)                               \
uint8_t __xdata uart1_rxbuffer[sz];                             \
const uint8_t __code uart1_rxbuffer_size[] = { sz };

#define UART1_DEFINE_TXBUFFER(sz)                               \
uint8_t __xdata uart1_txbuffer[sz];                             \
const uint8_t __code uart1_txbuffer_size[] = { sz };

#endif

#if defined SDCC
#pragma callee_saves uart1_txbufptr,uart1_txfreelinear,uart1_rxbufptr,uart1_rxcountlinear
#pragma callee_saves uart1_txfree,uart1_rxcount,uart1_wait_txfree,uart1_wait_rxcount
#pragma callee_saves uart1_txbuffersize,uart1_rxbuffersize,uart1_rxpeek,uart1_txpoke,uart1_txpokehex
#pragma callee_saves uart1_init,uart1_poll,uart1_wait_txfree,uart1_wait_rxcount,uart1_rx,uart1_tx
extern __reentrantb void uart1_irq(void) __interrupt(12) __naked;
#elif defined __CX51__ || defined __C51__
#elif defined __ICC8051__
#else
#error "Compiler unsupported"
#endif

extern __reentrantb uint8_t uart1_poll(void) __reentrant __naked;
extern __reentrantb uint8_t __xdata *uart1_txbufptr(uint8_t idx) __reentrant __naked;
extern __reentrantb uint8_t uart1_txfreelinear(void) __reentrant __naked;
extern __reentrantb uint8_t uart1_txidle(void) __reentrant __naked;
extern __reentrantb uint8_t uart1_txfree(void) __reentrant __naked;
extern __reentrantb const uint8_t __xdata *uart1_rxbufptr(uint8_t idx) __reentrant __naked;
extern __reentrantb uint8_t uart1_rxcountlinear(void) __reentrant __naked;
extern __reentrantb uint8_t uart1_rxcount(void) __reentrant __naked;
extern __reentrantb uint8_t uart1_txbuffersize(void) __reentrant __naked;
extern __reentrantb uint8_t uart1_rxbuffersize(void) __reentrant __naked;
extern __reentrantb uint8_t uart1_rxpeek(uint8_t idx) __reentrant __naked;
extern __reentrantb void uart1_txpoke(uint8_t idx, uint8_t ch) __reentrant __naked;
extern __reentrantb void uart1_txpokehex(uint8_t idx, uint8_t ch) __reentrant __naked;
extern __reentrantb void uart1_rxadvance(uint8_t idx) __reentrant __naked;
extern __reentrantb void uart1_txadvance(uint8_t idx) __reentrant __naked;

extern void uart1_init(uint8_t timernr, uint8_t wl, uint8_t stop);
extern void uart1_stop(void);
extern __reentrantb void uart1_wait_txdone(void) __reentrant;
extern __reentrantb void uart1_wait_txfree(uint8_t v) __reentrant;
extern __reentrantb void uart1_wait_rxcount(uint8_t v) __reentrant;
extern __reentrantb uint8_t uart1_rx(void) __reentrant;
extern __reentrantb void uart1_tx(uint8_t v) __reentrant;
extern __reentrantb void uart1_writestr(const char __generic *ch) __reentrant;
extern __reentrantb uint8_t uart1_writenum16(uint16_t val, uint8_t nrdig, uint8_t flags) __reentrant;
extern __reentrantb uint8_t uart1_writehex16(uint16_t val, uint8_t nrdig, uint8_t flags) __reentrant;
#if defined(SDCC)
extern __reentrantb uint8_t uart1_writenum32(uint32_t val, uint8_t nrdig, uint8_t flags) __reentrant;
extern __reentrantb uint8_t uart1_writehex32(uint32_t val, uint8_t nrdig, uint8_t flags) __reentrant;
#else
extern uint8_t uart1_writenum32(uint32_t val, uint8_t nrdig, uint8_t flags);
extern uint8_t uart1_writehex32(uint32_t val, uint8_t nrdig, uint8_t flags);
#endif

/* legacy */
extern __reentrantb void uart1_writehexu16(uint16_t val, uint8_t nrdig) __reentrant;
extern void uart1_writehexu32(uint32_t val, uint8_t nrdig);
extern  __reentrantb void uart1_writeu16(uint16_t val, uint8_t nrdig) __reentrant;
extern void uart1_writeu32(uint32_t val, uint8_t nrdig);


#endif /* LIBMFUART1_H */
