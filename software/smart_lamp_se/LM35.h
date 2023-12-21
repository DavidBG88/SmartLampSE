#ifndef LM35_H
#define LM35_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "adc.h"
    
#define LM35_SCALE_FACTOR 10.0

uint16_t LM35_read_celsius(ADCPin adc_pin);

#ifdef __cplusplus
}
#endif

#endif  // LM35_H
