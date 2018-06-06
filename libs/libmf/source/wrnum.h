#ifndef WRNUM_H
#define WRNUM_H

#include "libmftypes.h"

#define WRNUM_MASK    (WRNUM_SIGNED | WRNUM_PLUS | WRNUM_ZEROPLUS | WRNUM_PADZERO | WRNUM_TSDSEP | WRNUM_LCHEX)
#define WRNUM_DIGCONT 0x80
#define WRNUM_DIGSET  0x40

#if defined SDCC
#pragma callee_saves libmf_num16_digit,libmf_num32_digit

extern __reentrantb uint32_t libmf_num32_digit(uint32_t v, uint8_t __auto *dp) __reentrant;
#else
extern uint32_t libmf_num32_digit(uint32_t v, uint8_t __auto *dp);
#endif

extern __reentrantb uint16_t libmf_num16_digit(uint16_t v, uint8_t __auto *dp) __reentrant;

#endif /* WRNUM_H */
