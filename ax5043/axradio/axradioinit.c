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

#include "axradioinit.h"

#include <unistd.h>

#include "axradioinit_p.h"
#include "../ax5043support/ax5043init.h"
#include "../crc/crc.h"
#include "../generated/config.h"
#include "../spi/ax5043spi_p.h"

volatile uint8_t axradio_mode = AXRADIO_MODE_UNINIT;
volatile axradio_trxstate_t axradio_trxstate = trxstate_off;

struct axradio_address_mask axradio_localaddr;
uint8_t axradio_rxbuffer[PKTDATA_BUFLEN];

extern const uint32_t axradio_phy_chanfreq[];
extern const uint8_t axradio_phy_chanpllrnginit[];
extern uint8_t axradio_phy_chanpllrng[];
extern const uint8_t axradio_phy_vcocalib;
extern uint8_t axradio_phy_chanvcoi[];
extern const uint8_t axradio_phy_chanvcoiinit[];
extern const uint8_t axradio_framing_swcrclen;
extern const uint8_t axradio_phy_innerfreqloop;
extern const uint8_t axradio_phy_pn9;
extern const uint8_t axradio_framing_addrlen;
extern const uint8_t axradio_framing_destaddrpos;

static void axradio_setaddrregs(void)
{
	uint8_t regValue;

    ax5043WriteReg(AX5043_PKTADDR0, axradio_localaddr.addr[0]);
    ax5043WriteReg(AX5043_PKTADDR1, axradio_localaddr.addr[1]);
    ax5043WriteReg(AX5043_PKTADDR2, axradio_localaddr.addr[2]);
    ax5043WriteReg(AX5043_PKTADDR3, axradio_localaddr.addr[3]);

    ax5043WriteReg(AX5043_PKTADDRMASK0, axradio_localaddr.mask[0]);
    ax5043WriteReg(AX5043_PKTADDRMASK1, axradio_localaddr.mask[1]);
    ax5043WriteReg(AX5043_PKTADDRMASK2, axradio_localaddr.mask[2]);
    ax5043WriteReg(AX5043_PKTADDRMASK3, axradio_localaddr.mask[3]);

    if (axradio_phy_pn9 && axradio_framing_addrlen) {
        uint16_t pn = 0x1ff;
        uint8_t inv = -(ax5043ReadReg(AX5043_ENCODING) & 0x01);
        if (axradio_framing_destaddrpos != 0xff) {
            pn = pn9_advance_bits(pn, axradio_framing_destaddrpos << 3);
    	}
        regValue = ax5043ReadReg(AX5043_PKTADDR0);
        regValue ^= pn ^ inv;
        ax5043WriteReg(AX5043_PKTADDR0, regValue);
        pn = pn9_advance_byte(pn);

        regValue = ax5043ReadReg(AX5043_PKTADDR1);
        regValue ^= pn ^ inv;
        ax5043WriteReg(AX5043_PKTADDR1, regValue);
        pn = pn9_advance_byte(pn);

        regValue = ax5043ReadReg(AX5043_PKTADDR2);
        regValue ^= pn ^ inv;
        ax5043WriteReg(AX5043_PKTADDR2, regValue);
        pn = pn9_advance_byte(pn);

        regValue = ax5043ReadReg(AX5043_PKTADDR3);
        regValue ^= pn ^ inv;
        ax5043WriteReg(AX5043_PKTADDR3, regValue);
    }
}

static void ax5043_init_registers(void)
{
	uint8_t regValue;

    ax5043_set_registers();

    regValue = ax5043ReadReg(AX5043_PKTLENOFFSET);
    regValue += axradio_framing_swcrclen; // add len offs for software CRC16 (used for both, fixed and variable length packets
    ax5043WriteReg(AX5043_PKTLENOFFSET, regValue);

    ax5043WriteReg(AX5043_PINFUNCIRQ, 0x00); // No IRQ used for now
    ax5043WriteReg(AX5043_PKTSTOREFLAGS, axradio_phy_innerfreqloop ? 0x13 : 0x15); // store RF offset, RSSI and delimiter timing
    axradio_setaddrregs();
}

void axradio_wait_for_xtal(void) {
	//printf("INFO: Waiting for crystal (axradio_wait_for_xtal)\n");
	while ((ax5043ReadReg(AX5043_XTALSTATUS) & 0x01) == 0) {
		usleep(1000);
	}
	//printf("INFO: Crystal is ready\n");
}

