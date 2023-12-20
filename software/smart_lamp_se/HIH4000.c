#include "HIH4000.h"

uint16_t HIH4000_read_humidity(ADCPin adc_pin) {
    float voltage = ADC_VREF * ADC_read(adc_pin) / ADC_RESOLUTION;
    float rh = ((voltage - V_MIN) / (V_MAX - V_MIN)) * 100.0;

    if (rh < 0)
        rh = 0;
    if (rh > 100)
        rh = 100;

    return (uint16_t)rh;
}