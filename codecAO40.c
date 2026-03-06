#include <string.h>
#include "codecAO40.h"

/* ---------------------- */
/* AO40 Encoder - Decoder */
/* ---------------------- */

/* Scramble and RS encode 256 byte blocks of data into 5200 bits
 *  or Descramble and RS decode 5200 bits into the 256 bytes of data
 *
 * --------------------------------------------------------------------------
 * This decoder has evolved extensively through the work of Phil Karn.  It draws
 * on his own ideas and optimisations, and on the work of others.  The lineage
 * is as below, and parts of the authors' notices are included here.  (JRM)

 * AO40 encoder / decoder
 * Copyright 2002 Phil Karn, KA9Q
 * May be used under the terms of the GNU General Public License (GPL)
 *
 * Reed-Solomon coding and decoding
 * Phil Karn (karn@ka9q.ampr.org) September 1996
 *
 * This file is derived from the program "new_rs_erasures.c" by Robert
 * Morelos-Zaragoza (robert@spectra.eng.hawaii.edu) and Hari Thirumoorthy
 * (harit@spectra.eng.hawaii.edu), Aug 1995
 * --------------------------------------------------------------------------
 *
 * From the RM-Z & HT program:
 * The encoding and decoding methods are based on the
 * book "Error Control Coding: Fundamentals and Applications",
 * by Lin and Costello, Prentice Hall, 1983, ISBN 0-13-283796-X
 * Portions of this program are from a Reed-Solomon encoder/decoder
 * in C, written by Simon Rockliff (simon@augean.ua.oz.au) on 21/9/89.
 * --------------------------------------------------------------------------
 *
 * From the 1989/1991 SR program (also based on Lin and Costello):
 * This program may be freely modified and/or given to whoever wants it.
 * A condition of such distribution is that the author's contribution be
 * acknowledged by his name being left in the comments heading the program,
 *                               Simon Rockliff, 26th June 1991
 *
 */

/* Defines for RS Decoder(s) */
#ifndef min
#define min(a,b)        ((a) < (b) ? (a) : (b))
#endif

/*
CCodecAO40::CCodecAO40(void)
{
}


CCodecAO40::~CCodecAO40(void)
{
}

*/
//int CCodecAO40::mod255(int x) {
int mod255(int x) {
    while (x >= 255) {
        x -= 255;
        x = (x >> 8) + (x & 255);
    }
    return x;
}

