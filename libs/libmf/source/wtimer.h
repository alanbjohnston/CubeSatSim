#ifndef WTIMER_H
#define WTIMER_H

#include "libmfwtimer.h"

struct wtimer_state {
	struct wtimer_state_time {
		uint32_t cur;
		uint16_t ref;
	} time;
	struct wtimer_desc __xdata *queue;
};

extern struct wtimer_state __xdata wtimer_state[2];
extern struct wtimer_callback __xdata * __xdata wtimer_pending;

#define WTIMER_NULL      (0xFFFF)
#define WTIMER_NULLDESC  ((struct wtimer_desc __xdata *)WTIMER_NULL)
#define WTIMER_NULLCB    ((struct wtimer_callback __xdata *)WTIMER_NULL)
#define WTIMER_PTR(x)    ((struct wtimer_desc __xdata *)((char __xdata *)&(x) - (char __xdata *)&((struct wtimer_desc __xdata *)0)->next))
#define WTIMER_CBPTR(x)  ((struct wtimer_callback __xdata *)((char __xdata *)&(x) - (char __xdata *)&((struct wtimer_callback __xdata *)0)->next))

// the following routines must be called with (wtimer) interrupts disabled
extern __reentrantb void wtimer_addcb_core(struct wtimer_callback __xdata *desc) __reentrant;
extern __reentrantb uint8_t wtimer_removecb_core(struct wtimer_callback __xdata *desc) __reentrant;
extern __reentrantb void wtimer0_schedq(void) __reentrant;
extern __reentrantb void wtimer0_update(void) __reentrant;
extern __reentrantb void wtimer0_addcore(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb uint8_t wtimer0_removecb_core(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb void wtimer1_schedq(void) __reentrant;
extern __reentrantb void wtimer1_update(void) __reentrant;
extern __reentrantb void wtimer1_addcore(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb uint8_t wtimer1_removecb_core(struct wtimer_desc __xdata *desc) __reentrant;

#define WTIMER_USEASM

#endif /* WTIMER_H */
