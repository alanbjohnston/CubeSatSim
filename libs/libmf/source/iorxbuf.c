#include "ax8052.h"

#if UART == 0
#include "libmfuart0.h"

UART0_DEFINE_RXBUFFER(64)

#elif UART == 1
#include "libmfuart1.h"

UART1_DEFINE_RXBUFFER(64)

#elif UART == 2
#include "libmfdbglink.h"

DBGLINK_DEFINE_RXBUFFER(64)

#else
#error "UART not set"
#endif