//int CCodecAO40::decode_rs_8(char *data, int *eras_pos, int no_eras){
int decode_rs_8(char *data, int *eras_pos, int no_eras){

    int deg_lambda, el, deg_omega;
    int i, j, r,k;
    unsigned char u,q,tmp,num1,num2,den,discr_r;
    unsigned char lambda[NROOTS+1], s[NROOTS];   /* Err+Eras Locator poly and syndrome poly */
    unsigned char b[NROOTS+1], t[NROOTS+1], omega[NROOTS+1];
    unsigned char root[NROOTS], reg[NROOTS+1], loc[NROOTS];
    int syn_error, count;

    /* form the syndromes; i.e., evaluate data(x) at roots of g(x) */
    for(i=0;i<NROOTS;i++)
        s[i] = data[0];

    for(j=1;j<NN;j++){
        for(i=0;i<NROOTS;i++){
            if(s[i] == 0){
                s[i] = data[j];
            } else {
                s[i] = data[j] ^ ALPHA_TO[mod255(INDEX_OF[s[i]] + (FCR+i)*PRIM)];
            }
        }
    }

    /* Convert syndromes to index form, checking for nonzero condition */
    syn_error = 0;
    for(i=0;i<NROOTS;i++){
        syn_error |= s[i];
        s[i] = INDEX_OF[s[i]];
    }

    if (!syn_error) {
        /* if syndrome is zero, data[] is a codeword and there are no
        * errors to correct. So return data[] unmodified
        */
        count = 0;
        goto finish;
    }
    memset(&lambda[1],0,NROOTS*sizeof(lambda[0]));
    lambda[0] = 1;

    if (no_eras > 0) {
        /* Init lambda to be the erasure locator polynomial */
        lambda[1] = ALPHA_TO[mod255(PRIM*(NN-1-eras_pos[0]))];
        for (i = 1; i < no_eras; i++) {
            u = mod255(PRIM*(NN-1-eras_pos[i]));
            for (j = i+1; j > 0; j--) {
                tmp = INDEX_OF[lambda[j - 1]];
                if(tmp != A0)
                    lambda[j] ^= ALPHA_TO[mod255(u + tmp)];
            }
        }
    }
    for(i=0;i<NROOTS+1;i++)
        b[i] = INDEX_OF[lambda[i]];

    /*
    * Begin Berlekamp-Massey algorithm to determine error+erasure
    * locator polynomial
    */
    r = no_eras;
    el = no_eras;
    while (++r <= NROOTS) {       /* r is the step number */
        /* Compute discrepancy at the r-th step in poly-form */
        discr_r = 0;
        for (i = 0; i < r; i++){
            if ((lambda[i] != 0) && (s[r-i-1] != A0)) {
                discr_r ^= ALPHA_TO[mod255(INDEX_OF[lambda[i]] + s[r-i-1])];
            }
        }
        discr_r = INDEX_OF[discr_r];        /* Index form */
        if (discr_r == A0) {
            /* 2 lines below: B(x) <-- x*B(x) */
            memmove(&b[1],b,NROOTS*sizeof(b[0]));
            b[0] = A0;
        } else {
            /* 7 lines below: T(x) <-- lambda(x) - discr_r*x*b(x) */
            t[0] = lambda[0];
            for (i = 0 ; i < NROOTS; i++) {
                if(b[i] != A0)
                    t[i+1] = lambda[i+1] ^ ALPHA_TO[mod255(discr_r + b[i])];
                else
                    t[i+1] = lambda[i+1];
            }
            if (2 * el <= r + no_eras - 1) {
                el = r + no_eras - el;
                /*
                * 2 lines below: B(x) <-- inv(discr_r) *
                * lambda(x)
                */
                for (i = 0; i <= NROOTS; i++)
                    b[i] = (lambda[i] == 0) ? A0 : mod255(INDEX_OF[lambda[i]] - discr_r + NN);
            } else {
                /* 2 lines below: B(x) <-- x*B(x) */
                memmove(&b[1],b,NROOTS*sizeof(b[0]));
                b[0] = A0;
            }
            memcpy(lambda,t,(NROOTS+1)*sizeof(t[0]));
        }
    }

    /* Convert lambda to index form and compute deg(lambda(x)) */
    deg_lambda = 0;
    for(i=0;i<NROOTS+1;i++){
        lambda[i] = INDEX_OF[lambda[i]];
        if(lambda[i] != A0)
            deg_lambda = i;
    }
    /* Find roots of the error+erasure locator polynomial by Chien search */
    memcpy(&reg[1],&lambda[1],NROOTS*sizeof(reg[0]));
    count = 0;            /* Number of roots of lambda(x) */
    for (i = 1,k=IPRIM-1; i <= NN; i++,k = mod255(k+IPRIM)) {
        q = 1; /* lambda[0] is always 0 */
        for (j = deg_lambda; j > 0; j--){
            if (reg[j] != A0) {
                reg[j] = mod255(reg[j] + j);
                q ^= ALPHA_TO[reg[j]];
            }
        }
        if (q != 0)
            continue; /* Not a root */
        /* store root (index-form) and error location number */
        root[count] = i;
        loc[count] = k;
        /* If we've already found max possible roots,
        * abort the search to save time
        */
        if(++count == deg_lambda)
            break;
    }
    if (deg_lambda != count) {
        /*
        * deg(lambda) unequal to number of roots => uncorrectable
        * error detected
        */
        count = -1;
        goto finish;
    }
    /*
    * Compute err+eras evaluator poly omega(x) = s(x)*lambda(x) (modulo
    * x**NROOTS). in index form. Also find deg(omega).
    */
    deg_omega = 0;
    for (i = 0; i < NROOTS;i++){
        tmp = 0;
        j = (deg_lambda < i) ? deg_lambda : i;
        for(;j >= 0; j--){
            if ((s[i - j] != A0) && (lambda[j] != A0))
                tmp ^= ALPHA_TO[mod255(s[i - j] + lambda[j])];
        }
        if(tmp != 0)
            deg_omega = i;
        omega[i] = INDEX_OF[tmp];
    }
    omega[NROOTS] = A0;

    /*
    * Compute error values in poly-form. num1 = omega(inv(X(l))), num2 =
    * inv(X(l))**(FCR-1) and den = lambda_pr(inv(X(l))) all in poly-form
    */
    for (j = count-1; j >=0; j--) {
        num1 = 0;
        for (i = deg_omega; i >= 0; i--) {
            if (omega[i] != A0)
                num1  ^= ALPHA_TO[mod255(omega[i] + i * root[j])];
        }
        num2 = ALPHA_TO[mod255(root[j] * (FCR - 1) + NN)];
        den = 0;

        /* lambda[i+1] for i even is the formal derivative lambda_pr of lambda[i] */
        for (i = min(deg_lambda,NROOTS-1) & ~1; i >= 0; i -=2) {
            if(lambda[i+1] != A0)
                den ^= ALPHA_TO[mod255(lambda[i+1] + i * root[j])];
        }
        if (den == 0) {
            count = -1;
            goto finish;
        }
        /* Apply error to data */
        if (num1 != 0) {
            data[loc[j]] ^= ALPHA_TO[mod255(INDEX_OF[num1] + INDEX_OF[num2] + NN - INDEX_OF[den])];
        }
    }
finish:
    if(eras_pos != NULL){
        for(i=0;i<count;i++)
            eras_pos[i] = loc[i];
    }
    return count;
}

