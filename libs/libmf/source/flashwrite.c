#include "ax8052.h"
#include "libmfflash.h"

int8_t flash_write(uint16_t waddr, uint16_t wdata)
{
	NVADDR0 = waddr;
	NVADDR1 = waddr >> 8;
	NVDATA0 = wdata;
	NVDATA1 = wdata >> 8;
	NVSTATUS = 0x30;
	return flash_wait(128);
}
