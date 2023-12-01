#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000  // necessary for __delay_us

#include <stdio.h>
#include <xc.h>

#include "adc.h"
#include "command.h"
#include "pwm.h"
#include "timing.h"
#include "uart.h"

uint8_t debug_10ms = 0;
uint8_t debug_1000ms = 0;
uint8_t debug_5000ms = 0;
uint16_t adc = 0;

void every_10ms() {
    debug_10ms = !debug_10ms;
}

void every_1000ms() {
    debug_1000ms = !debug_1000ms;

    char int_buff[32];

    itoa(read_adc_0(), int_buff, 10);
    uart_puts("ADC 0: ");
    uart_puts(int_buff);
    uart_puts("\r\n");

    itoa(read_adc_1(), int_buff, 10);
    uart_puts("ADC 1: ");
    uart_puts(int_buff);
    uart_puts("\r\n");

    itoa(read_adc_2(), int_buff, 10);
    uart_puts("ADC 2: ");
    uart_puts(int_buff);
    uart_puts("\r\n");
}

void every_5000ms() {
    debug_5000ms = !debug_5000ms;
}

int main() {
    // Enable interrupts
    INTCONbits.GIE = 1;   // Enable interrupts
    INTCONbits.PEIE = 1;  // Enable peripheral interrupts
    INTCONbits.T0IE = 1;  // Enable Timer0 interrupts

    // Initialize ADC
    init_adc();
    adc = read_adc_0();

    // Initialize PWM
    init_pwm();
    set_pwm_period(255, 0);
    set_pwm_duty_cycle(0);

    // Initialize timing
    init_timing();
    set_tick_duration(256 - 250, PR1to8);  // Tick = 1.6us * 250 = 400us

    // Set timing callbacks
    run_every_n_ticks(25, every_10ms);       // Every 10ms   -> 25 ticks
    run_every_n_ticks(2500, every_1000ms);   // Every 1000ms -> 2500 ticks
    run_every_n_ticks(12500, every_5000ms);  // Every 5000ms -> 12500 ticks

    // Initialize UART
    uart_init();
    uart_set_baud_rate(32);  // 9600 bauds / sec
    PIE1bits.RCIE = 1;       // Enable RX interrupt

    while (1) {
        //SLEEP();
        //NOP();
    }

    return (EXIT_SUCCESS);
}

void __interrupt() interrupt_handler() {
    if (PIR1bits.RCIF) {
        uint8_t i = 0;
        uint8_t input_data[32];
        while (i < 32 && uart_read_available())
            input_data[i++] = uart_read_byte();

        uart_write_data(input_data, i);

        PIR1bits.RCIF = 0;
    } else if (PIR1bits.TMR2IF) {
        pwm_tmr2_interrupt_handler();
        PIR1bits.TMR2IF = 0;
    } else if (INTCONbits.T0IF) {
        timing_tmr0_interrupt_handler();
        INTCONbits.T0IF = 0;
    }
}