/* ---------- */
/* Re-encoder */
/* ---------- */

/* Reference encoder for proposed coded AO-40 telemetry format - v1.0  7 Jan 2002
* Copyright 2002, Phil Karn, KA9Q
* This software may be used under the terms of the GNU Public License (GPL)
*/

/* Adapted from  the above enc_ref.c  as used by the spacecraft (JRM) */


/* Write one binary channel symbol into the interleaver frame and update the pointers */
//void CCodecAO40::interleave_symbol(int c)
void interleave_symbol(int c)
{
    int row,col;
    col=m_ileaver_index/COLUMNS;
    row=m_ileaver_index%COLUMNS;
    if(c)
    {
        m_encoded[row*ROWS+col] = 1;
    }
    m_ileaver_index++;
}

/* Convolutionally encode and interleave one byte */
//void CCodecAO40::encode_and_interleave(unsigned char c,int cnt){
void encode_and_interleave(unsigned char c,int cnt){
    while(cnt-- != 0)
    {
        m_conv_sr = (m_conv_sr << 1) | (c >> 7);
        c <<= 1;
        interleave_symbol( Partab[m_conv_sr & CPOLYA]);
        interleave_symbol(!Partab[m_conv_sr & CPOLYB]); /* Second encoder symbol is inverted */
    }
}

/* Scramble a byte, convolutionally encode and interleave into frame */
//void CCodecAO40::scramble_and_encode(unsigned char c){
void scramble_and_encode(unsigned char c){
    c ^= Scrambler[m_encoded_bytes]; /* Scramble byte */
    encode_and_interleave(c,8); /* RS encode and place into reencode buffer */
}

/* Encodes the 256 byte source block RSdecdata[] into 5200 byte block of symbols
*  results stored in m_encoded.
*  On success encoded buffer is returned, an zeroed buffer on failure
*/
//const unsigned char *CCodecAO40::encode(unsigned char *source_bytes, int byte_count)
const unsigned char *encode(unsigned char *source_bytes, int byte_count)
{
    if(BLOCKSIZE != byte_count || NULL == source_bytes )
    {
		memset(m_encoded, 0, BLOCKSIZE);
        return m_encoded;
    }

    init_encoder();

    for(int i=0;i<BLOCKSIZE;i++)
    {
        encode_byte(source_bytes[i]) ;
    }

    for(int i=0;i<64;i++)
    {
        encode_parity();
    }    

    return m_encoded;
}

/* The original three C user's entry points now follow.  They are:

init_encoder()                   Called once before using system.
encode_byte(unsigned char c)     Called with each user byte (i.e. 256 calls)
encode_parity()                  Called 64 times to finish off

*/

/* This function initializes the encoder. */
//void CCodecAO40::init_encoder(void){
void init_encoder(void){
    int i,j,sr;

    m_encoded_bytes  = 0;
    m_conv_sr = 0;
    m_ileaver_index  = COLUMNS;   /* Sync vector is in first column; data starts here */

    for(j=0;j<RSBLOCKS;j++)       /* Flush parity array */
    {
        for(i=0;i<NROOTS;i++)
        {
            m_RS_block[j][i] = 0;
        }
    }

    /* Clear re-encoded array */
    for(i=0;i<SYMPBLOCK;i++)
    {
        m_encoded[i] = 0;
    }

    /* Generate sync vector, interleave into re-encode array, 1st column */
    sr = 0x7f;
    for(i=0;i<65;i++)
    {
        if(sr & 64)
        {
            m_encoded[ROWS*i] = 1;      /* Every 80th symbol is a sync bit */
        }
        sr = (sr << 1) | Partab[sr & SYNC_POLY];
    }
}

