#include "libmftypes.h"
#include "libmfradio.h"

#define RADIO 5042

#include "radiodefs.h"

#if DEEPSLEEP

__reentrantb void radio_enter_deepsleep(void) __reentrant
{
	PORTR |= 0x09;
	// ensure last bit read before entering deep sleep is a zero;
	// this is held until after wakeup is complete; otherwise,
	// the wakeup protocol will not work
	RADIO_PWRMODE = PWRMODE_PWRDOWN;
	RADIO_PWRMODE = PWRMODE_DEEPSLEEP;
	RADIOMUX &= (uint8_t)~0x40;
	// turn off pull-up if MISO is driven low
	PORTR &= 0xF7 | PINR;
}

__reentrantb uint8_t radio_wakeup_deepsleep(void) __reentrant
{
	DIRR = 0x15;
	PORTR = 0xEB;
	RADIOMUX = 0x07;
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
	{
		uint8_t i = radio_wakeup_deepsleep_core();
		if (i)
			return i;
	}
	if (radio_probeirq())
		return RADIO_ERR_IRQ;
	return 0;
}

#endif
