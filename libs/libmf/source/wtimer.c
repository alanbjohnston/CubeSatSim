#include "ax8052.h"
#include "wtimer.h"

#define WTIMER0_MARGIN       0x1000
#define WTIMER1_MARGIN       0x1000
#define WTIMER_LPXOSC_SLEEP  8

struct wtimer_state __xdata wtimer_state[2];
struct wtimer_callback __xdata * __xdata wtimer_pending;

#if defined(SDCC)

static void dummy0(void) __naked
{
	__asm;	
	.area HOME   (CODE)
	__endasm;
}

void wtimer_irq(void) __interrupt(1)
{
	uint8_t __autodata dpssave = DPS;
	uint8_t __autodata s = WTSTAT;
	DPS = 0;
	if (s & 0x01) {
		wtimer0_update();
		wtimer0_schedq();
		s |= WTSTAT;
	}
	if (s & 0x20) {
		wtimer1_update();
		wtimer1_schedq();
		WTSTAT;
	}
	DPS = dpssave;
}

static void dummy1(void) __naked
{
	__asm;	
	.area CSEG   (CODE)
	__endasm;
}

#elif defined __CX51__ || defined __C51__

void wtimer_irq(void) interrupt 1
{
	uint8_t __autodata dpssave = DPS;
	uint8_t __autodata s = WTSTAT;
	DPS = 0;
	if (s & 0x01) {
		wtimer0_update();
		wtimer0_schedq();
		s |= WTSTAT;
	}
	if (s & 0x20) {
		wtimer1_update();
		wtimer1_schedq();
		WTSTAT;
	}
	DPS = dpssave;
}

#elif defined __ICC8051__

#pragma vector=0x0b
__interrupt void wtimer_irq(void)
{
	uint8_t __autodata dpssave = DPS;
	uint8_t __autodata s = WTSTAT;
	DPS = 0;
	if (s & 0x01) {
		wtimer0_update();
		wtimer0_schedq();
		s |= WTSTAT;
	}
	if (s & 0x20) {
		wtimer1_update();
		wtimer1_schedq();
		WTSTAT;
	}
	DPS = dpssave;
}

#else

#error "Compiler unsupported"

#endif

static __reentrantb void wtimer_doinit(uint8_t wakeup) __reentrant
{
	IE_1 = 0;
	wtimer_pending = WTIMER_NULLCB;
	WTIRQEN = 0x21;
	if (wakeup) {
		if (SILICONREV == 0x8E && !(DBGLNKSTAT & 0x10))
			wtimer_state[0].time.ref = 0;
		wtimer0_update();
	} else {
		wtimer_state[0].time.ref = WTCNTA0;
		wtimer_state[0].time.ref |= ((uint16_t)WTCNTR1) << 8;
		wtimer_state[0].time.cur = 0;
		wtimer_state[0].queue = WTIMER_NULLDESC;
	}
	wtimer_state[1].time.ref = WTCNTB0;
	wtimer_state[1].time.ref |= ((uint16_t)WTCNTR1) << 8;
	wtimer_state[1].time.cur = 0;
	wtimer_state[1].queue = WTIMER_NULLDESC;
	wtimer0_schedq();
	wtimer1_schedq();
	IE_1 = 1;
}

__reentrantb void wtimer_init(void) __reentrant
{
	wtimer_doinit(PCON & 0x40);
}

