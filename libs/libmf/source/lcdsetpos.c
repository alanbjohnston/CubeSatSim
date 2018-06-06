#include "libmflcd.h"

void lcd_setpos(uint8_t v)
{
	lcd_writecmd(0x80 | v);
}
