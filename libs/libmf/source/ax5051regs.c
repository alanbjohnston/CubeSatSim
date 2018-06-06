#if defined __ICC8051__

#define AXCOMPILER_H
# define SBIT(name, addr, bit)
# define SFR(name, addr)
# define SFRX(name, addr)       __no_init __root volatile unsigned char __xdata name @ addr ;
# define SFR16(name, addr)
# define SFR16E(name, fulladdr)
# define SFR16LEX(name, addr)
# define SFR32(name, fulladdr)
# define SFR32E(name, fulladdr)

#define AX8052_H

#include "ax8052f151.h"

#endif