static int16_t axradio_tunevoltage(void)
{
    int16_t r = 0;
    uint8_t cnt = 64;
    do {
    	ax5043WriteReg(AX5043_GPADCCTRL, 0x84);
        do {} while (ax5043ReadReg(AX5043_GPADCCTRL) & 0x80);
    } while (--cnt);
    cnt = 32;
    do {
    	ax5043WriteReg(AX5043_GPADCCTRL, 0x84);
        do {} while (ax5043ReadReg(AX5043_GPADCCTRL) & 0x80);
        {
            int16_t x = ax5043ReadReg(AX5043_GPADC13VALUE1) & 0x03;
            x <<= 8;
            x |= ax5043ReadReg(AX5043_GPADC13VALUE0);
            r += x;
        }
    } while (--cnt);
    return r;
}

static uint8_t axradio_adjustvcoi(uint8_t rng)
{
    uint8_t offs;
    uint8_t bestrng;
    uint16_t bestval = ~0;
    rng &= 0x7F;
    bestrng = rng;
    for (offs = 0; offs != 16; ++offs) {
        uint16_t val;
        if (!((uint8_t)(rng + offs) & 0xC0)) {
            ax5043WriteReg(AX5043_PLLVCOI, 0x80 | (rng + offs));
            val = axradio_tunevoltage();
            if (val < bestval) {
                bestval = val;
                bestrng = rng + offs;
            }
        }
        if (!offs)
            continue;
        if (!((uint8_t)(rng - offs) & 0xC0)) {
        	ax5043WriteReg(AX5043_PLLVCOI, 0x80 | (rng - offs));
            val = axradio_tunevoltage();
            if (val < bestval) {
                bestval = val;
                bestrng = rng - offs;
            }
        }
    }
    // if we hit the lower rail, do not change anything
    if (bestval <= 0x0010)
        return rng | 0x80;
    return bestrng | 0x80;
}

static uint8_t axradio_calvcoi(void)
{
    uint8_t i;
    uint8_t r = 0;
    uint16_t vmin = 0xffff;
    uint16_t vmax = 0x0000;
    for (i = 0x40; i != 0;) {
        uint16_t curtune;
        --i;
        ax5043WriteReg(AX5043_PLLVCOI, 0x80 | i);
        ax5043ReadReg(AX5043_PLLRANGINGA); // clear PLL lock loss
        curtune = axradio_tunevoltage();
        ax5043ReadReg(AX5043_PLLRANGINGA); // clear PLL lock loss
        ((uint16_t *)axradio_rxbuffer)[i] = curtune;
        if (curtune > vmax)
            vmax = curtune;
        if (curtune < vmin) {
            vmin = curtune;
            // check whether the PLL is locked
            if (!(0xC0 & (uint8_t)~ax5043ReadReg(AX5043_PLLRANGINGA)))
                r = i | 0x80;
        }
    }
    if (!(r & 0x80) || vmax >= 0xFF00 || vmin < 0x0100 || vmax - vmin < 0x6000)
        return 0;
    return r;
}

