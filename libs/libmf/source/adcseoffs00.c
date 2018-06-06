#include "ax8052.h"
#include "libmfadc.h"

SFRX(ADCCALG00GAIN0,  0x7030)   /* ADC Calibration Range 00 Gain Low Byte */
SFRX(ADCCALG00GAIN1,  0x7031)   /* ADC Calibration Range 00 Gain High Byte */
SFR16LEX(ADCCALG00GAIN, 0x7030) /* ADC Calibration Range 00 Gain */

uint16_t adc_singleended_offset_x01(void)
{
#if defined(SDCC)
	uint16_t __autodata cal = ADCCALG00GAIN;
#else
	uint16_t __autodata cal = (((uint16_t)ADCCALG00GAIN1) << 8) | ADCCALG00GAIN0;
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
