#include "ax8052.h"
#include "libmfadc.h"

SFRX(ADCCALTEMPGAIN0, 0x7038)   /* ADC Calibration Temperature Gain Low Byte */
SFRX(ADCCALTEMPGAIN1, 0x7039)   /* ADC Calibration Temperature Gain High Byte */
SFRX(ADCCALTEMPOFFS0, 0x703A)   /* ADC Calibration Temperature Offset Low Byte */
SFRX(ADCCALTEMPOFFS1, 0x703B)   /* ADC Calibration Temperature Offset High Byte */

#if defined(SDCC)

void adc_calibrate_temp(void)
{
	__asm
ar2 = 0x02
ar3 = 0x03
ar4 = 0x04
ar5 = 0x05
ar6 = 0x06
ar7 = 0x07
ar0 = 0x00
ar1 = 0x01
	clr	a
	push	_IE
	mov	_IE,a
	push	_EIE
	mov	_EIE,#0x40
	push	_E2IE
	mov	_E2IE,a
	mov	_ADCCONV,a
	mov	r0,a
	mov	r1,a
	mov	r2,a
	mov	r3,a
	mov	_ADCCLKSRC,#0x28	; 0.625MHz/16
	mov	_ADCCH0CONFIG,#0xf8
	mov	_ADCCH1CONFIG,#0xf9
	mov	_ADCCH2CONFIG,#0xff
	mov	_ADCCH3CONFIG,#0xff
	mov	dptr,#_ADCTUNE1
	mov	a,#0xf2
	movx	@dptr,a
	mov	dptr,#_ADCCH0VAL0
	movx	a,@dptr
	mov	b,#0x10
adccaltloop:
	mov	_ADCCONV,#0x01
adcwait1:
	mov	a,_PCON
	anl	a,#0x0C
	orl	a,#0x01
	mov	_PCON,a
	mov	a,_ADCCLKSRC
	jb	acc.7,adcwait1
	mov	dptr,#_ADCCH0VAL0
	;; channel 0
	movx	a,@dptr
	inc	dptr
	swap	a
	anl	a,#0x0f
	mov	r6,a
	movx	a,@dptr
	inc	dptr
	swap	a
	mov	r7,a
	anl	a,#0xf0
	orl	a,r6
	add	a,r0
	mov	r0,a
	mov	a,r7
	anl	a,#0x0f
	addc	a,r1
	mov	r1,a
	;; channel 1
	movx	a,@dptr
	inc	dptr
	swap	a
	anl	a,#0x0f
	mov	r6,a
	movx	a,@dptr
	inc	dptr
	swap	a
	mov	r7,a
	anl	a,#0xf0
	orl	a,r6
	add	a,r2
	mov	r2,a
	mov	a,r7
	anl	a,#0x0f
	addc	a,r3
	mov	r3,a
	djnz	b,adccaltloop
	;; switch off ADC
	mov	dptr,#_ADCTUNE1
	mov	a,#0x02
	movx	@dptr,a
	mov	_ADCCLKSRC,#0x07	; off
	pop	_E2IE
	pop	_EIE
	pop	_IE
	;; convert calibration constant to calibration value
caltconst0	= 0x13C08312	; 0.96*82.3*2^-6*2^8*2^14*2^6
caltconst1	= 0x0000A532	; 82.3*4.0145*2^8/2
;caltconst2	= 0x00014877	; (82.3*3.68+25.6)*2^8
caltconst2	= 0x00013F77	; (82.3*3.68+25.6-9)*2^8
	push	ar0
	push	ar1
	;; first compute offset
	mov	__mullong_PARM_2,r2
	mov	(__mullong_PARM_2+1),r3
	mov	a,r3
	rlc	a
	subb	a,acc
	mov	(__mullong_PARM_2+2),a
	mov	(__mullong_PARM_2+3),a
	mov	dptr,#caltconst1&0xffff
	mov	b,#caltconst1>>16
	mov	a,#caltconst1>>24
	lcall	__mullong
	mov	r1,a
	pop	acc
	pop	ar0
	push	ar0
	push	acc
	clr	c
	rrc	a
	mov	(__divslong_PARM_2 + 1),a
	mov	a,r0
	rrc	a
	mov	__divslong_PARM_2,a
	clr	a
	mov	(__divslong_PARM_2 + 2),a
	mov	(__divslong_PARM_2 + 3),a	
	mov	a,r1
	lcall	__divslong
	mov	r0,dph
	mov	a,dpl
	add	a,#caltconst2&0xff
	mov	dptr,#_ADCCALTEMPOFFS0
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	addc	a,#caltconst2>>8
	movx	@dptr,a
	;; second compute gain
	pop	(__divslong_PARM_2 + 1)
	pop	__divslong_PARM_2
	clr	a
	mov	(__divslong_PARM_2 + 2),a
	mov	(__divslong_PARM_2 + 3),a	
	mov	dptr,#caltconst0&0xffff
	mov	b,#caltconst0>>16
	mov	a,#caltconst0>>24
	lcall	__divslong
	mov	a,dpl
	mov	r0,dph
	mov	dptr,#_ADCCALTEMPGAIN0
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	__endasm;
}

