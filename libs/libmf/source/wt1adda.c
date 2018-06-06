#include "ax8052.h"
#include "wtimer.h"

__reentrantb void wtimer1_addabsolute(struct wtimer_desc __xdata *desc) __reentrant
{
	uint8_t iesave = IE & 0x80;
	EA = 0;
	wtimer1_update();
	wtimer1_addcore(desc);
	wtimer1_schedq();
	IE |= iesave;
}
