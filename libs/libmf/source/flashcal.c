#include "ax8052.h"
#include "libmf.h"
#include "libmfflash.h"
#include "libmfcrc.h"
#include "libmfcalsector.h"

SFRX(REF,             0x7F16)   /* Reference Tuning */
SFRX(POWCTRL0,        0x7F10)   /* Power Control 0 */
SFRX(POWCTRL1,        0x7F11)   /* Power Control 1 */
SFRX(ADCCALG00GAIN0,  0x7030)   /* ADC Calibration Range 00 Gain Low Byte */
SFRX(ADCCALG00GAIN1,  0x7031)   /* ADC Calibration Range 00 Gain High Byte */
SFRX(ADCCALG01GAIN0,  0x7032)   /* ADC Calibration Range 01 Gain Low Byte */
SFRX(ADCCALG01GAIN1,  0x7033)   /* ADC Calibration Range 01 Gain High Byte */
SFRX(ADCCALG10GAIN0,  0x7034)   /* ADC Calibration Range 10 Gain Low Byte */
SFRX(ADCCALG10GAIN1,  0x7035)   /* ADC Calibration Range 10 Gain High Byte */
SFRX(ADCCALTEMPGAIN0, 0x7038)   /* ADC Calibration Temperature Gain Low Byte */
SFRX(ADCCALTEMPGAIN1, 0x7039)   /* ADC Calibration Temperature Gain High Byte */
SFRX(ADCCALTEMPOFFS0, 0x703A)   /* ADC Calibration Temperature Offset Low Byte */
SFRX(ADCCALTEMPOFFS1, 0x703B)   /* ADC Calibration Temperature Offset High Byte */

#if defined(SDCC)

uint8_t flash_apply_calibration(void)
{
	__asm
	;; REF = 0x04;
	mov	dptr,#_REF
	mov	a,#0x04
	movx	@dptr,a
	;; ADCTUNE1 = 0x06;
	mov	dptr,#_ADCTUNE1
	mov	a,#0x06
	movx	@dptr,a
	;; check signature
	mov	dptr,#_flash_calsector
	movx	a,@dptr
	xrl	a,#0x80
	jnz	00001$
	inc	dptr
	movx	a,@dptr
	xrl	a,#0xfe
	jnz	00001$
	inc	dptr
	movx	a,@dptr
	xrl	a,#'C'
	jnz	00001$
	inc	dptr
	movx	a,@dptr
	xrl	a,#'A'
	jnz	00001$
	inc	dptr
	movx	a,@dptr
	xrl	a,#'L'
	jnz	00001$
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	add	a,#-26
	jnc	00001$
	inc	dptr
	mov	r3,#0xff
	mov	r4,_IE
	clr	_EA
	mov	r5,_XPAGE
	mov	_XPAGE,dph
	mov	r0,dpl
	mov	dptr,#_crc_crc8ccitt_msbtable	
00002$:	movx	a,@r0
	inc	r0
	xrl	a,r3
	movc	a,@a+dptr
	mov	r3,a
	djnz	r2,00002$
	mov	_XPAGE,r5
	mov	_IE,r4
	xrl	a,#0xf3
	jz	00003$
	;; error exit
00001$:	mov	dptr,#_ADCCALG00GAIN0
	mov	a,#14429 & 0xff
	movx	@dptr,a
	inc	dptr
	mov	a,#14429 >> 8
	movx	@dptr,a
	inc	dptr
	mov	a,#15873 & 0xff
	movx	@dptr,a
	inc	dptr
	mov	a,#15873 >> 8
	movx	@dptr,a
	inc	dptr
	mov	a,#16603 & 0xff
	movx	@dptr,a
	inc	dptr
	mov	a,#16603 >> 8
	movx	@dptr,a
	mov	dptr,#_ADCCALTEMPGAIN0
	mov	a,#10407 & 0xff
	movx	@dptr,a
	inc	dptr
	mov	a,#10407 >> 8
	movx	@dptr,a
	inc	dptr
	mov	a,#5671 & 0xff
	movx	@dptr,a
	inc	dptr
	mov	a,#5671 >> 8
	movx	@dptr,a
	mov	dptr,#_FRCOSCFREQ0
	mov	a,#2114 & 0xff
	movx	@dptr,a
	inc	dptr
	mov	a,#2114 >> 8
	movx	@dptr,a
	mov	dptr,#_LPOSCCONFIG
	movx	a,@dptr
	mov	dptr,#_LPOSCFREQ0
	jnb	acc.6,00004$
	mov	a,#52779 & 0xff
	movx	@dptr,a
	inc	dptr
	mov	a,#52779 >> 8
	movx	@dptr,a
	mov	dptr,#0
	ret
00004$:	mov	a,#53443 & 0xff
	movx	@dptr,a
	inc	dptr
	mov	a,#53443 >> 8
	movx	@dptr,a
	mov	dptr,#0
	ret

	;; restore calibration
	;; POWCTRL0, POWCTRL1, REF
00003$:	mov	dptr,#(_flash_calsector + 0x001c)
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	mov	dptr,#_POWCTRL0
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	mov	dptr,#_REF
	mov	a,r2
	movx	@dptr,a
	;; ADCCALGxxGAINy
	mov	dptr,#(_flash_calsector + 0x000c)
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	mov	r5,a
	mov	dptr,#_ADCCALG00GAIN0
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	inc	dptr
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	inc	dptr
	mov	a,r4
	movx	@dptr,a
	inc	dptr
	mov	a,r5
	movx	@dptr,a
	;; ADCCALTEMPGAINx, ADCCALTEMPOFFSx
	mov	dptr,#(_flash_calsector + 0x0012)
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	a,r1
	add	a,#-20
	jc	00005$
	mov	a,r1
	rl	a
	rl	a
	anl	a,#0xfc
	mov	r1,a
	mov	a,r0
	rl	a
	rl	a
	mov	r0,a
	anl	a,#0x03
	orl	a,r1
	mov	r1,a
	mov	a,r0
	anl	a,#0xfc
	mov	r0,a
	mov	a,r3
	rl	a
	rl	a
	anl	a,#0xfc
	mov	r3,a
	mov	a,r2
	rl	a
	rl	a
	mov	r2,a
	anl	a,#0x03
	orl	a,r3
	mov	r3,a
	mov	a,r2
	anl	a,#0xfc
	mov	r2,a
00005$:	mov	dptr,#_ADCCALTEMPGAIN0
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	inc	dptr
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	;; FRCOSCFREQx
	mov	dptr,#(_flash_calsector + 0x0016)
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	mov	dptr,#_FRCOSCFREQ0
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	;; LPOSCFREQx
	mov	dptr,#_LPOSCCONFIG
	movx	a,@dptr
	mov	dptr,#(_flash_calsector + 0x0018)
	jnb	acc.6,00006$
	mov	dptr,#(_flash_calsector + 0x001a)
00006$:	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	mov	dptr,#_LPOSCFREQ0
	mov	a,r0
	movx	@dptr,a
	inc	dptr
	mov	a,r1
	movx	@dptr,a
	mov	dptr,#1
	__endasm;
}

