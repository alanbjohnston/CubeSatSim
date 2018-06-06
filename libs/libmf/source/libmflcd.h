#ifndef LIBMFLCD_H
#define LIBMFLCD_H

#include "libmftypes.h"

/*
 * LC Display
 */
#if defined SDCC
#pragma callee_saves lcd_waitlong,lcd_waitshort,lcd_write,lcd_writecmd,lcd_writedata,lcd_portoff,lcd_portinit,lcd_init,lcd_setpos
#endif

extern void lcd_waitlong(void);
extern void lcd_waitshort(void);
extern void lcd_write(uint8_t v);
extern void lcd_writecmd(uint8_t cmd);
extern void lcd_writedata(uint8_t d);
extern void lcd_portoff(void);
extern void lcd_portinit(void);
extern void lcd_init(void);
extern void lcd_setpos(uint8_t v);
extern void lcd_writestr(const char __generic *ch);
extern void lcd_cleardisplay(void);
extern void lcd_clear(uint8_t pos, uint8_t len);
extern __reentrantb uint8_t lcd_writenum16(uint16_t val, uint8_t nrdig, uint8_t flags) __reentrant;
extern uint8_t lcd_writenum32(uint32_t val, uint8_t nrdig, uint8_t flags);
extern __reentrantb uint8_t lcd_writehex16(uint16_t val, uint8_t nrdig, uint8_t flags) __reentrant;
extern uint8_t lcd_writehex32(uint32_t val, uint8_t nrdig, uint8_t flags);

/* legacy */
extern void lcd_writehexu16(uint16_t val, uint8_t nrdig, uint8_t pos);
extern void lcd_writehexu32(uint32_t val, uint8_t nrdig, uint8_t pos);
extern void lcd_writeu16(uint16_t val, uint8_t nrdig, uint8_t pos);
extern void lcd_writeu32(uint32_t val, uint8_t nrdig, uint8_t pos);

#endif /* LIBMFLCD_H */
