#ifndef HIH4000_H
#define HIH4000_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "adc.h"

#define V_MIN 0.8  // Voltage at 0% RH
#define V_MAX 3.9  // Voltage at 100% RH

uint16_t HIH4000_read_humidity(ADCPin adc_pin, uint16_t temp_celsius);

#ifdef __cplusplus
}
#endif

#endif  // HIH4000_H
