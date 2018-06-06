#include "ax8052.h"
#include "libmftypes.h"

#if defined(SDCC)

#pragma codeseg WTSTDBY0

// fixme: make enter_* reentrant, make wtimer_runcallbacks/idle reentrant

__reentrantb void wtimer_standby(void) __reentrant
{
	__asm
	.area HOME     (CODE)
	.area WTSTDBY0 (CODE)
	.area WTSTDBY1 (CODE)
	.area WTSTDBY2 (CODE)

	.area WTSTDBY2 (CODE)
	mov	a,#0x0C
	anl	a,_PCON
	orl	a,#0x01
	mov	_PCON,a
	ret
	__endasm;
}


#else

__reentrantb void wtimer_standby(void) __reentrant
{
	enter_standby();
}

#endif
