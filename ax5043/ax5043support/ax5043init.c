// Copyright (c) 2018 Brandenburg Tech, LLC
// All rights reserved.
//
// This file is derived from LibMF, a library provided AXSEM AG. Based on the
// context of the library and the provision of source code, it is assumed
// the source code can be modified for specific applications of the AX5043.
//
// The Lib MF documentation provides the following notice:
//    LibMF is available in source and binary form for SDCC, Keil C51 and IAR
//    ICC.
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
// The original files did not contain a copyright notice for AXSEM AG, but the
// documentation contained the following notice.
//
// Copyright (c) 2011, 2012, 2013 AXSEM AG

#include "ax5043init.h"

#include <unistd.h>

#include "../axradio/axradioinit.h"
#include "../spi/ax5043spi.h"

uint8_t ax5043_reset(void)
{
	//printf("INFO: Resetting AX5043 (ax5043_reset)\n");
	uint8_t i;
	// Initialize Interface
	// Reset Device
	ax5043WriteReg(AX5043_PWRMODE, 0x80);
	ax5043WriteReg(AX5043_PWRMODE, AX5043_PWRSTATE_POWERDOWN);
	// Wait some time for regulator startup
	usleep(10000);

	// Check Scratch
	i = ax5043ReadReg(AX5043_SILICONREVISION);
	i = ax5043ReadReg(AX5043_SILICONREVISION);

	if (i != SILICONREV1)
		return RADIO_ERR_REVISION;

	ax5043WriteReg(AX5043_SCRATCH, 0x55);
	if (ax5043ReadReg(AX5043_SCRATCH) != 0x55)
		return RADIO_ERR_COMM;

	ax5043WriteReg(AX5043_SCRATCH, 0xAA);
	if (ax5043ReadReg(AX5043_SCRATCH) != 0xAA)
		return RADIO_ERR_COMM;

	return RADIO_OK;
}