/* This function is called with each user data byte to be encoded into the
* current frame. It should be called in sequence 256 times per frame, followed
* by 64 calls to encode_parity().
*/
//void CCodecAO40::encode_byte(unsigned char c){
void encode_byte(unsigned char c){
    unsigned char *rp;                /* RS block pointer */
    int i;
    unsigned char feedback;

    /* Update the appropriate Reed-Solomon codeword */
    rp = m_RS_block[m_encoded_bytes & 1];

    /* Compute feedback term */
    feedback = INDEX_OF[c ^ rp[0]];

    /* If feedback is non-zero, multiply by each generator polynomial coefficient and
    * add to corresponding shift register elements
    */
    if(feedback != A0){
        int j;

        /* This loop exploits the palindromic nature of the generator polynomial
        * to halve the number of discrete multiplications
        */
        for(j=0;j<15;j++){
            unsigned char t;

            t = ALPHA_TO[mod255(feedback + RS_poly[j])];
            rp[j+1] ^= t; rp[31-j] ^= t;
        }
        rp[16] ^= ALPHA_TO[mod255(feedback + RS_poly[15])];
    }

    /* Shift 32 byte RS register one position down */
    for(i=0;i<31;i++)
        rp[i] = rp[i+1];

    /* Handle highest order coefficient, which is unity */
    if(feedback != A0){
        rp[31] = ALPHA_TO[feedback];
    } else {
        rp[31] = 0;
    }
    scramble_and_encode(c);
    m_encoded_bytes++;
}

/* This function should be called 64 times after the 256 data bytes
* have been passed to update_encoder. Each call scrambles, encodes and
* interleaves one byte of Reed-Solomon parity.
*/
//void CCodecAO40::encode_parity(void) {
void encode_parity(void) {
    unsigned char c;

    c =  m_RS_block[m_encoded_bytes & 1][(m_encoded_bytes-256)>>1];
    scramble_and_encode(c);
    if(++m_encoded_bytes == 320){
        /* Tail off the convolutional encoder (flush) */
        encode_and_interleave(0,6);
    }
}



//void CCodecAO40::descramble_to_rsblocks(unsigned char viterbi_decoded[NBITS_OUT],    char rsblocks[RSBLOCKS][NN])
void descramble_to_rsblocks(unsigned char viterbi_decoded[NBITS_OUT],    char rsblocks[RSBLOCKS][NN])
{
    /* interleave into Reed Solomon codeblocks */
    memset(rsblocks,0,RSBLOCKS*NN);  /* Zero rsblocks array */
    int di = 0;
    int si = 0;
    for(int col=RSPAD;col<NN;col++)
    {
        for(int row=0;row<RSBLOCKS;row++)
        {
            rsblocks[row][col] = viterbi_decoded[di++] ^ Scrambler[si++];  /* Remove scrambling */
        }
    }
}

/* -------- */
/* Decoder  */
/* -------- */

    /* ------------------- */

    /* There are two RS decoders, processing 128 bytes each.
    *
    * If both RS decoders are SUCCESSFUL
    * On exit:
    *   rs_failures = 0
    *   rserrs[x]   = number of errors corrected;  range 0 to 16   (x= 0 or 1)
    *   Data output is in array RSdecdata[256].
    *
    * If an RS decoder FAILS
    * On exit:
    *   rs_failures = 1 or 2   (i.e. != 0)
    *   rserrs[x] contains -1
    *   Data output should not be used.
    */
//int CCodecAO40::decode(unsigned char vitdecdata[NBITS_OUT], unsigned char *decoded_data)
int decode(unsigned char vitdecdata[NBITS_OUT], unsigned char *decoded_data)
{    
    int row, col, row_errors, total_errors;    
    char rsblocks[RSBLOCKS][NN];

    descramble_to_rsblocks(vitdecdata, rsblocks);        

    /* Run RS-decoder(s) */
    row_errors = total_errors = 0;
    for(row=0; row<RSBLOCKS && row_errors!= -1; row++)
    {
        // decode row, returns -1 on failure or number of corrected errors
        row_errors = decode_rs_8(rsblocks[row],NULL,0);
        total_errors += row_errors;
    }

    if(row_errors != -1)
    {
        /* if frame decoded OK, deinterleave data from RS codeword(s) */
        int j = 0;
        for(col=RSPAD;col<KK;col++)
        {
            for(row=0;row<RSBLOCKS;row++)
            {
                decoded_data[j++] = rsblocks[row][col];
            }
        }
    }
    else
    {
        total_errors = -1;
    }

    return total_errors;
}

/* Compairs raw input symbols to current buffer of encoded symbols and counts the errors */            
//int CCodecAO40::count_errors(unsigned char *raw_symbols)       
int count_errors(unsigned char *raw_symbols)       
{  
    int error_count = 0;
    for(int i=0;i<SYMPBLOCK;i++)
    {
        if ( m_encoded[i] != (raw_symbols[i]>>7) ) 
        {
            error_count++ ;
        }
    }
    return error_count;
}
