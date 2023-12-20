#include "LM35.h"

uint16_t LM35_read_celsius(ADCPin adc_pin) {
    uint16_t analog_value = ADC_read(adc_pin);

    float voltage = analog_value / ADC_RESOLUTION * ADC_VREF;
    float temperature = voltage / (LM35_SCALE_FACTOR / 1000.0);

    return (uint16_t)temperature;
}
