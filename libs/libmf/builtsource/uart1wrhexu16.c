#include "libmfuart1.h"
#define uart_txfree	   uart1_txfree
#define uart_rxcount	   uart1_rxcount
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

__reentrantb void uart_writehexu16(uint16_t val, uint8_t nrdig) __reentrant
{
	uint8_t nrdig1 = nrdig;
	uint8_t digit = nrdig1;
	uart_wait_txfree(nrdig);
       	while (digit) {
		--digit;
		uart_txpokehex(digit, val);
		val >>= 4;
	}
	uart_txadvance(nrdig1);
}
