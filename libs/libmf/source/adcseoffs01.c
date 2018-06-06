#include "ax8052.h"
#include "libmfadc.h"

SFRX(ADCCALG01GAIN0,  0x7032)   /* ADC Calibration Range 01 Gain Low Byte */
SFRX(ADCCALG01GAIN1,  0x7033)   /* ADC Calibration Range 01 Gain High Byte */
SFR16LEX(ADCCALG01GAIN, 0x7032) /* ADC Calibration Range 01 Gain */

uint16_t adc_singleended_offset_x1(void)
{
#if defined(SDCC)
	uint16_t __autodata cal = ADCCALG01GAIN;
#else
	uint16_t __autodata cal = (((uint16_t)ADCCALG01GAIN1) << 8) | ADCCALG01GAIN0;
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
