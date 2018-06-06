#include "libmftypes.h"
#define RADIO 5043
#include "radiodefs.h"

__reentrantb uint8_t radio_wakeup_deepsleep_core(void) __reentrant
{
	uint8_t i = 3, j = 0;
	PORTR &= (uint8_t)~0x08;
	do {
		do {
			// precharge
			DIRR |= 0x08;
			DIRR &= (uint8_t)~0x08;
			PORTR &= (uint8_t)~1;
			if (PINR & 0x08) {
				i = 0;
				goto dswakeup;
			}
			PORTR |= 1;
		} while (--j);
	} while (--i);
	i = RADIO_ERR_WAKEUPTIMEOUT;
  dswakeup:
	PORTR |= 0x09;
	RADIOMUX = 0x47;
	return i;
}
