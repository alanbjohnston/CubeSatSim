#ifndef LIBMFADC_H
#define LIBMFADC_H

#include "libmftypes.h"

/*
 * AD Converter
 */
extern __reentrantb int16_t adc_measure_temperature(void) __reentrant;

extern void adc_calibrate_gain(void);
extern void adc_calibrate_temp(void);
extern void adc_calibrate(void);
extern void adc_uncalibrate(void);
extern uint16_t adc_singleended_offset_x01(void);
extern uint16_t adc_singleended_offset_x1(void);
extern uint16_t adc_singleended_offset_x10(void);

#endif /* LIBMFADC_H */
