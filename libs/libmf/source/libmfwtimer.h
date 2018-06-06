#ifndef LIBMFWTIMER_H
#define LIBMFWTIMER_H

#include "libmftypes.h"

struct wtimer_callback;
struct wtimer_desc;

#if defined(__ICC8051__)
typedef void (*wtimer_callback_handler_t)(struct wtimer_callback __xdata *desc);
typedef void (*wtimer_desc_handler_t)(struct wtimer_desc __xdata *desc);
#else
typedef __reentrantb void (*wtimer_callback_handler_t)(struct wtimer_callback __xdata *desc) __reentrant;
typedef __reentrantb void (*wtimer_desc_handler_t)(struct wtimer_desc __xdata *desc) __reentrant;
#endif

struct wtimer_callback {
	// do not change the order
	// must be compatible with wtimer_desc
	struct wtimer_callback __xdata *next;
	wtimer_callback_handler_t handler;
};

struct wtimer_desc {
	// do not change the order
	struct wtimer_desc __xdata *next;
	wtimer_desc_handler_t handler;
	uint32_t time;
};

#define WTFLAG_CANSLEEP                 0x01
#define WTFLAG_CANSTANDBY               0x02
#define WTFLAG_CANSLEEPCONT             0x04

#define WTIDLE_WORK                     0x01
#define WTIDLE_SLEEP                    0x02

extern __reentrantb void wtimer0_setconfig(uint8_t cfg) __reentrant;
extern __reentrantb void wtimer1_setconfig(uint8_t cfg) __reentrant;

#define wtimer0_setclksrc(clksrc, prescaler)                    \
do {                                                            \
	uint8_t c = clksrc, p = prescaler;                      \
	wtimer0_setconfig((c & 0x07) | ((p & 0x07) << 3));      \
} while (0)

#define wtimer1_setclksrc(clksrc, prescaler)                    \
do {                                                            \
	uint8_t c = clksrc, p = prescaler;                      \
	wtimer1_setconfig((c & 0x07) | ((p & 0x07) << 3));      \
} while (0)

extern __reentrantb void wtimer_init(void) __reentrant;
extern __reentrantb void wtimer_init_deepsleep(void) __reentrant;
extern __reentrantb uint8_t wtimer_idle(uint8_t flags) __reentrant;
#if defined(__ICC8051__)
extern uint8_t wtimer_runcallbacks(void);
#else
// note: wtimer_runcallbacks is only reentrant if the handlers
// it might call are reentrant too
extern __reentrantb uint8_t wtimer_runcallbacks(void) __reentrant;
#endif

extern __reentrantb uint32_t wtimer0_curtime(void) __reentrant;
extern __reentrantb uint32_t wtimer1_curtime(void) __reentrant;
extern __reentrantb void wtimer0_addabsolute(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb void wtimer1_addabsolute(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb void wtimer0_addrelative(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb void wtimer1_addrelative(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb uint8_t wtimer_remove(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb uint8_t wtimer0_remove(struct wtimer_desc __xdata *desc) __reentrant;
extern __reentrantb uint8_t wtimer1_remove(struct wtimer_desc __xdata *desc) __reentrant;

extern __reentrantb void wtimer_add_callback(struct wtimer_callback __xdata *desc) __reentrant;
extern __reentrantb uint8_t wtimer_remove_callback(struct wtimer_callback __xdata *desc) __reentrant;

extern __reentrantb uint8_t wtimer_cansleep(void) __reentrant;

#if defined SDCC
extern void wtimer_irq(void) __interrupt(1);
#elif defined __CX51__ || defined __C51__
#elif defined __ICC8051__
#else
#error "Compiler unsupported"
#endif

#endif /* LIBMFWTIMER_H */
