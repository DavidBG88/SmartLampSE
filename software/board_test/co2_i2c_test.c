#include "co2_i2c_test.h"

#include "i2c.h"
#include "iAQCore.h"
#include "uart.h"
#include "xc.h"
#include "stdio.h"

#define _XTAL_FREQ 20000000  // necessary for __delay_us

void co2_i2c_test() {
    UART_init();
    UART_set_baud_rate(32);  // 9600 bauds / sec
    
    I2C_init();
    IAQCORE_init();
    
    char buffer[32];
    IAQCORE_Reading reading;
    while (1) {
        reading = IAQCORE_read();
       
        sprintf(buffer, "CO2: %u\r\n", reading.eco2_ppm);
        UART_puts(buffer);
        
        __delay_ms(1000);
    }
}