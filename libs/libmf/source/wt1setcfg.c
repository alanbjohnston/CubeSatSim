#include "ax8052.h"
#include "wtimer.h"

__reentrantb void wtimer1_setconfig(uint8_t cfg) __reentrant
{
	if (!((WTCFGB ^ cfg) & 0x3F))
		return;
	cfg &= 0x3F;
	WTCFGB |= 0x04;
	WTCFGB = 0x0F;
	WTCFGB &= cfg | 0xFC;
	WTCFGB = cfg;
}
