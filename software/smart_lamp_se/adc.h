#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum ADCPin { ADC0 = 8, ADC1 = 10, ADC2 = 12 } ADCPin;

/**
 * ADC init function.
 * @brief Initializes the ADC module.
 * @warning Must be called before any other ADC function.
 */
void init_adc();

/**
 * Read ADC pin 
 * @brief Reads the ADC pin analog signal.
 * @return Digital value of the analog signal in ADC pin.
 * @warning This function assumes that the acquisition time has already been waited for.
 */
uint16_t read_adc(ADCPin pin);

#ifdef __cplusplus
}
#endif

#endif  // ADC_H
