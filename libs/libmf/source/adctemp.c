#include "ax8052.h"
#include "libmfadc.h"

#if defined(SDCC) 

__reentrantb int16_t adc_measure_temperature(void) __reentrant
{
	__asm
	clr	a
	mov	b,_IE
	mov	_IE,a
	mov	r0,_EIE
	mov	_EIE,a
	mov	r1,_E2IE
	mov	_E2IE,a

	mov	dptr,#_ADCTUNE0
	mov	a,#0x01
	movx	@dptr,a
	inc	dptr
	mov	a,#0x06
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	mov	r2,a
	clr	a
	movx	@dptr,a

	mov	r3,_ADCCLKSRC
	mov	_ADCCLKSRC,#0x30

	mov	r4,_ADCCH0CONFIG
	mov	_ADCCH0CONFIG,#0xd8
	mov	r5,_ADCCH1CONFIG
	mov	_ADCCH1CONFIG,#0xd8
	mov	r6,_ADCCH2CONFIG
	mov	_ADCCH2CONFIG,#0xd8
	mov	r7,_ADCCH3CONFIG
	mov	_ADCCH3CONFIG,#0xd8

	mov	_ADCCONV,#0x01
	setb	_EIE_6
00000$:	mov	a,_ADCCONV
	jb	acc.7,00001$
	mov	a,_PCON
	anl	a,#0x0c
	orl	a,#0x01
	mov	_PCON,a
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	sjmp	00000$
00001$:	;; restore registers
	mov	_ADCCH0CONFIG,r4
	mov	_ADCCH1CONFIG,r5
	mov	_ADCCH2CONFIG,r6
	mov	_ADCCH3CONFIG,r7

	mov	_ADCCLKSRC,r3

	mov	dptr,#_ADCTUNE2
	mov	a,r2
	movx	@dptr,a

	mov	_EIE,r0
	mov	_E2IE,r1

	mov	dptr,#_ADCCH1VAL0
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
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a

	mov	_IE,b

	;; compute median

	;; A=R5:R4 - R3:R2
	mov	a,r4
	clr	c
	subb	a,r2
	mov	a,r5
	subb	a,r3
	jnb	acc.7,00010$
	;; if R3:R2 > R5:R4, swap
	mov	a,r2
	xch	a,r4
	mov	r2,a
	mov	a,r3
	xch	a,r5
	mov	r3,a
00010$:
	;; A=R7:R6 - R5:R4
	mov	a,r6
	clr	c
	subb	a,r4
	mov	a,r7
	subb	a,r5
	jnb	acc.7,00011$
	;; if R5:R4 > R7:R6, swap
	mov	a,r4
	xch	a,r6
	mov	r4,a
	mov	a,r5
	xch	a,r7
	mov	r5,a
00011$:
	;; A=R5:R4 - R3:R2
	mov	a,r4
	clr	c
	subb	a,r2
	mov	a,r5
	subb	a,r3
	jnb	acc.7,00012$
	;; if R3:R2 > R5:R4, swap
	mov	a,r2
	xch	a,r4
	mov	r2,a
	mov	a,r3
	xch	a,r5
	mov	r3,a
00012$:

	;; return is in R5:R4
	mov	dpl,r4
	mov	dph,r5
	__endasm;
}

#else

// returns 256*temperature [deg]
// median of ADC channels 1,2 and 3 is used.
// Note: MCU is put in standby mode with all interrupts disabled during conversion!

__reentrantb int16_t adc_measure_temperature(void) __reentrant
{
	int16_t t1, t2, t3;
	{
		uint8_t ie_save = IE;
		uint8_t eie_save = EIE;
		uint8_t e2ie_save = E2IE;
		uint8_t tune2save, clksrcsave, ch0cfgsave, ch1cfgsave, ch2cfgsave, ch3cfgsave;
		IE = 0;
		EIE = 0;
		E2IE = 0;

		ADCTUNE0 = 0x01;
		ADCTUNE1 = 0x06;
		tune2save = ADCTUNE2;
		ADCTUNE2 = 0x00;
	
		clksrcsave = ADCCLKSRC;
		ADCCLKSRC = 0x30;

		// configure all channels for TEMP
		ch0cfgsave = ADCCH0CONFIG;
		ADCCH0CONFIG = 0xD8;
		ch1cfgsave = ADCCH1CONFIG;
		ADCCH1CONFIG = 0xD8;
		ch2cfgsave = ADCCH2CONFIG;
		ADCCH2CONFIG = 0xD8;
		ch3cfgsave = ADCCH3CONFIG;
		ADCCH3CONFIG = 0xD8;

		ADCCONV = 0x01;
		EIE_6 = 1;
		for (;;) {
			if (ADCCONV & 0x80)
				break;
			enter_standby();
		}

		t1 = ADCCH1VAL1;
		t1 <<= 8;
		t1 |= ADCCH1VAL0;

		t2 = ADCCH2VAL1;
		t2 <<= 8;
		t2 |= ADCCH2VAL0;

		t3 = ADCCH3VAL1;
		t3 <<= 8;
		t3 |= ADCCH3VAL0;

		ADCCH0CONFIG = ch0cfgsave;
		ADCCH1CONFIG = ch1cfgsave;
		ADCCH2CONFIG = ch2cfgsave;
		ADCCH3CONFIG = ch3cfgsave;
		ADCCLKSRC = clksrcsave;
		ADCTUNE2 = tune2save;

		E2IE = e2ie_save;
		EIE = eie_save;
		IE = ie_save;

	}
	// compute median
	if (t1 > t2) {
		int16_t x = t1;
		t1 = t2;
		t2 = x;
	}
	if (t2 > t3) {
		int16_t x = t2;
		t2 = t3;
		t3 = x;
	}
	if (t1 > t2) {
		int16_t x = t1;
		t1 = t2;
		t2 = x;
	}
	return t2;
}

#endif
