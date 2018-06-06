#include "libmftypes.h"
#include "libmfradio.h"

#if RADIO == 5031
#define RADIO 5031
#elif RADIO == 5042
#define RADIO 5042
#elif RADIO == 5043
#define RADIO 5043
#elif RADIO == 5051
#define RADIO 5051
#endif

#include "radiodefs.h"

__reentrantb void radio_comminit(void) __reentrant
{
	DIRR = 0x15;
	PORTR = 0xEB;
	RADIOMUX = 0x47;
	RADIOACC = RACC;
#if defined SDCC
	RADIOFDATAADDR = FDATA;
	RADIOFSTATADDR = FSTAT;
#else
	RADIOFDATAADDR0 = (FDATA) & 0xFF;
	RADIOFDATAADDR1 = (FDATA) >> 8;
	RADIOFSTATADDR0 = (FSTAT) & 0xFF;
	RADIOFSTATADDR1 = (FSTAT) >> 8;
#endif
	GPIOENABLE = 1;
}
