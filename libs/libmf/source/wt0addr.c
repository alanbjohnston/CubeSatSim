#include "ax8052.h"
#include "wtimer.h"

#if defined(WTIMER_USEASM) && defined(SDCC)

__reentrantb void wtimer0_addrelative(struct wtimer_desc __xdata *desc) __reentrant __naked
{
	__asm
	mov	a,_IE
	anl	a,#0x80
	push	acc
	clr	_EA
	push	dpl
	push	dph
	lcall	_wtimer0_update
	;; desc->time += wtimer_state[0].time.cur
	mov	dptr,#(_wtimer_state + 0x0000)
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
	pop	dph
	pop	dpl
	mov	r6,dpl
	mov	r7,dph
	inc	dptr
	inc	dptr
	inc	dptr
	inc	dptr
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
	addc	a,r5
	movx	@dptr,a
	mov	dpl,r6
	mov	dph,r7
	lcall	_wtimer0_addcore
	lcall	_wtimer0_schedq
	pop	acc
	orl	_IE,a
	ret
	__endasm;
}

#else

__reentrantb void wtimer0_addrelative(struct wtimer_desc __xdata *desc) __reentrant
{
        uint8_t iesave = IE & 0x80;
        EA = 0;
        wtimer0_update();
	desc->time += wtimer_state[0].time.cur;
        wtimer0_addcore(desc);
        wtimer0_schedq();
        IE |= iesave;
}

#endif
