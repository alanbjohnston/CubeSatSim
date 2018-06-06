#include "ax8052.h"
#include "libmftypes.h"

#ifdef SDCC

/**
 * \brief enter sleep mode
 *
 */
__reentrantb void enter_sleep(void) __reentrant __naked
{
	__asm
	mov     dptr,#_RADIOMUX
	movx    a,@dptr
	anl     a,#~0x40
	movx    @dptr,a
	clr     _EA
	clr     a
	mov     _EIE,a
	mov     _E2IE,a
	mov     dptr,#_GPIOENABLE
	movx    @dptr,a
	mov     r0,#0xff
	mov     @r0,a
	dec     r0
	mov     @r0,a
	mov     a,_PCON
	anl     a,#0x0c
	orl     a,#0x02
	mov     _PCON,a
	mov     _CODECONFIG,#0xd3
	ljmp    0xe047
	__endasm;
}

#elif defined __CX51__ || defined __C51__

__reentrantb void enter_sleep(void) __reentrant
{
	for (;;) {
		RADIOMUX &= (uint8_t)~0x40;
		EA = 0;
		EIE = 0x00;
		E2IE = 0x00;
		GPIOENABLE = 0;
		PCON = 0x02 | (PCON & 0x0C);
		CODECONFIG = 0xD3;
#pragma asm
		ljmp 0xE047
#pragma endasm
	}
}

#elif defined __ICC8051__

__noreturn __reentrantb void enter_sleep(void) __reentrant
{
	for (;;) {
		RADIOMUX &= (uint8_t)~0x40;
		EA = 0;
		EIE = 0x00;
		E2IE = 0x00;
		asm("clr     a");
		asm("mov     r0,#0xff");
		asm("mov     @r0,a");
		asm("dec     r0");
		asm("mov     @r0,a");
		GPIOENABLE = 0;
		PCON = 0x02 | (PCON & 0x0C);
		CODECONFIG = 0xD3;
		asm("ljmp 0xE047");
	}
}

#else

#error "Compiler unsupported"

#endif
