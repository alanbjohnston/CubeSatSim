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

#include "ax5043tx.h"

#include "../axradio/axradioinit.h"
#include "../spi/ax5043spi.h"

void ax5043_writefifo(const uint8_t *ptr, uint8_t len)
{
	if (!len)
		return;
	do {
		ax5043WriteReg(AX5043_FIFODATA, *ptr++);
	} while (--len);
}

