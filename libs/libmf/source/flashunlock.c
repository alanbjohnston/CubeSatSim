#include "ax8052.h"
#include "libmfflash.h"

void flash_unlock(void)
{
	uint8_t __autodata iesave = IE & 0x80;
	EA = 0;
	NVKEY = 0x41;
	NVKEY = 0x78;
	IE |= iesave;
}
