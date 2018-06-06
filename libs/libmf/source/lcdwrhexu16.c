#include "libmflcd.h"

void lcd_writehexu16(uint16_t val, uint8_t nrdig, uint8_t pos)
{
	while (nrdig) {
		uint8_t __autodata v1 = val;
		val >>= 4;
		v1 &= 0x0F;
		if (v1 >= 10)
			v1 += 'A' - '9' - 1;
		lcd_setpos(pos);
		lcd_waitshort();
		lcd_writedata('0' + v1);
		lcd_waitshort();
		--pos;
		--nrdig;
	}
}
