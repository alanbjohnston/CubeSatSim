#include "libmftypes.h"
#include "libmfradio.h"

#define RADIO 5043

#include "radiodefs.h"

__reentrantb uint8_t radio_reset(void) __reentrant
{
	uint8_t i;
	// Initialize Interface
	DIRR = 0x15;
	PORTR = 0xEB;
#if DEEPSLEEP
	RADIOMUX = 0x07;
#else
	RADIOMUX = 0x47;
#endif
	RADIOACC = RACC;
	GPIOENABLE = 1;
#if defined SDCC
	RADIOFDATAADDR = FDATA;
	RADIOFSTATADDR = FSTAT;
#else
	RADIOFDATAADDR0 = (FDATA) & 0xFF;
	RADIOFDATAADDR1 = (FDATA) >> 8;
	RADIOFSTATADDR0 = (FSTAT) & 0xFF;
	RADIOFSTATADDR1 = (FSTAT) >> 8;
#endif
#if DEEPSLEEP
	// Ensure Device is not in Deep Sleep
	radio_wakeup_deepsleep_core();
#endif
	// Reset Device
	RADIO_PWRMODE = 0x80;
	RADIO_PWRMODE = PWRMODE_PWRDOWN;
	// Wait some time for regulator startup
#if defined(VREGDELAY) && VREGDELAY > 0
	delay(VREGDELAY);
#endif
	// Check Scratch
	i = RADIO_SILICONREVISION;
	i = RADIO_SILICONREVISION;
#ifdef SILICONREV2
	if (i != SILICONREV1 && i != SILICONREV2)
		return RADIO_ERR_REVISION;
#else
	if (i != SILICONREV1)
		return RADIO_ERR_REVISION;
#endif
	RADIO_SCRATCH = 0x55;
	if (RADIO_SCRATCH != 0x55)
		return RADIO_ERR_COMM;
	RADIO_SCRATCH = 0xAA;
	if (RADIO_SCRATCH != 0xAA)
		return RADIO_ERR_COMM;
	// Initialize Radio Interface Registers
	if (radio_probeirq())
		return RADIO_ERR_IRQ;
	return RADIO_OK;
}


SFRX(RADIODRV,        0x7045)

__reentrantb uint8_t ax5043_probeirq(void) __reentrant
{
	uint8_t p = 0x60;
	uint8_t iesave = IE;
	IE_4 = 0;
	PORTR &= 0xEB;
	PORTR |= 0x2B;
	AX5043_PINFUNCIRQ = 0x01; /* IRQ Line 1 */
	p &= PINR;
	AX5043_PINFUNCIRQ = 0x00; /* IRQ Line 0 */
	p &= (uint8_t)~PINR;
	AX5043_PINFUNCIRQ = 0x03;
	switch (p) {
	case 0x20: /* IRQ on PR5 */
		RADIOMUX &= (uint8_t)~0x08;
		break;

	case 0x40: /* IRQ on PR6 */
		RADIOMUX |= 0x08;
		break;

	default:
		/* Error */
		AX5043_PINFUNCIRQ = 0x02; /* Disable IRQ line */
		IE = iesave;
		return 1;
	}
	PORTR &= (uint8_t)~p; /* disable pullup */
	IE = iesave;
	return 0;
}

