#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000  // necessary for __delay_us

#include <stdio.h>
#include <xc.h>

#include "pwm.h"
#include "timing.h"

uint8_t debug_10ms = 0;
uint8_t debug_1000ms = 0;
uint8_t debug_5000ms = 0;

void every_10ms() {
    debug_10ms = !debug_10ms;
}

void every_1000ms() {
    debug_1000ms = !debug_1000ms;
}

void every_5000ms() {
    debug_5000ms = !debug_5000ms;
}

int main() {
    INTCONbits.GIE = 1;   // Enable interrupts
    INTCONbits.PEIE = 1;  // Enable peripheral interrupts
    INTCONbits.T0IE = 1;  // Enable Timer0 interrupts

    init_pwm();
    set_pwm_period(255, 0);
    set_pwm_duty_cycle(get_max_pwm_duty_cycle() / 2);

    init_timing();
    set_tick_duration(256 - 250, PR1to8);    // Tick = 1.6us * 250 = 400us
    run_every_n_ticks(25, every_10ms);       // Every 10ms   -> 25 ticks
    run_every_n_ticks(2500, every_1000ms);   // Every 1000ms -> 2500 ticks
    run_every_n_ticks(12500, every_5000ms);  // Every 5000ms -> 12500 ticks

    while (1)
        ;

    return (EXIT_SUCCESS);
}

void __interrupt() interrupt_handler() {
    if (PIR1bits.TMR2IF) {
        pwm_tmr2_interrupt_handler();
    } else if (INTCONbits.T0IF) {
        timing_tmr0_interrupt_handler();
    }
}
