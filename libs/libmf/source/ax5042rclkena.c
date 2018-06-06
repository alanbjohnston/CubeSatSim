#include "ax8052f142.h"
#include "libmftypes.h"
#include "libmfradio.h"

__reentrantb void ax5042_rclk_enable(uint8_t div) __reentrant
{
	uint8_t p, irqe;
	if (div >= 11)
		div = 11;
	div += 4;
	irqe = IE & 0x80;
	EA = 0;
	AX5042_PINCFG1 = (AX5042_PINCFG1 & 0xF0) | div;
	PORTR &= (uint8_t)~0x02;
	p = AX5042_PWRMODE;
	if (!(p & 0x0F))
		AX5042_APEOVERRIDE = 0x00;
	IE |= irqe;
}
