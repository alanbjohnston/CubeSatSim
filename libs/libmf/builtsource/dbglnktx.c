#include "ax8052.h"

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
