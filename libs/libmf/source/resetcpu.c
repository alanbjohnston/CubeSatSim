#include "ax8052.h"
#include "libmftypes.h"

#ifdef SDCC

/**
 * \brief reset the CPU
 *
 */
__reentrantb void reset_cpu(void) __reentrant
{
	EA = 0;
	GPIOENABLE = 0;
	CODECONFIG = 0xD3;
	__asm
	ljmp 0xE047
	__endasm;
}

#elif defined __CX51__ || defined __C51__

__reentrantb void reset_cpu(void) __reentrant
{
	EA = 0;
	GPIOENABLE = 0;
	CODECONFIG = 0xD3;
#pragma asm
	ljmp 0xE047
#pragma endasm
}

#elif defined __ICC8051__

__noreturn __reentrantb void reset_cpu(void) __reentrant
{
	EA = 0;
	GPIOENABLE = 0;
	CODECONFIG = 0xD3;
	asm("ljmp 0xE047");
}

#else

#error "Compiler unsupported"

#endif
