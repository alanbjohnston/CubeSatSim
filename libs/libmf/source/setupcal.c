#include "ax8052.h"
#include "libmfosc.h"

/**
 * \brief set up oscillator calibration
 *
 */

static __reentrantb uint8_t compute_frcosccfg(uint32_t reffreq) __reentrant
{
	uint8_t x = 0;
	while (reffreq >= 500) {
		if (x & 0x80)
			return x;
		x += 0x08;
		reffreq >>= 1;
	}
	if (reffreq < 250)
		return 0x80;
	return x;
}

static __reentrantb void compute_frcoscref(uint32_t reffreq, uint8_t calcfg) __reentrant
{
	static const uint32_t __code refs[9] = {
		10000000, 10000000 << 1, 10000000 << 2, 10000000 << 3, 10000000 << 4, 10000000 << 5, 10000000 << 6, 10000000 << 7, 10000000 << 8
	};
	calcfg >>= 3;
	if (calcfg > 8) {
		calcfg -= 8;
		reffreq >>= calcfg;
		calcfg = 8;
	}
#ifdef SDCC
	FRCOSCREF = refs[calcfg] / reffreq;
#else
	{
		uint16_t x = refs[calcfg] / reffreq;
		FRCOSCREF0 = x;
		FRCOSCREF1 = x >> 8;
	}
#endif
}

static __reentrantb uint8_t compute_lposccfg(uint32_t reffreq) __reentrant
{
	uint8_t x = 0;
	reffreq /= 320;
	while (reffreq >= 0xC000) {
		if (x & 0x40)
			return x;
		x += 0x08;
		reffreq >>= 1;
	}
	if (reffreq < 0x6000)
		return 0x40;
#ifdef SDCC
	LPOSCREF = reffreq;
#else
	LPOSCREF0 = reffreq;
	LPOSCREF1 = reffreq >> 8;
#endif
	return x;
}

__reentrantb uint8_t setup_osc_calibration(uint32_t reffreq, uint8_t refosc) __reentrant
{
	uint8_t refosc1 = refosc;
	uint8_t lposccfg;
	uint8_t frcosccfg;

	frcosccfg = compute_frcosccfg(reffreq);
	if (frcosccfg & 0x80)
		return 1;
	compute_frcoscref(reffreq, frcosccfg);
	lposccfg = compute_lposccfg(reffreq);
	if (lposccfg & 0x40) {
#ifdef SDCC
		LPOSCREF = 0x7A12;
#else
		LPOSCREF0 = 0x12;
		LPOSCREF1 = 0x7A;
#endif
		lposccfg = 0x80;
	} else {
		lposccfg |= 0x07;
	}
	lposccfg |= LPOSCCONFIG & 0x40;
	switch (refosc1) {
	case CLKSRC_XOSC:
		setup_xosc();
		OSCFORCERUN |= 0x04;
		frcosccfg |= 0x02;
		lposccfg &= 0xF8 | 0x02;
		break;

	case CLKSRC_LPXOSC:
		setup_lpxosc();
		OSCFORCERUN |= 0x08;
		frcosccfg |= 0x03;
		lposccfg &= 0xF8 | 0x03;
		break;

	case CLKSRC_RSYSCLK:
		frcosccfg |= 0x04;
		lposccfg &= 0xF8 | 0x04;
		break;

	default:
		return 2;
	}
#ifdef SDCC
	FRCOSCKFILT = 0x4000;
	LPOSCKFILT = 0x4000;
#else
	FRCOSCKFILT0 = 0x00;
	FRCOSCKFILT1 = 0x40;
	LPOSCKFILT0 = 0x00;
	LPOSCKFILT1 = 0x40;
#endif
	FRCOSCCONFIG = frcosccfg;
	LPOSCCONFIG = lposccfg;
	return 0;
}
