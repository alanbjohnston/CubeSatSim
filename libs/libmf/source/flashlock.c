#include "ax8052.h"
#include "libmfflash.h"

void flash_lock(void)
{
	NVKEY = 0;
}
