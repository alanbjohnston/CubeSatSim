#include "ax8052.h"
#include "libmfosc.h"

/**
 * \brief turn off LPXOSC if accidentally enabled
 *
 */
__reentrantb void turn_off_lpxosc(void) __reentrant
{
	uint8_t iesave, portasave, dirasave, i;

	if (SILICONREV != SILICONREVISION_V1 && (MISCCTRL & 0x01))
		return;
	MISCCTRL |= 0x01;
	iesave = IE & 0x80;
	EA = 0;
	portasave = PORTA;
	dirasave = DIRA;
	PORTA_4 = 1;
	PORTA_5 = 0;
	DIRA |= 0x18;
	OSCFORCERUN &= (uint8_t)~0x08;
	i = 6;
	do {
		uint8_t j = 0x80;
		do {
			nop();
		} while (--j);
		PORTA ^= 0x18;
	} while (--i);
	DIRA = dirasave;
	PORTA = portasave;
	IE |= iesave;
}
