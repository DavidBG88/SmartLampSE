#include "pwm.h"

void init_pwm()
{
    TRISCbits.TRISC2 = 1;       // Disable CCP1 PWM output
    T2CONbits.TMR2ON = 0;       // Disable Timer2
    CCP1CONbits.CCP1M = 0b1100; // Set CCP1 to PWM mode

    // Equation 11-1 from the manual
    // T = 1 / (30 KHz) = 3.33e-5 seg
    // T = (PR2 + 1) * 4 * TOSC
    // T = (PR2 + 1) * 4 * 5e-8 (Assuming TOSC 20 MHz)
    // PR2 = 3.33e-5 / (4 * 5e-8) - 1
    // PR2 = 165.66 ~= 166
    PR2 = 166;
    T2CONbits.T2CKPS = 0b00;

    // Reset PWM duty cycle on CCP1
    CCPR1L = 0;
    CCP1CONbits.DC1B0 = 0;
    CCP1CONbits.DC1B1 = 0;

    PIR1bits.TMR2IF = 0; // Clear Timer2 interrupt flag
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    PIE1bits.TMR2IE = 1; // Enable Timer2 interrupts

    T2CONbits.TMR2ON = 1; // Enable Timer2
}

void pwm_tmr2_interrupt_handler()
{
    TRISCbits.TRISC2 = 0; // Enable CCP1 PWM output
    PIE1bits.TMR2IE = 0;  // Disable Timer2 interrupts
    PIR1bits.TMR2IF = 0;  // Clear Timer2 interrupt flag
}

void set_pwm_duty_cycle(uint16_t conf_value)
{
    CCPR1L = (uint8_t)(conf_value >> 2);
    CCP1CONbits.DC1B1 = conf_value & (1 << 1);
    CCP1CONbits.DC1B0 = conf_value & (1 << 0);
}

uint16_t get_max_pwm_duty_cycle_conf_value()
{
    // Equation 13-3 from the manual
    // DCR = conf_value / (4 * (PR2 + 1))
    // 1 = conf_value / (4 * (PR2 + 1))
    // (4 * (PR2 + 1)) = conf_value
    return (uint16_t) ((PR2 + 1) << 2);
}