#include "libmflcd.h"

void lcd_writestr(const char __generic *ch)
{
	for (;;) {
		char __autodata c = *ch++;
		if (!c)
			break;
		if (c == '\n') {
			lcd_writecmd(0xc0);
		} else {
			lcd_writedata(c);
		}
		lcd_waitshort();
	}
}
