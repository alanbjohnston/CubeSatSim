#include "ax8052.h"
#include "libmfadc.h"

SFRX(ADCCALG00GAIN0,  0x7030)   /* ADC Calibration Range 00 Gain Low Byte */
SFRX(ADCCALG00GAIN1,  0x7031)   /* ADC Calibration Range 00 Gain High Byte */
SFRX(ADCCALG01GAIN0,  0x7032)   /* ADC Calibration Range 01 Gain Low Byte */
SFRX(ADCCALG01GAIN1,  0x7033)   /* ADC Calibration Range 01 Gain High Byte */
SFRX(ADCCALG10GAIN0,  0x7034)   /* ADC Calibration Range 10 Gain Low Byte */
SFRX(ADCCALG10GAIN1,  0x7035)   /* ADC Calibration Range 10 Gain High Byte */

#if defined(SDCC)

void adc_calibrate_gain(void)
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
	mov	r4,a
	mov	r5,a
	mov	_ADCCLKSRC,#0x28	; 0.625MHz/16
	mov	_ADCCH0CONFIG,#0xe0
	mov	_ADCCH1CONFIG,#0xe8
	mov	_ADCCH2CONFIG,#0xf0
	mov	_ADCCH3CONFIG,#0xff
	mov	dptr,#_ADCTUNE1
	mov	a,#0xf2
	movx	@dptr,a
	mov	dptr,#_ADCCH0VAL0
	movx	a,@dptr
	mov	b,#0x10
adccalloop:
	mov	_ADCCONV,#0x01
adcwait0:
	mov	a,_PCON
	anl	a,#0x0C
	orl	a,#0x01
	mov	_PCON,a
	mov	a,_ADCCLKSRC
	jb	acc.7,adcwait0
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
	;; channel 2
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
	add	a,r4
	mov	r4,a
	mov	a,r7
	anl	a,#0x0f
	addc	a,r5
	mov	r5,a
	djnz	b,adccalloop
	;; switch off ADC
	mov	dptr,#_ADCTUNE1
	mov	a,#0x02
	movx	@dptr,a
	mov	_ADCCLKSRC,#0x07	; off
	pop	_E2IE
	pop	_EIE
	pop	_IE
	;; convert calibration constant to calibration value
calconst0	= 0x0F5C28F6 ; 0.48*2^29
calconst1	= 0x1C28F5C3 ; 0.88*2^29
calconst2	= 0x1999999A ; 0.8*2^29
	push	ar4
	push	ar5
	push	ar2
	push	ar3
	mov	__divslong_PARM_2,r0
	mov	(__divslong_PARM_2 + 1),r1
	clr	a
	mov	(__divslong_PARM_2 + 2),a
	mov	(__divslong_PARM_2 + 3),a
	mov	dptr,#calconst0&0xffff
	mov	b,#calconst0>>16
	mov	a,#calconst0>>24
	lcall	__divslong
	mov	a,dpl
	mov	r0,dph
	mov	dptr,#_ADCCALG00GAIN0
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	pop	(__divslong_PARM_2 + 1)
	pop	__divslong_PARM_2
	clr	a
	mov	(__divslong_PARM_2 + 2),a
	mov	(__divslong_PARM_2 + 3),a
	mov	dptr,#calconst1&0xffff
	mov	b,#calconst1>>16
	mov	a,#calconst1>>24
	lcall	__divslong
	mov	a,dpl
	mov	r0,dph
	mov	dptr,#_ADCCALG01GAIN0
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	pop	(__divslong_PARM_2 + 1)
	pop	__divslong_PARM_2
	clr	a
	mov	(__divslong_PARM_2 + 2),a
	mov	(__divslong_PARM_2 + 3),a
	mov	dptr,#calconst2&0xffff
	mov	b,#calconst2>>16
	mov	a,#calconst2>>24
	lcall	__divslong
	mov	a,dpl
	mov	r0,dph
	mov	dptr,#_ADCCALG10GAIN0
	movx	@dptr,a
	inc	dptr
	mov	a,r0
	movx	@dptr,a
	__endasm;
}

#elif defined __CX51__ || defined __C51__

