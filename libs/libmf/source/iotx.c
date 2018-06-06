#include "ax8052.h"

#if UART == 0
#include "libmfuart0.h"
#define uart_poll	   uart0_poll
#define uart_txidle	   uart0_txidle
#define uart_txfree	   uart0_txfree
#define uart_rxcount	   uart0_rxcount
#define uart_wait_txdone   uart0_wait_txdone
#define uart_wait_txfree   uart0_wait_txfree
#define uart_wait_rxcount  uart0_wait_rxcount
#define uart_rxpeek	   uart0_rxpeek
#define uart_txpokehex	   uart0_txpokehex
#define uart_txpoke	   uart0_txpoke
#define uart_rxadvance	   uart0_rxadvance
#define uart_txadvance	   uart0_txadvance
#define uart_rx		   uart0_rx
#define uart_tx            uart0_tx
#define uart_writestr      uart0_writestr
#define uart_writehexu16   uart0_writehexu16
#define uart_writehexu32   uart0_writehexu32
#define uart_writeu16      uart0_writeu16
#define uart_writeu32      uart0_writeu32
#elif UART == 1
#include "libmfuart1.h"
#define uart_poll	   uart1_poll
#define uart_txidle	   uart1_txidle
#define uart_txfree	   uart1_txfree
#define uart_rxcount	   uart1_rxcount
#define uart_wait_txdone   uart1_wait_txdone
#define uart_wait_txfree   uart1_wait_txfree
#define uart_wait_rxcount  uart1_wait_rxcount
#define uart_rxpeek	   uart1_rxpeek
#define uart_txpokehex	   uart1_txpokehex
#define uart_txpoke	   uart1_txpoke
#define uart_rxadvance	   uart1_rxadvance
#define uart_txadvance	   uart1_txadvance
#define uart_rx		   uart1_rx
#define uart_tx            uart1_tx
#define uart_writestr      uart1_writestr
#define uart_writehexu16   uart1_writehexu16
#define uart_writehexu32   uart1_writehexu32
#define uart_writeu16      uart1_writeu16
#define uart_writeu32      uart1_writeu32
#elif UART == 2
#include "libmfdbglink.h"
#define uart_poll	   dbglink_poll
#define uart_txidle	   dbglink_txidle
#define uart_txfree	   dbglink_txfree
#define uart_rxcount	   dbglink_rxcount
#define uart_wait_txdone   dbglink_wait_txdone
#define uart_wait_txfree   dbglink_wait_txfree
#define uart_wait_rxcount  dbglink_wait_rxcount
#define uart_rxpeek	   dbglink_rxpeek
#define uart_txpokehex	   dbglink_txpokehex
#define uart_txpoke	   dbglink_txpoke
#define uart_rxadvance	   dbglink_rxadvance
#define uart_txadvance	   dbglink_txadvance
#define uart_rx		   dbglink_rx
#define uart_tx            dbglink_tx
#define uart_writestr      dbglink_writestr
#define uart_writehexu16   dbglink_writehexu16
#define uart_writehexu32   dbglink_writehexu32
#define uart_writeu16      dbglink_writeu16
#define uart_writeu32      dbglink_writeu32
#elif UART == 4
#include "libaxlcd2.h"
#define uart_poll	   lcd2_poll
#define uart_txidle	   lcd2_txidle
#define uart_txfree	   lcd2_txfree
#define uart_wait_txdone   lcd2_wait_txdone
#define uart_wait_txfree   lcd2_wait_txfree
#define uart_txpokehex	   lcd2_txpokehex
#define uart_txpoke	   lcd2_txpoke
#define uart_txadvance	   lcd2_txadvance
#define uart_tx            lcd2_tx
#define uart_writestr      lcd2_writestr
#define uart_writehexu16   lcd2_writehexu16
#define uart_writehexu32   lcd2_writehexu32
#define uart_writeu16      lcd2_writeu16
#define uart_writeu32      lcd2_writeu32
#else
#error "UART not set"
#endif

__reentrantb void uart_wait_txfree(uint8_t v) __reentrant
{
	uint8_t iesave = (IE & 0x80);
	for (;;) {
		EA = 0;
		if (uart_txfree() >= v)
			break;
		if (!uart_poll())
			wtimer_standby();
		IE |= iesave;
	}
	IE |= iesave;
}

__reentrantb void uart_wait_txdone(void) __reentrant
{
	uint8_t iesave = (IE & 0x80);
	for (;;) {
		EA = 0;
		if (uart_txidle())
			break;
		if (!uart_poll())
			wtimer_standby();
		IE |= iesave;
	}
	IE |= iesave;
}

__reentrantb void uart_tx(uint8_t v) __reentrant
{
	uart_wait_txfree(1);
	uart_txpoke(0, v);
	uart_txadvance(1);
}
