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

#include "axradiotx.h"

//#include <process.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../ax5043support/ax5043tx.h"
#include "../crc/crc.h"
#include "../generated/config.h"
#include "../spi/ax5043spi_p.h"
#include "axradioinit.h"
#include "axradioinit_p.h"
#include "axradiomode.h"

static void transmit_loop(axradio_trxstate_t axradio_trxstate, uint16_t axradio_txbuffer_len,
		uint8_t axradio_txbuffer[], uint16_t axradio_txbuffer_cnt);

extern const uint16_t axradio_phy_preamble_len;
extern const uint8_t axradio_phy_preamble_flags;
extern const uint8_t axradio_phy_preamble_byte;
extern const uint8_t axradio_phy_preamble_appendbits;
extern const uint8_t axradio_phy_preamble_appendpattern;
extern const uint16_t axradio_phy_preamble_longlen;
extern const uint8_t axradio_phy_pn9;

extern const uint8_t axradio_framing_synclen;
extern const uint8_t axradio_framing_syncflags;
extern const uint8_t axradio_framing_syncword[];

extern const uint8_t axradio_framing_maclen;
extern const uint8_t axradio_framing_addrlen;
extern const uint8_t axradio_framing_destaddrpos;
extern const uint8_t axradio_framing_sourceaddrpos;
extern const uint8_t axradio_framing_lenpos;
extern const uint8_t axradio_framing_lenoffs;
extern const uint8_t axradio_framing_lenmask;
extern const uint8_t axradio_framing_swcrclen;

uint8_t transmit_packet(const struct axradio_address *addr, const uint8_t *pkt, uint16_t pktlen) {
	axradio_trxstate_t axradio_trxstate;
	uint16_t axradio_txbuffer_len;
	uint8_t axradio_txbuffer[PKTDATA_BUFLEN];
	struct axradio_address_mask axradio_localaddr;
	uint16_t axradio_txbuffer_cnt = 0;

    axradio_txbuffer_len = pktlen + axradio_framing_maclen;
    if (axradio_txbuffer_len > sizeof(axradio_txbuffer))
        return AXRADIO_ERR_INVALID;
    memset(axradio_txbuffer, 0, axradio_framing_maclen);
    memcpy(&axradio_txbuffer[axradio_framing_maclen], pkt, pktlen);
    if (axradio_framing_destaddrpos != 0xff)
        memcpy(&axradio_txbuffer[axradio_framing_destaddrpos], &addr->addr, axradio_framing_addrlen);
    if (axradio_framing_sourceaddrpos != 0xff)
        memcpy(&axradio_txbuffer[axradio_framing_sourceaddrpos], &axradio_localaddr.addr, axradio_framing_addrlen);
    if (axradio_framing_lenmask) {
        uint8_t len_byte = (uint8_t)(axradio_txbuffer_len - axradio_framing_lenoffs) & axradio_framing_lenmask; // if you prefer not counting the len byte itself, set LENOFFS = 1
        axradio_txbuffer[axradio_framing_lenpos] = (axradio_txbuffer[axradio_framing_lenpos] & (uint8_t)~axradio_framing_lenmask) | len_byte;
    }
    if (axradio_framing_swcrclen)
        axradio_txbuffer_len = axradio_framing_append_crc(axradio_txbuffer, axradio_txbuffer_len);
    if (axradio_phy_pn9)
        pn9_buffer(axradio_txbuffer, axradio_txbuffer_len, 0x1ff, -(ax5043ReadReg(AX5043_ENCODING) & 0x01));
    axradio_txbuffer_cnt = axradio_phy_preamble_longlen;

    ax5043_prepare_tx();

    ax5043ReadReg(AX5043_RADIOEVENTREQ0); // make sure REVRDONE bit is cleared, so it is a reliable indicator that the packet is out
    ax5043WriteReg(AX5043_FIFOSTAT, 3); // clear FIFO data & flags (prevent transmitting anything left over in the FIFO, this has no effect if the FIFO is not powerered, in this case it is reset any way)
    axradio_trxstate = trxstate_tx_longpreamble;

    if ((ax5043ReadReg(AX5043_MODULATION) & 0x0F) == 9) { // 4-FSK
        ax5043WriteReg(AX5043_FIFODATA, AX5043_FIFOCMD_DATA | (7 << 5));
        ax5043WriteReg(AX5043_FIFODATA, 2);  // length (including flags)
        ax5043WriteReg(AX5043_FIFODATA, 0x01);  // flag PKTSTART -> dibit sync
        ax5043WriteReg(AX5043_FIFODATA, 0x11); // dummy byte for forcing dibit sync
    }
    transmit_loop(axradio_trxstate, axradio_txbuffer_len, axradio_txbuffer, axradio_txbuffer_cnt);
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_FULL_TX);

    ax5043ReadReg(AX5043_RADIOEVENTREQ0);
    //printf("INFO: Waiting for transmission to complete\n");
    while (ax5043ReadReg(AX5043_RADIOSTATE) != 0) {
        usleep(1000);
    }
    //printf("INFO: Transmission complete\n");

    ax5043WriteReg(AX5043_RADIOEVENTMASK0, 0x00);

    return AXRADIO_ERR_NOERROR;
}

