#include "ax8052.h"
#include "wtimer.h"

__reentrantb void wtimer_add_callback(struct wtimer_callback __xdata *desc) __reentrant
{
        uint8_t iesave = IE & 0x80;
        EA = 0;
        wtimer_addcb_core(desc);
        IE |= iesave;
}
