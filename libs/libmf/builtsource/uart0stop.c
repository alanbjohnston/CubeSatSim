#include "ax8052.h"
#include "libmfuart.h"

#include "libmfuart0.h"
#define UARTS0               UART0S0
#define UARTS1               UART0S1
#define UARTS2               UART0S2
#define UARTS3               UART0S3
#define UARTS4               UART0S4
#define UARTS5               UART0S5
#define USHREG               U0SHREG
#define UMODE                U0MODE
#define UCTRL                U0CTRL
#define USTATUS              U0STATUS
#define _USHREG              _U0SHREG
#define _UMODE               _U0MODE
#define _UCTRL               _U0CTRL
#define _USTATUS             _U0STATUS
#define IRQENA               EIE_4
#define uart_irq_nr          11
#define uart_vector_addr     0x5B
#define uart_init            uart0_init
#define uart_stop            uart0_stop
#define uart_iocore          uart0_iocore
#define _uart_iocore         _uart0_iocore
#define uart_irq             uart0_irq
#define uart_poll            uart0_poll
#define uart_rxbufptr        uart0_rxbufptr
#define _uart_rxbufptr       _uart0_rxbufptr
#define uart_txbufptr        uart0_txbufptr
#define _uart_txbufptr       _uart0_txbufptr
#define uart_txfreelinear    uart0_txfreelinear
#define _uart_txidle	     _uart0_txidle
#define uart_txidle	     uart0_txidle
#define uart_txfree	     uart0_txfree
#define uart_rxcountlinear   uart0_rxcountlinear
#define uart_rxcount	     uart0_rxcount
#define uart_txbuffersize    uart0_txbuffersize
#define uart_rxbuffersize    uart0_rxbuffersize
#define uart_wait_txfree     uart0_wait_txfree
#define uart_wait_rxcount    uart0_wait_rxcount
#define uart_rxpeek	     uart0_rxpeek
#define uart_txpokehex	     uart0_txpokehex
#define uart_txpoke	     uart0_txpoke
#define uart_rxadvance	     uart0_rxadvance
#define uart_txadvance	     uart0_txadvance
#define uart_rx		     uart0_rx
#define uart_tx              uart0_tx
#define _uart_txpoke	     _uart0_txpoke
#define uart_rxbuffer        uart0_rxbuffer
#define _uart_rxbuffer       _uart0_rxbuffer
#define uart_txbuffer        uart0_txbuffer
#define _uart_txbuffer       _uart0_txbuffer
#define _uart_buffer_size    _uart0_buffer_size
#define _uart_buffer_negsize _uart0_buffer_negsize
#define uart_rxbuffer_size   uart0_rxbuffer_size
#define _uart_rxbuffer_size  _uart0_rxbuffer_size
#define uart_txbuffer_size   uart0_txbuffer_size
#define _uart_txbuffer_size  _uart0_txbuffer_size

static volatile uint8_t __data fiforxwr;
static volatile uint8_t __data fiforxrd;
static volatile uint8_t __data fifotxwr;
static volatile uint8_t __data fifotxrd;

extern uint8_t __xdata uart_rxbuffer[];
extern uint8_t __xdata uart_txbuffer[];

#if !defined(SDCC)
extern const uint8_t __code uart_rxbuffer_size[];
extern const uint8_t __code uart_txbuffer_size[];
#endif

void uart_stop(void)
{
	IRQENA = 0;
	UMODE = 0;
	UCTRL = 0;
}
