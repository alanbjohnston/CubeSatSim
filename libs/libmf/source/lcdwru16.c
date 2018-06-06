#include "libmflcd.h"

void lcd_writeu16(uint16_t val, uint8_t nrdig, uint8_t pos)
{
	while (nrdig) {
		uint8_t __autodata v1 = val;
		val /= 10;
		v1 -= 10 * (uint8_t)val;
		lcd_setpos(pos);
		lcd_waitshort();
		lcd_writedata('0' + v1);
		lcd_waitshort();
		--pos;
		--nrdig;
	}
}
