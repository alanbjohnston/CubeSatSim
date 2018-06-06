#include "libmftypes.h"

__data uint16_t random_seed;

#ifdef SDCC

/**
 * \brief Pseudo Random Number Generator for Channel Access
 *
 */
uint16_t random(void)
{
	__asm
	mov	a,_random_seed
	mov	b,#<28629
	mul	ab
	add	a,#<157
	mov	dpl,a
	mov	a,b
	addc	a,#>157
	mov	dph,a
	mov	a,_random_seed
	mov	b,#>28629
	mul	ab
	add	a,dph
	mov	dph,a
	mov	a,_random_seed+1
	mov	b,#<28629
	mul	ab
	add	a,dph
	mov	dph,a
	mov	_random_seed,dpl
	mov	_random_seed+1,a
	ret
	__endasm;
}

#else

uint16_t random(void)
{
	random_seed = 28629U * random_seed + 157U;
	return random_seed;
}

#endif
