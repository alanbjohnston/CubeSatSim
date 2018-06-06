#include "ax8052.h"
#include "libmfosc.h"

/**
 * \brief set up LPXOSC
 *
 */
__reentrantb void setup_lpxosc(void) __reentrant
{
	ANALOGA |= 0x18;
	DIRA &= (uint8_t)~0x18;
	PORTA &= (uint8_t)~0x18;
	MISCCTRL &= (uint8_t)~0x01;
}
