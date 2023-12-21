#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000  // necessary for __delay_us

#include <stdio.h>
#include <stdlib.h>

#include <xc.h>

#include "APA102.h"
#include "HIH4000.h"
#include "LM35.h"
#include "VEML7700.h"
#include "adc.h"
#include "eeprom.h"
#include "iAQCore.h"
#include "pwm.h"
#include "timing.h"
#include "uart.h"

#define NUM_LEDS 10
#define SOUND_BUFFER_SIZE 100

#define SOUND_ADC_PIN ADC0
#define TEMPERATURE_ADC_PIN ADC1
#define HUMIDITY_ADC_PIN ADC2

#define UART_CO2_TX_CODE 0
#define UART_HUMIDITY_TX_CODE 1
#define UART_TEMPERATURE_TX_CODE 2
#define UART_SOUND_TX_CODE 3
#define UART_LIGHT_TX_CODE 4

#define UART_LIGHT_RX_CODE 0
#define UART_FAN_RX_CODE 1

#define CONFIG_MEM_CONTROL 0b01010101
#define CONFIG_MEM_CONTROL_ADDR 0x0
#define LIGHT_P_MEM_ADDR 0x1
#define LIGHT_R_MEM_ADDR 0x2
#define LIGHT_G_MEM_ADDR 0x3
#define LIGHT_B_MEM_ADDR 0x4
#define FAN_SPEED_MEM_ADDR 0X5

#define DEFAULT_FAN_SPEED 0
#define DEFAULT_P_LIGHT 16
#define DEFAULT_R_LIGHT 255
#define DEFAULT_G_LIGHT 255
#define DEFAULT_B_LIGHT 255

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

void record_and_send_temperature(void) {
    uint16_t temp = LM35_read_celsius(TEMPERATURE_ADC_PIN);

    uint8_t message_bytes[] = {UART_TEMPERATURE_TX_CODE, (uint8_t)(temp << 8), (uint8_t)temp};
    //uint8_t message_bytes[] = {UART_TEMPERATURE_TX_CODE, 10, 10};

    UART_write_n_bytes(message_bytes, 3);
}

void record_and_send_light_lux(void) {
    //uint16_t lux = VEML7700_read_light_lux();

    //uint8_t message_bytes[] = {UART_LIGHT_TX_CODE, (uint8_t)(lux << 8), (uint8_t)lux};
    uint8_t message_bytes[] = {UART_LIGHT_TX_CODE, 10, 10};

    UART_write_n_bytes(message_bytes, 3);
}

void record_and_send_humidity(void) {
    uint16_t humidity = HIH4000_read_humidity(HUMIDITY_ADC_PIN);

    uint8_t message_bytes[] = {UART_HUMIDITY_TX_CODE, (uint8_t)(humidity << 8), (uint8_t)humidity};
    //uint8_t message_bytes[] = {UART_HUMIDITY_TX_CODE, 10, 10};

    UART_write_n_bytes(message_bytes, 3);
}

void record_and_send_environment_data(void) {
    record_and_send_co2_data();
    record_and_send_temperature();
    record_and_send_light_lux();
    record_and_send_humidity();
}

void update_light(uint8_t p, uint8_t r, uint8_t g, uint8_t b) {
    APA102_set_color(p, r, g, b);
}

void update_fan_speed(uint8_t speed) {
    PWM_set_duty_cycle(PWM_get_max_duty_cycle() * speed / 255);
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

inline bool prev_conf_exists() {
    return EEPROM_read(CONFIG_MEM_CONTROL_ADDR) == CONFIG_MEM_CONTROL;
}

int main(void) {
    // Enable interrupts
    INTCONbits.GIE = 1;   // Enable interrupts
    INTCONbits.PEIE = 1;  // Enable peripheral interrupts
    INTCONbits.T0IE = 1;  // Enable Timer0 interrupts

    uint8_t fan_speed;
    uint8_t p, r, g, b;

    if (!prev_conf_exists()) {
        EEPROM_write(LIGHT_P_MEM_ADDR, DEFAULT_P_LIGHT);
        EEPROM_write(LIGHT_R_MEM_ADDR, DEFAULT_R_LIGHT);
        EEPROM_write(LIGHT_G_MEM_ADDR, DEFAULT_G_LIGHT);
        EEPROM_write(LIGHT_B_MEM_ADDR, DEFAULT_B_LIGHT);
        EEPROM_write(FAN_SPEED_MEM_ADDR, DEFAULT_FAN_SPEED);
    }

    p = EEPROM_read(LIGHT_P_MEM_ADDR);
    r = EEPROM_read(LIGHT_R_MEM_ADDR);
    g = EEPROM_read(LIGHT_G_MEM_ADDR);
    b = EEPROM_read(LIGHT_B_MEM_ADDR);
    fan_speed = EEPROM_read(FAN_SPEED_MEM_ADDR);

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
