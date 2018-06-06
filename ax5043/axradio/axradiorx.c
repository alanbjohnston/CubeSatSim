// Copyright (c) 2018 Brandenburg Tech, LLC
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY BRANDENBURG TECH, LLC AND CONTRIBUTORS
// ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL BRANDENBURT TECH, LLC
// AND CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2007,2008,2009,2010,2011,2012,2013, 2014 AXSEM AG
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     1.Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     2.Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     3.Neither the name of AXSEM AG, Duebendorf nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//     4.All advertising materials mentioning features or use of this software
//       must display the following acknowledgement:
//       This product includes software developed by AXSEM AG and its contributors.
//     5.The usage of this source code is only granted for operation with AX5043
//       and AX8052F143. Porting to other radio or communication devices is
//       strictly prohibited.
//
// THIS SOFTWARE IS PROVIDED BY AXSEM AG AND CONTRIBUTORS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL AXSEM AG AND CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "axradiorx.h"

#include "../ax5043support/ax5043rx.h"
#include "../spi/ax5043spi_p.h"
#include "axradioinit.h"

extern const uint8_t axradio_phy_innerfreqloop;
extern uint8_t axradio_rxbuffer[];

static uint8_t receive_loop(void);

uint8_t receive_packet(void) {
    return receive_loop();
}

uint8_t receive_loop(void) {
    uint8_t fifo_cmd;
    uint8_t i;
    uint8_t b0 __attribute__((unused));
    uint8_t b1 __attribute__((unused));
    uint8_t b2 __attribute__((unused));
    uint8_t b3 __attribute__((unused));

    uint8_t len = ax5043ReadReg(AX5043_RADIOEVENTREQ0); // clear request so interrupt does not fire again. sync_rx enables interrupt on radio state changed in order to wake up on SDF detected

    uint8_t bytesRead = 0;

    //printf("INFO: Waiting for a packet\n");

    while ((ax5043ReadReg(AX5043_FIFOSTAT) & 0x01) != 1) { // FIFO not empty
        fifo_cmd = ax5043ReadReg(AX5043_FIFODATA); // read command
        len = (fifo_cmd & 0xE0) >> 5; // top 3 bits encode payload len
        if (len == 7)
            len = ax5043ReadReg(AX5043_FIFODATA); // 7 means variable length, -> get length byte
        fifo_cmd &= 0x1F;
        switch (fifo_cmd) {
        case AX5043_FIFOCMD_DATA:
            if (!len)
                break;

            ax5043ReadReg(AX5043_FIFODATA); // Discard the flags
            --len;
            bytesRead = ax5043_readfifo(axradio_rxbuffer, len);
        	break;

        case AX5043_FIFOCMD_RFFREQOFFS:
            if (axradio_phy_innerfreqloop || len != 3)
                goto dropchunk;
            i = ax5043ReadReg(AX5043_FIFODATA);
            i &= 0x0F;
            i |= 1 + (uint8_t)~(i & 0x08);

            b3 = ((int8_t)i) >> 8;
            b2 = i;
            b1 = ax5043ReadReg(AX5043_FIFODATA);
            b0 = ax5043ReadReg(AX5043_FIFODATA);

            //printf("INFO: RF Frequency Offset: 0x%02x%02x%02x%02x\n", b3, b2, b1, b0);
            break;

        case AX5043_FIFOCMD_FREQOFFS:
             if (!axradio_phy_innerfreqloop || len != 2)
                 goto dropchunk;

             b1 = ax5043ReadReg(AX5043_FIFODATA);
             b0 = ax5043ReadReg(AX5043_FIFODATA);

             //printf("INFO: Frequency offset: 0x%02x%02x\n", b1, b2);
             break;

        case AX5043_FIFOCMD_RSSI:
            if (len != 1)
                goto dropchunk;
            {
                int8_t r __attribute__((unused));
                r = ax5043ReadReg(AX5043_FIFODATA);

                //printf("INFO: RSSI %d\n", (int)r);
            }
            break;

        default:
        	//fprintf(stderr, "ERROR: Unknown chunk in FIFO\n");
        	dropchunk:
			//fprintf(stderr, "WARNING: Discarding chunk in FIFO\n");
            if (!len)
                break;
            i = len;
            do {
                ax5043ReadReg(AX5043_FIFODATA);        // purge FIFO
            }
            while (--i);
        	break;
        }
    }

    //printf("INFO: Done waiting for a packet\n");

    return bytesRead;
}
