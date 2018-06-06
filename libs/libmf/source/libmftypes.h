#ifndef LIBMFTYPES_H
#define LIBMFTYPES_H

#define LIBMFVERSION 20160321L

/*
 * Address Space Tags
 */

#if defined SDCC
#define __autodata __data
#define __auto __data
#define __generic
#define __reentrantb
#define __interruptb(x)
#endif

#if defined __CX51__ || defined __C51__
#include <intrins.h>

#define __autodata
#define __auto data
#define __code code
#define __data data
#define __xdata xdata
#define __pdata pdata
#define __generic
#define __reentrantb
#define __reentrant small reentrant
#define __naked
#define __interruptb(x)
#define __interrupt(x) interrupt x
#endif

#if defined __ICC8051__
#include <intrinsics.h>
#pragma language=extended

#define __autodata
#if !defined(__CALLING_CONVENTION__) || __CALLING_CONVENTION__ < 3
#define __auto __data
#elif __CALLING_CONVENTION__ == 3
#define __auto __pdata
#else
#define __auto __xdata
#endif
#if !defined(__CALLING_CONVENTION__) || __CALLING_CONVENTION__ < 2
#define __reentrantb __idata_reentrant
#else
#define __reentrantb
#endif
#define __reentrant
#define __naked
#define __interrupt_0  _Pragma("vector=0x03")
#define __interrupt_1  _Pragma("vector=0x0B")
#define __interrupt_2  _Pragma("vector=0x13")
#define __interrupt_3  _Pragma("vector=0x1B")
#define __interrupt_4  _Pragma("vector=0x23")
#define __interrupt_5  _Pragma("vector=0x2B")
#define __interrupt_6  _Pragma("vector=0x33")
#define __interrupt_7  _Pragma("vector=0x3B")
#define __interrupt_8  _Pragma("vector=0x43")
#define __interrupt_9  _Pragma("vector=0x4B")
#define __interrupt_10 _Pragma("vector=0x53")
#define __interrupt_11 _Pragma("vector=0x5B")
#define __interrupt_12 _Pragma("vector=0x63")
#define __interrupt_13 _Pragma("vector=0x6B")
#define __interrupt_14 _Pragma("vector=0x73")
#define __interrupt_15 _Pragma("vector=0x7B")
#define __interrupt_16 _Pragma("vector=0x83")
#define __interrupt_17 _Pragma("vector=0x8B")
#define __interrupt_18 _Pragma("vector=0x93")
#define __interrupt_19 _Pragma("vector=0x9B")
#define __interrupt_20 _Pragma("vector=0xA3")
#define __interrupt_21 _Pragma("vector=0xAB")
#define __interrupt_INT_EXTERNAL0    __interrupt_0
#define __interrupt_INT_WAKEUPTIMER  __interrupt_1
#define __interrupt_INT_EXTERNAL1    __interrupt_2
#define __interrupt_INT_GPIO         __interrupt_3
#define __interrupt_INT_RADIO        __interrupt_4
#define __interrupt_INT_CLOCKMGMT    __interrupt_5
#define __interrupt_INT_POWERMGMT    __interrupt_6
#define __interrupt_INT_TIMER0       __interrupt_7
#define __interrupt_INT_TIMER1       __interrupt_8
#define __interrupt_INT_TIMER2       __interrupt_9
#define __interrupt_INT_SPI0         __interrupt_10
#define __interrupt_INT_UART0        __interrupt_11
#define __interrupt_INT_UART1        __interrupt_12
#define __interrupt_INT_GPADC        __interrupt_13
#define __interrupt_INT_DMA          __interrupt_14
#define __interrupt_INT_OUTPUTCOMP0  __interrupt_15
#define __interrupt_INT_OUTPUTCOMP1  __interrupt_16
#define __interrupt_INT_INPUTCAPT0   __interrupt_17
#define __interrupt_INT_INPUTCAPT1   __interrupt_18
#define __interrupt_INT_DEBUGLINK    __interrupt_21
#define __interruptb(x) __interrupt_##x __interrupt
#define __interrupt(x)
#endif

/*
 * C99 size types
 */

typedef signed char int8_t;       /**< \brief C99 signed 8bit type */
typedef signed int int16_t;       /**< \brief C99 signed 16bit type */
typedef signed long int32_t;      /**< \brief C99 signed 32bit type */
typedef unsigned char uint8_t;    /**< \brief C99 unsigned 8bit type */
typedef unsigned int uint16_t;    /**< \brief C99 unsigned 16bit type */
typedef unsigned long uint32_t;   /**< \brief C99 unsigned 32bit type */

/*
 * Delay
 */
extern __reentrantb void delay(uint16_t us) __reentrant __naked;

/*
 * Random Number Generator
 */
extern uint16_t __data random_seed;
extern uint16_t random(void);

/*
 * Hamming Weight - also sometimes called population count
 */
