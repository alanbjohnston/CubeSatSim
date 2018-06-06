#include "ax8052f131.h"
#include "libmftypes.h"
#include "libmfradio.h"

__reentrantb void ax5031_rclk_disable(void) __reentrant
{
	uint8_t p, irqe;
	irqe = IE & 0x80;
	EA = 0;
	PORTR |= 0x02;
	AX5031_PINCFG1 = (AX5031_PINCFG1 & 0xF0) | 0x01;
	p = AX5031_PWRMODE;
	if ((p & 0x0F) == 0x05)
		AX5031_PWRMODE = (p & 0xF0);
	IE |= irqe;
}