uint8_t axradio_init(void)
{
	uint8_t regValue;

    axradio_mode = AXRADIO_MODE_UNINIT;
    axradio_trxstate = trxstate_off;
    if (ax5043_reset())
        return AXRADIO_ERR_NOCHIP;
    ax5043_init_registers();
    ax5043_set_registers_tx();
    ax5043WriteReg(AX5043_PLLLOOP, 0x09); // default 100kHz loop BW for ranging
    ax5043WriteReg(AX5043_PLLCPI, 0x08);

    // range all channels
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_XTAL_ON);
    ax5043WriteReg(AX5043_MODULATION, 0x08);
    ax5043WriteReg(AX5043_FSKDEV2, 0x00);
    ax5043WriteReg(AX5043_FSKDEV1, 0x00);
    ax5043WriteReg(AX5043_FSKDEV0, 0x00);
    axradio_wait_for_xtal();

    {
		uint32_t f = axradio_phy_chanfreq[0];
		ax5043WriteReg(AX5043_FREQA0, f);
		ax5043WriteReg(AX5043_FREQA1, f >> 8);
		ax5043WriteReg(AX5043_FREQA2, f >> 16);
		ax5043WriteReg(AX5043_FREQA3, f >> 24);
	}
	axradio_trxstate = trxstate_pll_ranging;
	{
		uint8_t r;
		if( !(axradio_phy_chanpllrnginit[0] & 0xF0) ) { // start values for ranging available
			r = axradio_phy_chanpllrnginit[0] | 0x10;
		}
		else {
			r = 0x18;
		}
		ax5043WriteReg(AX5043_PLLRANGINGA, r); // init ranging process starting from "range"
	}
	//printf("INFO: Waiting for PLL ranging process\n");
	while ((ax5043ReadReg(AX5043_PLLRANGINGA) & 0x10) != 0) {
		usleep(1000);
	}
	//printf("INFO: PLL ranging process complete\n");
	axradio_trxstate = trxstate_off;
	axradio_phy_chanpllrng[0] = ax5043ReadReg(AX5043_PLLRANGINGA);

    // VCOI Calibration
    if (axradio_phy_vcocalib) {
        ax5043_set_registers_tx();
        ax5043WriteReg(AX5043_MODULATION, 0x08);
        ax5043WriteReg(AX5043_FSKDEV2, 0x00);
        ax5043WriteReg(AX5043_FSKDEV1, 0x00);
        ax5043WriteReg(AX5043_FSKDEV0, 0x00);

        regValue = ax5043ReadReg(AX5043_PLLLOOP);
        regValue |= 0x04;
        ax5043WriteReg(AX5043_PLLLOOP, regValue);
        {
            uint8_t x = ax5043ReadReg(AX5043_0xF35);
            x |= 0x80;
            if (2 & (uint8_t)~x)
                ++x;
            ax5043WriteReg(AX5043_0xF35, x);
        }
        ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_SYNTH_TX);
        {
            uint8_t vcoisave = ax5043ReadReg(AX5043_PLLVCOI);
            uint8_t j = 2;
			axradio_phy_chanvcoi[0] = 0;
			ax5043WriteReg(AX5043_PLLRANGINGA, axradio_phy_chanpllrng[0] & 0x0F);
			{
				uint32_t f = axradio_phy_chanfreq[0];
				ax5043WriteReg(AX5043_FREQA0, f);
				ax5043WriteReg(AX5043_FREQA1, f >> 8);
				ax5043WriteReg(AX5043_FREQA2, f >> 16);
				ax5043WriteReg(AX5043_FREQA3, f >> 24);
			}
			do {
				if (axradio_phy_chanvcoiinit[0]) {
					uint8_t x = axradio_phy_chanvcoiinit[0];
					if (!(axradio_phy_chanpllrnginit[0] & 0xF0))
						x += (axradio_phy_chanpllrng[0] & 0x0F) - (axradio_phy_chanpllrnginit[0] & 0x0F);
					axradio_phy_chanvcoi[0] = axradio_adjustvcoi(x);
				} else {
					axradio_phy_chanvcoi[0] = axradio_calvcoi();
				}
			} while (--j);
			j = 1;
			ax5043WriteReg(AX5043_PLLVCOI, vcoisave);
        }
    }
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_POWERDOWN);
    ax5043_init_registers();
    ax5043_set_registers_rx();
    ax5043WriteReg(AX5043_PLLRANGINGA, axradio_phy_chanpllrng[0] & 0x0F);
    {
        uint32_t f = axradio_phy_chanfreq[0];
        ax5043WriteReg(AX5043_FREQA0, f);
        ax5043WriteReg(AX5043_FREQA1, f >> 8);
        ax5043WriteReg(AX5043_FREQA2, f >> 16);
        ax5043WriteReg(AX5043_FREQA3, f >> 24);
    }

    axradio_mode = AXRADIO_MODE_OFF;
	if (axradio_phy_chanpllrng[0] & 0x20)
		return AXRADIO_ERR_RANGING;
    return AXRADIO_ERR_NOERROR;
}

