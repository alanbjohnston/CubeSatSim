#include "ax8052f143.h"
#include "libmftypes.h"
#include "libmfradio.h"

__reentrantb void ax5043_rclk_enable(uint8_t div) __reentrant
{
	uint8_t p, irqe;
	if (div >= 10)
		div = 10;
	div += 4;
	irqe = IE & 0x80;
	EA = 0;
	AX5043_PINFUNCSYSCLK = div;
	PORTR &= (uint8_t)~0x02;
	p = AX5043_PWRMODE;
	if (!(p & 0x0F))
		AX5043_PWRMODE = (p & 0xF0) | 0x05;
	IE |= irqe;
}
