#include "ax8052.h"
#include "libmfosc.h"

/**
 * \brief set up XOSC
 *
 */
__reentrantb void setup_xosc(void) __reentrant
{
	ANALOGA |= 0x03;
	DIRA &= (uint8_t)~0x03;
	PORTA &= (uint8_t)~0x03;
	MISCCTRL &= (uint8_t)~0x02;
}
