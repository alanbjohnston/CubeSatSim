#include "ax8052.h"
#include "libmfadc.h"

SFRX(ADCCALG10GAIN0,  0x7034)   /* ADC Calibration Range 10 Gain Low Byte */
SFRX(ADCCALG10GAIN1,  0x7035)   /* ADC Calibration Range 10 Gain High Byte */
SFR16LEX(ADCCALG10GAIN, 0x7034) /* ADC Calibration Range 10 Gain */

uint16_t adc_singleended_offset_x10(void)
{
#if defined(SDCC)
	uint16_t __autodata cal = ADCCALG10GAIN;
#else
	uint16_t __autodata cal = (((uint16_t)ADCCALG10GAIN1) << 8) | ADCCALG10GAIN0;
#endif
	uint16_t __autodata sum = 0x8000;
	cal <<= 1;
	sum += cal;
	cal >>= 5;
	sum += cal;
	cal >>= 4;
	sum += cal;
	cal >>= 4;
	sum -= cal;
	return sum;
}
