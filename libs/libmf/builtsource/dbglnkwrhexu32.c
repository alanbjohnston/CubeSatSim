#include "libmfdbglink.h"
#define uart_txfree	   dbglink_txfree
#define uart_rxcount	   dbglink_rxcount
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

void uart_writehexu32(uint32_t val, uint8_t nrdig)
{
	uint8_t __autodata nrdig1 = nrdig;
	uint8_t __autodata digit = nrdig1;
	uart_wait_txfree(nrdig1);
	while (digit) {
		--digit;
		uart_txpokehex(digit, val);
		val >>= 4;
	}
	uart_txadvance(nrdig1);
}
