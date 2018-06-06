#include "ax8052.h"
#include "libmfflash.h"

int8_t flash_pageerase(uint16_t pgaddr)
{
	NVADDR0 = pgaddr;
	NVADDR1 = pgaddr >> 8;
	NVSTATUS = 0x20;
	return flash_wait(65535);
}
