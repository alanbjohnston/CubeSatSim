#include "libmfuart0.h"
#define uart_txfree	   uart0_txfree
#define uart_rxcount	   uart0_rxcount
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

void uart_writeu32(uint32_t val, uint8_t nrdig)
{
	uint8_t __autodata nrdig1 = nrdig;
	uint8_t __autodata digit = nrdig1;
	uart_wait_txfree(nrdig1);
	while (digit) {
		uint8_t __autodata v1 = val;
		val /= 10;
		v1 -= 10 * (uint8_t)val;
		--digit;
		uart_txpoke(digit, '0' + v1);
	}
	uart_txadvance(nrdig1);
}
