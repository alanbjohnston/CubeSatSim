#include "ax8052.h"
#include "libmftypes.h"

#if defined(SDCC)

/**
 * \brief enter standby mode
 *
 */
__reentrantb void enter_standby(void) __reentrant __naked
{
	__asm
	mov	a,_PCON
	anl	a,#0x0C
	orl	a,#0x01
	mov	_PCON,a
	nop
	ret
	nop
	nop
	nop
	nop
	nop
	__endasm;
}

#elif 0 && (defined(__CX51__) || defined(__C51__))

__reentrantb void enter_standby(void) __reentrant
{
#pragma asm
	mov	a,_PCON
	anl	a,#0x0C
	orl	a,#0x01
	mov	_PCON,a
	nop
	ret
	nop
	nop
	nop
	nop
	nop
#pragma endasm
}

#else

__reentrantb void enter_standby(void) __reentrant
{
	PCON = (PCON & 0x0C) | 0x01;
	nop();
	nop();
	nop();
	nop();
	nop();
	nop();
	nop();
}

#endif
