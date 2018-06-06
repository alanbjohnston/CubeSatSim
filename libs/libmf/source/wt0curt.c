#include "ax8052.h"
#include "wtimer.h"

__reentrantb uint32_t wtimer0_curtime(void) __reentrant
{
	uint32_t r;
	uint8_t iesave = IE & 0x80;
	EA = 0;
	wtimer0_update();
	r = wtimer_state[0].time.cur;
	IE |= iesave;
	return r;
}
