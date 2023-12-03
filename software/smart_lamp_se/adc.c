#include "adc.h"

#include <xc.h>

void init_adc() {
    ADCON0bits.ADCS = 0b10;  // ADCS = 1.6 us at 20 MHz
    ADCON1bits.ADFM = 1;     // Justify right
    ADCON1bits.VCFG0 = 0;    // AVdd
    ADCON1bits.VCFG1 = 0;    // AVdd
    ADCON0bits.ADON = 1;     // Enable ADC
}

uint16_t read_adc(ADCPin adc_pin) {
    PIR1bits.ADIF = 0;         // Clear ADC end flag
    ADCON0bits.CHS = adc_pin;  // Select ADC channel
    ADCON0bits.GO = 1;         // Start ADC capture

    while (ADCON0bits.nDONE) {}  // Only waits 11 cycles

    return ((uint16_t)ADRESH << 8) | ADRESL;
}
