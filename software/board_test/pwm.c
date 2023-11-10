#include "pwm.h"

void init_pwm()
{
    TRISCbits.TRISC2 = 1;       // Disable CCP1 PWM output
    CCP1CONbits.CCP1M = 0b1100; // Set CCP1 to PWM mode

    // Periodo = 1 / (30 KHz) = 3.33e-5 seg
    // Periodo = (PR2 + 1) * 4 * TOSC
    // Periodo = (PR2 + 1) * 4 * 5e-8 (Asumiendo TOSC 20 MHz)
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

void set_pwm_duty_cycle(uint16_t duty_cycle)
{
    CCP2CONbits.DC2B0 = duty_cycle & 0b01;
    CCP2CONbits.DC2B1 = duty_cycle & 0b10;
    CCPR2L = (uint8_t)(duty_cycle >> 2);
}

void set_pwm_duty_cycle_percentage(uint8_t percentage)
{
    if (percentage > 100)
        percentage = 100;
    else if (percentage < 0)
        percentage = 0;

    // R = duty_cycle / (4 * (PR2 + 1));
    // R * (4 * (PR2 + 1)) = duty_cycle;
    uint16_t duty_cycle = (uint16_t)((percentage * 4 * (PR2 + 1)) / 100);
    set_pwm_duty_cycle(duty_cycle);
}