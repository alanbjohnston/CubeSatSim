#include "ax8052f151.h"
#include "libmftypes.h"
#include "libmfradio.h"

__reentrantb void ax5051_rclk_disable(void) __reentrant
{
	uint8_t p, irqe;
	irqe = IE & 0x80;
	EA = 0;
	PORTR |= 0x02;
	AX5051_PINCFG1 = (AX5051_PINCFG1 & 0xF0) | 0x01;
	p = AX5051_PWRMODE;
	if ((p & 0x0F) == 0x05)
		AX5051_PWRMODE = (p & 0xF0);
	IE |= irqe;
}