uint8_t axradio_setfreq(int32_t f) {
	uint8_t regValue;

	// range all channels
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_XTAL_ON);
    axradio_wait_for_xtal();

    {
    	int32_t f1 = axradio_conv_freq_fromhz(f);

    	/* Set LSB, per AX5043 documentation, to prevent synthesizer spurs */
    	f1 |= 1;

    	ax5043WriteReg(AX5043_FREQA0, f1);
		ax5043WriteReg(AX5043_FREQA1, f1 >> 8);
		ax5043WriteReg(AX5043_FREQA2, f1 >> 16);
		ax5043WriteReg(AX5043_FREQA3, f1 >> 24);
	}
	axradio_trxstate = trxstate_pll_ranging;
	{
		uint8_t r;
		if( !(axradio_phy_chanpllrnginit[0] & 0xF0) ) { // start values for ranging available
			r = axradio_phy_chanpllrnginit[0] | 0x10;
		}
		else {
			r = 0x18;
		}
		ax5043WriteReg(AX5043_PLLRANGINGA, r); // init ranging process starting from "range"
	}
	//printf("INFO: Waiting for PLL ranging process\n");
	while ((ax5043ReadReg(AX5043_PLLRANGINGA) & 0x10) != 0) {
		usleep(1000);
	}
	//printf("INFO: PLL ranging process complete\n");
	axradio_trxstate = trxstate_off;
	axradio_phy_chanpllrng[0] = ax5043ReadReg(AX5043_PLLRANGINGA);

    // VCOI Calibration
    if (axradio_phy_vcocalib) {
        ax5043_set_registers_tx();
        ax5043WriteReg(AX5043_MODULATION, 0x08);
        ax5043WriteReg(AX5043_FSKDEV2, 0x00);
        ax5043WriteReg(AX5043_FSKDEV1, 0x00);
        ax5043WriteReg(AX5043_FSKDEV0, 0x00);

        regValue = ax5043ReadReg(AX5043_PLLLOOP);
        regValue |= 0x04;
        ax5043WriteReg(AX5043_PLLLOOP, regValue);
        {
            uint8_t x = ax5043ReadReg(AX5043_0xF35);
            x |= 0x80;
            if (2 & (uint8_t)~x)
                ++x;
            ax5043WriteReg(AX5043_0xF35, x);
        }
        ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_SYNTH_TX);
        {
            uint8_t vcoisave = ax5043ReadReg(AX5043_PLLVCOI);
            uint8_t j = 2;
			axradio_phy_chanvcoi[0] = 0;
			ax5043WriteReg(AX5043_PLLRANGINGA, axradio_phy_chanpllrng[0] & 0x0F);
			{
		    	int32_t f1 = axradio_conv_freq_fromhz(f);

		    	/* Set LSB, per AX5043 documentation, to prevent synthesizer spurs */
		    	f1 |= 1;

				ax5043WriteReg(AX5043_FREQA0, f1);
				ax5043WriteReg(AX5043_FREQA1, f1 >> 8);
				ax5043WriteReg(AX5043_FREQA2, f1 >> 16);
				ax5043WriteReg(AX5043_FREQA3, f1 >> 24);
			}
			do {
				if (axradio_phy_chanvcoiinit[0]) {
					uint8_t x = axradio_phy_chanvcoiinit[0];
					if (!(axradio_phy_chanpllrnginit[0] & 0xF0))
						x += (axradio_phy_chanpllrng[0] & 0x0F) - (axradio_phy_chanpllrnginit[0] & 0x0F);
					axradio_phy_chanvcoi[0] = axradio_adjustvcoi(x);
				} else {
					axradio_phy_chanvcoi[0] = axradio_calvcoi();
				}
			} while (--j);
			j = 1;
			ax5043WriteReg(AX5043_PLLVCOI, vcoisave);
        }
    }
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_POWERDOWN);
    ax5043_init_registers();
    ax5043_set_registers_rx();
    ax5043WriteReg(AX5043_PLLRANGINGA, axradio_phy_chanpllrng[0] & 0x0F);
    {
    	int32_t f1 = axradio_conv_freq_fromhz(f);

    	/* Set LSB, per AX5043 documentation, to prevent synthesizer spurs */
    	f1 |= 1;

        ax5043WriteReg(AX5043_FREQA0, f1);
        ax5043WriteReg(AX5043_FREQA1, f1 >> 8);
        ax5043WriteReg(AX5043_FREQA2, f1 >> 16);
        ax5043WriteReg(AX5043_FREQA3, f1 >> 24);
    }

    axradio_mode = AXRADIO_MODE_OFF;
	if (axradio_phy_chanpllrng[0] & 0x20)
		return AXRADIO_ERR_RANGING;
    return AXRADIO_ERR_NOERROR;
}
