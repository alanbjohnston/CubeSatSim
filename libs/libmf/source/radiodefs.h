#ifndef RADIODEFS_H
#define RADIODEFS_H

#if RADIO == 5031

#include "ax8052f131.h"

#define radio_probeirq         ax5031_probeirq
#define radio_comminit         ax5031_comminit
#define radio_commsleepexit    ax5031_commsleepexit
#define radio_reset            ax5031_reset
#define radio_readfifo         ax5031_readfifo
#define radio_writefifo        ax5031_writefifo
#define RACC                   0x00
#define FDATA                  0x005
#define FSTAT                  0x004
#define DEEPSLEEP              0
#define VREGDELAY              10

#define RADIO_SILICONREVISION  AX5031_SILICONREVISION
#define RADIO_SCRATCH          AX5031_SCRATCH
#define RADIO_PWRMODE          AX5031_PWRMODE

#define PWRMODE_PWRDOWN        0x00

#define SILICONREV1            0x21
#undef SILICONREV2

#elif RADIO == 5042

#include "ax8052f142.h"

#define radio_probeirq         ax5042_probeirq
#define radio_comminit         ax5042_comminit
#define radio_commsleepexit    ax5042_commsleepexit
#define radio_reset            ax5042_reset
#define radio_readfifo         ax5042_readfifo
#define radio_writefifo        ax5042_writefifo
#define RACC                   0x00
#define FDATA                  0x005
#define FSTAT                  0x004
#define DEEPSLEEP              0
#define VREGDELAY              0

#define RADIO_SILICONREVISION  AX5042_SILICONREVISION
#define RADIO_SCRATCH          AX5042_SCRATCH
#define RADIO_PWRMODE          AX5042_PWRMODE

#define PWRMODE_PWRDOWN        0x00

#define SILICONREV1            0x02
#undef SILICONREV2

#elif RADIO == 5043

#include "ax8052f143.h"

#define radio_probeirq         ax5043_probeirq
#define radio_comminit         ax5043_comminit
#define radio_commsleepexit    ax5043_commsleepexit
#define radio_reset            ax5043_reset
#define radio_readfifo         ax5043_readfifo
#define radio_writefifo        ax5043_writefifo
#define radio_enter_deepsleep  ax5043_enter_deepsleep
#define radio_wakeup_deepsleep ax5043_wakeup_deepsleep
#define RACC                   0x0c
#define FDATA                  0x029
#define FSTAT                  0x028
#define DEEPSLEEP              1
#define VREGDELAY              10

#define RADIO_SILICONREVISION  AX5043_SILICONREVISION
#define RADIO_SCRATCH          AX5043_SCRATCH
#define RADIO_PWRMODE          AX5043_PWRMODE

#define PWRMODE_PWRDOWN        0x00
#define PWRMODE_DEEPSLEEP      0x01

#define SILICONREV1            0x51
#undef SILICONREV2

#elif RADIO == 5051

#include "ax8052f151.h"

#define radio_probeirq         ax5051_probeirq
#define radio_comminit         ax5051_comminit
#define radio_commsleepexit    ax5051_commsleepexit
#define radio_reset            ax5051_reset
#define radio_readfifo         ax5051_readfifo
#define radio_writefifo        ax5051_writefifo
#define RACC                   0x00
#define FDATA                  0x005
#define FSTAT                  0x004
#define DEEPSLEEP              0
#define VREGDELAY              0

#define RADIO_SILICONREVISION  AX5051_SILICONREVISION
#define RADIO_SCRATCH          AX5051_SCRATCH
#define RADIO_PWRMODE          AX5051_PWRMODE

#define PWRMODE_PWRDOWN        0x00

#define SILICONREV1            0x16
#define SILICONREV2            0x14

#else
#error unknown radio
#endif

#define RADIO_OK                0
#define RADIO_ERR_REVISION      1
#define RADIO_ERR_COMM          2
#define RADIO_ERR_IRQ           3
#define RADIO_ERR_WAKEUPTIMEOUT 4

extern __reentrantb uint8_t radio_probeirq(void) __reentrant;
extern __reentrantb uint8_t radio_wakeup_deepsleep_core(void) __reentrant;

#endif /* RADIODEFS_H */
