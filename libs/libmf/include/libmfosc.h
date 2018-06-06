#ifndef LIBMFOSC_H
#define LIBMFOSC_H

#include "libmftypes.h"

extern __reentrantb void turn_off_xosc(void) __reentrant;
extern __reentrantb void turn_off_lpxosc(void) __reentrant;

#if defined SDCC
#pragma callee_saves setup_xosc,setup_lpxosc
#endif
extern __reentrantb void setup_xosc(void) __reentrant;
extern __reentrantb void setup_lpxosc(void) __reentrant;

extern __reentrantb uint8_t setup_osc_calibration(uint32_t reffreq, uint8_t refosc) __reentrant;

/*
 * Clock Sources
 */
#define CLKSRC_FRCOSC   0
#define CLKSRC_LPOSC    1
#define CLKSRC_XOSC     2
#define CLKSRC_LPXOSC   3
#define CLKSRC_RSYSCLK  4
#define CLKSRC_TCLK     5
#define CLKSRC_SYSCLK   6
#define CLKSRC_OFF      7

#if defined(__ICC8051__)
#define __osc_set_frcoscref(x) do { FRCOSCREF0 = (x); FRCOSCREF1 = (x) >> 8; } while(0)
#define __osc_set_lposcref(x)  do { LPOSCREF0 = (x); LPOSCREF1 = (x) >> 8; } while(0)
#define __osc_set_filt()       do { FRCOSCKFILT0 = 0x00; FRCOSCKFILT1 = 0x40; LPOSCKFILT0 = 0x00; LPOSCKFILT1 = 0x40; } while (0)
#else
#define __osc_set_frcoscref(x) do { FRCOSCREF = (x); } while(0)
#define __osc_set_lposcref(x)  do { LPOSCREF = (x); } while(0)
#define __osc_set_filt()       do { FRCOSCKFILT = 0x4000; LPOSCKFILT = 0x4000; } while (0)
#endif

#define setup_osc_calibration_const(reffreq,refosc)				\
do {										\
	uint8_t ret = 0;							\
	uint8_t lposccfg;							\
	uint8_t frcosccfg;							\
	if ((reffreq) >= 32768000UL) {						\
		frcosccfg = 0;							\
		ret |= 1;							\
	} else if ((reffreq) >= 16384000UL) {					\
		frcosccfg = 0x78 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 8) / ((reffreq) >> 7));	\
	} else if ((reffreq) >= 8192000UL) {					\
		frcosccfg = 0x70 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 8) / ((reffreq) >> 6));	\
	} else if ((reffreq) >= 4096000UL) {					\
		frcosccfg = 0x68 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 8) / ((reffreq) >> 5));	\
	} else if ((reffreq) >= 2048000UL) {					\
		frcosccfg = 0x60 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 8) / ((reffreq) >> 4));	\
	} else if ((reffreq) >= 1024000UL) {					\
		frcosccfg = 0x58 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 8) / ((reffreq) >> 3));	\
	} else if ((reffreq) >= 512000UL) {					\
		frcosccfg = 0x50 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 8) / ((reffreq) >> 2));	\
	} else if ((reffreq) >= 256000UL) {					\
		frcosccfg = 0x48 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 8) / ((reffreq) >> 1));	\
	} else if ((reffreq) >= 128000UL) {					\
		frcosccfg = 0x40 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 8) / (reffreq));		\
	} else if ((reffreq) >= 64000UL) {					\
		frcosccfg = 0x38 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 7) / (reffreq));		\
	} else if ((reffreq) >= 32000UL) {					\
		frcosccfg = 0x30 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 6) / (reffreq));		\
	} else if ((reffreq) >= 16000UL) {					\
		frcosccfg = 0x28 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 5) / (reffreq));		\
	} else if ((reffreq) >= 8000UL) {					\
		frcosccfg = 0x20 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 4) / (reffreq));		\
	} else if ((reffreq) >= 4000UL) {					\
		frcosccfg = 0x18 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 3) / (reffreq));		\
	} else if ((reffreq) >= 2000UL) {					\
		frcosccfg = 0x10 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 2) / (reffreq));		\
	} else if ((reffreq) >= 1000UL) {					\
		frcosccfg = 0x08 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 1) / (reffreq));		\
	} else if ((reffreq) >= 500UL) {					\
		frcosccfg = 0x00 | ((refosc) & 0x07);				\
		__osc_set_frcoscref((10000000UL << 0) / (reffreq));		\
	} else {								\
		frcosccfg = 0;							\
		ret |= 1;							\
	}									\
	if ((reffreq) >= 2013265920UL || (reffreq) < 7864320UL) {		\
		lposccfg = 0x08 | CLKSRC_FRCOSC;				\
		__osc_set_lposcref(31250);					\
	} else if ((reffreq) >= 1006632960UL) {					\
		lposccfg = 0x38 | ((refosc) & 0x07);				\
		__osc_set_lposcref((reffreq) / 40960UL);			\
	} else if ((reffreq) >= 503316480UL) {					\
		lposccfg = 0x30 | ((refosc) & 0x07);				\
		__osc_set_lposcref((reffreq) / 20480UL);			\
	} else if ((reffreq) >= 251658240UL) {					\
		lposccfg = 0x28 | ((refosc) & 0x07);				\
		__osc_set_lposcref((reffreq) / 10240UL);			\
	} else if ((reffreq) >= 125829120UL) {					\
		lposccfg = 0x20 | ((refosc) & 0x07);				\
		__osc_set_lposcref((reffreq) / 5120UL);				\
	} else if ((reffreq) >= 62914560UL) {					\
		lposccfg = 0x18 | ((refosc) & 0x07);				\
		__osc_set_lposcref((reffreq) / 2560UL);				\
	} else if ((reffreq) >= 31457280UL) {					\
		lposccfg = 0x10 | ((refosc) & 0x07);				\
		__osc_set_lposcref((reffreq) / 1280UL);				\
	} else if ((reffreq) >= 15728640UL) {					\
		lposccfg = 0x08 | ((refosc) & 0x07);				\
		__osc_set_lposcref((reffreq) / 640UL);				\
	} else {								\
		lposccfg = 0x00 | ((refosc) & 0x07);				\
		__osc_set_lposcref((reffreq) / 320UL);				\
	}									\
	if (!ret) {								\
		switch (refosc) {						\
		case CLKSRC_XOSC:						\
			setup_xosc();						\
			OSCFORCERUN |= 0x04;					\
			break;							\
										\
		case CLKSRC_LPXOSC:						\
			setup_lpxosc();						\
			OSCFORCERUN |= 0x08;					\
			break;							\
										\
		case CLKSRC_RSYSCLK:						\
			break;							\
										\
		default:							\
			ret = 2;						\
			break;							\
		}								\
		if (!ret) {							\
			__osc_set_filt();					\
			FRCOSCCONFIG = frcosccfg;				\
			LPOSCCONFIG = lposccfg;					\
		}								\
	}									\
} while (0)

#endif /* LIBMFOSC_H */
