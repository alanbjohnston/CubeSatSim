#ifndef LIBMFFLASH_H
#define LIBMFFLASH_H

#include "libmftypes.h"

/*
 * FLASH
 */
extern void flash_unlock(void);
extern void flash_lock(void);
extern int8_t flash_wait(uint16_t timeout);
extern int8_t flash_pageerase(uint16_t pgaddr);
extern int8_t flash_write(uint16_t waddr, uint16_t wdata);
extern uint16_t flash_read(uint16_t raddr);

extern uint8_t flash_apply_calibration(void);
typedef uint8_t flash_deviceid_t[6];
#if defined(SDCC)
static __xdata flash_deviceid_t __at(0xfc06) flash_deviceid;
#elif defined __CX51__ || defined __C51__
static flash_deviceid_t xdata flash_deviceid _at_ 0xfc06;
#elif defined __ICC8051__
static __xdata __no_init flash_deviceid_t flash_deviceid @ 0xfc06;
#endif

#endif /* LIBMFFLASH_H */
