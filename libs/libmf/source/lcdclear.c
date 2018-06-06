#include "libmflcd.h"

void lcd_clear(uint8_t pos, uint8_t len)
{
	uint8_t __autodata x;
	lcd_setpos(pos);
	if (!len)
		return;
	x = pos & 0x3f;
	if (x >= 0x10)
		return;
	x = 0x10 - x;
	if (len > x)
		len = x;
	do {
		lcd_waitshort();
		lcd_writedata(' ');
	} while (--len);
}
