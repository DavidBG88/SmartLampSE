#include "HIH4000.h"

uint16_t HIH4000_read_humidity(ADCPin adc_pin, uint16_t temp_celsius) {
    float voltage = ADC_VREF * (float)ADC_read(adc_pin) / ADC_RESOLUTION;
    float rh = (voltage - 0.852) / 0.030;
    float true_rh = rh / (1.0546 - (0.00216 * temp_celsius));

    if (true_rh < 0)
        true_rh = 0;
    if (true_rh > 100)
        true_rh = 100;
    
    
    //return ADC_read(adc_pin);
    return (uint16_t)(100 * true_rh);
}