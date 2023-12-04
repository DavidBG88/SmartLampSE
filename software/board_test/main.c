#pragma config CPD = OFF, BOREN = OFF, IESO = OFF, DEBUG = OFF, FOSC = HS
#pragma config FCMEN = OFF, MCLRE = ON, WDTE = OFF, CP = OFF, LVP = OFF
#pragma config PWRTE = ON, BOR4V = BOR21V, WRT = OFF

#pragma intrinsic(_delay)
#define _XTAL_FREQ 20000000  // necessary for __delay_us

#include <stdio.h>
#include <xc.h>

#include "co2_i2c_test.h"
#include "eeprom_test.h"
#include "light_spi_test.h"
#include "pwm_test.h"
#include "timing_test.h"
#include "uart_rx_test.h"
#include "uart_tx_test.h"

// If a new test is added, update the run_test function switch case expression
typedef enum Test {
    UART_TX_TEST,
    UART_RX_TEST,
    PWM_TEST,
    LIGHT_SPI_TEST,
    CO2_I2C_TEST,
    EEPROM_TEST,
    TIMING_TEST,
} Test;

void run_test(Test test);

void main() {
    // Select test to run here
    run_test(UART_TX_TEST);
}

void run_test(Test test) {
    switch (test) {
        case UART_TX_TEST:
            uart_tx_test();
            break;
        case UART_RX_TEST:
            uart_rx_test();
            break;
        case PWM_TEST:
            pwm_test();
            break;
        case LIGHT_SPI_TEST:
            light_spi_test();
            break;
        case CO2_I2C_TEST:
            co2_i2c_test();
            break;
        case EEPROM_TEST:
            eeprom_test();
            break;
        case TIMING_TEST:
            timing_test();
            break;
    }
}
