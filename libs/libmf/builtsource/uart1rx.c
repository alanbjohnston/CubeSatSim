#include "ax8052.h"

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

__reentrantb void uart_wait_rxcount(uint8_t v) __reentrant
{
	uint8_t iesave = (IE & 0x80);
	for (;;) {
		EA = 0;
		if (uart_rxcount() >= v)
			break;
		if (!uart_poll())
			wtimer_standby();
		IE |= iesave;
	}
	IE |= iesave;
}

__reentrantb uint8_t uart_rx(void) __reentrant
{
	uint8_t x;
	uart_wait_rxcount(1);
	x = uart_rxpeek(0);
	uart_rxadvance(1);
	return x;
}
