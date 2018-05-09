// Copyright (c) 2018 Brandenburg Tech, LLC

#ifndef CRC_H_
#define CRC_H_

#include <stdint.h>

uint16_t crc_crc16(const uint8_t *buf, uint16_t buflen, uint16_t crc);
uint16_t crc_crc16_byte(uint16_t crc, uint8_t c);
uint16_t pn9_buffer(uint8_t *buf, uint16_t buflen, uint16_t pn9, uint8_t xor);
uint16_t pn9_advance_bits(uint16_t pn9, uint16_t bits);
uint16_t pn9_advance_byte(uint16_t pn9);



#endif /* CRC_H_ */
