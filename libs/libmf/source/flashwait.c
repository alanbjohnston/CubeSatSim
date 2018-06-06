#include "ax8052.h"
#include "libmfflash.h"

int8_t flash_wait(uint16_t timeout)
{
	uint8_t __autodata st;
	uint8_t __autodata cnth = timeout >> 8;
	uint8_t __autodata cntl = timeout;
	++cnth;
	++cntl;
	do {
		do {
			st = NVSTATUS;
			if (!(st & 2))
				return -2;
			if (!(st & 1))
				return 0;
			--cntl;
		} while (cntl);
		--cnth;
	} while (cnth);
	return -1;
}