#elif defined __CX51__ || defined __C51__

void adc_calibrate_temp(void)
{
#pragma asm
	using	0
	extrn	CODE (?C?LMUL)
	extrn	CODE (?C?SLDIV)
	clr	a
	push	IE
	mov	IE,a
	push	EIE
	mov	EIE,#0x40
	push	E2IE
	mov	E2IE,a
	mov	ADCCONV,a
	mov	r0,a
	mov	r1,a
	mov	r2,a
	mov	r3,a
	mov	ADCCLKSRC,#0x28	; 0.625MHz/16
	mov	ADCCH0CONFIG,#0xf8
	mov	ADCCH1CONFIG,#0xf9
	mov	ADCCH2CONFIG,#0xff
	mov	ADCCH3CONFIG,#0xff
	mov	dptr,#ADCTUNE1
	mov	a,#0xf2
	movx	@dptr,a
	mov	dptr,#ADCCH0VAL0
	movx	a,@dptr
	mov	b,#0x10
adccaltloop:
	mov	ADCCONV,#0x01
adcwait1:
	mov	a,PCON
	anl	a,#0x0C
	orl	a,#0x01
	mov	PCON,a
	mov	a,ADCCLKSRC
	jb	acc.7,adcwait1
	mov	dptr,#ADCCH0VAL0
	;; channel 0
	movx	a,@dptr
	inc	dptr
	swap	a
	anl	a,#0x0f
	mov	r6,a
	movx	a,@dptr
	inc	dptr
	swap	a
	mov	r7,a
	anl	a,#0xf0
	orl	a,r6
	add	a,r0
	mov	r0,a
	mov	a,r7
	anl	a,#0x0f
	addc	a,r1
	mov	r1,a
	;; channel 1
	movx	a,@dptr
	inc	dptr
	swap	a
	anl	a,#0x0f
	mov	r6,a
	movx	a,@dptr
	inc	dptr
	swap	a
	mov	r7,a
	anl	a,#0xf0
	orl	a,r6
	add	a,r2
	mov	r2,a
	mov	a,r7
	anl	a,#0x0f
	addc	a,r3
	mov	r3,a
	djnz	b,adccaltloop
	;; switch off ADC
	mov	dptr,#ADCTUNE1
	mov	a,#0x02
	movx	@dptr,a
	mov	ADCCLKSRC,#0x07	; off
	pop	E2IE
	pop	EIE
	pop	IE
	;; convert calibration constant to calibration value
caltconst0l	equ 0x8312	; 0.96*82.3*2^-6*2^8*2^14*2^6
caltconst0h	equ 0x13C0
caltconst1l	equ 0xA532	; 82.3*4.0145*2^8/2
caltconst1h	equ 0x0000
;caltconst2l	equ 0x4877	; (82.3*3.68+25.6)*2^8
;caltconst2h	equ 0x0001
caltconst2l	equ 0x3F77	; (82.3*3.68+25.6-9)*2^8
caltconst2h	equ 0x0001
	push	ar0
	push	ar1
	;; first compute offset
	mov	r7,ar2
	mov	a,r3
	mov	r6,a
	rlc	a
	subb	a,acc
	mov	r5,a
	mov	r4,a
	mov	r3,#caltconst1l&0xff
	mov	r2,#caltconst1l>>8
	mov	r1,#caltconst1h&0xff
	mov	r0,#caltconst1h>>8
	lcall	?C?LMUL
	pop	acc
	pop	ar3
	push	ar3
	push	acc
	clr	c
	rrc	a
	mov	r2,a
	mov	a,r3
	rrc	a
	mov	r3,a
	clr	a
	mov	r1,a
	mov	r0,a
	lcall	?C?SLDIV
	mov	a,r7
	add	a,#caltconst2l&0xff
	mov	dptr,#ADCCALTEMPOFFS0
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	addc	a,#caltconst2l>>8
	movx	@dptr,a
	;; second compute gain
	pop	ar2
	pop	ar3
	clr	a
	mov	r1,a
	mov	r0,a
	mov	r7,#caltconst0l&0xff
	mov	r6,#caltconst0l>>8
	mov	r5,#caltconst0h&0xff
	mov	r4,#caltconst0h>>8
	lcall	?C?SLDIV
	mov	a,r7
	mov	dptr,#ADCCALTEMPGAIN0
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
#pragma endasm
}

