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

#include "axradiomode.h"

#include "../generated/config.h"
#include "../spi/ax5043spi_p.h"
#include "axradioinit.h"
#include "axradioinit_p.h"

extern uint8_t axradio_phy_chanpllrng[];
extern const uint8_t axradio_phy_vcocalib;
extern uint8_t axradio_phy_chanvcoi[];
extern const uint8_t axradio_phy_chanvcoiinit[];
extern const uint8_t axradio_phy_chanpllrnginit[];
extern const int8_t axradio_phy_rssireference;

static uint8_t ax5043_init_registers_common(void);

uint8_t mode_tx() {
	int retVal;

    retVal = ax5043_off();
	if (retVal != AXRADIO_ERR_NOERROR) {
		return retVal;
	}

	retVal = ax5043_init_registers_tx();
	if (retVal != AXRADIO_ERR_NOERROR) {
		return retVal;
	}

	return AXRADIO_ERR_NOERROR;
}

uint8_t mode_rx() {
	int retVal;

    retVal = ax5043_off();
	if (retVal != AXRADIO_ERR_NOERROR) {
		return retVal;
	}

	retVal = ax5043_init_registers_rx();
	if (retVal != AXRADIO_ERR_NOERROR) {
		return retVal;
	}

	retVal = ax5043_receiver_on_continuous();
	if (retVal != AXRADIO_ERR_NOERROR) {
		return retVal;
	}

	return AXRADIO_ERR_NOERROR;
}


uint8_t ax5043_off(void)
{
    uint8_t retVal;

	retVal = ax5043_off_xtal();
	if (retVal != AXRADIO_ERR_NOERROR) {
		return retVal;
	}

	ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_POWERDOWN);

	return AXRADIO_ERR_NOERROR;
}

uint8_t ax5043_off_xtal(void)
{
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_XTAL_ON);
    ax5043WriteReg(AX5043_LPOSCCONFIG, 0x00); // LPOSC off
    return AXRADIO_ERR_NOERROR;
}

uint8_t ax5043_init_registers_tx(void)
{
    ax5043_set_registers_tx();
    return ax5043_init_registers_common();
}

static uint8_t ax5043_init_registers_common(void)
{
    uint8_t rng = axradio_phy_chanpllrng[0];
    if (rng & 0x20)
        return AXRADIO_ERR_RANGING;
    if (ax5043ReadReg(AX5043_PLLLOOP) & 0x80) {
        ax5043WriteReg(AX5043_PLLRANGINGB, rng & 0x0F);
    } else {
        ax5043WriteReg(AX5043_PLLRANGINGA, rng & 0x0F);
    }
    rng = axradio_get_pllvcoi();
    if (rng & 0x80)
        ax5043WriteReg(AX5043_PLLVCOI, rng);

    return AXRADIO_ERR_NOERROR;
}

uint8_t axradio_get_pllvcoi(void)
{
    if (axradio_phy_vcocalib) {
        uint8_t x = axradio_phy_chanvcoi[0];
        if (x & 0x80)
            return x;
    }
    {
        uint8_t x = axradio_phy_chanvcoiinit[0];
        if (x & 0x80) {
            if (!(axradio_phy_chanpllrnginit[0] & 0xF0)) {
                x += (axradio_phy_chanpllrng[0] & 0x0F) - (axradio_phy_chanpllrnginit[0] & 0x0F);
                x &= 0x3f;
                x |= 0x80;
            }
            return x;
        }
    }
    return ax5043ReadReg(AX5043_PLLVCOI);
}

uint8_t ax5043_init_registers_rx(void) {
    ax5043_set_registers_rx();
    return ax5043_init_registers_common();

}

uint8_t ax5043_receiver_on_continuous(void) {
    uint8_t regValue;

    ax5043WriteReg(AX5043_RSSIREFERENCE, axradio_phy_rssireference);
    ax5043_set_registers_rxcont();

    regValue = ax5043ReadReg(AX5043_PKTSTOREFLAGS);
    regValue &= (uint8_t)~0x40;
    ax5043WriteReg(AX5043_PKTSTOREFLAGS, regValue);


    ax5043WriteReg(AX5043_FIFOSTAT, 3); // clear FIFO data & flags
    ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_FULL_RX);

    return AXRADIO_ERR_NOERROR;
}