#else

uint8_t flash_apply_calibration(void)
{
	REF = 0x04;
	ADCTUNE1 = 0x06;
	if (flash_calsector.id[0] != 0x80) {
	  err:
		ADCCALG00GAIN0   = 14429 & 0xff;
		ADCCALG00GAIN1   = 14429 >> 8;
		ADCCALG01GAIN0   = 15873 & 0xff;
		ADCCALG01GAIN1   = 15873 >> 8;
		ADCCALG10GAIN0   = 16603 & 0xff;
		ADCCALG10GAIN1   = 16603 >> 8;
		ADCCALTEMPGAIN0  = 10407 & 0xff;
		ADCCALTEMPGAIN1  = 10407 >> 8;
		ADCCALTEMPOFFS0  =  5671 & 0xff;
		ADCCALTEMPOFFS1  =  5671 >> 8;
		FRCOSCFREQ0      =  2114 & 0xff;
		FRCOSCFREQ1      =  2114 >> 8;
		if (LPOSCCONFIG & 0x40) {
			LPOSCFREQ0  = 52779 & 0xff;
			LPOSCFREQ1  = 52779 >> 8;
		} else {
			LPOSCFREQ0  = 53443 & 0xff;
			LPOSCFREQ1  = 53443 >> 8;
		}
		return 0;
	}
	if (flash_calsector.id[1] != 0xfe)
		goto err;
	if (flash_calsector.id[2] != (uint8_t)'C')
		goto err;
	if (flash_calsector.id[3] != (uint8_t)'A')
		goto err;
	if (flash_calsector.id[4] != (uint8_t)'L')
		goto err;
	if (flash_calsector.len < (uint8_t)26)
		goto err;
	if (crc_crc8ccitt_msb((&flash_calsector.len) + 1, flash_calsector.len, 0xff) != 0xf3)
		goto err;
	POWCTRL0 = flash_calsector.powctrl0;
	POWCTRL1 = flash_calsector.powctrl1;
	REF = flash_calsector.ref;
	ADCCALG00GAIN0 = flash_calsector.calg00gain[0];
	ADCCALG00GAIN1 = flash_calsector.calg00gain[1];
	ADCCALG01GAIN0 = flash_calsector.calg01gain[0];
	ADCCALG01GAIN1 = flash_calsector.calg01gain[1];
	ADCCALG10GAIN0 = flash_calsector.calg10gain[0];
	ADCCALG10GAIN1 = flash_calsector.calg10gain[1];
	{
		uint8_t __autodata g0, g1, o0, o1;
		g0 = flash_calsector.caltempgain[0];
		g1 = flash_calsector.caltempgain[1];
		o0 = flash_calsector.caltempoffs[0];
		o1 = flash_calsector.caltempoffs[1];
		if (g1 < 20) {
			ADCCALTEMPGAIN0 = g0 << 2;
			ADCCALTEMPGAIN1 = (g1 << 2) | (g0 >> 6);
			ADCCALTEMPOFFS0 = o0 << 2;
			ADCCALTEMPOFFS1 = (o1 << 2) | (o0 >> 6);
		} else {
			ADCCALTEMPGAIN0 = g0;
			ADCCALTEMPGAIN1 = g1;
			ADCCALTEMPOFFS0 = o0;
			ADCCALTEMPOFFS1 = o1;
		}
	}
	FRCOSCFREQ0 = flash_calsector.frcoscfreq[0];
	FRCOSCFREQ1 = flash_calsector.frcoscfreq[1];
	if (LPOSCCONFIG & 0x40) {
		LPOSCFREQ0 = flash_calsector.lposcfreq_fast[0];
		LPOSCFREQ1 = flash_calsector.lposcfreq_fast[1];
	} else {
		LPOSCFREQ0 = flash_calsector.lposcfreq[0];
		LPOSCFREQ1 = flash_calsector.lposcfreq[1];
	}
	return 1;
}

#endif