static void transmit_loop(axradio_trxstate_t axradio_trxstate, uint16_t axradio_txbuffer_len,
		uint8_t axradio_txbuffer[], uint16_t axradio_txbuffer_cnt)
{

    for (;;) {
        uint8_t cnt = ax5043ReadReg(AX5043_FIFOFREE0);

        if (ax5043ReadReg(AX5043_FIFOFREE1))
            cnt = 0xff;

        switch (axradio_trxstate) {
        case trxstate_tx_longpreamble:
            if (!axradio_txbuffer_cnt) {
                axradio_trxstate = trxstate_tx_shortpreamble;
                axradio_txbuffer_cnt = axradio_phy_preamble_len;
                goto shortpreamble;
            }
            if (cnt < 4) {
                ax5043WriteReg(AX5043_FIFOSTAT, 4); // commit
                usleep(1000);
                continue;
            }
            cnt = 7;
            if (axradio_txbuffer_cnt < 7)
                cnt = axradio_txbuffer_cnt;
            axradio_txbuffer_cnt -= cnt;
            cnt <<= 5;
            ax5043WriteReg(AX5043_FIFODATA, AX5043_FIFOCMD_REPEATDATA | (3 << 5));
            ax5043WriteReg(AX5043_FIFODATA, axradio_phy_preamble_flags);
            ax5043WriteReg(AX5043_FIFODATA, cnt);
            ax5043WriteReg(AX5043_FIFODATA, axradio_phy_preamble_byte);
            break;

        case trxstate_tx_shortpreamble:
        shortpreamble:
            if (!axradio_txbuffer_cnt) {
                if (cnt < 15) {
                    ax5043WriteReg(AX5043_FIFOSTAT, 4); // commit
                    usleep(1000);
                    continue;
                }
                if (axradio_phy_preamble_appendbits) {
                    uint8_t byte;
                    ax5043WriteReg(AX5043_FIFODATA, AX5043_FIFOCMD_DATA | (2 << 5));
                    ax5043WriteReg(AX5043_FIFODATA, 0x1C);
                    byte = axradio_phy_preamble_appendpattern;
                    if (ax5043ReadReg(AX5043_PKTADDRCFG) & 0x80) {
                        // msb first -> stop bit below
                        byte &= 0xFF << (8-axradio_phy_preamble_appendbits);
                        byte |= 0x80 >> axradio_phy_preamble_appendbits;
                    } else {
                         // lsb first -> stop bit above
                        byte &= 0xFF >> (8-axradio_phy_preamble_appendbits);
                        byte |= 0x01 << axradio_phy_preamble_appendbits;
                    }
                    ax5043WriteReg(AX5043_FIFODATA, byte);
                }
                if ((ax5043ReadReg(AX5043_FRAMING) & 0x0E) == 0x06 && axradio_framing_synclen) {
                    // write SYNC word if framing mode is raw_patternmatch, might use SYNCLEN > 0 as a criterion, but need to make sure SYNCLEN=0 for WMBUS (chip automatically sends SYNCWORD but matching in RX works via MATCH0PAT)
                    uint8_t len_byte = axradio_framing_synclen;
                    uint8_t i = (len_byte & 0x07) ? 0x04 : 0;
                    // SYNCLEN in bytes, rather than bits. Ceiled to next integer e.g. fractional bits are counted as full bits;
                    len_byte += 7;
                    len_byte >>= 3;
                    ax5043WriteReg(AX5043_FIFODATA, AX5043_FIFOCMD_DATA | ((len_byte + 1) << 5));
                    ax5043WriteReg(AX5043_FIFODATA, axradio_framing_syncflags | i);
                    for (i = 0; i < len_byte; ++i) {
                        // better put a brace, it might prevent SDCC from optimizing away the assignement...
                        ax5043WriteReg(AX5043_FIFODATA, axradio_framing_syncword[i]);
                    }
                }
                axradio_trxstate = trxstate_tx_packet;
                continue;
            }
            if (cnt < 4) {
                ax5043WriteReg(AX5043_FIFOSTAT, 4); // commit
                usleep(1000);
                continue;
            }
            cnt = 255;
            if (axradio_txbuffer_cnt < 255*8)
                cnt = axradio_txbuffer_cnt >> 3;
            if (cnt) {
                axradio_txbuffer_cnt -= ((uint16_t)cnt) << 3;
                ax5043WriteReg(AX5043_FIFODATA, AX5043_FIFOCMD_REPEATDATA | (3 << 5));
                ax5043WriteReg(AX5043_FIFODATA, axradio_phy_preamble_flags);
                ax5043WriteReg(AX5043_FIFODATA, cnt);
                ax5043WriteReg(AX5043_FIFODATA, axradio_phy_preamble_byte);
                continue;
            }
            {
                uint8_t byte = axradio_phy_preamble_byte;
                cnt = axradio_txbuffer_cnt;
                axradio_txbuffer_cnt = 0;
                ax5043WriteReg(AX5043_FIFODATA, AX5043_FIFOCMD_DATA | (2 << 5));
                ax5043WriteReg(AX5043_FIFODATA, 0x1C);
                if (ax5043ReadReg(AX5043_PKTADDRCFG) & 0x80) {
                    // msb first -> stop bit below
                    byte &= 0xFF << (8-cnt);
                    byte |= 0x80 >> cnt;
                } else {
                     // lsb first -> stop bit above
                    byte &= 0xFF >> (8-cnt);
                    byte |= 0x01 << cnt;
                }
                ax5043WriteReg(AX5043_FIFODATA, byte);
            }
            continue;

        case trxstate_tx_packet:
            if (cnt < 11) {
                ax5043WriteReg(AX5043_FIFOSTAT, 4); // commit
                usleep(1000);
                continue;
            }
            {
                uint8_t flags = 0;
                if (!axradio_txbuffer_cnt)
                    flags |= 0x01; // flag byte: pkt_start
                {
                    uint16_t len = axradio_txbuffer_len - axradio_txbuffer_cnt;
                    cnt -= 3;
                    if (cnt >= len) {
                        cnt = len;
                        flags |= 0x02; // flag byte: pkt_end
                    }
                }
                if (!cnt)
                    goto pktend;
                ax5043WriteReg(AX5043_FIFODATA, AX5043_FIFOCMD_DATA | (7 << 5));
                ax5043WriteReg(AX5043_FIFODATA, cnt + 1); // write FIFO chunk length byte (length includes the flag byte, thus the +1)
                ax5043WriteReg(AX5043_FIFODATA, flags);
                ax5043_writefifo(&axradio_txbuffer[axradio_txbuffer_cnt], cnt);
                axradio_txbuffer_cnt += cnt;
                if (flags & 0x02)
                    goto pktend;
            }
            break;

        default:
        	fprintf(stderr, "ERROR: Unexpected state found in transmit_isr\n");
        	exit(EXIT_FAILURE);
        }
    }

pktend:
    ax5043WriteReg(AX5043_RADIOEVENTMASK0, 0x01); // enable REVRDONE event
    ax5043WriteReg(AX5043_FIFOSTAT, 4); // commit
}

void ax5043_prepare_tx(void)
{
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_XTAL_ON);
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_FIFO_ON);
    ax5043_init_registers_tx();
    ax5043WriteReg(AX5043_FIFOTHRESH1, 0);
    ax5043WriteReg(AX5043_FIFOTHRESH0, 0x80);
	axradio_wait_for_xtal();
    ax5043ReadReg(AX5043_POWSTICKYSTAT); // clear pwr management sticky status --> brownout gate works
}
