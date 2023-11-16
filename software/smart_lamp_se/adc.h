#ifndef ADC_H
#define ADC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ADC init function.
 * @brief Initializes the ADC module.
 * @warning Must be called before any other ADC function.
 */
void init_adc();

/**
 * Read ADC pin 0.
 * @brief Reads the ADC pin 0 analog signal.
 * @return Digital value of the analog signal in ADC pin 0.
 */
uint16_t read_adc_0();

/**
 * Read ADC pin 1.
 * @brief Reads the ADC pin 1 analog signal.
 * @return Digital value of the analog signal in ADC pin 1.
 */
uint16_t read_adc_1();

/**
 * Read ADC pin 2.
 * @brief Reads the ADC pin 2 analog signal.
 * @return Digital value of the analog signal in ADC pin 2.
 */
uint16_t read_adc_2();

#ifdef __cplusplus
}
#endif

#endif  // ADC_H
