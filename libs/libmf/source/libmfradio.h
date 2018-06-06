#ifndef LIBMFRADIO_H
#define LIBMFRADIO_H

#include "libmftypes.h"

#if defined(SDCC)
#pragma callee_saves radio_read16,radio_read24,radio_read32,radio_write16,radio_write24,radio_write32
#endif
extern __reentrantb uint16_t radio_read16(uint16_t addr) __reentrant;
extern __reentrantb uint32_t radio_read24(uint16_t addr) __reentrant;
extern __reentrantb uint32_t radio_read32(uint16_t addr) __reentrant;
extern __reentrantb void radio_write16(uint16_t addr, uint16_t d) __reentrant;
extern __reentrantb void radio_write24(uint16_t addr, uint32_t d) __reentrant;
extern __reentrantb void radio_write32(uint16_t addr, uint32_t d) __reentrant;

/*
 * AX5031
 */
extern __reentrantb void ax5031_comminit(void) __reentrant;
extern __reentrantb void ax5031_commsleepexit(void) __reentrant;
extern __reentrantb uint8_t ax5031_reset(void) __reentrant;
extern __reentrantb void ax5031_rclk_enable(uint8_t div) __reentrant;
extern __reentrantb void ax5031_rclk_disable(void) __reentrant;
extern __reentrantb void ax5031_readfifo(uint8_t __generic *ptr, uint8_t len) __reentrant;
extern __reentrantb void ax5031_writefifo(const uint8_t __generic *ptr, uint8_t len) __reentrant;

/*
 * AX5042
 */
extern __reentrantb void ax5042_comminit(void) __reentrant;
extern __reentrantb void ax5042_commsleepexit(void) __reentrant;
extern __reentrantb uint8_t ax5042_reset(void) __reentrant;
extern __reentrantb void ax5042_rclk_enable(uint8_t div) __reentrant;
extern __reentrantb void ax5042_rclk_disable(void) __reentrant;
extern __reentrantb void ax5042_readfifo(uint8_t __generic *ptr, uint8_t len) __reentrant;
extern __reentrantb void ax5042_writefifo(const uint8_t __generic *ptr, uint8_t len) __reentrant;

/*
 * AX5043
 */
extern __reentrantb void ax5043_comminit(void) __reentrant;
extern __reentrantb void ax5043_commsleepexit(void) __reentrant;
extern __reentrantb uint8_t ax5043_reset(void) __reentrant;
extern __reentrantb void ax5043_enter_deepsleep(void) __reentrant;
extern __reentrantb uint8_t ax5043_wakeup_deepsleep(void) __reentrant;
extern __reentrantb void ax5043_rclk_enable(uint8_t div) __reentrant;
extern __reentrantb void ax5043_rclk_disable(void) __reentrant;
extern __reentrantb void ax5043_readfifo(uint8_t __generic *ptr, uint8_t len) __reentrant;
extern __reentrantb void ax5043_writefifo(const uint8_t __generic *ptr, uint8_t len) __reentrant;

/*
 * AX5051
 */
extern __reentrantb void ax5051_comminit(void) __reentrant;
extern __reentrantb void ax5051_commsleepexit(void) __reentrant;
extern __reentrantb uint8_t ax5051_reset(void) __reentrant;
extern __reentrantb void ax5051_rclk_enable(uint8_t div) __reentrant;
extern __reentrantb void ax5051_rclk_disable(void) __reentrant;
extern __reentrantb void ax5051_readfifo(uint8_t __generic *ptr, uint8_t len) __reentrant;
extern __reentrantb void ax5051_writefifo(const uint8_t __generic *ptr, uint8_t len) __reentrant;

#endif /* LIBMFRADIO_H */
