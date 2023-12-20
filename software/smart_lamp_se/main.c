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
#define SOUND_ADC_PIN ADC0
#define SOUND_BUFFER_SIZE 100

#define UART_CO2_TX_CODE 0
#define UART_HUMIDITY_TX_CODE 1
#define UART_TEMPERATURE_TX_CODE 2
#define UART_SOUND_TX_CODE 3

#define UART_LIGHT_RX_CODE 0
#define UART_FAN_RX_CODE 1

uint16_t max_sound = 0;

void record_sound_level(void) {
    uint16_t read_sound = ADC_read(SOUND_ADC_PIN);

    if (read_sound > max_sound)
        max_sound = read_sound;
}

void send_sound_level(void) {
    uint8_t message_bytes[] = {UART_SOUND_TX_CODE, ((uint8_t*)&max_sound)[1],
                               ((uint8_t*)&max_sound)[0]};

    UART_write_n_bytes(message_bytes, 3);

    max_sound = 0;
}

void record_and_send_co2_data(void) {
    //IAQCORE_Reading reading = IAQCORE_read();

    //uint8_t message_bytes[] = {UART_CO2_TX_CODE, ((uint8_t*)&reading.eco2_ppm)[1],
    //                           ((uint8_t*)&reading.eco2_ppm)[0]};
    uint8_t message_bytes[] = {UART_CO2_TX_CODE, 10, 10};

    UART_write_n_bytes(message_bytes, 3);
}

void record_and_send_temperature(void) {}

void record_and_send_light_lux(void) {}

void record_and_send_humidity(void) {}

void record_and_send_environment_data(void) {
    record_and_send_co2_data();
    record_and_send_temperature();
    record_and_send_light_lux();
    record_and_send_humidity();
}

void update_light(uint8_t p, uint8_t r, uint8_t g, uint8_t b) {
    APA102_set_color(p, r, g, b);

    char message[16];
    sprintf(message, "LIGHT: (%d, %d, %d)\r\n", r, g, b);
    UART_puts(message);
}

void update_fan_speed(uint8_t speed) {
    PWM_set_duty_cycle(PWM_get_max_duty_cycle() * speed / 255);

    char message[16];
    sprintf(message, "FAN SPEED: %d\r\n", speed);
    UART_puts(message);
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

    uint8_t command = UART_read_byte();

    switch (command) {
        case UART_LIGHT_RX_CODE: {  // Light
            uint8_t prgb[4];
            UART_read_n_bytes(prgb, sizeof(prgb));
            update_light(prgb[0], prgb[1], prgb[2], prgb[3]);
            break;
        }
        case UART_FAN_RX_CODE: {  // Fan speed
            uint8_t speed;
            UART_read_n_bytes(&speed, sizeof(speed));
            update_fan_speed(speed);
            break;
        }
        default:
            //report_invalid_command(command);
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
    PWM_init();
    PWM_set_period(255, 0);
    PWM_set_duty_cycle(0);

    // Initialize timing
    TIMING_init();
    TIMING_set_tick_duration(256 - 250, PR1to8);  // Tick = 1.6us * 250 = 400us

    // Set timing callbacks
    TIMING_run_every_n_ticks(25, record_sound_level);  // Every 10ms   -> 25 ticks
    TIMING_run_every_n_ticks(2500, send_sound_level);  // Every 1000ms -> 2500 ticks
    TIMING_run_every_n_ticks(12500,
                             record_and_send_environment_data);  // Every 5000ms -> 12500 ticks

    // Initialize UART
    UART_init();
    UART_set_baud_rate(32);  // 9600 bauds / sec
    PIE1bits.RCIE = 1;       // Enable RX interrupt

    while (1) {
        //SLEEP();
        //NOP();
    }

    return (EXIT_SUCCESS);
}

void __interrupt() interrupt_handler(void) {
    if (PIR1bits.RCIF) {
        // ECHO BACK UART FOR TESTING
        //uint8_t input_byte = UART_read_byte();
        //UART_write_byte(input_byte);

        match_incomming_uart_command();
        PIR1bits.RCIF = 0;
    } else if (PIR1bits.TMR2IF) {
        PWM_tmr2_interrupt_handler();
        PIR1bits.TMR2IF = 0;
    } else if (INTCONbits.T0IF) {
        TIMING_tmr0_interrupt_handler();
        INTCONbits.T0IF = 0;
    }
}
