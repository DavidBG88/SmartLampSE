#ifndef HIH4000_H
#define HIH4000_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "adc.h"

#define ADC_VREF 5
#define ADC_RESOLUTION 1024
#define LM35_SCALE_FACTOR 10.0

#define V_MIN 0.8  // Voltage at 0% RH
#define V_MAX 3.9  // Voltage at 100% RH

uint16_t HIH4000_read_humidity(ADCPin adc_pin);

#ifdef __cplusplus
}
#endif

#endif  // HIH4000_H
