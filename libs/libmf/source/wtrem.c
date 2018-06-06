#include "ax8052.h"
#include "wtimer.h"

#if defined(WTIMER_USEASM) && defined(SDCC)

__reentrantb uint8_t wtimer_removecb_core(struct wtimer_callback __xdata *desc) __reentrant __naked
{
	__asm
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00

	;; R5:R4 = &wtimer_pending
	mov	r4,#_wtimer_pending
	mov	r5,#(_wtimer_pending >> 8)
_wtimer_removecb_core_body:
	;; R0 = 0
	mov	r0,#0
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
	;; check R7:R6 == WTIMER_NULL
	cjne	r6,#(WTIMER_NULL & 0xFF),00002$
	cjne	r7,#(WTIMER_NULL >> 8),00002$
00001$:
	mov	dpl,r0
	ret
00002$:
	;; check R7:R6 == R3:R2
	mov	a,r2
	cjne	a,ar6,00003$
	mov	a,r3
	cjne	a,ar7,00003$
	;; R7:R6 = R7:R6->next
	mov	dpl,r6
	mov	dph,r7
	movx	a,@dptr
	mov	r6,a
	inc	dptr
	movx	a,@dptr
	mov	r7,a
	;; R5:R4->next = R7:R6
	mov	dpl,r4
	mov	dph,r5
	mov	a,r6
	movx	@dptr,a
	inc	dptr
	mov	a,r7
	movx	@dptr,a
	;; ++R0
	inc	r0
	;; check R7:R6 == WTIMER_NULL
	cjne	r6,#(WTIMER_NULL & 0xFF),00003$
	cjne	r7,#(WTIMER_NULL >> 8),00003$
	sjmp	00001$
00003$:
	;; R5:R4 = R7:R6
	mov	r4,ar6
	mov	r5,ar7
	sjmp	00000$
	__endasm;
}

__reentrantb uint8_t wtimer0_removecb_core(struct wtimer_desc __xdata *desc) __reentrant __naked
{
	__asm
	;; R5:R4 = &wtimer_state[0].queue
	mov	r4,#(_wtimer_state + 0x0006)
	mov	r5,#((_wtimer_state + 0x0006) >> 8)
	sjmp	_wtimer_removecb_core_body
	__endasm;
}

__reentrantb uint8_t wtimer1_removecb_core(struct wtimer_desc __xdata *desc) __reentrant __naked
{
	__asm
	;; R5:R4 = &wtimer_state[1].queue
	mov	r4,#(_wtimer_state + 0x000E)
	mov	r5,#((_wtimer_state + 0x000E) >> 8)
	sjmp	_wtimer_removecb_core_body
	__endasm;
}

#else

__reentrantb uint8_t wtimer_removecb_core(struct wtimer_callback __xdata *desc) __reentrant
{
	struct wtimer_callback __xdata *d;
	uint8_t ret = 0;
	d = WTIMER_CBPTR(wtimer_pending);
	for (;;) {
		struct wtimer_callback __xdata *dn = d->next;
		if (dn == WTIMER_NULLCB)
			break;
		if (dn == desc) {
			dn = dn->next;
			d->next = dn;
			++ret;
			if (dn == WTIMER_NULLCB)
				break;
		}
		d = dn;
	}
	return ret;
}

__reentrantb uint8_t wtimer0_removecb_core(struct wtimer_desc __xdata *desc) __reentrant
{
	struct wtimer_desc __xdata *d;
	uint8_t ret = 0;
	d = WTIMER_PTR(wtimer_state[0].queue);
	for (;;) {
		struct wtimer_desc __xdata *dn = d->next;
		if (dn == WTIMER_NULLDESC)
			break;
		if (dn == desc) {
			dn = dn->next;
			d->next = dn;
			++ret;
			if (dn == WTIMER_NULLDESC)
				break;
		}
		d = dn;
	}
	return ret;
}

__reentrantb uint8_t wtimer1_removecb_core(struct wtimer_desc __xdata *desc) __reentrant
{
	struct wtimer_desc __xdata *d;
	uint8_t ret = 0;
	d = WTIMER_PTR(wtimer_state[1].queue);
	for (;;) {
		struct wtimer_desc __xdata *dn = d->next;
		if (dn == WTIMER_NULLDESC)
			break;
		if (dn == desc) {
			dn = dn->next;
			d->next = dn;
			++ret;
			if (dn == WTIMER_NULLDESC)
				break;
		}
		d = dn;
	}
	return ret;
}

#endif
