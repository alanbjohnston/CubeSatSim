#ifndef LIBMFUART_H
#define LIBMFUART_H

#include "libmftypes.h"

/*
 * UART
 */
extern __reentrantb void uart_timer0_baud(uint8_t clksrc, uint32_t baud, uint32_t clkfreq) __reentrant;
extern __reentrantb void uart_timer1_baud(uint8_t clksrc, uint32_t baud, uint32_t clkfreq) __reentrant;
extern __reentrantb void uart_timer2_baud(uint8_t clksrc, uint32_t baud, uint32_t clkfreq) __reentrant;

#endif /* LIBMFUART_H */
