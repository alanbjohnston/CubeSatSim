#include "ax8052.h"
#include "wtimer.h"

__reentrantb uint8_t wtimer0_remove(struct wtimer_desc __xdata *desc) __reentrant
{
	struct wtimer_desc __xdata *d;
	uint8_t ret;
	uint8_t iesave = IE & 0x80;
	EA = 0;
	ret = wtimer_removecb_core((struct wtimer_callback __xdata *)desc);
	ret += wtimer0_removecb_core(desc);
	IE |= iesave;
	return ret;
}
