#include "pwm.h"

#include <xc.h>

void init_pwm() {
    TRISCbits.TRISC2 = 1;        // Disable CCP1 PWM output
    T2CONbits.TMR2ON = 0;        // Disable Timer2
    CCP1CONbits.CCP1M = 0b1100;  // Set CCP1 to PWM mode

    // Equation 11-1 from the datasheet
    // T = 1 / (30 KHz) = 3.33e-5 seg
    // T = (PR2 + 1) * 4 * TOSC
    // T = (PR2 + 1) * 4 * 5e-8 (Assuming TOSC 20 MHz)
    // PR2 = 3.33e-5 / (4 * 5e-8) - 1
    // PR2 = 165.66 ~= 166
    // PR2 = 166;
    // T2CONbits.T2CKPS = 0b00;
    set_pwm_period(166, 0);  // Default frequency is 30 Khz

    // Reset PWM duty cycle on CCP1
    CCPR1L = 0;
    CCP1CONbits.DC1B0 = 0;
    CCP1CONbits.DC1B1 = 0;

    PIR1bits.TMR2IF = 0;  // Clear Timer2 interrupt flag
    PIE1bits.TMR2IE = 1;  // Enable Timer2 interrupts

    T2CONbits.TMR2ON = 1;  // Enable Timer2
}

void pwm_tmr2_interrupt_handler() {
    TRISCbits.TRISC2 = 0;  // Enable CCP1 PWM output
    PIE1bits.TMR2IE = 0;   // Disable Timer2 interrupts
    PIR1bits.TMR2IF = 0;   // Clear Timer2 interrupt flag
}

void set_pwm_period(uint8_t pr2, uint8_t tmr2ckps) {
    PR2 = pr2;
    T2CONbits.T2CKPS = tmr2ckps & 0b11;
}

void set_pwm_duty_cycle(uint16_t duty_cycle) {
    CCPR1L = (uint8_t)(duty_cycle >> 2);
    CCP1CONbits.DC1B1 = duty_cycle & (1 << 1);
    CCP1CONbits.DC1B0 = duty_cycle & (1 << 0);
}

uint16_t get_max_pwm_duty_cycle() {
    // Equation 13-3 from the datasheet
    // DCR = conf_value / (4 * (PR2 + 1))
    // 1 = conf_value / (4 * (PR2 + 1))
    // (4 * (PR2 + 1)) = conf_value
    return (uint16_t)((PR2 + 1) << 2);
}