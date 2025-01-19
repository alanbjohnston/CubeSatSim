/*    AO40 encoder / decoder
 *    Copyright 2002 Phil Karn, KA9Q
 *    May be used under the terms of the GNU General Public License (GPL) 
 *    See CodecAO40.cpp for lineage
 *
 *    This file is part of FUNcubeLib.
 *
 *    FUNcubeLib is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    FUNcubeLib is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with FUNcubeLib If not, see <http://www.gnu.org/licenses/>.
 *
*/

#pragma once

#include "fecConstants.h"

//class CCodecAO40
//{
//public:
//	CCodecAO40(void);
//    virtual ~CCodecAO40(void);

    int decode(unsigned char viterbi_decoded[NBITS_OUT], unsigned char *decoded_data);

    /* Encodes the 256 byte source block into 5200 byte block of symbols into m_encoded buffer */
     const unsigned char *encode(
        unsigned char *source_bytes,  /* input to encode */    
        int byte_count);              /* input length in bytes */
    
    /* Compares raw input symbols to current buffer of encoded symbols and counts the errors */            
    int count_errors( unsigned char *raw_symbols);        

//private:
    int mod255(int x);
    int decode_rs_8(char *data, int *eras_pos, int no_eras);
    void scramble_and_encode(unsigned char c);
    void encode_and_interleave(unsigned char c,int cnt);

    void descramble_to_rsblocks(
        unsigned char viterbi_decoded[NBITS_OUT],
        char rsblocks[RSBLOCKS][NN]);

    void init_encoder(void);
    void encode_byte(unsigned char c);
    void encode_parity(void);
    
    void interleave_symbol(int c);

    int m_encoded_bytes;               /* Byte counter for encode_data() */
    int m_ileaver_index;               /* Byte counter for interleaver */
    unsigned char m_conv_sr;           /* Convolutional encoder shift register state */

    unsigned char m_RS_block[RSBLOCKS][NROOTS]; /* RS parity blocks */
    unsigned char m_encoded[SYMPBLOCK] ;       /* encoded symbols */
//};