#else

void adc_calibrate_temp(void)
{
	static const int32_t caltconst0 = 0x13C08312; /* 0.96*82.3*2^-6*2^8*2^14*2^6 */
	static const int32_t caltconst1 = 0x0000A532; /* 82.3*4.0145*2^8/2 */
	/*static const uint16_t caltconst2 = 0x00014877; */ /* (82.3*3.68+25.6)*2^8 */
	static const uint16_t caltconst2 = 0x00013F77; /* (82.3*3.68+25.6-9)*2^8 */
	uint8_t __autodata iesave = IE, eiesave = EIE, e2iesave = E2IE, cnt = 0x10;
	uint16_t __autodata adcv0 = 0, adcv1 = 0, v;
	int32_t __autodata x;
	IE = 0;
	EIE = 0x40;
	E2IE = 0;
	ADCCONV = 0;
	ADCCLKSRC = 0x28; /* 0.625MHz/16 */
	ADCCH0CONFIG = 0xf8;
	ADCCH1CONFIG = 0xf9;
	ADCCH2CONFIG = 0xff;
	ADCCH3CONFIG = 0xff;
	ADCTUNE1 = 0xf2;
	ADCCH0VAL0;
	do {
		ADCCONV = 0x01;
		do {
			enter_standby();
		} while (ADCCLKSRC & 0x80);
		v = ADCCH0VAL1;
		v <<= 4;
		v |= ADCCH0VAL0 >> 4;
		adcv0 += v;
		v = ADCCH1VAL1;
		v <<= 4;
		v |= ADCCH1VAL0 >> 4;
		adcv1 += v;
	} while (--cnt);
	/* switch off ADC */
	ADCTUNE1 = 0x02;
	ADCCLKSRC = 0x07;
	E2IE = e2iesave;
	EIE = eiesave;
	IE = iesave;
	/* convert calibration constant to calibration value */
	/* first compute offset */
	v = (caltconst1 * (int16_t)adcv1) / (adcv0 >> 1);
	v += caltconst2;
	ADCCALTEMPOFFS0 = v;
	ADCCALTEMPOFFS1 = v >> 8;
	/* second compute gain */
	v = caltconst0 / adcv0;
	ADCCALTEMPGAIN0 = v;
	ADCCALTEMPGAIN1 = v >> 8;
}

#endif
