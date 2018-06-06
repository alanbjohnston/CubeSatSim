#include "ax8052.h"
#include "libmfadc.h"

void adc_calibrate(void)
{
	adc_calibrate_gain();
	adc_calibrate_temp();
}