__reentrantb void wtimer_init_deepsleep(void) __reentrant
{
	wtimer_doinit(0);
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer_addcb_core(struct wtimer_callback __xdata *desc) __reentrant
{
	struct wtimer_callback __xdata *d = WTIMER_CBPTR(wtimer_pending);
	for (;;) {
		struct wtimer_callback __xdata *dn = d->next;
		if (dn == WTIMER_NULLCB)
			break;
		d = dn;
	}
	d->next = (struct wtimer_callback __xdata *)desc;
	desc->next = WTIMER_NULLCB;
}

#if defined(WTIMER_USEASM) && defined(SDCC)

static void dummy2(void) __naked
{
	__asm;	
	.area HOME   (CODE)
	__endasm;
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer0_schedq(void) __reentrant
{
	__asm
	;; check wtimer_state[0].queue != WTIMER_NULL
	mov	dptr,#(_wtimer_state + 0x0006)
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	cjne	r6,#(WTIMER_NULL & 0xFF),00000$
	cjne	r7,#(WTIMER_NULL >> 8),00000$
00001$:
	;; WTEVTA = wtimer_state[0].time.ref + (0x10000-WTIMER0_MARGIN)
	mov	dptr,#(_wtimer_state + 0x0004)
	movx	a,@dptr
	add	a,#((0x10000-WTIMER0_MARGIN) & 0xFF)
	mov	_WTEVTA0,a
	inc	dptr
	movx	a,@dptr
	addc	a,#((0x10000-WTIMER0_MARGIN) >> 8)
	mov	_WTEVTA1,a
	ret
00000$:
	;; R5:R4:R3:R2 = wtimer_state[0].time.cur - wtimer_state[0].queue->time;
	mov	dpl,r6
	mov	dph,r7
	inc	dptr
	inc	dptr
	inc	dptr
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
	mov	dptr,#(_wtimer_state + 0x0000)
	movx	a,@dptr
	clr	c
	subb	a,r2
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	subb	a,r3
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	subb	a,r4
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	subb	a,r5
	mov	r5,a
	jb	acc.7,00002$
	;; case R5:R4:R3:R2 >= 0
	;; wtimer_state[0].queue = wtimer_state[0].queue->next
	mov	dpl,r6
	mov	dph,r7
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#(_wtimer_state + 0x0006)
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	;; wtimer_addcb_core(old wtimer_state[0].queue)
	mov	dpl,r6
	mov	dph,r7
	lcall	_wtimer_addcb_core
	sjmp	_wtimer0_schedq
00002$:	
	;; check R5:R4:R3:R2 < -(0x10000-WTIMER0_MARGIN)
	;; equivalent: (R5:R4:R3:R2 + (0x10000-WTIMER0_MARGIN)) < 0
	mov	a,r2
	add	a,#((0x10000-WTIMER0_MARGIN) & 0xFF)
	mov	a,r3
	addc	a,#((0x10000-WTIMER0_MARGIN) >> 8)
	clr	a
	addc	a,r4
	clr	a
	addc	a,r5
	jb	acc.7,00001$
	;; WTEVTA = wtimer_state[0].time.ref - R3:R2
	mov	dptr,#(_wtimer_state + 0x0004)
	movx	a,@dptr
	inc	dptr
	clr	c
	subb	a,r2
	mov	_WTEVTA0,a
	movx	a,@dptr
	subb	a,r3
	mov	_WTEVTA1,a
	ret
	__endasm;
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer0_update(void) __reentrant __naked
{
	__asm
	mov	r2,_WTCNTA0
	mov	r3,_WTCNTR1
	mov	dptr,#(_wtimer_state + 0x0004)
	movx	a,@dptr
	xch	a,r2
	movx	@dptr,a
	inc	dptr
	clr	c
	subb	a,r2
	mov	r2,a
	movx	a,@dptr
	xch	a,r3
	movx	@dptr,a
	subb	a,r3
	mov	r3,a
	orl	a,ar2
	jz	00000$
	mov	dptr,#(_wtimer_state + 0x0000)
	movx	a,@dptr
	add	a,r2
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,r3
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	movx	@dptr,a
00000$:	ret
	__endasm;
}

static void dummy3(void) __naked
{
	__asm;	
	.area CSEG   (CODE)
	__endasm;
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer0_addcore(struct wtimer_desc __xdata *desc) __reentrant __naked
{
	__asm
	;; R5:R4 = &wtimer_state[0].queue
	mov	r4,#(_wtimer_state + 0x0006)
	mov	r5,#((_wtimer_state + 0x0006) >> 8)
_wtimer_addcore:
	;; R3:R2 = desc
	mov	r2,dpl
	mov	r3,dph
00000$:
	;; R7:R6 = R5:R4->next
	mov	dpl,r4
	mov	dph,r5
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	;; check R7:R6 != WTIMER_NULL
	cjne	r6,#(WTIMER_NULL & 0xFF),00002$
	cjne	r7,#(WTIMER_NULL >> 8),00002$
00001$:
	;; R3:R2->next = R7:R6
	mov	dpl,r2
	mov	dph,r3
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	;; R5:R4->next = R3:R2
	mov	dpl,r4
	mov	dph,r5
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	ret
00002$:
	;; A.7 = sign (R3:R2->time - R7:R6->time)
	push	ar6
	push	ar7
	mov	dpl,r6
	mov	dph,r7
	inc	dptr
	inc	dptr
	inc	dptr
	inc	dptr
	movx	a,@dptr
	mov	r0,a
	inc	dptr
	movx	a,@dptr
	mov	r1,a
	inc	dptr
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	mov	dpl,r2
	mov	dph,r3
	inc	dptr
	inc	dptr
	inc	dptr
	inc	dptr
	clr	c
	movx	a,@dptr
	subb	a,r0
	inc	dptr
	movx	a,@dptr
	subb	a,r1
	inc	dptr
	movx	a,@dptr
	subb	a,r6
	inc	dptr
	movx	a,@dptr
	subb	a,r7
	jb	acc.7,00003$
	pop	ar5
	pop	ar4
	sjmp	00000$
00003$:
	pop	ar7
	pop	ar6
	sjmp	00001$
	__endasm;
}

static void dummy4(void) __naked
{
	__asm;	
	.area HOME   (CODE)
	__endasm;
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer1_schedq(void) __reentrant
{
	__asm
	;; check wtimer_state[1].queue != WTIMER_NULL
	mov	dptr,#(_wtimer_state + 0x000E)
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	cjne	r6,#(WTIMER_NULL & 0xFF),00000$
	cjne	r7,#(WTIMER_NULL >> 8),00000$
00001$:
	;; WTEVTB = wtimer_state[1].time.ref + (0x10000-WTIMER1_MARGIN)
	mov	dptr,#(_wtimer_state + 0x000C)
	movx	a,@dptr
	add	a,#((0x10000-WTIMER1_MARGIN) & 0xFF)
	mov	_WTEVTB0,a
	inc	dptr
	movx	a,@dptr
	addc	a,#((0x10000-WTIMER1_MARGIN) >> 8)
	mov	_WTEVTB1,a
	ret
00000$:
	;; R5:R4:R3:R2 = wtimer_state[1].time.cur - wtimer_state[1].queue->time;
	mov	dpl,r6
	mov	dph,r7
	inc	dptr
	inc	dptr
	inc	dptr
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
	mov	dptr,#(_wtimer_state + 0x0008)
	movx	a,@dptr
	clr	c
	subb	a,r2
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	subb	a,r3
	mov	r3,a
	inc	dptr
	movx	a,@dptr
	subb	a,r4
	mov	r4,a
	inc	dptr
	movx	a,@dptr
	subb	a,r5
	mov	r5,a
	jb	acc.7,00002$
	;; case R5:R4:R3:R2 >= 0
	;; wtimer_state[1].queue = wtimer_state[1].queue->next
	mov	dpl,r6
	mov	dph,r7
	movx	a,@dptr
	mov	r2,a
	inc	dptr
	movx	a,@dptr
	mov	r3,a
	mov	dptr,#(_wtimer_state + 0x000E)
	mov	a,r2
	movx	@dptr,a
	inc	dptr
	mov	a,r3
	movx	@dptr,a
	;; wtimer_addcb_core(old wtimer_state[1].queue)
	mov	dpl,r6
	mov	dph,r7
	lcall	_wtimer_addcb_core
	sjmp	_wtimer1_schedq
00002$:	
	;; check R5:R4:R3:R2 < -(0x10000-WTIMER1_MARGIN)
	;; equivalent: (R5:R4:R3:R2 + (0x10000-WTIMER1_MARGIN)) < 0
	mov	a,r2
	add	a,#((0x10000-WTIMER1_MARGIN) & 0xFF)
	mov	a,r3
	addc	a,#((0x10000-WTIMER1_MARGIN) >> 8)
	clr	a
	addc	a,r4
	clr	a
	addc	a,r5
	jb	acc.7,00001$
	;; WTEVTB = wtimer_state[1].time.ref - R3:R2
	mov	dptr,#(_wtimer_state + 0x000C)
	movx	a,@dptr
	inc	dptr
	clr	c
	subb	a,r2
	mov	_WTEVTB0,a
	movx	a,@dptr
	subb	a,r3
	mov	_WTEVTB1,a
	ret
	__endasm;
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer1_update(void) __reentrant __naked
{
	__asm
	mov	r2,_WTCNTB0
	mov	r3,_WTCNTR1
	mov	dptr,#(_wtimer_state + 0x000C)
	movx	a,@dptr
	xch	a,r2
	movx	@dptr,a
	inc	dptr
	clr	c
	subb	a,r2
	mov	r2,a
	movx	a,@dptr
	xch	a,r3
	movx	@dptr,a
	subb	a,r3
	mov	r3,a
	orl	a,ar2
	jz	00000$
	mov	dptr,#(_wtimer_state + 0x0008)
	movx	a,@dptr
	add	a,r2
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,r3
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	movx	@dptr,a
00000$:	ret
	__endasm;
}

static void dummy5(void) __naked
{
	__asm;	
	.area CSEG   (CODE)
	__endasm;
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer1_addcore(struct wtimer_desc __xdata *desc) __reentrant __naked
{
	__asm
	;; R5:R4 = &wtimer_state[1].queue
	mov	r4,#(_wtimer_state + 0x000E)
	mov	r5,#((_wtimer_state + 0x000E) >> 8)
	ljmp	_wtimer_addcore
	__endasm;
}

static void wtimer_preparesleep(void) __naked
{
	__asm
	mov	_WTCFGB,#0x0F
	mov	_WTIRQEN,#0x01
	mov	dptr,#_SILICONREV
	movx	a,@dptr
	cjne	a,#0x8E,00000$
	;; R3:R2 = WTEVTA - wtimer_state[0].time.ref
	;; wtimer_state[0].time.ref = WTEVTA
	mov	dptr,#(_wtimer_state + 0x0004)
	mov	r2,_WTEVTA0
	movx	a,@dptr
	xch	a,r2
	movx	@dptr,a
	clr	c
	subb	a,r2
	mov	r2,a
	inc	dptr
	mov	r3,_WTEVTA1
	movx	a,@dptr
	xch	a,r3
	movx	@dptr,a
	subb	a,r3
	mov	r3,a
	mov	r4,#0
	;; check for LPXOSC source
	mov	a,_DBGLNKSTAT
	anl	a,#0x10
	jnz	00001$
	mov	a,_WTCFGA
	anl	a,#0x07
	cjne	a,#CLKSRC_LPXOSC,00001$
	mov	a,_WTCFGA
	swap	a
	rl	a
	anl	a,#0x07
	inc	a
	mov	r4,a
	mov	a,#((WTIMER_LPXOSC_SLEEP) << 2)
00002$:
	clr	c
	rrc	a
	djnz	r4,00002$
	add	a,r2
	mov	r2,a
	clr	a
	addc	a,r3
	mov	r3,a
	clr	a
	addc	a,r4
	mov	r4,a
00001$:
	;; wtimer_state[0].time.cur += R4:R3:R2
	mov	dptr,#(_wtimer_state + 0x0000)
	movx	a,@dptr
	add	a,r2
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,r3
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,r4
	movx	@dptr,a
	inc	dptr
	movx	a,@dptr
	addc	a,#0
	movx	@dptr,a
00000$:
	ret
	__endasm;
}

static uint8_t wtimer_checkexpired(void) __naked
{
	__asm
	;; R3:R2 = WTEVTA1:WTEVTA0 - WTCNTR1:WTCNTA0 - 1
	setb	c
	mov	a,_WTEVTA0
	subb	a,_WTCNTA0
	mov	r2,a
	mov	a,_WTEVTA1
	subb	a,_WTCNTR1
	mov	r3,a
	;; R3:R2 + WTIMER0_MARGIN
	.if	(WTIMER0_MARGIN & 0xFF)
	mov	a,#(WTIMER0_MARGIN & 0xFF)
	add	a,r2
	mov	a,#(WTIMER0_MARGIN >> 8)
	addc	a,r3
	.else
	mov	a,#(WTIMER0_MARGIN >> 8)
	add	a,r3
	.endif
	jc	00000$
	;; R3:R2 = WTEVTB1:WTEVTB0 - WTCNTR1:WTCNTB0 - 1
	setb	c
	mov	a,_WTEVTB0
	subb	a,_WTCNTB0
	mov	r2,a
	mov	a,_WTEVTB1
	subb	a,_WTCNTR1
	mov	r3,a
	;; R3:R2 + WTIMER1_MARGIN
	.if	(WTIMER1_MARGIN & 0xFF)
	mov	a,#(WTIMER1_MARGIN & 0xFF)
	add	a,r2
	mov	a,#(WTIMER1_MARGIN >> 8)
	addc	a,r3
	.else
	mov	a,#(WTIMER1_MARGIN >> 8)
	add	a,r3
	.endif
	jc	00000$
	;; check WTSTAT & 0x21
	mov	a,_WTSTAT
	anl	a,#0x21
	jnz	00000$
	mov	dpl,a
	ret
00000$:
	mov	dpl,#1
	ret	
	__endasm;
}

#else

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer0_schedq(void) __reentrant
{
	while (wtimer_state[0].queue != WTIMER_NULLDESC) {
		int32_t __autodata td = wtimer_state[0].time.cur - wtimer_state[0].queue->time;
		if (td >= 0) {
			struct wtimer_callback __xdata * __autodata d = (struct wtimer_callback __xdata *)wtimer_state[0].queue;
			wtimer_state[0].queue = wtimer_state[0].queue->next;
			wtimer_addcb_core(d);
			continue;
		}
		if (td < -(0x10000-WTIMER0_MARGIN))
			break;
		{
			uint16_t __autodata nxt = wtimer_state[0].time.ref - (uint16_t)td;
			WTEVTA0 = nxt;
			WTEVTA1 = nxt >> 8;
		}
		return;
	}
	{
		uint16_t __autodata nxt = wtimer_state[0].time.ref + (uint16_t)(0x10000-WTIMER0_MARGIN);
		WTEVTA0 = nxt;
		WTEVTA1 = nxt >> 8;
	}
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer0_update(void) __reentrant
{
	uint16_t __autodata t;
	t = WTCNTA0;
	t |= ((uint16_t)WTCNTR1) << 8;
	{
		uint16_t __autodata t1 = wtimer_state[0].time.ref;
		wtimer_state[0].time.ref = t;
		t -= t1;
	}
	if (!t)
		return;
	wtimer_state[0].time.cur += t;
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer0_addcore(struct wtimer_desc __xdata *desc) __reentrant
{
	struct wtimer_desc __xdata * __autodata d = WTIMER_PTR(wtimer_state[0].queue);
	for (;;) {
		struct wtimer_desc __xdata * __autodata dn = d->next;
		int32_t __autodata td;
		if (dn == WTIMER_NULLDESC)
			break;
		td = desc->time - dn->time;
		if (td < 0)
			break;
		d = dn;
	}
	desc->next = d->next;
	d->next = desc;
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer1_schedq(void) __reentrant
{
	while (wtimer_state[1].queue != WTIMER_NULLDESC) {
		int32_t __autodata td = wtimer_state[1].time.cur - wtimer_state[1].queue->time;
		if (td >= 0) {
			struct wtimer_callback __xdata * __autodata d = (struct wtimer_callback __xdata *)wtimer_state[1].queue;
			wtimer_state[1].queue = wtimer_state[1].queue->next;
			wtimer_addcb_core(d);
			continue;
		}
		if (td < -(0x10000-WTIMER1_MARGIN))
			break;
		{
			uint16_t __autodata nxt = wtimer_state[1].time.ref - (uint16_t)td;
			WTEVTB0 = nxt;
			WTEVTB1 = nxt >> 8;
		}
		return;
	}
	{
		uint16_t __autodata nxt = wtimer_state[1].time.ref + (uint16_t)(0x10000-WTIMER1_MARGIN);
		WTEVTB0 = nxt;
		WTEVTB1 = nxt >> 8;
	}
}

// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer1_update(void) __reentrant
{
	uint16_t __autodata t;
	t = WTCNTB0;
	t |= ((uint16_t)WTCNTR1) << 8;
	{
		uint16_t __autodata t1 = wtimer_state[1].time.ref;
		wtimer_state[1].time.ref = t;
		t -= t1;
	}
	if (!t)
		return;
	wtimer_state[1].time.cur += t;
}
	
// Must be called with (wtimer) interrupts disabled
__reentrantb void wtimer1_addcore(struct wtimer_desc __xdata *desc) __reentrant
{
	struct wtimer_desc __xdata * __autodata d = WTIMER_PTR(wtimer_state[1].queue);
	for (;;) {
		struct wtimer_desc __xdata * __autodata dn = d->next;
		int32_t __autodata td;
		if (dn == WTIMER_NULLDESC)
			break;
		td = desc->time - dn->time;
		if (td < 0)
			break;
		d = dn;
	}
	desc->next = d->next;
	d->next = desc;
}

static __reentrantb void wtimer_preparesleep(void) __reentrant
{
	uint16_t __autodata t;
	WTCFGB = 0x0F;
	WTIRQEN = 0x01;
	if (SILICONREV != 0x8E)
		return;
	t = WTEVTA0;
	t |= ((uint16_t)WTEVTA1) << 8;
	{
		uint16_t t1 = wtimer_state[0].time.ref;
		wtimer_state[0].time.ref = t;
		t -= t1;
	}
	if (!(DBGLNKSTAT & 0x10))
		if ((WTCFGA & 0x07) == CLKSRC_LPXOSC)
			t += (((uint8_t)(WTIMER_LPXOSC_SLEEP)) << 1) >> ((WTCFGA >> 3) & 0x07);
	wtimer_state[0].time.cur += t;
}

static __reentrantb uint8_t wtimer_checkexpired(void) __reentrant
{
	{
		uint16_t __autodata t;
		t = WTCNTA0;
		t |= ((uint16_t)WTCNTR1) << 8;
		t -= WTEVTA0 | (((uint16_t)WTEVTA1) << 8);
		if (t < WTIMER0_MARGIN)
			return 1;
	}
	{
		uint16_t __autodata t;
		t = WTCNTB0;
		t |= ((uint16_t)WTCNTR1) << 8;
		t -= WTEVTB0 | (((uint16_t)WTEVTB1) << 8);
		if (t < WTIMER1_MARGIN)
			return 1;
	}
	if (WTSTAT & 0x21)
		return 1;
	return 0;
}

#endif

#if defined(SDCC)
extern __reentrantb uint8_t wtimer_cansleep(void) __reentrant;

static void wtimer_cansleep_dummy(void) __naked
{
	__asm
	.area HOME      (CODE)
	.area WTCANSLP0 (CODE)
	.area WTCANSLP1 (CODE)
	.area WTCANSLP2 (CODE)

	.area WTCANSLP0 (CODE)
_wtimer_cansleep_ret0:
	mov	dpl,#0x00
	ret

	.globl	_wtimer_cansleep
_wtimer_cansleep:
	;; wtimer_state[1].queue == WTIMER_NULLDESC
	mov	dptr,#(_wtimer_state + 0x000e)
	movx	a,@dptr
	cpl	a
	jnz	_wtimer_cansleep_ret0
	inc	dptr
	movx	a,@dptr
	cpl	a
	jnz	_wtimer_cansleep_ret0

	.area WTCANSLP2 (CODE)
	mov	dpl,#0x01
	ret

	.area HOME     (CODE)
	.area WTSTDBY0 (CODE)
	.area WTSTDBY1 (CODE)
	.area WTSTDBY2 (CODE)

	.area WTSTDBY1 (CODE)
	push	ar0
	push	ar1
	push	ar2
	push	ar3
	push	ar4
	push	ar5
	push	ar6
	push	ar7
	lcall	_wtimer_runcallbacks
	mov	dpl,#WTFLAG_CANSTANDBY
	lcall	_wtimer_idle
	pop	ar7
	pop	ar6
	pop	ar5
	pop	ar4
	pop	ar3
	pop	ar2
	pop	ar1
	pop	ar0
	ret

	.area CSEG      (CODE)
	__endasm;
}

#else
__reentrantb uint8_t wtimer_cansleep(void) __reentrant
{
	return wtimer_state[1].queue == WTIMER_NULLDESC;
}
#endif

#if defined(SDCC)
typedef __reentrantb void (*handler_t)(struct wtimer_callback __xdata *desc) __reentrant;
#else
typedef void (*handler_t)(struct wtimer_callback __xdata *desc);
#endif

/*
 * This function is reentrant even though it is not marked reentrant.
 * When marked reentrant, code generation gets worse for SDCC
 * (IE is placed on stack rather than a register)
 */

#if defined(SDCC)
#pragma nooverlay wtimer_runcallbacks
uint8_t wtimer_runcallbacks(void)
#elif defined(__ICC8051__)
uint8_t wtimer_runcallbacks(void)
#else
__reentrantb uint8_t wtimer_runcallbacks(void) __reentrant
#endif
{
	uint8_t __autodata ret = 0;
	for (;;) {
		uint8_t __autodata iesave = IE & 0x80;
		EA = 0;
		wtimer0_update();
		wtimer0_schedq();
		wtimer1_update();
		wtimer1_schedq();
		for (;;) {
			{
				struct wtimer_callback __xdata * __autodata d = wtimer_pending;
				if (d != WTIMER_NULLCB) {
					wtimer_pending = d->next;
					IE |= iesave;
					++ret;
					((handler_t)(d->handler))(d);
					EA = 0;
					continue;
				}
			}
			{
				uint8_t __autodata exp = wtimer_checkexpired();
				IE |= iesave;
				if (exp)
					break;
				return ret;
			}
		}
	}
}

#if defined(SDCC) || defined(__ICC8051__)
#define WTFLAG_CANSLEEPANY (WTFLAG_CANSLEEP | WTFLAG_CANSLEEPCONT)
#else
#define WTFLAG_CANSLEEPANY (WTFLAG_CANSLEEP)
#endif

__reentrantb uint8_t wtimer_idle(uint8_t flags) __reentrant
{
	uint8_t iesave = IE & 0x80;
	EA = 0;
	if (wtimer_pending != WTIMER_NULLCB || wtimer_checkexpired()) {
		IE |= iesave;
		return WTIDLE_WORK;
	}
	if (flags & WTFLAG_CANSLEEPANY && wtimer_cansleep()) {
		wtimer_preparesleep();
		// FIXME: copy wtimer_state[0] to IRAM if there are not too many cb's?
		if ((void __xdata *)(&wtimer_state[0]) < (void __xdata *)0x1000)
			PCON = (PCON & 0x0C) | 0x04;
		if ((void __xdata *)(&wtimer_state[0]) >= (void __xdata *)(0x1000-sizeof(wtimer_state[0])))
			PCON = (PCON & 0x0C) | 0x08;
#if defined(SDCC) || defined(__ICC8051__)
		if (flags & WTFLAG_CANSLEEPCONT) {
			enter_sleep_cont();
			IE |= iesave;
			return WTIDLE_SLEEP;
		}
#endif
		enter_sleep();
	} else if (flags & WTFLAG_CANSTANDBY) {
		enter_standby();
	}
	IE |= iesave;
	return 0;
}
