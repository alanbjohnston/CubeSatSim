#include "ax8052.h"
#include "libmfosc.h"

/**
 * \brief turn off XOSC if accidentally enabled
 *
 */
__reentrantb void turn_off_xosc(void) __reentrant
{
	uint8_t iesave, portasave, dirasave, xtalreadysave, i;

	if (SILICONREV != SILICONREVISION_V1 && (MISCCTRL & 0x02))
		return;
	MISCCTRL |= 0x02;
	iesave = IE & 0x80;
	EA = 0;
	portasave = PORTA;
	dirasave = DIRA;
	xtalreadysave = XTALREADY;
	XTALREADY = 0x00;
	PORTA_0 = 1;
	PORTA_1 = 0;
	DIRA |= 0x03;
	OSCFORCERUN &= (uint8_t)~0x04;
	i = 6;
	do {
		PORTA ^= 0x03;
	} while (--i);
	DIRA = dirasave;
	PORTA = portasave;
	XTALREADY = xtalreadysave;
	IE |= iesave;
}