void adc_calibrate_gain(void)
{
#pragma asm
	using	0
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
	mov	r4,a
	mov	r5,a
	mov	ADCCLKSRC,#0x28	; 0.625MHz/16
	mov	ADCCH0CONFIG,#0xe0
	mov	ADCCH1CONFIG,#0xe8
	mov	ADCCH2CONFIG,#0xf0
	mov	ADCCH3CONFIG,#0xff
	mov	dptr,#ADCTUNE1
	mov	a,#0xf2
	movx	@dptr,a
	mov	dptr,#ADCCH0VAL0
	movx	a,@dptr
	mov	b,#0x10
adccalloop:
	mov	ADCCONV,#0x01
adcwait0:
	mov	a,PCON
	anl	a,#0x0C
	orl	a,#0x01
	mov	PCON,a
	mov	a,ADCCLKSRC
	jb	acc.7,adcwait0
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
	;; channel 2
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
	add	a,r4
	mov	r4,a
	mov	a,r7
	anl	a,#0x0f
	addc	a,r5
	mov	r5,a
	djnz	b,adccalloop
	;; switch off ADC
	mov	dptr,#ADCTUNE1
	mov	a,#0x02
	movx	@dptr,a
	mov	ADCCLKSRC,#0x07	; off
	pop	E2IE
	pop	EIE
	pop	IE
	;; convert calibration constant to calibration value
calconst0l	equ 0x28F6 ; 0.48*2^29
calconst0h	equ 0x0F5C
calconst1l	equ 0xF5C3 ; 0.88*2^29
calconst1h	equ 0x1C28
calconst2l	equ 0x999A ; 0.8*2^29
calconst2h	equ 0x1999
	push	ar4
	push	ar5
	push	ar2
	push	ar3
	mov	r3,ar0
	mov	r2,ar1
	clr	a
	mov	r1,a
	mov	r0,a
	mov	r7,#calconst0l&0xff
	mov	r6,#calconst0l>>8
	mov	r5,#calconst0h&0xff
	mov	r4,#calconst0h>>8
	lcall	?C?SLDIV
	mov	dptr,#ADCCALG00GAIN0
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
	pop	ar2
	pop	ar3
	clr	a
	mov	r1,a
	mov	r0,a
	mov	r7,#calconst1l&0xff
	mov	r6,#calconst1l>>8
	mov	r5,#calconst1h&0xff
	mov	r4,#calconst1h>>8
	lcall	?C?SLDIV
	mov	dptr,#ADCCALG01GAIN0
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
	pop	ar2
	pop	ar3
	clr	a
	mov	r1,a
	mov	r0,a
	mov	r7,#calconst2l&0xff
	mov	r6,#calconst2l>>8
	mov	r5,#calconst2h&0xff
	mov	r4,#calconst2h>>8
	lcall	?C?SLDIV
	mov	dptr,#ADCCALG10GAIN0
	mov	a,r7
	movx	@dptr,a
	inc	dptr
	mov	a,r6
	movx	@dptr,a
#pragma endasm
}

#else

#if defined __ICC8051__
// 6.4: Internal error: [EbkCodeNode::SetNextSpan]: Jump size optimization will not terminate!
#pragma optimize=low
#endif

void adc_calibrate_gain(void)
{
	static const int32_t calconst0 = 0x0F5C28F6; /* 0.48*2^29 */
	static const int32_t calconst1 = 0x1C28F5C3; /* 0.88*2^29 */
	static const int32_t calconst2 = 0x1999999A; /* 0.8*2^29 */
	uint8_t __autodata iesave = IE, eiesave = EIE, e2iesave = E2IE, cnt = 0x10;
	uint16_t __autodata adcv0 = 0, adcv1 = 0, adcv2 = 0, v;
	IE = 0;
	EIE = 0x40;
	E2IE = 0;
	ADCCONV = 0;
	ADCCLKSRC = 0x28; /* 0.625MHz/16 */
	ADCCH0CONFIG = 0xe0;
	ADCCH1CONFIG = 0xe8;
	ADCCH2CONFIG = 0xf0;
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
		v = ADCCH2VAL1;
		v <<= 4;
		v |= ADCCH2VAL0 >> 4;
		adcv2 += v;
	} while (--cnt);
	/* switch off ADC */
	ADCTUNE1 = 0x02;
	ADCCLKSRC = 0x07;
	E2IE = e2iesave;
	EIE = eiesave;
	IE = iesave;
	/* convert calibration constant to calibration value */
	v = calconst0 / adcv0;
	ADCCALG00GAIN0 = v;
	ADCCALG00GAIN1 = v >> 8;
	v = calconst1 / adcv1;
	ADCCALG01GAIN0 = v;
	ADCCALG01GAIN1 = v >> 8;
	v = calconst2 / adcv2;
	ADCCALG10GAIN0 = v;
	ADCCALG10GAIN1 = v >> 8;
}

#endif

#if 0

uint32_t mulx(uint8_t a, uint8_t b, int32_t c, int32_t d)
{
	return c * d;
}


uint16_t divx(uint8_t a, uint8_t b, int32_t c, int16_t d)
{
	return c / d;
}

#endif
