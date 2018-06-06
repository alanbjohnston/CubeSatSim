#include "ax8052.h"
#include "libmftypes.h"

#if defined(SDCC)

#define REGTABLE_SIG0 0xBE
#define REGTABLE_SIG1 0x37

/**
 * \brief enter sleep mode, continue after call
 *
 */
__reentrantb void enter_sleep_cont(void) __reentrant __naked
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
	mov     @r0,#REGTABLE_SIG0
	dec     r0
	mov     @r0,#REGTABLE_SIG1
	dec     r0
	mov     @r0,sp
	mov     a,_PCON
	anl     a,#0x0c
	orl     a,#0x02
	mov     _PCON,a
	mov     _CODECONFIG,#0xd3
	ljmp    0xe047
	__endasm;
}

static void dummy(void) __naked
{
	__asm
	.area CSEG    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)

	.area GSINIT1 (CODE)
	mov     r0,#0xff
	mov     a,_PCON
	jb      acc.6,00001$
	clr     a
	mov     @r0,a
	dec     r0
	mov     @r0,a
	sjmp    00000$
00001$:
	cjne    @r0,#REGTABLE_SIG0,00000$
	dec     r0
	cjne    @r0,#REGTABLE_SIG1,00000$
	dec     r0
	mov     sp,@r0
	ljmp    __sdcc_external_startup
00000$:
	__endasm;
}

#elif defined(__ICC8051__)

__reentrantb void enter_sleep_cont(void) __reentrant
{
        for (;;) {
                RADIOMUX &= (uint8_t)~0x40;
                EA = 0;
                EIE = 0x00;
                E2IE = 0x00;
		asm("mov     r0,#0xff");
		asm("mov     @r0,#0xBE");
		asm("dec     r0");
		asm("mov     @r0,#0x37");
		asm("dec     r0");
		asm("mov     @r0,sp");
                GPIOENABLE = 0;
                PCON = 0x02 | (PCON & 0x0C);
                CODECONFIG = 0xD3;
                asm("ljmp    0xE047");
        }
}

#endif
