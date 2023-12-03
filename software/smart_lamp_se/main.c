#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000  // necessary for __delay_us

#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

#include "APA102.h"
#include "adc.h"
#include "eeprom.h"
#include "iAQCore.h"
#include "pwm.h"
#include "timing.h"
#include "uart.h"

#define NUM_LEDS 10

uint8_t debug_10ms = 0;
uint8_t debug_1000ms = 0;
uint8_t debug_5000ms = 0;
uint16_t adc = 0;

void every_10ms(void) {
    debug_10ms = !debug_10ms;
}

void every_1000ms(void) {
    debug_1000ms = !debug_1000ms;

    char message[16];

    sprintf(message, "ADC 0: %d\r\n", ADC_read(ADC0));
    uart_puts(message);

    sprintf(message, "ADC 1: %d\r\n", ADC_read(ADC1));
    uart_puts(message);

    sprintf(message, "ADC 2: %d\r\n", ADC_read(ADC2));
    uart_puts(message);
}

void every_5000ms(void) {
    debug_5000ms = !debug_5000ms;
}

void update_light(uint8_t p, uint8_t r, uint8_t g, uint8_t b) {
    APA102_set_color(p, r, g, b);

    /*
    char message[16];
    sprintf(message, "LIGHT: (%d, %d, %d)\r\n", r, g, b);
    uart_puts(message);
    */
}

void update_fan_speed(uint8_t speed) {
    set_pwm_duty_cycle(get_max_pwm_duty_cycle() * speed / 255);

    /*
    char message[16];
    sprintf(message, "FAN SPEED: %d\r\n", speed);
    uart_puts(message);
    */
}

void report_invalid_command(uint8_t command) {
    // NO INVALID COMMAND REPORT PROTOCOL SPECIFIED
}

void match_incomming_uart_command(void) {
    /*
    | Dato       | Codigo | Tipo                                 |
    | ---------- | ------ | ------------------------------------ |
    | Luz        | 0      | (uint8_t, uint8_t, uint8_t, uint8_t) |
    | Ventilador | 1      | uint8_t                              |
    */

    uint8_t command = uart_read_byte();

    switch (command) {
        case 0: {  // Light
            uint8_t prgb[4];
            uart_read_n_bytes(sizeof(prgb), prgb);
            update_light(prgb[0], prgb[1], prgb[2], prgb[3]);
            break;
        }
        case 1: {  // Fan speed
            uint8_t speed;
            uart_read_n_bytes(sizeof(speed), &speed);
            update_fan_speed(speed);
            break;
        }
        default:
            report_invalid_command(command);
            break;
    }
}

int main(void) {
    // Enable interrupts
    INTCONbits.GIE = 1;   // Enable interrupts
    INTCONbits.PEIE = 1;  // Enable peripheral interrupts
    INTCONbits.T0IE = 1;  // Enable Timer0 interrupts

    // Initialize ADC
    ADC_init();

    // Initialize iAQCore sensor
    IAQCORE_init();

    // Initialize APA102 light strip
    APA102_init(NUM_LEDS);

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

void __interrupt() interrupt_handler(void) {
    if (PIR1bits.RCIF) {
        /* ECHO BACK UART FOR TESTING
        uint8_t input_byte = uart_read_byte();

        char output[8];
        sprintf(output, "%d", input_byte);
        uart_puts(output);
        */

        match_incomming_uart_command();
        PIR1bits.RCIF = 0;
    } else if (PIR1bits.TMR2IF) {
        pwm_tmr2_interrupt_handler();
        PIR1bits.TMR2IF = 0;
    } else if (INTCONbits.T0IF) {
        timing_tmr0_interrupt_handler();
        INTCONbits.T0IF = 0;
    }
}
