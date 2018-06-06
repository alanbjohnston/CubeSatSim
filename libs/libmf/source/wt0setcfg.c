#include "ax8052.h"
#include "wtimer.h"

__reentrantb void wtimer0_setconfig(uint8_t cfg) __reentrant
{
	if (!((WTCFGA ^ cfg) & 0x3F))
		return;
	cfg &= 0x3F;
	WTCFGA |= 0x04;
	WTCFGA = 0x0F;
	WTCFGA &= cfg | 0xFC;
	WTCFGA = cfg;
}
