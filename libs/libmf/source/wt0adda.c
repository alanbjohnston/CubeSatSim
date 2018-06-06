#include "ax8052.h"
#include "wtimer.h"

__reentrantb void wtimer0_addabsolute(struct wtimer_desc __xdata *desc) __reentrant
{
        uint8_t iesave = IE & 0x80;
        EA = 0;
        wtimer0_update();
        wtimer0_addcore(desc);
        wtimer0_schedq();
        IE |= iesave;
}
