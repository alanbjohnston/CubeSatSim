#include "ax8052f142.h"
#include "libmftypes.h"
#include "libmfradio.h"

__reentrantb void ax5042_rclk_disable(void) __reentrant
{
	uint8_t p, irqe;
	irqe = IE & 0x80;
	EA = 0;
	PORTR |= 0x02;
	AX5042_PINCFG1 = (AX5042_PINCFG1 & 0xF0) | 0x01;
	p = AX5042_PWRMODE;
	if ((p & 0x2F) == 0x00)
		AX5042_APEOVERRIDE = 0x80;
	IE |= irqe;
}
