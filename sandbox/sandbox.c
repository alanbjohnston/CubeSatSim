// Copyright (c) 2018 Brandenburg Tech, LLC
// All right reserved.
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

#include <stdint.h>
#include <stdio.h>
#include "generated/config.h"

uint32_t calc1(int32_t freq);
uint32_t calc2(int32_t freq);

int main(void)
{
	//int32_t axradio_conv_freq_fromhz(int32_t f)

	int32_t f = 435300000;

	uint32_t f1 = calc1(f);
	printf("calc1 f1 = 0x%08x\n", f1);

	uint32_t f2 = calc2(f);
	printf("calc1 f2 = 0x%08x\n", f2);

	return 0;
}

uint32_t calc1(int32_t freq)
{
	uint64_t w;

	/* Multiply, first, to preserve accuracy */
	w = (uint64_t)freq;
	w <<= 24;

	/* Divide by crystal frequency, assumed to be 16 MHz */
	w /= 16000000;

	/* Set LSB, per AX5043 documentation, to prevent synthesizer spurs */
	w |= 1;

	/* Upper bits should be zero, otherwise error */
	if (w >> 32) {
		/* Do not change freq. */
		return (uint32_t)-1;
	}

	return (uint32_t)w;
}

uint32_t calc2(int32_t f)
{
	f = axradio_conv_freq_fromhz(f);

	/* Set LSB, per AX5043 documentation, to prevent synthesizer spurs */
	f |= 1;

	return (uint32_t)f;
}
