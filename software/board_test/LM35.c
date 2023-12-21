#include "LM35.h"

uint16_t LM35_read_celsius(ADCPin adc_pin) {
    uint16_t analog_value = ADC_read(adc_pin);

    return 100 * ADC_VREF * analog_value / ADC_RESOLUTION;
}
