#ifndef LIBMFDBGLINK_H
#define LIBMFDBGLINK_H

#include "libmftypes.h"

/*
 * DebugLink
 */
#if defined SDCC

#define DBGLINK_DEFINE_RXBUFFER(sz)                             \
static void dbglink_define_rxbuffer(void) __naked               \
{                                                               \
	__asm                                                   \
	.area XSEG    (XDATA)                                   \
	.globl  _dbglink_rxbuffer                               \
_dbglink_rxbuffer:                                              \
	.ds	(sz)                                            \
                                                                \
	.area HOME     (CODE)                                   \
	.area DBGLINK0 (CODE)                                   \
	.area DBGLINK1 (CODE)                                   \
	.area DBGLINK2 (CODE)                                   \
	.area DBGLINK3 (CODE)                                   \
	.area DBGLINK4 (CODE)                                   \
	.area DBGLINK5 (CODE)                                   \
                                                                \
	.area DBGLINK2 (CODE)                                   \
	.db	256-(sz)                                        \
	.db	257-(sz)                                        \
                                                                \
	.area DBGLINK5 (CODE)                                   \
	.db	(sz)                                            \
                                                                \
	.area CSEG    (CODE)                                    \
	__endasm;                                               \
}

#define DBGLINK_DEFINE_TXBUFFER(sz)                             \
static void dbglink_define_txbuffer(void) __naked               \
{                                                               \
	__asm                                                   \
	.area XSEG    (XDATA)                                   \
	.globl  _dbglink_txbuffer                               \
_dbglink_txbuffer:                                              \
	.ds	(sz)                                            \
                                                                \
	.area HOME     (CODE)                                   \
	.area DBGLINK0 (CODE)                                   \
	.area DBGLINK1 (CODE)                                   \
	.area DBGLINK2 (CODE)                                   \
	.area DBGLINK3 (CODE)                                   \
	.area DBGLINK4 (CODE)                                   \
	.area DBGLINK5 (CODE)                                   \
                                                                \
	.area DBGLINK1 (CODE)                                   \
	.db	256-(sz)                                        \
	.db	257-(sz)                                        \
                                                                \
	.area DBGLINK4 (CODE)                                   \
	.db	(sz)                                            \
                                                                \
	.area CSEG    (CODE)                                    \
	__endasm;                                               \
}

#elif defined __CX51__ || defined __C51__

#define DBGLINK_DEFINE_RXBUFFER(sz)                             \
uint8_t __xdata dbglink_rxbuffer[sz];                           \
const uint8_t __code dbglink_rxbuffer_size[] = { sz, 256-(sz), 257-(sz) };

#define DBGLINK_DEFINE_TXBUFFER(sz)                             \
uint8_t __xdata dbglink_txbuffer[sz];                           \
const uint8_t __code dbglink_txbuffer_size[] = { sz, 256-(sz), 257-(sz) };

#else

#define DBGLINK_DEFINE_RXBUFFER(sz)                             \
uint8_t __xdata dbglink_rxbuffer[sz];                           \
const uint8_t __code dbglink_rxbuffer_size[] = { sz };

#define DBGLINK_DEFINE_TXBUFFER(sz)                             \
uint8_t __xdata dbglink_txbuffer[sz];                           \
const uint8_t __code dbglink_txbuffer_size[] = { sz };

#endif

#if defined SDCC
#pragma callee_saves dbglink_txbufptr,dbglink_txfreelinear,dbglink_rxbufptr,dbglink_rxcountlinear
#pragma callee_saves dbglink_txfree,dbglink_rxcount,dbglink_wait_txfree,dbglink_wait_rxcount
#pragma callee_saves dbglink_txbuffersize,dbglink_rxbuffersize,dbglink_rxpeek,dbglink_txpoke,dbglink_txpokehex
#pragma callee_saves dbglink_init,dbglink_poll,dbglink_wait_txfree,dbglink_wait_rxcount,dbglink_rx,dbglink_tx
extern void dbglink_irq(void) __interrupt(21) __naked;
#elif defined __CX51__ || defined __C51__
#elif defined __ICC8051__
#else
#error "Compiler unsupported"
#endif

extern __reentrantb uint8_t dbglink_poll(void) __reentrant __naked;
extern __reentrantb uint8_t __xdata *dbglink_txbufptr(uint8_t idx) __reentrant __naked;
extern __reentrantb uint8_t dbglink_txfreelinear(void) __reentrant __naked;
extern __reentrantb uint8_t dbglink_txidle(void) __reentrant __naked;
extern __reentrantb uint8_t dbglink_txfree(void) __reentrant __naked;
extern __reentrantb const uint8_t __xdata *dbglink_rxbufptr(uint8_t idx) __reentrant __naked;
extern __reentrantb uint8_t dbglink_rxcountlinear(void) __reentrant __naked;
extern __reentrantb uint8_t dbglink_rxcount(void) __reentrant __naked;
extern __reentrantb uint8_t dbglink_txbuffersize(void) __reentrant __naked;
extern __reentrantb uint8_t dbglink_rxbuffersize(void) __reentrant __naked;
extern __reentrantb uint8_t dbglink_rxpeek(uint8_t idx) __reentrant __naked;
extern __reentrantb void dbglink_txpoke(uint8_t idx, uint8_t ch) __reentrant __naked;
extern __reentrantb void dbglink_txpokehex(uint8_t idx, uint8_t ch) __reentrant __naked;
extern __reentrantb void dbglink_rxadvance(uint8_t idx) __reentrant __naked;
extern __reentrantb void dbglink_txadvance(uint8_t idx) __reentrant __naked;

extern __reentrantb void dbglink_init(void) __reentrant;
extern __reentrantb void dbglink_wait_txdone(void) __reentrant;
extern __reentrantb void dbglink_wait_txfree(uint8_t v) __reentrant;
extern __reentrantb void dbglink_wait_rxcount(uint8_t v) __reentrant;
extern __reentrantb uint8_t dbglink_rx(void) __reentrant;
extern __reentrantb void dbglink_tx(uint8_t v) __reentrant;
extern __reentrantb void dbglink_writestr(const char __generic *ch) __reentrant;
extern __reentrantb uint8_t dbglink_writenum16(uint16_t val, uint8_t nrdig, uint8_t flags) __reentrant;
extern __reentrantb uint8_t dbglink_writehex16(uint16_t val, uint8_t nrdig, uint8_t flags) __reentrant;
#if defined(SDCC)
extern __reentrantb uint8_t dbglink_writenum32(uint32_t val, uint8_t nrdig, uint8_t flags) __reentrant;
extern __reentrantb uint8_t dbglink_writehex32(uint32_t val, uint8_t nrdig, uint8_t flags) __reentrant;
#else
extern uint8_t dbglink_writenum32(uint32_t val, uint8_t nrdig, uint8_t flags);
extern uint8_t dbglink_writehex32(uint32_t val, uint8_t nrdig, uint8_t flags);
#endif

/* legacy */
extern __reentrantb void dbglink_writehexu16(uint16_t val, uint8_t nrdig) __reentrant;
extern void dbglink_writehexu32(uint32_t val, uint8_t nrdig);
extern __reentrantb void dbglink_writeu16(uint16_t val, uint8_t nrdig) __reentrant;
extern void dbglink_writeu32(uint32_t val, uint8_t nrdig);

#endif /* LIBMFDBGLINK_H */
