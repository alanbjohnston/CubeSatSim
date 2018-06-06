#ifndef LIBMFBCH_H
#define LIBMFBCH_H

#include "libmftypes.h"

/*
 * BCH(31,21,5) Code (with and without parity) routines
 * Data Format:
 *   Bits 31-11: Data
 *   Bits 10- 1: BCH Parity Bits
 *   Bit      0: Even Parity Bit
 */

extern uint16_t bch3121_syndrome(uint32_t cw);
extern uint32_t bch3121_encode(uint32_t cw);
extern uint32_t bch3121_encode_parity(uint32_t cw);

/* Bit 0 of the return value indicates decoding error */
extern uint32_t bch3121_decode(uint32_t cw);
extern uint32_t bch3121_decode_parity(uint32_t cw);

/*
 * BCH Internals
 */
extern const uint16_t __code bch3121_syndrometable[1024];

#endif /* LIBMFBCH_H */
