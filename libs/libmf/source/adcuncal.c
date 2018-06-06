#include "ax8052.h"
#include "libmfadc.h"

SFRX(ADCCALG00GAIN0,  0x7030)   /* ADC Calibration Range 00 Gain Low Byte */
SFRX(ADCCALG00GAIN1,  0x7031)   /* ADC Calibration Range 00 Gain High Byte */
SFRX(ADCCALG01GAIN0,  0x7032)   /* ADC Calibration Range 01 Gain Low Byte */
SFRX(ADCCALG01GAIN1,  0x7033)   /* ADC Calibration Range 01 Gain High Byte */
SFRX(ADCCALG10GAIN0,  0x7034)   /* ADC Calibration Range 10 Gain Low Byte */
SFRX(ADCCALG10GAIN1,  0x7035)   /* ADC Calibration Range 10 Gain High Byte */
SFRX(ADCCALTEMPGAIN0, 0x7038)   /* ADC Calibration Temperature Gain Low Byte */
SFRX(ADCCALTEMPGAIN1, 0x7039)   /* ADC Calibration Temperature Gain High Byte */
SFRX(ADCCALTEMPOFFS0, 0x703A)   /* ADC Calibration Temperature Offset Low Byte */
SFRX(ADCCALTEMPOFFS1, 0x703B)   /* ADC Calibration Temperature Offset High Byte */

void adc_uncalibrate(void)
{
	ADCCALG00GAIN0 = 0x00;
	ADCCALG00GAIN1 = 0x40;
	ADCCALG01GAIN0 = 0x00;
	ADCCALG01GAIN1 = 0x40;
	ADCCALG10GAIN0 = 0x00;
	ADCCALG10GAIN1 = 0x40;
	ADCCALTEMPGAIN0 = 0x00;
	ADCCALTEMPGAIN1 = 0x40;
	ADCCALTEMPOFFS0 = 0x00;
	ADCCALTEMPOFFS1 = 0x00;
}