#if defined SDCC
#pragma callee_saves hweight8,hweight16,hweight32
#pragma callee_saves signextend12,signextend16,signextend20,signextend24
extern __reentrantb int32_t signextend12(int16_t x) __reentrant __naked;
extern __reentrantb int32_t signextend16(int16_t x) __reentrant __naked;
extern __reentrantb int32_t signextend20(int32_t x) __reentrant __naked;
extern __reentrantb int32_t signextend24(int32_t x) __reentrant __naked;
#else
extern __reentrantb int32_t signextend12(int16_t x) __reentrant;
extern __reentrantb int32_t signextend16(int16_t x) __reentrant;
extern __reentrantb int32_t signextend20(int32_t x) __reentrant;
extern __reentrantb int32_t signextend24(int32_t x) __reentrant;
#endif
extern __reentrantb uint8_t hweight8(uint8_t x) __reentrant;
extern __reentrantb uint8_t hweight16(uint16_t x) __reentrant;
extern __reentrantb uint8_t hweight32(uint32_t x) __reentrant;
#define parity8(x) (hweight8(x) & 1)
#define parity16(x) (hweight16(x) & 1)
#define parity32(x) (hweight32(x) & 1)
#if defined SDCC
#pragma callee_saves signedlimit16,checksignedlimit16,signedlimit32,checksignedlimit32
#pragma callee_saves gray_encode8,gray_decode8
extern __reentrantb int16_t signedlimit16(int16_t x, int16_t lim) __reentrant __naked;
extern __reentrantb uint8_t checksignedlimit16(int16_t x, int16_t lim) __reentrant __naked;
extern __reentrantb int32_t signedlimit32(int32_t x, int32_t lim) __reentrant __naked;
extern __reentrantb uint8_t checksignedlimit32(int32_t x, int32_t lim) __reentrant __naked;
extern __reentrantb uint8_t gray_encode8(uint8_t x) __reentrant __naked;
extern __reentrantb uint8_t gray_decode8(uint8_t x) __reentrant __naked;
#else
extern __reentrantb int16_t signedlimit16(int16_t x, int16_t lim) __reentrant;
extern __reentrantb uint8_t checksignedlimit16(int16_t x, int16_t lim) __reentrant;
extern __reentrantb int32_t signedlimit32(int32_t x, int32_t lim) __reentrant;
extern __reentrantb uint8_t checksignedlimit32(int32_t x, int32_t lim) __reentrant;
extern __reentrantb uint8_t gray_encode8(uint8_t x) __reentrant;
extern __reentrantb uint8_t gray_decode8(uint8_t x) __reentrant;
#endif

/*
 * Reverse Bits
 */
extern __reentrantb uint8_t rev8(uint8_t x) __reentrant;

/*
 * fast memset and memcpy
 */

#if defined __ICC8051__
__idata_reentrant void fmemset(void __generic *p, char c, uint16_t n);
__idata_reentrant void fmemcpy(void __generic *d, const void __generic *s, uint16_t n);
#else
__reentrantb void fmemset(void __generic *p, char c, uint16_t n) __reentrant;
__reentrantb void fmemcpy(void __generic *d, const void __generic *s, uint16_t n) __reentrant;
#endif

/*
 * Power Management
 */

#if defined SDCC
#define nop() do { __asm nop __endasm; } while (0)
#elif defined __CX51__ || defined __C51__
#define nop() do { _nop_(); } while (0)
#elif defined __ICC8051__
#define nop() do { __no_operation(); } while (0)
#endif

#if defined SDCC
#pragma callee_saves wtimer_standby,enter_standby,enter_sleep,enter_deepsleep
#endif
extern __reentrantb void wtimer_standby(void) __reentrant;
extern __reentrantb void enter_standby(void) __reentrant;
extern __reentrantb void enter_sleep(void) __reentrant;
#if defined(SDCC) || defined(__ICC8051__)
extern __reentrantb void enter_sleep_cont(void) __reentrant;
#endif
extern __reentrantb void enter_deepsleep(void) __reentrant;
extern __reentrantb void reset_cpu(void) __reentrant;
extern __reentrantb void turn_off_xosc(void) __reentrant;
extern __reentrantb void turn_off_lpxosc(void) __reentrant;

/*
 * Clock Sources
 */
#define CLKSRC_FRCOSC   0
#define CLKSRC_LPOSC    1
#define CLKSRC_XOSC     2
#define CLKSRC_LPXOSC   3
#define CLKSRC_RSYSCLK  4
#define CLKSRC_TCLK     5
#define CLKSRC_SYSCLK   6
#define CLKSRC_OFF      7

/*
 * wrnum<xx> Flags
 */
#define WRNUM_SIGNED   0x01
#define WRNUM_PLUS     0x02
#define WRNUM_ZEROPLUS 0x04
#define WRNUM_PADZERO  0x08
#define WRNUM_TSDSEP   0x10
#define WRNUM_LCHEX    0x20

/*
 * IAR Stacks
 */
#if defined __ICC8051__
extern __reentrantb void __pdata *getpsp(void) __reentrant;
extern __reentrantb void __xdata *getxsp(void) __reentrant;
#endif

#endif /* LIBMFTYPES_H */
