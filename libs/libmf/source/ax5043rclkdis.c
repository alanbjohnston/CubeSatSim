#include "ax8052f143.h"
#include "libmftypes.h"
#include "libmfradio.h"

__reentrantb void ax5043_rclk_disable(void) __reentrant
{
	uint8_t p, irqe;
	irqe = IE & 0x80;
	EA = 0;
	PORTR |= 0x02;
	AX5043_PINFUNCSYSCLK = 0x01;
	p = AX5043_PWRMODE;
	if ((p & 0x0F) == 0x05)
		AX5043_PWRMODE = (p & 0xF0);
	IE |= irqe;
}
