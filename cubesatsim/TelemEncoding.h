/*
 * TelemEncoding.h
 *
 *  Created on: Feb 3, 2014
 *      Author: fox
 */

#ifndef TELEMENCODING_H_
#define TELEMENCODING_H_

void update_rs(
   unsigned char parity[32], // 32-byte encoder state; zero before each frame
   unsigned char c          // Current data byte to update
);

#define CHARACTER_BITS 10
#define CHARACTERS_PER_LONGWORD 3
#define CHARACTER_MASK ((1<<CHARACTER_BITS)-1)
#define SYNC_CHARACTER -1


#define PARITY_BYTES_PER_CODEWORD 32U     // Number of parity symbols in frame
#define NP 32U //For Phil's code
#define DATA_BYTES_PER_CODE_WORD 223

#endif /* TELEMENCODING_H_ */


